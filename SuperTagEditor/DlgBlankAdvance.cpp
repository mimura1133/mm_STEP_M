// DlgBlankAdvance.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgBlankAdvance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlankAdvance プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgBlankAdvance, COptionPage)

CDlgBlankAdvance::CDlgBlankAdvance() : COptionPage(CDlgBlankAdvance::IDD)
{
	//{{AFX_DATA_INIT(CDlgBlankAdvance)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

CDlgBlankAdvance::~CDlgBlankAdvance()
{
}

void CDlgBlankAdvance::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlankAdvance)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlankAdvance, COptionPage)
	//{{AFX_MSG_MAP(CDlgBlankAdvance)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlankAdvance メッセージ ハンドラ
