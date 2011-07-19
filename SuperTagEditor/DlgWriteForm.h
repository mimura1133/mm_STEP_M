#if !defined(AFX_DLGWRITEFORM_H__F38CAF64_740C_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGWRITEFORM_H__F38CAF64_740C_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWriteForm.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgWriteForm ダイアログ

class CDlgWriteForm : public COptionPage
{
	DECLARE_DYNCREATE(CDlgWriteForm)

// コンストラクション
public:
	CDlgWriteForm();
	~CDlgWriteForm();

// ダイアログ データ
	//{{AFX_DATA(CDlgWriteForm)
	enum { IDD = IDD_DLG_WRITE_FORM };
	CButton	m_btWriteHtml;
	CButton	m_btIsHtml;
	CButton	m_btWriteSelected;
	CEdit	m_editFileName;
	CEdit	m_editExtName;
	CComboBox	m_listFormatType;
	int		m_nFormatType;
	//}}AFX_DATA

	WRITE_FORMAT	m_writeFormat[WRITE_FORMAT_MAX];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgWriteForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgWriteForm)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtNameChange();
	afx_msg void OnBtRefFile();
	afx_msg void OnSelChangeListFormatType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGWRITEFORM_H__F38CAF64_740C_11D4_9459_00402641B29B__INCLUDED_)
