#if !defined(AFX_DLGREPLACEFILENAME_H__98327800_1941_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGREPLACEFILENAME_H__98327800_1941_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReplaceFileName.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgReplaceFileName ダイアログ

class CDlgReplaceFileName : public COptionPage
{
	DECLARE_DYNCREATE(CDlgReplaceFileName)

// コンストラクション
public:
	CDlgReplaceFileName();
	~CDlgReplaceFileName();

// ダイアログ データ
	//{{AFX_DATA(CDlgReplaceFileName)
	enum { IDD = IDD_DLG_REPLACE_FILENAME };
	CString	m_strBefore[FILENAME_REPLACE_MAX];
	CString	m_strAfter[FILENAME_REPLACE_MAX];
	CString	m_strUserBefore[USER_FILENAME_REPLACE_MAX];
	CString	m_strUserAfter[USER_FILENAME_REPLACE_MAX];
	int	 m_nExtChange;
	BOOL	m_bFileNameReplace;
	//}}AFX_DATA

	UINT m_nFileUnifyAlpha;
	UINT m_nFileUnifyHiraKata;
	UINT m_nFileUnifyKata;
	UINT m_nFileUnifyKigou;
	UINT m_nFileUnifySuji;
	UINT m_nFileUnifyUpLow;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgReplaceFileName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgReplaceFileName)
	virtual BOOL OnInitDialog();
	afx_msg void OnChFilenameMultiByte();
	afx_msg void OnBtFileNameUnify();
	afx_msg void OnBtResetPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGREPLACEFILENAME_H__98327800_1941_11D4_9459_00402641B29B__INCLUDED_)
