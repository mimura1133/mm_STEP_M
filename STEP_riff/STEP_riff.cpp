// STEP_riff.cpp : DLL �p�̏����������̒�`���s���܂��B
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
// CSTEP_riffApp

BEGIN_MESSAGE_MAP(CSTEP_riffApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_riffApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_riffApp �̍\�z

CSTEP_riffApp::CSTEP_riffApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_riffApp �I�u�W�F�N�g

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
	return TEXT("Version 1.02 Copyright (C) 2003-2006 haseta\r\nRIFF WAV/AVI�`�����T�|�[�g���Ă��܂�");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
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
			strMsg.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("WAV�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
			strMsg.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AVI�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
			strMsg.Format(TEXT("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("WAV�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	if (nFormat == nFileTypeAVI) {
		if (WriteAttributeFileWAV(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(TEXT("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, TEXT("AVI�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
	page.SetTitle(CString(STEPGetPluginName()) + " �I�v�V�����ݒ�");
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
		return "URL(�֘A)";
	case COLUMN_ENCODEST:		// �G���R�[�h�����l
	case COLUMN_OTHER:			// ���̑�
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
