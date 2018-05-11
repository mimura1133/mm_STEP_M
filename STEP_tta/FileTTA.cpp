#include "stdafx.h"
#include <windows.h>
#include <mmreg.h>
#include <time.h>
#include "id3tag.h"
#include "crc32.h"
#include "ttadec.h"
#include "STEPlugin.h"

#include "ID3v2/Id3tagv1.h"
#include "ID3v2/Id3tagv2.h"

extern bool bOptID3TagAutoWrite;
extern bool bOptID3TagAutoDelete;

//HANDLE heap;
#define	heap GetProcessHeap()

static void get_id3v1_tag (tta_info *ttainfo);
static void get_id3v2_tag (tta_info *ttainfo);
static bool del_id3v1_tag (tta_info *ttainfo);
//static void del_id3v2_tag (tta_info *ttainfo);

static int open_tta_file (LPCTSTR filename, tta_info *ttainfo) {
	tta_hdr ttahdr;
	unsigned long checksum;
	unsigned long datasize;
	unsigned long origsize;
	DWORD result;

	// clear the memory
	ZeroMemory (ttainfo, sizeof(tta_info));

	ttainfo->HFILE = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (ttainfo->HFILE == INVALID_HANDLE_VALUE) {
		ttainfo->STATE = OPEN_ERROR;
		return -1;
	}

	// get file size
	ttainfo->FILESIZE = GetFileSize(ttainfo->HFILE, NULL);
#if UNICODE
	{
		TCHAR tmp[MAX_PATH];
		if (GetShortPathName(filename, tmp, MAX_PATH)) {
			lstrcpynA((char*)ttainfo->filename, CStringA(tmp), MAX_PATH - 1);
		} else {
			lstrcpynA((char*)ttainfo->filename, CStringA(filename), MAX_PATH - 1);
		}
	}
#else
	lstrcpyn((char*)ttainfo->filename, filename, MAX_PATH - 1);
#endif

	// ID3V1 support
	get_id3v1_tag(ttainfo);

	// ID3V2 minimal support
	get_id3v2_tag(ttainfo);

	// read TTA header
	if (!ReadFile(ttainfo->HFILE, &ttahdr, sizeof(tta_hdr), &result, NULL) ||
		result != sizeof(tta_hdr)) {
		CloseHandle(ttainfo->HFILE);
		ttainfo->STATE = READ_ERROR;
		return -1;
	}

	// check for TTA3 signature
	if (ttahdr.TTAid != TTA1_SIGN) {
		CloseHandle(ttainfo->HFILE);
		ttainfo->STATE = FORMAT_ERROR;
		return -1;
	}

	checksum = crc32((unsigned char *) &ttahdr,
	sizeof(tta_hdr) - sizeof(long));
	if (checksum != ttahdr.CRC32) {
		CloseHandle(ttainfo->HFILE);
		ttainfo->STATE = FILE_ERROR;
		return -1;
	}

	// check for player supported formats
	if ((ttahdr.AudioFormat != WAVE_FORMAT_PCM &&
		ttahdr.AudioFormat != WAVE_FORMAT_IEEE_FLOAT &&
		ttahdr.AudioFormat != WAVE_FORMAT_EXTENSIBLE) ||
		ttahdr.BitsPerSample > MAX_BPS ||
		ttahdr.NumChannels > MAX_NCH) {
		CloseHandle(ttainfo->HFILE);
		ttainfo->STATE = PLAYER_ERROR;
		return -1;
	}
	CloseHandle(ttainfo->HFILE);

	// fill the File Info
	ttainfo->NCH = ttahdr.NumChannels;
	ttainfo->BPS = ttahdr.BitsPerSample;
	ttainfo->BSIZE = (ttahdr.BitsPerSample + 7)/8;
	ttainfo->FORMAT = ttahdr.AudioFormat;
	ttainfo->SAMPLERATE = ttahdr.SampleRate;
	ttainfo->DATALENGTH = ttahdr.DataLength;
	ttainfo->FRAMELEN = (long) (FRAME_TIME * ttahdr.SampleRate);
	ttainfo->LENGTH = ttahdr.DataLength / ttahdr.SampleRate * 1000;

	datasize = ttainfo->FILESIZE - ttainfo->id3v2.size;
	origsize = ttainfo->DATALENGTH * ttainfo->BSIZE * ttainfo->NCH;

	ttainfo->COMPRESS = (float) datasize / origsize;
	ttainfo->BITRATE = (long) ((ttainfo->COMPRESS *
		ttainfo->SAMPLERATE * ttainfo->NCH * ttainfo->BPS) / 1000);

	return 0;
}

static void get_id3v1_tag (tta_info *ttainfo) {
	SetFilePointer(ttainfo->HFILE, 0, NULL, FILE_BEGIN);
}

static char *unwrap (char *str, int length) {
	char *ptr = str, *e = str + length;
	for (;ptr < e; ptr++) {
		if (*ptr == '\r' && *(ptr + 1) == '\n') {
			MoveMemory(ptr, ptr+1, e - ptr);
			if (*ptr == '\n') *ptr = ' ';
		}
	}
	return str;
}

static bool del_id3v1_tag (tta_info *ttainfo) {
	HANDLE hFile;

	if (!ttainfo->id3v1.id3has) return true;

	// delete ID3V1 tag
	hFile = CreateFileA(reinterpret_cast<LPCSTR>(ttainfo->filename), GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		//tta_error(OPEN_ERROR, ttainfo->filename);
		return false;
	}

	SetFilePointer(hFile, -(int) sizeof(id3v1_tag), NULL, FILE_END);
	SetEndOfFile(hFile);
	CloseHandle(hFile);

	ttainfo->id3v1.id3has = 0;
	return true;
}

/***********************************************************************
 * ID3 tags manipulation routines
 *
 * Provides read/write access to ID3v1 tags v1.1
 * Provides read/write access to ID3v2 tags v2.3.x and above
 * Supported ID3v2 frames: Title, Artist, Album, Track, Year,
 *                         Genre, Comment.
 *
 **********************************************************************/

static void pack_sint28 (unsigned int value, char *ptr) {
	ptr[0] = (value >> 21) & 0x7f;
	ptr[1] = (value >> 14) & 0x7f;
	ptr[2] = (value >>  7) & 0x7f;
	ptr[3] = (value & 0x7f);
}

static unsigned int unpack_sint28 (const char *ptr) {
	unsigned int value = 0;

	if (ptr[0] & 0x80) return 0;

	value =  value       | (ptr[0] & 0x7f);
	value = (value << 7) | (ptr[1] & 0x7f);
	value = (value << 7) | (ptr[2] & 0x7f);
	value = (value << 7) | (ptr[3] & 0x7f);

	return value;
}

static void pack_sint32 (unsigned int value, char *ptr) {
	ptr[0] = (value >> 24) & 0xff;
	ptr[1] = (value >> 16) & 0xff;
	ptr[2] = (value >>  8) & 0xff;
	ptr[3] = (value & 0xff);
}

static unsigned int unpack_sint32 (const char *ptr) {
	unsigned int value = 0;

	if (ptr[0] & 0x80) return 0;

	value = (value << 8) | ptr[0];
	value = (value << 8) | ptr[1];
	value = (value << 8) | ptr[2];
	value = (value << 8) | ptr[3];

	return value;
}

static int get_frame_id (const char *id) {
	if (!memcmp(id, "TIT2", 4)) return TIT2;	// Title
	if (!memcmp(id, "TPE1", 4)) return TPE1;	// Artist
	if (!memcmp(id, "TALB", 4)) return TALB;	// Album
	if (!memcmp(id, "TRCK", 4)) return TRCK;	// Track
	if (!memcmp(id, "TYER", 4)) return TYER;	// Year
	if (!memcmp(id, "TCON", 4)) return TCON;	// Genre
	if (!memcmp(id, "COMM", 4)) return COMM;	// Comment
	return 0;
}

static void get_id3v2_tag (tta_info *ttainfo) {
	id3v2_tag id3v2;
	ULONG result;
	int id3v2_size;

	if (!ReadFile(ttainfo->HFILE, &id3v2, sizeof(id3v2_tag), &result, NULL) ||
		result != sizeof(id3v2_tag) || memcmp(id3v2.id, "ID3", 3)) {
		SetFilePointer(ttainfo->HFILE, 0, NULL, FILE_BEGIN);
		return;
	}

	id3v2_size = unpack_sint28((const char*)id3v2.size) + 10;

	if (id3v2.flags & ID3_FOOTERPRESENT_FLAG) id3v2_size += 10;
	SetFilePointer(ttainfo->HFILE, id3v2_size, NULL, FILE_BEGIN);
	ttainfo->id3v2.size = id3v2_size;

	ttainfo->id3v2.id3has = 1;
}

bool LoadAttributeFileTTA(FILE_INFO *pFileMP3);
bool WriteAttributeFileTTA(FILE_INFO *pFileMP3);

bool LoadAttributeFileTTA(FILE_INFO *pFileMP3)
{
	CId3tagv1 tagv1;	
	if (tagv1.Load(GetFullPath(pFileMP3)) == ERROR_SUCCESS && tagv1.IsEnable()) {
		SetTrackName(pFileMP3, tagv1.GetTitle());
		SetArtistName(pFileMP3, tagv1.GetArtist());
		SetAlbumName(pFileMP3, tagv1.GetAlbum());
		SetYear(pFileMP3, tagv1.GetYear());
		SetGenre(pFileMP3, STEPGetGenreNameSIF(tagv1.GetGenreNum()));
		SetComment(pFileMP3, tagv1.GetComment());
		if (tagv1.GetTrackNoNum() > 0) {
			SetBTrackNumber(pFileMP3, tagv1.GetTrackNoNum());
		}
	}

	CId3tagv2 tagv2;
	if (tagv2.Load(GetFullPath(pFileMP3)) == ERROR_SUCCESS && tagv2.IsEnable()) {
		SetTrackNameSI(pFileMP3, tagv2.GetTitle());
		SetArtistNameSI(pFileMP3, tagv2.GetArtist());
		SetAlbumNameSI(pFileMP3, tagv2.GetAlbum());
		SetYearSI(pFileMP3, tagv2.GetYear());
		SetGenreSI(pFileMP3, tagv2.GetGenre());
		SetCommentSI(pFileMP3, tagv2.GetComment());
		SetTrackNumberSI(pFileMP3, tagv2.GetTrackNo());
	} else if (tagv1.IsEnable()) {
		SetTrackNameSI(pFileMP3, tagv1.GetTitle());
		SetArtistNameSI(pFileMP3, tagv1.GetArtist());
		SetAlbumNameSI(pFileMP3, tagv1.GetAlbum());
		SetYearSI(pFileMP3, tagv1.GetYear());
		SetGenreSI(pFileMP3, tagv1.GetGenre());
		SetCommentSI(pFileMP3, tagv1.GetComment());
		SetTrackNumberSI(pFileMP3, tagv1.GetTrackNo());
	}

	tta_info ttainfo;
	if (open_tta_file(GetFullPath(pFileMP3), &ttainfo) == 0) {
		CString strFormat;
		strFormat.Format(TEXT("%d kb/s %d Hz %.02f %%"), (int)ttainfo.BITRATE, (int)ttainfo.SAMPLERATE, ttainfo.COMPRESS);
		SetAudioFormat(pFileMP3, strFormat);
		SetPlayTime(pFileMP3, ttainfo.LENGTH/1000);
	} else {
		SetAudioFormat(pFileMP3, TEXT("Unknown"));
	} 
    return true;
}

#ifndef iskanji
#define iskanji(c)		((unsigned char)(c) >= 0x81 && (unsigned char)(c) <= 0x9f || (unsigned char)(c) >= 0xe0 && (unsigned char)(c) <= 0xfc)
#endif
void	StringCopyN(char *, const char *, int, BOOL = TRUE);
void	StringCopyN2(char *, const char *, int, BOOL = TRUE);
void StringCopyN(char *sDest, const char *sSrc, int nLen, BOOL bTerm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ZeroMemory(sDest, nLen);
	if (strlen(sSrc) < (unsigned int)nLen) {
		if (bTerm) strcpy(sDest, sSrc);
		else       memcpy(sDest, sSrc, strlen(sSrc));
		return;
	}
	while(nLen > 0) {
		if (iskanji(*sSrc)) {
			if (nLen >= 2) {
				*sDest++ = *sSrc++;
				*sDest++ = *sSrc++;
			} else {
				if (bTerm) *sDest = '\0';
			}
			nLen -= 2;
		} else {
			*sDest++ = *sSrc++;
			nLen--;
		}
	}
}
void StringCopyN2(char *sDest, const char *sSrc, int nLen, BOOL bTerm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ZeroMemory(sDest, nLen);
	if (strlen(sSrc) < (unsigned int)nLen) {
		if (bTerm) strcpy(sDest, sSrc);
		else       memcpy(sDest, sSrc, strlen(sSrc));
		return;
	}
	bool bCR = false;
	while(nLen > 0) {
		if (*sSrc == '\r') bCR = true;
		if (iskanji(*sSrc)) {
			if (nLen >= 2) {
				if (!bCR) {
					*sDest++ = *sSrc++;
					*sDest++ = *sSrc++;
				} else {
					if (bTerm) *sDest++ = '\0';
					if (bTerm) *sDest++ = '\0';
				}
			} else {
				if (bTerm) *sDest = '\0';
			}
			nLen -= 2;
		} else {
			if (!bCR) {
				*sDest++ = *sSrc++;
			} else {
				if (bTerm) *sDest++ = '\0';
			}
			nLen--;
		}
	}
}

bool WriteAttributeFileTTA(FILE_INFO *pFileMP3)
{
	CId3tagv2 tagv2;
	if (tagv2.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		if (!tagv2.IsEnable()) {
			if (tagv2.MakeTag(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
				return false;
			}
		}
	}
	tagv2.SetTitle(GetTrackNameSI(pFileMP3));
	tagv2.SetArtist(GetArtistNameSI(pFileMP3));
	tagv2.SetAlbum(GetAlbumNameSI(pFileMP3));
	tagv2.SetYear(GetYearSI(pFileMP3));
	tagv2.SetComment(GetCommentSI(pFileMP3));
	tagv2.SetGenre(GetGenreSI(pFileMP3));
	tagv2.SetTrackNo(GetTrackNumberSI(pFileMP3));

	if (tagv2.Save(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}

	if (bOptID3TagAutoWrite) {
		STEPConvSiFieldToId3tag(pFileMP3);
	}
	/*
	if (bOptID3TagAutoWrite) {
		SetTrackName(pFileMP3, tagv2.GetTitle());
		SetArtistName(pFileMP3, tagv2.GetArtist());
		SetAlbumName(pFileMP3, tagv2.GetAlbum());
		SetYear(pFileMP3, tagv2.GetYear());
		SetGenre(pFileMP3, tagv2.GetGenre()));
		SetComment(pFileMP3, tagv2.GetComment());
		if (STEPGetIntegerTrackNumber(tagv2.GetTrackNo()) > 0) {
			SetBTrackNumber(pFileMP3, STEPGetIntegerTrackNumber(tagv2.GetTrackNo()));
		}
	}
	*/

	CId3tagv1 tagv1;
	if (tagv1.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		if  (!bOptID3TagAutoDelete) {
			if (!tagv1.IsEnable()) {
				if (tagv1.MakeTag(NULL, GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
					return false;
				}
			}
		}
	}
	if  (!bOptID3TagAutoDelete) {
		tagv1.SetTitle(static_cast<CStringA>(GetTrackName(pFileMP3)));
		tagv1.SetArtist(static_cast<CStringA>(GetArtistName(pFileMP3)));
		tagv1.SetAlbum(static_cast<CStringA>(GetAlbumName(pFileMP3)));
		tagv1.SetYear(static_cast<CStringA>(GetYear(pFileMP3)));
		tagv1.SetComment(static_cast<CStringA>(GetComment(pFileMP3)));
		tagv1.SetGenre(STEPGetGenreCode(GetGenre(pFileMP3)));
		if (GetBTrackNumber(pFileMP3) > 0) {
			tagv1.SetTrackNo(GetBTrackNumber(pFileMP3));
		}
		if (tagv1.Save(NULL, GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
			return false;
		}
	} else if (tagv1.IsEnable()) {
		if (tagv1.DelTag(NULL, GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
			return false;
		}
		STEPInitDataID3(pFileMP3);
	}

    return true;
}
