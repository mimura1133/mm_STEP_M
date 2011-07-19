#if !defined(AFX_DLGTEIKEIPASTE_H__6FE8BF7D_7406_43FB_B10A_E5309B977462__INCLUDED_)
#define AFX_DLGTEIKEIPASTE_H__6FE8BF7D_7406_43FB_B10A_E5309B977462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeikeiPaste.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTeikeiPaste ダイアログ

class CDlgTeikeiPaste : public CDialog
{
// コンストラクション
public:
	CString m_strTitle;
	CDlgTeikeiPaste(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgTeikeiPaste)
	enum { IDD = IDD_DIALOG_TEIKEI_PASTE };
	int		m_nTeikeiPaste;
	BOOL	m_bAddSpace;
	CString	m_strFront;
	CString	m_strBack;
	BOOL	m_bAddChar;
	BOOL	m_bShowDialog;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgTeikeiPaste)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgTeikeiPaste)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGTEIKEIPASTE_H__6FE8BF7D_7406_43FB_B10A_E5309B977462__INCLUDED_)
