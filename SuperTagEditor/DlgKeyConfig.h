#if !defined(AFX_DLGKEYCONFIG_H__37E97340_12D4_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGKEYCONFIG_H__37E97340_12D4_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKeyConfig.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyConfig ダイアログ

class CDlgKeyConfig : public COptionPage
{
	DECLARE_DYNCREATE(CDlgKeyConfig)

// コンストラクション
public:
	CDlgKeyConfig();
	~CDlgKeyConfig();

// ダイアログ データ
	//{{AFX_DATA(CDlgKeyConfig)
	enum { IDD = IDD_DLG_KEY_CONFIG };
	CButton	m_btAlt;
	CButton	m_btCtrl;
	CListBox	m_listKey;
	CComboBox	m_listGroup;
	CListBox	m_listCommand;
	//}}AFX_DATA

	bool	m_bExecCommandChange;
	void	UpdateKeyList(void);
	void	UpdateCommandList(void);
	void	UpdateHotKey(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgKeyConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgKeyConfig)
	afx_msg void OnChCtrl();
	afx_msg void OnChAlt();
	afx_msg void OnSelChangeListCommand();
	afx_msg void OnSelChangeListGroup();
	afx_msg void OnSelChangeListKey();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGKEYCONFIG_H__37E97340_12D4_11D4_9459_00402641B29B__INCLUDED_)
