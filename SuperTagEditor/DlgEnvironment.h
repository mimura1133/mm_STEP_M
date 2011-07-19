#if !defined(AFX_DLGENVIRONMENT_H__FE011EA3_EADB_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DLGENVIRONMENT_H__FE011EA3_EADB_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvironment.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvironment ダイアログ

class CDlgEnvironment : public COptionPage
{
	DECLARE_DYNCREATE(CDlgEnvironment)

// コンストラクション
public:
	CDlgEnvironment();
	~CDlgEnvironment();

	void	UpdateStatus(void);

// ダイアログ データ
	//{{AFX_DATA(CDlgEnvironment)
	enum { IDD = IDD_DLG_ENVIRONMENT };
	BOOL	m_bEditOkDown;
	BOOL	m_bEnterBeginEdit;
	BOOL	m_bKeepTimeStamp;
	BOOL	m_bAutoOpenFolder;
	BOOL	m_bLoadFileAdjustColumn;
	BOOL	m_bSyncCreateTime;
	BOOL	m_bLoadFileChecked;
	BOOL	m_bHideMP3ListFile;
	BOOL	m_bESCEditCancel;
	BOOL	m_bEnableEditCursorExit;
	BOOL	m_bDropSearchSubFolder;
	BOOL	m_bShowZenSpace;
	BOOL	m_bSortIgnoreCase;
	BOOL	m_bSortIgnoreZenHan;
	BOOL	m_bShowTotalParent;
	BOOL	m_bSortIgnoreKataHita;
	BOOL	m_bShowTips;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgEnvironment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgEnvironment)
	virtual BOOL OnInitDialog();
	afx_msg void OnChKeepTimeStamp();
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGENVIRONMENT_H__FE011EA3_EADB_11D3_9459_00402641B29B__INCLUDED_)
