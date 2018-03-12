// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "AutoBuildCount.h"
#include "SuperTagEditor.h"
#include "SuperTagEditorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LOCATION, OnUpdateIndicatorEnable)
	ON_MESSAGE(WM_USER_SET_STATUS_POS, OnSetStatusPos)
	ON_MESSAGE(WM_USER_SET_STATUS_SIZE, OnSetStatusSize)
	ON_MESSAGE(WM_USER_SET_STATUS_INPUT_MODE, OnSetStatusInputMode)
	ON_COMMAND(ID_POPUP_CUSTOMIZE, OnPopupCustomize)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CUSTOMIZE, OnUpdatePopupCustomize)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CToolBarInfo CMainFrame::mainToolBar[] =
{
	{{0, ID_FILE_NEW  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}, "���X�g���N���A"     },
	{{1, ID_OPEN_FOLDER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN /* StartInaction 053 */, 0, 1}, "�t�H���_���J��"    },
	{{2, ID_SAVE_ALL_TAG , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2}, "�^�O�����X�V"    },
	{{3, ID_SELECT_EDIT_DESTORY  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3}, "�ҏW�O�̏�Ԃɖ߂�"     },
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 4}, ""        },
	{{4, ID_EDIT_CUT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5}, "�؂���"    },
	{{5, ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6}, "�R�s�["   },
	{{6, ID_EDIT_PASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7}, "�\��t��"   },
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 8}, ""        },
	{{7, ID_CELL_COPY_DOWN , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9}, "�������փR�s�["},
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 10}, ""        },
	{{8, ID_EDIT_TD3_TAG , TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 11}, "ID3 tag��\���^�ҏW"},
	{{9, ID_EDIT_FIELD_SI , TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 12}, "SI�t�B�[���h��\���^�ҏW"},
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 13}, ""        },
	{{10, ID_WINAMP_PLAY_PREV , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 14}, "�O�̋Ȃ�"},
	{{11, ID_WINAMP_PLAY , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 15}, "�Đ�"},
	{{12, ID_WINAMP_STOP , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 16}, "��~"},
	{{13, ID_WINAMP_PLAY_NEXT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 17}, "���̋Ȃ�"},
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 18}, ""        },
	{{14, ID_DLG_ENVIRONMENT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 19}, "�I�v�V����"},
	{{15, ID_APP_ABOUT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 20}, "�o�[�W�������"},
	{{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 21}, ""        },
	{{16, ID_CONV_FORMAT_USER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0, 22}, "���[�U�����ؑ�"},
	{{17, ID_SET_NUMBER , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 23}, "�������֘A��"},
	{{18, ID_SET_NUMBER_ADD , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 24}, "�������֘A�Ԃ�ǉ�"},
	{{19, ID_SELECT_TREE_COLUM , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 25}, "�c���[�z���̃Z���ꊇ�I��"},
	{{20, ID_TEIKEI_TOOL , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0, 26}, "��^���\��t��"},
	{{21, ID_CHECK_FILENAME_MAX , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 27}, "�t�@�C�����̒������`�F�b�N"},
	{{22, ID_SELECT_DELETE_LIST , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 28}, "���X�g����폜"},
	{{23, ID_DELETE_CHAR , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 29}, "�擪/�����̂��������폜"},
	{{24, ID_UNIFY_CHAR , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 30}, "������̓���"},
	{{25, ID_END_EDIT_RIGHT , TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 31}, "���ڕύX��ɃJ�[�\�����E�Ɉړ�"}, /* BeachMonster 091 */
	{{26, ID_FAVORITES_FOLDER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0, 32}, "���C�ɓ���̃t�H���_"}, /* RockDance 129 */
	{{27, ID_CALC_FOLDER_TOTAL , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 33}, "���v�̎Z�o"}, /* RockDance 128 */
	{{28, ID_EDIT_PASTE_ADD , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 34}, "�ǉ��œ\��t��"}, /* Baja 171 */
	{{29, ID_DELETE_CHAR_SPACE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 35}, "�擪/�����̋󔒕������폜"}, /* Rumble 192 */
	{{30, ID_SELECT_TREE_FILE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 36}, "�c���[�z���̃t�@�C���ꊇ�I��"}, /* STEP 013 */
	{{31, ID_MOVE_TO_PARENT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 37}, "�J�[�\����e�c���[�ֈړ�"}, /* STEP 014 */
//	{{32, ID_MOVE_TO_PREVIOUS , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 38}, "�J�[�\���ʒu�����̃c���[�ֈړ�"}, /* STEP 014 */
//	{{33, ID_MOVE_TO_NEXT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 39}, "�J�[�\���ʒu������̃c���[�ֈړ�"}, /* STEP 014 */
	{{32, ID_DLG_SETUP_PLUGIN , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 38}, "�v���O�C���I�v�V����"}, /* STEP 039*/
	/**
	 �y���Ӂz�R�}���h��ǉ������ꍇ�́A_APS_NEXT_COMMAND_VALUE���X�V�����̂�STEP_api.cpp�����R���p�C�����邱��
	 **/
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_SIZE, /* WildCherry 072 */
	ID_INDICATOR_LOCATION,
	ID_INDICATOR_INPUT_MODE, /* Misirlou 150 */
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����

CMainFrame::CMainFrame()
{
	// TODO: ���̈ʒu�Ƀ����o�̏����������R�[�h��ǉ����Ă��������B
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (/*!m_wndToolBar.CreateEx(this) || AstralCircle 041 */
		!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		AFX_IDW_TOOLBAR, mainToolBar,
		"Software\\MERCURY\\SuperTagEditor\\haseta\\Toolbar",
		"ToolBar Settings") ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}

	extern CDkToolBar* STEP_wndToolBar;
	STEP_wndToolBar = &m_wndToolBar;

	// �_�C�A���O�o�[�쐬
//	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR)) {
//		TRACE0("Failed to create dialogbar\n");
//		return -1;		// �쐬�Ɏ��s
//	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)/* ||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // �쐬�Ɏ��s
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}

	// TODO: �����c�[�� �`�b�v�X���K�v�Ȃ��ꍇ�A�������폜���Ă��������B
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);


	// �o�[�W�����A�b�v��̏���N��������
//	if (strcmp(PROG_VERSION, "0.32") == 0) {
//		if (g_bIsVersionUp == false) {
//			LoadBarState(_T("ToolBar"));
//		}
//	} else {
		LoadBarState(_T("ToolBar"));
//	}

	{ /* AstralCircle 041 */
        LONG lStyle = GetWindowLong(m_wndToolBar, GWL_STYLE);
        //  TBSTYLE_FLAT�X�^�C�����w��
        lStyle |= TBSTYLE_FLAT;
        //  �V�����E�B���h�E�X�^�C����ݒ�
        SetWindowLong(m_wndToolBar, GWL_STYLE, lStyle);
		m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
		DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_CONV_FORMAT_USER));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_CONV_FORMAT_USER), dwStyle);
		dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_TEIKEI_TOOL));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_TEIKEI_TOOL), dwStyle);
		// StartInaction 053 */
		dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_OPEN_FOLDER));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_OPEN_FOLDER), dwStyle);
		// RockDance 129 */
		dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_FAVORITES_FOLDER));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FAVORITES_FOLDER), dwStyle);

		int nIndex = m_wndToolBar.GetToolBarCtrl().AddBitmap(USER_CONV_FORMAT_MAX/* 3 LastTrain 057 */, IDR_TOOLBAR_DYNAMIC);
		m_wndToolBar.nDyImageIndex = nIndex;

		m_wndToolBar.RestoreState();
	}
	UpdateToolBar();

	return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	if (g_rectMainWindow.top != 0
	&&  g_rectMainWindow.left != 0
	&&  g_rectMainWindow.bottom != 0
	&&  g_rectMainWindow.right != 0) {
		// �O��̃E�B���h�E�T�C�Y�ŊJ��
		cs.x	= g_rectMainWindow.left;
		cs.y	= g_rectMainWindow.top;
		cs.cx	= g_rectMainWindow.right - g_rectMainWindow.left;
		cs.cy	= g_rectMainWindow.bottom - g_rectMainWindow.top;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��
void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);

	// �E�B���h�E�̏�Ԃ�ۑ�
	WINDOWPLACEMENT wndpl;
	AfxGetMainWnd()->GetWindowPlacement(&wndpl);
	if (AfxGetMainWnd()->IsZoomed() == FALSE && AfxGetMainWnd()->IsIconic() == FALSE) {
		GetWindowRect(&g_rectMainWindow);
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	// �E�B���h�E�̏�Ԃ�ۑ�
	WINDOWPLACEMENT wndpl;
	AfxGetMainWnd()->GetWindowPlacement(&wndpl);
	if (AfxGetMainWnd()->IsZoomed() == FALSE && AfxGetMainWnd()->IsIconic() == FALSE) {
		GetWindowRect(&g_rectMainWindow);
	}
}

void CMainFrame::OnClose() 
{
	SaveBarState(_T("ToolBar"));

	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CFrameWnd::OnUpdateFrameTitle(bAddToTitle);

	CString	strTitle;
	strTitle.Format(PROG_NAME " Ver " PROG_VERSION);
	/*strTitle += "��";*/
	SetWindowText(strTitle);
}

void CMainFrame::OnUpdateIndicatorEnable(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

LRESULT CMainFrame::OnSetStatusPos(WPARAM wParam, LPARAM lParam)
{
	static	CString	str;
	str.Format("%d�F%d", (signed short)((lParam>>16)&0xffff), (signed short)(lParam&0xffff));
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOCATION), (LPCSTR)str);
	m_wndStatusBar.UpdateWindow();
	return 0;
}

LRESULT CMainFrame::OnSetStatusSize(WPARAM wParam, LPARAM lParam) /* WildCherry 072 */
{
	CClientDC dc(&m_wndStatusBar);
	CFont *pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize size = dc.GetTextExtent((LPCSTR)lParam);
	//int margin = dc.GetTextExtent(_T(" ")).cx * 2;
	dc.SelectObject(pOldFont);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_SIZE), ID_INDICATOR_SIZE, 0, size.cx/* + margin*/);
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_SIZE), (LPCSTR)lParam);
	m_wndStatusBar.UpdateWindow();
	return 0;
}

LRESULT CMainFrame::OnSetStatusInputMode(WPARAM wParam, LPARAM lParam) /* Misirlou 150 */
{
	CClientDC dc(&m_wndStatusBar);
	CFont *pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize size = dc.GetTextExtent((LPCSTR)lParam);
	//int margin = dc.GetTextExtent(_T(" ")).cx * 2;
	dc.SelectObject(pOldFont);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INPUT_MODE), ID_INDICATOR_INPUT_MODE, 0, size.cx/* + margin*/);
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INPUT_MODE), (LPCSTR)lParam);
	m_wndStatusBar.UpdateWindow();
	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	CSuperTagEditorApp	*pApp = (CSuperTagEditorApp *)AfxGetApp();

	if (pApp != NULL && pApp->GetAccelHandle() != 0) {
		// �Ǝ��̃A�N�Z�����[�^�L�[����������
		if (TranslateAccelerator(m_hWnd, pApp->GetAccelHandle(), pMsg)) {
			return TRUE;
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::ToolbarDropDown(int item){
	NMTOOLBAR nmhdr = {0};
	nmhdr.iItem = item;

	OnToolbarDropDown((NMHDR*)&nmhdr,NULL);
}

void CMainFrame::OnToolbarDropDown(NMHDR* pnmtb_, LRESULT *plr) /* AstralCircle 041 */
{
	NMTOOLBAR *pnmtb;
	pnmtb = (NMTOOLBAR*)pnmtb_;
	CWnd *pWnd = &m_wndToolBar;
	//UINT nID;

	// Switch on button command id's.
	CMenu menu;
	CMenu* pPopup = NULL;
	switch (pnmtb->iItem) {
	case ID_CONV_FORMAT_USER:
		{
			menu.LoadMenu(IDR_MENU_CONV_FORMAT_USER);
			pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup);
			CString strName;
			strName.Format("01�F%s", g_userConvFormat[0].strName);
			pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_01, MF_BYCOMMAND, ID_CONV_FORMAT_USER_01, (LPCTSTR)strName);
			strName.Format("02�F%s", g_userConvFormat[1].strName);
			pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_02, MF_BYCOMMAND, ID_CONV_FORMAT_USER_02, (LPCTSTR)strName);
			strName.Format("03�F%s", g_userConvFormat[2].strName);
			pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_03, MF_BYCOMMAND, ID_CONV_FORMAT_USER_03, (LPCTSTR)strName);
			strName.Format("04�F%s", g_userConvFormat[3].strName); /* LastTrain 057 */
			pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_04, MF_BYCOMMAND, ID_CONV_FORMAT_USER_04, (LPCTSTR)strName);
			strName.Format("05�F%s", g_userConvFormat[4].strName); /* LastTrain 057 */
			pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_05, MF_BYCOMMAND, ID_CONV_FORMAT_USER_05, (LPCTSTR)strName);
		}
		break;
	case ID_TEIKEI_TOOL:
		{
			menu.LoadMenu(IDR_MENU_TEIKEI);
			pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup);
			CString strName;
			// �O���[�v�P
			strName.Format("01: %s", shortString(g_teikeiInfo[0].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_01, MF_BYCOMMAND, ID_TEIKEI_01, (LPCTSTR)strName);
			strName.Format("02: %s", shortString(g_teikeiInfo[1].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_02, MF_BYCOMMAND, ID_TEIKEI_02, (LPCTSTR)strName);
			strName.Format("03: %s", shortString(g_teikeiInfo[2].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_03, MF_BYCOMMAND, ID_TEIKEI_03, (LPCTSTR)strName);
			strName.Format("04: %s", shortString(g_teikeiInfo[3].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_04, MF_BYCOMMAND, ID_TEIKEI_04, (LPCTSTR)strName);
			strName.Format("05: %s", shortString(g_teikeiInfo[4].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_05, MF_BYCOMMAND, ID_TEIKEI_05, (LPCTSTR)strName);
			strName.Format("06: %s", shortString(g_teikeiInfo[5].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_06, MF_BYCOMMAND, ID_TEIKEI_06, (LPCTSTR)strName);
			strName.Format("07: %s", shortString(g_teikeiInfo[6].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_07, MF_BYCOMMAND, ID_TEIKEI_07, (LPCTSTR)strName);
			strName.Format("08: %s", shortString(g_teikeiInfo[7].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_08, MF_BYCOMMAND, ID_TEIKEI_08, (LPCTSTR)strName);
			strName.Format("09: %s", shortString(g_teikeiInfo[8].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_09, MF_BYCOMMAND, ID_TEIKEI_09, (LPCTSTR)strName);
			strName.Format("10: %s", shortString(g_teikeiInfo[9].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_10, MF_BYCOMMAND, ID_TEIKEI_10, (LPCTSTR)strName);
			/* FreeFall 046 */
			// �O���[�v�Q
			strName.Format("01: %s", shortString(g_teikeiInfo[10].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_01, MF_BYCOMMAND, ID_TEIKEI_2_01, (LPCTSTR)strName);
			strName.Format("02: %s", shortString(g_teikeiInfo[11].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_02, MF_BYCOMMAND, ID_TEIKEI_2_02, (LPCTSTR)strName);
			strName.Format("03: %s", shortString(g_teikeiInfo[12].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_03, MF_BYCOMMAND, ID_TEIKEI_2_03, (LPCTSTR)strName);
			strName.Format("04: %s", shortString(g_teikeiInfo[13].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_04, MF_BYCOMMAND, ID_TEIKEI_2_04, (LPCTSTR)strName);
			strName.Format("05: %s", shortString(g_teikeiInfo[14].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_05, MF_BYCOMMAND, ID_TEIKEI_2_05, (LPCTSTR)strName);
			strName.Format("06: %s", shortString(g_teikeiInfo[15].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_06, MF_BYCOMMAND, ID_TEIKEI_2_06, (LPCTSTR)strName);
			strName.Format("07: %s", shortString(g_teikeiInfo[16].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_07, MF_BYCOMMAND, ID_TEIKEI_2_07, (LPCTSTR)strName);
			strName.Format("08: %s", shortString(g_teikeiInfo[17].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_08, MF_BYCOMMAND, ID_TEIKEI_2_08, (LPCTSTR)strName);
			strName.Format("09: %s", shortString(g_teikeiInfo[18].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_09, MF_BYCOMMAND, ID_TEIKEI_2_09, (LPCTSTR)strName);
			strName.Format("10: %s", shortString(g_teikeiInfo[19].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_2_10, MF_BYCOMMAND, ID_TEIKEI_2_10, (LPCTSTR)strName);
			// �O���[�v�R
			strName.Format("01: %s", shortString(g_teikeiInfo[20].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_01, MF_BYCOMMAND, ID_TEIKEI_3_01, (LPCTSTR)strName);
			strName.Format("02: %s", shortString(g_teikeiInfo[21].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_02, MF_BYCOMMAND, ID_TEIKEI_3_02, (LPCTSTR)strName);
			strName.Format("03: %s", shortString(g_teikeiInfo[22].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_03, MF_BYCOMMAND, ID_TEIKEI_3_03, (LPCTSTR)strName);
			strName.Format("04: %s", shortString(g_teikeiInfo[23].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_04, MF_BYCOMMAND, ID_TEIKEI_3_04, (LPCTSTR)strName);
			strName.Format("05: %s", shortString(g_teikeiInfo[24].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_05, MF_BYCOMMAND, ID_TEIKEI_3_05, (LPCTSTR)strName);
			strName.Format("06: %s", shortString(g_teikeiInfo[25].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_06, MF_BYCOMMAND, ID_TEIKEI_3_06, (LPCTSTR)strName);
			strName.Format("07: %s", shortString(g_teikeiInfo[26].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_07, MF_BYCOMMAND, ID_TEIKEI_3_07, (LPCTSTR)strName);
			strName.Format("08: %s", shortString(g_teikeiInfo[27].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_08, MF_BYCOMMAND, ID_TEIKEI_3_08, (LPCTSTR)strName);
			strName.Format("09: %s", shortString(g_teikeiInfo[28].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_09, MF_BYCOMMAND, ID_TEIKEI_3_09, (LPCTSTR)strName);
			strName.Format("10: %s", shortString(g_teikeiInfo[29].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
			pPopup->ModifyMenu((UINT)ID_TEIKEI_3_10, MF_BYCOMMAND, ID_TEIKEI_3_10, (LPCTSTR)strName);
			// �O���[�v���̂𔽉f
			for (int k = 0; k < 3; k++) {
				CMenu* pSubMenu = pPopup->GetSubMenu(0);
				pPopup->RemoveMenu(0, MF_BYPOSITION);
				pPopup->AppendMenu(MF_POPUP , (UINT)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
			}
		}
		break;
	case ID_OPEN_FOLDER: /* StartInaction 053 */
		{
			menu.LoadMenu(IDR_MAINFRAME);
			pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup);
			int nCount = pPopup->GetMenuItemCount();
			for (int j=0;j<nCount;j++) {
				CString strTitle;
				pPopup->GetMenuString(j, strTitle, MF_BYPOSITION);
				if (strTitle == "�ŋߎg�����t�H���_(&R)") {
					pPopup = pPopup->GetSubMenu(j);
					break;
				}
			}
		}
		break;
	case ID_FAVORITES_FOLDER: /* RockDance 129 */
		{
			menu.LoadMenu(IDR_MENU_FAVORITE_FOLDER);
			pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup);
			CString strName;
			strName.Format("01�F%s", g_strFavorite[0]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_01, MF_BYCOMMAND, ID_FAVORITE_FOLDER_01, (LPCTSTR)strName);
			strName.Format("02�F%s", g_strFavorite[1]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_02, MF_BYCOMMAND, ID_FAVORITE_FOLDER_02, (LPCTSTR)strName);
			strName.Format("03�F%s", g_strFavorite[2]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_03, MF_BYCOMMAND, ID_FAVORITE_FOLDER_03, (LPCTSTR)strName);
			strName.Format("04�F%s", g_strFavorite[3]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_04, MF_BYCOMMAND, ID_FAVORITE_FOLDER_04, (LPCTSTR)strName);
			strName.Format("05�F%s", g_strFavorite[4]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_05, MF_BYCOMMAND, ID_FAVORITE_FOLDER_05, (LPCTSTR)strName);
			strName.Format("06�F%s", g_strFavorite[5]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_06, MF_BYCOMMAND, ID_FAVORITE_FOLDER_06, (LPCTSTR)strName);
			strName.Format("07�F%s", g_strFavorite[6]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_07, MF_BYCOMMAND, ID_FAVORITE_FOLDER_07, (LPCTSTR)strName);
			strName.Format("08�F%s", g_strFavorite[7]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_08, MF_BYCOMMAND, ID_FAVORITE_FOLDER_08, (LPCTSTR)strName);
			strName.Format("09�F%s", g_strFavorite[8]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_09, MF_BYCOMMAND, ID_FAVORITE_FOLDER_09, (LPCTSTR)strName);
			strName.Format("10�F%s", g_strFavorite[9]);
			pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_10, MF_BYCOMMAND, ID_FAVORITE_FOLDER_10, (LPCTSTR)strName);
		}
		break;
	default:
		return;
	}
	
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &rc);
}

void CMainFrame::UpdateToolBar()
{
	m_wndToolBar.UpdateButtonImage();
}

void CMainFrame::OnPopupCustomize() /* WildCherry4 084 */
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	m_wndToolBar.Customize();	
}

void CMainFrame::OnUpdatePopupCustomize(CCmdUI* pCmdUI) /* WildCherry4 084 */
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	extern UINT nStartID;
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
	extern bool OnToolTipNotify(UINT nID, LPTSTR& szText);
    if(OnToolTipNotify(nID, pTTT->lpszText)) {
        return(TRUE);
    }
    return(FALSE);
}

void CMainFrame::GetMessageString( UINT nID, CString& rMessage ) const
{
	extern bool GetMessageString(UINT nID, CString& strMessage);
    if(GetMessageString(nID, rMessage)) {
		;
	} else {
		CFrameWnd::GetMessageString(nID, rMessage);
	}
}
