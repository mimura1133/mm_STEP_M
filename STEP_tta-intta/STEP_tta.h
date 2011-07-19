// STEP_tta.h : STEP_TTA アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_TTA_H__CD5253BB_27F2_482F_90F3_B6692867943A__INCLUDED_)
#define AFX_STEP_TTA_H__CD5253BB_27F2_482F_90F3_B6692867943A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_ttaApp
// このクラスの動作の定義に関しては STEP_tta.cpp ファイルを参照してください。
//

class CSTEP_ttaApp : public CWinApp
{
public:
	CSTEP_ttaApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_ttaApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_ttaApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_TTA_H__CD5253BB_27F2_482F_90F3_B6692867943A__INCLUDED_)
