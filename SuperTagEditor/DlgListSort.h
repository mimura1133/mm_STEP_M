#if !defined(AFX_DLGLISTSORT_H__91A7A8E1_FFF9_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DLGLISTSORT_H__91A7A8E1_FFF9_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgListSort.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgListSort ダイアログ

class CDlgListSort : public CDialog
{
// コンストラクション
public:
	CDlgListSort(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgListSort)
	enum { IDD = IDD_DLG_LIST_SORT };
	CComboBox	m_listKey5;
	CComboBox	m_listKey4;
	CComboBox	m_listKey3;
	CComboBox	m_listKey2;
	CComboBox	m_listKey1;
	int		m_nSortType1;
	int		m_nSortType2;
	int		m_nSortType3;
	int		m_nKeyColumn1;
	int		m_nKeyColumn2;
	int		m_nKeyColumn3;
	int		m_nKeyColumn4;
	int		m_nKeyColumn5;
	int		m_nSortType4;
	int		m_nSortType5;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgListSort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgListSort)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGLISTSORT_H__91A7A8E1_FFF9_11D3_9459_00402641B29B__INCLUDED_)
