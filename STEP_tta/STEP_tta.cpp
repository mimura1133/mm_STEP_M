// STEP_tta.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_tta.h"
#include "STEPlugin.h"

#include "DlgSetup.h"
#include "id3tag.h"

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
// CSTEP_ttaApp

BEGIN_MESSAGE_MAP(CSTEP_ttaApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_ttaApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_ttaApp �̍\�z

CSTEP_ttaApp::CSTEP_ttaApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_ttaApp �I�u�W�F�N�g

CSTEP_ttaApp theApp;

CString strINI;
bool bOptGenreListSelect;
bool bOptID3TagAutoWrite;
bool bOptID3TagAutoDelete;

UINT nPluginID;
UINT nFileTypeTTA;

// STEP - TTA�ł͔񓯊������Ȃ�
bool bOptNotUnSyncAlways = true;
bool bOptUnSyncNew = true;
//CString strINI;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TEXT("Version 1.00 Copyright (C) 2005-2006 haseta\r\nTTA�`�����T�|�[�g���Ă��܂�");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += "STEP_tta.ini";
	bOptGenreListSelect = GetPrivateProfileInt(TEXT("TTA"), TEXT("GenreListSelect"), 0, strINI) ? true : false;
	bOptID3TagAutoWrite = GetPrivateProfileInt(TEXT("TTA"), TEXT("ID3TagAutoWrite"), 0, strINI) ? true : false;
	bOptID3TagAutoDelete = GetPrivateProfileInt(TEXT("TTA"), TEXT("ID3TagAutoDelete"), 0, strINI) ? true : false;

	HBITMAP hTTABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_TTA));
	nFileTypeTTA = STEPRegisterExt(nPluginID, TEXT("tta"), hTTABitmap);
	DeleteObject(hTTABitmap);

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
	return TEXT("STEP_tta");
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
	if (!isEditSIF && bOptID3TagAutoWrite)	return _NULL;

	switch (nColumn) {
	case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
	case COLUMN_ALBUM_NAME:
	case COLUMN_YEAR:
	case COLUMN_TRACK_NUMBER:
		return _EDIT;
	case COLUMN_GENRE:
		if (bOptGenreListSelect) {
			return _CBOX;
		} else {
			return _EDIT;
		}
	case COLUMN_COMMENT:
		if (isEditSIF) {
			return _MEDIT;
		} else {
			return _EDIT;
		}
	}
	return _NULL;
}


STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:
			return 30;
		case COLUMN_ARTIST_NAME:
			return 30;
		case COLUMN_ALBUM_NAME:
			return 30;
		case COLUMN_TRACK_NUMBER:
			return 3;
		case COLUMN_YEAR:
			return 5;
		case COLUMN_GENRE:
			return 128;
		case COLUMN_COMMENT:
			return 28;
		default:
			return 0;
		}
	} else {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:
			return 1024;
		case COLUMN_ALBUM_NAME:
			return 1024;
		case COLUMN_ARTIST_NAME:
			return 1024;
		case COLUMN_COMMENT:
			return 1024;
		case COLUMN_YEAR:
			return 256;
		case COLUMN_TRACK_NUMBER:
			return 256;
		case COLUMN_GENRE:
			return 256;
		}
	}
	return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_tcsicmp(szExt, TEXT("tta")) == 0) {
		extern	bool LoadAttributeFileTTA(FILE_INFO *pFile);
		if (LoadAttributeFileTTA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("TTA�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			SetFormat(pFileMP3, nFileTypeTTA);
			SetFileTypeName(pFileMP3, TEXT("True Audio"));
			return STEP_SUCCESS;
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if (nFormat == nFileTypeTTA) {
		extern bool WriteAttributeFileTTA(FILE_INFO *pFileMP3);
		if (WriteAttributeFileTTA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("TTA�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
	dlg1.m_bID3TagAutoDelete = bOptID3TagAutoDelete;
	dlg1.m_bID3TagAutoWrite = bOptID3TagAutoWrite;
	page.AddPage(&dlg1);
	page.SetTitle(CString(STEPGetPluginName()) + " �I�v�V�����ݒ�");
	if (page.DoModal() == IDOK) {
		bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;
		bOptID3TagAutoDelete = dlg1.m_bID3TagAutoDelete ? true : false;
		bOptID3TagAutoWrite = dlg1.m_bID3TagAutoWrite ? true : false;

		WritePrivateProfileString(TEXT("TTA"), TEXT("GenreListSelect"), bOptGenreListSelect ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("TTA"), TEXT("ID3TagAutoDelete"), bOptID3TagAutoDelete ? TEXT("1") : TEXT("0"), strINI);
		WritePrivateProfileString(TEXT("TTA"), TEXT("ID3TagAutoWrite"), bOptID3TagAutoWrite ? TEXT("1") : TEXT("0"), strINI);
		STEPUpdateCellInfo();
	}
}

STEP_API bool WINAPI STEPOnConvSiFieldToId3tag(FILE_INFO* pFileInfo)
{
	return true;
}