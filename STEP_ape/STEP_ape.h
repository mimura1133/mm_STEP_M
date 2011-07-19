// STEP_ape.h : STEP_APE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_APE_H__09E46255_DC52_47F5_951F_E8DFBA3C14D9__INCLUDED_)
#define AFX_STEP_APE_H__09E46255_DC52_47F5_951F_E8DFBA3C14D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_apeApp
// このクラスの動作の定義に関しては STEP_ape.cpp ファイルを参照してください。
//

class CSTEP_apeApp : public CWinApp
{
public:
	CSTEP_apeApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_apeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_apeApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_APE_H__09E46255_DC52_47F5_951F_E8DFBA3C14D9__INCLUDED_)
