// DoubleZeroString.h: interface for the CDoubleZeroString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUBLEZEROSTRING_H__0E067530_2B7A_11D2_A0F2_0000832CDDC7__INCLUDED_)
#define AFX_DOUBLEZEROSTRING_H__0E067530_2B7A_11D2_A0F2_0000832CDDC7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDoubleZeroString  
{
public:
		// ctor and dtor
	CDoubleZeroString();
	virtual ~CDoubleZeroString();
		// initialization
	void Empty();

		// creating a dz string
		//
		// AddDoubleZero expects a double zeroterminated string:
		// "aa\0bb\0cc\0\0"
	bool AddDoubleZero(LPCTSTR dzstring);
		// Add adds a 'normal' zeroterminated string to the
		// CDoubleTerminatedString
	bool Add(LPCTSTR str);

		// accessing (parsing) the CDoubleTerminatedString
		//
		// get the complete double zeroterminated string:
		// "aa\0bb\0cc\0\0"
	operator LPCTSTR();
		// get the single substring at position 'index'
		// will return NULL if index is to high
		// "aa\0bb\0cc\0\0"
		// Get(0) --> "aa"
		// Get(3) --> NULL
	LPCTSTR Get(int index);
		// get the count of substrings:
		// "aa\0bb\0cc\0\0"
		// GetCount() -> 3
	int GetCount() const;
protected:
		// this array contains all substrings that were added by
		// AddDoubleZero() and Add()
	CStringArray m_ar;
		// returned by operator const char*()
	LPTSTR		m_buf;
};

#endif // !defined(AFX_DOUBLEZEROSTRING_H__0E067530_2B7A_11D2_A0F2_0000832CDDC7__INCLUDED_)
