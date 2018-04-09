// ======================================================
// 概  要    ： フォルダ選択ダイアログのラッパクラス
// ターゲット： Windows95/98/NT
// 処理系    ： Visual C++ Ver 6.0
// 作成者    ： MERCURY
// 作成日    ： 00/06/17(土)
// 著作権表記： Copyright(C) 2000 MERCURY.
// ======================================================



// ======================================
// =====   条件コンパイルフラグ     =====
// ======================================



// ======================================
// =====   インクルードファイル     =====
// ======================================
#include "stdafx.h"
#include "SHBrowseForFolder.h"



// ======================================
// =====           定  数           =====
// ======================================
#define IDC_CH_SUB_DIR		1000		// フォルダ参照の[サブディレクトリを検索]ボタンのID



// ======================================
// =====           マクロ           =====
// ======================================



// ======================================
// =====       typedef／enum        =====
// ======================================



// ======================================
// =====       構造体／共用体       =====
// ======================================



// ======================================
// =====         const 変数         =====
// ======================================



// ======================================
// =====        extern 変数         =====
// ======================================
bool	CSHBrowseForFolder::m_bEnableSubDirButton = false;
bool	CSHBrowseForFolder::m_bSearchSubDirState = false;
LPCTSTR CSHBrowseForFolder::pCheckBoxTitle = NULL;/* FunnyCorn 185 *///"サブディレクトリを検索";
WNDPROC	CSHBrowseForFolder::m_VSSelectOrgProc;



// ======================================
// =====        static 変数         =====
// ======================================
static	CButton	*pSubDir = NULL; /* WildCherry2 080 */



// ======================================
// =====     関数プロトタイプ       =====
// ======================================



// ======================================
// =====      プログラム領域        =====
// ======================================
// =============================================
// CSHBrowseForFolder::CSHBrowseForFolder
// Func  : コンストラクタ
// Input : bEnable		= [サブディレクトリを検索]ボタンを追加するか?
//       : bState		= [サブディレクトリを検索]ボタンの初期状態
// Output: none
// =============================================
CSHBrowseForFolder::CSHBrowseForFolder(bool bEnable, bool bState)
{
	SetEnableSubDirButton(bEnable);
	SetSearchSubDirState(bState);
	SetCheckBoxTitle(TEXT("サブディレクトリを検索")); /* FunnyCorn 185 */
}

// =============================================
// CSHBrowseForFolder::~CSHBrowseForFolder
// Func  : デストラクタ
// Input : none
// Output: none
// =============================================
CSHBrowseForFolder::~CSHBrowseForFolder()
{
}

// =============================================
// VSSelectFolderSubProc
// 概要  : フォルダ参照の[サブディレクトリを検索]ボタン処理
// 引数  : ...
// 戻り値: int CALLBACK
// =============================================
LRESULT CALLBACK CSHBrowseForFolder::VSSelectFolderSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((uMsg == WM_COMMAND) && (LOWORD(wParam) == IDC_CH_SUB_DIR)) {
		// ボタンの処理
		CWnd	*pClient = CWnd::FromHandle(hWnd);
		CButton	*pSubDir = (CButton *)pClient->GetDlgItem(IDC_CH_SUB_DIR);

		// チェック状態を反転
		SetSearchSubDirState(GetSearchSubDirState() ? false : true);
		pSubDir->SetCheck(GetSearchSubDirState() ? TRUE : FALSE);
	}
	return(CallWindowProc(m_VSSelectOrgProc, hWnd, uMsg, wParam, lParam));
}

// =============================================
// CallbackSelectDir
// 概要  : フォルダ参照で使用するコールバック関数
//       : 開いたときにフォルダを選択した状態で開かせる処理をする場合に必要
// 引数  : ...
// 戻り値: int CALLBACK
// =============================================
int CALLBACK CSHBrowseForFolder::CallbackSelectDir(HWND hWnd, UINT uMsg, LPARAM, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		// デフォルトのフォルダを選択させる
		SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		{
			// フォルダーツリーで常に選択状態を表示
			HWND hwndTree = FindWindowEx(hWnd, NULL, TEXT("SysTreeView32"), NULL);
			if (hwndTree != NULL) {
				LONG style;
				style = GetWindowLong(hwndTree, GWL_STYLE);
				style |= TVS_SHOWSELALWAYS;
				SetWindowLong(hwndTree, GWL_STYLE, style);
			}
		}

		// [サブディレクトリを検索] ボタンを追加
		if (GetEnableSubDirButton()) {
			pSubDir = new CButton; /* WildCherry2 080 */
			if (pSubDir) {
				RECT	rectClient, rectOK, rectCancel, rect;
				CWnd	*pClient	= CWnd::FromHandle(hWnd);
				CWnd	*pOK		= pClient->GetDlgItem(IDOK);
				CWnd	*pCANCEL	= pClient->GetDlgItem(IDCANCEL);
				// クライアントウィンドウサイズを取得
				pClient->GetClientRect(&rectClient);
				// [OK] / [CANCEL] のウィンドウ位置を取得
				pOK->GetWindowRect(&rectOK);
				pCANCEL->GetWindowRect(&rectCancel);
				pClient->ScreenToClient(&rectOK) ;
				pClient->ScreenToClient(&rectCancel) ;
				// サブディレクトリ対象チェックボタンの RECT を求める
				rect = CRect(rectClient.right - rectCancel.right,
				             rectCancel.top,
				             rectOK.left,
				             rectCancel.bottom);
				// チェックボタン作成
				if (pSubDir->Create(GetCheckBoxTitle(),
				                    WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP,
				                    rect, pClient, IDC_CH_SUB_DIR)) {
					// フォントを設定
					HFONT hFontCurr = (HFONT)pClient->SendMessage(WM_GETFONT, 0, 0);
					if (hFontCurr != NULL) pSubDir->PostMessage(WM_SETFONT, (WPARAM)hFontCurr, 0); 

					// チェック状態を設定
					pSubDir->SetCheck(GetSearchSubDirState() ? TRUE : FALSE);

					// サブクラス化
					m_VSSelectOrgProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)VSSelectFolderSubProc);
				}
			}
		}
	}
	return 0;
}

// =============================================
// CSHBrowseForFolder::SelectDirectory
// 概要  : フォルダ参照ダイアログ処理
// 引数  : sLocal			= パス(入出力)
// 戻り値: bool
// =============================================
bool CSHBrowseForFolder::Exec(LPTSTR sLocal)
{
	BOOL			bResult = FALSE;
	BROWSEINFO		bi;
	LPTSTR			lpBuffer;
	LPITEMIDLIST	pidlRoot;	   // ブラウズのルートPIDL
	LPITEMIDLIST	pidlBrowse;    // ユーザーが選択したPIDL
	LPMALLOC		lpMalloc = NULL;

	HRESULT hr = SHGetMalloc(&lpMalloc);
	if (FAILED(hr)) return(FALSE);

	HWND	hwnd = AfxGetMainWnd()->GetSafeHwnd();

	// ブラウズ情報受け取りバッファ領域の確保
	if ((lpBuffer = (LPTSTR) lpMalloc->Alloc(_MAX_PATH)) == NULL) {
		lpMalloc->Release(); /* WildCherry2 080 */
		return(FALSE);
	}
	// ダイアログ表示時のルートフォルダのPIDLを取得
	// ※以下はデスクトップをルートとしている。デスクトップをルートとする
	//	 場合は、単に bi.pidlRoot に０を設定するだけでもよい。その他の特
	//	 殊フォルダをルートとする事もできる。詳細はSHGetSpecialFolderLoca
	//	 tionのヘルプを参照の事。
	if (!SUCCEEDED(SHGetSpecialFolderLocation(	hwnd,
												CSIDL_DESKTOP,
												&pidlRoot))) {
		lpMalloc->Free(lpBuffer);
		lpMalloc->Release(); /* WildCherry2 080 */
		return(FALSE);
	}
	// BROWSEINFO構造体の初期値設定
	// ※BROWSEINFO構造体の各メンバの詳細説明もヘルプを参照
	bi.hwndOwner = hwnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = lpBuffer;
	bi.lpszTitle = TEXT("フォルダを選択して下さい。");
	bi.ulFlags = 0;
	bi.lpfn = CallbackSelectDir;		// コールバック関数のアドレスを設定
	bi.lParam = (LPARAM)sLocal;			// 指定したいパスを設定
	// フォルダ選択ダイアログの表示
	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL) {
		// PIDL形式の戻り値のファイルシステムのパスに変換
		if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
			// 取得成功
			lstrcpy(sLocal, lpBuffer);
			bResult = TRUE;
		}
		// SHBrowseForFolderの戻り値PIDLを解放
		lpMalloc->Free(pidlBrowse);
	}
	// クリーンアップ処理
	lpMalloc->Free(pidlRoot);
	lpMalloc->Free(lpBuffer);
	lpMalloc->Release();
	if (pSubDir) { /* WildCherry2 080 */
		delete pSubDir;
		pSubDir = NULL;
	}
	return(bResult ? true : false);
}
