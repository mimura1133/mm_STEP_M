// DlgNameChange.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNameChange ダイアログ


CDlgNameChange::CDlgNameChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNameChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNameChange)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgNameChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNameChange)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNameChange, CDialog)
	//{{AFX_MSG_MAP(CDlgNameChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNameChange メッセージ ハンドラ

BOOL CDlgNameChange::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 選択状態にする
	m_editName.SetSel(0, -1);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgNameChange::OnOK() 
{
	CDialog::OnOK();

	if (m_strName.IsEmpty() == TRUE) {
		m_strName = _T("名称未設定");
	}
}
