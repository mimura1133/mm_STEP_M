// DlgReplaceFileName.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgReplaceFileName.h"

#include "DlgUnifyChar.h" /* LastTrain 058 */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReplaceFileName ダイアログ




IMPLEMENT_DYNCREATE(CDlgReplaceFileName, COptionPage)


CDlgReplaceFileName::CDlgReplaceFileName() : COptionPage(CDlgReplaceFileName::IDD)
{
	//{{AFX_DATA_INIT(CDlgReplaceFileName)
	m_bFileNameReplace = FALSE;
	m_nExtChange = -1;
	//}}AFX_DATA_INIT
}

CDlgReplaceFileName::~CDlgReplaceFileName()
{
}


void CDlgReplaceFileName::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReplaceFileName)
	DDX_Text(pDX, IDC_ED_BEFORE1, m_strBefore[0]);
	DDX_Text(pDX, IDC_ED_BEFORE2, m_strBefore[1]);
	DDX_Text(pDX, IDC_ED_BEFORE3, m_strBefore[2]);
	DDX_Text(pDX, IDC_ED_BEFORE4, m_strBefore[3]);
	DDX_Text(pDX, IDC_ED_BEFORE5, m_strBefore[4]);
	DDX_Text(pDX, IDC_ED_BEFORE6, m_strBefore[5]);
	DDX_Text(pDX, IDC_ED_BEFORE7, m_strBefore[6]);
	DDX_Text(pDX, IDC_ED_BEFORE8, m_strBefore[7]);
	DDX_Text(pDX, IDC_ED_BEFORE9, m_strBefore[8]);
	DDX_Text(pDX, IDC_ED_BEFORE10, m_strBefore[9]);
	DDX_Text(pDX, IDC_ED_BEFORE11, m_strBefore[10]);
	DDX_Text(pDX, IDC_ED_BEFORE12, m_strBefore[11]);
	DDX_Text(pDX, IDC_ED_AFTER1, m_strAfter[0]);
	DDX_Text(pDX, IDC_ED_AFTER2, m_strAfter[1]);
	DDX_Text(pDX, IDC_ED_AFTER3, m_strAfter[2]);
	DDX_Text(pDX, IDC_ED_AFTER4, m_strAfter[3]);
	DDX_Text(pDX, IDC_ED_AFTER5, m_strAfter[4]);
	DDX_Text(pDX, IDC_ED_AFTER6, m_strAfter[5]);
	DDX_Text(pDX, IDC_ED_AFTER7, m_strAfter[6]);
	DDX_Text(pDX, IDC_ED_AFTER8, m_strAfter[7]);
	DDX_Text(pDX, IDC_ED_AFTER9, m_strAfter[8]);
	DDX_Text(pDX, IDC_ED_AFTER10, m_strAfter[9]);
	DDX_Text(pDX, IDC_ED_AFTER11, m_strAfter[10]);
	DDX_Text(pDX, IDC_ED_AFTER12, m_strAfter[11]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE1, m_strUserBefore[0]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE2, m_strUserBefore[1]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE3, m_strUserBefore[2]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE4, m_strUserBefore[3]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE5, m_strUserBefore[4]);
	DDX_Text(pDX, IDC_ED_USER_BEFORE6, m_strUserBefore[5]);
	DDX_Text(pDX, IDC_ED_USER_AFTER1, m_strUserAfter[0]);
	DDX_Text(pDX, IDC_ED_USER_AFTER2, m_strUserAfter[1]);
	DDX_Text(pDX, IDC_ED_USER_AFTER3, m_strUserAfter[2]);
	DDX_Text(pDX, IDC_ED_USER_AFTER4, m_strUserAfter[3]);
	DDX_Text(pDX, IDC_ED_USER_AFTER5, m_strUserAfter[4]);
	DDX_Text(pDX, IDC_ED_USER_AFTER6, m_strUserAfter[5]);
	DDX_Radio(pDX, IDC_RADIO_EXT_NONE, m_nExtChange);
	DDX_Check(pDX, IDC_CH_FILENAME_MULTI_BYTE, m_bFileNameReplace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReplaceFileName, COptionPage)
	//{{AFX_MSG_MAP(CDlgReplaceFileName)
	ON_BN_CLICKED(IDC_CH_FILENAME_MULTI_BYTE, OnChFilenameMultiByte)
	ON_BN_DOUBLECLICKED(IDC_CH_FILENAME_MULTI_BYTE, OnChFilenameMultiByte)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	ON_BN_CLICKED(IDC_BT_FILENAME_UNIFY, OnBtFileNameUnify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReplaceFileName メッセージ ハンドラ

BOOL CDlgReplaceFileName::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgReplaceFileName::OnChFilenameMultiByte() 
{
}

// 初期値に戻す
void CDlgReplaceFileName::OnBtResetPage() 
{
	((CButton *)GetDlgItem(IDC_CH_FILENAME_MULTI_BYTE))->SetCheck(TRUE);

	((CWnd *)GetDlgItem(IDC_ED_AFTER1))->SetWindowText(g_sRepTable[0][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER2))->SetWindowText(g_sRepTable[1][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER3))->SetWindowText(g_sRepTable[2][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER4))->SetWindowText(g_sRepTable[3][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER5))->SetWindowText(g_sRepTable[4][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER6))->SetWindowText(g_sRepTable[5][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER7))->SetWindowText(g_sRepTable[6][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER8))->SetWindowText(g_sRepTable[7][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER9))->SetWindowText(g_sRepTable[8][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER10))->SetWindowText(g_sRepTable[9][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER11))->SetWindowText(g_sRepTable[10][1]);
	((CWnd *)GetDlgItem(IDC_ED_AFTER12))->SetWindowText(g_sRepTable[11][1]);

	/* FreeFall 050 */
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE1))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE2))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE3))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE4))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE5))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_BEFORE6))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER1))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER2))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER3))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER4))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER5))->SetWindowText(TEXT(""));
	((CWnd *)GetDlgItem(IDC_ED_USER_AFTER6))->SetWindowText(TEXT(""));

	m_nFileUnifyAlpha = 0;
	m_nFileUnifyHiraKata = 0;
	m_nFileUnifyKata = 0;
	m_nFileUnifyKigou = 0;
	m_nFileUnifySuji = 0;
	m_nFileUnifyUpLow = 0;
	m_nExtChange = 0;
}

void CDlgReplaceFileName::OnBtFileNameUnify() /* LastTrain 058 */
{
	CDlgUnifyChar dlgUnify;

	dlgUnify.m_nConvAlpha = m_nFileUnifyAlpha;
	dlgUnify.m_nConvHiraKata = m_nFileUnifyHiraKata;
	dlgUnify.m_nConvKata = m_nFileUnifyKata;
	dlgUnify.m_nConvKigou = m_nFileUnifyKigou;
	dlgUnify.m_nConvSuji = m_nFileUnifySuji;
	dlgUnify.m_nConvUpLow = m_nFileUnifyUpLow;
	if (dlgUnify.DoModal() == IDOK) {
		m_nFileUnifyAlpha = dlgUnify.m_nConvAlpha;
		m_nFileUnifyHiraKata = dlgUnify.m_nConvHiraKata;
		m_nFileUnifyKata = dlgUnify.m_nConvKata;
		m_nFileUnifyKigou = dlgUnify.m_nConvKigou;
		m_nFileUnifySuji = dlgUnify.m_nConvSuji;
		m_nFileUnifyUpLow = dlgUnify.m_nConvUpLow;
	}
}
