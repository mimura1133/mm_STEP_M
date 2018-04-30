// STEP_riff.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_riff.h"
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
// CSTEP_riffApp

BEGIN_MESSAGE_MAP(CSTEP_riffApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_riffApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_riffApp の構築

CSTEP_riffApp::CSTEP_riffApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_riffApp オブジェクト

CSTEP_riffApp theApp;

UINT nPluginID;
UINT nFileTypeWAV;
UINT nFileTypeAVI;

CString strINI;

bool bOptWavGenreListSelect;
bool bOptAviGenreListSelect;
bool bOptWavSupportTrackNumber;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TEXT("Version 1.02 Copyright (C) 2003-2006 haseta\r\nRIFF WAV/AVI形式をサポートしています");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INIファイルの読み込み
	strINI = szPluginFolder;
	strINI += "STEP_riff.ini";
	bOptWavGenreListSelect = GetPrivateProfileInt(TEXT("WAV"), TEXT("GenreListSelect"), 0, strINI) ? true : false;
	bOptAviGenreListSelect = GetPrivateProfileInt(TEXT("AVI"), TEXT("GenreListSelect"), 0, strINI) ? true : false;
	bOptWavSupportTrackNumber = GetPrivateProfileInt(TEXT("WAV"), TEXT("SupportTrackNumber"), 0, strINI) ? true : false;

	HBITMAP hWAVBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WAV));
	nFileTypeWAV = STEPRegisterExt(nPluginID, TEXT("wav"), hWAVBitmap);
	DeleteObject(hWAVBitmap);
	HBITMAP hAVIBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AVI));
	nFileTypeAVI = STEPRegisterExt(nPluginID, TEXT("avi"), hAVIBitmap);
	DeleteObject(hAVIBitmap);

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
	return TEXT("STEP_riff");
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nFormat == nFileTypeWAV) {
		return true;
	}
	return false;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF)	return _NULL;
	if (nFormat == nFileTypeWAV) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:
		case COLUMN_ARTIST_NAME:
		case COLUMN_ALBUM_NAME:
		case COLUMN_YEAR:
		case COLUMN_COMMENT:
		case COLUMN_COPYRIGHT:
		case COLUMN_ENGINEER:
		case COLUMN_SOURCE:
		case COLUMN_SOFTWARE:
			return _EDIT;
		case COLUMN_GENRE:
			if (bOptWavGenreListSelect) {
				return _CBOX;
			} else {
				return _EDIT;
			}
		}
		if (bOptWavSupportTrackNumber && nColumn == COLUMN_TRACK_NUMBER) {
			return _EDIT;
		}
	}
	if (nFormat == nFileTypeAVI) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:
		case COLUMN_COMMENT:
			return _MEDIT;
		case COLUMN_ARTIST_NAME:
		case COLUMN_ALBUM_NAME:
		case COLUMN_YEAR:
		case COLUMN_COPYRIGHT:
		case COLUMN_ENGINEER:
		case COLUMN_SOURCE:
		case COLUMN_SOFTWARE:
			return _EDIT;
		case COLUMN_GENRE:
			if (bOptAviGenreListSelect) {
				return _CBOX;
			} else {
				return _EDIT;
			}
		}
	}
	return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
		case COLUMN_TRACK_NAME:
		case COLUMN_COMMENT:
		case COLUMN_ARTIST_NAME:
		case COLUMN_ALBUM_NAME:
		case COLUMN_GENRE:
		case COLUMN_YEAR:
		case COLUMN_COPYRIGHT:
		case COLUMN_ENGINEER:
		case COLUMN_SOURCE:
		case COLUMN_SOFTWARE:
			return 256;
	}
	if (bOptWavSupportTrackNumber && nColumn == COLUMN_TRACK_NUMBER) {
		return 256;
	}
	return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	extern	bool LoadAttributeFileWAV(FILE_INFO *pFile);
	if (_tcsicmp(szExt, TEXT("wav")) == 0) {
		if (LoadAttributeFileWAV(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s の読み込みに失敗しました"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("WAVファイルの読み込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			SetFormat(pFileMP3, nFileTypeWAV);
			SetFileTypeName(pFileMP3, TEXT("WAV"));
			return STEP_SUCCESS;
		}
	}
	if (_tcsicmp(szExt, TEXT("avi")) == 0) {
		if (LoadAttributeFileWAV(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s の読み込みに失敗しました"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AVIファイルの読み込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			SetFormat(pFileMP3, nFileTypeAVI);
			SetFileTypeName(pFileMP3, TEXT("AVI"));
			return STEP_SUCCESS;
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	extern bool WriteAttributeFileWAV(FILE_INFO *pFileMP3);
	if (nFormat == nFileTypeWAV) {
		if (WriteAttributeFileWAV(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s の書き込みに失敗しました"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("WAVファイルの書き込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeAVI) {
		if (WriteAttributeFileWAV(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s の書き込みに失敗しました"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AVIファイルの書き込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
	dlg1.m_bWavGenreListSelect = bOptWavGenreListSelect;
	dlg1.m_bAviGenreListSelect = bOptAviGenreListSelect;
	dlg1.m_bWavSupportTrackNumber = bOptWavSupportTrackNumber;
	page.AddPage(&dlg1);
	page.SetTitle(CString(STEPGetPluginName()) + " オプション設定");
	if (page.DoModal() == IDOK) {
		bOptWavGenreListSelect = dlg1.m_bWavGenreListSelect ? true : false;
		bOptAviGenreListSelect = dlg1.m_bAviGenreListSelect ? true : false;
		bOptWavSupportTrackNumber = dlg1.m_bWavSupportTrackNumber ? true : false;

		WritePrivateProfileString(TEXT("WAV"), TEXT("GenreListSelect"), bOptWavGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("AVI"), TEXT("GenreListSelect"), bOptAviGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("WAV"), TEXT("SupportTrackNumber"), bOptWavSupportTrackNumber ? TEXT("1") : TEXT("0"), strINI);

		STEPUpdateCellInfo();
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

/*
STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_URL:			// URL
		return "URL(関連)";
	case COLUMN_ENCODEST:		// エンコードした人
	case COLUMN_OTHER:			// その他
		return "URL(Album)";
		break;
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}
*/
