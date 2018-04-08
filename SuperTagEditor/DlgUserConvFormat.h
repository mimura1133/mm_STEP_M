#if !defined(AFX_DLGUSERCONVFORMAT_H__2A199EC2_0F41_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGUSERCONVFORMAT_H__2A199EC2_0F41_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserConvFormat.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormat ダイアログ

class CDlgUserConvFormat : public COptionPage
{
	DECLARE_DYNCREATE(CDlgUserConvFormat)

// コンストラクション
public:
	CDlgUserConvFormat();
	~CDlgUserConvFormat();


// ダイアログ データ
	//{{AFX_DATA(CDlgUserConvFormat)
	enum { IDD = IDD_DLG_USER_CONV_FORMAT };
	CComboBox	m_listFormatType;
	CEdit	m_editTag2File;
	CEdit	m_editFile2Tag;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_CONV_FORMAT	m_userFormat[USER_CONV_FORMAT_MAX];

	int		m_nOutputFormatType;
	void	OutputFormatString(LPCTSTR);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgUserConvFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgUserConvFormat)
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtNameChange();
	afx_msg void OnSetfocusEdFormatFile2tag();
	afx_msg void OnSetfocusEdFormatTag2file();
	afx_msg void OnSelChangeListFormatType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtTrackNumber3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGUSERCONVFORMAT_H__2A199EC2_0F41_11D4_9459_00402641B29B__INCLUDED_)
