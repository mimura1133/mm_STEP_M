#if !defined(AFX_DLGBLANKADVANCE_H__1D5F7C64_5549_4A48_B632_BEDF3B74F01F__INCLUDED_)
#define AFX_DLGBLANKADVANCE_H__1D5F7C64_5549_4A48_B632_BEDF3B74F01F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlankAdvance.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBlankAdvance ダイアログ

class CDlgBlankAdvance : public COptionPage
{
	DECLARE_DYNCREATE(CDlgBlankAdvance)

// コンストラクション
public:
	CDlgBlankAdvance();
	~CDlgBlankAdvance();

// ダイアログ データ
	//{{AFX_DATA(CDlgBlankAdvance)
	enum { IDD = IDD_DIALOG_BLANK_ADVANCE };
		// メモ - ClassWizard はこの位置にデータ メンバを追加します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgBlankAdvance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgBlankAdvance)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGBLANKADVANCE_H__1D5F7C64_5549_4A48_B632_BEDF3B74F01F__INCLUDED_)
