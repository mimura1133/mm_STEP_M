#if !defined(AFX_DLGENVCONF_H__5ECD9180_13F6_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGENVCONF_H__5ECD9180_13F6_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvConf.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvConf ダイアログ

class CDlgEnvConf : public COptionPage
{
	DECLARE_DYNCREATE(CDlgEnvConf)

// コンストラクション
public:
	CDlgEnvConf();
	~CDlgEnvConf();

// ダイアログ データ
	//{{AFX_DATA(CDlgEnvConf)
	enum { IDD = IDD_DLG_ENV_CONF };
	BOOL	m_bConfDeleteFile;
	BOOL	m_bConfDeleteList;
	BOOL	m_bConfEditModify;
	BOOL	m_bConfFolderSync;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgEnvConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgEnvConf)
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGENVCONF_H__5ECD9180_13F6_11D4_9459_00402641B29B__INCLUDED_)
