// STEP_vqf.h : STEP_VQF アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_VQF_H__25415629_D6A6_4EE7_BCD6_41F3D1F31363__INCLUDED_)
#define AFX_STEP_VQF_H__25415629_D6A6_4EE7_BCD6_41F3D1F31363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_vqfApp
// このクラスの動作の定義に関しては STEP_vqf.cpp ファイルを参照してください。
//

class CSTEP_vqfApp : public CWinApp
{
public:
	CSTEP_vqfApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_vqfApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_vqfApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_VQF_H__25415629_D6A6_4EE7_BCD6_41F3D1F31363__INCLUDED_)
