#if !defined(AFX_DLGLYRICFILE_H__34B29241_448D_4BF4_950D_35DE90673465__INCLUDED_)
#define AFX_DLGLYRICFILE_H__34B29241_448D_4BF4_950D_35DE90673465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLyricFile.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLyricFile ダイアログ

class CDlgLyricFile : public COptionPage
{
	DECLARE_DYNCREATE(CDlgLyricFile)

// コンストラクション
public:
	CDlgLyricFile();
	~CDlgLyricFile();

// ダイアログ データ
	//{{AFX_DATA(CDlgLyricFile)
	enum { IDD = IDD_DLG_LYRIC };
	CButton	m_btSearchLyricsSubDir;
	CEdit	m_editLyricsPath;
	CButton	m_btSetLyricsDir;
	CButton	m_btRefLyrics;
	BOOL	m_bChangeTextFile;
	CString	m_strLyricsPath;
	BOOL	m_bSetLyricsDir;
	BOOL	m_bSearchLyricsSubDir;
	//}}AFX_DATA

	void	UpdateStatus(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgLyricFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgLyricFile)
	afx_msg void OnBtRefLyrics();
	afx_msg void OnChSetLyricsDir();
	afx_msg void OnChChangeTextFilename();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGLYRICFILE_H__34B29241_448D_4BF4_950D_35DE90673465__INCLUDED_)
