#if !defined(AFX_MYFILEDIALOGPLAYLIST_H__ED73F544_5953_4A29_9311_7775A9CCB828__INCLUDED_)
#define AFX_MYFILEDIALOGPLAYLIST_H__ED73F544_5953_4A29_9311_7775A9CCB828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDialogPlaylist.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialogPlaylist ダイアログ

class CMyFileDialogPlaylist : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialogPlaylist)

public:
	BOOL OnFileNameOK();
	void OnInitDone();
	IFileDialogCustomize* custom;
	BOOL m_bShowLoadPlaylistDlg;
	CMyFileDialogPlaylist(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CMyFileDialogPlaylist)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYFILEDIALOGPLAYLIST_H__ED73F544_5953_4A29_9311_7775A9CCB828__INCLUDED_)
