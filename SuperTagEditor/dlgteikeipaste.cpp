// DlgTeikeiPaste.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgTeikeiPaste.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTeikeiPaste ダイアログ


CDlgTeikeiPaste::CDlgTeikeiPaste(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeikeiPaste::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTeikeiPaste)
	m_nTeikeiPaste = -1;
	m_bAddSpace = FALSE;
	m_strFront = _T("");
	m_strBack = _T("");
	m_bAddChar = FALSE;
	m_bShowDialog = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTeikeiPaste::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTeikeiPaste)
	DDX_Radio(pDX, IDC_RADIO_REPLACE, m_nTeikeiPaste);
	DDX_Check(pDX, IDC_CHECK_ADD_SPACE, m_bAddSpace);
	DDX_Text(pDX, IDC_EDIT_ADD_FRONT, m_strFront);
	DDX_Text(pDX, IDC_EDIT_ADD_BACK, m_strBack);
	DDX_Check(pDX, IDC_CHECK_ADD_CHAR, m_bAddChar);
	DDX_Check(pDX, IDC_CH_SHOW_DIALOG, m_bShowDialog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTeikeiPaste, CDialog)
	//{{AFX_MSG_MAP(CDlgTeikeiPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeikeiPaste メッセージ ハンドラ

BOOL CDlgTeikeiPaste::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if (!m_strTitle.IsEmpty()) {
		SetWindowText(m_strTitle);
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
