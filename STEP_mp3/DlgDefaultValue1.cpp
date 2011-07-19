// DlgDefaultValue1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "STEP_mp3.h"
#include "DlgDefaultValue1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDefaultValue ダイアログ


DlgDefaultValue::DlgDefaultValue(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDefaultValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDefaultValue)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void DlgDefaultValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDefaultValue)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDefaultValue, CDialog)
	//{{AFX_MSG_MAP(DlgDefaultValue)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDefaultValue メッセージ ハンドラ
