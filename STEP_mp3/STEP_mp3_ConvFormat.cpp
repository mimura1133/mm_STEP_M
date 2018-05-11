#include "stdafx.h"
#include "STEP_mp3.h"
#include "ID3v2/Id3tagv1.h"
#include "ID3v2/Id3tagv2.h"
#include "ID3v2/RMP.h"
#include "STEPlugin.h"

extern UINT nFileTypeMP3;
extern UINT nFileTypeMP3V1;
extern UINT nFileTypeMP3V11;
extern UINT nFileTypeID3V2;
extern UINT nFileTypeRMP;
extern bool	bOptAutoConvID3v2;
extern bool	bOptAutoConvRMP;
extern bool	bOptID3v2ID3tagAutoWrite;
extern bool	bOptRmpID3tagAutoWrite;
extern bool	bOptChangeFileExt;

extern int		nId3v2EncodeNew;
extern int		nId3v2VersionNew;

bool ConvExt(FILE_INFO* pFileMP3)
{
	// �g���q�̕ύX
	UINT nFormat = GetFormat(pFileMP3);
	if (bOptChangeFileExt) {
		TCHAR	fname[_MAX_FNAME];
		_tsplitpath(GetFileName(pFileMP3), NULL, NULL, fname, NULL);
		CString strNewFileName = GetFileName(pFileMP3);

		if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11 || nFormat == nFileTypeID3V2) {
			// MP3 �`���ɕϊ�
			strNewFileName.Format(TEXT("%s%s"), fname, TEXT(".mp3"));
		} else if (nFormat == nFileTypeRMP) {
			// RIFF MP3 �`���ɕϊ�
			strNewFileName.Format(TEXT("%s%s"), fname, TEXT(".rmp"));
		}
		if (STEPFileNameChange(pFileMP3, strNewFileName) == false) {
			return false;
		}
	}
	return true;
}

bool DeleteTagID3v1(LPCTSTR sFileName, HWND hWnd)
{
	// ID3v1 �^�O�̎擾
	CId3tagv1	id3v1/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v1.Load(sFileName) != ERROR_SUCCESS 	// �ǂݍ��ݎ��s
		||  id3v1.IsEnable() == FALSE) {		// ID3v1 �ł͂Ȃ�
		return false;
	}
	// ID3v1 �^�O�̍폜
	if (id3v1.DelTag(hWnd, sFileName) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

bool DeleteTagID3v2(LPCTSTR sFileName, HWND hWnd)
{
	// ID3v2 �^�O�̎擾
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	if (id3v2.Load(sFileName) != ERROR_SUCCESS 	// �ǂݍ��ݎ��s
	||  id3v2.IsEnable() == FALSE			// ID3v2 �ł͂Ȃ�
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// ���Ή��̃o�[�W����
		return false;
	}
	// ID3v2 �^�O�̍폜
	if (id3v2.DelTag(/*hWnd,*/ sFileName) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

bool DeleteTagSIF(LPCTSTR sFileName, HWND hWnd) {
	CRMP rmp;
	if (rmp.Load(sFileName) != ERROR_SUCCESS) {
		return false;
	}
	if (rmp.DelTag(hWnd, sFileName) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

bool MakeTagID3v2(LPCTSTR sFileName, HWND hWnd)
{
	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
	switch (nId3v2VersionNew) {
	case 0:	// v2.2
		id3v2.SetVer(0x0200);
		break;
	case 1:	// v2.3
		id3v2.SetVer(0x0300);
		break;
	case 2:	// v2.4
		id3v2.SetVer(0x0400);
		break;
	default:
		break;
	}
	switch (nId3v2EncodeNew) {
	case 0:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_ISO_8859_1);
		break;
	case 1:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16);
		break;
	case 2:
		id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_8);
		break;
	//case 3:
	//	id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16BE);
	//	break;
	default:
		break;
	}

	if (id3v2.MakeTag(/*hWnd,*/ sFileName) != ERROR_SUCCESS) {
		// �ϊ����s
		return(false);
	}
	return(true);
}

bool MakeTagSIF(LPCTSTR sFileName, HWND hWnd) {
	CRMP rmp;
	if (rmp.MakeTag(hWnd, sFileName) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

bool ConvFileFormat(FILE_INFO* pFileMP3, UINT nType, HWND hWnd) {
	// ���O�Ƀ^�O���X�V
	int nFormat = GetFormat(pFileMP3);
	if (nType == nFileTypeMP3 || nType == nFileTypeMP3V1 || nType == nFileTypeMP3V11) { // �W��MP3�`���ɕϊ�
		if (nFormat == nFileTypeRMP) { // RIFF�`�� => �W��MP3�`��
			if (DeleteTagSIF(GetFullPath(pFileMP3), hWnd) == false) {
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"),  GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeMP3);
			STEPInitDataSIF(pFileMP3);
			if (bOptAutoConvID3v2 || bOptAutoConvRMP) {
				SetGenreSI(pFileMP3, GetGenre(pFileMP3));
				SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
				SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
			}
		} else if (nFormat == nFileTypeID3V2) { // ID3v2�`�� => �W��MP3�`��
			if (DeleteTagID3v2(GetFullPath(pFileMP3), hWnd) == false) {
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"),  GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeMP3);
			STEPInitDataSIF(pFileMP3);
			if (bOptAutoConvID3v2 || bOptAutoConvRMP/*bOptID3v2ID3tagAutoWrite*/) {
				SetGenreSI(pFileMP3, GetGenre(pFileMP3));
				SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
				SetDiskNumberSI(pFileMP3, GetDiskNumber(pFileMP3));
			}
		} else {
			// ���Ή��̌`��
			//return true;
		}
	} else if (nType == nFileTypeID3V2) { // ID3v2�`���ɕϊ�
		if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) { // �W��MP3�`�� => ID3v2�`��
			if (MakeTagID3v2(GetFullPath(pFileMP3), hWnd) == false) {
				STEPInitDataSIF(pFileMP3);				// SIF �̃N���A
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeID3V2);
			extern bool ConvID3tagToSIField(FILE_INFO *pFileMP3);
			ConvID3tagToSIField(pFileMP3);	// ID3 tag ���� SIF �ɃR�s�[
			if (lstrlen(GetTrackNumberSI(pFileMP3)) == 0 && GetBTrackNumber(pFileMP3) != (BYTE)0xff) {
				CString strTrackNumber;
				strTrackNumber.Format(TEXT("%d"), GetBTrackNumber(pFileMP3));
				SetTrackNumberSI(pFileMP3, strTrackNumber);
			}
			if (lstrlen(GetDiskNumberSI(pFileMP3)) == 0 && GetBDiskNumber(pFileMP3) != (BYTE)0xff) {
				CString strDiskNumber;
				strDiskNumber.Format(TEXT("%d"), GetBDiskNumber(pFileMP3));
				SetDiskNumberSI(pFileMP3, strDiskNumber);
			}
		} else if (nFormat == nFileTypeRMP) { // RIFF�`�� => ID3v2�`��
			// RIFF MP3 => (�W��MP3�`��) => ID3v2 �ϊ�
			if (DeleteTagSIF(GetFullPath(pFileMP3), hWnd) == false) {
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"),  GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeMP3);
			if (MakeTagID3v2(GetFullPath(pFileMP3), hWnd) == false) {
				STEPInitDataSIF(pFileMP3);				// SIF �̃N���A
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeID3V2);
			if (GetBTrackNumber(pFileMP3) != (BYTE)0xff) {
				CString strTrackNumber;
				strTrackNumber.Format(TEXT("%d"), GetBTrackNumber(pFileMP3));
				SetTrackNumberSI(pFileMP3, strTrackNumber);
			}
			if (GetBDiskNumber(pFileMP3) != (BYTE)0xff) {
				CString strDiskNumber;
				strDiskNumber.Format(TEXT("%d"), GetBDiskNumber(pFileMP3));
				SetDiskNumberSI(pFileMP3, strDiskNumber);
			}
		} else {
			// ���Ή��̌`��
			//return true;
		}
	} else if (nType == nFileTypeRMP) { // RIFF�`���ɕϊ�
		if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) { // �W��MP3�`�� => RIFF�`��
			if (MakeTagSIF(GetFullPath(pFileMP3), hWnd) == FALSE) {
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeRMP);
			SetFileTypeName(pFileMP3, TEXT("RIFF MP3"));
			extern bool ConvID3tagToSIField(FILE_INFO *pFileMP3);
			ConvID3tagToSIField(pFileMP3);	// ID3 tag ���� SIF �ɃR�s�[
		} else if (nFormat == nFileTypeID3V2) { // ID3v2�`�� => RIFF�`��
			// ID3v2 => (�W��MP3�`��) => RIFF MP3 �ϊ�
			if (DeleteTagID3v2(GetFullPath(pFileMP3), hWnd) == false) {
				// �ϊ����s
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return(false);
			}
			SetFormat(pFileMP3, nFileTypeMP3);
			if (MakeTagSIF(GetFullPath(pFileMP3), hWnd) == FALSE) {
				MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				return false;
			}
			SetFormat(pFileMP3, nFileTypeRMP);
			SetFileTypeName(pFileMP3, TEXT("RIFF MP3"));
		} else {
			// ���Ή��̌`��
			//return true;
		}
	} else {
		// ���Ή��̌`��
		return false;
	}

	/*
	SetModifyFlag(pFileMP3, true); // �^�O���X�V������FileType��ݒ�
	if (STEPWriteTag(pFileMP3) == false) {
		return false;
	}
	SetModifyFlag(pFileMP3, false);
	*/
	if (bOptID3v2ID3tagAutoWrite || bOptRmpID3tagAutoWrite) {
		SetModifyFlag(pFileMP3, true); // �^�O���X�V�K�{
	}
	if (isModify(pFileMP3)) {
		if (STEPWriteTag(pFileMP3) == false) {
			return false;
		}
	}
	extern bool LoadFileMP3(FILE_INFO *pFileMP3);
	if (LoadFileMP3(pFileMP3) == false) {
		return false;
	}

	// �g���q�̕ύX
	return ConvExt(pFileMP3);
}


/**----- �R�[���o�b�N -----**/
bool WINAPI ConvFileFormatMP3(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	int nFormat = GetFormat(pFileInfo);
	if (!TYPE_IS_SUPPORT(nFormat)) { /* 2005.09.21 */
		return true;
	}
	if (ConvFileFormat(pFileInfo, nFileTypeMP3, hWnd) == false) {
		return false;
	} 
	return true;
}

bool WINAPI ConvFileFormatRMP(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	int nFormat = GetFormat(pFileInfo);
	if (!TYPE_IS_SUPPORT(nFormat)) { /* 2005.09.21 */
		return true;
	}
	if (ConvFileFormat(pFileInfo, nFileTypeRMP, hWnd) == false) {
		return false;
	} 
	return true;
}

bool WINAPI ConvFileFormatID3v2(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	if (ConvFileFormat(pFileInfo, nFileTypeID3V2, hWnd) == false) {
		return false;
	} 
	return true;
}

/*
bool WINAPI ConvSiFieldToTd3tag(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	extern void ConvSIFieldToID3tag(FILE_INFO *pFileMP3);
	ConvSIFieldToID3tag(pFileInfo);
	return true;
}
*/

bool WINAPI DeleteId3tag(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	int nFormat = GetFormat(pFileInfo);
	if (!TYPE_IS_SUPPORT(nFormat)) { /* 2005.09.21 */
		return true;
	}
	// ���O�Ƀ^�O���X�V
	if (isModify(pFileInfo)) {
		if (STEPWriteTag(pFileInfo) == false) {
			return false;
		}
	}
	bool result = DeleteTagID3v1(GetFullPath(pFileInfo), hWnd);
	STEPInitDataID3(pFileInfo);
	STEPInitDataSIF(pFileInfo);
	extern bool LoadFileMP3(FILE_INFO *pFileMP3);
	LoadFileMP3(pFileInfo);
//	CId3tagv2	id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
//	if (id3v2.Load(GetFullPath(pFileInfo))  != ERROR_SUCCESS	// �ǂݍ��ݎ��s
//	||  id3v2.IsEnable() == FALSE			// ID3v2 �ł͂Ȃ�
//	||  !TRUE/*id3v2.IsSafeVer()*/) {				// ���Ή��̃o�[�W����
//		// �ǂݍ��ݎ��s
//		// �^�O�Ȃ�
//		SetFileTypeName(pFileInfo, "MP3");
//		SetFormat(pFileInfo, nFileTypeMP3);	// �t�@�C���`���FMP3
//		return false;
//	}
//	extern void setFileType(CId3tagv2& id3v2, FILE_INFO *pFileMP3);
//	setFileType(id3v2, pFileInfo);
	return result;
}

bool WINAPI ConvFileFormatAuto(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	int nFormat = GetFormat(pFileInfo);
	if (!TYPE_IS_SUPPORT(nFormat)) { /* 2005.09.21 */
		return true;
	}
	// ���O�Ƀ^�O���X�V
	if (isModify(pFileInfo)) {
		if (STEPWriteTag(pFileInfo) == false) {
			return false;
		}
	}
	if (nFormat == nFileTypeRMP) {
		if (ConvFileFormat(pFileInfo, nFileTypeID3V2, hWnd) == false) {
			return false;
		} 
	}
	extern bool IsCreateID3v2(FILE_INFO *pFileMP3);
	extern bool IsCreateID3v2SI(FILE_INFO *pFileMP3, bool bID3v1Only);
	if (IsCreateID3v2(pFileInfo) == false && IsCreateID3v2SI(pFileInfo, TRUE) == false) {
		extern void ConvSIFieldToID3tag(FILE_INFO *pFileMP3);
		if (!(nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)) { // �W��MP3�`���ȊO�̏ꍇ
			ConvSIFieldToID3tag(pFileInfo);	// SIF ���� ID3 tag �ɃR�s�[
		}
		if (ConvFileFormat(pFileInfo, nFileTypeMP3, hWnd) == false) {
			return false;
		}
		//STEPInitDataSIF(pFileInfo);
	} else  if (GetFormat(pFileInfo) != nFileTypeID3V2) {
		if (ConvFileFormat(pFileInfo, nFileTypeID3V2, hWnd) == false) {
			return false;
		} 
	}
#ifdef RECONVERT_ID3V2
	else if(GetFormat(pFileInfo) == nFileTypeID3V2) {
		if (DeleteTagID3v2(GetFullPath(pFileInfo), hWnd) == false) {
			MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileInfo), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
		if (MakeTagID3v2(GetFullPath(pFileInfo), hWnd) == false) {
			MessageBox(hWnd, TEXT("�t�@�C���`���̕ϊ��Ɏ��s���܂���"), GetFullPath(pFileInfo), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
	}
#endif
	/*
	SetModifyFlag(pFileInfo, true);
	if (STEPWriteTag(pFileInfo) == false) {
		return false;
	}
	SetModifyFlag(pFileInfo, false);
	*/
	//extern bool LoadFileMP3(FILE_INFO *pFileMP3);
	//LoadFileMP3(pFileInfo);

	// �g���q�̕ύX
	return ConvExt(pFileInfo);
}

bool WINAPI ConvID3v2Version(FILE_INFO* pFileInfo, int nProcFlag, HWND hWnd) {
	if (nProcFlag == 3) {
		return false;
	}
	int nFormat = GetFormat(pFileInfo);
	if (!TYPE_IS_SUPPORT(nFormat)) { /* 2005.09.21 */
		return true;
	}
	// ���O�Ƀ^�O���X�V
	if (isModify(pFileInfo)) {
		if (STEPWriteTag(pFileInfo) == false) {
			return false;
		}
	}
	if (GetFormat(pFileInfo) != nFileTypeID3V2)	return true;
	CId3tagv2	id3v2;
	if (id3v2.Load(GetFullPath(pFileInfo))  != ERROR_SUCCESS	// �ǂݍ��ݎ��s
	||  id3v2.IsEnable() == FALSE			// ID3v2 �ł͂Ȃ�
	||  !TRUE/*id3v2.IsSafeVer()*/) {				// ���Ή��̃o�[�W����
		// �ǂݍ��ݎ��s
		return false;
	}
	extern int		nId3v2EncodeConv;
	extern int		nId3v2VersionConv;
	extern int		nId3v2Encode;
	extern int		nId3v2Version;
	bool needConv = false;
	WORD oldVersion = id3v2.GetVer();
	if (nId3v2VersionConv != 0) {
		// �ύX���Ȃ��ȊO
		switch (nId3v2VersionConv) {
		case 1:	// v2.2
			if (id3v2.GetVer() != 0x0200)	needConv = true;
			id3v2.SetVer(0x0200);
			break;
		case 2:	// v2.3
			if (id3v2.GetVer() != 0x0300)	needConv = true;
			id3v2.SetVer(0x0300);
			break;
		case 3:	// v2.4
			if (id3v2.GetVer() != 0x0400)	needConv = true;
			id3v2.SetVer(0x0400);
			break;
		default:
			break;
		}
	}
	if (nId3v2EncodeConv != 0) {
		switch (nId3v2EncodeConv) {
		case 1:
			if (id3v2.GetCharEncode() != CId3tagv2::ID3V2CHARENCODE_ISO_8859_1)	needConv = true;
			id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_ISO_8859_1);
			break;
		case 2:
			if (id3v2.GetCharEncode() != CId3tagv2::ID3V2CHARENCODE_UTF_16)	needConv = true;
			id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16);
			break;
		case 3:
			if (id3v2.GetCharEncode() != CId3tagv2::ID3V2CHARENCODE_UTF_8)	needConv = true;
			id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_8);
			break;
		//case 4:
		//	if (id3v2.GetCharEncode() != CId3tagv2::ID3V2CHARENCODE_UTF_16BE)	needConv = true;
		//	id3v2.SetCharEncode(CId3tagv2::ID3V2CHARENCODE_UTF_16BE);
		//	break;
		default:
			break;
		}
	}
	if (needConv == false)	return true;
	bool result = id3v2.Save(GetFullPath(pFileInfo)) == ERROR_SUCCESS ? true : false;
	if (result) {
		/*
		int nSaveId3v2Version = nId3v2Version;
		int nSaveId3v2Encode = nId3v2Encode;
		nId3v2Version = nId3v2VersionConv;
		nId3v2Encode = nId3v2EncodeConv;
		SetModifyFlag(pFileInfo, true); // �^�O���X�V������FileType��ݒ�
		result = STEPWriteTag(pFileInfo);
		//nId3v2VersionConv = nId3v2Version;
		//nId3v2EncodeConv = nId3v2Encode;
		nId3v2Version = nSaveId3v2Version;
		nId3v2Encode = nSaveId3v2Encode;
		if (result == false) {
			return false;
		}
		SetModifyFlag(pFileInfo, false);
		*/
		if (oldVersion == 0x0400 || id3v2.GetVer() == 0x0400) { /* 2005.09.22 */
			// ID3v2.4�̏ꍇ�AFrameID��ύX��v����(TDRC<=>TYER)�̂ōčX�V
			SetModifyFlag(pFileInfo, true);
			// �f�t�H���g��ύX���ččX�V��Ɍ��ɖ߂� /* 2006.04.18 */
			int		nId3v2EncodeOrg = nId3v2Encode;
			int		nId3v2VersionOrg = nId3v2Version;
			nId3v2Encode = nId3v2EncodeConv;
			nId3v2Version = nId3v2VersionConv;
			result = STEPWriteTag(pFileInfo);
			nId3v2Encode = nId3v2EncodeOrg;
			nId3v2Version = nId3v2VersionOrg;
			if (result == false) {
				return false;
			}
		}
		extern bool LoadFileMP3(FILE_INFO *pFileMP3);
		if (LoadFileMP3(pFileInfo) == false) {
			return false;
		}

		// �g���q�̕ύX
		return ConvExt(pFileInfo);
	}
	return result;
}