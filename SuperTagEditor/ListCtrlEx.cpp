//////////////////////////////////////////////
// *.cpp
// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "list.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPHk::WNDINFO_MAP  CPHk::m_wndInfo;

//////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx() : m_bActive(TRUE)
{
}

CListCtrlEx::~CListCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnPaint() 
{
  if (m_bActive) {
	  CRect  rc;
	  GetParent()->GetClientRect(&rc);

	  if (rc.Size() == CSize(0,0))
		return;

	  CPaintDC dc(this);
	  CDC     MemoryDC;
	  CBitmap Bitmap;
	  CBrush  Brush;

	  CRect rcWnd;
	  GetWindowRect(&rcWnd);
	  int rcWndW = rcWnd.Width(), rcWndH = rcWnd.Height();

	  // Paint to a compatible DC
	  MemoryDC.CreateCompatibleDC(&dc);
	  Bitmap.CreateCompatibleBitmap(&dc, rcWndW, rcWndH);
	  CBitmap *pOldBitmap = MemoryDC.SelectObject(&Bitmap);
   
	  // Fill background (does what WM_ERASEBKGND would do)
	  Brush.CreateSolidBrush(GetBkColor());
	  MemoryDC.FillRect(&CRect(0,0, rcWndW,rcWndH), &Brush);

	  // Hook the header control so that paint gets redirected
	  // to the memory dc
	  CPHk  hookHeaderCtrl(GetHeaderCtrl()->GetSafeHwnd(), MemoryDC);

	  // Do client and children areas first otherwise system
	  // scrollbars don't draw correctly.
	  Print(&MemoryDC, PRF_CLIENT|PRF_CHILDREN);
	  Print(&MemoryDC, PRF_NONCLIENT);

	  // Offset the MemoryDC drawing rectangle by rcBounds 
	  CRect rcDraw = rc - rc.TopLeft();

	  // Dump bits to real device context.
	  dc.BitBlt(rc.left,rc.top, rc.Width(),rc.Height(), &MemoryDC, rcDraw.left,rcDraw.top, SRCCOPY);

	  MemoryDC.SelectObject(pOldBitmap);
  } else {
	  CListCtrl::OnPaint();
  }
}

BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
  if (m_bActive) {
  } else {
	  CListCtrl::OnEraseBkgnd(pDC);
  }
  return TRUE;
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  if (m_bActive) {
	  switch (nSBCode)
	  {
	  case SB_PAGEDOWN:
	  case SB_PAGEUP:
	  case SB_LINEDOWN:
	  case SB_LINEUP:
		// Don't update window if paging up or down. It looks
		// terrible!
		LockWindowUpdate();
		CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
		UnlockWindowUpdate();
		break;

	  default:
		CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
		break;
	  }
  } else {
	  CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
  }
}

void CListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (m_bActive) {
	  const UINT ENHANCEDKEY = 0x0100;
	  const UINT PAGEUPKEY   = 0x21;
	  const UINT PAGEDNKEY   = 0x22;

	  // Handle enhanced keys since OnChar doesn't seem to get
	  // them...
		if (nFlags & ENHANCEDKEY)
	  {
		// Don't update window if paging up or down. It looks
		// terrible!
		switch (nChar)
		{
		case PAGEUPKEY:
		  OnVScroll(SB_PAGEUP, 0, 0);
		  return;

		case PAGEDNKEY:
		  OnVScroll(SB_PAGEDOWN, 0, 0);
		  return;
		}
	  }
  }

  CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
