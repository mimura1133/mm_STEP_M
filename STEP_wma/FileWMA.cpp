//
//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//

#include "stdafx.h"
#include <TCHAR.h>
#include <stdio.h>
#include <string.h>
#include "wmsdk.h"
#include "FileWMA.h"

#ifndef SAFE_RELEASE

#define SAFE_RELEASE( x )  \
    if( NULL != x )        \
    {                      \
        x->Release( );     \
        x = NULL;          \
    }

#endif // SAFE_RELEASE

//
// List of all the ID3v2 Tag's Attribute names
//
LPCWSTR ID3TagDefs[] =
{
	L"WM/AlbumTitle", // This is same as the global variable g_wszWMAlbumTitle
	L"WM/GenreID",    // This is same as the global variable g_wszWMGenreID
	L"Author",        // This is same as the global variable g_wszWMAuthor
	L"WM/Track",      // This is same as the global variable g_wszWMTrack
	L"WM/TrackNumber",// WMP9
	L"WM/PartOfSet",
	L"Title",         // This is same as the global variable g_wszWMTitle
	L"WM/Year",       // This is same as the global variable g_wszWMYear
	L"Description",   // This is same as the global variable g_wszWMDescription
	L"WM/GenreID",
	L"WM/Genre",
	L"Copyright",
	L"Is_Protected",
	L"Duration",
	L"WM/PromotionURL",
	L"WM/AlbumCoverURL",
	L"WM/OriginalArtist",
	L"WM/Composer",		// WMP9
	L"WM/Publisher",	// WMP9
	L"WM/AlbumArtist",	// WMP9
	L"WM/Writer",
	NULL,
} ;

bool LoadAttributeFileWMA(FILE_INFO *pFileMP3)
{
	HRESULT hr = S_OK;

	LPWSTR	pwszInFile = NULL;
	hr = ConvertMBtoWC(GetFullPath(pFileMP3), &pwszInFile);
	if (FAILED(hr)) return(false);

	CoInitialize(NULL);

    IWMMetadataEditor* pEditor      = NULL;
    IWMHeaderInfo3*    pHeaderInfo  = NULL;
	BOOL	bIsProtected = FALSE;

	do {
		// Lets create a Metadata Editor.
		hr = WMCreateEditor(&pEditor);
		if (FAILED(hr)) {
			TRACE( _T( "Could not create Metadata Editor (hr=0x%08x).\n" ), hr );
			break ;
		}

		hr = pEditor->Open(pwszInFile);
		if (FAILED(hr)) {
			TRACE( _T( "Could not open outfile %ws (hr=0x%08x).\n" ), pwszInFile, hr );
			break ;
		}

		hr = pEditor->QueryInterface( IID_IWMHeaderInfo3, ( void ** ) &pHeaderInfo );
		if (FAILED(hr)) {
			TRACE( _T( "Could not QI for IWMHeaderInfo (hr=0x%08x).\n" ), hr );
			break ;
		}

		int i; for (i = 0; ID3TagDefs[i] != NULL; i++) {
			hr = LoadHeaderAttribute(pHeaderInfo, ID3TagDefs[i], pFileMP3, &bIsProtected);
			if (FAILED(hr)) break;
		}

		hr = pEditor->Close();
		if (FAILED(hr)) {
			TRACE( _T( "Could not close the file %ws (hr=0x%08x).\n" ), pwszInFile, hr) ;
			break;
		}
	}
	while(FALSE);

	SAFE_RELEASE( pHeaderInfo ) ;
	SAFE_RELEASE( pEditor ) ;

	CoUninitialize();

	delete[] pwszInFile;
	pwszInFile = NULL;

	extern bool GetValues_mp3infp(FILE_INFO *pFileMP3);
	GetValues_mp3infp(pFileMP3);

	if (bIsProtected) {
		CString fmt = GetAudioFormat(pFileMP3);
		fmt += ", DRM";
		SetAudioFormat(pFileMP3, fmt);
	}
//	return(bIsProtected ? false : true);
	if (FAILED(hr)) {
		return false;
	} else {
		return true;
	}
}

bool WriteAttributeFileWMA(FILE_INFO *pFileMP3)
{
	HRESULT hr = S_OK;

	LPWSTR	pwszInFile = NULL;
	hr = ConvertMBtoWC((LPCSTR)GetFullPath(pFileMP3), &pwszInFile);
	if (FAILED(hr)) return(false);

	CoInitialize(NULL);

	IWMMetadataEditor* pEditor      = NULL;
#ifndef USE_WMSDK9
    IWMHeaderInfo*     pHeaderInfo  = NULL;
#else
    IWMHeaderInfo3*    pHeaderInfo  = NULL;
#endif

	do {
		//
		// Lets create a Metadata Editor.
		//
		hr = WMCreateEditor( &pEditor );
		if (FAILED(hr)) {
			TRACE( _T( "Could not create Metadata Editor (hr=0x%08x).\n" ), hr );
			break ;
		}

		hr = pEditor->Open( pwszInFile ) ;
		if (FAILED(hr)) {
			TRACE( _T( "Could not open the file %ws (hr=0x%08x).\n" ), pwszInFile, hr );
			break ;
		}
		hr = pEditor->QueryInterface( IID_IWMHeaderInfo3, ( void ** ) &pHeaderInfo );
		if (FAILED(hr)) {
			TRACE( _T( "Could not QI for IWMHeaderInfo (hr=0x%08x).\n" ), hr );
			break ;
		}
		// ƒ^ƒOî•ñ‚Ìo—Í
		if (strlen(GetTrackNumberSI(pFileMP3)) > 0 && atoi(GetTrackNumberSI(pFileMP3)) > 0) {
			WriteAttributeDWORD(pHeaderInfo, L"WM/Track", atoi(GetTrackNumberSI(pFileMP3)) - 1);
			WriteAttributeDWORD(pHeaderInfo, L"WM/TrackNumber", atoi(GetTrackNumberSI(pFileMP3)));
		}
		WriteAttributeStr(pHeaderInfo, L"WM/PartOfSet" , GetDiskNumberSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"WM/AlbumTitle", GetAlbumNameSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"Author"       , GetArtistNameSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"Copyright"    , GetCopyrightSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"Title"        , GetTrackNameSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"Description"  , GetCommentSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"WM/Year"      , GetYearSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"WM/Genre"     , GetGenreSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/GenreID"   , GetGenreSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/PromotionURL", GetURLSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/AlbumCoverURL", GetOther(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/OriginalArtist", GetOrigArtistSI(pFileMP3));
		WriteAttributeStr(pHeaderInfo, L"WM/Composer"  , GetComposerSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/AlbumArtist"  , GetAlbumArtistSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/Writer"		, GetWriterSI(pFileMP3), TRUE);
		WriteAttributeStr(pHeaderInfo, L"WM/Publisher"  , GetEngineerSI(pFileMP3), TRUE); // WMP9

		hr = pEditor->Flush();
		if (FAILED(hr)) {
			TRACE( _T( "Flush failed (hr=0x%08x).\n" ), hr) ;
			break;
		}

		hr = pEditor->Close();
		if (FAILED(hr)) {
			TRACE( _T( "Could not close the file %ws (hr=0x%08x).\n" ), pwszInFile, hr) ;
			break;
		}
	}
	while( FALSE ) ;

	SAFE_RELEASE( pHeaderInfo ) ;
	SAFE_RELEASE( pEditor ) ;

	CoUninitialize();

	delete[] pwszInFile;
	pwszInFile = NULL;

	return(FAILED(hr) ? false : true);
}
