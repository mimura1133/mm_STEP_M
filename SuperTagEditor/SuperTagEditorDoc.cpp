// SuperTagEditorDoc.cpp : CSuperTagEditorDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Shlwapi.h"
#include "FileMP3.h"
#include "SuperTagEditor.h"
#include "DlgCommonProg.h"
#include "DlgEnvSheet.h"
#include "SHBrowseForFolder.h"
#include "SuperTagEditorView.h"
#include "SuperTagEditorDoc.h"
#include "DlgPluginSetup.h"

#include "INI/Profile.h"

#include <vector>

//#include "NewAPIs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if false
//////////////////////////////////////////////////////////////////////////////
//GetLongPathName
//短いファイル名を長いファイル名に変換
//////////////////////////////////////////////////////////////////////////////
/*
引数：
    LPSTR lpszLongFileName  長いファイル名を受け取るバッファへのポインタ
    LPSTR lpszShortPathName 短いファイル名を含むパス名へのポインタ
    DWORD dwSize            バッファサイズ
戻り値:
    DWORD バッファにコピーした文字列の長さ
        ０のとき異常終了
*/
DWORD GetLongPathName(LPSTR lpszLongFileName, LPSTR lpszShortPathName, DWORD dwSize)
{
	WIN32_FIND_DATA fd;
	HANDLE	hFind;

	//短いファイル名でファイルを検索
	if((hFind = FindFirstFile(lpszShortPathName,&fd)) != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		if((DWORD)lstrlen(fd.cFileName) <= dwSize) {
			//長いファイル名をバッファにコピー
			lstrcpy(lpszLongFileName,fd.cFileName);
			//バッファにコピーした文字列を返す
			return lstrlen(lpszLongFileName);
		}
	}
	lstrcpy(lpszLongFileName, "");
	//バッファのサイズを超えたかファイルが見つからなかったときは異常終了
	return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc

IMPLEMENT_DYNCREATE(CSuperTagEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSuperTagEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CSuperTagEditorDoc)
	ON_UPDATE_COMMAND_UI(ID_OPEN_FOLDER, OnUpdateOpenFolder)
	ON_COMMAND(ID_OPEN_FOLDER, OnOpenFolder)
	ON_UPDATE_COMMAND_UI(ID_SAVE_ALL_TAG, OnUpdateSaveAllTag)
	ON_COMMAND(ID_SAVE_ALL_TAG, OnSaveAllTag)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIELD_SI, OnUpdateEditFieldSi)
	ON_COMMAND(ID_EDIT_FIELD_SI, OnEditFieldSi)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TD3_TAG, OnUpdateEditTd3Tag)
	ON_COMMAND(ID_EDIT_TD3_TAG, OnEditTd3Tag)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CHANGE_FIELD, OnUpdateEditChangeField)
	ON_COMMAND(ID_EDIT_CHANGE_FIELD, OnEditChangeField)
	ON_UPDATE_COMMAND_UI(ID_EXEC_CLASSIFICATION, OnUpdateExecClassification)
	ON_COMMAND(ID_EXEC_CLASSIFICATION, OnExecClassification)
	ON_COMMAND(ID_DLG_SETUP_PLUGIN, OnDlgSetupPlugin)
	ON_UPDATE_COMMAND_UI(ID_DLG_SETUP_PLUGIN, OnUpdateDlgSetupPlugin)
	ON_COMMAND(ID_DLG_ENVIRONMENT, OnDlgEnvironment)
	ON_UPDATE_COMMAND_UI(ID_DLG_ENVIRONMENT, OnUpdateDlgEnvironment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc クラスの構築/消滅

CSuperTagEditorDoc::CSuperTagEditorDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
	m_dlgLoadProgress = new CDlgCommonProgress;
	m_nArrayFileCount = 0;
	m_nArrayRequestFileCount = 0;

	m_StartLoadFileCount = 0;//追加 by Kobarin
	m_TagUpdatingCount = 0;//追加 by Kobarin

	m_bInitialized = false; /* StartInaction2 055 */
}

CSuperTagEditorDoc::~CSuperTagEditorDoc()
{
	delete	m_dlgLoadProgress;
	m_dlgLoadProgress = NULL;
}

BOOL CSuperTagEditorDoc::OnNewDocument()
{
	// 保存確認
	if (m_nArrayFileCount && CanCloseFrame((CFrameWnd *)AfxGetMainWnd()) == FALSE) {
		return FALSE;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	InitFileList();			// ファイルリストの初期化
	ClearRequestFiles();	// 追加リクエストファイルリストの初期化


	if (!m_bInitialized) {
		InitPlugin();			// プラグインのロード

		// 初期化処理
		GetView()->m_List.InitializeGrid();
		// フォントの設定
		GetView()->UpdateFontListView();
	}
	InitGridList();			// グリッドを初期化

	m_bInitialized = true;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc シリアライゼーション

void CSuperTagEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc クラスの診断

#ifdef _DEBUG
void CSuperTagEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSuperTagEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc コマンド
//****************************************************************************
//                              ファイル検索処理関連
//****************************************************************************
// =============================================
// CSuperTagEditorDoc::GetFileTime
// 概要  : ファイルのタイムスタンプを取得
// 引数  : sFileName	= ファイル名
// 戻り値: CTime		= タイムスタンプ(CTime(0):エラー)
// =============================================
CTime CSuperTagEditorDoc::GetFileTime(LPCTSTR sFileName)
{
	CFileStatus	status;
	if (sFileName == NULL
	||  CFile::GetStatus(sFileName, status) == FALSE) {
		return(CTime(0));
	}
	if (status.m_mtime == -1) return(CTime(0));
	if (status.m_mtime.GetTime() == -1) return(CTime(0));
	return(status.m_mtime);
}

// =============================================
// CSuperTagEditorDoc::AddRequestFile
// 概要  : ファイル追加リクエストリストにファイルを追加する
// 引数  : sFillPath		= フルパス名
//       : pParent			= 親アイテム(NULLも可)
// 戻り値: int				= インデックス(追加しなかった場合は-1を返す)
// =============================================
int CSuperTagEditorDoc::AddRequestFile(LPCTSTR sFillPath, CSuperGridCtrl::CTreeItem *pParent)
{
	//追加 by Kobarin
	if(IsTagUpdating())//タグを更新中は追加しない
		return -1;
	if(m_dlgLoadProgress->IsCanceled()){
		//キャンセルボタンが押されている
		return -1;
	}

	FILE_STATE	stat;
	stat.strFullPathName = sFillPath;
	stat.pParent = pParent;
	// 配列に追加
	if (m_nArrayRequestFileCount > m_arrayRequestFiles.GetUpperBound()) {
		// 配列が足りない場合は、確保サイズを増加させる
		m_arrayRequestFiles.SetSize(m_arrayRequestFiles.GetUpperBound() + ADD_ARRAY_SIZE);
	}

	m_arrayRequestFiles[m_nArrayRequestFileCount] = stat;
	m_nArrayRequestFileCount++;
	return(m_nArrayRequestFileCount);
}

void CSuperTagEditorDoc::StartLoadFile(LPCTSTR sTitle)
{
	if (m_dlgLoadProgress->GetSafeHwnd() == NULL) {
		// プログレスバーを生成
//		m_dlgLoadProgress = new CDlgCommonProgress;
		m_dlgLoadProgress->Create(AfxGetMainWnd());
		m_dlgLoadProgress->SetWindowText(sTitle);
		UpdateAllViews(NULL);
	}
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("ファイル検索中..."));
    m_StartLoadFileCount++;//追加 by Kobarin
}

void CSuperTagEditorDoc::EndLoadFile(void)
{
	if(--m_StartLoadFileCount != 0)return;//追加 by Kobarin
	if (m_dlgLoadProgress->GetSafeHwnd() != NULL) {
		m_dlgLoadProgress->DestroyWindow();
		m_dlgLoadProgress->SetCanceled(FALSE);//追加 by Kobarin
//		delete	m_dlgLoadProgress;
//		m_dlgLoadProgress = NULL;
	}
}

// =============================================
// CSuperTagEditorDoc::ExecRequestFiles
// 概要  : 追加リクエストリストのファイルを処理する
// 引数  : bListClear		= 追加処理終了後に追加リクエストのリストをクリアするか？
//       : bCheck			= チェックマークを付けるかどうか
// 戻り値: none
// =============================================
void CSuperTagEditorDoc::ExecRequestFiles(bool bListClear, bool bCheck)
{
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("タグ情報の読み込み中..."));

//	CMySuperGrid	&listCtrl = GetListCtrl();
//	listCtrl.SetRedraw(FALSE);

	//int		nFileCount = m_nArrayFileCount; /* STEP 015 */
	for (int no = 0; no < m_nArrayRequestFileCount; no++) {
		int		nFileCount = m_nArrayFileCount; /* STEP 015 */
		// プログレスバー更新
		if (m_nArrayRequestFileCount > 1) {
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayRequestFileCount-1));
		}
		// 追加 by Kobarin
		if(m_dlgLoadProgress->IsCanceled()){
			//キャンセルボタンが押されていた
			break;
		}

		// 多重登録チェック
		if (nFileCount > 0) {
			bool	bFound = false;
			CString	*strFileName = &m_arrayRequestFiles[no].strFullPathName;
			int i; for (i = 0; i < nFileCount; i++) {
				if (GetListMP3(i)->strFullPathName == *strFileName) {
					bFound = true;
					break;
				}
			}
			if (bFound == false) {
				// ＭＰ３ファイル情報の取得
				FoundFile(*strFileName, m_arrayRequestFiles[no].pParent, bCheck);
			}
		} else {
			// ＭＰ３ファイル情報の取得
			FoundFile(m_arrayRequestFiles[no].strFullPathName, m_arrayRequestFiles[no].pParent, bCheck);
		}

	}

//	listCtrl.SetRedraw(TRUE);

	if (bListClear) {
		ClearRequestFiles();
	}

	if (g_bOptShowTotalParent) {
		CMySuperGrid	&listCtrl = GetListCtrl();
		listCtrl.CalcSum(0);
	}

	// カラム幅の自動調整
	if (g_bOptLoadFileAdjustColumn) {
		CMySuperGrid	&listCtrl = GetListCtrl();
		listCtrl.AutoSizeColumns(-1);
	}
}

// =============================================
// CSuperTagEditorDoc::ClearRequestFiles
// 概要  : 追加リクエストファイルリストの初期化
// 引数  : none
// 戻り値: none
// =============================================
void CSuperTagEditorDoc::ClearRequestFiles(void)
{
	m_arrayRequestFiles.RemoveAll();
	m_arrayRequestFiles.SetSize(INIT_ARRAY_SIZE);
	m_nArrayRequestFileCount = 0;
}

// =============================================
// CSuperTagEditorDoc::LoadPlayList
// 概要  : プレイリストを読みこみます
// 引数  : sPlayList	= プレイリストファイル名(絶対パスで指定)
// 戻り値: bool			= true:正常終了 / false:エラー
// =============================================
bool CSuperTagEditorDoc::LoadPlayList(LPCTSTR sPlayList)
{
	TRY {
		CFile	file;

		// プレイリストファイルのカレントディレクトリを取得
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sPlayList, drive, dir, NULL, NULL);

		if (file.Open(sPlayList, CFile::modeRead)) {
			CString		strLine;
			CArchive	ar(&file, CArchive::load);
			while(ar.ReadString(strLine)) {
				if (strLine.GetLength() >= 3) {
					if (strLine[0] != '#') {
						// リストに追加
						if ((strLine[0] == '\\' && strLine[1] == '\\')
						||  strLine[1] == ':') {
							// 絶対パス
							AddRequestFile(strLine, NULL);
						} else {
							// 相対パス
							CString	strName;
							if (strLine[0] == '\\') {
								// ドライブ名無しのルート指定
								strName.Format(TEXT("%s%s"), drive, strLine);
							} else {
								// ルート以外
								strName.Format(TEXT("%s%s%s"), drive, dir, strLine);
							}
							AddRequestFile(strName, NULL);
						}
					}
				}
			}
		}
	}
	CATCH( CFileException, e) {
		CString	str;
		str.Format(TEXT("%s の読み込みに失敗しました"), sPlayList);
		MessageBox(NULL, str, TEXT("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
	}
	END_CATCH

	return(true);
}

// =============================================
// CSuperTagEditorDoc::FoundFile
// 概要  : ファイル検索：ファイルを発見
// 引数  : sFileName	= ファイル名
//       : pItemParent	= 親アイテム(NULL=ルート)
//       : bCheck		= チェックマークを付けるかどうか
// 戻り値: bool			= true:正常終了 / false:異常終了
// =============================================
bool CSuperTagEditorDoc::FoundFile(LPCTSTR sFileName, CSuperGridCtrl::CTreeItem *pItemParent, bool bCheck)
{
	// ＭＰ３ファイル情報の取得
	CFileMP3	fileMP3;
	if (fileMP3.Attach(sFileName)) {
		int		nIndex = m_nArrayFileCount;

		// 配列に追加
		if (m_nArrayFileCount > m_arrayFiles.GetUpperBound()) {
			// 配列が足りない場合は、確保サイズを増加させる
			m_arrayFiles.SetSize(m_arrayFiles.GetUpperBound() + ADD_ARRAY_SIZE);
		}
		m_arrayFiles[m_nArrayFileCount] = fileMP3.m_fileMP3;
		m_nArrayFileCount++;

		// リストコントロールを取得
		CMySuperGrid	&listCtrl = GetListCtrl();
		if (g_classInfo.nType == 0) {
			// フォルダで分類
			listCtrl.AddFile(GetListMP3(nIndex), pItemParent, (LPARAM)nIndex, bCheck);
		} else {
			// タグ情報で分類
			listCtrl.AddFile2(GetListMP3(nIndex), pItemParent, (LPARAM)nIndex, bCheck);
		}

		return(true);
	} else {
		CString	strMessage;
		strMessage.Format(TEXT("%s の読み込みに失敗しました"), sFileName);
		//MessageBox(NULL, strMessage, "ファイル読み込みエラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(strMessage);	// ステータスバーにエラーを表示
	}

	return(false);
}

// =============================================
// CSuperTagEditorDoc::SearchFileReent
// 概要  : ファイル検索：検索処理
// 引数  : sDir		= 検索するディレクトリ
//       : pItemDir	= ディレクトリアイテム(NULL=ルート)
// 戻り値: bool		= true:正常終了 / false:異常終了
// =============================================
bool CSuperTagEditorDoc::SearchFileReent(LPCTSTR sDir, CSuperGridCtrl::CTreeItem *pItemDir)
{
	//追加 by Kobarin
	if(m_dlgLoadProgress->IsCanceled()){
		return true;
	}

	HANDLE	hFindFile;
	WIN32_FIND_DATA	data;
	std::vector<TCHAR> sCurFile(MAX_PATH + 1);

	// 検索開始ディレクトリの場合の処理
	if (pItemDir == NULL) {
		// ルートにディレクトリアイテム追加
		if (g_classInfo.nType == 0) {
			// フォルダで分類
			CMySuperGrid	&listCtrl = GetListCtrl();
//			pItemDir = listCtrl.AddDirectory(sDir, listCtrl.m_itemClassOK, 0);
			pItemDir = listCtrl.m_pItemRoot;
		}
	}

	// 同一ディレクトリの全てのファイルを検索
	// サブディレクトリの検索は行わない
	//static	char	*sFindPattern[] = {
	//	"*.rmp", "*.mp3", "*.ape", "*.wma", "*.wmv", "*.asf", "*.ogg"/* Conspiracy 196 */, "*.wav"/* Conspiracy 197 */, "*.avi"/* Conspiracy 197 */, NULL,
	//};
	extern void GetFileExtList(CStringArray& arExt);
	CStringArray arExt;
	GetFileExtList(arExt);
	for (int nType = 0; nType < arExt.GetSize() ; nType++) {
		// 検索ファイルのパターンを設定
		_stprintf(sCurFile.data(), TEXT("%s%s"), sDir, TEXT("*.") + arExt[nType]);

		// ファイル検索
		if ((hFindFile = FindFirstFile(sCurFile.data(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				m_dlgLoadProgress->SetPos(0);//追加 by Kobarin
				if(m_dlgLoadProgress->IsCanceled()){
					break;
				}
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					// ディレクトリ付きファイル名作成
					lstrcpy(sCurFile.data(), sDir);
					lstrcat(sCurFile.data(), data.cFileName);
					m_sTargetFile = sCurFile.data();
					// ファイル発見処理
					AddRequestFile(m_sTargetFile.c_str(), pItemDir);
				}
			} while(FindNextFile(hFindFile, &data) == TRUE);
		}
		if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);
	}

	if (g_bEnableSearchSubDir) {	// サブディレクトリの検索
		_stprintf(sCurFile.data(), TEXT("%s*.*"), sDir);
		if ((hFindFile = FindFirstFile(sCurFile.data(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				m_dlgLoadProgress->SetPos(0);//追加 by Kobarin
				if(m_dlgLoadProgress->IsCanceled()){
					break;
				}
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
					if (lstrcmp(data.cFileName, TEXT(".") ) != 0		// カレントディレクトリ以外
					&&  lstrcmp(data.cFileName, TEXT("..")) != 0		// 親ディレクトリ以外
					&& (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0/* STEP 032 */) { // 隠しフォルダ以外
						// ディレクトリ付きファイル名作成
						lstrcpy(sCurFile.data(), sDir);
						lstrcat(sCurFile.data(), data.cFileName);
						m_sTargetFile = sCurFile.data();
						// ツリーにディレクトリアイテム追加
						CSuperGridCtrl::CTreeItem	*pItemSubDir = NULL;
						if (g_classInfo.nType == 0) {
							// フォルダで分類
							CMySuperGrid	&listCtrl = GetListCtrl();
							pItemSubDir = listCtrl.AddDirectory(data.cFileName, pItemDir);
						}
						// サブディレクトリ突入処理
						lstrcat(sCurFile.data(), TEXT("\\"));
						if (!SearchFileReent(sCurFile.data(), pItemSubDir)) {
							break;				// サブディレクトリ検索エラー
						}
						sCurFile[lstrlen(sCurFile.data())-1] = NULL;
						m_sTargetFile = sCurFile.data();	// サブディレクトリの処理が入るので再設定
					}
				}
			} while(FindNextFile(hFindFile, &data) == TRUE);
		}
		if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile); /* 2004.02.18 こっちに移動 */
	}

	return(true);
}

// =============================================
// CSenkyokuMeijinDoc::SelectDirectory
// 概要  : フォルダ参照ダイアログ処理
// 引数  : sLocal			= パス(入出力)
// 戻り値: bool
// =============================================
BOOL CSuperTagEditorDoc::SelectDirectory(LPTSTR sLocal)
{
	bool	bResult;
	CSHBrowseForFolder	browse(true, g_bEnableSearchSubDir);
	bResult = browse.Exec(sLocal);
	g_bEnableSearchSubDir = browse.GetSearchSubDirState();
	return(bResult);
}

void CSuperTagEditorDoc::OpenFolder(LPCTSTR sOpenDir)
{
	//追加 by Kobarin
	if(IsTagUpdating())//タグを更新中は追加しない
		return;
	if(m_dlgLoadProgress->IsCanceled()){
		//キャンセルボタンが押されている
		return ;
	}

	((CSuperTagEditorApp *)AfxGetApp())->AddToRecentFileList (sOpenDir); /* StartInaction 053 */

	// ディレクトリのファイルを読み込む
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};

	lstrcpy(sFolderName, sOpenDir);

	int		nLen;
	nLen = lstrlen(sFolderName);
	if (nLen > 0) {
		if (IsFolderName(sFolderName) == false) {
			// 最後は '\\' である事
			lstrcat(sFolderName, TEXT("\\"));
		}

		// プログレスバー初期化
		BOOL	bIsOpen = (m_dlgLoadProgress->GetSafeHwnd() == NULL) ? TRUE : FALSE;
		if (bIsOpen) StartLoadFile(TEXT("ＭＰ３ファイル読み込み中..."));

		// フォルダ読み込み
		if (g_classInfo.nType == 0) {
			// フォルダで分類
			CMySuperGrid	&listCtrl = GetListCtrl();
			TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
			_tsplitpath(sFolderName, drive, dir, NULL, NULL);
			CString	str;
			str.Format(TEXT("%s%s"), drive, dir);
			SearchFileReent(sFolderName, listCtrl.AddDirectory(str, listCtrl.m_pItemRoot, 2));
		} else {
			// タグ情報で分類
			SearchFileReent(sFolderName);
		}

		// 追加リクエストのあったファイルのタグ情報を読み込む
#ifdef FLICKERFREE
		//CMySuperGrid	&listCtrl = GetListCtrl();
		//listCtrl.SetRedraw(FALSE);
		//GetView()->SetRedraw(FALSE);
#endif
		ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//listCtrl.SetRedraw(TRUE);
		//GetView()->SetRedraw(TRUE);
#endif

		// プログレスバー終了
		if (bIsOpen) EndLoadFile();

		UpdateAllViews(NULL);
	}
}

void CSuperTagEditorDoc::OnUpdateOpenFolder(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnOpenFolder()
{
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};

	// フォルダ選択ダイアログを開く
	lstrcpy(sFolderName, g_strCurrentDirectory);
	if (SelectDirectory(sFolderName) == TRUE) {
		g_strCurrentDirectory = sFolderName;

		OpenFolder(g_strCurrentDirectory);
	}
}

BOOL CSuperTagEditorDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	if (CheckFileModified()) {
		int		nResult;
		nResult = MessageBox(AfxGetMainWnd()->GetSafeHwnd(), TEXT("タグ情報が変更されています。\n"
								   "変更されているファイルを更新してもよろしいですか？"),
								   TEXT("保存確認"), MB_YESNOCANCEL);
		switch(nResult) {
		case IDYES:
			//OnSaveAllTag();
			SaveAllTag(TRUE);
			if (m_bSaveAllTagResult == false) {
				return(FALSE);
			}
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return(FALSE);
		}
	}
	InitFileList(); /* Conspiracy 196 */
	return CDocument::CanCloseFrame(pFrame);
}

void CSuperTagEditorDoc::OnUpdateSaveAllTag(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorDoc::OnSaveAllTag()
{
	SaveAllTag(FALSE);
}

void CSuperTagEditorDoc::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

// [オプション設定]-[全部]
void CSuperTagEditorDoc::OnUpdateDlgEnvironment(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnDlgEnvironment()
{
	CDlgEnvSheet	dialog(TEXT("オプション設定"));
	bool	bKeepTimeStamp = g_bOptKeepTimeStamp;
	bool	bHideMP3ListFile = g_bOptHideMP3ListFile;
	bool	bFileNameMaxCellColor = g_bFileNameMaxCellColor; /* SeaKnows 036 */

	dialog.m_pDoc = this;
	dialog.DoModal(CDlgEnvSheet::ENV_ALL);

	CMySuperGrid	&listCtrl = GetListCtrl();

	// タイムスタンプ保持の設定項目が変更された
	// MP3 List のファイル名表示の有無が変更された
	if (bKeepTimeStamp != g_bOptKeepTimeStamp
	||  bHideMP3ListFile != g_bOptHideMP3ListFile) {
		// 表示を更新させる
		listCtrl.UpdateAllFiles();
		// カラム幅の自動調整
		if (g_bOptLoadFileAdjustColumn) listCtrl.AutoSizeColumns(-1);
	}
	if (bFileNameMaxCellColor != g_bFileNameMaxCellColor) { /* SeaKnows 036 */
		// 表示を更新させる
		listCtrl.UpdateAllFiles();
	}

	// 分類済みフォルダの名称を更新する
	listCtrl.UpdateSyncFolderItemName();

	// ジャンルリストを更新させる
	listCtrl.MakeStrListGenre();
}

BOOL CSuperTagEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;

	TCHAR	sFileName[FILENAME_MAX];
	lstrcpy(sFileName, lpszPathName);

	// 初期化処理
	if (m_bInitialized == false) { /* StartInaction2 055 */
		InitFileList();			// ファイルリストの初期化
		ClearRequestFiles();	// 追加リクエストファイルリストの初期化
		m_bInitialized = true;
		InitPlugin();			// プラグインのロード
		// 初期化処理
		GetView()->m_List.InitializeGrid();
		// フォントの設定
		GetView()->UpdateFontListView();
		InitGridList();			// グリッドを初期化
	}

	// プレイリストか？
	CString	strFileName = sFileName;
	strFileName.MakeLower();
	if (strFileName.Find(TEXT(".m3u")) != -1) {
		// プレイリスト追加
		GetView()->LoadPlayList(sFileName);
		return(TRUE);
	}

	// プログレスバー初期化
	StartLoadFile(TEXT("ＭＰ３ファイル読み込み中..."));

	CFileStatus	status;
	if (CFile::GetStatus(sFileName, status) == TRUE && status.m_mtime != -1) {
		if (status.m_attribute & CFile::directory) {
			// ディレクトリの場合
			// 最後の '\\' は必要
			if (IsFolderName(sFileName) == false) {
				lstrcat(sFileName, TEXT("\\"));
			}
			bool bEnableSearchSubDir = g_bEnableSearchSubDir; /* TyphoonSwell 026 */
			if (g_bOptDropSearchSubFolder) {
				g_bEnableSearchSubDir = true;
			}

			/* WildCherry2 078 */
			//SearchFileReent(sFileName);
			OpenFolder(sFileName);
			g_bEnableSearchSubDir = bEnableSearchSubDir;
		} else {
			// ファイルの場合(ロングファイル名に変換してから処理する)
			if (GetLongPathName(sFileName, status.m_szFullName, FILENAME_MAX) != 0) {
				CString	strLongFileName;
				TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
				_tsplitpath(status.m_szFullName, drive, dir, NULL, NULL);
				strLongFileName.Format(TEXT("%s%s%s"), drive, dir, sFileName);
				AddRequestFile(strLongFileName, NULL);
			}
		}
	} else if (lstrlen(sFileName) == 3 && sFileName[1] == ':' && sFileName[2] == '\\') { /* STEP 027 */
			// ドライブの場合
			// 下位階層を検索
			bool bEnableSearchSubDir = g_bEnableSearchSubDir;
			if (g_bOptDropSearchSubFolder) {
				g_bEnableSearchSubDir = true;
			}
			OpenFolder(sFileName);
			g_bEnableSearchSubDir = bEnableSearchSubDir;
	}

	// 追加リクエストのあったファイルのタグ情報を読み込む
#ifdef FLICKERFREE
		//GetView()->SetRedraw(FALSE);
#endif
	ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//GetView()->SetRedraw(TRUE);
#endif

	// プログレスバー終了
	EndLoadFile();

	UpdateAllViews(NULL);

	return(TRUE);
}

// SI フィールドを表示／編集
void CSuperTagEditorDoc::OnUpdateEditFieldSi(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_bOptEditFieldSIF ? TRUE : FALSE);
}
void CSuperTagEditorDoc::OnEditFieldSi()
{
	if (g_bOptEditFieldSIF != true) {
		g_bOptEditFieldSIF = true;

		// 情報を更新
		UpdateAllFiles(true);
	}
}

// ID3 tag 情報を表示／編集
void CSuperTagEditorDoc::OnUpdateEditTd3Tag(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_bOptEditFieldSIF ? FALSE : TRUE);
}
void CSuperTagEditorDoc::OnEditTd3Tag()
{
	if (g_bOptEditFieldSIF != false) {
		g_bOptEditFieldSIF = false;

		// 情報を更新
		UpdateAllFiles(true);
	}
}

// [ID3 tag] <=> [SIフィールド]を切替
void CSuperTagEditorDoc::OnUpdateEditChangeField(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnEditChangeField()
{
	g_bOptEditFieldSIF = g_bOptEditFieldSIF ? false : true;

	// 情報を更新
	UpdateAllFiles(true);
}

// 分類表示の更新
void CSuperTagEditorDoc::OnUpdateExecClassification(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_nArrayFileCount > 0 && g_classInfo.nType) ? TRUE : FALSE);
}
void CSuperTagEditorDoc::OnExecClassification()
{
	// プレイリスト読みこみ
	StartLoadFile(TEXT("分類表示を更新中..."));
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("処理中..."));
	m_dlgLoadProgress->EnableCancelButton(FALSE);

	// グリッドに再登録
	CMySuperGrid	&listCtrl = GetListCtrl();
	listCtrl.DeleteAllEx();			// グリッドを初期化
	for (int no = 0; no < m_nArrayFileCount; no++) {
		// プログレスバー更新
		if (m_nArrayFileCount > 1) {
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayFileCount-1));
		}
		// フォルダで分類
		if (GetListMP3(no)->nFormat != FILE_FORMAT_UNKNOWN) {
			listCtrl.AddFile2(GetListMP3(no), NULL, (LPARAM)no, g_bOptLoadFileChecked);
		}
	}

	// ファイルのタグ情報を読み込む
	EndLoadFile();

	UpdateAllViews(NULL);
}

#include "DlgFileOverWrite.h"
bool CSuperTagEditorDoc::CheckFileAttribute(FILE_MP3 *fileMP3)
{
	extern	int		g_nWriteTagProcFlag;
	if (g_nWriteTagProcFlag == 3) {
		return(false);						// 以降中止
	}

	// ファイル情報を取得
	CFileStatus	fileStatus;
	if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
		return(false);
	}
	// 読み出し専用ファイルは、上書き確認を行う
	if (fileStatus.m_attribute & CFile::readOnly) {
		if (g_nWriteTagProcFlag != 1) {
			// 上書き確認ダイアログを開く
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
			case 0:				// このファイルを上書き
			case 1:				// 以降全て上書き
				break;
			case 2:				// スキップ
			case 3:				// 中止
			default:
				return(false);
			}
		}

		// 読み込み専用属性を解除
		SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
	}
	return(true);
}

void CSuperTagEditorDoc::OnDlgSetupPlugin()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CDlgPluginSetup	dialog;

	if (dialog.DoModal() == IDOK) {
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator(); /* STEP 030 */
	}
}

void CSuperTagEditorDoc::OnUpdateDlgSetupPlugin(CCmdUI* pCmdUI)
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorDoc::SaveAllTag(BOOL bSaveOnly)
{
	m_bSaveAllTagResult = true;
	extern	int		g_nWriteTagProcFlag;

	// プログレスバー表示
	StartLoadFile(TEXT("タグ情報を更新中..."));
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("処理中..."));

	// 更新ファイルのタグ情報更新
	g_nWriteTagProcFlag = 0;
	for (int no = 0; no < m_nArrayFileCount; no++) {
		FILE_MP3	*fileMP3 = GetListMP3(no);
		if (m_nArrayFileCount != 1) {
			// プログレスバー更新
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayFileCount-1));
			// 追加 by Kobarin
			if(m_dlgLoadProgress->IsCanceled()){
				m_bSaveAllTagResult = false;
				break;
			}
		}
		if (fileMP3->nFormat != FILE_FORMAT_UNKNOWN && fileMP3->bModifyFlag) {
			if (CFileMP3::WriteTag(fileMP3, g_bOptKeepTimeStamp)) {
				fileMP3->bModifyFlag = FALSE;
			} else {
				MessageBox(NULL, TEXT("タグ情報をファイルを更新できませんでした"), fileMP3->strFullPathName, MB_ICONSTOP|MB_OK|MB_TOPMOST);
				m_bSaveAllTagResult = false;
			}
		}
	}

	// プログレスバー終了
	EndLoadFile();

	if (bSaveOnly)	return;

	// 情報を更新
	CMySuperGrid	&listCtrl = GetListCtrl();
#ifdef FLICKERFREE
	//listCtrl.SetRedraw(FALSE);
#endif
	listCtrl.UpdateAllFiles();
	UpdateAllFiles(true);
	if (g_bOptLoadFileAdjustColumn) listCtrl.AutoSizeColumns(-1); /* 2003.06.26 */
#ifdef FLICKERFREE
	//listCtrl.SetRedraw(TRUE);
#endif
}
