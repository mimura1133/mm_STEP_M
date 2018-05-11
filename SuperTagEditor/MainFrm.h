// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
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
	
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// �A�g���r���[�g
protected:
public:

	inline	void	SetStatusBarText(LPCTSTR sText) {
		if (IsWindow(m_wndStatusBar.m_hWnd)) m_wndStatusBar.SetPaneText(0, sText); /* Baja 161 */
			}

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual	void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void ToolbarDropDown(int item);
	void UpdateToolBar();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o
	CStatusBar  m_wndStatusBar;
	CDkToolBar/*CToolBar AstralCircle 041 */   m_wndToolBar;
	static CToolBarInfo	 mainToolBar[]; /* AstralCircle 041 */ 
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__D67B6448_BFAB_11D3_9459_00402641B29B__INCLUDED_)
