////////////////////
// Header
#if !defined(AFX_LISTCTRLEX_H)
#define AFX_LISTCTRLEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

#pragma warning( disable : 4786 )
#include <map>

///////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bActive;
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


// Class to convert non-dc related painting messages to a
// messages with a dc. I will be glad when MicroSlop come
// out with a version of ListCtrl that supports DCs which
// include scroll bar and header redraw.
class CPHk/*WMPaintHookŽ¯•ÊŽq‚ª’·‚·‚¬‚é‚ÆŒ¾‚í‚ê‚é‚Ì‚Å’Z‚ß‚É‚µ‚Ä‚¨‚­*/
{
public:
/**
  struct wndInfo
  {
    wndInfo(HDC hdc, WNDPROC pPrevFunc) : m_hdc(hdc), m_pPrevFunc(pPrevFunc) {};

    HDC     m_hdc;
    WNDPROC m_pPrevFunc;
    short   m_sCount;
  };
**/
	struct wndInfo 
	{ 
		wndInfo(HDC hdc, WNDPROC pPrevFunc) 
		: m_hdc(hdc), 
		m_pPrevFunc(pPrevFunc), 
		m_sCount(0) // original code missed this attribute 
	{ }; 
	wndInfo& operator=(const wndInfo& MC ) { // this should never be called but just in case 
		m_sCount = MC.m_sCount; 
		m_hdc = MC.m_hdc; 
		m_pPrevFunc = MC.m_pPrevFunc; 
		return *this; 
	} 
	HDC m_hdc; 
	WNDPROC m_pPrevFunc; 
	short m_sCount; 
	}; 
  typedef std::map<HWND, wndInfo> WNDINFO_MAP;

public:
  CPHk()  { _ASSERT(0); }
  CPHk(HWND hWnd, HDC hdc) : m_hWnd(hWnd)
  {
    WNDPROC pPrevFunc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (long)MsgProc);

    WNDINFO_MAP::iterator it = m_wndInfo.find(m_hWnd);
    if (it == m_wndInfo.end())
      m_wndInfo.insert(WNDINFO_MAP::value_type(hWnd, wndInfo(hdc, pPrevFunc)));
    else
      ((*it).second).m_sCount++;
  }

  ~CPHk()
  {
    WNDINFO_MAP::iterator it = m_wndInfo.find(m_hWnd);
    _ASSERT(it != m_wndInfo.end());
    wndInfo &wi = (*it).second;

    if (wi.m_sCount > 0)
      wi.m_sCount--;
    else
    {
      VERIFY((WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (long)wi.m_pPrevFunc) == MsgProc);
      m_wndInfo.erase(it);
    }
  }

private:
  static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    WNDINFO_MAP::iterator it = m_wndInfo.find(hWnd);
    _ASSERT(it != m_wndInfo.end());
    wndInfo &wi = (*it).second;

    switch (uMsg)
    {
    case WM_PAINT:
    case WM_NCPAINT:
      return ::CallWindowProc(wi.m_pPrevFunc, hWnd, WM_PRINT, (WPARAM)wi.m_hdc, lParam);

    case WM_ERASEBKGND:
      if ((WPARAM)wi.m_hdc == wParam)
        return ::CallWindowProc(wi.m_pPrevFunc, hWnd, WM_ERASEBKGND, (WPARAM)wi.m_hdc, lParam);
      else
        return TRUE;

    default:
      return ::CallWindowProc(wi.m_pPrevFunc, hWnd, uMsg, wParam, lParam);
    }
  }

private:
  HWND    m_hWnd;
  static WNDINFO_MAP  m_wndInfo;
};

//////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H)
