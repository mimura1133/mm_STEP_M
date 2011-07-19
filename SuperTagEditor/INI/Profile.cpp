/**************************************************************************

	Profile.c

	(C) Copyright 1996-2002 By Tomoaki Nakashima. All right reserved.
		http://www.nakka.com/
		nakka@nakka.com

**************************************************************************/

/**************************************************************************
	Include Files
**************************************************************************/
#include "stdafx.h"

#define _INC_OLE
#include <windows.h>
#undef  _INC_OLE

#include "Profile.h"

/**************************************************************************
	Define
**************************************************************************/

#define BUF_SIZE				256
#define ALLOC_CNT				10

#ifdef UNICODE
#define a2i						_wtoi
#else
#define a2i						atoi
#endif

#ifndef CopyMemory
#define CopyMemory				memcpy
#endif


/**************************************************************************
	Global Variables
**************************************************************************/

typedef struct _KEY {
	TCHAR KeyName[BUF_SIZE];
	int hash;
	TCHAR *String;
	BOOL CommentFlag;
} KEY_INFO;

typedef struct _SECTION {
	TCHAR SectionName[BUF_SIZE];
	int hash;
	KEY_INFO *KeyInfo;
	int KeyCnt;
	int KeyAllocCnt;
} SECTION_INFO;


static SECTION_INFO *SectionInfo;
static int SectionCnt;
static int SectionAllocCnt;


/**************************************************************************
	Local Function Prototypes
**************************************************************************/

static BOOL Trim(TCHAR *buf);
static int str2hash(const TCHAR *str);
static TCHAR *StrCpy_Profile(TCHAR *ret, const TCHAR *buf);
static BOOL WriteAsciiFile(const HANDLE hFile, const TCHAR *buf, const int len);
static BOOL AddSection(const TCHAR *SectionName);
static int FindSection(const TCHAR *SectionName);
static BOOL AddKey(SECTION_INFO *si, const TCHAR *KeyName, const TCHAR *String, const BOOL cFlag);
static int FindKey(const SECTION_INFO *si, const TCHAR *KeyName);
static BOOL Profile_WriteData(const TCHAR *Section, const TCHAR *Key, const TCHAR *str, const TCHAR *File);


/******************************************************************************

	Trim

	文字列の前後の空白, Tabを除去する

******************************************************************************/

static BOOL Trim(TCHAR *buf)
{
	TCHAR *p, *r;

	//前後の空白を除いたポインタを取得
	for(p = buf; (*p == TEXT(' ') || *p == TEXT('\t')) && *p != TEXT('\0'); p++);
	for(r = buf + lstrlen(buf) - 1; r > p && (*r == TEXT(' ') || *r == TEXT('\t')); r--);
	*(r + 1) = TEXT('\0');

	//元の文字列にコピーを行う
	lstrcpy(buf, p);
	return TRUE;
}


/******************************************************************************

	str2hash

	文字列のハッシュ値を取得

******************************************************************************/

static int str2hash(const TCHAR *str)
{
#define ToLower(c)		((c >= TEXT('A') && c <= TEXT('Z')) ? (c - TEXT('A') + TEXT('a')) : c)
	int hash = 0;

	for(; *str != TEXT('\0'); str++){
		hash = ((hash << 1) + ToLower(*str));
	}
	return hash;
}


/******************************************************************************

	StrCpy

	文字列をコピーして最後の文字のアドレスを返す

******************************************************************************/

static TCHAR *StrCpy_Profile(TCHAR *ret, const TCHAR *buf)
{
	if(buf == NULL){
		*ret = TEXT('\0');
		return ret;
	}
	while(*(ret++) = *(buf++));
	ret--;
	return ret;
}


/******************************************************************************

	WriteAsciiFile

	マルチバイトに変換して保存

******************************************************************************/

static BOOL WriteAsciiFile(const HANDLE hFile, const TCHAR *buf, const int len)
{
#ifdef UNICODE
	char *str;
	DWORD ret;
	int clen;

	clen = WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
	str = (char *)LocalAlloc(LMEM_FIXED, clen + 1);
	if(str == NULL){
		return FALSE;
	}
	WideCharToMultiByte(CP_ACP, 0, buf, -1, str, clen, NULL, NULL);
	if(WriteFile(hFile, str, clen - 1, &ret, NULL) == FALSE){
		LocalFree(str);
		return FALSE;
	}
	LocalFree(str);
	return TRUE;
#else
	DWORD ret;

	return WriteFile(hFile, buf, len, &ret, NULL);
#endif
}


/******************************************************************************

	AddSection

	セクションの追加

******************************************************************************/

static BOOL AddSection(const TCHAR *SectionName)
{
	SECTION_INFO *TmpSection;

	if(SectionName == NULL || *SectionName == TEXT('\0')){
		return FALSE;
	}

	if(SectionAllocCnt < SectionCnt + 1){
		//再確保
		SectionAllocCnt += ALLOC_CNT;
		TmpSection = (SECTION_INFO *)LocalAlloc(LPTR, sizeof(SECTION_INFO) * SectionAllocCnt);
		if(TmpSection == NULL){
			return FALSE;
		}
		if(SectionInfo != NULL){
			CopyMemory(TmpSection, SectionInfo, sizeof(SECTION_INFO) * SectionCnt);
			LocalFree(SectionInfo);
		}
		SectionInfo = TmpSection;
	}
	//セクション追加
	lstrcpyn((SectionInfo + SectionCnt)->SectionName, SectionName, BUF_SIZE);
	Trim((SectionInfo + SectionCnt)->SectionName);
	(SectionInfo + SectionCnt)->hash = str2hash((SectionInfo + SectionCnt)->SectionName);

	SectionCnt++;
	return TRUE;
}


/******************************************************************************

	FindSection

	セクションの検索

******************************************************************************/

static int FindSection(const TCHAR *SectionName)
{
	int hash;
	int i;

	if(SectionInfo == NULL || SectionName == NULL || *SectionName == TEXT('\0')){
		return -1;
	}

	hash = str2hash(SectionName);
	for(i = 0; i < SectionCnt; i++){
		if((SectionInfo + i)->hash != hash){
			continue;
		}
		if(lstrcmpi((SectionInfo + i)->SectionName, SectionName) == 0){
			return i;
		}
	}
	return -1;
}


/******************************************************************************

	AddKey

	キーの追加

******************************************************************************/

static BOOL AddKey(SECTION_INFO *si, const TCHAR *KeyName, const TCHAR *String, const BOOL cFlag)
{
	KEY_INFO *TmpKey;
	int index = -1;

	if(KeyName == NULL || *KeyName == TEXT('\0') || String == NULL){
		return FALSE;
	}

	if(si->KeyAllocCnt < si->KeyCnt + 1){
		//再確保
		si->KeyAllocCnt += ALLOC_CNT;
		TmpKey = (KEY_INFO *)LocalAlloc(LPTR, sizeof(KEY_INFO) * si->KeyAllocCnt);
		if(TmpKey == NULL){
			return FALSE;
		}
		if(si->KeyInfo != NULL){
			CopyMemory(TmpKey, si->KeyInfo, sizeof(KEY_INFO) * si->KeyCnt);
			LocalFree(si->KeyInfo);
		}
		si->KeyInfo = TmpKey;
	}
	//キー追加
	(si->KeyInfo + si->KeyCnt)->String = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (lstrlen(String) + 1));
	if((si->KeyInfo + si->KeyCnt)->String == NULL){
		return FALSE;
	}
	lstrcpyn((si->KeyInfo + si->KeyCnt)->KeyName, KeyName, BUF_SIZE);
	Trim((si->KeyInfo + si->KeyCnt)->KeyName);
	if(cFlag == FALSE){
		(si->KeyInfo + si->KeyCnt)->hash = str2hash((si->KeyInfo + si->KeyCnt)->KeyName);
	}
	lstrcpy((si->KeyInfo + si->KeyCnt)->String, String);
	(si->KeyInfo + si->KeyCnt)->CommentFlag = cFlag;

	si->KeyCnt++;
	return TRUE;
}


/******************************************************************************

	FindKey

	キーの検索

******************************************************************************/

static int FindKey(const SECTION_INFO *si, const TCHAR *KeyName)
{
	int hash;
	int i;

	if(si->KeyInfo == NULL || KeyName == NULL || *KeyName == TEXT('\0')){
		return -1;
	}

	hash = str2hash(KeyName);
	for(i = 0; i < si->KeyCnt; i++){
		if((si->KeyInfo + i)->CommentFlag == TRUE ||
			(si->KeyInfo + i)->hash != hash){
			continue;
		}
		if(lstrcmpi((si->KeyInfo + i)->KeyName, KeyName) == 0){
			return i;
		}
	}
	return -1;
}


/******************************************************************************

	Profile_Initialize

	初期化

******************************************************************************/

BOOL Profile_Initialize(const TCHAR *path, const BOOL ReadFlag)
{
	HANDLE hFile;
	TCHAR *buf, *p, *r, *s;
	TCHAR tmp[BUF_SIZE];
	char *cBuf;
	DWORD fSizeLow, fSizeHigh;
	DWORD ret;
	long FileSize;
#ifdef UNICODE
	long Len;
#endif

	if(ReadFlag == FALSE){
		SectionInfo = NULL;
		return TRUE;
	}

	//ファイルを開く
	hFile = CreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL || hFile == (HANDLE)-1){
		return FALSE;
	}
	// 確保するサイズの取得
	fSizeLow = GetFileSize(hFile, &fSizeHigh);
	if(fSizeLow == 0xFFFFFFFF){
		CloseHandle(hFile);
		return FALSE;
	}
	FileSize = (long)fSizeLow;

	//読み取る領域の確保
	cBuf = (char *)LocalAlloc(LMEM_FIXED, FileSize + 1);
	if(cBuf == NULL){
		CloseHandle(hFile);
		return FALSE;
	}
	//ファイルを読み込む
	if(ReadFile(hFile, cBuf, fSizeLow, &ret, NULL) == FALSE){
		LocalFree(cBuf);
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	*(cBuf + FileSize) = '\0';

#ifdef UNICODE
	Len = MultiByteToWideChar(CP_ACP, 0, cBuf, -1, NULL, 0);
	buf = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (Len + 1));
	if(buf == NULL){
		LocalFree(cBuf);
		CloseHandle(hFile);
		return FALSE;
	}
	MultiByteToWideChar(CP_ACP, 0, cBuf, -1, buf, Len);
	FileSize = Len;
	LocalFree(cBuf);
#else
	buf = cBuf;
#endif

	//セクションの確保
	SectionInfo = (SECTION_INFO *)LocalAlloc(LPTR, sizeof(SECTION_INFO) * ALLOC_CNT);
	if(SectionInfo == NULL){
		LocalFree(buf);
		return FALSE;
	}
	SectionCnt = 1;
	SectionAllocCnt = ALLOC_CNT;

	p = buf;
	while((FileSize > (p - buf)) && *p != TEXT('\0')){
		for(r = p; (FileSize > (r - buf)) && (*r != TEXT('\r') && *r != TEXT('\n')); r++);

		switch(*p)
		{
		case TEXT('['):
			//セクションの追加
			if(p == r || *(r - 1) != TEXT(']')) break;
			*(r - 1) = TEXT('\0');
			AddSection(p + 1);
			break;

		case TEXT('\r'):
		case TEXT('\n'):
			break;

		default:
			if(SectionInfo == NULL || p == r) break;
			if(*p == TEXT('#')){
				//コメント
				for(s = tmp; p < r; p++, s++){
					*s = *p;
				}
				*s = TEXT('\0');
				AddKey((SectionInfo + SectionCnt - 1), tmp, TEXT(""), TRUE);
			}else{
				//キーの追加
				for(s = tmp; p < r; p++, s++){
					if(*p == TEXT('=')) break;
					*s = *p;
				}
				*s = TEXT('\0');
				if(*p == TEXT('=')) p++;
				*r = TEXT('\0');
				AddKey((SectionInfo + SectionCnt - 1), tmp, p, FALSE);
			}

			if(FileSize > (r - buf)) r++;
		}
		p = r;
		for(; (FileSize > (p - buf)) && (*p == TEXT('\r') || *p == TEXT('\n')); p++);
	}
	LocalFree(buf);
	return TRUE;
}


/******************************************************************************

	Profile_Flush

	バッファをファイルに書き込む

******************************************************************************/

BOOL Profile_Flush(const TCHAR *path)
{
	HANDLE hFile;
	TCHAR *buf, *p;
	int len;
	int i, j;

	if(SectionInfo == NULL){
		return FALSE;
	}

	//保存サイズの計算
	len = 0;
	for(i = 0; i < SectionCnt; i++){
		if((SectionInfo + i)->KeyInfo == NULL){
			continue;
		}

		//セクション名
		if(i != 0){
			len += lstrlen((SectionInfo + i)->SectionName) + 2 + 2;
		}

		for(j = 0; j < (SectionInfo + i)->KeyCnt; j++){
			if(*((SectionInfo + i)->KeyInfo + j)->KeyName == TEXT('\0')){
				continue;
			}
			//キー名
			len += lstrlen(((SectionInfo + i)->KeyInfo + j)->KeyName);
			if(((SectionInfo + i)->KeyInfo + j)->CommentFlag == FALSE){
				len++;
				if(((SectionInfo + i)->KeyInfo + j)->String != NULL){
					//文字列
					len += lstrlen(((SectionInfo + i)->KeyInfo + j)->String);
				}
			}
			len += 2;
		}
		len += 2;
	}

	//保存するための領域の確保
	p = buf = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (len + 1));
	if(buf == NULL){
		return FALSE;
	}

	//保存文字列の作成
	for(i = 0; i < SectionCnt; i++){
		if((SectionInfo + i)->KeyInfo == NULL){
			continue;
		}

		//セクション名
		if(i != 0){
			*(p++) = TEXT('[');
			p = StrCpy_Profile(p, (SectionInfo + i)->SectionName);
			p = StrCpy_Profile(p, TEXT("]\r\n"));
		}

		for(j = 0; j < (SectionInfo + i)->KeyCnt; j++){
			if(*((SectionInfo + i)->KeyInfo + j)->KeyName == TEXT('\0')){
				continue;
			}
			//キー名
			p = StrCpy_Profile(p, ((SectionInfo + i)->KeyInfo + j)->KeyName);
			if(((SectionInfo + i)->KeyInfo + j)->CommentFlag == FALSE){
				*(p++) = TEXT('=');

				if(((SectionInfo + i)->KeyInfo + j)->String != NULL){
					//文字列
					p = StrCpy_Profile(p, ((SectionInfo + i)->KeyInfo + j)->String);
				}
			}
			p = StrCpy_Profile(p, TEXT("\r\n"));
		}
		p = StrCpy_Profile(p, TEXT("\r\n"));
	}
	*p = TEXT('\0');

	//ファイルを開く
	hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL || hFile == (HANDLE)-1){
		LocalFree(buf);
		return FALSE;
	}

	//ファイルに書き込む
	if(WriteAsciiFile(hFile, buf, len) == FALSE){
		LocalFree(buf);
		CloseHandle(hFile);
		return FALSE;
	}

	//メモリの解放
	LocalFree(buf);
	//ファイルを閉じる
	CloseHandle(hFile);
	return TRUE;
}


/******************************************************************************

	Profile_Free

	設定情報の解放

******************************************************************************/

void Profile_Free(void)
{
	int i, j;

	if(SectionInfo == NULL){
		return;
	}
	for(i = 0; i < SectionCnt; i++){
		if((SectionInfo + i)->KeyInfo == NULL){
			continue;
		}
		//キーの解放
		for(j = 0; j < (SectionInfo + i)->KeyCnt; j++){
			if(((SectionInfo + i)->KeyInfo + j)->String != NULL){
				LocalFree(((SectionInfo + i)->KeyInfo + j)->String);
			}
		}
		LocalFree((SectionInfo + i)->KeyInfo);
	}
	LocalFree(SectionInfo);
	SectionInfo = NULL;
	SectionCnt = 0;
	SectionAllocCnt = 0;
}


/******************************************************************************

	Profile_GetString

	文字列の取得

******************************************************************************/

long Profile_GetString(const TCHAR *Section, const TCHAR *Key, const TCHAR *Default, TCHAR *ret, const long size, const TCHAR *File)
{
	TCHAR *buf, *p;
	int SectionIndex;
	int KeyIndex;
	int len;

	//セクションの検索
	SectionIndex = FindSection(Section);
	if(SectionIndex == -1){
		lstrcpyn(ret, Default, size);
		return lstrlen(ret);
	}

	//キーの検索
	KeyIndex = FindKey((SectionInfo + SectionIndex), Key);
	if(KeyIndex == -1 || ((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String == NULL){
		lstrcpyn(ret, Default, size);
		return lstrlen(ret);
	}

	//内容の取得
	buf = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (lstrlen(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String) + 1));
	if(buf != NULL){
		lstrcpy(buf, ((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String);
		Trim(buf);
		p = (*buf == TEXT('\"')) ? buf + 1 : buf;
		len = lstrlen(p);
		if(len > 0){
			if(*(p + len - 1) == TEXT('\"')) *(p + len - 1) = TEXT('\0');
		}
		lstrcpyn(ret, p, size);
		LocalFree(buf);
	}else{
		lstrcpyn(ret, ((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String, size);
	}
	return lstrlen(ret);
}


/******************************************************************************

	Profile_GetInt

	数値の取得

******************************************************************************/

int Profile_GetInt(const TCHAR *Section, const TCHAR *Key, const int Default, const TCHAR *File)
{
	TCHAR *buf, *p;
	int SectionIndex;
	int KeyIndex;
	int ret;
	int len;

	//セクションの検索
	SectionIndex = FindSection(Section);
	if(SectionIndex == -1){
		return Default;
	}

	//キーの検索
	KeyIndex = FindKey((SectionInfo + SectionIndex), Key);
	if(KeyIndex == -1 || ((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String == NULL){
		return Default;
	}

	//内容の取得
	buf = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (lstrlen(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String) + 1));
	if(buf != NULL){
		lstrcpy(buf, ((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String);
		Trim(buf);
		p = (*buf == TEXT('\"')) ? buf + 1 : buf;
		len = lstrlen(p);
		if(len > 0){
			if(*(p + len - 1) == TEXT('\"')) *(p + len - 1) = TEXT('\0');
		}
		ret = a2i(p);
		LocalFree(buf);
	}else{
		ret = a2i(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String);
	}
	return ret;
}


/******************************************************************************

	Profile_WriteData

	データの書き込み

******************************************************************************/

static BOOL Profile_WriteData(const TCHAR *Section, const TCHAR *Key, const TCHAR *str, const TCHAR *File)
{
	int SectionIndex;
	int KeyIndex;
	int j;

	if(Section == NULL){
		return FALSE;
	}

	if(SectionInfo == NULL){
		//セクションの確保
		SectionInfo = (SECTION_INFO *)LocalAlloc(LPTR, sizeof(SECTION_INFO) * ALLOC_CNT);
		if(SectionInfo == NULL){
			return FALSE;
		}
		SectionCnt = 1;
		SectionAllocCnt = ALLOC_CNT;
	}

	//セクションの検索
	SectionIndex = FindSection(Section);
	if(SectionIndex == -1){
		//セクションの追加
		if(AddSection(Section) == FALSE){
			return FALSE;
		}
		SectionIndex = SectionCnt - 1;
	}

	if(Key == NULL){
		if((SectionInfo + SectionIndex)->KeyInfo != NULL){
			//キーの削除
			for(j = 0; j < (SectionInfo + SectionIndex)->KeyCnt; j++){
				if(((SectionInfo + SectionIndex)->KeyInfo + j)->String != NULL){
					LocalFree(((SectionInfo + SectionIndex)->KeyInfo + j)->String);
				}
			}
			LocalFree((SectionInfo + SectionIndex)->KeyInfo);
			(SectionInfo + SectionIndex)->KeyInfo = NULL;
			(SectionInfo + SectionIndex)->KeyCnt = 0;
			(SectionInfo + SectionIndex)->KeyAllocCnt = 0;
		}
		return TRUE;
	}

	//キーの検索
	KeyIndex = FindKey((SectionInfo + SectionIndex), Key);
	if(KeyIndex == -1){
		//キーの追加
		return AddKey((SectionInfo + SectionIndex), Key, str, FALSE);
	}else{
		//内容の変更
		if(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String != NULL){
			LocalFree(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String);
		}
		if(str == NULL){
			*((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->KeyName = TEXT('\0');
			((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String = NULL;
			return TRUE;
		}
		((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (lstrlen(str) + 1));
		if(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String == NULL){
			return FALSE;
		}
		lstrcpy(((SectionInfo + SectionIndex)->KeyInfo + KeyIndex)->String, str);
	}
	return TRUE;
}


/******************************************************************************

	Profile_WriteString

	文字列の書き込み

******************************************************************************/

BOOL Profile_WriteString(const TCHAR *Section, const TCHAR *Key, const TCHAR *str, const TCHAR *File)
{
	TCHAR *buf, *p;
	BOOL ret;

	if(str == NULL || *str == TEXT('\0')){
		return Profile_WriteData(Section, Key, TEXT(""), File);
	}

	buf = (TCHAR *)LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (lstrlen(str) + 3));
	if(buf == NULL){
		return Profile_WriteData(Section, Key, str, File);
	}
	p = StrCpy_Profile(buf, TEXT("\""));
	p = StrCpy_Profile(p, str);
	p = StrCpy_Profile(p, TEXT("\""));
	ret = Profile_WriteData(Section, Key, buf, File);
	LocalFree(buf);
	return ret;
}


/******************************************************************************

	Profile_WriteInt

	数値の書き込み

******************************************************************************/

BOOL Profile_WriteInt(const TCHAR *Section, const TCHAR *Key, const int num, const TCHAR *File)
{
	TCHAR ret[BUF_SIZE];

	wsprintf(ret, TEXT("%d"), num);
	return Profile_WriteData(Section, Key, ret, File);
}
/* End of source */
