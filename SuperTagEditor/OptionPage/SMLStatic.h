/*!
	\file
*/
#if !defined(AFX_SMLSTATIC_H__463F779A_1BA6_4B9F_8262_F25744700BA4__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_SMLSTATIC_H__463F779A_1BA6_4B9F_8262_F25744700BA4__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMLStatic.h : header file
//

#include "SMLDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSMLStatic window

/*!
	\brief	A CRichEditCtrl derived class capable of displaying text from a
			markup document.

	The CSMLStatic class represents a CRichEditCtrl derived object which can
	parse and display formatted text. The text is formatted using an markup
	language loosly based on HTML and includes many of the same tag constructs.
	Formatting for color, font, style, and justification is included and can
	be provided to the control either by a string buffer or a resource ID.

	CSMLStatic should always be used as a child control in a CWnd derived
	object such as a CView or CDialog.

	\sa SML CSMLDoc

*/
class CSMLStatic : public CRichEditCtrl
{
private:
	CSMLDoc	*m_Document;	//!<	Document object.
	CString	m_Text;			//!<	Plain text.

	/*!
		\brief	Inserts the plain text or the document text into the CRichEditCtrl.

		If a CSMLDoc object exists (m_Document) it will be used as the text to add
		to the rich edit control. If the CSMLDoc object does not exists, the plain
		text contained in m_Text will be used instead.
	*/
	void InsertDocument();


// Construction
public:
	/*!
		\brief	Constructor.
	*/
	CSMLStatic();

	/*!
		Destructor.
	*/
	virtual ~CSMLStatic();

	/*!
		\brief Creates a CSMLDoc object and parses the character stream.

		This method is called in order to create a CSMLDoc object and parse the
		character string. The parsing expects the character string to contain
		formatting tags, but they are not necessary.

		\param	stream	NULL terminated string containing the text to parse.

		\return	State of the parsing operation
		\retval	TRUE	Operation was successful.
		\retval FALSE	Operation failed.
	*/
	BOOL SetSMLText(LPCSTR stream);

	/*!
		\brief Creates a CSMLDoc object and parses the character stream.

		This method is called in order to create a CSMLDoc object and parse the
		contents of the specified resource. The parsing expects the resource to
		be of type "SMLDOC" and be NULL terminated.

		\param	nID	ID of the resource item to parse.

		\return	State of the parsing operation
		\retval	TRUE	Operation was successful.
		\retval FALSE	Operation failed.
	*/
	BOOL SetSMLText(UINT nID);

	/*!
		\brief	Sets the plain text to use for display.

		\param	lpszText	Pointer to a NULL terminated string.

		\return	State of the operation.
		\retval	TRUE	Operation was successful.
		\retval FALSE	Operation failed.
	*/
	BOOL SetPlainText(LPCSTR lpszText);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMLStatic)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CSMLStatic)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

#endif

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMLSTATIC_H__463F779A_1BA6_4B9F_8262_F25744700BA4__INCLUDED_)
