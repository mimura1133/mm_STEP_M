// DlgDeleteChar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgDeleteChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDeleteChar ダイアログ


DlgDeleteChar::DlgDeleteChar(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDeleteChar::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDeleteChar)
	m_nDelCount = 0;
	m_nPos = -1;
	//}}AFX_DATA_INIT
}


void DlgDeleteChar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDeleteChar)
	DDX_Text(pDX, IDC_EDIT_DELETE_CHAR, m_nDelCount);
	DDV_MinMaxUInt(pDX, m_nDelCount, 1, 999);
	DDX_Radio(pDX, IDC_RADIO_ADD_START, m_nPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDeleteChar, CDialog)
	//{{AFX_MSG_MAP(DlgDeleteChar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDeleteChar メッセージ ハンドラ
