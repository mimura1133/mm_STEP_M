// DlgFileNameMaxCheck.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFileNameMaxCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameMaxCheck ダイアログ


CDlgFileNameMaxCheck::CDlgFileNameMaxCheck(CWnd* pParent /*=NULL*/)
	: COptionPage(CDlgFileNameMaxCheck::IDD)
{
	//{{AFX_DATA_INIT(CDlgFileNameMaxCheck)
	m_bFileNameMaxCheck = FALSE;
	m_nFileNameMaxChar = 0;
	m_bFileNameMaxCellColor = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFileNameMaxCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileNameMaxCheck)
	DDX_Control(pDX, IDC_CH_FILENAME_MAX_CELL_COLOR, m_checkFileNameMaxCellColor);
	DDX_Check(pDX, IDC_CH_FILENAME_MAX_CHECK, m_bFileNameMaxCheck);
	DDX_Text(pDX, IDC_ED_MAX_CHAR, m_nFileNameMaxChar);
	DDV_MinMaxUInt(pDX, m_nFileNameMaxChar, 1, 255);
	DDX_Check(pDX, IDC_CH_FILENAME_MAX_CELL_COLOR, m_bFileNameMaxCellColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileNameMaxCheck, CDialog)
	//{{AFX_MSG_MAP(CDlgFileNameMaxCheck)
	ON_BN_CLICKED(IDC_CH_FILENAME_MAX_CHECK, OnChFilenameMaxCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameMaxCheck メッセージ ハンドラ

void CDlgFileNameMaxCheck::OnChFilenameMaxCheck() /* SeaKnows 036 */
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	if (m_bFileNameMaxCheck) {
		m_checkFileNameMaxCellColor.EnableWindow(TRUE);
	} else {
		m_checkFileNameMaxCellColor.EnableWindow(FALSE);
	}
}
