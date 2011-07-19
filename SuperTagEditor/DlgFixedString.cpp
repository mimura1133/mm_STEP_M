// DlgFixedString.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFixedString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedString ダイアログ


CDlgFixedString::CDlgFixedString(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFixedString::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFixedString)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFixedString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFixedString)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFixedString, CDialog)
	//{{AFX_MSG_MAP(CDlgFixedString)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedString メッセージ ハンドラ
