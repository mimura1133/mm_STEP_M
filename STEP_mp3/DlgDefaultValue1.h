#if !defined(AFX_DLGDEFAULTVALUE1_H__DDCBFFF9_307E_4029_81DA_4F28BE3094BA__INCLUDED_)
#define AFX_DLGDEFAULTVALUE1_H__DDCBFFF9_307E_4029_81DA_4F28BE3094BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefaultValue1.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// DlgDefaultValue ダイアログ

class DlgDefaultValue : public CDialog
{
// コンストラクション
public:
	DlgDefaultValue(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(DlgDefaultValue)
	enum { IDD = IDD_DLG_DEFAUT_VALUE };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(DlgDefaultValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(DlgDefaultValue)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGDEFAULTVALUE1_H__DDCBFFF9_307E_4029_81DA_4F28BE3094BA__INCLUDED_)
