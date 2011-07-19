// STEP_wma.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_wma.h"
#include "STEPlugin.h"

#include "DlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
extern void Free_mp3infp();

//
//	メモ!
//
//		この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされた
//		どの関数も関数の最初に追加される AFX_MANAGE_STATE 
//		マクロを含んでいなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		これは関数内の最初のステートメントでなければな
//		らないことを意味します、コンストラクタが MFC 
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_wmaApp

BEGIN_MESSAGE_MAP(CSTEP_wmaApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_wmaApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_wmaApp の構築

CSTEP_wmaApp::CSTEP_wmaApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_wmaApp オブジェクト

CSTEP_wmaApp theApp;

UINT nPluginID;
UINT nFileTypeWMA;
UINT nFileTypeWMV;
UINT nFileTypeASF;

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "Version 1.01 Copyright (C) 2003-2006 haseta\r\nVersion 1.03M Copyright (C) 2008-2010 Mimura\r\nWMA/WMV/ASF形式をサポートしています"
			"\r\n(Windows Media Format 9 or later Runtime required)"
			;
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INIファイルの読み込み
	strINI = szPluginFolder;
	strINI += "STEP_wma.ini";
	bOptGenreListSelect = GetPrivateProfileInt("WMA", "GenreListSelect", 0, strINI) ? true : false;

	HBITMAP hWMABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	HBITMAP hWMVBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	HBITMAP hASFBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	nFileTypeWMA = STEPRegisterExt(nPluginID, "wma", hWMABitmap);
	nFileTypeWMV = STEPRegisterExt(nPluginID, "wmv", hWMVBitmap);
	nFileTypeASF = STEPRegisterExt(nPluginID, "asf", hASFBitmap);
	DeleteObject(hWMABitmap);
	DeleteObject(hWMVBitmap);
	DeleteObject(hASFBitmap);

	return true;
}

STEP_API void WINAPI STEPFinalize() {
	Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "STEP_wma";
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF)	return _NULL;
	switch (nColumn) {
	case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
	case COLUMN_ALBUM_NAME:
	case COLUMN_TRACK_NUMBER:
	case COLUMN_DISK_NUMBER:
	case COLUMN_YEAR:
	case COLUMN_URL:
	case COLUMN_COPYRIGHT:
	case COLUMN_OTHER:
	case COLUMN_ORIG_ARTIST: /* 2005.08.19 add */
	case COLUMN_COMPOSER: /* 2005.08.19 add */
	case COLUMN_ENGINEER: /* 2005.09.07 add */
	case COLUMN_ALBM_ARTIST: /* Mimura add */
	case COLUMN_WRITER: /* Mimura add */
		return _EDIT;
	case COLUMN_COMMENT:
		return _MEDIT;
	case COLUMN_GENRE:
		if (bOptGenreListSelect) {
			return _CBOX;
		} else {
			return _EDIT;
		}
	}
	return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_TRACK_NAME:		return 256;
	case COLUMN_ARTIST_NAME:	return 256;
	case COLUMN_ALBUM_NAME:		return 256;
	case COLUMN_TRACK_NUMBER:	return 3;
	case COLUMN_DISK_NUMBER:	return 3;
	case COLUMN_YEAR:			return 256;
	case COLUMN_GENRE:			return 256;
	case COLUMN_COMMENT:		return 256;
	case COLUMN_URL:			return 256;
	case COLUMN_COPYRIGHT:		return 256;
	case COLUMN_OTHER:			return 256;
	case COLUMN_ORIG_ARTIST:	return 256; /* 2005.08.19 add */
	case COLUMN_COMPOSER:		return 256; /* 2005.08.19 add */
	case COLUMN_ENGINEER:		return 256; /* 2005.09.07 add */
	case COLUMN_ALBM_ARTIST:	return 256; /* Mimura add */
	case COLUMN_WRITER:			return 256; /* Mimura add */
	}
	return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (stricmp(szExt, "wma") == 0 || stricmp(szExt, "wmv") == 0 || stricmp(szExt, "asf") == 0) {
		extern	bool LoadAttributeFileWMA(FILE_INFO *pFile);
		if (LoadAttributeFileWMA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format("%s の読み込みに失敗しました", GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, "WMAファイルの読み込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			if (stricmp(szExt, "wma") == 0) {
				SetFormat(pFileMP3, nFileTypeWMA);
				SetFileTypeName(pFileMP3, "WMA");
				return STEP_SUCCESS;
			}
			if (stricmp(szExt, "wmv") == 0) {
				SetFormat(pFileMP3, nFileTypeWMV);
				SetFileTypeName(pFileMP3, "WMV");
				return STEP_SUCCESS;
			}
			if (stricmp(szExt, "asf") == 0) {
				SetFormat(pFileMP3, nFileTypeASF);
				SetFileTypeName(pFileMP3, "ASF");
				return STEP_SUCCESS;
			}
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if (nFormat == nFileTypeWMA || nFormat == nFileTypeWMV || nFormat == nFileTypeASF) {
		extern bool WriteAttributeFileWMA(FILE_INFO *pFileMP3);
		if (WriteAttributeFileWMA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format("%s の書き込みに失敗しました", GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, "WMAファイルの書き込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgSetup dlg1;
	CPropertySheet page;
	dlg1.m_bGenreListSelect = bOptGenreListSelect;
	page.AddPage(&dlg1);
	page.SetTitle(CString(STEPGetPluginName()) + " オプション設定");
	if (page.DoModal() == IDOK) {
		bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;

		WritePrivateProfileString("WMA", "GenreListSelect", bOptGenreListSelect ? "1" : "0", strINI);
	}
}



/*
STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}
*/

/*
STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}
*/

/*
STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return false;
}
*/

/*
STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return false;
}
*/

/*
STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
*/


STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_URL:			// URL
		return "URL(関連)";
	case COLUMN_OTHER:			// その他
		return "URL(Album)";
		break;
	case COLUMN_ENGINEER:		// エンジニア(制作者) /* 2005.09.07 add */
		return "レーベル";
		break;
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

