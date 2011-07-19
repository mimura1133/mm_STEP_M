/* Conspiracy 198 */
#include "stdafx.h"
#include "../FileMP3.h"

#include "mp3infp_ExportFunc.h"

HMODULE hMp3infpDll = NULL;
LPMP3INFP_GETVER lpmp3infp_GetVer = NULL;
LPMP3INFP_GETVALUE lpmp3infp_GetValue = NULL;
LPMP3INFP_LOAD lpmp3infp_Load = NULL;
DWORD dwMp3InfpVer = 0;

bool Load_mp3infp() {
	if(hMp3infpDll != NULL) {
		return true;
	}
	//DLLÇÃÉçÅ[Éh
	hMp3infpDll = LoadLibrary("mp3infp.dll");
	if(hMp3infpDll == NULL) {
		return false;
	}
	lpmp3infp_GetVer = (LPMP3INFP_GETVER )GetProcAddress(hMp3infpDll,"mp3infp_GetVer");
	lpmp3infp_GetValue = (LPMP3INFP_GETVALUE )GetProcAddress(hMp3infpDll,"mp3infp_GetValue");
	lpmp3infp_Load = (LPMP3INFP_LOAD )GetProcAddress(hMp3infpDll,"mp3infp_Load");
	if (lpmp3infp_GetVer == NULL || lpmp3infp_GetValue == NULL) {
		return false;
	}
	dwMp3InfpVer = lpmp3infp_GetVer();
	return true;
}
