// STEP_mpc.h : STEP_MPC アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_MPC_H__945A76CB_C015_4FBE_BEBF_E68D45ABD57E__INCLUDED_)
#define AFX_STEP_MPC_H__945A76CB_C015_4FBE_BEBF_E68D45ABD57E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mpcApp
// このクラスの動作の定義に関しては STEP_mpc.cpp ファイルを参照してください。
//

class CSTEP_mpcApp : public CWinApp
{
public:
	CSTEP_mpcApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_mpcApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_mpcApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_MPC_H__945A76CB_C015_4FBE_BEBF_E68D45ABD57E__INCLUDED_)
