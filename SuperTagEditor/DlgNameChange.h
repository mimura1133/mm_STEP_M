#if !defined(AFX_DLGNAMECHANGE_H__F6C936A0_0F4C_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGNAMECHANGE_H__F6C936A0_0F4C_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNameChange.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNameChange ダイアログ

class CDlgNameChange : public CDialog
{
// コンストラクション
public:
	CDlgNameChange(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgNameChange)
	enum { IDD = IDD_DLG_NAME_CHANGE };
	CEdit	m_editName;
	CString	m_strName;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgNameChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgNameChange)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGNAMECHANGE_H__F6C936A0_0F4C_11D4_9459_00402641B29B__INCLUDED_)
