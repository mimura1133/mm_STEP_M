#if !defined(AFX_DLGCONVID3V2VERSION_H__949D751B_4994_4BBA_8532_64763197473C__INCLUDED_)
#define AFX_DLGCONVID3V2VERSION_H__949D751B_4994_4BBA_8532_64763197473C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConvID3v2Version.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConvID3v2Version ダイアログ

class CDlgConvID3v2Version : public CDialog
{
// コンストラクション
public:
	CDlgConvID3v2Version(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgConvID3v2Version)
	enum { IDD = IDD_DLG_CONV_ID3V2_VERSION };
	CComboBox	m_cbId3v2Version;
	CComboBox	m_cbId3v2Encode;
	int		m_nId3v2Encode;
	int		m_nId3v2Version;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgConvID3v2Version)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgConvID3v2Version)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboId3v2Version();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGCONVID3V2VERSION_H__949D751B_4994_4BBA_8532_64763197473C__INCLUDED_)
