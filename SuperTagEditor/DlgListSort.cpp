// DlgListSort.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgListSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgListSort ダイアログ


CDlgListSort::CDlgListSort(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgListSort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgListSort)
	m_nSortType1 = -1;
	m_nSortType2 = -1;
	m_nSortType3 = -1;
	m_nKeyColumn1 = -1;
	m_nKeyColumn2 = -1;
	m_nKeyColumn3 = -1;
	m_nKeyColumn4 = -1;
	m_nKeyColumn5 = -1;
	m_nSortType4 = -1;
	m_nSortType5 = -1;
	//}}AFX_DATA_INIT
}


void CDlgListSort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgListSort)
	DDX_Control(pDX, IDC_LIST_KEY5, m_listKey5);
	DDX_Control(pDX, IDC_LIST_KEY4, m_listKey4);
	DDX_Control(pDX, IDC_LIST_KEY3, m_listKey3);
	DDX_Control(pDX, IDC_LIST_KEY2, m_listKey2);
	DDX_Control(pDX, IDC_LIST_KEY1, m_listKey1);
	DDX_Radio(pDX, IDC_RA_UP_SORT1, m_nSortType1);
	DDX_Radio(pDX, IDC_RA_UP_SORT2, m_nSortType2);
	DDX_Radio(pDX, IDC_RA_UP_SORT3, m_nSortType3);
	DDX_CBIndex(pDX, IDC_LIST_KEY1, m_nKeyColumn1);
	DDX_CBIndex(pDX, IDC_LIST_KEY2, m_nKeyColumn2);
	DDX_CBIndex(pDX, IDC_LIST_KEY3, m_nKeyColumn3);
	DDX_CBIndex(pDX, IDC_LIST_KEY4, m_nKeyColumn4);
	DDX_CBIndex(pDX, IDC_LIST_KEY5, m_nKeyColumn5);
	DDX_Radio(pDX, IDC_RA_UP_SORT4, m_nSortType4);
	DDX_Radio(pDX, IDC_RA_UP_SORT5, m_nSortType5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListSort, CDialog)
	//{{AFX_MSG_MAP(CDlgListSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgListSort メッセージ ハンドラ

BOOL CDlgListSort::OnInitDialog() 
{
	CDialog::OnInitDialog();

	extern	const TCHAR	***g_sNameList;
	int i; for (i = 0; g_sNameList[0][i] != NULL; i++) {
		m_listKey1.AddString(g_sNameList[0][i]);
		m_listKey2.AddString(g_sNameList[0][i]);
		m_listKey3.AddString(g_sNameList[0][i]);
		m_listKey4.AddString(g_sNameList[0][i]);
		m_listKey5.AddString(g_sNameList[0][i]);
	}
	m_listKey1.SetCurSel(m_nKeyColumn1);
	m_listKey2.SetCurSel(m_nKeyColumn2);
	m_listKey3.SetCurSel(m_nKeyColumn3);
	m_listKey4.SetCurSel(m_nKeyColumn4);
	m_listKey5.SetCurSel(m_nKeyColumn5);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgListSort::OnOK() 
{
	CDialog::OnOK();
}
