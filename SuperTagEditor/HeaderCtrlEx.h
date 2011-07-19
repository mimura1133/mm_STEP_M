#if !defined(AFX_HEADERCTRLEX_H__C6C3D360_E1BC_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_HEADERCTRLEX_H__C6C3D360_E1BC_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlEx.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx ウィンドウ

class CHeaderCtrlEx : public CHeaderCtrl
{
// コンストラクション
public:
	CHeaderCtrlEx();

protected:
	int		m_nSortCol;
	BOOL	m_bSortAsc;

public:
	int		SetSortImage(int, BOOL);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHeaderCtrlEx();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HEADERCTRLEX_H__C6C3D360_E1BC_11D3_9459_00402641B29B__INCLUDED_)
