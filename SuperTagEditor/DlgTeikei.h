#if !defined(AFX_DLGTEIKEI_H__FFEA78C8_E872_4903_9D2C_B2D2C6CC159E__INCLUDED_)
#define AFX_DLGTEIKEI_H__FFEA78C8_E872_4903_9D2C_B2D2C6CC159E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeikei.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTeikei ダイアログ

class CDlgTeikei : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgTeikei)

// コンストラクション
public:
	CDlgTeikei();
	~CDlgTeikei();

// ダイアログ データ
	//{{AFX_DATA(CDlgTeikei)
	enum { IDD = IDD_DIALOG_TEIKEI };
	CString	m_strTeikei1;
	CString	m_strTeikei10;
	CString	m_strTeikei2;
	CString	m_strTeikei3;
	CString	m_strTeikei4;
	CString	m_strTeikei5;
	CString	m_strTeikei6;
	CString	m_strTeikei7;
	CString	m_strTeikei8;
	CString	m_strTeikei9;
	CString	m_strGroupName;
	//}}AFX_DATA

	TEIKEI_INFO m_teikeiInfo[10];


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgTeikei)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgTeikei)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtDetail01();
	afx_msg void OnBtDetail10();
	afx_msg void OnBtDetail02();
	afx_msg void OnBtDetail03();
	afx_msg void OnBtDetail04();
	afx_msg void OnBtDetail05();
	afx_msg void OnBtDetail06();
	afx_msg void OnBtDetail07();
	afx_msg void OnBtDetail08();
	afx_msg void OnBtDetail09();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BYTE m_nGroupNumber;
private:
	void OnDetail(int nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGTEIKEI_H__FFEA78C8_E872_4903_9D2C_B2D2C6CC159E__INCLUDED_)
