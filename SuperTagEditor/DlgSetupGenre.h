#if !defined(AFX_DLGSETUPGENRE_H__F618C220_2DE7_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGSETUPGENRE_H__F618C220_2DE7_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupGenre.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGenre ダイアログ


class CDlgSetupGenre : public COptionPage
{
	DECLARE_DYNCREATE(CDlgSetupGenre)

// コンストラクション
public:
	CDlgSetupGenre();
	~CDlgSetupGenre();

// ダイアログ データ
	//{{AFX_DATA(CDlgSetupGenre)
	enum { IDD = IDD_DLG_SETUP_GENRE };
	CListCtrl	m_listGenre;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgSetupGenre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgSetupGenre)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtCheckAll();
	afx_msg void OnBtUncheckAll();
	afx_msg void OnBtCheckId3();
	afx_msg void OnBtCheckWinamp();
	afx_msg void OnBtCheckScmpx();
	afx_msg void OnBtResetPage();
	afx_msg void OnBtAddUser();
	afx_msg void OnBtDelUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddUserGenre(int nIndex);
public:
	USER_GENRE_LIST*	m_genreListUSER;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSETUPGENRE_H__F618C220_2DE7_11D4_9459_00402641B29B__INCLUDED_)
