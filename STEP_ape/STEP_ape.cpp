// STEP_ape.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_ape.h"
#include "STEPlugin.h"

#include "Tag_Ape.h"

#include "DlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
extern void Free_mp3infp();

#define ID3_LEN_TRACK_NAME		30		// �g���b�N��    (������c30BYTE)
#define ID3_LEN_ARTIST_NAME		30		// �A�[�e�B�X�g��(������c30BYTE)
#define ID3_LEN_ALBUM_NAME		30		// �A���o����    (������c30BYTE)
#define ID3_LEN_COMMENT			30		// �R�����g      (������c30BYTE)
#define ID3_LEN_YEAR			4		// �����[�X�N��  (������c 4BYTE)

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
// CSTEP_apeApp

BEGIN_MESSAGE_MAP(CSTEP_apeApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_apeApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_apeApp �̍\�z

CSTEP_apeApp::CSTEP_apeApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_apeApp �I�u�W�F�N�g

CSTEP_apeApp theApp;

UINT nPluginID;
UINT nFileTypeAPE;
UINT nFileTypeAPEID3; /* �^�O��ID3�`�� */

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TEXT("Version 1.01 Copyright (C) 2003-2006 haseta\r\nMonkey's Audio�`�����T�|�[�g���Ă��܂�");
}


STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += "STEP_ape.ini";
	bOptGenreListSelect = GetPrivateProfileInt(TEXT("APE"), TEXT("GenreListSelect"), 0, strINI) ? true : false;

	HBITMAP hAPEBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_APE));
	nFileTypeAPE = STEPRegisterExt(nPluginID, TEXT("ape"), hAPEBitmap);
	nFileTypeAPEID3 = STEPRegisterExt(nPluginID, TEXT("ape"), hAPEBitmap);
	DeleteObject(hAPEBitmap);

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
	return TEXT("STEP_ape");
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
	case COLUMN_WRITER:
	case COLUMN_COMPOSER:
	case COLUMN_ORIG_ARTIST:
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
	if (nFormat == nFileTypeAPE || true) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		return 1024;
		case COLUMN_ARTIST_NAME:	return 1024;
		case COLUMN_ALBUM_NAME:		return 1024;
		case COLUMN_TRACK_NUMBER:	return 1024;
		case COLUMN_YEAR:			return 1024;
		case COLUMN_GENRE:			return 1024;
		case COLUMN_COMMENT:		return 1024;
		case COLUMN_WRITER:			return 1024;
		case COLUMN_COMPOSER:		return 1024;
		case COLUMN_ORIG_ARTIST:	return 1024;
		}
	/*
	} else if (nFormat == nFileTypeAPEID3) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		return ID3_LEN_TRACK_NAME;
		case COLUMN_ARTIST_NAME:	return ID3_LEN_ARTIST_NAME;
		case COLUMN_ALBUM_NAME:		return ID3_LEN_ALBUM_NAME;
		case COLUMN_TRACK_NUMBER:	return 3;
		case COLUMN_YEAR:			return ID3_LEN_YEAR;
		case COLUMN_GENRE:			return 3;
		case COLUMN_COMMENT:		return ID3_LEN_COMMENT;
		}
	*/
	}
	return 0;
}

bool ReadTagAPE(FILE_INFO *pFileMP3)
{
	GetValues_mp3infp(pFileMP3);
	CTag_Ape ape;
	if (ape.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}

	
	CString buff;
	// �g���b�N��
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_TITLE, buff);
	SetTrackNameSI(pFileMP3, buff);
	// �A�[�e�B�X�g��
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_ARTIST, buff);
	SetArtistNameSI(pFileMP3, buff);
	// �A���o����
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_ALBUM, buff);
	SetAlbumNameSI(pFileMP3, buff);
	// �����[�X�N��
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_YEAR, buff);
	SetYearSI(pFileMP3, buff);
	// �R�����g
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_COMMENT, buff);
	SetCommentSI(pFileMP3, buff);
	// �g���b�N�ԍ�
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_TRACK, buff);
	SetTrackNumberSI(pFileMP3, buff);
	// �W�������ԍ�
	ape.GetComment(CTag_Ape::APE_TAG_FIELD_GENRE, buff);
	SetGenreSI(pFileMP3, buff);
	//SetBGenre(STEPGetGenreCode(buff));
	// ��Ȏ�
	ape.GetComment("Composer", buff);
	SetComposerSI(pFileMP3, buff);
	// �p�t�H�[�}�[
	ape.GetComment("Performer", buff);
	SetOrigArtistSI(pFileMP3, buff);
	// �t�@�C���`���FAPE
	SetFileTypeName(pFileMP3, TEXT("Monkey's Audio"));
	if (!ape.HasApetag() && ape.HasId3tag()) {
		SetFormat(pFileMP3, nFileTypeAPEID3);
		SetFileTypeName(pFileMP3, TEXT("Monkey's Audio(ID3)"));
	} else {
		SetFormat(pFileMP3, nFileTypeAPE);
		if (ape.HasApetag()) {
			if (ape.isApetagV1()) {
				SetFileTypeName(pFileMP3, TEXT("Monkey's Audio(APE)"));
			} else {
				SetFileTypeName(pFileMP3, TEXT("Monkey's Audio(APEv2)"));
			}
		}
	}

	return true;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_tcsicmp(szExt, TEXT("ape")) == 0) {
		if (ReadTagAPE(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("Monkey's Audio�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			return STEP_SUCCESS;
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

bool WriteTagAPE(FILE_INFO *pFileMP3)
{
	CTag_Ape ape;
	if (ape.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}
	//ape.Release();
	// �g���b�N��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_TITLE, GetTrackNameSI(pFileMP3));
	// �A�[�e�B�X�g��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_ARTIST, GetArtistNameSI(pFileMP3));
	// �A���o����
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_ALBUM, GetAlbumNameSI(pFileMP3));
	// �����[�X�N��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_YEAR, GetYearSI(pFileMP3));
	// �R�����g
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_COMMENT, GetCommentSI(pFileMP3));
	// �g���b�N�ԍ�
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_TRACK, GetTrackNumberSI(pFileMP3));
	// �W�������ԍ�
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_GENRE, GetGenreSI(pFileMP3));
	// ��Ȏ�
	ape.SetComment("Composer", GetComposerSI(pFileMP3));
	//  �p�t�H�[�}�[
	ape.SetComment("Performer", GetOrigArtistSI(pFileMP3));

	if (ape.Save(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}

	return true;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if (nFormat == nFileTypeAPE || nFormat == nFileTypeAPEID3) {
		if (WriteTagAPE(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("Monkey's Audio�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
	page.SetTitle(CString(STEPGetPluginName()) + " �I�v�V�����ݒ�");
	if (page.DoModal() == IDOK) {
		bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;

		WritePrivateProfileString(TEXT("APE"), TEXT("GenreListSelect"), bOptGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
	}
}

STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_ORIG_ARTIST:
		return TEXT("�p�t�H�[�}�[");
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}
