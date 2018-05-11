// SMLStatic.cpp : implementation file
//

#include "stdafx.h"
#include "SMLStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMLStatic

CSMLStatic::CSMLStatic()
{
	m_Document = NULL;
}

CSMLStatic::~CSMLStatic()
{
	if(NULL != m_Document) {
		delete m_Document;
	}
}


BEGIN_MESSAGE_MAP(CSMLStatic, CRichEditCtrl)
	//{{AFX_MSG_MAP(CSMLStatic)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMLStatic message handlers

void CSMLStatic::OnLButtonDblClk(UINT nFlags, CPoint point) {}
void CSMLStatic::OnLButtonDown(UINT nFlags, CPoint point) {}
void CSMLStatic::OnLButtonUp(UINT nFlags, CPoint point) {}
void CSMLStatic::OnMButtonDblClk(UINT nFlags, CPoint point) {}
void CSMLStatic::OnMButtonDown(UINT nFlags, CPoint point) {}
void CSMLStatic::OnMButtonUp(UINT nFlags, CPoint point) {}
void CSMLStatic::OnRButtonDblClk(UINT nFlags, CPoint point) {}
void CSMLStatic::OnRButtonDown(UINT nFlags, CPoint point) {}
void CSMLStatic::OnRButtonUp(UINT nFlags, CPoint point) {}

BOOL CSMLStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return TRUE;
}


BOOL CSMLStatic::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= ES_MULTILINE | WS_VISIBLE | WS_VSCROLL;
	
	return CRichEditCtrl::PreCreateWindow(cs);
}


int CSMLStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InsertDocument();
	
	return 0;
}



// Implementation
BOOL CSMLStatic::SetSMLText(UINT nID)
{
	BOOL result;

	if(NULL != m_Document) {
		delete m_Document;
	}

	m_Document = new CSMLDoc;

	result = m_Document->ParseSMLText(nID);

	InsertDocument();

	return result;
}


BOOL CSMLStatic::SetSMLText(LPCTSTR stream)
{
	BOOL result;

	if(NULL != m_Document) {
		delete m_Document;
	}

	m_Document = new CSMLDoc;

	result = m_Document->ParseSMLText(stream);

	InsertDocument();

	return result;
}


BOOL CSMLStatic::SetPlainText(LPCTSTR lpszText)
{
	if(NULL != m_Document) {
		delete m_Document;
	}

	m_Text = lpszText;

	InsertDocument();

	return TRUE;
}



void CSMLStatic::InsertDocument()
{
	if(FALSE == ::IsWindow(m_hWnd)) {
		return;
	}

	CSMLDoc::CSMLTextGroup				*group;
	CSMLDoc::CSMLTextGroup::CSMLTextBlock	*blk;
	int			grpCount;
	int			grpIndex;
	int			blkCount;
	int			blkIndex;

	// Make sure we can insert text to the rich edit control
	SetReadOnly(FALSE);
	SetSel(0, -1);
	Clear();

	// If a SML document is not present, set the plain text and return
	if(NULL == m_Document) {
		ReplaceSel(m_Text, FALSE);
		return;
	}

	// Set the background color to the document
	SetBackgroundColor(FALSE, m_Document->GetBkgndColor());

	// Step through all text groups
	grpCount = m_Document->GetGroupCount();
	for(grpIndex = 0; grpIndex < grpCount; grpIndex++) {
		PARAFORMAT	format;
		DWORD		style;
		group = m_Document->GetTextGroup(grpIndex);

		blkCount = group->GetBlockCount();

		// Get the default font information for the group
		format.cbSize = sizeof(format);
		format.dwMask = PFM_ALIGNMENT;
		format.wAlignment = PFA_LEFT;

		// Step through all text blocks for the group and collect the
		// text justification
		for(blkIndex = 0; blkIndex < blkCount; blkIndex++) {
			blk = group->GetBlock(blkIndex);

			// Process the style information
			style = blk->GetStyle() & JUSTIFY_MASK;

			if(0 != style) {
				switch(style) {
				case JUSTIFY_LEFT:
					format.wAlignment =	PFA_LEFT;
					break;
				case JUSTIFY_CENTER:
					format.wAlignment =	PFA_CENTER;
					break;
				case JUSTIFY_RIGHT:
					format.wAlignment =	PFA_RIGHT;
					break;
				default:
					ASSERT(0);
				}
				break;
			}
		}
		
		// Set the paragraph font and style information
		SetParaFormat(format);


		// Step through all text blocks and insert the text
		for(blkIndex = 0; blkIndex < blkCount; blkIndex++) {
			CHARFORMAT	charFmt;
			CString		typeface;

			blk = group->GetBlock(blkIndex);
			

			// Create the text block font formatting information for the
			// rich edit control
			charFmt.cbSize = sizeof(charFmt);
			charFmt.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_SIZE | CFM_COLOR;
			charFmt.dwEffects = 0;
			charFmt.yHeight = blk->GetPointSize() * 20;
			charFmt.crTextColor = blk->GetColor();

			// Add typeface
			typeface = blk->GetTypeface();
			if(0 == typeface.GetLength()) {
				typeface = m_Document->GetTypeface();
			}
			
			if(typeface.GetLength() >= LF_FACESIZE) {
				TRACE1("Typeface %s is too long for CHARFORMAT member\n", typeface.GetBuffer(0));
			} else {
				lstrcpy(charFmt.szFaceName, typeface);
				charFmt.bPitchAndFamily = FF_DONTCARE;
				charFmt.dwMask |= CFM_FACE;
			}

			// Add style
			style = blk->GetStyle();
			if(style & STYLE_BOLD) {
				charFmt.dwEffects |= CFE_BOLD;
			}

			if(style & STYLE_ITALICS) {
				charFmt.dwEffects |= CFE_ITALIC;
			}

			if(style & STYLE_UNDERLINE) {
				charFmt.dwEffects |= CFE_UNDERLINE;
			}

			SetSelectionCharFormat(charFmt);
			
			ReplaceSel(blk->GetText(), FALSE);
		}

		ReplaceSel(TEXT("\n"), FALSE);
	}


	SetReadOnly(TRUE);
}

