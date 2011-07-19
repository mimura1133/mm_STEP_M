/* VorbisComments.cpp
 **
 ** This program is distributed under the GNU General Public License, version 2.
 ** A copy of this license is included with this source.
 **
 ** Copyright 2001, Warren Spits <spits@cyberdude.com>
 **
 ** A class that holds value tag pairs.
 **
 **/

#include "StdAfx.h"

#include "VorbisComments.h"

//#include <windows.h>
#include <vorbis/codec.h>
//#include <string.h> // memcpy, memmove, strcpy
#include "utf8.h"

// Initialise the vorbis comments (Constructor)
VorbisComments::VorbisComments() :
	allocated(8),
	count(0)
{
	tags = new tag[allocated];
}

// Destroy the vorbis comments (Destructor)
VorbisComments::~VorbisComments()
{
	for (unsigned int i = 0; i < count; i++)
	{
		delete[] tags[i].name;
		delete[] tags[i].value;
	}
	delete[] tags;
}

// Get a tags name
LPCSTR VorbisComments::getName(unsigned int item) const
{
	LPCSTR result = NULL;

	if (item < count)
		result = tags[item].name;

	return result;
}

// Get a tags value
LPCWSTR VorbisComments::getValue(unsigned int item) const
{
	LPCWSTR result = NULL;

	if (item < count)
		result = tags[item].value;

	return result;
}

// Returns the number of tags
unsigned int VorbisComments::getCount() const
{
	return count;
}

// Adds a tag
void VorbisComments::addTag(LPCSTR name, LPCWSTR value)
{
	int len;

	if (count == allocated)
	{
		// Increase the allocated size
		tag *newtags;

		newtags = new tag[allocated * 2];
		memcpy(newtags, tags, sizeof(tag) * allocated);
		allocated *= 2;
		delete[] tags;
		tags = newtags;
	}

	// Add the name
	len = lstrlenA(name);
	tags[count].name = new CHAR[len +1];
	lstrcpyA(tags[count].name, name);
	CharUpperA(tags[count].name);

	// Add the value
	len = lstrlenW(value);
	tags[count].value = new WCHAR[len +1];
	memcpy(tags[count].value, value, sizeof(WCHAR)*(len +1));

	count++;
}

// Removes a tag
void VorbisComments::removeTag(unsigned int item)
{
	if (item < count)
	{
		delete[] tags[item].name;
		delete[] tags[item].value;
		memmove(tags + item, tags + item + 1, sizeof(tag) * (count - item - 1));
		count--;
	}
}

// Remove all the tags
void VorbisComments::removeAll()
{
	for (unsigned int i = 0; i < count; i++)
	{
		delete[] tags[i].name;
		delete[] tags[i].value;
	}
	count = 0;
}

// Retrieve the comments from a vorbis_comment structure
void VorbisComments::retrieve(const vorbis_comment *vc)
{
	LPSTR name;
	LPWSTR value;
	size_t len;
	char *comment;
	char *equalpos;
	char *valuepos;

	removeAll();

	int i; for (i = 0; i < vc->comments; i++)
	{
		// Copy the comment
		comment = new char[vc->comment_lengths[i] +1];
		strcpy(comment, vc->user_comments[i]);

		// Find the '='
		equalpos = strchr(comment, '=');

		if (equalpos == NULL)
		{
			// The comments do not follow the comment spec!
			len = lstrlenA("COMMENT");
			name = new CHAR[len +1];
			lstrcpyA(name, "COMMENT");
			valuepos = comment;
		}
		else
		{
			*equalpos = 0;
			len = strlen(comment);
			name = new CHAR[len +1];
			lstrcpyA(name, comment);
			valuepos = equalpos +1;
		}
		
		// Convert the UTF-8 string to WCHAR string
		len = UTF8ToWideChar(NULL, valuepos);
		if (len == UTF8_ILSEQ) // Check for invalid UTF-8
		{
			// Interpret it through current code page
			len = MultiByteToWideChar(CP_ACP, 0, valuepos, -1, NULL, 0);
			value = new WCHAR[len];
			MultiByteToWideChar(CP_ACP, 0, valuepos, -1, value, len);
		}
		else
		{
			// Do the UTF-8 conversion
			value = new WCHAR[len];
			UTF8ToWideChar(value, valuepos);
		}

		// Add the tag
		addTag(name, value); 
		delete[] value;
		delete[] name;
		delete[] comment;
	}
}

// Store the comments in a vorbis_comment structure
void VorbisComments::store(vorbis_comment *vc) const
{
	int len;
	char *value;

	vorbis_comment_clear(vc);
	vorbis_comment_init(vc);

	for (unsigned int i = 0; i < count; i++)
	{
		// Convert the Value to a UTF-8 string.
		len = WideCharToUTF8(NULL, tags[i].value);
		value = new char[len];
		WideCharToUTF8(value, tags[i].value);
		vorbis_comment_add_tag(vc, tags[i].name, value);
        //CString str = tags[i].value;
		//char szString[1024];
        //lstrcpyn(szString, (LPCSTR)str, sizeof(szString));
        //vorbis_comment_add_tag(vc, tags[i].name, szString);

		delete[] value;
	}
}

// Swap two comments around
void VorbisComments::swapPos(unsigned int pos1, unsigned int pos2)
{
	tag temp;

	if ((pos1 != pos2) && (pos1 < count) && (pos2 < count))
	{
		temp = tags[pos1];
		tags[pos1] = tags[pos2];
		tags[pos2] = temp;
	}
}

// Modify a comment
void VorbisComments::modifyTag(unsigned int item, LPCSTR name, LPCWSTR value)
{
	int len;

	if (item < count)
	{
		// Set the name
		delete[] tags[item].name;
		len = lstrlenA(name);
		tags[item].name = new CHAR[len +1];
		lstrcpyA(tags[item].name, name);
		CharUpperA(tags[item].name);

		// Set the value
		delete[] tags[item].value;
		len = lstrlenW(value);
		tags[item].value = new WCHAR[len +1];
		memcpy(tags[item].value, value, sizeof(WCHAR)*(len +1));
	}
}

// Checks a name for validity
bool VorbisComments::isValidName(LPCSTR name)
{
	if (lstrlenA(name) == 0)
		return false;

	while (*name != '\0')
	{
		if ((*name < 0x20) || (*name > 0x7D) || (*name == '='))
			return false;
		name++;
	}

	return true;	
}

// Checks a value for validity
bool VorbisComments::isValidValue(LPCWSTR value)
{
	if (lstrlenW(value) == 0)
		return false;

	return true;
}

// The copy constructor
VorbisComments::VorbisComments(const VorbisComments &copy) :
	allocated(8),
	count(0)
{
	tags = new tag[allocated];
	for (unsigned int i = 0; i < copy.count; i++)
		addTag(copy.tags[i].name, copy.tags[i].value);
}

// The assignment operator (VorbisComments)
VorbisComments &VorbisComments::operator=(const VorbisComments &copy)
{
	if (this == &copy) return *this;

	removeAll();
	for (unsigned int i = 0; i < copy.count; i++)
		addTag(copy.tags[i].name, copy.tags[i].value);
	return *this;
}


