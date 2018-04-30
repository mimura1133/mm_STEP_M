#include "stdafx.h"
#include "STEPlugin.h"
#include "mp3infp_ExportFunc.h"

extern UINT nFileTypeAVI;

HMODULE hMp3infpDll = NULL;
LPMP3INFP_GETVER lpmp3infp_GetVer = NULL;
LPMP3INFP_GETVALUE lpmp3infp_GetValue = NULL;
LPMP3INFP_LOAD lpmp3infp_Load = NULL;

bool GetValues_mp3infp(FILE_INFO *pFileMP3);
void Free_mp3infp() {
	FreeLibrary(hMp3infpDll);
}

bool Load_mp3infp() {
	if(hMp3infpDll != NULL) {
		return true;
	}
	//DLLのロード
	hMp3infpDll = LoadLibrary(TEXT("mp3infp.dll"));
	if(hMp3infpDll == NULL) {
		return false;
	}
	lpmp3infp_GetVer = (LPMP3INFP_GETVER )GetProcAddress(hMp3infpDll,"mp3infp_GetVer");
	lpmp3infp_GetValue = (LPMP3INFP_GETVALUE )GetProcAddress(hMp3infpDll,"mp3infp_GetValue");
	lpmp3infp_Load = (LPMP3INFP_LOAD )GetProcAddress(hMp3infpDll,"mp3infp_Load");
	if (lpmp3infp_GetVer == NULL || lpmp3infp_GetValue == NULL) {
		return false;
	}
	//バージョンチェック
	DWORD dwVer = lpmp3infp_GetVer();
	if(dwVer < 0x0232) {
		return false;
	}
	return true;
}

bool GetValues_mp3infp(FILE_INFO *pFileMP3)
{
	// mp3infp.dllを利用して演奏時間などの情報を取得
	if (!Load_mp3infp()) {
		return false;
	}
	if(lpmp3infp_Load(NULL,(char*)(const char *)GetFullPath(pFileMP3)) != ERROR_SUCCESS)
	{
		//printf("ロード失敗\n");
		return false;
	}
	char* buf = NULL;
	// 演奏時間
	{
		lpmp3infp_GetValue("TIME",&buf);
		UINT nDummy, nTime;
		if (strchr(buf, ':') == strrchr(buf, ':')) { /* Hurricane 217 */
			sscanf(buf, "%u:%u (%usec)", &nDummy, &nDummy, &nTime);
		} else {
			sscanf(buf, "%u:%u:%u (%usec)", &nDummy, &nDummy, &nDummy, &nTime);  /* Hurricane 217 */
		}
		SetPlayTime(pFileMP3, nTime);
	}
	// 音声フォーマット
	{
		lpmp3infp_GetValue("AFMT",&buf);
		SetAudioFormat(pFileMP3, static_cast<CString>(buf));
	}
	// 映像フォーマット
	/*if (GetFormat(pFileMP3) == nFileTypeAVI)*/ {
		lpmp3infp_GetValue("VFMT",&buf);
		SetVisualFormat(pFileMP3, static_cast<CString>(buf));
	}
    return true;
}
