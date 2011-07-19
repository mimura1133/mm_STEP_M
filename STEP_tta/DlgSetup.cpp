// DlgSetup.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "STEP_tta.h"
#include "DlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgSetup, CPropertyPage)

CDlgSetup::CDlgSetup() : CPropertyPage(CDlgSetup::IDD)
{
	//{{AFX_DATA_INIT(CDlgSetup)
	m_bGenreListSelect = FALSE;
	m_bID3TagAutoDelete = FALSE;
	m_bID3TagAutoWrite = FALSE;
	//}}AFX_DATA_INIT
}

CDlgSetup::~CDlgSetup()
{
}

void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetup)
	DDX_Check(pDX, IDC_CH_GENRE_LIST_SELECT, m_bGenreListSelect);
	DDX_Check(pDX, IDC_CH_ID3TAG_AUTO_DELETE, m_bID3TagAutoDelete);
	DDX_Check(pDX, IDC_CH_ID3TAG_AUTO_WRITE, m_bID3TagAutoWrite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgSetup)
	ON_BN_CLICKED(IDC_CH_ID3TAG_AUTO_DELETE, OnChId3tagAutoDelete)
	ON_BN_CLICKED(IDC_CH_ID3TAG_AUTO_WRITE, OnChId3tagAutoWrite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup メッセージ ハンドラ

void CDlgSetup::OnChId3tagAutoDelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	if (m_bID3TagAutoDelete) {
		m_bID3TagAutoWrite = FALSE;
		UpdateData(FALSE);
	}
}

void CDlgSetup::OnChId3tagAutoWrite() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	if (m_bID3TagAutoWrite) {
		m_bID3TagAutoDelete = FALSE;
		UpdateData(FALSE);
	}
}
