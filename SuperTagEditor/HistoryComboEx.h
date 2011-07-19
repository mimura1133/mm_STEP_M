/*********************************************************************************************/
// File:	HistoryComboEx.h
// Version: 1.0
// Created: 22. 10. 2001
//
// This class is based of CHistoryCombo from Paul S. Vicherky and it enables you to load and 
// save the items of an ComboBoxEx.
//
// The modifications are maded by Nabil Hussein
// E-mail: unc@index.com.jo
/*********************************************************************************************/

////////////////////////////////////////////////////////////////////////////
// File:	HistoryCombo.h
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

#if !defined(AFX_HISTORYCOMBOEX_H__B5E3D56E_F078_4152_909F_E9FD867BAD8B__INCLUDED_)
#define AFX_HISTORYCOMBOEX_H__B5E3D56E_F078_4152_909F_E9FD867BAD8B__INCLUDED_

#include "afxadv.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryComboEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboEx window

class CHistoryComboEx : public CComboBoxEx
{
// Construction
public:
	CHistoryComboEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryComboEx)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearHistory(BOOL bDeleteRegistryEntries = TRUE);
	void SetMaxHistoryItems(int nMaxItems) { m_nMaxHistoryItems = nMaxItems; }
	void SaveHistory(BOOL bAddCurrentItemToHistory = TRUE);
	CString LoadHistory(CRecentFileList* pListMRU, BOOL bSelectMostRecent = TRUE);
	CString LoadHistory(LPCTSTR lpszSection, LPCTSTR lpszKeyPrefix, BOOL bSaveRestoreLastCurrent = TRUE, LPCTSTR lpszKeyCurItem = NULL);
	int InsertItem (CString strItem);
	int InsertItem (const COMBOBOXEXITEM* pCBItem);
	virtual ~CHistoryComboEx();

	// Generated message map functions
protected:
	CString m_sSection;
	CString m_sKeyPrefix;
	CString m_sKeyCurItem;
	BOOL m_bSaveRestoreLastCurrent;
	int m_nMaxHistoryItems;

	//{{AFX_MSG(CHistoryComboEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYCOMBOEX_H__B5E3D56E_F078_4152_909F_E9FD867BAD8B__INCLUDED_)
