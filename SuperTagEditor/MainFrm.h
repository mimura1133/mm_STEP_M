// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D67B6448_BFAB_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_MAINFRM_H__D67B6448_BFAB_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DkToolBar.h"

class CSuperTagEditorView;

class CMainFrame : public CFrameWnd
{
	
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// アトリビュート
protected:
public:

	inline	void	SetStatusBarText(LPCSTR sText) {
		if (IsWindow(m_wndStatusBar.m_hWnd)) m_wndStatusBar.SetPaneText(0, sText); /* Baja 161 */
			}

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual	void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void ToolbarDropDown(int item);
	void UpdateToolBar();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CDkToolBar/*CToolBar AstralCircle 041 */   m_wndToolBar;
	static CToolBarInfo	 mainToolBar[]; /* AstralCircle 041 */ 
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	afx_msg void OnToolbarDropDown(NMHDR* pnmh, LRESULT* plRes);
	afx_msg void OnPopupCustomize();
	afx_msg void OnUpdatePopupCustomize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateIndicatorEnable(CCmdUI* pCmdUI);
	LRESULT OnSetStatusPos(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStatusSize(WPARAM wParam, LPARAM lParam); /* WildCherry 072 */
	LRESULT OnSetStatusInputMode(WPARAM wParam, LPARAM lParam); /* Misirlou 150 */
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

	void GetMessageString( UINT nID, CString& rMessage ) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__D67B6448_BFAB_11D3_9459_00402641B29B__INCLUDED_)
