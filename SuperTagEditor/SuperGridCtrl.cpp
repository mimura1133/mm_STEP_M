// SuperGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderCtrlEx.h"
#include "SuperTagEditor.h"
#include "SuperGridCtrl.h"
#include "ListEditCtrl.h"

#ifdef FLICKERFREE
//#define CListCtrl CListCtrlEx
#endif

#define COLUMN_MIN			1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef minmax3
#define minmax3(x1,x2,x3,x4,x5)    x1>x2 ? (x5=x1,x4=x2) : (x4=x1,x5=x2), x3>x5 ? x5=x3 : (x3<x4 ? x4=x3 : 0)
#endif

bool 	g_bOpenEditWindow;
CEdit	*g_pEditWindow;

/*
    SuperGrid, YALC (yet another listview control)

	Written by me(Allan Nielsen), that is .. if the code works ;-)
		mailto:allan.nielsen.msfan@bluewin.ch

	Copyright (c) 1999.

	If you find bugs, please notify me and I'll contact the author who wrote the darn thing ;)

    You may use it, abuse it, redistribute it, in any way you desire, but
	if you choose to use this control in any commercial or non commercial application then please
	send me an email letting me know, makes me ;) .

    Also thanks for all the emails, I count around 200 in my emailbox on this one, all with
	bugfixes, improvements and suggestions ..thanks. If only I had more time....

	HISTORY


	4 dec. 1998.

				- MeasureItem now uses current font
				- Combobox support in CItemInfo
				  see the ShowList in MySuperGrid.h/cpp, PreTranslateMessage functions
				  and the file ComboInListView.cpp/h
                - function to indicate a combobox, it draws a down arrow ..see DrawComboBox
				  this idea came from Robert Bouwens.;).
				- in file TestTreeDlg.cpp:  due to the MeasureItem message-reflection
				  the listctrl is now created in the CDialog::OnCreate function and later on resized
				- LVIS_DROPHILITED support when dragging items
				- Support for LVS_EX_CHECKBOXES
				- Added some examples on how to print preview selected item(or at least to get some information from of the grid)
				  the sample is in the CMySuperGrid.h/cpp which is derived from CSuperGridCtrl it shows you
				  how to initalize the grid, sort items, search for items and subitems, select and delete thems
				- added virtual GetIcon function to set your default icon handler for a listview index.
				- added virtual GetCellRGB() to set the color for current selected cell
				- renamed CTreeNode to CTreeItem, sorry ;-(
				- the nested class CTreeItem has been stripped down to nothing but simple (struct)data members
				  only servering as a linked list of lists now :-)
				- well.. moved all the importent stuff from CTreeItem to CSuperGridCtrl e.g
				  all operations on a CTreeItem now resides in the CSuperGridCtrl, much more OO-like ;-)
				- added Quicksort function
				- got rid of TheOpenCloseThing function.
				- added virtual function OnUpdateListViewItem..called when ever an Item is about to be updated
				- added virtual function OnControlLButtonDown...called when ever LButtondown in a cell
				- added virtual function CreateDragImageEx...

	11 jan 1999:
				- added SetFocus in OnDblclk and in OnLButtonDown, big thank you to Dieter Gollwitzer
				- fixed a minor hittest error in the HitTestOnSign(), again thanks to Dieter Gollwitzer


	16 jan 1999:
				- added virtual OnItemExpanding(....
				- added virtual OnItemExpanded(...
				- added virtual OnCollapsing(....
				- added virtual OnItemCollapsed(...
				- added virtual OnDeleteItem(....
				- added virtual OnVKMultiply(...
				- added virtual OnVkSubTract(..
				- added virtual OnVKAdd(....
				- added SetChildrenFlag();

	1 feb 1999:
				- CItemInfo now supports individual cell-color
				- fixed some bugs in regards to listdata associated with in each cell
				- added virtual BOOL OnVkReturn(........
				- added virtual BOOL OnItemLButtonDown(.....

	4 mai 1999  - moved to Switzerland to work as a systemdeveloper for 2-3 years :).
				- new email :)
				- added new class for encapsulating drawing + - buttons.

   22.june 1999 - Added GetCount()
				- removed support for VC++ 5.0.
				- added GetTreeItem(.....)
				- GetSelectedItem(.....)
				- Howto disable Drag/drop in CMySuperGrid
				- level 4 compiled
				- Clean up some redudant code crap

	30.june 99  - fixed minor error in DrawItem, a drawing error occurred when 1.st column was hidden,
				  it was previous fixed, but some how it was introduced again :).


	9. juli 99  - CRAIG SCHMIDT "the beachboy" ;-)

				  Craig did the basic implementation of the Multiple Root issue

				  he added the following functions :

				  - CTreeItem*  InsertRootItem(.....
				  - void DeleteRootItem(....
				  - BOOL IsRoot(...
				  - CTreeItem* GetRootItem(..
				  - int GetRootIndex(..
				  - int GetRootCount(...
				  - and a member variable, CPtrList m_RootItems.
				  see the file CMySuperGrid.cpp for usage of the InsertRootItem.

   	10.juli 99    added simple wrappers for the rootitems collection

				  -	Added GetRootHeadPosition(...)
				  -	Added GetRootTailPosition();
				  -	Added CTreeItem* GetNextRoot(...
				  -	Added CTreeItem* GetPrevRoot(...


	12. juli 99  - fixed minor error in CreateDragImageEx, an error occurred when 1st column was hidden and a drag operation was initiated

	13. juli 99	 - Added void DeleteAll()...deletes all items in the grid, uhh
				 - Removed GetNextSiblingItem(...), GetPrevSiblingItem(...) due to support for multiple roots.
				 - Added CTreeItem *GetNext(...) and CTreeItem* GetPrev(..) due to support for multiple roots.

	14. juli 99	 - Fixed a memory leak in CMySuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized....

    --
	meantime     - working my ass of with other projects, still am ..
	--

	4. dec. 99	 - Added SortEx, the previous Sort function did not sort RootItems.
				   My thanks goes to Thomas Brammer for pointing this out to me.
				   also he is the guy who added one pixel extra to the MakeShortString ;)
				   this should finally take care of the "header resizing problem".

				 - changed the void CMySuperGrid::SortData() example.
				 *
				 * Today itｴs actually one year ago this code went public (good or bad you decide).
                 *

	8. dec. 99   - Added Arne Schildｴs improvements, see his improvements in the comment section on the website.
				    - redesign of DeleteAll()..and them some.
				    - redesign of MakeShortString.
				    - HitTestOnSign....
				 - Bug fix in DrawItem, did not use the GetData function to get the Item string or substring for drawing.
				 - UpdateData now has a default parametre(BOOL bUpdateRow), which forces the listview item to be written.
				   this means you donｴt have to call UpdateData and CListCtrl::SetItemText afterwards when doing dynamic updates.
				 - added InvalidateItemRect(int nItem);
				 - added InvalidateItemRectPtr(CTreeItem *);
				 - added InvalidateItem..does a basic SetItemText given a CTreeItem ptr
				 - added a test function: DynamicUpdateSomeItems(int nItem) in CMySuperGrid....
				 - Subitems now as support for an image call the
				   SetSubItemText(int iSubItem, const CString& strSubItem, int iImage) function to set the text and image of a subitem.
				   if you just want to set the image call SetSubItemImage(isubitem,iNewImage)
				 - added CItemInfo::GetSubItemImage which returns an image index into an imagelist or -1 given an subitem as parametre
				   this function is called from DrawItem.

				 - added SetSubItemImageList which only copies an imagelist, call this function
				   if you want subitems to support images.

				 - adjusted the DrawItem to account for subitems having a image.
				   this adjustment had impact on
				   - DrawFocusCell
				   - DrawComboBox
				   which now take an extra parametre as argument..of course this is a given subitems new images offet.

                 - added some examples in CMySuperGrid::InitializeGrid() and in CMySuperGrid::DynamicUpdateSomeItems
				   on how to associate an image with a particular subitem.
				 - added an example howto change the image of a subitem see the void CMySuperGrid::SetNewImage function


	WHAT'S UP :
				- NEW RELEASES can in future be found at www.codeproject.com, donｴt forget to subscribe.
				- Peace on earth.
				- Atl version is still on my todo list.
				- Better documentation ;)
				- Visual Developer Studio 7.0.
				- Windows 2001 ;)
*/

/////////////////////////////////////////////////////////////////////////////
// CSuperGridCtrl
CSuperGridCtrl::CSuperGridCtrl()
{
	m_cxImage = m_cyImage = m_bIsDragging = m_CurSubItem = 0;
	m_bMouseLeft = false;
	m_nAutoScrollTimer = 0;
    m_nDragTarget=m_nDragItem = -1;
	m_psTreeLine.CreatePen(PS_SOLID, 1, RGB(192,192,192));
	m_psRectangle.CreatePen(PS_SOLID, 1, RGB(198,198,198));
	m_psPlusMinus.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	m_brushErase.CreateSolidBrush(RGB(255,255,255));
	m_himl = NULL;
	m_bMultiSelect = false;
	m_bMultiSelectLine = false;
	g_bOpenEditWindow = false;
	g_pEditWindow = NULL;
#ifdef SUPORT_DRAGDROP
	m_redrawcount = 0;
#endif
}


CSuperGridCtrl::~CSuperGridCtrl()
{
	m_psPlusMinus.DeleteObject();
	m_psRectangle.DeleteObject();
	m_psTreeLine.DeleteObject();
	m_brushErase.DeleteObject();

	while(m_RootItems.GetCount())
	{
		CTreeItem * root = (CTreeItem*)m_RootItems.RemoveHead();
		if(root!=NULL && GetData(root) != NULL)
			delete GetData(root);
		delete root;
	}
	m_RootItems.RemoveAll();
}



BEGIN_MESSAGE_MAP(CSuperGridCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSuperGridCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperGridCtrl message handlers

BOOL CSuperGridCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS;
	return CListCtrl::PreCreateWindow(cs);
}



int CSuperGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ヘッダのサブクラス化
//	CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
//	m_headerCtrl.SubclassWindow(pHeader->GetSafeHwnd());

	return 0;
}



#define OFFSET_FIRST	2
#define OFFSET_OTHER	6
void CSuperGridCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_LISTVIEW)
        return;

	if(lpDrawItemStruct->itemAction == ODA_DRAWENTIRE)
	{
		if(m_himl==NULL)
		{
			m_himl = (HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, (WPARAM)(int)(LVSIL_SMALL), 0L);
			if(m_himl==NULL)
				return;
		}

		LV_ITEM lvi;
		static _TCHAR szBuff[MAX_PATH];
		LPCTSTR pszText;

		int nItem = lpDrawItemStruct->itemID;
		CRect rcItem(lpDrawItemStruct->rcItem);

		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM;
		lvi.iItem = nItem;
		lvi.iSubItem =0;
		lvi.pszText = szBuff;
		lvi.cchTextMax = sizeof(szBuff);
		lvi.stateMask = 0xFFFF;
		GetItem(&lvi);

		CTreeItem *pSelItem = (CTreeItem*)lpDrawItemStruct->itemData;

		CRect rcLabel;
		GetItemRect(nItem, rcLabel, LVIR_LABEL);

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		ASSERT(pDC);

		CFont* pFont = GetFont();
		CFont* pOldFont = pDC->SelectObject(pFont);

		CRect rcClipBox;
		pDC->GetClipBox(rcClipBox);

		COLORREF crBackground, crText;

		if (m_bMultiSelect == true && m_bMultiSelectLine == false) {
			// 範囲選択中の場合
			if (GetSelectedItem() == (int)lpDrawItemStruct->itemID) {
				// 選択されている行
				crBackground = GetSysColor(COLOR_HIGHLIGHT);
				crText = GetSysColor (COLOR_HIGHLIGHTTEXT);
			} else {
				// 選択されていない行
				crBackground = GetSysColor(COLOR_WINDOW);
				crText = GetSysColor (COLOR_WINDOWTEXT);
			}
		} else {
			// 範囲選択中ではない場合
			if (lpDrawItemStruct->itemState & ODS_SELECTED) {
				// 選択されている行
				crBackground = GetSysColor(COLOR_HIGHLIGHT);
				crText = GetSysColor (COLOR_HIGHLIGHTTEXT);
			} else {
				// 選択されていない行
				crBackground = GetSysColor(COLOR_WINDOW);
				crText = GetSysColor (COLOR_WINDOWTEXT);
			}
		}

		//Get current Icon, you have overridden this I hope :-)
		int iImage = GetIcon(pSelItem);
		if(iImage!=-1)
		{
			if(lvi.iImage!=iImage)
			{
				LV_ITEM lvItem;
				lvItem.mask =  LVIF_IMAGE;
				lvItem.iImage = iImage;
				lvItem.iItem = nItem;
				lvItem.iSubItem = 0;
				SetItem(&lvItem);
				lvi.iImage = iImage;
			}
		}

		CRect rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);

		CRect rcIcon;
		GetItemRect(nItem, rcIcon, LVIR_ICON);
		//Draw Current image
		int nOffset = (rcItem.Height() - m_cyImage)/2;
		int nY = rcItem.bottom - m_cyImage - nOffset;
		int nWidth = m_cxImage;//def icon size
		//do not draw icon out side 1.st column.
		int iTryIndent = GetIndent(pSelItem) * m_cxImage + m_cxImage;
		if(rc.left + iTryIndent  > GetColumnWidth(0)-2/*looks better -2*/)
			nWidth = (rc.left + iTryIndent) - GetColumnWidth(0);

		UINT uiFlags = ILD_TRANSPARENT;

		if( GetItemState(nItem, LVIF_STATE) & LVIS_DROPHILITED)//if dragging show a SelectDropTarget alike effect :)
			uiFlags |= ILD_BLEND50;

		if((nWidth=m_cxImage-nWidth) >-1)//calc width of icon
		{
			ImageList_DrawEx(m_himl, lvi.iImage, pDC->m_hDC,
							 rc.left + (GetIndent(pSelItem) * m_cxImage),
							 nY,
							 nWidth,
							 m_cyImage,
							 CLR_DEFAULT,
							 CLR_DEFAULT,
							 uiFlags);
			DrawTreeItem(pDC, pSelItem, nItem, rc);
			// ルートは違うフォントで描画
			//if (ItemHasChildren(pSelItem)) {
			//	pDC->SelectObject(pOldFont);
			//	pOldFont = pDC->SelectObject(m_rootItemFont);
			//}
		}

		//Draw selection bar (erase old selection too)
		pDC->SetBkColor(crBackground);

		CRect rcClip = lpDrawItemStruct->rcItem;
		rcClip.left += GetIndent(pSelItem) * m_cxImage + m_cxImage + 2;
		if(rcClip.left > GetColumnWidth(0))
			rcClip.left = GetColumnWidth(0);
		//fill background color
		ExtTextOut(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, rcClip, NULL, 0, NULL);

		//draw color in first col if any
		rcClip.right = rcLabel.right;
		CItemInfo *pItemInfo = GetData(pSelItem);
		COLORREF clf = pItemInfo->GetItemClr();
		if (clf != -1) {
			CBrush br(clf);
			pDC->FillRect(rcClip, &br);
		}
		//draw selection rect in 1.st col if selected
//		if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (m_CurSubItem==0)) {
		if (GetSelectedItem() == (int)lpDrawItemStruct->itemID && (m_CurSubItem==0)) {
			CBrush br(GetCellRGB());
			pDC->FillRect(rcClip,&br);
			pDC->DrawFocusRect(rcClip);
		}

		//if checkbox style
		UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
		if (nStateImageMask)
		{
			int nImage = (nStateImageMask>>12) - 1;
			extern	CImageList	g_imageCheckMark;
			CImageList *pImageList = &g_imageCheckMark;

			if (pItemInfo->GetLParam() < 0) {
				// ディレクトリの場合
				if (pItemInfo->GetFlagExt() & CItemInfo::FLAG_EX_CHECK_GRAY) {
					nImage = 2;
				}
				// ルートは違うフォントで描画
				//pDC->SelectObject(pOldFont);
				pOldFont = pDC->SelectObject(m_rootItemFont);
			}

			if (pImageList) {
				int		cxIcon,cyIcon=0;
				ImageList_GetIconSize(pImageList->m_hImageList, &cxIcon, &cyIcon);
				if(rc.left + (GetIndent(pSelItem) * m_cxImage) + m_cxImage + cxIcon < GetColumnWidth(0))
					pImageList->Draw(pDC, nImage,CPoint(rc.left + (GetIndent(pSelItem) * m_cxImage) + cxIcon, nY), ILD_TRANSPARENT);
			}
		}

		//thatｴs new
		CString strItemText = GetData(pSelItem)->GetItemText();
		//draw 1. item
		GetItemRect(nItem, rcItem, LVIR_LABEL);
		/* Conspiracy 199 */
		CRect rcItemRectSave = rcItem;
		if (pItemInfo->GetLParam() < 0 && g_bAudioListShow) {
			LV_COLUMN lvc;
			lvc.mask = LVCF_FMT | LVCF_WIDTH;
			for(int nColumn=1; GetColumn(nColumn, &lvc) &&nColumn <= 3; nColumn++) {
				rcItem.right+=lvc.cx;
			}
		}
		pszText = MakeShortString(pDC, strItemText.GetBuffer(1)/*szBuff*/, rcItem.right - rcItem.left, OFFSET_FIRST);
		rcLabel = rcItem;
		rcLabel.left += OFFSET_FIRST;
		rcLabel.right -= OFFSET_FIRST;
		rcItem = rcItemRectSave; /* Conspiracy 199 */

		pDC->SetBkColor(crBackground);
		pDC->SetTextColor(crText);
		//pDC->SelectObject(GetFont());
		pDC->DrawText(pszText,-1, rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

		//draw subitems..
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;
		for(int nColumn=1; GetColumn(nColumn, &lvc); nColumn++)
		{
			int nSubItemWidth=0;
			rcItem.left=rcItem.right;
			rcItem.right+=lvc.cx;
			if (rcItem.left < rcClipBox.right && rcItem.right > rcClipBox.left && rcItem.right > rcItem.left)
			{
				//support for colors in each cell
				COLORREF clf = pItemInfo->GetBkColor(nColumn-1);
				if(clf != -1) {
					CBrush br(clf);
					pDC->FillRect(rcItem, &br);
				}

				// ユーザーによるセルの背景描画処理
				BOOL	bChangeTextColor;
				bChangeTextColor = OnDrawCustomCellBG(pDC, nItem, nColumn, rcItem);

				//check for subitems who has an image associated
				int iSubImage = pItemInfo->GetSubItemImage(nColumn-1);
				if (iSubImage != -1) {
					//m_cxImage is actually the width of the "tree" imagelist not your subitem imagelist..
					//remember that, when you create your bitmap, I was to lazy getting the icon size of the subitem imagelist
					//in a perfect world you would get the icon size of the subitem image list...
					nSubItemWidth = m_cxImage;
					CRect rect;
					GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
					HIMAGELIST himl = m_iSubItemImage.GetSafeHandle();
					ImageList_DrawEx(himl, iSubImage, pDC->m_hDC,
									 rect.left, nY, nSubItemWidth, m_cyImage,
									 CLR_DEFAULT,CLR_DEFAULT, uiFlags);
					nSubItemWidth+=2;//+2 arhh you decide
				}

			    CString str = GetData(pSelItem)->GetSubItem(nColumn-1);

				pszText = MakeShortString(pDC,str.GetBuffer(1),(rcItem.right - rcItem.left) - nSubItemWidth, OFFSET_OTHER);

				UINT nJustify=DT_LEFT;

				if(pszText==str.GetBuffer(1))
				{
					switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
					{
					case LVCFMT_RIGHT:
						nJustify=DT_RIGHT;
						break;
					case LVCFMT_CENTER:
						nJustify=DT_CENTER;
						break;
					default:
						break;
					}
				}

				rcLabel=rcItem;
				rcLabel.left+=OFFSET_OTHER + nSubItemWidth;
				rcLabel.right-=OFFSET_OTHER;
				if (lpDrawItemStruct->itemState & ODS_SELECTED && !m_bIsDragging)
					DrawFocusCell(pDC, lpDrawItemStruct->itemID, nColumn, nSubItemWidth);
				if (pszText != NULL) {
					COLORREF	colOld;
					if (bChangeTextColor) {
						colOld = pDC->SetTextColor(RGB(255, 255, 64));
					}
					//pDC->DrawText(pszText,-1,rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
					MyDrawText(pDC, pszText,-1,rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
					if (bChangeTextColor) {
						pDC->SetTextColor(colOld);
					}
				}
			}//if

			//draw down arrow if combobox regardless of rcClipBox rgn
			if (lpDrawItemStruct->itemState & ODS_SELECTED) {
				if (GetSelectedCount() == 1 && GetSelectedItem() == (int)lpDrawItemStruct->itemID) {
					DrawComboBox(pDC, pSelItem, nItem, nColumn, nSubItemWidth);
				}
			}
		}//for
		pDC->SelectObject(pOldFont);
	}//ODA_DRAWENTIRE
}



void CSuperGridCtrl::DrawComboBox(CDC* pDC, CTreeItem *pSelItem, int nItem, int nColumn,int iSubIconOffset)
{
	CItemInfo* pInfo = GetData(pSelItem);
	CONTROLTYPE ctrlType;
	if(pInfo->GetControlType(nColumn-1, ctrlType))
	{
		if(ctrlType == combobox)
		{
			CRect rect;
			GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
			rect.left=rect.right - GetSystemMetrics(SM_CYVSCROLL);
			pDC->DrawFrameControl(rect, DFC_SCROLL, DFCS_SCROLLDOWN);
		}
		//you could draw your own control here
	}
}


//this piece of code is borrowed from the wndproc.c file in the odlistvw.exe example from MSDN and was converted to mfc-style
void CSuperGridCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (lpMeasureItemStruct->CtlType != ODT_LISTVIEW)
        return;

	TEXTMETRIC tm;
	CClientDC dc(this);
	CFont* pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);
	dc.GetTextMetrics(&tm);
	int nItemHeight = tm.tmHeight + tm.tmExternalLeading;
	lpMeasureItemStruct->itemHeight = nItemHeight + 4; //or should I go for max(nItemheight+4, m_cxImage+2);
	dc.SelectObject(pOldFont);
}



//the basic rutine making the ... thing snatched it from some tedious code example from the MFC rowlist example
//Arne Schildｴ had a comment on the static const _TCHAR thing. this is now changed
LPCTSTR CSuperGridCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[]=_T("...");
	static CString csShort;

	int nStringLen=lstrlen(lpszLong);
	if (nStringLen > 256) { /* 2003.06.20 add */
		nStringLen = 256;
	}

	if(nStringLen==0 || pDC->GetTextExtent(lpszLong,nStringLen).cx + nOffset < nColumnLen+1)
		return(lpszLong);

	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	csShort = lpszLong;

	int i; for(i=nStringLen-1; i > 0; i--)
	{
		csShort = csShort.Left( csShort.GetLength() - 1 );
		if(pDC->GetTextExtent(csShort,i).cx + nOffset + nAddLen < nColumnLen)
			break;
	}
	BOOL bLead = FALSE;
	for (i=0;i<csShort.GetLength();i++) {
		if (_ismbblead(csShort[i]) && !bLead) {
			bLead = TRUE;
		} else {
			bLead = FALSE;
		}
	}
	if (bLead) {
		csShort.Delete(csShort.GetLength() - 1);
	}
	csShort += szThreeDots;
	return csShort;
}




void CSuperGridCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	switch(pLVKeyDow->wVKey)
	{
		case VK_MULTIPLY:
			{
				int nIndex = GetSelectedItem();
				if(nIndex != -1)
				{
					CWaitCursor wait;
					SetRedraw(0);
					CTreeItem *pParent = GetTreeItem(nIndex);
					int nScroll=0;
					if(OnVKMultiply(pParent, nIndex))
					{
						ExpandAll(pParent, nScroll);
					}
					SetRedraw(1);
					RedrawItems(nIndex, nScroll);
					EnsureVisible(nScroll, TRUE);
				 }
			 }break;

		case VK_ADD:
			{
					int nIndex = GetSelectedItem();
					if(nIndex!=-1)
					{
						CWaitCursor wait;
						CTreeItem *pSelItem = GetTreeItem(nIndex);
						int nScrollIndex = 0;
						if(OnVKAdd(pSelItem, nIndex))
						{
							 nScrollIndex = Expand(pSelItem, nIndex);
						}
						InvalidateItemRect(nIndex);
						EnsureVisible(nScrollIndex, 1);
					}

			}break;


		case VK_SUBTRACT:
			{
				int nIndex = GetSelectedItem();
				if(nIndex!=-1)
				{
					CWaitCursor wait;
					CTreeItem *pSelItem = GetTreeItem(nIndex);
					if(OnVkSubTract(pSelItem, nIndex))
					{
						Collapse(pSelItem);
					}
					InvalidateItemRect(nIndex);
				}
			}break;
#if SUPORT_DRAGDROP
		case VK_UP:
			if (GetAsyncKeyState(VK_MENU) & 0x8000) {
				int nIndex = GetSelectedItem();
				if(nIndex > 1)
				{
					//SetRedraw(0);
					CTreeItem *pItem = GetTreeItem(nIndex);
					CItemInfo* lp = GetData(pItem);
					CTreeItem *pPrevItem = GetTreeItem(nIndex-1);
					CItemInfo* lpPrev = GetData(pPrevItem);
					if (((int)lp->GetLParam() < 0)) {// 選択されているのがフォルダ
						if (nIndex == 1)	break;
						if (GetIndent(pItem) == GetIndent(pPrevItem)) {
							pPrevItem = GetTreeItem(nIndex-2);
							lpPrev = GetData(pPrevItem);
						}
						if (((int)lpPrev->GetLParam() < 0)) {
							// １つ上がフォルダ => そのフォルダさらに１つ上の末尾に置く
							//SetRedraw(0);
							if (nIndex-2 > 0) {
								pPrevItem = GetTreeItem(nIndex-2);
							} else {
								pPrevItem = GetTreeItem(0);
							}
							CItemInfo* lpPrev = GetData(pPrevItem);
							//if (!((int)lpPrev->GetLParam() < 0)) {
							//	pPrevItem = GetParentItem(pPrevItem);
							//}
						}
						//SetRedraw(0);
						if(DoDragDrop(pPrevItem, pItem)) {
							UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
							SetItemState(NodeToIndex(pPrevItem)+1, uflag, uflag);
							//delete source
							int nIndex = NodeToIndex(pItem);
							DeleteItem(nIndex);
							HideChildren(pItem, TRUE, nIndex);
							Delete(pItem);
							InternaleUpdateTree();
							OnUpdateTotal();
							InvalidateRect(NULL);
							UpdateWindow();
						}
						//SetRedraw(1);
						break;
					}
					if (((int)lpPrev->GetLParam() < 0)) {
						// １つ上がフォルダ => そのフォルダさらに１つ上の末尾に置く
						//SetRedraw(0);
						if (nIndex-2 > 0) {
							pPrevItem = GetTreeItem(nIndex-2);
						} else {
							pPrevItem = GetTreeItem(0);
						}
						CItemInfo* lpPrev = GetData(pPrevItem);
						if (!((int)lpPrev->GetLParam() < 0)) {
							pPrevItem = GetParentItem(pPrevItem);
						}
						CTreeItem* pNewItem = InsertItem(pPrevItem, CopyData(lp), TRUE, INSERT_FILE);
						OnRefreshTreeItem(pItem, pNewItem); /* STEP 037 */
						int nIndex = NodeToIndex(pItem);
						DeleteItem(nIndex);
						HideChildren(pItem, TRUE, nIndex);
						Delete(pItem);
						InternaleUpdateTree();
						OnUpdateTotal();
						UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
						SetItemState(NodeToIndex(pNewItem), uflag, uflag);
						//SetRedraw(1);
						break;
					}
					if(DoDragDrop(pItem, pPrevItem)) {
						UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
						SetItemState(NodeToIndex(pPrevItem)+1, uflag, uflag);
						//delete source
						int nIndex = NodeToIndex(pPrevItem);
						DeleteItem(nIndex);
						HideChildren(pPrevItem, TRUE, nIndex);
						Delete(pPrevItem);
						InternaleUpdateTree();
						//OnUpdateTotal();
						//InvalidateRect(NULL);
						InvalidateItemRect(nIndex);
						InvalidateItemRect(nIndex-1);
						//UpdateWindow();
					}
					int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) - 1;//GetSelectedItem();
					EnsureVisible(nIndex, FALSE); /* STEP 011 */
				}
			}
			break;
		case VK_DOWN:
			if (GetAsyncKeyState(VK_MENU) & 0x8000) {
				//int nIndex = GetNextItem(GetItemCount()-1, LVNI_ABOVE | LVNI_SELECTED);//GetSelectedItem();
				int nIndex = GetSelectedItem();
				if(nIndex > 0 && GetItemCount() > (nIndex+1)) {
					//SetRedraw(0);
					CTreeItem *pItem = GetTreeItem(nIndex);
					CItemInfo* lp = GetData(pItem);
					if (((int)lp->GetLParam() < 0)) {// 選択されているのがフォルダ
						// 次のフォルダを探す
						CTreeItem *pNextItem = NULL;
						for (int i=nIndex+1;i<GetItemCount();i++) {
							pNextItem = GetTreeItem(i);
							if (pItem->m_nIndent >= pNextItem->m_nIndent) {
								break;
							}
							pNextItem = NULL;
							// 移動元よりインデントが後ろにあるところには移動できない => 次を探す
						}
						if (pNextItem == NULL) {
							if (GetIndent(pItem) > 2) {
								//SetRedraw(0);
								BOOL bExpand = !IsCollapsed(pItem);
								pNextItem = GetParentItem(GetParentItem(pItem));
								CTreeItem* pNewItem = InsertItem(pNextItem, CopyData(lp), TRUE, INSERT_TAIL);
								OnRefreshTreeItem(pItem, pNewItem); /* STEP 037 */
								if(ItemHasChildren(pItem))
									CopyChildren(pNewItem, pItem);
								int nIndex = NodeToIndex(pItem);
								DeleteItem(nIndex);
								HideChildren(pItem, TRUE, nIndex);
								Delete(pItem);
								InternaleUpdateTree();
								//OnUpdateTotal();
								UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
								SetItemState(NodeToIndex(pNewItem), uflag, uflag);
								if (bExpand) Expand(pNewItem, NodeToIndex(pNewItem));
								Invalidate();
								UpdateWindow();
								//SetRedraw(1);
							}
							break;
						}
						//SetRedraw(0);
						if(DoDragDrop(pNextItem, pItem)) {
							UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
							SetItemState(NodeToIndex(pNextItem)+1, uflag, uflag);
							//delete source
							int nIndex = NodeToIndex(pItem);
							DeleteItem(nIndex);
							HideChildren(pItem, TRUE, nIndex);
							Delete(pItem);
							InternaleUpdateTree();
							//OnUpdateTotal();
							InvalidateRect(NULL);
							UpdateWindow();
						}
						break;
					}
					CTreeItem *pNextItem = GetTreeItem(nIndex+1);
					CItemInfo* lpNext = GetData(pNextItem);
					if (((int)lpNext->GetLParam() < 0)) {
						// １つ下がフォルダ => そのフォルダ直下に置く
						CTreeItem* pNewItem = InsertItem(pNextItem, CopyData(lp), TRUE, INSERT_HEAD);
						OnRefreshTreeItem(pItem, pNewItem); /* STEP 037 */
						int nIndex = NodeToIndex(pItem);
						DeleteItem(nIndex);
						HideChildren(pItem, TRUE, nIndex);
						Delete(pItem);
						InternaleUpdateTree();
						OnUpdateTotal();
						UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
						SetItemState(NodeToIndex(pNewItem), uflag, uflag);
						break;
					}
					if(DoDragDrop(pNextItem, pItem)) {
						UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
						SetItemState(NodeToIndex(pNextItem)+1, uflag, uflag);
						int nIndex = NodeToIndex(pItem);
						DeleteItem(nIndex);
						HideChildren(pItem, TRUE, nIndex);
						Delete(pItem);
						InternaleUpdateTree();
						InvalidateItemRect(nIndex);
						InvalidateItemRect(nIndex+1);
					}
					int nIndex = GetSelectedItem() + 1;
					EnsureVisible(nIndex, FALSE); /* STEP 011 */
				}
			}
#endif
		default :break;
	}
	*pResult = 0;
	SetRedraw(1); /* STEP 011 */
}



BOOL CSuperGridCtrl::HitTestOnSign(CPoint point, LVHITTESTINFO& ht)
{
	ht.pt = point;
	// Test which subitem was clicked.
	SubItemHitTest(&ht);
	if(ht.iItem!=-1)
	{
		//first hittest on checkbox
		BOOL bHit = FALSE;
		if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
		{
			if (ht.flags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))//isn't this allways ownerdrawfixed :-)
			{
				CRect rcIcon,rcLabel;
				GetItemRect(ht.iItem, rcIcon, LVIR_ICON);//has to be between these two ....right :)
				GetItemRect(ht.iItem, rcLabel, LVIR_LABEL);
				// check if hit was on a state icon
				if (point.x > rcIcon.left && point.x < rcLabel.left)
					bHit = TRUE;
			}
			else if (ht.flags & LVHT_ONITEMSTATEICON)
				bHit = TRUE;
		}

		CTreeItem* pItem = GetTreeItem(ht.iItem);
		if(pItem!=NULL)
		{
			if(bHit)//if checkbox
			{
				//yes I know..have to maintain to sets of checkstates here...
				//one for listview statemask and one for CTreeItem..but its located here so no harm done
				SetCheckEx(ht.iItem, !GetCheck(ht.iItem));
			}
			//if haschildren and clicked on + or - then expand/collapse
			if(ItemHasChildren(pItem))
			{
				//hittest on the plus/sign "button"
				CRect rcBounds;
				GetItemRect(ht.iItem, rcBounds, LVIR_BOUNDS);
				CRectangle rect(this, NULL, GetIndent(pItem), rcBounds);
				BOOL bRedraw=0;//if OnItemExpanding or OnCollapsing returns false, dont redraw
				if(rect.HitTest(point))
				{
					SetRedraw(0);

					//Arne Schild///////////////////////////////////////////////
					int nScrollIndex = GetCurIndex( pItem ); // AND NOT = 0 !!!
					/////////////////////////////////////////////////////////////

					if(IsCollapsed(pItem))
					{
						if(OnItemExpanding(pItem, ht.iItem))
						{
							nScrollIndex = Expand(pItem, ht.iItem);
							OnItemExpanded(pItem, ht.iItem);
							bRedraw=1;
						}
					}
					else {
					   if(OnCollapsing(pItem))
					   {
							Collapse(pItem);
							OnItemCollapsed(pItem);
							bRedraw=1;
					   }
					}
					SetRedraw(1);
					if(bRedraw)
					{
						InvalidateItemRect(ht.iItem);
						EnsureVisible(nScrollIndex, 1);
						return 0;
					}
				}
			}//has kids
		}//pItem!=NULL
	}
	return 1;
}



void CSuperGridCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	if( GetFocus() != this)
		SetFocus();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO ht;
	ht.pt = pNMListView->ptAction;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht))
	{
		BOOL bSelect=1;
		bSelect = HitTestOnSign(pNMListView->ptAction, ht);
		//normal selection
		if(bSelect && ht.iItem !=-1)
		{
			int nIndex = GetSelectedItem();
			if(nIndex!=-1)
			{
				CTreeItem *pSelItem = GetTreeItem(nIndex);
				if (pSelItem != NULL)
				{
					BOOL bRedraw=0;
					if(ItemHasChildren(pSelItem))
					{
						SetRedraw(0);
						int nScrollIndex=0;
						if(IsCollapsed(pSelItem))
						{
							if(OnItemExpanding(pSelItem, nIndex))
							{
								nScrollIndex = Expand(pSelItem, nIndex);
								OnItemExpanded(pSelItem, nIndex);
								bRedraw=1;
							}
						}

						else
						{
						   if(OnCollapsing(pSelItem))
						   {
								Collapse(pSelItem);
								OnItemCollapsed(pSelItem);
								bRedraw=1;
						   }
						}
						SetRedraw(1);

						if(bRedraw)
						{
							InvalidateItemRect(nIndex);
							EnsureVisible(nScrollIndex,1);
						}
					} else {
						if (ht.iSubItem < COLUMN_MIN) {
							// アイテムがダブルクリックされた
							OnItemDblClick(nIndex);
						} else {
							// 項目編集
							PostMessage(WM_KEYDOWN, VK_F2, NULL);
						}
					}
				}//!=NULL
			}
		}
	}
	*pResult = 0;
}

// アイテムがダブルクリックされた
BOOL CSuperGridCtrl::OnItemDblClick(int)
{
	return(TRUE);
}


void CSuperGridCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
#if SUPORT_DRAGDROP
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		m_nDragItem = pNMListView->iItem;
		CImageList* pDragImage=NULL;
		pDragImage = CreateDragImageEx(m_nDragItem);//override this if you want another dragimage or none at all.
		if(pDragImage)
		{
			pDragImage->BeginDrag(0, CPoint(0,0));
			pDragImage->DragEnter(this, pNMListView->ptAction);
			SetCapture();
			m_bIsDragging = TRUE;
		}
		delete pDragImage;
	}
#endif
	*pResult = 0;
}




//Create dragimage : Icon + the itemtext
CImageList *CSuperGridCtrl::CreateDragImageEx(int nItem)
{
    CImageList *pList = new CImageList;
	//get image index
	LV_ITEM lvItem;
	lvItem.mask =  LVIF_IMAGE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	GetItem(&lvItem);

	CRect rc;
	GetItemRect(nItem, &rc, LVIR_BOUNDS);

	CString str;
	str = GetItemText(nItem, 0);
	CFont *pFont = GetFont();

	rc.OffsetRect(-rc.left, -rc.top);
	rc.right = GetColumnWidth(0);
	pList->Create(rc.Width(), rc.Height(),ILC_COLOR24| ILC_MASK , 1, 1);
	CDC *pDC = GetDC();
	if(pDC)
	{
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap bmpMap;
		bmpMap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());

		CBitmap *pOldBmp = dc.SelectObject(&bmpMap);
		CFont *pOldFont = dc.SelectObject(pFont);
		dc.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));
		CImageList *pImgList = GetImageList(LVSIL_SMALL);
		if(pImgList)
			pImgList->Draw(&dc, lvItem.iImage, CPoint(0,0), ILD_TRANSPARENT);
		dc.TextOut(m_cxImage + 4, 0, str);
		dc.SelectObject(pOldFont);
		dc.SelectObject(pOldBmp);
		//causes an error if the 1st column is hidden so must check the imagelist
		if(pList->m_hImageList != NULL)
			pList->Add(&bmpMap, RGB(255,255,255));
		else {
			delete pList;
			pList=NULL;
		}
		ReleaseDC(pDC);
	}
	return pList;
}



void CSuperGridCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bMouseLeft) {
		LVHITTESTINFO ht;
		ht.pt = point;
		SubItemHitTest(&ht);
		if (m_CurSubItem == 0) {
			// 行の範囲選択
			int		sy, ey;
			if (m_posMultiSelect[0].y < ht.iItem) {
				sy = m_posMultiSelect[0].y;
				ey = ht.iItem;
			} else {
				sy = ht.iItem;
				ey = m_posMultiSelect[0].y;
			}
//			minmax3(m_posMultiSelect[0].y, m_posMultiSelect[1].y, ht.iItem, sy, ey);
			if (m_posMultiSelect[1].y != ht.iItem) {
				m_posMultiSelect[1].y = ht.iItem;
				if (sy >= 0) {
					SelectAllItems(FALSE);
					for (int y = sy; y <= ey; y++) {
						SetItemState(y, LVIS_SELECTED, LVIS_SELECTED);
					}
					SetItemState(ht.iItem, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
				}
			}
		} else {
			// セルの範囲選択
			if (ht.iItem != m_posMultiSelect[1].y) {
				// 全行選択解除
				SelItemRange(FALSE, 0, -1);

				SetItemState(ht.iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				OnChangeSelect();			// 選択が変更されたことを通知
			}
			if (ht.iSubItem != m_posMultiSelect[1].x) {
				// 桁の選択変更
				int		nOrder = IndexToOrder(ht.iSubItem);
				if (nOrder != -1) {
					// 桁の選択変更
					m_CurSubItem = nOrder;
					OnChangeSelect();		// 選択が変更されたことを通知
				}
			}
		}
	}
#if SUPORT_DRAGDROP
    else if(m_bIsDragging)
    {

		KillTimer(1);
		if (CWnd::GetCapture() != this)
			m_bIsDragging=0;

		if(nFlags==MK_RBUTTON || nFlags==MK_MBUTTON)
			m_bIsDragging=0;


		if(GetKeyState(VK_ESCAPE) < 0)
			m_bIsDragging=0;

		if(!m_bIsDragging)//why not put this in a funtion :)
		{
			SetItemState (m_nDragTarget, 0, LVIS_DROPHILITED);
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			InvalidateRect(NULL);
			UpdateWindow();
		}
		else
		{
			CPoint ptList(point);
			MapWindowPoints(this, &ptList, 1);
			CImageList::DragMove(ptList);
			UINT uHitTest = LVHT_ONITEM;
			m_nDragTarget = HitTest(ptList, &uHitTest);
			// try turn off hilight for previous DROPHILITED state
			int nPrev = GetNextItem(-1,LVNI_DROPHILITED);
			if(nPrev != m_nDragTarget)//prevents flicker
				SetItemState(nPrev, 0, LVIS_DROPHILITED);

			CRect rect;
			GetClientRect (rect);
			int cy = rect.Height();
			if(m_nDragTarget!=-1)
			{
				SetItemState(m_nDragTarget, LVIS_DROPHILITED, LVIS_DROPHILITED);
				CTreeItem* pTarget = GetTreeItem(m_nDragTarget);
				if ((point.y >= 0 && point.y <= m_cyImage) || (point.y >= cy - m_cyImage && point.y <= cy) ||
					( pTarget!=NULL && ItemHasChildren(pTarget) && IsCollapsed(pTarget)))
				{
					SetTimer(1, 3000, NULL);
				}
			}
		}
    }
#endif

	CListCtrl::OnMouseMove(nFlags, point);
}



void CSuperGridCtrl::OnTimer(UINT nIDEvent)
{
	CListCtrl::OnTimer(nIDEvent);
	if(nIDEvent == 1) {
		if(CWnd::GetCapture() != this) {
			SetItemState(m_nDragTarget, 0, LVIS_DROPHILITED);
			m_bIsDragging=0;
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			InvalidateRect(NULL);
			UpdateWindow();
			KillTimer(1);
			return;
		}

		SetTimer(1,300,NULL);//reset timer
		DWORD dwPos = ::GetMessagePos();
		CPoint ptList(LOWORD(dwPos),HIWORD(dwPos));
		ScreenToClient(&ptList);

		CRect rect;
		GetClientRect(rect);
		int cy = rect.Height();
		//
		// perform autoscroll if the cursor is near the top or bottom.
		//
		if (ptList.y >= 0 && ptList.y <= m_cyImage)
		{
			int n = GetTopIndex();
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer(1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		else if (ptList.y >= cy - m_cyImage && ptList.y <= cy)
		{
			int n = GetTopIndex();
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer (1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		//Hover test
		CImageList::DragMove(ptList);
		UINT uHitTest = LVHT_ONITEM;
		m_nDragTarget = HitTest(ptList, &uHitTest);

		if(m_nDragTarget!=-1)
		{
			//if the target has children
			//expand them
			CTreeItem* pTarget=GetTreeItem(m_nDragTarget);
			if(pTarget != NULL && ItemHasChildren(pTarget) && IsCollapsed(pTarget) && (m_nDragItem!=-1))
			{
				CImageList::DragShowNolock(0);
				CTreeItem* pSource = GetTreeItem(m_nDragItem);

				SetRedraw(0);
				int nScrollIndex=0;
				if(ItemHasChildren(pTarget) && IsCollapsed(pTarget))
				{
					if(OnItemExpanding(pTarget, m_nDragTarget))
					{
						nScrollIndex = Expand(pTarget, m_nDragTarget);
						OnItemExpanded(pTarget, m_nDragTarget);
					}
				}
				m_nDragItem = NodeToIndex(pSource);
				SetRedraw(1);
				EnsureVisible(nScrollIndex, 1);
				InvalidateRect(NULL);
				UpdateWindow();
				CImageList::DragShowNolock(1);
				KillTimer(1);
				return;
			}
		}
		KillTimer(1);
	}

	if (nIDEvent == 2 && CWnd::GetCapture() == this) {
		m_nAutoScrollTimer -= 5;
		if (m_nAutoScrollTimer < 20) m_nAutoScrollTimer = 20;
		SetTimer(2, m_nAutoScrollTimer, NULL);		//reset timer

		// マウス座標を取得
		DWORD dwPos = ::GetMessagePos();
		CPoint pClient(LOWORD(dwPos),HIWORD(dwPos));
		ScreenToClient(&pClient);

		// クライアントサイズを取得
		CRect rect;
		GetClientRect(rect);

		// 上下スクロール
		if (pClient.y < m_cyImage) {
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			if (m_nAutoScrollTimer == 20) {
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			}
		} else if (pClient.y > rect.Height()) {
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			if (m_nAutoScrollTimer == 20) {
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			}
		}

		// 左右スクロール
		if (pClient.x < 0) {
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
		} else if (pClient.x > rect.Width()) {
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
		}
	}
}



void CSuperGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bMouseLeft) {
		KillTimer(2);
		m_bMouseLeft = false;
		ReleaseCapture();					// マウスのキャプチャ終了

		if (m_CurSubItem == 0) {
			// 選択が変更されたことを通知
			OnChangeSelect(); /* WildCherry 072 */
//			if (m_posMultiSelect[1].y >= 0) {
//				// 選択マーク更新
//				SetSelectionMark(m_posMultiSelect[1].y);
//			}
		} else {
			// 範囲選択を行っていない場合は選択解除
			if (m_posMultiSelect[0].x == m_posMultiSelect[1].x
			&&  m_posMultiSelect[0].y == m_posMultiSelect[1].y) {
				m_bMultiSelect = false; /* LastTrain 062 */
				OnChangeSelect();		// 選択が変更されたことを通知
			} else {
				OnMouseMove(0, point);			// 選択を更新させる
			}
		}
	}
#if SUPORT_DRAGDROP
	else if(m_bIsDragging == TRUE)
    {
		KillTimer(1);
        CImageList::DragLeave(this);
        CImageList::EndDrag();
        ReleaseCapture();
        m_bIsDragging = FALSE;
		SetItemState(m_nDragTarget, 0, LVIS_DROPHILITED);
        if((m_nDragTarget != -1) && (m_nDragTarget != m_nDragItem) && (m_nDragItem!=-1))//no drop on me self
        {

			CTreeItem* pSource = GetTreeItem(m_nDragItem);
			CTreeItem* pTarget = GetTreeItem(m_nDragTarget);
			if(IsRoot(pSource))
				return;
			CTreeItem* pParent = GetParentItem(pSource);
//			if(pParent==pTarget) //can't drag child to parent
//				return;

			if(!IsChildOf(pSource, pTarget) || pParent == pTarget)//can't drag parent to child
			{
				CWaitCursor wait;
				SetRedraw(0);
				if(DoDragDrop(pTarget, pSource))
				{
					UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
					SetItemState(m_nDragTarget+1/* SUPORT_DRAGDROP */, uflag, uflag);
					m_nDragItem=-1;
					//delete source
					int nIndex = NodeToIndex(pSource);
					DeleteItem(nIndex);
					HideChildren(pSource, TRUE, nIndex);
					Delete(pSource);
					InternaleUpdateTree();
					OnUpdateTotal();
					SetRedraw(1);
					InvalidateRect(NULL);
					UpdateWindow();
				}else
					SetRedraw(1);
			}
	    }
    }
#endif

	CListCtrl::OnLButtonUp(nFlags, point);
}

void CSuperGridCtrl::OnKillFocus(CWnd* pNewWnd)
{
	// マウスが左クリックされている場合は、マウスのキャプチャを終了する
	if (m_bMouseLeft) {
		KillTimer(2);
		OnLButtonUp(0, CPoint(0,0));
		m_bMouseLeft = false;
		ReleaseCapture();					// マウスのキャプチャ終了
	}

	CListCtrl::OnKillFocus(pNewWnd);
}


//used with the drag/drop operation
void CSuperGridCtrl::CopyChildren(CTreeItem* pDest, CTreeItem* pSrc)
{
	if (ItemHasChildren(pSrc))
	{
		POSITION pos = pSrc->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			CTreeItem* pItem = (CTreeItem *)pSrc->m_listChild.GetNext(pos);
			CItemInfo* lp = CopyData(GetData(pItem));
			CTreeItem* pNewItem = InsertItem(pDest, lp);
			OnRefreshTreeItem(pItem, pNewItem); /* STEP 037 */
			CopyChildren(pNewItem, pItem);
		}
	}
}


//hmmm
BOOL CSuperGridCtrl::DoDragDrop(CTreeItem* pTarget, CTreeItem* pSource)
{
	if(pTarget==NULL)
		return 0;

	BOOL bUpdate=FALSE;
	if(!IsCollapsed(pTarget))
		bUpdate=TRUE; //children are expanded, want to see update right away

	//make a copy of the source data
	CItemInfo* lp = CopyData(GetData(pSource));
	//create new node with the source data and make pTarget the parent

	CItemInfo* pTargetInfo = GetData(pTarget);
	CTreeItem* pNewParent;
	if (!((int)pTargetInfo->GetLParam() < 0)) {
		pNewParent = InsertItem(pTarget, lp, bUpdate, INSERT_NEXT);
	} else {
		pNewParent = InsertItem(pTarget, lp, bUpdate, INSERT_HEAD);
	}
	//if the source has children copy all source data and make the newly create item the parent
	if(ItemHasChildren(pSource))
		CopyChildren(pNewParent, pSource);
	else
		OnRefreshTreeItem(pSource, pNewParent); /* STEP 037 */

	if (bUpdate) Expand(pNewParent, NodeToIndex(pNewParent));

	return 1;
}

// 現在のカーソル位置のセル編集開始
bool CSuperGridCtrl::CellEditStart(void)
{
	int iItem = GetSelectedItem();
	if( m_CurSubItem != -1 && iItem != -1) {
		CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
		int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem);
		if (iSubItem == 0) {	//that's just me saying all nodes in col 0 are edit-controls, you may modify this
			CRect rcItem;
			GetItemRect(iItem, rcItem, LVIR_LABEL);
			DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL|ES_LEFT;
			CEdit *pEdit = new CListEditCtrl(iItem, iSubItem, GetItemText(iItem, iSubItem));
			pEdit->Create(dwStyle, rcItem, this, 0x1233);
		} else {
			CListEditCtrl *pEdit = (CListEditCtrl *)EditLabelEx(iItem, iSubItem);
			if (pEdit != NULL) {
				pEdit->ChangeInputMode(false); 	/* STEP 017 */
				DWORD style = pEdit->GetStyle();
				if (style & ES_MULTILINE) { /* Misirlou 151 */
					CString strText;
					pEdit->GetWindowText(strText);
					pEdit->SetSel(strText.GetLength(), -1);
				} else
					pEdit->SetSel(pEdit->LineLength(), -1);	// カーソルを行末に移動
			}
		}
		return(true);
	}

	return(false);
}

// 編集ウィンドウの位置を調整する
void CSuperGridCtrl::AdjustEditWindowPos(void)
{
	CRect	rectNow, rect;
	int		lx, ly;

	// 新しい編集ウィンドウの位置を求める
	GetSubItemRect(GetSelectedItem(), GetCurSubItem(), LVIR_BOUNDS, rect);

	// 移動前の編集ウィンドウのウィンドウ位置を取得
	g_pEditWindow->GetWindowRect(&rectNow);

	// 編集ウィンドウの移動量を求める
	lx = rect.left - rectNow.left;
	ly = rect.top - rectNow.top;

	// 編集ウィンドウを新しい位置に移動させる
	rectNow.left += lx;
	rectNow.right += lx;
	rectNow.top += ly;
	rectNow.bottom += ly;
	g_pEditWindow->MoveWindow(&rectNow);
}

void CSuperGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (g_pEditWindow != NULL) {
		CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

		// 編集ウィンドウの位置を調整する
		AdjustEditWindowPos();
		InvalidateItemRect(GetSelectedItem());
	} else {
		// フォーカスを自ウィンドウにする事で、エディットを終了させる
		if( GetFocus() != this) SetFocus();

		CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


void CSuperGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (g_pEditWindow != NULL) {
		CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

		// 編集ウィンドウの位置を調整する
		AdjustEditWindowPos();
		InvalidateItemRect(GetSelectedItem());
	} else {
		// フォーカスを自ウィンドウにする事で、エディットを終了させる
		if( GetFocus() != this) SetFocus();

		CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}


void CSuperGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (g_pEditWindow != NULL) {
		CListCtrl::OnSize(nType, cx, cy);

		// 編集ウィンドウの位置を調整する
		AdjustEditWindowPos();
	} else {
		// フォーカスを自ウィンドウにする事で、エディットを終了させる
		if( GetFocus() != this) SetFocus();

		CListCtrl::OnSize(nType, cx, cy);
	}
}

// チェックマークを設定する
void CSuperGridCtrl::SetCheckEx(int nIndex, BOOL bCheck)
{
	ASSERT(nIndex != -1);
	if (nIndex != -1) {
		SetCheckEx(GetTreeItem(nIndex), bCheck);
	}
}
void CSuperGridCtrl::SetCheckEx(CTreeItem *pItem, BOOL bCheck, BOOL bRecursive)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == TRUE) {
			// ファイルの場合
			if (pInfo->GetCheck() == bCheck) return;		// 変更無し
			if (nIndex != -1) SetCheck(nIndex, bCheck);
			pInfo->SetCheck(bCheck);
			// チェック状態が変更された
			OnChangeCheck(pItem, bCheck);
		} else {
			// フォルダの場合
			if (nIndex != -1) SetCheck(nIndex, bCheck);
			pInfo->SetCheck(bCheck);
			pInfo->SetFlagExt(pInfo->GetFlagExt() &~ CItemInfo::FLAG_EX_CHECK_MASK);

			// 子アイテムを全てチェックする
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						SetCheckEx((CTreeItem *)GetNextChild(pSelItem, pos), bCheck, TRUE);
					}
				}
			}
			bCheck = TRUE;
		}
		// 再帰呼び出しか？
		if (bRecursive == FALSE) {
			// 親アイテムのチェック状態を更新する
			CTreeItem	*pParent = GetParentItem(pItem);
			while(pParent != NULL) {
				// 親のチェック状態を更新する
				int		nIndexP = NodeToIndex(pParent);
				if (nIndexP != -1) SetCheck(nIndexP, bCheck);
				CItemInfo *pInfoP = GetData(pParent);
				pInfoP->SetCheck(bCheck);
				pInfoP->SetFlagExt((pInfo->GetFlagExt() &~ CItemInfo::FLAG_EX_CHECK_MASK) | CItemInfo::FLAG_EX_CHECK_GRAY);
//				InvalidateItem(pParent);

				// さらに親
				pParent = GetParentItem(pParent);
			}
		}
	}
}

// チェック状態が変更された
void CSuperGridCtrl::OnChangeCheck(CTreeItem *pItem, BOOL bCheck)
{
}

// スペースが押された場合の処理：チェックマーク変更
void CSuperGridCtrl::OnKeyDownSpace(void)
{
	if (GetSelectedCount() == 0) return;
	if ((GetExtendedStyle() & LVS_EX_CHECKBOXES) == 0) return;

	if (GetSelectedCount() == 1) {
		// 単一選択
		int nIndex = GetSelectedItem();
		if (nIndex != -1) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			if (pItem != NULL) {
				SetCheckEx(pItem, !GetCheck(nIndex));
			}
		}
	} else {
		// 複数選択
		int		nIndex = -1;
		int		nCheck;
		BOOL	bFirst = TRUE;
		BOOL	bReverse = TRUE;
		// 選択範囲で異なるチェック状態がある場合には、一番先頭の状態に統一する
		while((nIndex = GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >= 0) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			// ファイルのみ処理する
			if (pItem != NULL && IsItemFile(pItem) == TRUE) {
				CItemInfo *pInfo = GetData(pItem);
				if (bFirst == TRUE) {
					// 先頭のファイル
					nCheck = pInfo->GetCheck();
					bFirst = FALSE;
				} else {
					// ２番目以降のファイル
					if (nCheck != pInfo->GetCheck()) {
						// チェック状態が異なる場合は、先頭のファイルと同じにする
						SetCheckEx(pItem, nCheck);
						bReverse = FALSE;
					}
				}
			}
		}
		if (bReverse == TRUE) {
			// 選択範囲が全て同じチェック状態だった場合は、チェック状態を反転する
			nIndex = -1;
			while((nIndex = GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >= 0) {
				CTreeItem	*pItem = GetTreeItem(nIndex);
				// ファイルのみ処理する
				if (pItem != NULL && IsItemFile(pItem) == TRUE) {
					SetCheckEx(pItem, !nCheck);
				}
			}
		}
	}
}

BOOL CSuperGridCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		if (GetFocus() == this) {
			if (m_CurSubItem == 0) {
				if (pMsg->wParam == VK_SPACE) {
					// スペースが押された場合の処理：チェックマーク変更
					OnKeyDownSpace();
					return(TRUE);
				}
			}

			bool	bChangeSelect = true;
			if (pMsg->wParam == VK_RETURN) {
				if (g_bOptEnterBeginEdit) {
					// 項目の編集開始
					pMsg->wParam = VK_F2;
				} else {
					// ↓／↑の項目に移動
					pMsg->wParam = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? VK_UP : VK_DOWN;
				}
			} else if (pMsg->wParam == VK_TAB) {
				// ←／→の項目に移動
				pMsg->wParam = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? VK_LEFT : VK_RIGHT;
				bChangeSelect = false;
			} else {
				switch(pMsg->wParam) {
				case VK_UP:				// ↑
				case VK_DOWN:			// ↓
				case VK_LEFT:			// ←
				case VK_RIGHT:			// →
				case VK_PRIOR:			// PageUp
				case VK_NEXT:			// PageDown
				case VK_END:			// End
				case VK_HOME:			// Home
					// SHIFT + カーソル(範囲選択処理)
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						if (m_bMultiSelect == false) {
							// 範囲選択開始処理
							SelectRangeStart();
						}
					}
					// CTRL は無視
					if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && !(GetAsyncKeyState(VK_SHIFT) & 0x8000)/* STEP 001 */) {
/* 2003.06.18 少し変更 */
#ifdef FLICKERFREE
						//SetRedraw(FALSE);
#endif
						if (pMsg->wParam == VK_DOWN) { /* Misirlou 144 */
							SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
							SendMessage(WM_VSCROLL, SB_ENDSCROLL, 0);
							if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000)) { /* Baja 164 */
								int nIndex = GetSelectedItem();
								CRect rect;
								GetItemRect(nIndex, &rect, LVIR_BOUNDS);
								ClientToScreen(&rect);
								CHeaderCtrl* pHeader = GetHeaderCtrl();
								CRect headerRect;
								pHeader->GetClientRect(&headerRect);
								ClientToScreen(&headerRect);
								SelItemRange(FALSE, 0, -1);
								SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
								if (rect.top < headerRect.bottom) {
									SelItemRange(FALSE, 0, -1);
									SetItemState(nIndex+1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
									InvalidateItemRect(nIndex+1);
									OnChangeSelect();
								}
							}
						} else if (pMsg->wParam == VK_UP) {
							SendMessage(WM_VSCROLL, SB_LINEUP, 0);
							SendMessage(WM_VSCROLL, SB_ENDSCROLL, 0);
							if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000)) { /* Baja 164 */
								int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);//GetSelectedItem();
								CRect rect;
								GetItemRect(nIndex, &rect, LVIR_BOUNDS);
								ClientToScreen(&rect);
								CRect parentRect;
								GetClientRect(&parentRect);
								ClientToScreen(&parentRect);
								parentRect.bottom -= GetSystemMetrics(SM_CYHSCROLL);
								SelItemRange(FALSE, 0, -1);
								SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
								if (rect.top > parentRect.bottom) {
									SelItemRange(FALSE, 0, -1);
									SetItemState(nIndex-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
									InvalidateItemRect(nIndex-1);
									OnChangeSelect();
								}
							}
						}
#ifdef FLICKERFREE
						SetRedraw(TRUE);
#endif
						return(TRUE);
					}
					break;
				}
			}

			switch(pMsg->wParam) {
/* 2003.06.19 start */
			case VK_UP:
				{
					//BOOL bResult = CListCtrl::PreTranslateMessage(pMsg);
					int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) - 2;//GetSelectedItem();
					EnsureVisible(nIndex, FALSE);
					//return bResult;
				}
				break;
			case VK_DOWN:
				{
					//BOOL bResult = CListCtrl::PreTranslateMessage(pMsg);
					int nIndex = GetSelectedItem() + 2;
					EnsureVisible(nIndex, FALSE);
					//return bResult;
				}
				break;
/* 2003.06.19 end */
			case VK_LEFT:
				if (GetSelectedItem() >= 0) {
					// 範囲選択処理中
					if (m_bMultiSelect && GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						if (m_bMultiSelectLine == false) {
							// 列の範囲選択中
							if (m_CurSubItem <= 1) return(TRUE);
						}
					}
					// Decrement the order number.
					m_CurSubItem--;
					if(m_CurSubItem < 0) {
						m_CurSubItem = 0;
					} else {
						CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
						MakeColumnVisible( Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
						int iItem = GetSelectedItem();
						if( iItem != -1 ) InvalidateItemRect(iItem);
						if (bChangeSelect) OnChangeSelect();	// 選択が変更されたことを通知
					}
				}
				return TRUE;

			case VK_RIGHT:
				if (GetSelectedItem() >= 0) {
					// 範囲選択処理中
					if (m_bMultiSelect && GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						if (m_bMultiSelectLine == true) {
							// 行の範囲選択中
							return(TRUE);
						}
					}
					// Increment the order number.
					m_CurSubItem++;
					CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
					int nColumnCount = pHeader->GetItemCount();
					// Don't go beyond the last column.
					if( m_CurSubItem > nColumnCount-1) {
						m_CurSubItem = nColumnCount-1;
					} else {
						MakeColumnVisible(Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
						int iItem = GetSelectedItem();
						if( iItem != -1 ) InvalidateItemRect(iItem);
						if (bChangeSelect) OnChangeSelect();	// 選択が変更されたことを通知
					}
				}
				return TRUE;

			case VK_F2:			// edit itemdata
				{
					BOOL bResult = OnVkReturn();
					if(!bResult) {
						// 現在のカーソル位置のセル編集開始
						if (CellEditStart()) {
							return(1);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	if(pMsg->message == WM_KEYUP) {
		if (GetFocus() == this) {
			switch(pMsg->wParam) {
			case VK_UP:				// ↑
			case VK_DOWN:			// ↓
			case VK_PRIOR:			// PageUp
			case VK_NEXT:			// PageDown
			case VK_END:			// End
			case VK_HOME:			// Home
				{
					if (GetAsyncKeyState(VK_CONTROL) & 0x8000 ||
						(GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)/* Baja 164 */) { /* Misirlou 144 */
						if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP) {
							return(TRUE);
						}
					}
					BOOL	bResult;
					bResult = CListCtrl::PreTranslateMessage(pMsg);
					if (m_bMultiSelectLine == false) {
						// 行選択解除
						int		iItem = GetSelectedItem();
#ifdef FLICKERFREE
						SelItemRange(FALSE, 0, -1);
						SetItemState(iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
#endif
					}
					OnChangeSelect();		// 選択が変更されたことを通知
					return(bResult);
				}
				break;
			}
		}
	}

	return(CListCtrl::PreTranslateMessage(pMsg));
}


#define IDC_EDITCTRL 0x1234
CEdit* CSuperGridCtrl::EditLabelEx(int nItem, int nCol)
{
	CRect rect;
	int offset = 0;
	if(!EnsureVisible(nItem, TRUE))
		return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size(offset + rect.left,0);
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right)
	   rect.right = rcClient.right;

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);

	DWORD dwStyle;
	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else
		dwStyle = ES_CENTER;

	//YOU MAY WANNA COMMENT THIS OUT,YOU DECIDE ..
	//now you could take into account here, that an subitem might have an icon
	boolean isMultiEdit = false; /* BeachMonster 089 */
	boolean isReadOnly = false;
	CTreeItem *p = GetTreeItem(nItem);
	if(p!=NULL)
	{
		CItemInfo *pInfo = GetData(p);
		if(pInfo!=NULL)
		{
			int iSubImage = pInfo->GetSubItemImage(nCol-1);
			if(iSubImage!=-1)
			{
				//m_cxImage is actually the width of the "tree" imagelist not your subitem imagelist..
				//remember that, when you create your bitmap, I was to lazy getting the icon size of the subitem imagelist
				rect.left+=m_cxImage;
			}
			CONTROLTYPE ctrlType; /* BeachMonster 089 */
			if (pInfo->GetControlType(nCol-1, ctrlType) == 0) {
				if (ctrlType == multiedit) {
					isMultiEdit = true;
				}
				if (ctrlType == multieditreadonly) {
					isMultiEdit = true;
					isReadOnly = true;
				}
			}
		}
	}
	//////////////////////////////////
	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	if (isMultiEdit) { /* BeachMonster 089 */
		dwStyle |= ES_MULTILINE|ES_AUTOVSCROLL;
		dwStyle &= ~ES_AUTOHSCROLL;
	}
	if (isReadOnly) {
		dwStyle |= ES_READONLY;
	}
	CListEditCtrl *pEdit = new CListEditCtrl(nItem, nCol, GetItemText(nItem, nCol));
	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);
	//pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE); //funny thing happend here, uncomment this,
												//and then edit an item,
												//enter a long text so that the ES_AUTOHSCROLL comes to rescue
												//yes that's look funny, ???.
	pEdit->LimitText(OnGetColumLimitText(nItem, nCol));	// 30文字まで
	pEdit->SetFocus();

	return pEdit;
}




void CSuperGridCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
 	LV_ITEM *plvItem = &plvDispInfo->item;
	if (plvItem->pszText != NULL)//valid text
	{
		if(plvItem->iItem != -1) //valid item
		{
			CTreeItem*pSelItem = GetTreeItem(plvItem->iItem);
			if(pSelItem != NULL) {
				OnUpdateListViewItem(pSelItem, plvItem);
			}
		}
	}
	*pResult = 0;
}



int CSuperGridCtrl::GetNumCol()
{
	CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
	return pHeader ? pHeader->GetItemCount() : 0;
}



//Think Rex Myer is spooking here
void CSuperGridCtrl::MakeColumnVisible(int nCol)
{
	if(nCol < 0)
		return;
	// Get the order array to total the column offset.
	CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();

	int nColCount = pHeader->GetItemCount();
	ASSERT( nCol < nColCount);
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	// Get the column offset
	int offset = 0;
	for(int i = 0; pOrderarray[i] != nCol; i++)
		offset += GetColumnWidth(pOrderarray[i]);

	int colwidth = GetColumnWidth(nCol);
	delete[] pOrderarray;

	CRect rect;
	GetItemRect(0, &rect, LVIR_BOUNDS);
	// Now scroll if we need to show the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset + rect.left < 0) {
		CSize size(offset + rect.left, 0);
		Scroll(size);
		InvalidateRect(NULL);
		UpdateWindow();
	} else if (offset + colwidth + rect.left > rcClient.right) {
		CSize size((offset + colwidth + rect.left) - rcClient.right, 0);
		Scroll(size);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}



//Think Rex Myer is spooking here
int CSuperGridCtrl::IndexToOrder( int iIndex )
{
	// This translates a column index value to a column order value.
	CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
	int nColCount = pHeader->GetItemCount();
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	int i; for(i=0; i<nColCount; i++)
	{
		if(pOrderarray[i] == iIndex )
		{
			delete[] pOrderarray;
			return i;
		}
	}
	delete[] pOrderarray;
	return -1;
}



void CSuperGridCtrl::DrawFocusCell(CDC *pDC, int nItem, int iSubItem,int nIconOffset)
{
	if(nItem == GetSelectedItem() && iSubItem == m_CurSubItem)
	{
		CRect rect;
		GetSubItemRect(nItem, iSubItem, LVIR_BOUNDS, rect);
		CBrush br(GetCellRGB());
		if(iSubItem==0)
			GetItemRect(iSubItem, rect, LVIR_LABEL);
		rect.left+=nIconOffset;
		pDC->FillRect(rect, &br);
		pDC->DrawFocusRect(rect);
	}
}




//insert item and return new parent pointer.
CSuperGridCtrl::CTreeItem* CSuperGridCtrl::InsertItem(CTreeItem *pParent, CItemInfo* lpInfo,  BOOL bUpdate, int nInsertType)
{
	if(pParent==NULL)
		return NULL;

	CTreeItem *pItem = NULL;
	pItem =  new CTreeItem();

	if(lpInfo==NULL)
		lpInfo = new CItemInfo;

	UpdateData(pItem, lpInfo);
	SetIndent(pItem, GetIndent(pParent)+1);
	SetParentItem(pItem, pParent);

	switch(nInsertType) {
	case INSERT_HEAD:
		// 先頭に追加
		pParent->m_listChild.AddHead(pItem);
		break;
	case INSERT_TAIL:
		// 最後に追加
		pParent->m_listChild.AddTail(pItem);
		break;
#if SUPORT_DRAGDROP
	case INSERT_NEXT:
		{
			CTreeItem *pParentFolder = GetParentItem(pParent);
			POSITION	pos = pParentFolder->m_listChild.GetHeadPosition();
			while (pos != NULL) {
				POSITION	posTarget = pos;
				CTreeItem	*pTarget = (CTreeItem *)pParentFolder->m_listChild.GetNext(pos);
				if (pTarget == pParent) {
					SetIndent(pItem, GetIndent(pParentFolder)+1);
					SetParentItem(pItem, pParentFolder);
					pParentFolder->m_listChild.InsertAfter(posTarget, pItem);
					break;
				}
			}
		}
		break;
#endif
	case INSERT_FILE:
		// ファイル(フォルダの前に追加)
		{
			POSITION	pos = pParent->m_listChild.GetHeadPosition();
			bool	bAddItem = false;
			while (pos != NULL) {
				POSITION	posTarget = pos;
				CTreeItem	*pTarget = (CTreeItem *)pParent->m_listChild.GetNext(pos);
				if (pTarget != NULL) {
					CItemInfo *pItemInfo = GetData(pTarget);
					if (pItemInfo->GetLParam() < 0) {		// フォルダ？
						// この位置に挿入
						pParent->m_listChild.InsertBefore(posTarget, pItem);
						bAddItem = true;
						break;
					}
				}
			}
			if (bAddItem == false) {
				// 最後に追加
				pParent->m_listChild.AddTail(pItem);
			}
		}
		break;
	case INSERT_DIRECTORY:
		// ディレクトリ(フォルダ同士でソートして追加)
		{
			POSITION	pos;
			const auto& sItemText = GetData(pItem)->GetItemText();
			if (sItemText.GetLength() == 0) {
				// 項目名が空
				pos = NULL;
			} else {
				pos = pParent->m_listChild.GetHeadPosition();
			}
			bool	bAddItem = false;
			while (pos != NULL) {
				POSITION	posTarget = pos;
				CTreeItem	*pTarget = (CTreeItem *)pParent->m_listChild.GetNext(pos);
				if (pTarget != NULL) {
					CItemInfo *pItemInfo = GetData(pTarget);
					const auto& sTargetText = pItemInfo->GetItemText();
					if (pItemInfo->GetLParam() < 0		// フォルダ
					&&  sTargetText.Compare(sItemText) > 0) {
						// この位置に挿入
						pParent->m_listChild.InsertBefore(posTarget, pItem);
						bAddItem = true;
						break;
					}
				}
			}
			if (bAddItem == false) {
				// 最後に追加
				pParent->m_listChild.AddTail(pItem);
			}
		}
		break;
	}

	if (!bUpdate) {
		 Hide(pParent, TRUE);
	} else {
		//calc listview index for the new node
		int nIndex = NodeToIndex(pItem);
		CString str = GetData(pItem)->GetItemText();
		LV_ITEM     lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
		lvItem.pszText = str.GetBuffer(1);
		//insert item
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam = (LPARAM)pItem;
		lvItem.iIndent = GetIndent(pItem);
		CListCtrl::InsertItem(&lvItem);
		if(lpInfo->GetCheck())
			SetCheck(nIndex);
		//Get subitems
		int nSize = GetData(pItem)->GetItemCount();
		int i; for(i=0; i < nSize;i++)
		{
		   CString str = GetData(pItem)->GetSubItem(i);
		   lvItem.mask = LVIF_TEXT;
		   lvItem.iSubItem = i+1;
		   lvItem.pszText = str.GetBuffer(1);
		   SetItem(&lvItem);
		}
		InternaleUpdateTree();//better do this
	}
	return pItem;
}



void CSuperGridCtrl::InternaleUpdateTree()
{
	int nItems = GetItemCount();
	for(int nItem=0; nItem < nItems; nItem++)
	{
		CTreeItem* pItem = GetTreeItem(nItem);
		SetCurIndex(pItem, nItem);
	}
}


int CSuperGridCtrl::NodeToIndex(CTreeItem *pNode)
{
	int nStartIndex=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * root = (CTreeItem*)m_RootItems.GetNext(pos);
		int ret = _NodeToIndex(root, pNode, nStartIndex);
		if(ret != -1)
			return ret;
	}
	return -1;
}




CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetRootItem(int nIndex)
{
	POSITION pos = m_RootItems.FindIndex(nIndex);
	if(pos==NULL)
		return NULL;
	return (CTreeItem*)m_RootItems.GetAt(pos);
}



int CSuperGridCtrl::GetRootIndex(CTreeItem * root)
{
	int nIndex = 0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos != NULL)
	{
		CTreeItem * pItem = (CTreeItem*)m_RootItems.GetNext(pos);
		if(pItem== root)
			return nIndex;
		nIndex++;
	}
	return -1;
}



BOOL CSuperGridCtrl::IsRoot(CTreeItem * lpItem)
{
	return m_RootItems.Find(lpItem) != NULL;
}


void CSuperGridCtrl::DeleteRootItem(CTreeItem * root)
{
	POSITION pos = m_RootItems.Find(root);
	if(pos!=NULL)
	{
		CTreeItem* pRoot=(CTreeItem*)m_RootItems.GetAt(pos);
		if(pRoot->m_lpNodeInfo!=NULL)
				delete pRoot->m_lpNodeInfo;
		delete pRoot;
		m_RootItems.RemoveAt(pos);
	}
}



CSuperGridCtrl::CTreeItem*  CSuperGridCtrl::InsertRootItem(CItemInfo * lpInfo)
{
	if(lpInfo==NULL)
		lpInfo = new CItemInfo;

	CTreeItem* pRoot = NULL;

	pRoot =  new CTreeItem();

	CleanMe(pRoot);
	UpdateData(pRoot, lpInfo);
	SetIndent(pRoot, 1);
	SetCurIndex(pRoot, GetItemCount());
	SetParentItem(pRoot, NULL);

	CItemInfo* lp = GetData(pRoot);
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
	CString strItem = lp->GetItemText();
	lvItem.pszText = strItem.GetBuffer(1);
	lvItem.iItem = GetItemCount();
	lvItem.lParam = (LPARAM)pRoot;
	lvItem.iIndent = 1;
	lvItem.iSubItem = 0;
	CListCtrl::InsertItem(&lvItem);
	int nSize = lp->GetItemCount();
	int i; for(i=0; i < nSize;i++)
	{
	   CString str = lp->GetSubItem(i);
	   lvItem.mask = LVIF_TEXT;
	   lvItem.iSubItem = i+1;
	   lvItem.pszText = str.GetBuffer(1);
	   SetItem(&lvItem);
	}
	m_RootItems.AddTail(pRoot);
	return pRoot;
}



void CSuperGridCtrl::DrawTreeItem(CDC* pDC, CTreeItem* pSelItem, int nListItem, const CRect& rcBounds)
{
	int nColWidth = GetColumnWidth(0);
	int yDown = rcBounds.top;
    CPen* pPenTreeLine = pDC->SelectObject(&m_psTreeLine);
	int iIndent = GetIndent(pSelItem);
	int nHalfImage = (m_cxImage >> 1);
	int nBottomDown = yDown + nHalfImage + ((rcBounds.Height() - m_cyImage) >> 1);
	//
	BOOL bChild = ItemHasChildren(pSelItem);
	BOOL bCollapsed = IsCollapsed(pSelItem);
	//draw outline
	while(1)
	{
		CTreeItem* pParent = GetParentItem(pSelItem);
		if(pParent==NULL)//no more parents, stop
			break;

		POSITION pos = pParent->m_listChild.GetTailPosition();
		while(pos!=NULL)
		{
			CTreeItem *pLastChild = (CTreeItem*)pParent->m_listChild.GetPrev(pos);
			int nIndex = GetCurIndex(pLastChild);
			int nCurIndent = GetIndent(pLastChild);
			if(nListItem > nIndex && iIndent > nCurIndent)//no need to go further in this loop
				break;

			//no drawing outside the 1st columns right
			int xLine =  rcBounds.left + nCurIndent * m_cxImage - nHalfImage;
			if(nIndex == nListItem && nCurIndent==iIndent)
			{
				//draw '-
				int x;
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, nBottomDown);
				// -
				xLine + nHalfImage > nColWidth ? x = nColWidth: x = xLine + nHalfImage;

				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent==iIndent)
			{
				//draw |-
				int x;
				xLine + nHalfImage > nColWidth ? x = nColWidth : x = xLine + nHalfImage;
				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				//-
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent < iIndent)
			{
				//draw |
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
		}
		pSelItem = pParent;//next
	}

	//draw plus/minus sign
	if(bChild)
	{
		CRectangle rect(this, pDC, iIndent, rcBounds);

		rect.DrawRectangle(this);

		CPen* pPenPlusMinus = pDC->SelectObject(&m_psPlusMinus);
		if(bCollapsed)
			rect.DrawPlus();
		else {
			rect.DrawMinus();
			//draw line up to parent folder
			CPen* pLine = pDC->SelectObject(&m_psTreeLine);
			int nOffset = (rcBounds.Height() - m_cyImage)/2;
			pDC->MoveTo(rect.GetLeft()+ m_cxImage, rcBounds.top + m_cyImage+nOffset);
			pDC->LineTo(rect.GetLeft() + m_cxImage, rcBounds.bottom);
			pDC->SelectObject(pLine);
		}
		pDC->SelectObject(pPenPlusMinus);
	}
	pDC->SelectObject(pPenTreeLine);
}



//walk all over the place setting the hide/show flag of the nodes.
//it also deletes items from the listviewctrl.
void CSuperGridCtrl::HideChildren(CTreeItem *pItem, BOOL bHide,int nItem)
{
	if(!IsCollapsed(pItem))
	if(ItemHasChildren(pItem))
	{
		Hide(pItem, bHide);
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			HideChildren((CTreeItem *)pItem->m_listChild.GetNext(pos),bHide,nItem+1);
			DeleteItem(nItem);

		}
	}
}




void CSuperGridCtrl::Collapse(CTreeItem *pItem)
{
	if(pItem != NULL && ItemHasChildren(pItem))
	{
		SetRedraw(0);
		int nIndex = NodeToIndex(pItem);
		HideChildren(pItem, TRUE, nIndex+1);
		InternaleUpdateTree();
		SetRedraw(1);
	}
}


void CSuperGridCtrl::ExpandAll(CTreeItem *pItem, int& nScroll)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		int nIndex = NodeToIndex(pItem);
		nScroll = Expand(pItem, nIndex);
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandAll(pChild, nScroll);
	}

}



int CSuperGridCtrl::Expand(CTreeItem* pSelItem, int nIndex)
{
	if(ItemHasChildren(pSelItem) && IsCollapsed(pSelItem)) {
		LV_ITEM lvItem;
		lvItem.mask = LVIF_INDENT;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam=(LPARAM)pSelItem;
		lvItem.iIndent = GetIndent(pSelItem);
		SetItem(&lvItem);

		Hide(pSelItem, FALSE);
		//expand children
		POSITION pos = pSelItem->m_listChild.GetHeadPosition();
		while(pos != NULL)
		{
			CTreeItem* pNextNode = (CTreeItem*)pSelItem->m_listChild.GetNext(pos);
			CString str = GetData(pNextNode)->GetItemText();
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
			lvItem.pszText =str.GetBuffer(1);
			lvItem.iItem = nIndex + 1;
			lvItem.iSubItem = 0;
			lvItem.lParam=(LPARAM)pNextNode;
			lvItem.iIndent = GetIndent(pSelItem)+1;
			CListCtrl::InsertItem(&lvItem);
			if(GetData(pNextNode)->GetCheck())
				SetCheck(nIndex + 1);
			//get subitems
			int nSize = GetData(pNextNode)->GetItemCount();
			int i; for(i=0; i< nSize;i++)
			{
			   CString str=GetData(pNextNode)->GetSubItem(i);
			   lvItem.mask = LVIF_TEXT;
			   lvItem.iSubItem = i+1;
			   lvItem.pszText=str.GetBuffer(1);
			   SetItem(&lvItem);
			}
			nIndex++;
		}
	}
	InternaleUpdateTree();
	return nIndex;
}




int CSuperGridCtrl::SelectNode(CTreeItem *pLocateNode)
{
	if(IsRoot(pLocateNode))
	{
		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		SetItemState(0, uflag, uflag);
		return 0;
	}
	int nSelectedItem=-1;
	CTreeItem* pNode = pLocateNode;
	CTreeItem* pTopLevelParent=NULL;
	//Get top parent
	while(1)
	{
		CTreeItem *pParent = GetParentItem(pLocateNode);
		if(IsRoot(pParent))
			break;
		pLocateNode = pParent;
	}
	pTopLevelParent = pLocateNode;//on top of all
	//Expand the folder
	if(pTopLevelParent != NULL)
	{
		SetRedraw(0);
		CWaitCursor wait;
		CTreeItem *pRoot = GetParentItem(pTopLevelParent);

		if(IsCollapsed(pRoot))
			Expand(pRoot,0);

		ExpandUntil(pTopLevelParent, pNode);

		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		nSelectedItem = NodeToIndex(pNode);

		SetItemState(nSelectedItem, uflag, uflag);

		SetRedraw(1);
		EnsureVisible(nSelectedItem, TRUE);
	}
	return nSelectedItem;
}




void CSuperGridCtrl::ExpandUntil(CTreeItem *pItem, CTreeItem* pStopAt)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos)
		{
			CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
			if(pChild == pStopAt)
			{
				int nSize = GetIndent(pChild);
				CTreeItem** ppParentArray = new CTreeItem*[nSize];
				int i=0;
				while(1)
				{
					CTreeItem *pParent = GetParentItem(pChild);

					if(IsRoot(pParent))
						break;
					pChild = pParent;
					ppParentArray[i] = pChild;
					i++;
				}

				for(int x=i; x > 0; x--)
				{
					CTreeItem *pParent = ppParentArray[x-1];
					Expand(pParent, NodeToIndex(pParent));
				}
				delete [] ppParentArray;
				return;
			}
		}
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandUntil(pChild, pStopAt);
	}

}

void CSuperGridCtrl::DeleteItemExNoRedraw(CTreeItem *pSelItem, int nItem)
{
	DeleteItem(nItem);//delete cur item in listview
	//delete/hide all children in pSelItem
	HideChildren(pSelItem, TRUE, nItem);
	//delete all internal nodes
	// If root, must delete from m_rootData
	if(GetParentItem(pSelItem) == NULL )
	{
		DeleteRootItem(pSelItem);
	}
	else
		Delete(pSelItem);

	InternaleUpdateTree();
	if (nItem-1 >= 0) {
		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		CRect rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem;//just to get the documention right :)
		if (GetItemRect(nItem, rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem, LVIR_LABEL)) {
			SetItemState(nItem, uflag, uflag);
		} else {
			SetItemState(nItem-1, uflag, uflag);
		}
	}
}

void CSuperGridCtrl::DeleteItemEx(CTreeItem *pSelItem, int nItem)
{
	SetRedraw(0);
	DeleteItemExNoRedraw(pSelItem, nItem);
	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();
}



void CSuperGridCtrl::CleanMe(CTreeItem *pItem)
{
	// delete child nodes
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pItemData = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		if(pItemData!=NULL)
		{
			if(pItemData->m_lpNodeInfo!=NULL)
				delete pItemData->m_lpNodeInfo;

			pItemData->m_listChild.RemoveAll();
			delete pItemData;
		}
	}
	pItem->m_listChild.RemoveAll();
}




CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNext(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static BOOL bFound;
	if (bInit)
		bFound = FALSE;

	if (pNode == pStartAt)
		bFound = TRUE;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE, TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE,FALSE);
				if (pChild != NULL)
					return pChild;
			}
	}
	// if reached top and last level return original
	if (bInit)
		return pNode;
	else
		return NULL;
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrev(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static CTreeItem* pPrev;
	if (bInit)
		pPrev = pStartAt;

	if (pNode == pStartAt)
		return pPrev;

	pPrev = pStartAt;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pCur = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				CTreeItem* pChild = GetPrev(pCur,pNode, FALSE,TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
		POSITION pos = pStartAt->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			CTreeItem* pCur = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
			CTreeItem* pChild = GetPrev(pCur,pNode, FALSE,FALSE);
			if (pChild != NULL)
				return pChild;
		}
	}

	if (bInit)
		return pPrev;
	else
		return NULL;
}


int CSuperGridCtrl::_NodeToIndex(CTreeItem *pStartpos, CTreeItem* pNode, int& nIndex, BOOL binit)
{
	static BOOL bFound;
	// Account for other root nodes
	if(GetParentItem(pStartpos) == NULL && GetRootIndex(pStartpos) !=0)
		nIndex++;

	if(binit)
		bFound=FALSE;

	if(pStartpos==pNode)
		bFound=TRUE;

	if(!IsCollapsed(pStartpos))
	{
		POSITION pos = GetHeadPosition(pStartpos);
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pStartpos, pos);
			if(bFound)
				return nIndex;

//	Craig Schmidt: Cannot set nIndex as return value.  Worked find with single root but
//				   the calling function get confused since the return value may indicate
//				   that the next root needs to be searched.  Didn'd spend much time on
//				   this so there is probably a better way of doing this.
//			nIndex = _NodeToIndex(pChild, pNode, nIndex, binit);
			_NodeToIndex(pChild, pNode, nIndex, binit);
			nIndex++;
		}
	}
	if(binit && bFound)
		return nIndex;
	else
		return -1;
}


BOOL CSuperGridCtrl::Delete(CTreeItem* pNode, BOOL bClean)
{
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		if(_Delete(pRoot, pNode, bClean))
			return TRUE;
	}
	return FALSE;
}



BOOL CSuperGridCtrl::_Delete(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bClean)
{
	POSITION pos = pStartAt->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posPrev = pos;
		CTreeItem *pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
		if (pChild == pNode)
		{
			pStartAt->m_listChild.RemoveAt(posPrev);
			if(bClean)
			{
				if(GetData(pNode)!=NULL)
					delete GetData(pNode);
				delete pNode;
			}
			return TRUE;
		}
		if (_Delete(pChild, pNode) == TRUE)
			return TRUE;
	}
	return FALSE;
}




BOOL CSuperGridCtrl::IsChildOf(const CTreeItem* pParent, const CTreeItem* pChild) const
{
	if (pChild == pParent)
		return TRUE;
	POSITION pos = pParent->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pNode = (CTreeItem*)pParent->m_listChild.GetNext(pos);
		if (IsChildOf(pNode, pChild))
			return TRUE;
	}
	return FALSE;
}



void CSuperGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( GetFocus() != this) SetFocus();

	LVHITTESTINFO ht;
	ht.pt = point;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht)) {
		BOOL bSelect=1;

		bSelect = HitTestOnSign(point, ht);
		if (bSelect && ht.iSubItem <= 0) {
			if (m_bMultiSelect) {
				// 自前の複数選択処理を解除
				m_bMultiSelect = m_bMultiSelectLine = false;
				// 選択領域を再描画させる
				InvalidateSelectRangeItem();
			}
			m_CurSubItem = 0;
			if ((GetAsyncKeyState(VK_SHIFT  ) & 0x8000)
			||  (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
				// SHIFT / CTRL が押されている場合は、デフォルト処理
				CListCtrl::OnLButtonDown(nFlags, point);
				OnChangeSelect();			// 選択が変更されたことを通知 /* LastTrain 062 */
			} else {
				// 行を選択
				m_posMultiSelect[0].y = m_posMultiSelect[1].y = ht.iItem;
				m_posMultiSelect[0].x = m_posMultiSelect[1].x = 0;
				SelectAllItems(FALSE);
				SetItemState(ht.iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				SetSelectionMark(ht.iItem);	// 選択マーク更新
				m_bMouseLeft = true;		// 左ボタンが押された
				OnChangeSelect();			// 選択が変更されたことを通知 /* LastTrain 062 */
				SetCapture();				// マウスのキャプチャ開始
				m_nAutoScrollTimer = 75;
				SetTimer(2, m_nAutoScrollTimer, NULL);	// 自動スクロール処理用
			}
			return;
		}

		if (bSelect && ht.iItem != -1) {
			// 範囲選択中ではなく、アクティブなセルをクリックした場合は
			// そのセルの編集開始
			if (m_bMultiSelect == false
			&&  m_bMultiSelectLine == false
			&&  ht.iItem == GetSelectedItem()
			&&  ht.iSubItem == GetCurSubItem()) {
				// 編集を開始させる
				PostMessage(WM_KEYDOWN, VK_F2, NULL);
				return;
			}

			// SHIFT + マウススクリック(範囲選択処理)
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
				// 範囲選択開始処理
				SelectRangeStart();
			}

			// 選択を解除
			SelItemRange(FALSE, 0, -1);

			m_CurSubItem = IndexToOrder(ht.iSubItem);
			CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
			// Make the column fully visible.
			MakeColumnVisible(Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem));
			// 行を選択
			SetItemState(ht.iItem, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);

			bool	bCtrlOpen;
			bCtrlOpen = OnControlLButtonDown(nFlags, point, ht);
			//update row anyway for selection bar
			InvalidateItemRect(ht.iItem);

			if (bCtrlOpen == false) {
				// マウスによる範囲選択処理
				if (m_bMultiSelect == true) {
					// 現在の選択を解除
					OnChangeSelect();
				}
				m_bMouseLeft = true;		// 左ボタンが押された

				OnChangeSelect();			// 選択が変更されたことを通知

				SetCapture();				// マウスのキャプチャ開始
				m_nAutoScrollTimer = 75;
				SetTimer(2, m_nAutoScrollTimer, NULL);	// 自動スクロール処理用
			} else {
				OnChangeSelect();			// 選択が変更されたことを通知
			}
		} else {
			// 選択を解除
			SelItemRange(FALSE, 0, -1);
		}
	}
}


void CSuperGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if( GetFocus() != this) SetFocus();
	CListCtrl::OnRButtonDown(nFlags, point);
}


void CSuperGridCtrl::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//default implementation you would go for this 9 out of 10 times
	CItemInfo *lp = GetData(lpItem);
	CString str = (CString)plvItem->pszText;
	if(lp!=NULL)
	{
		if(plvItem->iSubItem==0)
			lp->SetItemText(str);
		else //subitem data
			lp->SetSubItemText(plvItem->iSubItem-1, str);
	   UpdateData(lpItem, lp); // do not use bUpdateRow here, hence we only update a specific item or subitem..not all of them
	}
	SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);

}


//////////////// Arne Schild ///////////////////////////////////////////////
void CSuperGridCtrl::DeleteAll()
{
	SetRedraw(false);
	DeleteAllItems();
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		if(pRoot!=NULL)
		{
			//DeleteItem(0);//delete cur item in listview: no Sorry Arne you donｴt need this
			//HideChildren(pRoot, TRUE, 0);  no Sorry Arne you donｴt need this

			// If root, must delete from m_rootData
			if(GetParentItem(pRoot) == NULL )
			{
				DeleteRootItem(pRoot);
			}
			else
				Delete(pRoot);
		//	InternaleUpdateTree(); no Sorry Arne you donｴt need too
		//	other than that, thanks.
		}
	}
	SetRedraw(true);
	InvalidateRect(NULL);
	UpdateWindow();
}

POSITION CSuperGridCtrl::GetRootHeadPosition() const
{
	return m_RootItems.GetHeadPosition();
}


POSITION CSuperGridCtrl::GetRootTailPosition() const
{
	return m_RootItems.GetTailPosition();
}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNextRoot(POSITION& pos) const
{
	return (CTreeItem*)m_RootItems.GetNext(pos);
}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrevRoot(POSITION& pos) const
{
	return (CTreeItem*)m_RootItems.GetNext(pos);
}


POSITION CSuperGridCtrl::GetHeadPosition(CTreeItem* pItem) const
{
	return pItem->m_listChild.GetHeadPosition();
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNextChild(CTreeItem *pItem, POSITION& pos) const
{
	return (CTreeItem*)pItem->m_listChild.GetNext(pos);
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrevChild(CTreeItem *pItem, POSITION& pos) const
{
	return (CTreeItem*)pItem->m_listChild.GetPrev(pos);
}



POSITION CSuperGridCtrl::GetTailPosition(CTreeItem *pItem) const
{
	return pItem->m_listChild.GetTailPosition();
}



void CSuperGridCtrl::AddTail(CTreeItem *pParent, CTreeItem *pChild)
{
	pParent->m_listChild.AddTail(pChild);
}


inline int StrComp(const CString* pElement1, const CString* pElement2)
{
	return pElement1->Compare(*pElement2);
}



int CSuperGridCtrl::CompareChildren(const void* p1, const void* p2)
{
	CTreeItem * pChild1 = *(CTreeItem**)p1;
	CTreeItem * pChild2 = *((CTreeItem**)p2);
	CItemInfo *pItem1=(*pChild1).m_lpNodeInfo;
	CItemInfo *pItem2=(*pChild2).m_lpNodeInfo;
	return StrComp(&(pItem1->GetItemText()), &(pItem2->GetItemText()));
}


//void CSuperGridCtrl::Sort(CTreeItem* pParent, BOOL bSortChildren)
//{
//	const int nChildren = NumChildren(pParent);
//	if (nChildren > 1)
//	{
//		CTreeItem** ppSortArray = new CTreeItem*[nChildren];
//		// Fill in array with pointers to our children.
//		POSITION pos = pParent->m_listChild.GetHeadPosition();
//		for (int i=0; pos; i++)
//		{
//			ASSERT(i < nChildren);
//			ppSortArray[i] = (CTreeItem*)pParent->m_listChild.GetAt(pos);
//			pParent->m_listChild.GetNext(pos);
//		}
//
//		qsort(ppSortArray, nChildren, sizeof(CTreeItem*), CompareChildren);
//		// reorg children with new sorted list
//		pos = pParent->m_listChild.GetHeadPosition();
//		for (i=0; pos; i++)
//		{
//			ASSERT(i < nChildren);
//			pParent->m_listChild.SetAt(pos, ppSortArray[i]);
//			pParent->m_listChild.GetNext(pos);
//		}
//
//		delete [] ppSortArray;
//	}
//
//	if(bSortChildren)
//	{
//		POSITION pos = pParent->m_listChild.GetHeadPosition();
//		while (pos)
//		{
//			CTreeItem *pChild = (CTreeItem*)pParent->m_listChild.GetNext(pos);
//			Sort(pChild, TRUE);
//		}
//	}
//}
//
//void CSuperGridCtrl::SortEx(BOOL bSortChildren)
//{
//	int nItems = m_RootItems.GetCount();
//	if (nItems > 0)
//	{
//		if(bSortChildren)
//		{
//			POSITION posSortChildren = GetRootHeadPosition();
//			while(posSortChildren != NULL)
//			{
//					CTreeItem *pParent =(CTreeItem*)GetNextRoot(posSortChildren);
//					Sort(pParent, TRUE);//sort children
//			}
//		}
//		//set hideflag for rootitems
//		POSITION posHide = GetRootHeadPosition();
//		while(posHide != NULL)
//		{
//				CTreeItem *pParent =(CTreeItem*)GetNextRoot(posHide);
//				Collapse(pParent);
//		}
//
//		DeleteAllItems();//this is quite okay, I donｴt delete the internal state
//		//sort rootitems
//		CTreeItem** ppSortArray = new CTreeItem*[nItems];
//		// Fill in array with pointers to our children.
//		POSITION posCur = m_RootItems.GetHeadPosition();
//		for (int i=0; posCur; i++)
//		{
//			ppSortArray[i] = (CTreeItem*)m_RootItems.GetAt(posCur);
//			m_RootItems.GetNext(posCur);
//		}
//
//		qsort(ppSortArray, nItems, sizeof(CTreeItem*), CompareChildren);
//		// reorg rootitems with new sorted list
//		posCur = m_RootItems.GetHeadPosition();
//		for (i=0; posCur; i++)
//		{
//			m_RootItems.SetAt(posCur, ppSortArray[i]);
//			m_RootItems.GetNext(posCur);
//		}
//		delete [] ppSortArray;
//		//do a "repaint" of only the rootitems...you could "refresh" the children with a expand
//		int nIndex=0;//no suprise here
//		POSITION pos = GetRootHeadPosition();
//		while(pos != NULL)
//		{
//			CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos);
//			if(pParent!=NULL)
//			{
//				LV_ITEM lvItem;
//				lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
//				CItemInfo* lp = GetData(pParent);
//				CString str = lp->GetItemText();
//				lvItem.pszText = str.GetBuffer(1);
//				lvItem.iItem = nIndex;
//				lvItem.iSubItem = 0;
//				lvItem.lParam = (LPARAM)pParent;//associate the root and all its children with this listviewitem
//				lvItem.iIndent = GetIndent(pParent);
//				CListCtrl::InsertItem(&lvItem);
//				int nSize = lp->GetItemCount();
//				for(i=0; i < nSize; i++)
//				{
//				   CString strSubItems = lp->GetSubItem(i);
//				   lvItem.mask = LVIF_TEXT;
//				   lvItem.iSubItem = i+1;
//				   lvItem.pszText = strSubItems.GetBuffer(1);
//				   SetItem(&lvItem);
//				}
//				nIndex++;
//			}
//		}//while
//	}//nItems
//}


int CSuperGridCtrl::NumChildren(const CTreeItem *pItem) const
{
	return pItem->m_listChild.GetCount();
}


BOOL CSuperGridCtrl::ItemHasChildren(const CTreeItem* pItem) const
{
	BOOL bChildren = pItem->m_listChild.GetCount() != 0;
	//see if we have a flag
	int nFlag = pItem->m_bSetChildFlag;
	if(nFlag!=-1)
		return 1;
	else
		return bChildren;
}


void CSuperGridCtrl::SetChildrenFlag(CTreeItem *pItem, int nFlag) const
{
	pItem->m_bSetChildFlag = nFlag;
}


BOOL CSuperGridCtrl::IsCollapsed(const CTreeItem* pItem) const
{
	return pItem->m_bHideChildren;//e.g not visible
}


void CSuperGridCtrl::Hide(CTreeItem* pItem, BOOL bFlag)
{
	pItem->m_bHideChildren=bFlag;
}


int CSuperGridCtrl::GetIndent(const CTreeItem* pItem) const
{
	return pItem->m_nIndent;
}


void CSuperGridCtrl::SetIndent(CTreeItem *pItem, int iIndent)
{
	pItem->m_nIndent = iIndent;
}


int CSuperGridCtrl::GetCurIndex(const CTreeItem *pItem) const
{
	return pItem->m_nIndex;
}


void CSuperGridCtrl::SetCurIndex(CTreeItem* pItem, int nIndex)
{
	pItem->m_nIndex = nIndex;
}


void CSuperGridCtrl::SetParentItem(CTreeItem*pItem, CTreeItem* pParent)
{
	pItem->m_pParent=pParent;

}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetParentItem(const CTreeItem* pItem)
{
	return pItem->m_pParent;
};



CItemInfo* CSuperGridCtrl::GetData(const CTreeItem* pItem)
{
	return pItem->m_lpNodeInfo;
}




void CSuperGridCtrl::UpdateData(CTreeItem* pItem, CItemInfo* lpInfo, BOOL bUpdateRow)
{
	pItem->m_lpNodeInfo = lpInfo;
	if(bUpdateRow)//update listview item and subitems
		InvalidateItem(pItem);
}


//overrides
CItemInfo* CSuperGridCtrl::CopyData(CItemInfo* lpSrc)
{
	ASSERT(FALSE);//debug
	return NULL;  //release
}

//default implementation for setting icons
int CSuperGridCtrl::GetIcon(const CTreeItem* pItem)
{

	return 0;//just take the first item in CImageList ..what ever that is
}


bool CSuperGridCtrl::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	if(ht.iSubItem==0)
	{
		CRect rcItem;
		GetItemRect(ht.iItem, rcItem, LVIR_LABEL);
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL|ES_LEFT;
		CEdit *pEdit = new CListEditCtrl(ht.iItem, ht.iSubItem, GetItemText(ht.iItem, ht.iSubItem));
		pEdit->Create(dwStyle, rcItem, this, 0x1233);
	} else {
		EditLabelEx(ht.iItem, ht.iSubItem);
	}
	return(false);
}

COLORREF CSuperGridCtrl::GetCellRGB()
{
	return RGB(192,0,0);
}


BOOL CSuperGridCtrl::OnVKMultiply(CTreeItem *pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnVkSubTract(CTreeItem *pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnVKAdd(CTreeItem *pItem, int nIndex)
{
	return 1;
}


BOOL CSuperGridCtrl::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	return 1;
}


// 選択カーソルが変更された
BOOL CSuperGridCtrl::OnChangeSelect(void)
{
	bool	bShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? true : false;
	if (bShift == true || m_bMouseLeft == true) {
		int		iItem = GetSelectedItem();
		int		iSubItem = GetCurSubItem();
		int		sx, sy, ex, ey;
		if (m_bMultiSelect == false) {
			// 範囲選択開始処理
			SelectRangeStart();
			sy = ey = iItem;
			sx = ex = iSubItem;
		} else {
			minmax3(m_posMultiSelect[0].x, m_posMultiSelect[1].x, iSubItem, sx, ex);
			minmax3(m_posMultiSelect[0].y, m_posMultiSelect[1].y, iItem, sy, ey);
			m_posMultiSelect[1].x = iSubItem;
			m_posMultiSelect[1].y = iItem;
		}
		// 選択領域を再描画させる
		InvalidateRangeItem(sx, sy, ex, ey);
	} else {
		if (m_bMultiSelect) {
			// 選択が解除された
			m_bMultiSelect = m_bMultiSelectLine = false;

			// 選択領域を再描画させる
			InvalidateSelectRangeItem();
		}
	}

	return 1;
}


// ユーザーによるセルの背景描画処理
BOOL CSuperGridCtrl::OnDrawCustomCellBG(CDC *pDC, int nItem, int nColumn, RECT &rcItem)
{
	/* SeaKnows 036 */
	/* MySyperGridに移動
	if (m_bMultiSelect) {
		int		sx, sy, ex, ey, nTemp;
		sx = m_posMultiSelect[0].x;
		sy = m_posMultiSelect[0].y;
		ex = m_posMultiSelect[1].x;
		ey = m_posMultiSelect[1].y;
		if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
		if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}

		if (nColumn >= COLUMN_MIN && nItem >= sy && nItem <= ey && nColumn >= sx && nColumn <= ex) {
			CBrush br(RGB(0, 0, 0));
			pDC->FillRect(&rcItem, &br);
			return(TRUE);
		}
	}
	*/
	return(FALSE);
}

// カラムの最大文字数取得処理
int CSuperGridCtrl::OnGetColumLimitText(int, int)
{
	return(80);
}

BOOL CSuperGridCtrl::OnVkReturn(void)
{
	return 0;
}


void CSuperGridCtrl::OnSysColorChange()
{
	CListCtrl::OnSysColorChange();
	//nop nothing yet
}



UINT CSuperGridCtrl::_GetCount(CTreeItem* pItem, UINT& nCount)
{
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		nCount = _GetCount(pChild, nCount);
		nCount++;
	}
	return nCount;
}



UINT CSuperGridCtrl::GetCount(void)
{
	UINT nCount=0;
	UINT _nCount=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		nCount += _GetCount(pRoot, _nCount) + 1;
	}
	return nCount;
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetTreeItem(int nIndex /*nIndex must be valid of course*/ )
{
	return reinterpret_cast<CTreeItem*>(GetItemData(nIndex));
}



int CSuperGridCtrl::GetSelectedItem(void) const
{
	return GetNextItem(-1, LVNI_ALL | LVNI_SELECTED | LVIS_FOCUSED);
	//return GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); /* WildCherry 073 */ /* WildCherry3 083 */
}


void CSuperGridCtrl::InvalidateItem(CTreeItem *pItem)
{
	if(pItem!=NULL)
	{
		int nItem = GetCurIndex(pItem);//just has to be visible
		if(nItem != -1)
		{
			CItemInfo *lp = GetData(pItem);
			int nSize = lp->GetItemCount();
			SetRedraw(false);
			SetItemText(nItem, 0,lp->GetItemText());
			int i; for(i=0; i < nSize;i++)
			{
				CString str = lp->GetSubItem(i);
				SetItemText(nItem, i+1, str.GetBuffer(1));
			}
			SetRedraw(true);
		}
	}
}

void CSuperGridCtrl::InvalidateItemRect(int nItem/*better be valid item*/)
{
	if(nItem != -1)
	{
		CRect rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);
		InvalidateRect(rc);
		UpdateWindow();
	}
}


void CSuperGridCtrl::InvalidateItemRectPtr(CTreeItem *pItem)
{
	if(pItem!=NULL)
	{
		int nItem = GetCurIndex(pItem);
		if(nItem!=-1)
			InvalidateItemRect(nItem);
	}
}


void CSuperGridCtrl::InvalidateRangeItem(int sy, int ey)
{
	int		nTemp, nItem;

	if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}

	for (nItem = sy; nItem <= ey; nItem++) {
		CRect	rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);
		InvalidateRect(rc, FALSE);
	}
	UpdateWindow();
}

void CSuperGridCtrl::InvalidateRangeItem(int sx, int sy, int ex, int ey)
{
	int		nTemp, nItem;

	if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
	if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}

	for (nItem = sy; nItem <= ey; nItem++) {
		CRect	rc;
		int		px;
		GetSubItemRect(nItem, sx, LVIR_BOUNDS, rc);
		px = rc.left;
		GetSubItemRect(nItem, ex, LVIR_BOUNDS, rc);
		rc.left = px;
		InvalidateRect(rc, FALSE);
	}
	UpdateWindow();
}

void CSuperGridCtrl::InvalidateSelectRangeItem(void)
{
	InvalidateRangeItem(m_posMultiSelect[0].x, m_posMultiSelect[0].y, m_posMultiSelect[1].x, m_posMultiSelect[1].y);
}

// =============================================
// CSuperGridCtrl::SelectRangeStart
// 概要  : 範囲選択開始処理
// 引数  : none
// 戻り値: none
// =============================================
void CSuperGridCtrl::SelectRangeStart(int nIndex/* STEP 008 */)
{
	if (m_bMultiSelect == false) {
		int		iItem = nIndex == -1 ? GetSelectedItem() : nIndex /* STEP 008 */;
		int		iSubItem = GetCurSubItem();
		m_posMultiSelect[0].x = iSubItem;
		m_posMultiSelect[0].y = iItem;
		m_posMultiSelect[1] = m_posMultiSelect[0];
		m_bMultiSelect = true;
		m_bMultiSelectLine = iSubItem ? false : true;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000) { /* STEP 001 */
			m_bMultiSelectLine = true;
			m_posMultiSelect[0].x = 0;
			m_posMultiSelect[1].x = 0;
		}
	}
}

bool CSuperGridCtrl::IsCurrentCellEditOK(void)
{
	int		nItem = GetSelectedItem();
	if (nItem >= 0) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL
		&&  ItemHasChildren(pItem) == 0
		&&  GetCurSubItem() >= COLUMN_MIN) {
			CItemInfo* pInfo = GetData(pItem);
			CONTROLTYPE ctrlType;
			if (pInfo->GetControlType(GetCurSubItem()-1, ctrlType) == 1
			&&  ctrlType == invalid) {
				// 編集不可
				return(false);
			}
			// 編集可能
			return(true);
		}
	}
	return(false);
}


// SelItemRange - Selects/Deselect a range of items
// Returns              - The number of new items selected
// bSelect              - TRUE to select, FALSE to deselect
// nFirstItem           - index of first item to select
// nLastItem            - index of last item to select
int CSuperGridCtrl::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
{
	// make sure nFirstItem and nLastItem are valid
	if (nFirstItem == -1) nFirstItem = 0;
	if (nLastItem == -1) nLastItem = GetItemCount() - 1;

	if (nFirstItem >= GetItemCount() || nLastItem >= GetItemCount())
		return 0;

	int		nItemsSelected = 0;
	int		nFlags = bSelect ? 0 : LVNI_SELECTED;
	int		nItem = nFirstItem - 1;
	while((nItem = GetNextItem(nItem, nFlags)) >=0 && nItem <= nLastItem) {
		nItemsSelected++;
		SetItemState(nItem, bSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);
	}
	return nItemsSelected;
}

// 全てのアイテムの選択／選択解除
void CSuperGridCtrl::SelectAllItems(BOOL bSelect)
{
	LV_ITEM theItem;

	theItem.mask		= LVIF_STATE;
	theItem.iItem		= -1;
	theItem.iSubItem	= 0;
	theItem.state		= bSelect ? LVIS_SELECTED : 0;
	theItem.stateMask	= LVIS_SELECTED;

	this->SetItemState(-1, &theItem);

	return;
}


//////////////////////////////////////////////////////////////////////////
//
// not much but ...
//
//////////////////////////////////////////////////////////////////////////

CSuperGridCtrl::CTreeItem::~CTreeItem()
{
	// delete child nodes
	POSITION pos = m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pItem = (CTreeItem*)m_listChild.GetNext(pos);
		if(pItem!=NULL)
		{
			if(pItem->m_lpNodeInfo!=NULL)
				delete pItem->m_lpNodeInfo;
			delete pItem;
		}
	}
	m_listChild.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
//
// Simple class CRectangle for the + - sign,
//
//////////////////////////////////////////////////////////////////////////

CRectangle::CRectangle(CSuperGridCtrl* pCtrl, CDC* pDC, int iIndent, const CRect& rcBounds)
{
	m_pDC=pDC;
	int nHalfImage = (pCtrl->m_cxImage >> 1);
	int nBottomDown = rcBounds.top + nHalfImage + ((rcBounds.Height() - pCtrl->m_cyImage) >> 1);
	m_right_bottom.cx = (pCtrl->m_cxImage>>1)+2+1;
	m_right_bottom.cy = (pCtrl->m_cyImage>>1)+2+1;
	m_left = rcBounds.left  + iIndent * pCtrl->m_cxImage - nHalfImage;
	m_top = nBottomDown - (m_right_bottom.cy >> 1);
	m_left_top.x = m_left -  (m_right_bottom.cx >> 1);
	m_left_top.y = m_top;
	m_topdown = nBottomDown;
}


void CRectangle::DrawRectangle(CSuperGridCtrl* pCtrl)
{
	//erase bkgrnd
	CRect rc(m_left_top, m_right_bottom);
	m_pDC->FillRect(rc, &pCtrl->m_brushErase);
	//draw rectangle
	CPen* pPenRectangle = m_pDC->SelectObject(&pCtrl->m_psRectangle);
	m_pDC->Rectangle(rc);
	m_pDC->SelectObject(pPenRectangle);
}


CRectangle::~CRectangle()
{
}


BOOL CRectangle::HitTest(CPoint pt)
{
	CRect rc = GetHitTestRect();
	return rc.PtInRect(pt);
}


void CRectangle::DrawPlus(void)
{
	m_pDC->MoveTo(m_left, m_topdown-2);
	m_pDC->LineTo(m_left, m_topdown+3);

	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}


void CRectangle::DrawMinus(void)
{
	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}

void CSuperGridCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
}

int CSuperGridCtrl::MyDrawText(CDC* pDC, LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat) /* BeachMonster 107 */
{
	CString strText = lpszString;
	if (g_bOptShowZenSpace && (nFormat & (DT_CENTER | DT_RIGHT)) == 0/* RockDance 125 */ &&
			((strText.Find(TEXT("\r\n")) != -1 || strText.Find(TEXT("　")) != -1)/* Misirlou 147 */ || strText.Find(TEXT("\t"))/* Baja 156 */)) { /* 2003.06.26 change */
		COLORREF	colOrg;
		CString strChar = "";
		CString strNormal = "";
		CRect rect = lpRect;
		//rect.right = rect.left;
		colOrg = pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetTextColor(colOrg);
		BOOL bLead = FALSE;
		for (int i=0;i<strText.GetLength();i++) {
			if ((_ismbblead(strText[i]) || strText[i] == '\r') && !bLead) {
				bLead = TRUE;
				strChar += strText[i];
				continue;
			} else {
				bLead = FALSE;
				strChar += strText[i];
				if (strChar == "　") {
					pDC->DrawText(strNormal, rect, nFormat);
					rect.left += pDC->GetTextExtent(strNormal).cx;
					CPen pen(PS_SOLID, 1, RGB(192, 192, 192));
					CPen* oldPen = (CPen*)pDC->SelectObject(&pen);
					CRect box(rect);
					box.right = box.left + pDC->GetTextExtent(strChar).cx;
					box.DeflateRect(2, 2);
					pDC->MoveTo(box.left, box.top);
					pDC->LineTo(box.left, box.bottom);
					pDC->LineTo(box.right, box.bottom);
					pDC->LineTo(box.right, box.top);
					pDC->LineTo(box.left, box.top);
					pDC->SelectObject(oldPen);
					pen.DeleteObject();
				} else if (strChar == "\r\n") {
					pDC->DrawText(strNormal, rect, nFormat);
					rect.left += pDC->GetTextExtent(strNormal).cx;
					pDC->SetTextColor(RGB(192, 192, 192));
					strChar = "↓"; /* RockDance 137 */
					pDC->DrawText(strChar/* RockDance 137 */, 2, rect, nFormat);
				} else if (strChar == "\t") { /* Baja 156 */
					pDC->DrawText(strNormal, rect, nFormat);
					rect.left += pDC->GetTextExtent(strNormal).cx;
					pDC->SetTextColor(RGB(192, 192, 192));
					strChar = "→";
					pDC->DrawText(strChar, 2, rect, nFormat);
				} else if (strChar.FindOneOf(g_sOptShowOtherChar) != -1) {
					pDC->DrawText(strNormal, rect, nFormat);
					rect.left += pDC->GetTextExtent(strNormal).cx;
					pDC->SetTextColor(RGB(255, 0, 123));
					pDC->DrawText(strChar, 1, rect, nFormat);
					//CRect rectChar = rect;
					//rectChar.right = rectChar.left + pDC->GetTextExtent(strChar).cx;
					//pDC->FillSolidRect(&rectChar, RGB(192, 192, 192));
				} else {
					//pDC->SetTextColor(colOrg);
					//pDC->DrawText(strChar, rect, nFormat);
					strNormal += strChar;
					strChar.Empty();
					continue;
				}
				rect.left += pDC->GetTextExtent(strChar).cx;
				strChar.Empty();
				strNormal.Empty();
				pDC->SetTextColor(colOrg);
				if (rect.left >= rect.right) { /* Hurricane 211 */
					break;
				}
			}
		}
		//pDC->SetTextColor(colOrg);
		if (!strNormal.IsEmpty()) {
			pDC->DrawText(strNormal, rect, nFormat);
		}
	} else {
		pDC->DrawText(lpszString,-1,lpRect, nFormat);
	}
	return 0;
}

void CSuperGridCtrl::OnUpdateTotal()
{

}

void CSuperGridCtrl::OnRefreshTreeItem(CTreeItem* pOldItem, CTreeItem* pNewItem) /* STEP 037 */
{

}


void CSuperGridCtrl::SetRedraw(BOOL bRedraw)
{
	if (! bRedraw) {
		if (m_redrawcount++ <= 0) {
		   CListCtrl::SetRedraw(false);
		}
	} else {
		if (--m_redrawcount <= 0) {
		   CListCtrl::SetRedraw(true);
		   m_redrawcount = 0;
		   //Invalidate();
		   //UpdateWindow();
		}
	}
}

void CSuperGridCtrl::UpdateWindow()
{
#ifdef FLICKERFREE
	if (m_redrawcount > 0) return;
#endif
	CListCtrl::UpdateWindow();
}

CString CSuperGridCtrl::GetItemText(int nItem, int nSubItem) /* Hurricane 209 */
{
	/* 256文字以上取得できるように */
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	CString str;
	//int nLen = 	OnGetColumLimitText(nItem, nSubItem);
	int nRes, nLen;
	int i = 1;
	do {
		nLen = i * 512;
		lvi.cchTextMax = nLen;
		lvi.pszText = str.GetBufferSetLength(nLen);
		nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
		str.ReleaseBuffer();
		i++;
	} while (!(nRes < nLen-1));
	return str;
}
