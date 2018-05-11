#include "stdafx.h"

#define APSTUDIO_INVOKED
#include "resource.h"
#undef APSTUDIO_INVOKED

#include "STEP_api.h"

#include "DkToolBar.h"
#include "SuperTagEditor.h"
#include "FileMP3.h"
#include "MySuperGrid.h"
#include "SuperTagEditorDoc.h"

#include "Plugin.h"

#include "INI/Profile.h"

CDkToolBar* STEP_wndToolBar;
CImageList* STEP_ImageList;
HWND STEP_hWnd;
CMySuperGrid* STEP_List;

UINT nCommandID = _APS_NEXT_COMMAND_VALUE;
UINT nFileTypeID = 1;
CUIntArray arFormatType; // FILE_MP3.nFormat�ɐݒ肳���l�̃��X�g

extern "C" {
extern STEP_API UINT WINAPI STEPGetCommandID(void);
extern STEP_API HMENU WINAPI STEPGetMenu(UINT);
extern STEP_API void WINAPI STEPAddToolBarButton(HBITMAP, UINT, LPCTSTR);
extern STEP_API UINT WINAPI STEPRegisterExt(UINT, LPCTSTR, HBITMAP);
extern STEP_API LPCTSTR WINAPI STEPGetGenreNameSIF(BYTE byGenre);
extern STEP_API BYTE WINAPI STEPGetGenreCode(LPCTSTR szGenre);
extern STEP_API bool WINAPI STEPIsUserGenre(LPCTSTR szGenre);
extern STEP_API int WINAPI STEPGetNumericTrackNumber(const char* szTrackNumber, LPTSTR szNumericNumber, int size); /* STEP 037 */
extern STEP_API int WINAPI STEPGetIntegerTrackNumber(const char* szTrackNumber); /* STEP 037 */
extern STEP_API bool WINAPI STEPIsNumeric(const char* szText); /* STEP 037 */
extern STEP_API void WINAPI STEPConvSiFieldToId3tag(FILE_INFO* pFileInfo); /* STEP 037 */
}

CPlugin plugins;

PSTEPlugin STEPluginLoadFile(LPCTSTR strPluginFile)
{
	HINSTANCE hLib;
	CString strPluginFolder;
	{
		auto pApp = (CSuperTagEditorApp*)AfxGetApp();
		auto szName = pApp->MakeFileName(TEXT(""), TEXT(""));
		TCHAR full[_MAX_PATH] = {};
		if (PathIsRelative(strPluginFile)) {
			TCHAR buff[_MAX_PATH] = {};
			PathCombine(buff, szName.c_str(), strPluginFile);
			if (!PathCanonicalize(full, buff)) {
				return nullptr;
			}
		} else {
			if (!PathCanonicalize(full, strPluginFile)) {
				return nullptr;
			}
		}
		
#ifndef DEBUG
		{
			// exe�O�̊O�ɂ���t�@�C����ǂ܂Ȃ��悤�Ƀ`�F�b�N
			TCHAR buff[_MAX_PATH];
			if (!PathRelativePathTo(buff, szName.c_str(), FILE_ATTRIBUTE_DIRECTORY, full, 0)) {
				return nullptr;
			}

			if (_tcsstr(buff, TEXT(".."))) {
				return nullptr;
			}
		}
#endif
		{
			TCHAR buff[_MAX_PATH] = {};
			lstrcpy(buff, full);
			PathRemoveFileSpec(buff);
			PathAddBackslash(buff);
			strPluginFolder = buff;
		}

		// https://support.microsoft.com/help/2389418/secure-loading-of-libraries-to-prevent-dll-preloading-attacks
		SetDllDirectory(TEXT(""));
		hLib = LoadLibraryEx(full, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
		if (!hLib) {
			return nullptr;
		}
	}

	UINT (WINAPI *STEPGetAPIVersion)(void);
	bool (WINAPI *STEPInit)(UINT, LPCTSTR);
	LPCTSTR (WINAPI *STEPGetPluginName)(void);

	(FARPROC&)STEPInit = GetProcAddress(hLib, "_STEPInit@8");
	(FARPROC&)STEPGetAPIVersion = GetProcAddress(hLib, "_STEPGetAPIVersion@0");
	(FARPROC&)STEPGetPluginName = GetProcAddress(hLib, "_STEPGetPluginName@0");
	if (STEPInit == NULL || STEPGetAPIVersion == NULL || STEPGetPluginName == NULL)	return  NULL;
	if ((STEPGetAPIVersion() & 0xFFFF00) > (STEP_API_VERSION & 0xFFFF00))	return NULL; /* STEP 029 */
	PSTEPlugin pPlugin = new STEPlugin;
	pPlugin->hLib = hLib;
	pPlugin->sFileName = strPluginFile;
	void const* const pluginName = STEPGetPluginName();
	pPlugin->sPluginName = (LPCTSTR)pluginName;
	(FARPROC&)pPlugin->STEPGetToolTipText = GetProcAddress(hLib, "_STEPGetToolTipText@4");
	(FARPROC&)pPlugin->STEPGetStatusMessage = GetProcAddress(hLib, "_STEPGetStatusMessage@4");
	(FARPROC&)pPlugin->STEPOnUpdateCommand = GetProcAddress(hLib, "_STEPOnUpdateCommand@4");
	(FARPROC&)pPlugin->STEPOnCommand = GetProcAddress(hLib, "_STEPOnCommand@8");
	(FARPROC&)pPlugin->STEPSupportSIF = GetProcAddress(hLib, "_STEPSupportSIF@4");
	(FARPROC&)pPlugin->STEPSupportTrackNumberSIF = GetProcAddress(hLib, "_STEPSupportTrackNumberSIF@4");
	(FARPROC&)pPlugin->STEPSupportGenreSIF = GetProcAddress(hLib, "_STEPSupportGenreSIF@4");
	(FARPROC&)pPlugin->STEPGetControlType = GetProcAddress(hLib, "_STEPGetControlType@12");
	(FARPROC&)pPlugin->STEPGetColumnMax = GetProcAddress(hLib, "_STEPGetColumnMax@12");
	(FARPROC&)pPlugin->STEPGetColumnName = GetProcAddress(hLib, "_STEPGetColumnName@8");
	(FARPROC&)pPlugin->STEPHasSpecificColumnName = GetProcAddress(hLib, "_STEPHasSpecificColumnName@4");
	(FARPROC&)pPlugin->STEPLoad = GetProcAddress(hLib, "_STEPLoad@8");
	(FARPROC&)pPlugin->STEPSave = GetProcAddress(hLib, "_STEPSave@4");
	(FARPROC&)pPlugin->STEPShowOptionDialog = GetProcAddress(hLib, "_STEPShowOptionDialog@4");
	(FARPROC&)pPlugin->STEPOnLoadMenu = GetProcAddress(hLib, "_STEPOnLoadMenu@8");
	(FARPROC&)pPlugin->STEPOnLoadMainMenu = GetProcAddress(hLib, "_STEPOnLoadMainMenu@0");
	(FARPROC&)pPlugin->STEPGetPluginInfo = GetProcAddress(hLib, "_STEPGetPluginInfo@0");
	(FARPROC&)pPlugin->STEPInitFileSpecificInfo = GetProcAddress(hLib, "_STEPInitFileSpecificInfo@4");
	(FARPROC&)pPlugin->STEPOnConvSiFieldToId3tag = GetProcAddress(hLib, "_STEPOnConvSiFieldToId3tag@4");

	// STEPGetPluginName()���A���t�@�x�b�g�݂̂�Ԃ����Ƃ�O��Ƃ���
	// 2Byte�ڂ� 0x00 ���ǂ����Ŗ{�̂ƃv���O�C����Unicode�w�肪��v���Ă��邩�𔻒肷��
#ifdef UNICODE
	if (static_cast<char const* const>(pluginName)[1] != 0) {
		FreeLibrary(pPlugin->hLib);
		delete pPlugin;
		return nullptr;
	}
#else
	if (static_cast<char const* const>(pluginName)[1] == 0) {
		FreeLibrary(pPlugin->hLib);
		delete pPlugin;
		return nullptr;
	}
#endif

	plugins.arPlugins.Add(pPlugin);

	if ((*STEPInit)(plugins.arPlugins.GetSize(), strPluginFolder) == false) {
		return NULL;
	}

	return pPlugin;
}

void STEPluginLoad(HWND hWnd) {
	STEP_hWnd = hWnd;

	CSuperTagEditorApp	*pApp = (CSuperTagEditorApp *)AfxGetApp();
	CString strINI;
	{
		auto	szName = pApp->MakeFileName(TEXT("Plugin"), TEXT("ini"));
		strINI = szName.c_str();
		BOOL isExists = Profile_Initialize(strINI, TRUE);
		Profile_Free();
		if (!isExists) {
			TCHAR   drive[_MAX_DRIVE];
			TCHAR   dir[_MAX_DIR];
			TCHAR   buff[_MAX_PATH] = { '\0' };
			_tsplitpath(szName.c_str(), drive, dir, NULL, NULL);
			_tmakepath_s(buff, _MAX_PATH, drive, dir, TEXT("DefaultPlugin"), TEXT("ini"));
			strINI = buff;
		}
	}

	CString strSection;
	int i; for ( i=0;;i++) {
		TCHAR   buff[_MAX_PATH] = {'\0'};
		strSection.Format(TEXT("Load%03d"), i);
		Profile_Initialize(strINI, TRUE);
		Profile_GetString(strSection, TEXT("Path"), TEXT(""), buff, sizeof(buff), strINI);
		Profile_Free();
		if (lstrlen(buff) == 0) {
			break;
		}
		PSTEPlugin pPlugin = STEPluginLoadFile(buff);
		if (pPlugin == NULL) {
			//pPlugin->bUse = false;
			MessageBox(hWnd, CString("�v���O�C��(") + buff + ")�̓ǂݍ��݂Ɏ��s���܂���", TEXT("�v���O�C���G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			continue;
		}
		Profile_Initialize(strINI, TRUE);
		pPlugin->bUse = Profile_GetInt(strSection, TEXT("Use"), 1, strINI) ? true : false;
		Profile_Free();
	}
	STEP_wndToolBar->UpdatePluginButton();
}

bool OnToolTipNotify(UINT nID, LPTSTR& szText) {
	UINT nNextID = _APS_NEXT_COMMAND_VALUE;
	if (nID < nNextID) return false;
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!pPlugin->bUse) continue;
		if (pPlugin->STEPGetToolTipText != NULL) {
			szText = const_cast<LPTSTR>((*pPlugin->STEPGetToolTipText)(nID));
			if (szText != NULL) {
				return true;
			}
		}
	}
	return false;
}
bool GetMessageString(UINT nID, CString& strMessage) {
	UINT nNextID = _APS_NEXT_COMMAND_VALUE;
	if (nID < nNextID) return false;
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!pPlugin->bUse) continue;
		if (pPlugin->STEPGetStatusMessage != NULL) {
			LPCTSTR szText = (*pPlugin->STEPGetStatusMessage)(nID);
			strMessage = "[";
			strMessage += pPlugin->sPluginName;
			strMessage += "] ";
			if (szText != NULL) {
				strMessage += szText;
				return true;
			}
		}
	}
	return false;
}


UINT GetFileTypeCount() {
	UINT nCount = 0;
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		nCount += plugin->arExtInfo.GetSize();
	}
	return nCount;
}


BOOL OnUpdatePluginCommand(UINT nID) {
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!plugin->bUse) continue;
		if (plugin->nOptionCommandID == nID) {
			return true;
		}
		if (plugin->STEPOnUpdateCommand != NULL) {
			if (plugin->STEPOnUpdateCommand(nID)) {
				return true;
			}	
		}
	}
	return FALSE;
}

void OnPluginCommand(UINT nID) {
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!plugin->bUse) continue;
		/* DlgPluginSetup �Ɉړ�
		if (plugin->nOptionCommandID == nID) {
			plugin->STEPShowOptionDialog(STEP_hWnd);
			return;
		}
		*/
		if (plugin->STEPOnCommand != NULL) {
			if (plugin->STEPOnCommand(nID, STEP_hWnd)) {
				return;
			}
		}
	}
	return;
}

void OnLoadMenu(CMenu* pMenu, UINT nType) {
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!plugin->bUse) continue;
		if (plugin->STEPOnLoadMenu != NULL) {
			plugin->STEPOnLoadMenu(pMenu->GetSubMenu(0)->m_hMenu, nType);
		}
	}
}

void OnLoadMainMenu() {
	int i; for ( i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!plugin->bUse) continue;
		if (plugin->STEPOnLoadMainMenu != NULL) {
			plugin->STEPOnLoadMainMenu();
		}
	}
}

BOOL IsPluginSupportSIF(const FILE_MP3 *fileMP3) {
	int nIndex = plugins.GetPluginIndex(fileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return FALSE;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
		if (!plugin->bUse) return FALSE;
	if (plugin->STEPSupportSIF == NULL) {
		return FALSE;
	}
	return plugin->STEPSupportSIF(fileMP3->nFormat) ? TRUE : FALSE;
}

BOOL IsPluginSupportTrackNumberSIF(const FILE_MP3 *fileMP3) {
	int nIndex = plugins.GetPluginIndex(fileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return FALSE;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
		if (!plugin->bUse) return FALSE;
	if (plugin->STEPSupportTrackNumberSIF == NULL) {
		return IsPluginSupportSIF(fileMP3);// FALSE;
	}
	return plugin->STEPSupportTrackNumberSIF(fileMP3->nFormat) ? TRUE : FALSE;
}

BOOL IsPluginSupportGenreSIF(const FILE_MP3 *fileMP3) {
	int nIndex = plugins.GetPluginIndex(fileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return FALSE;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
		if (!plugin->bUse) return FALSE;
	if (plugin->STEPSupportGenreSIF == NULL) {
		return IsPluginSupportSIF(fileMP3);// FALSE;
	}
	return plugin->STEPSupportGenreSIF(fileMP3->nFormat) ? TRUE : FALSE;
}

CONTROLTYPE GetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	int nIndex = plugins.GetPluginIndex(nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return invalid;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
		if (!plugin->bUse) return invalid;
	if (plugin->STEPGetControlType == NULL) {
		return invalid;
	}
	return plugin->STEPGetControlType(nFormat, nColumn, isEditSIF);
}

int GetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	int nIndex = plugins.GetPluginIndex(nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return 0;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (!plugin->bUse) return 0;
	if (plugin->STEPGetColumnMax == NULL) {
		return 0;
	}
	return plugin->STEPGetColumnMax(nFormat, nColumn, isEditSIF);
}

LPCTSTR GetColumnName(UINT nFormat, COLUMNTYPE nColumn) {
	int nIndex = plugins.GetPluginIndex(nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return 0;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (!plugin->bUse) return 0;
	if (plugin->STEPGetColumnName == NULL) {
		return 0;
	}
	return plugin->STEPGetColumnName(nFormat, nColumn);
}

bool HasOriginalColumnName(UINT nFormat) {
	int nIndex = plugins.GetPluginIndex(nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return false;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (!plugin->bUse) return false;
	if (plugin->STEPGetColumnName == NULL) {
		return false;
	}
	if (plugin->STEPHasSpecificColumnName == NULL) {
		return false;
	}
	return plugin->STEPHasSpecificColumnName(nFormat);
}

UINT GetFormatType(UINT nIndex)
{
	return arFormatType.GetAt(nIndex);
}
UINT GetFormatTypeIndex(UINT nFormat)
{
	int i; for (i=0;i<arFormatType.GetSize();i++) {
		if (arFormatType.GetAt(i) == nFormat) {
			return i;
		}
	}
	return -1;
}

bool LoadFile(LPCTSTR sFileName, LPCTSTR sExt, FILE_MP3* pFileMP3)
{
	UINT result = false;
	FILE_INFO fileInfo;
	fileInfo.pFileMP3 = pFileMP3;
	int i; for (i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!plugin->bUse) continue;
		if (plugin->STEPLoad != NULL) {
			result = plugin->STEPLoad(&fileInfo, sExt);
			if (result == STEP_SUCCESS) {
				break;
			} else if (result == STEP_ERROR) {
				CFileMP3::InitData(pFileMP3);
			}
		}
	}
	if (result != STEP_SUCCESS) {
		return false;
	}
	return true;
}

bool SaveFile(FILE_MP3* pFileMP3)
{
	if (!pFileMP3->bModifyFlag)	return true;
	UINT result = STEP_ERROR;
	FILE_INFO fileInfo;
	fileInfo.pFileMP3 = pFileMP3;
	int nIndex = plugins.GetPluginIndex(pFileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return false;
	}
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (!plugin->bUse) return false;
	if (plugin->STEPSave != NULL) {
		result = plugin->STEPSave(&fileInfo);
		if (result == STEP_SUCCESS) {
			result = CFileMP3::ConvFileName(pFileMP3) == true ? STEP_SUCCESS : STEP_ERROR;
		}
	}
	if (result != STEP_SUCCESS) {
		return false;
	}
	pFileMP3->bModifyFlag = false;
	return true;
}

int GetFileImageIndex(UINT nFormat, bool bModify)
{
	int nIndex = plugins.GetPluginIndex(nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return -1;
	}
	PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (!pPlugin->bUse) return -1;
	int i; for (i=0;i<pPlugin->arExtInfo.GetSize();i++) {
		PSTEPExtInfo info = (PSTEPExtInfo)pPlugin->arExtInfo.GetAt(i);
		if (info->nFormatType == nFormat && info->nImageIndex != -1) {
			return bModify ? (info->nImageIndex+1) : info->nImageIndex;
		}
	}
	return -1;
}

void GetFileExtList(CStringArray& arExt)
{
	int i; for (i=0;i<plugins.arPlugins.GetSize();i++) {
		PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(i);
		if (!pPlugin->bUse) continue;
		int j; for (j=0;j<pPlugin->arExtInfo.GetSize();j++) {
			PSTEPExtInfo info = (PSTEPExtInfo)pPlugin->arExtInfo.GetAt(j);
			for (int k=0;k<arExt.GetSize();k++) {
				if (arExt.GetAt(k) == info->strExt) {
					info = NULL;
					break;
				}
			}
			if (info != NULL) {
				arExt.Add(info->strExt);
			}
		}
	}
}

bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext)
{
	if (pFileMP3 == NULL)	return false;
	int nIndex = plugins.GetPluginIndex(pFileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return false;
	}
	PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	int j; for (j=0;j<pPlugin->arExtInfo.GetSize();j++) {
		PSTEPExtInfo info = (PSTEPExtInfo)pPlugin->arExtInfo.GetAt(j);
		if (_tcsicmp(ext, info->strExt) == 0) {
			return true;
		}
		if (ext[0] == '.' && _tcsicmp(ext+1, info->strExt) == 0) {
			return true;
		}
	}
	return false;
}

void InitFileSpecificInfo(FILE_MP3* pFileMP3)
{
	int nIndex = plugins.GetPluginIndex(pFileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return;
	}
	FILE_INFO fileInfo;
	fileInfo.pFileMP3 = pFileMP3;
	PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (pPlugin->STEPInitFileSpecificInfo != NULL) {
		pPlugin->STEPInitFileSpecificInfo(&fileInfo);
		return;
	}
}

void ConvSIFieldToID3tag(FILE_MP3* pFileMP3) /* STEP 037 */
{

	int nIndex = STEP_List->GetIndexFromFileMP3(pFileMP3);
	ASSERT(nIndex >= 0);
	if (nIndex < 0) return;

	CString		strText;
	bool saveEditField = g_bOptEditFieldSIF;
	int i; for (i=	COLUMN_TRACK_NAME;i<COLUMN_MAX;i++) {
		g_bOptEditFieldSIF = TRUE; // �\���t�B�[���h�ؑ�
		// SI/ID3v2�t�B�[���h�̓��e���擾
		CString strTextSI = STEP_List->GetFileColumnText(pFileMP3, i);
		g_bOptEditFieldSIF = FALSE; // �\���t�B�[���h�ؑ�
		// ID3tag�t�B�[���h�֓���
		STEP_List->ChangeSubItemText(nIndex, g_nColumnNumberList[i], strTextSI, 0, false, "", "", true);
	}
	g_bOptEditFieldSIF = saveEditField; // �\���t�B�[���h�߂�
	STEP_List->UpdateFileStatus((CSuperGridCtrl::CTreeItem*)pFileMP3->pTreeItem, pFileMP3);
}

void OnConvSIFieldToID3tag(FILE_MP3* pFileMP3) /* STEP 037 */
{
	int nIndex = plugins.GetPluginIndex(pFileMP3->nFormat);
	if (nIndex < 0) {
		// �Ή��v���O�C���Ȃ�
		return;
	}
	FILE_INFO fileInfo;
	fileInfo.pFileMP3 = pFileMP3;
	PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
	if (pPlugin->STEPOnConvSiFieldToId3tag != NULL) {
		if (pPlugin->STEPOnConvSiFieldToId3tag(&fileInfo) == false) {
			return; // ���T�|�[�g or �v���O�C�����Ŏ��s
		}
	} else {
		return; // ���T�|�[�g
	}
	ConvSIFieldToID3tag(pFileMP3);
}

/*----------------------------*/
extern "C" STEP_API UINT WINAPI STEPGetCommandID()
{
	return nCommandID++;
}

extern "C" 	STEP_API HMENU WINAPI STEPGetMenu(UINT nType)
{
	CWnd	*pMainWnd = AfxGetMainWnd();
	CMenu	*STEP_hMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
	CMenu* subMenu;
	switch (nType) {
	case MENU_FILE:
		subMenu = STEP_hMenu->GetSubMenu(0);
		return subMenu->m_hMenu;
	case MENU_EDIT:
		subMenu = STEP_hMenu->GetSubMenu(1);
		return subMenu->m_hMenu;
	case MENU_DISP:
		subMenu = STEP_hMenu->GetSubMenu(2);
		return subMenu->m_hMenu;
	case MENU_CONV:
		subMenu = STEP_hMenu->GetSubMenu(3);
		return subMenu->m_hMenu;
	case MENU_PLAYLIST:
		subMenu = STEP_hMenu->GetSubMenu(4);
		return subMenu->m_hMenu;
	case MENU_PLAYER:
		subMenu = STEP_hMenu->GetSubMenu(5);
		return subMenu->m_hMenu;
	case MENU_OPTION:
		subMenu = STEP_hMenu->GetSubMenu(6);
		return subMenu->m_hMenu;
/*
	case MENU_OPTION_PLUGIN:
		subMenu = STEP_hMenu->GetSubMenu(6);
		subMenu = subMenu->GetSubMenu(13);
		return subMenu->m_hMenu;
*/
	case MENU_HELP:
		subMenu = STEP_hMenu->GetSubMenu(6);
		return subMenu->m_hMenu;
	}
	return NULL;
}

extern "C" 	STEP_API void WINAPI STEPAddToolBarButton(HBITMAP hBitmap, UINT nCommandID, LPCTSTR lpszRegName)
{
	plugins.arHandle.Add(hBitmap);
	CBitmap* pBitmap = CBitmap::FromHandle(hBitmap);
	CToolBarCtrl* pToolbarCtrl = &STEP_wndToolBar->GetToolBarCtrl();
	pToolbarCtrl->AddBitmap(1, pBitmap);
	int nButCount = pToolbarCtrl->GetButtonCount();
	int nImageCount = pToolbarCtrl->GetImageList()->GetImageCount();
	TBBUTTON tb;
	tb.iBitmap = nImageCount-1;
	tb.idCommand = nCommandID;
	tb.fsState = TBSTATE_ENABLED;
	tb.fsStyle = TBSTYLE_BUTTON;
	tb.dwData = 0;
	tb.iString = NULL;
	STEP_wndToolBar->InsertButton(nButCount, &tb, lpszRegName);
}

extern "C" STEP_API void WINAPI STEPKeyAssign(UINT nCommandID, char* lpszName, LPTSTR lpszRegName)
{
	KEY_CONFIG* key = new KEY_CONFIG;
	key->wCmdID = nCommandID;
	key->dwKeyCode = 0;
	key->nGroupID = COMMAND_GROUP_PLUGIN;
	key->sName = lpszName;
	key->sRegName = lpszRegName;
	plugins.arPluginKey.Add(key);
}

extern "C" STEP_API UINT WINAPI STEPRegisterExt(UINT nID, LPCTSTR szExt, HBITMAP hBitmap)
{
	PSTEPlugin plugin = (PSTEPlugin)plugins.arPlugins.GetAt(nID-1);
	UINT nFormatType = nID << 8 | (plugin->arExtInfo.GetSize());
	arFormatType.Add(nFormatType);
	
	PSTEPExtInfo info = new STEPExtInfo;
	info->strExt = szExt;
	CBitmap* pBitmap = CBitmap::FromHandle(hBitmap);
	if (hBitmap == NULL) {
		info->nImageIndex = -1;
	} else {
		info->nImageIndex = STEP_ImageList->Add(pBitmap, RGB(255, 0, 255));
		pBitmap->Detach();
	}
	info->nFormatType = nFormatType;
	plugin->arExtInfo.Add(info);
	return nFormatType;
}

extern "C" STEP_API LPCTSTR WINAPI STEPGetGenreNameSIF(BYTE byGenre)
{
	return GetGenreNameSIF(byGenre);
}

extern "C" STEP_API BYTE WINAPI STEPGetGenreCode(LPCTSTR szGenre)
{
	return GetGenreCode(szGenre);
}

extern "C" STEP_API bool WINAPI STEPIsUserGenre(LPCTSTR szGenre)
{
	return IsUserGenre(szGenre);
}

extern "C" STEP_API int WINAPI STEPGetNumericTrackNumber(const char* szTrackNumber, LPTSTR szNumericNumber, int size) /* STEP 037 */
{
	CString strTrackNumber = szTrackNumber;
	CFileMP3::GetIntTrackNo(strTrackNumber);
	_tcsncpy(szNumericNumber, strTrackNumber, size);
	return strTrackNumber.GetLength();
}

extern "C" STEP_API int WINAPI STEPGetIntegerTrackNumber(const char* szTrackNumber) /* STEP 037 */
{
	return _tstoi(CFileMP3::GetIntTrackNo(szTrackNumber));
}

extern "C" STEP_API int WINAPI STEPGetNumericDiskNumber(const char* szDiskNumber, LPTSTR szNumericNumber, int size) /* STEP 037 */
{
	CString strDiskNumber = szDiskNumber;
	CFileMP3::GetIntDiskNo(strDiskNumber);
	_tcsncpy(szNumericNumber, strDiskNumber, size);
	return strDiskNumber.GetLength();
}

extern "C" STEP_API int WINAPI STEPGetIntegerDiskNumber(const char* szDiskNumber) /* STEP 037 */
{
	return _tstoi(CFileMP3::GetIntDiskNo(szDiskNumber));
}

extern "C" STEP_API bool WINAPI STEPIsNumeric(const char* szText) /* STEP 037 */
{
	return CFileMP3::IsNumeric(szText);
}

extern "C" STEP_API void WINAPI STEPConvSiFieldToId3tag(FILE_INFO* pFileInfo) /* STEP 037 */
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	
	ConvSIFieldToID3tag(pFileMP3);
}

extern "C" STEP_API LPCTSTR WINAPI STEPGetValue(FILE_INFO* pFileInfo, FIELDTYPE nField)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case FIELD_FULL_PATH_NAME:			// �t�@�C����(�t���p�X)
		return pFileMP3->strFullPathName;
	case FIELD_FILE_NAME:				// strFullPathName �̃t�@�C��������
		return pFileMP3->strFileName;
	case FIELD_FILE_PATH:				// strFullPathName �̃p�X����
		return pFileMP3->strFilePath;
	case FIELD_VISUAL_FORMAT:			// �f���t�H�[�}�b�g
		return pFileMP3->strVisualFormat;
	case FIELD_AUDIO_FORMAT:			// �����t�H�[�}�b�g
		return pFileMP3->strAudioFormat;
	case FIELD_TRACK_NAME:				// �g���b�N��
		return pFileMP3->strTrackName;
	case FIELD_ARTIST_NAME:				// �A�[�e�B�X�g��
		return pFileMP3->strArtistName;
	case FIELD_ALBUM_NAME:				// �A���o����
		return pFileMP3->strAlbumName;
	case FIELD_COMMENT:					// �R�����g
		return pFileMP3->strComment;
	case FIELD_YEAR:					// �����[�X�N��
		return pFileMP3->strYear;
	case FIELD_TRACK_NUMBER:			// �g���b�N�ԍ�
		return pFileMP3->strTrackNumber;
	case FIELD_DISK_NUMBER:			// �g���b�N�ԍ�
		return pFileMP3->strDiskNumber;
	case FIELD_GENRE:					// �W��������
		return pFileMP3->strGenre;
	case FIELD_TRACK_NAME_SI:			// �Ȗ�
		return pFileMP3->strTrackNameSI;
	case FIELD_ARTIST_NAME_SI:			// �A�[�e�B�X�g��
		return pFileMP3->strArtistNameSI;
	case FIELD_ALBUM_NAME_SI:			// ���i��
		return pFileMP3->strAlbumNameSI;
	case FIELD_COMMENT_SI:				// �R�����g������
		return pFileMP3->strCommentSI;
	case FIELD_YEAR_SI:					// �����[�X�N��
		return pFileMP3->strYearSI;
	case FIELD_TRACK_NUMBER_SI:			// �g���b�N�ԍ�
		return pFileMP3->strTrackNumberSI;
	case FIELD_DISK_NUMBER_SI:			// �g���b�N�ԍ�
		return pFileMP3->strDiskNumberSI;
	case FIELD_GENRE_SI:				// �W��������
		return pFileMP3->strGenreSI;
	case FIELD_COPYRIGHT_SI:			// ���쌠
		return pFileMP3->strCopyrightSI;
	case FIELD_ENGINEER_SI:				// �G���W�j�A
		return pFileMP3->strEngineerSI;
	case FIELD_SOURCE_SI:				// �\�[�X
		return pFileMP3->strSourceSI;
	case FIELD_SOFTWARE_SI:				// �\�t�g�E�F�A
		return pFileMP3->strSoftwareSI;
	case FIELD_KEYWORD_SI:				// �L�[���[�h
		return pFileMP3->strKeywordSI;
	case FIELD_TECHNICIAN_SI:			// �Z�p��
		return pFileMP3->strTechnicianSI;
	case FIELD_LYRIC_SI:				// �̎�
		return pFileMP3->strLyricSI;
	case FIELD_COMMISSION_SI:			// �R�~�b�V����
		return pFileMP3->strCommissionSI;
	case FIELD_WRITER_SI:				// �쎌
		return pFileMP3->strWriterSI;
	case FIELD_COMPOSER_SI:				// ���
		return pFileMP3->strComposerSI;
	case FIELD_ALBM_ARTIST_SI:			// albm.�A�[�e�B�X�g
		return pFileMP3->strAlbmArtistSI;
	case FIELD_ORIG_ARTIST_SI:			// Orig.�A�[�e�B�X�g
		return pFileMP3->strOrigArtistSI;
	case FIELD_URL_SI:					// URL
		return pFileMP3->strURLSI;
	case FIELD_ENCODEST:				// �G���R�[�h�����l
		return pFileMP3->strEncodest;
	case FIELD_OTHER:					// ���̑�
		return pFileMP3->strOther;
	case FILED_FILE_TYPE_NAME:			// �t�@�C���^�C�v������
		return pFileMP3->strFileTypeName;
	}
	return TEXT("");
}

extern "C" STEP_API void WINAPI STEPSetValue(FILE_INFO* pFileInfo, FIELDTYPE nField, LPCTSTR szValue)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case FIELD_FULL_PATH_NAME:			// �t�@�C����(�t���p�X)
		pFileMP3->strFullPathName = szValue;
		break;
	case FIELD_FILE_NAME:				// strFullPathName �̃t�@�C��������
		pFileMP3->strFileName = szValue;
		break;
	case FIELD_FILE_PATH:				// strFullPathName �̃p�X����
		pFileMP3->strFilePath = szValue;
		break;
	case FIELD_VISUAL_FORMAT:			// �f���t�H�[�}�b�g
		pFileMP3->strVisualFormat = szValue;
		break;
	case FIELD_AUDIO_FORMAT:			// �����t�H�[�}�b�g
		pFileMP3->strAudioFormat = szValue;
		break;
	case FIELD_TRACK_NAME:				// �g���b�N��
		pFileMP3->strTrackName = szValue;
		break;
	case FIELD_ARTIST_NAME:				// �A�[�e�B�X�g��
		pFileMP3->strArtistName = szValue;
		break;
	case FIELD_ALBUM_NAME:				// �A���o����
		pFileMP3->strAlbumName = szValue;
		break;
	case FIELD_COMMENT:					// �R�����g
		pFileMP3->strComment = szValue;
		break;
	case FIELD_YEAR:					// �����[�X�N��
		pFileMP3->strYear = szValue;
		break;
	case FIELD_TRACK_NUMBER:			// �g���b�N�ԍ�
		pFileMP3->strTrackNumber = szValue;
		if (_tstoi(szValue) == 0 || _tstoi(szValue) > 0x0ff) {
			pFileMP3->byTrackNumber = 0x0ff;
		} else {
			pFileMP3->byTrackNumber = _tstoi(szValue);
		}
		break;
	case FIELD_DISK_NUMBER:			// �g���b�N�ԍ�
		pFileMP3->strDiskNumber = szValue;
		if (_tstoi(szValue) == 0 || _tstoi(szValue) > 0x0ff) {
			pFileMP3->byDiskNumber = 0x0ff;
		} else {
			pFileMP3->byDiskNumber = _tstoi(szValue);
		}
		break;
	case FIELD_GENRE:					// �W��������
		pFileMP3->strGenre = szValue;
		break;
	case FIELD_TRACK_NAME_SI:			// �Ȗ�
		pFileMP3->strTrackNameSI = szValue;
		break;
	case FIELD_ARTIST_NAME_SI:			// �A�[�e�B�X�g��
		pFileMP3->strArtistNameSI = szValue;
		break;
	case FIELD_ALBUM_NAME_SI:			// ���i��
		pFileMP3->strAlbumNameSI = szValue;
		break;
	case FIELD_COMMENT_SI:				// �R�����g������
		pFileMP3->strCommentSI = szValue;
		break;
	case FIELD_YEAR_SI:					// �����[�X�N��
		pFileMP3->strYearSI = szValue;
		break;
	case FIELD_TRACK_NUMBER_SI:			// �g���b�N�ԍ�
		pFileMP3->strTrackNumberSI = szValue;
		break;
	case FIELD_DISK_NUMBER_SI:			// �f�B�X�N�ԍ�
		pFileMP3->strDiskNumberSI = szValue;
		break;
	case FIELD_GENRE_SI:				// �W��������
		pFileMP3->strGenreSI = szValue;
		break;
	case FIELD_COPYRIGHT_SI:			// ���쌠
		pFileMP3->strCopyrightSI = szValue;
		break;
	case FIELD_ENGINEER_SI:				// �G���W�j�A
		pFileMP3->strEngineerSI = szValue;
		break;
	case FIELD_SOURCE_SI:				// �\�[�X
		pFileMP3->strSourceSI = szValue;
		break;
	case FIELD_SOFTWARE_SI:				// �\�t�g�E�F�A
		pFileMP3->strSoftwareSI = szValue;
		break;
	case FIELD_KEYWORD_SI:				// �L�[���[�h
		pFileMP3->strKeywordSI = szValue;
		break;
	case FIELD_TECHNICIAN_SI:			// �Z�p��
		pFileMP3->strTechnicianSI = szValue;
		break;
	case FIELD_LYRIC_SI:				// �̎�
		pFileMP3->strLyricSI = szValue;
		break;
	case FIELD_COMMISSION_SI:			// �R�~�b�V����
		pFileMP3->strCommissionSI = szValue;
		break;
	case FIELD_WRITER_SI:
		pFileMP3->strWriterSI = szValue;
		break;
	case FIELD_COMPOSER_SI:				// ���
		pFileMP3->strComposerSI = szValue;
		break;
	case FIELD_ALBM_ARTIST_SI:			// Albm.�A�[�e�B�X�g
		pFileMP3->strAlbmArtistSI = szValue;
		break;
	case FIELD_ORIG_ARTIST_SI:			// Orig.�A�[�e�B�X�g
		pFileMP3->strOrigArtistSI = szValue;
		break;
	case FIELD_URL_SI:					// URL
		pFileMP3->strURLSI = szValue;
		break;
	case FIELD_ENCODEST:				// �G���R�[�h�����l
		pFileMP3->strEncodest = szValue;
		break;
	case FIELD_OTHER:					// ���̑�
		pFileMP3->strOther = szValue;
		break;
	case FILED_FILE_TYPE_NAME:			// �t�@�C���^�C�v������
		pFileMP3->strFileTypeName = szValue;
		break;
	}
}

extern "C" STEP_API bool WINAPI STEPGetBooleanValue(FILE_INFO* pFileInfo, UINT nField)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �O������ҏW���ꂽ���ǂ����̃t���O
		return pFileMP3->bModifyFlag;
	case 1:			// �`�F�b�N���
		return pFileMP3->bCheckFlag;
	case 2:			// �̎��t�@�C���̑���
		return pFileMP3->bExistLyricFile;
	}
	return false;
}

extern "C" STEP_API void WINAPI STEPSetBooleanValue(FILE_INFO* pFileInfo, UINT nField, bool bFlag)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �O������ҏW���ꂽ���ǂ����̃t���O
		pFileMP3->bModifyFlag = bFlag;
		break;
	case 1:			// �`�F�b�N���
		pFileMP3->bCheckFlag = bFlag;
		break;
	case 2:			// �̎��t�@�C���̑���
		pFileMP3->bExistLyricFile = bFlag;
		break;
	}
}

extern "C" STEP_API long WINAPI STEPGetLongValue(FILE_INFO* pFileInfo, UINT nField)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���T�C�Y
		return pFileMP3->lFileSize;
	}
	return 0;
}

extern "C" STEP_API void WINAPI STEPSetLongValue(FILE_INFO* pFileInfo, UINT nField, long lValue)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���T�C�Y
		pFileMP3->lFileSize = lValue;
		break;
	}
}

extern "C" STEP_API int WINAPI STEPGetIntValue(FILE_INFO* pFileInfo, UINT nField)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���t�H�[�}�b�g
		return pFileMP3->nFormat;
	case 1:			// ���t����(�P�ʁF�b)
		return pFileMP3->nPlayTime;
	case 2:			// �g���b�N�ԍ�(���l�ҏW�p)
		return pFileMP3->byTrackNumber;
	case 3:			// �W�������ԍ�
		return pFileMP3->byGenre;
	case 4:			// �f�B�X�N�ԍ�
		return pFileMP3->byDiskNumber;
	}
	return 0;
}

extern "C" STEP_API void WINAPI STEPSetIntValue(FILE_INFO* pFileInfo, UINT nField, int nValue)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���t�H�[�}�b�g
		pFileMP3->nFormat = nValue;
		break;
	case 1:			// ���t����(�P�ʁF�b)
		pFileMP3->nPlayTime = nValue;
		break;
	case 2:			// �g���b�N�ԍ�(���l�ҏW�p)
		pFileMP3->byTrackNumber = nValue;
		if (pFileMP3->byTrackNumber != 0 && pFileMP3->byTrackNumber < 0xff) {
			CString strTrackNumber;
			strTrackNumber.Format(TEXT("%d"), pFileMP3->byTrackNumber);
			STEPSetValue(pFileInfo, FIELD_TRACK_NUMBER, strTrackNumber);
		} else {
			STEPSetValue(pFileInfo, FIELD_TRACK_NUMBER, TEXT(""));
		}
		break;
	case 3:			// �W�������ԍ�
		pFileMP3->byGenre = nValue;
		break;
	case 4:			// �f�B�X�N�ԍ�
		pFileMP3->byDiskNumber = nValue;
		if (pFileMP3->byDiskNumber != 0 && pFileMP3->byDiskNumber < 0xff) {
			CString strDiskNumber;
			strDiskNumber.Format(TEXT("%d"), pFileMP3->byDiskNumber);
			STEPSetValue(pFileInfo, FIELD_DISK_NUMBER, strDiskNumber);
		} else {
			STEPSetValue(pFileInfo, FIELD_DISK_NUMBER, TEXT(""));
		}
		break;
	}
}

extern "C" STEP_API void* WINAPI STEPGetVoidValue(FILE_INFO* pFileInfo, UINT nField)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���ŗL���
		return pFileMP3->fileSpecificInfo;
	}
	return NULL;
}

extern "C" STEP_API void WINAPI STEPSetVoidValue(FILE_INFO* pFileInfo, UINT nField, void* pInfo)
{
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	switch (nField) {
	case 0:			// �t�@�C���ŗL���
		pFileMP3->fileSpecificInfo = pInfo;
		break;
	}
}

extern "C" STEP_API void WINAPI STEPProcessSelectedFiles(LPCTSTR szTitle, STEPProcessSelectedFilesCB* callback)
{
	STEP_List->ProcessSelectedFiles(szTitle, callback);
}

extern "C" STEP_API void WINAPI STEPProcessSelectedFilesForUpdate(LPCTSTR szTitle, STEPProcessSelectedFilesCB* callback)
{
	STEP_List->ProcessSelectedFilesForUpdate(szTitle, callback);
}

extern "C" STEP_API bool WINAPI STEPIsRangeSelected(void)
{
	return STEP_List->IsRangeSelected();
}

extern "C" STEP_API int WINAPI STEPGetSelectedCount(void)
{
	return STEP_List->GetSelectedCount();
}

extern "C" STEP_API int WINAPI STEPGetSelectedItem(void)
{
	return STEP_List->GetSelectedItem();
}

extern "C" STEP_API bool WINAPI STEPIsCurrentCellEditOK(void)
{
	return STEP_List->IsCurrentCellEditOK();
}

extern "C" STEP_API bool WINAPI STEPItemHasChildren(int nIndex)
{
	return STEP_List->ItemHasChildren(STEP_List->GetTreeItem(nIndex/*STEPGetSelectedItem()*/)) ? true : false;
}

extern "C" STEP_API bool WINAPI STEPGetSelectedRange(int* sx, int* sy, int* ex, int* ey)
{
	return STEP_List->GetSelectedRange(*sx, *sy, *ex, *ey);
}

extern "C" STEP_API bool WINAPI STEPIsItemFile(int nItem)
{
	CSuperGridCtrl::CTreeItem	*pItem = STEP_List->GetTreeItem(nItem);
	if (pItem != NULL && STEP_List->IsItemFile(pItem) == TRUE) {
		return true;
	}
	return false;
}

extern "C" STEP_API void WINAPI STEPChangeSubItemText(int nItem, int nColumn, LPCTSTR szText)
{
	STEP_List->ChangeSubItemText(nItem, nColumn, szText);
	STEP_List->InvalidateItemRect(nItem);
}

extern "C" STEP_API LPCTSTR WINAPI STEPGetSubItemText(int nItem, int nColumn)
{
	CSuperGridCtrl::CTreeItem	*pItem = STEP_List->GetTreeItem(nItem);
	CItemInfo	*pItemInfo = STEP_List->GetData(pItem);
	return pItemInfo->GetSubItem(nColumn-1);
}

extern "C" STEP_API void WINAPI STEPGetFileInfo(int nItem, FILE_INFO* info)
{
	CSuperGridCtrl::CTreeItem	*pItem = STEP_List->GetTreeItem(nItem);
	CItemInfo	*pItemInfo = STEP_List->GetData(pItem);
	FILE_MP3	*fileMP3 = STEP_List->m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
	info->pFileMP3 = fileMP3;
}



extern "C" STEP_API void WINAPI STEPInitDataSIF(FILE_INFO* pFileInfo) {
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	CFileMP3::InitDataSIF(pFileMP3);
}

extern "C" STEP_API void WINAPI STEPInitDataID3(FILE_INFO* pFileInfo) {
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	CFileMP3::InitDataID3(pFileMP3);
}

extern int g_nColumnTypeList[];
extern "C" STEP_API UINT WINAPI STEPGETColumnType(int nColumn)
{
	return g_nColumnTypeList[nColumn];
}


extern "C" STEP_API void WINAPI STEPInitData(FILE_INFO* pFileInfo) {
	FILE_MP3* pFileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	CFileMP3::InitData(pFileMP3);
}

extern "C" STEP_API bool WINAPI STEPFileNameChange(FILE_INFO* pFileInfo, LPCTSTR szNewFileName) {
	FILE_MP3* fileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;

	TCHAR	fname[_MAX_FNAME];
	CString		strFileNameOld = fileMP3->strFileName;
	fileMP3->strFileName = szNewFileName;
	_tsplitpath(fileMP3->strFileName, NULL, NULL, fname, NULL);

	// �t�@�C�����ύX
	CFileStatus	fileStatus;
	bool	bKeepTimeStamp = g_bOptKeepTimeStamp;
	// �^�C���X�^���v���擾
	fileStatus.m_mtime = -1;
	if (bKeepTimeStamp
	&&  CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
		bKeepTimeStamp = false;
	}
	if (CFileMP3::ConvFileName(fileMP3) == false) {
		// ���s�����ꍇ�́A�t�@�C���������ɖ߂�
		fileMP3->bModifyFlag = TRUE;
		fileMP3->strFileName = strFileNameOld;
		return false;
	} else {
		if (bKeepTimeStamp) {
			// �t�@�C���X�V���Ԃ�ݒ�
			fileStatus.m_mtime = fileMP3->time;
			if (g_bOptSyncCreateTime) fileStatus.m_ctime = fileMP3->time;
			CFile::SetStatus(fileMP3->strFullPathName, fileStatus);
		} else {
			if (fileStatus.m_mtime.GetTime() != -1) {
				fileMP3->time = fileStatus.m_mtime;
			}
		}
	}
	return true;
}

extern "C" STEP_API bool WINAPI STEPWriteTag(FILE_INFO* pFileInfo) {
	FILE_MP3* fileMP3 = (FILE_MP3*)pFileInfo->pFileMP3;
	return SaveFile(fileMP3);
}

extern "C" STEP_API void WINAPI STEPUpdateCellInfo(void) {
	STEP_List->UpdateCellInfo();
	STEP_List->UpdateAllFiles();
}