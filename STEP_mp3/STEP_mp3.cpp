// STEP_mp3.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_mp3.h"
#include "ID3v2/Id3tagv2.h"
#include "ID3v2/RMP.h"
#include "STEPlugin.h"

#include "DlgFileRmpID3v2.h"
#include "DlgDefaultValue.h"
#include "DlgConvID3v2Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
extern void Free_mp3infp();

#pragma pack(push, 1)
struct	ID3TAG	{						// <<< ID3 Tag のデータ形式 >>>
	BYTE	byData[128];				// 128バイトデータ
};

#define ID3_LEN_TRACK_NAME		30		// トラック名    (文字列…30BYTE)
#define ID3_LEN_ARTIST_NAME		30		// アーティスト名(文字列…30BYTE)
#define ID3_LEN_ALBUM_NAME		30		// アルバム名    (文字列…30BYTE)
#define ID3_LEN_COMMENT			30		// コメント      (文字列…30BYTE)
#define ID3_LEN_YEAR			4		// リリース年号  (文字列… 4BYTE)

struct	ID3TAG_V10	{							// <<< ID3 Tag v1.0 のデータ形式 >>>
	TCHAR	sTAG[3];							// "TAG"         (文字列… 3BYTE)
	TCHAR	sTrackName[ID3_LEN_TRACK_NAME];		// トラック名    (文字列…30BYTE)
	TCHAR	sArtistName[ID3_LEN_ARTIST_NAME];	// アーティスト名(文字列…30BYTE)
	TCHAR	sAlbumName[ID3_LEN_ALBUM_NAME];		// アルバム名    (文字列…30BYTE)
	TCHAR	sYear[4];							// リリース年号  (文字列… 4BYTE)
	TCHAR	sComment[ID3_LEN_COMMENT];			// コメント      (文字列…30BYTE)
	BYTE	byGenre;							// ジャンル番号  (数字…… 1BYTE)
};

struct	ID3TAG_V11	{							// <<< ID3 Tag v1.1 のデータ形式 >>>
	TCHAR	sTAG[3];							// "TAG"         (文字列… 3BYTE)
	TCHAR	sTrackName[ID3_LEN_TRACK_NAME];		// トラック名    (文字列…30BYTE)
	TCHAR	sArtistName[ID3_LEN_ARTIST_NAME];	// アーティスト名(文字列…30BYTE)
	TCHAR	sAlbumName[ID3_LEN_ALBUM_NAME];		// アルバム名    (文字列…30BYTE)
	TCHAR	sYear[4];							// リリース年号  (文字列… 4BYTE)
	TCHAR	sComment[ID3_LEN_COMMENT-2];		// コメント      (文字列…30BYTE)
	TCHAR	cZero;								// '\0'          (文字列… 1BYTE)
	BYTE	byTrackNumber;						// トラック番号  (数字…… 1BYTE)
	BYTE	byGenre;							// ジャンル番号  (数字…… 1BYTE)
};
#pragma pack(pop)

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
// CSTEP_mp3App

BEGIN_MESSAGE_MAP(CSTEP_mp3App, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_mp3App)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App の構築

CSTEP_mp3App::CSTEP_mp3App()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_mp3App オブジェクト

CSTEP_mp3App theApp;

UINT nPluginID;
UINT nFileTypeMP3;
UINT nFileTypeMP3V1;
UINT nFileTypeMP3V11;
UINT nFileTypeID3V2;
UINT nFileTypeRMP;

enum	{SIF_CONV_ALL_FIELD, SIF_CONV_LENGTH_OK};

CString strINI;
// オプション設定
bool	bOptAutoConvID3v2;
bool	bOptAutoConvRMP;
int		nOptSIFieldConvType;
bool	bOptID3v2ID3tagAutoWrite;
bool	bOptRmpID3tagAutoWrite;
bool	bOptID3v2GenreAddNumber;
bool	bOptChangeFileExt;
bool	bOptID3v2GenreListSelect;
bool	bOptRmpGenreListSelect;
bool	bOptID3v2ID3tagAutoDelete;
CString	strOptSoftwareTag;
bool	bOptNotUnSyncAlways;
bool	bOptUnSyncNew;
int		nId3v2Encode;
int		nId3v2Version;
int		nId3v2EncodeNew;
int		nId3v2VersionNew;
int		nId3v2EncodeConv;
int		nId3v2VersionConv;

// コマンドID
UINT nIDFileConvAutoID3;
UINT nIDFileConvMP3;
UINT nIDFileConvID3v2;
UINT nIDFileConvRMP;
//UINT nIDConvSIFieldToID3Tag; /* 1.02 本体標準機能を使う */
UINT nIDDeleteID3;
UINT nIDConvID3v2Version;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "Version 1.02 Copyright (C) 2003-2006 haseta\r\nVersion 1.04M Copyright (C) 2008-2010 Mimura\r\nMP3(ID3v1/ID3v2)/RIFF形式をサポートしています";
}

void AddConvMenu(HMENU hMenu) {
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvMP3, "標準MP3形式に変換");
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvRMP, "RIFF MP3形式に変換");
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvID3v2, "ID3v2形式に変換");
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvAutoID3, "標準MP3形式/ID3v2形式に自動変換");
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDConvID3v2Version, "ID3v2バージョン/文字エンコードの変換");
//	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
//	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDConvSIFieldToID3Tag, "SI/ID3v2フィールドからID3tagに変換");
}

void AddEditMenu(HMENU hMenu) {
	char szMenu[100];
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_TYPE;
	bool bAddDeleteID3 = false;
	for (int iPos=0;iPos<GetMenuItemCount(hMenu);iPos++) {
		mii.dwTypeData = (LPTSTR)szMenu;
		mii.cch = sizeof(szMenu)-1;
		if (GetMenuItemInfo(hMenu, iPos, true, &mii)) {
			if (mii.fType == MFT_STRING) {
				if (strcmp(szMenu, "編集前の状態に戻す") == 0) {
					InsertMenu(hMenu, iPos, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, "ID3tagを削除");
					bAddDeleteID3 = true;
					iPos++;
				}
			}
		}
	}

	if (!bAddDeleteID3) {
		InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, "ID3tagを削除");
	}
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INIファイルの読み込み
	strINI = szPluginFolder;
	strINI += "STEP_mp3.ini";
	bOptAutoConvID3v2 = GetPrivateProfileInt("RMP_ID3V2", "AutoConvID3v2", 1, strINI) ? true : false;
	bOptAutoConvRMP = GetPrivateProfileInt("RMP_ID3V2", "AutoConvRMP", 0, strINI) ? true : false;
	nOptSIFieldConvType = GetPrivateProfileInt("RMP_ID3V2", "SIFieldConvType", SIF_CONV_ALL_FIELD, strINI);
	bOptID3v2ID3tagAutoWrite = GetPrivateProfileInt("RMP_ID3V2", "ID3v2ID3tagAutoWrite", 1, strINI) ? true : false;
	bOptRmpID3tagAutoWrite = GetPrivateProfileInt("RMP_ID3V2", "RmpID3tagAutoWrite", 1, strINI) ? true : false;
	bOptID3v2GenreListSelect = GetPrivateProfileInt("RMP_ID3V2", "ID3v2GenreListSelect", 1, strINI) ? true : false;
	bOptRmpGenreListSelect = GetPrivateProfileInt("RMP_ID3V2", "RmpGenreListSelect", 1, strINI) ? true : false;
	bOptID3v2ID3tagAutoDelete = GetPrivateProfileInt("RMP_ID3V2", "ID3v2ID3tagAutoDelete", 0, strINI) ? true : false;
	bOptID3v2GenreAddNumber = false/* 2005.08.23 GetPrivateProfileInt("RMP_ID3V2", "ID3v2GenreAddNumber", 0, strINI) ? true : false*/;
	bOptChangeFileExt = GetPrivateProfileInt("RMP_ID3V2", "ChangeFileExt", 0, strINI) ? true : false;

	GetPrivateProfileString("OTHER", "SoftwareTag", "", strOptSoftwareTag.GetBufferSetLength(255+1), 255, strINI);
	strOptSoftwareTag.ReleaseBuffer();

	bOptNotUnSyncAlways = GetPrivateProfileInt("OTHER", "ID3v2NotUnSync", 0, strINI) ? true : false;
	bOptUnSyncNew = GetPrivateProfileInt("OTHER", "ID3v2UnSyncNew", 0, strINI) ? true : false;

	nId3v2Encode = GetPrivateProfileInt("OTHER", "ID3v2CharEncode", 0, strINI);
	nId3v2Version = GetPrivateProfileInt("OTHER", "ID3v2Version", 0, strINI);
	nId3v2EncodeNew = GetPrivateProfileInt("OTHER", "ID3v2CharEncodeNew", 0, strINI);
	nId3v2VersionNew = GetPrivateProfileInt("OTHER", "ID3v2VersionNew", 0, strINI);

	// サポートしている拡張子を登録
	HBITMAP hMP3Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hMP3V1Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hMP3V11Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hID3v2Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_ID3V2));
	HBITMAP hRMPBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_RMP));
	nFileTypeMP3 = STEPRegisterExt(nPluginID, "mp3", hMP3Bitmap);
	nFileTypeMP3V1 = STEPRegisterExt(nPluginID, "mp3", hMP3V1Bitmap);
	nFileTypeMP3V11 = STEPRegisterExt(nPluginID, "mp3", hMP3V11Bitmap);
	nFileTypeID3V2 = STEPRegisterExt(nPluginID, "mp3", hID3v2Bitmap);
	nFileTypeRMP = STEPRegisterExt(nPluginID, "rmp", hRMPBitmap);
	DeleteObject(hMP3Bitmap);
	DeleteObject(hMP3V1Bitmap);
	DeleteObject(hMP3V11Bitmap);
	DeleteObject(hID3v2Bitmap);
	DeleteObject(hRMPBitmap);

	// ツールバーへの登録
	COLORMAP map; // 背景色の変換
	map.from = RGB(192,192,192);
	map.to = GetSysColor(COLOR_3DFACE);

	nIDFileConvMP3 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_MP3, 0, &map, 1), nIDFileConvMP3, "STEP_mp3_FILE_CONV_MP3");
	STEPKeyAssign(nIDFileConvMP3, "標準MP3形式に変換", "STEP_mp3_KEY_FILE_CONV_MP3");

	nIDFileConvRMP = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_RMP, 0, &map, 1), nIDFileConvRMP, "STEP_mp3_FILE_CONV_RMP");
	STEPKeyAssign(nIDFileConvRMP, "RIFF MP3形式に変換", "STEP_mp3_KEY_FILE_CONV_RMP");

	nIDFileConvID3v2 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_ID3V2, 0, &map, 1), nIDFileConvID3v2, "STEP_mp3_FILE_CONV_ID3V2");
	STEPKeyAssign(nIDFileConvID3v2, "ID3v2 形式に変換", "STEP_mp3_KEY_FILE_CONV_ID3V2");

	nIDFileConvAutoID3 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_AUTO_ID3, 0, &map, 1), nIDFileConvAutoID3, "STEP_mp3_FILE_CONV_AUTO_ID3");
	STEPKeyAssign(nIDFileConvAutoID3, "標準MP3形式/ID3v2 形式に自動変換", "STEP_mp3_KEY_FILE_CONV_AUTO_ID3");

//	nIDConvSIFieldToID3Tag = STEPGetCommandID();
//	STEPKeyAssign(nIDConvSIFieldToID3Tag, "SI/ID3v2フィールドからID3tagに変換", "STEP_mp3_KEY_CONV_SI_FIELD_TO_ID3TAG");

	nIDDeleteID3 = STEPGetCommandID();
	STEPKeyAssign(nIDDeleteID3, "ID3tag を削除", "STEP_mp3_KEY_DELETE_ID3");

	nIDConvID3v2Version = STEPGetCommandID();
	STEPKeyAssign(nIDConvID3v2Version, "ID3v2バージョン/文字エンコードの変換", "STEP_mp3_KEY_CONV_ID3V2_VERSION");

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
	return "STEP_mp3";
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) {
		return false;
	}
	if (nFormat == nFileTypeID3V2) {
		return true;
	}
	if (nFormat == nFileTypeRMP) {
		return true;
	}
	return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nFormat == nFileTypeID3V2) {
		return true;
	}
	if ((nFormat == nFileTypeMP3  || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && bOptAutoConvID3v2) {
		return true;
	}
	return false;
}

STEP_API bool WINAPI STEPSupportGenreSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nFormat == nFileTypeID3V2) {
		return true;
	}
	if (nFormat == nFileTypeRMP) {
		return true;
	}
	if ((nFormat == nFileTypeMP3  || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && (bOptAutoConvID3v2 || bOptAutoConvRMP)) {
		return true;
	}
	return false;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
	case COLUMN_ALBUM_NAME:
	case COLUMN_TRACK_NUMBER:
	case COLUMN_DISK_NUMBER:
	case COLUMN_YEAR:
		if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
			return _NULL;
		}
		if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
			return _NULL;
		}
		/* 2005.09.20 add */
		if (!isEditSIF && bOptID3v2ID3tagAutoDelete && !TYPE_IS_RMP(nFormat)) {
			if ((!bOptAutoConvRMP && TYPE_IS_MP3V1(nFormat)) || TYPE_IS_MP3V2(nFormat)) {
				return _NULL;
			}
		}
		return _EDIT;
	case COLUMN_GENRE:
		if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
			return _NULL;
		}
		if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
			return _NULL;
		}
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if (TYPE_IS_MP3V2(nFormat) && isEditSIF) {
			if (bOptID3v2GenreListSelect) {
				return _CBOX;
			} else {
				return _EDIT;
			}
		}
		if (TYPE_IS_RMP(nFormat) && isEditSIF) {
			if (bOptRmpGenreListSelect) {
				return _CBOX;
			} else {
				return _EDIT;
			}
		}
		return _CBOX;
	case COLUMN_SOFTWARE:
	case COLUMN_COPYRIGHT:
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if (isEditSIF) {
			return _EDIT;
		} else {
			return _NULL;
		}
	case COLUMN_COMMENT:
		if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
			return _NULL;
		}
		if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
			return _NULL;
		}
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if ((isEditSIF && TYPE_IS_MP3V1(nFormat) && bOptAutoConvID3v2) || (isEditSIF && TYPE_IS_MP3V2(nFormat))) {
			return _MEDIT;
		} else {
			return _EDIT;
		}
	case COLUMN_WRITER:
	case COLUMN_COMPOSER:
	case COLUMN_ALBM_ARTIST:
	case COLUMN_ORIG_ARTIST:
	case COLUMN_URL:
	case COLUMN_ENCODEST:
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if (!isEditSIF) {
			return _NULL;
		}
		if ((TYPE_IS_MP3V1(nFormat) && bOptAutoConvID3v2) || nFormat == nFileTypeID3V2) {
			return _EDIT;
		} else {
			return _NULL;
		}
	case COLUMN_SOURCE:
	case COLUMN_KEYWORD:
	case COLUMN_TECHNICIAN:
	case COLUMN_LYRIC:
	case COLUMN_COMMISSION:
		if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
			return _NULL;
		}
		if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
			return _NULL;
		}
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if (!isEditSIF) {
			return _NULL;
		}
		if ((TYPE_IS_MP3V1(nFormat) && bOptAutoConvRMP) || nFormat == nFileTypeRMP) {
			return _EDIT;
		} else {
			return _NULL;
		}
	case COLUMN_ENGINEER:
		if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
			return _NULL;
		}
		if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
			return _NULL;
		}
		//if (!isEditSIF && (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite)) {
		//	return _NULL;
		//}
		if (!isEditSIF) {
			return _NULL;
		}
		if (((TYPE_IS_MP3V1(nFormat) && bOptAutoConvRMP) || nFormat == nFileTypeRMP) ||
			 ((TYPE_IS_MP3V1(nFormat) && bOptAutoConvID3v2) || nFormat == nFileTypeID3V2)){
			return _EDIT;
		} else {
			return _NULL;
		}
	}
	return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		return ID3_LEN_TRACK_NAME;
		case COLUMN_ARTIST_NAME:	return ID3_LEN_ARTIST_NAME;
		case COLUMN_ALBUM_NAME:		return ID3_LEN_ALBUM_NAME;
		case COLUMN_TRACK_NUMBER:	return 3;
		case COLUMN_DISK_NUMBER:	return 3;
		case COLUMN_YEAR:			return ID3_LEN_YEAR;
		case COLUMN_GENRE:			return 128; // ID3v1の場合、ジャンル名称と入力できる最大長は違うので
		case COLUMN_COMMENT:		return ID3_LEN_COMMENT;
		default:					return 0;
		}
	}
	if (((nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && bOptAutoConvID3v2) || nFormat == nFileTypeID3V2) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		// トラック名
		case COLUMN_ARTIST_NAME:	// アーティスト名
		case COLUMN_ALBUM_NAME:		// アルバム名
		case COLUMN_TRACK_NUMBER:	// トラック番号
		case COLUMN_DISK_NUMBER:	// ディスク番号
		case COLUMN_YEAR:			// リリース年号
		case COLUMN_GENRE:			// ジャンル番号
		case COLUMN_COPYRIGHT:		// 著作権
		case COLUMN_SOFTWARE:		// ソフトウェア
		case COLUMN_WRITER:			// 作詞
		case COLUMN_COMPOSER:		// 作曲
		case COLUMN_ALBM_ARTIST:	// Albm.アーティスト
		case COLUMN_ORIG_ARTIST:	// Orig.アーティスト
		case COLUMN_URL:			// URL
		case COLUMN_ENCODEST:		// エンコードした人
		case COLUMN_ENGINEER:		// エンジニア（出版）
			//return 256;
		case COLUMN_COMMENT:		// コメント
			return 2048; /* 2003.06.20 増やした */
		}
	} else {
		switch (nColumn) {
		case COLUMN_TRACK_NUMBER:	// トラック番号
			return 3;
		case COLUMN_DISK_NUMBER:	// ディスク番号
			return 3;
		case COLUMN_TRACK_NAME:		// トラック名
		case COLUMN_ARTIST_NAME:	// アーティスト名
		case COLUMN_ALBUM_NAME:		// アルバム名
		case COLUMN_YEAR:			// リリース年号
		case COLUMN_GENRE:			// ジャンル番号
		case COLUMN_COMMENT:		// コメント
		case COLUMN_COPYRIGHT:		// 著作権
		case COLUMN_ENGINEER:		// エンジニア(製作者)
		case COLUMN_SOURCE:			// ソース
		case COLUMN_SOFTWARE:		// ソフトウェア
		case COLUMN_KEYWORD:		// キーワード
		case COLUMN_TECHNICIAN:		// 技術者
		case COLUMN_LYRIC:			// 歌詞
			return 2048;
		}
	}
	return 0;
}

// ID3 Tag v1.0/v1.1 かどうかを調べる
static	bool	IsID3Tag(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return((((const ID3TAG_V10 *)(data))->sTAG[0] == 'T' &&
			((const ID3TAG_V10 *)(data))->sTAG[1] == 'A' &&
			((const ID3TAG_V10 *)(data))->sTAG[2] == 'G') ? true : false);
}
// ID3 Tag v1.0 かどうかを調べる
static	bool	IsID3Tag10(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero != '\0') || ((const ID3TAG_V11 *)data)->byTrackNumber == '\0')) ? true : false);
}
// ID3 Tag v1.1 かどうかを調べる
static	bool	IsID3Tag11(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero == '\0') && ((const ID3TAG_V11 *)data)->byTrackNumber != '\0'))? true : false);
}

/* 漢字の１バイト目かどうかの検査 */
#ifndef iskanji
#define iskanji(c)		((unsigned char)(c) >= 0x81 && (unsigned char)(c) <= 0x9f || (unsigned char)(c) >= 0xe0 && (unsigned char)(c) <= 0xfc)
#endif
void	StringCopyN(char *, const char *, int, BOOL = TRUE);
void	StringCopyN2(char *, const char *, int, BOOL = TRUE);
void StringCopyN(char *sDest, const char *sSrc, int nLen, BOOL bTerm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strlen(sSrc) < (unsigned int)nLen) {
		if (bTerm) strcpy(sDest, sSrc);
		else       memcpy(sDest, sSrc, strlen(sSrc));
		return;
	}
	while(nLen > 0) {
		if (iskanji(*sSrc)) {
			if (nLen >= 2) {
				*sDest++ = *sSrc++;
				*sDest++ = *sSrc++;
			} else {
				if (bTerm) *sDest = '\0';
			}
			nLen -= 2;
		} else {
			*sDest++ = *sSrc++;
			nLen--;
		}
	}
}
void StringCopyN2(char *sDest, const char *sSrc, int nLen, BOOL bTerm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bCR = false;
	while(nLen > 0) {
		if (*sSrc == '\r') bCR = true;
		if (iskanji(*sSrc)) {
			if (nLen >= 2) {
				if (!bCR) {
					*sDest++ = *sSrc++;
					*sDest++ = *sSrc++;
				} else {
					if (bTerm) *sDest++ = '\0';
					if (bTerm) *sDest++ = '\0';
				}
			} else {
				if (bTerm) *sDest = '\0';
			}
			nLen -= 2;
		} else {
			if (!bCR) {
				*sDest++ = *sSrc++;
			} else {
				if (bTerm) *sDest++ = '\0';
			}
			nLen--;
		}
	}
}

void DeleteLineEndSpace(TCHAR *sBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int		nPos = strlen(sBuffer) - 1;
	while(nPos >= 0 && sBuffer[nPos] == ' ') {
		sBuffer[nPos] = '\0';
		nPos--;
	}
}

void setFileType(CId3tagv2& id3v2, FILE_INFO *pFileMP3)
{
	CString strFileTypeName;
	switch(id3v2.GetVer()){
	case 0x0200:
		strFileTypeName = "MP3(ID3v2.2";
		break;
	case 0x0300:
		strFileTypeName = "MP3(ID3v2.3";
		break;
	case 0x0400:
		strFileTypeName = "MP3(ID3v2.4";
		break;
	default:
		strFileTypeName = "MP3(ID3v2.?";
		break;
	}
	if (GetFormat(pFileMP3) == nFileTypeMP3V1) {
		strFileTypeName = strFileTypeName + "+v1.0";
	}
	if (GetFormat(pFileMP3) == nFileTypeMP3V11) {
		strFileTypeName = strFileTypeName + "+v1.1";
	}
	strFileTypeName = strFileTypeName + ")";
	if (id3v2.GetCharEncode() == CId3tagv2::ID3V2CHARENCODE_ISO_8859_1) {
	} else if (id3v2.GetCharEncode() == CId3tagv2::ID3V2CHARENCODE_UTF_16) {
		strFileTypeName = strFileTypeName + ",UTF16";
	} else if (id3v2.GetCharEncode() == CId3tagv2::ID3V2CHARENCODE_UTF_16BE) {
		strFileTypeName = strFileTypeName + ",UTF16BE";
	} else if (id3v2.GetCharEncode() == CId3tagv2::ID3V2CHARENCODE_UTF_8) {
		strFileTypeName = strFileTypeName + ",UTF8";
	}
	//if (id3v2.GetUniocdeEncode()) {
	//	strFileTypeName = strFileTypeName + ",Uni";
	//}
	if (id3v2.GetUnSynchronization()) {
		strFileTypeName = strFileTypeName + ",US";
	}
	SetFileTypeName(pFileMP3, strFileTypeName);
	SetFormat(pFileMP3, nFileTypeID3V2);		// ファイル形式：MP3(ID3v2)
}

bool ReadTagID3(LPCSTR sFileName, FILE_INFO *pFileMP3, char *sHead)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRY {
		ID3TAG	tag;
		CFile	file(sFileName, CFile::modeRead | CFile::shareDenyNone);

		// 先頭の４バイトを取得
		if (sHead != NULL && file.Read(sHead, 4) != 4) {
			// 読み込み失敗
			return(false);
		}

		if (GetFileSize(pFileMP3) > sizeof(ID3TAG)) {
			// === ID3 tag v1.0/1.1 の読み込み ===
			// ファイルの最後の128バイトを取得
			file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			if (file.Read(&tag.byData[0], sizeof(ID3TAG)) != sizeof(ID3TAG)) {
				// 読み込み失敗
				return(false);
			}

			if (IsID3Tag(&tag) == true) {
				// ID3 tag が存在する
				ID3TAG_V11	*pTag = (ID3TAG_V11 *)&tag;
				TCHAR	sBuffer[30+1];
				// トラック名
				StringCopyN(sBuffer, pTag->sTrackName, ID3_LEN_TRACK_NAME);
				sBuffer[ID3_LEN_TRACK_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetTrackName(pFileMP3, sBuffer);
				// アーティスト名
				StringCopyN(sBuffer, pTag->sArtistName, ID3_LEN_ARTIST_NAME);
				sBuffer[ID3_LEN_ARTIST_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetArtistName(pFileMP3, sBuffer);
				// アルバム名
				StringCopyN(sBuffer, pTag->sAlbumName, ID3_LEN_ALBUM_NAME);
				sBuffer[ID3_LEN_ALBUM_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetAlbumName(pFileMP3, sBuffer);
				// リリース年号
				StringCopyN(sBuffer, pTag->sYear, ID3_LEN_YEAR);
				sBuffer[ID3_LEN_YEAR] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetYear(pFileMP3, sBuffer);
				// コメント
				if (IsID3Tag11(&tag) == true) {
					// ID3 tag Ver 1.1
					StringCopyN(sBuffer, pTag->sComment, ID3_LEN_COMMENT-2);
					sBuffer[28] = '\0';
					DeleteLineEndSpace(sBuffer);
					SetComment(pFileMP3, sBuffer);
					// トラック番号
					SetBTrackNumber(pFileMP3, pTag->byTrackNumber);
					if (GetBTrackNumber(pFileMP3) == 0x00) {
						SetBTrackNumber(pFileMP3, 0xff);
					}
					SetFormat(pFileMP3, nFileTypeMP3V11);	// ファイル形式：MP3V1.1
					SetFileTypeName(pFileMP3, "MP3(ID3v1.1)");
				} else {
					// ID3 tag Ver 1.0
					StringCopyN(sBuffer, pTag->sComment, ID3_LEN_COMMENT);
					sBuffer[30] = '\0';
					DeleteLineEndSpace(sBuffer);
					SetComment(pFileMP3, sBuffer);
					// トラック番号
					SetBTrackNumber(pFileMP3, 0xff);
					SetFormat(pFileMP3, nFileTypeMP3V1);	// ファイル形式：MP3V1.0
					SetFileTypeName(pFileMP3, "MP3(ID3v1.0)");
				}
				// ジャンル番号
				SetBGenre(pFileMP3, pTag->byGenre);
				SetGenre(pFileMP3, STEPGetGenreNameSIF(pTag->byGenre));
			} else {
				SetFileTypeName(pFileMP3, "MP3");
				SetFormat(pFileMP3, nFileTypeMP3);	// ファイル形式：MP3
			}

			// ファイルを閉じる
			file.Close();
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format("%s がオープンできませんでした", sFileName);
		MessageBox(NULL, strMsg, "ファイルのオープン失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return true ;
}

bool ReadTagID3v2(LPCSTR sFileName, FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ID3v2 タグの取得
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v2.Load(sFileName) != ERROR_SUCCESS// 読み込み失敗
	||  id3v2.IsEnable() == FALSE			// ID3v2 ではない
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// 未対応のバージョン
		// 読み込み失敗
		if (id3v2.GetVer() > 0x0000 && !TRUE/*id3v2.IsSafeVer()*/) {
			return(true);
		}
		return(false);
	}

	// *** タグ情報の設定 ***
	SetTrackNameSI(pFileMP3, id3v2.GetTitle());		// タイトル
	SetArtistNameSI(pFileMP3, id3v2.GetArtist());		// アーティスト名
	SetAlbumNameSI(pFileMP3, id3v2.GetAlbum());		// アルバム名
	SetYearSI(pFileMP3, id3v2.GetYear());				// リリース
	SetCommentSI(pFileMP3, id3v2.GetComment());		// コメント
	SetGenreSI(pFileMP3, id3v2.GetGenre());			// ジャンル名
	SetTrackNumberSI(pFileMP3, id3v2.GetTrackNo());		// トラック番号
	SetDiskNumberSI(pFileMP3, id3v2.GetDiskNo());		// ディスク番号
	SetSoftwareSI(pFileMP3, id3v2.GetEncoder());		// エンコーダ
	SetCopyrightSI(pFileMP3, id3v2.GetCopyright());	// 著作権
	SetWriterSI(pFileMP3,id3v2.GetWriter());			// 作詞
	SetComposerSI(pFileMP3, id3v2.GetComposer());		// 作曲
	SetAlbumArtistSI(pFileMP3, id3v2.GetAlbmArtist());	// Albm. アーティスト
	SetOrigArtistSI(pFileMP3, id3v2.GetOrigArtist());	// Orig.アーティスト
	SetURLSI(pFileMP3, id3v2.GetUrl());				// URL
	SetEncodest(pFileMP3, id3v2.GetEncodest());			// エンコードした人
	SetEngineerSI(pFileMP3,id3v2.GetEngineer());		// エンジニア（出版）

//	SetFileTypeName(pFileMP3, "MP3(ID3v2)");
	setFileType(id3v2, pFileMP3);
	return(true);
}

bool ReadTagSIF(LPCSTR sFileName, FILE_INFO *pFileMP3)
{
	CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// 読み込み失敗
		||  rmp.IsEnable() == FALSE) {
		// 読み込み失敗
		return(false);
	}

	// 読み込み成功
	// SI フィールド
	SetTrackNameSI(pFileMP3,	rmp.GetNAM());	// 曲名
	SetArtistNameSI(pFileMP3,	rmp.GetART());	// アーティスト名
	SetAlbumNameSI(pFileMP3,	rmp.GetPRD());	// 製品名
	SetCommentSI(pFileMP3,		rmp.GetCMT());	// コメント文字列
	SetYearSI(pFileMP3,			rmp.GetCRD());	// リリース年号
	SetGenreSI(pFileMP3,		rmp.GetGNR());	// ジャンル名
	SetCopyrightSI(pFileMP3,	rmp.GetCOP());	// 著作権
	SetEngineerSI(pFileMP3,		rmp.GetENG());	// エンジニア
	SetSourceSI(pFileMP3,		rmp.GetSRC());	// ソース
	SetSoftwareSI(pFileMP3,		rmp.GetSFT());	// ソフトウェア
	SetKeywordSI(pFileMP3,		rmp.GetKEY());	// キーワード
	SetTechnicianSI(pFileMP3,	rmp.GetTCH());	// 技術者
	SetLyricSI(pFileMP3,		rmp.GetLYC());	// 歌詞
	SetCommissionSI(pFileMP3,	rmp.GetCMS());	// コミッション

	SetFormat(pFileMP3, nFileTypeRMP);		// ファイル形式：RIFF MP3
	SetFileTypeName(pFileMP3, "RIFF MP3");

	if (bOptRmpID3tagAutoWrite)
		SetGenreSI(pFileMP3, GetGenre(pFileMP3));

	return(true);
}

bool LoadFileMP3(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// === ID3v1.0/1.1 形式ファイル ===
	char	sHead[5] = {' ', ' ', ' ', ' ', '\0'};
	if (ReadTagID3(GetFullPath(pFileMP3), pFileMP3, &sHead[0]) == false) {
		// 読み込み失敗
		return false;
	}

	// === ID3v2 形式ファイル ===
	if (strncmp(sHead, "ID3", 3) == 0) {
		if (bOptAutoConvID3v2/*bOptID3v2ID3tagAutoWrite*/) {
			SetGenreSI(pFileMP3, GetGenre(pFileMP3));
			SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
			SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
		}
		if (ReadTagID3v2(GetFullPath(pFileMP3), pFileMP3) == false) {
			MessageBox(NULL, "ID3v2 タグの読み込みに失敗しました", GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return false;
		}
	}
	else
	// === RIFF MP3 形式ファイル(SIフィールドの読み込み) ===
	if (strcmp(sHead, "RIFF") == 0) {
		if (ReadTagSIF(GetFullPath(pFileMP3), pFileMP3) == false) {
			MessageBox(NULL, "RIFF MP3 タグの読み込みに失敗しました", GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
	}
	else
	// === ID3v1.0/1.1 のみ ===
	{
		if (bOptAutoConvID3v2 || bOptAutoConvRMP/*bOptID3v2ID3tagAutoWrite*/) {
			SetGenreSI(pFileMP3, GetGenre(pFileMP3));
			SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
			SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
		}
	}

	// mp3infpにより取得
	GetValues_mp3infp(pFileMP3);

	return true;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (stricmp(szExt, "mp3") == 0 || stricmp(szExt, "rmp") == 0) {
		if (LoadFileMP3(pFileMP3) == false) {
			return STEP_ERROR;
		}
		if (GetFormat(pFileMP3) != FILE_FORMAT_UNKNOWN) {
			return STEP_SUCCESS;
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

bool WriteTagID3(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRY {
		ID3TAG	tag;
		CFile	file(GetFullPath(pFileMP3), CFile::modeReadWrite | CFile::shareDenyWrite);
		if (file.GetLength() > sizeof(ID3TAG)) {
			// ファイルの最後の128バイトを取得
			file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			if (file.Read(&tag.byData[0], sizeof(ID3TAG)) != sizeof(ID3TAG)) {
				// 読み込み失敗
				return false;
			}
			if (IsID3Tag(&tag) == false) {
				// ファイルの最後にシーク
				file.Seek(0, CFile::end);
			} else {
				file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			}

			// スペースで埋める
			memset(&tag, 0x00/*0x20*/, sizeof(ID3TAG));

			ID3TAG_V11	*pTag = (ID3TAG_V11 *)&tag;
			pTag->sTAG[0] = 'T';
			pTag->sTAG[1] = 'A';
			pTag->sTAG[2] = 'G';

			int		nLen;
			// トラック名
			nLen = strlen(GetTrackName(pFileMP3));
			if (nLen > ID3_LEN_TRACK_NAME) nLen = ID3_LEN_TRACK_NAME;
			StringCopyN(&pTag->sTrackName[0], GetTrackName(pFileMP3), nLen, FALSE);

			// アーティスト名
			nLen = strlen(GetArtistName(pFileMP3));
			if (nLen > ID3_LEN_ARTIST_NAME) nLen = ID3_LEN_ARTIST_NAME;
			StringCopyN(&pTag->sArtistName[0], GetArtistName(pFileMP3), nLen, FALSE);

			// アルバム名
			nLen = strlen(GetAlbumName(pFileMP3));
			if (nLen > ID3_LEN_ALBUM_NAME) nLen = ID3_LEN_ALBUM_NAME;
			StringCopyN(&pTag->sAlbumName[0], GetAlbumName(pFileMP3), nLen, FALSE);

			// リリース年号
			nLen = strlen(GetYear(pFileMP3));
			if (nLen > ID3_LEN_YEAR) nLen = ID3_LEN_YEAR;
			StringCopyN(&pTag->sYear[0], GetYear(pFileMP3), nLen, FALSE);

			if (GetBTrackNumber(pFileMP3) == 0xff) {
				// ID3 tag Ver 1.0
				// コメント
				nLen = strlen(GetComment(pFileMP3));
				if (nLen > ID3_LEN_COMMENT) nLen = ID3_LEN_COMMENT;
				memset(&pTag->sComment[0], 0x00, ID3_LEN_COMMENT);
				StringCopyN2(&pTag->sComment[0], GetComment(pFileMP3), nLen, FALSE);

				// 再設定
				SetFormat(pFileMP3, nFileTypeMP3V1);	// ファイル形式：MP3V1.0
				SetFileTypeName(pFileMP3, "MP3(ID3v1.0)");
			} else {
				// ID3 tag Ver 1.1
				// コメント
				nLen = strlen(GetComment(pFileMP3));
				if (nLen > ID3_LEN_COMMENT-2) nLen = ID3_LEN_COMMENT-2;
				memset(&pTag->sComment[0], 0x00, ID3_LEN_COMMENT-2);
				StringCopyN2(&pTag->sComment[0], GetComment(pFileMP3), nLen, FALSE);

				pTag->cZero = '\0';
				// トラック番号
				pTag->byTrackNumber = GetBTrackNumber(pFileMP3);

				// 再設定
				CString strTrackNumber;
				strTrackNumber.Format("%d", pTag->byTrackNumber);
				SetTrackNumber(pFileMP3, strTrackNumber);
				SetFormat(pFileMP3, nFileTypeMP3V11);	// ファイル形式：MP3V1.1
				SetFileTypeName(pFileMP3, "MP3(ID3v1.1)");
			}
			// ジャンル
			pTag->byGenre = GetBGenre(pFileMP3);
			SetGenre(pFileMP3, STEPGetGenreNameSIF(pTag->byGenre));

			file.Write(pTag, sizeof(ID3TAG));
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format("%s がオープンできませんでした", GetFullPath(pFileMP3));
		MessageBox(NULL, strMsg, "ファイルのオープン失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return false;
	}
	END_CATCH

	return true;
}


bool ConvID3tagToSIField(FILE_INFO *pFileMP3)
{
	SetModifyFlag(pFileMP3, true);						// 変更されたフラグをセット
	SetTrackNameSI(pFileMP3, GetTrackName(pFileMP3));	// 曲名
	SetArtistNameSI(pFileMP3, GetArtistName(pFileMP3));	// アーティスト名
	SetAlbumNameSI(pFileMP3, GetAlbumName(pFileMP3));	// 製品名
	SetCommentSI(pFileMP3, GetComment(pFileMP3));		// コメント文字列
	SetYearSI(pFileMP3, GetYear(pFileMP3));				// リリース年号

#define LIMIT_TEXT_LENGTH(strID3, nLen)	{			\
	TCHAR	sWorkBuffer[nLen+1];					\
	StringCopyN(sWorkBuffer, GetValue(pFileMP3, strID3), nLen);			\
	sWorkBuffer[nLen] = '\0';						\
	SetValue(pFileMP3, strID3, sWorkBuffer);			\
}
	// ID3 tag の文字数を調整(自動変換の為の対処)
	LIMIT_TEXT_LENGTH(FIELD_TRACK_NAME		, ID3_LEN_TRACK_NAME);
	LIMIT_TEXT_LENGTH(FIELD_ARTIST_NAME		, ID3_LEN_ARTIST_NAME);
	LIMIT_TEXT_LENGTH(FIELD_ALBUM_NAME		, ID3_LEN_ALBUM_NAME);
	LIMIT_TEXT_LENGTH(FIELD_YEAR			, ID3_LEN_YEAR);
	if (GetBTrackNumber(pFileMP3) != (BYTE)0xff) {
		LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT-2);
	} else {
		LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT);
	}
#undef LIMIT_TEXT_LENGTH
	// ソフトウェアの設定
	UINT nFormat = GetFormat(pFileMP3);
	if (!(nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)) {
		if (strlen(GetSoftwareSI(pFileMP3)) == 0) {
			SetSoftwareSI(pFileMP3, strOptSoftwareTag);
		}
	}

	if (strlen(GetGenreSI(pFileMP3)) == 0 || false /* 常に再設定→未設定時のみ */) {
		//SetGenreSI(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
		SetGenreSI(pFileMP3, GetGenre(pFileMP3));
	}
	if (strlen(GetTrackNumberSI(pFileMP3)) == 0) { /* 未設定時のみ */
		SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
	}
	if (strlen(GetDiskNumberSI(pFileMP3)) == 0) { /* 未設定時のみ */
		SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
	}
	return true;
}

void ConvSIFieldToID3tag(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char	sBuffer[256+1];

	if (GetFormat(pFileMP3) != nFileTypeID3V2 && GetFormat(pFileMP3) != nFileTypeRMP) {
		return;
	}

	SetModifyFlag(pFileMP3, TRUE);				// 変更されたフラグをセット

// コピー対象のフィールドかどうかをチェックして、必要な場合だけコピーします
#define COPY_FIELD(strID3, strSIF, len)	{			\
	if (nOptSIFieldConvType == SIF_CONV_ALL_FIELD	\
	|| strlen(strSIF) <= (len)) {					\
		StringCopyN(sBuffer, strSIF, len);			\
		sBuffer[len] = '\0';						\
		SetValue(pFileMP3, strID3, sBuffer);		\
	}												\
}
#define COPY_FIELD2(strID3, strSIF, len)	{			\
	if (nOptSIFieldConvType == SIF_CONV_ALL_FIELD	\
	|| strlen(strSIF) <= (len)) {					\
		StringCopyN2(sBuffer, strSIF, len);			\
		sBuffer[len] = '\0';						\
		SetValue(pFileMP3, strID3, sBuffer);		\
	}												\
}
	// 曲名
	COPY_FIELD(FIELD_TRACK_NAME, GetTrackNameSI(pFileMP3), ID3_LEN_TRACK_NAME);
	// アーティスト名
	COPY_FIELD(FIELD_ARTIST_NAME, GetArtistNameSI(pFileMP3), ID3_LEN_ARTIST_NAME);
	// アルバム名
	COPY_FIELD(FIELD_ALBUM_NAME, GetAlbumNameSI(pFileMP3), ID3_LEN_ALBUM_NAME);
	// トラック番号 //コメント設定より先に処理する
	if (strlen(GetTrackNumberSI(pFileMP3)) == 0) {
		if (GetFormat(pFileMP3) == nFileTypeID3V2) {
			SetBTrackNumber(pFileMP3, (BYTE)0xff);
		}
	} else {
		SetBTrackNumber(pFileMP3, (BYTE)STEPGetIntegerTrackNumber(GetTrackNumberSI(pFileMP3)));
	}
	// ディスク番号
	if (strlen(GetDiskNumberSI(pFileMP3)) == 0) {
		if (GetFormat(pFileMP3) == nFileTypeID3V2) {
			SetBDiskNumber(pFileMP3, (BYTE)0xff);
		}
	} else {
		SetBDiskNumber(pFileMP3, (BYTE)STEPGetIntegerDiskNumber(GetDiskNumberSI(pFileMP3)));
	}
	// コメント文字列
	if (GetBTrackNumber(pFileMP3) == (BYTE)0xff) {
		// ID3 v1.0
		COPY_FIELD2(FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT);
	} else {
		// ID3 v1.1
		COPY_FIELD2(FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT-2);
	}
	// リリース年号
	COPY_FIELD(FIELD_YEAR, GetYearSI(pFileMP3), ID3_LEN_YEAR);
	// ジャンル
	SetBGenre(pFileMP3, STEPGetGenreCode(GetGenreSI(pFileMP3)));
	SetGenre(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
#undef COPY_FIELD
#undef COPY_FIELD2

	return;
}

bool WriteTagID3v2(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ID3v2 タグの取得
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v2.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// 読み込み失敗
	||  id3v2.IsEnable() == FALSE			// ID3v2 ではない
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// 未対応のバージョン
		// 読み込み失敗
		return(false);
	}

	switch (nId3v2Version) {
	case 1:	// v2.2
		id3v2.SetVer(0x0200);
		break;
	case 2:	// v2.3
		id3v2.SetVer(0x0300);
		break;
	case 3:	// v2.4
		id3v2.SetVer(0x0400);
		break;
	default:
		break;
	}
	switch (nId3v2Encode) {
	case 1:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_ISO_8859_1);
		break;
	case 2:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16);
		break;
	case 3:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_8);
		break;
	//case 4:
	//	id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16BE);
	//	break;
	default:
		break;
	}

	// ID3tag の自動更新
//	if (bOptID3v2ID3tagAutoWrite) {
//		ConvSIFieldToID3tag(pFileMP3);
//	}
	// ソフトウェアの設定
	if (strlen(GetSoftwareSI(pFileMP3)) == 0) {
		SetSoftwareSI(pFileMP3, strOptSoftwareTag);
	}

	// *** タグ情報の設定 ***
	id3v2.SetTitle(GetTrackNameSI(pFileMP3));			// タイトル
	id3v2.SetArtist(GetArtistNameSI(pFileMP3));			// アーティスト名
	id3v2.SetAlbum(GetAlbumNameSI(pFileMP3));			// アルバム名
	id3v2.SetYear(GetYearSI(pFileMP3));					// リリース
	id3v2.SetEncoder(GetSoftwareSI(pFileMP3));			// エンコーダ
	id3v2.SetTrackNo(GetTrackNumberSI(pFileMP3));		// トラック番号
	id3v2.SetDiskNo(GetDiskNumberSI(pFileMP3));			// ディスク番号
	id3v2.SetComment(GetCommentSI(pFileMP3));			// コメント
	id3v2.SetCopyright(GetCopyrightSI(pFileMP3));		// 著作権
	id3v2.SetWriter(GetWriterSI(pFileMP3));				// 作詞
	id3v2.SetComposer(GetComposerSI(pFileMP3));			// 作曲
	id3v2.SetAlbmArtist(GetAlbumArtistSI(pFileMP3));		// Albm. アーティスト
	id3v2.SetOrigArtist(GetOrigArtistSI(pFileMP3));		// Orig.アーティスト
	id3v2.SetUrl(GetURLSI(pFileMP3));					// URL
	id3v2.SetEncodest(GetEncodest(pFileMP3));			// エンコードした人
	id3v2.SetEngineer(GetEngineerSI(pFileMP3));			// エンジニア（出版）
	// ジャンル名
	CString	strGenre;
	BYTE	byGenre;
	byGenre = STEPGetGenreCode(GetGenreSI(pFileMP3));
	if (!strlen(GetGenreSI(pFileMP3)) == 0) {
		if (byGenre == (BYTE)0xff || STEPIsUserGenre(GetGenreSI(pFileMP3)) || bOptID3v2GenreAddNumber == false) strGenre.Format("%s", GetGenreSI(pFileMP3));
		else                       strGenre.Format("(%d)%s", STEPGetGenreCode(GetGenreSI(pFileMP3)), GetGenreSI(pFileMP3));
	}
	id3v2.SetGenre(strGenre/* 2005.08.23 del , bOptID3v2GenreAddNumber*/);

	// *** タグ情報を更新する ***
	//return(id3v2.Save(/*AfxGetMainWnd()->GetSafeHwnd(),*/ GetFullPath(pFileMP3)) == ERROR_SUCCESS ? true : false);
	bool result = id3v2.Save(GetFullPath(pFileMP3)) == ERROR_SUCCESS ? true : false;
	if (result) {
		setFileType(id3v2, pFileMP3);
	}
	return result;
}

bool WriteTagSIF(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strlen(GetSoftwareSI(pFileMP3)) == 0) {
		SetSoftwareSI(pFileMP3, strOptSoftwareTag);
	}

	CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// 読み込み失敗
		||  rmp.IsEnable() == FALSE) {
		// 読み込み失敗
		return(false);
	}

	bool isNeedID3 = false;
	if (strlen(GetTrackName(pFileMP3))	> 0)	isNeedID3 = true;
	if (strlen(GetArtistName(pFileMP3))	> 0)	isNeedID3 = true;
	if (strlen(GetAlbumName(pFileMP3))	> 0)	isNeedID3 = true;
	if (strlen(GetComment(pFileMP3))	> 0)	isNeedID3 = true;
	if (strlen(GetYear(pFileMP3))		> 0)	isNeedID3 = true;
	if (strlen(GetGenre(pFileMP3))		> 0)	isNeedID3 = true;
	if (strlen(GetTrackNumber(pFileMP3))> 0)	isNeedID3 = true;
	if (isNeedID3 && !rmp.HasId3tag()) {
		rmp.SetHasId3tag(TRUE);
	}

	// ID3tag の自動更新
	//if (bOptRmpID3tagAutoWrite) {
	//	ConvSIFieldToID3tag(pFileMP3);
	//}

	// SI フィールドを更新
	rmp.SetNAM(GetTrackNameSI(pFileMP3));	// 曲名
	rmp.SetART(GetArtistNameSI(pFileMP3));	// アーティスト名
	rmp.SetPRD(GetAlbumNameSI(pFileMP3));	// 製品名
	rmp.SetCMT(GetCommentSI(pFileMP3));		// コメント文字列
	rmp.SetCRD(GetYearSI(pFileMP3));		// リリース年号
	rmp.SetGNR(GetGenreSI(pFileMP3));		// ジャンル名
	rmp.SetCOP(GetCopyrightSI(pFileMP3));	// 著作権
	rmp.SetENG(GetEngineerSI(pFileMP3));	// エンジニア
	rmp.SetSRC(GetSourceSI(pFileMP3));		// ソース
	rmp.SetSFT(GetSoftwareSI(pFileMP3));	// ソフトウェア
	rmp.SetKEY(GetKeywordSI(pFileMP3));		// キーワード
	rmp.SetTCH(GetTechnicianSI(pFileMP3));	// 技術者
	rmp.SetLYC(GetLyricSI(pFileMP3));		// 歌詞
	rmp.SetCMS(GetCommissionSI(pFileMP3));	// コミッション
	// SI フィールドを更新
	bool result = rmp.Save(NULL, GetFullPath(pFileMP3)) == ERROR_SUCCESS ? true : false;
	if (result) {
		rmp.Release();
		if (isNeedID3) {
			result = WriteTagID3(pFileMP3);
		}
		SetFormat(pFileMP3, nFileTypeRMP);		// ファイル形式：RIFF MP3
		SetFileTypeName(pFileMP3, "RIFF MP3");
	}
	return result;
}

bool IsCreateID3v2(FILE_INFO *pFileMP3)
{
	if (strlen(GetTrackName(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (strlen(GetArtistName(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (strlen(GetAlbumName(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (strlen(GetComment(pFileMP3))	> ID3_LEN_COMMENT-2)	return true;
	if (strlen(GetYear(pFileMP3))		> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))					return true;
	if (CString(GetComment(pFileMP3)).Find('\n') > -1)			return true;
	if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))				return true;
	if (!STEPIsNumeric(GetDiskNumberSI(pFileMP3)))				return true;
	/*
	if (strlen(GetCopyrightSI(pFileMP3)) > 0)					return true;	// 著作権
	if (strlen(GetComposerSI(pFileMP3)) > 0)					return true;	// 作曲
	if (strlen(GetOrigArtistSI(pFileMP3)) > 0)					return true;	// Orig.アーティスト
	if (strlen(GetURLSI(pFileMP3)) > 0)							return true;	// URL
	if (strlen(GetEncodest(pFileMP3)) > 0)						return true;	// エンコードした人
	if (strlen(GetSoftwareSI(pFileMP3)) > 0
		&& strcmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)		return true;	// ソフトウェア
	*/
	return false;
}

bool IsCreateRMP(FILE_INFO *pFileMP3)
{
	if (strlen(GetTrackName(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (strlen(GetArtistName(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (strlen(GetAlbumName(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (strlen(GetComment(pFileMP3))	> ID3_LEN_COMMENT-2)	return true;
	if (strlen(GetYear(pFileMP3))		> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))					return true;
	/*
	if (strlen(GetCopyrightSI(pFileMP3))	> 0)	return true;
	rmp.SetENG(GetEngineerSI(pFileMP3));	// エンジニア
	rmp.SetSRC(GetSourceSI(pFileMP3));		// ソース
	rmp.SetSFT(GetSoftwareSI(pFileMP3));	// ソフトウェア
	rmp.SetKEY(GetKeywordSI(pFileMP3));		// キーワード
	rmp.SetTCH(GetTechnicianSI(pFileMP3));	// 技術者
	rmp.SetLYC(GetLyricSI(pFileMP3));		// 歌詞
	rmp.SetCMS(GetCommissionSI(pFileMP3));	// コミッション
	*/

	return false;
}

bool IsCreateID3v2SI(FILE_INFO *pFileMP3, bool bID3v1Only = FALSE)
{
	if (strlen(GetTrackNameSI(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (strlen(GetArtistNameSI(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (strlen(GetAlbumNameSI(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (strlen(GetCommentSI(pFileMP3))		> ID3_LEN_COMMENT-2)	return true;
	if (strlen(GetYearSI(pFileMP3))			> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))						return true;
	if (CString(GetCommentSI(pFileMP3)).Find('\n') > -1)			return true;
	if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))					return true;
	if (!STEPIsNumeric(GetDiskNumberSI(pFileMP3)))					return true;
	if (bID3v1Only == TRUE)	return false;
	if (strlen(GetCopyrightSI(pFileMP3)) > 0)						return true;	// 著作権
	if (strlen(GetComposerSI(pFileMP3)) > 0)						return true;	// 作曲
	if (strlen(GetOrigArtistSI(pFileMP3)) > 0)						return true;	// Orig.アーティスト
	if (strlen(GetAlbumArtistSI(pFileMP3)) > 0)						return true;	// Albm.アーティスト
	if (strlen(GetWriterSI(pFileMP3)) > 0)							return true;	// 作詞者
	if (strlen(GetURLSI(pFileMP3)) > 0)								return true;	// URL
	if (strlen(GetEncodest(pFileMP3)) > 0)							return true;	// エンコードした人
	if (strlen(GetEngineerSI(pFileMP3)) > 0)						return true;	// エンジニア
	if (strlen(GetSoftwareSI(pFileMP3)) > 0
		&& strcmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)		return true;	// ソフトウェア
	return false;
}

bool IsMatchID3SI(FILE_INFO* pFileMP3)
{
	if (CString(GetTrackName(pFileMP3)) != GetTrackNameSI(pFileMP3))		return false;
	if (CString(GetArtistName(pFileMP3)) != GetArtistNameSI(pFileMP3))		return false;
	if (CString(GetAlbumName(pFileMP3)) != GetAlbumNameSI(pFileMP3))		return false;
	if (CString(GetComment(pFileMP3)) != GetCommentSI(pFileMP3))			return false;
	if (CString(GetYear(pFileMP3)) != GetYearSI(pFileMP3))					return false;
	if (CString(GetGenre(pFileMP3)) != GetGenreSI(pFileMP3))				return false;
	if (CString(GetTrackNumber(pFileMP3)) != GetTrackNumberSI(pFileMP3))	return false;
	if (CString(GetDiskNumber(pFileMP3)) != GetDiskNumberSI(pFileMP3))		return false;
	return true;
}

void copySIField(FILE_INFO *pFileMP3)
{
	if (GetFormat(pFileMP3) != nFileTypeRMP) {
		CString strTrackNumber = GetTrackNumberSI(pFileMP3);
		if (strTrackNumber.IsEmpty()) {
			SetBTrackNumber(pFileMP3, 0xff);
		} else {
			SetBTrackNumber(pFileMP3, (BYTE)STEPGetIntegerTrackNumber(strTrackNumber));
		}

		CString strDiskNumber = GetDiskNumberSI(pFileMP3);
		if (strDiskNumber.IsEmpty()) {
			SetBDiskNumber(pFileMP3, 0xff);
		} else {
			SetBDiskNumber(pFileMP3, (BYTE)STEPGetIntegerDiskNumber(strDiskNumber));
		}
	}

	CString strGenre = STEPGetGenreNameSIF(STEPGetGenreCode(GetGenreSI(pFileMP3)));
	SetGenre(pFileMP3, strGenre);
	SetBGenre(pFileMP3, STEPGetGenreCode(GetGenreSI(pFileMP3)));
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

/***
	if ((bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite || bOptAutoConvID3v2 || bOptAutoConvRMP) &&
		(nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)) {
		// ID3tag自動更新の場合はSIに設定されるのでコピー
		copySIField(pFileMP3);
	}
***/
	if (bOptID3v2ID3tagAutoWrite && nFormat == nFileTypeID3V2) {
		copySIField(pFileMP3);
	}
	if (bOptRmpID3tagAutoWrite && nFormat == nFileTypeRMP) {
		copySIField(pFileMP3);
	}

	if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) {
		// MP3 => RMP/ID3v2 自動フォーマット変換処理
		if (bOptAutoConvID3v2) {
			if (IsCreateID3v2(pFileMP3) || IsCreateID3v2SI(pFileMP3)/* || !IsMatchID3SI(pFileMP3)*/) {
				// ID3tag自動更新の場合はSIに設定されるのでコピー
				copySIField(pFileMP3);
				extern bool WINAPI ConvFileFormatID3v2(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd);
				if (ConvFileFormatID3v2(pFileMP3, 0, NULL) == false) {
					return STEP_ERROR;
				} else {
					return STEP_SUCCESS; // タグを変換処理で更新しているので終了
				}
			} else {
				// ID3tag自動更新の場合はSIに設定されるのでコピー
				copySIField(pFileMP3);
			}
		} else if (bOptAutoConvRMP) {
			if (IsCreateRMP(pFileMP3) /*|| !IsMatchID3SI(pFileMP3)*/) {
				// ID3tag自動更新の場合はSIに設定されるのでコピー
				copySIField(pFileMP3);
				extern bool WINAPI ConvFileFormatRMP(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd);
				if (ConvFileFormatRMP(pFileMP3, 0, NULL) == false) {
					return STEP_ERROR;
				} else {
					return STEP_SUCCESS; // タグを変換処理で更新しているので終了
				}
			} else {
				// ID3tag自動更新の場合はSIに設定されるのでコピー
				copySIField(pFileMP3);
			}
		}
		if (WriteTagID3(pFileMP3) == false) { // 自動変換しないとき
			return STEP_ERROR;
		}
		ConvID3tagToSIField(pFileMP3);
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeID3V2) {
		// ID3tag の自動更新
		if (bOptID3v2ID3tagAutoWrite) {
			ConvSIFieldToID3tag(pFileMP3);
		}

		if (WriteTagID3(pFileMP3) == false) { // FileType設定のためID3v2の前に書き換え
			return STEP_ERROR;
		}
		if (bOptID3v2ID3tagAutoDelete) {
			SetFormat(pFileMP3, nFileTypeID3V2);
		}
		if (WriteTagID3v2(pFileMP3) == false) {
			return STEP_ERROR;
		}
		if (bOptID3v2ID3tagAutoDelete) {
			// ID3 tagを削除
			extern bool DeleteTagID3v1(const char *sFileName, HWND hWnd);
			DeleteTagID3v1(GetFullPath(pFileMP3), NULL);
		}
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeRMP) {
		// ID3tag の自動更新
		if (bOptRmpID3tagAutoWrite) {
			ConvSIFieldToID3tag(pFileMP3);
		}
		if (WriteTagSIF(pFileMP3) == false) {
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPropertySheet page;
	CDlgFileRmpID3v2 dlg1;
	CDlgDefaultValue dlg2;
	page.SetTitle(CString(STEPGetPluginName()) + " オプション設定");
	dlg1.m_bRmpID3tagAutoWrite = bOptRmpID3tagAutoWrite;
	dlg1.m_bAutoConvRMP = bOptAutoConvRMP;
	dlg1.m_nSIFieldConvertType = nOptSIFieldConvType;
	dlg1.m_bRmpGenreListSelect = bOptRmpGenreListSelect;
	dlg1.m_bChangeFileExt = bOptChangeFileExt;
	dlg1.m_bAutoConvID3v2 = bOptAutoConvID3v2;
	dlg1.m_bID3v2ID3tagAutoWrite = bOptID3v2ID3tagAutoWrite;
	dlg1.m_bID3v2GenreListSelect = bOptID3v2GenreListSelect;
	dlg1.m_bID3v2GenreAddNumber = bOptID3v2GenreAddNumber;
	dlg1.m_bID3v2Id3tagAutoDelete = bOptID3v2ID3tagAutoDelete;
	page.AddPage(&dlg1);
	dlg2.m_strSoftwareTag = strOptSoftwareTag;
	dlg2.m_bID3v2UnSync = bOptNotUnSyncAlways;
	dlg2.m_bID3v2UnSyncNew = bOptUnSyncNew;
	dlg2.m_nId3v2Encode = nId3v2Encode;
	dlg2.m_nId3v2Version = nId3v2Version;
	dlg2.m_nId3v2EncodeNew = nId3v2EncodeNew;
	dlg2.m_nId3v2VersionNew = nId3v2VersionNew;

	page.AddPage(&dlg2);
	if (page.DoModal() == IDOK) {
		bOptRmpID3tagAutoWrite = dlg1.m_bRmpID3tagAutoWrite ? true : false;
		bOptAutoConvRMP = dlg1.m_bAutoConvRMP ? true : false;
		nOptSIFieldConvType = dlg1.m_nSIFieldConvertType;
		bOptRmpGenreListSelect = dlg1.m_bRmpGenreListSelect ? true : false;
		bOptChangeFileExt = dlg1.m_bChangeFileExt ? true : false;
		bOptAutoConvID3v2 = dlg1.m_bAutoConvID3v2 ? true : false;
		bOptID3v2ID3tagAutoWrite = dlg1.m_bID3v2ID3tagAutoWrite ? true : false;
		bOptID3v2GenreListSelect = dlg1.m_bID3v2GenreListSelect ? true : false;
		bOptID3v2GenreAddNumber = dlg1.m_bID3v2GenreAddNumber ? true : false;
		bOptID3v2ID3tagAutoDelete = dlg1.m_bID3v2Id3tagAutoDelete ? true : false;
		strOptSoftwareTag = dlg2.m_strSoftwareTag;
		bOptNotUnSyncAlways = dlg2.m_bID3v2UnSync ? true : false;
		bOptUnSyncNew = dlg2.m_bID3v2UnSyncNew ? true : false;
		nId3v2Encode = dlg2.m_nId3v2Encode;
		nId3v2Version = dlg2.m_nId3v2Version;
		nId3v2EncodeNew = dlg2.m_nId3v2EncodeNew;
		nId3v2VersionNew = dlg2.m_nId3v2VersionNew;

		// INIに保存
		WritePrivateProfileString("RMP_ID3V2", "AutoConvID3v2", bOptAutoConvID3v2 ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "AutoConvRMP", bOptAutoConvRMP  ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "SIFieldConvType", nOptSIFieldConvType == SIF_CONV_ALL_FIELD ? "0" : "1", strINI);
		WritePrivateProfileString("RMP_ID3V2", "ID3v2ID3tagAutoWrite", bOptID3v2ID3tagAutoWrite ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "RmpID3tagAutoWrite", bOptRmpID3tagAutoWrite ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "ID3v2GenreListSelect", bOptID3v2GenreListSelect ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "RmpGenreListSelect", bOptRmpGenreListSelect ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "ID3v2ID3tagAutoDelete", bOptID3v2ID3tagAutoDelete ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "ID3v2GenreAddNumber", bOptID3v2GenreAddNumber ? "1" : "0", strINI);
		WritePrivateProfileString("RMP_ID3V2", "ChangeFileExt", bOptChangeFileExt ? "1" : "0", strINI);

		WritePrivateProfileString("OTHER", "SoftwareTag", strOptSoftwareTag, strINI);

		WritePrivateProfileString("OTHER", "ID3v2NotUnSync", bOptNotUnSyncAlways ? "1" : "0", strINI);
		WritePrivateProfileString("OTHER", "ID3v2UnSyncNew", bOptUnSyncNew ? "1" : "0", strINI);

		CString sValue;
		sValue.Format("%d", nId3v2Encode);
		WritePrivateProfileString("OTHER", "ID3v2CharEncode", sValue, strINI);
		sValue.Format("%d", nId3v2Version);
		WritePrivateProfileString("OTHER", "ID3v2Version", sValue, strINI);
		sValue.Format("%d", nId3v2EncodeNew);
		WritePrivateProfileString("OTHER", "ID3v2CharEncodeNew", sValue, strINI);
		sValue.Format("%d", nId3v2VersionNew);
		WritePrivateProfileString("OTHER", "ID3v2VersionNew", sValue, strINI);

		STEPUpdateCellInfo();
	}
}

STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		return "標準MP3形式/ID3v2形式に自動変換";
	}
	if (nID == nIDFileConvMP3) {
		return "標準MP3形式に変換";
	}
	if (nID == nIDFileConvID3v2) {
		return "ID3v2形式に変換";
	}
	if (nID == nIDFileConvRMP) {
		return "RIFF MP3形式に変換";
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		return "SI/ID3v2フィールドからID3tagに変換";
//	}
	if (nID == nIDDeleteID3) {
		return "ID3tagを削除";
	}
	if (nID == nIDConvID3v2Version) {
		return "ID3v2バージョンの変換";
	}
	return NULL;
}

STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		return "タグの入力桁数/内容によって標準MP3形式またはID3v2形式のいずれかに変換します";
	}
	if (nID == nIDFileConvMP3) {
		return "選択されているファイルを標準MP3形式に変換します";
	}
	if (nID == nIDFileConvID3v2) {
		return "選択されているファイルをID3v2形式に変換します";
	}
	if (nID == nIDFileConvRMP) {
		return "選択されているファイルをRIFF MP3形式に変換します";
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		return "SI/ID3v2フィールドからID3tagに変換します";
//	}
	if (nID == nIDDeleteID3) {
		return "選択されているMP3ファイルのID3tagを削除します";
	}
	if (nID == nIDConvID3v2Version) {
		return "ID3v2バージョン/文字エンコードの変換";
	}
	return NULL;
}

STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
	if (nID == nIDFileConvMP3) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
	if (nID == nIDFileConvID3v2) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
	if (nID == nIDFileConvRMP) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		if (STEPGetSelectedItem() < 0) return false;
//		else return true;
//	}
	if (nID == nIDDeleteID3) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
	if (nID == nIDConvID3v2Version) {
		if (STEPGetSelectedItem() < 0) return false;
		else return true;
	}
	return false;
}

STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		static	const char *sMessage = "選択されているファイルを 標準MP3 形式 または ID3v2 形式に変換します\n\n"
									   "変換を実行してもよろしいですか？";
		if (/*g_bConfConvID3v2 == false || */MessageBox(hWnd, sMessage, "標準MP3 形式/ID3v2 形式に変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatAuto(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("標準MP3形式/ID3v2 形式に変換中.....", ConvFileFormatAuto);
		}
		return true;
	}
	if (nID == nIDFileConvMP3) {
		static	const char *sMessage = "選択されているファイルを標準MP3 形式に変換します\n"
									   "標準MP3 形式に変換すると、タグ情報の一部を失う可能性があります\n\n"
									   "変換を実行してもよろしいですか？";
		if (/*g_bConfConvMP3 == false || */MessageBox(hWnd, sMessage, "標準MP3 形式に変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatMP3(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("標準MP3 形式に変換中.....", ConvFileFormatMP3);
		}
		return true;
	}
	if (nID == nIDFileConvID3v2) {
		static	const char *sMessage = "選択されているファイルを ID3v2 形式に変換します\n\n"
									   "変換を実行してもよろしいですか？";
		if (/*g_bConfConvID3v2 == false || */MessageBox(hWnd, sMessage, "ID3v2 形式に変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatID3v2(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("ID3v2 形式に変換中.....", ConvFileFormatID3v2);
		}
		return true;
	}
	if (nID == nIDFileConvRMP) {
		static	const char *sMessage = "選択されているファイルを RIFF MP3 形式に変換します\n\n"
									   "変換を実行してもよろしいですか？";
		if (/*g_bConfConvRMP == false || */MessageBox(hWnd, sMessage, "RIFF MP3 形式に変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatRMP(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("RIFF MP3 形式に変換中.....", ConvFileFormatRMP);
		}
		return true;
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		extern bool WINAPI ConvSiFieldToTd3tag(FILE_INFO*, int, HWND);
//		STEPProcessSelectedFilesForUpdate("SI/ID3v2フィールドからID3tagに変換中.....", ConvSiFieldToTd3tag);
//		return true;
//	}
	if (nID == nIDDeleteID3) {
		static	const char *sMessage = "選択されているファイルのID3v1タグを削除します\n\n"
									   "実行してもよろしいですか？";
		if (MessageBox(hWnd, sMessage, "ID3tag 削除", MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI DeleteId3tag(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("ID3tag を削除中.....", DeleteId3tag);
		}
		return true;
	}
	if (nID == nIDConvID3v2Version) {
		CDlgConvID3v2Version dlg;
		dlg.m_nId3v2Encode = nId3v2Encode;
		dlg.m_nId3v2Version = nId3v2Version;
		if (dlg.DoModal() == IDOK) {
			nId3v2EncodeConv = dlg.m_nId3v2Encode;
			nId3v2VersionConv = dlg.m_nId3v2Version;
			extern bool WINAPI ConvID3v2Version(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate("ID3v2 バージョン/文字エンコードを変換中.....", ConvID3v2Version);
		}
		return true;
	}

	return false;
}

STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nType) {
	case MENU_SELECT_FOLDER:
	case MENU_FILE_RANGE:
	case MENU_FILE_EDIT_OK:
		AddConvMenu(hMenu);
		AddEditMenu(hMenu);
		break;
	}
}

STEP_API void WINAPI STEPOnLoadMainMenu()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// メニューへの追加
	HMENU hMenu = STEPGetMenu(MENU_CONV);
	AddConvMenu(hMenu);
	hMenu = STEPGetMenu(MENU_EDIT);
	AddEditMenu(hMenu);
}

STEP_API bool WINAPI STEPOnConvSiFieldToId3tag(FILE_INFO* pFileInfo)
{
	ConvSIFieldToID3tag(pFileInfo);
	return false;
}