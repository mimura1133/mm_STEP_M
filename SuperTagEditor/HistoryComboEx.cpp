/*********************************************************************************************/
// File:	HistoryComboEx.h
// Version: 1.0
// Created: 22. 10. 2001
//
// This class is based of CHistoryCombo from Paul S. Vicherky and it enables you to load and 
// save the items of an ComboBoxEx.
//
// The modifications are maded by: Nabil Hussein
// E-mail:						   unc@index.com.jo
/*********************************************************************************************/

////////////////////////////////////////////////////////////////////////////
// File:	HistoryCombo.cpp
// Version:	1.0.0.0
// Created:	12-Apr-2001
//
// Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
//
// Implementation of CHistoryCombo which incorporates functionality to help
// with Loading and Saving of history in a combo box
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2001 Paul S. Vickery
//
// PLEASE LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

// HistoryComboEx.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryComboEx.h"

#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_HISTORY_ITEMS 10

#include <atlbase.h>  // required for CRegKey

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboEx

CHistoryComboEx::CHistoryComboEx()
{
	m_nMaxHistoryItems = MAX_HISTORY_ITEMS;
	m_bSaveRestoreLastCurrent = TRUE;
}

CHistoryComboEx::~CHistoryComboEx()
{
}

BOOL CHistoryComboEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// warn if creating with CBS_SORT style
	// (unfortunately we can't turn it off)
	if (cs.style & CBS_SORT)
		TRACE("WARNING: Creating History combo with CBS_SORT style\n");
	
	return CComboBoxEx::PreCreateWindow(cs);
}

BEGIN_MESSAGE_MAP(CHistoryComboEx, CComboBoxEx)
	//{{AFX_MSG_MAP(CHistoryComboEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboEx message handlers

// this version of InsertItem adds a string only if it doesn't already exist
// in the list, and in any case, makes sure that the string is the first
// in the list (ie most recent in history)
// also makes sure number of items in the list doesn't exceed the maximum allowed
int CHistoryComboEx::InsertItem(const COMBOBOXEXITEM *pCBItem)
{
	// if it's not set up as a history combo then call base class
	if (m_sSection.IsEmpty() || m_sKeyPrefix.IsEmpty())
		return CComboBoxEx::InsertItem(pCBItem);

	int nRet = -1;

	// don't add it already there
	COMBOBOXEXITEM cbiTemp;	
	cbiTemp = *pCBItem;
	CString str = (CString) cbiTemp.pszText;
	
	//str.TrimLeft(" ");
	//str.TrimRight(" ");
	cbiTemp.pszText = (LPTSTR)(LPCTSTR) str;	
	cbiTemp.iItem = 0;
	//nRet = CComboBoxEx::InsertItem(&cbiTemp);
	
	int nIndex = FindStringExact(0, cbiTemp.pszText);
	
	if (nIndex != -1/* && nIndex != 0*/)
		DeleteString(nIndex);
	nRet = CComboBoxEx::InsertItem(&cbiTemp);
	
	SetCurSel(nRet);
	
	return nRet;
}

// This version enables you to send a CString value only.
int CHistoryComboEx::InsertItem(CString strItem)
{
	COMBOBOXEXITEM pCBItem;

	pCBItem.mask = CBEIF_TEXT;
	pCBItem.iItem = 0;
	pCBItem.pszText = (LPTSTR)(LPCTSTR)strItem;

	return InsertItem (&pCBItem);
}

// loads the history from the specified profile area, and returns the 
// text selected
// the profile area is kept so that in doesn't need to specified again
// when saving the history
CString CHistoryComboEx::LoadHistory(LPCTSTR lpszSection, LPCTSTR lpszKeyPrefix, BOOL bSaveRestoreLastCurrent, LPCTSTR lpszKeyCurItem)
{
	if (lpszSection == NULL || lpszKeyPrefix == NULL || *lpszSection == '\0')
		return "";

	COMBOBOXEXITEM cbiItem;
	cbiItem.mask = CBEIF_TEXT;
	cbiItem.iItem = 0;

	m_sSection = lpszSection;
	m_sKeyPrefix = lpszKeyPrefix;
	m_sKeyCurItem = lpszKeyCurItem == NULL ? TEXT("") : lpszKeyCurItem;
	m_bSaveRestoreLastCurrent = bSaveRestoreLastCurrent;
	//CWinApp* pApp = AfxGetApp();

	int n = 0;
	CString sText;

	CString strINI = AfxGetApp()->m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	Profile_Initialize(strINI, TRUE);
	do
	{
		CString sKey;
		sKey.Format(TEXT("%s%d"), m_sKeyPrefix, n++);
		sText = MyGetProfileString(m_sSection, sKey, NULL);
		cbiItem.pszText = const_cast<LPTSTR>(implicit_cast<LPCTSTR>(sText));
    
		if (!sText.IsEmpty()) {
			CComboBoxEx::InsertItem(&cbiItem);
			cbiItem.iItem++;
		}
	} while (!sText.IsEmpty());
  
	if (m_bSaveRestoreLastCurrent)
	{
		CString sKey;
		
		if (!m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
    
		sText = MyGetProfileString(m_sSection, sKey, NULL);
    
		if (!sText.IsEmpty())
		{
			int nIndex = FindStringExact(-1, sText);
			if (nIndex != -1)
				SetCurSel(nIndex);
      		else if (GetStyle() & CBS_DROPDOWN)
				SetWindowText(sText);
		}
	}
	//Profile_Free();
	Profile_Flush(strINI);
	Profile_Free();
  
	return sText;
}

// loads the history from the specified MRU list, and populates the combo list
// if bSelectMostRecent is TRUE, selects the most recent into the edit control
// returns the selected text
// if using this overload of LoadHistory then SaveHistory will not function
// but you should simply save the MRU in the normal way instead
// note that the MRU should have already been read from the profile before
// being passed to this function, as this function will not call MRU->ReadList()

CString CHistoryComboEx::LoadHistory(CRecentFileList *pListMRU, BOOL bSelectMostRecent)
{
	if (pListMRU == NULL)
		return "";

	int nNumItems = pListMRU->GetSize();
	COMBOBOXEXITEM cbiItem;
	cbiItem.mask = CBEIF_TEXT;
	cbiItem.iItem = 0;

	for (int n = 0; n < nNumItems; n++)
	{		
		cbiItem.pszText = const_cast<LPTSTR>(implicit_cast<LPCTSTR>((*pListMRU)[n]));
		CComboBoxEx::InsertItem(&cbiItem);
	}
	
	if (bSelectMostRecent)
		SetCurSel(0);
  
	CString sText;
  	GetWindowText(sText);
  
	return sText;
}

// saves the history to the profile specified when calling LoadHistory
// if no profile information (ie LoadHistory() wasn't called with it) then
// this function does nothing
void CHistoryComboEx::SaveHistory(BOOL bAddCurrentItemtoHistory)
{
	COMBOBOXEXITEM cbiItem;
	cbiItem.mask = CBEIF_TEXT;
	cbiItem.iItem = 0;

	if (m_sSection.IsEmpty())
		return;

	//CWinApp* pApp = AfxGetApp();
  	CString strINI = AfxGetApp()->m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	Profile_Initialize(strINI, TRUE);

	if (bAddCurrentItemtoHistory)
	{
		CString sCurItem;
		GetWindowText(sCurItem);
		// trim it, so we items which differ only by a leading/trailing space
		//sCurItem.TrimLeft();
		//sCurItem.TrimRight();
		
		if (!sCurItem.IsEmpty())
		{
			cbiItem.pszText = const_cast<LPTSTR>(implicit_cast<LPCTSTR>(sCurItem));
			InsertItem(&cbiItem);
		}
	}
	
	// save history to info cached earlier
	int nMax = min(GetCount(), m_nMaxHistoryItems + 1);
	
	for (int n = 0; n < nMax; n++)
	{
		CString sKey;
		sKey.Format(TEXT("%s%d"), m_sKeyPrefix, n);
		CString sText;
		GetLBText(n, sText);
		//pApp->WriteProfileString(m_sSection, sKey, sText);		
		MyWriteProfileString(m_sSection, sKey, sText);
	}
	
	if (m_bSaveRestoreLastCurrent)
	{
		CString sText;
		GetWindowText(sText);
		CString sKey;
		
		if (!m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
    
		//pApp->WriteProfileString(m_sSection, sKey, sText);
		MyWriteProfileString(m_sSection, sKey, sText);
	}
	//Profile_Free();
	Profile_Flush(strINI);
	Profile_Free();
}

// removes all the items from the history list, and optionally deletes
// the registry items. Note that if the history list is generated from
// a CRecentFileList, then registry entries will not be deleted
void CHistoryComboEx::ClearHistory(BOOL bDeleteRegistryEntries)
{
	ResetContent();
	
	if (! m_sSection.IsEmpty() && bDeleteRegistryEntries)
	{
		// get the actual reg key used
		CWinApp* pApp = AfxGetApp();
		CRegKey rk;
		CString sKey = "SOFTWARE\\";
    
		if (pApp->m_pszRegistryKey == NULL || pApp->m_pszAppName == NULL)
			return;
    
		sKey += pApp->m_pszRegistryKey + CString("\\");    
		sKey += pApp->m_pszAppName + CString("\\");
		sKey += m_sSection;
		if (rk.Open(HKEY_CURRENT_USER, sKey) != ERROR_SUCCESS)
			return;
    
		// delete actual values
		int nMax = m_nMaxHistoryItems + 1;
    
		for (int n = 0; n < nMax; n++)
		{
			sKey.Format(TEXT("%s%d"), m_sKeyPrefix, n);
			rk.DeleteValue(sKey);
		}
    
		if (!m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
		
		rk.DeleteValue(sKey);
	}
}