#if !defined(AFX_DLGFIXEDSTRING_H__242A5F81_213F_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGFIXEDSTRING_H__242A5F81_213F_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFixedString.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedString ダイアログ

class CDlgFixedString : public CDialog
{
// コンストラクション
public:
	CDlgFixedString(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgFixedString)
	enum { IDD = IDD_DLG_FIXED_STRING };
	CString	m_strText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFixedString)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFixedString)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFIXEDSTRING_H__242A5F81_213F_11D4_9459_00402641B29B__INCLUDED_)
