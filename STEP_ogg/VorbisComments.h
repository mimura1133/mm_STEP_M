/* VorbisComments.h
 **
 ** This program is distributed under the GNU General Public License, version 2.
 ** A copy of this license is included with this source.
 **
 ** Copyright 2001, Warren Spits <spits@cyberdude.com>
 **
 ** A class that holds value tag pairs.
 **
 **/

#ifndef VORBISCOMMENTS_H_INCLUDED
#define VORBISCOMMENTS_H_INCLUDED

//#include "<windows.h>
#include <vorbis/codec.h>

class VorbisComments  
{
public:
	VorbisComments();
	VorbisComments(const VorbisComments &copy);
	virtual ~VorbisComments();

	VorbisComments &operator=(const VorbisComments &copy);

	static bool isValidName(LPCSTR name);
	static bool isValidValue(LPCWSTR value);

	void modifyTag(unsigned int item, LPCSTR name, LPCWSTR value);
	void swapPos(unsigned int pos1, unsigned int pos2);
	void store(vorbis_comment *vc) const;
	void retrieve(const vorbis_comment *vc);
	void removeAll();

	unsigned int getCount() const;
	LPCSTR getName(unsigned int item) const;
	LPCWSTR getValue(unsigned int item) const;

	void addTag(LPCSTR name, LPCWSTR value);
	void removeTag(unsigned int item);

private:
	struct tag
	{
		LPSTR name;
		LPWSTR value;
	};

	unsigned int allocated;
	unsigned int count;
	tag *tags;
};

#endif // VORBISCOMMENTS_H_INCLUDED
