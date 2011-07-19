// DlgFolderSync.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFolderSync.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFolderSync プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgFolderSync, COptionPage)

CDlgFolderSync::CDlgFolderSync() : COptionPage(CDlgFolderSync::IDD)
{
	//{{AFX_DATA_INIT(CDlgFolderSync)
	m_bEnableFolderSync = FALSE;
	m_strRootFolder = _T("");
	m_bMoveLyricsFile = FALSE;
	m_bDeleteFolder = FALSE;
	m_bSelectAlways = FALSE;
	//}}AFX_DATA_INIT
}

CDlgFolderSync::~CDlgFolderSync()
{
}

void CDlgFolderSync::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFolderSync)
	DDX_Control(pDX, IDC_ED_ROOT_FOLDER, m_editRootFolder);
	DDX_Check(pDX, IDC_CH_ENABLE_FOLDER_SYNC, m_bEnableFolderSync);
	DDX_Text(pDX, IDC_ED_ROOT_FOLDER, m_strRootFolder);
	DDX_Check(pDX, IDC_CH_MOVE_LYRICS_FILE, m_bMoveLyricsFile);
	DDX_Check(pDX, IDC_CH_DELETE_FOLDER, m_bDeleteFolder);
	DDX_Check(pDX, IDC_CH_SELECT_ALWAYS, m_bSelectAlways);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFolderSync, COptionPage)
	//{{AFX_MSG_MAP(CDlgFolderSync)
	ON_BN_CLICKED(IDC_CH_ENABLE_FOLDER_SYNC, OnChEnableFolderSync)
	ON_BN_CLICKED(IDC_BT_REF_ROOT, OnBtRefRoot)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFolderSync メッセージ ハンドラ

BOOL CDlgFolderSync::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	UpdateStatus();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgFolderSync::UpdateStatus(void)
{
	UpdateData();

	BOOL	bFlag = m_bEnableFolderSync ? TRUE : FALSE;
	((CWnd *)GetDlgItem(IDC_ST_ROOT_FOLDER     ))->EnableWindow(bFlag);
	((CWnd *)GetDlgItem(IDC_ST_OPTION          ))->EnableWindow(bFlag);
	((CWnd *)GetDlgItem(IDC_ED_ROOT_FOLDER     ))->EnableWindow(bFlag);
	((CWnd *)GetDlgItem(IDC_BT_REF_ROOT        ))->EnableWindow(bFlag);
	((CWnd *)GetDlgItem(IDC_CH_MOVE_LYRICS_FILE))->EnableWindow(bFlag);
	((CWnd *)GetDlgItem(IDC_CH_DELETE_FOLDER   ))->EnableWindow(bFlag);
}

void CDlgFolderSync::OnChEnableFolderSync() 
{
	UpdateStatus();
}

void CDlgFolderSync::OnBtRefRoot() 
{
	// 現在指定されているログファイル名を取得
	CString	strFileName;
	GetDlgItemText(IDC_ED_ROOT_FOLDER, strFileName);

	// フォルダ選択ダイアログを開く
	extern	BOOL SelectDirectory(char *);
	char	sFolderName[_MAX_PATH] = {'\0'};
	strcpy(sFolderName, strFileName);
	if (SelectDirectory(sFolderName) == TRUE) {
		SetDlgItemText(IDC_ED_ROOT_FOLDER, sFolderName);
	}
}

// 初期値に戻す
void CDlgFolderSync::OnBtResetPage() 
{
	// 有効にする
	((CButton *)GetDlgItem(IDC_CH_ENABLE_FOLDER_SYNC))->SetCheck(FALSE);

	// root に対応するフォルダ
	((CWnd *)GetDlgItem(IDC_ED_ROOT_FOLDER))->SetWindowText("");
	((CButton *)GetDlgItem(IDC_CH_SELECT_ALWAYS))->SetCheck(FALSE);

	// 動作設定
	((CButton *)GetDlgItem(IDC_CH_DELETE_FOLDER))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CH_MOVE_LYRICS_FILE))->SetCheck(TRUE);

	UpdateStatus();
}
