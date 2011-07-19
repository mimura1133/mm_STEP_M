// STEP_riff.h : STEP_RIFF アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_RIFF_H__F23777D0_EA75_4557_9825_FCDF3FCA17CE__INCLUDED_)
#define AFX_STEP_RIFF_H__F23777D0_EA75_4557_9825_FCDF3FCA17CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_riffApp
// このクラスの動作の定義に関しては STEP_riff.cpp ファイルを参照してください。
//

class CSTEP_riffApp : public CWinApp
{
public:
	CSTEP_riffApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_riffApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_riffApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_RIFF_H__F23777D0_EA75_4557_9825_FCDF3FCA17CE__INCLUDED_)
