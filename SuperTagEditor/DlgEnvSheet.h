#if !defined(AFX_DLGDEFINESHEET_H__2C765F2A_BD1E_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DLGDEFINESHEET_H__2C765F2A_BD1E_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvSheet.h : ヘッダー ファイル
//

#include "OptionPage/OptionSheet.h"

class CSuperTagEditorDoc;

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSheet

class CDlgEnvSheet : public COptionSheet/*CPropertySheet*/
{
	DECLARE_DYNAMIC(CDlgEnvSheet)

// コンストラクション
public:
	CDlgEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// アトリビュート
public:
	CSuperTagEditorDoc	*m_pDoc;

// オペレーション
public:
	enum	{ENV_NORMAL = 1, ENV_CONV = 2, ENV_ADVANCED = 4, ENV_ALL = 7};
	int		DoModal(int);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgEnvSheet)
	public:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDlgEnvSheet();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDlgEnvSheet)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGDEFINESHEET_H__2C765F2A_BD1E_11D3_9459_00402641B29B__INCLUDED_)
