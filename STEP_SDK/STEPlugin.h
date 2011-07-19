#ifndef _STEPLUGIN_H_
#define _STEPLUGIN_H_

#include "STEP_api.h"

//extern HINSTANCE g_hInstance;
//extern HINSTANCE hLib;

/* プラグインから呼び出せる関数 */
extern "C" {
extern STEP_API UINT (WINAPI *STEPGetCommandID)(void);
extern STEP_API HMENU (WINAPI *STEPGetMenu)(UINT);
extern STEP_API void (WINAPI *STEPAddToolBarButton)(HBITMAP, UINT, char*);
extern STEP_API UINT (WINAPI *STEPRegisterExt)(UINT, LPCTSTR, HBITMAP);
extern STEP_API UINT (WINAPI *STEPKeyAssign)(UINT, LPCTSTR, LPCTSTR);
extern STEP_API const char* (WINAPI *STEPGetGenreNameSIF)(BYTE byGenre);
extern STEP_API BYTE (WINAPI *STEPGetGenreCode)(const char* szGenre);
extern STEP_API bool (WINAPI *STEPIsUserGenre)(const char* szGenre);
extern STEP_API int (WINAPI *STEPGetNumericTrackNumber)(const char* szTrackNumber, char* szNumericNumber, int size); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetIntegerTrackNumber)(const char* szTrackNumber); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetNumericDiskNumber)(const char* szDiskNumber, char* szNumericNumber, int size); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetIntegerDiskNumber)(const char* szDiskNumber); /* STEP 037 */

extern STEP_API void (WINAPI *STEPProcessSelectedFiles)(LPCTSTR, STEPProcessSelectedFilesCB);
extern STEP_API void (WINAPI *STEPProcessSelectedFilesForUpdate)(LPCTSTR, STEPProcessSelectedFilesCB);

extern STEP_API bool (WINAPI *STEPIsRangeSelected)(void);
extern STEP_API int (WINAPI *STEPGetSelectedCount)(void);
extern STEP_API int (WINAPI *STEPGetSelectedItem)(void);
extern STEP_API bool (WINAPI *STEPIsCurrentCellEditOK)(void);
extern STEP_API bool (WINAPI *STEPItemHasChildren)(int);
extern STEP_API bool (WINAPI *STEPGetSelectedRange)(int*, int*, int* , int*);
extern STEP_API bool (WINAPI *STEPIsItemFile)(int);
extern STEP_API void (WINAPI *STEPChangeSubItemText)(int nItem, int nColumn, LPCTSTR szText);
extern STEP_API const char* (WINAPI *STEPGetSubItemText)(int nItem, int nColumn);
extern STEP_API void (WINAPI *STEPGetFileInfo)(int nItem, FILE_INFO* info);
extern STEP_API UINT (WINAPI *STEPGETColumnType)(int nColumn);
extern STEP_API bool (WINAPI *STEPIsCurrentCellEditOK)(void);
extern STEP_API bool (WINAPI *STEPIsNumeric)(const char* szText); /* STEP 037 */
extern STEP_API void (WINAPI *STEPConvSiFieldToId3tag)(FILE_INFO* pFileInfo); /* STEP 037 */

extern STEP_API void (WINAPI *STEPInitDataSIF)(FILE_INFO* info);
extern STEP_API void (WINAPI *STEPInitDataID3)(FILE_INFO* info); /* STEP 029 */
extern STEP_API void (WINAPI *STEPInitData)(FILE_INFO* info);
extern STEP_API bool (WINAPI *STEPFileNameChange)(FILE_INFO* pFileInfo, LPCTSTR szNewFileName);
extern STEP_API bool (WINAPI *STEPWriteTag)(FILE_INFO* pFileInfo);

extern STEP_API bool (WINAPI *STEPUpdateCellInfo)(void);
}

extern bool Initialize(void);
extern void Finalize(void);

/* タグ情報の取得／設定 */
extern LPCTSTR GetFullPath(FILE_INFO* info);
extern LPCTSTR GetFileName(FILE_INFO* info);
extern LPCTSTR GetFilePath(FILE_INFO* info);
extern LPCTSTR GetVisualFormat(FILE_INFO* info);
extern LPCTSTR GetAudioFormat(FILE_INFO* info);
extern LPCTSTR GetTrackName(FILE_INFO* info);
extern LPCTSTR GetArtistName(FILE_INFO* info);
extern LPCTSTR GetAlbumName(FILE_INFO* info);
extern LPCTSTR GetComment(FILE_INFO* info);
extern LPCTSTR GetYear(FILE_INFO* info);
extern LPCTSTR GetTrackNumber(FILE_INFO* info);
extern LPCTSTR GetDiskNumber(FILE_INFO* info);
extern LPCTSTR GetGenre(FILE_INFO* info);
extern LPCTSTR GetTrackNameSI(FILE_INFO* info);
extern LPCTSTR GetArtistNameSI(FILE_INFO* info);
extern LPCTSTR GetAlbumNameSI(FILE_INFO* info);
extern LPCTSTR GetCommentSI(FILE_INFO* info);
extern LPCTSTR GetYearSI(FILE_INFO* info);
extern LPCTSTR GetTrackNumberSI(FILE_INFO* info);
extern LPCTSTR GetDiskNumberSI(FILE_INFO* info);
extern LPCTSTR GetGenreSI(FILE_INFO* info);
extern LPCTSTR GetCopyrightSI(FILE_INFO* info);
extern LPCTSTR GetEngineerSI(FILE_INFO* info);
extern LPCTSTR GetSourceSI(FILE_INFO* info);
extern LPCTSTR GetSoftwareSI(FILE_INFO* info);
extern LPCTSTR GetKeywordSI(FILE_INFO* info);
extern LPCTSTR GetTechnicianSI(FILE_INFO* info);
extern LPCTSTR GetLyricSI(FILE_INFO* info);
extern LPCTSTR GetCommissionSI(FILE_INFO* info);
extern LPCTSTR GetWriterSI(FILE_INFO* info);
extern LPCTSTR GetComposerSI(FILE_INFO* info);
extern LPCTSTR GetAlbumArtistSI(FILE_INFO* info);
extern LPCTSTR GetOrigArtistSI(FILE_INFO* info);
extern LPCTSTR GetURLSI(FILE_INFO* info);
extern LPCTSTR GetEncodest(FILE_INFO* info);
extern LPCTSTR GetOther(FILE_INFO* info);
extern LPCTSTR GetFileTypeName(FILE_INFO* info);
extern void SetFullPathName(FILE_INFO* info, LPCTSTR szValue);
extern void SetFileName(FILE_INFO* info, LPCTSTR szValue);
extern void SetFilePath(FILE_INFO* info, LPCTSTR szValue);
extern void SetVisualFormat(FILE_INFO* info, LPCTSTR szValue);
extern void SetAudioFormat(FILE_INFO* info, LPCTSTR szValue);
extern void SetTrackName(FILE_INFO* info, LPCTSTR szValue);
extern void SetArtistName(FILE_INFO* info, LPCTSTR szValue);
extern void SetAlbumName(FILE_INFO* info, LPCTSTR szValue);
extern void SetComment(FILE_INFO* info, LPCTSTR szValue);
extern void SetYear(FILE_INFO* info, LPCTSTR szValue);
extern void SetTrackNumber(FILE_INFO* info, LPCTSTR szValue);
extern void SetGenre(FILE_INFO* info, LPCTSTR szValue);
extern void SetTrackNameSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetArtistNameSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetAlbumNameSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetCommentSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetYearSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetTrackNumberSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetDiskNumberSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetGenreSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetCopyrightSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetEngineerSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetSourceSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetSoftwareSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetKeywordSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetTechnicianSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetLyricSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetCommissionSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetWriterSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetComposerSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetAlbumArtistSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetOrigArtistSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetURLSI(FILE_INFO* info, LPCTSTR szValue);
extern void SetEncodest(FILE_INFO* info, LPCTSTR szValue);
extern void SetOther(FILE_INFO* info, LPCTSTR szValue);
extern void SetFileTypeName(FILE_INFO* info, LPCTSTR szValue);

extern bool isModify(FILE_INFO*);
extern void SetModifyFlag(FILE_INFO* pInfo, bool bModify);
extern bool isChecked(FILE_INFO* pInfo);
extern void SetCheckFlag(FILE_INFO* pInfo, bool bCheck);
extern bool isExistLyricFile(FILE_INFO* pInfo);
extern void SetExistLyricFile(FILE_INFO* pInfo, bool bExist);
extern long GetFileSize(FILE_INFO* pInfo);
extern void SetFileSize(FILE_INFO* pInfo, long size);
extern UINT GetFormat(FILE_INFO* pInfo);
extern void SetFormat(FILE_INFO* pInfo, UINT nFormat);
extern int  GetPlayTime(FILE_INFO* pInfo);
extern void SetPlayTime(FILE_INFO* pInfo, int nPlayTime);
extern BYTE GetBTrackNumber(FILE_INFO* pInfo);
extern void SetBTrackNumber(FILE_INFO* pInfo, BYTE nTrackNumber);
extern BYTE GetBDiskNumber(FILE_INFO* pInfo);
extern void SetBDiskNumber(FILE_INFO* pInfo, BYTE nDiskNumber);
extern BYTE GetBGenre(FILE_INFO* pInfo);
extern void SetBGenre(FILE_INFO* pInfo, BYTE nGenre);
extern void* GetFileSpecificInfo(FILE_INFO* pInfo);
extern void SetFileSpecificInfo(FILE_INFO* pInfo, void* info);

extern LPCTSTR GetValue(FILE_INFO* pInfo, FIELDTYPE nField);
extern void SetValue(FILE_INFO* pInfo, FIELDTYPE nField, LPCTSTR szValue);

#endif