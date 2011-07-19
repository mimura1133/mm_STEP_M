// Plugin.h: CPlugin クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGIN_H__E74822CA_38D9_44B7_B15D_04C4C6E60704__INCLUDED_)
#define AFX_PLUGIN_H__E74822CA_38D9_44B7_B15D_04C4C6E60704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "STEP_api.h"

typedef struct _STEPlugin {
	HINSTANCE hLib;
	bool bUse;
	CString sFileName;
	CString sPluginName;
	CPtrArray arExtInfo;				// サポートしている拡張子情報など
	UINT nOptionCommandID;

	LPCTSTR (WINAPI *STEPGetToolTipText)(UINT);
	LPCTSTR (WINAPI *STEPGetStatusMessage)(UINT);
	bool (WINAPI *STEPOnUpdateCommand)(UINT);
	bool (WINAPI *STEPOnCommand)(UINT, HWND hWnd);
	bool (WINAPI *STEPSupportSIF)(UINT);
	bool (WINAPI *STEPSupportTrackNumberSIF)(UINT);
	bool (WINAPI *STEPSupportGenreSIF)(UINT);
	CONTROLTYPE (WINAPI *STEPGetControlType)(UINT, COLUMNTYPE, bool);
	int (WINAPI *STEPGetColumnMax)(UINT, COLUMNTYPE, bool);
	LPCTSTR (WINAPI *STEPGetColumnName)(UINT, COLUMNTYPE);
	bool (WINAPI *STEPHasSpecificColumnName)(UINT);
	UINT (WINAPI *STEPLoad)(FILE_INFO *, LPCTSTR);
	UINT (WINAPI *STEPSave)(FILE_INFO *);
	void (WINAPI *STEPShowOptionDialog)(HWND);
	void (WINAPI *STEPOnLoadMenu)(HMENU, UINT);
	void (WINAPI *STEPOnLoadMainMenu)();
	LPCTSTR (WINAPI *STEPGetPluginInfo)(void);
	void (WINAPI *STEPInitFileSpecificInfo)(FILE_INFO*);
	bool (WINAPI *STEPOnConvSiFieldToId3tag)(FILE_INFO*);

} STEPlugin, *PSTEPlugin;

typedef struct _STEPExtInfo {
	CString strExt;
	int nImageIndex;
	UINT nFormatType;
} STEPExtInfo, *PSTEPExtInfo;


class CPlugin {
public:
	CPlugin() {};
	~CPlugin() {
		{
			bool (WINAPI *STEPFinalize)(void);
			for (int i=0;i<arPlugins.GetSize();i++) {
				PSTEPlugin pPlugin = (PSTEPlugin)arPlugins.GetAt(i);
				(FARPROC&)STEPFinalize = GetProcAddress(pPlugin->hLib, "_STEPFinalize@0");
				if (STEPFinalize != NULL) STEPFinalize();
			}
		}
		{
			for (int i=0;i<arPlugins.GetSize();i++) {
				PSTEPlugin pPlugin = (PSTEPlugin)arPlugins.GetAt(i);
				for (int j=0;j<pPlugin->arExtInfo.GetSize();j++) {
					PSTEPExtInfo info = (PSTEPExtInfo)pPlugin->arExtInfo.GetAt(j);
					delete info;
				}
			}
		}
		{
			for (int i=0;i<arPluginKey.GetSize();i++) {
				KEY_CONFIG* key = (KEY_CONFIG*)arPluginKey.GetAt(i);
				delete key;
			}
		}
		{
			for (int j=0;j<arHandle.GetSize();j++) {
				HGDIOBJ obj = (HGDIOBJ)arHandle.GetAt(j);
				DeleteObject(obj);
			}
		}
		{
			for (int i=0;i<arPlugins.GetSize();i++) {
				PSTEPlugin pPlugin = (PSTEPlugin)arPlugins.GetAt(i);
				FreeLibrary(pPlugin->hLib);
				delete pPlugin;
			}
		}
	}
	/*
	UINT GetFileTypeID(LPCTSTR szExt) {
		for (int i=0;arPlugins.GetSize();i++) {
			PSTEPlugin plugin = (PSTEPlugin)arPlugins.GetAt(i);
			int j; for (int j=0;j<plugin->arExtInfo.GetSize();j++) {
				if (stricmp(szExt, ((PSTEPExtInfo)(plugin->arExtInfo.GetAt(j)))->strExt) == 0) {
					return (i+1) << 8 | j;
				}
			}
		}
		return 0;
	}
	*/
	inline int GetPluginIndex(UINT nID) {
		return (((nID & 0xff00) >> 8) & 0x00ff) - 1;
	}
	CPtrArray arPlugins;
	CPtrArray arPluginKey;
	CPtrArray arHandle;
};

#endif // !defined(AFX_PLUGIN_H__E74822CA_38D9_44B7_B15D_04C4C6E60704__INCLUDED_)
