// STEP_ogg.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_ogg.h"
#include "STEPlugin.h"

#include "Tag_Ogg.h"

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
// CSTEP_oggApp

BEGIN_MESSAGE_MAP(CSTEP_oggApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_oggApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_oggApp の構築

CSTEP_oggApp::CSTEP_oggApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_oggApp オブジェクト

CSTEP_oggApp theApp;

UINT nPluginID;
UINT nFileTypeOGG;
UINT nFileTypeOGA;

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "Version 1.00 Copyright (C) 2003-2005 haseta\r\nOggVorbis形式をサポートしています";
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INIファイルの読み込み
	strINI = szPluginFolder;
	strINI += "STEP_ogg.ini";
	bOptGenreListSelect = GetPrivateProfileInt("OGGVorbis", "GenreListSelect", 0, strINI) ? true : false;

	HBITMAP hOGGBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
	HBITMAP hOGABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
	nFileTypeOGG = STEPRegisterExt(nPluginID, "ogg", hOGGBitmap);
	nFileTypeOGA = STEPRegisterExt(nPluginID, "oga", hOGABitmap);
	DeleteObject(hOGGBitmap);
	DeleteObject(hOGABitmap);

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
	return "STEP_ogg";
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
	case COLUMN_YEAR:
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
	case COLUMN_TRACK_NUMBER:	return 256;
	case COLUMN_YEAR:			return 256;
	case COLUMN_GENRE:			return 256;
	case COLUMN_COMMENT:		return 256;
	}
	return 0;
}


STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ((stricmp(szExt, "ogg") == 0) || (stricmp(szExt, "oga") == 0)) {
		extern	bool LoadAttributeFileOGG(FILE_INFO *pFile);
		if (LoadAttributeFileOGG(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format("%s の読み込みに失敗しました", GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, "OggVorbisファイルの読み込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			if(stricmp(szExt, "ogg") == 0){
				SetFormat(pFileMP3, nFileTypeOGG);
				SetFileTypeName(pFileMP3, "OggVorbis");
				return STEP_SUCCESS;
			}
			if(stricmp(szExt, "oga") == 0){
				SetFormat(pFileMP3, nFileTypeOGA);
				SetFileTypeName(pFileMP3, "OggVorbis");
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

	if ((nFormat == nFileTypeOGG)|(nFormat == nFileTypeOGA)) {
		extern bool WriteAttributeFileOGG(FILE_INFO *pFileMP3);
		if (WriteAttributeFileOGG(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format("%s の書き込みに失敗しました", GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, "OggVorbisファイルの書き込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
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

		WritePrivateProfileString("OGGVorbis", "GenreListSelect", bOptGenreListSelect ? "1" : "0", strINI);
	}
}

STEP_API void WINAPI STEPInitFileSpecificInfo(FILE_INFO* pFileMP3) {
	CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
	if (fileOGG != NULL)	delete fileOGG;
}