// ListEditCtrl.cpp : implementation file
//

// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml

#include "stdafx.h"
#include "SuperTagEditor.h"
#include "ListEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	bool	g_bOpenEditWindow;
extern	CEdit	*g_pEditWindow;
static bool bShowScrollBar = false;

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl
BEGIN_MESSAGE_MAP(CListEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CListEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl message handlers

CListEditCtrl::CListEditCtrl(int iItem, int iSubItem, CString sInitText):m_strInitText(sInitText)
{	
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bVK_ESCAPE = 0;
	m_bCursorExit = g_bOptEnableEditCursorExit;
	g_bOpenEditWindow = true;
	g_pEditWindow = this;
	bShowScrollBar = false;
}

int CListEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)		
		return -1;	

	CFont* font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText(m_strInitText);
	CRect rect;
	if (CalculateSize(rect)) {  /* BeachMonster 089 */
		MoveWindow( &rect );
	}
//	SetFocus();
	SetSel(0, 0);

	return 0;
}

BOOL CListEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_ESCAPE || GetKeyState( VK_CONTROL)) {
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}
	}
	//TRACE1("CListEditCtrl::PreTranslateMessage %x\n", pMsg->message);
	return CEdit::PreTranslateMessage(pMsg);
}


void CListEditCtrl::OnKillFocus(CWnd* pNewWnd)
{	
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);
	{// 隣のセルにゴミが残らないように /* BeachMonster 089 */
		CRect rect;
		CalculateSize(rect);
		GetParent()->InvalidateRect(rect);
	}

	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;
	lvDispInfo.item.iItem = m_iItem;
	lvDispInfo.item.iSubItem = m_iSubItem;
	lvDispInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	DestroyWindow();
	AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_INPUT_MODE, 0, (LPARAM)" "); /* Misirlou 150 */
}


void CListEditCtrl::OnNcDestroy()
{
	CEdit::OnNcDestroy();
	delete this;
	g_bOpenEditWindow = false;
	g_pEditWindow = NULL;
}


void CListEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar) {
	case VK_RETURN:
		nChar = VK_DOWN;
	case VK_TAB:
		if (g_bOptEditOkDown) {
			// 親ウィンドウにポスト(カーソルを移動させるため)
			if (!g_bEndEditMoveRightCell) { /* BeachMonster 091 */
				GetParent()->PostMessage(WM_KEYDOWN, nChar, 0);
				GetParent()->PostMessage(WM_KEYUP, nChar, 0); /* Baja 165 */
			} else {
				GetParent()->PostMessage(WM_KEYDOWN, VK_RIGHT, 0); /* BeachMonster 091 */
			}
		}
		// 終了
		GetParent()->SetFocus();
		return;	
	case VK_ESCAPE:
		if (g_bOptESCEditCancel == false) {
			// 現在、何か入力されている場合は、文字をクリアする
			if (LineLength() != 0) {
				SetWindowText(_T(""));
				return;
			}
		}
		// 何も入力されていない状態で [ESC] が押された場合はキャンセル
		m_bVK_ESCAPE = 1;
		// 終了
		GetParent()->SetFocus();
		return;	
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	// ここからしたごっそり変更 /* BeachMonster 089 */
	CRect rect;
	if (CalculateSize(rect)) {
		MoveWindow( &rect );
	}
}

void CListEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_bCursorExit && (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_LEFT || nChar == VK_RIGHT)) {
		GetParent()->PostMessage(WM_KEYDOWN, nChar, 0);
		GetParent()->SetFocus();
		return;
	}
	if (nChar == VK_F2) { /* Misirlou 150 */
		m_bCursorExit = !m_bCursorExit;
		ShowInputMode();
		return;
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

bool CListEditCtrl::CalculateSize(CRect& rect) /* BeachMonster 089 */
{
	// Resize edit control if needed
	// Get text extent	
	CString str;
	GetWindowText( str );	
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject(pFont);
	// Get client rect
	CRect rcParent;	
	GetClientRect(&rect);
	GetParent()->GetClientRect(&rcParent);
	// Transform rect to parent coordinates	
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	if (bShowScrollBar) { // スクロールバーを考慮
		rect.right += GetSystemMetrics(SM_CXVSCROLL); 
	}

	CSize size;
	if (!(GetStyle() & ES_MULTILINE) || str.Find('\n') == -1) {
		size = dc.GetTextExtent(str);	
		size.cx += 5; // add some extra buffer	
	} else {
		size.cy = size.cx = 0;
		while (true) { // ホントは１行づつ処理したくないのだがDrawTextが横幅を勝手に広げて縦が足りなくなるので自分でやる
			bool bEmpty = false;
			int nIndex = str.Find("\r");
			CString line = str;
			if (nIndex >= 0) {
				line = str.Left(nIndex);
			}
			if (line.IsEmpty()) {
				line = " ";
				bEmpty = true;
			}
			while (true) {
				//CRect thinrect( rect );  // To measure the skinniest text box
				CRect widerect( rect );  // To measure the wides text box
				widerect.right = rcParent.right - 2;
				// Use the shortest of the two box sizes.
//				int thinheight = dc.DrawText( line, &thinrect, DT_SINGLELINE|DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS/*|DT_WORDBREAK*/ );
				int wideheight = dc.DrawText( line, &widerect, DT_SINGLELINE|DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS/*|DT_WORDBREAK*/ );
				if (widerect.right > rcParent.right - 4) {
					line.Delete(line.GetLength() - 1);
					BOOL bLead = FALSE;
					for (int i=0;i<line.GetLength();i++) {
						if (_ismbblead(line[i]) && !bLead) {
							bLead = TRUE;
						} else {
							bLead = FALSE;
						}
					}
					if (bLead) {
						line.Delete(line.GetLength() - 1);
					}
					continue;
				}
/*				if ( thinheight >= wideheight ) {*/
					size.cy += wideheight;
					if (size.cx < (widerect.right - widerect.left)) {
						size.cx = widerect.right - widerect.left;
					}
/*				} else {
					size.cy += thinheight;
					if (size.cx < (thinrect.right - thinrect.left)) {
						size.cx = thinrect.right - thinrect.left;
					}
				}*/
				if (!bEmpty) {
					str.Delete(0, line.GetLength());
				}
				break;
			}
			if (str.IsEmpty()) {
				break;
			}
			str.TrimLeft("\r");// "\r\n"の束が纏めてTrimされないようにわけてやる
			str.TrimLeft("\n");
		}
		size.cy += 5;
		size.cx += 5;
	}
	dc.SelectObject(pFontDC);
	// Check whether control needs to be resized
	// and whether there is space to grow
	int changed = 0;
	bool bNeedVScrollBar = false;
	if ( size.cx > rect.Width() ) {
		if ( size.cx + rect.left < rcParent.right-2 )
			rect.right = rect.left + size.cx;
		else
			rect.right = rcParent.right-2;
		changed = 1;
	}
	if ( size.cy > rect.Height() ) {
		if( size.cy + rect.top < rcParent.bottom-2 ) {
			rect.bottom = rect.top + size.cy;
		}
		else {
			rect.bottom = rcParent.bottom-2;
			bNeedVScrollBar = true;
		}
		changed = 1;
	}
	if (bNeedVScrollBar) {
		if (bShowScrollBar == false) {
			if ( rect.right + GetSystemMetrics(SM_CXVSCROLL) > rcParent.right-2) {
				//rect.right = rcParent.right-2 - GetSystemMetrics(SM_CXVSCROLL); 
			} else {
				rect.right += GetSystemMetrics(SM_CXVSCROLL); 
			}
			ShowScrollBar( SB_VERT );
			bShowScrollBar = true;
			changed = true;
		}
	} else {
		if (bShowScrollBar == true) {
			//rect.right -= GetSystemMetrics(SM_CXVSCROLL); 
			ShowScrollBar( SB_VERT, FALSE );
			bShowScrollBar = false;
			changed = true;
		}
	}
	// If the size became larger rposition the window.
	if( changed ) {
		return true;
	}
	return false;
}

void CListEditCtrl::OnSetfocus() /* BeachMonster 089 */
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	/* ES_MULTILINE のときに何故か最後の行が選択状態になってしまうのを回避するため */
	/* Misirlou 151 *//*
	keybd_event(VK_END, 0, 0, 0);
	keybd_event(VK_END, 0, KEYEVENTF_KEYUP, 0);
	*/
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); /* BeachMonster 089 */
	ShowInputMode();	/* STEP 017 */
	return;	
}

void CListEditCtrl::ShowInputMode() /* Misirlou 150 */
{
	if (m_bCursorExit) {
		AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_INPUT_MODE, 0, (LPARAM)"入力");
	} else {
		AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_INPUT_MODE, 0, (LPARAM)"編集");
	}
}

void CListEditCtrl::ChangeInputMode(bool bCursorExit)	/* STEP 017 */
{
	m_bCursorExit = bCursorExit;
	ShowInputMode();
}
