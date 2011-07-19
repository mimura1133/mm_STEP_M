// DlgConv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgConv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConv プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgConv, COptionPage)

CDlgConv::CDlgConv() : COptionPage(CDlgConv::IDD)
{
	//{{AFX_DATA_INIT(CDlgConv)
	m_bFirstUpperIgnoreWord = FALSE;
	m_strFirstUpperIgnoreWords = _T("");
	m_strFirstUpperSentenceSeparator = _T("");
	m_bUserConvAddMenu = FALSE;
	m_bZenHanKigouKana = FALSE;
	//}}AFX_DATA_INIT
}

CDlgConv::~CDlgConv()
{
}

void CDlgConv::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConv)
	DDX_Control(pDX, IDC_EDIT_FIRST_UPPER_SENTENCE_SEPARATOR, m_cFirstUpperSentenceSeparator);
	DDX_Control(pDX, IDC_EDIT_FIRST_UPPER_IGNORE_WORD, m_cFirstUperIgnoreWords);
	DDX_Check(pDX, IDC_CHECK_FIRST_UPPER_IGNORE_WORD, m_bFirstUpperIgnoreWord);
	DDX_Text(pDX, IDC_EDIT_FIRST_UPPER_IGNORE_WORD, m_strFirstUpperIgnoreWords);
	DDX_Text(pDX, IDC_EDIT_FIRST_UPPER_SENTENCE_SEPARATOR, m_strFirstUpperSentenceSeparator);
	DDX_Check(pDX, IDC_CHECK_USER_CONV_MENU, m_bUserConvAddMenu);
	DDX_Check(pDX, IDC_CH_ZENHAN_KIGOU_KANA, m_bZenHanKigouKana);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConv, COptionPage)
	//{{AFX_MSG_MAP(CDlgConv)
	ON_BN_CLICKED(IDC_CHECK_FIRST_UPPER_IGNORE_WORD, OnCheckFirstUpperIgnoreWord)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConv メッセージ ハンドラ

void CDlgConv::OnCheckFirstUpperIgnoreWord() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	if (m_bFirstUpperIgnoreWord) {
		m_cFirstUperIgnoreWords.EnableWindow(TRUE);
		m_cFirstUpperSentenceSeparator.EnableWindow(TRUE);
	} else {
		m_cFirstUperIgnoreWords.EnableWindow(FALSE);
		m_cFirstUpperSentenceSeparator.EnableWindow(FALSE);
	}
}

BOOL CDlgConv::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	OnCheckFirstUpperIgnoreWord();
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgConv::OnBtResetPage() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_strFirstUpperIgnoreWords = "a,an,and,at,by,for,in,into,of,on,or,the,to,with";
	m_strFirstUpperSentenceSeparator = ".";
	m_bFirstUpperIgnoreWord = FALSE;
	m_bUserConvAddMenu = FALSE;

	UpdateData(FALSE);
	OnCheckFirstUpperIgnoreWord();
}
