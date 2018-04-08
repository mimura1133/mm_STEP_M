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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* 漢字の１バイト目かどうかの検査 */
#ifndef iskanji
#define iskanji(c)		((c) >= 0x81 && (c) <= 0x9f || (c) >= 0xe0 && (c) <= 0xfc)
#endif

#define COLUMN_MIN			1

#define MINCOLWIDTH			24  /* or whatever */

const char	*g_sNameListDefault[] = {
	_T("未指定"),
	_T("Audio List"),
	_T("ファイル名"),
	_T("トラック名"),
	_T("アーティスト名"),
	_T("アルバム名"),
	_T("TrackNo"),
	_T("DiskNo"),
	_T("年号"),
	_T("ジャンル"),
	_T("コメント"),
	_T("著作権"),
	_T("製作者"),
	_T("ソース"),
	_T("ソフトウェア"),
	_T("キーワード"),
	_T("技術者"),
	_T("歌詞"),
	_T("コミッション"),
	_T("作詞者"),
	_T("作曲者"), /* Baja 154 */
	_T("Albm.アーティスト"), /* Baja 154 */
	_T("Orig.アーティスト"), /* Baja 154 */
	_T("URL"), /* Baja 154 */
	_T("エンコードした人"), /* Baja 154 */
	_T("サイズ"),
	_T("更新日"),
	_T("パス名"),
	_T("演奏時間"),
	_T("歌詞ファイル"),
	_T("ファイルの種類"),
	_T("フォーマット"), 	//_T("BitRate"), /* Conspiracy 198 */
	_T("その他"), /* Conspiracy 196 */
	_T("作成日"), /* 2003.06.19 add */
	NULL,
};

struct COLUMN_STATUS	g_columnStatus[COLUMN_MAX] = {
	{"TreeItem"  , TRUE ,  0, 128,   0, LVCFMT_LEFT },	// ツリーアイテム名(フルパス名)
	{"FileName"  , TRUE ,  1, 150,   0, LVCFMT_LEFT },	// ファイル名
	{"TrackName" , TRUE ,  2, 128,   0, LVCFMT_LEFT },	// 曲名
	{"ArtistName", TRUE ,  3, 128,   0, LVCFMT_LEFT },	// アーティスト名
	{"AlbumName" , TRUE ,  4, 128,   0, LVCFMT_LEFT },	// アルバム名／製品名
	{"TrackNo"   , TRUE ,  5,  48,  48, LVCFMT_RIGHT},	// トラック番号(ID3 tag v1.1のみ)
	{"DiskNo"    , FALSE,  6,  48,  48, LVCFMT_RIGHT},	// ディスク番号(ID3 tag v1.1のみ)
	{"Year"      , TRUE ,  7,  48,  48, LVCFMT_LEFT },	// リリース年号
	{"Genre"     , TRUE ,  8, 128,   0, LVCFMT_LEFT },	// ジャンル
	{"Comment"   , TRUE ,  9, 128,   0, LVCFMT_LEFT },	// コメント文字列
	{"Copyright" , FALSE, 10, 128,   0, LVCFMT_LEFT },	// 著作権
	{"Engineer"  , FALSE, 11, 128,   0, LVCFMT_LEFT },	// エンジニア(製作者)
	{"Source"    , FALSE, 12, 128,   0, LVCFMT_LEFT },	// ソース
	{"Software"  , FALSE, 13, 128,   0, LVCFMT_LEFT },	// ソフトウェア
	{"Keyword"   , FALSE, 14, 128,   0, LVCFMT_LEFT },	// キーワード
	{"Technician", FALSE, 15, 128,   0, LVCFMT_LEFT },	// 技術者
	{"Lyric"     , FALSE, 16, 128,   0, LVCFMT_LEFT },	// 歌詞
	{"Commission", FALSE, 17, 128,   0, LVCFMT_LEFT },	// コミッション
	{"Writer"    , FALSE, 18, 128,   0, LVCFMT_LEFT },	// 作詞
	{"Composer"  , FALSE, 19, 128,   0, LVCFMT_LEFT },	// 作曲 /* Baja 154 */
	{"AlbmArtist", FALSE, 20, 128,   0, LVCFMT_LEFT },	// Albm.アーティスト /* Baja 154 */
	{"OrigArtist", FALSE, 21, 128,   0, LVCFMT_LEFT },	// Orig.アーティスト /* Baja 154 */
	{"URL"       , FALSE, 22, 128,   0, LVCFMT_LEFT },	// URL /* Baja 154 */
	{"Encodest"  , FALSE, 23, 128,   0, LVCFMT_LEFT },	// エンコードした人/* Baja 154 */
	{"FileSize"  , TRUE , 24,  56,  56, LVCFMT_RIGHT},	// ファイルサイズ
	{"FileTime"  , FALSE, 25, 128, 128, LVCFMT_LEFT },	// ファイル更新日
	{"FilePath"  , TRUE , 26, 128, 128, LVCFMT_LEFT },	// ファイルパス名
	{"PlayTime"  , FALSE, 27, 128, 128, LVCFMT_LEFT },	// 演奏時間
	{"LyricFile" , FALSE, 28, 128, 128, LVCFMT_LEFT },	// 歌詞ファイル
	{"FileType"  , TRUE , 29, 128, 128, LVCFMT_LEFT },	// ファイルの種類
// 	{"BitRate"   , FALSE, 30, 128, 0, LVCFMT_LEFT },	// ビットレート /* Conspiracy 198 */
	{"Format"    , FALSE, 30, 128, 0, LVCFMT_LEFT },	// フォーマット /* Conspiracy 198 */
	{"Other"     , FALSE, 31, 128, 128, LVCFMT_LEFT },	// その他 /* Conspiracy 196 */
	{"FileCTime" , FALSE, 32, 128, 128, LVCFMT_LEFT },	// ファイル作成日 /* 2003.06.19 add */
};

// カラムタイプ(タグ項目) => カラム番号 への変換テーブル(InitializeGrid() で作成される)
int		g_nColumnNumberList[COLUMN_MAX];
// カラム番号 => カラムタイプ(タグ項目) への変換テーブルInitializeGrid() で作成される
int		g_nColumnTypeList[COLUMN_MAX];

/*
ツリーアイテム名, ファイル名, 曲名, アーティスト名, アルバム名, トラック番号, リリース年号, ジャンル, コメント
著作権, エンジニア(製作者), ソース, ソフトウェア, キーワード, 技術者, 歌詞, コミッション
作曲, Orig.アーティスト, URL, エンコードした人, ファイルサイズ, ファイル更新日, ファイルパス名, 演奏時間
歌詞ファイル, ファイルの種類, フォーマット, その他
*/
static	CONTROLTYPE	**g_nControlTypeTableSIF;
static	CONTROLTYPE	**g_nControlTypeTable = NULL;

// ↑と↓はプラグインから任意のタイミングで更新できるようにする

// 各項目の最大文字数
static int **g_nColMaxSIF;
static int **g_nColMax;
const char ***g_sNameList;
static UINT nCurrentFormatOfHeader = FILE_TYPE_UNKNOWN;
static bool bCurrentFormatOfHeaderIsDefault = true;

// strcnv.cpp の宣言
enum	{CONV_SUJI=1, CONV_ALPHA=2, CONV_KATA=4, CONV_KIGOU=8, CONV_ALL=15};
extern "C" {
extern	int conv_han2zens(unsigned char *, const unsigned char *, int);
extern	int conv_zen2hans(unsigned char *, const unsigned char *, int);
extern	void conv_kata2hira(unsigned char *);
extern	void conv_kata_erase_dakuon(unsigned char *);
extern	void conv_hira2kata(unsigned char *);
extern	void conv_upper(unsigned char *);
extern	void conv_lower(unsigned char *);
extern	void conv_first_upper(unsigned char *, const char *, const char *, bool);

//extern	DWORD conv_kan2hira(HWND, unsigned char *, DWORD);
//extern	void conv_romaji(HWND hwnd, unsigned char *str, unsigned char *sRomaji);
}

extern	void conv_fixed_upper_lower(unsigned char *, CStringArray&); /* STEP 040 */

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

	static char *GetToken(char *buffer, char *sToken)
	{
		static char *pBuffer = NULL;

		if (buffer != NULL) pBuffer = buffer;
		if (pBuffer == NULL) return(NULL);
		if (*pBuffer == '\0') return(NULL);

		// 文字列を検索
		char *pNow = pBuffer;
		pBuffer = strstr(pBuffer, sToken);
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

		// 文字列を検索
		char *pNow = pBuffer;
		if (*pNow == '"') {
			int nPos = 1;
			while (1) {
				// 次の行も同じセルのデータなので、終わりの " を探す
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
	static CString StrReplaceEx(CString &strOrg, const char *sKey, const char *sRep, bool bIsHtml)
	{
		if (bIsHtml && strlen(sRep) == 0) sRep = "　";
		/* BeachMonster 119 *///if (bIsHtml)  return(StrReplace(StrReplace(strOrg, sKey, sRep), "\r\n", "<BR>")); /* BeachMonster 089 */
		return(StrReplace(strOrg, sKey, sRep));
	}

	static	CString	ConvFileName(CString strFullPath, CString strBaseDir)
	{
		CString	strBuffer;
		if (_strnicmp(strFullPath, strBaseDir, strBaseDir.GetLength()) == 0) {
			// 相対パスに変換可能(パスが一致)
			strBuffer.Format("%s", strFullPath.Right(strFullPath.GetLength() - strBaseDir.GetLength()));
		} else if (strFullPath[1] == ':' && _strnicmp(strFullPath, strBaseDir, 1) == 0) {
			// 相対パスに変換可能(ドライブ名だけ一緒)
			strBuffer.Format("%s", strFullPath.Right(strFullPath.GetLength() - 2));
		} else {
			// 相対パスに変換不可能
			strBuffer.Format("%s", strFullPath);
		}
		return(strBuffer);
	}


	// =============================================
	//  CharComp
	//  概要  : １文字比較関数
	//  引数  : sSrc		= 比較元文字列
	//        : sDest		= 比較先文字列
	//        : bDiffFlag	= 大文字／小文字区別フラグ
	//  戻り値: BOOL		= TRUE:正常終了 / FALSE:エラー
	// =============================================
	static	BOOL CharComp(LPCSTR sSrc, LPCSTR sDest, BOOL bDiffFlag)
	{
		BOOL	bIsKanjiSrc = iskanji(*sSrc) ? TRUE : FALSE;
		BOOL	bIsKanjiDest = iskanji(*sDest) ? TRUE : FALSE;
		if (bIsKanjiSrc == bIsKanjiDest) {
			if (bIsKanjiSrc) {			// 漢字
				if (sSrc[0] == sDest[0]
					&& sSrc[1] == sDest[1]) {
					return(TRUE);
				}
			} else {					// ASCII
				if (bDiffFlag) {
					// 大文字／小文字を区別
					if (*sSrc == *sDest) return(TRUE);
				} else {
					// 大文字／小文字を同一視
					if (toupper(*sSrc) == toupper(*sDest)) return(TRUE);
				}
			}
		}
		return(FALSE);
	}

	// =============================================
	//  StringComp
	//  概要  : 文字列比較関数(２バイト文字対応)
	//  引数  : sSrc		= 比較元文字列
	//        : sDest		= 比較先文字列
	//        : nDestLen	= 長さ
	//        : bDiffFlag	= 大文字／小文字区別フラグ
	//  戻り値: BOOL		= TRUE:正常終了 / FALSE:エラー
	// =============================================
	static	BOOL StringComp(LPCSTR sSrc, LPCSTR sDest, int nDestLen, BOOL bDiffFlag)
	{
		int		nSrcLen = strlen(sSrc);
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
	//  概要  : 数字を３桁のカンマ区切りに変換する
	//  引数  : strNum		= 数字(文字列)
	//  戻り値: 			= 変換後の文字列
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
	//  概要  : ファイルタイプ文字列を取得する
	//  引数  : fileMP3		= ファイル情報
	//  戻り値: CString		= ファイルタイプ文字列
	// =============================================
	static CString GetFileType(const FILE_MP3 *fileMP3)
	{
		if (fileMP3->strFileTypeName.IsEmpty()) {
			return("Unknown");
		}
		return fileMP3->strFileTypeName;
	}

	/// <summary>
	/// SIF の項目
	/// </summary>
	CString StrReplaceSIF(const CString& strOrg, const FILE_MP3* fileMP3)
	{
		tstring strText = strOrg;
		StrReplace(strText, TEXT("%COPYRIGHT%"), fileMP3->strCopyrightSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%ENGINEER%"), fileMP3->strEngineerSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%SOURCE%"), fileMP3->strSourceSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%SOFTWARE%"), fileMP3->strSoftwareSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%KEYWORD%"), fileMP3->strKeywordSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%TECHNICIAN%"), fileMP3->strTechnicianSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%LYRIC%"), fileMP3->strLyricSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%COMMISSION%"), fileMP3->strCommissionSI.SpanExcluding("\r"));
		StrReplace(strText, TEXT("%WRITER%"), fileMP3->strWriterSI.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%COMPOSER%"), fileMP3->strComposerSI.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%ALBM_ARTIST%"), fileMP3->strAlbmArtistSI.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%ORIG_ARTIST%"), fileMP3->strOrigArtistSI.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%URL%"), fileMP3->strURLSI.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%ENCODEST%"), fileMP3->strEncodest.SpanExcluding("\r")); /* Baja 154 */
		StrReplace(strText, TEXT("%OTHER%"), fileMP3->strOther.SpanExcluding("\r")); /* Conspiracy 196 */
		return strText.c_str();
	}

	std::tuple<CString, CString, CString> GetIntTrackNo(const CString& no)
	{
		CString	strTrackNumber, strTrackNumber2, strTrackNumber3;
		if (const auto nTrackNumber = atoi(CFileMP3::GetIntTrackNo(no))) {
			// １桁のトラック番号
			strTrackNumber.Format(TEXT("%d"), nTrackNumber);
			// ２桁のトラック番号
			strTrackNumber2.Format(TEXT("%02d"), nTrackNumber);
			// ３桁のトラック番号
			strTrackNumber3.Format(TEXT("%03d"), nTrackNumber);
		}

		return {
			strTrackNumber,
			strTrackNumber2,
			strTrackNumber3,
		};
	}
}

// =============================================
//  CMySuperGrid::CMySuperGrid
//  概要  : コンストラクタ
//  引数  : なし
//  戻り値: なし
// =============================================
CMySuperGrid::CMySuperGrid()
{
	m_bDrag = TRUE;
	m_pDoc = NULL;

	// ジャンル名リストの作成
	MakeStrListGenre();

	m_bSortAscending = TRUE;
	m_nSortedCol = -1;

	m_rootItemFont = NULL;

	STEClipbordFormat = RegisterClipboardFormat(PROG_NAME/* "改"*/); /* Misirlou 146 */
}

// =============================================
//  CMySuperGrid::~CMySuperGrid
//  概要  : デストラクタ
//  引数  : なし
//  戻り値: なし
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


	// チェックマーク用イメージリスト
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
				// [▼]の部分をクリックされた場合は、リストを開く
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
			// AudioList にファイル名を表示しない＆分類をはみ出て表示する場合、
			// 分類を考慮しない(＝アイコン・チェックボックスのサイズ）でセルサイズを決定する
			int indentmax = 0;
			int nItems = GetItemCount();
			for(int nItem=0; nItem < nItems; nItem++) {
				CTreeItem* pItem = GetTreeItem(nItem);
				int indent = GetIndent(pItem);
				if (indentmax < indent)	indentmax = indent;
				//if (!IsItemFile(pItem)) {
				//	InvalidateItemRectPtr(pItem); // 分類を再描画
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
	m_pItemRoot = AddDirectory("Root", NULL, 2);

	// RootItemフォルダの名称を更新する
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
	g_sNameList = new const char**[nFileTypeCount];

	for (i=0;i<nFileTypeCount*2;i++) {
		CONTROLTYPE* controlTable = new CONTROLTYPE[COLUMN_MAX];
		int* colMax = new int[COLUMN_MAX];
		const char** nameList = new const char*[COLUMN_MAX];
		for (j=0;j<COLUMN_MAX;j++) {
			controlTable[j] = _NULL;
		}
		for (j=0;j<COLUMN_MAX;j++) {
			colMax[j] = 0;
		}
		for (j=0;j<COLUMN_MAX;j++) {
			nameList[j] = NULL;
		}
		controlTable[COLUMN_FILE_NAME] = _EDIT; // ファイル名は編集可
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
// 概要  : グリッドの初期化処理
// 引数  : none
// 戻り値: none
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

	// セル情報をレジストリから読み出す
	CellStateControl(0);

	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	UpdateCellInfo();
	int		i, nColumn;


	// テーブルを -1 でクリア
	for(nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
		g_nColumnNumberList[nColumn] = -1;
		g_nColumnTypeList[nColumn] = -1;
	}

	// カラム番号 => カラムタイプ(タグ項目) への変換テーブル作成
	for (i = 0; i < COLUMN_MAX; i++) {
		struct COLUMN_STATUS	*Stat = &g_columnStatus[i];
		if (Stat->bShowFlag == TRUE) {
			g_nColumnTypeList[Stat->nNumber] = i;
		}
	}
	// 非表示設定で空いているところを詰める
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

	// カラムタイプ(タグ項目) => カラム番号 への変換テーブル作成
	for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
		int		nType = g_nColumnTypeList[nColumn];
		if (nType == -1) break;

		// CListCtrl にカラムを追加
		lvColumn.fmt		= g_columnStatus[nType].nFormat;
		lvColumn.pszText	= (LPTSTR)g_sNameList[0][nType+1];
		lvColumn.cx			= g_columnStatus[nType].nWidth;
		InsertColumn(nColumn, &lvColumn);

		g_nColumnNumberList[nType] = nColumn;
	}

	// 歌詞ファイルの検索が必要かどうかを設定
	g_bOptSearchLyricFile = g_columnStatus[COLUMN_LYRIC_FILE].bShowFlag ? true : false;

}

// =============================================
// CMySuperGrid::SetItemModifyFlag
// 概要  : アイテムの変更フラグを設定する
// 引数  : pItem		= アイテム
//       : flag			= フラグ
// 戻り値: none
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
// 概要  : アイテムの変更フラグを取得する
// 引数  : pItem		= アイテム
// 戻り値: bool			= フラグ
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
// 概要  : アイテムのファイル形式を取得する
// 引数  : pItem		= アイテム
// 戻り値: int			= 形式(FILE_FORMAT_xxxxxxx)
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
// 概要  : カーソル位置のセルの色を取得する
// 引数  : none
// 戻り値: COLORREF		= 色情報
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
				// 左端のセルの場合
				col = RGB(192, 192, 192);
			} else {
				// それ以外のセルの場合
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
// 概要  : アイテムのイメージアイコン番号の取得処理
// 引数  : pItem		= アイテム
// 戻り値: int			= イメージ番号
// =============================================
int CMySuperGrid::GetIcon(const CTreeItem* pItem)
{
	if (pItem != NULL) {
		int		n = GetData(pItem)->GetImage();
		if (n != -1) {
			CItemInfo	*pInfo = GetData(pItem);
			if (ItemHasChildren(pItem) || (int)pInfo->GetLParam() < 0) {
				// ディレクトリの場合
				return(IsCollapsed(pItem) ? n : n+1);
			}

			// ファイルの場合
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
// 概要  : アイテムデータが変更された場合の処理
// 引数  : lpItem
//       : plvItem
// 戻り値: none
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
// 概要  : セルの内容を更新する
// 引数  : iItem		= アイテム
//       : iSubItem		= サブアイテム
//       : sText		= 設定する文字列
// 戻り値: none
// =============================================
void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, const char *sText)
{
	ChangeSubItemText(iItem, iSubItem, sText, 0);
}

// =============================================
// CMySuperGrid::ChangeSubItemText
// 概要  : セルの内容を更新する
// 引数  : iItem		= アイテム
//       : iSubItem		= サブアイテム
//       : sText		= 設定する文字列
//       : nPos			= 文字列をセルに追加する 1:前 2:後ろ 0:置き換え
// 戻り値: none
// =============================================
void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, const char *sUpdateText, int nPos)
{
	ChangeSubItemText(iItem, iSubItem, sUpdateText, nPos, false, "", "");
}


// =============================================
// CMySuperGrid::OnVkReturn
// 概要  : [Enter]が押された場合の処理(編集開始処理)
// 引数  : none
// 戻り値: BOOL
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
				// フォルダの場合は、[Enter]を押すたびにノードを開く／閉じるを
				// 繰り返す
				SendMessage(WM_KEYDOWN, IsCollapsed(pSelItem) ? VK_ADD : VK_SUBTRACT, 0);
				return(TRUE);
			}

			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			if (iSubItem == 0) {
				// 左端のセルの場合
				return(TRUE);
			} else {
				// それ以外のセルの場合
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
// 概要  : [DELETE]が押された場合の処理
// 引数  : pItem
//       : nIndex
// 戻り値: BOOL
// =============================================
BOOL CMySuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	// 削除禁止
	return(FALSE);
}

// =============================================
// CMySuperGrid::OnChangeSelect
// 概要  : 選択カーソル位置が変更された
// 引数  : none
// 戻り値: BOOL
// =============================================
BOOL CMySuperGrid::OnChangeSelect(void)
{
	// カーソル位置情報を更新
	UpdateStatusLocation();

	// ステータスバーのファイル名情報を更新
	CString	strText;
	int		iItem;
	if ((iItem = GetSelectedItem()) != -1) {
		CTreeItem	*pItem = GetTreeItem(iItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
			strText = fileMP3->strFullPathName;
			// ファイルタイプ毎のヘッダーに変更
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
							lvColumn.pszText = (LPTSTR)g_sNameList[0][nType+1]; /* デフォルトのカラム名 */
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
					strText.Format("%d個のファイルを選択", nCount);
				} else {
					strText.Format("全て(%d個)のファイルを選択", nCount);
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
				// ファイルのみ処理する
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
		strText.Format("%dファイル/%.2fMB/%d:%02d", nCount, dSize / 1024 / 1024, dPlayTime / 60, dPlayTime % 60 /* BeachMonster5 123 */);
		AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_SIZE, 0, (LPARAM)strText.GetBuffer(0));
	}
	return(CSuperGridCtrl::OnChangeSelect());
}

// =============================================
// CMySuperGrid::MakeStrListGenre
// 概要  : ジャンル名リストの作成
// 引数  : none
// 戻り値: none
// =============================================
void CMySuperGrid::MakeStrListGenre(void)
{
	int		nGenre;
	m_strListGenre.RemoveAll();

	// 未対応のジャンルのための項目
//	m_strListGenre.AddTail(_T("-NotSupported-"));

	// SCMPX互換ジャンル番号の登録
	for (nGenre = 0; g_genreListSCMPX[nGenre].sName != NULL; nGenre++) {
		if (g_genreListSCMPX[nGenre].bAddList) {
			m_strListGenre.AddTail(g_genreListSCMPX[nGenre].sName);
		}
	}
	// ユーザジャンルの登録
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
// 概要  : リストにファイルを追加する
// 引数  : sDirName		= ディレクトリ名
//       : pItemParent	= 親アイテム
//       : nIcon		= アイコン
//       : bSort		= ソートフラグ
// 戻り値: CTreeItem *	= アイテム情報(NULL:エラー)
// =============================================
CSuperGridCtrl::CTreeItem *CMySuperGrid::AddDirectory(const char *sDirName, CTreeItem *pItemParent, int nIcon)
{
	CItemInfo* lp = new CItemInfo();

	lp->SetFlagExt(0x00000000);
	lp->SetLParam(-1);
	lp->SetImage(nIcon);

	// ディレクトリ名を登録
	lp->SetItemText(sDirName);

	// 全カラムを編集禁止にする
	for (int nCol = 0; nCol < COLUMN_MAX; nCol++) {
		lp->SetControlType(invalid, nCol);
	}

	// ツリーに追加
	CTreeItem *pItem;
	if (pItemParent == NULL) pItem = InsertRootItem(lp);
	else                     pItem = InsertItem(pItemParent, lp, TRUE, INSERT_DIRECTORY);

	return(pItem);
}

// =============================================
// CMySuperGrid::AddFile
// 概要  : リストにファイルを追加する
// 引数  : fileMP3		= MP3ファイル情報
//       : pItemParent	= 親アイテム
//       : lParam		= アイテムに設定するLPARAM情報(リストのインデックス)
//       : bCheck		= チェックマークを付けるかどうか
// 戻り値: bool			= true:正常終了 / false:エラー
// =============================================
bool CMySuperGrid::AddFile(const FILE_MP3 *fileMP3, CTreeItem *pItemParent, LPARAM lParam, bool bCheck)
{
	CString	strBuffer;
	CItemInfo* lp = new CItemInfo();

	lp->SetFlagExt(0x00000000);
	lp->SetLParam(lParam);
	lp->SetImage(6);

	// 各項目をダミーで登録
	// ファイル名
	lp->AddSubItemText("");
	// フルパス
	lp->AddSubItemText("");
	// トラック名
	lp->AddSubItemText("");
	// アーティスト名
	lp->AddSubItemText("");
	// アルバム名
	lp->AddSubItemText("");
	// トラック番号
	lp->AddSubItemText("");
	// ディスク番号
	lp->AddSubItemText("");
	// 年号
	lp->AddSubItemText("");
	// ジャンル
	lp->AddSubItemText("");
	// コメント
	lp->AddSubItemText("");

	// === RIFF MP3 拡張項目 ===
	// MP3 形式
	lp->AddSubItemText("");
	// 著作権
	lp->AddSubItemText("");
	// エンジニア(製作者)
	lp->AddSubItemText("");
	// ソース
	lp->AddSubItemText("");
	// ソフトウェア
	lp->AddSubItemText("");
	// キーワード
	lp->AddSubItemText("");
	// 技術者
	lp->AddSubItemText("");
	// 歌詞
	lp->AddSubItemText("");
	// コミッション
	lp->AddSubItemText("");

	// === その他 ===
	// ファイルサイズ
	lp->AddSubItemText("");
	// ファイル更新日
	lp->AddSubItemText("");
	// ファイルパス名
	lp->AddSubItemText("");
	// 演奏時間
	lp->AddSubItemText("");
	// 歌詞ファイル
	lp->AddSubItemText("");

	// ジャンルのコンボボックスの設定
	int		nColNum = g_nColumnNumberList[COLUMN_GENRE];
	if (nColNum >= 0) {
		lp->SetControlType(combobox, nColNum-1);
		lp->SetListData(nColNum-1, &m_strListGenre);
	}

	// ツリーに追加
	CTreeItem *pItem;
	if (pItemParent == NULL) pItemParent = m_pItemRoot;
	if (pItemParent == NULL) pItem = InsertRootItem(lp);
	else                     pItem = InsertItem(pItemParent, lp, TRUE, INSERT_FILE);

	if (pItem != NULL) {
		// 各項目に正しい情報を設定
		UpdateFileStatus(pItem, fileMP3);

		// チェック状態を設定
		SetCheckEx(pItem, bCheck);
	}

	return(pItem ? true : false);
}

// =============================================
// CSuperGridCtrl::SearchChildrenItem
// 概要  : 子アイテムの中から、指定された名前のアイテムを検索する
// 引数  : strName		= 名前
//       : pItemParent	= 親アイテム
// 戻り値: CTreeItem *	= 発見したアイテム(NULL:発見できなかった)
// =============================================
CSuperGridCtrl::CTreeItem *CMySuperGrid::SearchChildrenItem(const CString &strName, CTreeItem *pItemParent)
{
	POSITION pos = pItemParent ? GetHeadPosition(pItemParent) : GetRootHeadPosition();
	while(pos != NULL) {
		CTreeItem	*pParent = (CTreeItem*)(pItemParent ? GetNextChild(pItemParent, pos) : GetNextRoot(pos));
		CTreeItem	*pItem = pParent;
		CItemInfo	*lp = GetData(pParent);
		if ((int)lp->GetLParam() < 0) {
			// フォルダのみ処理する
			CString		strData = lp->GetItemText();
			if (strData == strName) {
				// 発見
				return(pItem);
			}
		}
	}
	return((CTreeItem *)NULL);
}

// フィールドテキストを取得
CString CMySuperGrid::GetFileColumnText(const FILE_MP3 *fileMP3, int nColumn)
{
	enum	{READ_ID3, READ_SIF, READ_SIF_ID3};
	int		nReadType = IsEditFieldSIF(fileMP3) ? READ_SIF : READ_ID3;
	int		nReadTypeTrackNumber = IsEditTrackNumberSIF(fileMP3) ? READ_SIF : READ_ID3;
	int		nReadTypeTrackGenre = IsEditGenreSIF(fileMP3) ? READ_SIF : READ_ID3;

	CString	strBuffer;
	switch(nColumn) {
	case COLUMN_FULL_PATH_NAME:		// フルパス名
		return(fileMP3->strFilePath);
	case COLUMN_FILE_NAME:		// ファイル名
		return(fileMP3->strFileName);
	case COLUMN_TRACK_NAME:		// トラック名
		if (nReadType == READ_ID3) return(fileMP3->strTrackName);
		if (nReadType == READ_SIF) return(fileMP3->strTrackNameSI);
	case COLUMN_ARTIST_NAME:	// アーティスト名
		if (nReadType == READ_ID3) return(fileMP3->strArtistName);
		if (nReadType == READ_SIF) return(fileMP3->strArtistNameSI);
	case COLUMN_ALBUM_NAME:		// アルバム名
		if (nReadType == READ_ID3) return(fileMP3->strAlbumName);
		if (nReadType == READ_SIF) return(fileMP3->strAlbumNameSI);
	case COLUMN_DISK_NUMBER:	// ディスク番号
		if (nReadType == READ_ID3) return(fileMP3->strDiskNumber);
		if (nReadType == READ_SIF) return(fileMP3->strDiskNumberSI);
	case COLUMN_TRACK_NUMBER:	// トラック番号
		if (nReadTypeTrackNumber == READ_ID3) return(fileMP3->strTrackNumber);
		if (nReadTypeTrackNumber == READ_SIF) return(fileMP3->strTrackNumberSI);
		break;
	case COLUMN_YEAR:			// リリース年号
		if (nReadType == READ_ID3) return(fileMP3->strYear);
		if (nReadType == READ_SIF) return(fileMP3->strYearSI);
		return(fileMP3->strYearSI.IsEmpty() ? fileMP3->strYear : fileMP3->strYearSI);
	case COLUMN_GENRE:			// ジャンル番号
		if (nReadTypeTrackGenre == READ_ID3) return(fileMP3->strGenre);
		if (nReadTypeTrackGenre == READ_SIF) return(fileMP3->strGenreSI);
	case COLUMN_COMMENT:		// コメント
		if (nReadType == READ_ID3) return(fileMP3->strComment);
		if (nReadType == READ_SIF) return(fileMP3->strCommentSI);
		return(fileMP3->strCommentSI.IsEmpty() ? fileMP3->strComment : fileMP3->strCommentSI);
	case COLUMN_COPYRIGHT:		// 著作権
		return(fileMP3->strCopyrightSI);
	case COLUMN_ENGINEER:		// エンジニア(製作者)
		return(fileMP3->strEngineerSI);
	case COLUMN_SOURCE:			// ソース
		return(fileMP3->strSourceSI);
	case COLUMN_SOFTWARE:		// ソフトウェア
		return(fileMP3->strSoftwareSI);
	case COLUMN_KEYWORD:		// キーワード
		return(fileMP3->strKeywordSI);
	case COLUMN_TECHNICIAN:		// 技術者
		return(fileMP3->strTechnicianSI);
	case COLUMN_LYRIC:			// 歌詞
		return(fileMP3->strLyricSI);
	case COLUMN_COMMISSION:		// コミッション
		return(fileMP3->strCommissionSI);

	case COLUMN_FILE_SIZE:		// ファイルサイズ
		strBuffer.Format("%.2fMB", (float)fileMP3->lFileSize / 1024 / 1024);
		break;
	case COLUMN_FILE_TIME:		// ファイル更新日
		strBuffer.Format("%04d/%02d/%02d %02d:%02d:%02d",
		                  fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
		                  fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
		break;
	case COLUMN_FILE_PATH:		// ファイルパス名
		return(fileMP3->strFilePath);
	case COLUMN_PLAY_TIME:		// 演奏時間
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format("%02d:%02d", fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
		}
		break;
	case COLUMN_LYRIC_FILE:		// 歌詞ファイル
		strBuffer.Format("%s", fileMP3->bExistLyricFile ? "あり" : "なし");
		break;
	case COLUMN_FILE_TYPE:		// ファイルの種類
		strBuffer = GetFileType(fileMP3);
		break;
	case COLUMN_FORMAT:			// フォーマット /* Conspiracy 198 */
		if (fileMP3->strVisualFormat.IsEmpty()) {
			return(fileMP3->strAudioFormat);
		} else {
			return(fileMP3->strVisualFormat + "\n" + fileMP3->strAudioFormat);
		}
		break;
	case COLUMN_WRITER:			// 作詞 /* ADD MIMURA */
		return(fileMP3->strWriterSI);
		break;
	case COLUMN_COMPOSER:		// 作曲 /* Baja 154 */
		return(fileMP3->strComposerSI);
		break;
	case COLUMN_ALBM_ARTIST:	// Albm.アーティスト /* Baja 154 */
		return(fileMP3->strAlbmArtistSI);
		break;
	case COLUMN_ORIG_ARTIST:	// Orig.アーティスト /* Baja 154 */
		return(fileMP3->strOrigArtistSI);
		break;
	case COLUMN_URL:			// URL /* Baja 154 */
		return(fileMP3->strURLSI);
		break;
	case COLUMN_ENCODEST:		// エンコードした人 /* Baja 154 */
		return(fileMP3->strEncodest);
		break;
	case COLUMN_OTHER:			//  その他 /* Conspiracy 196 */
		return(fileMP3->strOther);
		break;
/* 2003.06.19 start */
	case COLUMN_FILE_CTIME:		// ファイル作成日
		strBuffer.Format("%04d/%02d/%02d %02d:%02d:%02d",
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

	// 必要ならフォルダを作成する
	for (int nClass = 0; nClass < CLASS_MAX; nClass++) {
		int		nColumn = g_classInfo.nColumn[nClass];
		if (nColumn >= 0 && nColumn < COLUMN_MAX) {
			CString	strBuffer;

			// ファイル情報の指定のカラム番号の情報を取得
			strBuffer = GetFileColumnText(fileMP3, nColumn);
			if (strBuffer.IsEmpty() == FALSE) {
				CTreeItem	*pItem;
				if ((pItem = SearchChildrenItem(strBuffer, pParent)) == NULL) {
					// 新しいフォルダを作成
					pItem = AddDirectory(strBuffer, pParent, 2);
				}
				pParent = pItem;
			}
		}
	}

	// ファイルを追加
	if (pParent == m_pItemRoot || pParent == NULL) {
		// 分類できなかった
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

	// ファイル名を登録
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

	// セルのテキストを設定
	int		nColumn;
	for (nColumn = COLUMN_FILE_NAME; nColumn < COLUMN_MAX; nColumn++) {
		// テキストを取得
		strBuffer = GetFileColumnText(fileMP3, nColumn);

		int		nColNum = g_nColumnNumberList[nColumn];
		if (nColNum >= 0) {
			pInfo->SetSubItemText(nColNum-1, strBuffer);
			SetItemText(nIndex, nColNum, strBuffer);
		}
	}

	// ファイル形式依存による編集可能／不可能設定処理
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

	// その他
	// ファイルサイズを変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_SIZE);
	// ファイル更新日のセルのコントロールタイプを更新する
	if (g_bOptKeepTimeStamp) {
		SetColumnControlType(pInfo, edit, COLUMN_FILE_TIME);
	} else {
		SetColumnControlType(pInfo, invalid, COLUMN_FILE_TIME);
	}
	// ファイルパス名を変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_PATH);
	// 演奏時間を変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_PLAY_TIME);
	// 歌詞ファイルを変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_LYRIC_FILE);
	// ファイルの種類を変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_TYPE);
	// フォーマットを変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_FORMAT/* Conspiracy 198 */);
	// その他を変更不可にする
	//SetColumnControlType(pInfo, invalid, COLUMN_OTHER); /* Conspiracy 196 */
	// 作成日を変更不可にする
	SetColumnControlType(pInfo, invalid, COLUMN_FILE_CTIME); /* 2003.06.19 add */

	UpdateData(pItem, pInfo, FALSE);

	// チェック状態を更新
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

// カラムの最大文字数取得処理
int CMySuperGrid::OnGetColumLimitText(int nItem, int nCol)
{
	if (nItem >= 0) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL) {
			int		nType = g_nColumnTypeList[nCol];
			if (nType >= 0 && nType < COLUMN_MAX) {
				// ファイルサイズ／ファイルパス名／演奏時間／歌詞ファイル
				if (nType == COLUMN_FILE_SIZE || nType == COLUMN_FILE_PATH
				||  nType == COLUMN_PLAY_TIME || nType == COLUMN_LYRIC_FILE
				||	nType == COLUMN_FORMAT /* Conspiracy 198 */) {
					return(0);
				}
				// タイムスタンプ
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
					||  IS_RANGE(wCode, VK_NUMPAD0, VK_NUMPAD9)	// '9' - '0'(テンキー)
					||  wCode == VK_SPACE				// スペース
					||  wCode == VK_SEPARATOR			// ?????
					||  wCode == VK_MULTIPLY			// '*'(テンキー)
					||  wCode == VK_ADD					// '+'(テンキー)
					||  wCode == VK_SUBTRACT			// '-'(テンキー)
					||  wCode == VK_DECIMAL				// '.'(テンキー)
					||  wCode == VK_DIVIDE				// '/'(テンキー)
					||  wCode == 0x00bd					// '-'(キーボード１列目)
					||  wCode == 0x00de					// '^'(キーボード１列目)
					||  wCode == 0x00dc					// '\'(キーボード１列目)
					||  wCode == 0x00c0					// '@'(キーボード２列目)
					||  wCode == 0x00db					// '['(キーボード２列目)
					||  wCode == 0x00bb					// ';'(キーボード３列目)
					||  wCode == 0x00ba					// ':'(キーボード３列目)
					||  wCode == 0x00dd					// ']'(キーボード３列目)
					||  wCode == 0x00bc					// ','(キーボード４列目)
					||  wCode == 0x00be					// '.'(キーボード４列目)
					||  wCode == 0x00bf					// '/'(キーボード４列目)
					||  wCode == 0x00e2					// '_'(キーボード４列目)
					||  (wCode == VK_PROCESSKEY && !(GetAsyncKeyState(VK_CONVERT)&0x8000) && !(GetAsyncKeyState(VK_NONCONVERT)&0x8000) && /*!(GetAsyncKeyState(0xf2)&0x8000) && !(GetAsyncKeyState(0xf3)&0x8000) && !(GetAsyncKeyState(0xf4)&0x8000) &&*//* Baja 155 */ !((pMsg->lParam&0x00FF0000)==0x00700000/*ひらがな*/ || (pMsg->lParam&0x00FF0000)==0x00290000/*半角/全角*/))/* Misirlou 151 */) {		// 日本語入力(?)
						if (pInfo->GetControlType(iSubItem-1, ctrlType) == 0) {
							// エディットコントロールの場合
							CEdit	*pEdit;
							pEdit = EditLabelEx(iItem, iSubItem);
							if (pEdit != NULL) {
								pEdit->SetSel(0, -1);		// 全体を選択
								pEdit->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
							}
							return(TRUE);
						} else if (ctrlType == combobox) {
							// コンボボックスの場合
							CStringList *list = NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox *pList = ShowList(iItem, iSubItem, list);
							if (pList != NULL) {
								pList->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
							}
							return(TRUE);
						}
					}
					if (wCode == VK_DELETE) {			// セルの内容をクリア
						CellClearSelected();
						return(TRUE);
					}

					// チェックマーク有りの場合
					if (GetExtendedStyle() & LVS_EX_CHECKBOXES) {
						if (wCode == VK_SPACE) {			// スペースバー
							// チェックマークの変更なので無視する
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


void ShowLastError(LPSTR title)
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
    MessageBox(NULL, (char*)lpMsgBuf, title, MB_OK);
    LocalFree(lpMsgBuf);
}


// =============================================
// CMySuperGrid::ClipboardCopy
// 概要  : クリップボードへのコピー処理
// 引数  : none
// 戻り値: bool			= true:正常終了 / false:エラー
// =============================================
bool CMySuperGrid::ClipboardCopy(void)
{
	CString		strBuffer;
	CString		strBufferText; /* RockDance 124 */

	if (IsRangeSelected() == false) {
		// 現在のカーソル位置を選択範囲として登録する
		if (GetSelectedItem() == -1) return(false);	// エラー
		SelectRangeStart();
		m_bMultiSelect = m_bMultiSelectLine = false;
	}

	// 選択範囲のセルの内容を文字列に変換
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

	// クリップボード転送用のバッファを確保
	LPSTR pBuffer;
	HGLOBAL hMem, hMem2/* RockDance 124 */;
	hMem = GlobalAlloc(GHND|GMEM_DDESHARE, strBuffer.GetLength()+1);
	pBuffer = (LPSTR)GlobalLock(hMem);
	strcpy(pBuffer, strBuffer);
	GlobalUnlock(hMem);
	/* RockDance 124 */
	hMem2 = GlobalAlloc(GHND|GMEM_DDESHARE, strBufferText.GetLength()+1);
	pBuffer = (LPSTR)GlobalLock(hMem2);
	strcpy(pBuffer, strBufferText);
	GlobalUnlock(hMem2);

	// クリップボードを開いてコピー
	OpenClipboard();
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, (HANDLE)hMem2);
	::SetClipboardData(STEClipbordFormat/*CF_DSPTEXT*//* Misirlou 146 */, (HANDLE)hMem); /* RockDance 124 */
	CloseClipboard();

	return(true);
}

// =============================================
// CMySuperGrid::ClipboardCut
// 概要  : クリップボードへのカット処理
// 引数  : none
// 戻り値: bool			= true:正常終了 / false:エラー
// =============================================
bool CMySuperGrid::ClipboardCut(void)
{
	ClipboardCopy();
	CellClearSelected();

	return(true);
}

void CMySuperGrid::PasteString(char *sBuffer)
{
#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	PasteString(sBuffer, 0, true/* RockDance 124 */);	/* FreeFall 052 */
#ifdef FLICKERFREE
	//SetRedraw(TRUE);
#endif
}

void CMySuperGrid::PasteString(char *sBuffer, int nPastePos, bool bText/* RockDance 124 */)	/* FreeFall 052 */
{
	PasteString(sBuffer, nPastePos, false, false, "", "");
}

// =============================================
// CMySuperGrid::ClipboardPaste
// 概要  : クリップボードへからのペースト処理
// 引数  : none
// 戻り値: bool			= true:正常終了 / false:エラー
// =============================================
bool CMySuperGrid::ClipboardPaste(void)
{
	return ClipboardPaste(0, false, "", ""); /* Baja 171 */
}

// セルの内容を下方向へコピー
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
					// セルに文字列を設定
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
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
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
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
	}

#ifdef FLICKERFREE
	//SetRedraw(FALSE);
#endif
	int		nNumber = nStart;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CString	strNumber;
			strNumber.Format("%d", nNumber);
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
		atoi(str1) > 0 && atoi(str2) > 0) { /* BeachMonster4 118 */
		result = atoi(str1) - atoi(str2);
	} else if (nColum == g_nColumnNumberList[COLUMN_DISK_NUMBER] &&
		atoi(str1) > 0 && atoi(str2) > 0) { /* BeachMonster4 118 */
		result = atoi(str1) - atoi(str2);
	} else if (nColum == g_nColumnNumberList[COLUMN_FILE_SIZE]) { /* RockDance 127 */
		result = (int)(atof(str1)*100 - atof(str2)*100);
	} else {
		char	*buffer1 = new char[str1.GetLength()*2+2];
		char	*buffer2 = new char[str2.GetLength()*2+2];
		CString strComp1 = str1; /* FunnyCorn 179 */
		CString strComp2 = str2; /* FunnyCorn 179 */
		if (g_bOptSortIgnoreKataHira) { /* FunnyCorn 179 */
			strcpy(buffer1, str1);
			strcpy(buffer2, str2);
			conv_hira2kata((unsigned char *)buffer1);
			conv_hira2kata((unsigned char *)buffer2);
			conv_kata_erase_dakuon((unsigned char *)buffer1);
			conv_kata_erase_dakuon((unsigned char *)buffer2);
			strComp1 = buffer1;
			strComp2 = buffer2;
		}
		if (g_bOptSortIgnoreZenHan) {
			conv_zen2hans((unsigned char *)buffer1, (unsigned char *)strComp1/* str1*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
			conv_zen2hans((unsigned char *)buffer2, (unsigned char *)strComp2/* str2*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
		} else {
			strcpy(buffer1, str1);
			strcpy(buffer2, str2);
		}
		if (FALSE && g_bOptSortIgnoreKataHira) { /* FunnyCorn 179 */
			strComp1 = buffer1;
			strComp2 = buffer2;
			conv_han2zens((unsigned char *)buffer1, (unsigned char *)strComp1/* str1*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
			conv_han2zens((unsigned char *)buffer2, (unsigned char *)strComp2/* str2*//* FunnyCorn 179 */.GetBuffer(0), CONV_ALL);
		}
		CString han1 = buffer1;
		if (g_bOptSortIgnoreCase) {
			result = han1.CompareNoCase(buffer2);
			//result = _mbsicmp((unsigned char *)buffer1, (unsigned char *)buffer2);
		} else {
			result = han1.Compare(buffer2);
			//result = _mbscmp((unsigned char *)buffer1, (unsigned char *)buffer2);
		}
		delete [] buffer1;
		delete [] buffer2;
	}
	if (!bSortAscending) {
		result *= -1;
	}
	return result;
}

int CMySuperGrid::CompFunc(SORT_STATE *sortState, int nKeys, int nItem1, int nItem2)
{
	int i; for (i = 0; i < nKeys; i++) {
		// 指定されたキーで比較
		int		nColumn = g_nColumnNumberList[sortState->nKeyColumn];
		if (nColumn >= 0 && nColumn < COLUMN_MAX) {
			int		nResult;
			nResult = CompFunc(GetItemText(nItem1, nColumn),
			                   GetItemText(nItem2, nColumn),
			                   nColumn,
			                   (sortState->nType == 0) ? true : false);
			if (nResult != 0) return(nResult);
		}

		// 次のキー
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

				// チェック状態も入れ替え
				BOOL	bCheck1 = pInfo1->GetCheck();
				BOOL	bCheck2 = pInfo2->GetCheck();
				SetCheck(hi, bCheck2);
				SetCheck(lo, bCheck1);

				// nMidItem が交換された場合は、番号を調整する
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

	// 再描画
	InvalidateRect(NULL);
	UpdateWindow();
}

void CMySuperGrid::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_bSortAscending = (pNMListView->iSubItem == m_nSortedCol) ? !m_bSortAscending : TRUE;
	m_nSortedCol = pNMListView->iSubItem;

	// 編集中の場合への対処
	// (フォーカスを戻して編集を完了させてからソートする)
	if (GetFocus() != this) SetFocus();

	CWaitCursor	wait;

	bool	bControl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? true : false;
	if (bControl) {
		// クリックされたカラムの幅を調節
		// カラム幅の自動調節
		AutoSizeColumns(m_nSortedCol);
	} else {
		// クリックされたカラムでソート実行
		SORT_STATE	sortState;
		sortState.nKeyColumn = g_nColumnTypeList[m_nSortedCol];
		sortState.nType = m_bSortAscending ? 0 : 1;
		LockWindowUpdate();			// 描画禁止(ちらつき防止)
		MultiColumnSort(&sortState, 1);
		UnlockWindowUpdate();		// 描画許可(ちらつき防止)
	}

	*pResult = 0;
}

// =============================================
// CMySuperGrid::OnContextMenu
// 概要  : ポップアップメニュー処理
// 引数  : ...
// 戻り値: none
// =============================================
void CMySuperGrid::OnContextMenu(CWnd*, CPoint point)
{
	if (GetSelectedItem() == -1) return;

	//キーストロークによる発動の場合は、位置を調整する
	if (point.x == -1 && point.y == -1) {
		// カレントのセル位置にメニューを開く
		CRect rect;
		GetSubItemRect(GetSelectedItem(), GetCurSubItem(), LVIR_BOUNDS, rect);
		// セルの左から1/3、上から1/2の位置に調整
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
		// セルの複数選択
		VERIFY(menu.LoadMenu(IDR_MENU_CELL_RANGE));
		OnLoadMenu(&menu, MENU_CELL_RANGE);
		bUpdateMenuText = true;
	} else if (GetSelectedCount() > 2) {
		// 複数行選択
		VERIFY(menu.LoadMenu(IDR_MENU_FILE_RANGE));
		OnLoadMenu(&menu, MENU_FILE_RANGE);
		bUpdateMenuText2= true;
		bUpdateUserConvMenu = true;
	} else {
		// 単一行選択
		int		nIndex = GetSelectedItem();
		if (nIndex >= 0) {
			CTreeItem	*pItem = GetTreeItem(nIndex);
			if (pItem != NULL && IsItemFile(pItem) == TRUE) {
				// ファイル
//				if (IsCurrentCellEditOK()) {
				if (GetCurSubItem() >= COLUMN_MIN) {
					// 編集可能なセル
					VERIFY(menu.LoadMenu(IDR_MENU_FILE_EDIT_OK));
					OnLoadMenu(&menu, MENU_FILE_EDIT_OK);
					bUpdateMenuText = true;
					bUpdateUserConvMenu = true; /* STEP 030 */
				} else {
					// 編集不可のセル
					VERIFY(menu.LoadMenu(IDR_MENU_FILE_RANGE));
					OnLoadMenu(&menu, MENU_FILE_RANGE);
					bUpdateUserConvMenu = true; /* STEP 030 */
				}
			} else {
				// フォルダ
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
			if (strTitle == "ユーザー指定変換(&U)") {
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

	// メニュー項目名の更新
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
			// メニュー項目の書き換え
			CString	strName;
			// メニュー項目名を取得
			switch(dwCmdID[i]) {
			case ID_CONV_FORMAT_EX_01:
				strName.Format("01：%s", g_userConvFormatEx[0].strName);
				break;
			case ID_CONV_FORMAT_EX_02:
				strName.Format("02：%s", g_userConvFormatEx[1].strName);
				break;
			case ID_CONV_FORMAT_EX_03:
				strName.Format("03：%s", g_userConvFormatEx[2].strName);
				break;
			case ID_CONV_FORMAT_EX_04:
				strName.Format("04：%s", g_userConvFormatEx[3].strName);
				break;
			case ID_CONV_FORMAT_EX_05:
				strName.Format("05：%s", g_userConvFormatEx[4].strName);
				break;
			case ID_CONV_FORMAT_EX_06:
				strName.Format("06：%s", g_userConvFormatEx[5].strName);
				break;
			case ID_CONV_FORMAT_EX_07:
				strName.Format("07：%s", g_userConvFormatEx[6].strName);
				break;
			case ID_CONV_FORMAT_EX_08:
				strName.Format("08：%s", g_userConvFormatEx[7].strName);
				break;
			case ID_CONV_FORMAT_EX_09:
				strName.Format("09：%s", g_userConvFormatEx[8].strName);
				break;
			case ID_CONV_FORMAT_EX_10:
				strName.Format("10：%s", g_userConvFormatEx[9].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// メニュー文字列を更新
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
			// メニュー項目の書き換え
			CString	strName;
			// メニュー項目名を取得
			switch(dwCmdID[i]) {
			case ID_CONV_FORMAT_USER_01:
			case ID_CONV_FORMAT_USER_T2F_01: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_01: /* STEP 030 */
				strName.Format("01：%s", g_userConvFormat[0].strName);
				break;
			case ID_CONV_FORMAT_USER_02:
			case ID_CONV_FORMAT_USER_T2F_02: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_02: /* STEP 030 */
				strName.Format("02：%s", g_userConvFormat[1].strName);
				break;
			case ID_CONV_FORMAT_USER_03:
			case ID_CONV_FORMAT_USER_T2F_03: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_03: /* STEP 030 */
				strName.Format("03：%s", g_userConvFormat[2].strName);
				break;
			case ID_CONV_FORMAT_USER_04: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_04: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_04: /* STEP 030 */
				strName.Format("04：%s", g_userConvFormat[3].strName);
				break;
			case ID_CONV_FORMAT_USER_05: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_05: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_05: /* STEP 030 */
				strName.Format("05：%s", g_userConvFormat[4/* WildCherry 066 */].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// メニュー文字列を更新
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
			// メニュー項目の書き換え
			CString	strName;
			// メニュー項目名を取得
			switch(dwCmdID[i]) {
			case ID_CONV_TAG_TO_TAG_01:
				strName.Format("01：%s", g_userConvFormatTag2Tag[0].strName);
				break;
			case ID_CONV_TAG_TO_TAG_02:
				strName.Format("02：%s", g_userConvFormatTag2Tag[1].strName);
				break;
			case ID_CONV_TAG_TO_TAG_03:
				strName.Format("03：%s", g_userConvFormatTag2Tag[2].strName);
				break;
			case ID_CONV_TAG_TO_TAG_04:
				strName.Format("04：%s", g_userConvFormatTag2Tag[3].strName);
				break;
			case ID_CONV_TAG_TO_TAG_05:
				strName.Format("05：%s", g_userConvFormatTag2Tag[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// メニュー文字列を更新
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
		/* FreeFall 046 */ // もっとスマートにできないかな....
		CMenu* pTeikeiMenu = NULL;
		int nCount = pPopup->GetMenuItemCount();
		for (int j=0;j<nCount;j++) {
			CString strTitle;
			pPopup->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "定型文貼り付け") {
				pTeikeiMenu = pPopup->GetSubMenu(j);
				break;
			}
		}
		for (int k = 0; k < 3 && pTeikeiMenu; k++) {
			CMenu* pSubMenu = pTeikeiMenu->GetSubMenu(0);
			pTeikeiMenu->RemoveMenu(0, MF_BYPOSITION);
			pTeikeiMenu->AppendMenu(MF_POPUP , (UINT)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
			int i; for (i = 0; i < 10; i++) {
				// メニュー項目の書き換え
				CString	strName;
				// メニュー項目名を取得
				strName.Format("%02d：%s", i+1, shortString(g_teikeiInfo[i+k*10].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				// メニュー文字列を更新
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
			// メニュー項目の書き換え
			CString	strName;
			// メニュー項目名を取得
			switch(dwCmdID[i]) {
			case ID_MOVE_FOLDER_01: /* LastTrain 065 */
				strName.Format("01：%s", g_userMoveFolder[0].strName);
				break;
			case ID_MOVE_FOLDER_02: /* LastTrain 065 */
				strName.Format("02：%s", g_userMoveFolder[1].strName);
				break;
			case ID_MOVE_FOLDER_03: /* LastTrain 065 */
				strName.Format("03：%s", g_userMoveFolder[2].strName);
				break;
			case ID_MOVE_FOLDER_04: /* LastTrain 065 */
				strName.Format("04：%s", g_userMoveFolder[3].strName);
				break;
			case ID_MOVE_FOLDER_05: /* LastTrain 065 */
				strName.Format("05：%s", g_userMoveFolder[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// メニュー文字列を更新
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
			// メニュー項目の書き換え
			CString	strName;
			// メニュー項目名を取得
			switch(dwCmdID[i]) {
			case ID_EDIT_COPY_FORMAT_01:
				strName.Format("01：%s", g_userCopyFormat[0].strName);
				break;
			case ID_EDIT_COPY_FORMAT_02:
				strName.Format("02：%s", g_userCopyFormat[1].strName);
				break;
			case ID_EDIT_COPY_FORMAT_03:
				strName.Format("03：%s", g_userCopyFormat[2].strName);
				break;
			case ID_EDIT_COPY_FORMAT_04:
				strName.Format("04：%s", g_userCopyFormat[3].strName);
				break;
			case ID_EDIT_COPY_FORMAT_05:
				strName.Format("05：%s", g_userCopyFormat[4].strName);
				break;
			default:
				pPopup->GetMenuString((UINT)dwCmdID[i], strName, MF_BYCOMMAND);
				break;
			}
			// メニュー文字列を更新
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
// 概要  : セル情報の制御処理処理
// 引数  : nMode
// 戻り値: none
// =============================================
void CMySuperGrid::CellStateControl(int nMode)
{
	CString strINI = AfxGetApp()->m_pszProfileName;
	InitProfile();
	//Profile_Initialize(strINI, TRUE);
	static	char	*sSectionColumnState	= "ColumnState";

	int		nColumn;
	CWinApp	*pApp = AfxGetApp();

	if (nMode == 0) {
		// カラム情報をレジストリから読み出す

		int ColNumFix = 0;		// 旧バージョンからのカウント補正

		CString	strVersion;		// 補正情報用
		double dVersion;
		strVersion = MyGetProfileString("Option","Version", "");
		dVersion = atof(strVersion);

		for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
			CString	strKey;
			struct COLUMN_STATUS	*Stat = &g_columnStatus[nColumn];

			// 表示フラグ
			strKey.Format("%sShow", Stat->sRegKey);
			Stat->bShowFlag = MyGetProfileInt(sSectionColumnState, strKey, Stat->bShowFlag);

			// カラム位置
			strKey.Format("%sColNum", Stat->sRegKey);
			Stat->nNumber = MyGetProfileInt(sSectionColumnState, strKey, Stat->nNumber) + ColNumFix;

			// カラム幅
			strKey.Format("%sWidth", Stat->sRegKey);
			Stat->nWidth = MyGetProfileInt(sSectionColumnState, strKey, Stat->nWidth);

			// 最大カラム幅
			strKey.Format("%sWidthMax", Stat->sRegKey);
			Stat->nWidthMax = MyGetProfileInt(sSectionColumnState, strKey, Stat->nWidthMax);

			if(strcmp(Stat->sRegKey,"DiskNo") == 0) {
				if((dVersion == 1.04)|(dVersion == 1.041))
					ColNumFix++;
			}
		}
	} else if (nMode == 1) {
		// 現在のカラムサイズを取得
		CHeaderCtrl *pHeader = (CHeaderCtrl *)GetHeaderCtrl();
		int nColumnCount = pHeader->GetItemCount();
		for (nColumn = 0; nColumn < nColumnCount; nColumn++) {
			g_columnStatus[g_nColumnTypeList[nColumn]].nWidth = GetColumnWidth(nColumn);
		}
		// カラム情報をレジストリに書き出す
		for (nColumn = 0; nColumn < COLUMN_MAX; nColumn++) {
			CString	strKey;
			struct COLUMN_STATUS	*Stat = &g_columnStatus[nColumn];

			// 表示フラグ
			strKey.Format("%sShow", Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->bShowFlag);

			// カラム位置
			strKey.Format("%sColNum", Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->nNumber);

			// カラム幅
			strKey.Format("%sWidth", Stat->sRegKey);
			MyWriteProfileInt(sSectionColumnState, strKey, Stat->nWidth);

			// 最大カラム幅
			strKey.Format("%sWidthMax", Stat->sRegKey);
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
		// セル情報をレジストリに保存
		CellStateControl(1);
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

bool CMySuperGrid::ConvTagInfo(int nIndex, int nType, const char *sFormat)
{
	return(ConvTagInfo(GetTreeItem(nIndex), nType, sFormat));
}

bool CMySuperGrid::ConvTagInfoSelected(int nType, const char *sFormat)
{
	// 選択されているファイルを全て処理する
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];
		ConvTagInfo(GetTreeItem(nIndex), nType, sFormat);
	}
	return(true);
}

bool CMySuperGrid::ConvTagInfo(CTreeItem *pItem, int nType, const char *sFormat, const char* sBaseTag)
{
	if (pItem == NULL || IsItemFile(pItem) == FALSE) return(false);

	int		nIndex = NodeToIndex(pItem);
	CItemInfo	*pItemInfo = GetData(pItem);
	FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pItemInfo->GetLParam());
	switch(nType) {
	default:
	case 1:		// ファイル名 => トラック名
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
	case 2:		// トラック名 => ファイル名
		{
			CString	strTrackName = GetFileColumnText(fileMP3, COLUMN_TRACK_NAME);
			if (strTrackName.IsEmpty() == false) {
				ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_FILE_NAME], strTrackName);
				InvalidateItemRect(nIndex);
			}
		}
		break;
	case 3:		// ユーザー指定の[タグ情報] => [ファイル名]変換
		{
			CString	strFileName = sFormat;
			CString	strDiskNumber,strDiskNumber2,strDiskNumber3;
			auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));
			strDiskNumber.Format("%d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
			strDiskNumber2.Format("%02d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
			strDiskNumber3.Format("%03d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
			strFileName = StrReplace(strFileName, "%TRACK_NAME%"   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding("\r"));
			strFileName = StrReplace(strFileName, "%ALBUM_NAME%"   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding("\r"));
			strFileName = StrReplace(strFileName, "%ARTIST_NAME%"  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding("\r"));
			strFileName = StrReplace(strFileName, "%TRACK_NUMBER%" , strTrackNumber);
			strFileName = StrReplace(strFileName, "%TRACK_NUMBER2%", strTrackNumber2);
			strFileName = StrReplace(strFileName, "%TRACK_NUMBER3%", strTrackNumber3);
			strFileName = StrReplace(strFileName, "%DISK_NUMBER%" , strDiskNumber);
			strFileName = StrReplace(strFileName, "%DISK_NUMBER2%", strDiskNumber2);
			strFileName = StrReplace(strFileName, "%DISK_NUMBER3%", strDiskNumber3);
			strFileName = StrReplace(strFileName, "%RELEASE_YEAR%" , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding("\r"));
			strFileName = StrReplace(strFileName, "%COMMENT%"      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding("\r")/* BeachMonster 089 */);
			strFileName = StrReplace(strFileName, "%GENRE%"        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding("\r"));
			// SIF の項目
			strFileName = StrReplaceSIF(strFileName, fileMP3);

			ChangeSubItemText(nIndex, g_nColumnNumberList[COLUMN_FILE_NAME], strFileName);
			InvalidateItemRect(nIndex);
		}
		break;
	case 4:		// ユーザー指定の[ファイル名] => [タグ情報]変換
	case 5:		// タグ振分変換 /* STEP 034 */
		{
			// ファイル名部分を取得
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			char	*pFileName; /* STEP 034 */
			CString strFileName; /* STEP 034 */
			if (nType == 4) { /* STEP 034 */
				char	sFileName[_MAX_FNAME];
				char	sDirName[_MAX_DIR];	// 2003.06.18 add */
				_tsplitpath(fileMP3->strFullPathName, NULL, sDirName, sFileName, NULL); /* 2003.06.18 add */
	//			_tsplitpath(fileMP3->strFileName, NULL, NULL, sFileName, NULL); /* 2003.06.18 del */
	/* 2003.06.18 start */
				strFileName = sFileName;
				// 書式にディレクトリが含まれているかチェックしてその階層数分のディレクトリを変換元に追加する
				for (UINT nPos=0;nPos<strlen(sFormat);nPos++) {
					if (_ismbblead(sFormat[nPos])) {
						nPos++;
						continue;
					}
					if (sFormat[nPos] == '\\') {
						sDirName[strlen(sDirName)-1] = '\0';
						CString sPathName = sDirName;
						char	sExtName[_MAX_EXT];
						_tsplitpath(sPathName, NULL, sDirName, sFileName, sExtName); /* 2003.06.18 add */
						if (strlen(sFileName) == 0)	break;
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

				if (_strnicmp(sFormat, "%TRACK_NAME%", 12) == 0) {
					nColumn = COLUMN_TRACK_NAME;
					nLen = 12;
				} else if (_strnicmp(sFormat, "%ALBUM_NAME%", 12) == 0) {
					nColumn = COLUMN_ALBUM_NAME;
					nLen = 12;
				} else if (_strnicmp(sFormat, "%ARTIST_NAME%", 13) == 0) {
					nColumn = COLUMN_ARTIST_NAME;
					nLen = 13;
				} else if (_strnicmp(sFormat, "%DISK_NUMBER%", 13) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 14;
				} else if (_strnicmp(sFormat, "%DISK_NUMBER2%", 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_strnicmp(sFormat, "%DISK_NUMBER3%", 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_strnicmp(sFormat, "%TRACK_NUMBER%", 14) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 14;
				} else if (_strnicmp(sFormat, "%TRACK_NUMBER2%", 15) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_strnicmp(sFormat, "%TRACK_NUMBER3%", 15) == 0) {
					nColumn = COLUMN_TRACK_NUMBER;
					nLen = 15;
				} else if (_strnicmp(sFormat, "%RELEASE_YEAR%", 14) == 0) {
					nColumn = COLUMN_YEAR;
					nLen = 14;
				} else if (_strnicmp(sFormat, "%COMMENT%", 9) == 0) {
					nColumn = COLUMN_COMMENT;
					nLen = 9;
				} else if (_strnicmp(sFormat, "%GENRE%", 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_GENRE;
					nLen = 7;
				} else if (_strnicmp(sFormat, "%COPYRIGHT%", 11) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COPYRIGHT;
					nLen = 11;
				} else if (_strnicmp(sFormat, "%ENGINEER%", 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ENGINEER;
					nLen = 10;
				} else if (_strnicmp(sFormat, "%SOURCE%", 8) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_SOURCE;
					nLen = 8;
				} else if (_strnicmp(sFormat, "%SOFTWARE%", 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_SOFTWARE;
					nLen = 10;
				} else if (_strnicmp(sFormat, "%KEYWORD%", 9) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_KEYWORD;
					nLen = 9;
				} else if (_strnicmp(sFormat, "%TECHNICIAN%", 12) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_TECHNICIAN;
					nLen = 12;
				} else if (_strnicmp(sFormat, "%LYRIC%", 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_LYRIC;
					nLen = 7;
				} else if (_strnicmp(sFormat, "%COMMISSION%", 12) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COMMISSION;
					nLen = 12;
				} else if (_strnicmp(sFormat, "%WRITER%", 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_WRITER;
					nLen = 10;
				} else if (_strnicmp(sFormat, "%COMPOSER%", 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_COMPOSER;
					nLen = 10;
				} else if (_strnicmp(sFormat, "%ALBM_ARTIST%", 13) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ALBM_ARTIST;
					nLen = 13;
				} else if (_strnicmp(sFormat, "%ORIG_ARTIST%", 13) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ORIG_ARTIST;
					nLen = 13;
				} else if (_strnicmp(sFormat, "%URL%", 5) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_URL;
					nLen = 5;
				} else if (_strnicmp(sFormat, "%ENCODEST%", 10) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_ENCODEST;
					nLen = 10;
				} else if (_strnicmp(sFormat, "%OTHER%", 7) == 0) { /* Conspiracy 200 */
					nColumn = COLUMN_OTHER;
					nLen = 7;
				} else if (_strnicmp(sFormat, "%SKIP%", 6) == 0) {
					nColumn = COLUMN_DUMMY;
					nLen = 6;
				} else {
					if (*sFormat != *pFileName) break;	// フォーマット不一致
					pFileName++;
					sFormat++;
				}

				// カラム情報更新
				if (nColumn >= 0) {
					CString	strWord;
					if (sFormat[nLen] == '\0') {
						// 最後まで
						strWord = pFileName;
					} else {
						// 次の区切り文字まで
						CString		strWork = &sFormat[nLen];
						// 次の % までも文字列を取得
						LPCSTR	sToken = strtok(strWork.GetBuffer(0), "%");
						if (sToken == NULL) {		// 最後まで
							strWord = pFileName;
						} else {					// 次のキーワードまで
							int		nLen = strlen(sToken);
							while(*pFileName != '\0' && strncmp(pFileName, sToken, nLen) != 0) {
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
							if(_strnicmp(sFormat, "%TRACK_NUMBER%", 14) == 0) {
								strWord.Format("%d", atoi(strWord));
							}
							if(_strnicmp(sFormat, "%TRACK_NUMBER2%", 15) == 0) {
								strWord.Format("%02d", atoi(strWord));
							}
							if(_strnicmp(sFormat, "%TRACK_NUMBER3%", 15) == 0) {
								strWord.Format("%03d", atoi(strWord));
							}
							if(_strnicmp(sFormat, "%DISK_NUMBER%", 13) == 0) {
								strWord.Format("%d", atoi(strWord));
							}
							if(_strnicmp(sFormat, "%DISK_NUMBER2%", 14) == 0) {
								strWord.Format("%02d", atoi(strWord));
							}
							if(_strnicmp(sFormat, "%DISK_NUMBER3%", 14) == 0) {
								strWord.Format("%03d", atoi(strWord));
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

const char *CMySuperGrid::GetSelectFileName(void)
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
			// ファイルの場合
			if (pInfo->GetCheck()) {
				CItemInfo	*pInfo = GetData(pItem);
				FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

				// ファイル名を出力
				CString	strBuffer;
				// 相対パス対応
				strBuffer = ConvFileName(fileMP3->strFullPathName, strCurDir);
				strBuffer += "\r\n";
				file.Write(strBuffer, strBuffer.GetLength());
			}
		} else {
			// フォルダの場合
			// 子アイテムを全て処理する
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

bool CMySuperGrid::WritePlayList(const char *sFileName)
{
	return(WritePlayList(sFileName, GetRootHeadPosition()));
}

bool CMySuperGrid::WritePlayList(const char *sFileName, POSITION posRoot)
{
	TRY {
		// 出力ファイルを開く
		CFile	file(sFileName, CFile::modeCreate|CFile::modeWrite);

		// プレイリストファイルのカレントディレクトリを取得
		CString	strCurDir;
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sFileName, drive, dir, NULL, NULL);
		strCurDir.Format("%s%s", drive, dir);

		// ルートのアイテムを全て処理する
		POSITION pos = posRoot;
		while(pos != NULL) {
			CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
			WritePlayList(file, pItem, strCurDir);
		}
	}
	CATCH(CFileException, e) {
		CString	strMsg;
		strMsg.Format("ファイルがオープンできませんでした\n中断します\n\n%s", sFileName);
		MessageBox(strMsg, "ファイルのオープン失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return(true);
}

bool CMySuperGrid::WriteTreePlayList(const char *sFolder, CTreeItem *pItem, CString &strNameParent)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		if (pItem != m_pItemRoot) {
			// プレイリスト出力
			CString	strFileName;
			strFileName.Format("%s%s.m3u", sFolder, strNameParent);
			WritePlayList(strFileName, GetHeadPosition(pItem));
		}

		// サブフォルダの処理
		CItemInfo *pInfo = GetData(pItem);
		int		nIndex = NodeToIndex(pItem);
		if (IsItemFile(pItem) == FALSE) {
			// フォルダの場合
			if (ItemHasChildren(pItem) == TRUE) {
				CTreeItem *pSelItem = pItem;
				if (ItemHasChildren(pSelItem)) {
					POSITION pos = GetHeadPosition(pSelItem);
					while(pos != NULL) {
						CTreeItem	*pItem = (CTreeItem*)GetNextChild(pSelItem, pos);
						CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
						if (pInfo && IsItemFile(pItem) == FALSE && ItemHasChildren(pItem)) {
							// フォルダのみ処理する
							CString		strName;
							if (strNameParent.IsEmpty()) strName.Format("%s", pInfo->GetItemText());
							else                         strName.Format("%s - %s", strNameParent, pInfo->GetItemText());
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

bool CMySuperGrid::WriteTreePlayList(const char *sFolder)
{
	// 全てのファイルを出力
	CString	strFileName;
	strFileName.Format("%sAllFiles.m3u", sFolder);
	WritePlayList(strFileName);

	// ルートを処理する
	CString		strEmpty;
	WriteTreePlayList(sFolder, m_pItemRoot, strEmpty);

	return(true);
}

// =============================================
// CMySuperGrid::MakeSelectFileArray
// 概要  : 選択されているファイルの配列を作成する
// 引数  : array	= 配列(出力用)
// 戻り値: int		= 有効な配列の要素数
// =============================================
int CMySuperGrid::MakeSelectFileArray(CArray <int, const int &> &array)
{
	const int	nInitSize = 256;
	int		nCount;

	// 配列の初期化
	nCount = 0;
	array.RemoveAll();
	array.SetSize(nInitSize);

	if (GetSelectedCount() > 0) {
		if (GetSelectedCount() == 1) {
			// 単一選択
			nCount = AddSelectFileArray(GetSelectedItem(), array, nCount);
		} else {
			// 複数選択
			// 複数選択の場合は、全ての選択されているアイテムを処理する
			// ただし、閉じられているフォルダ内のファイルは無視する
			int		nIndex = -1;
			while((nIndex = GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >=0) {
				if (IsItemFile(GetTreeItem(nIndex)) == TRUE) {
					// ファイルのみ処理する
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

	// 配列の初期化
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
// 概要  : 選択されているファイルを配列に追加する
// 引数  : nIndex		= 追加するファイルのインデックス
//       : array		= 配列
//       : nCount		= 配列の有効な要素数
// 戻り値: int			= 追加後の配列の有効な要素数
// =============================================
int CMySuperGrid::AddSelectFileArray(int nIndex, CArray <int, const int &> &array, int nCount)
{
	const int	nAddSize = 256;

	// 範囲外エラー
	if (nIndex < 0) return(nCount);

	CTreeItem	*pItem = GetTreeItem(nIndex);
	if (ItemHasChildren(pItem) == FALSE) {
		if (IsItemFile(pItem)) {
			// 配列を拡大する必要があるか？
			if (nCount > array.GetUpperBound()) {
				// 配列が足りない場合は、確保サイズを増加させる
				array.SetSize(array.GetUpperBound() + nAddSize);
			}

			// 配列に追加
			array[nCount] = nIndex;
			nCount++;
		}
	} else {
		POSITION pos = GetHeadPosition(pItem);
		// ノードが閉じている場合は開く
		if(ItemHasChildren(pItem) && IsCollapsed(pItem)) {
			Expand(pItem, nIndex);
			InvalidateItemRect(nIndex);
		}
		// 子アイテムを全て処理する
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

// アイテムがダブルクリックされた
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

// チェック状態が変更された
void CMySuperGrid::OnChangeCheck(CTreeItem *pItem, BOOL bCheck)
{
	if (pItem != NULL && IsItemFile(pItem) == TRUE) {
		// ファイルのみ処理する
		CItemInfo	*pInfo = GetData(pItem);
		if (pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			if (fileMP3) {
				// チェックフラグを更新
				fileMP3->bCheckFlag = bCheck ? true : false;
			}
		}
	}
}

// =============================================
// CMySuperGrid::SetCheckAllFiles
// 概要  : 全てのファイルのチェック状態を設定する
// 引数  : bCheck		= チェック状態
// 戻り値: none
// =============================================
void CMySuperGrid::SetCheckAllFiles(BOOL bCheck)
{
	// ルートのアイテムを全て処理する
	POSITION pos = GetRootHeadPosition();
	while(pos != NULL) {
		CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
		SetCheckEx(NodeToIndex(pItem), bCheck);
	}
}

// =============================================
// CMySuperGrid::SetCheckFileSelect
// 概要  : チェックされているファイルを選択状態にする
// 引数  : none
// 戻り値: none
// =============================================
void CMySuperGrid::SetCheckFileSelect(void)
{
	// 全てのアイテムの選択解除
	SelectAllItems(FALSE);

	int		nIndex = -1;
	while((nIndex = GetNextItem(nIndex, LVNI_ALL)) >=0) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			// ファイルのみ処理する
			CItemInfo *pInfo = GetData(pItem);
			if (pInfo && pInfo->GetCheck()) {
				// 選択する
				//SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED); // カーソル位置はそのまま
			}
		}
	}
	OnChangeSelect(); // http://bbs1.parks.jp/22/hasetaS/bbs.cgi?Action=Res&Mode=Tree&Base=94&Fx=1
}

// =============================================
// CMySuperGrid::SetSelectFileCheck
// 概要  : 選択されているファイルのチェック状態を設定する
// 引数  : bCheck		= チェック状態
// 戻り値: none
// =============================================
void CMySuperGrid::SetSelectFileCheck(BOOL bCheck)
{
	// 選択されているファイルを全て処理する
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	int i; for (i = 0; i < nCount; i++) {
		SetCheckEx(GetTreeItem(arrayList[i]), bCheck);
	}
}

// =============================================
// CMySuperGrid::ReverseCheckFiles
// 概要  : 全てのファイルのチェック状態を反転する
// 引数  : none
// 戻り値: none
// =============================================
void CMySuperGrid::ReverseCheckFiles(void)
{
	int		nIndex = -1;
	while((nIndex = GetNextItem(nIndex, LVNI_ALL)) >=0) {
		CTreeItem	*pItem = GetTreeItem(nIndex);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			// ファイルのみ処理する
			CItemInfo *pInfo = GetData(pItem);
			if (pInfo) {
				// チェック状態を反転
				SetCheckEx(pItem, pInfo->GetCheck() ? FALSE : TRUE);
			}
		}
	}
}

// =============================================
// CMySuperGrid::CheckFileAtColumnState
// 概要  : タグ情報の内容でチェックする
// 引数  : chkWord		= 条件
// 戻り値: none
// =============================================
void CMySuperGrid::CheckFileAtColumnState(CHECK_WORD_STATE *chkWord)
{
	// 正規表現のコンパイル
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// コンパイルエラー
			MessageBox("正規表現に指定されている文字列を、正しくコンパイル\n出来ませんでした\n\n正規表現の記述を確認してください", "コンパイルエラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return;
		}
	}

	// 検索開始位置を初期化
	chkWord->nMatchPos = 0;
	chkWord->nMatchLength = 0;

	if (chkWord->bRangeSelected) {
		// 選択されているファイルを全て処理する
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = MakeSelectFileArray(arrayList);
		int i; for (i = 0; i < nCount; i++) {
			CheckFileAtColumnState(chkWord, GetTreeItem(arrayList[i]));
		}
	} else {
		// ルートのアイテムを全て処理する
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL) {
			CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
			CheckFileAtColumnState(chkWord, pItem);
		}
	}
}

// =============================================
// CMySuperGrid::CheckFileAtColumnState
// 概要  : タグ情報の内容でチェックする(アイテム単位の処理)
// 引数  : chkWord		= 条件
//         pItem		= 処理するアイテム
// 戻り値: none
// =============================================
void CMySuperGrid::CheckFileAtColumnState(CHECK_WORD_STATE *chkWord, CTreeItem *pItem)
{
	ASSERT(pItem != NULL);
	if (pItem != NULL) {
		if (IsItemFile(pItem) == TRUE) {
			// ファイルの場合
			chkWord->nMatchPos = 0; /* STEP 002 */
			if (IsMatchItem(chkWord, pItem)) {
				int		nIndex;
				if ((nIndex = NodeToIndex(pItem)) != -1) {
					if (chkWord->bRangeSelected == FALSE
					&&  chkWord->bMatchSelected == TRUE) {
						// 選択する
						SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
						OnChangeSelect();			// 選択が変更されたことを通知
					} else {
						// チェックする
						SetCheckEx(nIndex, TRUE);
					}
				}
			}
		} else {
			// フォルダの場合
			// 子アイテムを全て処理する
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
	// 正規表現のコンパイル
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// コンパイルエラー
			MessageBox("正規表現に指定されている文字列を、正しくコンパイル\n出来ませんでした\n\n正規表現の記述を確認してください", "コンパイルエラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return;
		}
	}

	if (bRepAll) {
		// 全て／選択範囲を全て置換
		if (chkWord->bRangeSelected) {
			// 選択されているファイルを全て処理する
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				ReplaceMatchItem(chkWord, GetTreeItem(arrayList[i]));
			}
		} else {
			// ルートのアイテムを全て処理する
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				ReplaceMatchItem(chkWord, pItem);
			}
		}
	} else {
		// 現在の行のみ置換
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
			// ファイルの場合
			int		nRetryMax = 128;
			// 検索開始位置を初期化
			chkWord->nMatchPos = 0;
			chkWord->nMatchLength = 0;
			while(IsMatchItem(chkWord, pItem) && --nRetryMax) {
				int		nIndex;
				if ((nIndex = NodeToIndex(pItem)) != -1) {
					CItemInfo *pInfo = GetData(pItem);
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
					CString	strText;

					// セルのテキストを取得
					strText = GetFileColumnText(fileMP3, chkWord->nMatchColumn);

					// 対象は[ファイル名]？
					if (chkWord->nMatchColumn == COLUMN_FILE_NAME) {
						// 拡張子部分は置換の対象にしない
						if (chkWord->nMatchPos >= strText.GetLength()-4) break;
					}

					CString	strNew;
					strNew = strText.Left(chkWord->nMatchPos)
					       + chkWord->strReplaceWord
					       + strText.Right(strText.GetLength() - (chkWord->nMatchPos+chkWord->nMatchLength));
					ChangeSubItemText(nIndex, g_nColumnNumberList[chkWord->nMatchColumn], strNew);
					if (strNew == strText) {
						// 無限ループ回避用
						break;
					}

					// 置換後の文字を次の置換の対象にしないようにする処理
					chkWord->nMatchPos += chkWord->strReplaceWord.GetLength();
				}
			}
		} else {
			// フォルダの場合
			// 子アイテムを全て処理する
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
// 概要  : アイテムが条件に一致するかどうかを調べる
// 引数  : chkWord		= 条件
//         pItem		= チェックするアイテム
// 戻り値: bool			= true:一致 / false:不一致
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

			// セルのテキストを取得
			switch(nTargetColumn) {
			case COLUMN_TRACK_NUMBER:	// トラック番号
				strText = fileMP3->strTrackNumber;
				break;
			case COLUMN_DISK_NUMBER:	// ディスク番号
				strText = fileMP3->strDiskNumber;
				break;
			case COLUMN_GENRE:			// ジャンル番号
				strText = fileMP3->strGenre;
				break;
			case COLUMN_FULL_PATH_NAME:
				strText = fileMP3->strFullPathName;
				break;
			default:					// その他
				// セルのテキストを取得
				strText = GetFileColumnText(fileMP3, nTargetColumn);
				break;
			}

			int		nMatchPos = 0, nMatchLen = 0;
			if (nType == TYPE_STRING) {
				if (chkWord->bRegExp == FALSE) {
					// 通常検索
					int		nCount = strText.GetLength() - chkWord->strSearchWord.GetLength();
					if (nCount >= 0) {
						int		nPos;
						int		nLength = chkWord->strSearchWord.GetLength();
						if (chkWord->strSearchWord.GetLength() == 1) {
							TCHAR	chSearch = chkWord->strSearchWord.GetAt(0);
							for (nPos = chkWord->nMatchPos; nPos <= nCount; nPos++) {
								if (iskanji(strText.GetAt(nPos))) {
									// 漢字はスキップ
									nPos++;
								} else {
									// ASCII文字
									if (chkWord->bCheckDiffUL == TRUE) {
										// 大文字／小文字を区別
										if (strText.GetAt(nPos) == chSearch) {
											// 一致
											nMatchPos = nPos;
											nMatchLen = nLength;
											break;
										}
									} else {
										// 大文字／小文字を同一視
										if (toupper(strText.GetAt(nPos)) == toupper(chSearch)) {
											// 一致
											nMatchPos = nPos;
											nMatchLen = nLength;
											break;
										}
									}
								}
							}
						} else {
							for (nPos = chkWord->nMatchPos; nPos <= nCount; nPos++) {
								if (StringComp(((LPCSTR)strText)+nPos, (LPCSTR)chkWord->strSearchWord, nLength, chkWord->bCheckDiffUL)) {
									// 一致
									nMatchPos = nPos;
									nMatchLen = nLength;
									break;
								}
								if (iskanji(strText.GetAt(nPos))) { /* RockDance 132 */
									// 漢字はスキップ
									nPos++;
								}
							}
						}
					}
				} else {
					// 正規表現
					if ((nMatchPos = chkWord->regexp.RegFind(strText.GetBuffer(0)+chkWord->nMatchPos)) >= 0) {
						nMatchPos += chkWord->nMatchPos;
						nMatchLen = chkWord->regexp.GetFindLen();
					}
				}
				if (chkWord->bMatchComplete) {
					// 完全一致
					bMatch = (nMatchPos == 0 && nMatchLen != 0 && nMatchLen == strText.GetLength()) ? true : false;
				} else {
					// 部分一致
					bMatch = nMatchLen ? true : false;
				}
			} else if (nType == TYPE_NUMERIC) {
				bMatch = ((BYTE)atoi(chkWord->strSearchWord) == (BYTE)dwData) ? true : false;
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
	// 正規表現のコンパイル
	if (chkWord->bRegExp) {
		if (chkWord->regexp.RegComp(chkWord->strSearchWord) == NULL) {
			// コンパイルエラー
			MessageBox("正規表現に指定されている文字列を、正しくコンパイル\n出来ませんでした\n\n正規表現の記述を確認してください", "コンパイルエラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(-1);
		}
	}

	// 検索開始位置を初期化
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
				// ファイルのみ処理する
				if (IsMatchItem(chkWord, pItem)) {
					// 一致した
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
		// 行の選択変更
		SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		EnsureVisible(nIndex, FALSE);

		int		iSubItem = g_nColumnNumberList[nColType];
		if (iSubItem != -1) {
			// 桁の選択変更
			SetCurSubItem(iSubItem);
			MakeColumnVisible(iSubItem);
		}
		OnChangeSelect();			// 選択が変更されたことを通知
	}
}

bool CMySuperGrid::ConvUserFormatEx(USER_CONV_FORMAT_EX *pForm)
{
	if (pForm->strFormat.IsEmpty()) {
		// 変換書式が空
		return(false);					// エラー
	}

	int		sx, sy, ex, ey;
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
	}

	// 固定文字列を入力させる
	if (pForm->strFormat.Find("%STRING%") != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// キャンセル
			return(false);
		}
		pForm->strFixString = dialog.m_strText;
	}

	// 連番変換書式の設定
	char	sNumFormat[20] = {0};
	sprintf_s(sNumFormat,20,"%s0%dd","%",pForm->nColumnCount);

	int		nNumber = pForm->nInitNumber;
	int		fileCount = 0;
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			// ファイル名から拡張子を除いた文字を取得
			CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
			TCHAR	sFileName[_MAX_FNAME];
			_tsplitpath(strFileName, NULL, NULL, sFileName, NULL);

			CString	strText = pForm->strFormat;
			CString	strNumber, strDiskNumber, strDiskNumber2, strDiskNumber3;
			auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));

			// 連番
			strNumber.Format(sNumFormat, nNumber);

			// １桁のディスク番号
			strDiskNumber.Format("%d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
			// ２桁のディスク番号
			strDiskNumber2.Format("%02d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
			// ３桁のディスク番号
			strDiskNumber3.Format("%03d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));

			// 書式を使って文字列を作成
			strText = StrReplace(strText, "%FILE_NAME%"    , sFileName);
			strText = StrReplace(strText, "%TRACK_NAME%"   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding("\r"));
			strText = StrReplace(strText, "%ALBUM_NAME%"   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding("\r"));
			strText = StrReplace(strText, "%ARTIST_NAME%"  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding("\r"));
			strText = StrReplace(strText, "%TRACK_NUMBER%" , strTrackNumber);
			strText = StrReplace(strText, "%TRACK_NUMBER2%", strTrackNumber2);
			strText = StrReplace(strText, "%TRACK_NUMBER3%", strTrackNumber3);
			strText = StrReplace(strText, "%DISK_NUMBER%" , strDiskNumber);
			strText = StrReplace(strText, "%DISK_NUMBER2%", strDiskNumber2);
			strText = StrReplace(strText, "%DISK_NUMBER3%", strDiskNumber3);
			strText = StrReplace(strText, "%RELEASE_YEAR%" , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding("\r"));
			strText = StrReplace(strText, "%COMMENT%"      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding("\r")/* BeachMonster 089 */);
			strText = StrReplace(strText, "%GENRE%"        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding("\r"));
			strText = StrReplace(strText, "%NUMBER%"       , strNumber);
			strText = StrReplace(strText, "%STRING%"       , pForm->strFixString);
			// SIF の項目
			StrReplaceSIF(strText, fileMP3);
			/* STEP 007 */
			if (fileCount == 0) {
				fileCount = GetFolderFileCount(NodeToIndex(GetParentItem(pItem)));
			}
			CString strFileCount;
			strFileCount.Format(sNumFormat, fileCount);
			strText = StrReplace(strText, "%FOLDER_FILE_COUNT%"	, strFileCount);

			// セルに作成した文字列を設定
			for (int nColumn = sx; nColumn <= ex; nColumn++) {
				ChangeSubItemText(nItem, nColumn, strText);
				InvalidateItemRect(nItem);
			}

			// 連番を進める
			nNumber += pForm->nAddNumber;
		} else {
			// 書き込みできるセルが途切れたら連番を初期化
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
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
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

				// セルのテキストを取得
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// ファイル名の場合、拡張子が全角に変換されないようにする
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					switch(nType) {
					case CONV_STR_HIRA:			// 全角かなに変換
					case CONV_STR_KATA:			// 全角カナに変換
					case CONV_STR_HAN_ALL:		// 全角 => 半角(全て)変換
					case CONV_STR_HAN_KIGOU:	// 全角 => 半角(記号)変換
					case CONV_STR_HAN_SUJI:		// 全角 => 半角(数字)変換
					case CONV_STR_HAN_KATA:		// 全角 => 半角(カタカナ)変換
					case CONV_STR_HAN_ALPHA:	// 全角 => 半角(アルファベット)変換
					case CONV_STR_ZEN_ALL:		// 半角 => 全角(全て)変換
					case CONV_STR_ZEN_KIGOU:	// 半角 => 全角(記号)変換
					case CONV_STR_ZEN_SUJI:		// 半角 => 全角(数字)変換
					case CONV_STR_ZEN_KATA:		// 半角 => 全角(カタカナ)変換
					case CONV_STR_ZEN_ALPHA:	// 半角 => 全角(アルファベット)変換
					default:
						_tsplitpath(strText, NULL, NULL, fname, ext);
						extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
						if (/*strlen(ext) == 4 && */CheckFileExt(fileMP3, ext)) {
						/*
						&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
							 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
							 strcmpi(ext, ".asf") == 0 ||
							 strcmpi(ext, ".ape") == 0 ||
							 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
							 strcmpi(ext, ".ogg") == 0)) {
						*/
							// 拡張子を取り除く
							strText = fname;
							bKeepExt = true;
						}
						break;
					}
				}

				if (strText.GetLength() > 0) {
					// 変換
					char	*buffer = new char[strText.GetLength()*2+2];
					switch(nType) {
					case CONV_STR_UPPER:		// 大文字に変換
						conv_upper((unsigned char *)strText.GetBuffer(0));
						break;
					case CONV_STR_LOWER:		// 小文字に変換
						conv_lower((unsigned char *)strText.GetBuffer(0));
						break;
					case CONV_STR_HANKAKU:		// 半角に変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALL);
						strText = buffer;
						break;
					case CONV_STR_HIRA:			// 全角かなに変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALL);
						conv_kata2hira((unsigned char *)buffer);
						strText = buffer;
						break;
					case CONV_STR_KATA:			// 全角カナに変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALL);
						conv_hira2kata((unsigned char *)buffer);
						strText = buffer;
						break;


					case CONV_STR_HAN_ALL:		// 全角 => 半角(全て)変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALL);
						strText = buffer;
						break;
					case CONV_STR_HAN_KIGOU:	// 全角 => 半角(記号)変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_KIGOU);
						strText = buffer;
						break;
					case CONV_STR_HAN_SUJI:		// 全角 => 半角(数字)変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_SUJI);
						strText = buffer;
						break;
					case CONV_STR_HAN_KATA:		// 全角 => 半角(カタカナ)変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_KATA);
						strText = buffer;
						break;
					case CONV_STR_HAN_ALPHA:	// 全角 => 半角(アルファベット)変換
						conv_zen2hans((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALPHA);
						strText = buffer;
						break;
					case CONV_STR_ZEN_ALL:		// 半角 => 全角(全て)変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALL);
						strText = buffer;
						break;
					case CONV_STR_ZEN_KIGOU:	// 半角 => 全角(記号)変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_KIGOU);
						strText = buffer;
						break;
					case CONV_STR_ZEN_SUJI:		// 半角 => 全角(数字)変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_SUJI);
						strText = buffer;
						break;
					case CONV_STR_ZEN_KATA:		// 半角 => 全角(カタカナ)変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_KATA);
						strText = buffer;
						break;
					case CONV_STR_ZEN_ALPHA:	// 半角 => 全角(アルファベット)変換
						conv_han2zens((unsigned char *)buffer, (unsigned char *)strText.GetBuffer(0), CONV_ALPHA);
						strText = buffer;
						break;
					case CONV_STR_TO_UPPER:		// 小文字 => 大文字変換
						conv_upper((unsigned char *)strText.GetBuffer(0));
						break;
					case CONV_STR_TO_LOWER:		// 大文字 => 小文字変換
						conv_lower((unsigned char *)strText.GetBuffer(0));
						break;
					case CONV_STR_FIRST_UPPER:	// 単語の１文字目のみ大文字
						conv_lower((unsigned char *)strText.GetBuffer(0));
						conv_first_upper((unsigned char *)strText.GetBuffer(0),/* STEP 026 */ g_strFirstUpperIgnoreWords, g_strFirstUpperSentenceSeparator, g_bFirstUpperIgnoreWord);
						break;
					case CONV_STR_FIXED_UP_LOW:	// 大文字小文字固定 /* STEP 040 */
						conv_fixed_upper_lower((unsigned char *)strText.GetBuffer(0), g_arFixedWords);
						break;
					case CONV_STR_HIRA2KATA:	// ひらがな => カタカナ変換
						conv_hira2kata((unsigned char *)strText.GetBuffer(0));
						break;
					case CONV_STR_KATA2HIRA:	// カタカナ => ひらがな変換
						conv_kata2hira((unsigned char *)strText.GetBuffer(0));
						break;
					}
					delete[]	buffer;

					// 取り除いていた拡張子を付けなおす
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
		if (GetSelectedItem() == -1) return(false);	// エラー
		if (GetCurSubItem() < COLUMN_MIN) return(false);		// エラー
		// 範囲選択無し
		sx = ex = GetCurSubItem();
		sy = ey = GetSelectedItem();
	} else {
		// 範囲選択あり
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

void CMySuperGrid::ExecFolderTreeSync(const char *sRootFolder, bool bCheckOnly)
{
	CString	strFolder;
	int		nLen = strlen(sRootFolder);
	if (nLen == 0) return;

	// フォルダ名の最後を必ず '\\' にする
	strFolder = sRootFolder;
	if (IsFolderName(sRootFolder) == false) strFolder += '\\';

	// プログレスバー初期化
	m_pDoc->StartLoadFile("フォルダ同期処理中...");
	m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, "しばらくお待ち下さい...");

	// 再起型のフォルダの同期処理を行う
	m_nFolderSyncCounter = 0;
	ExecFolderTreeSync(strFolder, m_pItemRoot, bCheckOnly, true);

	// プログレスバー終了
	m_pDoc->EndLoadFile();
}

bool CMySuperGrid::CheckExist(const char *sTarget)
{
	bool	bReturn = false;
//	int		nLen = strlen(sTarget);

	if ((sTarget[0] == '\\' && sTarget[1] == '\\')
	&&  _mbschr((const unsigned char *)(sTarget+2), '\\') == NULL) {
		// "\\ネットワーク名" のみの場合は常に真
		bReturn = true;
	} else {
		DWORD	dwAttr;
		DWORD	dwLastError;
		dwAttr = GetFileAttributes(sTarget);
		dwLastError = GetLastError();
		if (dwAttr != 0xFFFFFFFF) {
			// ファイルが存在する
			bReturn = true;
		} else {
			// エラー内容を調べる
			bReturn = false;
			switch(dwLastError) {
			case ERROR_SUCCESS:				// 成功(有り得ないと思うけど一応…)
			case ERROR_FILE_NOT_FOUND:		// ファイルが無い
			case ERROR_PATH_NOT_FOUND:		// パスが無い
				// 本当にファイルが無いだけ
				break;
			default:
				{	// ネットワーク切断などの致命的なエラーが発生
					CString	strBuffer;
					LPVOID lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								  FORMAT_MESSAGE_FROM_SYSTEM |
								  FORMAT_MESSAGE_IGNORE_INSERTS,
								  NULL, dwLastError,
								  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
								  (LPTSTR)&lpMsgBuf, 0, NULL);
					strBuffer.Format("ファイルの存在チェックに失敗しました。\n致命的なエラーが原因である可能性があるので、\n処理を中断します\n\n対象ファイル：%s\n\n%s", sTarget, (const char *)lpMsgBuf);
					LocalFree(lpMsgBuf);
					MessageBox(strBuffer, "致命的エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
				}
				break;
			}
		}
	}
	return(bReturn);
}

// =============================================
// CMySuperGrid::CheckFileName
// 概要  : 有効なファイル名かどうかを調べる
// 引数  : sTarget		= ファイル名
// 戻り値: bool			= true:有効 / false:無効(不正なファイル名)
// =============================================
bool CMySuperGrid::CheckFileName(const char *sTarget)
{
	if ((sTarget[0] == '\\' && sTarget[1] == '\\')
	&&  _mbschr((const unsigned char *)(sTarget+2), '\\') == NULL) {
		// "\\ネットワーク名" のみの場合は常に真
		return(true);
	}

	if (GetFileAttributes(sTarget) == 0xFFFFFFFF) {
		// エラー内容を調べる
		if (GetLastError() == ERROR_INVALID_NAME) {
			// 不正なファイル名、フォルダ名、ボリュームラベル名
			CString	strBuffer;
			strBuffer.Format("ファイル名／フォルダ名に不正な文字が含まれています\n対象ファイル：%s", sTarget);
			MessageBox(strBuffer, "致命的エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return(false);
		}
	}
	return(true);
}

void CMySuperGrid::ChangeFileAttr(const char *sFileName, DWORD dwAttr)
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
	char	*sTemp;
	int		i, nLen = strDir.GetLength();

	// 必ずドライブ名を含む絶対パスで指定すること
	if (nLen <= 3) return(false);	// パスが短すぎる!
//	if (sDir[1] != ':' || sDir[2] != '\\') return(false);

	sTemp = strDir.GetBuffer(strDir.GetLength());
	// i = 3 で初期化しているのは、最初の'\\'(ﾙｰﾄﾃﾞｨﾚｸﾄﾘ)を飛ばす為
	for (i = 3; i <= nLen; i++) {
		unsigned char	c = sTemp[i];
		if (iskanji(c)) {
			i++;
			continue;
		}
		if (c == '\\' || i == nLen) {
			sTemp[i] = NULL;		// '\\' を '\0' に置き換える
			if (!CheckExist((const char *)sTemp)) {
				// ディレクトリが存在しない
RETRY:
				if (CreateDirectory(sTemp, NULL) == FALSE) {
					CString	strBuffer;
					LPVOID lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								  FORMAT_MESSAGE_FROM_SYSTEM |
								  FORMAT_MESSAGE_IGNORE_INSERTS,
								  NULL, GetLastError(),
								  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
								  (LPTSTR)&lpMsgBuf, 0, NULL);
					strBuffer.Format("%s\n\n%s", sTemp, (const char *)lpMsgBuf);
					LocalFree(lpMsgBuf);
					switch(MessageBox(strBuffer, "フォルダの作成に失敗", MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST)) {
					case IDRETRY:		// 再試行
						goto RETRY;
					case IDABORT:		// 中止
						break;
					case IDIGNORE:		// 無視
						break;
					}
					return(false);	// エラー
				}
			}
			if (i != nLen) sTemp[i] = '\\';	// '\0' を '\\' に戻す
		}
	}
	return(true);
}

bool CMySuperGrid::DirectoryRemove(CString &strDir)
{
	// 削除するディレクトリの属性をクリア
	ChangeFileAttr(strDir, CFile::normal);

	char	*buffer;
	buffer = strDir.GetBuffer(strDir.GetLength());
	if (RemoveDirectory(buffer) == FALSE) {
//		CString	strBuffer;
//		LPVOID lpMsgBuf;
//		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
//					  FORMAT_MESSAGE_FROM_SYSTEM |
//					  FORMAT_MESSAGE_IGNORE_INSERTS,
//					  NULL, GetLastError(),
//					  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
//					  (LPTSTR)&lpMsgBuf, 0, NULL);
//		strBuffer.Format("%s\n\n%s", buffer, (const char *)lpMsgBuf);
//		LocalFree(lpMsgBuf);
//		switch(MessageBox(strBuffer, "フォルダの削除に失敗", MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST)) {
//		case IDRETRY:				// 再試行
//			goto RETRY;
//		case IDABORT:				// 中止
//			break;
//		case IDIGNORE:				// 無視
//			break;
//		}
		return(false);	// エラー
	}
	return(true);
}

void CMySuperGrid::ExecFolderTreeSync(const char *sFolder, CTreeItem *pItem, bool bCheckOnly, bool bIsRoot)
{
	ASSERT(pItem != NULL);

	// 子アイテムを持つフォルダ以外の場合は何もせずに終了
	if (pItem == NULL) return;
	if (IsItemFile(pItem) == TRUE) return;
	if (ItemHasChildren(pItem) == FALSE) return;

	// フォルダの場合
	CString	strFolder;
	bool	bMakeDir = true;

	// フォルダを作成する
	if (bIsRoot == false) {
		CItemInfo	*lp = GetData(pItem);
		CString		strText = lp->GetItemText();
		// ファイル名に使用できない文字を置換
		strText = ReplaceFileName(NULL, strText);
		// 最後が '.' になっている場合は、'.' を削除する
		// ※Windows では最後が '.' のフォルダは作成できないため
		while(strText.GetLength() > 0) {
			if (strText[strText.GetLength()-1] != '.') break;
			strText = strText.Left(strText.GetLength()-1);
		}
		strFolder.Format("%s%s\\", sFolder, strText);
	} else {
		strFolder = sFolder;
	}

	// 子アイテムを全て処理する
	int		nIndex = NodeToIndex(pItem);
	POSITION pos = GetHeadPosition(pItem);
	while(pos != NULL) {
		CTreeItem	*pChild = (CTreeItem *)GetNextChild(pItem, pos);
		if (pChild != NULL) {
			if (IsItemFile(pChild) == FALSE) {
				// フォルダ(再帰呼び出し)
				ExecFolderTreeSync(strFolder, pChild, bCheckOnly);
			} else {
				// ファイル
				// プログレスバー更新
				if (m_pDoc->GetArrayFileCount() > 0) {
					m_nFolderSyncCounter++;
					m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(m_nFolderSyncCounter * 100 / m_pDoc->GetArrayFileCount());
					CString	strMessage;
					strMessage.Format("処理中...(%d/%d)", m_nFolderSyncCounter, m_pDoc->GetArrayFileCount());
					m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, strMessage);
				}

				CItemInfo *pInfo = GetData(pChild);
				if (bCheckOnly == false || pInfo->GetCheck()) {
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

					// ディレクトリを作成する
					if (bMakeDir == true) {
						DirectoryMake(strFolder);
						bMakeDir = false;
					}

					// ファイルを移動する
					CString		strOldFolder = fileMP3->strFilePath;
					if (CFileMP3::CopyFile(fileMP3, strFolder, true)) {
						// 移動に成功した場合は、フォルダが空になっていたらフォルダを削除する
						if (g_bSyncDeleteFolder) {
							while(DirectoryRemove(strOldFolder)) {
								// 最後の '\\' を除去
								strOldFolder = strOldFolder.Left(strOldFolder.GetLength()-1);
								// １つ上のディレクトリ名を作成
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
			// ファイルの場合
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			CString strText = MakeFormatFileBody(fileMP3, strBody, bIsHtml, pStatus, bWriteHtml); /* Rumble 190 */

			// １ファイル分を出力
			file.Write(strText, strText.GetLength());
		} else {
			// フォルダの場合
			// 子アイテムを全て処理する
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

	// 出力
	file.Write(strText, strText.GetLength());

	return(true);
}

bool CMySuperGrid::WriteFormatFile(const char *sFileName, const CString &strHead, const CString &strBody, const CString &strFoot, bool bSelected, bool bIsHtml, bool bIsWriteAppend, bool bWriteHtml)
{
	TRY {
		// 出力ファイルを開く
		UINT	iOpenMode = bIsWriteAppend ? (CFile::modeWrite) : (CFile::modeCreate|CFile::modeWrite);
		CFile	file(sFileName, iOpenMode);

		// 追加の場合はファイルの最後に移動
		if (bIsWriteAppend) file.Seek(0, CFile::end);

		// リスト出力情報
		LIST_WRITE_STATUS	status;
		status.nFileNumber	= 0;			// 現在のファイル番号
		status.nFileCount	= 0;			// ファイル総数
		status.fTotalSize	= 0.0f;			// 総合サイズ
		status.nTotalTime	= 0;			// 総合演奏時間

		// リストファイルのディレクトリを取得
		CString	strCurDir;
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sFileName, drive, dir, NULL, NULL);
		status.strDirectory.Format("%s%s", drive, dir);

		// ヘッダを出力
		CString strText = strHead; /* Rumble 190 */
		if (bSelected) { /* Rumble 190 */
			// 選択されているファイルを全て処理する
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				strText = WriteFormatFileHeader(file, GetTreeItem(arrayList[i]), strText, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ルートのアイテムを全て処理する
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				strText = WriteFormatFileHeader(file, pItem, strText, bIsHtml, &status, bWriteHtml);
			}
		}
		/* Rumble 190 *///file.Write(strHead, strHead.GetLength());
		strText = MakeFormatFileFoot(strText, bIsHtml, &status, bWriteHtml); /* Rumble 190 */
		file.Write(strText, strText.GetLength()); /* Rumble 190 */
		// ステータスをリセット /* Rumble 190 */
		status.nFileNumber	= 0;			// 現在のファイル番号
		status.nFileCount	= 0;			// ファイル総数
		status.fTotalSize	= 0.0f;			// 総合サイズ
		status.nTotalTime	= 0;			// 総合演奏時間

		// ボディを出力
		int		nFileNumber = 1;
		if (bSelected) {
			// 選択されているファイルを全て処理する
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				WriteFormatFileBody(file, GetTreeItem(arrayList[i]), strBody, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ルートのアイテムを全て処理する
			POSITION pos = GetRootHeadPosition();
			while(pos != NULL) {
				CTreeItem	*pItem = (CTreeItem*)GetNextRoot(pos);
				WriteFormatFileBody(file, pItem, strBody, bIsHtml, &status, bWriteHtml);
			}
		}

		// フッタを出力
		// ステータスをリセット /* Rumble 190 */
		status.nFileNumber	= 0;			// 現在のファイル番号
		status.nFileCount	= 0;			// ファイル総数
		status.fTotalSize	= 0.0f;			// 総合サイズ
		status.nTotalTime	= 0;			// 総合演奏時間
		CString strTextFoot = strFoot; /* Rumble 190 */
		if (bSelected) { /* Rumble 190 */
			// 選択されているファイルを全て処理する
			CArray <int, const int &> arrayList;
			int		nCount;
			nCount = MakeSelectFileArray(arrayList);
			int i; for (i = 0; i < nCount; i++) {
				strTextFoot = WriteFormatFileHeader(file, GetTreeItem(arrayList[i]), strTextFoot, bIsHtml, &status, bWriteHtml);
			}
		} else {
			// ルートのアイテムを全て処理する
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
		strMsg.Format("ファイルがオープンできませんでした\n中断します\n\n%s", sFileName);
		MessageBox(strMsg, "ファイルのオープン失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH

	return(true);
}

// =============================================
//  CMySuperGrid::ReplaceFileName
//  概要  : ファイル名に使用できない文字を置換する
//  引数  : strName		= ファイル名
//  戻り値: 			= 置換後のファイル名
// =============================================
CString	CMySuperGrid::ReplaceFileName(const FILE_MP3 *fileMP3, CString strName)
{
	TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
	TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
	bool	bKeepExt = false;

	/* LastTrain 058 */
	_tsplitpath(strName, drive, dir, fname, ext); /* BeachMonster 100 */
	extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
	if (/*strlen(ext) == 4 &&*/ CheckFileExt(fileMP3, ext)) {
	/*
		&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
			 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
			 strcmpi(ext, ".asf") == 0 ||
			 strcmpi(ext, ".ape") == 0 ||
			 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
			 strcmpi(ext, ".ogg") == 0)) {
	*/
		// 拡張子を取り除く
		strName = fname;
		strName = dir + strName; /* BeachMonster 100 */
		strName = drive + strName; /* BeachMonster 100 */
		bKeepExt = true;
	}
	{ /* LastTrain 058 */
		char	*buffer = new char[strName.GetLength()*2+2];

		switch (g_nFileUnifyHiraKata) { // 平仮名片仮名
		case 1:// ひらがな
			conv_kata2hira((unsigned char *)strName.GetBuffer(0));
			break;
		case 2:// カタカナ
			conv_hira2kata((unsigned char *)strName.GetBuffer(0));
			break;
		}
		switch (g_nFileUnifyKigou /* BeachMonster 103 */) { // 記号
		case 1:// 全角
			conv_han2zens((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_KIGOU);
			strName = buffer;
			break;
		case 2:// 半角
			conv_zen2hans((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_KIGOU);
			strName = buffer;
			break;
		}
		switch (g_nFileUnifySuji) { // 数字
		case 1:// 全角
			conv_han2zens((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_SUJI);
			strName = buffer;
			break;
		case 2:// 半角
			conv_zen2hans((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_SUJI);
			strName = buffer;
			break;
		}
		switch (g_nFileUnifyKata) { // カタカナ
		case 1:// 全角
			conv_han2zens((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_KATA);
			strName = buffer;
			break;
		case 2:// 半角
			conv_zen2hans((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_KATA);
			strName = buffer;
			break;
		}
		switch (g_nFileUnifyAlpha) { // アルファベット
		case 1:// 全角
			conv_han2zens((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_ALPHA);
			strName = buffer;
			break;
		case 2:// 半角
			conv_zen2hans((unsigned char *)buffer, (unsigned char *)strName.GetBuffer(0), CONV_ALPHA);
			strName = buffer;
			break;
		}
		switch (g_nFileUnifyUpLow) { // 大文字小文字
		case 1:// 大文字
			conv_upper((unsigned char *)strName.GetBuffer(0));
			break;
		case 2:// 小文字
			conv_lower((unsigned char *)strName.GetBuffer(0));
			break;
		case 3:// 単語の１文字目のみ大文字
			conv_lower((unsigned char *)strName.GetBuffer(0));
			conv_first_upper((unsigned char *)strName.GetBuffer(0), /* STEP 026 */ g_strFirstUpperIgnoreWords, g_strFirstUpperSentenceSeparator, g_bFirstUpperIgnoreWord);
			break;
		}
		/* STEP 006 */
		if (fileMP3 != NULL) {
			switch (g_nFileExtChange) {
			case 0:// そのまま
				break;
			case 1://小文字
				conv_lower((unsigned char*)ext);
				break;
			case 2://大文字
				conv_upper((unsigned char *)ext);
				break;
			}
		}
		delete [] buffer;
	}
	{// 文字を置換(ユーザ指定) /* FreeFall 050 */
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
	// 文字を置換
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
	/* 2003.06.19 デフォルトフォント場合に"_"が見えないので +1 */
	/* 2006.02.02 CSuperGridCtrl::MeasureItem()にあわせて +4 となるようにさらに +1 もしくはアイコンの高さ*/ 
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
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
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

				// セルのテキストを取得
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// ファイル名の場合、拡張子が全角に変換されないようにする
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
					if (/*strlen(ext) == 4 &&*/ CheckFileExt(fileMP3, ext)
					) {/*
					&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						 strcmpi(ext, ".asf") == 0 ||
						 strcmpi(ext, ".ape") == 0 ||
						 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						 strcmpi(ext, ".ogg") == 0)) {
					*/
						// 拡張子を取り除く
						strText = fname;
						bKeepExt = true;
					}
				}

				if (strText.GetLength() > nCount) {
					// 削除
					if (nPos == 0) {
						strText.Delete(0, nCount);
					} else {
						strText = strText.Left(strText.GetLength()-nCount);
					}
					// 取り除いていた拡張子を付けなおす
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
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
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
			strNumber.Format("%0*d", nWidth, nNumber);
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
						extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
						if (/*strlen(ext) != 4 || */!CheckFileExt(fileMP3, ext)
						)/*
						||  !(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
							  strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
							  strcmpi(ext, ".asf") == 0 ||
							  strcmpi(ext, ".ape") == 0 ||
							  strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
							  strcmpi(ext, ".ogg") == 0)) {//修正 by Kobarin
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
	// 書式が空のときは選択したフォルダに移動 /* WildCherry 067 */
//	if (pForm->strFormat.IsEmpty()) {
//		// 変換書式が空
//		return(false);					// エラー
//	}

	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	/*
	int		sx, sy, ex, ey;
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
	}
	*/

	// 固定文字列を入力させる
	if (pForm->strFormat.Find("%STRING%") != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// キャンセル
			return(false);
		}
		pForm->strFixString = dialog.m_strText;
	}

	CStringArray strMoveFile;
	// 連番変換書式の設定
	//char	*sNumFormat = "%01d";
	//sNumFormat[2] = '0' + pForm->nColumnCount;

	//int		nNumber = pForm->nInitNumber;
	// プログレスバー初期化
	if (isCopy) {
		m_pDoc->StartLoadFile("ファイルをコピー中...");
	} else {
		m_pDoc->StartLoadFile("ファイルを移動中...");
	}
	m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, "しばらくお待ち下さい...");
	CString strConvFileNameBefore;
	CString strConvFileNameAfter;

	int nOverWriteFlag = 0; /* STEP 041 */
	for (int i=0;i<nCount;i++) {
		if (nOverWriteFlag == 3)	break;
		// プログレスバー更新
		if (nCount > 1) {
			m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
		}
		int		nIndex = arrayList[i];
		int		nNumber = (int)GetLParamFromIndex(nIndex);
		if (!(nNumber >= 0))	continue;
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);
		if (fileMP3->bModifyFlag == true) {
			MessageBox("変更が保存されていないファイルが含まれています\n\n"
					   "変更が保存されていないファイルは無視します",
					   "選択ファイルを移動/コピー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			continue;
		}

		// ファイル名から拡張子を除いた文字を取得
		CString	strFileName = GetFileColumnText(fileMP3, COLUMN_FILE_NAME);
		TCHAR	sFileName[_MAX_FNAME];
		_tsplitpath_s(strFileName, NULL,NULL, NULL,NULL, sFileName,_MAX_FNAME, NULL,NULL);

		CString	strText = pForm->strFormat;
		CString	strDiskNumber, strDiskNumber2, strDiskNumber3;
		auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));

		// １桁のディスク番号
		strDiskNumber.Format("%d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
		// ２桁のディスク番号
		strDiskNumber2.Format("%02d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
		// ３桁のディスク番号
		strDiskNumber3.Format("%03d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));


		// 書式を使って文字列を作成
		strText = StrReplace(strText, "%FILE_NAME%"    , sFileName);
		strText = StrReplace(strText, "%TRACK_NAME%"   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%ALBUM_NAME%"   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%ARTIST_NAME%"  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%TRACK_NUMBER%" , strTrackNumber);
		strText = StrReplace(strText, "%TRACK_NUMBER2%", strTrackNumber2);
		strText = StrReplace(strText, "%TRACK_NUMBER3%", strTrackNumber3);
		strText = StrReplace(strText, "%DISK_NUMBER%" , strDiskNumber);
		strText = StrReplace(strText, "%DISK_NUMBER2%", strDiskNumber2);
		strText = StrReplace(strText, "%DISK_NUMBER3%", strDiskNumber3);
		strText = StrReplace(strText, "%RELEASE_YEAR%" , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding("\r"));
		strText = StrReplace(strText, "%COMMENT%"      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding("\r")/* BeachMonster 089 */);
		strText = StrReplace(strText, "%GENRE%"        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding("\r"));
		strText = StrReplace(strText, "%STRING%"       , pForm->strFixString);
		// SIF の項目
		strText = StrReplaceSIF(strText, fileMP3);

		// 制御コード（一部）をスペースに置き換え /* SeaKnows2 040 */
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
						//dialog.m_strMsg.Format("作成するフォルダ名を %d 文字以内に変更してください", g_nConfFileNameMaxChar);
						dialog.m_strMsgFormat = "作成するフォルダ名を %d(%d) 文字以内に変更してください";
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

		{// フォルダを作成して移動またはコピー
			int nStart = 0;
			BOOL res = TRUE;
			CString strMakePath;
			strFolder.TrimRight('\\');
			CString strPathName = strFolder + "\\" + strText;
			//if (strPathName.Right(1) == "\\") {
			//	strPathName=strPathName.Left(strPathName.GetLength()-1);
			//}

			// ディレクトリを1つずつ繰り返し作成
			CString strNewPath = strPathName;
			strPathName.TrimRight('\\');
			strPathName += _T("\\");
			while(strMakePath + _T("\\") != strPathName)
			{
				// 作成するディレクトリ名を設定
				nStart = strPathName.Find('\\', nStart + 1);
				strMakePath = strPathName.Left(nStart);
				strNewPath = strMakePath;
				CFileStatus rStatus;
				// 末尾のピリオドを削除
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
				// ディレクトリが存在するかチェックし無ければ作成
				DWORD dwAttr = GetFileAttributes(strNewPath);
				if (dwAttr == 0xFFFFFFFF || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
				//if(!(CFile::GetStatus(strNewPath, rStatus) || (strNewPath.GetLength() == 2 && strNewPath.GetAt(1) == ':'))) {
					res = ::CreateDirectory(strNewPath, NULL);
					if (!res) break;
				}
			}
			if (!res) {
				MessageBox(strNewPath + " が作成できませんでした", "移動/コピーエラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
				break;
			}
			strNewPath += _T("\\");
			SHFILEOPSTRUCT sfo;
			ZeroMemory(&sfo, sizeof(sfo));
			sfo.hwnd = GetSafeHwnd();
			sfo.wFunc = FO_MOVE;
			sfo.lpszProgressTitle = _T("移動しています...");
			CDoubleZeroString strFile;
			strFile.Add(GetFileColumnText(fileMP3, COLUMN_FULL_PATH_NAME) + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
			{
				TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				TCHAR	sLyricsFile[FILENAME_MAX+1];
				// MP3 ファイルと同じフォルダにある歌詞ファイルを検索
				_tsplitpath(fileMP3->strFullPathName, drive, dir, fname, ext);
				// .lrc => .txt の順で検索
				int i; for (i = 0; i < 2; i++) {
					LPSTR	sLyricsExt = (i == 0) ? ".lrc" : ".txt";
					_tmakepath_s(sLyricsFile,FILENAME_MAX+1, drive, dir, fname, sLyricsExt);
					if (GetFileAttributes(sLyricsFile) != 0xFFFFFFFF) {
						// ファイルを発見：歌詞ファイルを加える
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
					dialog.m_strSize.Format("%ld byte", fileStatus.m_size);
					if (fileStatus.m_mtime.GetTime() < 0) {
						dialog.m_strTimeStamp.Format("----/--/-- --:--:--");
					} else {
						dialog.m_strTimeStamp.Format("%04d/%02d/%02d %02d:%02d:%02d",
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
					case 0:				// このファイルを上書き
					case 1:				// 以降全て上書き
						break;
					case 2:				// スキップ
						continue;
					case 3:				// 中止
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
				sfo.lpszProgressTitle = _T("コピーしています...");
			}
			CString strStatus;
			if (isCopy) {
				strStatus.Format("%s を %s へコピーしています (%d/%d)", strFile.Get(0) /* FreeFall 047 */, strNewPath, i, nCount);
			} else {
				strStatus.Format("%s を %s へ移動しています (%d/%d)", strFile.Get(0) /* FreeFall 047 */, strNewPath, i, nCount);
			}
			((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(strStatus);	// ステータスバーに進捗状況表示
			if (::SHFileOperation(&sfo) == 0) {
				strMoveFile.Add(strNewPath + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
			} else {
				break;
			}
		}
	}
	OnChangeSelect();	// ステータスバーの表示を元に戻す
	// プログレスバー終了
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
		// 成功したものをリストから削除
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
		// 全てのアイテムの選択解除
		SelectAllItems(FALSE);
		m_pDoc->UpdateAllViews(NULL);
		// 成功したものをリストに追加
		for (int i=0;i<strMoveFile.GetSize();i++) {
			m_pDoc->AddRequestFile(strMoveFile[i], NULL);
		}
		m_pDoc->StartLoadFile("ＭＰ３ファイル読み込み中...");
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
			// 曲数を表示
			CItemInfo	*pInfo = GetData(pTotalParent);
			CString strBuffer;
			CString strLeft, strRight;
			int		nIndex = NodeToIndex(pTotalParent);
			int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
			if (nColNum >= 0) {
				strBuffer = pInfo->GetSubItem(nColNum-1);
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
					strBuffer.Format("%d", atoi(strBuffer)-1);
				} else {
					if (strBuffer.Find("/") != -1) {
						strLeft = strBuffer.Left(strBuffer.Find("/"));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find("/")+1);
						if (pTotalParent != pItemParent) {
							strBuffer.Format("%s/%d", strLeft, atoi(strRight)-1);
						} else {
							strBuffer.Format("%d/%d", atoi(strLeft)-1, atoi(strRight)-1);
						}
					} else {
						if (pTotalParent == pItemParent) {
							ASSERT(0);
							//strBuffer.Format("%d/%d", atoi(strBuffer)-1, atoi(strBuffer)-1); // いらないか？
						} else {
						if (atoi(strBuffer) > 1) {
							strBuffer.Format("%s/%d", strBuffer, atoi(strBuffer)-1);
						} else {
							strBuffer.Format("%s", strBuffer);
						}
						}
					}
				}
				pInfo->SetSubItemText(nColNum-1, strBuffer);
				SetItemText(nIndex, nColNum, strBuffer);
			}
			// 演奏時間合計を表示
			nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
			if (nColNum >= 0) {
				strBuffer = GetFileColumnText(fileMP3, COLUMN_PLAY_TIME);
				UINT nTotalTime = Time2Sec(strBuffer);
				strBuffer = pInfo->GetSubItem(nColNum-1);
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
					nTotalTime = Time2Sec(strBuffer) - nTotalTime;
					strBuffer.Format(_T("%d:%02d"), nTotalTime/60, nTotalTime%60);
				} else {
					if (strBuffer.Find("/") != -1) {
						strLeft = strBuffer.Left(strBuffer.Find("/"));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find("/")+1);
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
			//サイズ合計を表示
			nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
			if (nColNum >= 0) {
				strBuffer = pInfo->GetSubItem(nColNum-1);
				strBuffer.TrimRight("MB");
				if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
					strBuffer.Format("%.2fMB", atof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024);
				} else {
					if (strBuffer.Find("/") != -1) {
						strLeft = strBuffer.Left(strBuffer.Find("/"));
						strRight = strBuffer;
						strRight = strBuffer.Mid(strBuffer.Find("/")+1);
						if (pTotalParent != pItemParent) {
							strBuffer.Format("%s/%.2fMB", strLeft, atof(strRight) - (float)fileMP3->lFileSize / 1024 / 1024);
						} else {
							strBuffer.Format("%.2fMB/%.2fMB", atof(strLeft) - (float)fileMP3->lFileSize / 1024 / 1024, atof(strRight) - (float)fileMP3->lFileSize / 1024 / 1024);
						}
					} else {
						if (pTotalParent == pItemParent) {
							ASSERT(0);
							//strBuffer.Format("%.2fMB/%.2fMB", atof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024, atof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024); // いらないか？
						} else {
							strLeft.Format("%sMB", strBuffer);
							if ((atof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024) > 0.0) {
								strRight.Format("%.2fMB", atof(strBuffer) - (float)fileMP3->lFileSize / 1024 / 1024);
								strBuffer.Format("%s/%s", strLeft, strRight);
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
						// 漢字リーディングバイト
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
			// 詳細を設定
			if (strToolTip != "Root") {
				if (ItemHasChildren(pItem)) {
					int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[Track数] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
					nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[全演奏時間] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
					nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
					if (nColNum >= 0) {
						if (pInfo->GetSubItem(nColNum-1).GetLength() > 0) {
							strToolTip += (_T("\n[サイズ合計] ") + pInfo->GetSubItem(nColNum-1));
						}
					}
				} else if ((int)pInfo->GetLParam() >= 0) {
					FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
					strToolTip += (_T("\r[ﾀｲﾄﾙ] ") + GetFileColumnText(fileMP3, COLUMN_TRACK_NAME));
					strToolTip += (_T("\n[ｱｰﾃｨｽﾄ] ") + GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME));
					strToolTip += (_T("\n[ｱﾙﾊﾞﾑ] ") + GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME));
					strToolTip += (_T("\n[TrackNo] ") + GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER));
					strToolTip += (_T("\n[演奏時間] ") + GetFileColumnText(fileMP3, COLUMN_PLAY_TIME));
					strToolTip += (_T("\n[年] ") + GetFileColumnText(fileMP3, COLUMN_YEAR));
					strToolTip += (_T("\n[ｼﾞｬﾝﾙ] ") + GetFileColumnText(fileMP3, COLUMN_GENRE));
					CString strComment = GetFileColumnText(fileMP3, COLUMN_COMMENT);
					{
						BOOL bLead = FALSE;
						for (int i=0;i<strComment.GetLength();i++) {
							if (_ismbblead(strComment[i])) {
								// 漢字リーディングバイト
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
					strToolTip += (_T("\n[ｺﾒﾝﾄ] ") + strComment);
					/* Conspiracy 198 */
					strComment = GetFileColumnText(fileMP3, COLUMN_FORMAT);
					{
						BOOL bLead = FALSE;
						for (int i=0;i<strComment.GetLength();i++) {
							if (_ismbblead(strComment[i])) {
								// 漢字リーディングバイト
								i++;
							} else {
								if (strComment[i] == '\r') {
									strComment.Delete(i);
									i--;
								}
							}
						}
					}
					strToolTip += (_T("\n[ﾌｫｰﾏｯﾄ] ") + strComment);
					strToolTip += (_T("\n[ファイル] ") + GetFileColumnText(fileMP3, COLUMN_FILE_NAME));
					strToolTip += (_T("\n[ﾊﾟｽ] ") + GetFileColumnText(fileMP3, COLUMN_FULL_PATH_NAME));
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
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if (!OnNotifyMessage(wParam, lParam, pResult)) {
		return TRUE;
	}
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CMySuperGrid::OnKillFocus(CWnd* pNewWnd)
{
	CListCtrl::OnKillFocus(pNewWnd);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
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

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
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
						// 編集不可のため灰色
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
			// ファイル名文字数オーバー
			TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
			_tsplitpath(fileMP3->strFileName, NULL, NULL, fname, ext);
			CDlgFileNameChange dialog;
			dialog.m_nMaxChar = g_nConfFileNameMaxChar - strlen(ext);
			dialog.m_strFileName = fname;
			dialog.m_strOrgFileName = fname;
			//dialog.m_strMsg.Format("ファイル名を %d 文字以内に変更してください", dialog.m_nMaxChar);
			dialog.m_strMsgFormat = "ファイル名を %d(%d) 文字以内に変更してください";
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
	strText.Replace("&", "&amp;");
	strText.Replace(" ", "&nbsp;");
	strText.Replace("<", "&lt;");
	strText.Replace(">", "&gt;");
	strText.Replace("\"", "&quot;");
	strText = StrReplace(strText, "\r\n", "<BR>");
	return strText;
}

CString CMySuperGrid::quoteForComment(CString &str) /* RockDance 124 */
{
	if (str.IsEmpty()) {
		return str;
	}
	if (str.Find("\r\n") != -1 || str[0] == '"') {
		str.Replace("\"", "\"\"");
		str = '"' + str + '"';
	}
	return str;
}

bool CMySuperGrid::isOneCellClipboard(CString strBuffer) /* RockDance 124 */
{
	// クリップボードから取得データが１セル分のデータかチェックする
	//strBuffer.Replace("\"\"", ""); // "" は無視する
	int		nLength = strBuffer.GetLength();
	if (nLength == 0) return true;

	//if (ey >= GetItemCount()) ey = GetItemCount() - 1;
	int nCount = 0;
	// 選択範囲のセルの内容を文字列に変換
	char	*pBuffer = strBuffer.GetBuffer(nLength);
	int		nPos = 0;
	while (true) {
		// １行分のバッファを取得
		char	*pLineTop = pBuffer + nPos;
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

		char	*pStr;
		if ((pStr = GetToken(pLineTop, "\t")) != NULL) {
			nCount++;
			while (true) {
				if ((pStr = GetToken(NULL, "\t")) != NULL) {
					nCount++;
				} else {
					break;
				}
			}
		}

		// [CR] [LF] の残りをスキップ
		while(pBuffer[nPos] == '\n') {
			nPos++;
		}

		if (nPos >= nLength) {
			// バッファの文字列が無くなった
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
		str.Replace("\"\"", "\""); // "" を " にする
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
			//  ファルダ
			//count = GetFolderFileCount(pTop, pParent, count);
			continue; // サブフォルダは対象としない
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
		pInfo->SetSubItemText(nColNum-1, "");
		SetItemText(nIndex, nColNum, "");
	}
	nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
	if (nColNum >= 0) {
		pInfo->SetSubItemText(nColNum-1, "");
		SetItemText(nIndex, nColNum, "");
	}
	nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
	if (nColNum >= 0) {
		pInfo->SetSubItemText(nColNum-1, "");
		SetItemText(nIndex, nColNum, "");
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
			//  ファルダ
			int		nIndex = NodeToIndex(pParent);
			int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, "");
				SetItemText(nIndex, nColNum, "");
			}
			nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, "");
				SetItemText(nIndex, nColNum, "");
			}
			nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
			if (nColNum >= 0) {
				lp->SetSubItemText(nColNum-1, "");
				SetItemText(nIndex, nColNum, "");
			}
			CalcSum(pTop, pParent);
		} else {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)lp->GetLParam());
			CTreeItem* pTotalParent = pItemParent;
			while (pTotalParent != NULL) {
				// 曲数を表示
				CItemInfo	*pInfo = GetData(pTotalParent);
				CString strBuffer;
				CString strLeft, strRight;
				int		nIndex = NodeToIndex(pTotalParent);
				int		nColNum = g_nColumnNumberList[COLUMN_TRACK_NUMBER];
				if (nColNum >= 0) {
					strBuffer = pInfo->GetSubItem(nColNum-1);
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
						strBuffer.Format("%d", atoi(strBuffer)+1);
					} else {
						if (strBuffer.Find("/") != -1) {
							strLeft = strBuffer.Left(strBuffer.Find("/"));
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find("/")+1);;
							strBuffer.Format("%d/%d", atoi(strLeft) + (pTotalParent == pItemParent ? 1 : 0), atoi(strRight)+1);
						} else {
							if (pTotalParent == pItemParent) {
								ASSERT(0);
							} else {
								CString strOrg = strBuffer;
								strBuffer.Format("%s/%d", strOrg, atoi(strBuffer)+1);
							}
						}
					}
					pInfo->SetSubItemText(nColNum-1, strBuffer);
					SetItemText(nIndex, nColNum, strBuffer);
				}
				// 演奏時間合計を表示
				nColNum = g_nColumnNumberList[COLUMN_PLAY_TIME];
				if (nColNum >= 0) {
					strBuffer = GetFileColumnText(fileMP3, COLUMN_PLAY_TIME);
					UINT nTotalTime = Time2Sec(strBuffer);
					strBuffer = pInfo->GetSubItem(nColNum-1);
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
						nTotalTime += Time2Sec(strBuffer);
						strBuffer.Format(_T("%d:%02d"), nTotalTime/60, nTotalTime%60);
					} else {
						if (strBuffer.Find("/") != -1) {
							strLeft = strBuffer.Left(strBuffer.Find("/"));
							if (pTotalParent == pItemParent) {
								UINT nTime = nTotalTime;
								nTime += Time2Sec(strLeft);
								strLeft.Format("%d:%02d", nTime/60, nTime%60);
							}
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find("/")+1);;
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
				//サイズ合計を表示
				nColNum = g_nColumnNumberList[COLUMN_FILE_SIZE];
				if (nColNum >= 0) {
					strBuffer = pInfo->GetSubItem(nColNum-1);
					strBuffer.TrimRight("MB");
					if ((pTotalParent == pItemParent || nIndex == 0) && strBuffer.Find("/") == -1) {
						strBuffer.Format("%.2fMB", atof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024);
					} else {
						if (strBuffer.Find("/") != -1) {
							strLeft = strBuffer.Left(strBuffer.Find("/"));
							if (pTotalParent == pItemParent) {
								strLeft.Format("%.2fMB", atof(strLeft) + (float)fileMP3->lFileSize / 1024 / 1024);
							}
							strRight = strBuffer;
							strRight = strBuffer.Mid(strBuffer.Find("/")+1);;
							strBuffer.Format("%s/%.2fMB", strLeft, atof(strRight) + (float)fileMP3->lFileSize / 1024 / 1024);
						} else {
							if (pTotalParent == pItemParent) {
								ASSERT(0);
							} else {
								strLeft.Format("%sMB", strBuffer);
								strRight.Format("%.2fMB", atof(strBuffer) + (float)fileMP3->lFileSize / 1024 / 1024);
								strBuffer.Format("%s/%s", strLeft, strRight);
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
	// クリップボードを開いてテキストを取得
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
		LPSTR pBuffer;
		if ((pBuffer = (LPSTR)GlobalLock(hMem)) != NULL) {
			/* RockDance 124 *///PasteString(pBuffer);
#ifdef FLICKERFREE
			//SetRedraw(FALSE);
#endif
			if (strlen(pBuffer) > 0) { /* STEP 033 */
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

void CMySuperGrid::PasteString(char *sBuffer, int nPastePos, bool bText, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */) /* Baja 171 */
{
	bool bSaveLoadFileAdjustColumn = g_bOptLoadFileAdjustColumn; /* RockDance 130 */
	g_bOptLoadFileAdjustColumn = false; /* RockDance 130 */

	int		sx, sy, ex, ey;
	if (IsRangeSelected() == false) {
		if (GetSelectedItem() == -1) return;	// エラー
		if (GetCurSubItem() < COLUMN_MIN) return;		// エラー
		// 範囲選択無し
		sx = GetCurSubItem();
		sy = GetSelectedItem();
		ex = ey = 99999;
	} else {
		// 範囲選択あり
		int		nTemp;
		sx = m_posMultiSelect[0].x;
		sy = m_posMultiSelect[0].y;
		ex = m_posMultiSelect[1].x;
		ey = m_posMultiSelect[1].y;
		if (ex < sx) {nTemp = sx; sx = ex; ex = nTemp;}
		if (ey < sy) {nTemp = sy; sy = ey; ey = nTemp;}
		if (sx <= COLUMN_MIN) sx = COLUMN_MIN;

		// 最後の改行を取り除く
		int		nLen = strlen(sBuffer);
		while(nLen > 0 && (sBuffer[nLen-1] == '\r' || sBuffer[nLen-1] == '\n')) {
			nLen--;
			sBuffer[nLen] = '\0';
		}
		// タブか改行が含まれているか？
		 /* RockDance 124 *///if (strchr(sBuffer, '\t') == NULL && strchr(sBuffer, '\n') == NULL) {
		if ((bText == true && strchr(sBuffer, '\t') == NULL && strchr(sBuffer, '\n') == NULL) || (bText == false && isOneCellClipboard(sBuffer))) {  /* RockDance 124 */
			CString	strBuffer = sBuffer;
			// 含まれていない場合は、選択されている範囲全てにペースト
			for (int nItem = sy; nItem <= ey; nItem++) {
				CTreeItem	*pItem = GetTreeItem(nItem);
				if (pItem != NULL && IsItemFile(pItem) == TRUE) {
					CItemInfo	*pItemInfo = GetData(pItem);
					for (int nColumn = sx; nColumn <= ex; nColumn++) {
						// セルに文字列を設定
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

	// 選択範囲のセルの内容を文字列に変換
	char	*pBuffer = strBuffer.GetBuffer(nLength);
	int		nPos = 0;
	int		nEndColumn = 0; /* RockDance 130 */
	for (int nItem = sy; nItem <= ey; nItem++) {
		CTreeItem	*pItem = GetTreeItem(nItem);
		if (pItem != NULL && IsItemFile(pItem) == TRUE) {
			CItemInfo	*pItemInfo = GetData(pItem);

			// １行分のバッファを取得
			char	*pLineTop = pBuffer + nPos;
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
						// 見つからなかった
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

			char	*pStr;
			if ((pStr = GetToken(pLineTop, "\t")) != NULL) {
				for (int nColumn = sx; nColumn <= ex; nColumn++) {
					nEndColumn = nColumn; /* RockDance 130 */
					// セルに文字列を設定
					/* RockDance 124 *///ChangeSubItemText(nItem, nColumn, pStr, nPastePos/* FreeFall 052 */);
					if (bText) { /* RockDance 124 */
						ChangeSubItemText(nItem, nColumn, pStr, nPastePos, bAddSpace, strAddFront, strAddBack);
					} else {
						ChangeSubItemText(nItem, nColumn, unquoteForComment(CString(pStr)), nPastePos, bAddSpace, strAddFront, strAddBack);
					}
#ifdef FLICKERFREE
					//InvalidateItemRect(nItem);
#endif

					// 次のセルの文字列を取得
					if ((pStr = GetToken(NULL, "\t")) == NULL) break;
				}
			}

			// [CR] [LF] の残りをスキップ
			while(pBuffer[nPos] == '\n') {
				nPos++;
			}

			if (nPos >= nLength) {
				// バッファの文字列が無くなった
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

void CMySuperGrid::ChangeSubItemText(int iItem, int iSubItem, const char *sUpdateText, int nPos, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */, bool bUpdateInternal/* STEP 037 */)
{
	if (iItem != -1 && iSubItem > 0 && sUpdateText/* sText FreeFall 052 */ != NULL) {
		CTreeItem	*pSelItem = GetTreeItem(iItem);

		if (pSelItem != NULL) {
			CItemInfo	*pInfo = GetData(pSelItem);

			// 変更不可のセルの場合は、無視する
			CONTROLTYPE ctrlType;
			if (pInfo->GetControlType(iSubItem-1, ctrlType)) {
				if (ctrlType == invalid && !bUpdateInternal) {
					return;
				}
			}

			// フォルダ
			if ((int)pInfo->GetLParam() < 0) return;

			CString sText = sUpdateText;
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());
			CString strOrgText = GetFileColumnText(fileMP3, g_nColumnTypeList[iSubItem]);
			switch (nPos) {
			case 0:
				sText = strAddFront + sUpdateText + strAddBack;
				break;
			case 1:
				sText = strAddFront + sUpdateText + strAddBack + ((bAddSpace ? " " : "") +  strOrgText);
				break;
			case 2:
				sText = strOrgText + (bAddSpace ? " " : "") + strAddFront + sUpdateText + strAddBack;
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

			// 文字数をチェックする
			int		nLimit = OnGetColumLimitText(iItem, iSubItem);
			if (nLimit < (int)strlen(sText)) {
				// 文字数を最大文字数に収まるように調整
				char*	sBuffer = new char[nLimit+1];
				StringCopyN(sBuffer, sText, nLimit);
				sBuffer[nLimit] = '\0';
				strText = sBuffer;
				delete [] sBuffer;
			} else {
				// 最大文字数以下なのでそのまま
				strText = sText;
			}
			strText.TrimRight();

			switch(g_nColumnTypeList[iSubItem]) {
			case COLUMN_FILE_NAME:		// ファイル名
				{
					// 事前にやらないと'\','/'が_tsplitpath()でパス区切りとして認識されてしまう /* WildCherry2 077 */
					if (g_nOptCheckFileName == FILENAME_CONV_MULTIBYTE) {
						// ファイル名に使用できない文字を置換
						strText = ReplaceFileName(fileMP3, strText);
					}
					// 拡張子が無い場合は、拡張子を付ける
					TCHAR	extOrg[_MAX_EXT];
					TCHAR	fname[_MAX_FNAME], ext[_MAX_EXT];
					_tsplitpath(fileMP3->strFullPathName, NULL, NULL, NULL, extOrg);
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
					if (/*strlen(ext) != 4 ||*/ !CheckFileExt(fileMP3, ext)
					)/*
					||  !(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						  strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						  strcmpi(ext, ".asf") == 0 ||
						  strcmpi(ext, ".ape") == 0 ||
						  strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						  strcmpi(ext, ".ogg") == 0)) {//修正 by Kobarin
					*/
						strText += extOrg;
					/*}*/

					// 上に移動 /* WildCherry2 077 */
					// 拡張子変換のため再度呼び出す /* STEP 006 */
					if (g_nOptCheckFileName == FILENAME_CONV_MULTIBYTE) {
						// ファイル名に使用できない文字を置換
						strText = ReplaceFileName(fileMP3, strText);
					}
				}
				if (fileMP3->strFileName != strText) {
					fileMP3->strFileName = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_FILE_TIME:		// ファイル更新日
				if (g_bOptKeepTimeStamp == true) {
					CTime	time = fileMP3->time;
					// 入力されたタイムスタンプを解析
					if (strText.GetLength() > 0) {
						char	*sBuffer = new char[strText.GetLength()+1];
						char	*ptr;
						strcpy(sBuffer, sText);
						int		nYear = 2000, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
						if ((ptr = strtok(sBuffer, "/")) != NULL) {		// 年
							nYear = atoi(ptr);
						}
						if (ptr && (ptr = strtok(NULL, "/")) != NULL) {	// 月
							nMonth = atoi(ptr);
						}
						if (ptr && (ptr = strtok(NULL, " ")) != NULL) {	// 日
							nDay = atoi(ptr);
						}
						if (ptr && (ptr = strtok(NULL, ":")) != NULL) {	// 時
							nHour = atoi(ptr);
						}
						if (ptr && (ptr = strtok(NULL, ":")) != NULL) {	// 分
							nMin = atoi(ptr);
						}
						if (ptr && (ptr = strtok(NULL, "")) != NULL) {	// 秒
							nSec = atoi(ptr);
						}
						delete[]	sBuffer;
						// 入力された値を調整
						if (nYear < 1971) nYear = 1971;
						// ※本来は 1970 年だが、1970/01/01 00:00:00 だと CTime で
						//   Assert が発生するので、1971 年からとする
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
						// 入力された時間を設定
						time = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
					}

					// 更新？
					if (fileMP3->time != time) {
						fileMP3->time = time;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
					strText.Format("%04d/%02d/%02d %02d:%02d:%02d",
					               fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
					               fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
				}
				break;

			case COLUMN_TRACK_NAME:		// トラック名
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strTrackNameSI != strText) {
						fileMP3->strTrackNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					if (fileMP3->strTrackName != strText) {
						fileMP3->strTrackName = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_ARTIST_NAME:	// アーティスト名
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strArtistNameSI != strText) {
						fileMP3->strArtistNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					if (fileMP3->strArtistName != strText) {
						fileMP3->strArtistName = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_ALBUM_NAME:		// アルバム名
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strAlbumNameSI != strText) {
						fileMP3->strAlbumNameSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					if (fileMP3->strAlbumName != strText) {
						fileMP3->strAlbumName = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_TRACK_NUMBER:	// トラック番号
				if (!IsEditTrackNumberSIF(fileMP3) && !CFileMP3::IsNumeric(strText)) {
					int		nTrackNumber;
					nTrackNumber = atoi(CFileMP3::GetIntTrackNo(strText));
					if (nTrackNumber < 0) nTrackNumber = 0;
					if (nTrackNumber > 255) nTrackNumber = 254;
					strText.Format("%d", nTrackNumber);
				}
				if (IsEditTrackNumberSIF(fileMP3)) {
					if (fileMP3->strTrackNumberSI != strText) {
						fileMP3->strTrackNumberSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					int		nTrackNumber;
					if (strText.IsEmpty() != TRUE) {
						nTrackNumber = atoi(CFileMP3::GetIntTrackNo(strText));
						if (nTrackNumber < 0) nTrackNumber = 0;
						if (nTrackNumber > 255) nTrackNumber = 254;
						if (nTrackNumber == 0) {
							nTrackNumber = 0xFF;
							strText = "";
						} else if (!IsEditTrackNumberSIF(fileMP3)) {
							strText.Format("%d", nTrackNumber);
						}
					} else {
						nTrackNumber = 0xFF;
					}
					if (fileMP3->strTrackNumber != strText || fileMP3->byTrackNumber != nTrackNumber) {
						fileMP3->strTrackNumber = strText;
						fileMP3->byTrackNumber = nTrackNumber;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_DISK_NUMBER:	// ディスク番号
				if (!IsEditFieldSIF(fileMP3) && !CFileMP3::IsNumeric(strText)) {
					int		nDiskNumber;
					nDiskNumber = atoi(CFileMP3::GetIntDiskNo(strText));
					if (nDiskNumber < 0) nDiskNumber = 0;
					if (nDiskNumber > 255) nDiskNumber = 254;
					strText.Format("%d", nDiskNumber);
				}
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strDiskNumberSI != strText) {
						fileMP3->strDiskNumberSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					int		nDiskNumber;
					if (strText.IsEmpty() != TRUE) {
						nDiskNumber = atoi(CFileMP3::GetIntDiskNo(strText));
						if (nDiskNumber < 0) nDiskNumber = 0;
						if (nDiskNumber > 255) nDiskNumber = 254;
						if (nDiskNumber == 0) {
							nDiskNumber = 0xFF;
							strText = "";
						} else if (!IsEditFieldSIF(fileMP3)) {
							strText.Format("%d", nDiskNumber);
						}
					} else {
						nDiskNumber = 0xFF;
					}
					if (fileMP3->strDiskNumber != strText || fileMP3->byDiskNumber != nDiskNumber) {
						fileMP3->strDiskNumber = strText;
						fileMP3->byDiskNumber = nDiskNumber;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_YEAR:			// リリース年号
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strYearSI != strText) {
						fileMP3->strYearSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					if (fileMP3->strYear != strText) {
						fileMP3->strYear = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_GENRE:			// ジャンル番号
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
					if (strText.IsEmpty()) {		// 空の文字列(=クリア)
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

			case COLUMN_COMMENT:		// コメント
				if (IsEditFieldSIF(fileMP3)) {
					if (fileMP3->strCommentSI != strText) {
						fileMP3->strCommentSI = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				} else {
					if (fileMP3->strComment != strText) {
						fileMP3->strComment = strText;
						fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
					}
				}
				break;

			case COLUMN_COPYRIGHT:		// 著作権
				if (fileMP3->strCopyrightSI != strText) {
					fileMP3->strCopyrightSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_ENGINEER:		// エンジニア(製作者)
				if (fileMP3->strEngineerSI != strText) {
					fileMP3->strEngineerSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_SOURCE:			// ソース
				if (fileMP3->strSourceSI != strText) {
					fileMP3->strSourceSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_SOFTWARE:		// ソフトウェア
				if (fileMP3->strSoftwareSI != strText) {
					fileMP3->strSoftwareSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_KEYWORD:		// キーワード
				if (fileMP3->strKeywordSI != strText) {
					fileMP3->strKeywordSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_TECHNICIAN:		// 技術者
				if (fileMP3->strTechnicianSI != strText) {
					fileMP3->strTechnicianSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_LYRIC:			// 歌詞
				if (fileMP3->strLyricSI != strText) {
					fileMP3->strLyricSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_COMMISSION:		// コミッション
				if (fileMP3->strCommissionSI != strText) {
					fileMP3->strCommissionSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_WRITER:		// 作詞 /* ADD MIMURA */
				if (fileMP3->strWriterSI != strText) {
					fileMP3->strWriterSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_COMPOSER:		// 作曲 /* Baja 154 */
				if (fileMP3->strComposerSI != strText) {
					fileMP3->strComposerSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_ALBM_ARTIST:	// Orig.アーティスト /* Baja 154 */
				if (fileMP3->strAlbmArtistSI != strText) {
					fileMP3->strAlbmArtistSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_ORIG_ARTIST:	// Orig.アーティスト /* Baja 154 */
				if (fileMP3->strOrigArtistSI != strText) {
					fileMP3->strOrigArtistSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_URL:		// URL /* Baja 154 */
				if (fileMP3->strURLSI != strText) {
					fileMP3->strURLSI = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_ENCODEST:		// エンコードした人 /* Baja 154 */
				if (fileMP3->strEncodest != strText) {
					fileMP3->strEncodest = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;

			case COLUMN_OTHER:		// その他
				if (fileMP3->strOther != strText) {
					fileMP3->strOther = strText;
					fileMP3->bModifyFlag = TRUE;	// 編集フラグを設定する
				}
				break;
			}

			// セル内容を更新
			if (!bUpdateInternal) { /* STEP 037 */
				pInfo->SetSubItemText(iSubItem-1, strText);
				UpdateData(pSelItem, pInfo);
				SetItemText(iItem, iSubItem, strText);

				// カラム幅の自動調節
				if (g_bOptLoadFileAdjustColumn) AutoSizeColumns(iSubItem);
			}
		}
	}
}

void CMySuperGrid::ClipboardCopyFormat(USER_COPY_FORMAT_FORMAT *pForm) /* FunnyCorn 175 */
{
	if (pForm->strFormat.IsEmpty()) {
		// 書式が空
		return;
	}

	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);
	/*
	int		sx, sy, ex, ey;
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
	}
	*/

	// 固定文字列を入力させる
	if (pForm->strFormat.Find("%STRING%") != -1) {
		CDlgFixedString	dialog;
		dialog.m_strText = pForm->strFixString;
		if (dialog.DoModal() == IDCANCEL) {
			// キャンセル
			return;
		}
		pForm->strFixString = dialog.m_strText;
	}

	CStringArray strMoveFile;
	// 連番変換書式の設定
	//char	*sNumFormat = "%01d";
	//sNumFormat[2] = '0' + pForm->nColumnCount;

	//int		nNumber = pForm->nInitNumber;
	CString strClipboard = "";
	for (int i=0;i<nCount;i++) {
		int		nIndex = arrayList[i];
		int		nNumber = (int)GetLParamFromIndex(nIndex);
		if (!(nNumber >= 0))	continue;
		FILE_MP3	*fileMP3 = m_pDoc->GetListMP3(nNumber);

		// ファイル名から拡張子を除いた文字を取得
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
		CString	strDiskNumber, strDiskNumber2, strDiskNumber3;
		auto[strTrackNumber, strTrackNumber2, strTrackNumber3] = GetIntTrackNo(GetFileColumnText(fileMP3, COLUMN_TRACK_NUMBER).SpanExcluding(TEXT("\r")));

		// １桁のディスク番号
		strDiskNumber.Format("%d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
		// ２桁のディスク番号
		strDiskNumber2.Format("%02d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));
		// ３桁のディスク番号
		strDiskNumber3.Format("%03d", atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER).SpanExcluding("\r"))));

		// 書式を使って文字列を作成
		strText = StrReplace(strText, "%FILE_NAME%"    , sFileName);
		strText = StrReplace(strText, "%TRACK_NAME%"   , GetFileColumnText(fileMP3, COLUMN_TRACK_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%ALBUM_NAME%"   , GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%ARTIST_NAME%"  , GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME).SpanExcluding("\r"));
		strText = StrReplace(strText, "%TRACK_NUMBER%" , strTrackNumber);
		strText = StrReplace(strText, "%TRACK_NUMBER2%", strTrackNumber2);
		strText = StrReplace(strText, "%TRACK_NUMBER3%", strTrackNumber3);
		strText = StrReplace(strText, "%DISK_NUMBER%" , strDiskNumber);
		strText = StrReplace(strText, "%DISK_NUMBER2%", strDiskNumber2);
		strText = StrReplace(strText, "%DISK_NUMBER3%", strDiskNumber3);
		strText = StrReplace(strText, "%RELEASE_YEAR%" , GetFileColumnText(fileMP3, COLUMN_YEAR).SpanExcluding("\r"));
		strText = StrReplace(strText, "%COMMENT%"      , GetFileColumnText(fileMP3, COLUMN_COMMENT).SpanExcluding("\r")/* BeachMonster 089 */);
		strText = StrReplace(strText, "%GENRE%"        , GetFileColumnText(fileMP3, COLUMN_GENRE).SpanExcluding("\r"));
		strText = StrReplace(strText, "%STRING%"       , pForm->strFixString);
		// SIF の項目
		strText = StrReplaceSIF(strText, fileMP3);

		// 総合演奏時間
		CString strBuffer;
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format("%02d:%02d", fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
		} else {
			strBuffer = "     ";
		}
		strText = StrReplace(strText, "%TOTAL_SEC%", strBuffer);
		// 総合演奏時間 /* STEP 020 */
		if (fileMP3->nPlayTime >= 0 && fileMP3->nPlayTime/60/60 > 0) {
			strBuffer.Format("%d:%02d:%02d", fileMP3->nPlayTime/60/60, (fileMP3->nPlayTime/60)%60, fileMP3->nPlayTime%60);
		}
		strText = StrReplace(strText, "%TOTAL_SEC3%", strBuffer);
		// 総合演奏時間(秒)
		if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
			strBuffer.Format("%d", fileMP3->nPlayTime);
		} else {
			strBuffer = "     ";
		}
		strText = StrReplace(strText, "%TOTAL_SEC2%", strBuffer);
		// フォーマット
		{ /* STEP 012 */
			CString strFormat = fileMP3->strVisualFormat;
			if (strFormat.GetLength() > 0 && fileMP3->strAudioFormat.GetLength() > 0) {
				strFormat += "\n";
			}
			strFormat += fileMP3->strAudioFormat;
			strText = StrReplace(strText, "%FORMAT%", strFormat);
		}

		// 制御コード（一部）をスペースに置き換え
		strText = StrReplace(strText, "\n", " ");
		strText = StrReplace(strText, "\r", " ");
		//strText = StrReplace(strText, "\t", " ");

		if (!strClipboard.IsEmpty()) {
			strClipboard += "\r\n";
		}
		strClipboard += strText;
	}
	LPSTR pBuffer;
	HGLOBAL hMem;
	hMem = GlobalAlloc(GHND|GMEM_DDESHARE, strClipboard.GetLength()+1);
	pBuffer = (LPSTR)GlobalLock(hMem);
	strcpy(pBuffer, strClipboard);
	GlobalUnlock(hMem);

	// クリップボードを開いてコピー
	OpenClipboard();
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, (HANDLE)hMem);
	CloseClipboard();

	return;
}

bool CMySuperGrid::DeleteCharSpace(int /*nPos*/) /* FunnyCorn 177 */
{
	int		sx, sy, ex, ey;
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return(false);					// 範囲選択無し
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

				// セルのテキストを取得
				strText = GetFileColumnText(fileMP3, g_nColumnTypeList[nColumn]);

				// ファイル名の場合、拡張子が全角に変換されないようにする
				if (g_nColumnTypeList[nColumn] == COLUMN_FILE_NAME) {
					_tsplitpath(strText, NULL, NULL, fname, ext);
					extern bool CheckFileExt(const FILE_MP3* pFileMP3, const char* ext);
					if (/*strlen(ext) == 4 && */CheckFileExt(fileMP3, ext)
					) {/*
					&&	(_strcmpi(ext, ".mp3") == 0 || strcmpi(ext, ".rmp") == 0 ||
						 strcmpi(ext, ".wma") == 0 || strcmpi(ext, ".wmv") == 0 ||
						 strcmpi(ext, ".asf") == 0 ||
						 strcmpi(ext, ".ape") == 0 ||
						 strcmpi(ext, ".wav") == 0 || strcmpi(ext, ".avi") == 0 || /* Conspiracy 197 *//*
						 strcmpi(ext, ".ogg") == 0)) {
					*/
						// 拡張子を取り除く
						strText = fname;
						bKeepExt = true;
					}
				}


				int nLength;
				do {
					nLength = strText.GetLength();
					strText.TrimLeft(" ");
					strText.TrimLeft("\t");
					if (strText.GetLength() > 1) {
						CString strChar = strText[0];
						strChar += strText[1];
						if (strChar == "　") {
							strText.Delete(0, 2);
						}
					}
				} while (nLength != strText.GetLength());
				do {
					nLength = strText.GetLength();
					strText.TrimRight(" ");
					strText.TrimRight("\t");
					if (strText.GetLength() > 1) {
						CString strChar = strText[strText.GetLength()-2];
						strChar += strText[strText.GetLength()-1];
						if (strChar == "　") {
							strText.Delete(strText.GetLength()-2, 2);
						}
					}
				} while (nLength != strText.GetLength());
				// 取り除いていた拡張子を付けなおす
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

	int		nDiskNumber;
	CString	strDiskNumber;
	CString	strDiskNumber2;
	CString	strDiskNumber3;
	if ((nDiskNumber = atoi(CFileMP3::GetIntDiskNo(GetFileColumnText(fileMP3, COLUMN_DISK_NUMBER)))) != 0) {
		strDiskNumber.Format("%d", nDiskNumber);
		strDiskNumber2.Format("%02d", nDiskNumber);
		strDiskNumber3.Format("%03d", nDiskNumber);
	}

	CString	strFileSize, strFileSizeByte;
	strFileSize.Format("%.2fMB", (float)fileMP3->lFileSize / 1024 / 1024);
	strFileSizeByte.Format("%d", fileMP3->lFileSize);
	CString	strFileTime;
	strFileTime.Format("%04d/%02d/%02d %02d:%02d:%02d",
					  fileMP3->time.GetYear(), fileMP3->time.GetMonth(), fileMP3->time.GetDay(),
					  fileMP3->time.GetHour(), fileMP3->time.GetMinute(), fileMP3->time.GetSecond());
	CString	strFileTimeCreate;
	strFileTimeCreate.Format("%04d/%02d/%02d %02d:%02d:%02d",
							 fileMP3->timeCreate.GetYear(), fileMP3->timeCreate.GetMonth(), fileMP3->timeCreate.GetDay(),
							 fileMP3->timeCreate.GetHour(), fileMP3->timeCreate.GetMinute(), fileMP3->timeCreate.GetSecond());

	CString	strFileNumber;
	strFileNumber.Format("%d", 1 + pStatus->nFileNumber);

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


	// 書式を使って文字列を作成
	CString	strText = strBody;
	strText = StrReplaceEx(strText, "%FILE_NUMBER%"     , strFileNumber, bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_TYPE%"       , GetFileType(fileMP3), bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_NAME%"       , ConvHTML(fileMP3->strFileName, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_NAME_SHORT%" , ConvHTML(strFileNameShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%PATH_NAME%"       , ConvHTML(fileMP3->strFilePath, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%PATH_NAME_SHORT%" , ConvHTML(strPathNameShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%FULL_PATH%"       , ConvHTML(fileMP3->strFullPathName, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%FULL_PATH_SHORT%" , ConvHTML(strFullPathShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%PLAY_PATH%"       , ConvHTML(strPlayPath, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%PLAY_PATH_SHORT%"       , ConvHTML(strPlayPathShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%TRACK_NAME%"      , ConvHTML(GetFileColumnText(fileMP3, COLUMN_TRACK_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%ALBUM_NAME%"      , ConvHTML(GetFileColumnText(fileMP3, COLUMN_ALBUM_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%ARTIST_NAME%"     , ConvHTML(GetFileColumnText(fileMP3, COLUMN_ARTIST_NAME), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%TRACK_NUMBER%"    , strTrackNumber, bIsHtml);
	strText = StrReplaceEx(strText, "%TRACK_NUMBER2%"   , strTrackNumber2, bIsHtml);
	strText = StrReplaceEx(strText, "%TRACK_NUMBER3%"   , strTrackNumber3, bIsHtml);
	strText = StrReplaceEx(strText, "%DISK_NUMBER%"    , strDiskNumber, bIsHtml);
	strText = StrReplaceEx(strText, "%DISK_NUMBER2%"   , strDiskNumber2, bIsHtml);
	strText = StrReplaceEx(strText, "%DISK_NUMBER3%"   , strDiskNumber3, bIsHtml);
	strText = StrReplaceEx(strText, "%RELEASE_YEAR%"    , ConvHTML(GetFileColumnText(fileMP3, COLUMN_YEAR), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%COMMENT%"         , ConvHTML(GetFileColumnText(fileMP3, COLUMN_COMMENT), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%GENRE%"           , ConvHTML(GetFileColumnText(fileMP3, COLUMN_GENRE), bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_SIZE%"       , strFileSize, bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_SIZE_BYTE%"  , ConvNumber(strFileSizeByte), bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_TIME%"       , strFileTime, bIsHtml);
	strText = StrReplaceEx(strText, "%FILE_CTIME%"      , strFileTimeCreate, bIsHtml);
	strText = StrReplaceEx(strText, "%EXIST_LYRIC_FILE%", strLyricFile.IsEmpty() ? "なし" : "あり", bIsHtml);
	strText = StrReplaceEx(strText, "%LYRIC_FILE_NAME%" , ConvHTML(strLyricFile, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%LYRIC_FILE_NAME_SHORT%" , ConvHTML(strLyricFileShort, bWriteHtml), bIsHtml); /* Rumble 191 */
	strText = StrReplaceEx(strText, "%FILE_EXTENSION%"  , strExt, bIsHtml); /* Baja 162 */
	// SIF の情報
	strText = StrReplaceEx(strText, "%COPYRIGHT%"       , ConvHTML(fileMP3->strCopyrightSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%ENGINEER%"        , ConvHTML(fileMP3->strEngineerSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%SOURCE%"          , ConvHTML(fileMP3->strSourceSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%SOFTWARE%"        , ConvHTML(fileMP3->strSoftwareSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%KEYWORD%"         , ConvHTML(fileMP3->strKeywordSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%TECHNICIAN%"      , ConvHTML(fileMP3->strTechnicianSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%LYRIC%"           , ConvHTML(fileMP3->strLyricSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%COMMISSION%"      , ConvHTML(fileMP3->strCommissionSI, bWriteHtml)/* BeachMonster5 120 */, bIsHtml);
	strText = StrReplaceEx(strText, "%WRITER%"			, ConvHTML(fileMP3->strWriterSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%COMPOSER%"		, ConvHTML(fileMP3->strComposerSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%ALBM_ARTIST%"		, ConvHTML(fileMP3->strAlbmArtistSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%ORIG_ARTIST%"		, ConvHTML(fileMP3->strOrigArtistSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%URL%"				, ConvHTML(fileMP3->strURLSI, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%ENCODEST%"		, ConvHTML(fileMP3->strEncodest, bWriteHtml), bIsHtml); /* Baja 154 */
	strText = StrReplaceEx(strText, "%OTHER%"			, ConvHTML(fileMP3->strOther, bWriteHtml), bIsHtml); /* Conspiracy 196 */

	// 総合演奏時間
	CString strBuffer;
	if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
		strBuffer.Format("%02d:%02d", fileMP3->nPlayTime/60, fileMP3->nPlayTime%60);
	} else {
		strBuffer = "     ";
	}
	strText = StrReplaceEx(strText, "%TOTAL_SEC%", strBuffer, bIsHtml);
	// 総合演奏時間 /* STEP 020 */
	if (fileMP3->nPlayTime >= 0 && fileMP3->nPlayTime/60/60 > 0) {
		strBuffer.Format("%d:%02d:%02d", fileMP3->nPlayTime/60/60, (fileMP3->nPlayTime/60)%60, fileMP3->nPlayTime%60);
	}
	strText = StrReplaceEx(strText, "%TOTAL_SEC3%", strBuffer, bIsHtml);
	// 総合演奏時間(秒) /* FunnyCorn 172 */
	if (fileMP3->nPlayTime >= 0) { /* Conspiracy 195 */
		strBuffer.Format("%d", fileMP3->nPlayTime);
	} else {
		strBuffer = "     ";
	}
	strText = StrReplaceEx(strText, "%TOTAL_SEC2%", strBuffer, bIsHtml);
	// フォーマット
	{ /* STEP 012 */
		CString strFormat = fileMP3->strVisualFormat;
		if (strFormat.GetLength() > 0 && fileMP3->strAudioFormat.GetLength() > 0) {
			strFormat += "\n";
		}
		strFormat += fileMP3->strAudioFormat;
		strText = StrReplaceEx(strText, "%FORMAT%", strFormat, bIsHtml);
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
	// ファイル総数
	CString	strFileCount;
	strFileCount.Format("%d", pStatus->nFileCount);

	// 合計演奏時間
	int		nMinute = pStatus->nTotalTime / 60;
	CString	strTotalTime, strTotalTime2;
	strTotalTime.Format("%02d:%02d", nMinute, pStatus->nTotalTime % 60);
	if ((nMinute/60) > 0) {
		strTotalTime2.Format("%02d:%02d:%02d", nMinute / 60, nMinute % 60, pStatus->nTotalTime % 60);
	} else {
		strTotalTime2 = strTotalTime;
	}

	// 合計ファイルサイズ
	CString	strTotalSize;
	strTotalSize.Format("%.2fMB", (float)pStatus->fTotalSize / 1024 / 1024);

	// 書式を使って文字列を作成
	CString	strText = strFoot;
	strText = StrReplaceEx(strText, "%FILE_COUNT%", strFileCount, bIsHtml);
	strText = StrReplaceEx(strText, "%TOTAL_TIME%", strTotalTime, bIsHtml);
	strText = StrReplaceEx(strText, "%TOTAL_TIME2%", strTotalTime2, bIsHtml);
	strText = StrReplaceEx(strText, "%TOTAL_SIZE%", strTotalSize, bIsHtml);

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
			// ファイルの場合
			CItemInfo	*pInfo = GetData(pItem);
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			strText = MakeFormatFileBody(fileMP3, strText, bIsHtml, pStatus, bWriteHtml);
		} else {
			// フォルダの場合
			// 子アイテムを全て処理する
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
	// 選択されているファイルを全て処理する
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);

	// プログレスバー初期化
	if (szProgressBarTitle != NULL) {
		m_pDoc->StartLoadFile(szProgressBarTitle);
		m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, "しばらくお待ち下さい...");
	}

	extern	int		g_nWriteTagProcFlag;
	g_nWriteTagProcFlag = 0;
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];

		// プログレスバー更新
		if (szProgressBarTitle != NULL) {
			if (nCount > 1) {
				m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
			}
		}

		// 処理実行
		CTreeItem	*pItem = GetTreeItem(nIndex);
		CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
		if (pItem != NULL && pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			FILE_INFO info;
			info.pFileMP3 = fileMP3;
			bool result = (*callback)(&info, g_nWriteTagProcFlag, GetSafeHwnd());
			if (result == false) {
				// 中止
				break;
			}
		}
	}

	// プログレスバー終了
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
	// 選択されているファイルを全て処理する
	CArray <int, const int &> arrayList;
	int		nCount;
	nCount = MakeSelectFileArray(arrayList);

	// プログレスバー初期化
	if (szProgressBarTitle != NULL) {
		m_pDoc->StartLoadFile(szProgressBarTitle);
		m_pDoc->GetDlgLoadProgressBarPtr()->SetDlgItemText(IDC_ST_MESSAGE, "しばらくお待ち下さい...");
	}

	extern	int		g_nWriteTagProcFlag;
	g_nWriteTagProcFlag = 0;
	int i; for (i = 0; i < nCount; i++) {
		int		nIndex = arrayList[i];

		// プログレスバー更新
		if (szProgressBarTitle != NULL) {
			if (nCount > 1) {
				m_pDoc->GetDlgLoadProgressBarPtr()->SetPos(i * 100 / (nCount-1));
			}
		}

		// 処理実行
		CTreeItem	*pItem = GetTreeItem(nIndex);
		CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
		if (pItem != NULL && pInfo != NULL) {
			FILE_MP3	*fileMP3 = m_pDoc->GetListMP3((int)pInfo->GetLParam());

			// 更新日時の保存
			// 読み取り属性のチェック
			bool	bKeepTimeStamp = g_bOptKeepTimeStamp;
			CFileStatus	fileStatus;
			if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
				return false;
			}
			if (fileStatus.m_attribute & CFile::readOnly) {
				if (g_nWriteTagProcFlag != 1) {
					// 上書き確認ダイアログを開く
					CDlgFileOverWrite	dialog;
					dialog.m_strFileName = fileMP3->strFullPathName;
					dialog.m_strSize.Format("%ld byte", fileStatus.m_size);
					if (fileStatus.m_mtime.GetTime() < 0) {
						dialog.m_strTimeStamp.Format("----/--/-- --:--:--");
					} else {
						dialog.m_strTimeStamp.Format("%04d/%02d/%02d %02d:%02d:%02d",
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
					case 0:				// このファイルを上書き
					case 1:				// 以降全て上書き
						break;
					case 2:				// スキップ
					case 3:				// 中止
					default:
						return true;
					}
				}
				TRY {
					// 読み取り専用属性を解除
					SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
					// 変更後に属性を戻させるために bKeepTimeStamp を true にする
					bKeepTimeStamp = true;
				}
				CATCH(CFileException, e) {
					CString	strMsg;
					strMsg.Format("%s がオープンできませんでした", fileMP3->strFullPathName);
					MessageBox(strMsg, "読み取り専用属性の解除エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
					return false;
				}
				END_CATCH
			}

			FILE_INFO info;
			info.pFileMP3 = fileMP3;
			result = (*callback)(&info, g_nWriteTagProcFlag, GetSafeHwnd());
			if (bKeepTimeStamp) {
				if (fileMP3->time.GetTime() != -1) {
					// ファイル更新時間を設定
					fileStatus.m_mtime = fileMP3->time;
					if (g_bOptSyncCreateTime) fileStatus.m_ctime = fileMP3->time;
				}
				TRY {
					CFile::SetStatus(fileMP3->strFullPathName, fileStatus);
				}
				CATCH(CFileException, e) {
					CString	strMsg;
					strMsg.Format("%s がオープンできませんでした", fileMP3->strFullPathName);
					MessageBox(strMsg, "タイムスタンプの更新エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
					result = false;
				}
				END_CATCH
			} else {
				// 更新後のタイムスタンプを取得
				if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) != FALSE) {
					fileMP3->time = fileStatus.m_mtime;
				}
			}
			if (result == false) {
				// 中止
				break;
			}
		}
	}

	// プログレスバー終了
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
		// 変換書式が空
		return false;					// エラー
	}

	int		sx, sy, ex, ey;
	// 選択範囲を取得
	if (GetSelectedRange(sx, sy, ex, ey) == false) {
		return false;					// 範囲選択無し
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
	// 選択されているファイルを全て処理する
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

	// SI フィールドを ID3 tag 情報にコピー
	void OnConvSIFieldToID3tag(FILE_MP3* pFileMP3);
	OnConvSIFieldToID3tag(fileMP3);
	UpdateFileStatus(pItem, fileMP3);
	return(true);
}
