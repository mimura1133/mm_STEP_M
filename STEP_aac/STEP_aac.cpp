// STEP_aac.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_aac.h"
#include "STEPlugin.h"

#include "DlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CSTEP_aacApp

BEGIN_MESSAGE_MAP(CSTEP_aacApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_aacApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_aacApp �̍\�z

CSTEP_aacApp::CSTEP_aacApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_aacApp �I�u�W�F�N�g

CSTEP_aacApp theApp;

UINT nPluginID;
UINT nFileTypeMP4;
UINT nFileTypeM4V;
UINT nFileTypeM4A;

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TEXT("Version 1.00 Copyright (C) 2004-2005 haseta\r\nVersion 1.02M Copyright (C) 2008-2010 Mimura\r\nMP4(mp4,m4v,m4a)�`�����T�|�[�g���Ă��܂�\r\n�^�O�X�V��mp4v2.dll���g�p���Ă��܂����ASTEP�p�ɃJ�X�^�}�C�Y�������̂ł�");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += "STEP_aac.ini";
	bOptGenreListSelect = GetPrivateProfileInt(TEXT("AAC"), TEXT("GenreListSelect"), 0, strINI) ? true : false;

	HBITMAP hM4VBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
	HBITMAP hM4ABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
	HBITMAP hMP4Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
	nFileTypeM4A = STEPRegisterExt(nPluginID, TEXT("m4a"), hM4ABitmap);
	nFileTypeMP4 = STEPRegisterExt(nPluginID, TEXT("mp4"), hMP4Bitmap);
	nFileTypeM4V = STEPRegisterExt(nPluginID, TEXT("m4v"), hM4VBitmap);
	DeleteObject(hM4VBitmap);
	DeleteObject(hM4ABitmap);
	DeleteObject(hMP4Bitmap);

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
	return TEXT("STEP_aac");
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
	case COLUMN_COPYRIGHT:
	case COLUMN_COMPOSER:
	case COLUMN_KEYWORD:
	case COLUMN_SOFTWARE:
	case COLUMN_ALBM_ARTIST:
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
	switch (nColumn) { // iTunes�ɏ���
	case COLUMN_TRACK_NAME:		return 260;
	case COLUMN_ARTIST_NAME:	return 260;
	case COLUMN_ALBUM_NAME:		return 260;
	case COLUMN_TRACK_NUMBER:	return 7;
	case COLUMN_DISK_NUMBER:	return 7;
	case COLUMN_YEAR:			return 260;
	case COLUMN_GENRE:			return 260;
	case COLUMN_COMMENT:		return 260;
	case COLUMN_COPYRIGHT:		return 260;
	case COLUMN_COMPOSER:		return 260;
	case COLUMN_KEYWORD:		return 260;
	case COLUMN_SOFTWARE:		return 260;
	case COLUMN_ALBM_ARTIST:	return 260;
	}
	return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ((_tcsicmp(szExt, TEXT("m4a")) == 0)|(_tcsicmp(szExt, TEXT("m4v")) == 0)|(_tcsicmp(szExt, TEXT("mp4")) == 0)) {
		extern	bool LoadFileAAC(FILE_INFO *pFile);
		if (LoadFileAAC(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AAC�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			if(_tcsicmp(szExt, TEXT("m4a")) == 0){
				SetFormat(pFileMP3, nFileTypeM4A);
				SetFileTypeName(pFileMP3, TEXT("MP4(Audio)"));
				return STEP_SUCCESS;
			}
			if(_tcsicmp(szExt, TEXT("m4v")) == 0){
				SetFormat(pFileMP3, nFileTypeM4V);
				SetFileTypeName(pFileMP3, TEXT("MP4(Video)"));
				return STEP_SUCCESS;
			}
			if(_tcsicmp(szExt, TEXT("mp4")) == 0){
				SetFormat(pFileMP3, nFileTypeMP4);
				SetFileTypeName(pFileMP3, TEXT("MP4"));
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

	if ((nFormat == nFileTypeM4A)|(nFormat == nFileTypeM4V)|(nFormat == nFileTypeMP4)) {
		extern bool WriteFileAAC(FILE_INFO *pFileMP3);
		if (WriteFileAAC(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AAC�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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

		WritePrivateProfileString(TEXT("AAC"), TEXT("GenreListSelect"), bOptGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
	}
}

STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_KEYWORD:
		return TEXT("����");
	case COLUMN_TRACK_NUMBER:
		return TEXT("TrackNo/Total");
	case COLUMN_DISK_NUMBER:
		return TEXT("DiskNo/Total");
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}
