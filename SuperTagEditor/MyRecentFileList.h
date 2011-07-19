// MyRecentFileList.h: CMyRecentFileList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYRECENTFILELIST_H__90D1B982_45B6_4E03_87C4_A6030A469961__INCLUDED_)
#define AFX_MYRECENTFILELIST_H__90D1B982_45B6_4E03_87C4_A6030A469961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxadv.h>  // for CRecentFileList

class CMyRecentFileList : public CRecentFileList /* StartInaction 053 */
{
public:
	void Add(LPCTSTR lpszPathName);
    CMyRecentFileList(UINT nStart, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen = AFX_ABBREV_FILENAME_LEN) : CRecentFileList(nStart, lpszSection,lpszEntryFormat, nSize, nMaxDispLen) {}
    virtual void UpdateMenu(CCmdUI* pCmdUI);

};

#endif // !defined(AFX_MYRECENTFILELIST_H__90D1B982_45B6_4E03_87C4_A6030A469961__INCLUDED_)
