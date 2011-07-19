/**************************************************************************

	Profile.h

	(C) Copyright 1996-2002 By Tomoaki Nakashima. All right reserved.
		http://www.nakka.com/
		nakka@nakka.com

**************************************************************************/

#ifndef _INC_PROFILE_H
#define _INC_PROFILE_H

/**************************************************************************
	Include Files
**************************************************************************/

/**************************************************************************
	Define
**************************************************************************/

/**************************************************************************
	Global Variables
**************************************************************************/

/**************************************************************************
	Struct
**************************************************************************/

/**************************************************************************
	Function Prototypes
**************************************************************************/

BOOL Profile_Initialize(const TCHAR *path, const BOOL ReadFlag);
BOOL Profile_Flush(const TCHAR *path);
void Profile_Free(void);
long Profile_GetString(const TCHAR *Section, const TCHAR *Key, const TCHAR *Default, TCHAR *ret, const long size, const TCHAR *File);
int Profile_GetInt(const TCHAR *Section, const TCHAR *Key, const int Default, const TCHAR *File);
BOOL Profile_WriteString(const TCHAR *Section, const TCHAR *Key, const TCHAR *str, const TCHAR *File);
BOOL Profile_WriteInt(const TCHAR *Section, const TCHAR *Key, const int num, const TCHAR *File);

#endif
/* End of source */
