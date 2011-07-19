#if !defined(AFX_CDLGSETUP_H__BAB08FBC_1AFB_4304_9AD2_1D604A3CF061__INCLUDED_)
#define AFX_CDLGSETUP_H__BAB08FBC_1AFB_4304_9AD2_1D604A3CF061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgSetup.h : ヘッダー ファイル
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
	BOOL	m_bWavGenreListSelect;
	BOOL	m_bAviGenreListSelect;
	BOOL	m_bWavSupportTrackNumber;
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
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CDLGSETUP_H__BAB08FBC_1AFB_4304_9AD2_1D604A3CF061__INCLUDED_)
