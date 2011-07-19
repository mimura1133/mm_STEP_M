#if !defined(AFX_DLGADDNUMBER_H__44D702A8_6504_45B4_AE28_A8512B372D7D__INCLUDED_)
#define AFX_DLGADDNUMBER_H__44D702A8_6504_45B4_AE28_A8512B372D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddNumber.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// DlgAddNumber ダイアログ

class DlgAddNumber : public CDialog
{
// コンストラクション
public:
	DlgAddNumber(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(DlgAddNumber)
	enum { IDD = IDD_DLG_ADD_NUMBER };
	UINT	m_nWidth;
	CString	m_strSeparator;
	int		m_nAddPosition;
	BOOL	m_bSpaceInitNumber;
	UINT	m_nAddNumber;
	UINT	m_nInitNumber;
	CString	m_strAfter;
	CString	m_strBefore;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(DlgAddNumber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(DlgAddNumber)
	afx_msg void OnRadioReplace();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGADDNUMBER_H__44D702A8_6504_45B4_AE28_A8512B372D7D__INCLUDED_)
