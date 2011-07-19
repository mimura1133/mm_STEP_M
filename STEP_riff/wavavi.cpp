#include "stdafx.h"
#include "RiffSif.h"
#include "STEPlugin.h"

#include "mp3infp_ExportFunc.h"

//WAV と AVI のタグ情報を読み書きする
//基本的に WAV も AVI も同じ扱い
bool LoadAttributeFileWAV(FILE_INFO *pFileMP3);
bool WriteAttributeFileWAV(FILE_INFO *pFileMP3);

bool LoadAttributeFileWAV(FILE_INFO *pFileMP3)
{
	CRiffSIF riff;
	TCHAR ext[_MAX_EXT];
	_tsplitpath(GetFullPath(pFileMP3), NULL, NULL, NULL, ext);
    if(_strcmpi(ext, ".wav") == 0){
        if(riff.Load(GetFullPath(pFileMP3),'W','A','V','E') != ERROR_SUCCESS){
            return false;
        }
    }
    else if(_strcmpi(ext, ".avi") == 0){
        if(riff.Load(GetFullPath(pFileMP3),'A','V','I',' ') != ERROR_SUCCESS){
            return false;
        }
	    //ISBJ songname
	    SetTrackNameSI(pFileMP3, riff.GetField('I','S','B','J'));
    }
    else{
        return false;
    }
    //INAM/ISBJ タイトル
    //ISBJ よりも INAM を優先
    SetTrackNameSI(pFileMP3, riff.GetField('I','N','A','M'));
    if(strlen(GetTrackNameSI(pFileMP3)) == 0){
        SetTrackNameSI(pFileMP3, riff.GetField('I','S','B','J'));
    }
	//IART アーティスト名
	SetArtistNameSI(pFileMP3, riff.GetField('I','A','R','T'));
	//IPRD アルバム名
	SetAlbumNameSI(pFileMP3, riff.GetField('I','P','R','D'));
	//ICMT コメント
	SetCommentSI(pFileMP3, riff.GetField('I','C','M','T'));
	//ICRD 日付
	SetYearSI(pFileMP3, riff.GetField('I','C','R','D'));
	//IGNR ジャンル
	SetGenreSI(pFileMP3, riff.GetField('I','G','N','R'));
	//ICOP 著作権
	SetCopyrightSI(pFileMP3, riff.GetField('I','C','O','P'));
	//IENG エンジニア	
    SetEngineerSI(pFileMP3, riff.GetField('I','E','N','G'));
	//ISRC ソース	
	SetSourceSI(pFileMP3, riff.GetField('I','S','R','C'));
	//ISFT ソフトウェア
	SetSoftwareSI(pFileMP3, riff.GetField('I','S','F','T'));
	//ITRK トラック番号
	SetTrackNumberSI(pFileMP3, riff.GetField('I','T','R','K'));

	extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
	GetValues_mp3infp(pFileMP3);
    return true;
}

bool WriteAttributeFileWAV(FILE_INFO *pFileMP3)
{
	CRiffSIF riff;
	TCHAR ext[_MAX_EXT];
	_tsplitpath(GetFullPath(pFileMP3), NULL, NULL, NULL, ext);
    if(_strcmpi(ext, ".wav") == 0){
        if(riff.Load(GetFullPath(pFileMP3),'W','A','V','E') != ERROR_SUCCESS){
            return false;
        }
    }
    else if(_strcmpi(ext, ".avi") == 0){
        if(riff.Load(GetFullPath(pFileMP3),'A','V','I',' ') != ERROR_SUCCESS){
            return false;
        }
    }
    else{
        return false;
    }
    //INAM タイトル
	riff.SetField('I','N','A','M', GetTrackNameSI(pFileMP3));
	riff.SetField('I','S','B','J', ""); /* 2005.12.26 mp3infpと同様に ISBJを削除 */
	//IART アーティスト名
	riff.SetField('I','A','R','T', GetArtistNameSI(pFileMP3));
	//IPRD アルバム名
	riff.SetField('I','P','R','D', GetAlbumNameSI(pFileMP3));
	//ICMT コメント
	riff.SetField('I','C','M','T', GetCommentSI(pFileMP3));
	//ICRD 日付
	riff.SetField('I','C','R','D', GetYearSI(pFileMP3));
	//IGNR ジャンル
	riff.SetField('I','G','N','R', GetGenreSI(pFileMP3));
	//ICOP 著作権
	riff.SetField('I','C','O','P', GetCopyrightSI(pFileMP3));
	//IENG エンジニア	
	riff.SetField('I','E','N','G', GetEngineerSI(pFileMP3));
	//ISRC ソース	
	riff.SetField('I','S','R','C', GetSourceSI(pFileMP3));
	//ISFT ソフトウェア
	riff.SetField('I','S','F','T', GetSoftwareSI(pFileMP3));
	//ITRK トラック番号
	riff.SetField('I','T','R','K', GetTrackNumberSI(pFileMP3));
    return riff.Save(NULL, GetFullPath(pFileMP3)) == ERROR_SUCCESS;
}
