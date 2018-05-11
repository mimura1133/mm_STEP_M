// STEP_mp3.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_mp3.h"
#include "ID3v2/Id3tagv2.h"
#include "ID3v2/RMP.h"
#include "STEPlugin.h"

#include "DlgFileRmpID3v2.h"
#include "DlgDefaultValue.h"
#include "DlgConvID3v2Version.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
extern void Free_mp3infp();

#pragma pack(push, 1)
struct	ID3TAG	{						// <<< ID3 Tag �̃f�[�^�`�� >>>
	BYTE	byData[128];				// 128�o�C�g�f�[�^
};

#define ID3_LEN_TRACK_NAME		30u		// �g���b�N��    (������c30BYTE)
#define ID3_LEN_ARTIST_NAME		30u		// �A�[�e�B�X�g��(������c30BYTE)
#define ID3_LEN_ALBUM_NAME		30u		// �A���o����    (������c30BYTE)
#define ID3_LEN_COMMENT			30u		// �R�����g      (������c30BYTE)
#define ID3_LEN_YEAR			4u		// �����[�X�N��  (������c 4BYTE)

struct	ID3TAG_V10	{							// <<< ID3 Tag v1.0 �̃f�[�^�`�� >>>
	CHAR	sTAG[3];							// "TAG"         (������c 3BYTE)
	CHAR	sTrackName[ID3_LEN_TRACK_NAME];		// �g���b�N��    (������c30BYTE)
	CHAR	sArtistName[ID3_LEN_ARTIST_NAME];	// �A�[�e�B�X�g��(������c30BYTE)
	CHAR	sAlbumName[ID3_LEN_ALBUM_NAME];		// �A���o����    (������c30BYTE)
	CHAR	sYear[4];							// �����[�X�N��  (������c 4BYTE)
	CHAR	sComment[ID3_LEN_COMMENT];			// �R�����g      (������c30BYTE)
	BYTE	byGenre;							// �W�������ԍ�  (�����c�c 1BYTE)
};
static_assert(sizeof(ID3TAG_V10) == 128, "sizeof(ID3TAG_V10) == 128");

struct	ID3TAG_V11	{							// <<< ID3 Tag v1.1 �̃f�[�^�`�� >>>
	CHAR	sTAG[3];							// "TAG"         (������c 3BYTE)
	CHAR	sTrackName[ID3_LEN_TRACK_NAME];		// �g���b�N��    (������c30BYTE)
	CHAR	sArtistName[ID3_LEN_ARTIST_NAME];	// �A�[�e�B�X�g��(������c30BYTE)
	CHAR	sAlbumName[ID3_LEN_ALBUM_NAME];		// �A���o����    (������c30BYTE)
	CHAR	sYear[4];							// �����[�X�N��  (������c 4BYTE)
	CHAR	sComment[ID3_LEN_COMMENT-2];		// �R�����g      (������c30BYTE)
	CHAR	cZero;								// '\0'          (������c 1BYTE)
	BYTE	byTrackNumber;						// �g���b�N�ԍ�  (�����c�c 1BYTE)
	BYTE	byGenre;							// �W�������ԍ�  (�����c�c 1BYTE)
};
static_assert(sizeof(ID3TAG_V11) == 128, "sizeof(ID3TAG_V11) == 128");
#pragma pack(pop)

//
//	����!
//
//		���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//		MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//		�ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE
//		�}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//		}
//
//		���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//		�ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//		����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//		��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC
//		DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//		�W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//		�ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//		58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App

BEGIN_MESSAGE_MAP(CSTEP_mp3App, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_mp3App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App �̍\�z

CSTEP_mp3App::CSTEP_mp3App()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_mp3App �I�u�W�F�N�g

CSTEP_mp3App theApp;

UINT nPluginID;
UINT nFileTypeMP3;
UINT nFileTypeMP3V1;
UINT nFileTypeMP3V11;
UINT nFileTypeID3V2;
UINT nFileTypeRMP;

enum	{SIF_CONV_ALL_FIELD, SIF_CONV_LENGTH_OK};

CString strINI;
// �I�v�V�����ݒ�
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

// �R�}���hID
UINT nIDFileConvAutoID3;
UINT nIDFileConvMP3;
UINT nIDFileConvID3v2;
UINT nIDFileConvRMP;
//UINT nIDConvSIFieldToID3Tag; /* 1.02 �{�̕W���@�\���g�� */
UINT nIDDeleteID3;
UINT nIDConvID3v2Version;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TEXT("Version 1.02 Copyright (C) 2003-2006 haseta\r\nVersion 1.04M Copyright (C) 2008-2010 Mimura\r\nMP3(ID3v1/ID3v2)/RIFF�`�����T�|�[�g���Ă��܂�");
}

void AddConvMenu(HMENU hMenu) {
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvMP3, TEXT("�W��MP3�`���ɕϊ�"));
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvRMP, TEXT("RIFF MP3�`���ɕϊ�"));
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvID3v2, TEXT("ID3v2�`���ɕϊ�"));
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvAutoID3, TEXT("�W��MP3�`��/ID3v2�`���Ɏ����ϊ�"));
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDConvID3v2Version, TEXT("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�"));
//	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
//	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDConvSIFieldToID3Tag, TEXT("SI/ID3v2�t�B�[���h����ID3tag�ɕϊ�"));
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
				if (strcmp(szMenu, "�ҏW�O�̏�Ԃɖ߂�") == 0) {
					InsertMenu(hMenu, iPos, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, TEXT("ID3tag���폜"));
					bAddDeleteID3 = true;
					iPos++;
				}
			}
		}
	}

	if (!bAddDeleteID3) {
		InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, TEXT("ID3tag���폜"));
	}
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += "STEP_mp3.ini";
	bOptAutoConvID3v2 = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("AutoConvID3v2"), 1, strINI) ? true : false;
	bOptAutoConvRMP = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("AutoConvRMP"), 0, strINI) ? true : false;
	nOptSIFieldConvType = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("SIFieldConvType"), SIF_CONV_ALL_FIELD, strINI);
	bOptID3v2ID3tagAutoWrite = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("ID3v2ID3tagAutoWrite"), 1, strINI) ? true : false;
	bOptRmpID3tagAutoWrite = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("RmpID3tagAutoWrite"), 1, strINI) ? true : false;
	bOptID3v2GenreListSelect = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("ID3v2GenreListSelect"), 1, strINI) ? true : false;
	bOptRmpGenreListSelect = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("RmpGenreListSelect"), 1, strINI) ? true : false;
	bOptID3v2ID3tagAutoDelete = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("ID3v2ID3tagAutoDelete"), 0, strINI) ? true : false;
	bOptID3v2GenreAddNumber = false/* 2005.08.23 GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("ID3v2GenreAddNumber"), 0, strINI) ? true : false*/;
	bOptChangeFileExt = GetPrivateProfileInt(TEXT("RMP_ID3V2"), TEXT("ChangeFileExt"), 0, strINI) ? true : false;

	GetPrivateProfileString(TEXT("OTHER"), TEXT("SoftwareTag"), TEXT(""), strOptSoftwareTag.GetBufferSetLength(255+1), 255, strINI);
	strOptSoftwareTag.ReleaseBuffer();

	bOptNotUnSyncAlways = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2NotUnSync"), 0, strINI) ? true : false;
	bOptUnSyncNew = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2UnSyncNew"), 0, strINI) ? true : false;

	nId3v2Encode = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2CharEncode"), 0, strINI);
	nId3v2Version = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2Version"), 0, strINI);
	nId3v2EncodeNew = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2CharEncodeNew"), 0, strINI);
	nId3v2VersionNew = GetPrivateProfileInt(TEXT("OTHER"), TEXT("ID3v2VersionNew"), 0, strINI);

	// �T�|�[�g���Ă���g���q��o�^
	HBITMAP hMP3Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hMP3V1Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hMP3V11Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
	HBITMAP hID3v2Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_ID3V2));
	HBITMAP hRMPBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_RMP));
	nFileTypeMP3 = STEPRegisterExt(nPluginID, TEXT("mp3"), hMP3Bitmap);
	nFileTypeMP3V1 = STEPRegisterExt(nPluginID, TEXT("mp3"), hMP3V1Bitmap);
	nFileTypeMP3V11 = STEPRegisterExt(nPluginID, TEXT("mp3"), hMP3V11Bitmap);
	nFileTypeID3V2 = STEPRegisterExt(nPluginID, TEXT("mp3"), hID3v2Bitmap);
	nFileTypeRMP = STEPRegisterExt(nPluginID, TEXT("rmp"), hRMPBitmap);
	DeleteObject(hMP3Bitmap);
	DeleteObject(hMP3V1Bitmap);
	DeleteObject(hMP3V11Bitmap);
	DeleteObject(hID3v2Bitmap);
	DeleteObject(hRMPBitmap);

	// �c�[���o�[�ւ̓o�^
	COLORMAP map; // �w�i�F�̕ϊ�
	map.from = RGB(192,192,192);
	map.to = GetSysColor(COLOR_3DFACE);

	nIDFileConvMP3 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_MP3, 0, &map, 1), nIDFileConvMP3, "STEP_mp3_FILE_CONV_MP3");
	STEPKeyAssign(nIDFileConvMP3, TEXT("�W��MP3�`���ɕϊ�"), TEXT("STEP_mp3_KEY_FILE_CONV_MP3"));

	nIDFileConvRMP = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_RMP, 0, &map, 1), nIDFileConvRMP, "STEP_mp3_FILE_CONV_RMP");
	STEPKeyAssign(nIDFileConvRMP, TEXT("RIFF MP3�`���ɕϊ�"), TEXT("STEP_mp3_KEY_FILE_CONV_RMP"));

	nIDFileConvID3v2 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_ID3V2, 0, &map, 1), nIDFileConvID3v2, "STEP_mp3_FILE_CONV_ID3V2");
	STEPKeyAssign(nIDFileConvID3v2, TEXT("ID3v2 �`���ɕϊ�"), TEXT("STEP_mp3_KEY_FILE_CONV_ID3V2"));

	nIDFileConvAutoID3 = STEPGetCommandID();
	STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_AUTO_ID3, 0, &map, 1), nIDFileConvAutoID3, "STEP_mp3_FILE_CONV_AUTO_ID3");
	STEPKeyAssign(nIDFileConvAutoID3, TEXT("�W��MP3�`��/ID3v2 �`���Ɏ����ϊ�"), TEXT("STEP_mp3_KEY_FILE_CONV_AUTO_ID3"));

//	nIDConvSIFieldToID3Tag = STEPGetCommandID();
//	STEPKeyAssign(nIDConvSIFieldToID3Tag, TEXT("SI/ID3v2�t�B�[���h����ID3tag�ɕϊ�"), TEXT("STEP_mp3_KEY_CONV_SI_FIELD_TO_ID3TAG"));

	nIDDeleteID3 = STEPGetCommandID();
	STEPKeyAssign(nIDDeleteID3, TEXT("ID3tag ���폜"), TEXT("STEP_mp3_KEY_DELETE_ID3"));

	nIDConvID3v2Version = STEPGetCommandID();
	STEPKeyAssign(nIDConvID3v2Version, TEXT("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�"), TEXT("STEP_mp3_KEY_CONV_ID3V2_VERSION"));

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
	return TEXT("STEP_mp3");
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
		case COLUMN_GENRE:			return 128; // ID3v1�̏ꍇ�A�W���������̂Ɠ��͂ł���ő咷�͈Ⴄ�̂�
		case COLUMN_COMMENT:		return ID3_LEN_COMMENT;
		default:					return 0;
		}
	}
	if (((nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && bOptAutoConvID3v2) || nFormat == nFileTypeID3V2) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		// �g���b�N��
		case COLUMN_ARTIST_NAME:	// �A�[�e�B�X�g��
		case COLUMN_ALBUM_NAME:		// �A���o����
		case COLUMN_TRACK_NUMBER:	// �g���b�N�ԍ�
		case COLUMN_DISK_NUMBER:	// �f�B�X�N�ԍ�
		case COLUMN_YEAR:			// �����[�X�N��
		case COLUMN_GENRE:			// �W�������ԍ�
		case COLUMN_COPYRIGHT:		// ���쌠
		case COLUMN_SOFTWARE:		// �\�t�g�E�F�A
		case COLUMN_WRITER:			// �쎌
		case COLUMN_COMPOSER:		// ���
		case COLUMN_ALBM_ARTIST:	// Albm.�A�[�e�B�X�g
		case COLUMN_ORIG_ARTIST:	// Orig.�A�[�e�B�X�g
		case COLUMN_URL:			// URL
		case COLUMN_ENCODEST:		// �G���R�[�h�����l
		case COLUMN_ENGINEER:		// �G���W�j�A�i�o�Łj
			//return 256;
		case COLUMN_COMMENT:		// �R�����g
			return 2048; /* 2003.06.20 ���₵�� */
		}
	} else {
		switch (nColumn) {
		case COLUMN_TRACK_NUMBER:	// �g���b�N�ԍ�
			return 3;
		case COLUMN_DISK_NUMBER:	// �f�B�X�N�ԍ�
			return 3;
		case COLUMN_TRACK_NAME:		// �g���b�N��
		case COLUMN_ARTIST_NAME:	// �A�[�e�B�X�g��
		case COLUMN_ALBUM_NAME:		// �A���o����
		case COLUMN_YEAR:			// �����[�X�N��
		case COLUMN_GENRE:			// �W�������ԍ�
		case COLUMN_COMMENT:		// �R�����g
		case COLUMN_COPYRIGHT:		// ���쌠
		case COLUMN_ENGINEER:		// �G���W�j�A(�����)
		case COLUMN_SOURCE:			// �\�[�X
		case COLUMN_SOFTWARE:		// �\�t�g�E�F�A
		case COLUMN_KEYWORD:		// �L�[���[�h
		case COLUMN_TECHNICIAN:		// �Z�p��
		case COLUMN_LYRIC:			// �̎�
			return 2048;
		}
	}
	return 0;
}

// ID3 Tag v1.0/v1.1 ���ǂ����𒲂ׂ�
static	bool	IsID3Tag(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return((((const ID3TAG_V10 *)(data))->sTAG[0] == 'T' &&
			((const ID3TAG_V10 *)(data))->sTAG[1] == 'A' &&
			((const ID3TAG_V10 *)(data))->sTAG[2] == 'G') ? true : false);
}
// ID3 Tag v1.0 ���ǂ����𒲂ׂ�
static	bool	IsID3Tag10(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero != '\0') || ((const ID3TAG_V11 *)data)->byTrackNumber == '\0')) ? true : false);
}
// ID3 Tag v1.1 ���ǂ����𒲂ׂ�
static	bool	IsID3Tag11(const ID3TAG *data) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero == '\0') && ((const ID3TAG_V11 *)data)->byTrackNumber != '\0'))? true : false);
}

/* �����̂P�o�C�g�ڂ��ǂ����̌��� */
#ifndef iskanji
#define iskanji(c)		((unsigned char)(c) >= 0x81 && (unsigned char)(c) <= 0x9f || (unsigned char)(c) >= 0xe0 && (unsigned char)(c) <= 0xfc)
#endif
void	StringCopyN(LPSTR, LPCSTR, int, BOOL = TRUE);
void	StringCopyN2(LPSTR, LPCSTR, int, BOOL = TRUE);
void StringCopyN(LPSTR sDest, LPCSTR sSrc, int nLen, BOOL bTerm)
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
void StringCopyN2(LPSTR sDest, LPCSTR sSrc, int nLen, BOOL bTerm)
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

void DeleteLineEndSpace(WCHAR *sBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int		nPos = lstrlenW(sBuffer) - 1;
	while(nPos >= 0 && sBuffer[nPos] == L' ') {
		sBuffer[nPos] = L'\0';
		nPos--;
	}
}
void DeleteLineEndSpace(CHAR *sBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int		nPos = strlen(sBuffer) - 1;
	while (nPos >= 0 && sBuffer[nPos] == ' ') {
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
	SetFormat(pFileMP3, nFileTypeID3V2);		// �t�@�C���`���FMP3(ID3v2)
}

bool ReadTagID3(LPCTSTR sFileName, FILE_INFO *pFileMP3, char *sHead)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRY {
		ID3TAG	tag;
		CFile	file(sFileName, CFile::modeRead | CFile::shareDenyNone);

		// �擪�̂S�o�C�g���擾
		if (sHead != NULL && file.Read(sHead, 4) != 4) {
			// �ǂݍ��ݎ��s
			return(false);
		}

		if (GetFileSize(pFileMP3) > sizeof(ID3TAG)) {
			// === ID3 tag v1.0/1.1 �̓ǂݍ��� ===
			// �t�@�C���̍Ō��128�o�C�g���擾
			file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			if (file.Read(&tag.byData[0], sizeof(ID3TAG)) != sizeof(ID3TAG)) {
				// �ǂݍ��ݎ��s
				return(false);
			}

			if (IsID3Tag(&tag) == true) {
				// ID3 tag �����݂���
				ID3TAG_V11	*pTag = (ID3TAG_V11 *)&tag;
				CHAR	sBuffer[30+1];
				// �g���b�N��
				StringCopyN(sBuffer, pTag->sTrackName, ID3_LEN_TRACK_NAME);
				sBuffer[ID3_LEN_TRACK_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetTrackName(pFileMP3, CString(sBuffer));
				// �A�[�e�B�X�g��
				StringCopyN(sBuffer, pTag->sArtistName, ID3_LEN_ARTIST_NAME);
				sBuffer[ID3_LEN_ARTIST_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetArtistName(pFileMP3, CString(sBuffer));
				// �A���o����
				StringCopyN(sBuffer, pTag->sAlbumName, ID3_LEN_ALBUM_NAME);
				sBuffer[ID3_LEN_ALBUM_NAME] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetAlbumName(pFileMP3, CString(sBuffer));
				// �����[�X�N��
				StringCopyN(sBuffer, pTag->sYear, ID3_LEN_YEAR);
				sBuffer[ID3_LEN_YEAR] = '\0';
				DeleteLineEndSpace(sBuffer);
				SetYear(pFileMP3, CString(sBuffer));
				// �R�����g
				if (IsID3Tag11(&tag) == true) {
					// ID3 tag Ver 1.1
					StringCopyN(sBuffer, pTag->sComment, ID3_LEN_COMMENT-2);
					sBuffer[28] = '\0';
					DeleteLineEndSpace(sBuffer);
					SetComment(pFileMP3, CString(sBuffer));
					// �g���b�N�ԍ�
					SetBTrackNumber(pFileMP3, pTag->byTrackNumber);
					if (GetBTrackNumber(pFileMP3) == 0x00) {
						SetBTrackNumber(pFileMP3, 0xff);
					}
					SetFormat(pFileMP3, nFileTypeMP3V11);	// �t�@�C���`���FMP3V1.1
					SetFileTypeName(pFileMP3, TEXT("MP3(ID3v1.1)"));
				} else {
					// ID3 tag Ver 1.0
					StringCopyN(sBuffer, pTag->sComment, ID3_LEN_COMMENT);
					sBuffer[30] = '\0';
					DeleteLineEndSpace(sBuffer);
					SetComment(pFileMP3, CString(sBuffer));
					// �g���b�N�ԍ�
					SetBTrackNumber(pFileMP3, 0xff);
					SetFormat(pFileMP3, nFileTypeMP3V1);	// �t�@�C���`���FMP3V1.0
					SetFileTypeName(pFileMP3, TEXT("MP3(ID3v1.0)"));
				}
				// �W�������ԍ�
				SetBGenre(pFileMP3, pTag->byGenre);
				SetGenre(pFileMP3, STEPGetGenreNameSIF(pTag->byGenre));
			} else {
				SetFileTypeName(pFileMP3, TEXT("MP3"));
				SetFormat(pFileMP3, nFileTypeMP3);	// �t�@�C���`���FMP3
			}

			// �t�@�C�������
			file.Close();
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format(TEXT("%s ���I�[�v���ł��܂���ł���"), sFileName);
		MessageBox(NULL, strMsg, TEXT("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return true ;
}

bool ReadTagID3v2(LPCTSTR sFileName, FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ID3v2 �^�O�̎擾
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v2.Load(sFileName) != ERROR_SUCCESS// �ǂݍ��ݎ��s
	||  id3v2.IsEnable() == FALSE			// ID3v2 �ł͂Ȃ�
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// ���Ή��̃o�[�W����
		// �ǂݍ��ݎ��s
		if (id3v2.GetVer() > 0x0000 && !TRUE/*id3v2.IsSafeVer()*/) {
			return(true);
		}
		return(false);
	}

	// *** �^�O���̐ݒ� ***
	SetTrackNameSI(pFileMP3, id3v2.GetTitle());		// �^�C�g��
	SetArtistNameSI(pFileMP3, id3v2.GetArtist());		// �A�[�e�B�X�g��
	SetAlbumNameSI(pFileMP3, id3v2.GetAlbum());		// �A���o����
	SetYearSI(pFileMP3, id3v2.GetYear());				// �����[�X
	SetCommentSI(pFileMP3, id3v2.GetComment());		// �R�����g
	SetGenreSI(pFileMP3, id3v2.GetGenre());			// �W��������
	SetTrackNumberSI(pFileMP3, id3v2.GetTrackNo());		// �g���b�N�ԍ�
	SetDiskNumberSI(pFileMP3, id3v2.GetDiskNo());		// �f�B�X�N�ԍ�
	SetSoftwareSI(pFileMP3, id3v2.GetEncoder());		// �G���R�[�_
	SetCopyrightSI(pFileMP3, id3v2.GetCopyright());	// ���쌠
	SetWriterSI(pFileMP3,id3v2.GetWriter());			// �쎌
	SetComposerSI(pFileMP3, id3v2.GetComposer());		// ���
	SetAlbumArtistSI(pFileMP3, id3v2.GetAlbmArtist());	// Albm. �A�[�e�B�X�g
	SetOrigArtistSI(pFileMP3, id3v2.GetOrigArtist());	// Orig.�A�[�e�B�X�g
	SetURLSI(pFileMP3, id3v2.GetUrl());				// URL
	SetEncodest(pFileMP3, id3v2.GetEncodest());			// �G���R�[�h�����l
	SetEngineerSI(pFileMP3,id3v2.GetEngineer());		// �G���W�j�A�i�o�Łj

//	SetFileTypeName(pFileMP3, "MP3(ID3v2)");
	setFileType(id3v2, pFileMP3);
	return(true);
}

bool ReadTagSIF(LPCTSTR sFileName, FILE_INFO *pFileMP3)
{
	CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// �ǂݍ��ݎ��s
		||  rmp.IsEnable() == FALSE) {
		// �ǂݍ��ݎ��s
		return(false);
	}

	// �ǂݍ��ݐ���
	// SI �t�B�[���h
	SetTrackNameSI(pFileMP3,	rmp.GetNAM());	// �Ȗ�
	SetArtistNameSI(pFileMP3,	rmp.GetART());	// �A�[�e�B�X�g��
	SetAlbumNameSI(pFileMP3,	rmp.GetPRD());	// ���i��
	SetCommentSI(pFileMP3,		rmp.GetCMT());	// �R�����g������
	SetYearSI(pFileMP3,			rmp.GetCRD());	// �����[�X�N��
	SetGenreSI(pFileMP3,		rmp.GetGNR());	// �W��������
	SetCopyrightSI(pFileMP3,	rmp.GetCOP());	// ���쌠
	SetEngineerSI(pFileMP3,		rmp.GetENG());	// �G���W�j�A
	SetSourceSI(pFileMP3,		rmp.GetSRC());	// �\�[�X
	SetSoftwareSI(pFileMP3,		rmp.GetSFT());	// �\�t�g�E�F�A
	SetKeywordSI(pFileMP3,		rmp.GetKEY());	// �L�[���[�h
	SetTechnicianSI(pFileMP3,	rmp.GetTCH());	// �Z�p��
	SetLyricSI(pFileMP3,		rmp.GetLYC());	// �̎�
	SetCommissionSI(pFileMP3,	rmp.GetCMS());	// �R�~�b�V����

	SetFormat(pFileMP3, nFileTypeRMP);		// �t�@�C���`���FRIFF MP3
	SetFileTypeName(pFileMP3, TEXT("RIFF MP3"));

	if (bOptRmpID3tagAutoWrite)
		SetGenreSI(pFileMP3, GetGenre(pFileMP3));

	return(true);
}

bool LoadFileMP3(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// === ID3v1.0/1.1 �`���t�@�C�� ===
	char	sHead[5] = {' ', ' ', ' ', ' ', '\0'};
	if (ReadTagID3(GetFullPath(pFileMP3), pFileMP3, &sHead[0]) == false) {
		// �ǂݍ��ݎ��s
		return false;
	}

	// === ID3v2 �`���t�@�C�� ===
	if (strncmp(sHead, "ID3", 3) == 0) {
		if (bOptAutoConvID3v2/*bOptID3v2ID3tagAutoWrite*/) {
			SetGenreSI(pFileMP3, GetGenre(pFileMP3));
			SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
			SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
		}
		if (ReadTagID3v2(GetFullPath(pFileMP3), pFileMP3) == false) {
			MessageBox(NULL, TEXT("ID3v2 �^�O�̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return false;
		}
	}
	else
	// === RIFF MP3 �`���t�@�C��(SI�t�B�[���h�̓ǂݍ���) ===
	if (strcmp(sHead, "RIFF") == 0) {
		if (ReadTagSIF(GetFullPath(pFileMP3), pFileMP3) == false) {
			MessageBox(NULL, TEXT("RIFF MP3 �^�O�̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
	}
	else
	// === ID3v1.0/1.1 �̂� ===
	{
		if (bOptAutoConvID3v2 || bOptAutoConvRMP/*bOptID3v2ID3tagAutoWrite*/) {
			SetGenreSI(pFileMP3, GetGenre(pFileMP3));
			SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
			SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
		}
	}

	// mp3infp�ɂ��擾
	GetValues_mp3infp(pFileMP3);

	return true;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_tcsicmp(szExt, TEXT("mp3")) == 0 || _tcsicmp(szExt, TEXT("rmp")) == 0) {
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
			// �t�@�C���̍Ō��128�o�C�g���擾
			file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			if (file.Read(&tag.byData[0], sizeof(ID3TAG)) != sizeof(ID3TAG)) {
				// �ǂݍ��ݎ��s
				return false;
			}
			if (IsID3Tag(&tag) == false) {
				// �t�@�C���̍Ō�ɃV�[�N
				file.Seek(0, CFile::end);
			} else {
				file.Seek(-(int)(sizeof(ID3TAG)), CFile::end);
			}

			// �X�y�[�X�Ŗ��߂�
			memset(&tag, 0x00/*0x20*/, sizeof(ID3TAG));

			ID3TAG_V11	*pTag = (ID3TAG_V11 *)&tag;
			pTag->sTAG[0] = 'T';
			pTag->sTAG[1] = 'A';
			pTag->sTAG[2] = 'G';

			int		nLen;
			CStringA text;

			// �g���b�N��
			text = GetTrackName(pFileMP3);
			nLen = (std::min)(strlen(text), ID3_LEN_TRACK_NAME);
			StringCopyN(&pTag->sTrackName[0], text, nLen, FALSE);

			// �A�[�e�B�X�g��
			text = GetArtistName(pFileMP3);
			nLen = (std::min)(strlen(text), ID3_LEN_ARTIST_NAME);
			StringCopyN(&pTag->sArtistName[0], text, nLen, FALSE);

			// �A���o����
			text = GetAlbumName(pFileMP3);
			nLen = (std::min)(strlen(text), ID3_LEN_ALBUM_NAME);
			StringCopyN(&pTag->sAlbumName[0], text, nLen, FALSE);

			// �����[�X�N��
			text = GetYear(pFileMP3);
			nLen = (std::min)(strlen(text), ID3_LEN_YEAR);
			StringCopyN(&pTag->sYear[0], text, nLen, FALSE);

			if (GetBTrackNumber(pFileMP3) == 0xff) {
				// ID3 tag Ver 1.0
				// �R�����g
				text = GetComment(pFileMP3);
				nLen = (std::min)(strlen(text), ID3_LEN_COMMENT);
				memset(&pTag->sComment[0], 0x00, ID3_LEN_COMMENT);
				StringCopyN2(&pTag->sComment[0], text, nLen, FALSE);

				// �Đݒ�
				SetFormat(pFileMP3, nFileTypeMP3V1);	// �t�@�C���`���FMP3V1.0
				SetFileTypeName(pFileMP3, TEXT("MP3(ID3v1.0)"));
			} else {
				// ID3 tag Ver 1.1
				// �R�����g
				text = GetComment(pFileMP3);
				nLen = (std::min)(strlen(text), ID3_LEN_COMMENT - 2);
				memset(&pTag->sComment[0], 0x00, ID3_LEN_COMMENT - 2);
				StringCopyN2(&pTag->sComment[0], text, nLen, FALSE);

				pTag->cZero = '\0';
				// �g���b�N�ԍ�
				pTag->byTrackNumber = GetBTrackNumber(pFileMP3);

				// �Đݒ�
				CString strTrackNumber;
				strTrackNumber.Format(TEXT("%d"), pTag->byTrackNumber);
				SetTrackNumber(pFileMP3, strTrackNumber);
				SetFormat(pFileMP3, nFileTypeMP3V11);	// �t�@�C���`���FMP3V1.1
				SetFileTypeName(pFileMP3, TEXT("MP3(ID3v1.1)"));
			}
			// �W������
			pTag->byGenre = GetBGenre(pFileMP3);
			SetGenre(pFileMP3, STEPGetGenreNameSIF(pTag->byGenre));

			file.Write(pTag, sizeof(ID3TAG));
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format(TEXT("%s ���I�[�v���ł��܂���ł���"), GetFullPath(pFileMP3));
		MessageBox(NULL, strMsg, TEXT("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return false;
	}
	END_CATCH

	return true;
}


bool ConvID3tagToSIField(FILE_INFO *pFileMP3)
{
	SetModifyFlag(pFileMP3, true);						// �ύX���ꂽ�t���O���Z�b�g
	SetTrackNameSI(pFileMP3, GetTrackName(pFileMP3));	// �Ȗ�
	SetArtistNameSI(pFileMP3, GetArtistName(pFileMP3));	// �A�[�e�B�X�g��
	SetAlbumNameSI(pFileMP3, GetAlbumName(pFileMP3));	// ���i��
	SetCommentSI(pFileMP3, GetComment(pFileMP3));		// �R�����g������
	SetYearSI(pFileMP3, GetYear(pFileMP3));				// �����[�X�N��

	auto LIMIT_TEXT_LENGTH = [pFileMP3] (FIELDTYPE strID3, std::size_t nLen) {
		CHAR sWorkBuffer[30 + 1];
		StringCopyN(sWorkBuffer, CStringA(GetValue(pFileMP3, strID3)), nLen);
		sWorkBuffer[nLen] = '\0';
		SetValue(pFileMP3, strID3, CString(sWorkBuffer));
	};

	// ID3 tag �̕������𒲐�(�����ϊ��ׂ̈̑Ώ�)
	LIMIT_TEXT_LENGTH(FIELD_TRACK_NAME		, ID3_LEN_TRACK_NAME);
	LIMIT_TEXT_LENGTH(FIELD_ARTIST_NAME		, ID3_LEN_ARTIST_NAME);
	LIMIT_TEXT_LENGTH(FIELD_ALBUM_NAME		, ID3_LEN_ALBUM_NAME);
	LIMIT_TEXT_LENGTH(FIELD_YEAR			, ID3_LEN_YEAR);
	if (GetBTrackNumber(pFileMP3) != (BYTE)0xff) {
		LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT-2);
	} else {
		LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT);
	}

	// �\�t�g�E�F�A�̐ݒ�
	UINT nFormat = GetFormat(pFileMP3);
	if (!(nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)) {
		if (lstrlen(GetSoftwareSI(pFileMP3)) == 0) {
			SetSoftwareSI(pFileMP3, strOptSoftwareTag);
		}
	}

	if (lstrlen(GetGenreSI(pFileMP3)) == 0 || false /* ��ɍĐݒ聨���ݒ莞�̂� */) {
		//SetGenreSI(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
		SetGenreSI(pFileMP3, GetGenre(pFileMP3));
	}
	if (lstrlen(GetTrackNumberSI(pFileMP3)) == 0) { /* ���ݒ莞�̂� */
		SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
	}
	if (lstrlen(GetDiskNumberSI(pFileMP3)) == 0) { /* ���ݒ莞�̂� */
		SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
	}
	return true;
}

void ConvSIFieldToID3tag(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (GetFormat(pFileMP3) != nFileTypeID3V2 && GetFormat(pFileMP3) != nFileTypeRMP) {
		return;
	}

	SetModifyFlag(pFileMP3, TRUE);				// �ύX���ꂽ�t���O���Z�b�g

	// �R�s�[�Ώۂ̃t�B�[���h���ǂ������`�F�b�N���āA�K�v�ȏꍇ�����R�s�[���܂�
	auto COPY_FIELD = [pFileMP3](auto strID3, CStringA strSIF, std::size_t len) {
		if ((nOptSIFieldConvType == SIF_CONV_ALL_FIELD) || (strlen(strSIF) <= len)) {
			CHAR sBuffer[256 + 1];
			StringCopyN(sBuffer, strSIF, len);
			sBuffer[len] = '\0';
			SetValue(pFileMP3, strID3, CString(sBuffer));
		}
	};

	auto COPY_FIELD2 = [pFileMP3](auto strID3, CStringA strSIF, std::size_t len) {
		if ((nOptSIFieldConvType == SIF_CONV_ALL_FIELD) || (strlen(strSIF) <= len)) {
			CHAR sBuffer[256 + 1];
			StringCopyN2(sBuffer, strSIF, len);
			sBuffer[len] = '\0';
			SetValue(pFileMP3, strID3, CString(sBuffer));
		}
	};

	// �Ȗ�
	COPY_FIELD(FIELD_TRACK_NAME, GetTrackNameSI(pFileMP3), ID3_LEN_TRACK_NAME);
	// �A�[�e�B�X�g��
	COPY_FIELD(FIELD_ARTIST_NAME, GetArtistNameSI(pFileMP3), ID3_LEN_ARTIST_NAME);
	// �A���o����
	COPY_FIELD(FIELD_ALBUM_NAME, GetAlbumNameSI(pFileMP3), ID3_LEN_ALBUM_NAME);
	// �g���b�N�ԍ� //�R�����g�ݒ����ɏ�������
	if (lstrlen(GetTrackNumberSI(pFileMP3)) == 0) {
		if (GetFormat(pFileMP3) == nFileTypeID3V2) {
			SetBTrackNumber(pFileMP3, (BYTE)0xff);
		}
	} else {
		SetBTrackNumber(pFileMP3, (BYTE)STEPGetIntegerTrackNumber(GetTrackNumberSI(pFileMP3)));
	}
	// �f�B�X�N�ԍ�
	if (lstrlen(GetDiskNumberSI(pFileMP3)) == 0) {
		if (GetFormat(pFileMP3) == nFileTypeID3V2) {
			SetBDiskNumber(pFileMP3, (BYTE)0xff);
		}
	} else {
		SetBDiskNumber(pFileMP3, (BYTE)STEPGetIntegerDiskNumber(GetDiskNumberSI(pFileMP3)));
	}
	// �R�����g������
	if (GetBTrackNumber(pFileMP3) == (BYTE)0xff) {
		// ID3 v1.0
		COPY_FIELD2(FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT);
	} else {
		// ID3 v1.1
		COPY_FIELD2(FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT-2);
	}
	// �����[�X�N��
	COPY_FIELD(FIELD_YEAR, GetYearSI(pFileMP3), ID3_LEN_YEAR);
	// �W������
	SetBGenre(pFileMP3, STEPGetGenreCode(GetGenreSI(pFileMP3)));
	SetGenre(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
#undef COPY_FIELD2

	return;
}

bool WriteTagID3v2(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ID3v2 �^�O�̎擾
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v2.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// �ǂݍ��ݎ��s
	||  id3v2.IsEnable() == FALSE			// ID3v2 �ł͂Ȃ�
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// ���Ή��̃o�[�W����
		// �ǂݍ��ݎ��s
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

	// ID3tag �̎����X�V
//	if (bOptID3v2ID3tagAutoWrite) {
//		ConvSIFieldToID3tag(pFileMP3);
//	}
	// �\�t�g�E�F�A�̐ݒ�
	if (lstrlen(GetSoftwareSI(pFileMP3)) == 0) {
		SetSoftwareSI(pFileMP3, strOptSoftwareTag);
	}

	// *** �^�O���̐ݒ� ***
	id3v2.SetTitle(GetTrackNameSI(pFileMP3));			// �^�C�g��
	id3v2.SetArtist(GetArtistNameSI(pFileMP3));			// �A�[�e�B�X�g��
	id3v2.SetAlbum(GetAlbumNameSI(pFileMP3));			// �A���o����
	id3v2.SetYear(GetYearSI(pFileMP3));					// �����[�X
	id3v2.SetEncoder(GetSoftwareSI(pFileMP3));			// �G���R�[�_
	id3v2.SetTrackNo(GetTrackNumberSI(pFileMP3));		// �g���b�N�ԍ�
	id3v2.SetDiskNo(GetDiskNumberSI(pFileMP3));			// �f�B�X�N�ԍ�
	id3v2.SetComment(GetCommentSI(pFileMP3));			// �R�����g
	id3v2.SetCopyright(GetCopyrightSI(pFileMP3));		// ���쌠
	id3v2.SetWriter(GetWriterSI(pFileMP3));				// �쎌
	id3v2.SetComposer(GetComposerSI(pFileMP3));			// ���
	id3v2.SetAlbmArtist(GetAlbumArtistSI(pFileMP3));		// Albm. �A�[�e�B�X�g
	id3v2.SetOrigArtist(GetOrigArtistSI(pFileMP3));		// Orig.�A�[�e�B�X�g
	id3v2.SetUrl(GetURLSI(pFileMP3));					// URL
	id3v2.SetEncodest(GetEncodest(pFileMP3));			// �G���R�[�h�����l
	id3v2.SetEngineer(GetEngineerSI(pFileMP3));			// �G���W�j�A�i�o�Łj
	// �W��������
	CString	strGenre;
	BYTE	byGenre;
	byGenre = STEPGetGenreCode(GetGenreSI(pFileMP3));
	if (!lstrlen(GetGenreSI(pFileMP3)) == 0) {
		if (byGenre == (BYTE)0xff || STEPIsUserGenre(GetGenreSI(pFileMP3)) || bOptID3v2GenreAddNumber == false) strGenre.Format(TEXT("%s"), GetGenreSI(pFileMP3));
		else                       strGenre.Format(TEXT("(%d)%s"), STEPGetGenreCode(GetGenreSI(pFileMP3)), GetGenreSI(pFileMP3));
	}
	id3v2.SetGenre(strGenre/* 2005.08.23 del , bOptID3v2GenreAddNumber*/);

	// *** �^�O�����X�V���� ***
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
	if (lstrlen(GetSoftwareSI(pFileMP3)) == 0) {
		SetSoftwareSI(pFileMP3, strOptSoftwareTag);
	}

	CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS	// �ǂݍ��ݎ��s
		||  rmp.IsEnable() == FALSE) {
		// �ǂݍ��ݎ��s
		return(false);
	}

	bool isNeedID3 = false;
	if (lstrlen(GetTrackName(pFileMP3))	> 0)	isNeedID3 = true;
	if (lstrlen(GetArtistName(pFileMP3))	> 0)	isNeedID3 = true;
	if (lstrlen(GetAlbumName(pFileMP3))	> 0)	isNeedID3 = true;
	if (lstrlen(GetComment(pFileMP3))	> 0)	isNeedID3 = true;
	if (lstrlen(GetYear(pFileMP3))		> 0)	isNeedID3 = true;
	if (lstrlen(GetGenre(pFileMP3))		> 0)	isNeedID3 = true;
	if (lstrlen(GetTrackNumber(pFileMP3))> 0)	isNeedID3 = true;
	if (isNeedID3 && !rmp.HasId3tag()) {
		rmp.SetHasId3tag(TRUE);
	}

	// ID3tag �̎����X�V
	//if (bOptRmpID3tagAutoWrite) {
	//	ConvSIFieldToID3tag(pFileMP3);
	//}

	// SI �t�B�[���h���X�V
	rmp.SetNAM(GetTrackNameSI(pFileMP3));	// �Ȗ�
	rmp.SetART(GetArtistNameSI(pFileMP3));	// �A�[�e�B�X�g��
	rmp.SetPRD(GetAlbumNameSI(pFileMP3));	// ���i��
	rmp.SetCMT(GetCommentSI(pFileMP3));		// �R�����g������
	rmp.SetCRD(GetYearSI(pFileMP3));		// �����[�X�N��
	rmp.SetGNR(GetGenreSI(pFileMP3));		// �W��������
	rmp.SetCOP(GetCopyrightSI(pFileMP3));	// ���쌠
	rmp.SetENG(GetEngineerSI(pFileMP3));	// �G���W�j�A
	rmp.SetSRC(GetSourceSI(pFileMP3));		// �\�[�X
	rmp.SetSFT(GetSoftwareSI(pFileMP3));	// �\�t�g�E�F�A
	rmp.SetKEY(GetKeywordSI(pFileMP3));		// �L�[���[�h
	rmp.SetTCH(GetTechnicianSI(pFileMP3));	// �Z�p��
	rmp.SetLYC(GetLyricSI(pFileMP3));		// �̎�
	rmp.SetCMS(GetCommissionSI(pFileMP3));	// �R�~�b�V����
	// SI �t�B�[���h���X�V
	bool result = rmp.Save(NULL, GetFullPath(pFileMP3)) == ERROR_SUCCESS ? true : false;
	if (result) {
		rmp.Release();
		if (isNeedID3) {
			result = WriteTagID3(pFileMP3);
		}
		SetFormat(pFileMP3, nFileTypeRMP);		// �t�@�C���`���FRIFF MP3
		SetFileTypeName(pFileMP3, TEXT("RIFF MP3"));
	}
	return result;
}

bool IsCreateID3v2(FILE_INFO *pFileMP3)
{
	if (lstrlen(GetTrackName(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (lstrlen(GetArtistName(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (lstrlen(GetAlbumName(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (lstrlen(GetComment(pFileMP3))	> ID3_LEN_COMMENT-2)	return true;
	if (lstrlen(GetYear(pFileMP3))		> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))					return true;
	if (CString(GetComment(pFileMP3)).Find('\n') > -1)			return true;
	if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))				return true;
	if (!STEPIsNumeric(GetDiskNumberSI(pFileMP3)))				return true;
	/*
	if (lstrlen(GetCopyrightSI(pFileMP3)) > 0)					return true;	// ���쌠
	if (lstrlen(GetComposerSI(pFileMP3)) > 0)					return true;	// ���
	if (lstrlen(GetOrigArtistSI(pFileMP3)) > 0)					return true;	// Orig.�A�[�e�B�X�g
	if (lstrlen(GetURLSI(pFileMP3)) > 0)							return true;	// URL
	if (lstrlen(GetEncodest(pFileMP3)) > 0)						return true;	// �G���R�[�h�����l
	if (lstrlen(GetSoftwareSI(pFileMP3)) > 0
		&& strcmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)		return true;	// �\�t�g�E�F�A
	*/
	return false;
}

bool IsCreateRMP(FILE_INFO *pFileMP3)
{
	if (lstrlen(GetTrackName(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (lstrlen(GetArtistName(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (lstrlen(GetAlbumName(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (lstrlen(GetComment(pFileMP3))	> ID3_LEN_COMMENT-2)	return true;
	if (lstrlen(GetYear(pFileMP3))		> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))					return true;
	/*
	if (lstrlen(GetCopyrightSI(pFileMP3))	> 0)	return true;
	rmp.SetENG(GetEngineerSI(pFileMP3));	// �G���W�j�A
	rmp.SetSRC(GetSourceSI(pFileMP3));		// �\�[�X
	rmp.SetSFT(GetSoftwareSI(pFileMP3));	// �\�t�g�E�F�A
	rmp.SetKEY(GetKeywordSI(pFileMP3));		// �L�[���[�h
	rmp.SetTCH(GetTechnicianSI(pFileMP3));	// �Z�p��
	rmp.SetLYC(GetLyricSI(pFileMP3));		// �̎�
	rmp.SetCMS(GetCommissionSI(pFileMP3));	// �R�~�b�V����
	*/

	return false;
}

bool IsCreateID3v2SI(FILE_INFO *pFileMP3, bool bID3v1Only = FALSE)
{
	if (lstrlen(GetTrackNameSI(pFileMP3))	> ID3_LEN_TRACK_NAME)	return true;
	if (lstrlen(GetArtistNameSI(pFileMP3))	> ID3_LEN_ARTIST_NAME)	return true;
	if (lstrlen(GetAlbumNameSI(pFileMP3))	> ID3_LEN_ALBUM_NAME)	return true;
	if (lstrlen(GetCommentSI(pFileMP3))		> ID3_LEN_COMMENT-2)	return true;
	if (lstrlen(GetYearSI(pFileMP3))		> ID3_LEN_YEAR)			return true;
	if (STEPIsUserGenre(GetGenreSI(pFileMP3)))						return true;
	if (CString(GetCommentSI(pFileMP3)).Find('\n') > -1)			return true;
	if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))					return true;
	if (!STEPIsNumeric(GetDiskNumberSI(pFileMP3)))					return true;
	if (bID3v1Only == TRUE)	return false;
	if (lstrlen(GetCopyrightSI(pFileMP3)) > 0)						return true;	// ���쌠
	if (lstrlen(GetComposerSI(pFileMP3)) > 0)						return true;	// ���
	if (lstrlen(GetOrigArtistSI(pFileMP3)) > 0)						return true;	// Orig.�A�[�e�B�X�g
	if (lstrlen(GetAlbumArtistSI(pFileMP3)) > 0)					return true;	// Albm.�A�[�e�B�X�g
	if (lstrlen(GetWriterSI(pFileMP3)) > 0)							return true;	// �쎌��
	if (lstrlen(GetURLSI(pFileMP3)) > 0)							return true;	// URL
	if (lstrlen(GetEncodest(pFileMP3)) > 0)							return true;	// �G���R�[�h�����l
	if (lstrlen(GetEngineerSI(pFileMP3)) > 0)						return true;	// �G���W�j�A
	if (lstrlen(GetSoftwareSI(pFileMP3)) > 0
		&& lstrcmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)	return true;	// �\�t�g�E�F�A
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
		// ID3tag�����X�V�̏ꍇ��SI�ɐݒ肳���̂ŃR�s�[
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
		// MP3 => RMP/ID3v2 �����t�H�[�}�b�g�ϊ�����
		if (bOptAutoConvID3v2) {
			if (IsCreateID3v2(pFileMP3) || IsCreateID3v2SI(pFileMP3)/* || !IsMatchID3SI(pFileMP3)*/) {
				// ID3tag�����X�V�̏ꍇ��SI�ɐݒ肳���̂ŃR�s�[
				copySIField(pFileMP3);
				extern bool WINAPI ConvFileFormatID3v2(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd);
				if (ConvFileFormatID3v2(pFileMP3, 0, NULL) == false) {
					return STEP_ERROR;
				} else {
					return STEP_SUCCESS; // �^�O��ϊ������ōX�V���Ă���̂ŏI��
				}
			} else {
				// ID3tag�����X�V�̏ꍇ��SI�ɐݒ肳���̂ŃR�s�[
				copySIField(pFileMP3);
			}
		} else if (bOptAutoConvRMP) {
			if (IsCreateRMP(pFileMP3) /*|| !IsMatchID3SI(pFileMP3)*/) {
				// ID3tag�����X�V�̏ꍇ��SI�ɐݒ肳���̂ŃR�s�[
				copySIField(pFileMP3);
				extern bool WINAPI ConvFileFormatRMP(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd);
				if (ConvFileFormatRMP(pFileMP3, 0, NULL) == false) {
					return STEP_ERROR;
				} else {
					return STEP_SUCCESS; // �^�O��ϊ������ōX�V���Ă���̂ŏI��
				}
			} else {
				// ID3tag�����X�V�̏ꍇ��SI�ɐݒ肳���̂ŃR�s�[
				copySIField(pFileMP3);
			}
		}
		if (WriteTagID3(pFileMP3) == false) { // �����ϊ����Ȃ��Ƃ�
			return STEP_ERROR;
		}
		ConvID3tagToSIField(pFileMP3);
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeID3V2) {
		// ID3tag �̎����X�V
		if (bOptID3v2ID3tagAutoWrite) {
			ConvSIFieldToID3tag(pFileMP3);
		}

		if (WriteTagID3(pFileMP3) == false) { // FileType�ݒ�̂���ID3v2�̑O�ɏ�������
			return STEP_ERROR;
		}
		if (bOptID3v2ID3tagAutoDelete) {
			SetFormat(pFileMP3, nFileTypeID3V2);
		}
		if (WriteTagID3v2(pFileMP3) == false) {
			return STEP_ERROR;
		}
		if (bOptID3v2ID3tagAutoDelete) {
			// ID3 tag���폜
			extern bool DeleteTagID3v1(LPCTSTR sFileName, HWND hWnd);
			DeleteTagID3v1(GetFullPath(pFileMP3), NULL);
		}
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeRMP) {
		// ID3tag �̎����X�V
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
	page.SetTitle(CString(STEPGetPluginName()) + " �I�v�V�����ݒ�");
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

		// INI�ɕۑ�
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("AutoConvID3v2"), bOptAutoConvID3v2 ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("AutoConvRMP"), bOptAutoConvRMP  ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("SIFieldConvType"), nOptSIFieldConvType == SIF_CONV_ALL_FIELD ? TEXT("0") : TEXT("1"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("ID3v2ID3tagAutoWrite"), bOptID3v2ID3tagAutoWrite ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("RmpID3tagAutoWrite"), bOptRmpID3tagAutoWrite ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("ID3v2GenreListSelect"), bOptID3v2GenreListSelect ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("RmpGenreListSelect"), bOptRmpGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("ID3v2ID3tagAutoDelete"), bOptID3v2ID3tagAutoDelete ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("ID3v2GenreAddNumber"), bOptID3v2GenreAddNumber ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("RMP_ID3V2"), TEXT("ChangeFileExt"), bOptChangeFileExt ? TEXT("1") : TEXT("0"), strINI);

		WritePrivateProfileString(TEXT("OTHER"), TEXT("SoftwareTag"), strOptSoftwareTag, strINI);

		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2NotUnSync"), bOptNotUnSyncAlways ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2UnSyncNew"), bOptUnSyncNew ? TEXT("1") : TEXT("0"), strINI);

		CString sValue;
		sValue.Format(TEXT("%d"), nId3v2Encode);
		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2CharEncode"), sValue, strINI);
		sValue.Format(TEXT("%d"), nId3v2Version);
		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2Version"), sValue, strINI);
		sValue.Format(TEXT("%d"), nId3v2EncodeNew);
		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2CharEncodeNew"), sValue, strINI);
		sValue.Format(TEXT("%d"), nId3v2VersionNew);
		WritePrivateProfileString(TEXT("OTHER"), TEXT("ID3v2VersionNew"), sValue, strINI);

		STEPUpdateCellInfo();
	}
}

STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		return TEXT("�W��MP3�`��/ID3v2�`���Ɏ����ϊ�");
	}
	if (nID == nIDFileConvMP3) {
		return TEXT("�W��MP3�`���ɕϊ�");
	}
	if (nID == nIDFileConvID3v2) {
		return TEXT("ID3v2�`���ɕϊ�");
	}
	if (nID == nIDFileConvRMP) {
		return TEXT("RIFF MP3�`���ɕϊ�");
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		return TEXT("SI/ID3v2�t�B�[���h����ID3tag�ɕϊ�");
//	}
	if (nID == nIDDeleteID3) {
		return TEXT("ID3tag���폜");
	}
	if (nID == nIDConvID3v2Version) {
		return TEXT("ID3v2�o�[�W�����̕ϊ�");
	}
	return NULL;
}

STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDFileConvAutoID3) {
		return TEXT("�^�O�̓��͌���/���e�ɂ���ĕW��MP3�`���܂���ID3v2�`���̂����ꂩ�ɕϊ����܂�");
	}
	if (nID == nIDFileConvMP3) {
		return TEXT("�I������Ă���t�@�C����W��MP3�`���ɕϊ����܂�");
	}
	if (nID == nIDFileConvID3v2) {
		return TEXT("�I������Ă���t�@�C����ID3v2�`���ɕϊ����܂�");
	}
	if (nID == nIDFileConvRMP) {
		return TEXT("�I������Ă���t�@�C����RIFF MP3�`���ɕϊ����܂�");
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		return TEXT("SI/ID3v2�t�B�[���h����ID3tag�ɕϊ����܂�");
//	}
	if (nID == nIDDeleteID3) {
		return TEXT("�I������Ă���MP3�t�@�C����ID3tag���폜���܂�");
	}
	if (nID == nIDConvID3v2Version) {
		return TEXT("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�");
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
		static LPCTSTR sMessage = TEXT("�I������Ă���t�@�C���� �W��MP3 �`�� �܂��� ID3v2 �`���ɕϊ����܂�\n\n"
									   "�ϊ������s���Ă���낵���ł����H");
		if (/*g_bConfConvID3v2 == false || */MessageBox(hWnd, sMessage, TEXT("�W��MP3 �`��/ID3v2 �`���ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatAuto(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate(TEXT("�W��MP3�`��/ID3v2 �`���ɕϊ���....."), ConvFileFormatAuto);
		}
		return true;
	}
	if (nID == nIDFileConvMP3) {
		static LPCTSTR sMessage = TEXT("�I������Ă���t�@�C����W��MP3 �`���ɕϊ����܂�\n"
									   "�W��MP3 �`���ɕϊ�����ƁA�^�O���̈ꕔ�������\��������܂�\n\n"
									   "�ϊ������s���Ă���낵���ł����H");
		if (/*g_bConfConvMP3 == false || */MessageBox(hWnd, sMessage, TEXT("�W��MP3 �`���ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatMP3(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate(TEXT("�W��MP3 �`���ɕϊ���....."), ConvFileFormatMP3);
		}
		return true;
	}
	if (nID == nIDFileConvID3v2) {
		static LPCTSTR sMessage = TEXT("�I������Ă���t�@�C���� ID3v2 �`���ɕϊ����܂�\n\n"
									   "�ϊ������s���Ă���낵���ł����H");
		if (/*g_bConfConvID3v2 == false || */MessageBox(hWnd, sMessage, TEXT("ID3v2 �`���ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatID3v2(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate(TEXT("ID3v2 �`���ɕϊ���....."), ConvFileFormatID3v2);
		}
		return true;
	}
	if (nID == nIDFileConvRMP) {
		static LPCTSTR sMessage = TEXT("�I������Ă���t�@�C���� RIFF MP3 �`���ɕϊ����܂�\n\n"
									   "�ϊ������s���Ă���낵���ł����H");
		if (/*g_bConfConvRMP == false || */MessageBox(hWnd, sMessage, TEXT("RIFF MP3 �`���ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI ConvFileFormatRMP(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate(TEXT("RIFF MP3 �`���ɕϊ���....."), ConvFileFormatRMP);
		}
		return true;
	}
//	if (nID == nIDConvSIFieldToID3Tag) {
//		extern bool WINAPI ConvSiFieldToTd3tag(FILE_INFO*, int, HWND);
//		STEPProcessSelectedFilesForUpdate("SI/ID3v2�t�B�[���h����ID3tag�ɕϊ���.....", ConvSiFieldToTd3tag);
//		return true;
//	}
	if (nID == nIDDeleteID3) {
		static LPCTSTR sMessage = TEXT("�I������Ă���t�@�C����ID3v1�^�O���폜���܂�\n\n"
									   "���s���Ă���낵���ł����H");
		if (MessageBox(hWnd, sMessage, TEXT("ID3tag �폜"), MB_YESNO|MB_TOPMOST) == IDYES) {
			extern bool WINAPI DeleteId3tag(FILE_INFO*, int, HWND);
			STEPProcessSelectedFilesForUpdate(TEXT("ID3tag ���폜��....."), DeleteId3tag);
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
			STEPProcessSelectedFilesForUpdate(TEXT("ID3v2 �o�[�W����/�����G���R�[�h��ϊ���....."), ConvID3v2Version);
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
	// ���j���[�ւ̒ǉ�
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