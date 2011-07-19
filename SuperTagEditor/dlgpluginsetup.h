#if !defined(AFX_CDLGPLUGINSETUP_H__45B0172E_AA3B_4139_97BC_FF5564E3C887__INCLUDED_)
#define AFX_CDLGPLUGINSETUP_H__45B0172E_AA3B_4139_97BC_FF5564E3C887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgPluginSetup.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPluginSetup ダイアログ

class CDlgPluginSetup : public CDialog
{
// コンストラクション
public:
	CDlgPluginSetup(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgPluginSetup)
	enum { IDD = IDD_DLG_SETUP_PLUGIN };
	CListCtrl	m_listPlugin;
	CString	m_strPluginInfo;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgPluginSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPluginSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedListPlugin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtSetup();
	afx_msg void OnBtInstall();
	afx_msg void OnBtUp();
	afx_msg void OnBtDown();
	virtual void OnOK();
	afx_msg void OnBtUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CDLGPLUGINSETUP_H__45B0172E_AA3B_4139_97BC_FF5564E3C887__INCLUDED_)
