#if !defined(AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_)
#define AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgMoveFolder.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder ダイアログ

class CDlgMoveFolder : public COptionPage
{
// コンストラクション
public:
	void OnOK();
	CDlgMoveFolder(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgMoveFolder)
	enum { IDD = IDD_DLG_MOVE_FOLDER };
	CEdit	m_editInitPath;
	CButton	m_chkCopy;
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_MOVE_FODLER_FORMAT	m_userFormatEx[USER_MOVE_FODLER_FORMAT_MAX];


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgMoveFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgMoveFolder)
	afx_msg void OnSelChangeListFormatType();
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtFileName();
	afx_msg void OnBtFixString();
	afx_msg void OnBtNameChange();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtTrackNumber3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtRef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OutputFormatString(const char *sText);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_)
