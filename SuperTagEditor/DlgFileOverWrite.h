#if !defined(AFX_DLGFILEOVERWRITE_H__AF8B4A60_059E_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGFILEOVERWRITE_H__AF8B4A60_059E_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileOverWrite.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileOverWrite ダイアログ

class CDlgFileOverWrite : public CDialog
{
// コンストラクション
public:
	CDlgFileOverWrite(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgFileOverWrite)
	enum { IDD = IDD_DLG_FILE_OVERWRITE };
	CString	m_strFileName;
	CString	m_strTimeStamp;
	CString	m_strSize;
	//}}AFX_DATA

	int		m_nResult;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFileOverWrite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFileOverWrite)
	virtual void OnOK();
	afx_msg void OnBtOverwrite();
	afx_msg void OnBtAllOverwrite();
	afx_msg void OnBtSkip();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILEOVERWRITE_H__AF8B4A60_059E_11D4_9459_00402641B29B__INCLUDED_)
