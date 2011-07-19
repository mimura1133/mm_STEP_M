#include "stdafx.h"
#include <windows.h>
#include <mmreg.h>
#include <time.h>
#include "id3tag.h"
#include "crc32.h"
#include "ttadec.h"
#include "STEPlugin.h"

extern bool bOptID3TagAutoWrite;
extern bool bOptID3TagAutoDelete;

//HANDLE heap;
#define	heap GetProcessHeap()

static void get_id3v1_tag (tta_info *ttainfo);
static void get_id3v2_tag (tta_info *ttainfo);
static bool save_id3v1_tag (tta_info *ttainfo);
static bool save_id3v2_tag (tta_info *ttainfo);
static bool del_id3v1_tag (tta_info *ttainfo);
//static void del_id3v2_tag (tta_info *ttainfo);

static int open_tta_file (const char *filename, tta_info *ttainfo) {
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
	lstrcpyn((char*)ttainfo->filename, filename, MAX_PATH - 1);

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
	id3v1_tag id3v1;
	ULONG result;

	SetFilePointer(ttainfo->HFILE, -(int) sizeof(id3v1_tag), NULL, FILE_END);
	if (ReadFile(ttainfo->HFILE, &id3v1, sizeof(id3v1_tag), &result, NULL) &&
		result == sizeof(id3v1_tag) && !memcmp(id3v1.id, "TAG", 3)) {
		CopyMemory(ttainfo->id3v1.title, id3v1.title, 30);
		CopyMemory(ttainfo->id3v1.artist, id3v1.artist, 30);
		CopyMemory(ttainfo->id3v1.album, id3v1.album, 30);
		CopyMemory(ttainfo->id3v1.year, id3v1.year, 4);
		CopyMemory(ttainfo->id3v1.comment, id3v1.comment, 28);
		ttainfo->id3v1.track = id3v1.track;
		ttainfo->id3v1.genre = id3v1.genre;
		ttainfo->id3v1.id3has = 1;
	}
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

static bool save_id3v1_tag (tta_info *ttainfo) {
	HANDLE hFile;
	id3v1_tag tag;
	int offset;
	ULONG result;

	ZeroMemory(&tag, sizeof(id3v1_tag));
	CopyMemory(tag.id, "TAG", 3);

	CopyMemory(tag.title, ttainfo->id3v1.title, sizeof(tag.title));
	CopyMemory(tag.artist, ttainfo->id3v1.artist, sizeof(tag.artist));
	CopyMemory(tag.album, ttainfo->id3v1.album, sizeof(tag.album));
	CopyMemory(tag.year, ttainfo->id3v1.year, sizeof(tag.year));
	CopyMemory(tag.comment, ttainfo->id3v1.comment, sizeof(tag.comment));
//	CopyMemory(tag.comment, unwrap((char*)ttainfo->id3v1.comment,
//		sizeof(tag.comment)), sizeof(tag.comment));
	tag.track = ttainfo->id3v1.track;
	tag.genre = ttainfo->id3v1.genre;

	// update ID3V1 tag
	hFile = CreateFile((char*)ttainfo->filename, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		//tta_error(OPEN_ERROR, ttainfo->filename);
		return false;
	}

	offset = (ttainfo->id3v1.id3has)? -(int) sizeof(id3v1_tag):0;
	SetFilePointer(hFile, offset, NULL, FILE_END);
	if (!WriteFile(hFile, &tag, sizeof(id3v1_tag), &result, 0) ||
		result != sizeof(id3v1_tag)) {
		CloseHandle(hFile);
		//tta_error(WRITE_ERROR, ttainfo->filename);
		return false;
	}
	CloseHandle(hFile);

	ttainfo->id3v1.id3has = 1;
	return true;
}

static bool del_id3v1_tag (tta_info *ttainfo) {
	HANDLE hFile;

	if (!ttainfo->id3v1.id3has) return true;

	// delete ID3V1 tag
	hFile = CreateFile((char*)ttainfo->filename, GENERIC_READ|GENERIC_WRITE,
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
	HANDLE hMap;
	id3v2_tag id3v2;
	id3v2_frame frame_header;
	unsigned char *buffer, *ptr;
	ULONG result;
	int id3v2_size;

	if (!ReadFile(ttainfo->HFILE, &id3v2, sizeof(id3v2_tag), &result, NULL) ||
		result != sizeof(id3v2_tag) || memcmp(id3v2.id, "ID3", 3)) {
		SetFilePointer(ttainfo->HFILE, 0, NULL, FILE_BEGIN);
		return;
	}

	id3v2_size = unpack_sint28((const char*)id3v2.size) + 10;

	if ((id3v2.flags & ID3_UNSYNCHRONISATION_FLAG) ||
		(id3v2.flags & ID3_EXPERIMENTALTAG_FLAG) ||
		(id3v2.version < 3)) goto done;

	hMap = CreateFileMapping(ttainfo->HFILE, NULL, PAGE_READONLY, 0, id3v2_size, NULL);
	if (!hMap) goto done;

	buffer = (unsigned char*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, id3v2_size);
	if (!buffer) {
		CloseHandle(hMap);
		goto done;
	}

	ptr = buffer + 10;

	// skip extended header if present
	if (id3v2.flags & ID3_EXTENDEDHEADER_FLAG) {
		int offset = (int) unpack_sint32((const char*)ptr);
		ptr += offset;
	}

	// read id3v2 frames
	while (ptr - buffer < id3v2_size) {
		int data_size, frame_id;
		int size, comments = 0;
		unsigned char *data;

		// get frame header
		CopyMemory(&frame_header, ptr, sizeof(id3v2_frame));
		ptr += sizeof(id3v2_frame);
		data_size = unpack_sint32((const char*)frame_header.size);

		if (!*frame_header.id) break;

		if (!(frame_id = get_frame_id((char*)frame_header.id))) {
			ptr += data_size;
			continue;
		}

		// skip unsupported frames
		if (frame_header.flags & FRAME_COMPRESSION_FLAG ||
			frame_header.flags & FRAME_ENCRYPTION_FLAG ||
			frame_header.flags & FRAME_UNSYNCHRONISATION_FLAG ||
			*ptr != FIELD_TEXT_ISO_8859_1) {
			ptr += data_size;
			continue;
		}

		ptr++; data_size--;

		switch (frame_id) {
		case TIT2:	data = ttainfo->id3v2.title;
					size = sizeof(ttainfo->id3v2.title) - 1; break;
		case TPE1:	data = ttainfo->id3v2.artist;
					size = sizeof(ttainfo->id3v2.artist) - 1; break;
		case TALB:	data = ttainfo->id3v2.album;
					size = sizeof(ttainfo->id3v2.album) - 1; break;
		case TRCK:	data = ttainfo->id3v2.track;
					size = sizeof(ttainfo->id3v2.track) - 1; break;
		case TYER:	data = ttainfo->id3v2.year;
					size = sizeof(ttainfo->id3v2.year) - 1; break;
		case TCON:	data = ttainfo->id3v2.genre;
					size = sizeof(ttainfo->id3v2.genre) - 1; break;
		case COMM:	if (comments++) goto next;
					data = ttainfo->id3v2.comment;
					size = sizeof(ttainfo->id3v2.comment) - 1;
					data_size -= 3; ptr += 3;
					// skip zero short description
					if (*ptr == 0) { data_size--; ptr++; }
					break;
		}
next:
		CopyMemory(data, ptr, (data_size <= size)? data_size:size);
		ptr += data_size;
	}

	UnmapViewOfFile((LPCVOID *) buffer);
	CloseHandle(hMap);

done:
	if (id3v2.flags & ID3_FOOTERPRESENT_FLAG) id3v2_size += 10;
	SetFilePointer(ttainfo->HFILE, id3v2_size, NULL, FILE_BEGIN);
	ttainfo->id3v2.size = id3v2_size;

	ttainfo->id3v2.id3has = 1;
}

static void add_text_frame (char *id, char **dest, char *src) {
	id3v2_frame frame_header;

	if (*src) {
		int size = lstrlen(src) + 1;
		CopyMemory(frame_header.id, id, 4);
		frame_header.flags = 0;
		pack_sint32(size, (char*)frame_header.size);
		CopyMemory(*dest, &frame_header, sizeof(id3v2_frame));
		*dest += sizeof(id3v2_frame);
		CopyMemory(*dest + 1, src, size); *dest += size;
	}
}

static void add_comm_frame (char *id, char **dest, char *src) {
	id3v2_frame frame_header;

	if (*src) {
		int size = lstrlen(src) + 1;
		CopyMemory(frame_header.id, id, 4);
		frame_header.flags = 0;
		pack_sint32(size + 4, (char*)frame_header.size);
		CopyMemory(*dest, &frame_header, sizeof(id3v2_frame));
		// skip language, add zero short description
		*dest += sizeof(id3v2_frame) + 4;
		CopyMemory(*dest + 1, src, size); *dest += size;
	}
}

static bool save_id3v2_tag (tta_info *ttainfo) {
	HANDLE hFile, hMap;
	id3v2_tag id3v2;
	unsigned char *buffer, *ptr;
	unsigned char *tag_data, *tptr;
	unsigned long new_size, id3v2_size, tag_size;
	int offset;
	ULONG result;
	BOOL copy_data = TRUE;

	hFile = CreateFile((char*)ttainfo->filename, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		//tta_error(OPEN_ERROR, ttainfo->filename);
		return false;
	}

	if (!ReadFile(hFile, &id3v2, sizeof(id3v2_tag), &result, NULL) ||
		result != sizeof(id3v2_tag)) {
		//tta_error(READ_ERROR, ttainfo->filename);
		CloseHandle(hFile);
		return false;
	}

	if (!memcmp(id3v2.id, "ID3", 3)) {
		id3v2_size = unpack_sint28((char*)id3v2.size) + 10;
		if (id3v2.flags & ID3_FOOTERPRESENT_FLAG) id3v2_size += 10;
	} else {
		ZeroMemory(&id3v2, sizeof(id3v2_tag));
		CopyMemory(id3v2.id, "ID3", 3);
		id3v2_size = 0;
	}
	
	tag_data = (unsigned char*)HeapAlloc(heap, HEAP_ZERO_MEMORY,
		id3v2_size + sizeof(id3v2_data));
	tptr = tag_data + 10;

	if (!(id3v2.flags & ID3_UNSYNCHRONISATION_FLAG) &&
		!(id3v2.flags & ID3_EXPERIMENTALTAG_FLAG) &&
		(id3v2.version >= ID3_VERSION) && id3v2_size) {

		hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, id3v2_size, NULL);
		if (!hMap) goto done;

		buffer = (unsigned char*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, id3v2_size);
		if (!buffer) {
			CloseHandle(hMap);
			goto done;
		}

		ptr = buffer + 10;

		// copy extended header if present
		if ((id3v2.flags & ID3_EXTENDEDHEADER_FLAG)) {
			int ext_size = (int) unpack_sint32((char*)ptr);
			CopyMemory(tptr, ptr, ext_size);
			ptr += ext_size; tptr += ext_size;
		}
	} else copy_data = FALSE;

	// add updated id3v2 frames
	add_text_frame("TIT2", (char**)&tptr, (char*)ttainfo->id3v2.title);
	add_text_frame("TPE1", (char**)&tptr, (char*)ttainfo->id3v2.artist);
	add_text_frame("TALB", (char**)&tptr, (char*)ttainfo->id3v2.album);
	add_text_frame("TRCK", (char**)&tptr, (char*)ttainfo->id3v2.track);
	add_text_frame("TYER", (char**)&tptr, (char*)ttainfo->id3v2.year);
	add_text_frame("TCON", (char**)&tptr, (char*)ttainfo->id3v2.genre);
	add_comm_frame("COMM", (char**)&tptr, (char*)ttainfo->id3v2.comment);

	if (!copy_data) goto save;

	// copy unchanged frames
	while ((UINT)(ptr - buffer) < id3v2_size) {
		int data_size, frame_size;
		int frame_id, comments = 0;
		id3v2_frame frame_header;

		// get frame header
		CopyMemory(&frame_header, ptr, sizeof(id3v2_frame));
		data_size = unpack_sint32((char*)frame_header.size);
		frame_size = sizeof(id3v2_frame) + data_size;

		if (!*frame_header.id) break;

		if ((frame_id = get_frame_id((char*)frame_header.id)))
			if (frame_id != COMM || !comments++) {
				ptr += frame_size; continue;
			}

		// copy frame
		CopyMemory(tptr, ptr, frame_size);
		tptr += frame_size; ptr += frame_size;
	}

	// copy footer if present
	if (id3v2.flags & ID3_FOOTERPRESENT_FLAG) {
		CopyMemory(tptr, ptr, 10);
		tptr += 10; ptr += 10;
	}

save:
	if (copy_data) {
		UnmapViewOfFile((LPCVOID *) buffer);
		CloseHandle(hMap);
	}

	new_size = tptr - tag_data;

	// fill ID3v2 header
	id3v2.flags &= ~ID3_UNSYNCHRONISATION_FLAG;
	id3v2.flags &= ~ID3_EXPERIMENTALTAG_FLAG;
	id3v2.version = ID3_VERSION;

	// write data
	if (new_size <= id3v2_size) {
		pack_sint28(id3v2_size - 10, (char*)id3v2.size);
		CopyMemory(tag_data, &id3v2, sizeof(id3v2_tag));

		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		if (!WriteFile(hFile, tag_data, id3v2_size, &result, 0) ||
			result != id3v2_size) {
			CloseHandle(hFile);
			//tta_error(WRITE_ERROR, ttainfo->filename);
			return false;
		}
		goto done;
	}

	pack_sint28(new_size - 10, (char*)id3v2.size);
	CopyMemory(tag_data, &id3v2, sizeof(id3v2_tag));
	offset = (int) new_size - id3v2_size;

	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0,
		ttainfo->FILESIZE + offset, NULL);
	if (!hMap) goto done;

	buffer = (unsigned char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0,
		ttainfo->FILESIZE + offset);
	if (!buffer) {
		CloseHandle(hMap);
		goto done;
	}

	MoveMemory(buffer + ((int)id3v2_size + offset),
		buffer + id3v2_size, ttainfo->FILESIZE);
	CopyMemory(buffer, tag_data, new_size);

	UnmapViewOfFile((LPCVOID *) buffer);
	CloseHandle(hMap);

	ttainfo->FILESIZE += offset;
	ttainfo->id3v2.size = new_size;

done:
	CloseHandle(hFile);
	HeapFree(heap, 0, tag_data);

	ttainfo->id3v2.id3has = 1;
	return true;
}

#if 0
static void del_id3v2_tag (tta_info *ttainfo) {
	HANDLE hFile, hMap;
	unsigned char *buffer;

	if (!ttainfo->id3v2.id3has) return;

	hFile = CreateFile((char*)ttainfo->filename, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		//tta_error(OPEN_ERROR, ttainfo->filename);
		return;
	}

	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (!hMap) {
		CloseHandle(hFile);
		CloseHandle(hMap);
		return;
	}

	buffer = (unsigned char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!buffer) {
		CloseHandle(hFile);
		CloseHandle(hMap);
		return;
	}

	MoveMemory(buffer, buffer + ttainfo->id3v2.size,
		ttainfo->FILESIZE - ttainfo->id3v2.size);

	UnmapViewOfFile((LPCVOID *) buffer);
	CloseHandle(hMap);

	SetFilePointer(hFile, -(int) ttainfo->id3v2.size, NULL, FILE_END);
	SetEndOfFile(hFile);
	CloseHandle(hFile);

	ttainfo->FILESIZE -= ttainfo->id3v2.size;
	ttainfo->id3v2.size = 0;

	ttainfo->id3v2.id3has = 0;
}
#endif

bool LoadAttributeFileTTA(FILE_INFO *pFileMP3);
bool WriteAttributeFileTTA(FILE_INFO *pFileMP3);

bool LoadAttributeFileTTA(FILE_INFO *pFileMP3)
{
	tta_info ttainfo;
	if (open_tta_file(GetFullPath(pFileMP3), &ttainfo) != 0) {
		return false;
	}
	if (ttainfo.id3v1.id3has) {
		SetTrackName(pFileMP3, (char*)ttainfo.id3v1.title);
		SetArtistName(pFileMP3, (char*)ttainfo.id3v1.artist);
		SetAlbumName(pFileMP3, (char*)ttainfo.id3v1.album);
		SetYear(pFileMP3, (char*)ttainfo.id3v1.year);
		SetGenre(pFileMP3, STEPGetGenreNameSIF(ttainfo.id3v1.genre));
		SetComment(pFileMP3, (char*)ttainfo.id3v1.comment);
		if (ttainfo.id3v1.track > 0) {
			SetBTrackNumber(pFileMP3, ttainfo.id3v1.track);
		}
	}
	if (ttainfo.id3v2.id3has) {
		SetTrackNameSI(pFileMP3, (char*)ttainfo.id3v2.title);
		SetArtistNameSI(pFileMP3, (char*)ttainfo.id3v2.artist);
		SetAlbumNameSI(pFileMP3, (char*)ttainfo.id3v2.album);
		SetYearSI(pFileMP3, (char*)ttainfo.id3v2.year);
		SetGenreSI(pFileMP3, (char*)ttainfo.id3v2.genre);
		SetCommentSI(pFileMP3, (char*)ttainfo.id3v2.comment);
		SetTrackNumberSI(pFileMP3, (char*)ttainfo.id3v2.track);
	}

	CString strFormat;
	strFormat.Format("%d kb/s %d Hz %.02f %%", (char*)ttainfo.BITRATE, (char*)ttainfo.SAMPLERATE, ttainfo.COMPRESS);
	SetAudioFormat(pFileMP3, strFormat);
	SetPlayTime(pFileMP3, ttainfo.LENGTH/1000);

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
	tta_info ttainfo;
	if (open_tta_file(GetFullPath(pFileMP3), &ttainfo) != 0) {
		return false;
	}
	if (!ttainfo.id3v1.id3has || ttainfo.id3v2.id3has || bOptID3TagAutoDelete) {
		StringCopyN((char*)ttainfo.id3v2.title, GetTrackNameSI(pFileMP3), sizeof(ttainfo.id3v2.title) - 1);
		StringCopyN((char*)ttainfo.id3v2.artist, GetArtistNameSI(pFileMP3), sizeof(ttainfo.id3v2.artist) - 1);
		StringCopyN((char*)ttainfo.id3v2.album, GetAlbumNameSI(pFileMP3), sizeof(ttainfo.id3v2.album) - 1);
		StringCopyN((char*)ttainfo.id3v2.year, GetYearSI(pFileMP3), sizeof(ttainfo.id3v2.year) - 1);
		StringCopyN((char*)ttainfo.id3v2.comment, GetCommentSI(pFileMP3), sizeof(ttainfo.id3v2.comment) - 1);
		StringCopyN((char*)ttainfo.id3v2.genre, GetGenreSI(pFileMP3), sizeof(ttainfo.id3v2.genre) - 1);
		StringCopyN((char*)ttainfo.id3v2.track, GetTrackNumberSI(pFileMP3), sizeof(ttainfo.id3v2.track) - 1);

		if (save_id3v2_tag(&ttainfo) != true) {
			return false;
		}
	}
	if (strlen(GetTrackName(pFileMP3)) > 0
		|| strlen(GetArtistName(pFileMP3)) > 0
		|| strlen(GetAlbumName(pFileMP3)) > 0
		|| strlen(GetYear(pFileMP3)) > 0
		|| STEPGetGenreCode(GetGenre(pFileMP3)) != 0xFF
		|| GetBTrackNumber(pFileMP3) > 0
		|| strlen(GetComment(pFileMP3)) > 0) {
		ttainfo.id3v1.id3has = true;
	}
	if (ttainfo.id3v1.id3has && !bOptID3TagAutoDelete || bOptID3TagAutoWrite) {
		if (bOptID3TagAutoWrite) {
			StringCopyN((char*)ttainfo.id3v1.title, GetTrackNameSI(pFileMP3), sizeof(ttainfo.id3v1.title) - 1);
			StringCopyN((char*)ttainfo.id3v1.artist, GetArtistNameSI(pFileMP3), sizeof(ttainfo.id3v1.artist) - 1);
			StringCopyN((char*)ttainfo.id3v1.album, GetAlbumNameSI(pFileMP3), sizeof(ttainfo.id3v1.album) - 1);
			StringCopyN((char*)ttainfo.id3v1.year, GetYearSI(pFileMP3), sizeof(ttainfo.id3v1.year) - 1);
			ttainfo.id3v1.genre = STEPGetGenreCode(GetGenreSI(pFileMP3));
			ttainfo.id3v1.track = STEPGetIntegerTrackNumber(GetTrackNumberSI(pFileMP3));
			StringCopyN2((char*)ttainfo.id3v1.comment, GetCommentSI(pFileMP3), sizeof(ttainfo.id3v1.comment) - 1);
		} else {
			StringCopyN((char*)ttainfo.id3v1.title, GetTrackName(pFileMP3), sizeof(ttainfo.id3v1.title) - 1);
			StringCopyN((char*)ttainfo.id3v1.artist, GetArtistName(pFileMP3), sizeof(ttainfo.id3v1.artist) - 1);
			StringCopyN((char*)ttainfo.id3v1.album, GetAlbumName(pFileMP3), sizeof(ttainfo.id3v1.album) - 1);
			StringCopyN((char*)ttainfo.id3v1.year, GetYear(pFileMP3), sizeof(ttainfo.id3v1.year) - 1);
			ttainfo.id3v1.genre = STEPGetGenreCode(GetGenre(pFileMP3));
			if (GetBTrackNumber(pFileMP3) > 0) {
				ttainfo.id3v1.track = GetBTrackNumber(pFileMP3);
			}
			StringCopyN2((char*)ttainfo.id3v1.comment, GetComment(pFileMP3), sizeof(ttainfo.id3v1.comment) - 1);
		}

		if (save_id3v1_tag(&ttainfo) != true) {
			return false;
		}
		if (bOptID3TagAutoWrite) {
			STEPConvSiFieldToId3tag(pFileMP3);
		}
	}
	if (bOptID3TagAutoDelete) {
		if (del_id3v1_tag(&ttainfo) != true) {
			return false;
		}
		STEPInitDataID3(pFileMP3);
	}
    return true;
}
