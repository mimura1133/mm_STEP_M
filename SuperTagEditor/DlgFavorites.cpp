// DlgFavorites.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFavorites.h"
#include "SHBrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL SelectDirectory(LPTSTR sLocal);

/////////////////////////////////////////////////////////////////////////////
// CDlgFavorites ダイアログ


CDlgFavorites::CDlgFavorites(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFavorites::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFavorites)
	//}}AFX_DATA_INIT
	for (int i=0;i<10;i++) {
		m_strFavirites[i] = _T("");
	}
}


void CDlgFavorites::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFavorites)
	DDX_Text(pDX, IDC_EDIT_FAVORITES1, m_strFavirites[0]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES10, m_strFavirites[9]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES2, m_strFavirites[1]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES3, m_strFavirites[2]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES4, m_strFavirites[3]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES5, m_strFavirites[4]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES6, m_strFavirites[5]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES7, m_strFavirites[6]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES8, m_strFavirites[7]);
	DDX_Text(pDX, IDC_EDIT_FAVORITES9, m_strFavirites[8]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFavorites, CDialog)
	//{{AFX_MSG_MAP(CDlgFavorites)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES1, OnBtRefFavorites1)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES10, OnBtRefFavorites10)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES2, OnBtRefFavorites2)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES3, OnBtRefFavorites3)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES4, OnBtRefFavorites4)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES5, OnBtRefFavorites5)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES6, OnBtRefFavorites6)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES7, OnBtRefFavorites7)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES8, OnBtRefFavorites8)
	ON_BN_CLICKED(IDC_BT_REF_FAVORITES9, OnBtRefFavorites9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFavorites メッセージ ハンドラ

void CDlgFavorites::OnBtRefFavorites1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[0]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites10() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[9]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[1]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[2]);
	UpdateData(FALSE);

}

void CDlgFavorites::OnBtRefFavorites4() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[3]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites5() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[4]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites6() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[5]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites7() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[6]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites8() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[7]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites9() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	OnBtRefFavorites(m_strFavirites[8]);
	UpdateData(FALSE);
}

void CDlgFavorites::OnBtRefFavorites(CString &strFavorite)
{
	// フォルダ選択ダイアログを開く
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};
	lstrcpy(sFolderName, strFavorite);
	if (SelectDirectory(sFolderName) == TRUE) {
		strFavorite = sFolderName;
	}
}
