#include "stdafx.h"
#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include <string.h>
#include "FileOGG.h"
#include "vorbis/vorbisfile.h"
#include "VorbisComments.h"
#include "vcedit.h"
#include "utf8.h"

#include "Tag_Ogg.h"

size_t win32_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	HANDLE file = (HANDLE) datasource;
	DWORD bytesread;
	size_t result;

	if (ReadFile(file, ptr, (DWORD) (size*nmemb), &bytesread, NULL))
		result = bytesread/size;
	else
		result = 0; /* It failed */

	return result;
}
/* The win32 equivalent of the ANSI C fwrite for a HANDLE */
size_t win32_write_func(const void *ptr, size_t size, size_t nmemb, void *datasource)
{
	HANDLE file = (HANDLE) datasource;
	DWORD byteswritten;
	size_t result;

	if (WriteFile(file, ptr, (DWORD) (size*nmemb), &byteswritten, NULL))
		result = byteswritten/size;
	else
		result = 0; /* It failed */

	return result;
}
/* The win32 equivalent of the ANSI C fseek for a HANDLE */
int win32_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	HANDLE file = (HANDLE) datasource;
	int seek_type;
	unsigned long retval;
	int seek_highword = (int) (offset >> 32);

	switch(whence)
	{
		case SEEK_SET:
			seek_type = FILE_BEGIN;
			break;
		case SEEK_CUR:
			seek_type = FILE_CURRENT;
			break;
		case SEEK_END:
			seek_type = FILE_END;
			break;
	}

	/* On failure, SetFilePointer returns 0xFFFFFFFF, which is (int)-1 */
	retval = SetFilePointer(file, (int) (offset & 0xffffffff), (long*)&seek_highword, seek_type);

	/* Exactly mimic stdio return values */
	return retval == 0xFFFFFFFF ? -1 : 0;
}
/* The win32 equivalent of the ANSI C fclose for a HANDLE */
int win32_close_func(void *datasource)
{
	HANDLE file = (HANDLE) datasource;
	/* Return value meaning is inverted from fclose() */
	return (CloseHandle(file) ? 0 : EOF);
}
/* The win32 equivalent of the ANSI C ftell for a HANDLE */
long win32_tell_func(void *datasource)
{
	HANDLE file = (HANDLE) datasource;
	/* This returns the right number */
	return (long) SetFilePointer(file, 0, NULL, FILE_CURRENT);
}

bool LoadAttributeFileOGG(FILE_INFO *pFileMP3)
{
	//if (fileOGG != NULL)	delete fileOGG;
	SetFileSpecificInfo(pFileMP3, new CTag_Ogg());
	CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
	if (fileOGG->Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}

	CString strTrackName;
	CString strArtistName;
	CString strAlbumName;
	CString strGenre;
	CString strYear;
	CString strComment;
	CString strTrackNumber;
    struct _VORBISCOMMENT_{
        char *szField;
        CString *pstrContents;
    }VC[] = {
        {"TITLE",       &strTrackName}, //タイトル
        {"ARTIST",      &strArtistName},//アーティスト
        {"ALBUM",       &strAlbumName}, //アルバム
        {"GENRE",       &strGenre},     //ジャンル
        {"DATE",        &strYear},      //年
        {"COMMENT",     &strComment},   //コメント
        {"TRACKNUMBER", &strTrackNumber},         //トラック番号
        {NULL,          NULL}
    };
    int i = 0;
    while(VC[i].szField){
		fileOGG->GetComment(VC[i].szField, 0, *VC[i].pstrContents);
		i++;
	}
	SetTrackNameSI(pFileMP3, strTrackName);
	SetArtistNameSI(pFileMP3, strArtistName);
	SetAlbumNameSI(pFileMP3, strAlbumName);
	SetGenreSI(pFileMP3, strGenre);
	SetYearSI(pFileMP3, strYear);
	SetCommentSI(pFileMP3, strComment);
	SetTrackNumberSI(pFileMP3, strTrackNumber);
    //トラック番号
	//if (!pFileMP3->strTrackNumber.IsEmpty()) {
	//	pFileMP3->byTrackNumber = (BYTE)atoi(CId3tagv2::GetIntTrackNo(pFileMP3->strTrackNumber));
	//}

//	pFileMP3->nPlayTime = fileOGG->GetTime();
//	pFileMP3->strBitRate = fileOGG->GetAudioFormatString();

	{ // 非標準タグ
		CString strDisp;
		CStringArray strArray;
		fileOGG->GetCommentNames(strArray);
		long items = 0;
		int i; for(i=0; i<strArray.GetSize(); i++)
		{
			CString strName = strArray.GetAt(i);
			CString strValue;
			CString _strValue;
			int j = 0;
			while(1)
			{
				if(!fileOGG->GetComment(strName,j++,strValue))
				{
					break;
				}

				//標準名は除外
				if((j == 1) &&
					(
					!strName.Compare("TITLE") ||
					!strName.Compare("TRACKNUMBER") ||
					!strName.Compare("ARTIST") ||
					!strName.Compare("ALBUM") ||
					!strName.Compare("DATE") ||
					!strName.Compare("GENRE") ||
					!strName.Compare("COMMENT"))
					)
				{
					continue;
				}
				if (strDisp.IsEmpty()) {
				} else {
					strDisp += "\r\n";
				}
				strDisp += "[" + strName + "] " + strValue;
				items++;
			}
		}
		SetOther(pFileMP3, strDisp);
	}
	extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
	GetValues_mp3infp(pFileMP3);
	return true;
}

bool WriteAttributeFileOGG(FILE_INFO *pFileMP3)
{
	CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
    //CString strTrackNumber;//トラック番号
    //if(pFileMP3->byTrackNumber != 0xFF && pFileMP3->strTrackNumber.IsEmpty()/* Conspiracy 196 */){
    //    char szTrackNumber[64];
    //    sprintf(szTrackNumber, "%d", (unsigned int)pFileMP3->byTrackNumber);
    //    strTrackNumber = szTrackNumber;
    //} else if (!pFileMP3->strTrackNumber.IsEmpty()) { /* Conspiracy 196 */
	//	strTrackNumber = pFileMP3->strTrackNumber;
	//}
	CString strTrackName = GetTrackNameSI(pFileMP3);
	CString strArtistName = GetArtistNameSI(pFileMP3);
	CString strAlbumName = GetAlbumNameSI(pFileMP3);
	CString strGenre = GetGenreSI(pFileMP3);
	CString strYear = GetYearSI(pFileMP3);
	CString strComment = GetCommentSI(pFileMP3);
	CString strTrackNumber = GetTrackNumberSI(pFileMP3);
    struct _VORBISCOMMENT_{
        char *szField;
        CString *pstrContents;
    }VC[] = {
        {"TITLE",       &strTrackName}, //タイトル
        {"ARTIST",      &strArtistName},//アーティスト
        {"ALBUM",       &strAlbumName}, //アルバム
        {"GENRE",       &strGenre},     //ジャンル
        {"DATE",        &strYear},      //年
        {"COMMENT",     &strComment},   //コメント
        {"TRACKNUMBER", &strTrackNumber},         //トラック番号
        {NULL,          NULL}
    };
    int i = 0;
    while(VC[i].szField){
		fileOGG->DelComment(VC[i].szField, 0);
		if (strlen(*VC[i].pstrContents) > 0) {
			fileOGG->AddComment(VC[i].szField, *VC[i].pstrContents);
		}
		//fileOGG->ReplaceComment(VC[i].szField, *VC[i].pstrContents, 0);
		i++;
	}
	if (fileOGG->Save(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}