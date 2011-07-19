#if !defined(AFX_DLGEDITSHOWCOLUMN_H__D7E15A02_13B7_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGEDITSHOWCOLUMN_H__D7E15A02_13B7_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditShowColumn.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEditShowColumn ダイアログ

class CDlgEditShowColumn : public COptionPage
{
	DECLARE_DYNCREATE(CDlgEditShowColumn)

// コンストラクション
public:
	CDlgEditShowColumn();
	~CDlgEditShowColumn();

// ダイアログ データ
	//{{AFX_DATA(CDlgEditShowColumn)
	enum { IDD = IDD_DLG_EDIT_SHOW_COLUMN };
	CListCtrl	m_listColumn;
	//}}AFX_DATA


	void	SwapListItem(int, int);
	void	SelChangeList(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgEditShowColumn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgEditShowColumn)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtDown();
	afx_msg void OnBtUp();
	virtual void OnOK();
	afx_msg void OnBtResetPage();
	afx_msg void OnChangeEditMaxWidth();
	afx_msg void OnItemChangedListColumn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangingListColumn(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGEDITSHOWCOLUMN_H__D7E15A02_13B7_11D4_9459_00402641B29B__INCLUDED_)
