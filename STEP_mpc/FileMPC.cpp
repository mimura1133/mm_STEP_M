#include "stdafx.h"
#include "STEPlugin.h"
#include "mpc_dec.h"
#include "in_mpc.h"
#include "idtag.h"

bool LoadAttributeFileMPC(FILE_INFO *pFileMP3);
bool WriteAttributeFileMPC(FILE_INFO *pFileMP3);

struct MPC_COMMENT_SET
{
    const char *cszName;
    void (*setFunc)(FILE_INFO*, const char*);
};
struct MPC_COMMENT_GET
{
    const char *cszName;
    LPCTSTR (*getFunc)(FILE_INFO*);
};

static const char *
profile_stringify(unsigned int profile)    // profile is 0...15, where 7...13 is used
{
    static const char na[] = "n.a.";
    static const char *Names[] = {
        na, "Experimental", na, na,
        na, na, na, "Telephone",
        "Thumb", "Radio", "Standard", "Xtreme",
        "Insane", "BrainDead", "BrainDead+", "BrainDead++"
    };

    return profile >=
        sizeof (Names) / sizeof (*Names) ? na : Names[profile];
}

int StreamInfo::ReadStreamInfo ( Reader* fp)
{
    unsigned int HeaderData[16];
    memset ( &simple, 0, sizeof (BasicData) );                        // Reset Info-Data
    if ( (simple.HeaderPosition =  JumpID3v2 (fp)) < 0 )              // get header position
        return ERROR_CODE_FILE;

    if ( fp->seek ( simple.HeaderPosition, SEEK_SET ) != 0 )         // seek to first byte of mpc data
        return ERROR_CODE_FILE;
	fp->read((void *) HeaderData, sizeof (int) * 16);
    if ( fp->seek ( 0L, SEEK_END ) != 0 )                                  // get filelength
        return ERROR_CODE_FILE;
    simple.TotalFileLength = fp->tell ();
    simple.TagOffset = simple.TotalFileLength;
    ReadTags ( fp );
    UINT  ByteLength = simple.TotalFileLength - (simple.TotalFileLength - simple.TagOffset) - simple.HeaderPosition;
    simple.StreamVersion = HeaderData[0] >> 24;
    if (simple.StreamVersion >= 7) {
        const long samplefreqs[4] = { 44100, 48000, 37800, 32000 };
        
        // read the file-header (SV7 and above)
        simple.Bitrate = 0;
        simple.Frames = HeaderData[1];
        simple.SampleFreq = samplefreqs[(HeaderData[2] >> 16) & 0x0003];
        simple.MaxBand = (HeaderData[2] >> 24) & 0x003F;
        simple.MS = (HeaderData[2] >> 30) & 0x0001;
        simple.Profile = (HeaderData[2] << 8) >> 28;
        simple.IS = (HeaderData[2] >> 31) & 0x0001;
        simple.BlockSize = 1;
        
        simple.EncoderVersion = (HeaderData[6] >> 24) & 0x00FF;
        simple.Channels = 2;
        // gain
		/*
        simple.EstPeakTitle = HeaderData[2] & 0xFFFF;    // read the ReplayGain data
		*/
        simple.GainTitle = (HeaderData[3] >> 16) & 0xFFFF;
        simple.PeakTitle = HeaderData[3] & 0xFFFF;
        simple.GainAlbum = (HeaderData[4] >> 16) & 0xFFFF;
        simple.PeakAlbum = HeaderData[4] & 0xFFFF;
        // gaples
        simple.IsTrueGapless = (HeaderData[5] >> 31) & 0x0001;    // true gapless: used?
        simple.LastFrameSamples = (HeaderData[5] >> 20) & 0x07FF;    // true gapless: valid samples for last frame
        
        if (simple.EncoderVersion == 0) {
            sprintf(simple.Encoder, "Klemm <= 1.05"); // Buschmann 1.7.x, Klemm <= 1.05
        } else {
            switch (simple.EncoderVersion % 10) {
            case 0:
                sprintf(simple.Encoder, "Buschmann %u.%u",
                    simple.EncoderVersion / 100,
                    simple.EncoderVersion / 10 % 10);
                break;
            case 2:
            case 4:
            case 6:
            case 8:
                sprintf(simple.Encoder, "Buschmann %u.%02u Beta",
                    simple.EncoderVersion / 100,
                    simple.EncoderVersion % 100);
                break;
            default:
                sprintf(simple.Encoder, "Buschmann %u.%02u Alpha",
                    simple.EncoderVersion / 100,
                    simple.EncoderVersion % 100);
                break;
            }
        }
        // estimation, exact value needs too much time
        simple.Bitrate = (UINT)(ByteLength * 8. * simple.SampleFreq / (1152 * simple.Frames - 576));
        
    } else {
        // read the file-header (SV6 and below)
        simple.Bitrate = ((HeaderData[0] >> 23) & 0x01FF) * 1000;    // read the file-header (SV6 and below)
        simple.MS = (HeaderData[0] >> 21) & 0x0001;
        simple.IS = (HeaderData[0] >> 22) & 0x0001;
        simple.StreamVersion = (HeaderData[0] >> 11) & 0x03FF;
        simple.MaxBand = (HeaderData[0] >> 6) & 0x001F;
        simple.BlockSize = (HeaderData[0]) & 0x003F;
        
        simple.Profile = 0;
        //gain
        simple.GainTitle = 0;    // not supported
        simple.PeakTitle = 0;
        simple.GainAlbum = 0;
        simple.PeakAlbum = 0;
        //gaples
        simple.LastFrameSamples = 0;
        simple.IsTrueGapless = 0;
        
        if (simple.StreamVersion >= 5)
            simple.Frames = HeaderData[1];    // 32 bit
        else
            simple.Frames = (HeaderData[1] >> 16);    // 16 bit
        
        simple.EncoderVersion = 0;
        simple.Encoder[0] = '\0';
        if (simple.StreamVersion < 6)    // Bugfix: last frame was invalid for up to SV5
            simple.Frames -= 1;
        
        simple.SampleFreq = 44100;    // AB: used by all files up to SV7
        simple.Channels = 2;
    }
    
    simple.ProfileName = profile_stringify(simple.Profile);
    
    simple.Duration = (int) (simple.Frames * 1152 / (simple.SampleFreq/1000.0));

    return 0;
}

void Reader::idle()
{
}

bool LoadAttributeFileMPC(FILE_INFO *pFileMP3)
{
    FILE *fp = fopen(GetFullPath(pFileMP3), "rb");
    if(!fp)return false;
    CString strTrackNumber;
    Reader_file file(fp);
    StreamInfo MPCinfo;
    MPCinfo.SetFilename(GetFullPath(pFileMP3));
    MPCinfo.ReadStreamInfo(&file);
    int i, j;
    size_t  vlen;
    char*   value = NULL;
    MPC_COMMENT_SET mc[] = {
        {APE_TAG_FIELD_TITLE,   SetTrackNameSI}, //タイトル
        {APE_TAG_FIELD_ARTIST,  SetArtistNameSI},//アーティスト
        {APE_TAG_FIELD_ALBUM,   SetAlbumNameSI}, //アルバム
        {APE_TAG_FIELD_GENRE,   SetGenreSI},     //ジャンル
        {APE_TAG_FIELD_YEAR,    SetYearSI},      //年
        {APE_TAG_FIELD_COMMENT, SetCommentSI},   //コメント
        {APE_TAG_FIELD_TRACK,   SetTrackNumberSI},         //トラック番号
        {NULL,          NULL}
    };
    i = 0;
    while(mc[i].cszName){
        value = NULL;
        vlen = 0;
        int count = MPCinfo.tagitem_count;
        for( j = 0; j < count; j++ ) {
            if(_strcmpi( MPCinfo.tagitems[j].Item, mc[i].cszName) == 0 ) {
                value = MPCinfo.tagitems[j].Value;
                vlen  = MPCinfo.tagitems[j].ValueSize;
                break;
            }
        }
        if(value){
            char* tmp = (char *)malloc ((vlen + 1) * 2);
            tmp[0] = 0;
            ConvertUTF8ToANSI ( value, tmp );
            (*mc[i].setFunc)(pFileMP3, tmp);
            free(tmp);
        }
        i++;
    }
	CString strFormat;
	strFormat.Format("%d kb/s %d Hz, Profile:%s(SV%d)", MPCinfo.simple.Bitrate/1000, (int)MPCinfo.simple.SampleFreq,
						MPCinfo.simple.ProfileName, MPCinfo.simple.StreamVersion);
	SetAudioFormat(pFileMP3, strFormat);
	if (strlen(MPCinfo.simple.Encoder) > 0) {
		SetEncodest(pFileMP3, MPCinfo.simple.Encoder);
	}
	SetPlayTime(pFileMP3, MPCinfo.simple.Duration/1000);
    return true;
}

bool WriteAttributeFileMPC(FILE_INFO *pFileMP3)
{
    FILE *fp = fopen(GetFullPath(pFileMP3), "r+b");
    if(!fp)return false;
    Reader_file file(fp);
    StreamInfo MPCinfo;
    MPCinfo.SetFilename(GetFullPath(pFileMP3));
    MPCinfo.ReadStreamInfo(&file);
    MPC_COMMENT_GET mc[] = {
        {APE_TAG_FIELD_TITLE,   GetTrackNameSI}, //タイトル
        {APE_TAG_FIELD_ARTIST,  GetArtistNameSI},//アーティスト
        {APE_TAG_FIELD_ALBUM,   GetAlbumNameSI}, //アルバム
        {APE_TAG_FIELD_GENRE,   GetGenreSI},     //ジャンル
        {APE_TAG_FIELD_YEAR,    GetYearSI},      //年
        {APE_TAG_FIELD_COMMENT, GetCommentSI},   //コメント
        {APE_TAG_FIELD_TRACK,   GetTrackNumberSI},//トラック番号
        {NULL,          NULL}
    };
    int i = 0;
    while(mc[i].cszName){
        char *utf8 = (char*)malloc(strlen((*mc[i].getFunc)(pFileMP3))*2+1);
        utf8[0] = 0;
        if(ConvertANSIToUTF8((*mc[i].getFunc)(pFileMP3), utf8 ) != 0 ) {
            utf8[0] = 0;
        }
        InsertTagField(mc[i].cszName, 0, utf8, 0, 0, &MPCinfo);
        free(utf8);
        i++;
    }
    MPCinfo.tagtype = auto_tag;
    WriteTag(&file, &MPCinfo);
    return true;
}
