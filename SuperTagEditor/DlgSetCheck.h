#if !defined(AFX_DLGSETCHECK_H__2541D100_113B_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGSETCHECK_H__2541D100_113B_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetCheck.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCheck ダイアログ

class CDlgSetCheck : public CDialog
{
// コンストラクション
public:
	CDlgSetCheck(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgSetCheck)
	enum { IDD = IDD_DLG_CHECK_WORD };
	CButton	m_btMatchSelected;
	CButton	m_btRegExp;
	CButton	m_btRangeSelected;
	CButton	m_btMatchComplete;
	CButton	m_btCheckDiffUL;
	CComboBox	m_listTargetColumn;
	BOOL	m_bCheckDiffUL;
	BOOL	m_bMatchComplete;
	BOOL	m_bRangeSelected;
	BOOL	m_bRegExp;
	CString	m_strSearchWord;
	int		m_nTargetColumn;
	BOOL	m_bMatchSelected;
	//}}AFX_DATA

	BOOL	m_bEnableRangeSelected;
	void	UpdateStatus(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgSetCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgSetCheck)
	virtual BOOL OnInitDialog();
	afx_msg void OnChRegexp();
	afx_msg void OnSelChangeListTarget();
	afx_msg void OnChRangeSelected();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSETCHECK_H__2541D100_113B_11D4_9459_00402641B29B__INCLUDED_)
