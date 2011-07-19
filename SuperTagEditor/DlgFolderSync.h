#if !defined(AFX_DLGFOLDERSYNC_H__6BDA3360_3651_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGFOLDERSYNC_H__6BDA3360_3651_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFolderSync.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFolderSync ダイアログ

class CDlgFolderSync : public COptionPage
{
	DECLARE_DYNCREATE(CDlgFolderSync)

// コンストラクション
public:
	CDlgFolderSync();
	~CDlgFolderSync();

// ダイアログ データ
	//{{AFX_DATA(CDlgFolderSync)
	enum { IDD = IDD_DLG_FOLDER_SYNC };
	CEdit	m_editRootFolder;
	BOOL	m_bEnableFolderSync;
	CString	m_strRootFolder;
	BOOL	m_bMoveLyricsFile;
	BOOL	m_bDeleteFolder;
	BOOL	m_bSelectAlways;
	//}}AFX_DATA

	void	UpdateStatus(void);


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgFolderSync)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFolderSync)
	virtual BOOL OnInitDialog();
	afx_msg void OnChEnableFolderSync();
	afx_msg void OnBtRefRoot();
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFOLDERSYNC_H__6BDA3360_3651_11D4_9459_00402641B29B__INCLUDED_)
