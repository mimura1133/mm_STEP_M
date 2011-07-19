/*!
	\file
*/
#if !defined(AFX_OPTIONPAGESTATICTEXT_H__B8D7375C_60C3_4F10_8DDA_FA777FB5DAB4__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_OPTIONPAGESTATICTEXT_H__B8D7375C_60C3_4F10_8DDA_FA777FB5DAB4__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionPageStaticText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionPageStaticText dialog
#include "OptionPage.h"
#include "SMLStatic.h"

/*!
	\brief Option page consisting only of formatted static text.

	This classes uses the CSMLDoc and CSMLStatic objects to provide a readily
	available solution for providing potion pages that contain formatted text.

	This COptionPage does not require a dialog resource as it takes either
	an "SMLDOC" resource id or a string buffer.

	\sa	SML CSMLDoc CSMLSatic
*/
class COptionPageStaticText : public COptionPage
{
	DECLARE_DYNAMIC(COptionPageStaticText)

private:
	CSMLStatic	m_Display;	//!<	SML Static control


// Construction
public:
	/*!
		\brief	Constructor

		Constructs a static text page.

		\param	captionID	ID of the string table resource to use for the caption.
		\param	lpszText	Pointer to a string buffer containing the SML or plain text.
		\param	bIsHText	If TRUE, the supplied text is SML formatted, otherwise it is plain text.
	*/
	COptionPageStaticText(UINT captionID, LPCSTR lpszText, BOOL bIsHText = FALSE);

	/*!
		\brief	Constructor

		Constructs a static text page.

		\param	captionID	ID of the string table resource to use for the caption.
		\param	idText		ID of the "SMLDOC" resource to be used 
	*/
	COptionPageStaticText(UINT captionID, UINT idText);

	/*!
		\brief	Destructor
	*/
	~COptionPageStaticText();

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Dialog Data
	//{{AFX_DATA(COptionPageStaticText)
//	enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPageStaticText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPageStaticText)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPAGESTATICTEXT_H__B8D7375C_60C3_4F10_8DDA_FA777FB5DAB4__INCLUDED_)
