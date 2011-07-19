#if !defined(AFX_DLGADDUSERGENRE_H__C4EF4D09_D12B_46A5_9B88_2A819FF6BA40__INCLUDED_)
#define AFX_DLGADDUSERGENRE_H__C4EF4D09_D12B_46A5_9B88_2A819FF6BA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddUserGenre.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUserGenre ダイアログ

class CDlgAddUserGenre : public CDialog
{
// コンストラクション
public:
	CDlgAddUserGenre(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgAddUserGenre)
	enum { IDD = IDD_DLG_SETUP_USER_GENRE };
	CButton	m_cBtAdd;
	CListCtrl	m_listGenre;
	CString	m_strName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgAddUserGenre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgAddUserGenre)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtAdd();
	afx_msg void OnItemchangedListGenre(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditName();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	USER_GENRE_LIST*	m_genreListUSER;

private:
	void EnableButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGADDUSERGENRE_H__C4EF4D09_D12B_46A5_9B88_2A819FF6BA40__INCLUDED_)
