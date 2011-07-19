#if !defined(AFX_DLGFILENAMEMAXCHECK_H__28A60472_8836_4BED_AF9A_D1A1BFCE90DC__INCLUDED_)
#define AFX_DLGFILENAMEMAXCHECK_H__28A60472_8836_4BED_AF9A_D1A1BFCE90DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileNameMaxCheck.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameMaxCheck ダイアログ

class CDlgFileNameMaxCheck : public COptionPage
{
// コンストラクション
public:
	CDlgFileNameMaxCheck(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgFileNameMaxCheck)
	enum { IDD = IDD_DLG_FILENAME_MAX_CHECK };
	CButton	m_checkFileNameMaxCellColor;
	BOOL	m_bFileNameMaxCheck;
	UINT	m_nFileNameMaxChar;
	BOOL	m_bFileNameMaxCellColor;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFileNameMaxCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFileNameMaxCheck)
	afx_msg void OnChFilenameMaxCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILENAMEMAXCHECK_H__28A60472_8836_4BED_AF9A_D1A1BFCE90DC__INCLUDED_)
