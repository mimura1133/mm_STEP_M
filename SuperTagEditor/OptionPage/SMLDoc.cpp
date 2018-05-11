// HTStatic.cpp : implementation file
//

#include "stdafx.h"
#include "SMLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_FONT_SIZE	0


enum {
	TAG_UNKNOWN,

	// Header tag ID's ***MUST*** be in sequential order
	HEADER1,
	HEADER2,
	HEADER3,
	HEADER4,
	HEADER5,
	BEGIN_BODY,
	END_BODY,
	END_HEADER,
	BEGIN_FONT,
	END_FONT,
	BEGIN_CENTER,
	END_CENTER,
	BEGIN_LEFTJUST,
	END_LEFTJUST,
	BEGIN_RIGHTJUST,
	END_RIGHTJUST,
	BEGIN_BOLD,
	END_BOLD,
	BEGIN_UNDERLINE,
	END_UNDERLINE,
	BEGIN_ITALICS,
	END_ITALICS,
	NEWLINE,
};


struct tagInfo {
	LPTSTR	tag;
	DWORD	idStart;
	DWORD	idEnd;
};

static tagInfo STagInfo[] = {
	{TEXT("body"), BEGIN_BODY, END_BODY},
	{TEXT("center"), BEGIN_CENTER, END_CENTER},
	{TEXT("left"), BEGIN_LEFTJUST, END_LEFTJUST},
	{TEXT("right"), BEGIN_RIGHTJUST, END_RIGHTJUST},
	{TEXT("font"), BEGIN_FONT, END_FONT},
	{TEXT("b"), BEGIN_BOLD, END_BOLD},
	{TEXT("u"), BEGIN_UNDERLINE, END_UNDERLINE},
	{TEXT("i"), BEGIN_ITALICS, END_ITALICS},
	{TEXT("br"), NEWLINE, NEWLINE},
	{TEXT("h1"), HEADER1, TAG_UNKNOWN},
	{TEXT("h2"), HEADER2, TAG_UNKNOWN},
	{TEXT("h3"), HEADER3, TAG_UNKNOWN},
	{TEXT("h4"), HEADER4, TAG_UNKNOWN},
	{TEXT("h5"), HEADER5, TAG_UNKNOWN},
	{TEXT("h"), TAG_UNKNOWN, END_HEADER},
	{NULL, 0, 0}
};

static DWORD headerFontSizes[5] = {8, 10, 12, 14, 18};



///////////////////////////////////////////////////////////////////////////////
//
//
//	Helper functions specific to parsing MiniHTML
//
//
///////////////////////////////////////////////////////////////////////////////

/* Skip whitespaces and return a pointer to the first non-whitespace character */
static LPCTSTR SkipWS(LPCTSTR ptr)
{
	while(0 != *ptr && *ptr <= ' ') {
		ptr++;
	}

	return ptr;
}


/* Parses a literal string enclosed in quotes from the markup stream */
static LPCTSTR GetString(LPCTSTR str, CString &output)
{
	ASSERT('"' == *str);
	LPCTSTR end;
	LPCTSTR start;
	
	str++;
	start = str;
	end = str;

	/* Find the end of the string literal */
	while(0 != *end && '"' != *end) {
		end++;
	}

	/*
		Make sure the last character found was an ending quote so that we don't
		go past the end of the input stream.
	*/
	str = end;
	if('"' == *str) {
		str++;
	}


	/* Calculate the length and allocate a conversion buffer */
	DWORD length = end - start;
	auto newstr = output.GetBuffer(length + 1);
	LPCTSTR src = start;
	auto dst = newstr;

	// Copy over the new string and convert carriage returns to spaces
	while(src < end) {
		if(0x0d == *src || 0x0a == *src) {
			src++;
			if(src < end && 0x0a == *src) {
				src++;
			}

			// Make sure that we only include one space for all carraige returns
			if(dst != newstr && dst[-1] != ' ') {
				*(dst++) = ' ';
			}
			continue;
		}
		*(dst++) = *(src++);
	}
	*dst = 0;

	output.ReleaseBuffer(length);
	
	/* Return a string object containing the literal */
	return str;
}


/*
	Translate an input stream into an ID tag.
	
	Parameters:
		src			Pointer to the input stream.
		tagString	Where the parsed tag or string will be stored.
		isString	Boolean indicating if the parsed data is a string literal
					or a tag identifier.
	Returns:
		Pointer to the first character after the tag or string literal.
*/
static LPCTSTR GetID(LPCTSTR src, CString &tagString, BOOL &isString)
{
	DWORD length;
	LPCTSTR retVal;
	LPCTSTR end;

	/* Skip the the beginning of the text to parse */
	src = SkipWS(src);

	/* If the first character is a quote, parse it as a string */
	if('"' == *src) {
		isString = TRUE;
		retVal = GetString(src, tagString);
	} else {
		isString = FALSE;

		end = src;

		// Find the end of the tag string
		while(0 != *end && '>' != *end && 0 != __iscsym(*end)) {
			end++;
		}
		retVal = end;

		// Store the tag ID into the string buffer
		length = end - src;
		auto tmp = tagString.GetBuffer(length + 1);
		memcpy(tmp, src, length);
		tmp[length] = 0;
		tagString.ReleaseBuffer(length);
	}

	return retVal;
}










///////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc::CSMLTagOption
//
//
///////////////////////////////////////////////////////////////////////////////
CSMLDoc::CSMLTagOption::CSMLTagOption(CString &param, CString &value)
{
	m_Param = param;
	m_Value = value;
}










///////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc::CSMLTagOptionList
//
//
///////////////////////////////////////////////////////////////////////////////
CSMLDoc::CSMLTagOptionList::~CSMLTagOptionList()
{
	Reset();
}


void CSMLDoc::CSMLTagOptionList::Reset()
{
	// Delete all options in the list
	while(FALSE == IsEmpty()) {
		delete RemoveHead();
	}
}


CString *CSMLDoc::CSMLTagOptionList::GetParam(LPCTSTR param) {
	POSITION pos;

	// Get the head of the list
	pos = GetHeadPosition();

	// Search until we reach the end
	while(NULL != pos) {
		CSMLTagOption *opt = GetNext(pos);

		// If this is the parameter we are looking for, return the param value
		if(0 == opt->m_Param.CompareNoCase(param)) {
			return &opt->m_Value;
		}
	}

	return NULL;
}


CString CSMLDoc::CSMLTagOptionList::GetParamTypeface(CString &defaultFace) {
	CString *value;

	// Get the 'face' param
	value = GetParam(_T("face"));

	if(NULL != value) {
		LPCTSTR faceStr = *value;
		UINT id = 0;

		// Translate the typeface
		if(0 == _tcsicmp(faceStr, TEXT("ANSI_FIXED_FONT"))) {
			id = ANSI_FIXED_FONT;
		} else if(0 == _tcsicmp(faceStr, TEXT("ANSI_VAR_FONT"))) {
			id = ANSI_VAR_FONT;
		} else if(0 == _tcsicmp(faceStr, TEXT("DEFAULT_GUI_FONT"))) {
			id = DEFAULT_GUI_FONT;
		} else if(0 == _tcsicmp(faceStr, TEXT("OEM_FIXED_FONT"))) {
			id = OEM_FIXED_FONT;
		} else if(0 == _tcsicmp(faceStr, TEXT("SYSTEM_FONT"))) {
			id = SYSTEM_FONT;
		} else if(0 == _tcsicmp(faceStr, TEXT("SYSTEM_FIXED_FONT"))) {
			id = SYSTEM_FIXED_FONT;
		}

		// If a windows specific font was found, get the typeface
		if(0 != id) {
			LOGFONT logFont;
			::GetObject(::GetStockObject(id), sizeof(LOGFONT), &logFont);

			return logFont.lfFaceName;

		}
		
		return *value;
	}

	return defaultFace;
}


DWORD CSMLDoc::CSMLTagOptionList::GetParamSize(DWORD defaultSize)
{
	CString *value;

	// Find the 'size' parameter
	value = GetParam(_T("size"));
	if(NULL != value) {
		// It was found, so translate it to a number
		defaultSize = _tstoi(*value);
	}

	return defaultSize;
}


static BOOL GetHexNibble(char ch, UCHAR *out)
{
	BOOL retval;

	retval = TRUE;
	ch = tolower(ch);

	if(ch >= '0' && ch <= '9') {
		*out = ch - '0';
	} else if(ch >= 'a' && ch <= 'f') {
		*out = 10 + (ch - 'a');
	} else {
		retval = FALSE;
	}

	return retval;
}

// Translate a color value or windows system color into an RGB value 
static COLORREF ConvertColor(COLORREF defaultColor, LPCTSTR srcTxt)
{
	if(0 == _tcsncicmp(srcTxt, _T("COLOR_"), 6)) {
		srcTxt = &(srcTxt[6]);
		if(0 == _tcsicmp(srcTxt, TEXT("3DDKSHADOW"))) {
			defaultColor = COLOR_3DDKSHADOW;
		} else if(0 == _tcsicmp(srcTxt, TEXT("3DFACE"))) {
			defaultColor = COLOR_3DFACE;
		} else if(0 == _tcsicmp(srcTxt, TEXT("BTNFACE"))) {
			defaultColor = COLOR_BTNFACE;
		} else if(0 == _tcsicmp(srcTxt, TEXT("3DHILIGHT"))) {
			defaultColor = COLOR_3DHILIGHT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("3DLIGHT"))) {
			defaultColor = COLOR_3DLIGHT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("3DSHADOW"))) {
			defaultColor = COLOR_3DSHADOW;
		} else if(0 == _tcsicmp(srcTxt, TEXT("ACTIVEBORDER"))) {
			defaultColor = COLOR_ACTIVEBORDER;
		} else if(0 == _tcsicmp(srcTxt, TEXT("ACTIVECAPTION"))) {
			defaultColor = COLOR_ACTIVECAPTION;
		} else if(0 == _tcsicmp(srcTxt, TEXT("APPWORKSPACE"))) {
			defaultColor = COLOR_APPWORKSPACE;
		} else if(0 == _tcsicmp(srcTxt, TEXT("BACKGROUND"))) {
			defaultColor = COLOR_BACKGROUND;
		} else if(0 == _tcsicmp(srcTxt, TEXT("BTNTEXT"))) {
			defaultColor = COLOR_BTNTEXT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("CAPTIONTEXT"))) {
			defaultColor = COLOR_CAPTIONTEXT;
#ifdef COLOR_GRADIENTACTIVECAPTION
		} else if(0 == _tcsicmp(srcTxt, TEXT("GRADIENTACTIVECAPTION"))) {
			defaultColor = COLOR_GRADIENTACTIVECAPTION;
#endif
#ifdef GRADIENTINACTIVECAPTION
		} else if(0 == _tcsicmp(srcTxt, TEXT("GRADIENTINACTIVECAPTION"))) {
			defaultColor = COLOR_GRADIENTINACTIVECAPTION;
#endif
		} else if(0 == _tcsicmp(srcTxt, TEXT("GRAYTEXT"))) {
			defaultColor = COLOR_GRAYTEXT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("HIGHLIGHT"))) {
			defaultColor = COLOR_HIGHLIGHT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("HIGHLIGHTTEXT"))) {
			defaultColor = COLOR_HIGHLIGHTTEXT;
#ifdef HOTLIGHT
		} else if(0 == _tcsicmp(srcTxt, TEXT("HOTLIGHT"))) {
			defaultColor = COLOR_HOTLIGHT;
#endif
		} else if(0 == _tcsicmp(srcTxt, TEXT("INACTIVEBORDER"))) {
			defaultColor = COLOR_INACTIVEBORDER;
		} else if(0 == _tcsicmp(srcTxt, TEXT("INACTIVECAPTION"))) {
			defaultColor = COLOR_INACTIVECAPTION;
		} else if(0 == _tcsicmp(srcTxt, TEXT("INACTIVECAPTIONTEXT"))) {
			defaultColor = COLOR_INACTIVECAPTIONTEXT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("INFOBK"))) {
			defaultColor = COLOR_INFOBK;
		} else if(0 == _tcsicmp(srcTxt, TEXT("INFOTEXT"))) {
			defaultColor = COLOR_INFOTEXT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("MENU"))) {
			defaultColor = COLOR_MENU;
		} else if(0 == _tcsicmp(srcTxt, TEXT("MENUTEXT"))) {
			defaultColor = COLOR_MENUTEXT;
		} else if(0 == _tcsicmp(srcTxt, TEXT("SCROLLBAR"))) {
			defaultColor = COLOR_SCROLLBAR;
		} else if(0 == _tcsicmp(srcTxt, TEXT("WINDOW"))) {
			defaultColor = COLOR_WINDOW;
		} else if(0 == _tcsicmp(srcTxt, TEXT("WINDOWFRAME"))) {
			defaultColor = COLOR_WINDOWFRAME;
		} else if(0 == _tcsicmp(srcTxt, TEXT("WINDOWTEXT"))) {
			defaultColor = COLOR_WINDOWTEXT;
		} else {
			TRACE1("Unknown system color %s\n", srcTxt);
			return defaultColor;
		}

		defaultColor = GetSysColor(defaultColor);
	} else {
		DWORD newVal;
		int i;
		BOOL err;

		newVal = 0;
		err = FALSE;

		srcTxt = SkipWS(srcTxt);

		// Check for HTML compatible color declaration
		if('#' == *srcTxt) {
			srcTxt++;
		}
		
		// Process 6 hex values
		for(i = 0; i < 6; i++) {
			UCHAR nibble;

			if(FALSE == GetHexNibble(*srcTxt, &nibble)) {
				err = TRUE;
				break;
			}

			newVal <<= 4;
			newVal |= nibble;
			srcTxt++;
		}

		// If no error was found in the conversion, translate it to RGB
		if(FALSE == err) {
			defaultColor = RGB(GetBValue(newVal), GetGValue(newVal), GetRValue(newVal));
		}
	}

	return defaultColor;
}


COLORREF CSMLDoc::CSMLTagOptionList::GetParamColor(COLORREF defaultColor)
{
	CString *value;

	// Get the color argument
	value = GetParam(_T("color"));

	if(NULL != value) {
		// Go translate the color
		defaultColor = ConvertColor(defaultColor, *value);
	}

	return defaultColor;
}


DWORD CSMLDoc::CSMLTagOptionList::GetBkgndColor(COLORREF defaultColor)
{
	CString *value;

	// Get the background color argument
	value = GetParam(_T("bgcolor"));

	if(NULL != value) {
		// Trnaslate the color
		defaultColor = ConvertColor(defaultColor, *value);
	}

	return defaultColor;
}









////////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc::CSMLFontInfo
//
//
////////////////////////////////////////////////////////////////////////////////
CSMLDoc::CSMLFontInfo::CSMLFontInfo()
{
	m_Size = 12;
	m_Style = 0;
	m_Color = RGB(0,0,0);
	m_Face = _T("");
}


CSMLDoc::CSMLFontInfo::CSMLFontInfo(CSMLDoc::CSMLFontInfo *info)
{
	m_Size = info->m_Size;
	m_Style = info->m_Style;
	m_Color = info->m_Color;
	m_Face = info->m_Face;
}


CSMLDoc::CSMLFontInfo::CSMLFontInfo(DWORD size, DWORD style, COLORREF color, CString &face)
{
	m_Size = size;
	m_Style = style;
	m_Color = color;
	m_Face = face;
}


CSMLDoc::CSMLFontInfo::CSMLFontInfo(DWORD size, DWORD style, COLORREF color, LPCSTR face)
{
	m_Size = size;
	m_Style = style;
	m_Color = color;
	if(NULL != face) {
		m_Face = face;
	}
}










///////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc::CSMLTextGroup::CSMLTextBlock
//
//
///////////////////////////////////////////////////////////////////////////////
CSMLDoc::CSMLTextGroup::CSMLTextBlock::CSMLTextBlock(LPCSTR text, CSMLDoc::CSMLFontInfo *info)
{
	ASSERT(NULL != text);
	ASSERT(NULL != info);

	m_Text = text;
	m_Style = info->m_Style;
	m_PointSize = info->m_Size;
	m_Typeface = info->m_Face;
	m_Color = info->m_Color;
}


CString &CSMLDoc::CSMLTextGroup::CSMLTextBlock::GetTypeface()
{
	return m_Typeface;
}


DWORD CSMLDoc::CSMLTextGroup::CSMLTextBlock::GetPointSize()
{
	return m_PointSize;
}


DWORD CSMLDoc::CSMLTextGroup::CSMLTextBlock::GetStyle()
{
	return m_Style;
}


COLORREF CSMLDoc::CSMLTextGroup::CSMLTextBlock::GetColor()
{
	return m_Color;
}


CString &CSMLDoc::CSMLTextGroup::CSMLTextBlock::GetText()
{
	return m_Text;
}










///////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc::CSMLTextGroup
//
//
///////////////////////////////////////////////////////////////////////////////
CSMLDoc::CSMLTextGroup::CSMLTextGroup()
{
}

CSMLDoc::CSMLTextGroup::~CSMLTextGroup()
{
	// Empty the contents of the block array and delete it's elements
	Empty(TRUE);
}

void CSMLDoc::CSMLTextGroup::operator=(CSMLDoc::CSMLTextGroup &line)
{
	// Copy the contents from a text group
	int size = line.m_Blocks.GetSize();
	m_Blocks.SetSize(size);
	for(int i = 0; i < size; i++) {
		m_Blocks[i] = line.m_Blocks[i];
	}
}


int CSMLDoc::CSMLTextGroup::GetBlockCount()
{
	return m_Blocks.GetSize();
}


void CSMLDoc::CSMLTextGroup::Empty(BOOL bDelete)
{
	if(TRUE == bDelete) {
		// Delete all objects in the array
		for(int i = 0; i < m_Blocks.GetSize(); i++) {
			delete m_Blocks[i];
		}
	}

	// Clear the array
	m_Blocks.RemoveAll();
}


CSMLDoc::CSMLTextGroup::CSMLTextBlock *CSMLDoc::CSMLTextGroup::GetBlock(int index)
{
	ASSERT(index >= 0 && index < m_Blocks.GetSize());

	return m_Blocks[index];
}


void CSMLDoc::CSMLTextGroup::Add(CSMLDoc::CSMLTextGroup::CSMLTextBlock *block)
{
	ASSERT(NULL != block);
	m_Blocks.Add(block);
}










///////////////////////////////////////////////////////////////////////////////
//
//
//	CSMLDoc
//
//
///////////////////////////////////////////////////////////////////////////////
CSMLDoc::~CSMLDoc()
{
	PurgeBlocks();
}


CString &CSMLDoc::GetTypeface()
{
	return m_PageTypeface;
}


COLORREF CSMLDoc::GetBkgndColor()
{
	return m_BkgndColor;
}


int CSMLDoc::GetGroupCount()
{
	return m_Blocks.GetSize();
}


void CSMLDoc::PurgeBlocks()
{
	// Delete all objects in the text block
	for(int i = 0; i < m_Blocks.GetSize(); i++) {
		delete m_Blocks[i];
	}
}


CSMLDoc::CSMLTextGroup *CSMLDoc::GetTextGroup(int index)
{
	ASSERT(index >= 0 && index < m_Blocks.GetSize());

	return m_Blocks[index];
}


void CSMLDoc::AddGroup(CSMLDoc::CSMLTextGroup &line)
{
	CSMLTextGroup *newLine = new CSMLTextGroup();
	*newLine = line;
	m_Blocks.Add(newLine);
}


//////////////////////////////////////////////////////////////////////////////
//	Parsing
//////////////////////////////////////////////////////////////////////////////
LPCTSTR CSMLDoc::ParseTag(LPCTSTR src, CString &tagText, DWORD &tagID, CSMLDoc::CSMLTagOptionList &paramList)
{
	ASSERT(NULL != src);
	ASSERT('<' == *src);

	BOOL isString;
	LPCTSTR retVal;
	bool endingTag;

	// Determine if this is a termination tag
	src = SkipWS(src + 1);
	if('/' == *src) {
		src++;
		endingTag = true;
	} else {
		endingTag = false;
	}


	retVal = src;
	tagID = TAG_UNKNOWN;

	/////////////////////////////////////////////////////////////////
	// Skip to the end of the tag statement
	/////////////////////////////////////////////////////////////////
	while(0 != *retVal && '>' != *retVal) {
		if('"' == *retVal) {
			retVal++;
			while(0 != *retVal && '"' != *retVal) {
				retVal++;
			}

			if('"' == *retVal) {
				retVal++;
			}

		} else {
			retVal++;
		}
	}

	if('>' == *retVal) {
		retVal++;
	}


	/////////////////////////////////////////////////////////////////
	// Parse all tag parameters
	/////////////////////////////////////////////////////////////////
	// Get the tag ID
	src = GetID(src, tagText, isString);
	tagInfo *info = STagInfo;

	while(NULL != info->tag) {
		if(0 == tagText.CompareNoCase(info->tag)) {
			tagID = (false == endingTag ? info->idStart : info->idEnd);
			break;
		}
		info++;
	}

	// If a tag ID was found, collect parameters for it
	if(TAG_UNKNOWN != tagID) {
		// Parse the tag parameters
		while(*src != '>') {
			CString param;
			CString value;

			// Skip any whitespaces and unneeded control characters
			src = SkipWS(src);
			while(0 != *src && '>' != *src && !isalpha(*src)) {
				src++;
			}

			// Get the parameter
			src = GetID(src, param, isString);
			src = SkipWS(src);
			// IF there is a parameter value, grap it
			if('=' == *src) {
				src = SkipWS(src + 1);
				src = GetID(src, value, isString);
				src = SkipWS(src);
			}

			paramList.AddTail(new CSMLTagOption(param, value));
		}
	}

	return retVal;
}


BOOL CSMLDoc::ParseSMLText(UINT nID)
{
	BOOL result = FALSE;
	HINSTANCE	hInst;
	HRSRC		hSrc;
	HGLOBAL		hRes;
	CString		text;

	// Get the instance handle of the application
	hInst = AfxGetInstanceHandle();

	// Find the resource
	hSrc = FindResource(hInst, MAKEINTRESOURCE(nID), TEXT("SMLDOC"));
	if(NULL == hSrc) {
		return FALSE;
	}

	// Load the resource
	hRes = LoadResource(hInst, hSrc);

	if(NULL == hRes) {
		return FALSE;
	}

	// Get a pointer to the resource memory and parse it
	text = (CHAR*)GlobalLock(hRes);
	ParseSMLText(text.GetBuffer(0));
	GlobalUnlock(hRes);

	return TRUE;
}


BOOL CSMLDoc::ParseSMLText(LPCTSTR src)
{
	ASSERT(NULL != src);

	CSMLFontInfo		*fontInfo;
	DWORD			length;
	DWORD			tagID;
	LPCTSTR			start;
	LPCTSTR			end;
	CString			tagString;
	CSMLTextGroup		textGroup;
	CSMLFontInfoList	fontInfoStack;
	CSMLTagOptionList	optionList;
	LOGFONT			logFont;
	bool			inBody;


	// Remove old blocks
	PurgeBlocks();

	// Get the default system font
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &logFont);


	// Reset the default color, style and font information
	inBody = false;
	m_BkgndColor = GetSysColor(COLOR_WINDOW);
	m_PageTypeface = logFont.lfFaceName;
	fontInfo = new CSMLFontInfo(headerFontSizes[DEFAULT_FONT_SIZE],
							   0,
							   GetSysColor(COLOR_WINDOWTEXT),
							   NULL);

	// Parse until we reach a null terminator
	while('\0' != *src) {

		switch(*src) {
		case '<':	// Parse a tag

			// Reset the options list
			optionList.Reset();
			src = ParseTag(src, tagString, tagID, optionList);

			// If we are not inside of the text body, do not process the tag
			if(false == inBody) {
				if(BEGIN_BODY != tagID) {
					continue;
				}
			}

			// Process the tag
			switch(tagID) {
			case TAG_UNKNOWN:
				TRACE1("Unknown tag: \"%s\"\n", tagString.GetBuffer(0));
				break;

			case HEADER1:
			case HEADER2:
			case HEADER3:
			case HEADER4:
			case HEADER5:
				// Save the current font information
				fontInfoStack.AddHead(fontInfo);
				// Create new font information based on the header tag
				fontInfo = new CSMLFontInfo(fontInfo);
				fontInfo->m_Color = optionList.GetParamColor(fontInfo->m_Color);
				fontInfo->m_Size = headerFontSizes[tagID - HEADER1];
				fontInfo->m_Face = optionList.GetParamTypeface(fontInfo->m_Face);
				break;

			case END_FONT:
			case END_HEADER:
				// Make sure we have font information saved
				if(0 != fontInfoStack.GetCount()) {
					// Delete the current font information and get the previous one
					// off of the stack
					delete fontInfo;
					fontInfo = fontInfoStack.RemoveHead();
				}
				break;

			case BEGIN_FONT:
				// Save the current font information
				fontInfoStack.AddHead(fontInfo);
				// Create new font information based on the header tag
				fontInfo = new CSMLFontInfo(fontInfo);
				fontInfo->m_Color = optionList.GetParamColor(fontInfo->m_Color);
				fontInfo->m_Size = optionList.GetParamSize(fontInfo->m_Size);
				fontInfo->m_Face = optionList.GetParamTypeface(fontInfo->m_Face);
				break;

			case END_BODY:
				// End of the text body
				inBody = false;
				break;

			case BEGIN_BODY:
				// Start the text body
				inBody = true;
				// Get any paramters for the text body
				m_BkgndColor = optionList.GetBkgndColor(m_BkgndColor);
				m_PageTypeface = optionList.GetParamTypeface(CString(logFont.lfFaceName));
				break;


			case BEGIN_CENTER:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				fontInfo->m_Style |= JUSTIFY_CENTER;
				break;

			case END_CENTER:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				break;

			case BEGIN_LEFTJUST:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				fontInfo->m_Style |= JUSTIFY_LEFT;
				break;

			case END_LEFTJUST:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				break;

			case BEGIN_RIGHTJUST:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				fontInfo->m_Style |= JUSTIFY_RIGHT;
				break;

			case END_RIGHTJUST:
				// Adjust the font styling
				fontInfo->m_Style &= ~JUSTIFY_MASK;
				break;

			case BEGIN_BOLD:
				// Adjust the font styling
				fontInfo->m_Style |= STYLE_BOLD;
				break;

			case END_BOLD:
				// Adjust the font styling
				fontInfo->m_Style &= ~STYLE_BOLD;
				break;

			case BEGIN_UNDERLINE:
				// Adjust the font styling
				fontInfo->m_Style |= STYLE_UNDERLINE;
				break;

			case END_UNDERLINE:
				// Adjust the font styling
				fontInfo->m_Style &= ~STYLE_UNDERLINE;
				break;

			case BEGIN_ITALICS:
				// Adjust the font styling
				fontInfo->m_Style |= STYLE_ITALICS;
				break;

			case END_ITALICS:
				// Adjust the font styling
				fontInfo->m_Style &= ~STYLE_ITALICS;
				break;

			case NEWLINE:
				// Go to the next line
				AddGroup(textGroup);
				textGroup.Empty(FALSE);
				break;

			default:
				ASSERT(0);
				break;
			}
			continue;

		default:
			// No tag specified, parse it as a text.
			start = src;
			end = src;

			// Parse until end of stream or we reach a tag
			while(0 != *end && '<' != *end) {
				end++;
			}
			length = (end - start);

			// Make sure we are in the text body before adding anything
			if(true == inBody && length > 0) {
				char *txtBuf = new char[length + 1];
				char *dst = txtBuf;

				// Collect and convert the text stream
				while(src < end) {
					if(*src <= ' ') {
						if(dst == txtBuf || (dst > txtBuf && dst[-1] != ' ')) {
							*(dst++) = ' ';
						}
					} else if('%' == *src) {
						UCHAR nibble1;
						UCHAR nibble2;

						// Process hex characters
						src++;

						if(TRUE == GetHexNibble(*src, &nibble1)) {
							src++;
							if(TRUE == GetHexNibble(*src, &nibble2)) {
								nibble1 = (nibble1 << 4) | nibble2;

								if(nibble1 >= 0x20 && nibble1 <= 0x7f) {
									*(dst++) = nibble1;
								}									
							}
						}
					} else {
						*(dst++) = *src;
					}
					src++;
				}

				*dst = 0;
				dst = txtBuf;

				// Remove leading spacing on the FIRST text block in a paragraph/line
				if(0 == textGroup.GetBlockCount()) {
					while(0 != *dst && ' ' == *dst) {
						dst++;
					}
				}

				// If there are actual characters here, add the text block
				if(0 != dst[0]) {
					textGroup.Add(new CSMLTextGroup::CSMLTextBlock(dst, fontInfo));
				}

				delete[] txtBuf;
			}

			src = end;
			break;
		}
	}

	if(0 != textGroup.GetBlockCount()) {
		AddGroup(textGroup);
		textGroup.Empty(FALSE);
	}

	optionList.Reset();

	delete fontInfo;

	return TRUE;
}

