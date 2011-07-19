#if !defined(AFX_DLGUNIFYCHAR_H__53F97B08_CBFE_40A2_A58E_A9DC1662818A__INCLUDED_)
#define AFX_DLGUNIFYCHAR_H__53F97B08_CBFE_40A2_A58E_A9DC1662818A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUnifyChar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUnifyChar ダイアログ

class CDlgUnifyChar : public CDialog
{
// コンストラクション
public:
	CDlgUnifyChar(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgUnifyChar)
	enum { IDD = IDD_DIALOG_UNIFY_CHAR };
	int		m_nConvAlpha;
	int		m_nConvHiraKata;
	int		m_nConvKata;
	int		m_nConvKigou;
	int		m_nConvSuji;
	int		m_nConvUpLow;
	int		m_nConvFixedUpLow;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgUnifyChar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgUnifyChar)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGUNIFYCHAR_H__53F97B08_CBFE_40A2_A58E_A9DC1662818A__INCLUDED_)
