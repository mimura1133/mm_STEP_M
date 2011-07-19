// DlgLyricFile.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "SHBrowseForFolder.h"
#include "DlgLyricFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// =============================================
// SelectDirectory
// 概要  : フォルダ参照ダイアログ処理
// 引数  : sLocal			= パス(入出力)
// 戻り値: BOOL
// =============================================
BOOL SelectDirectory(char *sLocal)
{
	CSHBrowseForFolder	browse;
	browse.SetEnableSubDirButton(false);
	return(browse.Exec(sLocal) ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgLyricFile プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgLyricFile, COptionPage)

CDlgLyricFile::CDlgLyricFile() : COptionPage(CDlgLyricFile::IDD)
{
	//{{AFX_DATA_INIT(CDlgLyricFile)
	m_bChangeTextFile = FALSE;
	m_strLyricsPath = _T("");
	m_bSetLyricsDir = FALSE;
	m_bSearchLyricsSubDir = FALSE;
	//}}AFX_DATA_INIT
}

CDlgLyricFile::~CDlgLyricFile()
{
}

void CDlgLyricFile::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLyricFile)
	DDX_Control(pDX, IDC_CH_SEARCH_LYRICS_SUB_DIR, m_btSearchLyricsSubDir);
	DDX_Control(pDX, IDC_ED_LYRICS_PATH, m_editLyricsPath);
	DDX_Control(pDX, IDC_CH_SET_LYRICS_DIR, m_btSetLyricsDir);
	DDX_Control(pDX, IDC_BT_REF_LYRICS, m_btRefLyrics);
	DDX_Check(pDX, IDC_CH_CHANGE_TEXT_FILENAME, m_bChangeTextFile);
	DDX_Text(pDX, IDC_ED_LYRICS_PATH, m_strLyricsPath);
	DDX_Check(pDX, IDC_CH_SET_LYRICS_DIR, m_bSetLyricsDir);
	DDX_Check(pDX, IDC_CH_SEARCH_LYRICS_SUB_DIR, m_bSearchLyricsSubDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLyricFile, COptionPage)
	//{{AFX_MSG_MAP(CDlgLyricFile)
	ON_BN_CLICKED(IDC_BT_REF_LYRICS, OnBtRefLyrics)
	ON_BN_CLICKED(IDC_CH_SET_LYRICS_DIR, OnChSetLyricsDir)
	ON_BN_CLICKED(IDC_CH_CHANGE_TEXT_FILENAME, OnChChangeTextFilename)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLyricFile メッセージ ハンドラ
BOOL CDlgLyricFile::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	UpdateStatus();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgLyricFile::UpdateStatus(void)
{
	UpdateData();

	m_btSetLyricsDir.EnableWindow(m_bChangeTextFile ? TRUE : FALSE);
	m_btRefLyrics.EnableWindow(m_bSetLyricsDir ? TRUE : FALSE);
	m_btSearchLyricsSubDir.EnableWindow(m_bSetLyricsDir ? TRUE : FALSE);
	m_editLyricsPath.EnableWindow(m_bSetLyricsDir ? TRUE : FALSE);
}

void CDlgLyricFile::OnBtRefLyrics() 
{
	// 現在指定されているログファイル名を取得
	CString	strFileName;
	GetDlgItemText(IDC_ED_LYRICS_PATH, strFileName);

	// フォルダ選択ダイアログを開く
	char	sFolderName[_MAX_PATH] = {'\0'};
	strcpy(sFolderName, strFileName);
	if (SelectDirectory(sFolderName) == TRUE) {
		SetDlgItemText(IDC_ED_LYRICS_PATH, sFolderName);
	}
}

void CDlgLyricFile::OnChSetLyricsDir() 
{
	UpdateStatus();
}

void CDlgLyricFile::OnChChangeTextFilename() 
{
	UpdateStatus();
}

// 初期値に戻す
void CDlgLyricFile::OnBtResetPage() 
{
	// 歌詞ファイル
	((CButton *)GetDlgItem(IDC_CH_CHANGE_TEXT_FILENAME))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_SET_LYRICS_DIR))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_CH_SEARCH_LYRICS_SUB_DIR))->SetCheck(FALSE);
	((CWnd *)GetDlgItem(IDC_ED_LYRICS_PATH))->SetWindowText("");

	UpdateStatus();
}
