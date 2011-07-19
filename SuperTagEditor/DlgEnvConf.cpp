// DlgEnvConf.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgEnvConf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvConf プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgEnvConf, COptionPage)

CDlgEnvConf::CDlgEnvConf() : COptionPage(CDlgEnvConf::IDD)
{
	//{{AFX_DATA_INIT(CDlgEnvConf)
	m_bConfDeleteFile = FALSE;
	m_bConfDeleteList = FALSE;
	m_bConfEditModify = FALSE;
	m_bConfFolderSync = FALSE;
	//}}AFX_DATA_INIT
}

CDlgEnvConf::~CDlgEnvConf()
{
}

void CDlgEnvConf::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvConf)
	DDX_Check(pDX, IDC_CH_CONF_DELETE_FILE, m_bConfDeleteFile);
	DDX_Check(pDX, IDC_CH_CONF_DELETE_LIST, m_bConfDeleteList);
	DDX_Check(pDX, IDC_CH_CONF_EDIT_MODIFY, m_bConfEditModify);
	DDX_Check(pDX, IDC_CH_CONF_FOLDER_SYNC, m_bConfFolderSync);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvConf, COptionPage)
	//{{AFX_MSG_MAP(CDlgEnvConf)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvConf メッセージ ハンドラ

// 初期値に戻す
void CDlgEnvConf::OnBtResetPage() 
{
	((CButton *)GetDlgItem(IDC_CH_CONF_CONV_MP3   ))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_CONV_RMP   ))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_CONV_ID3V2 ))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_DELETE_FILE))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_DELETE_LIST))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_EDIT_MODIFY))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_CONF_FOLDER_SYNC))->SetCheck(TRUE);
}
