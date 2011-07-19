#if !defined(AFX_DLGENVPLAYER_H__893320E2_73F8_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGENVPLAYER_H__893320E2_73F8_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvPlayer.h : ヘッダー ファイル
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvPlayer ダイアログ

class CDlgEnvPlayer : public COptionPage
{
	DECLARE_DYNCREATE(CDlgEnvPlayer)

// コンストラクション
public:
	CDlgEnvPlayer();
	~CDlgEnvPlayer();

// ダイアログ データ
	//{{AFX_DATA(CDlgEnvPlayer)
	enum { IDD = IDD_DLG_ENV_PLAYER };
	int		m_nPlayerType;
	CString	m_strWinAmpPath;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgEnvPlayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgEnvPlayer)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtRefWinamp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGENVPLAYER_H__893320E2_73F8_11D4_9459_00402641B29B__INCLUDED_)
