#if !defined(AFX_DLGDELETECHAR_H__DCE4D8FD_84C9_48B1_BF50_EF59313DEB22__INCLUDED_)
#define AFX_DLGDELETECHAR_H__DCE4D8FD_84C9_48B1_BF50_EF59313DEB22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDeleteChar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// DlgDeleteChar ダイアログ

class DlgDeleteChar : public CDialog
{
// コンストラクション
public:
	DlgDeleteChar(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(DlgDeleteChar)
	enum { IDD = IDD_DLG_DELETE_CHAR };
	UINT	m_nDelCount;
	int		m_nPos;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(DlgDeleteChar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(DlgDeleteChar)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGDELETECHAR_H__DCE4D8FD_84C9_48B1_BF50_EF59313DEB22__INCLUDED_)
