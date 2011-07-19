#if !defined(AFX_DLGFILERMPID3V2_H__DB58BAE7_750F_475F_A29E_95CD220EE90D__INCLUDED_)
#define AFX_DLGFILERMPID3V2_H__DB58BAE7_750F_475F_A29E_95CD220EE90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileRmpID3v2.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileRmpID3v2 ダイアログ

class CDlgFileRmpID3v2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgFileRmpID3v2)

// コンストラクション
public:
	CDlgFileRmpID3v2();
	~CDlgFileRmpID3v2();

	void	UpdateStatus(void);

// ダイアログ データ
	//{{AFX_DATA(CDlgFileRmpID3v2)
	enum { IDD = IDD_DLG_FILE_RMP_ID3V2 };
	BOOL	m_bRmpID3tagAutoWrite;
	BOOL	m_bAutoConvRMP;
	int		m_nSIFieldConvertType;
	BOOL	m_bRmpGenreListSelect;
	BOOL	m_bChangeFileExt;
	BOOL	m_bAutoConvID3v2;
	BOOL	m_bID3v2ID3tagAutoWrite;
	BOOL	m_bID3v2GenreListSelect;
	BOOL	m_bID3v2GenreAddNumber;
	BOOL	m_bID3v2Id3tagAutoDelete;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgFileRmpID3v2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgFileRmpID3v2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtResetPage();
	afx_msg void OnChAutoConvRmp();
	afx_msg void OnChAutoConvId3v2();
	afx_msg void OnChId3v2Id3tagAutoWrite();
	afx_msg void OnChId3v2Id3tagAutoDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILERMPID3V2_H__DB58BAE7_750F_475F_A29E_95CD220EE90D__INCLUDED_)
