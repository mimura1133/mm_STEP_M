// STEP_reConv.h : STEP_RECONV アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_RECONV_H__C661F9A7_109B_47E1_948B_4814CA6C8D16__INCLUDED_)
#define AFX_STEP_RECONV_H__C661F9A7_109B_47E1_948B_4814CA6C8D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_reConvApp
// このクラスの動作の定義に関しては STEP_reConv.cpp ファイルを参照してください。
//

class CSTEP_reConvApp : public CWinApp
{
public:
	CSTEP_reConvApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_reConvApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_reConvApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_RECONV_H__C661F9A7_109B_47E1_948B_4814CA6C8D16__INCLUDED_)
