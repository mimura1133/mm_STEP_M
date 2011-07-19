// DlgWriteForm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"
#include "DlgWriteForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWriteForm プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgWriteForm, COptionPage)

CDlgWriteForm::CDlgWriteForm() : COptionPage(CDlgWriteForm::IDD)
{
	//{{AFX_DATA_INIT(CDlgWriteForm)
	m_nFormatType = 0;
	//}}AFX_DATA_INIT
}

CDlgWriteForm::~CDlgWriteForm()
{
}

void CDlgWriteForm::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWriteForm)
	DDX_Control(pDX, IDC_CH_WRITE_HTML, m_btWriteHtml);
	DDX_Control(pDX, IDC_CH_IS_HTML, m_btIsHtml);
	DDX_Control(pDX, IDC_CH_WRITE_SELECTED, m_btWriteSelected);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Control(pDX, IDC_EDIT_EXT_NAME, m_editExtName);
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWriteForm, COptionPage)
	//{{AFX_MSG_MAP(CDlgWriteForm)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_REF_FILE, OnBtRefFile)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWriteForm メッセージ ハンドラ

BOOL CDlgWriteForm::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < WRITE_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format("%d: %s", i+1, m_writeFormat[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFileName.SetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.SetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_btWriteSelected.SetCheck(m_writeFormat[m_nFormatType].bWriteSelected ? TRUE : FALSE);
	m_btIsHtml.SetCheck(m_writeFormat[m_nFormatType].bIsHtml ? TRUE : FALSE);
	m_btWriteHtml.SetCheck(m_writeFormat[m_nFormatType].bWriteHtml ? TRUE : FALSE); /* BeachMonster5 120 */

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgWriteForm::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_writeFormat[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// 新しい名称に変更
		m_writeFormat[m_nFormatType].strName = dialog.m_strName;

		// リスト更新
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < WRITE_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format("%d: %s", i+1, m_writeFormat[i].strName);
			m_listFormatType.AddString(strName);
		}
		// 選択しなおす
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

void CDlgWriteForm::OnBtRefFile() 
{
	CString	strFileName;
	m_editFileName.GetWindowText(strFileName);

	// ファイル選択ダイアログを開く
	static	LPCSTR	sFileFilter =	"書式ファイル(*.txt)|*.txt|" \
									"All Files(*.*)|*.*|";
	CFileDialog		dialog(TRUE, ".txt", strFileName,
	                       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXTENSIONDIFFERENT | OFN_PATHMUSTEXIST,
	                       sFileFilter, NULL);
	if (dialog.DoModal() == IDOK) {
		m_editFileName.SetWindowText(dialog.GetPathName());
	}
}

void CDlgWriteForm::OnSelChangeListFormatType() 
{
	// 現在編集中の内容を反映させる
	m_editFileName.GetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.GetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_writeFormat[m_nFormatType].bWriteSelected	= m_btWriteSelected.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bIsHtml		= m_btIsHtml.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bWriteHtml		= m_btWriteHtml.GetCheck() ? true : false;

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFileName.SetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.SetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_btWriteSelected.SetCheck(m_writeFormat[m_nFormatType].bWriteSelected ? TRUE : FALSE);
	m_btIsHtml.SetCheck(m_writeFormat[m_nFormatType].bIsHtml ? TRUE : FALSE);
	m_btWriteHtml.SetCheck(m_writeFormat[m_nFormatType].bWriteHtml ? TRUE : FALSE);
}

void CDlgWriteForm::OnOK() 
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFileName.GetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.GetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_writeFormat[m_nFormatType].bWriteSelected	= m_btWriteSelected.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bIsHtml		= m_btIsHtml.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bWriteHtml		= m_btWriteHtml.GetCheck() ? true : false;

	COptionPage::OnOK();
}
