// STEP_fla.h : STEP_FLA アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_FLA_H__059AF0C3_4444_4A5B_BE23_2FC0A1C8D403__INCLUDED_)
#define AFX_STEP_FLA_H__059AF0C3_4444_4A5B_BE23_2FC0A1C8D403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_flaApp
// このクラスの動作の定義に関しては STEP_fla.cpp ファイルを参照してください。
//

class CSTEP_flaApp : public CWinApp
{
public:
	CSTEP_flaApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_flaApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_flaApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_FLA_H__059AF0C3_4444_4A5B_BE23_2FC0A1C8D403__INCLUDED_)
