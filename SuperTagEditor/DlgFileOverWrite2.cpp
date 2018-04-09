// DlgFileOverWrite2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFileOverWrite2.h"
#include "DlgFileNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite22 ダイアログ


CDlgFileOverWrite2::CDlgFileOverWrite2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileOverWrite2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileOverWrite2)
	m_strFileName = _T("");
	m_strTimeStamp = _T("");
	m_strSize = _T("");
	//}}AFX_DATA_INIT
	m_nResult = 3;
}


void CDlgFileOverWrite2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileOverWrite2)
	DDX_Text(pDX, IDC_ED_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_ED_TIME_STAMP, m_strTimeStamp);
	DDX_Text(pDX, IDC_ED_SIZE, m_strSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileOverWrite2, CDialog)
	//{{AFX_MSG_MAP(CDlgFileOverWrite2)
	ON_BN_CLICKED(IDC_BT_OVERWRITE, OnBtOverwrite)
	ON_BN_CLICKED(IDC_BT_ALL_OVERWRITE, OnBtAllOverwrite)
	ON_BN_CLICKED(IDC_BT_RENAME, OnBtRename)
	ON_BN_CLICKED(IDC_BT_SKIP, OnBtSkip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite2 メッセージ ハンドラ

void CDlgFileOverWrite2::OnOK() 
{
	CDialog::OnOK();
}

void CDlgFileOverWrite2::OnBtOverwrite() 
{
	m_nResult = 0;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite2::OnBtAllOverwrite() 
{
	m_nResult = 1;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite2::OnBtRename() 
{
	TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_tsplitpath(m_strFileName, drive, dir, fname, ext);
	CDlgFileNameChange dialog;
	dialog.m_nMaxChar = g_nConfFileNameMaxChar - lstrlen(ext);
	dialog.m_strFileName = fname;
	dialog.m_strOrgFileName = fname;
	//dialog.m_strMsg.Format("ファイル名を %d 文字以内に変更してください", dialog.m_nMaxChar);
	dialog.m_strMsgFormat = "ファイル名を %d(%d) 文字以内で変更してください";
	if (dialog.DoModal() == IDOK) {
		TCHAR	sDestFile[FILENAME_MAX+1];
		_tmakepath(sDestFile, drive, dir, dialog.m_strFileName, ext);
		if (GetFileAttributes(sDestFile) == 0xFFFFFFFF) {
			m_strFileName = sDestFile;
			UpdateData(FALSE);
			m_nResult = 4;
			PostMessage(WM_COMMAND, IDOK);
		} else {
			PostMessage(WM_COMMAND, IDC_BT_RENAME);
		}
	}
}


void CDlgFileOverWrite2::OnBtSkip() 
{
	m_nResult = 2;
	PostMessage(WM_COMMAND, IDOK);
}

void CDlgFileOverWrite2::OnCancel() 
{
	m_nResult = 3;
	PostMessage(WM_COMMAND, IDOK);
}
