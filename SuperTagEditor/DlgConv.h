#if !defined(AFX_DlgConv_H__205D5D6E_8399_4AF8_8224_4F1BD0A912EC__INCLUDED_)
#define AFX_DlgConv_H__205D5D6E_8399_4AF8_8224_4F1BD0A912EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgConv.h : ヘッダー ファイル
//

#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConv ダイアログ

class CDlgConv : public COptionPage
{
	DECLARE_DYNCREATE(CDlgConv)

// コンストラクション
public:
	CDlgConv();
	~CDlgConv();

// ダイアログ データ
	//{{AFX_DATA(CDlgConv)
	enum { IDD = IDD_DIALOG_CONV };
	CEdit	m_cFirstUpperSentenceSeparator;
	CEdit	m_cFirstUperIgnoreWords;
	BOOL	m_bFirstUpperIgnoreWord;
	CString	m_strFirstUpperIgnoreWords;
	CString	m_strFirstUpperSentenceSeparator;
	BOOL	m_bUserConvAddMenu;
	BOOL	m_bZenHanKigouKana;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgConv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgConv)
	afx_msg void OnCheckFirstUpperIgnoreWord();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CDlgConv_H__205D5D6E_8399_4AF8_8224_4F1BD0A912EC__INCLUDED_)
