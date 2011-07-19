#if !defined(AFX_DLGFILEOVERWRITE2_H__9D17F0DE_039D_4CFE_9106_52E983245F50__INCLUDED_)
#define AFX_DLGFILEOVERWRITE2_H__9D17F0DE_039D_4CFE_9106_52E983245F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileOverWrite2.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite22 ダイアログ

class CDlgFileOverWrite2 : public CDialog
{
// コンストラクション
public:
	CDlgFileOverWrite2(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgFileOverWrite2)
	enum { IDD = IDD_DLG_FILE_OVERWRITE2 };
	CString	m_strFileName;
	CString	m_strTimeStamp;
	CString	m_strSize;
	//}}AFX_DATA

	int		m_nResult;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFileOverWrite2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFileOverWrite2)
	virtual void OnOK();
	afx_msg void OnBtOverwrite();
	afx_msg void OnBtAllOverwrite();
	afx_msg void OnBtRename();
	afx_msg void OnBtSkip();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILEOVERWRITE2_H__9D17F0DE_039D_4CFE_9106_52E983245F50__INCLUDED_)
