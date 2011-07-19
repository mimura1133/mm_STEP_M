#if !defined(AFX_DLGLOADPLAYLIST_H__25201AA0_10AB_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGLOADPLAYLIST_H__25201AA0_10AB_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoadPlayList.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadPlayList ダイアログ

class CDlgLoadPlayList : public CDialog
{
// コンストラクション
public:
	CDlgLoadPlayList(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgLoadPlayList)
	enum { IDD = IDD_DLG_LOAD_PLAYLIST };
	BOOL	m_bClearCheck;
	BOOL	m_bClearList;
	int		m_nAddListFlag;
	int		m_nFileCheckFlag;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgLoadPlayList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgLoadPlayList)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGLOADPLAYLIST_H__25201AA0_10AB_11D4_9459_00402641B29B__INCLUDED_)
