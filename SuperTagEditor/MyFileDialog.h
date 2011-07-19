#if !defined(AFX_MYFILEDIALOG_H__67E743D3_C819_4A59_ABA0_B0D25E9EB131__INCLUDED_)
#define AFX_MYFILEDIALOG_H__67E743D3_C819_4A59_ABA0_B0D25E9EB131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog ダイアログ

class CMyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialog)

public:
	CMyFileDialog(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	BOOL	m_bIsWriteAppend;
	IFileDialogCustomize* custom;

protected:
	//{{AFX_MSG(CMyFileDialog)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	virtual BOOL OnFileNameOK( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYFILEDIALOG_H__67E743D3_C819_4A59_ABA0_B0D25E9EB131__INCLUDED_)
