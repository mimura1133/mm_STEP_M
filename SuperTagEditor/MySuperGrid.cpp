// MySuperGrid.cpp : implementation file
//

#include "stdafx.h"
#include "regexp.h"
#include "SuperTagEditor.h"
#include "SuperTagEditorDoc.h"
#include "SuperTagEditorView.h"
#include "DlgCommonProg.h"
#include "SuperGridCtrl.h"
#include "MySuperGrid.h"
#include "ComboInListView.h"
#include "DlgFixedString.h"
#include "DoubleZeroString.h"
#include "DlgFileNameChange.h"
#include "DlgFileOverWrite.h"
#include "DlgFileOverWrite2.h"
#include "STEP_api.h"

#include "Registry.h"

#include <string_view>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* �����̂P�o�C�g�ڂ��ǂ����̌��� */
#ifndef iskanji
#define iskanji(c)		((c) >= 0x81 && (c) <= 0x9f || (c) >= 0xe0 && (c) <= 0xfc)
#endif

#define COLUMN_MIN			1

#define MINCOLWIDTH			24  /* or whatever */

const TCHAR	*g_sNameListDefault[] = {
	_T("���w��"),
	_T("Audio List"),
	_T("�t�@�C����"),
	_T("�g���b�N��"),
	_T("�A�[�e�B�X�g��"),
	_T("�A���o����"),
	_T("TrackNo"),
	_T("DiskNo"),
	_T("�N��"),
	_T("�W������"),
	_T("�R�����g"),
	_T("���쌠"),
	_T("�����"),
	_T("�\�[�X"),
	_T("�\�t�g�E�F�A"),
	_T("�L�[���[�h"),
	_T("�Z�p��"),
	_T("�̎�"),
	_T("�R�~�b�V����"),
	_T("�쎌��"),
	_T("��Ȏ�"), /* Baja 154 */
	_T("Albm.�A�[�e�B�X�g"), /* Baja 154 */
	_T("Orig.�A�[�e�B�X�g"), /* Baja 154 */
	_T("URL"), /* Baja 154 */
	_T("�G���R�[�h�����l"), /* Baja 154 */
	_T("�T�C�Y"),
	_T("�X�V��"),
	_T("�p�X��"),
	_T("���t����"),
	_T("�̎��t�@�C��"),
	_T("�t�@�C���̎��"),
	_T("�t�H�[�}�b�g"), 	//_T("BitRate"), /* Conspiracy 198 */
	_T("���̑�"), /* Conspiracy 196 */
	_T("�쐬��"), /* 2003.06.19 add */
	NULL,
};

struct COLUMN_STATUS	g_columnStatus[COLUMN_MAX] = {
	{"TreeItem"  , TRUE ,  0, 128,   0, LVCFMT_LEFT },	// �c���[�A�C�e����(�t���p�X��)
	{"FileName"  , TRUE ,  1, 150,   0, LVCFMT_LEFT },	// �t�@�C����
	{"TrackName" , TRUE ,  2, 128,   0, LVCFMT_LEFT },	// �Ȗ�
	{"ArtistName", TRUE ,  3, 128,   0, LVCFMT_LEFT },	// �A�[�e�B�X�g��
	{"AlbumName" , TRUE ,  4, 128,   0, LVCFMT_LEFT },	// �A���o�����^���i��
	{"TrackNo"   , TRUE ,  5,  48,  48, LVCFMT_RIGHT},	// �g���b�N�ԍ�(ID3 tag v1.1�̂�)
	{"DiskNo"    , FALSE,  6,  48,  48, LVCFMT_RIGHT},	// �f�B�X�N�ԍ�(ID3 tag v1.1�̂�)
	{"Year"      , TRUE ,  7,  48,  48, LVCFMT_LEFT },	// �����[�X�N��
	{"Genre"     , TRUE ,  8, 128,   0, LVCFMT_LEFT },	// �W������
	{"Comment"   , TRUE ,  9, 128,   0, LVCFMT_LEFT },	// �R�����g������
	{"Copyright" , FALSE, 10, 128,   0, LVCFMT_LEFT },	// ���쌠
	{"Engineer"  , FALSE, 11, 128,   0, LVCFMT_LEFT },	// �G���W�j�A(�����)
	{"Source"    , FALSE, 12, 128,   0, LVCFMT_LEFT },	// �\�[�X
	{"Software"  , FALSE, 13, 128,   0, LVCFMT_LEFT },	// �\�t�g�E�F�A
	{"Keyword"   , FALSE, 14, 128,   0, LVCFMT_LEFT },	// �L�[���[�h
	{"Technician", FALSE, 15, 128,   0, LVCFMT_LEFT },	// �Z�p��
	{"Lyric"     , FALSE, 16, 128,   0, LVCFMT_LEFT },	// �̎�
	{"Commission", FALSE, 17, 128,   0, LVCFMT_LEFT },	// �R�~�b�V����
	{"Writer"    , FALSE, 18, 128,   0, LVCFMT_LEFT },	// �쎌
	{"Composer"  , FALSE, 19, 128,   0, LVCFMT_LEFT },	// ��� /* Baja 154 */
	{"AlbmArtist", FALSE, 20, 128,   0, LVCFMT_LEFT },	// Albm.�A�[�e�B�X�g /* Baja 154 */
	{"OrigArtist", FALSE, 21, 128,   0, LVCFMT_LEFT },	// Orig.�A�[�e�B�X�g /* Baja 154 */
	{"URL"       , FALSE, 22, 128,   0, LVCFMT_LEFT },	// URL /* Baja 154 */
	{"Encodest"  , FALSE, 23, 128,   0, LVCFMT_LEFT },	// �G���R�[�h�����l/* Baja 154 */
	{"FileSize"  , TRUE , 24,  56,  56, LVCFMT_RIGHT},	// �t�@�C���T�C�Y
	{"FileTime"  , FALSE, 25, 128, 128, LVCFMT_LEFT },	// �t�@�C���X�V��
	{"FilePath"  , TRUE , 26, 128, 128, LVCFMT_LEFT },	// �t�@�C���p�X��
	{"PlayTime"  , FALSE, 27, 128, 128, LVCFMT_LEFT },	// ���t����
	{"LyricFile" , FALSE, 28, 128, 128, LVCFMT_LEFT },	// �̎��t�@�C��
	{"FileType"  , TRUE , 29, 128, 128, LVCFMT_LEFT },	// �t�@�C���̎��
// 	{"BitRate"   , FALSE, 30, 128, 0, LVCFMT_LEFT },	// �r�b�g���[�g /* Conspiracy 198 */
	{"Format"    , FALSE, 30, 128, 0, LVCFMT_LEFT },	// �t�H�[�}�b�g /* Conspiracy 198 */
	{"Other"     , FALSE, 31, 128, 128, LVCFMT_LEFT },	// ���̑� /* Conspiracy 196 */
	{"FileCTime" , FALSE, 32, 128, 128, LVCFMT_LEFT },	// �t�@�C���쐬�� /* 2003.06.19 add */
};

// �J�����^�C�v(�^�O����) => �J�����ԍ� �ւ̕ϊ��e�[�u��(InitializeGrid() �ō쐬�����)
int		g_nColumnNumberList[COLUMN_MAX];
// �J�����ԍ� => �J�����^�C�v(�^�O����) �ւ̕ϊ��e�[�u��InitializeGrid() �ō쐬�����
int		g_nColumnTypeList[COLUMN_MAX];

/*
�c���[�A�C�e����, �t�@�C����, �Ȗ�, �A�[�e�B�X�g��, �A���o����, �g���b�N�ԍ�, �����[�X�N��, �W������, �R�����g
���쌠, �G���W�j�A(�����), �\�[�X, �\�t�g�E�F�A, �L�[���[�h, �Z�p��, �̎�, �R�~�b�V����
���, Orig.�A�[�e�B�X�g, URL, �G���R�[�h�����l, �t�@�C���T�C�Y, �t�@�C���X�V��, �t�@�C���p�X��, ���t����
�̎��t�@�C��, �t�@�C���̎��, �t�H�[�}�b�g, ���̑�
*/
static	CONTROLTYPE	**g_nControlTypeTableSIF;
static	CONTROLTYPE	**g_nControlTypeTable = NULL;

// ���Ɓ��̓v���O�C������C�ӂ̃^�C�~���O�ōX�V�ł���悤�ɂ���

// �e���ڂ̍ő啶����
static int **g_nColMaxSIF;
static int **g_nColMax;
const TCHAR ***g_sNameList;
static UINT nCurrentFormatOfHeader = FILE_TYPE_UNKNOWN;
static bool bCurrentFormatOfHeaderIsDefault = true;

#include "strcnv.h"

UINT STEClipbordFormat; /* Misirlou 146 */

/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid
namespace
{
	using tstring_view = std::basic_string_view<TCHAR>;

	template<class T>
	auto string_view_cast(const T& v) -> decltype(tstring_view(v))
	{
		return v;
	}

	tstring_view string_view_cast(const CString& v)
	{
		return tstring_view(v, v.GetLength());
	}

	static LPTSTR GetToken(LPTSTR buffer, LPTSTR sToken)
	{
		LPTSTR pBuffer = NULL;

		if (buffer != NULL) pBuffer = buffer;
		if (pBuffer == NULL) return(NULL);
		if (*pBuffer == '\0') return(NULL);

		// �����������
		auto pNow = pBuffer;
		pBuffer = _tcsstr(pBuffer, sToken);
		if (pBuffer != NULL) {
			*pBuffer = '\0';
			pBuffer++;
		}
		return(pNow);
	}

	static char *GetTokenQuote(char *buffer, char *sToken)
	{
		static char *pBuffer = NULL;

		if (buffer != NULL) pBuffer = buffer;
		if (pBuffer == NULL) return(NULL);
		if (*pBuffer == '\0') return(NULL);

		// �����������
		char *pNow = pBuffer;
		if (*pNow == '"') {
			int nPos = 1;
			while (1) {
				// ���̍s�������Z���̃f�[�^�Ȃ̂ŁA�I���� " ��T��
				while (pBuffer[nPos] != '\t' && pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
					nPos++;
				}
				if (pBuffer[nPos - 1] == '"') {
					break;
				}
				nPos++;
			}
			nPos--;
			pBuffer += nPos;
			*pBuffer = '\0';
			pBuffer++;
		} else {
			pBuffer = strstr(pBuffer, sToken);
			if (pBuffer != NULL) {
				*pBuffer = '\0';
				pBuffer++;
			}
		}
		return(pNow);
	}

	template<class T1, class T2, class T3>
	auto StrReplace(T1& strOrg, T2&& sKey, T3&& sRep) -> decltype(StrReplace(strOrg, tstring_view{}, tstring_view{}))
	{
		return StrReplace(strOrg, string_view_cast(sKey), string_view_cast(sRep));
	}
	CString StrReplace(CString &strOrg, tstring_view sKey, tstring_view sRep)
	{
		std::size_t nPos;
		tstring strWork = strOrg;
		while ((nPos = strWork.find(sKey)) != std::string::npos) {
			strWork.replace(nPos, sKey.length(), sRep);
		}
		return strWork.c_str();
	}
	tstring& StrReplace(tstring& strOrg, tstring_view sKey, tstring_view sRep)
	{
		std::size_t nPos;
		auto& strWork = strOrg;
		while ((nPos = strWork.find(sKey)) != std::string::npos) {
			strWork.replace(nPos, sKey.length(), sRep);
		}
		return strWork;
	}
	static CString StrReplaceEx(CString &strOrg, LPCTSTR sKey, LPCTSTR sRep, bool bIsHtml)
	{
		if (bIsHtml && lstrlen(sRep) == 0) sRep = TEXT("�@");
		/* BeachMonster 119 *///if (bIsHtml)  return(StrReplace(StrReplace(strOrg, sKey, sRep), "\r\n", "<BR>")); /* BeachMonster 089 */
		return(StrReplace(strOrg, sKey, sRep));
	}

	static	CString	ConvFileName(CString strFullPath, CString strBaseDir)
	{
		CString	strBuffer;
		if (_tcsncicmp(strFullPath, strBaseDir, strBaseDir.GetLength()) == 0) {
			// ���΃p�X�ɕϊ��\(�p�X����v)
			strBuffer.Format(TEXT("%s"), strFullPath.Right(strFullPath.GetLength()-strBaseDir.GetLength()));
		} else if (strFullPath[1] == ':' && _tcsncicmp(strFullPath, strBaseDir, 1) == 0) {
			// ���΃p�X�ɕϊ��\(�h���C�u�������ꏏ)
			strBuffer.Format(TEXT("%s"), strFullPath.Right(strFullPath.GetLength()-2));
		} else {
			// ���΃p�X�ɕϊ��s�\
			strBuffer.Format(TEXT("%s"), strFullPath);
		}
		return(strBuffer);
	}


	// =============================================
	//  CharComp
	//  �T�v  : �P������r�֐�
	//  ����  : sSrc		= ��r��������
	//        : sDest		= ��r�敶����
	//        : bDiffFlag	= �啶���^��������ʃt���O
	//  �߂�l: BOOL		= TRUE:����I�� / FALSE:�G���[
	// =============================================
	static	BOOL CharComp(LPCTSTR sSrc, LPCTSTR sDest, BOOL bDiffFlag)
	{
		BOOL	bIsKanjiSrc = iskanji(*sSrc) ? TRUE : FALSE;
		BOOL	bIsKanjiDest = iskanji(*sDest) ? TRUE : FALSE;
		if (bIsKanjiSrc == bIsKanjiDest) {
			if (bIsKanjiSrc) {			// ����
				if (sSrc[0] == sDest[0]
				&&  sSrc[1] == sDest[1]) {
					return(TRUE);
				}
			} else {					// ASCII
				if (bDiffFlag) {
					// �啶���^�����������
					if (*sSrc == *sDest) return(TRUE);
				} else {
					// �啶���^�������𓯈ꎋ
					if (toupper(*sSrc) == toupper(*sDest)) return(TRUE);
				}
			}
		}
		return(FALSE);
	}

	// =============================================
	//  StringComp
	//  �T�v  : �������r�֐�(�Q�o�C�g�����Ή�)
	//  ����  : sSrc		= ��r��������
	//        : sDest		= ��r�敶����
	//        : nDestLen	= ����
	//        : bDiffFlag	= �啶���^��������ʃt���O
	//  �߂�l: BOOL		= TRUE:����I�� / FALSE:�G���[
	// =============================================
	static	BOOL StringComp(LPCTSTR sSrc, LPCTSTR sDest, int nDestLen, BOOL bDiffFlag)
	{
		int		nSrcLen = lstrlen(sSrc);
		if (nSrcLen >= nDestLen) {
			while (*sDest != '\0') {
				if (CharComp(sSrc, sDest, bDiffFlag) == FALSE) break;
				if (iskanji(*sDest)) {
					sSrc += 2;
					sDest += 2;
				} else {
					sSrc++;
					sDest++;
				}
			}
			if (*sDest == '\0') return(TRUE);
		}
		return(FALSE);
	}

	// =============================================
	//  ConvNumber
	//  �T�v  : �������R���̃J���}��؂�ɕϊ�����
	//  ����  : strNum		= ����(������)
	//  �߂�l: 			= �ϊ���̕�����
	// =============================================
	CString	ConvNumber(CString strNum)
	{
		int		nCount = 1;
		while (strNum.GetLength() >= nCount * 4) {
			strNum = strNum.Left(strNum.GetLength() - (nCount * 4 - 1))
				   + CString(',')
				   + strNum.Right(nCount * 4 - 1);
			nCount++;
		}
		return(strNum);
	}

	// =============================================
	//  GetFileType
	//  �T�v  : �t�@�C���^�C�v��������擾����
	//  ����  : fileMP3		= �t�@�C�����
	//  �߂�l: CString		= �t�@�C���^�C�v������
	// =============================================
	static CString GetFileType(const FILE_MP3 *fileMP3)
	{
		if ( fileMP3->strFileTypeName.IsEmpty()) {
			return("Unknown");
		}
		return fileMP3->strFileTypeName;
	}

	/// <summary>
	/// SIF �̍���
	/// </summary>
	CString StrReplaceSIF(const CString& strOrg, const FILE_MP3* fileMP3)
	{
		tstring strText = strOrg;
		StrReplace(strText, TEXT("%COPYRIGHT%"), fileMP3->strCopyrightSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%ENGINEER%"), fileMP3->strEngineerSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%SOURCE%"), fileMP3->strSourceSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%SOFTWARE%"), fileMP3->strSoftwareSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%KEYWORD%"), fileMP3->strKeywordSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%TECHNICIAN%"), fileMP3->strTechnicianSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%LYRIC%"), fileMP3->strLyricSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%COMMISSION%"), fileMP3->strCommissionSI.SpanExcluding(TEXT("\r")));
		StrReplace(strText, TEXT("%WRITER%"), fileMP3->strWriterSI.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%COMPOSER%"), fileMP3->strComposerSI.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%ALBM_ARTIST%"), fileMP3->strAlbmArtistSI.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%ORIG_ARTIST%"), fileMP3->strOrigArtistSI.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%URL%"), fileMP3->strURLSI.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%ENCODEST%"), fileMP3->strEncodest.SpanExcluding(TEXT("\r"))); /* Baja 154 */
		StrReplace(strText, TEXT("%OTHER%"), fileMP3->strOther.SpanExcluding(TEXT("\r"))); /* Conspiracy 196 */
		return strText.c_str();
	}

	template<class Func>
	std::tuple<CString, CString, CString> GetIntNumber(const CString& no, Func func)
	{
		CString	number1, number2, number3;
		if (const auto nTrackNumber = _tstoi(func(no))) {
			number1.Format(TEXT("%d"), nTrackNumber);
			number2.Format(TEXT("%02d"), nTrackNumber);
			number3.Format(TEXT("%03d"), nTrackNumber);
		}

		return {
			number1,
			number2,
			number3,
		};
	}

	std::tuple<CString, CString, CString> GetIntTrackNo(const CString& no)
	{
		return GetIntNumber(no, CFileMP3::GetIntTrackNo);
	}

	std::tuple<CString, CString, CString> GetIntDiskNo(const CString& no)
	{
		return GetIntNumber(no, CFileMP3::GetIntDiskNo);
	}
}

// =============================================
//  CMySuperGrid::CMySuperGrid
//  �T�v  : �R���X�g���N�^
//  ����  : �Ȃ�
//  �߂�l: �Ȃ�
// =============================================
CMySuperGrid::CMySuperGrid()
{
	m_bDrag = TRUE;
	m_pDoc = NULL;

	// �W�����������X�g�̍쐬
	MakeStrListGenre();

	m_bSortAscending = TRUE;
	m_nSortedCol = -1;

	m_rootItemFont = NULL;

	STEClipbordFormat = RegisterClipboardFormat(PROG_NAME/* "��"*/); /* Misirlou 146 */
}

// =============================================
//  CMySuperGrid::~CMySuperGrid
//  �T�v  : �f�X�g���N�^
//  ����  : �Ȃ�
//  �߂�l: �Ȃ�
// =============================================
CMySuperGrid::~CMySuperGrid()
{
	if (m_rootItemFont != NULL) {
		m_rootItemFont->Detach();
		delete m_rootItemFont;
		m_rootItemFont = NULL;
	}
	{
		extern UINT GetFileTypeCount();
		int nFileTypeCount = GetFileTypeCount() + 1;
		for (int i=0;i<nFileTypeCount;i++) {
			delete g_nControlTypeTable[i];
			delete g_nControlTypeTableSIF[i];
			delete g_nColMax[i];
			delete g_nColMaxSIF[i];
			if (i > 0 && g_sNameList[i] != NULL) {
				delete g_sNameList[i];
			}
		}
		delete [] g_nControlTypeTable;
		delete [] g_nControlTypeTableSIF;
		delete [] g_nColMax;
		delete [] g_nColMaxSIF;
		delete [] g_sNameList;
	}
}

#define CListCtrl		CSuperGridCtrl
BEGIN_MESSAGE_MAP(CMySuperGrid, CListCtrl)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMySuperGrid)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT( )
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()
#undef CListCtrl

/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid message handlers
CImageList g_imageCheckMark;//Must provide an imagelist

int CMySuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this
	///////////////////

	//associate imagelist with listviewctrl,
	//this imagelist is only for the first column re: the tree column
	if(!m_image.Create(IDB_BITMAP_TREE,16,1,RGB(255, 0, 255)))
		return -1;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;
	extern CImageList* STEP_ImageList;
	STEP_ImageList = &m_image;


	// �`�F�b�N�}�[�N�p�C���[�W���X�g
	// if(!m_imageCheck.Create(IDB_BITMAP_CHECK,16,1,RGB(255, 0, 255)))
	// 	return -1;
	// SetImageList(&m_imageCheck, LVSIL_STATE);
	g_imageCheckMark.Create(IDB_BITMAP_CHECK,16,1,RGB(255, 0, 255));

	//If you want to assiciate images to subitems, create an imagelist
	//and call SetSubItemImageList
	//now the next time you want to map an image from this imagelist to a
	//particular subitem call SetSubItemText(col,text,image-index);
//	CImageList imageSubItem;
//	if(!imageSubItem.Create(IDB_SUBITEMS,16,1,RGB(0, 255, 255)))
//		return -1;
//
//	if(!SetSubItemImageList(&imageSubItem))
//		return -1;

	return 0;
}



//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CMySuperGrid::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class,
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


BOOL CMySuperGrid::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	return 1;
}

//override called when OnLButtondown
bool CMySuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem != NULL) {
		CItemInfo* pInfo = GetData(pSelItem);
		CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType)) {
			if(ctrlType == combobox) {
				// [��]�̕������N���b�N���ꂽ�ꍇ�́A���X�g���J��
				CRect rect;
				GetSubItemRect(ht.iItem, ht.iSubItem, LVIR_BOUNDS, rect);
				rect.left = rect.right - GetSystemMetrics(SM_CYVSCROLL);
				if (point.x >= rect.left && point.x <= rect.right) {
					CStringList *list = NULL;
					pInfo->GetListData(ht.iSubItem-1, list);
					CComboBox *pList = ShowList(ht.iItem, ht.iSubItem, list);
					return(true);
				}
			}
		}
	}
	return(false);
}


#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CMySuperGrid::ShowList(int nItem, int nCol, CStringList *lstItems)
{
	CString strFind = GetItemText(nItem, nCol);

	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right)
	   rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();	//dropdown area

	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | /*WS_HSCROLL |*/ CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL | CBS_SORT;
	CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
	pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}



int CMySuperGrid::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent=0;
	if(pWnd!=NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if(pFont!=NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while(pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}


BOOL CMySuperGrid::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CMySuperGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CMySuperGrid::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}



BOOL CMySuperGrid::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


CImageList *CMySuperGrid::CreateDragImageEx(int nItem)
{
		if(m_bDrag)
			return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
		else
			return NULL;
}


void CMySuperGrid::SetNewImage(int nItem)
{
	CTreeItem*pItem = GetTreeItem(nItem);
	CItemInfo *lp = GetData(pItem);
	if(lp!=NULL)
	{
//		lp->SetSubItemImage(3/*subitem*/,4/*image index*/);
//		lp->SetSubItemImage(2/*subitem*/,4/*image index*/);
		//update internal node
		UpdateData(pItem, lp, TRUE);
		InvalidateItemRect(nItem);
	}
}

int CMySuperGrid::GetColumnCount(void) const
{
	// get the header control
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	// return the number of items in it - ie. the number of columns
	return pHeader->GetItemCount();
}

#define MAX(n1, n2)		(((n1) < (n2)) ? (n2) : (n1))
void CMySuperGrid::AutoSizeColumns(int col /*=-1*/)
{
	// Call this after your list control is filled
	SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? GetColumnCount()-1 : col;
	for (col = mincol; col <= maxcol; col++) {
		if ((g_nColumnNumberList[COLUMN_TRACK_NUMBER] == col)|
			(g_nColumnNumberList[COLUMN_DISK_NUMBER] == col)) {
			SetColumnWidth(col, LVSCW_AUTOSIZE);
			int		wc = GetColumnWidth(col);
			int		wmax = g_columnStatus[g_nColumnTypeList[col]].nWidthMax;
			if (wmax > 0 && wc > wmax) wc = wmax;
			if (wc < MINCOLWIDTH) SetColumnWidth(col, MINCOLWIDTH);
/* 2003.06.19 start */
		} else if (g_nColumnNumberList[COLUMN_TREE_ITEM] == col && g_bAudioListShow && g_bOptHideMP3ListFile) {
			// AudioList �Ƀt�@�C������\�����Ȃ������ނ��͂ݏo�ĕ\������ꍇ�A
			// ���ނ��l�����Ȃ�(���A�C�R���E�`�F�b�N�{�b�N�X�̃T�C�Y�j�ŃZ���T�C�Y�����肷��
			int indentmax = 0;
			int nItems = GetItemCount();
			for(int nItem=0; nItem < nItems; nItem++) {
				CTreeItem* pItem = GetTreeItem(nItem);
				int indent = GetIndent(pItem);
				if (indentmax < indent)	indentmax = indent;
				//if (!IsItemFile(pItem)) {
				//	InvalidateItemRectPtr(pItem); // ���ނ��ĕ`��
				//}
			}
			//extern	CImageList	g_imageCheckMark;
			//CImageList *pImageList = &g_imageCheckMark;
			int		cxIcon,cyIcon=0;
			ImageList_GetIconSize(g_imageCheckMark.m_hImageList, &cxIcon, &cyIcon);
			SetColumnWidth(col, indentmax * m_cxImage + m_cxImage + 2 + cxIcon);
			int		wc = GetColumnWidth(col);
			int		wmax = g_columnStatus[g_nColumnTypeList[col]].nWidthMax;
			if (wmax > 0 && wc > wmax) SetColumnWidth(col, wmax);
/* 2003.06.19 end */
		} else {
			SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
			int		wc = GetColumnWidth(col);
			int		wmax = g_columnStatus[g_nColumnTypeList[col]].nWidthMax;
			if (wmax > 0 && wc > wmax) SetColumnWidth(col, wmax);
		}
	 }
	 // RecalcHeaderTips(); *** uncomment this if you use my header tips method
	 SetRedraw(true);
	 // Invalidate(); *** uncomment this if you don't use my SetRedraw function
}


void CMySuperGrid::DeleteAllEx(void)
{
	DeleteAll();

	m_bMultiSelect = false;

	// RootItem
	m_pItemRoot = AddDirectory(TEXT("Root"), NULL, 2);

	// RootItem�t�H���_�̖��̂��X�V����
	UpdateSyncFolderItemName();
	if (m_toolTip.GetSafeHwnd() == NULL) {
		m_toolTip.Create(GetParent());
		m_toolTip.AddTool(this, LPSTR_TEXTCALLBACK);
	}
}

void CMySuperGrid::UpdateCellInfo()
{
	int		i, j;
	extern UINT GetFileTypeCount();
	static int nFileTypeCount = 0;

	if (g_nControlTypeTable != NULL) {
		for (int i=0;i<nFileTypeCount;i++) {
			delete g_nControlTypeTable[i];
			delete g_nControlTypeTableSIF[i];
			delete g_nColMax[i];
			delete g_nColMaxSIF[i];
			if (i > 0 && g_sNameList[i] != NULL) {
				delete g_sNameList[i];
			}
		}
		delete [] g_nControlTypeTable;
		delete [] g_nControlTypeTableSIF;
		delete [] g_nColMax;
		delete [] g_nColMaxSIF;
		delete [] g_sNameList;
	}
	nFileTypeCount = GetFileTypeCount() + 1;

	g_nControlTypeTable = new CONTROLTYPE*[nFileTypeCount];
	g_nControlTypeTableSIF = new CONTROLTYPE*[nFileTypeCount];
	g_nColMax = new int*[nFileTypeCount];
	g_nColMaxSIF = new int*[nFileTypeCount];
	g_sNameList = new const TCHAR**[nFileTypeCount];

	for (i=0;i<nFileTypeCount*2;i++) {
		CONTROLTYPE* controlTable = new CONTROLTYPE[COLUMN_MAX];
		int* colMax = new int[COLUMN_MAX];
		const TCHAR** nameList = new const TCHAR*[COLUMN_MAX];
		for (j=0;j<COLUMN_MAX;j++) {
			controlTable[j] = _NULL;
		}
		for (j=0;j<COLUMN_MAX;j++) {
			colMax[j] = 0;
		}
		for (j=0;j<COLUMN_MAX;j++) {
			nameList[j] = NULL;
		}
		controlTable[COLUMN_FILE_NAME] = _EDIT; // �t�@�C�����͕ҏW��
		colMax[COLUMN_FILE_NAME] = FILENAME_MAX;
		if (i == 0 || i == 1) {
			delete [] nameList;
			nameList = g_sNameListDefault;
		} else {
			extern UINT GetFormatType(UINT nIndex);
			extern CONTROLTYPE GetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF);
			extern int GetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF);
			extern LPCTSTR GetColumnName(UINT nFormat, COLUMNTYPE nColumn);
			UINT nFormat = GetFormatType(i/2-1);
			bool isEditSIF = i%2 == 0 ? true : false;
//			controlTable[COLUMN_FILE_NAME] = GetControlType(nFormat, COLUMN_FILE_NAME, isEditSIF);
			controlTable[COLUMN_TRACK_NAME] = GetControlType(nFormat, COLUMN_TRACK_NAME, isEditSIF);
			controlTable[COLUMN_ARTIST_NAME] = GetControlType(nFormat, COLUMN_ARTIST_NAME, isEditSIF);
			controlTable[COLUMN_ALBUM_NAME] = GetControlType(nFormat, COLUMN_ALBUM_NAME, isEditSIF);
			controlTable[COLUMN_TRACK_NUMBER] = GetControlType(nFormat, COLUMN_TRACK_NUMBER, isEditSIF);
			controlTable[COLUMN_DISK_NUMBER] = GetControlType(nFormat, COLUMN_DISK_NUMBER, isEditSIF);
			controlTable[COLUMN_YEAR]  = GetControlType(nFormat, COLUMN_YEAR, isEditSIF);
			controlTable[COLUMN_GENRE] = GetControlType(nFormat, COLUMN_GENRE, isEditSIF);
			controlTable[COLUMN_COMMENT] = GetControlType(nFormat, COLUMN_COMMENT, isEditSIF);
			controlTable[COLUMN_COPYRIGHT] = GetControlType(nFormat, COLUMN_COPYRIGHT, isEditSIF);
			controlTable[COLUMN_ENGINEER] = GetControlType(nFormat, COLUMN_ENGINEER, isEditSIF);
			controlTable[COLUMN_SOURCE] = GetControlType(nFormat, COLUMN_SOURCE, isEditSIF);
			controlTable[COLUMN_SOFTWARE] = GetControlType(nFormat, COLUMN_SOFTWARE, isEditSIF);
			controlTable[COLUMN_KEYWORD] = GetControlType(nFormat, COLUMN_KEYWORD, isEditSIF);
			controlTable[COLUMN_TECHNICIAN] = GetControlType(nFormat, COLUMN_TECHNICIAN, isEditSIF);
			controlTable[COLUMN_LYRIC] = GetControlType(nFormat, COLUMN_LYRIC, isEditSIF);
			controlTable[COLUMN_COMMISSION] = GetControlType(nFormat, COLUMN_COMMISSION, isEditSIF);
			controlTable[COLUMN_WRITER] = GetControlType(nFormat,COLUMN_WRITER,isEditSIF);
			controlTable[COLUMN_COMPOSER] = GetControlType(nFormat, COLUMN_COMPOSER, isEditSIF);
			controlTable[COLUMN_ALBM_ARTIST] = GetControlType(nFormat, COLUMN_ALBM_ARTIST, isEditSIF);
			controlTable[COLUMN_ORIG_ARTIST] = GetControlType(nFormat, COLUMN_ORIG_ARTIST, isEditSIF);
			controlTable[COLUMN_URL] = GetControlType(nFormat, COLUMN_URL, isEditSIF);
			controlTable[COLUMN_ENCODEST] = GetControlType(nFormat, COLUMN_ENCODEST, isEditSIF);
			controlTable[COLUMN_OTHER] = GetControlType(nFormat, COLUMN_OTHER, isEditSIF);
			/*
			controlTable[COLUMN_FILE_SIZE] = GetControlType(nFormat, COLUMN_FILE_SIZE, isEditSIF);
			controlTable[COLUMN_FILE_TIME] = GetControlType(nFormat, COLUMN_FILE_TIME, isEditSIF);
			controlTable[COLUMN_FILE_PATH] = GetControlType(nFormat, COLUMN_FILE_PATH, isEditSIF);
			controlTable[COLUMN_PLAY_TIME] = GetControlType(nFormat, COLUMN_PLAY_TIME, isEditSIF);
			controlTable[COLUMN_LYRIC_FILE] = GetControlType(nFormat, COLUMN_LYRIC_FILE, isEditSIF);
			controlTable[COLUMN_FILE_TYPE] = GetControlType(nFormat, COLUMN_FILE_TYPE, isEditSIF);
			controlTable[COLUMN_FORMAT] = GetControlType(nFormat, COLUMN_FORMAT, isEditSIF);
			*/

//			colMax[COLUMN_FILE_NAME] = GetColumnMax(nFormat, COLUMN_FILE_NAME, isEditSIF);
			colMax[COLUMN_TRACK_NAME] = GetColumnMax(nFormat, COLUMN_TRACK_NAME, isEditSIF);
			colMax[COLUMN_ARTIST_NAME] = GetColumnMax(nFormat, COLUMN_ARTIST_NAME, isEditSIF);
			colMax[COLUMN_ALBUM_NAME] = GetColumnMax(nFormat, COLUMN_ALBUM_NAME, isEditSIF);
			colMax[COLUMN_TRACK_NUMBER] = GetColumnMax(nFormat, COLUMN_TRACK_NUMBER, isEditSIF);
			colMax[COLUMN_DISK_NUMBER] = GetColumnMax(nFormat, COLUMN_DISK_NUMBER, isEditSIF);
			colMax[COLUMN_YEAR]  = GetColumnMax(nFormat, COLUMN_YEAR, isEditSIF);
			colMax[COLUMN_GENRE] = GetColumnMax(nFormat, COLUMN_GENRE, isEditSIF);
			colMax[COLUMN_COMMENT] = GetColumnMax(nFormat, COLUMN_COMMENT, isEditSIF);
			colMax[COLUMN_COPYRIGHT] = GetColumnMax(nFormat, COLUMN_COPYRIGHT, isEditSIF);
			colMax[COLUMN_ENGINEER] = GetColumnMax(nFormat, COLUMN_ENGINEER, isEditSIF);
			colMax[COLUMN_SOURCE] = GetColumnMax(nFormat, COLUMN_SOURCE, isEditSIF);
			colMax[COLUMN_SOFTWARE] = GetColumnMax(nFormat, COLUMN_SOFTWARE, isEditSIF);
			colMax[COLUMN_KEYWORD] = GetColumnMax(nFormat, COLUMN_KEYWORD, isEditSIF);
			colMax[COLUMN_TECHNICIAN] = GetColumnMax(nFormat, COLUMN_TECHNICIAN, isEditSIF);
			colMax[COLUMN_LYRIC] = GetColumnMax(nFormat, COLUMN_LYRIC, isEditSIF);
			colMax[COLUMN_COMMISSION] = GetColumnMax(nFormat, COLUMN_COMMISSION, isEditSIF);
			colMax[COLUMN_WRITER] = GetColumnMax(nFormat,COLUMN_WRITER,isEditSIF);
			colMax[COLUMN_COMPOSER] = GetColumnMax(nFormat, COLUMN_COMPOSER, isEditSIF);
			colMax[COLUMN_ALBM_ARTIST] = GetColumnMax(nFormat, COLUMN_ALBM_ARTIST, isEditSIF);
			colMax[COLUMN_ORIG_ARTIST] = GetColumnMax(nFormat, COLUMN_ORIG_ARTIST, isEditSIF);
			colMax[COLUMN_URL] = GetColumnMax(nFormat, COLUMN_URL, isEditSIF);
			colMax[COLUMN_ENCODEST] = GetColumnMax(nFormat, COLUMN_ENCODEST, isEditSIF);
			colMax[COLUMN_OTHER] = GetColumnMax(nFormat, COLUMN_OTHER, isEditSIF);
			/*
			colMax[COLUMN_FILE_SIZE] = GetColumnMax(nFormat, COLUMN_FILE_SIZE, isEditSIF);
			colMax[COLUMN_FILE_TIME] = GetColumnMax(nFormat, COLUMN_FILE_TIME, isEditSIF);
			colMax[COLUMN_FILE_PATH] = GetColumnMax(nFormat, COLUMN_FILE_PATH, isEditSIF);
			colMax[COLUMN_PLAY_TIME] = GetColumnMax(nFormat, COLUMN_PLAY_TIME, isEditSIF);
			colMax[COLUMN_LYRIC_FILE] = GetColumnMax(nFormat, COLUMN_LYRIC_FILE, isEditSIF);
			colMax[COLUMN_FILE_TYPE] = GetColumnMax(nFormat, COLUMN_FILE_TYPE, isEditSIF);
			colMax[COLUMN_FORMAT] = GetColumnMax(nFormat, COLUMN_FORMAT, isEditSIF);
			*/
			if (i%2 == 0) {
				extern bool HasOriginalColumnName(UINT nFormat);
				if (HasOriginalColumnName(nFormat) == true) {
					nameList[COLUMN_FILE_NAME] = GetColumnName(nFormat, COLUMN_FILE_NAME);
					nameList[COLUMN_TRACK_NAME] = GetColumnName(nFormat, COLUMN_TRACK_NAME);
					nameList[COLUMN_ARTIST_NAME] = GetColumnName(nFormat, COLUMN_ARTIST_NAME);
					nameList[COLUMN_ALBUM_NAME] = GetColumnName(nFormat, COLUMN_ALBUM_NAME);
					nameList[COLUMN_TRACK_NUMBER] = GetColumnName(nFormat, COLUMN_TRACK_NUMBER);
					nameList[COLUMN_DISK_NUMBER] = GetColumnName(nFormat, COLUMN_DISK_NUMBER);
					nameList[COLUMN_YEAR]  = GetColumnName(nFormat, COLUMN_YEAR);
					nameList[COLUMN_GENRE] = GetColumnName(nFormat, COLUMN_GENRE);
					nameList[COLUMN_COMMENT] = GetColumnName(nFormat, COLUMN_COMMENT);
					nameList[COLUMN_COPYRIGHT] = GetColumnName(nFormat, COLUMN_COPYRIGHT);
					nameList[COLUMN_ENGINEER] = GetColumnName(nFormat, COLUMN_ENGINEER);
					nameList[COLUMN_SOURCE] = GetColumnName(nFormat, COLUMN_SOURCE);
					nameList[COLUMN_SOFTWARE] = GetColumnName(nFormat, COLUMN_SOFTWARE);
					nameList[COLUMN_KEYWORD] = GetColumnName(nFormat, COLUMN_KEYWORD);
					nameList[COLUMN_TECHNICIAN] = GetColumnName(nFormat, COLUMN_TECHNICIAN);
					nameList[COLUMN_LYRIC] = GetColumnName(nFormat, COLUMN_LYRIC);
					nameList[COLUMN_COMMISSION] = GetColumnName(nFormat, COLUMN_COMMISSION);
					nameList[COLUMN_WRITER] = GetColumnName(nFormat,COLUMN_WRITER);
					nameList[COLUMN_COMPOSER] = GetColumnName(nFormat, COLUMN_COMPOSER);
					nameList[COLUMN_ALBM_ARTIST] = GetColumnName(nFormat, COLUMN_ALBM_ARTIST);
					nameList[COLUMN_ORIG_ARTIST] = GetColumnName(nFormat, COLUMN_ORIG_ARTIST);
					nameList[COLUMN_URL] = GetColumnName(nFormat, COLUMN_URL);
					nameList[COLUMN_ENCODEST] = GetColumnName(nFormat, COLUMN_ENCODEST);
					nameList[COLUMN_OTHER] = GetColumnName(nFormat, COLUMN_OTHER);
				} else {
					delete [] nameList;
					nameList = NULL;
				}
			} else {
				delete [] nameList;
				nameList = NULL;
			}
		}
		if (i%2 == 0) {
			g_nControlTypeTableSIF[i/2] = controlTable;
			g_nColMaxSIF[i/2] = colMax;
			g_sNameList[i/2] = nameList;
		} else {
			g_nControlTypeTable[i/2] = controlTable;
			g_nColMax[i/2] = colMax;
			//g_sNameList[i/2] = nameList;
		}
	}
}
// =============================================
// CMySuperGrid::InitializeGrid
// �T�v  : �O���b�h�̏���������
// ����  : none
// �߂�l: none
// =============================================
void CMySuperGrid::InitializeGrid()
{
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	// �Z���������W�X�g������ǂݏo��
	CellStateControl(0);

	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	UpdateCellInfo();
	int		i, nColumn;


	// �e�[�u���� -1 �ŃN���A
	for(nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
		g_nColumnNumberList[nColumn] = -1;
		g_nColumnTypeList[nColumn] = -1;
	}

	// �J�����ԍ� => �J�����^�C�v(�^�O����) �ւ̕ϊ��e�[�u���쐬
	for (i = 0; i < COLUMN_MAX; i++) {
		struct COLUMN_STATUS	*Stat = &g_columnStatus[i];
		if (Stat->bShowFlag == TRUE) {
			g_nColumnTypeList[Stat->nNumber] = i;
		}
	}
	// ��\���ݒ�ŋ󂢂Ă���Ƃ�����l�߂�
	nColumn = 0;
	for (i = 0; i < COLUMN_MAX; i++) {
		if (g_nColumnTypeList[i] != -1) {
			if (i != nColumn) {
				int		nType = g_nColumnTypeList[i];
				g_nColumnTypeList[i] = -1;
				g_nColumnTypeList[nColumn] = nType;
			}
			nColumn++;
		}
	}

	// �J�����^�C�v(�^�O����) => �J�����ԍ� �ւ̕ϊ��e�[�u���쐬
	for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
		int		nType = g_nColumnTypeList[nColumn];
		if (nType == -1) break;

		// CListCtrl �ɃJ������ǉ�
		lvColumn.fmt		= g_columnStatus[nType].nFormat;
		lvColumn.pszText	= (LPTSTR)g_sNameList[0][nType+1];
		lvColumn.cx			= g_columnStatus[nType].nWidth;
		InsertColumn(nColumn, &lvColumn);

		g_nColumnNumberList[nType] = nColumn;
	}

	// �̎��t�@�C���̌������K�v���ǂ�����ݒ�
	g_bOptSearchLyricFile = g_columnStatus[COLUMN_LYRIC_FILE].bShowFlag ? true : false;

}

// =============================================
// CMySuperGrid::SetItemModifyFlag
// �T�v  : �A�C�e���̕ύX�t���O��ݒ肷��
// ����  : pItem		= �A�C�e��
//       : flag			= �t���O
// �߂�l: none
// =============================================
void CMySuperGrid::SetItemModifyFlag(CTreeItem *pItem, bool flag)
{
	CItemInfo	*info = GetData(pItem);
	LPARAM		lParam = info->GetLParam();
	if (lParam >= 0) {
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lParam);
		fileMP3->bModifyFlag = flag;
	}
}

// =============================================
// CMySuperGrid::GetItemModifyFlag
// �T�v  : �A�C�e���̕ύX�t���O���擾����
// ����  : pItem		= �A�C�e��
// �߂�l: bool			= �t���O
// =============================================
bool CMySuperGrid::GetItemModifyFlag(const CTreeItem *pItem)
{
	CItemInfo	*info = GetData(pItem);
	LPARAM		lParam = info->GetLParam();

	if (lParam < 0) return(false);

	FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lParam);
	return(fileMP3->bModifyFlag);
}

// =============================================
// CMySuperGrid::GetItemFormat
// �T�v  : �A�C�e���̃t�@�C���`�����擾����
// ����  : pItem		= �A�C�e��
// �߂�l: int			= �`��(FILE_FORMAT_xxxxxxx)
// =============================================
int CMySuperGrid::GetItemFormat(const CTreeItem *pItem)
{
	CItemInfo	*info = GetData(pItem);
	LPARAM		lParam = info->GetLParam();

	if (lParam < 0) return(FILE_FORMAT_UNKNOWN);

	FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lParam);
	return(fileMP3->nFormat);
}

// =============================================
// CMySuperGrid::GetCellRGB
// �T�v  : �J�[�\���ʒu�̃Z���̐F���擾����
// ����  : none
// �߂�l: COLORREF		= �F���
// =============================================
COLORREF CMySuperGrid::GetCellRGB()
{
	COLORREF col = RGB(192, 0, 0);
//	int iItem = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	int	iItem = GetSelectedItem();
	if (GetCurSubItem() != -1 && iItem != -1) {
		CTreeItem*pSelItem = GetTreeItem(iItem);
		if(pSelItem != NULL) {
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			if (iSubItem == 0) {
				// ���[�̃Z���̏ꍇ
				col = RGB(192, 192, 192);
			} else {
				// ����ȊO�̃Z���̏ꍇ
				CItemInfo* pInfo = GetData(pSelItem);
				CONTROLTYPE ctrlType;
				if (pInfo->GetControlType(iSubItem-1, ctrlType)) {
					if (ctrlType == invalid) {
						col = RGB(192, 192, 192);
					}
				}
			}
		}
	}
	return(col);
}

// =============================================
// CMySuperGrid::GetIcon
// �T�v  : �A�C�e���̃C���[�W�A�C�R���ԍ��̎擾����
// ����  : pItem		= �A�C�e��
// �߂�l: int			= �C���[�W�ԍ�
// =============================================
int CMySuperGrid::GetIcon(const CTreeItem* pItem)
{
	if (pItem != NULL) {
		int		n = GetData(pItem)->GetImage();
		if (n != -1) {
			CItemInfo	*pInfo = GetData(pItem);
			if (ItemHasChildren(pItem) || (int)pInfo->GetLParam() < 0) {
				// �f�B���N�g���̏ꍇ
				return(IsCollapsed(pItem) ? n : n+1);
			}

			// �t�@�C���̏ꍇ
			extern int GetFileImageIndex(UINT nFormat, bool bModify);
			int nIndex = GetFileImageIndex(GetItemFormat(pItem), GetItemModifyFlag(pItem));
			if (nIndex == -1) {
				return(GetItemModifyFlag(pItem) ? 7 : 6);
			} else {
				return nIndex;
			}
		}
	}
	return 0;
}

// =============================================
// CMySuperGrid::OnUpdateListViewItem
// �T�v  : �A�C�e���f�[�^���ύX���ꂽ�ꍇ�̏���
// ����  : lpItem
//       : plvItem
// �߂�l: none
// =============================================
void CMySuperGrid::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	CItemInfo *lp = GetData(lpItem);
	CString str = (CString)plvItem->pszText;
	if (lp != NULL) {
		if(plvItem->iSubItem == 0) {
			lp->SetItemText(str);
			UpdateData(lpItem, lp);
			SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
		} else {
			ChangeSubItemText(plvItem->iItem, plvItem->iSubItem, str);
		}
	}
}

// =============================================
// CMySuperGrid::ChangeSubItemText
// �T�v  : �Z���̓��e���X�V����
// ����  : iItem		= �A�C�e��
//       : iSubItem		= �T�u�A�C�e��
//       : sText		= �ݒ肷�镶����
// �߂�l: none
// =============================================
void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, LPCTSTR sText)
{
	ChangeSubItemText(iItem, iSubItem, sText, 0);
}

// =============================================
// CMySuperGrid::ChangeSubItemText
// �T�v  : �Z���̓��e���X�V����
// ����  : iItem		= �A�C�e��
//       : iSubItem		= �T�u�A�C�e��
//       : sText		= �ݒ肷�镶����
//       : nPos			= ��������Z���ɒǉ����� 1:�O 2:��� 0:�u������
// �߂�l: none
// =============================================
void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, LPCTSTR sUpdateText, int nPos)
{
	ChangeSubItemText(iItem, iSubItem, sUpdateText, nPos, false, TEXT(""), TEXT(""));
}


// =============================================
// CMySuperGrid::OnVkReturn
// �T�v  : [Enter]�������ꂽ�ꍇ�̏���(�ҏW�J�n����)
// ����  : none
// �߂�l: BOOL
// =============================================
BOOL CMySuperGrid::OnVkReturn()
{
	BOOL bResult=FALSE;
	int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if( GetCurSubItem() != -1 && iItem != -1)
	{
		CTreeItem *pSelItem = GetTreeItem(iItem);
		if(pSelItem!=NULL) {
			if (ItemHasChildren(pSelItem)) {
				// �t�H���_�̏ꍇ�́A[Enter]���������тɃm�[�h���J���^�����
				// �J��Ԃ�
				SendMessage(WM_KEYDOWN, IsCollapsed(pSelItem) ? VK_ADD : VK_SUBTRACT, 0);
				return(TRUE);
			}

			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			if (iSubItem == 0) {
				// ���[�̃Z���̏ꍇ
				return(TRUE);
			} else {
				// ����ȊO�̃Z���̏ꍇ
				CItemInfo* pInfo = GetData(pSelItem);
				CONTROLTYPE ctrlType;
				if (pInfo->GetControlType(iSubItem-1, ctrlType)) {
					switch(ctrlType) {
					/*put in your own control here*/
					case invalid:
						return(TRUE);
						break;
					case combobox:
						{
							CStringList *list = NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox * pList = ShowList(iItem, iSubItem, list);
							bResult = TRUE; //I'll handle it from here
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
	return bResult;
}

// =============================================
// CMySuperGrid::OnDeleteItem
// �T�v  : [DELETE]�������ꂽ�ꍇ�̏���
// ����  : pItem
//       : nIndex
// �߂�l: BOOL
// =============================================
BOOL CMySuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	// �폜�֎~
	return(FALSE);
}

// =============================================
// CMySuperGrid::OnChangeSelect
// �T�v  : �I���J�[�\���ʒu���ύX���ꂽ
// ����  : none
// �߂�l: BOOL
// =============================================
BOOL CMySuperGrid::OnChangeSelect(void)
{
	// �J�[�\���ʒu�����X�V
	UpdateStatusLocation();

	// �X�e�[�^�X�o�[�̃t�@�C���������X�V
	CString	strText;
	int		iItem;
	if ((iItem = GetSelectedItem()) != -1) {
		CTreeItem	*pItem = GetTreeItem(iItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
			strText = fileMP3->strFullPathName;
			// �t�@�C���^�C�v���̃w�b�_�[�ɕύX
			extern bool HasOriginalColumnName(UINT nFormat);
			extern UINT GetFormatTypeIndex(UINT nFormat);
			if (nCurrentFormatOfHeader != (UINT)fileMP3->nFormat) {
				bool bOriginal = HasOriginalColumnName(fileMP3->nFormat);
				int nIndex = GetFormatTypeIndex(fileMP3->nFormat) + 1;
				if (g_sNameList[nIndex] == NULL)	bOriginal = false;
				if (bOriginal == false && bCurrentFormatOfHeaderIsDefault) {
				} else {
					for (int nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
						int		nType = g_nColumnTypeList[nColumn];
						if (nType == -1)	break;
						LVCOLUMN lvColumn;
						lvColumn.mask = LVCF_TEXT;
						if (bOriginal && g_sNameList[nIndex][nType] != NULL) {
							lvColumn.pszText = (LPTSTR)g_sNameList[nIndex][nType];
						} else {
							lvColumn.pszText = (LPTSTR)g_sNameList[0][nType+1]; /* �f�t�H���g�̃J������ */
						}
						SetColumn(nColumn, &lvColumn);
						bCurrentFormatOfHeaderIsDefault = !bOriginal;
					}
				}
				nCurrentFormatOfHeader = fileMP3->nFormat;
			}
		} else if (pItem != NULL && IsItemFile(pItem) != TRUE && ItemHasChildren(pItem)) { /* LastTrain 062 */
			if (!IsRangeSelected()) {
				CArray <int, const int &> arrayList;
				int		nCount;
				nCount = MakeSelectFileArray(arrayList);
				if (GetSelectedItem() > 0) {
					strText.Format(TEXT("%d�̃t�@�C����I��"), nCount);
				} else {
					strText.Format(TEXT("�S��(%d��)�̃t�@�C����I��"), nCount);
				}
			}
		}
	}
	((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(strText);

	{ /* WildCherry 072 */
		int		nIndex = -1;
		double dSize = 0.0;
		unsigned long dPlayTime = 0; /* BeachMonster5 123 */
		int nCount=0;
		while((nIndex = GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >=0) {
			if (IsItemFile(GetTreeItem(nIndex)) == TRUE) {
				// �t�@�C���̂ݏ�������
				int		nNumber = (int)GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
					dSize += fileMP3->lFileSize;
					if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
						dPlayTime += fileMP3->nPlayTime; /* BeachMonster5 123 */
					}
					nCount++;
				}
			}
		}
		strText.Format(TEXT("%d�t�@�C��/%.2fMB/%d:%02d"), nCount, dSize / 1024 / 1024, dPlayTime / 60, dPlayTime % 60 /* BeachMonster5 123 */);
		AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_SIZE, 0, (LPARAM)strText.GetBuffer(0));
	}
	return(CSuperGridCtrl::OnChangeSelect());
}

// =============================================
// CMySuperGrid::MakeStrListGenre
// �T�v  : �W�����������X�g�̍쐬
// ����  : none
// �߂�l: none
// =============================================
void CMySuperGrid::MakeStrListGenre(void)
{
	int		nGenre;
	m_strListGenre.RemoveAll();

	// ���Ή��̃W�������̂��߂̍���
//	m_strListGenre.AddTail(_T("-NotSupported-"));

	// SCMPX�݊��W�������ԍ��̓o�^
	for (nGenre = 0; g_genreListSCMPX[nGenre].sName != NULL; nGenre++) {
		if (g_genreListSCMPX[nGenre].bAddList) {
			m_strListGenre.AddTail(g_genreListSCMPX[nGenre].sName);
		}
	}
	// ���[�U�W�������̓o�^
	for (nGenre = 0; nGenre < USER_GENRE_LIST_MAX; nGenre++) {
		USER_GENRE_LIST	*pGenre = &g_genreListUSER[nGenre];
		if (g_genreListUSER[nGenre].bUse == false) {
			continue;
		}
		if (g_genreListUSER[nGenre].bAddList) {
			m_strListGenre.AddTail(g_genreListUSER[nGenre].sName);
		}
	}
}

// =============================================
// CMySuperGrid::AddDirectory
// �T�v  : ���X�g�Ƀt�@�C����ǉ�����
// ����  : sDirName		= �f�B���N�g����
//       : pItemParent	= �e�A�C�e��
//       : nIcon		= �A�C�R��
//       : bSort		= �\�[�g�t���O
// �߂�l: CTreeItem *	= �A�C�e�����(NULL:�G���[)
// =============================================
CSuperGridCtrl::CTreeItem *CMySuperGrid::AddDirectory(LPCTSTR sDirName, CTreeItem *pItemParent, int nIcon)
{
	CItemInfo* lp = new CItemInfo();

	lp->SetFlagExt(0x00000000);
	lp->SetLParam(-1);
	lp->SetImage(nIcon);

	// �f�B���N�g������o�^
	lp->SetItemText(sDirName);

	// �S�J������ҏW�֎~�ɂ���
	for (int nCol = 0; nCol < COLUMN_MAX; nCol++) {
		lp->SetControlType(invalid, nCol);
	}

	// �c���[�ɒǉ�
	CTreeItem *pItem;
	if (pItemParent == NULL) pItem = InsertRootItem(lp);
	else                     pItem = InsertItem(pItemParent, lp, TRUE, INSERT_DIRECTORY);

	return(pItem);
}

// =============================================
// CMySuperGrid::AddFile
// �T�v  : ���X�g�Ƀt�@�C����ǉ�����
// ����  : fileMP3		= MP3�t�@�C�����
//       : pItemParent	= �e�A�C�e��
//       : lParam		= �A�C�e���ɐݒ肷��LPARAM���(���X�g�̃C���f�b�N�X)
//       : bCheck		= �`�F�b�N�}�[�N��t���邩�ǂ���
// �߂�l: bool			= true:����I�� / false:�G���[
// =============================================
bool CMySuperGrid::AddFile(const FILE_MP3 *fileMP3, CTreeItem *pItemParent, LPARAM lParam, bool bCheck)
{
	CString	strBuffer;
	CItemInfo* lp = new CItemInfo();

	lp->SetFlagExt(0x00000000);
	lp->SetLParam(lParam);
	lp->SetImage(6);

	// �e���ڂ��_�~�[�œo�^
	// �t�@�C����
	lp->AddSubItemText(TEXT(""));
	// �t���p�X
	lp->AddSubItemText(TEXT(""));
	// �g���b�N��
	lp->AddSubItemText(TEXT(""));
	// �A�[�e�B�X�g��
	lp->AddSubItemText(TEXT(""));
	// �A���o����
	lp->AddSubItemText(TEXT(""));
	// �g���b�N�ԍ�
	lp->AddSubItemText(TEXT(""));
	// �f�B�X�N�ԍ�
	lp->AddSubItemText(TEXT(""));
	// �N��
	lp->AddSubItemText(TEXT(""));
	// �W������
	lp->AddSubItemText(TEXT(""));
	// �R�����g
	lp->AddSubItemText(TEXT(""));

	// === RIFF MP3 �g������ ===
	// MP3 �`��
	lp->AddSubItemText(TEXT(""));
	// ���쌠
	lp->AddSubItemText(TEXT(""));
	// �G���W�j�A(�����)
	lp->AddSubItemText(TEXT(""));
	// �\�[�X
	lp->AddSubItemText(TEXT(""));
	// �\�t�g�E�F�A
	lp->AddSubItemText(TEXT(""));
	// �L�[���[�h
	lp->AddSubItemText(TEXT(""));
	// �Z�p��
	lp->AddSubItemText(TEXT(""));
	// �̎�
	lp->AddSubItemText(TEXT(""));
	// �R�~�b�V����
	lp->AddSubItemText(TEXT(""));

	// === ���̑� ===
	// �t�@�C���T�C�Y
	lp->AddSubItemText(TEXT(""));
	// �t�@�C���X�V��
	lp->AddSubItemText(TEXT(""));
	// �t�@�C���p�X��
	lp->AddSubItemText(TEXT(""));
	// ���t����
	lp->AddSubItemText(TEXT(""));
	// �̎��t�@�C��
	lp->AddSubItemText(TEXT(""));

	// �W�������̃R���{�{�b�N�X�̐ݒ�
	int		nColNum = g_nColumnNumberList[COLUMN_GENRE];
	if (nColNum >= 0) {
		lp->SetControlType(combobox, nColNum-1);
		lp->SetListData(nColNum-1, &m_strListGenre);
	}

	// �c���[�ɒǉ�
	CTreeItem *pItem;
	if (pItemParent == NULL) pItemParent = m_pItemRoot;
	if (pItemParent == NULL) pItem = InsertRootItem(lp);
	else                     pItem = InsertItem(pItemParent, lp, TRUE, INSERT_FILE);

	if (pItem != NULL) {
		// �e���ڂɐ���������ݒ�
		UpdateFileStatus(pItem, fileMP3);

		// �`�F�b�N��Ԃ�ݒ�
		SetCheckEx(pItem, bCheck);
	}

	return(pItem ? true : false);
}

// =============================================
// CSuperGridCtrl::SearchChildrenItem
// �T�v  : �q�A�C�e���̒�����A�w�肳�ꂽ���O�̃A�C�e������������
// ����  : strName		= ���O
//       : pItemParent	= �e�A�C�e��
// �߂�l: CTreeItem *	= ���������A�C�e��(NULL:�����ł��Ȃ�����)
// =============================================
CSuperGridCtrl::CTreeItem *CMySuperGrid::SearchChildrenItem(const CString &strName, CTreeItem *pItemParent)
{
	POSITION pos = pItemParent ? GetHeadPosition(pItemParent) : GetRootHeadPosition();
	while(pos != NULL) {
		CTreeItem	*pParent = (CTreeItem*)(pItemParent ? GetNextChild(pItemParent, pos) : GetNextRoot(pos));
		CTreeItem	*pItem = pParent;
		CItemInfo	*lp = GetData(pParent);
		if ((int)lp->GetLParam() < 0) {
			// �t�H���_�̂ݏ�������
			CString		strData = lp->GetItemText();
			if (strData == strName) {
				// ����
				return(pItem);
			}
		}
	}
	return((CTreeItem *)NULL);
}

// �t�B�[���h�e�L�X�g���擾
CString CMySuperGrid::GetFileColumnText(const FILE_MP3 *fileMP3, int nColumn)
{
	enum	{READ_ID3, READ_SIF, READ_SIF_ID3};
	int		nReadType = IsEditFieldSIF(fileMP3) ? READ_SIF : READ_ID3;
	int		nReadTypeTrackNumber = IsEditTrackNumberSIF(fileMP3) ? READ_SIF : READ_ID3;
	int		nReadTypeTrackGenre = IsEditGenreSIF(fileMP3) ? READ_SIF : READ_ID3;

	CString	strBuffer;
	switch(nColumn) {
	case COLUMN_FULL_PATH_NAME:		// �t���p�X��
		return(fileMP3->strFilePath);
	case COLUMN_FILE_NAME:		// �t�@�C����
		return(fileMP3->strFileName);
	case COLUMN_TRACK_NAME:		// �g���b�N��
		if (nReadType == READ_ID3) return(fileMP3->strTrackName);
		if (nReadType == READ_SIF) return(fileMP3->strTrackNameSI);
	case COLUMN_ARTIST_NAME:	// �A�[�e�B�X�g��
		if (nReadType == READ_ID3) return(fileMP3->strArtistName);
		if (nReadType == READ_SIF) return(fileMP3->strArtistNameSI);
	case COLUMN_ALBUM_NAME:		// �A���o����
		if (nReadType == READ_ID3) return(fileMP3->strAlbumName);
		if (nReadType == READ_SIF) return(fileMP3->strAlbumNameSI);
	case COLUMN_DISK_NUMBER:	// �f�B�X�N�ԍ�
		if (nReadType == READ_ID3) return(fileMP3->strDiskNumber);
		if (nReadType == READ_SIF) return(fileMP3->strDiskNumberSI);
	case COLUMN_TRACK_NUMBER:	// �g���b�N�ԍ�
		if (nReadTypeTrackNumber == READ_ID3) return(fileMP3->strTrackNumber);
		if (nReadTypeTrackNumber == READ_SIF) return(fileMP3->strTrackNumberSI);
		break;
	case COLUMN_YEAR:			// �����[�X�N��
		if (nReadType == READ_ID3) return(fileMP3->strYear);
		if (nReadType == READ_SIF) return(fileMP3->strYearSI);
		return(fileMP3->strYearSI.IsEmpty() ? fileMP3->strYear : fileMP3->strYearSI);
	case COLUMN_GENRE:			// �W�������ԍ�
		if (nReadTypeTrackGenre == READ_ID3) return(fileMP3->strGenre);
		if (nReadTypeTrackGenre == READ_SIF) return(fileMP3->strGenreSI);
	case COLUMN_COMMENT:		// �R�����g
		if (nReadType == READ_ID3) return(fileMP3->strComment);
		if (nReadType == READ_SIF) return(fileMP3->strCommentSI);
		return(fileMP3->strCommentSI.IsEmpty() ? fileMP3->strComment : fileMP3->strCommentSI);
	case COLUMN_COPYRIGHT:		// ���쌠
		return(fileMP3->strCopyrightSI);
	case COLUMN_ENGINEER:		// �G���W�j�A(�����)
		return(fileMP3->strEngineerSI);
	case COLUMN_SOURCE:			// �\�[�X
		return(fileMP3->strSourceSI);
	case COLUMN_SOFTWARE:		// �\�t�g�E�F�A
		return(fileMP3->strSoftwareSI);
	case COLUMN_KEYWORD:		// �L�[���[�h
		return(fileMP3->strKeywordSI);
	case COLUMN_TECHNICIAN:		// �Z�p��
		return(fileMP3->strTechnicianSI);
	case COLUMN_LYRIC:			// �̎�
		return(fileMP3->strLyricSI);
	case COLUMN_COMMISSION:		// �R�~�b�V����
		return(fileMP3->strCommissionSI);

	case COLUMN_FILE_SIZE:		// �t�@�C���T�C�Y
		strBuffer.Format(TEXT("%.2fMB"), (float)fileMP3->lFileSize / 1024 / 1024);
		break;
	case COLUMN_FILE_TIME:		// �t�@�C���X�V��
		strBuffer.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
		                  fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
		                  fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
		break;
	case COLUMN_FILE_PATH:		// �t�@�C���p�X��
		return(fileMP3->strFilePath);
	case COLUMN_PLAY_TIME:		// ���t����
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format(TEXT("%02d:%02d"), fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
		}
		break;
	case COLUMN_LYRIC_FILE:		// �̎��t�@�C��
		strBuffer.Format(TEXT("%s"), fileMP3->bExistLyricFile ? "����" : "�Ȃ�");
		break;
	case COLUMN_FILE_TYPE:		// �t�@�C���̎��
		strBuffer = GetFileType(fileMP3);
		break;
	case COLUMN_FORMAT:			// �t�H�[�}�b�g /* Conspiracy 198 */
		if (fileMP3->strVisualFormat.IsEmpty()) {
			return(fileMP3->strAudioFormat);
		} else {
			return(fileMP3->strVisualFormat + "\n" + fileMP3->strAudioFormat);
		}
		break;
	case COLUMN_WRITER:			// �쎌 /* ADD MIMURA */
		return(fileMP3->strWriterSI);
		break;
	case COLUMN_COMPOSER:		// ��� /* Baja 154 */
		return(fileMP3->strComposerSI);
		break;
	case COLUMN_ALBM_ARTIST:	// Albm.�A�[�e�B�X�g /* Baja 154 */
		return(fileMP3->strAlbmArtistSI);
		break;
	case COLUMN_ORIG_ARTIST:	// Orig.�A�[�e�B�X�g /* Baja 154 */
		return(fileMP3->strOrigArtistSI);
		break;
	case COLUMN_URL:			// URL /* Baja 154 */
		return(fileMP3->strURLSI);
		break;
	case COLUMN_ENCODEST:		// �G���R�[�h�����l /* Baja 154 */
		return(fileMP3->strEncodest);
		break;
	case COLUMN_OTHER:			//  ���̑� /* Conspiracy 196 */
		return(fileMP3->strOther);
		break;
/* 2003.06.19 start */
	case COLUMN_FILE_CTIME:		// �t�@�C���쐬��
		strBuffer.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
		                  fileMP3->timeCreate.GetYear(), fileMP3->timeCreate.GetMonth(), fileMP3->timeCreate.GetDay(),
		                  fileMP3->timeCreate.GetHour(), fileMP3->timeCreate.GetMinute(), fileMP3->timeCreate.GetSecond());
		break;
/* 2003.06.19 end */
	}
	return(strBuffer);
}

bool CMySuperGrid::AddFile2(const FILE_MP3 *fileMP3, CTreeItem *pItemParent, LPARAM lParam, bool bCheck)
{
	CTreeItem	*pParent = pItemParent ? pItemParent : m_pItemRoot;

	// �K�v�Ȃ�t�H���_���쐬����
	for (int nClass = 0; nClass < CLASS_MAX; nClass++) {
		int		nColumn = g_classInfo.nColumn[nClass];
		if (nColumn >= 0 && nColumn < COLUMN_MAX) {
			CString	strBuffer;

			// �t�@�C�����̎w��̃J�����ԍ��̏����擾
			strBuffer = GetFileColumnText(fileMP3, nColumn);
			if (strBuffer.IsEmpty() == FALSE) {
				CTreeItem	*pItem;
				if ((pItem = SearchChildrenItem(strBuffer, pParent)) == NULL) {
					// �V�����t�H���_���쐬
					pItem = AddDirectory(strBuffer, pParent, 2);
				}
				pParent = pItem;
			}
		}
	}

	// �t�@�C����ǉ�
	if (pParent == m_pItemRoot || pParent == NULL) {
		// ���ނł��Ȃ�����
		return(AddFile(fileMP3, m_pItemRoot, lParam, bCheck));
	}
	return(AddFile(fileMP3, pParent, lParam, bCheck));
}

bool CMySuperGrid::UpdateFileStatus(CTreeItem *pItem, const FILE_MP3 *fileMP3)
{
	if (pItem == NULL) return(false);
	((FILE_MP3*)fileMP3)->pTreeItem = pItem; /* STEP 037 */

	CItemInfo	*pInfo = GetData(pItem);
	int		nIndex = NodeToIndex(pItem);
	CString	strBuffer;

	// �t�@�C������o�^
	if (g_bOptHideMP3ListFile == false) {
		if (GetParentItem(pItem) == NULL) {
			strBuffer = fileMP3->strFullPathName;
		} else {
			TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
			_tsplitpath(fileMP3->strFullPathName, NULL, NULL, fname, ext);
			strBuffer = fname;
			strBuffer += ext;
//			strBuffer = fileMP3->strFileName;
		}
	}

	pInfo->SetItemText(strBuffer);
	SetItemText(nIndex, 0, strBuffer);

	// �Z���̃e�L�X�g��ݒ�
	int		nColumn;
	for (nColumn = COLUMN_FILE_NAME; nColumn < COLUMN_MAX; nColumn++) {
		// �e�L�X�g���擾
		strBuffer = GetFileColumnText(fileMP3, nColumn);

		int		nColNum = g_nColumnNumberList[nColumn];
		if (nColNum >= 0) {
			pInfo->SetSubItemText(nColNum-1, strBuffer);
			SetItemText(nIndex, nColNum, strBuffer);
		}
	}

	// �t�@�C���`���ˑ��ɂ��ҏW�\�^�s�\�ݒ菈��
	int		nColNum;
	extern UINT GetFormatTypeIndex(UINT nFormat);
	if (g_bOptEditFieldSIF) {
		for (nColNum = COLUMN_TRACK_NAME; nColNum <= COLUMN_EDIT_MAX; nColNum++) {
			SetColumnControlType(pInfo, g_nControlTypeTableSIF[GetFormatTypeIndex(fileMP3->nFormat)+1][nColNum], nColNum);
		}
	} else {
		for (nColNum = COLUMN_TRACK_NAME; nColNum <= COLUMN_EDIT_MAX; nColNum++) {
			SetColumnControlType(pInfo, g_nControlTypeTable[GetFormatTypeIndex(fileMP3->nFormat)+1][nColNum], nColNum);
		}
	}

	// ���̑�
	// �t�@�C���T�C�Y��ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_SIZE);
	// �t�@�C���X�V���̃Z���̃R���g���[���^�C�v���X�V����
	if (g_bOptKeepTimeStamp) {
		SetColumnControlType(pInfo, edit, COLUMN_FILE_TIME);
	} else {
		SetColumnControlType(pInfo, invalid, COLUMN_FILE_TIME);
	}
	// �t�@�C���p�X����ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_PATH);
	// ���t���Ԃ�ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_PLAY_TIME);
	// �̎��t�@�C����ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_LYRIC_FILE);
	// �t�@�C���̎�ނ�ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_TYPE);
	// �t�H�[�}�b�g��ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_FORMAT/* Conspiracy 198 */);
	// ���̑���ύX�s�ɂ���
	//SetColumnControlType(pInfo, invalid, COLUMN_OTHER); /* Conspiracy 196 */
	// �쐬����ύX�s�ɂ���
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_CTIME); /* 2003.06.19 add */

	UpdateData(pItem, pInfo, FALSE);

	// �`�F�b�N��Ԃ��X�V
	SetCheckEx(pItem, fileMP3->bCheckFlag ? TRUE : FALSE);

	return(true);
}

bool CMySuperGrid::UpdateAllFiles(void)
{
	CWaitCursor	wait;

	int		nMax = GetItemCount();
	int		nIndex;
	for (nIndex = 0; nIndex < nMax; nIndex++) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			UpdateFileStatus(pItem, m_pDoc->GetListMP3((int)pInfo->GetLParam()));
		}
	}

	return(true);
}

// �J�����̍ő啶�����擾����
int CMySuperGrid::OnGetColumLimitText(int nItem, int nCol)
{
	if (nItem >= 0) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL) {
			int		nType = g_nColumnTypeList[nCol];
			if (nType >= 0 && nType < COLUMN_MAX) {
				// �t�@�C���T�C�Y�^�t�@�C���p�X���^���t���ԁ^�̎��t�@�C��
				if (nType == COLUMN_FILE_SIZE || nType == COLUMN_FILE_PATH
				||  nType == COLUMN_PLAY_TIME || nType == COLUMN_LYRIC_FILE
				||	nType == COLUMN_FORMAT /* Conspiracy 198 */) {
					return(0);
				}
				// �^�C���X�^���v
				if (nType == COLUMN_FILE_TIME) {
					// 0000/00/00 00:00:00
					return(4+1+2+1+2+1+2+1+2+1+2);
				}

				int		nFileFormat = GetItemFormat(pItem);
				extern UINT GetFormatTypeIndex(UINT nFormat);
				if (g_bOptEditFieldSIF) {
					return g_nColMaxSIF[GetFormatTypeIndex(nFileFormat)+1][nType];
				} else {
					return g_nColMax[GetFormatTypeIndex(nFileFormat)+1][nType];
				}
			}
		}
	}
	return(0);
}

#define IS_RANGE(code, st, end)	((code) >= (st) && (code) <= (end))
BOOL CMySuperGrid::PreTranslateMessage(MSG* pMsg)
{
   	m_toolTip.RelayEvent(pMsg);
	if (pMsg->message == WM_KEYDOWN) {
		bool	bControl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? true : false;
		if (GetFocus() == this) {
			int		iItem = GetSelectedItem();
			int		iSubItem = GetCurSubItem();
			if (iItem != -1 && iSubItem > 0) {
				CONTROLTYPE	ctrlType;
				CTreeItem	*pSelItem = GetTreeItem(iItem);
				CItemInfo	*pInfo = GetData(pSelItem);
				if (bControl == false) {
					_RPT1(_CRT_WARN,"GetAsyncKeyState(VK_CONVERT)&0x8000=%x\n", GetAsyncKeyState(VK_CONVERT)&0x8000);
					_RPT1(_CRT_WARN,"GetAsyncKeyState(VK_NONCONVERT)&0x8000=%x\n", GetAsyncKeyState(VK_NONCONVERT)&0x8000);
					_RPT1(_CRT_WARN,"GetAsyncKeyState(0xf2)&0x8000=%x\n", GetAsyncKeyState(0x0f2)&0x8000);
					_RPT1(_CRT_WARN,"GetAsyncKeyState(0xf3)&0x8000=%x\n", GetAsyncKeyState(0x0f3)&0x8000);
					_RPT1(_CRT_WARN,"GetAsyncKeyState(0xf4)&0x8000=%x\n", GetAsyncKeyState(0x0f4)&0x8000);
					_RPT1(_CRT_WARN,"pMsg->lParam&0x00FF0000)==0x00700000=%x\n", (pMsg->lParam&0x00FF0000)==0x00700000);
					_RPT1(_CRT_WARN,"pMsg->lParam&0x00FF0000)==0x00290000=%x\n", (pMsg->lParam&0x00FF0000)==0x00290000);
					WORD	wCode = pMsg->wParam;
					if (IS_RANGE(wCode, 'A', 'Z')		// 'A' - 'Z'
					||  IS_RANGE(wCode, '0', '9')		// '9' - '0'
					||  IS_RANGE(wCode, VK_NUMPAD0, VK_NUMPAD9)	// '9' - '0'(�e���L�[)
					||  wCode == VK_SPACE				// �X�y�[�X
					||  wCode == VK_SEPARATOR			// ?????
					||  wCode == VK_MULTIPLY			// '*'(�e���L�[)
					||  wCode == VK_ADD					// '+'(�e���L�[)
					||  wCode == VK_SUBTRACT			// '-'(�e���L�[)
					||  wCode == VK_DECIMAL				// '.'(�e���L�[)
					||  wCode == VK_DIVIDE				// '/'(�e���L�[)
					||  wCode == 0x00bd					// '-'(�L�[�{�[�h�P���)
					||  wCode == 0x00de					// '^'(�L�[�{�[�h�P���)
					||  wCode == 0x00dc					// '\'(�L�[�{�[�h�P���)
					||  wCode == 0x00c0					// '@'(�L�[�{�[�h�Q���)
					||  wCode == 0x00db					// '['(�L�[�{�[�h�Q���)
					||  wCode == 0x00bb					// ';'(�L�[�{�[�h�R���)
					||  wCode == 0x00ba					// ':'(�L�[�{�[�h�R���)
					||  wCode == 0x00dd					// ']'(�L�[�{�[�h�R���)
					||  wCode == 0x00bc					// ','(�L�[�{�[�h�S���)
					||  wCode == 0x00be					// '.'(�L�[�{�[�h�S���)
					||  wCode == 0x00bf					// '/'(�L�[�{�[�h�S���)
					||  wCode == 0x00e2					// '_'(�L�[�{�[�h�S���)
					||  (wCode == VK_PROCESSKEY && !(GetAsyncKeyState(VK_CONVERT)&0x8000) && !(GetAsyncKeyState(VK_NONCONVERT)&0x8000) && /*!(GetAsyncKeyState(0xf2)&0x8000) && !(GetAsyncKeyState(0xf3)&0x8000) && !(GetAsyncKeyState(0xf4)&0x8000) &&*//* Baja 155 */ !((pMsg->lParam&0x00FF0000)==0x00700000/*�Ђ炪��*/ || (pMsg->lParam&0x00FF0000)==0x00290000/*���p/�S�p*/))/* Misirlou 151 */) {		// ���{�����(?)
						if (pInfo->GetControlType(iSubItem-1, ctrlType) == 0) {
							// �G�f�B�b�g�R���g���[���̏ꍇ
							CEdit	*pEdit;
							pEdit = EditLabelEx(iItem, iSubItem);
							if (pEdit != NULL) {
								pEdit->SetSel(0, -1);		// �S�̂�I��
								pEdit->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
							}
							return(TRUE);
						} else if (ctrlType == combobox) {
							// �R���{�{�b�N�X�̏ꍇ
							CStringList *list = NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox *pList = ShowList(iItem, iSubItem, list);
							if (pList != NULL) {
								pList->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
							}
							return(TRUE);
						}
					}
					if (wCode == VK_DELETE) {			// �Z���̓��e���N���A
						CellClearSelected();
						return(TRUE);
					}

					// �`�F�b�N�}�[�N�L��̏ꍇ
					if (GetExtendedStyle() & LVS_EX_CHECKBOXES) {
						if (wCode == VK_SPACE) {			// �X�y�[�X�o�[
							// �`�F�b�N�}�[�N�̕ύX�Ȃ̂Ŗ�������
							return(TRUE);
						}
					}
				}
			}
		}
	}

	return(CSuperGridCtrl::PreTranslateMessage(pMsg));
}
#undef IS_RANGE


void ShowLastError(LPCTSTR title)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0,
        NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, title, MB_OK);
    LocalFree(lpMsgBuf);
}


// =============================================
// CMySuperGrid::ClipboardCopy
// �T�v  : �N���b�v�{�[�h�ւ̃R�s�[����
// ����  : none
// �߂�l: bool			= true:����I�� / false:�G���[
// =============================================
bool CMySuperGrid::ClipboardCopy(void)
{
	CString		strBuffer;
	CString		strBufferText; /* RockDance 124 */

	if (IsRangeSelected() == false) {
		// ���݂̃J�[�\���ʒu��I��͈͂Ƃ��ēo�^����
		if (GetSelectedItem() == -1) return(false);	// �G���[
		SelectRangeStart();
		m_bMultiSelect = m_bMultiSelectLine = false;
	}

	// �I��͈͂̃Z���̓��e�𕶎���ɕϊ�
	int		sx, sy, ex, ey, nTemp;
	sx = m_posMultiSelect[0].x;
	sy = m_posMultiSelect[0].y;
	ex = m_posMultiSelect[1].x;
	ey = m_posMultiSelect[1].y;
	if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
	if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}
	if (sx <= COLUMN_MIN) sx = COLUMN_MIN;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItem);
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				CString	str = pItemInfo->GetSubItem(nColumn-1);
				if (g_nColumnNumberList[COLUMN_FILE_NAME] == nColumn) { /* Hurricane 214 */
					TCHAR	fname[_MAX_FNAME];
					_tsplitpath(str, NULL, NULL, fname, NULL);
					str = fname;
				}
				if (nColumn != sx) strBuffer += _T("\t");
				if (nColumn != sx) strBufferText += _T("\t"); /* RockDance 124 */
				strBuffer += quoteForComment(str)/* RockDance 124 */;
				strBufferText += str; /* RockDance 124 */
			}
			if (nItem < ey) strBuffer += _T("\r\n");
			if (nItem < ey) strBufferText += _T("\r\n"); /* RockDance 124 */
		}
	}

	// �N���b�v�{�[�h�]���p�̃o�b�t�@���m��
	LPTSTR pBuffer;
	HGLOBAL hMem, hMem2/* RockDance 124 */;
	hMem = GlobalAlloc(GHND|GMEM_DDESHARE, strBuffer.GetLength()+1);
	pBuffer = (LPTSTR)GlobalLock(hMem);
	lstrcpy(pBuffer, strBuffer);
	GlobalUnlock(hMem);
	/* RockDance 124 */
	hMem2 = GlobalAlloc(GHND|GMEM_DDESHARE, strBufferText.GetLength()+1);
	pBuffer = (LPTSTR)GlobalLock(hMem2);
	lstrcpy(pBuffer, strBufferText);
	GlobalUnlock(hMem2);

	// �N���b�v�{�[�h���J���ăR�s�[
	OpenClipboard();
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, (HANDLE)hMem2);
	::SetClipboardData(STEClipbordFormat/*CF_DSPTEXT*//* Misirlou 146 */, (HANDLE)hMem); /* RockDance 124 */
	CloseClipboard();

	return(true);
}

// =============================================
// CMySuperGrid::ClipboardCut
// �T�v  : �N���b�v�{�[�h�ւ̃J�b�g����
// ����  : none
// �߂�l: bool			= true:����I�� / false:�G���[
// =============================================
bool CMySuperGrid::ClipboardCut(void)
{
	ClipboardCopy();
	CellClearSelected();

	return(true);
}

void CMySuperGrid::PasteString(LPTSTR sBuffer)
{
#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	PasteString(sBuffer, 0, true/* RockDance 124 */);	/* FreeFall 052 */
#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif
}

void CMySuperGrid::PasteString(LPTSTR sBuffer, int nPastePos, bool bText/* RockDance 124 */)	/* FreeFall 052 */
{
	PasteString(sBuffer, nPastePos, false, false, TEXT(""), TEXT(""));
}

// =============================================
// CMySuperGrid::ClipboardPaste
// �T�v  : �N���b�v�{�[�h�ւ���̃y�[�X�g����
// ����  : none
// �߂�l: bool			= true:����I�� / false:�G���[
// =============================================
bool CMySuperGrid::ClipboardPaste(void)
{
	return ClipboardPaste(0, false, TEXT(""), TEXT("")); /* Baja 171 */
}

// �Z���̓��e���������փR�s�[
bool CMySuperGrid::CellCopyDown(void)
{
	if (IsRangeSelected() == false) return(false);

	int		sx, sy, ex, ey, nTemp;
	sx = m_posMultiSelect[0].x;
	sy = m_posMultiSelect[0].y;
	ex = m_posMultiSelect[1].x;
	ey = m_posMultiSelect[1].y;
	if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
	if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}
	if (sx <= COLUMN_MIN) sx = COLUMN_MIN;
	if (sy == ey) return(false);

	bool bSaveLoadFileAdjustColumn = g_bOptLoadFileAdjustColumn; /* RockDance 130 */
	g_bOptLoadFileAdjustColumn = false; /* RockDance 130 */

#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	int nColumn;
	int nItem;
	for (nColumn = sx; nColumn <= ex; nColumn++) {
		CTreeItem	*pItemBase = GetTreeItem(sy);
		if (pItemBase != NULL && IsItemFile(pItemBase) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItemBase);
			CString		strBase = pItemInfo->GetSubItem(nColumn-1);

			for (nItem = sy+1; nItem <= ey; nItem++) {
				CTreeItem	*pItem = GetTreeItem(nItem);
				if (pItem != NULL && IsItemFile(pItem) == TRUE) {
					// �Z���ɕ������ݒ�
#ifdef FLICKERFREE
					/*
					CItemInfo *pCurrentItemInfo = GetData(pItem);
					CString strCurrentText = pCurrentItemInfo->GetSubItem(nColumn-1);
					if (strCurrentText != strBase) {
						ChangeSubItemText(nItem, nColumn, strBase);
						InvalidateRangeItem(0, nItem, 0, nItem);
					} else*/
#endif
					ChangeSubItemText(nItem, nColumn, strBase);
#ifdef FLICKERFREE
					//InvalidateItemRect(nItem);
#endif
				}
			}
		}
	}
#ifdef FLICKERFREE
	InvalidateRangeItem(sx, sy, ex, ey);
	InvalidateRangeItem(0, sy, 0, ey);
#endif
	/* RockDance 130 */
	g_bOptLoadFileAdjustColumn = bSaveLoadFileAdjustColumn;
	for (nColumn = sx; nColumn <= ex; nColumn++) {
		if (g_bOptLoadFileAdjustColumn) AutoSizeColumns(nColumn);
	}
#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif
	return(true);
}

bool CMySuperGrid::CellClear(int iItem, int iSubItem)
{
	if (iItem < 0) return(false);

	ChangeSubItemText(iItem, iSubItem, _T(""));
	InvalidateItemRect(iItem);
	return(true);
}

bool CMySuperGrid::CellClearSelected(void)
{
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}
#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				ChangeSubItemText(nItem, nColumn, _T(""));
				InvalidateItemRect(nItem);
			}
		}
	}
#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif

	return(true);
}

bool CMySuperGrid::SetFillNumber(int nStart, int nStep, bool bSpaceInitNumber/* Conspiracy 194 */)
{
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	int		nNumber = nStart;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CString	strNumber;
			strNumber.Format(TEXT("%d"), nNumber);
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				ChangeSubItemText(nItem, nColumn, strNumber);
				InvalidateItemRect(nItem);
			}
			nNumber += nStep;
		} else { /* Conspiracy 194 */
			if (bSpaceInitNumber) {
				nNumber = nStart;
			}
		}
	}

#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif
	return(true);
}

int CMySuperGrid::CompFunc(CString &str1, CString &str2, int nColum, bool bSortAscending)
{
	/* BeachMonster4 114 */
	int result;
	if (nColum == g_nColumnNumberList[COLUMN_TRACK_NUMBER] &&
		_tstoi(str1) > 0 && _tstoi(str2) > 0) { /* BeachMonster4 118 */
		result = _tstoi(str1) - _tstoi(str2);
	} else if (nColum == g_nColumnNumberList[COLUMN_DISK_NUMBER] &&
		_tstoi(str1) > 0 && _tstoi(str2) > 0) { /* BeachMonster4 118 */
		result = _tstoi(str1) - _tstoi(str2);
	} else if (nColum == g_nColumnNumberList[COLUMN_FILE_SIZE]) { /* RockDance 127 */
		result = (int)(_tstof(str1)*100 - _tstof(str2)*100);
	} else {
		std::vector<TCHAR> buffer1(str1.GetLength()*2+2);
		std::vector<TCHAR> buffer2(str2.GetLength()*2+2);
		CString strComp1 = str1; /* FunnyCorn 179 */
		CString strComp2 = str2; /* FunnyCorn 179 */
		if (g_bOptSortIgnoreKataHira) { /* FunnyCorn 179 */
			lstrcpy(buffer1.data(), str1);
			lstrcpy(buffer2.data(), str2);
			conv_hira2kata(buffer1.data());
			conv_hira2kata(buffer2.data());
			conv_kata_erase_dakuon(buffer1.data());
			conv_kata_erase_dakuon(buffer2.data());
			strComp1 = buffer1.data();
			strComp2 = buffer2.data();
		}
		if (g_bOptSortIgnoreZenHan) {
			conv_zen2hans(buffer1.data(), strComp1/* str1*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
			conv_zen2hans(buffer2.data(), strComp2/* str2*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
		} else {
			lstrcpy(buffer1.data(), str1);
			lstrcpy(buffer2.data(), str2);
		}
		if (FALSE && g_bOptSortIgnoreKataHira) { /* FunnyCorn 179 */
			strComp1 = buffer1.data();
			strComp2 = buffer2.data();
			conv_han2zens(buffer1.data(), strComp1/* str1*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
			conv_han2zens(buffer2.data(), strComp2/* str2*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
		}
		CString han1 = buffer1.data();
		if (g_bOptSortIgnoreCase) {
			result = han1.CompareNoCase(buffer2.data());
			//result = _mbsicmp(buffer1, buffer2);
		} else {
			result = han1.Compare(buffer2.data());
			//result = _mbscmp(buffer1, buffer2);
		}
	}
	if (!bSortAscending) {
		result *= -1;
	}
	return result;
}

int CMySuperGrid::CompFunc(SORT_STATE *sortState, int nKeys, int nItem1, int nItem2)
{
	int i; for (i = 0; i < nKeys; i++) {
		// �w�肳�ꂽ�L�[�Ŕ�r
		int		nColumn = g_nColumnNumberList[sortState->nKeyColumn];
		if (nColumn >= 0 && nColumn < COLUMN_MAX) {
			int		nResult;
			nResult = CompFunc(GetItemText(nItem1, nColumn),
			                   GetItemText(nItem2, nColumn),
			                   nColumn,
			                   (sortState->nType == 0) ? true : false);
			if (nResult != 0) return(nResult);
		}

		// ���̃L�[
		sortState++;
	}
	return(0);
}

BOOL CMySuperGrid::SortTextItems(SORT_STATE *sortState, int nKeys, int low, int high)
{
	if (high == -1) high = GetItemCount() - 1;

	int		lo = low;
	int		hi = high;
	int		nMidItem;

	if (hi <= lo) return FALSE;

	nMidItem = (lo + hi) / 2;

	// loop through the list until indices cross
	while(lo <= hi) {
		while((lo < high) && (CompFunc(sortState, nKeys, lo, nMidItem) < 0))
			++lo;

		while((hi > low) && (CompFunc(sortState, nKeys, hi, nMidItem) > 0))
			--hi;

		if (lo <= hi) {
			// swap only if the items are not equal
			if (lo != hi && CompFunc(sortState, nKeys, lo, hi) != 0) {
				// swap the rows
				CItemInfo *pInfo1 = GetData(GetTreeItem(hi));
				CItemInfo *pInfo2 = GetData(GetTreeItem(lo));
				UpdateData(GetTreeItem(hi), pInfo2, TRUE);
				UpdateData(GetTreeItem(lo), pInfo1, TRUE);

				// �`�F�b�N��Ԃ�����ւ�
				BOOL	bCheck1 = pInfo1->GetCheck();
				BOOL	bCheck2 = pInfo2->GetCheck();
				SetCheck(hi, bCheck2);
				SetCheck(lo, bCheck1);

				// nMidItem ���������ꂽ�ꍇ�́A�ԍ��𒲐�����
				if      (nMidItem == lo) nMidItem = hi;
				else if (nMidItem == hi) nMidItem = lo;
			}

			++lo;
			--hi;
		}
	}

	if (low < hi) SortTextItems(sortState, nKeys, low, hi);
	if (lo < high) SortTextItems(sortState, nKeys, lo, high);

	return TRUE;
}

void CMySuperGrid::MultiColumnSort(SORT_STATE *sortState, int nKeys)
{
	int		nMax = GetItemCount();
	int		nIndex;

	for (nIndex = 0; nIndex < nMax; nIndex++) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CTreeItem	*pParent = GetParentItem(pItem);
			int nEnd; for (nEnd=nIndex+1; nEnd < nMax; nEnd++) {
				CTreeItem	*pItem = GetTreeItem(nEnd);
				if (pItem == NULL || IsItemFile(pItem) == FALSE || pParent != GetParentItem(pItem)) {
					break;
				}
			}
			if (nEnd-1 > nIndex) {
				SortTextItems(sortState, nKeys, nIndex, nEnd-1);
			}
			nIndex = nEnd - 1;
		}
	}

	// �ĕ`��
	InvalidateRect(NULL);
	UpdateWindow();
}

void CMySuperGrid::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_bSortAscending = (pNMListView->iSubItem == m_nSortedCol) ? !m_bSortAscending : TRUE;
	m_nSortedCol = pNMListView->iSubItem;

	// �ҏW���̏ꍇ�ւ̑Ώ�
	// (�t�H�[�J�X��߂��ĕҏW�����������Ă���\�[�g����)
	if (GetFocus() != this) SetFocus();

	CWaitCursor	wait;

	bool	bControl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? true : false;
	if (bControl) {
		// �N���b�N���ꂽ�J�����̕��𒲐�
		// �J�������̎�������
		AutoSizeColumns(m_nSortedCol);
	} else {
		// �N���b�N���ꂽ�J�����Ń\�[�g���s
		SORT_STATE	sortState;
		sortState.nKeyColumn = g_nColumnTypeList[m_nSortedCol];
		sortState.nType = m_bSortAscending ? 0 : 1;
		LockWindowUpdate();			// �`��֎~(������h�~)
		MultiColumnSort(&sortState, 1);
		UnlockWindowUpdate();		// �`�拖��(������h�~)
	}

	*pResult = 0;
}

// =============================================
// CMySuperGrid::OnContextMenu
// �T�v  : �|�b�v�A�b�v���j���[����
// ����  : ...
// �߂�l: none
// =============================================
void CMySuperGrid::OnContextMenu(CWnd*, CPoint point)
{
	if (GetSelectedItem() == -1) return;

	//�L�[�X�g���[�N�ɂ�锭���̏ꍇ�́A�ʒu�𒲐�����
	if (point.x == -1 && point.y == -1) {
		// �J�����g�̃Z���ʒu�Ƀ��j���[���J��
		CRect rect;
		GetSubItemRect(GetSelectedItem(), GetCurSubItem(), LVIR_BOUNDS, rect);
		// �Z���̍�����1/3�A�ォ��1/2�̈ʒu�ɒ���
		point.x = rect.left + (rect.right - rect.left) / 3;
		point.y = (rect.top + rect.bottom) / 2;
		ClientToScreen(&point);
	}

	CMenu menu;
	bool	bUpdateMenuText = false;
	bool	bUpdateMenuText2= false; /* TyphoonSwell 027 */
	bool	bUpdateUserConvMenu = false; /* STEP 030 */

	extern void OnLoadMenu(CMenu* pMenu, UINT nType);
	if (IsRangeSelected() == true) {
		// �Z���̕����I��
		VERIFY(menu.LoadMenu(IDR_MENU_CELL_RANGE));
		OnLoadMenu(&menu, MENU_CELL_RANGE);
		bUpdateMenuText = true;
	} else if (GetSelectedCount() > 2) {
		// �����s�I��
		VERIFY(menu.LoadMenu(IDR_MENU_FILE_RANGE));
		OnLoadMenu(&menu, MENU_FILE_RANGE);
		bUpdateMenuText2= true;
		bUpdateUserConvMenu = true;
	} else {
		// �P��s�I��
		int		nIndex = GetSelectedItem();
		if (nIndex >= 0) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			if (pItem != NULL && IsItemFile(pItem) == TRUE) {
				// �t�@�C��
//				if (IsCurrentCellEditOK()) {
				if (GetCurSubItem() >= COLUMN_MIN) {
					// �ҏW�\�ȃZ��
					VERIFY(menu.LoadMenu(IDR_MENU_FILE_EDIT_OK));
					OnLoadMenu(&menu, MENU_FILE_EDIT_OK);
					bUpdateMenuText = true;
					bUpdateUserConvMenu = true; /* STEP 030 */
				} else {
					// �ҏW�s�̃Z��
					VERIFY(menu.LoadMenu(IDR_MENU_FILE_RANGE));
					OnLoadMenu(&menu, MENU_FILE_RANGE);
					bUpdateUserConvMenu = true; /* STEP 030 */
				}
			} else {
				// �t�H���_
				VERIFY(menu.LoadMenu(IDR_MENU_SELECT_FOLDER));
				OnLoadMenu(&menu, MENU_SELECT_FOLDER);
				bUpdateUserConvMenu = true; /* STEP 030 */
			}
		}
		bUpdateMenuText2= true;
	}

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	CMenu convSubMenuT2F;
	CMenu convSubMenuF2T;
	if (bUpdateUserConvMenu && g_bUserConvAddMenu) { /* STEP 030 */
		CMenu* pUConvMenu = NULL;
		UINT nCount = pPopup->GetMenuItemCount();
		for (UINT j=0;j<nCount;j++) {
			CString strTitle;
			pPopup->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "���[�U�[�w��ϊ�(&U)") {
				pUConvMenu = pPopup->GetSubMenu(j);
				break;
			}
		}
		CString menuTitle;
		convSubMenuT2F.LoadMenu(IDR_MENU_CONV_FORMAT_USER_T2F);
		pUConvMenu->GetMenuString(ID_CONV_TAG2FILE_USER, menuTitle, MF_BYCOMMAND);
		pUConvMenu->ModifyMenu(ID_CONV_TAG2FILE_USER, MF_BYCOMMAND | MF_POPUP, (UINT)convSubMenuT2F.GetSubMenu(0)->GetSafeHmenu(), menuTitle);

		convSubMenuF2T.LoadMenu(IDR_MENU_CONV_FORMAT_USER_F2T);
		pUConvMenu->GetMenuString(ID_CONV_FILE2TAG_USER, menuTitle, MF_BYCOMMAND);
		pUConvMenu->ModifyMenu(ID_CONV_FILE2TAG_USER, MF_BYCOMMAND | MF_POPUP, (UINT)convSubMenuF2T.GetSubMenu(0)->GetSafeHmenu(), menuTitle);
	}

	// ���j���[���ږ��̍X�V
	if (bUpdateMenuText) {
		UINT	dwCmdID[] = {
			ID_CONV_FORMAT_EX_01,
			ID_CONV_FORMAT_EX_02,
			ID_CONV_FORMAT_EX_03,
			ID_CONV_FORMAT_EX_04,
			ID_CONV_FORMAT_EX_05,
			ID_CONV_FORMAT_EX_06,
			ID_CONV_FORMAT_EX_07,
			ID_CONV_FORMAT_EX_08,
			ID_CONV_FORMAT_EX_09,
			ID_CONV_FORMAT_EX_10,
		};
		int i; for (i = 0; i < 10; i++) {
			// ���j���[���ڂ̏�������
			CString	strName;
			// ���j���[���ږ����擾
			switch(dwCmdID[i]) {
			case ID_CONV_FORMAT_EX_01:
				strName.Format(TEXT("01�F%s"), g_userConvFormatEx[0].strName);
				break;
			case ID_CONV_FORMAT_EX_02:
				strName.Format(TEXT("02�F%s"), g_userConvFormatEx[1].strName);
				break;
			case ID_CONV_FORMAT_EX_03:
				strName.Format(TEXT("03�F%s"), g_userConvFormatEx[2].strName);
				break;
			case ID_CONV_FORMAT_EX_04:
				strName.Format(TEXT("04�F%s"), g_userConvFormatEx[3].strName);
				break;
			case ID_CONV_FORMAT_EX_05:
				strName.Format(TEXT("05�F%s"), g_userConvFormatEx[4].strName);
				break;
			case ID_CONV_FORMAT_EX_06:
				strName.Format(TEXT("06�F%s"), g_userConvFormatEx[5].strName);
				break;
			case ID_CONV_FORMAT_EX_07:
				strName.Format(TEXT("07�F%s"), g_userConvFormatEx[6].strName);
				break;
			case ID_CONV_FORMAT_EX_08:
				strName.Format(TEXT("08�F%s"), g_userConvFormatEx[7].strName);
				break;
			case ID_CONV_FORMAT_EX_09:
				strName.Format(TEXT("09�F%s"), g_userConvFormatEx[8].strName);
				break;
			case ID_CONV_FORMAT_EX_10:
				strName.Format(TEXT("10�F%s"), g_userConvFormatEx[9].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// ���j���[��������X�V
			pPopup->ModifyMenu((UINT)dwCmdID[i], MF_BYCOMMAND, dwCmdID[i], (LPCTSTR)strName);
		}
	}
	if (bUpdateMenuText2) {  /* TyphoonSwell 027 */
		UINT	dwCmdID[] = {
			ID_CONV_FORMAT_USER_01,
			ID_CONV_FORMAT_USER_02,
			ID_CONV_FORMAT_USER_03,
			ID_CONV_FORMAT_USER_04, /* LastTrain 057 */
			ID_CONV_FORMAT_USER_05, /* LastTrain 057 */
			/* STEP 030 */
			ID_CONV_FORMAT_USER_T2F_01,
			ID_CONV_FORMAT_USER_T2F_02,
			ID_CONV_FORMAT_USER_T2F_03,
			ID_CONV_FORMAT_USER_T2F_04,
			ID_CONV_FORMAT_USER_T2F_05,
			ID_CONV_FORMAT_USER_F2T_01,
			ID_CONV_FORMAT_USER_F2T_02,
			ID_CONV_FORMAT_USER_F2T_03,
			ID_CONV_FORMAT_USER_F2T_04,
			ID_CONV_FORMAT_USER_F2T_05,
		};
		int i; for (i = 0; i < /*3*//*5*//* WildCherry 066 */sizeof(dwCmdID)/sizeof(UINT)/* STEP 030 */; i++) {
			if (!(bUpdateUserConvMenu && g_bUserConvAddMenu) && i>=5)	break;
			// ���j���[���ڂ̏�������
			CString	strName;
			// ���j���[���ږ����擾
			switch(dwCmdID[i]) {
			case ID_CONV_FORMAT_USER_01:
			case ID_CONV_FORMAT_USER_T2F_01: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_01: /* STEP 030 */
				strName.Format(TEXT("01�F%s"), g_userConvFormat[0].strName);
				break;
			case ID_CONV_FORMAT_USER_02:
			case ID_CONV_FORMAT_USER_T2F_02: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_02: /* STEP 030 */
				strName.Format(TEXT("02�F%s"), g_userConvFormat[1].strName);
				break;
			case ID_CONV_FORMAT_USER_03:
			case ID_CONV_FORMAT_USER_T2F_03: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_03: /* STEP 030 */
				strName.Format(TEXT("03�F%s"), g_userConvFormat[2].strName);
				break;
			case ID_CONV_FORMAT_USER_04: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_04: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_04: /* STEP 030 */
				strName.Format(TEXT("04�F%s"), g_userConvFormat[3].strName);
				break;
			case ID_CONV_FORMAT_USER_05: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_05: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_05: /* STEP 030 */
				strName.Format(TEXT("05�F%s"), g_userConvFormat[4/* WildCherry 066 */].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// ���j���[��������X�V
			pPopup->ModifyMenu((UINT)dwCmdID[i], MF_BYCOMMAND, dwCmdID[i], (LPCTSTR)strName);
		}
	}
	if (bUpdateMenuText) {
		UINT	dwCmdID[] = {
			ID_CONV_TAG_TO_TAG_01,
			ID_CONV_TAG_TO_TAG_02,
			ID_CONV_TAG_TO_TAG_03,
			ID_CONV_TAG_TO_TAG_04,
			ID_CONV_TAG_TO_TAG_05,
		};
		int i; for (i = 0; i < 5; i++) {
			// ���j���[���ڂ̏�������
			CString	strName;
			// ���j���[���ږ����擾
			switch(dwCmdID[i]) {
			case ID_CONV_TAG_TO_TAG_01:
				strName.Format(TEXT("01�F%s"), g_userConvFormatTag2Tag[0].strName);
				break;
			case ID_CONV_TAG_TO_TAG_02:
				strName.Format(TEXT("02�F%s"), g_userConvFormatTag2Tag[1].strName);
				break;
			case ID_CONV_TAG_TO_TAG_03:
				strName.Format(TEXT("03�F%s"), g_userConvFormatTag2Tag[2].strName);
				break;
			case ID_CONV_TAG_TO_TAG_04:
				strName.Format(TEXT("04�F%s"), g_userConvFormatTag2Tag[3].strName);
				break;
			case ID_CONV_TAG_TO_TAG_05:
				strName.Format(TEXT("05�F%s"), g_userConvFormatTag2Tag[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// ���j���[��������X�V
			pPopup->ModifyMenu((UINT)dwCmdID[i], MF_BYCOMMAND, dwCmdID[i], (LPCTSTR)strName);
		}
	}
	if (bUpdateMenuText) { /* SeaKnows 030 */
		UINT	dwCmdID[] = {
			ID_TEIKEI_01,
			ID_TEIKEI_02,
			ID_TEIKEI_03,
			ID_TEIKEI_04,
			ID_TEIKEI_05,
			ID_TEIKEI_06,
			ID_TEIKEI_07,
			ID_TEIKEI_08,
			ID_TEIKEI_09,
			ID_TEIKEI_10,
			/* FreeFall 046 */
			ID_TEIKEI_2_01,
			ID_TEIKEI_2_02,
			ID_TEIKEI_2_03,
			ID_TEIKEI_2_04,
			ID_TEIKEI_2_05,
			ID_TEIKEI_2_06,
			ID_TEIKEI_2_07,
			ID_TEIKEI_2_08,
			ID_TEIKEI_2_09,
			ID_TEIKEI_2_10,
			ID_TEIKEI_3_01,
			ID_TEIKEI_3_02,
			ID_TEIKEI_3_03,
			ID_TEIKEI_3_04,
			ID_TEIKEI_3_05,
			ID_TEIKEI_3_06,
			ID_TEIKEI_3_07,
			ID_TEIKEI_3_08,
			ID_TEIKEI_3_09,
			ID_TEIKEI_3_10,
		};
		/* FreeFall 046 */ // �����ƃX�}�[�g�ɂł��Ȃ�����....
		CMenu* pTeikeiMenu = NULL;
		int nCount = pPopup->GetMenuItemCount();
		for (int j=0;j<nCount;j++) {
			CString strTitle;
			pPopup->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "��^���\��t��") {
				pTeikeiMenu = pPopup->GetSubMenu(j);
				break;
			}
		}
		for (int k = 0; k < 3 && pTeikeiMenu; k++) {
			CMenu* pSubMenu = pTeikeiMenu->GetSubMenu(0);
			pTeikeiMenu->RemoveMenu(0, MF_BYPOSITION);
			pTeikeiMenu->AppendMenu(MF_POPUP , (UINT)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
			int i; for (i = 0; i < 10; i++) {
				// ���j���[���ڂ̏�������
				CString	strName;
				// ���j���[���ږ����擾
				strName.Format(TEXT("%02d�F%s"), i+1, shortString(g_teikeiInfo[i+k*10].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				// ���j���[��������X�V
				pTeikeiMenu->ModifyMenu((UINT)dwCmdID[i+k*10], MF_BYCOMMAND, dwCmdID[i+k*10], (LPCTSTR)strName);
			}
		}
	}
	if (bUpdateMenuText || bUpdateMenuText2) {  /* LastTrain 065 */
		UINT	dwCmdID[] = {
			ID_MOVE_FOLDER_01,
			ID_MOVE_FOLDER_02,
			ID_MOVE_FOLDER_03,
			ID_MOVE_FOLDER_04,
			ID_MOVE_FOLDER_05,
		};
		int i; for (i = 0; i < 5; i++) {
			// ���j���[���ڂ̏�������
			CString	strName;
			// ���j���[���ږ����擾
			switch(dwCmdID[i]) {
			case ID_MOVE_FOLDER_01: /* LastTrain 065 */
				strName.Format(TEXT("01�F%s"), g_userMoveFolder[0].strName);
				break;
			case ID_MOVE_FOLDER_02: /* LastTrain 065 */
				strName.Format(TEXT("02�F%s"), g_userMoveFolder[1].strName);
				break;
			case ID_MOVE_FOLDER_03: /* LastTrain 065 */
				strName.Format(TEXT("03�F%s"), g_userMoveFolder[2].strName);
				break;
			case ID_MOVE_FOLDER_04: /* LastTrain 065 */
				strName.Format(TEXT("04�F%s"), g_userMoveFolder[3].strName);
				break;
			case ID_MOVE_FOLDER_05: /* LastTrain 065 */
				strName.Format(TEXT("05�F%s"), g_userMoveFolder[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// ���j���[��������X�V
			pPopup->ModifyMenu((UINT)dwCmdID[i], MF_BYCOMMAND, dwCmdID[i], (LPCTSTR)strName);
		}
	}
	if (bUpdateMenuText || bUpdateMenuText2) {  /* FunnyCorn 175 */
		UINT	dwCmdID[] = {
			ID_EDIT_COPY_FORMAT_01,
			ID_EDIT_COPY_FORMAT_02,
			ID_EDIT_COPY_FORMAT_03,
			ID_EDIT_COPY_FORMAT_04,
			ID_EDIT_COPY_FORMAT_05,
		};
		int i; for (i = 0; i < 5; i++) {
			// ���j���[���ڂ̏�������
			CString	strName;
			// ���j���[���ږ����擾
			switch(dwCmdID[i]) {
			case ID_EDIT_COPY_FORMAT_01:
				strName.Format(TEXT("01�F%s"), g_userCopyFormat[0].strName);
				break;
			case ID_EDIT_COPY_FORMAT_02:
				strName.Format(TEXT("02�F%s"), g_userCopyFormat[1].strName);
				break;
			case ID_EDIT_COPY_FORMAT_03:
				strName.Format(TEXT("03�F%s"), g_userCopyFormat[2].strName);
				break;
			case ID_EDIT_COPY_FORMAT_04:
				strName.Format(TEXT("04�F%s"), g_userCopyFormat[3].strName);
				break;
			case ID_EDIT_COPY_FORMAT_05:
				strName.Format(TEXT("05�F%s"), g_userCopyFormat[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// ���j���[��������X�V
			pPopup->ModifyMenu((UINT)dwCmdID[i], MF_BYCOMMAND, dwCmdID[i], (LPCTSTR)strName);
		}
	}

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

// =============================================
// CMySuperGrid::CellStateControl
// �T�v  : �Z�����̐��䏈������
// ����  : nMode
// �߂�l: none
// =============================================
void CMySuperGrid::CellStateControl(int nMode)
{
	CString strINI = AfxGetApp()->m_pszProfileName;
	InitProfile();
	//Profile_Initialize(strINI, TRUE);
	static LPCTSTR sSectionColumnState	= TEXT("ColumnState");

	int		nColumn;
	CWinApp	*pApp = AfxGetApp();

	if (nMode == 0) {
		// �J�����������W�X�g������ǂݏo��

		int ColNumFix = 0;		// ���o�[�W��������̃J�E���g�␳

		CString	strVersion;		// �␳���p
		double dVersion;
		strVersion = MyGetProfileString(TEXT("Option"), TEXT("Version"), TEXT(""));
		dVersion = _tstof(strVersion);

		for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
			CString	strKey;
			struct COLUMN_STATUS	*Stat = &g_columnStatus[nColumn];

			// �\���t���O
			strKey.Format(TEXT("%sShow"), Stat->sRegKey);
			Stat->bShowFlag = MyGetProfileInt(sSectionColumnState, strKey, Stat->bShowFlag);

			// �J�����ʒu
			strKey.Format(TEXT("%sColNum"), Stat->sRegKey);
			Stat->nNumber = MyGetProfileInt(sSectionColumnState, strKey, Stat->nNumber) + ColNumFix;

			// �J������
			strKey.Format(TEXT("%sWidth"), Stat->sRegKey);
			Stat->nWidth = MyGetProfileInt(sSectionColumnState, strKey, Stat->nWidth);

			// �ő�J������
			strKey.Format(TEXT("%sWidthMax"), Stat->sRegKey);
			Stat->nWidthMax = MyGetProfileInt(sSectionColumnState, strKey, Stat->nWidthMax);

			if(strcmp(Stat->sRegKey,"DiskNo") == 0) {
				if((dVersion == 1.04)|(dVersion == 1.041))
					ColNumFix++;
			}
		}
	} else if (nMode == 1) {
		// ���݂̃J�����T�C�Y���擾
		CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
		int nColumnCount = pHeader->GetItemCount();
		for (nColumn = 0; nColumn < nColumnCount; nColumn++) {
			g_columnStatus[g_nColumnTypeList[nColumn]].nWidth = GetColumnWidth(nColumn);
		}
		// �J�����������W�X�g���ɏ����o��
		for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
			CString	strKey;
			struct COLUMN_STATUS	*Stat = &g_columnStatus[nColumn];

			// �\���t���O
			strKey.Format(TEXT("%sShow"), Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->bShowFlag);

			// �J�����ʒu
			strKey.Format(TEXT("%sColNum"), Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->nNumber);

			// �J������
			strKey.Format(TEXT("%sWidth"), Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->nWidth);

			// �ő�J������
			strKey.Format(TEXT("%sWidthMax"), Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->nWidthMax);
		}
	}
	//Profile_Flush(strINI);
	SaveProfile();
	FreeProfile();
	//Profile_Free();
}

LRESULT CMySuperGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY) {
		// �Z���������W�X�g���ɕۑ�
		CellStateControl(1);
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

bool CMySuperGrid::ConvTagInfo(int nIndex, int nType, LPCTSTR sFormat)
{
	return(ConvTagInfo(GetTreeItem(nIndex), nType, sFormat));
}

bool CMySuperGrid::ConvTagInfoSelected(int nType, LPCTSTR sFormat)
{
	// �I������Ă���t�@�C����S�ď�������
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];
		ConvTagInfo(GetTreeItem(nIndex), nType, sFormat);
	}
	return(true);
}

bool CMySuperGrid::ConvTagInfo(CTreeItem *pItem, int nType, LPCTSTR sFormat, LPCTSTR sBaseTag)
{
	if (pItem == NULL || IsItemFile(pItem) == FALSE) return(false);

	int		nIndex = NodeToIndex(pItem);
	CItemInfo	*pItemInfo = GetData(pItem);
	FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
	switch(nType) {
	default:
	case 1:		// �t�@�C���� => �g���b�N��
		{
			CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
			CString	strTrackName = GetFileColumnText(fileMP3, COLUMN_TRACK_NAME);
//			if (strTrackName.IsEmpty()) {
				TCHAR	fname[_MAX_FNAME];
				_tsplitpath(strFileName, NULL, NULL, fname, NULL);
				ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_TRACK_NAME], fname);
				InvalidateItemRect(nIndex);
//			}
		}
		break;
	case 2:		// �g���b�N�� => �t�@�C����
		{
			CString	strTrackName = GetFileColumnText(fileMP3, COLUMN_TRACK_NAME);
			if (strTrackName.IsEmpty() == false) {
				ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_FILE_NAME], strTrackName);
				InvalidateItemRect(nIndex);
			}
		}
		break;
	case 3:		// ���[�U�[�w���[�^�O���] => [�t�@�C����]�ϊ�
		{
			CString	strFileName = sFormat;
			auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));
			auto[strDiskNumber, strDiskNumber2, strDiskNumber3] = GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding(TEXT("\r")));
			strFileName = StrReplace(strFileName, TEXT("%TRACK_NAME%")   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding(TEXT("\r")));
			strFileName = StrReplace(strFileName, TEXT("%ALBUM_NAME%")   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding(TEXT("\r")));
			strFileName = StrReplace(strFileName, TEXT("%ARTIST_NAME%")  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding(TEXT("\r")));
			strFileName = StrReplace(strFileName, TEXT("%TRACK_NUMBER%") , strTrackNumber);
			strFileName = StrReplace(strFileName, TEXT("%TRACK_NUMBER2%"), strTrackNumber2);
			strFileName = StrReplace(strFileName, TEXT("%TRACK_NUMBER3%"), strTrackNumber3);
			strFileName = StrReplace(strFileName, TEXT("%DISK_NUMBER%") , strDiskNumber);
			strFileName = StrReplace(strFileName, TEXT("%DISK_NUMBER2%"), strDiskNumber2);
			strFileName = StrReplace(strFileName, TEXT("%DISK_NUMBER3%"), strDiskNumber3);
			strFileName = StrReplace(strFileName, TEXT("%RELEASE_YEAR%") , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding(TEXT("\r")));
			strFileName = StrReplace(strFileName, TEXT("%COMMENT%")      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding(TEXT("\r"))/* BeachMonster 089 */);
			strFileName = StrReplace(strFileName, TEXT("%GENRE%")        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding(TEXT("\r")));
			// SIF �̍���
			strFileName = StrReplaceSIF(strFileName, fileMP3);

			ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_FILE_NAME], strFileName);
			InvalidateItemRect(nIndex);
		}
		break;
	case 4:		// ���[�U�[�w���[�t�@�C����] => [�^�O���]�ϊ�
	case 5:		// �^�O�U���ϊ� /* STEP 034 */
		{
			// �t�@�C�����������擾
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			TCHAR	*pFileName; /* STEP 034 */
			CString strFileName; /* STEP 034 */
			if (nType == 4) { /* STEP 034 */
				TCHAR	sFileName[_MAX_FNAME];
				TCHAR	sDirName[_MAX_DIR];	// 2003.06.18 add */
				_tsplitpath(fileMP3->strFullPathName, NULL, sDirName, sFileName, NULL); /* 2003.06.18 add */
	//			_tsplitpath(fileMP3->strFileName, NULL, NULL, sFileName, NULL); /* 2003.06.18 del */
	/* 2003.06.18 start */
				strFileName = sFileName;
				// �����Ƀf�B���N�g�����܂܂�Ă��邩�`�F�b�N���Ă��̊K�w�����̃f�B���N�g����ϊ����ɒǉ�����
				for (UINT nPos=0;nPos<lstrlen(sFormat);nPos++) {
					if (_ismbblead(sFormat[nPos])) {
						nPos++;
						continue;
					}
					if (sFormat[nPos] == '\\') {
						sDirName[lstrlen(sDirName)-1] = '\0';
						CString sPathName = sDirName;
						TCHAR	sExtName[_MAX_EXT];
						_tsplitpath(sPathName, NULL, sDirName, sFileName, sExtName); /* 2003.06.18 add */
						if (lstrlen(sFileName) == 0)	break;
						strFileName = CString(sFileName) + sExtName + "\\" + strFileName;
					}
				}
	//			char	*pFileName = &sFileName[0];
				/* STEP 034 *///char *pFileName = strFileName.GetBuffer(0);
			} else if (nType == 5) { /* STEP 034 */
				strFileName = sBaseTag;
			} else {
				ASSERT(0);
				return false;
			}
			pFileName = strFileName.GetBuffer(0);
/* 2003.06.18 end */
			while(*sFormat != '\0') {
				enum	{COLUMN_DUMMY = 99};
				int		nColumn = -1, nLen = 1;

				if (_tcsncicmp(sFormat, TEXT("%TRACK_NAME%"), 12) == 0) {
					nColumn = COLUMN_TRACK_NAME;
					nLen = 12;
				} else if (_tcsncicmp(sFormat, TEXT("%ALBUM_NAME%"), 12) == 0) {
					nColumn = COLUMN_ALBUM_NAME;
					nLen = 12;
				} else if (_tcsncicmp(sFormat, TEXT("%ARTIST_NAME%"), 13) == 0) {
					nColumn = COLUMN_ARTIST_NAME;
					nLen = 13;
				} else if (_tcsncicmp(sFormat, TEXT("%DISK_NUMBER%"), 13) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 14;
				} else if (_tcsncicmp(sFormat, TEXT("%DISK_NUMBER2%"), 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_tcsncicmp(sFormat, TEXT("%DISK_NUMBER3%"), 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER%"), 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 14;
				} else if (_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER2%"), 15) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER3%"), 15) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_tcsncicmp(sFormat, TEXT("%RELEASE_YEAR%"), 14) == 0) {
					nColumn = COLUMN_YEAR;
					nLen = 14;
				} else if (_tcsncicmp(sFormat, TEXT("%COMMENT%"), 9) == 0) {
					nColumn = COLUMN_COMMENT;
					nLen = 9;
				} else if (_tcsncicmp(sFormat, TEXT("%GENRE%"), 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_GENRE;
					nLen = 7;
				} else if (_tcsncicmp(sFormat, TEXT("%COPYRIGHT%"), 11) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COPYRIGHT;
					nLen = 11;
				} else if (_tcsncicmp(sFormat, TEXT("%ENGINEER%"), 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ENGINEER;
					nLen = 10;
				} else if (_tcsncicmp(sFormat, TEXT("%SOURCE%"), 8) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_SOURCE;
					nLen = 8;
				} else if (_tcsncicmp(sFormat, TEXT("%SOFTWARE%"), 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_SOFTWARE;
					nLen = 10;
				} else if (_tcsncicmp(sFormat, TEXT("%KEYWORD%"), 9) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_KEYWORD;
					nLen = 9;
				} else if (_tcsncicmp(sFormat, TEXT("%TECHNICIAN%"), 12) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_TECHNICIAN;
					nLen = 12;
				} else if (_tcsncicmp(sFormat, TEXT("%LYRIC%"), 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_LYRIC;
					nLen = 7;
				} else if (_tcsncicmp(sFormat, TEXT("%COMMISSION%"), 12) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COMMISSION;
					nLen = 12;
				} else if (_tcsncicmp(sFormat, TEXT("%WRITER%"), 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_WRITER;
					nLen = 10;
				} else if (_tcsncicmp(sFormat, TEXT("%COMPOSER%"), 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COMPOSER;
					nLen = 10;
				} else if (_tcsncicmp(sFormat, TEXT("%ALBM_ARTIST%"), 13) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ALBM_ARTIST;
					nLen = 13;
				} else if (_tcsncicmp(sFormat, TEXT("%ORIG_ARTIST%"), 13) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ORIG_ARTIST;
					nLen = 13;
				} else if (_tcsncicmp(sFormat, TEXT("%URL%"), 5) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_URL;
					nLen = 5;
				} else if (_tcsncicmp(sFormat, TEXT("%ENCODEST%"), 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ENCODEST;
					nLen = 10;
				} else if (_tcsncicmp(sFormat, TEXT("%OTHER%"), 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_OTHER;
					nLen = 7;
				} else if (_tcsncicmp(sFormat, TEXT("%SKIP%"), 6) == 0) {
					nColumn = COLUMN_DUMMY;
					nLen = 6;
				} else {
					if (*sFormat != *pFileName) break;	// �t�H�[�}�b�g�s��v
					pFileName++;
					sFormat++;
				}

				// �J�������X�V
				if (nColumn >= 0) {
					CString	strWord;
					if (sFormat[nLen] == '\0') {
						// �Ō�܂�
						strWord = pFileName;
					} else {
						// ���̋�؂蕶���܂�
						CString		strWork = &sFormat[nLen];
						// ���� % �܂ł���������擾
						auto sToken = _tcstok(strWork.GetBuffer(0), TEXT("%"));
						if (sToken == NULL) {		// �Ō�܂�
							strWord = pFileName;
						} else {					// ���̃L�[���[�h�܂�
							int		nLen = lstrlen(sToken);
							while(*pFileName != '\0' && _tcsncmp(pFileName, sToken, nLen) != 0) {
								strWord += *pFileName;
								if (_ismbblead(*pFileName)) { /* 2003.06.19 add */
									pFileName++;
									strWord += *pFileName;
								}
								pFileName++;
							}
						}
					}
					if (nColumn != COLUMN_DUMMY) {
//						CString	strText = GetFileColumnText(fileMP3, nColumn);
//						if (strText.IsEmpty()) {
						if (CFileMP3::IsNumeric(strWord)) { /* STEP 021 */
							if(_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER%"), 14) == 0) {
								strWord.Format(TEXT("%d"), _tstoi(strWord));
							}
							if(_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER2%"), 15) == 0) {
								strWord.Format(TEXT("%02d"), _tstoi(strWord));
							}
							if(_tcsncicmp(sFormat, TEXT("%TRACK_NUMBER3%"), 15) == 0) {
								strWord.Format(TEXT("%03d"), _tstoi(strWord));
							}
							if(_tcsncicmp(sFormat, TEXT("%DISK_NUMBER%"), 13) == 0) {
								strWord.Format(TEXT("%d"), _tstoi(strWord));
							}
							if(_tcsncicmp(sFormat, TEXT("%DISK_NUMBER2%"), 14) == 0) {
								strWord.Format(TEXT("%02d"), _tstoi(strWord));
							}
							if(_tcsncicmp(sFormat, TEXT("%DISK_NUMBER3%"), 14) == 0) {
								strWord.Format(TEXT("%03d"), _tstoi(strWord));
							}
						}
							ChangeSubItemText(nIndex, g_nColumnNumberList[nColumn], strWord);
//						}
					}
					sFormat += nLen;
				}
			}
			strFileName.ReleaseBuffer(); // 2003.06.18 add */
			InvalidateItemRect(nIndex);
		}
		break;
	}
	return(true);
}

LPCTSTR CMySuperGrid::GetSelectFileName(void)
{
	int		nIndex = GetSelectedItem();
	if (nIndex >= 0) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			return(m_pDoc->GetListMP3((int)pInfo->GetLParam())->strFullPathName);
		}
	}
	return(NULL);
}

bool CMySuperGrid::WritePlayList(CFile &file, CTreeItem *pItem, CString &strCurDir)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == TRUE) {
			// �t�@�C���̏ꍇ
			if (pInfo->GetCheck()) {
				CItemInfo	*pInfo = GetData(pItem);
				FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

				// �t�@�C�������o��
				CString	strBuffer;
				// ���΃p�X�Ή�
				strBuffer = ConvFileName(fileMP3->strFullPathName, strCurDir);
				strBuffer += "\r\n";
				file.Write(strBuffer, strBuffer.GetLength());
			}
		} else {
			// �t�H���_�̏ꍇ
			// �q�A�C�e����S�ď�������
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						WritePlayList(file, (CTreeItem *)GetNextChild(pSelItem, pos), strCurDir);
					}
				}
			}
		}
	}
	return(true);
}

bool CMySuperGrid::WritePlayList(LPCTSTR sFileName)
{
	return(WritePlayList(sFileName, GetRootHeadPosition()));
}

bool CMySuperGrid::WritePlayList(LPCTSTR sFileName, POSITION posRoot)
{
	TRY {
		// �o�̓t�@�C�����J��
		CFile	file(sFileName, CFile::modeCreate|CFile::modeWrite);

		// �v���C���X�g�t�@�C���̃J�����g�f�B���N�g�����擾
		CString	strCurDir;
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sFileName, drive, dir, NULL, NULL);
		strCurDir.Format(TEXT("%s%s"), drive, dir);

		// ���[�g�̃A�C�e����S�ď�������
		POSITION pos = posRoot;
		while(pos != NULL) {
			CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
			WritePlayList(file, pItem, strCurDir);
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format(TEXT("�t�@�C�����I�[�v���ł��܂���ł���\n���f���܂�\n\n%s"), sFileName);
		MessageBox(strMsg, TEXT("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return(true);
}

bool CMySuperGrid::WriteTreePlayList(LPCTSTR sFolder, CTreeItem *pItem, CString &strNameParent)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		if (pItem != m_pItemRoot) {
			// �v���C���X�g�o��
			CString	strFileName;
			strFileName.Format(TEXT("%s%s.m3u"), sFolder, strNameParent);
			WritePlayList(strFileName, GetHeadPosition(pItem));
		}

		// �T�u�t�H���_�̏���
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == FALSE) {
			// �t�H���_�̏ꍇ
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						CTreeItem	*pItem = (CTreeItem*)GetNextChild(pSelItem, pos);
						CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
						if (pInfo && IsItemFile(pItem) == FALSE && ItemHasChildren(pItem)) {
							// �t�H���_�̂ݏ�������
							CString		strName;
							if (strNameParent.IsEmpty()) strName.Format(TEXT("%s"), pInfo->GetItemText());
							else                         strName.Format(TEXT("%s - %s"), strNameParent, pInfo->GetItemText());
							strName = ReplaceFileName(NULL, strName);
							WriteTreePlayList(sFolder, pItem, strName);
						}
					}
				}
			}
		}
	}
	return(true);
}

bool CMySuperGrid::WriteTreePlayList(LPCTSTR sFolder)
{
	// �S�Ẵt�@�C�����o��
	CString	strFileName;
	strFileName.Format(TEXT("%sAllFiles.m3u"), sFolder);
	WritePlayList(strFileName);

	// ���[�g����������
	CString		strEmpty;
	WriteTreePlayList(sFolder, m_pItemRoot, strEmpty);

	return(true);
}

// =============================================
// CMySuperGrid::MakeSelectFileArray
// �T�v  : �I������Ă���t�@�C���̔z����쐬����
// ����  : array	= �z��(�o�͗p)
// �߂�l: int		= �L���Ȕz��̗v�f��
// =============================================
int CMySuperGrid::MakeSelectFileArray(CArray <int, const int &> &array)
{
	const int	nInitSize = 256;
	int		nCount;

	// �z��̏�����
	nCount = 0;
	array.RemoveAll();
	array.SetSize(nInitSize);

	if (GetSelectedCount() > 0) {
		if (GetSelectedCount() == 1) {
			// �P��I��
			nCount = AddSelectFileArray(GetSelectedItem(), array, nCount);
		} else {
			// �����I��
			// �����I���̏ꍇ�́A�S�Ă̑I������Ă���A�C�e������������
			// �������A�����Ă���t�H���_���̃t�@�C���͖�������
			int		nIndex = -1;
			while((nIndex = GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >=0) {
				if (IsItemFile(GetTreeItem(nIndex)) == TRUE) {
					// �t�@�C���̂ݏ�������
					nCount = AddSelectFileArray(nIndex, array, nCount);
				}
			}
		}
	}
	return(nCount);
}

int CMySuperGrid::MakeSelectFileArray(int nIndex, CArray <int, const int &> &array) /* STEP 008 */
{
	const int	nInitSize = 256;
	int		nCount;

	// �z��̏�����
	nCount = 0;
	array.RemoveAll();
	array.SetSize(nInitSize);

	if (nIndex > -1) {
		nCount = AddSelectFileArray(nIndex, array, nCount);
	}
	return(nCount);
}

// =============================================
// CMySuperGrid::AddSelectFileArray
// �T�v  : �I������Ă���t�@�C����z��ɒǉ�����
// ����  : nIndex		= �ǉ�����t�@�C���̃C���f�b�N�X
//       : array		= �z��
//       : nCount		= �z��̗L���ȗv�f��
// �߂�l: int			= �ǉ���̔z��̗L���ȗv�f��
// =============================================
int CMySuperGrid::AddSelectFileArray(int nIndex, CArray <int, const int &> &array, int nCount)
{
	const int	nAddSize = 256;

	// �͈͊O�G���[
	if (nIndex < 0) return(nCount);

	CTreeItem	*pItem = GetTreeItem(nIndex);
	if (ItemHasChildren(pItem) == FALSE) {
		if (IsItemFile(pItem)) {
			// �z����g�傷��K�v�����邩�H
			if (nCount > array.GetUpperBound()) {
				// �z�񂪑���Ȃ��ꍇ�́A�m�ۃT�C�Y�𑝉�������
				array.SetSize(array.GetUpperBound() + nAddSize);
			}

			// �z��ɒǉ�
			array[nCount] = nIndex;
			nCount++;
		}
	} else {
		POSITION pos = GetHeadPosition(pItem);
		// �m�[�h�����Ă���ꍇ�͊J��
		if(ItemHasChildren(pItem) && IsCollapsed(pItem)) {
			Expand(pItem, nIndex);
			InvalidateItemRect(nIndex);
		}
		// �q�A�C�e����S�ď�������
		while(pos != NULL) {
			CTreeItem *pChild = GetNextChild(pItem, pos);
			if (pChild != NULL) {
				nCount = AddSelectFileArray(NodeToIndex(pChild), array, nCount);
			}
		}
	}
	return(nCount);
}

void CMySuperGrid::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
}

// �A�C�e�����_�u���N���b�N���ꂽ
BOOL CMySuperGrid::OnItemDblClick(int nIndex)
{
	CSuperTagEditorView	*pView = (CSuperTagEditorView *)GetParent();

	if (pView != NULL) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
		if (pInfo != NULL) {
			pView->PlayFile((int)pInfo->GetLParam());
		}
	}
	return(TRUE);
}

// �`�F�b�N��Ԃ��ύX���ꂽ
void CMySuperGrid::OnChangeCheck(CTreeItem *pItem, BOOL bCheck)
{
	if (pItem != NULL && IsItemFile(pItem) == TRUE) {
		// �t�@�C���̂ݏ�������
		CItemInfo	*pInfo = GetData(pItem);
		if (pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			if (fileMP3) {
				// �`�F�b�N�t���O���X�V
				fileMP3->bCheckFlag = bCheck ? true : false;
			}
		}
	}
}

// =============================================
// CMySuperGrid::SetCheckAllFiles
// �T�v  : �S�Ẵt�@�C���̃`�F�b�N��Ԃ�ݒ肷��
// ����  : bCheck		= �`�F�b�N���
// �߂�l: none
// =============================================
void CMySuperGrid::SetCheckAllFiles(BOOL bCheck)
{
	// ���[�g�̃A�C�e����S�ď�������
	POSITION pos = GetRootHeadPosition();
	while(pos != NULL) {
		CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
		SetCheckEx(NodeToIndex(pItem), bCheck);
	}
}

// =============================================
// CMySuperGrid::SetCheckFileSelect
// �T�v  : �`�F�b�N����Ă���t�@�C����I����Ԃɂ���
// ����  : none
// �߂�l: none
// =============================================
void CMySuperGrid::SetCheckFileSelect(void)
{
	// �S�ẴA�C�e���̑I������
	SelectAllItems(FALSE);

	int		nIndex = -1;
	while((nIndex = GetNextItem(nIndex, LVNI_ALL)) >=0) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			// �t�@�C���̂ݏ�������
			CItemInfo *pInfo = GetData(pItem);
			if (pInfo && pInfo->GetCheck()) {
				// �I������
				//SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED); // �J�[�\���ʒu�͂��̂܂�
			}
		}
	}
	OnChangeSelect(); // http://bbs1.parks.jp/22/hasetaS/bbs.cgi?Action=Res&Mode=Tree&Base=94&Fx=1
}

// =============================================
// CMySuperGrid::SetSelectFileCheck
// �T�v  : �I������Ă���t�@�C���̃`�F�b�N��Ԃ�ݒ肷��
// ����  : bCheck		= �`�F�b�N���
// �߂�l: none
// =============================================
void CMySuperGrid::SetSelectFileCheck(BOOL bCheck)
{
	// �I������Ă���t�@�C����S�ď�������
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	int i; for (i = 0; i < nCount; i++) {
		SetCheckEx(GetTreeItem(arrayList[i]), bCheck);
	}
}

// =============================================
// CMySuperGrid::ReverseCheckFiles
// �T�v  : �S�Ẵt�@�C���̃`�F�b�N��Ԃ𔽓]����
// ����  : none
// �߂�l: none
// =============================================
void CMySuperGrid::ReverseCheckFiles(void)
{
	int		nIndex = -1;
	while((nIndex = GetNextItem(nIndex, LVNI_ALL)) >=0) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			// �t�@�C���̂ݏ�������
			CItemInfo *pInfo = GetData(pItem);
			if (pInfo) {
				// �`�F�b�N��Ԃ𔽓]
				SetCheckEx(pItem, pInfo->GetCheck() ? FALSE : TRUE);
			}
		}
	}
}

// =============================================
// CMySuperGrid::CheckFileAtColumnState
// �T�v  : �^�O���̓��e�Ń`�F�b�N����
// ����  : chkWord		= ����
// �߂�l: none
// =============================================
void CMySuperGrid::CheckFileAtColumnState(CHECK_WORD_STATE *chkWord)
{
	// ���K�\���̃R���p�C��
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// �R���p�C���G���[
			MessageBox(TEXT("���K�\���Ɏw�肳��Ă��镶������A�������R���p�C��\n�o���܂���ł���\n\n���K�\���̋L�q���m�F���Ă�������"), TEXT("�R���p�C���G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return;
		}
	}

	// �����J�n�ʒu��������
	chkWord->nMatchPos = 0;
	chkWord->nMatchLength = 0;

	if (chkWord->bRangeSelected) {
		// �I������Ă���t�@�C����S�ď�������
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = MakeSelectFileArray(arrayList);
		int i; for (i = 0; i < nCount; i++) {
			CheckFileAtColumnState(chkWord, GetTreeItem(arrayList[i]));
		}
	} else {
		// ���[�g�̃A�C�e����S�ď�������
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL) {
			CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
			CheckFileAtColumnState(chkWord, pItem);
		}
	}
}

// =============================================
// CMySuperGrid::CheckFileAtColumnState
// �T�v  : �^�O���̓��e�Ń`�F�b�N����(�A�C�e���P�ʂ̏���)
// ����  : chkWord		= ����
//         pItem		= ��������A�C�e��
// �߂�l: none
// =============================================
void CMySuperGrid::CheckFileAtColumnState(CHECK_WORD_STATE *chkWord, CTreeItem *pItem)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		if (IsItemFile(pItem) == TRUE) {
			// �t�@�C���̏ꍇ
			chkWord->nMatchPos = 0; /* STEP 002 */
			if (IsMatchItem(chkWord, pItem)) {
				int		nIndex;
				if ((nIndex = NodeToIndex(pItem)) != -1) {
					if (chkWord->bRangeSelected == FALSE
					&&  chkWord->bMatchSelected == TRUE) {
						// �I������
						SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
						OnChangeSelect();			// �I�����ύX���ꂽ���Ƃ�ʒm
					} else {
						// �`�F�b�N����
						SetCheckEx(nIndex, TRUE);
					}
				}
			}
		} else {
			// �t�H���_�̏ꍇ
			// �q�A�C�e����S�ď�������
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						CheckFileAtColumnState(chkWord, (CTreeItem *)GetNextChild(pSelItem, pos));
					}
				}
			}
		}
	}
}

void CMySuperGrid::ReplaceMatchItem(CHECK_WORD_STATE *chkWord, bool bRepAll)
{
	// ���K�\���̃R���p�C��
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// �R���p�C���G���[
			MessageBox(TEXT("���K�\���Ɏw�肳��Ă��镶������A�������R���p�C��\n�o���܂���ł���\n\n���K�\���̋L�q���m�F���Ă�������"), TEXT("�R���p�C���G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return;
		}
	}

	if (bRepAll) {
		// �S�ā^�I��͈͂�S�Ēu��
		if (chkWord->bRangeSelected) {
			// �I������Ă���t�@�C����S�ď�������
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				ReplaceMatchItem(chkWord, GetTreeItem(arrayList[i]));
			}
		} else {
			// ���[�g�̃A�C�e����S�ď�������
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				ReplaceMatchItem(chkWord, pItem);
			}
		}
	} else {
		// ���݂̍s�̂ݒu��
		int		nIndex = GetSelectedItem();
		if (nIndex != -1) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			if (pItem != NULL) {
				ReplaceMatchItem(chkWord, pItem);
			}
		}
	}
}

void CMySuperGrid::ReplaceMatchItem(CHECK_WORD_STATE *chkWord, CTreeItem *pItem)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		if (IsItemFile(pItem) == TRUE) {
			// �t�@�C���̏ꍇ
			int		nRetryMax = 128;
			// �����J�n�ʒu��������
			chkWord->nMatchPos = 0;
			chkWord->nMatchLength = 0;
			while(IsMatchItem(chkWord, pItem) && --nRetryMax) {
				int		nIndex;
				if ((nIndex = NodeToIndex(pItem)) != -1) {
					CItemInfo *pInfo = GetData(pItem);
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
					CString	strText;

					// �Z���̃e�L�X�g���擾
					strText = GetFileColumnText(fileMP3, chkWord->nMatchColumn);

					// �Ώۂ�[�t�@�C����]�H
					if (chkWord->nMatchColumn == COLUMN_FILE_NAME) {
						// �g���q�����͒u���̑Ώۂɂ��Ȃ�
						if (chkWord->nMatchPos >= strText.GetLength()-4) break;
					}

					CString	strNew;
					strNew = strText.Left(chkWord->nMatchPos)
					       + chkWord->strReplaceWord
					       + strText.Right(strText.GetLength() - (chkWord->nMatchPos+chkWord->nMatchLength));
					ChangeSubItemText(nIndex, g_nColumnNumberList[chkWord->nMatchColumn], strNew);
					if (strNew == strText) {
						// �������[�v���p
						break;
					}

					// �u����̕��������̒u���̑Ώۂɂ��Ȃ��悤�ɂ��鏈��
					chkWord->nMatchPos += chkWord->strReplaceWord.GetLength();
				}
			}
		} else {
			// �t�H���_�̏ꍇ
			// �q�A�C�e����S�ď�������
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						ReplaceMatchItem(chkWord, (CTreeItem *)GetNextChild(pSelItem, pos));
					}
				}
			}
		}
	}
}

// =============================================
// CMySuperGrid::IsMatchItem
// �T�v  : �A�C�e���������Ɉ�v���邩�ǂ����𒲂ׂ�
// ����  : chkWord		= ����
//         pItem		= �`�F�b�N����A�C�e��
// �߂�l: bool			= true:��v / false:�s��v
// =============================================
bool CMySuperGrid::IsMatchItem(CHECK_WORD_STATE *chkWord, CTreeItem *pItem)
{
	bool	bMatch = false;

	ASSERT(pItem != NULL);
	ASSERT(IsItemFile(pItem) == TRUE);

	if (pItem != NULL && IsItemFile(pItem) == TRUE) {
		int		nTargetColumn;
		int		nStart, nEnd;
		if (chkWord->nTargetColumn == -1) {
			nStart = COLUMN_TRACK_NAME;
			nEnd = COLUMN_EDIT_MAX;/*COLUMN_COMMISSION;*//* Baja 154 */
		} else {
			nStart = nEnd = chkWord->nTargetColumn;
		}

		int		nIndex = NodeToIndex(pItem);
		CItemInfo *pInfo = GetData(pItem);
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
		for (nTargetColumn = nStart; nTargetColumn <= nEnd; nTargetColumn++) {
			enum	{TYPE_NUMERIC, TYPE_STRING};
			int		nType = TYPE_STRING;
			CString	strText;
			UINT	dwData = 0;

			// �Z���̃e�L�X�g���擾
			switch(nTargetColumn) {
			case COLUMN_TRACK_NUMBER:	// �g���b�N�ԍ�
				strText = fileMP3->strTrackNumber;
				break;
			case COLUMN_DISK_NUMBER:	// �f�B�X�N�ԍ�
				strText = fileMP3->strDiskNumber;
				break;
			case COLUMN_GENRE:			// �W�������ԍ�
				strText = fileMP3->strGenre;
				break;
			case COLUMN_FULL_PATH_NAME:
				strText = fileMP3->strFullPathName;
				break;
			default:					// ���̑�
				// �Z���̃e�L�X�g���擾
				strText = GetFileColumnText(fileMP3, nTargetColumn);
				break;
			}

			int		nMatchPos = 0, nMatchLen = 0;
			if (nType == TYPE_STRING) {
				if (chkWord->bRegExp == FALSE) {
					// �ʏ팟��
					int		nCount = strText.GetLength() - chkWord->strSearchWord.GetLength();
					if (nCount >= 0) {
						int		nPos;
						int		nLength = chkWord->strSearchWord.GetLength();
						if (chkWord->strSearchWord.GetLength() == 1) {
							TCHAR	chSearch = chkWord->strSearchWord.GetAt(0);
							for (nPos = chkWord->nMatchPos; nPos <= nCount; nPos++) {
								if (iskanji(strText.GetAt(nPos))) {
									// �����̓X�L�b�v
									nPos++;
								} else {
									// ASCII����
									if (chkWord->bCheckDiffUL == TRUE) {
										// �啶���^�����������
										if (strText.GetAt(nPos) == chSearch) {
											// ��v
											nMatchPos = nPos;
											nMatchLen = nLength;
											break;
										}
									} else {
										// �啶���^�������𓯈ꎋ
										if (toupper(strText.GetAt(nPos)) == toupper(chSearch)) {
											// ��v
											nMatchPos = nPos;
											nMatchLen = nLength;
											break;
										}
									}
								}
							}
						} else {
							for (nPos = chkWord->nMatchPos; nPos <= nCount; nPos++) {
								if (StringComp(((LPCTSTR)strText)+nPos, chkWord->strSearchWord, nLength, chkWord->bCheckDiffUL)) {
									// ��v
									nMatchPos = nPos;
									nMatchLen = nLength;
									break;
								}
								if (iskanji(strText.GetAt(nPos))) { /* RockDance 132 */
									// �����̓X�L�b�v
									nPos++;
								}
							}
						}
					}
				} else {
					// ���K�\��
					if ((nMatchPos = chkWord->regexp.RegFind(strText.GetBuffer(0)+chkWord->nMatchPos)) >= 0) {
						nMatchPos += chkWord->nMatchPos;
						nMatchLen = chkWord->regexp.GetFindLen();
					}
				}
				if (chkWord->bMatchComplete) {
					// ���S��v
					bMatch = (nMatchPos == 0 && nMatchLen != 0 && nMatchLen == strText.GetLength()) ? true : false;
				} else {
					// ������v
					bMatch = nMatchLen ? true : false;
				}
			} else if (nType == TYPE_NUMERIC) {
				bMatch = ((BYTE)_tstoi(chkWord->strSearchWord) == (BYTE)dwData) ? true : false;
			}
			if (bMatch == true) {
				chkWord->nMatchColumn = nTargetColumn;
				chkWord->nMatchPos = nMatchPos;
				chkWord->nMatchLength = nMatchLen;
				break;
			}
		}
	}
	return(bMatch);
}

int CMySuperGrid::FindNextMatchItem(int nStart, CHECK_WORD_STATE *chkWord, bool bNext)
{
	// ���K�\���̃R���p�C��
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// �R���p�C���G���[
			MessageBox(TEXT("���K�\���Ɏw�肳��Ă��镶������A�������R���p�C��\n�o���܂���ł���\n\n���K�\���̋L�q���m�F���Ă�������"), TEXT("�R���p�C���G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(-1);
		}
	}

	// �����J�n�ʒu��������
	chkWord->nMatchPos = 0;
	chkWord->nMatchLength = 0;

	int		nIndex = nStart;
	int		nFlag;
	nFlag = chkWord->bRangeSelected ? LVNI_SELECTED : 0;
	nFlag |= bNext ? LVNI_BELOW : LVNI_ABOVE;
	while((nIndex = GetNextItem(nIndex, nFlag)) >= 0) {
		if (nIndex != nStart) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			if (pItem != NULL && IsItemFile(pItem) == TRUE) {
				// �t�@�C���̂ݏ�������
				if (IsMatchItem(chkWord, pItem)) {
					// ��v����
					return(nIndex);
				}
			}
		}
	}
	return(-1);
}

void CMySuperGrid::SelectAndVisibleColumn(int nIndex, int nColType)
{
	if (nIndex != -1) {
		// �s�̑I��ύX
		SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		EnsureVisible(nIndex, FALSE);

		int		iSubItem = g_nColumnNumberList[nColType];
		if (iSubItem != -1) {
			// ���̑I��ύX
			SetCurSubItem(iSubItem);
			MakeColumnVisible(iSubItem);
		}
		OnChangeSelect();			// �I�����ύX���ꂽ���Ƃ�ʒm
	}
}

bool CMySuperGrid::ConvUserFormatEx(USER_CONV_FORMAT_EX *pForm)
{
	if (pForm->strFormat.IsEmpty()) {
		// �ϊ���������
		return(false);					// �G���[
	}

	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

	// �Œ蕶�������͂�����
	if (pForm->strFormat.Find(TEXT("%STRING%")) != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// �L�����Z��
			return(false);
		}
		pForm->strFixString = dialog.m_strText;
	}

	// �A�ԕϊ������̐ݒ�
	TCHAR	sNumFormat[20] = {0};
	_stprintf_s(sNumFormat, 20, TEXT("%s0%dd"), TEXT("%"), pForm->nColumnCount);

	int		nNumber = pForm->nInitNumber;
	int		fileCount = 0;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			// �t�@�C��������g���q���������������擾
			CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
			TCHAR	sFileName[_MAX_FNAME];
			_tsplitpath(strFileName, NULL, NULL, sFileName, NULL);

			CString	strText = pForm->strFormat;
			CString	strNumber;
			auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));
			auto[strDiskNumber, strDiskNumber2, strDiskNumber3] = GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding(TEXT("\r")));

			// �A��
			strNumber.Format(sNumFormat, nNumber);

			// �������g���ĕ�������쐬
			strText = StrReplace(strText, TEXT("%FILE_NAME%")    , sFileName);
			strText = StrReplace(strText, TEXT("%TRACK_NAME%")   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding(TEXT("\r")));
			strText = StrReplace(strText, TEXT("%ALBUM_NAME%")   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding(TEXT("\r")));
			strText = StrReplace(strText, TEXT("%ARTIST_NAME%")  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding(TEXT("\r")));
			strText = StrReplace(strText, TEXT("%TRACK_NUMBER%") , strTrackNumber);
			strText = StrReplace(strText, TEXT("%TRACK_NUMBER2%"), strTrackNumber2);
			strText = StrReplace(strText, TEXT("%TRACK_NUMBER3%"), strTrackNumber3);
			strText = StrReplace(strText, TEXT("%DISK_NUMBER%") , strDiskNumber);
			strText = StrReplace(strText, TEXT("%DISK_NUMBER2%"), strDiskNumber2);
			strText = StrReplace(strText, TEXT("%DISK_NUMBER3%"), strDiskNumber3);
			strText = StrReplace(strText, TEXT("%RELEASE_YEAR%") , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding(TEXT("\r")));
			strText = StrReplace(strText, TEXT("%COMMENT%")      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding(TEXT("\r"))/* BeachMonster 089 */);
			strText = StrReplace(strText, TEXT("%GENRE%")        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding(TEXT("\r")));
			strText = StrReplace(strText, TEXT("%NUMBER%")       , strNumber);
			strText = StrReplace(strText, TEXT("%STRING%")       , pForm->strFixString);
			// SIF �̍���
			StrReplaceSIF(strText, fileMP3);
			/* STEP 007 */
			if (fileCount == 0) {
				fileCount = GetFolderFileCount(NodeToIndex(GetParentItem(pItem)));
			}
			CString strFileCount;
			strFileCount.Format(sNumFormat, fileCount);
			strText = StrReplace(strText, TEXT("%FOLDER_FILE_COUNT%")	, strFileCount);

			// �Z���ɍ쐬�����������ݒ�
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				ChangeSubItemText(nItem, nColumn, strText);
				InvalidateItemRect(nItem);
			}

			// �A�Ԃ�i�߂�
			nNumber += pForm->nAddNumber;
		} else {
			// �������݂ł���Z�����r�؂ꂽ��A�Ԃ�������
			if (pForm->bSpaceInitNumber) {
				nNumber = pForm->nInitNumber;
			}
			fileCount = GetFolderFileCount(NodeToIndex(pItem)); /* STEP 0007 */
		}
	}
	return(true);
}

bool CMySuperGrid::ConvString(int nType)
{
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				CString	strText;
				TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
				bool	bKeepExt = false;

				// �Z���̃e�L�X�g���擾
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// �t�@�C�����̏ꍇ�A�g���q���S�p�ɕϊ�����Ȃ��悤�ɂ���
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					switch(nType) {
					case CONV_STR_HIRA:			// �S�p���Ȃɕϊ�
					case CONV_STR_KATA:			// �S�p�J�i�ɕϊ�
					case CONV_STR_HAN_ALL:		// �S�p => ���p(�S��)�ϊ�
					case CONV_STR_HAN_KIGOU:	// �S�p => ���p(�L��)�ϊ�
					case CONV_STR_HAN_SUJI:		// �S�p => ���p(����)�ϊ�
					case CONV_STR_HAN_KATA:		// �S�p => ���p(�J�^�J�i)�ϊ�
					case CONV_STR_HAN_ALPHA:	// �S�p => ���p(�A���t�@�x�b�g)�ϊ�
					case CONV_STR_ZEN_ALL:		// ���p => �S�p(�S��)�ϊ�
					case CONV_STR_ZEN_KIGOU:	// ���p => �S�p(�L��)�ϊ�
					case CONV_STR_ZEN_SUJI:		// ���p => �S�p(����)�ϊ�
					case CONV_STR_ZEN_KATA:		// ���p => �S�p(�J�^�J�i)�ϊ�
					case CONV_STR_ZEN_ALPHA:	// ���p => �S�p(�A���t�@�x�b�g)�ϊ�
					default:
						_tsplitpath(strText, NULL, NULL, fname, ext);
						extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
						if (/*strlen(ext) == 4 && */CheckFileExt(fileMP3, ext)) {
						/*
						&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
							 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
							 strcmpi(ext, ".asf") == 0 ||
							 strcmpi(ext, ".ape") == 0 ||
							 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
							 strcmpi(ext, ".ogg") == 0)) {
						*/
							// �g���q����菜��
							strText = fname;
							bKeepExt = true;
						}
						break;
					}
				}

				if (strText.GetLength() > 0) {
					// �ϊ�
					std::vector<TCHAR> buffer(strText.GetLength()*2+2);
					switch(nType) {
					case CONV_STR_UPPER:		// �啶���ɕϊ�
						conv_upper(strText.GetBuffer(0));
						break;
					case CONV_STR_LOWER:		// �������ɕϊ�
						conv_lower(strText.GetBuffer(0));
						break;
					case CONV_STR_HANKAKU:		// ���p�ɕϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_ALL);
						strText = buffer.data();
						break;
					case CONV_STR_HIRA:			// �S�p���Ȃɕϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_ALL);
						conv_kata2hira(buffer.data());
						strText = buffer.data();
						break;
					case CONV_STR_KATA:			// �S�p�J�i�ɕϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_ALL);
						conv_hira2kata(buffer.data());
						strText = buffer.data();
						break;


					case CONV_STR_HAN_ALL:		// �S�p => ���p(�S��)�ϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_ALL);
						strText = buffer.data();
						break;
					case CONV_STR_HAN_KIGOU:	// �S�p => ���p(�L��)�ϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_KIGOU);
						strText = buffer.data();
						break;
					case CONV_STR_HAN_SUJI:		// �S�p => ���p(����)�ϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_SUJI);
						strText = buffer.data();
						break;
					case CONV_STR_HAN_KATA:		// �S�p => ���p(�J�^�J�i)�ϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_KATA);
						strText = buffer.data();
						break;
					case CONV_STR_HAN_ALPHA:	// �S�p => ���p(�A���t�@�x�b�g)�ϊ�
						conv_zen2hans(buffer.data(), strText.GetBuffer(0), CONV_ALPHA);
						strText = buffer.data();
						break;
					case CONV_STR_ZEN_ALL:		// ���p => �S�p(�S��)�ϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_ALL);
						strText = buffer.data();
						break;
					case CONV_STR_ZEN_KIGOU:	// ���p => �S�p(�L��)�ϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_KIGOU);
						strText = buffer.data();
						break;
					case CONV_STR_ZEN_SUJI:		// ���p => �S�p(����)�ϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_SUJI);
						strText = buffer.data();
						break;
					case CONV_STR_ZEN_KATA:		// ���p => �S�p(�J�^�J�i)�ϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_KATA);
						strText = buffer.data();
						break;
					case CONV_STR_ZEN_ALPHA:	// ���p => �S�p(�A���t�@�x�b�g)�ϊ�
						conv_han2zens(buffer.data(), strText.GetBuffer(0), CONV_ALPHA);
						strText = buffer.data();
						break;
					case CONV_STR_TO_UPPER:		// ������ => �啶���ϊ�
						conv_upper(strText.GetBuffer(0));
						break;
					case CONV_STR_TO_LOWER:		// �啶�� => �������ϊ�
						conv_lower(strText.GetBuffer(0));
						break;
					case CONV_STR_FIRST_UPPER:	// �P��̂P�����ڂ̂ݑ啶��
						conv_lower(strText.GetBuffer(0));
						conv_first_upper(strText.GetBuffer(0),/* STEP 026 */ g_strFirstUpperIgnoreWords, g_strFirstUpperSentenceSeparator, g_bFirstUpperIgnoreWord);
						break;
					case CONV_STR_FIXED_UP_LOW:	// �啶���������Œ� /* STEP 040 */
						conv_fixed_upper_lower(strText.GetBuffer(0), g_arFixedWords);
						break;
					case CONV_STR_HIRA2KATA:	// �Ђ炪�� => �J�^�J�i�ϊ�
						conv_hira2kata(strText.GetBuffer(0));
						break;
					case CONV_STR_KATA2HIRA:	// �J�^�J�i => �Ђ炪�ȕϊ�
						conv_kata2hira(strText.GetBuffer(0));
						break;
					}

					// ��菜���Ă����g���q��t���Ȃ���
					if (bKeepExt) {
						strText += ext;
					}

					ChangeSubItemText(nItem, nColumn, strText);
					InvalidateItemRect(nItem);
				}
			}
		}
	}
	return(true);
}


bool CMySuperGrid::GetSelectedRange(int &sx, int &sy, int &ex, int &ey)
{
	if (IsRangeSelected() == false) {
		if (GetSelectedItem() == -1) return(false);	// �G���[
		if (GetCurSubItem() < COLUMN_MIN) return(false);		// �G���[
		// �͈͑I�𖳂�
		sx = ex = GetCurSubItem();
		sy = ey = GetSelectedItem();
	} else {
		// �͈͑I������
		int		nTemp;
		sx = m_posMultiSelect[0].x;
		sy = m_posMultiSelect[0].y;
		ex = m_posMultiSelect[1].x;
		ey = m_posMultiSelect[1].y;
		if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
		if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}
		if (sx <= COLUMN_MIN) sx = COLUMN_MIN;
	}
	return(true);
}

void CMySuperGrid::ExecFolderTreeSync(LPCTSTR sRootFolder, bool bCheckOnly)
{
	CString	strFolder;
	int		nLen = lstrlen(sRootFolder);
	if (nLen == 0) return;

	// �t�H���_���̍Ō��K�� '\\' �ɂ���
	strFolder = sRootFolder;
	if (IsFolderName(sRootFolder) == false) strFolder += '\\';

	// �v���O���X�o�[������
	m_pDoc->StartLoadFile(TEXT("�t�H���_����������..."));
	m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, TEXT("���΂炭���҂�������..."));

	// �ċN�^�̃t�H���_�̓����������s��
	m_nFolderSyncCounter = 0;
	ExecFolderTreeSync(strFolder, m_pItemRoot, bCheckOnly, true);

	// �v���O���X�o�[�I��
	m_pDoc->EndLoadFile();
}

bool CMySuperGrid::CheckExist(LPCTSTR sTarget)
{
	bool	bReturn = false;
//	int		nLen = strlen(sTarget);

	if ((sTarget[0] == '\\' && sTarget[1] == '\\')
	&&  _mbschr((const unsigned char *)(sTarget+2), '\\') == NULL) {
		// "\\�l�b�g���[�N��" �݂̂̏ꍇ�͏�ɐ^
		bReturn = true;
	} else {
		DWORD	dwAttr;
		DWORD	dwLastError;
		dwAttr = GetFileAttributes(sTarget);
		dwLastError = GetLastError();
		if (dwAttr != 0xFFFFFFFF) {
			// �t�@�C�������݂���
			bReturn = true;
		} else {
			// �G���[���e�𒲂ׂ�
			bReturn = false;
			switch(dwLastError) {
			case ERROR_SUCCESS:				// ����(�L�蓾�Ȃ��Ǝv�����ǈꉞ�c)
			case ERROR_FILE_NOT_FOUND:		// �t�@�C��������
			case ERROR_PATH_NOT_FOUND:		// �p�X������
				// �{���Ƀt�@�C������������
				break;
			default:
				{	// �l�b�g���[�N�ؒf�Ȃǂ̒v���I�ȃG���[������
					CString	strBuffer;
					LPVOID lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								  FORMAT_MESSAGE_FROM_SYSTEM |
								  FORMAT_MESSAGE_IGNORE_INSERTS,
								  NULL, dwLastError,
								  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
								  (LPTSTR)&lpMsgBuf, 0, NULL);
					strBuffer.Format(TEXT("�t�@�C���̑��݃`�F�b�N�Ɏ��s���܂����B\n�v���I�ȃG���[�������ł���\��������̂ŁA\n�����𒆒f���܂�\n\n�Ώۃt�@�C���F%s\n\n%s"), sTarget, (LPCTSTR)lpMsgBuf);
					LocalFree(lpMsgBuf);
					MessageBox(strBuffer, TEXT("�v���I�G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				}
				break;
			}
		}
	}
	return(bReturn);
}

// =============================================
// CMySuperGrid::CheckFileName
// �T�v  : �L���ȃt�@�C�������ǂ����𒲂ׂ�
// ����  : sTarget		= �t�@�C����
// �߂�l: bool			= true:�L�� / false:����(�s���ȃt�@�C����)
// =============================================
bool CMySuperGrid::CheckFileName(LPCTSTR sTarget)
{
	if ((sTarget[0] == '\\' && sTarget[1] == '\\')
	&&  _mbschr((const unsigned char *)(sTarget+2), '\\') == NULL) {
		// "\\�l�b�g���[�N��" �݂̂̏ꍇ�͏�ɐ^
		return(true);
	}

	if (GetFileAttributes(sTarget) == 0xFFFFFFFF) {
		// �G���[���e�𒲂ׂ�
		if (GetLastError() == ERROR_INVALID_NAME) {
			// �s���ȃt�@�C�����A�t�H���_���A�{�����[�����x����
			CString	strBuffer;
			strBuffer.Format(TEXT("�t�@�C�����^�t�H���_���ɕs���ȕ������܂܂�Ă��܂�\n�Ώۃt�@�C���F%s"), sTarget);
			MessageBox(strBuffer, TEXT("�v���I�G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
	}
	return(true);
}

void CMySuperGrid::ChangeFileAttr(LPCTSTR sFileName, DWORD dwAttr)
{
	DWORD	dwAttrNow;
	if ((dwAttrNow = GetFileAttributes((LPTSTR)sFileName)) != (DWORD)-1L) {
		if (dwAttrNow != dwAttr) {
			SetFileAttributes((LPTSTR)sFileName, dwAttr);
		}
	}
}

bool CMySuperGrid::DirectoryMake(CString &strDir)
{
	LPTSTR sTemp;
	int		i, nLen = strDir.GetLength();

	// �K���h���C�u�����܂ސ�΃p�X�Ŏw�肷�邱��
	if (nLen <= 3) return(false);	// �p�X���Z������!
//	if (sDir[1] != ':' || sDir[2] != '\\') return(false);

	sTemp = strDir.GetBuffer(strDir.GetLength());
	// i = 3 �ŏ��������Ă���̂́A�ŏ���'\\'(ٰ��ިڸ��)���΂���
	for (i = 3; i <= nLen; i++) {
		auto	c = sTemp[i];
		if (iskanji(c)) {
			i++;
			continue;
		}
		if (c == '\\' || i == nLen) {
			sTemp[i] = NULL;		// '\\' �� '\0' �ɒu��������
			if (!CheckExist(sTemp)) {
				// �f�B���N�g�������݂��Ȃ�
RETRY:
				if (CreateDirectory(sTemp, NULL) == FALSE) {
					CString	strBuffer;
					LPVOID lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								  FORMAT_MESSAGE_FROM_SYSTEM |
								  FORMAT_MESSAGE_IGNORE_INSERTS,
								  NULL, GetLastError(),
								  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
								  (LPTSTR)&lpMsgBuf, 0, NULL);
					strBuffer.Format(TEXT("%s\n\n%s"), sTemp, (const char *)lpMsgBuf);
					LocalFree(lpMsgBuf);
					switch(MessageBox(strBuffer, TEXT("�t�H���_�̍쐬�Ɏ��s"), MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST)) {
					case IDRETRY:		// �Ď��s
						goto RETRY;
					case IDABORT:		// ���~
						break;
					case IDIGNORE:		// ����
						break;
					}
					return(false);	// �G���[
				}
			}
			if (i != nLen) sTemp[i] = '\\';	// '\0' �� '\\' �ɖ߂�
		}
	}
	return(true);
}

bool CMySuperGrid::DirectoryRemove(CString &strDir)
{
	// �폜����f�B���N�g���̑������N���A
	ChangeFileAttr(strDir, CFile::normal);

	LPTSTR buffer;
	buffer = strDir.GetBuffer(strDir.GetLength());
	if (RemoveDirectory(buffer) == FALSE) {
//		CString	strBuffer;
//		LPVOID lpMsgBuf;
//		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
//					  FORMAT_MESSAGE_FROM_SYSTEM |
//					  FORMAT_MESSAGE_IGNORE_INSERTS,
//					  NULL, GetLastError(),
//					  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
//					  (LPTSTR)&lpMsgBuf, 0, NULL);
//		strBuffer.Format("%s\n\n%s", buffer, (const char *)lpMsgBuf);
//		LocalFree(lpMsgBuf);
//		switch(MessageBox(strBuffer, "�t�H���_�̍폜�Ɏ��s", MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST)) {
//		case IDRETRY:				// �Ď��s
//			goto RETRY;
//		case IDABORT:				// ���~
//			break;
//		case IDIGNORE:				// ����
//			break;
//		}
		return(false);	// �G���[
	}
	return(true);
}

void CMySuperGrid::ExecFolderTreeSync(LPCTSTR sFolder, CTreeItem *pItem, bool bCheckOnly, bool bIsRoot)
{
	ASSERT(pItem != NULL);

	// �q�A�C�e�������t�H���_�ȊO�̏ꍇ�͉��������ɏI��
	if (pItem == NULL) return;
	if (IsItemFile(pItem) == TRUE) return;
	if (ItemHasChildren(pItem) == FALSE) return;

	// �t�H���_�̏ꍇ
	CString	strFolder;
	bool	bMakeDir = true;

	// �t�H���_���쐬����
	if (bIsRoot == false) {
		CItemInfo	*lp = GetData(pItem);
		CString		strText = lp->GetItemText();
		// �t�@�C�����Ɏg�p�ł��Ȃ�������u��
		strText = ReplaceFileName(NULL, strText);
		// �Ōオ '.' �ɂȂ��Ă���ꍇ�́A'.' ���폜����
		// ��Windows �ł͍Ōオ '.' �̃t�H���_�͍쐬�ł��Ȃ�����
		while(strText.GetLength() > 0) {
			if (strText[strText.GetLength()-1] != '.') break;
			strText = strText.Left(strText.GetLength()-1);
		}
		strFolder.Format(TEXT("%s%s\\"), sFolder, strText);
	} else {
		strFolder = sFolder;
	}

	// �q�A�C�e����S�ď�������
	int		nIndex = NodeToIndex(pItem);
	POSITION pos = GetHeadPosition(pItem);
	while(pos != NULL) {
		CTreeItem	*pChild = (CTreeItem *)GetNextChild(pItem, pos);
		if (pChild != NULL) {
			if (IsItemFile(pChild) == FALSE) {
				// �t�H���_(�ċA�Ăяo��)
				ExecFolderTreeSync(strFolder, pChild, bCheckOnly);
			} else {
				// �t�@�C��
				// �v���O���X�o�[�X�V
				if (m_pDoc->GetArrayFileCount() > 0) {
					m_nFolderSyncCounter++;
					m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(m_nFolderSyncCounter * 100 / m_pDoc->GetArrayFileCount());
					CString	strMessage;
					strMessage.Format(TEXT("������...(%d/%d)"), m_nFolderSyncCounter, m_pDoc->GetArrayFileCount());
					m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, strMessage);
				}

				CItemInfo *pInfo = GetData(pChild);
				if (bCheckOnly == false || pInfo->GetCheck()) {
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

					// �f�B���N�g�����쐬����
					if (bMakeDir == true) {
						DirectoryMake(strFolder);
						bMakeDir = false;
					}

					// �t�@�C�����ړ�����
					CString		strOldFolder = fileMP3->strFilePath;
					if (CFileMP3::CopyFile(fileMP3, strFolder, true)) {
						// �ړ��ɐ��������ꍇ�́A�t�H���_����ɂȂ��Ă�����t�H���_���폜����
						if (g_bSyncDeleteFolder) {
							while(DirectoryRemove(strOldFolder)) {
								// �Ō�� '\\' ������
								strOldFolder = strOldFolder.Left(strOldFolder.GetLength()-1);
								// �P��̃f�B���N�g�������쐬
								int		nPos;
								nPos = strOldFolder.ReverseFind('\\');
								if (nPos <= 0) break;
								strOldFolder = strOldFolder.Left(nPos);
							}
						}
					}
				}
			}
		}
	}
}

bool CMySuperGrid::WriteFormatFileBody(CFile &file, CTreeItem *pItem, const CString &strBody, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml /* BeachMonster5 120 */)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == TRUE) {
			// �t�@�C���̏ꍇ
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			CString strText = MakeFormatFileBody(fileMP3, strBody, bIsHtml, pStatus, bWriteHtml); /* Rumble 190 */

			// �P�t�@�C�������o��
			file.Write(strText, strText.GetLength());
		} else {
			// �t�H���_�̏ꍇ
			// �q�A�C�e����S�ď�������
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						WriteFormatFileBody(file, (CTreeItem *)GetNextChild(pSelItem, pos), strBody, bIsHtml, pStatus, bWriteHtml);
					}
				}
			}
		}
	}

	return(true);
}

bool CMySuperGrid::WriteFormatFileFoot(CFile &file, const CString &strFoot, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml)
{
	CString strText = MakeFormatFileFoot(strFoot, bIsHtml, pStatus, bWriteHtml); /* Rumble 190 */

	// �o��
	file.Write(strText, strText.GetLength());

	return(true);
}

bool CMySuperGrid::WriteFormatFile(LPCTSTR sFileName, const CString &strHead, const CString &strBody, const CString &strFoot, bool bSelected, bool bIsHtml, bool bIsWriteAppend, bool bWriteHtml)
{
	TRY {
		// �o�̓t�@�C�����J��
		UINT	iOpenMode = bIsWriteAppend ? (CFile::modeWrite) : (CFile::modeCreate|CFile::modeWrite);
		CFile	file(sFileName, iOpenMode);

		// �ǉ��̏ꍇ�̓t�@�C���̍Ō�Ɉړ�
		if (bIsWriteAppend) file.Seek(0, CFile::end);

		// ���X�g�o�͏��
		LIST_WRITE_STATUS	status;
		status.nFileNumber	= 0;			// ���݂̃t�@�C���ԍ�
		status.nFileCount	= 0;			// �t�@�C������
		status.fTotalSize	= 0.0f;			// �����T�C�Y
		status.nTotalTime	= 0;			// �������t����

		// ���X�g�t�@�C���̃f�B���N�g�����擾
		CString	strCurDir;
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sFileName, drive, dir, NULL, NULL);
		status.strDirectory.Format(TEXT("%s%s"), drive, dir);

		// �w�b�_���o��
		CString strText = strHead; /* Rumble 190 */
		if (bSelected) { /* Rumble 190 */
			// �I������Ă���t�@�C����S�ď�������
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				strText = WriteFormatFileHeader(file, GetTreeItem(arrayList[i]), strText, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ���[�g�̃A�C�e����S�ď�������
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				strText = WriteFormatFileHeader(file, pItem, strText, bIsHtml, &status, bWriteHtml);
			}
		}
		/* Rumble 190 *///file.Write(strHead, strHead.GetLength());
		strText = MakeFormatFileFoot(strText, bIsHtml, &status, bWriteHtml); /* Rumble 190 */
		file.Write(strText, strText.GetLength()); /* Rumble 190 */
		// �X�e�[�^�X�����Z�b�g /* Rumble 190 */
		status.nFileNumber	= 0;			// ���݂̃t�@�C���ԍ�
		status.nFileCount	= 0;			// �t�@�C������
		status.fTotalSize	= 0.0f;			// �����T�C�Y
		status.nTotalTime	= 0;			// �������t����

		// �{�f�B���o��
		int		nFileNumber = 1;
		if (bSelected) {
			// �I������Ă���t�@�C����S�ď�������
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				WriteFormatFileBody(file, GetTreeItem(arrayList[i]), strBody, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ���[�g�̃A�C�e����S�ď�������
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				WriteFormatFileBody(file, pItem, strBody, bIsHtml, &status, bWriteHtml);
			}
		}

		// �t�b�^���o��
		// �X�e�[�^�X�����Z�b�g /* Rumble 190 */
		status.nFileNumber	= 0;			// ���݂̃t�@�C���ԍ�
		status.nFileCount	= 0;			// �t�@�C������
		status.fTotalSize	= 0.0f;			// �����T�C�Y
		status.nTotalTime	= 0;			// �������t����
		CString strTextFoot = strFoot; /* Rumble 190 */
		if (bSelected) { /* Rumble 190 */
			// �I������Ă���t�@�C����S�ď�������
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				strTextFoot = WriteFormatFileHeader(file, GetTreeItem(arrayList[i]), strTextFoot, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ���[�g�̃A�C�e����S�ď�������
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				strTextFoot = WriteFormatFileHeader(file, pItem, strTextFoot, bIsHtml, &status, bWriteHtml);
			}
		}
		/* Rumble 190 *///file.Write(strHead, strHead.GetLength());
		strTextFoot = MakeFormatFileFoot(strTextFoot, bIsHtml, &status, bWriteHtml); /* Rumble 190 */
		/* Rumble 190 *///WriteFormatFileFoot(file, strFoot, bIsHtml, &status, bWriteHtml);
		WriteFormatFileFoot(file, strTextFoot, bIsHtml, &status, bWriteHtml);
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format(TEXT("�t�@�C�����I�[�v���ł��܂���ł���\n���f���܂�\n\n%s"), sFileName);
		MessageBox(strMsg, TEXT("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return(true);
}

// =============================================
//  CMySuperGrid::ReplaceFileName
//  �T�v  : �t�@�C�����Ɏg�p�ł��Ȃ�������u������
//  ����  : strName		= �t�@�C����
//  �߂�l: 			= �u����̃t�@�C����
// =============================================
CString	CMySuperGrid::ReplaceFileName(const FILE_MP3 *fileMP3, CString strName)
{
	TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
	TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
	bool	bKeepExt = false;

	/* LastTrain 058 */
	_tsplitpath(strName, drive, dir, fname, ext); /* BeachMonster 100 */
	extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
	if (/*strlen(ext) == 4 &&*/ CheckFileExt(fileMP3, ext)) {
	/*
		&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
			 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
			 strcmpi(ext, ".asf") == 0 ||
			 strcmpi(ext, ".ape") == 0 ||
			 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
			 strcmpi(ext, ".ogg") == 0)) {
	*/
		// �g���q����菜��
		strName = fname;
		strName = dir + strName; /* BeachMonster 100 */
		strName = drive + strName; /* BeachMonster 100 */
		bKeepExt = true;
	}
	{ /* LastTrain 058 */
		std::vector<TCHAR> buffer(strName.GetLength()*2+2);

		switch (g_nFileUnifyHiraKata) { // �������Љ���
		case 1:// �Ђ炪��
			conv_kata2hira(strName.GetBuffer(0));
			break;
		case 2:// �J�^�J�i
			conv_hira2kata(strName.GetBuffer(0));
			break;
		}
		switch (g_nFileUnifyKigou /* BeachMonster 103 */) { // �L��
		case 1:// �S�p
			conv_han2zens(buffer.data(), strName.GetBuffer(0), CONV_KIGOU);
			strName = buffer.data();
			break;
		case 2:// ���p
			conv_zen2hans(buffer.data(), strName.GetBuffer(0), CONV_KIGOU);
			strName = buffer.data();
			break;
		}
		switch (g_nFileUnifySuji) { // ����
		case 1:// �S�p
			conv_han2zens(buffer.data(), strName.GetBuffer(0), CONV_SUJI);
			strName = buffer.data();
			break;
		case 2:// ���p
			conv_zen2hans(buffer.data(), strName.GetBuffer(0), CONV_SUJI);
			strName = buffer.data();
			break;
		}
		switch (g_nFileUnifyKata) { // �J�^�J�i
		case 1:// �S�p
			conv_han2zens(buffer.data(), strName.GetBuffer(0), CONV_KATA);
			strName = buffer.data();
			break;
		case 2:// ���p
			conv_zen2hans(buffer.data(), strName.GetBuffer(0), CONV_KATA);
			strName = buffer.data();
			break;
		}
		switch (g_nFileUnifyAlpha) { // �A���t�@�x�b�g
		case 1:// �S�p
			conv_han2zens(buffer.data(), strName.GetBuffer(0), CONV_ALPHA);
			strName = buffer.data();
			break;
		case 2:// ���p
			conv_zen2hans(buffer.data(), strName.GetBuffer(0), CONV_ALPHA);
			strName = buffer.data();
			break;
		}
		switch (g_nFileUnifyUpLow) { // �啶��������
		case 1:// �啶��
			conv_upper(strName.GetBuffer(0));
			break;
		case 2:// ������
			conv_lower(strName.GetBuffer(0));
			break;
		case 3:// �P��̂P�����ڂ̂ݑ啶��
			conv_lower(strName.GetBuffer(0));
			conv_first_upper(strName.GetBuffer(0), /* STEP 026 */ g_strFirstUpperIgnoreWords, g_strFirstUpperSentenceSeparator, g_bFirstUpperIgnoreWord);
			break;
		}
		/* STEP 006 */
		if (fileMP3 != NULL) {
			switch (g_nFileExtChange) {
			case 0:// ���̂܂�
				break;
			case 1://������
				conv_lower(ext);
				break;
			case 2://�啶��
				conv_upper(ext);
				break;
			}
		}
	}
	{// ������u��(���[�U�w��) /* FreeFall 050 */
		int i; for (i = 0; i < USER_FILENAME_REPLACE_MAX; i++) {
			FILENAME_REPLACE	*pRep = &g_userFileNameReplace[i];
			if (!pRep->strBefore.IsEmpty() && pRep->strBefore != pRep->strAfter && pRep->strAfter.Find(pRep->strBefore) == -1) {
				strName = StrReplace(strName, pRep->strBefore, pRep->strAfter);
			}
		}
	}
	if (bKeepExt) { /* LastTrain 058 */
		strName += ext;
	}
	// ������u��
	int i; for (i = 0; i < FILENAME_REPLACE_MAX; i++) {
		FILENAME_REPLACE	*pRep = &g_fileNameReplace[i];
		if (pRep->strBefore != pRep->strAfter && pRep->strAfter.Find(pRep->strBefore) == -1) {
			strName = StrReplace(strName, pRep->strBefore, pRep->strAfter);
		}
	}
	return(strName);
}

LRESULT CMySuperGrid::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	return res;
}

void CMySuperGrid::MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	LOGFONT lf;
	GetFont()->GetLogFont( &lf );

	//CSuperGridCtrl::MeasureItem(lpMeasureItemStruct);
	/* 2003.06.19 �f�t�H���g�t�H���g�ꍇ��"_"�������Ȃ��̂� +1 */
	/* 2006.02.02 CSuperGridCtrl::MeasureItem()�ɂ��킹�� +4 �ƂȂ�悤�ɂ���� +1 �������̓A�C�R���̍���*/ 
	/**/
	if( lf.lfHeight < 0 )
		lpMeasureItemStruct->itemHeight = max(-lf.lfHeight + 2 + 1 + 1, m_cxImage);
	else
		lpMeasureItemStruct->itemHeight = max(lf.lfHeight + 2 + 1 + 1, m_cxImage);
	/**/
}

bool CMySuperGrid::DeleteChar(int nCount, int nPos)
{
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				CString	strText;
				TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
				bool	bKeepExt = false;

				// �Z���̃e�L�X�g���擾
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// �t�@�C�����̏ꍇ�A�g���q���S�p�ɕϊ�����Ȃ��悤�ɂ���
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
					if (/*strlen(ext) == 4 &&*/ CheckFileExt(fileMP3, ext)
					) {/*
					&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						 strcmpi(ext, ".asf") == 0 ||
						 strcmpi(ext, ".ape") == 0 ||
						 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						 strcmpi(ext, ".ogg") == 0)) {
					*/
						// �g���q����菜��
						strText = fname;
						bKeepExt = true;
					}
				}

				if (strText.GetLength() > nCount) {
					// �폜
					if (nPos == 0) {
						strText.Delete(0, nCount);
					} else {
						strText = strText.Left(strText.GetLength()-nCount);
					}
					// ��菜���Ă����g���q��t���Ȃ���
					if (bKeepExt) {
						strText += ext;
					}

					ChangeSubItemText(nItem, nColumn, strText);
					InvalidateItemRect(nItem);
				}
			}
		}
	}
	return(true);
}

bool CMySuperGrid::AddFillNumber(int nStart, int nStep, int nWidth, CString strBefore/* Conspiracy 194 */, CString strAfter/* Conspiracy 194 */, int nPos, bool bSpaceInitNumber/* Conspiracy 194 */)
{
	CString strSeparator = strBefore; /* Conspiracy 194 */
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	int		nNumber = nStart;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CString	strNumber;
			CItemInfo	*pItemInfo = GetData(pItem);
			strNumber.Format(TEXT("%0*d"), nWidth, nNumber);
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
				CString strText;
				if (nPos == 0) {
					strText = strNumber + strSeparator + GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);
				} else if (nPos == 2) {
					strText = strBefore + strNumber + strAfter;
				} else {
					if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) { /* Baja 157 */
						strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);
						TCHAR	extOrg[_MAX_EXT];
						TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
						_tsplitpath(fileMP3->strFullPathName, NULL, NULL, NULL, extOrg);
						_tsplitpath(strText, NULL, NULL, fname, ext);
						strText = fname + strSeparator + strNumber;
						extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
						if (/*strlen(ext) != 4 || */!CheckFileExt(fileMP3, ext)
						)/*
						||  !(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
							  strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
							  strcmpi(ext, ".asf") == 0 ||
							  strcmpi(ext, ".ape") == 0 ||
							  strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
							  strcmpi(ext, ".ogg") == 0)) {//�C�� by Kobarin
						*/
							strText += extOrg;
						/*}*/
					} else {
						strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]) + strSeparator + strNumber;
						/* Baja 158 */
						/*
						CString strText2 = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);
						strText = strSeparator + strNumber;
						strText.Insert(0, strText2);
						*/
					}
				}
				ChangeSubItemText(nItem, nColumn, strText);
				InvalidateItemRect(nItem);
			}
			nNumber += nStep;
		} else { /* Conspiracy 194 */
			if (bSpaceInitNumber) {
				nNumber = nStart;
			}
		}
	}
#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif

	return(true);
}

bool CMySuperGrid::MoveFolderFormat(USER_MOVE_FODLER_FORMAT *pForm, CString strFolder, bool isCopy)
{
	// ��������̂Ƃ��͑I�������t�H���_�Ɉړ� /* WildCherry 067 */
//	if (pForm->strFormat.IsEmpty()) {
//		// �ϊ���������
//		return(false);					// �G���[
//	}

	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	/*
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}
	*/

	// �Œ蕶�������͂�����
	if (pForm->strFormat.Find(TEXT("%STRING%")) != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// �L�����Z��
			return(false);
		}
		pForm->strFixString = dialog.m_strText;
	}

	CStringArray strMoveFile;
	// �A�ԕϊ������̐ݒ�
	//char	*sNumFormat = "%01d";
	//sNumFormat[2] = '0' + pForm->nColumnCount;

	//int		nNumber = pForm->nInitNumber;
	// �v���O���X�o�[������
	if (isCopy) {
		m_pDoc->StartLoadFile(TEXT("�t�@�C�����R�s�[��..."));
	} else {
		m_pDoc->StartLoadFile(TEXT("�t�@�C�����ړ���..."));
	}
	m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, TEXT("���΂炭���҂�������..."));
	CString strConvFileNameBefore;
	CString strConvFileNameAfter;

	int nOverWriteFlag = 0; /* STEP 041 */
	for (int i=0;i<nCount;i++) {
		if (nOverWriteFlag == 3)	break;
		// �v���O���X�o�[�X�V
		if (nCount > 1) {
			m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
		}
		int		nIndex = arrayList[i];
		int		nNumber = (int)GetLParamFromIndex(nIndex);
		if (!(nNumber >= 0))	continue;
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
		if (fileMP3->bModifyFlag == true) {
			MessageBox(TEXT("�ύX���ۑ�����Ă��Ȃ��t�@�C�����܂܂�Ă��܂�\n\n"
					   "�ύX���ۑ�����Ă��Ȃ��t�@�C���͖������܂�"),
					   TEXT("�I���t�@�C�����ړ�/�R�s�["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			continue;
		}

		// �t�@�C��������g���q���������������擾
		CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
		TCHAR	sFileName[_MAX_FNAME];
		_tsplitpath_s(strFileName, NULL,NULL, NULL,NULL, sFileName,_MAX_FNAME, NULL,NULL);

		CString	strText = pForm->strFormat;
		auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));
		auto[strDiskNumber, strDiskNumber2, strDiskNumber3] = GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding(TEXT("\r")));

		// �������g���ĕ�������쐬
		strText = StrReplace(strText, TEXT("%FILE_NAME%")    , sFileName);
		strText = StrReplace(strText, TEXT("%TRACK_NAME%")   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%ALBUM_NAME%")   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%ARTIST_NAME%")  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER%") , strTrackNumber);
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER2%"), strTrackNumber2);
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER3%"), strTrackNumber3);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER%") , strDiskNumber);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER2%"), strDiskNumber2);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER3%"), strDiskNumber3);
		strText = StrReplace(strText, TEXT("%RELEASE_YEAR%") , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%COMMENT%")      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding(TEXT("\r"))/* BeachMonster 089 */);
		strText = StrReplace(strText, TEXT("%GENRE%")        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%STRING%")       , pForm->strFixString);
		// SIF �̍���
		strText = StrReplaceSIF(strText, fileMP3);

		// ����R�[�h�i�ꕔ�j���X�y�[�X�ɒu������ /* SeaKnows2 040 */
		strText = StrReplace(strText, "\n", " ");
		strText = StrReplace(strText, "\r", " ");
		strText = StrReplace(strText, "\t", " ");
		{
			int nStart = 0;
			CString strFolder;
			CString strNewPath;
			strText.TrimLeft('\\');
			strText.TrimRight('\\');
			strText += "\\";
			while (true) {
				nStart = strText.Find('\\'/* AstralCircle2 042 , nStart + 1*/);
				if (nStart == -1) {
					break;
				}
				CString strNewPath = ReplaceFileName(fileMP3, strText.Left(nStart));
				if (strNewPath.GetLength() > g_nConfFileNameMaxChar && g_bConfFileNameMaxCheck) {
					if (strConvFileNameBefore == strNewPath) {
						strNewPath = strConvFileNameAfter;
					} else {
						CDlgFileNameChange dialog;
						dialog.m_nMaxChar = g_nConfFileNameMaxChar;
						dialog.m_strFileName = strNewPath;
						dialog.m_strOrgFileName = strNewPath;
						//dialog.m_strMsg.Format("�쐬����t�H���_���� %d �����ȓ��ɕύX���Ă�������", g_nConfFileNameMaxChar);
						dialog.m_strMsgFormat = "�쐬����t�H���_���� %d(%d) �����ȓ��ɕύX���Ă�������";
						if (dialog.DoModal() == IDOK) {
							strConvFileNameBefore = strNewPath;
							strNewPath = dialog.m_strFileName;
							strConvFileNameAfter = strNewPath;
						}
					}
				}
				strFolder += strNewPath;
				strText.Delete(0, nStart + 1);
				strFolder += "\\";
			}
			strFolder.TrimRight('\\');
			strText = strFolder;
		}

		{// �t�H���_���쐬���Ĉړ��܂��̓R�s�[
			int nStart = 0;
			BOOL res = TRUE;
			CString strMakePath;
			strFolder.TrimRight('\\');
			CString strPathName = strFolder + "\\" + strText;
			//if (strPathName.Right(1) == "\\") {
			//	strPathName=strPathName.Left(strPathName.GetLength()-1);
			//}

			// �f�B���N�g����1���J��Ԃ��쐬
			CString strNewPath = strPathName;
			strPathName.TrimRight('\\');
			strPathName += _T("\\");
			while(strMakePath + _T("\\") != strPathName)
			{
				// �쐬����f�B���N�g������ݒ�
				nStart = strPathName.Find('\\', nStart + 1);
				strMakePath = strPathName.Left(nStart);
				strNewPath = strMakePath;
				CFileStatus rStatus;
				// �����̃s���I�h���폜
				strNewPath.MakeReverse();
				for (int i=0;i<strNewPath.GetLength();i++) {
					if (strNewPath[i] == '.') {
						strNewPath.Delete(0, 1);
						i--;
					} else break;
				}
				strNewPath.MakeReverse();
				if (strNewPath.GetLength() == 0)	res = false;
				// UNC
				if (strNewPath.GetLength() < 3 && strNewPath[0] == '\\' && strNewPath[1] == '\\')	continue;
				if (strNewPath.GetLength() == 2 && strNewPath[1] == ':')	continue;
				if (strFolder == strNewPath)	continue;
				// �f�B���N�g�������݂��邩�`�F�b�N��������΍쐬
				DWORD dwAttr = GetFileAttributes(strNewPath);
				if (dwAttr == 0xFFFFFFFF || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
				//if(!(CFile::GetStatus(strNewPath, rStatus) || (strNewPath.GetLength() == 2 && strNewPath.GetAt(1) == ':'))) {
					res = ::CreateDirectory(strNewPath, NULL);
					if (!res) break;
				}
			}
			if (!res) {
				MessageBox(strNewPath + TEXT(" ���쐬�ł��܂���ł���"), TEXT("�ړ�/�R�s�[�G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
				break;
			}
			strNewPath += _T("\\");
			SHFILEOPSTRUCT sfo;
			ZeroMemory(&sfo, sizeof(sfo));
			sfo.hwnd = GetSafeHwnd();
			sfo.wFunc = FO_MOVE;
			sfo.lpszProgressTitle = _T("�ړ����Ă��܂�...");
			CDoubleZeroString strFile;
			strFile.Add(GetFileColumnText(fileMP3, COLUMN_FULL_PATH_NAME) + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
			{
				TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				TCHAR	sLyricsFile[FILENAME_MAX+1];
				// MP3 �t�@�C���Ɠ����t�H���_�ɂ���̎��t�@�C��������
				_tsplitpath(fileMP3->strFullPathName, drive, dir, fname, ext);
				// .lrc => .txt �̏��Ō���
				int i; for (i = 0; i < 2; i++) {
					LPCTSTR	sLyricsExt = (i == 0) ? TEXT(".lrc") : TEXT(".txt");
					_tmakepath_s(sLyricsFile,FILENAME_MAX+1, drive, dir, fname, sLyricsExt);
					if (GetFileAttributes(sLyricsFile) != 0xFFFFFFFF) {
						// �t�@�C���𔭌��F�̎��t�@�C����������
						strFile.Add(sLyricsFile);
					}
				}
			}
			sfo.pFrom = strFile;
			sfo.pTo = strNewPath;
			sfo.fFlags = FOF_NOCONFIRMATION | FOF_SILENT /* BeachMonster4 116 */;
			if (nOverWriteFlag != 1) { /* STEP 041 */
				TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				TCHAR	sDestFile[FILENAME_MAX+1];
				_tsplitpath(fileMP3->strFullPathName, drive, dir, fname, ext);
				_tmakepath_s(sDestFile,FILENAME_MAX+1, NULL, strNewPath, fname, ext);
				if (GetFileAttributes(sDestFile) != 0xFFFFFFFF) {
					CDlgFileOverWrite2	dialog;
					dialog.m_strFileName = sDestFile;
					CFileStatus	fileStatus;
					if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
						break;
					}
					dialog.m_strSize.Format(TEXT("%ld byte"), fileStatus.m_size);
					if (fileStatus.m_mtime.GetTime() < 0) {
						dialog.m_strTimeStamp.Format(TEXT("----/--/-- --:--:--"));
					} else {
						dialog.m_strTimeStamp.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
													 fileStatus.m_mtime.GetYear(),
													 fileStatus.m_mtime.GetMonth(),
													 fileStatus.m_mtime.GetDay(),
													 fileStatus.m_mtime.GetHour(),
													 fileStatus.m_mtime.GetMinute(),
													 fileStatus.m_mtime.GetSecond());
					}
					dialog.DoModal();
					nOverWriteFlag = dialog.m_nResult;
					switch(dialog.m_nResult) {
					case 0:				// ���̃t�@�C�����㏑��
					case 1:				// �ȍ~�S�ď㏑��
						break;
					case 2:				// �X�L�b�v
						continue;
					case 3:				// ���~
					default:
						continue;
					case 4:
						sfo.fFlags = sfo.fFlags | FOF_RENAMEONCOLLISION;
						sfo.pTo = strNewPath = dialog.m_strFileName;
						break;
					}
				}
			}
			if (isCopy) {
				sfo.wFunc = FO_COPY;
				sfo.lpszProgressTitle = _T("�R�s�[���Ă��܂�...");
			}
			CString strStatus;
			if (isCopy) {
				strStatus.Format(TEXT("%s �� %s �փR�s�[���Ă��܂� (%d/%d)"), strFile.Get(0) /* FreeFall 047 */, strNewPath, i, nCount);
			} else {
				strStatus.Format(TEXT("%s �� %s �ֈړ����Ă��܂� (%d/%d)"), strFile.Get(0) /* FreeFall 047 */, strNewPath, i, nCount);
			}
			((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(strStatus);	// �X�e�[�^�X�o�[�ɐi���󋵕\��
			if (::SHFileOperation(&sfo) == 0) {
				strMoveFile.Add(strNewPath + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
			} else {
				break;
			}
		}
	}
	OnChangeSelect();	// �X�e�[�^�X�o�[�̕\�������ɖ߂�
	// �v���O���X�o�[�I��
	m_pDoc->EndLoadFile();
#ifndef mogtan
	if (isCopy) {
		return(true);
	}
#endif
#ifdef mogtan
	{
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = MakeSelectFileArray(arrayList);
		int fileCount=0;
		int i; for (i = 0; i < nCount; i++, fileCount++) {
			if (nCount > strMoveFile.GetSize()) continue;
			int		nIndex = arrayList[i];
			int		nNumber = (int)GetLParamFromIndex(nIndex);
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
			if (nNumber >= 0) {
				CFileMP3	file;
				if (file.Attach(strMoveFile[fileCount])) {
					*fileMP3 = file.m_fileMP3;
					UpdateFileStatusFromIndex(nIndex, fileMP3);
				}
				m_pDoc->UpdateAllViews(NULL);
			}
		}
	}
#else
	{
		// �����������̂����X�g����폜
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = MakeSelectFileArray(arrayList);
		for (int i = nCount-1; i >= 0; i--) {
			if (nCount > strMoveFile.GetSize()) continue;
			int		nIndex = arrayList[i];
			int		nNumber = (int)GetLParamFromIndex(nIndex);
			if (nNumber >= 0) {
				FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
				UpdateParentAtDeleteItem(nIndex, fileMP3);
				CFileMP3::InitData(fileMP3);
				DeleteItemFromIndex(nIndex);
			}
		}
		// �S�ẴA�C�e���̑I������
		SelectAllItems(FALSE);
		m_pDoc->UpdateAllViews(NULL);
		// �����������̂����X�g�ɒǉ�
		for (int i=0;i<strMoveFile.GetSize();i++) {
			m_pDoc->AddRequestFile(strMoveFile[i], NULL);
		}
		m_pDoc->StartLoadFile(TEXT("�l�o�R�t�@�C���ǂݍ��ݒ�..."));
#ifdef FLICKERFREE
		//SetRedraw(FALSE);
#endif
		m_pDoc->ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//SetRedraw(TRUE);
#endif
		m_pDoc->EndLoadFile();
		m_pDoc->UpdateAllViews(NULL);
	}
#endif
	return(true);
}

UINT CMySuperGrid::Time2Sec(CString strTime)
{
	int nIndex = strTime.Find(_T(':'));
	if (nIndex == -1) {
		return 0;
	}
	return _ttoi(strTime.Left(nIndex)) * 60 + _ttoi(strTime.Mid(nIndex+1));
}

void CMySuperGrid::UpdateParentAtDeleteItem(UINT nIndex, FILE_MP3 *fileMP3)
{
	CTreeItem *pItem = GetTreeItem(nIndex);
	CTreeItem *pItemParent = GetParentItem(pItem);
	CItemInfo *pInfo = GetData(pItemParent);

	/* RockDance 128 */
	if (g_bOptShowTotalParent) {
		CTreeItem* pTotalParent = pItemParent;
		while (pTotalParent != NULL) { /* TunamiStruck 007 */
			// �Ȑ���\��
			CItemInfo	*pInfo = GetData(pTotalParent);
			CString strBuffer;
			CString strLeft, strRight;
			int		nIndex = NodeToIndex(pTotalParent);
			int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
			if (nColNum >= 0) {
				strBuffer = pInfo->GetSubItem(nColNum-1);
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
					strBuffer.Format(TEXT("%d"), _tstoi(strBuffer)-1);
				} else {
					if (strBuffer.Find(TEXT("/")) != -1) {
						strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);
						if (pTotalParent != pItemParent) {
							strBuffer.Format(TEXT("%s/%d"), strLeft, _tstoi(strRight)-1);
						} else {
							strBuffer.Format(TEXT("%d/%d"), _tstoi(strLeft)-1, _tstoi(strRight)-1);
						}
					} else {
						if (pTotalParent == pItemParent) {
							ASSERT(0);
							//strBuffer.Format("%d/%d", _tstoi(strBuffer)-1, _tstoi(strBuffer)-1); // ����Ȃ����H
						} else {
						if (_tstoi(strBuffer) > 1) {
							strBuffer.Format(TEXT("%s/%d"), strBuffer, _tstoi(strBuffer)-1);
						} else {
							strBuffer.Format(TEXT("%s"), strBuffer);
						}
						}
					}
				}
				pInfo->SetSubItemText(nColNum-1, strBuffer);
				SetItemText(nIndex, nColNum, strBuffer);
			}
			// ���t���ԍ��v��\��
			nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
			if (nColNum >= 0) {
				strBuffer = GetFileColumnText(fileMP3, COLUMN_PLAY_TIME);
				UINT nTotalTime = Time2Sec(strBuffer);
				strBuffer = pInfo->GetSubItem(nColNum-1);
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
					nTotalTime = Time2Sec(strBuffer) - nTotalTime;
					strBuffer.Format(_T("%d:%02d"), nTotalTime/60, nTotalTime%60);
				} else {
					if (strBuffer.Find(TEXT("/")) != -1) {
						strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);
						if (pTotalParent != pItemParent) {
						} else {
							UINT nTotalTimeTemp = Time2Sec(strLeft) - nTotalTime;
							strLeft.Format(_T("%d:%02d"), nTotalTimeTemp/60, nTotalTimeTemp%60);
						}
						UINT nTotalTimeTemp = Time2Sec(strRight) - nTotalTime;
						strBuffer.Format(_T("%s/%d:%02d"), strLeft, nTotalTimeTemp/60, nTotalTimeTemp%60);
					} else {
						if (pTotalParent == pItemParent) {
							ASSERT(0);
							//nTotalTime = Time2Sec(strBuffer) - nTotalTime;
							//strLeft.Format(_T("%d:%02d"), nTotalTime/60, nTotalTime%60);
							//strBuffer.Format(_T("%s/%d:%02d"), strLeft, nTotalTime/60, nTotalTime%60);
						} else {
							nTotalTime = Time2Sec(strBuffer) - nTotalTime;
							if (nTotalTime > 0) {
								strBuffer.Format(_T("%s/%d:%02d"), strBuffer, nTotalTime/60, nTotalTime%60);
							} else {
								strBuffer.Format(_T("%s"), strBuffer);
							}
						}
					}
				}
				pInfo->SetSubItemText(nColNum-1, strBuffer);
				SetItemText(nIndex, nColNum, strBuffer);
			}
			//�T�C�Y���v��\��
			nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
			if (nColNum >= 0) {
				strBuffer = pInfo->GetSubItem(nColNum-1);
				strBuffer.TrimRight(TEXT("MB"));
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
					strBuffer.Format(TEXT("%.2fMB"), _tstof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024);
				} else {
					if (strBuffer.Find(TEXT("/")) != -1) {
						strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);
						if (pTotalParent != pItemParent) {
							strBuffer.Format(TEXT("%s/%.2fMB"), strLeft, _tstof(strRight) - (float)fileMP3->lFileSize / 1024 / 1024);
						} else {
							strBuffer.Format(TEXT("%.2fMB/%.2fMB"), _tstof(strLeft) - (float)fileMP3->lFileSize / 1024 / 1024, _tstof(strRight) - (float)fileMP3->lFileSize / 1024 / 1024);
						}
					} else {
						if (pTotalParent == pItemParent) {
							ASSERT(0);
							//strBuffer.Format("%.2fMB/%.2fMB", _tstof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024, _tstof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024); // ����Ȃ����H
						} else {
							strLeft.Format(TEXT("%sMB"), strBuffer);
							if ((_tstof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024) > 0.0) {
								strRight.Format(TEXT("%.2fMB"), _tstof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024);
								strBuffer.Format(TEXT("%s/%s"), strLeft, strRight);
							} else {
								strBuffer = strLeft;
							}
						}
					}
				}
				pInfo->SetSubItemText(nColNum-1, strBuffer);
				SetItemText(nIndex, nColNum, strBuffer);
			}
			pTotalParent = GetParentItem(pTotalParent);
		}
	}
}

void CMySuperGrid::SetHeaderFont(LOGFONT listFont)
{
	if (m_rootItemFont != NULL) {
		m_rootItemFont->Detach();
		delete m_rootItemFont;
	}
	CFont* pFont = new CFont();
	LOGFONT logFont;
	pFont->CreatePointFont(8 * 10, listFont.lfFaceName);
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	pFont->Detach();
	pFont->CreateFontIndirect(&logFont);
	m_rootItemFont = pFont;
}

BOOL CMySuperGrid::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!g_bOptShowTips)	return FALSE; /* Rumble 188 */

    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
	LVHITTESTINFO ht;
	ht.pt = pt;
	SubItemHitTest(&ht);
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	if (ht.iItem > -1) {
		if (m_nPrevTipItem != ht.iItem || m_nPrevTipSubItem != ht.iSubItem) {
			TRACE1("CMySuperGrid::OnToolTipNotify [%d]\n", m_nPrevTipItem);
			TRACE1("CMySuperGrid::OnToolTipNotify [%d]\n", ht.iItem);
			TRACE1("CMySuperGrid::OnToolTipNotify [%d]\n", m_nPrevTipSubItem);
			TRACE1("CMySuperGrid::OnToolTipNotify [%d]\n", ht.iSubItem);
			m_nPrevTipItem = ht.iItem;
			m_nPrevTipSubItem = ht.iSubItem;
			CTreeItem *pItem = GetTreeItem(ht.iItem);
			CItemInfo *pInfo = GetData(pItem);
			CString strToolTip;
			if (ht.iSubItem == 0) {
				strToolTip = pInfo->GetItemText();
			} else {
				strToolTip = pInfo->GetSubItem(IndexToOrder(ht.iSubItem - 1));
			}
			//strToolTip.Remove(_T('\r'));
			{
				for (int i=0;i<strToolTip.GetLength();i++) {
					if (_ismbblead(strToolTip[i])) {
						// �������[�f�B���O�o�C�g
						i++;
					} else {
						if (strToolTip[i] == '\r') {
							strToolTip.Delete(i);
							i--;
						}
					}
				}
			}
			if (strToolTip.GetLength() == 0) {
				m_toolTip.Pop();
				TRACE0("CMySuperGrid::OnToolTipNotify NO TOOLTIP1\n");
				m_nPrevTipItem = -1;
				m_nPrevTipSubItem = -1;
				return FALSE;
			}
			// �ڍׂ�ݒ�
			if (strToolTip != "Root") {
				if (ItemHasChildren(pItem)) {
					int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[Track��] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
					nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[�S���t����] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
					nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[�T�C�Y���v] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
				} else if ((int)pInfo->GetLParam() >= 0) {
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
					strToolTip += (_T("\r[����] ") + GetFileColumnText(fileMP3, COLUMN_TRACK_NAME));
					strToolTip += (_T("\n[��è��] ") + GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME));
					strToolTip += (_T("\n[�����] ") + GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME));
					strToolTip += (_T("\n[TrackNo] ") + GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER));
					strToolTip += (_T("\n[���t����] ") + GetFileColumnText(fileMP3, COLUMN_PLAY_TIME));
					strToolTip += (_T("\n[�N] ") + GetFileColumnText(fileMP3, COLUMN_YEAR));
					strToolTip += (_T("\n[�ެ��] ") + GetFileColumnText(fileMP3, COLUMN_GENRE));
					CString strComment = GetFileColumnText(fileMP3, COLUMN_COMMENT);
					{
						BOOL bLead = FALSE;
						for (int i=0;i<strComment.GetLength();i++) {
							if (_ismbblead(strComment[i])) {
								// �������[�f�B���O�o�C�g
								i++;
							} else {
								if (strComment[i] == '\r') {
									strComment.Delete(i);
									i--;
								}
							}
						}
					}
					//strComment.Remove(_T('\r'));
					strToolTip += (_T("\n[����] ") + strComment);
					/* Conspiracy 198 */
					strComment = GetFileColumnText(fileMP3, COLUMN_FORMAT);
					{
						BOOL bLead = FALSE;
						for (int i=0;i<strComment.GetLength();i++) {
							if (_ismbblead(strComment[i])) {
								// �������[�f�B���O�o�C�g
								i++;
							} else {
								if (strComment[i] == '\r') {
									strComment.Delete(i);
									i--;
								}
							}
						}
					}
					strToolTip += (_T("\n[̫�ϯ�] ") + strComment);
					strToolTip += (_T("\n[�t�@�C��] ") + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
					strToolTip += (_T("\n[�߽] ") + GetFileColumnText(fileMP3, COLUMN_FULL_PATH_NAME));
				}
			}
			//if (m_strToolTip != strToolTip) {
			//	m_toolTip.Pop();
			//}
			m_strToolTip = strToolTip;
			pTTT->lpszText = m_strToolTip.GetBuffer(0);
			return TRUE;
		} else {
			TRACE0("CMySuperGrid::OnToolTipNotify NO TOOLTIP\n");
			return FALSE;
		}
	}
	m_toolTip.Pop();
	m_nPrevTipItem = -1;
	m_nPrevTipSubItem = -1;
	TRACE0("CMySuperGrid::OnToolTipNotify RESET\n");
	return FALSE;
}

BOOL CMySuperGrid::OnNotifyMessage(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
	LVHITTESTINFO ht;
	ht.pt = pt;
	SubItemHitTest(&ht);
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)lParam;
	if (pTTT->hdr.idFrom == IDR_MAINFRAME && pTTT->hdr.code == 2222) {
		if (ht.iItem > -1) {
			if (m_nPrevTipItem != ht.iItem || m_nPrevTipSubItem != ht.iSubItem) {
				pTTT->uFlags = 9999;
				m_toolTip.Pop();
				m_nPrevTipItem = -1;
				m_nPrevTipSubItem = -1;
				return TRUE;
			}
		} else {
			m_toolTip.Pop();
			m_nPrevTipItem = -1;
			m_nPrevTipSubItem = -1;
		}
	}
	return TRUE;
}

BOOL CMySuperGrid::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if (!OnNotifyMessage(wParam, lParam, pResult)) {
		return TRUE;
	}
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CMySuperGrid::OnKillFocus(CWnd* pNewWnd)
{
	CListCtrl::OnKillFocus(pNewWnd);

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if (m_toolTip) {
		m_toolTip.Pop();
		m_toolTip.Activate(FALSE);
		m_nPrevTipItem = -1;
		m_nPrevTipSubItem = -1;
	}
}

void CMySuperGrid::OnSetFocus(CWnd* pOldWnd)
{
	CListCtrl::OnSetFocus(pOldWnd);

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	m_toolTip.Activate(TRUE);
}

void CMySuperGrid::SelectTreeColumn() /* TyphoonSwell 025 */
{
	int nSIndex /* STEP 008 */ = GetSelectedItem();
	if (nSIndex < 0) return;

	CTreeItem	*pItem = GetTreeItem(nSIndex);
	if (ItemHasChildren(pItem) == FALSE) {
		//return;
		pItem = GetParentItem(pItem); /* STEP 008 */
	}
	CArray <int, const int &> arrayList;
	int		nCount;
	int nIndex = NodeToIndex(pItem); /* STEP 008 */
	if (nIndex < 0) return; /* STEP 008 */
	POINT	save_posMultiSelect[2]; /* STEP 008 */
	save_posMultiSelect[0] = m_posMultiSelect[0]; /* STEP 008 */
	save_posMultiSelect[1] = m_posMultiSelect[1]; /* STEP 008 */
	nCount = MakeSelectFileArray(nIndex /* STEP 008 */, arrayList);
	if (nCount == 0) return;
	SelectRangeStart(nIndex /* STEP 008 */);
	m_posMultiSelect[0].y = nIndex;
	m_posMultiSelect[1].y = arrayList[nCount-1];
	if (m_posMultiSelect[0].x == 0) { /* SeaKnows 035 */
		SelItemRange(TRUE, m_posMultiSelect[0].y+1, m_posMultiSelect[1].y);
		OnChangeSelect(); /* WildCherry 072 */
		return;
	} else {
		SelItemRange(FALSE, -1, -1);
		/* STEP 008 *///SelItemRange(TRUE, m_posMultiSelect[0].y, m_posMultiSelect[0].y);
		/* WildCherry2 074 *///OnChangeSelect(); /* WildCherry 072 */
		SetItemState(nSIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED); /* STEP 008 */
	}
	for (int i=m_posMultiSelect[0].y;i<=m_posMultiSelect[1].y/* STEP 018 */;i++) {
		pItem = GetTreeItem(i);
		if (IsItemFile(pItem) != TRUE)	continue; /* STEP 018 */
		m_posMultiSelect[0].y = i;
		break;
	}
	InvalidateRangeItem(save_posMultiSelect[0].x, save_posMultiSelect[0].y, save_posMultiSelect[1].x, save_posMultiSelect[1].y); /* STEP 008 */
	InvalidateSelectRangeItem();
}

#ifdef FLICKERFREE
			CBrush br(RGB(0, 0, 0));
#endif
BOOL CMySuperGrid::OnDrawCustomCellBG(CDC *pDC, int nItem, int nColumn, RECT &rcItem) /* SeaKnows 036 */
{
	if (m_bMultiSelect) {
		int		sx, sy, ex, ey, nTemp;
		sx = m_posMultiSelect[0].x;
		sy = m_posMultiSelect[0].y;
		ex = m_posMultiSelect[1].x;
		ey = m_posMultiSelect[1].y;
		if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
		if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}

		if (nColumn >= COLUMN_MIN && nItem >= sy && nItem <= ey && nColumn >= sx && nColumn <= ex) {
#ifndef FLICKERFREE
			CBrush br(RGB(0, 0, 0));
#endif
			/* STEP 028 */
			CTreeItem	*pSelItem = GetTreeItem(nItem);
			if (pSelItem != NULL) {
				CItemInfo	*pInfo = GetData(pSelItem);

				CONTROLTYPE ctrlType;
				if (pInfo->GetControlType(nColumn-1, ctrlType)) {
					if (ctrlType == invalid) {
						// �ҏW�s�̂��ߊD�F
						CBrush br(RGB(192, 192, 192));
						pDC->FillRect(&rcItem, &br);
						return(TRUE);
					}
				}
			}
			pDC->FillRect(&rcItem, &br);
			return(TRUE);
		}
	} else if (g_bConfFileNameMaxCheck && g_bFileNameMaxCellColor) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL) {
			int		nType = g_nColumnTypeList[nColumn];
			if (nType >= 0 && nType < COLUMN_MAX) {
				if (nType == COLUMN_FILE_NAME && IsItemFile(pItem) == TRUE) {
					CString strFileName = GetItemText(nItem, nColumn);
					if (strFileName.GetLength() > g_nConfFileNameMaxChar) {
						CBrush br(RGB(128, 0, 0));
						pDC->FillRect(&rcItem, &br);
						return(TRUE);
					}
				}
			}
		}
	}
	return(FALSE);

}

void CMySuperGrid::CheckFileNameMax() /* SeaKnows 037 */
{
	int nIndex = GetSelectedItem();
	if (nIndex < 0) return;

	CTreeItem	*pItem = GetTreeItem(nIndex);
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	if (nCount == 0) return;
	for (int i=0;i<nCount;i++) {
		int		nIndex = arrayList[i];
		int		nNumber = (int)GetLParamFromIndex(nIndex);
		if (!(nNumber >= 0))	continue;
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
		if (fileMP3->strFileName.GetLength() > g_nConfFileNameMaxChar) {
			// �t�@�C�����������I�[�o�[
			TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
			_tsplitpath(fileMP3->strFileName, NULL, NULL, fname, ext);
			CDlgFileNameChange dialog;
			dialog.m_nMaxChar = g_nConfFileNameMaxChar - lstrlen(ext);
			dialog.m_strFileName = fname;
			dialog.m_strOrgFileName = fname;
			//dialog.m_strMsg.Format("�t�@�C������ %d �����ȓ��ɕύX���Ă�������", dialog.m_nMaxChar);
			dialog.m_strMsgFormat = "�t�@�C������ %d(%d) �����ȓ��ɕύX���Ă�������";
			if (dialog.DoModal() == IDOK) {
				ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_FILE_NAME], dialog.m_strFileName + ext);
				InvalidateItemRect(nIndex);
			}
		}
	}
	//InvalidateSelectRangeItem();
}

int CMySuperGrid::GetSelectedItem() /* WildCherry3 083 */
{
	int iItem =GetNextItem(-1, LVNI_ALL | LVNI_SELECTED | LVIS_FOCUSED);
	if (iItem == -1) {
		iItem = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	}
	return iItem;
}

CString CMySuperGrid::ConvHTML(CString& str, bool bConv) /* BeachMonster4 120 */
{
	if (!bConv) {
		return str;
	}
	CString strText = str;
	strText.Replace(TEXT("&"), TEXT("&amp;"));
	strText.Replace(TEXT(" "), TEXT("&nbsp;"));
	strText.Replace(TEXT("<"), TEXT("&lt;"));
	strText.Replace(TEXT(">"), TEXT("&gt;"));
	strText.Replace(TEXT("\""), TEXT("&quot;"));
	strText = StrReplace(strText, TEXT("\r\n"), TEXT("<BR>"));
	return strText;
}

CString CMySuperGrid::quoteForComment(CString &str) /* RockDance 124 */
{
	if (str.IsEmpty()) {
		return str;
	}
	if (str.Find(TEXT("\r\n")) != -1 || str[0] == '"') {
		str.Replace(TEXT("\""), TEXT("\"\""));
		str = '"' + str + '"';
	}
	return str;
}

bool CMySuperGrid::isOneCellClipboard(CString strBuffer) /* RockDance 124 */
{
	// �N���b�v�{�[�h����擾�f�[�^���P�Z�����̃f�[�^���`�F�b�N����
	//strBuffer.Replace("\"\"", ""); // "" �͖�������
	int		nLength = strBuffer.GetLength();
	if (nLength == 0) return true;

	//if (ey >= GetItemCount()) ey = GetItemCount() - 1;
	int nCount = 0;
	// �I��͈͂̃Z���̓��e�𕶎���ɕϊ�
	auto pBuffer = strBuffer.GetBuffer(nLength);
	int		nPos = 0;
	while (true) {
		// �P�s���̃o�b�t�@���擾
		auto pLineTop = pBuffer + nPos;
		int nQuote = 0;
		nPos--;
		do {
			nPos++;
			while(pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
				if (pBuffer[nPos] == '"') {
					nQuote++;
				}
				nPos++;
			}
		} while (nQuote % 2 != 0);
		//bool bCR = pBuffer[nPos] == '\r' ? true : false;
		pBuffer[nPos] = '\0';
		nPos++;

		if (auto pStr = GetToken(pLineTop, TEXT("\t"))) {
			nCount++;
			while (true) {
				if ((pStr = GetToken(NULL, TEXT("\t"))) != NULL) {
					nCount++;
				} else {
					break;
				}
			}
		}

		// [CR] [LF] �̎c����X�L�b�v
		while(pBuffer[nPos] == '\n') {
			nPos++;
		}

		if (nPos >= nLength) {
			// �o�b�t�@�̕����񂪖����Ȃ���
			break;
		}
	}
	return nCount > 1 ? false : true;
}

CString CMySuperGrid::unquoteForComment(CString str) /* RockDance 124 */
{
	if (str.IsEmpty()) {
		return str;
	}
	if (str[0] == '"') {
		str.Delete(0);
		str.Delete(str.GetLength()-1);
		str.Replace(TEXT("\"\""), TEXT("\"")); // "" �� " �ɂ���
	}
	return str;
}

int CMySuperGrid::GetFolderFileCount(int nIndex) /* STEP 007 */
{
	if (nIndex < 0) return 0;

	CTreeItem	*pItemParent = GetTreeItem(nIndex);
	CItemInfo	*pInfo = GetData(pItemParent);
	if (!((int)pInfo->GetLParam() < 0)) {
		return 0;
	}
	return GetFolderFileCount(pItemParent, pItemParent, 0);
}

int CMySuperGrid::GetFolderFileCount(CTreeItem* pTop, CTreeItem *pItemParent, int fileCount) /* STEP 007 */
{
	//if (!g_bOptShowTotalParent)	return; /* STEP 004 */
	//POSITION pos = pItemParent ? GetHeadPosition(pItemParent) : GetRootHeadPosition();
	int count = fileCount;
	POSITION pos = GetHeadPosition(pItemParent);
	while(pos != NULL) {
		//CTreeItem	*pParent = (CTreeItem*)(pItemParent ? GetNextChild(pItemParent, pos) : GetNextRoot(pos));
		CTreeItem	*pParent = (CTreeItem*)GetNextChild(pItemParent, pos);
		CTreeItem	*pItem = pParent;
		CItemInfo	*lp = GetData(pParent);
		if ((int)lp->GetLParam() < 0) {
			//  �t�@���_
			//count = GetFolderFileCount(pTop, pParent, count);
			continue; // �T�u�t�H���_�͑ΏۂƂ��Ȃ�
		} else {
			//FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lp->GetLParam());
			CTreeItem* pTotalParent = pItemParent;
			while (pTotalParent != NULL) {
				count++;
				if (pTotalParent == pTop) {
					break;
				}
				pTotalParent = GetParentItem(pTotalParent);
			}
		}
	}
	return count;
}

void CMySuperGrid::CalcSum(int nIndex) /* RockDance 128 */
{
	if (nIndex < 0) return;

	CTreeItem	*pItemParent = GetTreeItem(nIndex);
	CItemInfo	*pInfo = GetData(pItemParent);
	if (!((int)pInfo->GetLParam() < 0)) {
		return;
	}
	int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
	if (nColNum >= 0) {
		pInfo->SetSubItemText(nColNum-1, TEXT(""));
		SetItemText(nIndex, nColNum, TEXT(""));
	}
	nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
	if (nColNum >= 0) {
		pInfo->SetSubItemText(nColNum-1, TEXT(""));
		SetItemText(nIndex, nColNum, TEXT(""));
	}
	nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
	if (nColNum >= 0) {
		pInfo->SetSubItemText(nColNum-1, TEXT(""));
		SetItemText(nIndex, nColNum, TEXT(""));
	}
	CalcSum(pItemParent, pItemParent);
}

void CMySuperGrid::CalcSum(CTreeItem* pTop, CTreeItem *pItemParent) /* RockDance 128 */
{
	//if (!g_bOptShowTotalParent)	return; /* STEP 004 */
	//POSITION pos = pItemParent ? GetHeadPosition(pItemParent) : GetRootHeadPosition();
	POSITION pos = GetHeadPosition(pItemParent);
	while(pos != NULL) {
		//CTreeItem	*pParent = (CTreeItem*)(pItemParent ? GetNextChild(pItemParent, pos) : GetNextRoot(pos));
		CTreeItem	*pParent = (CTreeItem*)GetNextChild(pItemParent, pos);
		CTreeItem	*pItem = pParent;
		CItemInfo	*lp = GetData(pParent);
		if ((int)lp->GetLParam() < 0) {
			//  �t�@���_
			int		nIndex = NodeToIndex(pParent);
			int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, TEXT(""));
				SetItemText(nIndex, nColNum, TEXT(""));
			}
			nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, TEXT(""));
				SetItemText(nIndex, nColNum, TEXT(""));
			}
			nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, TEXT(""));
				SetItemText(nIndex, nColNum, TEXT(""));
			}
			CalcSum(pTop, pParent);
		} else {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lp->GetLParam());
			CTreeItem* pTotalParent = pItemParent;
			while (pTotalParent != NULL) {
				// �Ȑ���\��
				CItemInfo	*pInfo = GetData(pTotalParent);
				CString strBuffer;
				CString strLeft, strRight;
				int		nIndex = NodeToIndex(pTotalParent);
				int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
				if (nColNum >= 0) {
					strBuffer = pInfo->GetSubItem(nColNum-1);
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
						strBuffer.Format(TEXT("%d"), _tstoi(strBuffer)+1);
					} else {
						if (strBuffer.Find(TEXT("/")) != -1) {
							strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);;
							strBuffer.Format(TEXT("%d/%d"), _tstoi(strLeft) + (pTotalParent == pItemParent ? 1 : 0), _tstoi(strRight)+1);
						} else {
							if (pTotalParent == pItemParent) {
								ASSERT(0);
							} else {
								CString strOrg = strBuffer;
								strBuffer.Format(TEXT("%s/%d"), strOrg, _tstoi(strBuffer)+1);
							}
						}
					}
					pInfo->SetSubItemText(nColNum-1, strBuffer);
					SetItemText(nIndex, nColNum, strBuffer);
				}
				// ���t���ԍ��v��\��
				nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
				if (nColNum >= 0) {
					strBuffer = GetFileColumnText(fileMP3, COLUMN_PLAY_TIME);
					UINT nTotalTime = Time2Sec(strBuffer);
					strBuffer = pInfo->GetSubItem(nColNum-1);
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
						nTotalTime += Time2Sec(strBuffer);
						strBuffer.Format(_T("%d:%02d"), nTotalTime/60, nTotalTime%60);
					} else {
						if (strBuffer.Find(TEXT("/")) != -1) {
							strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
							if (pTotalParent == pItemParent) {
								UINT nTime = nTotalTime;
								nTime += Time2Sec(strLeft);
								strLeft.Format(TEXT("%d:%02d"), nTime/60, nTime%60);
							}
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);;
							nTotalTime += Time2Sec(strRight);
							strBuffer.Format(_T("%s/%d:%02d"), strLeft, nTotalTime/60, nTotalTime%60);
						} else {
							if (pTotalParent == pItemParent) {
								ASSERT(0);
							} else {
								nTotalTime += Time2Sec(strBuffer);
								CString strOrg = strBuffer;
								strBuffer.Format(_T("%s/%d:%02d"), strOrg, nTotalTime/60, nTotalTime%60);
							}
						}
					}
					pInfo->SetSubItemText(nColNum-1, strBuffer);
					SetItemText(nIndex, nColNum, strBuffer);
				}
				//�T�C�Y���v��\��
				nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
				if (nColNum >= 0) {
					strBuffer = pInfo->GetSubItem(nColNum-1);
					strBuffer.TrimRight(TEXT("MB"));
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find(TEXT("/")) == -1) {
						strBuffer.Format(TEXT("%.2fMB"), _tstof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024);
					} else {
						if (strBuffer.Find(TEXT("/")) != -1) {
							strLeft = strBuffer.Left(strBuffer.Find(TEXT("/")));
							if (pTotalParent == pItemParent) {
								strLeft.Format(TEXT("%.2fMB"), _tstof(strLeft) + (float)fileMP3->lFileSize / 1024 / 1024);
							}
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find(TEXT("/"))+1);;
							strBuffer.Format(TEXT("%s/%.2fMB"), strLeft, _tstof(strRight) + (float)fileMP3->lFileSize / 1024 / 1024);
						} else {
							if (pTotalParent == pItemParent) {
								ASSERT(0);
							} else {
								strLeft.Format(TEXT("%sMB"), strBuffer);
								strRight.Format(TEXT("%.2fMB"), _tstof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024);
								strBuffer.Format(TEXT("%s/%s"), strLeft, strRight);
							}
						}
					}
					pInfo->SetSubItemText(nColNum-1, strBuffer);
					SetItemText(nIndex, nColNum, strBuffer);
				}
				if (pTotalParent == pTop) {
					break;
				}
				pTotalParent = GetParentItem(pTotalParent);
			}
		}
	}
}

void CMySuperGrid::OnUpdateTotal()
{
	if (!g_bOptShowTotalParent)	return; /* STEP 004 */
	CalcSum(0);
}

bool CMySuperGrid::ClipboardPaste(int nPastePos, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */) /* Baja 171 */
{
	// �N���b�v�{�[�h���J���ăe�L�X�g���擾
	HANDLE	hMem;
	OpenClipboard();
	/* RockDance 124 *///hMem = ::GetClipboardData(CF_TEXT);
	bool bText = false; /* RockDance 124 */
	hMem = ::GetClipboardData(STEClipbordFormat/*CF_DSPTEXT*//* Misirlou 146 */); /* RockDance 124 */
	if (hMem == NULL) { /* RockDance 124 */
		hMem = ::GetClipboardData(CF_TEXT);
		bText = true;
	}
	CloseClipboard();

	if (hMem != NULL) {
		if (auto pBuffer = (LPTSTR)GlobalLock(hMem)) {
			/* RockDance 124 *///PasteString(pBuffer);
#ifdef FLICKERFREE
			//SetRedraw(FALSE);
#endif
			if (lstrlen(pBuffer) > 0) { /* STEP 033 */
				PasteString(pBuffer, nPastePos, bText, bAddSpace/* Baja 171 */, strAddFront, strAddBack/* FunnyCorn 187 */); /* RockDance 124 */
			}
#ifdef FLICKERFREE
			//SetRedraw(TRUE);
#endif
			GlobalUnlock(hMem);
		}
	}

	return(true);

}

void CMySuperGrid::PasteString(LPTSTR sBuffer, int nPastePos, bool bText, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */) /* Baja 171 */
{
	bool bSaveLoadFileAdjustColumn = g_bOptLoadFileAdjustColumn; /* RockDance 130 */
	g_bOptLoadFileAdjustColumn = false; /* RockDance 130 */

	int		sx, sy, ex, ey;
	if (IsRangeSelected() == false) {
		if (GetSelectedItem() == -1) return;	// �G���[
		if (GetCurSubItem() < COLUMN_MIN) return;		// �G���[
		// �͈͑I�𖳂�
		sx = GetCurSubItem();
		sy = GetSelectedItem();
		ex = ey = 99999;
	} else {
		// �͈͑I������
		int		nTemp;
		sx = m_posMultiSelect[0].x;
		sy = m_posMultiSelect[0].y;
		ex = m_posMultiSelect[1].x;
		ey = m_posMultiSelect[1].y;
		if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
		if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}
		if (sx <= COLUMN_MIN) sx = COLUMN_MIN;

		// �Ō�̉��s����菜��
		int		nLen = lstrlen(sBuffer);
		while(nLen > 0 && (sBuffer[nLen-1] == '\r' || sBuffer[nLen-1] == '\n')) {
			nLen--;
			sBuffer[nLen] = '\0';
		}
		// �^�u�����s���܂܂�Ă��邩�H
		 /* RockDance 124 *///if (strchr(sBuffer, '\t') == NULL && strchr(sBuffer, '\n') == NULL) {
		if ((bText == true && _tcschr(sBuffer, TEXT('\t')) == NULL && _tcschr(sBuffer, TEXT('\n')) == NULL) || (bText == false && isOneCellClipboard(sBuffer))) {  /* RockDance 124 */
			CString	strBuffer = sBuffer;
			// �܂܂�Ă��Ȃ��ꍇ�́A�I������Ă���͈͑S�ĂɃy�[�X�g
			for (int nItem = sy; nItem <= ey; nItem++) {
				CTreeItem	*pItem = GetTreeItem(nItem);
				if (pItem != NULL && IsItemFile(pItem) == TRUE) {
					CItemInfo	*pItemInfo = GetData(pItem);
					for (int nColumn = sx; nColumn <= ex; nColumn++) {
						// �Z���ɕ������ݒ�
						/* RockDance 124 *///ChangeSubItemText(nItem, nColumn, strBuffer, nPastePos/* FreeFall 052 */);
						if (bText) { /* RockDance 124 */
							ChangeSubItemText(nItem, nColumn, strBuffer, nPastePos, bAddSpace, strAddFront, strAddBack);
						} else {
							ChangeSubItemText(nItem, nColumn, unquoteForComment(strBuffer), nPastePos, bAddSpace, strAddFront, strAddBack);
						}
#ifdef FLICKERFREE
						//InvalidateItemRect(nItem);
#endif
					}
				}
			}
#ifdef FLICKERFREE
			InvalidateRangeItem(sx, sy, ex, ey);
			InvalidateRangeItem(0, sy, 0, ey);
#endif
			/* RockDance 130 */
			g_bOptLoadFileAdjustColumn = bSaveLoadFileAdjustColumn;
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				if (g_bOptLoadFileAdjustColumn) AutoSizeColumns(nColumn);
			}
			return;
		}
	}

#ifdef FLICKERFREE
	SetRedraw(FALSE);
#endif

	CString	strBuffer(sBuffer);
	int		nLength = strBuffer.GetLength();
	if (nLength == 0) return;

	if (ey >= GetItemCount()) ey = GetItemCount() - 1;

	// �I��͈͂̃Z���̓��e�𕶎���ɕϊ�
	auto pBuffer = strBuffer.GetBuffer(nLength);
	int		nPos = 0;
	int		nEndColumn = 0; /* RockDance 130 */
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItem);

			// �P�s���̃o�b�t�@���擾
			auto pLineTop = pBuffer + nPos;
			if (bText == false ) { /* RockDance 124  */
				int nQuote = 0;
				nPos--;
				do {
					nPos++;
					while(pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
						if (pBuffer[nPos] == '"') {
							nQuote++;
						}
						nPos++;
					}
					if (nPos > nLength) {
						// ������Ȃ�����
						ASSERT(0);
						break;
					}
				} while (nQuote % 2 != 0);
			} else {
				while(pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
					nPos++;
				}
			}
			/* RockDance 124 *//*
			while(pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
				nPos++;
			}*/
			pBuffer[nPos] = '\0';
			nPos++;
			//while(pBuffer[nPos] != '\r' && pBuffer[nPos] != '\0') {
			//	nPos++;
			//}

			if (auto pStr = GetToken(pLineTop, TEXT("\t"))) {
				for (int nColumn = sx; nColumn <= ex; nColumn++) {
					nEndColumn = nColumn; /* RockDance 130 */
					// �Z���ɕ������ݒ�
					/* RockDance 124 *///ChangeSubItemText(nItem, nColumn, pStr, nPastePos/* FreeFall 052 */);
					if (bText) { /* RockDance 124 */
						ChangeSubItemText(nItem, nColumn, pStr, nPastePos, bAddSpace, strAddFront, strAddBack);
					} else {
						ChangeSubItemText(nItem, nColumn, unquoteForComment(CString(pStr)), nPastePos, bAddSpace, strAddFront, strAddBack);
					}
#ifdef FLICKERFREE
					//InvalidateItemRect(nItem);
#endif

					// ���̃Z���̕�������擾
					if ((pStr = GetToken(NULL, TEXT("\t"))) == NULL) break;
				}
			}

			// [CR] [LF] �̎c����X�L�b�v
			while(pBuffer[nPos] == '\n') {
				nPos++;
			}

			if (nPos >= nLength) {
				// �o�b�t�@�̕����񂪖����Ȃ���
				break;
			}
		}
	}
#ifdef FLICKERFREE
	InvalidateRangeItem(sx, sy, ex, ey);
	InvalidateRangeItem(0, sy, 0, ey);
#endif
	/* RockDance 130 */
	g_bOptLoadFileAdjustColumn = bSaveLoadFileAdjustColumn;
	for (int nColumn = sx; nColumn <= nEndColumn; nColumn++) {
		if (g_bOptLoadFileAdjustColumn) AutoSizeColumns(nColumn);
	}
#ifdef FLICKERFREE
	SetRedraw(TRUE);
#endif
}

void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, LPCTSTR sUpdateText, int nPos, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */, bool bUpdateInternal/* STEP 037 */)
{
	if (iItem != -1 && iSubItem > 0 && sUpdateText/* sText FreeFall 052 */ != NULL) {
		CTreeItem	*pSelItem = GetTreeItem(iItem);

		if (pSelItem != NULL) {
			CItemInfo	*pInfo = GetData(pSelItem);

			// �ύX�s�̃Z���̏ꍇ�́A��������
			CONTROLTYPE ctrlType;
			if (pInfo->GetControlType(iSubItem-1, ctrlType)) {
				if (ctrlType == invalid && !bUpdateInternal) {
					return;
				}
			}

			// �t�H���_
			if ((int)pInfo->GetLParam() < 0) return;

			CString sText = sUpdateText;
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			CString strOrgText = GetFileColumnText(fileMP3, g_nColumnTypeList[iSubItem]);
			switch (nPos) {
			case 0:
				sText = strAddFront + sUpdateText + strAddBack;
				break;
			case 1:
				sText = strAddFront + sUpdateText + strAddBack + ((bAddSpace ? TEXT(" ") : TEXT("")) +  strOrgText);
				break;
			case 2:
				sText = strOrgText + (bAddSpace ? TEXT(" ") : TEXT("")) + strAddFront + sUpdateText + strAddBack;
				{/* Baja 158 */
					/*
					CString sText2 = strOrgText;
					sText = sUpdateText;
					sText.Insert(0, sText2);
					*/
				}
				break;
			}
			CString		strText;

			// ���������`�F�b�N����
			int		nLimit = OnGetColumLimitText(iItem, iSubItem);
			if (nLimit < (int)lstrlen(sText)) {
				// ���������ő啶�����Ɏ��܂�悤�ɒ���
				std::vector<TCHAR> sBuffer(nLimit+1);
				StringCopyN(sBuffer.data(), sText, nLimit);
				sBuffer[nLimit] = '\0';
				strText = sBuffer.data();
			} else {
				// �ő啶�����ȉ��Ȃ̂ł��̂܂�
				strText = sText;
			}
			strText.TrimRight();

			switch(g_nColumnTypeList[iSubItem]) {
			case COLUMN_FILE_NAME:		// �t�@�C����
				{
					// ���O�ɂ��Ȃ���'\','/'��_tsplitpath()�Ńp�X��؂�Ƃ��ĔF������Ă��܂� /* WildCherry2 077 */
					if (g_nOptCheckFileName == FILENAME_CONV_MULTIBYTE) {
						// �t�@�C�����Ɏg�p�ł��Ȃ�������u��
						strText = ReplaceFileName(fileMP3, strText);
					}
					// �g���q�������ꍇ�́A�g���q��t����
					TCHAR	extOrg[_MAX_EXT];
					TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
					_tsplitpath(fileMP3->strFullPathName, NULL, NULL, NULL, extOrg);
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
					if (/*strlen(ext) != 4 ||*/ !CheckFileExt(fileMP3, ext)
					)/*
					||  !(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						  strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						  strcmpi(ext, ".asf") == 0 ||
						  strcmpi(ext, ".ape") == 0 ||
						  strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						  strcmpi(ext, ".ogg") == 0)) {//�C�� by Kobarin
					*/
						strText += extOrg;
					/*}*/

					// ��Ɉړ� /* WildCherry2 077 */
					// �g���q�ϊ��̂��ߍēx�Ăяo�� /* STEP 006 */
					if (g_nOptCheckFileName == FILENAME_CONV_MULTIBYTE) {
						// �t�@�C�����Ɏg�p�ł��Ȃ�������u��
						strText = ReplaceFileName(fileMP3, strText);
					}
				}
				if (fileMP3->strFileName != strText) {
					fileMP3->strFileName = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_FILE_TIME:		// �t�@�C���X�V��
				if (g_bOptKeepTimeStamp == true) {
					CTime	time = fileMP3->time;
					// ���͂��ꂽ�^�C���X�^���v�����
					if (strText.GetLength() > 0) {
						std::vector<TCHAR> sBuffer(strText.GetLength()+1);
						LPTSTR ptr;
						lstrcpy(sBuffer.data(), sText);
						int		nYear = 2000, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
						if ((ptr = _tcstok(sBuffer.data(), TEXT("/"))) != NULL) {		// �N
							nYear = _tstoi(ptr);
						}
						if (ptr && (ptr = _tcstok(NULL, TEXT("/"))) != NULL) {	// ��
							nMonth = _tstoi(ptr);
						}
						if (ptr && (ptr = _tcstok(NULL, TEXT(" "))) != NULL) {	// ��
							nDay = _tstoi(ptr);
						}
						if (ptr && (ptr = _tcstok(NULL, TEXT(":"))) != NULL) {	// ��
							nHour = _tstoi(ptr);
						}
						if (ptr && (ptr = _tcstok(NULL, TEXT(":"))) != NULL) {	// ��
							nMin = _tstoi(ptr);
						}
						if (ptr && (ptr = _tcstok(NULL, TEXT(""))) != NULL) {	// �b
							nSec = _tstoi(ptr);
						}
						// ���͂��ꂽ�l�𒲐�
						if (nYear < 1971) nYear = 1971;
						// ���{���� 1970 �N�����A1970/01/01 00:00:00 ���� CTime ��
						//   Assert ����������̂ŁA1971 �N����Ƃ���
						if (nYear > 2037) nYear = 2037;
						if (nMonth <  1) nMonth = 1;
						if (nMonth > 12) nMonth = 12;
						if (nDay <  1) nDay = 1;
						if (nDay > 31) nDay = 31;
						if (nHour <  0) nHour = 0;
						if (nHour > 23) nHour = 23;
						if (nMin <  0) nMin = 0;
						if (nMin > 59) nMin = 59;
						if (nSec <  0) nSec = 0;
						if (nSec > 59) nSec = 59;
						//nSec &= ~1;
						// ���͂��ꂽ���Ԃ�ݒ�
						time = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
					}

					// �X�V�H
					if (fileMP3->time != time) {
						fileMP3->time = time;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
					strText.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
					               fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
					               fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
				}
				break;

			case COLUMN_TRACK_NAME:		// �g���b�N��
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strTrackNameSI != strText) {
						fileMP3->strTrackNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					if (fileMP3->strTrackName != strText) {
						fileMP3->strTrackName = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_ARTIST_NAME:	// �A�[�e�B�X�g��
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strArtistNameSI != strText) {
						fileMP3->strArtistNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					if (fileMP3->strArtistName != strText) {
						fileMP3->strArtistName = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_ALBUM_NAME:		// �A���o����
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strAlbumNameSI != strText) {
						fileMP3->strAlbumNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					if (fileMP3->strAlbumName != strText) {
						fileMP3->strAlbumName = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_TRACK_NUMBER:	// �g���b�N�ԍ�
				if (!IsEditTrackNumberSIF(fileMP3) && !CFileMP3::IsNumeric(strText)) {
					int		nTrackNumber;
					nTrackNumber = _tstoi(CFileMP3::GetIntTrackNo(strText));
					if (nTrackNumber < 0) nTrackNumber = 0;
					if (nTrackNumber > 255) nTrackNumber = 254;
					strText.Format(TEXT("%d"), nTrackNumber);
				}
				if (IsEditTrackNumberSIF(fileMP3)) {
					if (fileMP3->strTrackNumberSI != strText) {
						fileMP3->strTrackNumberSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					int		nTrackNumber;
					if (strText.IsEmpty() != TRUE) {
						nTrackNumber = _tstoi(CFileMP3::GetIntTrackNo(strText));
						if (nTrackNumber < 0) nTrackNumber = 0;
						if (nTrackNumber > 255) nTrackNumber = 254;
						if (nTrackNumber == 0) {
							nTrackNumber = 0xFF;
							strText = TEXT("");
						} else if (!IsEditTrackNumberSIF(fileMP3)) {
							strText.Format(TEXT("%d"), nTrackNumber);
						}
					} else {
						nTrackNumber = 0xFF;
					}
					if (fileMP3->strTrackNumber != strText || fileMP3->byTrackNumber != nTrackNumber) {
						fileMP3->strTrackNumber = strText;
						fileMP3->byTrackNumber = nTrackNumber;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_DISK_NUMBER:	// �f�B�X�N�ԍ�
				if (!IsEditFieldSIF(fileMP3) && !CFileMP3::IsNumeric(strText)) {
					int		nDiskNumber;
					nDiskNumber = _tstoi(CFileMP3::GetIntDiskNo(strText));
					if (nDiskNumber < 0) nDiskNumber = 0;
					if (nDiskNumber > 255) nDiskNumber = 254;
					strText.Format(TEXT("%d"), nDiskNumber);
				}
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strDiskNumberSI != strText) {
						fileMP3->strDiskNumberSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					int		nDiskNumber;
					if (strText.IsEmpty() != TRUE) {
						nDiskNumber = _tstoi(CFileMP3::GetIntDiskNo(strText));
						if (nDiskNumber < 0) nDiskNumber = 0;
						if (nDiskNumber > 255) nDiskNumber = 254;
						if (nDiskNumber == 0) {
							nDiskNumber = 0xFF;
							strText = TEXT("");
						} else if (!IsEditFieldSIF(fileMP3)) {
							strText.Format(TEXT("%d"), nDiskNumber);
						}
					} else {
						nDiskNumber = 0xFF;
					}
					if (fileMP3->strDiskNumber != strText || fileMP3->byDiskNumber != nDiskNumber) {
						fileMP3->strDiskNumber = strText;
						fileMP3->byDiskNumber = nDiskNumber;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_YEAR:			// �����[�X�N��
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strYearSI != strText) {
						fileMP3->strYearSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					if (fileMP3->strYear != strText) {
						fileMP3->strYear = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_GENRE:			// �W�������ԍ�
				if (!IsEditGenreSIF(fileMP3) && IsUserGenre(strText)) {
					int		nGenre = GetGenreCode(strText);
					strText = GetGenreNameSIF(nGenre);
				}
				if (IsEditGenreSIF(fileMP3)) {
					if (fileMP3->strGenreSI != strText) {
						fileMP3->strGenreSI = strText;
						fileMP3->bModifyFlag = TRUE;
					}
				} else {
					int		nGenre = GetGenreCode(strText);
					if (strText.IsEmpty()) {		// ��̕�����(=�N���A)
						nGenre = 0xff;
						strText = GetGenreNameSIF(nGenre);
					}
					if (fileMP3->strGenre != strText) {
						fileMP3->byGenre = (BYTE)nGenre;
						if (!IsEditGenreSIF(fileMP3)) {
							fileMP3->strGenre = GetGenreNameSIF(nGenre);//strText;
						} else {
							fileMP3->strGenre = strText;
						}
						fileMP3->bModifyFlag = TRUE;
					}
				}
				break;

			case COLUMN_COMMENT:		// �R�����g
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strCommentSI != strText) {
						fileMP3->strCommentSI = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				} else {
					if (fileMP3->strComment != strText) {
						fileMP3->strComment = strText;
						fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
					}
				}
				break;

			case COLUMN_COPYRIGHT:		// ���쌠
				if (fileMP3->strCopyrightSI != strText) {
					fileMP3->strCopyrightSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_ENGINEER:		// �G���W�j�A(�����)
				if (fileMP3->strEngineerSI != strText) {
					fileMP3->strEngineerSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_SOURCE:			// �\�[�X
				if (fileMP3->strSourceSI != strText) {
					fileMP3->strSourceSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_SOFTWARE:		// �\�t�g�E�F�A
				if (fileMP3->strSoftwareSI != strText) {
					fileMP3->strSoftwareSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_KEYWORD:		// �L�[���[�h
				if (fileMP3->strKeywordSI != strText) {
					fileMP3->strKeywordSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_TECHNICIAN:		// �Z�p��
				if (fileMP3->strTechnicianSI != strText) {
					fileMP3->strTechnicianSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_LYRIC:			// �̎�
				if (fileMP3->strLyricSI != strText) {
					fileMP3->strLyricSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_COMMISSION:		// �R�~�b�V����
				if (fileMP3->strCommissionSI != strText) {
					fileMP3->strCommissionSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_WRITER:		// �쎌 /* ADD MIMURA */
				if (fileMP3->strWriterSI != strText) {
					fileMP3->strWriterSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_COMPOSER:		// ��� /* Baja 154 */
				if (fileMP3->strComposerSI != strText) {
					fileMP3->strComposerSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_ALBM_ARTIST:	// Orig.�A�[�e�B�X�g /* Baja 154 */
				if (fileMP3->strAlbmArtistSI != strText) {
					fileMP3->strAlbmArtistSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_ORIG_ARTIST:	// Orig.�A�[�e�B�X�g /* Baja 154 */
				if (fileMP3->strOrigArtistSI != strText) {
					fileMP3->strOrigArtistSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_URL:		// URL /* Baja 154 */
				if (fileMP3->strURLSI != strText) {
					fileMP3->strURLSI = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_ENCODEST:		// �G���R�[�h�����l /* Baja 154 */
				if (fileMP3->strEncodest != strText) {
					fileMP3->strEncodest = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;

			case COLUMN_OTHER:		// ���̑�
				if (fileMP3->strOther != strText) {
					fileMP3->strOther = strText;
					fileMP3->bModifyFlag = TRUE;	// �ҏW�t���O��ݒ肷��
				}
				break;
			}

			// �Z�����e���X�V
			if (!bUpdateInternal) { /* STEP 037 */
				pInfo->SetSubItemText(iSubItem-1, strText);
				UpdateData(pSelItem, pInfo);
				SetItemText(iItem, iSubItem, strText);

				// �J�������̎�������
				if (g_bOptLoadFileAdjustColumn) AutoSizeColumns(iSubItem);
			}
		}
	}
}

void CMySuperGrid::ClipboardCopyFormat(USER_COPY_FORMAT_FORMAT *pForm) /* FunnyCorn 175 */
{
	if (pForm->strFormat.IsEmpty()) {
		// ��������
		return;
	}

	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	/*
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}
	*/

	// �Œ蕶�������͂�����
	if (pForm->strFormat.Find(TEXT("%STRING%")) != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// �L�����Z��
			return;
		}
		pForm->strFixString = dialog.m_strText;
	}

	CStringArray strMoveFile;
	// �A�ԕϊ������̐ݒ�
	//char	*sNumFormat = "%01d";
	//sNumFormat[2] = '0' + pForm->nColumnCount;

	//int		nNumber = pForm->nInitNumber;
	CString strClipboard = TEXT("");
	for (int i=0;i<nCount;i++) {
		int		nIndex = arrayList[i];
		int		nNumber = (int)GetLParamFromIndex(nIndex);
		if (!(nNumber >= 0))	continue;
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);

		// �t�@�C��������g���q���������������擾
		CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
		TCHAR	sFileName[_MAX_FNAME];
		_tsplitpath(strFileName, NULL, NULL, sFileName, NULL);
		CString strExt;
		{
			TCHAR	ext[_MAX_EXT];
			_tsplitpath(fileMP3->strFileName, NULL, NULL, NULL, ext);
			strExt = &ext[1];
			strExt.MakeUpper();
		}

		CString	strText = pForm->strFormat;
		auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));
		auto[strDiskNumber, strDiskNumber2, strDiskNumber3] = GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding(TEXT("\r")));

		// �������g���ĕ�������쐬
		strText = StrReplace(strText, TEXT("%FILE_NAME%")    , sFileName);
		strText = StrReplace(strText, TEXT("%TRACK_NAME%")   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%ALBUM_NAME%")   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%ARTIST_NAME%")  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER%") , strTrackNumber);
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER2%"), strTrackNumber2);
		strText = StrReplace(strText, TEXT("%TRACK_NUMBER3%"), strTrackNumber3);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER%") , strDiskNumber);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER2%"), strDiskNumber2);
		strText = StrReplace(strText, TEXT("%DISK_NUMBER3%"), strDiskNumber3);
		strText = StrReplace(strText, TEXT("%RELEASE_YEAR%") , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%COMMENT%")      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding(TEXT("\r"))/* BeachMonster 089 */);
		strText = StrReplace(strText, TEXT("%GENRE%")        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding(TEXT("\r")));
		strText = StrReplace(strText, TEXT("%STRING%")       , pForm->strFixString);
		// SIF �̍���
		strText = StrReplaceSIF(strText, fileMP3);

		// �������t����
		CString strBuffer;
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format(TEXT("%02d:%02d"), fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
		} else {
			strBuffer = "     ";
		}
		strText = StrReplace(strText, TEXT("%TOTAL_SEC%"), strBuffer);
		// �������t���� /* STEP 020 */
		if (fileMP3->nPlayTime >= 0 && fileMP3->nPlayTime/60/60 > 0) {
			strBuffer.Format(TEXT("%d:%02d:%02d"), fileMP3->nPlayTime/60/60, (fileMP3->nPlayTime/60)%60, fileMP3->nPlayTime%60);
		}
		strText = StrReplace(strText, TEXT("%TOTAL_SEC3%"), strBuffer);
		// �������t����(�b)
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format(TEXT("%d"), fileMP3->nPlayTime);
		} else {
			strBuffer = "     ";
		}
		strText = StrReplace(strText, TEXT("%TOTAL_SEC2%"), strBuffer);
		// �t�H�[�}�b�g
		{ /* STEP 012 */
			CString strFormat = fileMP3->strVisualFormat;
			if (strFormat.GetLength() > 0 && fileMP3->strAudioFormat.GetLength() > 0) {
				strFormat += "\n";
			}
			strFormat += fileMP3->strAudioFormat;
			strText = StrReplace(strText, TEXT("%FORMAT%"), strFormat);
		}

		// ����R�[�h�i�ꕔ�j���X�y�[�X�ɒu������
		strText = StrReplace(strText, TEXT("\n"), TEXT(" "));
		strText = StrReplace(strText, TEXT("\r"), TEXT(" "));
		//strText = StrReplace(strText, "\t", " ");

		if (!strClipboard.IsEmpty()) {
			strClipboard += "\r\n";
		}
		strClipboard += strText;
	}
	LPTSTR pBuffer;
	HGLOBAL hMem;
	hMem = GlobalAlloc(GHND|GMEM_DDESHARE, strClipboard.GetLength()+1);
	pBuffer = (LPTSTR)GlobalLock(hMem);
	lstrcpy(pBuffer, strClipboard);
	GlobalUnlock(hMem);

	// �N���b�v�{�[�h���J���ăR�s�[
	OpenClipboard();
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, (HANDLE)hMem);
	CloseClipboard();

	return;
}

bool CMySuperGrid::DeleteCharSpace(int /*nPos*/) /* FunnyCorn 177 */
{
	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// �͈͑I�𖳂�
	}

	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				CString	strText;
				TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
				bool	bKeepExt = false;

				// �Z���̃e�L�X�g���擾
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// �t�@�C�����̏ꍇ�A�g���q���S�p�ɕϊ�����Ȃ��悤�ɂ���
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, LPCTSTR ext);
					if (/*strlen(ext) == 4 && */CheckFileExt(fileMP3, ext)
					) {/*
					&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						 strcmpi(ext, ".asf") == 0 ||
						 strcmpi(ext, ".ape") == 0 ||
						 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						 strcmpi(ext, ".ogg") == 0)) {
					*/
						// �g���q����菜��
						strText = fname;
						bKeepExt = true;
					}
				}


				int nLength;
				do {
					nLength = strText.GetLength();
					strText.TrimLeft(TEXT(" "));
					strText.TrimLeft(TEXT("\t"));
					if (strText.GetLength() > 1) {
						CString strChar = strText[0];
						strChar += strText[1];
						if (strChar == "�@") {
							strText.Delete(0, 2);
						}
					}
				} while (nLength != strText.GetLength());
				do {
					nLength = strText.GetLength();
					strText.TrimRight(TEXT(" "));
					strText.TrimRight(TEXT("\t"));
					if (strText.GetLength() > 1) {
						CString strChar = strText[strText.GetLength()-2];
						strChar += strText[strText.GetLength()-1];
						if (strChar == "�@") {
							strText.Delete(strText.GetLength()-2, 2);
						}
					}
				} while (nLength != strText.GetLength());
				// ��菜���Ă����g���q��t���Ȃ���
				if (bKeepExt) {
					strText += ext;
				}

				ChangeSubItemText(nItem, nColumn, strText);
				InvalidateItemRect(nItem);
			}
		}
	}
	return(true);
}

CString CMySuperGrid::MakeFormatFileBody(FILE_MP3	*fileMP3, const CString &strBody, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml)
{
	auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER));

	auto[strDiskNumber, strDiskNumber2, strDiskNumber3] = GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER));

	CString	strFileSize, strFileSizeByte;
	strFileSize.Format(TEXT("%.2fMB"), (float)fileMP3->lFileSize / 1024 / 1024);
	strFileSizeByte.Format(TEXT("%d"), fileMP3->lFileSize);
	CString	strFileTime;
	strFileTime.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
					  fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
					  fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
	CString	strFileTimeCreate;
	strFileTimeCreate.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
							 fileMP3->timeCreate.GetYear(), fileMP3->timeCreate.GetMonth(), fileMP3->timeCreate.GetDay(),
							 fileMP3->timeCreate.GetHour(), fileMP3->timeCreate.GetMinute(), fileMP3->timeCreate.GetSecond());

	CString	strFileNumber;
	strFileNumber.Format(TEXT("%d"), 1 + pStatus->nFileNumber);

	CString	strPlayPath;
	strPlayPath = ConvFileName(fileMP3->strFullPathName, pStatus->strDirectory);

	CString	strLyricFile;
	strLyricFile = CFileMP3::SearchLyricsFile(fileMP3);

	 /* Baja 162 */
	CString strExt;
	{
		TCHAR	ext[_MAX_EXT];
		_tsplitpath(fileMP3->strFileName, NULL, NULL, NULL, ext);
		strExt = &ext[1];
		strExt.MakeUpper();
	}
	/* Rumble 191 */
	CString strFileNameShort;
	{
		DWORD size = GetShortPathName(fileMP3->strFilePath + fileMP3->strFileName, NULL, 0);
		GetShortPathName(fileMP3->strFilePath + fileMP3->strFileName, strFileNameShort.GetBufferSetLength(size+1), size+1);
		strFileNameShort.ReleaseBuffer();
		TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
		_tsplitpath(strFileNameShort, NULL, NULL, fname, ext);
		strFileNameShort = fname;
		strFileNameShort += ext;
	}
	/* Rumble 191 */
	CString strPathNameShort;
	{
		DWORD size = GetShortPathName(fileMP3->strFilePath, NULL, 0);
		GetShortPathName(fileMP3->strFilePath, strPathNameShort.GetBufferSetLength(size+1), size+1);
		strPathNameShort.ReleaseBuffer();
	}
	/* Rumble 191 */
	CString strFullPathShort;
	{
		DWORD size = GetShortPathName(fileMP3->strFullPathName, NULL, 0);
		GetShortPathName(fileMP3->strFullPathName, strFullPathShort.GetBufferSetLength(size+1), size+1);
		strFullPathShort.ReleaseBuffer();
	}
	/* Rumble 191 */
	CString	strPlayPathShort;
	{
		DWORD size = GetShortPathName(fileMP3->strFullPathName, NULL, 0);
		GetShortPathName(fileMP3->strFullPathName, strPlayPathShort.GetBufferSetLength(size+1), size+1);
		strPlayPathShort.ReleaseBuffer();
		CString strDirShort;
		size = GetShortPathName(pStatus->strDirectory, NULL, 0);
		GetShortPathName(pStatus->strDirectory, strDirShort.GetBufferSetLength(size+1), size+1);
		strDirShort.ReleaseBuffer();
		strPlayPathShort = ConvFileName(strPlayPathShort, strDirShort);
	}
	/* Rumble 191 */
	CString	strLyricFileShort;
	{
		DWORD size = GetShortPathName(strLyricFile, NULL, 0);
		GetShortPathName(strLyricFile, strLyricFileShort.GetBufferSetLength(size+1), size+1);
		strLyricFileShort.ReleaseBuffer();
	}


	// �������g���ĕ�������쐬
	CString	strText = strBody;
	strText = StrReplaceEx(strText, TEXT("%FILE_NUMBER%")     , strFileNumber, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_TYPE%")       , GetFileType(fileMP3), bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_NAME%")       , ConvHTML(fileMP3->strFileName, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_NAME_SHORT%") , ConvHTML(strFileNameShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%PATH_NAME%")       , ConvHTML(fileMP3->strFilePath, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%PATH_NAME_SHORT%") , ConvHTML(strPathNameShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%FULL_PATH%")       , ConvHTML(fileMP3->strFullPathName, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FULL_PATH_SHORT%") , ConvHTML(strFullPathShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%PLAY_PATH%")       , ConvHTML(strPlayPath, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%PLAY_PATH_SHORT%")       , ConvHTML(strPlayPathShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%TRACK_NAME%")      , ConvHTML(GetFileColumnText(fileMP3, COLUMN_TRACK_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%ALBUM_NAME%")      , ConvHTML(GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%ARTIST_NAME%")     , ConvHTML(GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TRACK_NUMBER%")    , strTrackNumber, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TRACK_NUMBER2%")   , strTrackNumber2, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TRACK_NUMBER3%")   , strTrackNumber3, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%DISK_NUMBER%")    , strDiskNumber, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%DISK_NUMBER2%")   , strDiskNumber2, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%DISK_NUMBER3%")   , strDiskNumber3, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%RELEASE_YEAR%")    , ConvHTML(GetFileColumnText(fileMP3, COLUMN_YEAR), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%COMMENT%")         , ConvHTML(GetFileColumnText(fileMP3, COLUMN_COMMENT), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%GENRE%")           , ConvHTML(GetFileColumnText(fileMP3, COLUMN_GENRE), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_SIZE%")       , strFileSize, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_SIZE_BYTE%")  , ConvNumber(strFileSizeByte), bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_TIME%")       , strFileTime, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%FILE_CTIME%")      , strFileTimeCreate, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%EXIST_LYRIC_FILE%"), strLyricFile.IsEmpty() ? TEXT("�Ȃ�") : TEXT("����"), bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%LYRIC_FILE_NAME%") , ConvHTML(strLyricFile, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%LYRIC_FILE_NAME_SHORT%") , ConvHTML(strLyricFileShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, TEXT("%FILE_EXTENSION%")  , strExt, bIsHtml); /* Baja 162 */
	// SIF �̏��
	strText = StrReplaceEx(strText, TEXT("%COPYRIGHT%")       , ConvHTML(fileMP3->strCopyrightSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%ENGINEER%")        , ConvHTML(fileMP3->strEngineerSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%SOURCE%")          , ConvHTML(fileMP3->strSourceSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%SOFTWARE%")        , ConvHTML(fileMP3->strSoftwareSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%KEYWORD%")         , ConvHTML(fileMP3->strKeywordSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TECHNICIAN%")      , ConvHTML(fileMP3->strTechnicianSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%LYRIC%")           , ConvHTML(fileMP3->strLyricSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%COMMISSION%")      , ConvHTML(fileMP3->strCommissionSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%WRITER%")		, ConvHTML(fileMP3->strWriterSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%COMPOSER%")		, ConvHTML(fileMP3->strComposerSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%ALBM_ARTIST%")	, ConvHTML(fileMP3->strAlbmArtistSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%ORIG_ARTIST%")	, ConvHTML(fileMP3->strOrigArtistSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%URL%")			, ConvHTML(fileMP3->strURLSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%ENCODEST%")		, ConvHTML(fileMP3->strEncodest, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, TEXT("%OTHER%")			, ConvHTML(fileMP3->strOther, bWriteHtml), bIsHtml); /* Conspiracy 196 */

	// �������t����
	CString strBuffer;
	if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
		strBuffer.Format(TEXT("%02d:%02d"), fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
	} else {
		strBuffer = TEXT("     ");
	}
	strText = StrReplaceEx(strText, TEXT("%TOTAL_SEC%"), strBuffer, bIsHtml);
	// �������t���� /* STEP 020 */
	if (fileMP3->nPlayTime >= 0 && fileMP3->nPlayTime/60/60 > 0) {
		strBuffer.Format(TEXT("%d:%02d:%02d"), fileMP3->nPlayTime/60/60, (fileMP3->nPlayTime/60)%60, fileMP3->nPlayTime%60);
	}
	strText = StrReplaceEx(strText, TEXT("%TOTAL_SEC3%"), strBuffer, bIsHtml);
	// �������t����(�b) /* FunnyCorn 172 */
	if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
		strBuffer.Format(TEXT("%d"), fileMP3->nPlayTime);
	} else {
		strBuffer = "     ";
	}
	strText = StrReplaceEx(strText, TEXT("%TOTAL_SEC2%"), strBuffer, bIsHtml);
	// �t�H�[�}�b�g
	{ /* STEP 012 */
		CString strFormat = fileMP3->strVisualFormat;
		if (strFormat.GetLength() > 0 && fileMP3->strAudioFormat.GetLength() > 0) {
			strFormat += "\n";
		}
		strFormat += fileMP3->strAudioFormat;
		strText = StrReplaceEx(strText, TEXT("%FORMAT%"), strFormat, bIsHtml);
	}

	if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
		pStatus->nTotalTime += fileMP3->nPlayTime; /* Baja 163 */
	}
	pStatus->fTotalSize += (float)fileMP3->lFileSize;
	pStatus->nFileNumber++;
	pStatus->nFileCount++;

	return strText;
}

CString CMySuperGrid::MakeFormatFileFoot(const CString &strFoot, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml) /* Rumble 190 */
{
	// �t�@�C������
	CString	strFileCount;
	strFileCount.Format(TEXT("%d"), pStatus->nFileCount);

	// ���v���t����
	int		nMinute = pStatus->nTotalTime / 60;
	CString	strTotalTime, strTotalTime2;
	strTotalTime.Format(TEXT("%02d:%02d"), nMinute, pStatus->nTotalTime % 60);
	if ((nMinute/60) > 0) {
		strTotalTime2.Format(TEXT("%02d:%02d:%02d"), nMinute / 60, nMinute % 60, pStatus->nTotalTime % 60);
	} else {
		strTotalTime2 = strTotalTime;
	}

	// ���v�t�@�C���T�C�Y
	CString	strTotalSize;
	strTotalSize.Format(TEXT("%.2fMB"), (float)pStatus->fTotalSize / 1024 / 1024);

	// �������g���ĕ�������쐬
	CString	strText = strFoot;
	strText = StrReplaceEx(strText, TEXT("%FILE_COUNT%"), strFileCount, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TOTAL_TIME%"), strTotalTime, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TOTAL_TIME2%"), strTotalTime2, bIsHtml);
	strText = StrReplaceEx(strText, TEXT("%TOTAL_SIZE%"), strTotalSize, bIsHtml);

	return strText;
}

CString CMySuperGrid::WriteFormatFileHeader(CFile &file, CTreeItem *pItem, const CString &strHead, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml) /* Rumble 190 */
{
	ASSERT(pItem != NULL);
	CString strText = strHead;

	if (pItem != NULL) {
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == TRUE) {
			// �t�@�C���̏ꍇ
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			strText = MakeFormatFileBody(fileMP3, strText, bIsHtml, pStatus, bWriteHtml);
		} else {
			// �t�H���_�̏ꍇ
			// �q�A�C�e����S�ď�������
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						strText = /* Hurricane 216 */ WriteFormatFileHeader(file, (CTreeItem *)GetNextChild(pSelItem, pos), strText, bIsHtml, pStatus, bWriteHtml);
					}
				}
			}
		}
	}

	return strText;
}

bool CMySuperGrid::ProcessSelectedFiles(LPCTSTR szProgressBarTitle, STEPProcessSelectedFilesCB* callback)
{
	CWaitCursor	wait;
	// �I������Ă���t�@�C����S�ď�������
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);

	// �v���O���X�o�[������
	if (szProgressBarTitle != NULL) {
		m_pDoc->StartLoadFile(szProgressBarTitle);
		m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, TEXT("���΂炭���҂�������..."));
	}

	extern	int		g_nWriteTagProcFlag;
	g_nWriteTagProcFlag = 0;
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];

		// �v���O���X�o�[�X�V
		if (szProgressBarTitle != NULL) {
			if (nCount > 1) {
				m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
			}
		}

		// �������s
		CTreeItem	*pItem = GetTreeItem(nIndex);
		CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
		if (pItem != NULL && pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			FILE_INFO info;
			info.pFileMP3 = fileMP3;
			bool result = (*callback)(&info, g_nWriteTagProcFlag, GetSafeHwnd());
			if (result == false) {
				// ���~
				break;
			}
		}
	}

	// �v���O���X�o�[�I��
	if (szProgressBarTitle != NULL) {
		m_pDoc->EndLoadFile();
	}

	m_pDoc->UpdateAllFiles(true);

	return true;
}

bool CMySuperGrid::ProcessSelectedFilesForUpdate(LPCTSTR szProgressBarTitle, STEPProcessSelectedFilesCB* callback)
{
	bool result = true;
	CWaitCursor	wait;
	// �I������Ă���t�@�C����S�ď�������
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);

	// �v���O���X�o�[������
	if (szProgressBarTitle != NULL) {
		m_pDoc->StartLoadFile(szProgressBarTitle);
		m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, TEXT("���΂炭���҂�������..."));
	}

	extern	int		g_nWriteTagProcFlag;
	g_nWriteTagProcFlag = 0;
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];

		// �v���O���X�o�[�X�V
		if (szProgressBarTitle != NULL) {
			if (nCount > 1) {
				m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
			}
		}

		// �������s
		CTreeItem	*pItem = GetTreeItem(nIndex);
		CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
		if (pItem != NULL && pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			// �X�V�����̕ۑ�
			// �ǂݎ�葮���̃`�F�b�N
			bool	bKeepTimeStamp = g_bOptKeepTimeStamp;
			CFileStatus	fileStatus;
			if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
				return false;
			}
			if (fileStatus.m_attribute & CFile::readOnly) {
				if (g_nWriteTagProcFlag != 1) {
					// �㏑���m�F�_�C�A���O���J��
					CDlgFileOverWrite	dialog;
					dialog.m_strFileName = fileMP3->strFullPathName;
					dialog.m_strSize.Format(TEXT("%ld byte"), fileStatus.m_size);
					if (fileStatus.m_mtime.GetTime() < 0) {
						dialog.m_strTimeStamp.Format(TEXT("----/--/-- --:--:--"));
					} else {
						dialog.m_strTimeStamp.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
													 fileStatus.m_mtime.GetYear(),
													 fileStatus.m_mtime.GetMonth(),
													 fileStatus.m_mtime.GetDay(),
													 fileStatus.m_mtime.GetHour(),
													 fileStatus.m_mtime.GetMinute(),
													 fileStatus.m_mtime.GetSecond());
					}
					dialog.DoModal();
					g_nWriteTagProcFlag = dialog.m_nResult;
					switch(dialog.m_nResult) {
					case 0:				// ���̃t�@�C�����㏑��
					case 1:				// �ȍ~�S�ď㏑��
						break;
					case 2:				// �X�L�b�v
					case 3:				// ���~
					default:
						return true;
					}
				}
				TRY {
					// �ǂݎ���p����������
					SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
					// �ύX��ɑ�����߂����邽�߂� bKeepTimeStamp �� true �ɂ���
					bKeepTimeStamp = true;
				}
				CATCH(CFileException, e) {
					CString	strMsg;
					strMsg.Format(TEXT("%s ���I�[�v���ł��܂���ł���"), fileMP3->strFullPathName);
					MessageBox(strMsg, TEXT("�ǂݎ���p�����̉����G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
					return false;
				}
				END_CATCH
			}

			FILE_INFO info;
			info.pFileMP3 = fileMP3;
			result = (*callback)(&info, g_nWriteTagProcFlag, GetSafeHwnd());
			if (bKeepTimeStamp) {
				if (fileMP3->time.GetTime() != -1) {
					// �t�@�C���X�V���Ԃ�ݒ�
					fileStatus.m_mtime = fileMP3->time;
					if (g_bOptSyncCreateTime) fileStatus.m_ctime = fileMP3->time;
				}
				TRY {
					CFile::SetStatus(fileMP3->strFullPathName, fileStatus);
				}
				CATCH(CFileException, e) {
					CString	strMsg;
					strMsg.Format(TEXT("%s ���I�[�v���ł��܂���ł���"), fileMP3->strFullPathName);
					MessageBox(strMsg, TEXT("�^�C���X�^���v�̍X�V�G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
					result = false;
				}
				END_CATCH
			} else {
				// �X�V��̃^�C���X�^���v���擾
				if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) != FALSE) {
					fileMP3->time = fileStatus.m_mtime;
				}
			}
			if (result == false) {
				// ���~
				break;
			}
		}
	}

	// �v���O���X�o�[�I��
	if (szProgressBarTitle != NULL) {
		m_pDoc->EndLoadFile();
	}

	m_pDoc->UpdateAllFiles(true);

	return true;
}


void CMySuperGrid::SelectTreeFile() /* STEP 013 */
{
	if (GetSelectedItem() < 0) return;

	CTreeItem	*pItem = GetTreeItem(GetSelectedItem());
	if (ItemHasChildren(pItem) == FALSE) {
		pItem = GetParentItem(pItem);
	}
	CArray <int, const int &> arrayList;
	int		nCount;
	int nIndex = NodeToIndex(pItem);
	if (nIndex < 0) return;
	nCount = MakeSelectFileArray(nIndex, arrayList);
	if (nCount == 0) return;
	SelItemRange(FALSE, -1, -1);
	for (int i=0;i<nCount;i++) {
		nIndex++;
		SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
		pItem = GetTreeItem(nIndex);
		if (ItemHasChildren(pItem) == TRUE) i--;
	}
	int iItem =GetNextItem(-1, LVNI_ALL | LVIS_FOCUSED);
	pItem = GetTreeItem(iItem);
	if (ItemHasChildren(pItem) == TRUE) {
		SetItemState(iItem+1, LVIS_FOCUSED, LVIS_FOCUSED);
	}
	OnChangeSelect();
	InvalidateSelectRangeItem();
}

void CMySuperGrid::MoveToParent() /* STEP 014 */
{
	if (GetSelectedItem() < 0) return;

	CTreeItem	*pItem = GetTreeItem(GetSelectedItem());
	pItem = GetParentItem(pItem);
	if (pItem == NULL) return;
	SelItemRange(FALSE, -1, -1);
	int nIndex = NodeToIndex(pItem);
	SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	EnsureVisible(nIndex, FALSE);
	OnChangeSelect();
	InvalidateSelectRangeItem();
}

void CMySuperGrid::MoveToNext() /* STEP 014 */
{
	if (GetSelectedItem() < 0) return;

	CTreeItem	*pItem = GetTreeItem(GetSelectedItem());
	int nIndex = NodeToIndex(pItem);
	do {
		nIndex =GetNextItem(nIndex, LVNI_ALL);
		if (nIndex < 0)	break;
		pItem = GetTreeItem(nIndex);
		if (ItemHasChildren(pItem) == TRUE) {
			SelItemRange(FALSE, -1, -1);
			SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			EnsureVisible(nIndex+1, FALSE);
			OnChangeSelect();
			InvalidateSelectRangeItem();
			break;
		}
	} while (1);

}

void CMySuperGrid::MoveToPrevious() /* STEP 014 */
{
	if (GetSelectedItem() < 0) return;

	CTreeItem	*pItem = GetTreeItem(GetSelectedItem());
	int nIndex = NodeToIndex(pItem);
	do {
		nIndex =GetNextItem(nIndex, LVNI_ABOVE);
		if (nIndex < 0)	break;
		pItem = GetTreeItem(nIndex);
		if (ItemHasChildren(pItem) == TRUE) {
			SelItemRange(FALSE, -1, -1);
			SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			EnsureVisible(nIndex, FALSE);
			OnChangeSelect();
			InvalidateSelectRangeItem();
			break;
		}
	} while (1);

}

bool CMySuperGrid::ConvUserTag2Tag(USER_CONV_FORMAT_TAG2TAG *pForm) /* STEP 034 */
{
	if (pForm->strFormat.IsEmpty()) {
		// �ϊ���������
		return false;					// �G���[
	}

	int		sx, sy, ex, ey;
	// �I��͈͂��擾
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return false;					// �͈͑I�𖳂�
	}
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			CString strTagValue = pInfo->GetSubItem(sx-1);
			ConvTagInfo(pItem, 5, pForm->strFormat, strTagValue);
		}
	}
	return true;
}

void CMySuperGrid::OnRefreshTreeItem(CTreeItem* pOldItem, CTreeItem* pNewItem) /* STEP 037 */
{
	if (IsItemFile(pOldItem) == TRUE) {
		CItemInfo	*pInfo = GetData(pOldItem);
		FILE_MP3	*pFileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
		pFileMP3->pTreeItem = pNewItem;
	}
}


int CMySuperGrid::GetIndexFromFileMP3(FILE_MP3 *pFileMP3) /* STEP 037 */
{
	CTreeItem* pItem = (CTreeItem*)pFileMP3->pTreeItem;
	return NodeToIndex(pItem);
}

bool CMySuperGrid::ConvSiFieldToId3tag()
{
	// �I������Ă���t�@�C����S�ď�������
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];
		ConvSiFieldToId3tag(GetTreeItem(nIndex));
	}
	return(true);
}

bool CMySuperGrid::ConvSiFieldToId3tag(CTreeItem *pItem)
{
	if (pItem == NULL || IsItemFile(pItem) == FALSE) return(false);

	int		nIndex = NodeToIndex(pItem);
	CItemInfo	*pInfo = GetData(pItem);
	FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

	// SI �t�B�[���h�� ID3 tag ���ɃR�s�[
	void OnConvSIFieldToID3tag(FILE_MP3* pFileMP3);
	OnConvSIFieldToID3tag(fileMP3);
	UpdateFileStatus(pItem, fileMP3);
	return(true);
}
