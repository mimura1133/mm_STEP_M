// STEP_ogg.h : STEP_OGG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_OGG_H__B332FF76_A8C6_4148_9A68_232EE14E172E__INCLUDED_)
#define AFX_STEP_OGG_H__B332FF76_A8C6_4148_9A68_232EE14E172E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_oggApp
// このクラスの動作の定義に関しては STEP_ogg.cpp ファイルを参照してください。
//

class CSTEP_oggApp : public CWinApp
{
public:
	CSTEP_oggApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_oggApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_oggApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_OGG_H__B332FF76_A8C6_4148_9A68_232EE14E172E__INCLUDED_)
