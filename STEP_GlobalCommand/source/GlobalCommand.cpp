#include "stdafx.h"

#pragma comment(lib,"Version.lib")

//�����Ă���\���̂���Q�o�C�g�����Ή�
void mbsncpy2(unsigned char *dst,unsigned char *src,int c)
{
	int i; for(i=0; i<c; i++)
	{
		if(IsDBCSLeadByte(src[i]))
		{
			//�����Ă��郊�[�h�o�C�g�̓R�s�[���Ȃ�
			if((i+1) == c)
			{
				dst[i] = '\0';
				break;
			}
			dst[i] = src[i];
			i++;
		}
		dst[i] = src[i];
		if(src[i] == '\0')
		{
			break;
		}
	}
}

BOOL GetDLLVersion(IN LPTSTR szDLLFileName,
				   IN DWORD *pdwMajor,
				   IN DWORD *pdwMinor,
				   IN DWORD *pdwBuildNumber1,
				   IN DWORD *pdwBuildNumber2)
{
	*pdwMajor = 0;
	*pdwMinor = 0;
	*pdwBuildNumber1 = 0;
	*pdwBuildNumber2 = 0;
	TCHAR fileVersion[256];

	UINT len;
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(szDLLFileName,&dwHandle);
	if(dwSize)
	{
		char *versionInfo = (char *)malloc(dwSize+1);
		if(GetFileVersionInfo(szDLLFileName,dwHandle,dwSize,versionInfo))
		{
			_stprintf_s(fileVersion, TEXT("\\VarFileInfo\\Translation"));
			LPTSTR version = NULL;
			BOOL bRet = VerQueryValue(versionInfo, fileVersion, (LPVOID*)&version, &len);
			if(bRet && len == 4)
			{
				DWORD dwLangD;
				memcpy(&dwLangD,version,4);
				_stprintf_s(fileVersion, TEXT("\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion"),
					(dwLangD & 0xff00)>>8,dwLangD & 0xff,(dwLangD & 0xff000000)>>24,(dwLangD & 0xff0000)>>16);
			}
			else
			{
				_stprintf_s(fileVersion, TEXT("\\StringFileInfo\\%04X04B0\\FileVersion"), GetUserDefaultLangID());
			}
			bRet = VerQueryValue(versionInfo, fileVersion, (LPVOID*)&version, &len);
			if(!bRet)
			{
				free(versionInfo);
				return FALSE;
			}
			_tcsncpy_s(fileVersion, version, 255);
			fileVersion[255] = '\0';
			free(versionInfo);
		}
		else
		{
			free(versionInfo);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	

	//�o�[�W�������𐔎��ɕ���
	LPTSTR context = nullptr;
	auto ptr = _tcstok_s(fileVersion, TEXT(",. "), &context);
	if(ptr == NULL)
		return TRUE;
	*pdwMajor = _tstoi(ptr);
	
	ptr = _tcstok_s(NULL, TEXT(",. "), &context);
	if(ptr == NULL)
		return TRUE;
	*pdwMinor = _tstoi(ptr);
	
	ptr = _tcstok_s(NULL, TEXT(",. "), &context);
	if(ptr == NULL)
		return TRUE;
	*pdwBuildNumber1 = _tstoi(ptr);
	
	ptr = _tcstok_s(NULL, TEXT(",. "), &context);
	if(ptr == NULL)
		return TRUE;
	*pdwBuildNumber2 = _tstoi(ptr);

	return TRUE;
}

//������Yen�̂Ƃ�TRUE
BOOL IsTailYenSign(LPCTSTR szStr)
{
	const auto yen = _tcsrchr(szStr, '\\');
	if(yen && (&szStr[lstrlen(szStr)-1] == yen))
	{
		return TRUE;
	}
	return FALSE;
}

//������'\\'��ǉ�(����'\\'�̂Ƃ��͉������Ȃ�)
void AddTAilYenSigne(LPTSTR szStr)
{
	if(!IsTailYenSign(szStr))
	{
		lstrcat(szStr, TEXT("\\"));
	}
}

void AddTAilYenSigne(CString &str)
{
	if(!IsTailYenSign(str))
	{
		str += "\\";
	}
}

#ifdef _UNICODE
#define CHAR_FUDGE 1    // one TCHAR unused is good enough
#else
#define CHAR_FUDGE 2    // two BYTES unused for case of DBC last char
#endif

//LoadString�̃o�O�΍�(MSKB Q140452)
CString LoadResString(HINSTANCE hInstance,UINT uID)
{
	TCHAR szTmp[256*2];	//UNICODE��256����
	CString strRet;

	//�K�v�ȃT�C�Y���v�Z
	int iLength = LoadString(hInstance,uID,szTmp,256);
	if(256 - iLength > CHAR_FUDGE)
	{
		return szTmp;	//256�ő��肽�ꍇ
	}
	
	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	do
	{
		nSize += 256;
		iLength = LoadString(hInstance,uID,strRet.GetBuffer(nSize*2-1),nSize);
	}while(nSize - iLength <= CHAR_FUDGE);
	strRet.ReleaseBuffer();

	return strRet;
}

//---------------------------------------------------------------------
//���r�b�O�G���f�B�A��->���g���G���f�B�A���̕ϊ�
DWORD ExtractI4(unsigned char buf[4])
{
	DWORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	x <<= 8;
	x |= buf[2];
	x <<= 8;
	x |= buf[3];
	return x;
}

//---------------------------------------------------------------------
//���r�b�O�G���f�B�A��->���g���G���f�B�A���̕ϊ�
WORD ExtractI2(unsigned char buf[2])
{
	WORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	return x;
}

LPCTSTR getFileName(LPCTSTR szPath)
{
	auto szPtr = szPath;

	while(*szPtr != '\0')
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(*szPtr) == 0)
		{
			//[\],[/],[:]���������猻�ݒn+1�̃|�C���^��ۑ�
			if((*szPtr == '\\') || (*szPtr == '/') || (*szPtr == ':'))
			{
				szPath=szPtr+1;
			}
		}
		//���̕�����
		szPtr=CharNext(szPtr);
	}
	return szPath;
}

CString getFileNameExtName(const CString& path)
{
	int i = 0;
	int pathOffset = 0;
	while(path.GetLength() > i)
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(path[i]) == 0)
		{
			//[\],[/],[:]���������猻�ݒn+1�̃|�C���^��ۑ�
			if((path[i] == '\\') || (path[i] == '/') || (path[i] == ':'))
			{
				pathOffset = i+1;
			}
			i++;
		}
		else
		{
			i += 2;
		}
	}

	return path.Right(path.GetLength() - pathOffset);
}

LPCTSTR getExtName(LPCTSTR szPath)
{
	auto szPtr = szPath;

	//�t�@�C���������𕪗�
	szPtr=getFileName(szPath);
	//�g���q���܂܂Ȃ��Ƃ���""�ւ̃|�C���^
	szPath+=lstrlen(szPath);
	while(*szPtr != '\0')
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(*szPtr) == 0)
		{
			//[.]���������猻�ݒn�̃|�C���^��ۑ�
			if(*szPtr == '.')
			{
				szPath=szPtr;
			}
		}
		//���̕�����
		szPtr=CharNext(szPtr);
	}
	return szPath;
}

CString getExtName(CString &path)
{
	//�t�@�C���������𕪗�
	CString fName = getFileName(path);

	//�g���q���܂܂Ȃ��Ƃ���""�ւ̃|�C���^
	int i = 0;
	int pathOffset = -1;
	while(fName.GetLength() > i)
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(fName[i]) == 0)
		{
			//[.]���������猻�ݒn�̃|�C���^��ۑ�
			if(fName[i] == '.')
			{
				pathOffset = i;
			}
			i++;
		}
		else
		{
			i += 2;
		}
	}

	if(pathOffset == -1)
	{
		return "";
	}
	else
	{
		return fName.Right(fName.GetLength() - pathOffset);
	}
}

long check2ByteLength(const char *szTag,long lLimit)
{
	long i=0;
	while(szTag[i])
	{
		BOOL bIsLb = IsDBCSLeadByte(szTag[i]);
		if(bIsLb)
			i++;
		if(i >= lLimit)
		{
			if(bIsLb)
				return (i-1);
			return i;
		}
		i++;
	}
	return i;
}

void sysError(HWND hWnd, LPCTSTR mes)
{
	LPTSTR lpBuffer;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL );
	MessageBox(hWnd,lpBuffer,mes,MB_APPLMODAL | MB_ICONSTOP);
	LocalFree(lpBuffer);
	return;
}

void errMessageBox(HWND hWnd, DWORD dwErrorCode, LPCTSTR mes)
{
	LPTSTR lpBuffer;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwErrorCode,
			0,
			(LPTSTR )&lpBuffer,
			0,
			NULL );
	MessageBox(hWnd,lpBuffer,mes,MB_APPLMODAL | MB_ICONSTOP);
	LocalFree(lpBuffer);
	return;
}

void cutFileName(LPTSTR szPath)
{
	auto szEnd=szPath;
	
	while(*szPath != '\0')
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(*szPath) == 0)
		{
			if((*szPath == '\\') || (*szPath == '/') || (*szPath == ':'))
			{
				szEnd=szPath+1;
			}
		}
		szPath=CharNext(szPath);
	}
	//�p�X���Ƀt�@�C�����܂܂Ȃ������ꍇ�������Ȃ�
	if(szEnd == szPath)
	{
		return;
	}
	//�p�X������t�@�C������؂藣��
	*szEnd='\0';
	return;
}

CString getPathName(CString &path)
{
	int i = 0;
	int pathOffset = 0;
	while(path.GetLength() > i)
	{
		//�Q�o�C�g�����̐擪�̓X�L�b�v
		if(IsDBCSLeadByte(path[i]) == 0)
		{
			if((path[i] == '\\') || (path[i] == '/') || (path[i] == ':'))
			{
				pathOffset = i + 1;
			}
			i++;
		}
		else
		{
			i += 2;
		}
	}

	return path.Left(pathOffset);
}

void cutExtName(char *szFileName)
{
	char *pEnd = szFileName;
	char *pStart=szFileName;
	
	while(*pEnd != '\0')
	{
		if(IsDBCSLeadByte(*pEnd) != 0)
		{
			pEnd++;
		}else
		{
			if((*pEnd == '\\') || (*pEnd == '/') || (*pEnd == ':'))
			{
				pStart=pEnd+1;
			}
		}
		pEnd++;
	}
	pEnd=szFileName;
	while(*pStart != '\0')
	{
		if(IsDBCSLeadByte(*pStart) != 0)
		{
			pStart++;
		}else
		{
			if(*pStart == '.')
			{
				pEnd=pStart;
			}
		}
		pStart++;
	}
	//�t�@�C�����Ɋg���q���܂܂Ȃ������ꍇ�������Ȃ�
	if(pEnd == szFileName)
	{
		return;
	}
	//�t�@�C��������g���q��؂藣��
	*pEnd='\0';
}

CString getFileName(const CString& path)
{
	CString fName = getFileNameExtName(path);
	
	int i = 0;
	int pathOffset = 0;
	while(fName.GetLength() > i)
	{
		if(IsDBCSLeadByte(fName[i]) == 0)
		{
			if(fName[i] == '.')
			{
				pathOffset = i;
			}
			i++;
		}
		else
		{
			i += 2;
		}
	}

	return fName.Left(pathOffset);
}

CString divString(char *src,char c,int n)
{
	CString ret;
	int cnt = (n - strlen(src)%n)%n;
	while(*src)
	{
		ret += *src;
		src++;
		cnt++;
		if(*src && (cnt == n))
		{
			cnt = 0;
			ret += c;
		}
	}
	return ret;
}
