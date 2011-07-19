#if !defined(AFX_DLGFAVORITES_H__72E00FFA_954D_4161_907B_FD5191B44F29__INCLUDED_)
#define AFX_DLGFAVORITES_H__72E00FFA_954D_4161_907B_FD5191B44F29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFavorites.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFavorites ダイアログ

class CDlgFavorites : public CDialog
{
// コンストラクション
public:
	CDlgFavorites(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgFavorites)
	enum { IDD = IDD_DIALOG_FAVORITES };
	CString	m_strFavirites[10];
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgFavorites)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFavorites)
	afx_msg void OnBtRefFavorites1();
	afx_msg void OnBtRefFavorites10();
	afx_msg void OnBtRefFavorites2();
	afx_msg void OnBtRefFavorites3();
	afx_msg void OnBtRefFavorites4();
	afx_msg void OnBtRefFavorites5();
	afx_msg void OnBtRefFavorites6();
	afx_msg void OnBtRefFavorites7();
	afx_msg void OnBtRefFavorites8();
	afx_msg void OnBtRefFavorites9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnBtRefFavorites(CString& strFavorite);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFAVORITES_H__72E00FFA_954D_4161_907B_FD5191B44F29__INCLUDED_)
