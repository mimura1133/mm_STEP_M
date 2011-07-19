// STEP_mp3.h : STEP_MP3 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_MP3_H__F4707E45_2B14_44B2_8276_EFAFAFC465EE__INCLUDED_)
#define AFX_STEP_MP3_H__F4707E45_2B14_44B2_8276_EFAFAFC465EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App
// このクラスの動作の定義に関しては STEP_mp3.cpp ファイルを参照してください。
//

class CSTEP_mp3App : public CWinApp
{
public:
	CSTEP_mp3App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTEP_mp3App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTEP_mp3App)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#define TYPE_IS_MP3(nFormat) (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11 || nFormat == nFileTypeID3V2)
#define TYPE_IS_MP3V1(nFormat) (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)
#define TYPE_IS_MP3V2(nFormat) (nFormat == nFileTypeID3V2)
#define TYPE_IS_RMP(nFormat) (nFormat == nFileTypeRMP)
#define	TYPE_IS_SUPPORT(nFormat) (TYPE_IS_MP3(nFormat) || TYPE_IS_RMP(nFormat))

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_MP3_H__F4707E45_2B14_44B2_8276_EFAFAFC465EE__INCLUDED_)
