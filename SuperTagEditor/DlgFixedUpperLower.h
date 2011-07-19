#if !defined(AFX_DLGFIXEDUPPERLOWER_H__78AD705E_543C_4335_865A_1551CDDE8E8B__INCLUDED_)
#define AFX_DLGFIXEDUPPERLOWER_H__78AD705E_543C_4335_865A_1551CDDE8E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFixedUpperLower.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedUpperLower ダイアログ

class CDlgFixedUpperLower : public COptionPage
{
	DECLARE_DYNCREATE(CDlgFixedUpperLower)

// コンストラクション
public:
	void EnableButton();
	CDlgFixedUpperLower();
	~CDlgFixedUpperLower();

// ダイアログ データ
	//{{AFX_DATA(CDlgFixedUpperLower)
	enum { IDD = IDD_DLG_FIXED_UPPER_LOWER };
	CEdit	m_editFixedWord;
	CButton	m_buttonDelete;
	CButton	m_buttonAdd;
	CListCtrl	m_listUpperLower;
	CString	m_strFixedWord;
	//}}AFX_DATA

	CStringArray m_arFixedWords;
	BOOL m_bModify;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgFixedUpperLower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFixedUpperLower)
	afx_msg void OnBtAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFixedWord();
	afx_msg void OnBtDelete();
	afx_msg void OnItemchangedListUpperLower(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFIXEDUPPERLOWER_H__78AD705E_543C_4335_865A_1551CDDE8E8B__INCLUDED_)
