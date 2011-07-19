// OptionPageStaticText.cpp : implementation file
//

#include "stdafx.h"
#include "OptionPageStaticText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// COptionPageStaticText property page

IMPLEMENT_DYNAMIC(COptionPageStaticText, COptionPage)

COptionPageStaticText::COptionPageStaticText(UINT captionID,
											 LPCSTR lpszText,
											 BOOL bIsHText) : COptionPage((UINT)0, captionID)
{
	//{{AFX_DATA_INIT(COptionPageStaticText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	if(TRUE == bIsHText) {
		m_Display.SetSMLText((char*)lpszText);
	} else {
		m_Display.SetPlainText(lpszText);
	}
}

#include <stdlib.h>



COptionPageStaticText::COptionPageStaticText(UINT captionID,
											 UINT idText) : COptionPage((UINT)0, captionID)
{
	m_Display.SetSMLText(idText);
}




COptionPageStaticText::~COptionPageStaticText()
{
}

void COptionPageStaticText::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageStaticText)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPageStaticText, COptionPage)
	//{{AFX_MSG_MAP(COptionPageStaticText)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageStaticText message handlers

BOOL COptionPageStaticText::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	m_Display.Create(0, CRect(0,0,0,0), this, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPageStaticText::OnSize(UINT nType, int cx, int cy) 
{
	COptionPage::OnSize(nType, cx, cy);

	if(TRUE == ::IsWindow(m_Display)) {
		m_Display.MoveWindow(0, 0, cx, cy);
		m_Display.RequestResize();
	}
	// TODO: Add your message handler code here
	
}
