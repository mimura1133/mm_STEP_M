//
//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//


#ifndef __FILE_WMA_H__
#define __FILE_WMA_H__

#include "STEPlugin.h"

#define STREAM_NUM	0x0000

HRESULT LoadHeaderAttribute(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszName, FILE_INFO *pFileMP3, BOOL *pIsProtected);
HRESULT LoadHeaderAttribute(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszName, FILE_INFO *pFileMP3, BOOL *pIsProtected);

bool WriteAttributeStr(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszAttrName, LPCTSTR sValue, BOOL bSeparate = FALSE);
bool WriteAttributeStr(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName, LPCTSTR sValue, BOOL bSeparate = FALSE);
bool WriteAttributeDWORD(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszAttrName, DWORD dwValue);
bool WriteAttributeDWORD(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName, DWORD dwValue);

void DeleteAttribute(IWMHeaderInfo3 *pHeaderInfo, LPCWSTR pwszAttrName);

#ifndef UNICODE
HRESULT ConvertMBtoWC(LPCTSTR ptszInString, LPWSTR *ppwszOutString);
#endif

#endif  // __FILE_WMA_H__
