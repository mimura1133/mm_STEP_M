#if !defined(AFX_DLGDEFAULTVALUE_H__1F3F9F21_D999_4375_814B_07A16F70BB22__INCLUDED_)
#define AFX_DLGDEFAULTVALUE_H__1F3F9F21_D999_4375_814B_07A16F70BB22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefaultValue.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultValue ダイアログ

class CDlgDefaultValue : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgDefaultValue)

// コンストラクション
public:
	CDlgDefaultValue();
	~CDlgDefaultValue();

// ダイアログ データ
	//{{AFX_DATA(CDlgDefaultValue)
	enum { IDD = IDD_DLG_DEFAUT_VALUE };
	CComboBox	m_cbId3v2EncodeNew;
	CComboBox	m_cbId3v2VersionNew;
	CComboBox	m_cbId3v2Encode;
	CComboBox	m_cbId3v2Version;
	CString	m_strSoftwareTag;
	BOOL	m_bID3v2UnSync;
	BOOL	m_bID3v2UnSyncNew;
	int		m_nId3v2Encode;
	int		m_nId3v2Version;
	CString	m_strInfo;
	int		m_nId3v2VersionNew;
	int		m_nId3v2EncodeNew;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgDefaultValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgDefaultValue)
	afx_msg void OnBtResetPage();
	afx_msg void OnCheckId3v2Unicode();
	afx_msg void OnCheckId3v2OldComment();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboId3v2Version();
	afx_msg void OnSelchangeComboId3v2VersionNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGDEFAULTVALUE_H__1F3F9F21_D999_4375_814B_07A16F70BB22__INCLUDED_)
