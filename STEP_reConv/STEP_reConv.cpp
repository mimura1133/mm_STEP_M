// STEP_reConv.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_reConv.h"
#include "STEPlugin.h"

#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	メモ!
//
//		この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされた
//		どの関数も関数の最初に追加される AFX_MANAGE_STATE 
//		マクロを含んでいなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		これは関数内の最初のステートメントでなければな
//		らないことを意味します、コンストラクタが MFC 
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_reConvApp

BEGIN_MESSAGE_MAP(CSTEP_reConvApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_reConvApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_reConvApp の構築

CSTEP_reConvApp::CSTEP_reConvApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_reConvApp オブジェクト

CSTEP_reConvApp theApp;

UINT nPluginID;

CString strINI;

// コマンドID
UINT nIDReConvHiragana;
UINT nIDReConvRomaji;

void AddConvMenu(HMENU hMenu) {
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDReConvHiragana, "ひらがなに変換");
	InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDReConvRomaji, "ローマ字に変換");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INIファイルの読み込み
	strINI = szPluginFolder;
	strINI += "STEP_reConv.ini";

	nIDReConvHiragana = STEPGetCommandID();
	STEPKeyAssign(nIDReConvHiragana, "ひらがなに変換", "STEP_reConv_KEY_RE_CONV_HIRAGANA");
	nIDReConvRomaji = STEPGetCommandID();
	STEPKeyAssign(nIDReConvRomaji, "ローマ字に変換", "STEP_reConv_KEY_RE_CONV_ROMAJI");

	return true;
}

STEP_API void WINAPI STEPFinalize() {
	Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "STEP_reConv";
}

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "Version 0.03 Copyright (C) 2003-2006 haseta\r\nMS-IME2000/2002によりひらがな/ローマ字に変換します";
}

STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDReConvHiragana) {
		return "選択されている範囲をひらがなに変換します";
	}
	if (nID == nIDReConvRomaji) {
		return "選択されている範囲をローマ字に変換します";
	}
	return NULL;
}

STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDReConvHiragana) {
		if (STEPIsRangeSelected() || STEPIsCurrentCellEditOK()) return true;
		else return false;
	}
	if (nID == nIDReConvRomaji) {
		if (STEPIsRangeSelected() || STEPIsCurrentCellEditOK()) return true;
		else return false;
	}
	return false;
}

// strcnv.cpp
enum	{CONV_SUJI=1, CONV_ALPHA=2, CONV_KATA=4, CONV_KIGOU=8, CONV_ALL=15};
#ifdef __cplusplus
extern "C" {
#endif

extern	int conv_han2zens(unsigned char *, const unsigned char *, int);
extern	int conv_zen2hans(unsigned char *, const unsigned char *, int);
extern	void conv_kata2hira(unsigned char *);
extern	void conv_kata_erase_dakuon(unsigned char *);
extern	void conv_hira2kata(unsigned char *);
extern	void conv_upper(unsigned char *);
extern	void conv_lower(unsigned char *);
extern	void conv_first_upper(unsigned char *);
//extern	DWORD conv_kan2hira(HWND, unsigned char *, DWORD);
//extern	void conv_romaji(HWND hwnd, unsigned char *str, unsigned char *sRomaji);
#ifdef __cplusplus
}
#endif
CString conv_kan2hira(HWND hWnd, unsigned char * str)
{
	CString strText = str;
	// MS-IME2000ではうまく動いたが、ATOK16ではだめ。単語単位であればできる...
	HIMC himc = ::ImmGetContext(hWnd);
	DWORD dwRet = ::ImmGetConversionList(
					 ::GetKeyboardLayout(0), himc,
					 (const char *)str, NULL, 0,
					 GCL_REVERSECONVERSION);

	// 読み仮名格納領域を確保 
	CANDIDATELIST *lpCand;
	lpCand = (CANDIDATELIST *)malloc(dwRet);
	// 読み仮名を取得
	dwRet = ::ImmGetConversionList(
					 ::GetKeyboardLayout(0), himc,
					 (const char*)str, lpCand, dwRet,
					 GCL_REVERSECONVERSION);
	if (dwRet > 0 && lpCand->dwCount > 0) {
		char *work = (char*)lpCand + lpCand->dwOffset[0];
		strText = work;

		/*
		for (unsigned int i = 0; i< lpCand->dwCount; i++)
		{
			TRACE("%s", (LPBYTE)lpCand + lpCand->dwOffset[i]);
		}
		*/
	}

	free(lpCand);
	::ImmReleaseContext(hWnd, himc);
	return strText;
}

CString conv_romaji(HWND hwnd, unsigned char *str)
{
	static const char *romaji[] = {
		"っきゃ", "KKYA", "っきゅ", "KKYU", "っきょ", "KYO",
		"きゃ", "KYA", "きゅ", "KYU", "きょ", "KYO",
		"っしゃ", "SSHA", "っしゅ", "SSHU", "っしょ", "SSHO",
		"しゃ", "SHA", "しゅ", "SHU", "しょ", "SHO",
		"っちゃ", "CCHA", "っちゅ", "CCHU", "っちょ", "CCHO",
		"ちゃ", "CHA", "ちゅ", "CHU", "ちょ", "CHO",
		"っにゃ", "NNYA", "っにゅ", "NNYU", "っにょ", "NNYO",
		"にゃ", "NYA", "にゅ", "NYU", "にょ", "NYO",
		"っひゃ", "HHYA", "っひゅ", "HHYU", "っひょ", "HHYO",
		"ひゃ", "HYA", "ひゅ", "HYU", "ひょ", "HYO",
		"っみゃ", "MMYA", "っみゅ", "MMYU", "っみょ", "MMYO",
		"みゃ", "MYA", "みゅ", "MYU", "みょ", "MYO",
		"っりゃ", "RRYA", "っりゅ", "RRYU", "っりょ", "RRYO",
		"りゃ", "RYA", "りゅ", "RYU", "りょ", "RYO",
		"っぎゃ", "GGYA", "っぎゅ", "GGYU", "っぎょ", "GGYO",
		"ぎゃ", "GYA", "ぎゅ", "GYU", "ぎょ", "GYO",
		"っじゃ", "JJA", "っじゅ", "JJU", "っじょ", "JJO",
		"じゃ", "JA", "じゅ", "JU", "じょ", "JO",
		"っびゃ", "BBYA", "っびゅ", "BBYU", "っびょ", "BBYO",
		"びゃ", "BYA", "びゅ", "BYU", "びょ", "BYO",
		"っぴゃ", "PPYA", "っぴゅ", "PPYU", "っぴょ", "PPYO",
		"ぴゃ", "PYA", "ぴゅ", "PYU", "ぴょ", "PYO",

		"っか", "KKA", "っき", "KKI", "っく", "KKU", "っけ", "KKE", "っこ", "KKO",
		"か", "KA", "き", "KI", "く", "KU", "け", "KE", "こ", "KO",
		"っさ", "SSA", "っし", "SSHI", "っす", "SSU", "っせ", "SSE", "っそ", "SSO",
		"さ", "SA", "し", "SHI", "す", "SU", "せ", "SE", "そ", "SO",
		"った", "TTA", "っち", "CCHI", "っつ", "TTSU", "って", "TTE", "っと", "TTO",
		"た", "TA", "ち", "CHI", "つ", "TSU", "て", "TE", "と", "TO",
		"っな", "NNA", "っに", "NNI", "っぬ", "NNU", "っね", "NNE", "っの", "NNO",
		"な", "NA", "に", "NI", "ぬ", "NU", "ね", "NE", "の", "NO",
		"っは", "HHA", "っひ", "HHI", "っふ", "FFU", "っへ", "HHE", "っほ", "HHO",
		"は", "HA", "ひ", "HI", "ふ", "FU", "へ", "HE", "ほ", "HO",
		"っま", "MMA", "っみ", "MMI", "っむ", "MMU", "っめ", "MME", "っも", "MMO",
		"ま", "MA", "み", "MI", "む", "MU", "め", "ME", "も", "MO",
		"っや", "YYA", "っゆ", "YUYU", "っよ", "YYO",
		"や", "YA", "ゆ", "YU", "よ", "YO",
		"っら", "RRA", "っり", "RRI", "っる", "RRU", "っれ", "RRE", "っろ", "RRO",
		"ら", "RA", "り", "RI", "る", "RU", "れ", "RE", "ろ", "RO",
		"っわ", "WWA",
		"わ", "WA",
		"っが", "GGA", "っぎ", "GGI", "っぐ", "GGU", "っげ", "GGE", "っご", "GGO",
		"が", "GA", "ぎ", "GI", "ぐ", "GU", "げ", "GE", "ご", "GO",
		"っざ", "ZZA", "っじ", "JJI", "っず", "ZZU", "っぜ", "ZZE", "っぞ", "ZZO",
		"ざ", "ZA", "じ", "JI", "ず", "ZU", "ぜ", "ZE", "ぞ", "ZO",
		"っだ", "DDA", "っぢ", "JJI", "っづ", "ZZU", "っで", "DDE", "っど", "DDO",
		"だ", "DA", "ぢ", "JI", "づ", "ZU", "で", "DE", "ど", "DO",
		"っば", "BBA", "っび", "BBI", "っぶ", "BBU", "っべ", "BBE", "っぼ", "BBO",
		"ば", "BA", "び", "BI", "ぶ", "BU", "べ", "BE", "ぼ", "BO",
		"っぱ", "PPA", "っぴ", "PPI", "っぷ", "PPU", "っぺ", "PPE", "っぽ", "PPO",
		"ぱ", "PA", "ぴ", "PI", "ぷ", "PU", "ぺ", "PE", "ぽ", "PO",

		"っあ", "AA", "っい", "II", "っう", "UU", "っえ", "EE", "っお", "OO",
		"あ", "A", "い", "I", "う", "U", "え", "E", "お", "O",
		"っを", "OO", "っん", "NN",
		"を", "O", "ん", "N",
		"ー", "",
		NULL, NULL,
	};

	CString	strWork, strRep;
	int		nPos;
	int nRomaji = 0;

	strWork = str;
	/*
	while (romaji[nRomaji*2] != NULL) {
		while((nPos = strWork.Find(romaji[nRomaji*2])) != -1) {
			int		nLenOrg = strWork.GetLength();
			int		nLenKey = strlen(romaji[nRomaji*2]);
			strRep = romaji[nRomaji*2+1];
			conv_lower((unsigned char *)strRep.GetBuffer(0));
			strRep.ReleaseBuffer();
			strWork.Format("%s%s%s", strWork.Left(nPos), strRep, strWork.Right(nLenOrg-(nPos+nLenKey)));
		}
		nRomaji++;
	}
	*/

	strWork = conv_kan2hira(hwnd, (unsigned char*)strWork.GetBuffer(0));

	nRomaji = 0;
	while (romaji[nRomaji*2] != NULL) {
		while((nPos = strWork.Find(romaji[nRomaji*2])) != -1) {
			int		nLenOrg = strWork.GetLength();
			int		nLenKey = strlen(romaji[nRomaji*2]);
			strRep = romaji[nRomaji*2+1];
			conv_lower((unsigned char *)strRep.GetBuffer(0));
			strRep.ReleaseBuffer();
			conv_first_upper((unsigned char *)strRep.GetBuffer(0));
			strRep.ReleaseBuffer();
			strWork.Format("%s%s%s", strWork.Left(nPos), strRep, strWork.Right(nLenOrg-(nPos+nLenKey)));
		}
		nRomaji++;
	}

	conv_zen2hans((unsigned char *)strRep.GetBuffer(strWork.GetLength()*2+1), (const unsigned char *)(const char *)strWork, CONV_ALL);
	strWork.ReleaseBuffer();
	strRep.ReleaseBuffer();
	return strRep;
}

STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (nID == nIDReConvHiragana) {
		static	const char *sMessage = "MS-IME2002/2000以外では正常に動作しない可能性があります。\n\n"
									   "変換を実行してもよろしいですか？";
		static bool bKan2HiraConfirmIME = false;
		if (bKan2HiraConfirmIME == true || MessageBox(hWnd, sMessage, "ひらがなに変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			bKan2HiraConfirmIME = true;
			int sx, sy, ex, ey;
			if (STEPGetSelectedRange(&sx, &sy, &ex, &ey)) {
				for (int nItem = sy; nItem <= ey; nItem++) {
					if (STEPIsItemFile(nItem) == true) {
						//FILE_INFO info;
						//STEPGetFileInfo(nItem, &info);
						for (int nColumn = sx; nColumn <= ex; nColumn++) {
							CString	strText;
							// セルのテキストを取得
							strText = conv_kan2hira(hWnd, (unsigned char*)STEPGetSubItemText(nItem, nColumn));
							STEPChangeSubItemText(nItem, nColumn, (LPCTSTR)conv_kan2hira(hWnd, (unsigned char*)strText.GetBuffer(0)));
						}
					}
				}
			}
		}
		return true;
	}
	if (nID == nIDReConvRomaji) {
		static	const char *sMessage = "MS-IME2002/2000以外では正常に動作しない可能性があります。\n\n"
									   "変換を実行してもよろしいですか？";
		static bool bRomajiConfirmIME = false;
		if (bRomajiConfirmIME == true || MessageBox(hWnd, sMessage, "ローマ字に変換", MB_YESNO|MB_TOPMOST) == IDYES) {
			int sx, sy, ex, ey;
			if (STEPGetSelectedRange(&sx, &sy, &ex, &ey)) {
				for (int nItem = sy; nItem <= ey; nItem++) {
					if (STEPIsItemFile(nItem) == true) {
						//FILE_INFO info;
						//STEPGetFileInfo(nItem, &info);
						for (int nColumn = sx; nColumn <= ex; nColumn++) {
							CString	strText;
							// セルのテキストを取得
							strText = conv_romaji(hWnd, (unsigned char*)STEPGetSubItemText(nItem, nColumn));
							STEPChangeSubItemText(nItem, nColumn, (LPCTSTR)conv_kan2hira(hWnd, (unsigned char*)strText.GetBuffer(0)));
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nType) {
	case MENU_FILE_EDIT_OK:
		AddConvMenu(hMenu);
		break;
	}
}

STEP_API void WINAPI STEPOnLoadMainMenu()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// メニューへの追加
	HMENU hMenu = STEPGetMenu(MENU_CONV);
	AddConvMenu(hMenu);
}

bool g_bZenHanKigouKana = true; /* STEP 016 */
