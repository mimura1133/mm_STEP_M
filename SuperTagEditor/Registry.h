#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "INI/Profile.h"

#define InitProfile()							Profile_Initialize(strINI, TRUE)
#define	FreeProfile()							Profile_Free()
#define	SaveProfile()							Profile_Flush(strINI)
#define MyWriteProfileInt(sSec, sKey, value)	Profile_WriteInt(sSec, sKey, value, strINI)
#define MyGetProfileInt(sSec, sKey, value)		Profile_GetInt(sSec, sKey, value, strINI)
#define MyWriteProfileString(sSec, sKey, value)	Profile_WriteString(sSec, sKey, value, strINI)
#define MyGetProfileString(sSec, sKey, value)	MyProfile_GetString(sSec, sKey, value, strINI)

inline CString MyProfile_GetString(LPCTSTR sSec, LPCTSTR sKey, LPCTSTR value, LPCTSTR strINI) {
	static CString buff;
	buff.Empty();
	Profile_GetString(sSec, sKey, value == NULL ? TEXT("") : value, buff.GetBuffer(1024*2+1), 1024*2, strINI);
	buff.ReleaseBuffer();
	return buff;
}

#endif //__REGISTRY_H__