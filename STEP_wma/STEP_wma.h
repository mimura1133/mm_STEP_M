// STEP_wma.h : STEP_WMA アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_WMA_H__CBBF85F8_10D0_4AAE_BC99_25FE71369444__INCLUDED_)
#define AFX_STEP_WMA_H__CBBF85F8_10D0_4AAE_BC99_25FE71369444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_wmaApp
// このクラスの動作の定義に関しては STEP_wma.cpp ファイルを参照してください。
//

class CSTEP_wmaApp : public CWinApp
{
public:
	CSTEP_wmaApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_wmaApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_wmaApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_WMA_H__CBBF85F8_10D0_4AAE_BC99_25FE71369444__INCLUDED_)
