#if !defined(AFX_DLGSETUP_H__D0FD9CBD_1742_43E6_BBF3_A9A54F183867__INCLUDED_)
#define AFX_DLGSETUP_H__D0FD9CBD_1742_43E6_BBF3_A9A54F183867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup ダイアログ

class CDlgSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgSetup)

// コンストラクション
public:
	CDlgSetup();
	~CDlgSetup();

// ダイアログ データ
	//{{AFX_DATA(CDlgSetup)
	enum { IDD = IDD_DLG_SETUP };
	BOOL	m_bGenreListSelect;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgSetup)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSETUP_H__D0FD9CBD_1742_43E6_BBF3_A9A54F183867__INCLUDED_)
