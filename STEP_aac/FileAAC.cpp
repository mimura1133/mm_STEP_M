#include "stdafx.h"
#include <vector>
#include "FileAAC.h"
#include "mp4.h"

typedef struct {
	char* name;
	char* value;
	u_int32_t size;
} MetaData;

CString UTF8toString(const char* UTF8)
{
	auto size = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, nullptr, 0);
	std::vector<wchar_t> buff(size);
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, buff.data(), buff.size());
	return buff.data();
}

std::vector<char> StringtoUTF8(CStringW text)
{
	auto size = WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);
	std::vector<char> buff(size);
	WideCharToMultiByte(CP_UTF8, 0, text, -1, buff.data(), buff.size(), nullptr, nullptr);
	return buff;
}

bool LoadFileAAC(FILE_INFO *pFile)
{
    MP4FileHandle h = MP4Read(static_cast<CStringA>(GetFullPath(pFile)), 0);
    if (h == MP4_INVALID_FILE_HANDLE) {
		return false;
	}
	char* value;
	char* buff;
	u_int16_t no, total;
	if (MP4GetMetadataName(h, &value) == true) {
		SetTrackNameSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataArtist(h, &value) == true) {
		SetArtistNameSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataWriter(h, &value) == true) {
		SetComposerSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataComment(h, &value) == true) {
		SetCommentSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataTool(h, &value) == true) {
		SetSoftwareSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataYear(h, &value) == true) {
		SetYearSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataAlbum(h, &value) == true) {
		SetAlbumNameSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataAlbumArtist(h, &value) == true) { /* Žæ“¾‚Å‚«‚é‚æ‚¤‚Émp4v2.dll‚ð•ÏX */
		SetAlbumArtistSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataTrack(h, &no, &total) == true) {
		TCHAR trackNo[10];
		if (total > 0) {
			_stprintf(trackNo, TEXT("%d/%d"), no, total);
		} else {
			_stprintf(trackNo, TEXT("%d"), no);
		}
		SetTrackNumberSI(pFile, trackNo);
	}
	if (MP4GetMetadataDisk(h, &no, &total) == true) {
		TCHAR diskNo[10];
		if (total > 0) {
			_stprintf(diskNo, TEXT("%d/%d"), no, total);
		} else {
			_stprintf(diskNo, TEXT("%d"), no);
		}
		SetDiskNumberSI(pFile, diskNo);
	}
	if (MP4GetMetadataGenre(h, &value) == true) { /* Žæ“¾‚Å‚«‚é‚æ‚¤‚Émp4v2.dll‚ð•ÏX */
		SetGenreSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	if (MP4GetMetadataGrouping(h, &value) == true) { /* Žæ“¾‚Å‚«‚é‚æ‚¤‚Émp4v2.dll‚É’Ç‰Á */
		SetKeywordSI(pFile, UTF8toString(value));
		MP4Free(value);
	}
	CString strOther = "";
	{
		u_int16_t tempo;
		if (MP4GetMetadataTempo(h, &tempo) == true) {
			if (tempo > 0) {
				char buff[10];
				sprintf(buff, " %dBPM", tempo);
				strOther += buff;
			}
		}
	}
	{
		u_int8_t cpl;
		if (MP4GetMetadataCompilation(h, &cpl) == true) {
			if (cpl == 1) {
				strOther += " ºÝËßÚ°¼®Ý‚Ìˆê•”";
			}
		}
	}

	//MP4TrackId trackId = MP4FindTrackId(pFile, 0, MP4_AUDIO_TRACK_TYPE);
	//SetAudioFormat(pFile, MP4Info(h));
	// mp4info.cpp PrintAudioInfo()
	MP4TrackId trackId = MP4FindTrackId(h, 0);
	static const char* mpeg4AudioNames[] = {
		"MPEG-4 AAC main",
		"MPEG-4 AAC LC", 
		"MPEG-4 AAC SSR",
		"MPEG-4 AAC LTP",
		NULL,
		"MPEG-4 AAC Scalable",
		"MPEG-4 TwinVQ",
		"MPEG-4 CELP",
		"MPEG-4 HVXC",
		NULL, NULL,
		"MPEG-4 TTSI",
		"MPEG-4 Main Synthetic",
		"MPEG-4 Wavetable Syn",
		"MPEG-4 General MIDI",
		"MPEG-4 Algo Syn and Audio FX",
		"MPEG-4 ER AAC LC",
		NULL,
		"MPEG-4 ER AAC LTP",
		"MPEG-4 ER AAC Scalable",
		"MPEG-4 ER TwinVQ",
		"MPEG-4 ER BSAC",
		"MPEG-4 ER ACC LD",
		"MPEG-4 ER CELP",
		"MPEG-4 ER HVXC",
		"MPEG-4 ER HILN",
		"MPEG-4 ER Parametric",
	};
	static u_int8_t mpegAudioTypes[] = {
		MP4_MPEG2_AAC_MAIN_AUDIO_TYPE,	// 0x66
		MP4_MPEG2_AAC_LC_AUDIO_TYPE,	// 0x67
		MP4_MPEG2_AAC_SSR_AUDIO_TYPE,	// 0x68
		MP4_MPEG2_AUDIO_TYPE,			// 0x69
		MP4_MPEG1_AUDIO_TYPE,			// 0x6B
		MP4_PCM16_LITTLE_ENDIAN_AUDIO_TYPE,
		MP4_VORBIS_AUDIO_TYPE,
		MP4_ALAW_AUDIO_TYPE,
		MP4_ULAW_AUDIO_TYPE,
		MP4_G723_AUDIO_TYPE,
		MP4_PCM16_BIG_ENDIAN_AUDIO_TYPE,
	};
	static const char* mpegAudioNames[] = {
		"MPEG-2 AAC Main",
		"MPEG-2 AAC LC",
		"MPEG-2 AAC SSR",
		"MPEG-2 Audio (13818-3)",
		"MPEG-1 Audio (11172-3)",
		"PCM16 (little endian)",
		"Vorbis",
		"G.711 aLaw",
		"G.711 uLaw",
		"G.723.1",
		"PCM16 (big endian)",
	};
	static u_int8_t numMpegAudioTypes = 
		sizeof(mpegAudioTypes) / sizeof(u_int8_t);

	u_int8_t type =
		MP4GetTrackEsdsObjectTypeId(h, trackId);
	const char* typeName = "Unknown";

	if (type == MP4_MPEG4_AUDIO_TYPE) {
	  u_int8_t* pAacConfig = NULL;
	  u_int32_t aacConfigLength;

	  MP4GetTrackESConfiguration(h, 
				     trackId,
				     &pAacConfig,
				     &aacConfigLength);

	  if (pAacConfig != NULL && aacConfigLength >= 2) {
	    type = (pAacConfig[0] >> 3) & 0x1f;
	    if (type == 0 || type == 5 || type == 10 || type == 11 ||
		type == 18 || type >= 28) {
	      typeName = "MPEG-4";
	    } else {
	        typeName = mpeg4AudioNames[type - 1];
	    }
	    MP4Free(pAacConfig);
	  } else {
	    typeName = "MPEG-4";
	  }
	} else {
		for (u_int8_t i = 0; i < numMpegAudioTypes; i++) {
			if (type == mpegAudioTypes[i]) {
				typeName = mpegAudioNames[i];
				break;
			}
		}
	}

	u_int32_t timeScale =
		MP4GetTrackTimeScale(h, trackId);

	MP4Duration trackDuration =
		MP4GetTrackDuration(h, trackId);

	double msDuration =
		UINT64_TO_DOUBLE(MP4ConvertFromTrackDuration(h, trackId, 
			trackDuration, MP4_MSECS_TIME_SCALE));

	u_int32_t avgBitRate =
		MP4GetTrackBitRate(h, trackId);

	
	// type duration avgBitrate samplingFrequency
	char format[256];
	sprintf(format, "%s %ukbps %uHz", typeName, (avgBitRate + 500) / 1000, timeScale);
	SetAudioFormat(pFile, static_cast<CString>(format));
	SetPlayTime(pFile, (int)(msDuration / 1000.0));
	SetOther(pFile, strOther);

	MP4Close(h);
	return true;
}

bool WriteFileAAC(FILE_INFO *pFile)
{
    MP4FileHandle h = MP4Read(static_cast<CStringA>(GetFullPath(pFile)), 0/*MP4_DETAILS_ALL*/);
    if (h == MP4_INVALID_FILE_HANDLE) {
      return false;
    }

	u_int32_t index = 0;
	CPtrArray tag;
	typedef struct {
		char* name;
		char* value;
		u_int32_t size;
	} MetaData;
	char* name = 0;
	u_int8_t* value;
	u_int32_t size;
	while (MP4GetMetadataByIndex(h,index, &name, &value, &size) == true) {
		if (!stricmp(name, "trkn") || !stricmp(name, "tracknumber") ||
			!stricmp(name, "disk") || !stricmp(name, "disknumber") ||
			!stricmp(name, "©ART") ||
			!stricmp(name, "©wrt") ||
			!stricmp(name, "©nam") ||
			!stricmp(name, "©alb") ||
			!stricmp(name, "year") ||
			!stricmp(name, "aART") ||
			!stricmp(name, "©cmt") ||
			!stricmp(name, "©too") ||
			!stricmp(name, "©grp") ||
			!stricmp(name, "©gen")) {
			index++;
			continue;
		}
		MetaData* meta = new MetaData;
		meta->name = (char*)malloc(strlen(name)+1);
		strcpy(meta->name, name);
		meta->value = (char*)malloc(size + 1);
		memcpy(meta->value, value, size);
		meta->value[size] = '\0';
		meta->size = size;
		tag.Add(meta);
		index++;
	}
	if (MP4GetMetadataCoverArt(h, &value, &size) == true) {
		MetaData* meta = new MetaData;
		meta->name = (char*)malloc(strlen("covr")+1);
		strcpy(meta->name, "covr");
		meta->value = (char*)malloc(size + 1);
		memcpy(meta->value, value, size);
		meta->value[size] = '\0';
		meta->size = size;
		tag.Add(meta);
		index++;
	}
	
	MP4Close(h);
    h = MP4Modify(static_cast<CStringA>(GetFullPath(pFile)), 0/*MP4_DETAILS_ALL*/, 0);
    if (h == MP4_INVALID_FILE_HANDLE) {
      return false;
    }
	MP4MetadataDelete(h);

	char* buff;
	{
		unsigned __int16 trkn = 0, tot = 0;
		int t1 = 0, t2 = 0;
		LPCTSTR strTrackNo = GetTrackNumberSI(pFile);
		if (_tcsstr(strTrackNo, TEXT("/")) != NULL) {
			_stscanf(strTrackNo, TEXT("%d/%d"), &t1, &t2);
			trkn = t1, tot = t2;
		} else {
			_stscanf(strTrackNo, TEXT("%d"), &t1);
			trkn = t1;
		}
		if (trkn > 0) {
			MP4SetMetadataTrack(h, trkn, tot);
		}
	}
	{
		unsigned __int16 trkn = 0, tot = 0;
		int t1 = 0, t2 = 0;
		LPCTSTR strDiskNo = GetDiskNumberSI(pFile);
		if (_tcsstr(strDiskNo, TEXT("/")) != NULL) {
			_stscanf(strDiskNo, TEXT("%d/%d"), &t1, &t2);
			trkn = t1, tot = t2;
		} else {
			_stscanf(strDiskNo, TEXT("%d"), &t1);
			trkn = t1;
		}
		if (trkn > 0) {
			MP4SetMetadataDisk(h, trkn, tot);
		}
	}
	{
		MP4SetMetadataArtist(h, StringtoUTF8(GetArtistNameSI(pFile)).data());
		MP4SetMetadataWriter(h, StringtoUTF8(GetComposerSI(pFile)).data());
		MP4SetMetadataName(h, StringtoUTF8(GetTrackNameSI(pFile)).data());
		MP4SetMetadataAlbum(h, StringtoUTF8(GetAlbumNameSI(pFile)).data());
		MP4SetMetadataAlbumArtist(h, StringtoUTF8(GetAlbumArtistSI(pFile)).data());
		MP4SetMetadataYear(h, StringtoUTF8(GetYearSI(pFile)).data());
		MP4SetMetadataComment(h, StringtoUTF8(GetCommentSI(pFile)).data());
		MP4SetMetadataGenre(h, StringtoUTF8(GetGenreSI(pFile)).data());
		MP4SetMetadataTool(h, StringtoUTF8(GetSoftwareSI(pFile)).data());
		MP4SetMetadataGrouping(h, StringtoUTF8(GetKeywordSI(pFile)).data());
	}

	for (int i=0;i<tag.GetSize();i++) {
		MetaData* meta = (MetaData*)tag.GetAt(i);
		if (false) {
		}
		else if (!stricmp(meta->name, "disk") || !stricmp(meta->name, "disknumber"))
		{
			unsigned __int16 disk = 0, tot = 0;
			disk = meta->value[2] << 8 | (meta->value[3] & 0xff);
			tot = meta->value[4] << 8 | (meta->value[5] & 0xff);
			if (disk > 0) {
				MP4SetMetadataDisk(h, disk, tot);
			}
		}
		else if (!stricmp(meta->name, "cpil"))
		{
			unsigned __int8 cpil = meta->value[0];
			if (cpil != 0) {
				MP4SetMetadataCompilation(h, cpil);
			}
		}
		else if (!stricmp(meta->name, "tmpo"))
		{
			unsigned __int16 tempo = meta->value[0] << 8 | (meta->value[1] & 0xff);
			if (tempo != 0) {
				MP4SetMetadataTempo(h, tempo);
			}
		}
		else if (!stricmp(meta->name, "covr"))
		{
			MP4SetMetadataCoverArt(h, (u_int8_t*)meta->value, meta->size);
		}
		else
		{
			MP4SetMetadataFreeForm(h, meta->name, (u_int8_t*)meta->value, meta->size);
		}

		free(meta->name);
		free(meta->value);
		delete meta;
	}
	MP4Close(h);
	MP4Optimize(static_cast<CStringA>(GetFullPath(pFile)), NULL, 0/*MP4_DETAILS_ALL*/);

	return true;
}
