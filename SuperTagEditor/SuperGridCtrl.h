#if !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
#define AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuperGridCtrl.h : header file
//
#include <afxtempl.h>
#include "HeaderCtrlEx.h"

#include "STEP_api.h"

#define COMBO_BOX_COPY_USED		0

enum	{INSERT_HEAD, INSERT_TAIL, INSERT_DIRECTORY, INSERT_FILE, INSERT_NEXT};

class CItemInfo //should have called this LV_INSERTITEM ..what ever
{
public:
	CItemInfo():m_bCheck(0),m_iImage((const int)-1),m_lParam(NULL),m_clf((COLORREF)-1){};
	void SetItemText(const CString& strItem){ m_strItemName = strItem; }
	void SetItemText(const CString& strItem, COLORREF clf) { m_strItemName = strItem; m_clf = clf;}
	void AddSubItemText(const CString& strSubItem){ m_SubItems.Add(strSubItem); }
	void AddSubItemText(const CString& strSubItem, COLORREF clf)
	{
		int nIndex = m_SubItems.Add(strSubItem);
		m_mapClf.SetAt(nIndex, clf);
	}
	void SetSubItemText(int iSubItem, const CString& strSubItem, int iImage)
	{
		m_SubItems.SetAtGrow(iSubItem, strSubItem);
		m_iImageSubItems.SetAt(iSubItem, iImage);
	}
	void SetSubItemImage(int iSubItem,int iImage){m_iImageSubItems.SetAt(iSubItem, iImage);}

	void SetSubItemText(int iSubItem, const CString& strSubItem){ m_SubItems.SetAtGrow(iSubItem, strSubItem); }
	void SetSubItemText(int iSubItem, const CString& strSubItem, COLORREF clf)
	{
		m_SubItems.SetAtGrow(iSubItem, strSubItem);
		m_mapClf.SetAt(iSubItem, clf);
	}

	const CString& GetItemText(void){ return m_strItemName; }
	CString GetSubItem(int iSubItem)
	{
		if(iSubItem >= 0 && iSubItem < m_SubItems.GetSize())
			return m_SubItems.GetAt(iSubItem);
		else
			return _T("");
	}
	int GetItemCount(void) const { return m_SubItems.GetSize(); }

//	enum CONTROLTYPE {edit/*default*/, combobox, invalid, datecontrol/*your control*/, spinbutton/*your control*/, dropdownlistviewwhatevercontrol/*your control*/, multiedit/* BeachMonster 089 */, multieditreadonly};
	//all cols in this row is default edit
	void SetControlType(CONTROLTYPE enumCtrlType, int nCol=-1){ m_controlType.SetAt(nCol, enumCtrlType); }

	BOOL GetControlType(int nCol, CONTROLTYPE& controlType) const
	{
		if(!m_controlType.Lookup(nCol,controlType))
		{
			controlType = edit;//default;
			return 0;
		}
		return (controlType == edit || controlType == multiedit/* BeachMonster 089 */ || controlType == multieditreadonly) ? 0 : 1;
	}

	void SetListData(int iSubItem, CStringList *strInitArr)
	{
#if COMBO_BOX_COPY_USED
		CStringList *list;
		list = new CStringList;//will be deleted in destructor
		list->AddTail(strInitArr);
		m_listdata.SetAt(iSubItem, list);
#else
		m_listdata.SetAt(iSubItem, strInitArr);
#endif
	}

	BOOL GetListData(int iSubItem, CStringList*& pList) const
	{
		return m_listdata.Lookup(iSubItem, pList);
	}

	//used if LVS_EX_CHECKBOXES style
	void SetCheck(BOOL bCheck){ m_bCheck = bCheck;}
	BOOL GetCheck(void) const {return m_bCheck;}
	//icon stuff
	void SetImage(int iImage){m_iImage = iImage;}
	int GetImage(void) const {return m_iImage;}//return icon in tree column
	int GetSubItemImage(int iSubItem) const
	{
		int iSubImage=-1;
		m_iImageSubItems.Lookup(iSubItem, iSubImage);
		return iSubImage;
	}
	//color stuff
	COLORREF GetItemClr(void) const {return m_clf;}
	COLORREF GetBkColor(int iSubItem) const
	{
		COLORREF clref;
		if(!m_mapClf.Lookup(iSubItem,clref))
		{
			return (COLORREF)-1;
		}
		return clref;
	}

	LPARAM m_lParam;//why not use this like you use it in listbox, listctrl...
	void SetLParam(LPARAM param) {m_lParam = param;}
	LPARAM GetLParam(void) {return(m_lParam);}

	// 拡張処理用フラグ(３２ビット)
	enum	{
		FLAG_EX_CHECK_MASK		= 0x00000003,
	};
	enum	{FLAG_EX_CHECK_OFF, FLAG_EX_CHECK_ON, FLAG_EX_CHECK_GRAY};
	UINT	m_dwFlagExt;
	void SetFlagExt(UINT flag) {m_dwFlagExt = flag;}
	UINT GetFlagExt(void) {return(m_dwFlagExt);}

	//YOU SHOULD MODIFY THIS WHEN EVER YOU ADD NEW DATA TO THIS CLASS
	void CopyObjects(CItemInfo* pItemInfo)
	{
		SetItemText(pItemInfo->GetItemText());
		m_SubItems.Copy(pItemInfo->m_SubItems);
		CopyControls(pItemInfo);
		CopyColors(pItemInfo);
		SetCheck(pItemInfo->GetCheck());
		SetImage(pItemInfo->GetImage());
		m_lParam = pItemInfo->m_lParam;
		m_clf=pItemInfo->m_clf;
		CopySubItemImageList(pItemInfo);
	}

	void CopySubItemImageList(CItemInfo* pItemInfo)
	{
		POSITION pos = pItemInfo->m_iImageSubItems.GetStartPosition();
		while(pos != NULL)
		{
			int nKey;
			int iImage=-1;
			pItemInfo->m_iImageSubItems.GetNextAssoc(pos, nKey, iImage);
			if(iImage!=-1)
				m_iImageSubItems.SetAt(nKey, iImage);
		}
	}

	void CopyControls(CItemInfo* pItemInfo)
	{
		for(int nCol=0; nCol < pItemInfo->GetItemCount(); nCol++)
		{
			CONTROLTYPE ctrlType;
			if(pItemInfo->GetControlType(nCol, ctrlType))//true if other than edit-control
			{

				SetControlType(ctrlType, nCol);
				//should test if this is listdata material
				CStringList *list = NULL;
				pItemInfo->GetListData(nCol, list);
				if(list!=NULL)
					SetListData(nCol, list);
			}
		}
	}

	void CopyColors(CItemInfo* pItemInfo)
	{
		POSITION pos = pItemInfo->m_mapClf.GetStartPosition();
		while(pos != NULL)
		{
			int nKey;
			COLORREF clref;
			pItemInfo->m_mapClf.GetNextAssoc(pos, nKey, clref);
			m_mapClf.SetAt(nKey, clref);
		}
	}

	~CItemInfo()
	{
#if COMBO_BOX_COPY_USED
		POSITION pos = m_listdata.GetStartPosition();
		while(pos != NULL)
		{
			int nKey;
			CStringList* b;
			m_listdata.GetNextAssoc(pos, nKey, b);
			if(b!=NULL)
			{
				b->RemoveAll();
				delete b;
			}
		}
		m_listdata.RemoveAll();
#endif
	}

private:
	CONTROLTYPE m_enumCtrlType;
	CMap<int,int, CONTROLTYPE, CONTROLTYPE&> m_controlType;//hmm
	CMap<int,int, COLORREF, COLORREF&> m_mapClf;//colors
	CMap<int,int, CStringList*, CStringList*> m_listdata;//listbox
	CString m_strItemName;//col 0...
	CStringArray m_SubItems;//col 1... N
	BOOL m_bCheck;
	int m_iImage;
	CMap<int,int,int,int&> m_iImageSubItems;
	COLORREF m_clf;
};


/////////////////////////////////////////////////////////////////////////////
//
// CSuperGridCtrl
//
/////////////////////////////////////////////////////////////////////////////
#ifdef FLICKERFREE
//#include "ListCtrlEx.h"
#define CListCtrlEx CListCtrl
#else
#define CListCtrlEx CListCtrl
#endif
class CSuperGridCtrl : public CListCtrlEx /*CListCtrl */
{
// Construction
public:
	CSuperGridCtrl();
	//nested class forward decl.
	class CTreeItem;
protected:
	CHeaderCtrlEx	m_headerCtrl;
public:
	// Overrides
	//MUST override this to make a copy of CItemInfo..see the CMySuperGrid.cpp for implementation
	//used in drag/drop operations
	virtual CItemInfo* CopyData(CItemInfo* lpSrc);
	//sets the icon state...called from within DrawItem returns valid image index
	//You MUST override this function to set your current icon, must of course be a valid CImageList index
	virtual int GetIcon(const CTreeItem* pItem);
	//override this to set the color for current cell
	virtual COLORREF GetCellRGB(void);
	//override this to update listview items, called from within OnEndlabeledit.
	virtual void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	//override this to activate any control when lButtonDown in a cell, called from within OnLButtonDown
	virtual bool OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);
	//override this to provide your dragimage, default: creates an image + text
	virtual CImageList *CreateDragImageEx(int nItem);
    //called before item is about to explode, return TRUE to continue, FALSE to prevent expanding
	virtual BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	//called after item has expanded
	virtual BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	//called before item are collapsed,return TRUE to continue, FALSE to prevent collapse
	virtual BOOL OnCollapsing(CTreeItem *pItem);
	//called after item has collapsed
	virtual BOOL OnItemCollapsed(CTreeItem *pItem);
	//called before item is about to be deleted,return TRUE to continue, FALSE to prevent delete item
	virtual BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	//called before VK_MULTIPLY keydown, return TRUE to continue, FALSE to prevent expandall
	virtual BOOL OnVKMultiply(CTreeItem *pItem, int nIndex);
	//called before VK_SUBTRACT keydown, return TRUE to continue, FALSE to prevent collapse item
	virtual BOOL OnVkSubTract(CTreeItem *pItem, int nIndex);
	//called before VK_ADD keydown, return TRUE to continue, FALSE to prevent expanding item
	virtual BOOL OnVKAdd(CTreeItem *pItem, int nIndex);
	//called from PreTranslateMessage, override this to handle other controls than editctrl's
	virtual BOOL OnVkReturn(void);
	//called before from within OnlButtonDown and OnDblclk, but before anything happens, return TRUE to continue, FALSE to say not selecting the item
	virtual BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	// 選択カーソルが変更された
	virtual BOOL OnChangeSelect(void);
	// ユーザーによるセルの背景描画処理
	virtual BOOL OnDrawCustomCellBG(CDC *, int, int, RECT &);
	// カラムの最大文字数取得処理
	virtual int OnGetColumLimitText(int, int);
	// スペースキーが押された場合の処理
	virtual void OnKeyDownSpace(void);
	// チェック状態が変更された
	virtual void OnChangeCheck(CTreeItem *, BOOL);
	// アイテムがダブルクリックされた
	virtual BOOL OnItemDblClick(int);

public:
	//creates a root
	CTreeItem*  InsertRootItem(CItemInfo * lpRoot);
	//from the looks of it,...it deletes a rootitem
	void DeleteRootItem(CTreeItem * lpRoot);
	//hmm
	BOOL IsRoot(CTreeItem * lpItem);
	//given the rootindex it returns the rootptr
	CTreeItem* GetRootItem(int nIndex);
	int GetRootCount() { return m_RootItems.GetCount();}
	//call this to delete all items in grid
	void DeleteAll();
	//return previous node from pItem, given a RootItem
	CTreeItem* GetPrev(CTreeItem* pRoot, CTreeItem *pItem, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//return next node from pItem, given a RootItem
	CTreeItem* GetNext(CTreeItem* pRoot, CTreeItem* pNode, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//returns the selected item :)
	int GetSelectedItem(void) const;
	//returns the itemdata associated with the supergrid
	CTreeItem* GetTreeItem(int nIndex);
	// Retrieves the number of items associated with the SuperGrid control.
	UINT GetCount(void);
	//returns number of children given the pItem node ptr.
	int NumChildren(const CTreeItem *pItem) const;
	//Determines if this tree item is a child of the specified parent
	BOOL IsChildOf(const CTreeItem* pParent, const CTreeItem* pChild) const;
	//hmm
	BOOL ItemHasChildren(const CTreeItem* pItem) const;
	// Use this to indicate that pItem has children, but has not been inserted yet.
	void SetChildrenFlag(CTreeItem *pItem,int nFlag) const;
	//are children collapsed
	BOOL IsCollapsed(const CTreeItem* pItem) const;
	//return the Indent Level of pItem
	int GetIndent(const CTreeItem* pItem) const;
	//Sets the Indentlevel of pItem
	void SetIndent(CTreeItem *pItem, int iIndent);
	//get the pItems current listview index,
	int GetCurIndex(const CTreeItem *pItem) const;
	//set pItems current listview index
	void SetCurIndex(CTreeItem* pItem, int nIndex);
	//sets the pItem' parent
	void SetParentItem(CTreeItem*pItem, CTreeItem* pParent);
	//gets pItems parent
	CTreeItem* GetParentItem(const CTreeItem* pItem);
	//return ptr to CItemInfo daaa
	CItemInfo* GetData(const CTreeItem* pItem);
	//sets the CItemInfo ptr of pItem, if bUpdateRow true, it uses the InvalidateItem(CTreeItem pItem)
	void UpdateData(CTreeItem* pItem, CItemInfo* lpInfo, BOOL bUpdateRow=FALSE);
	//Insert item and return new parent node.
	//the bUpdate is here for performance reasons, when you insert say 100 node each having 10 children(1000 listview items)
	//the bUpdate should be set to FALSE(default) but when you want to insert an item, and you want to user to see it right away
	//set bUpdate to TRUE.(see the use of bUpdate in the HowToInsertItemsAfterTheGridHasBeenInitialized function in the CMySuperGridCtrl)
	CTreeItem* InsertItem(CTreeItem *pParent, CItemInfo* lpInfo, BOOL bUpdate=FALSE, int nAddType = INSERT_TAIL);
	//collapse all children from pItem
	void Collapse(CTreeItem *pItem);
	//expand one folder and return the last index of the expanded folder
	int Expand(CTreeItem* pSelItem, int nIndex);
	//expand all items from pItem and return last index of the expanded folder
	void ExpandAll(CTreeItem *pItem, int& nScroll);
	//expand all node in pItem and stop at pStopAt, used in SelectNode function
	void ExpandUntil(CTreeItem *pItem, CTreeItem* pStopAt);
	//use this if you want to select a node
	//if the node is collapsed all items with in the node are expanded and the node is selected
	//it returns the listview index for the selected node
	int SelectNode(CTreeItem *pLocateNode);
	//Delete an item in the listview
	//takes the node to be delete and its listview item index as arg.
	//note the item you delete must be visible, hence the nItem as arg.
	void DeleteItemEx(CTreeItem *pSelItem, int nItem);
	void DeleteItemExNoRedraw(CTreeItem *pSelItem, int nItem);
	//returns the number of columns in the listview
	int GetNumCol(void);
	//does a Quicksort of the rootitems and all children if bSortChildren set TRUE
//	void SortEx(BOOL bSortChildren);
	//does a Quicksort of the pParents children and if bSortChildren set,
	//all items from pParent are sorted.
//	void Sort(CTreeItem* pParent, BOOL bSortChildren);
	// simpel wrapper for the CObList in CTreeItem, same usage as in the COblist
	POSITION GetHeadPosition(CTreeItem* pItem) const;
	POSITION GetTailPosition(CTreeItem *pItem) const;
	CTreeItem* GetNextChild(CTreeItem *pItem, POSITION& pos) const;
	CTreeItem* GetPrevChild(CTreeItem *pItem, POSITION& pos) const;
	void AddTail(CTreeItem *pParent, CTreeItem *pChild);
	//simpel wrapper for the CPtrList collection of rootitems
	//feel free to add more of these simple wrappers.
	POSITION GetRootHeadPosition(void) const;
	POSITION GetRootTailPosition(void) const;
	CTreeItem* GetNextRoot(POSITION& pos) const;
	CTreeItem* GetPrevRoot(POSITION& pos) const;
	//invalidates the nItems rectbound.
	void InvalidateItemRect(int nItem);
	//invalidates the pItem rectbound.
	void InvalidateItemRectPtr(CTreeItem *pItem);
	//use this when you do dynamic updates,
	//it writes the content of pItem to the listview
	void InvalidateItem(CTreeItem *pItem);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperGridCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CSuperGridCtrl();
protected:
	//given the rootptr it returns the rootindex.
	int GetRootIndex(CTreeItem * lpRoot);
	//delete pItem and all of its children
	BOOL Delete(CTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	//used in drag/drop operation..
	void CopyChildren(CTreeItem* pDest, CTreeItem* pSrc);
	//drag/drop thing....clipboard not supported!
	BOOL DoDragDrop(CTreeItem* pTarget, CTreeItem* pSelItem);
	//updates internal nodes, called when ever insert,delete on listview
	void InternaleUpdateTree(void);
	//Get ListView Index from pNode
	int NodeToIndex(CTreeItem *pNode);
	//see if user clicked the [+] [-] sign, also handles LVS_EX_CHECKBOXES.
	BOOL HitTestOnSign(CPoint point, LVHITTESTINFO& ht);
	//positions an edit-control and creates it
	CEdit* EditLabelEx(int nItem, int nCol);
	int m_cxImage, m_cyImage;//icon size
	//translates column index value to a column order value.
	int IndexToOrder(int iIndex);
	//set hideflag for pItem
	void Hide(CTreeItem* pItem, BOOL bFlag);
	//hmm
	int GetCurSubItem(void){return m_CurSubItem;}
	void SetCurSubItem(int iSubItem) {m_CurSubItem = iSubItem;}
	int GetDragItem(void) const {return m_nDragItem; }
	int GetDropTargetItem(void) const {return m_nDragTarget; }
	BOOL SetSubItemImageList(CImageList *pImageList=NULL)
	{
		if(pImageList!=NULL)
		{
			m_iSubItemImage.Create(pImageList);
			int nCount=pImageList->GetImageCount();
			for (int i=0; i < nCount; i++)
				 m_iSubItemImage.Copy(i,pImageList,i);

			return TRUE;
		}
		else return FALSE;
	}
	void MakeColumnVisible(int nCol);
	//hmm
private:
	int MyDrawText(CDC* pDC, LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat ); /* BeachMonster 107 */
	//imagelist for subitems
	CImageList m_iSubItemImage;
	//list of rootItems
	CPtrList m_RootItems;
	//internal helpers
	BOOL _Delete(CTreeItem* pStartAt, CTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	UINT _GetCount(CTreeItem* pItem, UINT& nCount);
	//hmm
	void DrawTreeItem(CDC* pDC, CTreeItem* pSelItem, int nListItem, const CRect& rcBounds);
	//makes the dot ... thing
	LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	//set the hideflag from pItem and all its children
	void HideChildren(CTreeItem *pItem, BOOL bHide, int iIndent);
	//checks whether a column is visible, if not scrolls to it
	void DrawFocusCell(CDC *pDC, int nItem, int iSubItem,int nIconOffset);
	//draw the down arrow if any combobox precent in the listview item
	void DrawComboBox(CDC *pDC, CTreeItem *pSelItem, int nItem, int nColumn,int iSubIconOffset);
	int m_CurSubItem;//keyboard handling..it is what it says
	//hmm these represents the state of my expresso machine
    int m_nDragItem, m_nDragTarget;
    BOOL m_bIsDragging;
	//helper compare fn used with Quicksort
	static int CompareChildren(const void* p1, const void* p2);
	void CleanMe(CTreeItem *pItem);
	int _NodeToIndex(CTreeItem *pStartpos, CTreeItem* pNode, int& nIndex, BOOL binit = TRUE);
	CPen m_psTreeLine;
	CPen m_psRectangle;
	CPen m_psPlusMinus;
	CBrush m_brushErase;
	friend class CRectangle;
	HIMAGELIST m_himl;
	// 現在のカーソル位置のセル編集開始
	bool	CellEditStart(void);
	// 編集ウィンドウの位置を調整する
	void	AdjustEditWindowPos(void);

	// 範囲選択処理用
public:
	CString GetItemText(int nItem, int nSubItem);
	void UpdateWindow();
	void SetRedraw( BOOL bRedraw);
	virtual void OnUpdateTotal();
	virtual void CSuperGridCtrl::OnRefreshTreeItem(CTreeItem* pOldItem, CTreeItem* pNewItem); /* STEP 037 */
    bool	m_bMouseLeft;
    int		m_nAutoScrollTimer;
	bool	m_bMultiSelect;
	bool	m_bMultiSelectLine;
	POINT	m_posMultiSelect[2];
	void	InvalidateRangeItem(int, int);
	void	InvalidateRangeItem(int, int, int, int);
	void	InvalidateSelectRangeItem(void);
	void	SelectRangeStart(int nIndex = -1);
	bool	IsCurrentCellEditOK(void);
	void	SelectAllItems(BOOL = TRUE);
	int		SelItemRange(BOOL = FALSE, int = 0, int = -1);
	inline	bool		IsRangeSelected(void) {
				if (m_bMultiSelect
				&&  (m_posMultiSelect[0].x >= 1 || m_posMultiSelect[1].x >= 1)) {
					return(true);
				}
				return(false);
			}
	inline	const POINT	*GetRangeSelect(void) {return(&m_posMultiSelect[0]);}

	BOOL	IsItemFile(CTreeItem *pItem) {
		return(((int)GetData(pItem)->GetLParam() >= 0) ? TRUE : FALSE);
	}
	void	SetCheckEx(int, BOOL);
	void	SetCheckEx(CTreeItem *, BOOL, BOOL = FALSE);
	CFont* m_rootItemFont;

	// Generated message map functions
protected:
	int m_redrawcount;
	//{{AFX_MSG(CSuperGridCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSysColorChange();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class CRectangle
{
public:
	CRectangle(CSuperGridCtrl* pCtrl, CDC* pDC, int iIndent, const CRect& rcBounds);
	~CRectangle();
	void DrawRectangle(CSuperGridCtrl *pCtrl);
	BOOL HitTest(CPoint pt);
	void DrawPlus(void);
	void DrawMinus(void);
	int GetLeft(void){return m_left;}
	int GetTop(void){return m_top;}
	CRect GetHitTestRect(void) { return CRect(m_left_top, m_right_bottom);}
private:
	CDC *m_pDC;
	SIZE m_right_bottom;
	int m_left;
	int m_top;
	POINT m_left_top;
	int m_topdown;
};

//the nested class
class CSuperGridCtrl::CTreeItem : public CObject
{
	CTreeItem(): m_pParent(NULL), m_bHideChildren(0), m_nIndex(-1), m_nIndent(-1),m_bSetChildFlag(-1){};
	~CTreeItem();
	CObList m_listChild;
	CTreeItem* m_pParent;
	CItemInfo* m_lpNodeInfo;
	BOOL m_bHideChildren;
	int m_nIndex; //CListCtrl index
	int m_nIndent;
	int m_bSetChildFlag;
	friend class CSuperGridCtrl;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
