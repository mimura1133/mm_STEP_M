#if !defined(AFX_DLGFILENAMECHANGE_H__F4B855B0_E32B_4803_A6B4_02F39BCFB362__INCLUDED_)
#define AFX_DLGFILENAMECHANGE_H__F4B855B0_E32B_4803_A6B4_02F39BCFB362__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileNameChange.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameChange ダイアログ

class CDlgFileNameChange : public CDialog
{
public:
	CString m_strMsgFormat;
// コンストラクション
public:
	CDlgFileNameChange(CWnd* pParent = NULL);   // 標準のコンストラクタ

	UINT m_nMaxChar;
// ダイアログ データ
	//{{AFX_DATA(CDlgFileNameChange)
	enum { IDD = IDD_DLG_FILENAME_CHANGE };
	CStatic	m_staticMsg;
	CEdit	m_editName;
	CString	m_strFileName;
	CString	m_strMsg;
	CString	m_strOrgFileName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFileNameChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFileNameChange)
	afx_msg void OnChangeEditName();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILENAMECHANGE_H__F4B855B0_E32B_4803_A6B4_02F39BCFB362__INCLUDED_)
