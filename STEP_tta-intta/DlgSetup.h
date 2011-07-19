#if !defined(AFX_DLGSETUP_H__E7A8FD8E_E882_4311_84B7_810018582F8F__INCLUDED_)
#define AFX_DLGSETUP_H__E7A8FD8E_E882_4311_84B7_810018582F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup ダイアログ

class CDlgSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgSetup)

// コンストラクション
public:
	CDlgSetup();
	~CDlgSetup();

// ダイアログ データ
	//{{AFX_DATA(CDlgSetup)
	enum { IDD = IDD_DLG_SETUP };
	BOOL	m_bGenreListSelect;
	BOOL	m_bID3TagAutoDelete;
	BOOL	m_bID3TagAutoWrite;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgSetup)
	afx_msg void OnChId3tagAutoDelete();
	afx_msg void OnChId3tagAutoWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSETUP_H__E7A8FD8E_E882_4311_84B7_810018582F8F__INCLUDED_)
