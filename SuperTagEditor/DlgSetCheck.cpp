// DlgSetCheck.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SuperTagEditor.h"
#include "MySuperGrid.h"
#include "DlgSetCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCheck ダイアログ


CDlgSetCheck::CDlgSetCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetCheck)
	m_bCheckDiffUL = FALSE;
	m_bMatchComplete = FALSE;
	m_bRangeSelected = FALSE;
	m_bRegExp = FALSE;
	m_strSearchWord = _T("");
	m_nTargetColumn = -1;
	m_bMatchSelected = FALSE;
	//}}AFX_DATA_INIT
	m_bEnableRangeSelected = FALSE;
}


void CDlgSetCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetCheck)
	DDX_Control(pDX, IDC_CH_MATCH_SELECTED, m_btMatchSelected);
	DDX_Control(pDX, IDC_CH_REGEXP, m_btRegExp);
	DDX_Control(pDX, IDC_CH_RANGE_SELECTED, m_btRangeSelected);
	DDX_Control(pDX, IDC_CH_MATCH_COMPLETE, m_btMatchComplete);
	DDX_Control(pDX, IDC_CH_CHECK_UL, m_btCheckDiffUL);
	DDX_Control(pDX, IDC_LIST_TARGET, m_listTargetColumn);
	DDX_Check(pDX, IDC_CH_CHECK_UL, m_bCheckDiffUL);
	DDX_Check(pDX, IDC_CH_MATCH_COMPLETE, m_bMatchComplete);
	DDX_Check(pDX, IDC_CH_RANGE_SELECTED, m_bRangeSelected);
	DDX_Check(pDX, IDC_CH_REGEXP, m_bRegExp);
	DDX_Text(pDX, IDC_ED_SEARCH_WORD, m_strSearchWord);
	DDX_CBIndex(pDX, IDC_LIST_TARGET, m_nTargetColumn);
	DDX_Check(pDX, IDC_CH_MATCH_SELECTED, m_bMatchSelected);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetCheck, CDialog)
	//{{AFX_MSG_MAP(CDlgSetCheck)
	ON_BN_CLICKED(IDC_CH_REGEXP, OnChRegexp)
	ON_CBN_SELCHANGE(IDC_LIST_TARGET, OnSelChangeListTarget)
	ON_BN_DOUBLECLICKED(IDC_CH_REGEXP, OnChRegexp)
	ON_BN_CLICKED(IDC_CH_RANGE_SELECTED, OnChRangeSelected)
	ON_BN_DOUBLECLICKED(IDC_CH_RANGE_SELECTED, OnChRangeSelected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCheck メッセージ ハンドラ

BOOL CDlgSetCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	extern	const TCHAR	***g_sNameList;
	// m_listTargetColumn.AddString("全ての項目");
	for (int i = 2; g_sNameList[0][i] != NULL; i++) {
		m_listTargetColumn.AddString(g_sNameList[0][i]);
	}
	m_listTargetColumn.SetCurSel(m_nTargetColumn);

	UpdateStatus();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgSetCheck::UpdateStatus(void)
{
	if (
		(m_nTargetColumn == COLUMN_TRACK_NUMBER-COLUMN_FILE_NAME) |
		(m_nTargetColumn == COLUMN_DISK_NUMBER-COLUMN_FILE_NAME)
		){
		m_btCheckDiffUL.EnableWindow(FALSE);
		m_btRegExp.EnableWindow(FALSE);
		m_btMatchComplete.EnableWindow(FALSE);
	} else {
		m_btCheckDiffUL.EnableWindow(m_bRegExp ? FALSE : TRUE);
		m_btRegExp.EnableWindow(TRUE);
		m_btRangeSelected.EnableWindow(m_bEnableRangeSelected);
		m_btMatchComplete.EnableWindow(TRUE);
	}
	m_btRangeSelected.EnableWindow(m_bEnableRangeSelected);
	m_btMatchSelected.EnableWindow((m_bEnableRangeSelected == FALSE || m_bRangeSelected == FALSE) ? TRUE : FALSE);
}

void CDlgSetCheck::OnChRegexp() 
{
	UpdateData();
	UpdateStatus();
}

void CDlgSetCheck::OnSelChangeListTarget() 
{
	UpdateData();
	UpdateStatus();
}

void CDlgSetCheck::OnChRangeSelected() 
{
	UpdateData();
	UpdateStatus();
}
