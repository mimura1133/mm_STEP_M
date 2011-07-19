// DlgFileOverWrite.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFileOverWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite ダイアログ


CDlgFileOverWrite::CDlgFileOverWrite(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileOverWrite::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileOverWrite)
	m_strFileName = _T("");
	m_strTimeStamp = _T("");
	m_strSize = _T("");
	//}}AFX_DATA_INIT
	m_nResult = 3;
}


void CDlgFileOverWrite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileOverWrite)
	DDX_Text(pDX, IDC_ED_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_ED_TIME_STAMP, m_strTimeStamp);
	DDX_Text(pDX, IDC_ED_SIZE, m_strSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileOverWrite, CDialog)
	//{{AFX_MSG_MAP(CDlgFileOverWrite)
	ON_BN_CLICKED(IDC_BT_OVERWRITE, OnBtOverwrite)
	ON_BN_CLICKED(IDC_BT_ALL_OVERWRITE, OnBtAllOverwrite)
	ON_BN_CLICKED(IDC_BT_SKIP, OnBtSkip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite メッセージ ハンドラ

void CDlgFileOverWrite::OnOK() 
{
	CDialog::OnOK();
}

void CDlgFileOverWrite::OnBtOverwrite() 
{
	m_nResult = 0;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite::OnBtAllOverwrite() 
{
	m_nResult = 1;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite::OnBtSkip() 
{
	m_nResult = 2;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite::OnCancel() 
{
	m_nResult = 3;
	PostMessage(WM_COMMAND, IDOK);
}
