#if !defined(AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConvFormatEx.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx ダイアログ

class CDlgConvFormatEx : public COptionPage
{
	DECLARE_DYNCREATE(CDlgConvFormatEx)

// コンストラクション
public:
	CDlgConvFormatEx();
	~CDlgConvFormatEx();

// ダイアログ データ
	//{{AFX_DATA(CDlgConvFormatEx)
	enum { IDD = IDD_DLG_CONV_FORMAT_EX };
	CButton	m_btSpaceInitNumber;
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nAddNumber;
	int		m_nInitNumber;
	UINT	m_nMinColumn;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_CONV_FORMAT_EX	m_userFormatEx[USER_CONV_FORMAT_EX_MAX];

	int		m_nOutputFormatType;
	void	OutputFormatString(const char *);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgConvFormatEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgConvFormatEx)
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtFileName();
	afx_msg void OnBtFixString();
	afx_msg void OnBtNumber();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtNameChange();
	afx_msg void OnSelChangeListFormatType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtTrackNumber3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_)
