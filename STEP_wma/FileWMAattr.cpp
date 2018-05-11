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

#include "split_string.h"

namespace yol {
	auto make_string_view(const CStringW& text) -> std::basic_string_view<wchar_t>
	{
		return { static_cast<LPCWSTR>(text) };
	}
}

void SetAttribute(LPCWSTR pwszName, WMT_ATTR_DATATYPE type, BYTE* pValue, DWORD cbLength, FILE_INFO *pFileMP3, BOOL *pIsProtected, BOOL bAppend)
{
	CStringA strName = pwszName;
	CString	strData;
    switch(type) {
    case WMT_TYPE_DWORD:
		strData.Format(TEXT("%u"), *((DWORD *)pValue));
        //TRACE( _T("%ws:  %u\n" ), pwszName, *((DWORD *)pValue) );
        break;
    case WMT_TYPE_STRING:
		strData = (LPCWSTR)pValue;
        //TRACE( _T("%ws:   %ws\n" ), pwszName, (WCHAR *)pValue );
        break;
    case WMT_TYPE_BINARY:
        //TRACE( _T("%ws:   Type = Binary of Length %u\n" ), pwszName, cbLength );
        break;
    case WMT_TYPE_BOOL:
		strData.Format(TEXT("%d"), *((BOOL *)pValue) ? 1 : 0);
        //TRACE( _T("%ws:   %s\n" ), pwszName, ( * ( ( BOOL * ) pValue) ? _T( "true" ) : _T( "false" ) ) );
        break;
    case WMT_TYPE_WORD:
		strData.Format(TEXT("%hu"), *((WORD *)pValue));
        //TRACE( _T("%ws:  %hu\n" ), pwszName, *((WORD *) pValue) );
        break;
    case WMT_TYPE_QWORD:
		strData.Format(TEXT("%I64u"), *((QWORD *)pValue));
        //TRACE( _T("%ws:  %I64u\n" ), pwszName, *((QWORD *) pValue) );
        break;
    case WMT_TYPE_GUID:
		strData.Format(TEXT("%I64x%I64x"), *((QWORD *)pValue), *((QWORD *)pValue+1));
        //TRACE( _T("%ws:  %I64x%I64x\n" ), pwszName, *((QWORD *) pValue), *((QWORD *) pValue + 1) );
        break;
    default:
        //TRACE( _T("%ws:   Type = %d, Length %u\n" ), pwszName, type, cbLength );
        break;
    }
	if (strData.IsEmpty() == FALSE) {
		CString strPrev;
		if (false) { /* Conspiracy 198 */
//		if (strcmp(strName, "Duration") == 0) {
//			// ���t����
//			pFileMP3->nPlayTime = (int)((*((QWORD *)pValue) / (QWORD)10000000));
//		} else if (strcmp(strName, "Bitrate") == 0) {
//			// �r�b�g���[�g
//			pFileMP3->nBitRate = _tstoi(strData) / 1000;
		} else if (_strcmpi(strName, "WM/Track") == 0) {
			// �g���b�N�ԍ�
			//SetBTrackNumber(pFileMP3, (BYTE)_tstoi(strData) + 1);
			CString strTrack;
			strTrack.Format(TEXT("%d"), _tstoi(strData) + 1);
			SetTrackNumberSI(pFileMP3, strTrack);
		} else if (_strcmpi(strName, "WM/TrackNumber") == 0) {
			// �g���b�N�ԍ�
			if (_tstoi(strData) == 0) {
				//SetBTrackNumber(pFileMP3, (BYTE)0xFF);
				SetTrackNumberSI(pFileMP3, TEXT(""));
			} else {
				//SetBTrackNumber(pFileMP3, (BYTE)_tstoi(strData));
				CString strTrack;
				strTrack.Format(TEXT("%d"), _tstoi(strData));
				SetTrackNumberSI(pFileMP3, strTrack);
			}
		} else if (_strcmpi(strName, "WM/PartOfSet") == 0) {
			// �f�B�X�N�ԍ�
			CString strDisk;
			strDisk.Format(TEXT("%d"), _tstoi(strData));
			SetDiskNumberSI(pFileMP3, strDisk);
		}  else if (_strcmpi(strName, "WM/Year") == 0) {
			// �����[�X
			if (bAppend) {
				strPrev = GetYearSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetYearSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/AlbumTitle") == 0) {
			// �A���o��
			if (bAppend) {
				strPrev = GetAlbumNameSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetAlbumNameSI(pFileMP3,  strData);
		} else if (_strcmpi(strName, "Author") == 0) {
			// �A�[�e�B�X�g
			if (bAppend) {
				strPrev = GetArtistNameSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetArtistNameSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "Title") == 0) {
			// �^�C�g��
			if (bAppend) {
				strPrev = GetTrackNameSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetTrackNameSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "Description") == 0) {
			// �R�����g
			if (bAppend) {
				strPrev = GetCommentSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetCommentSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/Genre") == 0) {
			// �W������
			if (bAppend) {
				strPrev = GetGenreSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetGenreSI(pFileMP3, strData);
			//SetBGenre(pFileMP3, STEPGetGenreCode(strData));
		} else if (_strcmpi(strName, "WM/GenreID") == 0) {
			// �W������
			if (bAppend) {
				strPrev = GetGenreSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetGenreSI(pFileMP3, strData);
			//SetBGenre(pFileMP3, STEPGetGenreCode(strData));
		} else if (_strcmpi(strName, "Copyright") == 0) {
			if (bAppend) {
				strPrev = GetCopyrightSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetCopyrightSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/PromotionURL") == 0) {
			if (bAppend) {
				strPrev = GetURLSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetURLSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/AlbumCoverURL") == 0) {
			if (bAppend) {
				strPrev = GetOther(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetOther(pFileMP3, strData);
		} else if (_strcmpi(strName, "Is_Protected") == 0) {
			// DRM ���L���ȃf�[�^
			*pIsProtected = (_tstoi(strData) == 1) ? TRUE : FALSE;
		} else if (_strcmpi(strName, "WM/OriginalArtist") == 0) {
			if (bAppend) {
				strPrev = GetOrigArtistSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetOrigArtistSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/Composer") == 0) {
			if (bAppend) {
				strPrev = GetComposerSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetComposerSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/Publisher") == 0) {
			if (bAppend) {
				strPrev = GetEngineerSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetEngineerSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/AlbumArtist") == 0) {
			if (bAppend) {
				strPrev = GetAlbumArtistSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetAlbumArtistSI(pFileMP3, strData);
		} else if (_strcmpi(strName, "WM/Writer") == 0) {
			if (bAppend) {
				strPrev = GetWriterSI(pFileMP3);
				strData = strPrev + ";" + strData;
			}
			SetWriterSI(pFileMP3, strData);
		}

	}
}

HRESULT LoadHeaderAttribute(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszName, FILE_INFO *pFileMP3, BOOL *pIsProtected)
{
	HRESULT hr         = S_OK ;

	WORD    nstreamNum = 0 ;
	WORD    cbLength   = 0 ;
	BYTE*   pValue     = NULL ;

	WMT_ATTR_DATATYPE type ;
	// Get the no of bytes to be allocated for pValue
	hr = pHeaderInfo->GetAttributeByName( &nstreamNum,
										  pwszName,
										  &type,
										  NULL,
										  &cbLength );

	if( FAILED( hr ) && hr != ASF_E_NOTFOUND ) {
		TRACE( _T( "GetAttributeByName failed for Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
		return hr ;
	}

	if( cbLength == 0 && hr == ASF_E_NOTFOUND ) {
		hr = S_OK ;
		return hr ;
	}

	if (cbLength == 0) return(S_OK);

	pValue = new BYTE[ cbLength ];

	if( NULL == pValue ) {
		hr = HRESULT_FROM_WIN32( GetLastError() ) ;

		TRACE( _T( "Internal Error (hr=0x%08x).\n" ), hr ) ;
		return hr ;
	}

	//
	// Now, Get the value
	//
	hr = pHeaderInfo->GetAttributeByName(&nstreamNum, pwszName, &type, pValue, &cbLength );
	if( FAILED( hr ) ) {
		delete[] pValue ;

		TRACE( _T( "GetAttributeByName failed for Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
		return hr ;
	}

	SetAttribute(pwszName, type, pValue, cbLength, pFileMP3, pIsProtected, FALSE);

	delete[] pValue;
	pValue = NULL;

	return hr;
}

HRESULT LoadHeaderAttribute(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszName, FILE_INFO *pFileMP3, BOOL *pIsProtected)
{
#ifdef __IWMHeaderInfo3_FWD_DEFINED__ 
	HRESULT hr         = S_OK ;

	WORD    nstreamNum = 0 ;

	IWMHeaderInfo3*    pHeaderInfo3 = pHeaderInfo;
	WORD wCount = 0;
	WORD wLangIndex = 0;
	if (pHeaderInfo3->GetAttributeIndices(STREAM_NUM, pwszName, &wLangIndex, NULL, &wCount) != S_OK || wCount <= 0) {
		return hr;
	}
	WORD* wIndices = new WORD[wCount];
	if (pHeaderInfo3->GetAttributeIndices(STREAM_NUM, pwszName, &wLangIndex, wIndices, &wCount) == S_OK && wCount > 0) {
		WORD    Language = 0;
		BYTE   pbValue[1024+2];
		DWORD   cbValue  = 1024;
		WMT_ATTR_DATATYPE type;
		for (int i=0;i<wCount;i++) {
			Language = 0;
			cbValue = 1024;
			hr = pHeaderInfo3->GetAttributeByIndexEx(STREAM_NUM, wIndices[i], NULL, NULL, &type, &Language, pbValue, &cbValue);
			if (FAILED(hr)) {
				TRACE( _T( "GetAttributeByIndexEx failed for Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
			} else {
				SetAttribute(pwszName, type, pbValue, cbValue, pFileMP3, pIsProtected, i == 0 ? FALSE : TRUE);
			}
		}
	}
	delete [] wIndices;

#else
	#error Undefined symbol <IWMHeaderInfo3>
#endif
	return hr;
}

void DeleteAttribute(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName)
{
	WORD wLangIndex = 0;
	WORD wCount = 0;
	if (pHeaderInfo->GetAttributeIndices(STREAM_NUM, pwszAttrName, &wLangIndex, NULL, &wCount) == S_OK && wCount > 0) {
		WORD* wIndices = new WORD[wCount];
		pHeaderInfo->GetAttributeIndices(STREAM_NUM, pwszAttrName, &wLangIndex, wIndices, &wCount);
		for (int i=0;i<wCount;i++) {
			HRESULT hr = pHeaderInfo->DeleteAttribute(STREAM_NUM, wIndices[i]);
			if (FAILED(hr)) {
				TRACE( _T( "DeleteAttribute failed for Attribute name %ws %02x (hr=0x%08x).\n" ), pwszAttrName, wIndices[i], hr) ;
			}
		}
		delete [] wIndices;
	}
}

bool WriteAttributeStr(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszAttrName, LPCTSTR sValue, BOOL /*bSeparate*/)
{
	CStringW pwszValue = sValue;

	HRESULT hr = pHeaderInfo->SetAttribute(0, pwszAttrName, WMT_TYPE_STRING,
									reinterpret_cast<LPBYTE>(pwszValue.GetBuffer()), (wcslen(pwszValue)+1) * sizeof(WCHAR));

	if (FAILED(hr)) {
		TRACE( _T( "SetAttribute failed for Attribute name %ws (hr=0x%08x).\n" ), pwszAttrName, hr) ;
		return(false);
	}
	return(true);
}

bool WriteAttributeStr(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName, LPCTSTR sValue, BOOL bSeparate)
{
	//DeleteAttribute(pHeaderInfo, pwszAttrName);

	HRESULT hr = S_OK;

	WORD wLangIndex = 0;
	WORD wCount = 0;
	WORD* wIndices = NULL;
	if (pHeaderInfo->GetAttributeIndices(STREAM_NUM, pwszAttrName, &wLangIndex, NULL, &wCount) == S_OK && wCount > 0) {
		wIndices = new WORD[wCount];
		hr = pHeaderInfo->GetAttributeIndices(STREAM_NUM, pwszAttrName, &wLangIndex, wIndices, &wCount);
		if (FAILED(hr)) {
			TRACE( _T( "GetAttributeIndices failed for Attribute name %ws (hr=0x%08x).\n" ), pwszAttrName, hr) ;
			wCount = 0;
		}
	}

	CStringW strData = sValue;
	int nIndex = 0;
	for (auto strToken : yol::split_string(strData, bSeparate ? L';' : L'\0')) {
		if (wCount > nIndex) {
			hr = pHeaderInfo->ModifyAttribute(
				STREAM_NUM,
				wIndices[nIndex],
				WMT_TYPE_STRING,
				0,
				reinterpret_cast<const BYTE*>(strToken.data()),
				strToken.length() * sizeof(WCHAR));
			TRACE(_T("ModifyAttribute #%d for Attribute name %ws value %s #%02x (hr=0x%08x).\n"), nIndex + 1,
				pwszAttrName, strToken, wIndices[nIndex], hr);
		} else {
			hr = pHeaderInfo->AddAttribute(
				STREAM_NUM,
				pwszAttrName,
				nullptr,
				WMT_TYPE_STRING,
				0,
				reinterpret_cast<const BYTE*>(strToken.data()),
				strToken.length() * sizeof(WCHAR));
			TRACE(_T("AddAttribute #%d for Attribute name %ws value %s (hr=0x%08x).\n"), nIndex + 1,
				pwszAttrName, strToken, hr);
		}

		if (FAILED(hr)) {
			TRACE(_T("AddAttribute/ModifyAttribute failed for Attribute name %ws (hr=0x%08x).\n"), pwszAttrName, hr);
			break;
		}

		nIndex++;
	}

	if (!FAILED(hr)) {
		int i; for (int i=nIndex;i<wCount;i++) {
			HRESULT hr = pHeaderInfo->DeleteAttribute(STREAM_NUM, wIndices[i]);
			if (FAILED(hr)) {
				TRACE( _T( "DeleteAttribute failed for Attribute name %ws #%02x (hr=0x%08x).\n" ), pwszAttrName, wIndices[nIndex], hr) ;
			}
		}
	}
	delete [] wIndices;
	if (FAILED(hr)) {
		return(false);
	}
	return(true);
}

bool WriteAttributeDWORD(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszAttrName, DWORD dwValue)
{
	HRESULT hr = S_OK;
	hr = pHeaderInfo->SetAttribute(0, pwszAttrName, WMT_TYPE_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	if (FAILED(hr)) {
		TRACE( _T( "SetAttribute failed for Attribute name %ws (hr=0x%08x).\n" ), pwszAttrName, hr) ;
		return(false);
	}
	return(true);
}

bool WriteAttributeDWORD(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName, DWORD dwValue)
{
	HRESULT hr = S_OK;
	hr = pHeaderInfo->SetAttribute(0, pwszAttrName, WMT_TYPE_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	if (FAILED(hr)) {
		TRACE( _T( "SetAttribute failed for Attribute name %ws (hr=0x%08x).\n" ), pwszAttrName, hr) ;
		return(false);
	}
	return(true);
}

#ifndef UNICODE

HRESULT ConvertMBtoWC( LPCTSTR ptszInString, LPWSTR *ppwszOutString )
{
	if( ptszInString == NULL || ppwszOutString == NULL )
	{
		return E_INVALIDARG ;
	}

    HRESULT hr = S_OK ;

	int nSizeCount = 0 ;
    do
    {
        //
	    // Get the memory reqd for this string
	    //
        nSizeCount = MultiByteToWideChar( CP_ACP, 0, ptszInString, -1, NULL, 0 ) ;
        if( 0 ==  nSizeCount )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() ) ;
            break ;
        }

        *ppwszOutString = new WCHAR[nSizeCount] ;
        if( NULL == *ppwszOutString )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() ) ;
            break ;
        }

        if( 0 == MultiByteToWideChar( CP_ACP, 0, ptszInString, -1, *ppwszOutString, nSizeCount ) )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() ) ;
            break ;
        }
    }
    while( FALSE ) ;

    if( FAILED( hr ) )
    {
        delete[] (*ppwszOutString) ;
        *ppwszOutString = NULL ;
        TRACE( _T( "Internal error (hr=0x%08x)\n" ), hr );
    }

	return hr;
}

#endif // UNICODE
