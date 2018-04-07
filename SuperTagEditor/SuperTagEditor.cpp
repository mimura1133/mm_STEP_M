// SuperTagEditor.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "SuperTagEditor.h"
#include "AutoBuildCount.h"
#include "MainFrm.h"
#include "SuperTagEditorDoc.h"
#include "SuperTagEditorView.h"
//追加 by Kobarin
#include "dde/kbdde.h"
#include <dos.h> //コマンドラインの解析がめんどいので(^^;
#include "DlgCommonProg.h"

#include "FileMP3.h" /* WildCherry2 078 */

#include "Plugin.h"

#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RECT		g_rectMainWindow;		// メインウィンドウの座標
BOOL		g_bMainFrameZoomed;
BOOL		g_bMainFrameIconic;
CString		g_strCurrentDirectory;
CString		g_strCurrentPlayList;
CString		g_strCurrentMoveDirectory;
bool		g_bIsVersionUp;
int			g_nUserConvFormatType;
USER_CONV_FORMAT	g_userConvFormat[USER_CONV_FORMAT_MAX];
USER_CONV_FORMAT_EX	g_userConvFormatEx[USER_CONV_FORMAT_EX_MAX];
USER_MOVE_FODLER_FORMAT	g_userMoveFolder[USER_MOVE_FODLER_FORMAT_MAX];
USER_COPY_FORMAT_FORMAT	g_userCopyFormat[USER_COPY_FORMAT_FORMAT_MAX]; /* FunnyCorn 175 */
USER_CONV_FORMAT_TAG2TAG g_userConvFormatTag2Tag[USER_CONV_FORMAT_TAG2TAG_MAX]; /* STEP 034 */
TEIKEI_INFO g_teikeiInfo[TEIKEI_INFO_MAX]; /* STEP 035 */
WRITE_FORMAT		g_writeFormat[WRITE_FORMAT_MAX];
bool		g_bOptESCEditCancel;
bool		g_bOptEnableEditCursorExit;
bool		g_bOptEnterBeginEdit;
bool		g_bOptEditOkDown;
bool		g_bOptKeepTimeStamp;
bool		g_bOptChangeFileExt;
bool		g_bOptSyncCreateTime;
bool		g_bOptChangeTextFile;
int			g_nOptCheckFileName;
CString		g_sOptWinAmpPath;
int			g_nOptPlayerType;
bool		g_bOptEditFieldSIF;
bool		g_bOptAutoOpenFolder;
bool		g_bOptLoadFileChecked;
bool		g_bOptHideMP3ListFile;
bool		g_bOptDropSearchSubFolder; /* TyphoonSwell 026 */
bool		g_bOptShowZenSpace; /* BeachMonster 107 */
CString		g_sOptShowOtherChar; /* BeachMonster 107 */ // とりあえずいれとく
bool		g_bOptSortIgnoreCase; /* BeachMonster4 114 */
bool		g_bOptSortIgnoreZenHan; /* BeachMonster4 114 */
bool		g_bOptSortIgnoreKataHira; /* FunnyCorn 179 */
bool		g_bOptShowTotalParent; /* RockDance 128 */
bool		g_bOptShowTips; /* Rumble 188 */
bool		g_bOptLoadFileAdjustColumn;
bool		g_bOptSearchLyricFile;
bool		g_bOptSetLyricsDir;
bool		g_bOptSearchLyricsSubDir;
bool		g_bEnableSearchSubDir;
bool		g_bEnableMoveFolderCopy;
CString		g_strOptLyricsPath;
SORT_STATE	g_sortState[SORT_KEY_MAX];
CLASS_INFO	g_classInfo;
CHECK_WORD_STATE	g_chkWord[CHECK_STATE_MAX];

//bool		g_bOptID3v2GenreAddNumber;
bool		g_bConfFileNameMaxCheck;
bool		g_bFileNameMaxCellColor; /* SeaKnows 036 */
int			g_nConfFileNameMaxChar;

bool		g_bEnableFolderSync;
CString		g_strRootFolder;
bool		g_bSyncSelectAlways;
bool		g_bSyncDeleteFolder;
bool		g_bSyncLyricsFileMove;

bool		g_bConfDeleteFile;
bool		g_bConfDeleteList;
bool		g_bConfEditModify;
bool		g_bConfFolderSync;

bool		g_bPlayListClearList;
bool		g_bPlayListClearCheck;
bool		g_bPlayListAddList;
bool		g_bPlayListFileCheck;

/* STEP 035 *///CString		g_strTeikei[10+20];	/* SeaKnows 030 *//* FreeFall 046 */
CString		g_strTeikeiGroupName[3];	/* FreeFall 046 */
bool		g_bValidFolderSelect;	/* SeaKnows 033 */
bool		g_bValidDupExec;	/* FreeFall 045 */

int			g_nRecentFolder; /* StartInaction 053 */
bool		g_bSaveRepDlgPos; /* WildCherry4 086 */
int			g_nSaveRepDlgPosX; /* WildCherry4 086 */
int			g_nSaveRepDlgPosY; /* WildCherry4 086 */

// 文字種統一 /* StartInaction 054 */
UINT		g_nUnifyAlpha;
UINT		g_nUnifyHiraKata;
UINT		g_nUnifyKata;
UINT		g_nUnifyKigou;
UINT		g_nUnifySuji;
UINT		g_nUnifyUpLow;
UINT		g_nUnifyFixedUpLow; /* STEP 040 */

// ファイル名文字種統一 /* LastTrain 058 */
UINT		g_nFileUnifyAlpha;
UINT		g_nFileUnifyHiraKata;
UINT		g_nFileUnifyKata;
UINT		g_nFileUnifyKigou;
UINT		g_nFileUnifySuji;
UINT		g_nFileUnifyUpLow;

// 拡張子変換 /* STEP 006 */
UINT		g_nFileExtChange;

bool		g_bEndEditMoveRightCell; /* BeachMonster 091 */

bool		g_bShowLoadPlaylistDlg; /* RockDance 126 */

CString		g_strFavorite[10];	/* RockDance 129 */

int			g_nAddNumberWidth; /* Baja 159 */
int			g_nAddNumberPos; /* Baja 159 */
CString		g_strAddNumberSep; /* Baja 159 */
CString		g_strAddNumberBef; /* Conspiracy 194 */
CString		g_strAddNumberAft; /* Conspiracy 194 */

bool		g_bAudioListShow; /* Conspiracy 199 */

bool		g_bFirstUpperIgnoreWord; /* STEP 026 */
CString		g_strFirstUpperIgnoreWords; /* STEP 026 */
CString		g_strFirstUpperSentenceSeparator; /* STEP 026 */
bool		g_bUserConvAddMenu; /* STEP 030 */
bool		g_bZenHanKigouKana; /* STEP 016 */

LOGFONT		g_fontReport;			// レポートウィンドウのフォント

FILENAME_REPLACE	g_fileNameReplace[FILENAME_REPLACE_MAX];

FILENAME_REPLACE	g_userFileNameReplace[USER_FILENAME_REPLACE_MAX];	/* FreeFall 050 */

const LPCTSTR g_sRepTable[FILENAME_REPLACE_MAX][2] = {
	{TEXT("\""), TEXT("”")}, {TEXT("*") , TEXT("＊")}, {TEXT(","), TEXT("，")}, {TEXT("/"), TEXT("／")},
	{TEXT(":") , TEXT("：")}, {TEXT(";") , TEXT("；")}, {TEXT("<"), TEXT("＜")}, {TEXT(">"), TEXT("＞")},
	{TEXT("?") , TEXT("？")}, {TEXT("\\"), TEXT("￥")}, {TEXT("|"), TEXT("｜")}, {TEXT(" "), TEXT(" ")},
};

CStringArray g_arFixedWords; /* STEP 040 */

#define HK_CTRL_C				0x00020043
#define HK_CTRL_D				0x00020044
#define HK_CTRL_F				0x00020046
#define HK_CTRL_H				0x00020048
#define HK_CTRL_N				0x0002004e
#define HK_CTRL_O				0x0002004f
#define HK_CTRL_S				0x00020053
#define HK_CTRL_V				0x00020056
#define HK_CTRL_X				0x00020058

KEY_CONFIG	g_listKeyConfig[] = {
	/**
	 【注意】コマンドを追加した場合は、_APS_NEXT_COMMAND_VALUEが更新されるのでSTEP_api.cppをリコンパイルすること
	 **/
	// ファイル処理
	{ID_FILE_NEW                  , 0x00000000, COMMAND_GROUP_FILE, "リストをクリア", TEXT("FileNew")},
	{ID_OPEN_FOLDER               , HK_CTRL_O , COMMAND_GROUP_FILE, "フォルダを開く", TEXT("FileOpenFolder")},
	{ID_SAVE_ALL_TAG              , HK_CTRL_S , COMMAND_GROUP_FILE, "タグ情報を更新", TEXT("FileSaveAllTag")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_FILE, "--------------------", NULL},
	{ID_WRITE_LIST1               , 0x00000000, COMMAND_GROUP_FILE, "リスト出力：書式１", TEXT("WriteList1")},
	{ID_WRITE_LIST2               , 0x00000000, COMMAND_GROUP_FILE, "リスト出力：書式２", TEXT("WriteList2")},
	{ID_WRITE_LIST3               , 0x00000000, COMMAND_GROUP_FILE, "リスト出力：書式３", TEXT("WriteList3")},
	{ID_WRITE_LIST4               , 0x00000000, COMMAND_GROUP_FILE, "リスト出力：書式４", TEXT("WriteList4")},
	{ID_WRITE_LIST5               , 0x00000000, COMMAND_GROUP_FILE, "リスト出力：書式５", TEXT("WriteList5")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_FILE, "--------------------", NULL},
	{ID_MOVE_FOLDER_01            , 0x00000000, COMMAND_GROUP_FILE, "ファイル移動：書式１", TEXT("MoveFolder1")},
	{ID_MOVE_FOLDER_02            , 0x00000000, COMMAND_GROUP_FILE, "ファイル移動：書式２", TEXT("MoveFolder2")},
	{ID_MOVE_FOLDER_03            , 0x00000000, COMMAND_GROUP_FILE, "ファイル移動：書式３", TEXT("MoveFolder3")},
	{ID_MOVE_FOLDER_04            , 0x00000000, COMMAND_GROUP_FILE, "ファイル移動：書式４", TEXT("MoveFolder4")},
	{ID_MOVE_FOLDER_05            , 0x00000000, COMMAND_GROUP_FILE, "ファイル移動：書式５", TEXT("MoveFolder5")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_FILE, "--------------------", NULL},
	{ID_FAVORITE_FOLDER_01        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り１", TEXT("FavoriteFolder1")},
	{ID_FAVORITE_FOLDER_02        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り２", TEXT("FavoriteFolder2")},
	{ID_FAVORITE_FOLDER_03        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り３", TEXT("FavoriteFolder3")},
	{ID_FAVORITE_FOLDER_04        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り４", TEXT("FavoriteFolder4")},
	{ID_FAVORITE_FOLDER_05        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り５", TEXT("FavoriteFolder5")},
	{ID_FAVORITE_FOLDER_06        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り６", TEXT("FavoriteFolder6")},
	{ID_FAVORITE_FOLDER_07        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り７", TEXT("FavoriteFolder7")},
	{ID_FAVORITE_FOLDER_08        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り８", TEXT("FavoriteFolder8")},
	{ID_FAVORITE_FOLDER_09        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り９", TEXT("FavoriteFolder9")},
	{ID_FAVORITE_FOLDER_10        , 0x00000000, COMMAND_GROUP_FILE, "お気に入りのフォルダ：お気に入り10", TEXT("FavoriteFolder10")},

	// 編集処理
	{ID_EDIT_COPY                 , HK_CTRL_C , COMMAND_GROUP_EDIT, "コピー"  , TEXT("EditCopy")},
	{ID_EDIT_PASTE                , HK_CTRL_V , COMMAND_GROUP_EDIT, "貼り付け", TEXT("EditPaste")},
	{ID_EDIT_CUT                  , HK_CTRL_X , COMMAND_GROUP_EDIT, "切り取り", TEXT("EditCut")},
	{ID_EDIT_PASTE_ADD            , 0x00000000, COMMAND_GROUP_EDIT, "追加で貼り付け", TEXT("EditPasteAdd")}, /* FunnyCorn 176 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "----- 定型文 グループ１ -----", NULL},
	{ID_TEIKEI_01                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-１", TEXT("TeikeiPaste1-1")}, /* SeaKnows 030 */ 
	{ID_TEIKEI_02                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-２", TEXT("TeikeiPaste1-2")}, /* SeaKnows 030 */
	{ID_TEIKEI_03                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-３", TEXT("TeikeiPaste1-3")}, /* SeaKnows 030 */
	{ID_TEIKEI_04                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-４", TEXT("TeikeiPaste1-4")}, /* SeaKnows 030 */
	{ID_TEIKEI_05                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-５", TEXT("TeikeiPaste1-5")}, /* SeaKnows 030 */
	{ID_TEIKEI_06                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-６", TEXT("TeikeiPaste1-6")}, /* SeaKnows 030 */
	{ID_TEIKEI_07                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-７", TEXT("TeikeiPaste1-7")}, /* SeaKnows 030 */
	{ID_TEIKEI_08                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-８", TEXT("TeikeiPaste1-8")}, /* SeaKnows 030 */
	{ID_TEIKEI_09                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-９", TEXT("TeikeiPaste1-9")}, /* SeaKnows 030 */
	{ID_TEIKEI_10                 , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け１-１０", TEXT("TeikeiPaste1-10")}, /* SeaKnows 030 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "----- 定型文 グループ２ -----", NULL},
	{ID_TEIKEI_2_01               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-１", TEXT("TeikeiPaste2-1")}, /* FreeFall 046 */
	{ID_TEIKEI_2_02               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-２", TEXT("TeikeiPaste2-2")}, /* FreeFall 046 */
	{ID_TEIKEI_2_03               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-３", TEXT("TeikeiPaste2-3")}, /* FreeFall 046 */
	{ID_TEIKEI_2_04               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-４", TEXT("TeikeiPaste2-4")}, /* FreeFall 046 */
	{ID_TEIKEI_2_05               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-５", TEXT("TeikeiPaste2-5")}, /* FreeFall 046 */
	{ID_TEIKEI_2_06               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-６", TEXT("TeikeiPaste2-6")}, /* FreeFall 046 */
	{ID_TEIKEI_2_07               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-７", TEXT("TeikeiPaste2-7")}, /* FreeFall 046 */
	{ID_TEIKEI_2_08               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-８", TEXT("TeikeiPaste2-8")}, /* FreeFall 046 */
	{ID_TEIKEI_2_09               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-９", TEXT("TeikeiPaste2-9")}, /* FreeFall 046 */
	{ID_TEIKEI_2_10               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け２-１０", TEXT("TeikeiPaste2-10")}, /* FreeFall 046 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "----- 定型文 グループ３ -----", NULL},
	{ID_TEIKEI_3_01               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-１", TEXT("TeikeiPaste3-1")}, /* FreeFall 046 */
	{ID_TEIKEI_3_02               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-２", TEXT("TeikeiPaste3-2")}, /* FreeFall 046 */
	{ID_TEIKEI_3_03               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-３", TEXT("TeikeiPaste3-3")}, /* FreeFall 046 */
	{ID_TEIKEI_3_04               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-４", TEXT("TeikeiPaste3-4")}, /* FreeFall 046 */
	{ID_TEIKEI_3_05               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-５", TEXT("TeikeiPaste3-5")}, /* FreeFall 046 */
	{ID_TEIKEI_3_06               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-６", TEXT("TeikeiPaste3-6")}, /* FreeFall 046 */
	{ID_TEIKEI_3_07               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-７", TEXT("TeikeiPaste3-7")}, /* FreeFall 046 */
	{ID_TEIKEI_3_08               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-８", TEXT("TeikeiPaste3-8")}, /* FreeFall 046 */
	{ID_TEIKEI_3_09               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-９", TEXT("TeikeiPaste3-9")}, /* FreeFall 046 */
	{ID_TEIKEI_3_10               , 0x00000000, COMMAND_GROUP_EDIT, "定型文貼り付け３-１０", TEXT("TeikeiPaste3-10")}, /* FreeFall 046 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_FILE, "--------------------", NULL},
	{ID_EDIT_COPY_FORMAT_01       , 0x00000000, COMMAND_GROUP_FILE, "書式コピー：書式１", TEXT("CopyFormat1")},
	{ID_EDIT_COPY_FORMAT_02       , 0x00000000, COMMAND_GROUP_FILE, "書式コピー：書式２", TEXT("CopyFormat2")},
	{ID_EDIT_COPY_FORMAT_03       , 0x00000000, COMMAND_GROUP_FILE, "書式コピー：書式３", TEXT("CopyFormat3")},
	{ID_EDIT_COPY_FORMAT_04       , 0x00000000, COMMAND_GROUP_FILE, "書式コピー：書式４", TEXT("CopyFormat4")},
	{ID_EDIT_COPY_FORMAT_05       , 0x00000000, COMMAND_GROUP_FILE, "書式コピー：書式５", TEXT("CopyFormat5")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "--------------------", NULL},
	{ID_CELL_COPY_DOWN            , HK_CTRL_D , COMMAND_GROUP_EDIT, "下方向へコピー", TEXT("EditCopyDown")},
	{ID_SET_NUMBER                , HK_CTRL_N , COMMAND_GROUP_EDIT, "下方向へ連番"  , TEXT("EditSetNumber")},
	{ID_SET_NUMBER_ADD            , 0x00000000, COMMAND_GROUP_EDIT, "下方向へ連番（先頭に追加）"  , TEXT("EditSetNumberAdd")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "--------------------", NULL},
	{ID_EDIT_FIND                 , 0x00000000, COMMAND_GROUP_EDIT, "検索", TEXT("EditFind")},
	{ID_EDIT_REPLACE              , 0x00000000, COMMAND_GROUP_EDIT, "置換", TEXT("EditReplace")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "--------------------", NULL},
	{ID_SELECT_DELETE_LIST        , 0x00000000, COMMAND_GROUP_EDIT, "STEのリストから削除"         , TEXT("DeleteList")},
	{ID_SELECT_DELETE_FILE        , 0x00000000, COMMAND_GROUP_EDIT, "ファイルの削除"              , TEXT("DeleteFile")},
	{ID_SELECT_EDIT_DESTORY       , 0x00000000, COMMAND_GROUP_EDIT, "変更前の状態に戻す"          , TEXT("EditModify")},
	{ID_FOLDER_TREE_SYNC          , 0x00000000, COMMAND_GROUP_EDIT, "フォルダ構成の同期"          , TEXT("FolderSync")},
	{ID_CHECK_FILE_SYNC           , 0x00000000, COMMAND_GROUP_EDIT, "フォルダ構成の同期(チェックのみ)", TEXT("CheckFileSync")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, "--------------------", NULL},
	{ID_DELETE_CHAR_SPACE         , 0x00000000, COMMAND_GROUP_EDIT, "先頭/末尾の空白文字を削除"   , TEXT("EditDeleteCharSpace")}, /* Rumble 192 */
	{ID_DELETE_CHAR               , 0x00000000, COMMAND_GROUP_EDIT, "先頭/末尾のｎ文字を削除"     , TEXT("EditDeleteChar")},

	// 表示
	{ID_ADJUST_COLUMN_WIDTH       , 0x00000000, COMMAND_GROUP_DISP, "全てのカラム幅を調整", TEXT("AdjustColumnWidth")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_DISP, "--------------------", NULL},
	{ID_SORT_LIST                 , 0x00000000, COMMAND_GROUP_DISP, "並び替え"        , TEXT("ListSort")},
	{ID_SORT_LIST_DIRECT          , 0x00000000, COMMAND_GROUP_DISP, "並び替えを再実行", TEXT("ListSortDirect")},
	{ID_EXEC_CLASSIFICATION       , 0x00000000, COMMAND_GROUP_DISP, "分類表示を更新"  , TEXT("ExecClassification")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_DISP, "--------------------", NULL},
	{ID_EDIT_FIELD_SI             , 0x00000000, COMMAND_GROUP_DISP, "SIフィールドを表示/編集"       , TEXT("EditFieldSI")},
	{ID_EDIT_TD3_TAG              , 0x00000000, COMMAND_GROUP_DISP, "ID3 tag を表示/編集"           , TEXT("EditID3tag")},
	{ID_EDIT_CHANGE_FIELD         , 0x00000000, COMMAND_GROUP_DISP, "[ID3 tag]<=>[SIフィールド]切替", TEXT("EditChangeField")},

	// プレイリスト
	{ID_LOAD_PLAYLIST             , 0x00000000, COMMAND_GROUP_PLAYLIST, "プレイリストの読み込み"        , TEXT("LoadPlayList")},
	{ID_WRITE_PLAYLIST            , 0x00000000, COMMAND_GROUP_PLAYLIST, "プレイリストの出力"            , TEXT("WritePlayList")},
	{ID_WRITE_TREE_PLAYLIST       , 0x00000000, COMMAND_GROUP_PLAYLIST, "ツリー分類のプレイリスト出力"  , TEXT("WriteTreePlayList")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, "--------------------", NULL},
	{ID_ALL_FILES_CHECK           , 0x00000000, COMMAND_GROUP_PLAYLIST, "全てのファイルをチェックする"  , TEXT("AllFilesCheck")},
	{ID_ALL_FILES_UNCHECK         , 0x00000000, COMMAND_GROUP_PLAYLIST, "全てのファイルのチェックを外す", TEXT("AllFilesUnCheck")},
	{ID_REVERSE_CHECK             , 0x00000000, COMMAND_GROUP_PLAYLIST, "チェック状態を反転する"        , TEXT("ReverseCheck")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, "--------------------", NULL},
	{ID_CHECK_FILES_SELECT        , 0x00000000, COMMAND_GROUP_PLAYLIST, "チェックファイルを選択"        , TEXT("CheckFilesSelect")},
	{ID_SELECT_FILES_CHECK        , 0x00000000, COMMAND_GROUP_PLAYLIST, "選択ファイルをチェック"        , TEXT("SelectFilesCheck")},
	{ID_SELECT_FILES_UNCHECK      , 0x00000000, COMMAND_GROUP_PLAYLIST, "選択ファイルのチェックを外す"  , TEXT("SelectFilesUnCheck")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, "--------------------", NULL},
	{ID_CHECK_WORD                , 0x00000000, COMMAND_GROUP_PLAYLIST, "条件を指定してチェック"        , TEXT("CheckWord")},
	{ID_CHECK_FILENAME_MAX        , 0x00000000, COMMAND_GROUP_PLAYLIST, "ファイル名の長さをチェック"    , TEXT("CheckFileNameMax")}, /* SeaKnows 037 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, "--------------------", NULL},
	{ID_SELECT_TREE_COLUM         , 0x00000000, COMMAND_GROUP_PLAYLIST, "ツリー配下のセル一括選択"    , TEXT("SelectTreeColumn")}, /* TyphoonSwell 025 */
	{ID_SELECT_TREE_FILE          , 0x00000000, COMMAND_GROUP_PLAYLIST, "ツリー配下のファイル一括選択"  , TEXT("SelectTreeFile")}, /* STEP 013 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, "--------------------", NULL},
	{ID_MOVE_TO_PARENT            , 0x00000000, COMMAND_GROUP_PLAYLIST, "カーソルを親ツリーへ移動"      , TEXT("MoveToParent")}, /* STEP 014 */
	{ID_MOVE_TO_PREVIOUS          , 0x00000000, COMMAND_GROUP_PLAYLIST, "カーソル位置を一つ上のツリーへ移動"      , TEXT("MoveToPrevious")}, /* STEP 014 */
	{ID_MOVE_TO_NEXT              , 0x00000000, COMMAND_GROUP_PLAYLIST, "カーソル位置を一つ下のツリーへ移動"      , TEXT("MoveToNext")}, /* STEP 014 */

	// WinAmp 制御処理
	{ID_WINAMP_PLAY               , 0x00000000, COMMAND_GROUP_PLAYER, "再生"                  , TEXT("WinampPlay")},
	{ID_WINAMP_STOP               , 0x00000000, COMMAND_GROUP_PLAYER, "停止(Winampのみ有効)"  , TEXT("WinampStop")},
	{ID_WINAMP_EXIT               , 0x00000000, COMMAND_GROUP_PLAYER, "終了"                  , TEXT("WinampExit")},
	{ID_WINAMP_PLAY_PREV          , 0x00000000, COMMAND_GROUP_PLAYER, "前の曲(Winampのみ有効)", TEXT("WinampPlayPrev")},
	{ID_WINAMP_PLAY_NEXT          , 0x00000000, COMMAND_GROUP_PLAYER, "次の曲(Winampのみ有効)", TEXT("WinampPlayNext")},

	// 変換処理
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- デフォルト書式変換 -----", NULL},
	{ID_CONV_FILENAME_TO_TRACKNAME, 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => トラック名", TEXT("ConvFileNameToTrackName")},
	{ID_CONV_TRACKNAME_TO_FILENAME, 0x00000000, COMMAND_GROUP_CONV, "トラック名 => ファイル名", TEXT("ConvTrackNameToFileName")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- ユーザー指定書式変換 -----", NULL},
	{ID_CONV_TAG2FILE_USER        , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名", TEXT("ConvUserTagToFile")},
	{ID_CONV_FILE2TAG_USER        , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報", TEXT("ConvUserFileToTag")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "------ 文字変換 -----", NULL},
	{ID_CONV_STR_HAN_ALL          , 0x00000000, COMMAND_GROUP_CONV, "全角=>半角(全て)変換"          , TEXT("ConvHanAll")},
	{ID_CONV_STR_HAN_KIGOU        , 0x00000000, COMMAND_GROUP_CONV, "全角=>半角(記号)変換"          , TEXT("ConvHanKigou")},
	{ID_CONV_STR_HAN_SUJI         , 0x00000000, COMMAND_GROUP_CONV, "全角=>半角(数字)変換"          , TEXT("ConvHanSuji")},
	{ID_CONV_STR_HAN_KATA         , 0x00000000, COMMAND_GROUP_CONV, "全角=>半角(カタカナ)変換"      , TEXT("ConvHanLata")},
	{ID_CONV_STR_HAN_ALPHA        , 0x00000000, COMMAND_GROUP_CONV, "全角=>半角(アルファベット)変換", TEXT("ConvHanAlpha")},
	{ID_CONV_STR_ZEN_ALL          , 0x00000000, COMMAND_GROUP_CONV, "半角=>全角(全て)変換"          , TEXT("ConvZenAll")},
	{ID_CONV_STR_ZEN_KIGOU        , 0x00000000, COMMAND_GROUP_CONV, "半角=>全角(記号)変換"          , TEXT("ConvZenKigou")},
	{ID_CONV_STR_ZEN_SUJI         , 0x00000000, COMMAND_GROUP_CONV, "半角=>全角(数字)変換"          , TEXT("ConvZenSuji")},
	{ID_CONV_STR_ZEN_KATA         , 0x00000000, COMMAND_GROUP_CONV, "半角=>全角(カタカナ)変換"      , TEXT("ConvZenKata")},
	{ID_CONV_STR_ZEN_ALPHA        , 0x00000000, COMMAND_GROUP_CONV, "半角=>全角(アルファベット)変換", TEXT("ConvZenAlpha")},
	{ID_CONV_STR_TO_UPPER         , 0x00000000, COMMAND_GROUP_CONV, "小文字=>大文字変換"            , TEXT("ConvToUpper")},
	{ID_CONV_STR_TO_LOWER         , 0x00000000, COMMAND_GROUP_CONV, "大文字=>小文字変換"            , TEXT("ConvToLower")},
	{ID_CONV_STR_FIRST_UPPER      , 0x00000000, COMMAND_GROUP_CONV, "単語の１文字目のみ大文字"      , TEXT("ConvFirstUpper")},
	{ID_CONV_STR_FIXED_UPPER_LOWER, 0x00000000, COMMAND_GROUP_CONV, "大文字小文字固定"              , TEXT("ConvFIxedUpLow")}, /* STEP 040 */
	{ID_CONV_STR_HIRA2KATA        , 0x00000000, COMMAND_GROUP_CONV, "ひらがな=>カタカナ変換"        , TEXT("ConvHira2Kata")},
	{ID_CONV_STR_KATA2HIRA        , 0x00000000, COMMAND_GROUP_CONV, "カタカナ=>ひらがな変換"        , TEXT("ConvKata2Hira")},
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- 拡張書式変換 -----", NULL},
	{ID_CONV_FORMAT_EX_01         , 0x00000000, COMMAND_GROUP_CONV, "書式１", TEXT("ConvFormatEx01")},
	{ID_CONV_FORMAT_EX_02         , 0x00000000, COMMAND_GROUP_CONV, "書式２", TEXT("ConvFormatEx02")},
	{ID_CONV_FORMAT_EX_03         , 0x00000000, COMMAND_GROUP_CONV, "書式３", TEXT("ConvFormatEx03")},
	{ID_CONV_FORMAT_EX_04         , 0x00000000, COMMAND_GROUP_CONV, "書式４", TEXT("ConvFormatEx04")},
	{ID_CONV_FORMAT_EX_05         , 0x00000000, COMMAND_GROUP_CONV, "書式５", TEXT("ConvFormatEx05")},
	{ID_CONV_FORMAT_EX_06         , 0x00000000, COMMAND_GROUP_CONV, "書式６", TEXT("ConvFormatEx06")},
	{ID_CONV_FORMAT_EX_07         , 0x00000000, COMMAND_GROUP_CONV, "書式７", TEXT("ConvFormatEx07")},
	{ID_CONV_FORMAT_EX_08         , 0x00000000, COMMAND_GROUP_CONV, "書式８", TEXT("ConvFormatEx08")},
	{ID_CONV_FORMAT_EX_09         , 0x00000000, COMMAND_GROUP_CONV, "書式９", TEXT("ConvFormatEx09")},
	{ID_CONV_FORMAT_EX_10         , 0x00000000, COMMAND_GROUP_CONV, "書式１０", TEXT("ConvFormatEx10")},
	{ID_CONV_EX_SETUP             , 0x00000000, COMMAND_GROUP_CONV, "拡張書式変換書式設定", TEXT("ConvUserSetup")}, /* STEP 009 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- ユーザー指定書式の切替 -----", NULL},
	{ID_CONV_FORMAT_USER_01       , 0x00000000, COMMAND_GROUP_CONV, "書式１", TEXT("ConvFormatUser01")},
	{ID_CONV_FORMAT_USER_02       , 0x00000000, COMMAND_GROUP_CONV, "書式２", TEXT("ConvFormatUser02")},
	{ID_CONV_FORMAT_USER_03       , 0x00000000, COMMAND_GROUP_CONV, "書式３", TEXT("ConvFormatUser03")},
	{ID_CONV_FORMAT_USER_04       , 0x00000000, COMMAND_GROUP_CONV, "書式４", TEXT("ConvFormatUser04")}, /* LastTrain 057 */
	{ID_CONV_FORMAT_USER_05       , 0x00000000, COMMAND_GROUP_CONV, "書式５", TEXT("ConvFormatUser05")}, /* LastTrain 057 */
	{ID_CONV_USER_SETUP           , 0x00000000, COMMAND_GROUP_CONV, "ユーザー指定書式変換書式設定", TEXT("ConvUserSetup")}, /* STEP 009 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- ユーザー指定書式 タグ情報 => ファイル名 -----", NULL},
	{ID_CONV_FORMAT_USER_T2F_01   , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名 書式１", TEXT("ConvFormatUserT2F01")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_T2F_02   , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名 書式２", TEXT("ConvFormatUserT2F02")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_T2F_03   , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名 書式３", TEXT("ConvFormatUserT2F03")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_T2F_04   , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名 書式４", TEXT("ConvFormatUserT2F04")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_T2F_05   , 0x00000000, COMMAND_GROUP_CONV, "タグ情報 => ファイル名 書式５", TEXT("ConvFormatUserT2F05")}, /* STEP 030 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- ユーザー指定書式 ファイル名 => タグ情報 -----", NULL},
	{ID_CONV_FORMAT_USER_F2T_01   , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報 書式１", TEXT("ConvFormatUserF2T01")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_F2T_02   , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報 書式２", TEXT("ConvFormatUserF2T02")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_F2T_03   , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報 書式３", TEXT("ConvFormatUserF2T03")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_F2T_04   , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報 書式４", TEXT("ConvFormatUserF2T04")}, /* STEP 030 */
	{ID_CONV_FORMAT_USER_F2T_05   , 0x00000000, COMMAND_GROUP_CONV, "ファイル名 => タグ情報 書式５", TEXT("ConvFormatUserF2T05")}, /* STEP 030 */
	{0x0000                       , 0x00000000, COMMAND_GROUP_CONV, "----- タグ情報変換 -----", NULL}, /* STEP 034 */
	{ID_CONV_TAG_TO_TAG_01        , 0x00000000, COMMAND_GROUP_CONV, "書式１", TEXT("ConvFormatTag2Tag01")}, /* STEP 034 */
	{ID_CONV_TAG_TO_TAG_02        , 0x00000000, COMMAND_GROUP_CONV, "書式２", TEXT("ConvFormatTag2Tag02")}, /* STEP 034 */
	{ID_CONV_TAG_TO_TAG_03        , 0x00000000, COMMAND_GROUP_CONV, "書式３", TEXT("ConvFormatTag2Tag03")}, /* STEP 034 */
	{ID_CONV_TAG_TO_TAG_04        , 0x00000000, COMMAND_GROUP_CONV, "書式４", TEXT("ConvFormatTag2Tag04")}, /* STEP 034 */
	{ID_CONV_TAG_TO_TAG_05        , 0x00000000, COMMAND_GROUP_CONV, "書式５", TEXT("ConvFormatTag2Tag05")}, /* STEP 034 */

	{0x0000, 0x00000000, -1, NULL, NULL},		// 終端コード
	/**
	 【注意】コマンドを追加した場合は、_APS_NEXT_COMMAND_VALUEが更新されるのでSTEP_api.cppをリコンパイルすること
	 **/
};

KEY_CONFIG *SearchKeyConfigID(WORD wCmdID)
{
	int		i;
	for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
		KEY_CONFIG	*pKey = &g_listKeyConfig[i];
		if (pKey->wCmdID == wCmdID) {
			return(pKey);
		}
	}
	extern CPlugin plugins;
	for (i=0;i<plugins.arPluginKey.GetSize();i++) {
		KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
		if (pKey->wCmdID == wCmdID) {
			return pKey;
		}
	}
	return(NULL);
}

static	char	*g_sKeyName[] = {
	"",				// 00
	"LBUTTON",		// 01	マウスの左ボタン
	"RBUTTON",		// 02	マウスの右ボタン
	"CANCEL",		// 03	コントロール ブレーク処理に使用
	"MBUTTON",		// 04	マウスの中央ボタン (3つボタンのマウス)
	"","","",		// 05～07	未定義
	"BS",			// 08	BackSpaceキー
	"Tab",			// 09	Tabキー
	"","",			// 0A､ 0B	未定義
	"CLEAR",		// 0C	Clearキー
	"Enter",		// 0D	Enterキー
	"","",			// 0E､ 0F	未定義
	"Shift",		// 10	Shiftキー
	"Ctrl",			// 11	Ctrlキー
	"Alt",			// 12	Altキー
	"PAUSE",		// 13	Pauseキー
	"CAPITAL",		// 14	Caps Lockキー
	"KANA",			// 15	英数カナキー
	"","","",		// 16～18	漢字システム用に予約
	"KANJI",		// 19	漢字システム用に予約
	"",				// 1A	未定義
	"ESC",			// 1B	Escキー
	"CONVERT",		// 1C	漢字システム用に予約
	"NOCONVERT",	// 1D	漢字システム用に予約
	"","",			// 1E､ 1F	漢字システム用に予約
	"Space",		// 20	Spaceキー
	"PageUp",		// 21	Page Upキー
	"PageDown",		// 22	Page Downキー
	"End",			// 23	Endキー
	"Home",			// 24	Homeキー
	"←",			// 25	←キー
	"↑",			// 26	↑キー
	"→",			// 27	→キー
	"↓",			// 28	↓キー
	"SELECT",		// 29	Selectキー
	"",				// 2A	OEM指定
	"EXECUTE",		// 2B	Executeキー
	"SNAPSHOT",		// 2C	Print Screenキー (Windows 3.0以降用)
	"Ins",			// 2D	Insキー
	"Del",			// 2E	Delキー
	"Help",			// 2F	Helpキー
	"0",			// 30	0キー
	"1",			// 31	1キー
	"2",			// 32	2キー
	"3",			// 33	3キー
	"4",			// 34	4キー
	"5",			// 35	5キー
	"6",			// 36	6キー
	"7",			// 37	7キー
	"8",			// 38	8キー
	"9",			// 39	9キー
	"","","","","","","",	// 3A～40	未定義
	"A",			// 41	Aキー
	"B",			// 42	Bキー
	"C",			// 43	Cキー
	"D",			// 44	Dキー
	"E",			// 45	Eキー
	"F",			// 46	Fキー
	"G",			// 47	Gキー
	"H",			// 48	Hキー
	"I",			// 49	Iキー
	"J",			// 4A	Jキー
	"K",			// 4B	Kキー
	"L",			// 4C	Lキー
	"M",			// 4D	Mキー
	"N",			// 4E	Nキー
	"O",			// 4F	Oキー
	"P",			// 50	Pキー
	"Q",			// 51	Qキー
	"R",			// 52	Rキー
	"S",			// 53	Sキー
	"T",			// 54	Tキー
	"U",			// 55	Uキー
	"V",			// 56	Vキー
	"W",			// 57	Wキー
	"X",			// 58	Xキー
	"Y",			// 59	Yキー
	"Z",			// 5A	Zキー
	"","","","","",	// 5B～5F	未定義
	"NUM0",			// 60	テンキーの0キー
	"NUM1",			// 61	テンキーの1キー
	"NUM2",			// 62	テンキーの2キー
	"NUM3",			// 63	テンキーの3キー
	"NUM4",			// 64	テンキーの4キー
	"NUM5",			// 65	テンキーの5キー
	"NUM6",			// 66	テンキーの6キー
	"NUM7",			// 67	テンキーの7キー
	"NUM8",			// 68	テンキーの8キー
	"NUM9",			// 69	テンキーの9キー
	"[*]",			// 6A	テンキーの*キー
	"[+]",			// 6B	テンキーの+キー
	"SEPARATOR",	// 6C	Separatorキー
	"[-]",			// 6D	テンキーの－キー
	"[.]",			// 6E	テンキーの.キー
	"[/]",			// 6F	テンキーの/キー
	"F1",			// 70	F1キー
	"F2",			// 71	F2キー
	"F3",			// 72	F3キー
	"F4",			// 73	F4キー
	"F5",			// 74	F5キー
	"F6",			// 75	F6キー
	"F7",			// 76	F7キー
	"F8",			// 77	F8キー
	"F9",			// 78	F9キー
	"F10",			// 79	F10キー
	"F11",			// 7A	F11キー
	"F12",			// 7B	F12キー
	"F13",			// 7C	F13キー
	"F14",			// 7D	F14キー
	"F15",			// 7E	F15キー
	"F16",			// 7F	F16キー
	"F17",			// 80H	F17キー
	"F18",			// 81H	F18キー
	"F19",			// 82H	F19キー
	"F20",			// 83H	F20キー
	"F21",			// 84H	F21キー
	"F22",			// 85H	F22キー
	"F23",			// 86H	F23キー
	"F24",			// 87H	F24キー
	"","","","","","","","",	// 88～8F	未定義
	"NUMLOCK",		// 90	Num Lockキー
	"SCROLL",		// 91	Scroll Lockキー
};

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp

BEGIN_MESSAGE_MAP(CSuperTagEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CSuperTagEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_01, OnUpdateFavoriteFolder01)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_02, OnUpdateFavoriteFolder02)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_03, OnUpdateFavoriteFolder03)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_04, OnUpdateFavoriteFolder04)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_05, OnUpdateFavoriteFolder05)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_06, OnUpdateFavoriteFolder06)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_07, OnUpdateFavoriteFolder07)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_08, OnUpdateFavoriteFolder08)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_09, OnUpdateFavoriteFolder09)
	ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_10, OnUpdateFavoriteFolder10)
	ON_COMMAND(ID_FAVORITE_FOLDER_01, OnFavoriteFolder01)
	ON_COMMAND(ID_FAVORITE_FOLDER_02, OnFavoriteFolder02)
	ON_COMMAND(ID_FAVORITE_FOLDER_03, OnFavoriteFolder03)
	ON_COMMAND(ID_FAVORITE_FOLDER_04, OnFavoriteFolder04)
	ON_COMMAND(ID_FAVORITE_FOLDER_05, OnFavoriteFolder05)
	ON_COMMAND(ID_FAVORITE_FOLDER_06, OnFavoriteFolder06)
	ON_COMMAND(ID_FAVORITE_FOLDER_07, OnFavoriteFolder07)
	ON_COMMAND(ID_FAVORITE_FOLDER_08, OnFavoriteFolder08)
	ON_COMMAND(ID_FAVORITE_FOLDER_09, OnFavoriteFolder09)
	ON_COMMAND(ID_FAVORITE_FOLDER_10, OnFavoriteFolder10)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp クラスの構築

CSuperTagEditorApp::CSuperTagEditorApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_hAccel = 0;
	m_accelTable = NULL;		// アクセラレータテーブル
	m_nAccelTable = 0;
}

CSuperTagEditorApp::~CSuperTagEditorApp()
{
	CString strINI = m_pszProfileName;
	SaveProfile();
	FreeProfile();
	m_pRecentFileList->WriteList();
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSuperTagEditorApp オブジェクト

CSuperTagEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp クラスの初期化
// =============================================
// CSuperTagEditorApp::MakeFileName
// 概要  : 自アプリケーションのパスからファイル名を
//       : フルパスで作成する
// 引数  : sExt			= ファイル拡張子
// 戻り値: TCHAR *		= ファイル名(NULL=失敗)
// =============================================
TCHAR *CSuperTagEditorApp::MakeFileName(TCHAR *sExt)
{
	TCHAR   drive[_MAX_DRIVE];
	TCHAR   dir[_MAX_DIR];
	TCHAR   fname[_MAX_FNAME];
	TCHAR   buff[_MAX_PATH] = {'\0'};

	//自己アプリのパス所得（大小文字識別付き）
	GetModuleFileName(m_hInstance, buff, _MAX_PATH);
	WIN32_FIND_DATA wfd;

	HANDLE  h = ::FindFirstFile(buff, &wfd);
	if (h == NULL) return(NULL);

	_tsplitpath(buff, drive, dir, NULL, NULL);
	_tmakepath(buff, drive, dir, wfd.cFileName, NULL);
	::FindClose(h);

	_tsplitpath(buff, drive, dir, fname, NULL);
	_tmakepath(buff, drive, dir, fname, sExt);
	return(_tcsdup(buff));
}


BOOL CSuperTagEditorApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	// 多重起動禁止処理 /* FreeFall 045 */

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	//SetRegistryKey(_T("MERCURY"));
	free((void*)m_pszProfileName);
	m_pszProfileName = MakeFileName(TEXT("ini"));
	{ /* STEP 031 */
		BOOL bFlag = FALSE;
		for (int i = 1; i < __argc; i++) {
			LPCTSTR pszParam = __targv[i];
			if (bFlag) {
				free((void*)m_pszProfileName);
				m_pszProfileName = _tcsdup(pszParam);
				break;
			}
			if (pszParam[0] == '-' || pszParam[0] == '/')
			{
				if (lstrcmp(pszParam, TEXT("-I")) == 0 || lstrcmp(pszParam, TEXT("/I")) == 0) {
					bFlag = TRUE;
				}
			}
		}
	}
	//if (MyGetProfileInt("STEx", "UseRegistry", 0) ? true : false) {
	//	SetRegistryKey(_T("MERCURY"));
	//}

	// ↑ここで設定した INI ファイル名称はデストラクタで free される
	CString strINI = m_pszProfileName;
	InitProfile();
	ReadRegistry();

	LoadStdProfileSettings(g_nRecentFolder);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)  /* StartInaction 053 */

	// 多重起動禁止処理 /* FreeFall 045 */
	if (!g_bValidDupExec) {
		const auto sMutexName = PROG_NAME;
		HANDLE hPrevMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, sMutexName);
		if(hPrevMutex){
	//		MessageBox(NULL, "既に "PROG_NAME" は起動中です", "多重起動エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			// 修正 by Kobarin
			// 起動済みの STE にファイル名・フォルダ名を渡す
			if(__argc >= 2){
				KbDDEClient ddeClient(NULL, TEXT("SuperTagEditor"), TEXT("SuperTagEditor"));
				int i;
				for(i = 1; i < __argc; i++){
					ddeClient.Execute(__argv[i],//ファイル名
									  ""        //スイッチ（なし）
									  );
				}
			}
			CloseHandle(hPrevMutex);
			//CloseHandle(hPrevMutex);
			return FALSE;
		}
		m_hMutex = CreateMutex(FALSE, 0, sMutexName);
	}

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSuperTagEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CSuperTagEditorView));
	AddDocTemplate(pDocTemplate);

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 最大化起動対応
	if (g_bMainFrameZoomed) {
		CWinApp::m_nCmdShow = SW_SHOWMAXIMIZED;
	} else if (g_bMainFrameIconic) {
		CWinApp::m_nCmdShow = SW_SHOWMINIMIZED;
	}

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// ProcessShellCommand()で扱われなかったものをここで処理
	{ /* Misirlou 140 */
		BOOL bFirst = TRUE;
		for (int i = 1; i < __argc; i++) {
			LPCTSTR pszParam = __targv[i];
			BOOL bFlag = FALSE;
			BOOL bLast = ((i + 1) == __argc);
			if (pszParam[0] == '-' || pszParam[0] == '/')
			{
				// remove flag specifier
				bFlag = TRUE;
				++pszParam;
			}
			if (!bFlag && !bFirst) {
				OpenDocumentFile(pszParam);
			}
			if (!bFlag)	bFirst = FALSE;
		}
	}

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	if (g_bMainFrameZoomed) {
//		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
//		m_pMainWnd->UpdateWindow();
	} else if (g_bMainFrameIconic) {
//		m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
//		m_pMainWnd->UpdateWindow();
	} else {
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
	}

	// ドラッグ/ドロップ オープンを許可します
//	m_pMainWnd->DragAcceptFiles();

    //追加 by Kobarin
    //KbSTE のフルパスファイル名をレジストリに保存
    //（KbMedia Player と連携するため）
    //WriteKbSTEPathToRegistry();

	return TRUE;
}

int CSuperTagEditorApp::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	ReleaseMutex(m_hMutex);

	// アクセラレータテーブルの解放
	DestroyAccelerator();

	delete [] g_genreListUSER;

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	// バージョン情報の設定
	//m_strVersion.Format(PROG_NAME" Ver "PROG_VERSION"(Build:%d) 改", BUILDCOUNT_NUM);
	m_strVersion.Format(PROG_NAME TEXT(" Version ") PROG_VERSION);
	m_strVersion.Append(TEXT("\nforked from ") PROG_NAME_ORG TEXT(" Version ") PROG_VERSION_ORG);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_ST_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CSuperTagEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp メッセージ ハンドラ
static const auto sSectionOption			= TEXT("Option");
static const auto sKeyVersion				= TEXT("Version");
static const auto sKeyCurrentDir			= TEXT("CurrentDir");
static const auto sKeyCurrentPlayList		= TEXT("CurrentPlayList");
static const auto sKeyEnterBeginEdit		= TEXT("EnterBeginEdit");
static const auto sKeyESCEditCancel			= TEXT("ESCEditCancel");
static const auto sKeyEnableEditCursorExit	= TEXT("EnableEditCursorExit");
static const auto sKeyEditOkDown			= TEXT("EditOkDown");
static const auto sKeyKeepTimeStamp			= TEXT("KeepTimeStamp");
static const auto sKeySyncCreateTime		= TEXT("SyncCreateTime");
static const auto sKeyChangeFileExt			= TEXT("ChangeFileExt");
static const auto sKeyLoadFileChecked		= TEXT("LoadFileChecked");
static const auto sKeyHideMP3ListFile		= TEXT("HideMP3ListFile");
static const auto sKeyDropSearchSubFolder	= TEXT("DropSearchSubFolder");	/* TyphoonSwell 026 */
static const auto sKeyShowZenSpace			= TEXT("ShowZenSpace");	/* BeachMonster 107 */
static const auto sKeyShowOtherChar			= TEXT("ShowOtherChar");	/* BeachMonster 107 */
static const auto sKeySortIgnoreCase		= TEXT("SortIgnoreCase");	/* BeachMonster4 114 */
static const auto sKeySortIgnoreZenHan		= TEXT("SortIgnoreZenHan");	/* BeachMonster4 114 */
static const auto sKeySortIgnoreKataHira	= TEXT("SortIgnoreKataHira");	/* FunnyCorn 179 */
static const auto sKeyShowTotalParent		= TEXT("ShowTotalParent");	/* RockDance 128 */
static const auto sKeyShowTips				= TEXT("ShowTips");	/* Rumble 188 */
static const auto sKeyChangeTextFile		= TEXT("ChangeTextFile");
static const auto sKeyEditFiledSIF			= TEXT("EditFiledSI");
static const auto sKeyAutoOpenFolder		= TEXT("AutoOpenFolder");
static const auto sKeyLoadFileAdjustColumn	= TEXT("LoadFileAdjustColumn");
static const auto sKeySetLyricsDir			= TEXT("SetLyricsDir");
static const auto sKeySearchLyricsSubDir	= TEXT("SearchLyricsSubDir");
static const auto sKeyEnableSearchSubDir	= TEXT("EnableSearchSubDir");
static const auto sKeyLyricsPath			= TEXT("LyricsPath");
static const auto sKeyCheckFileName			= TEXT("CheckFileName");
static const auto sKeyPlayerType			= TEXT("PlayerType");
static const auto sKeyWinAmpPath			= TEXT("WinAmpPath");

static const auto sKeyID3v2GenreAddNumber	= TEXT("ID3v2GenreAddNumber");
static const auto sKeyFileNameMaxCheck		= TEXT("FileNameMaxCheck");
static const auto sKeyFileNameMaxChar		= TEXT("FileNameMaxChar");
static const auto sKeyFileNameMaxCellColor	= TEXT("FileNameMaxCellColor");

static const auto sSectionFolderSync		= TEXT("FolderSync");
static const auto sKeyEnableFolderSync		= TEXT("EnableFolderSync");
static const auto sKeySyncRootFolder		= TEXT("RootFolder");
static const auto sKeySyncSelectAlways		= TEXT("SelectAlways");
static const auto sKeySyncDeleteFolder		= TEXT("DeleteFolder");
static const auto sKeySyncLyricsFileMove	= TEXT("LyricsFileMove");

static const auto sSectionConfMessage	= TEXT("ConfMessage");
static const auto sKeyConfConvMP3		= TEXT("ConvMP3");
static const auto sKeyConfConvRMP		= TEXT("ConvRMP");
static const auto sKeyConfConvID3v2		= TEXT("ConvID3v2");
static const auto sKeyConfDeleteFile	= TEXT("DeleteFile");
static const auto sKeyConfDeleteList	= TEXT("DeleteList");
static const auto sKeyConfEditModify	= TEXT("EditModify");
static const auto sKeyConfFolderSync	= TEXT("FolderSync");

static const auto sSectionLoadPlayList	= TEXT("LoadPlayList");
static const auto sKeyClearList			= TEXT("ClearList");
static const auto sKeyClearCheck		= TEXT("ClearCheck");
static const auto sKeyAddList			= TEXT("AddList");
static const auto sKeyFileCheck			= TEXT("FileCheck");

static const LPCTSTR sSectionCheckWord[CHECK_STATE_MAX]	= {TEXT("CheckWord"), TEXT("ReplaceWord")};
static const auto sKeySearchWord	= TEXT("SearchWord");
static const auto sKeyReplaceWord	= TEXT("ReplaceWord");
static const auto sKeyTargetColumn	= TEXT("TargetColumn");
static const auto sKeyCheckDiffUL	= TEXT("CheckDiffUL");
static const auto sKeyRegExp		= TEXT("RegExp");
static const auto sKeyRangeSelected	= TEXT("RangeSelected");
static const auto sKeyMatchComplete	= TEXT("MatchComplete");
static const auto sKeyMatchSelected	= TEXT("MatchSelected");

// ユーザー書式変換
static const auto sSectionUserConvFormat	= TEXT("UserConvFormat");
static const auto sKeyConvFormatType		= TEXT("ConvFormatType");
static const auto sKeyUserFormName			= TEXT("Name");
static const auto sKeyUserFormTag2File		= TEXT("TagToFile");
static const auto sKeyUserFormFile2Tag		= TEXT("FileToTag");

// 拡張版書式変換
static const auto sSectionConvFormatEx	= TEXT("ConvFormatEx");
//static const auto  sKeyUserFormName	= TEXT("Name");
static const auto sKeyUserFormat		= TEXT("Format");
static const auto sKeyUserFixString		= TEXT("FixString");
static const auto sKeyUserInitNumber	= TEXT("InitNumber");
static const auto sKeyUserAddNumber		= TEXT("AddNumber");
static const auto sKeyUserColumnCount	= TEXT("ColumnCount");
static const auto sKeyUserSpaceInit		= TEXT("SpaceInit");

// 移動先フォルダ書式
static const auto sSectionMoveFolder		= TEXT("MoveFolderFormat");
//static const auto sKeyUserFormName		= TEXT("Name");
static const auto sKeyMoveFolderFormat		= TEXT("Format");
static const auto sKeyMoveFolderFixString	= TEXT("FixString");
static const auto sKeyMoveFolderCopy		= TEXT("Copy");
static const auto sKeyMoveFolderInitFolder	= TEXT("InitFolder");

// 書式コピー /* FunnyCorn 175 */
static const auto sSectionCopyFormat		= TEXT("CopyFormatFormat");
//static const auto sKeyUserFormName		= TEXT("Name");
static const auto sKeyCopyFormatFormat		= TEXT("Format");
static const auto sKeyCopyFormatFixString	= TEXT("FixString");

// タグ情報変換 /* STEP 034 */
static const auto sSectionConvFormatTag2Tag	= TEXT("ConvFormatTag2Tag");
static const auto sKeyUserTag2TagFormName	= TEXT("Name");
static const auto sKeyUserTagTagFormat		= TEXT("Format");

// 定型文貼り付け /* SeaKnows 030 */
static const auto sSectionTeikei			= TEXT("Teikei");
static const auto sSectionTeikeiGroupName	= TEXT("TeikeiGroupName");
static const auto sSectionTeikeiPaste		= TEXT("TeikeiPaste");
static const auto sSectionTeikeiAddSpace	= TEXT("TeikeiAddSpace");
static const auto sSectionTeikeiAddChar		= TEXT("TeikeiAddChar");
static const auto sSectionTeikeiFront		= TEXT("TeikeiAddFront");
static const auto sSectionTeikeiBack		= TEXT("TeikeiAddBack");
static const auto sSectionTeikeiShowDialog	= TEXT("TeikeiShowDialog");

// フォルダ単一選択 /* SeaKnows 033 */
static const auto sSectionValidFolderSelect	= TEXT("ValidFolderSelect");
// 多重起動を許可する /* FreeFall 045 */
static const auto sSectionValidDupExec		= TEXT("ValidDupExec");
// 最近使ったフォルダの数
static const auto sSectionRecentFolderNum	= TEXT("RecentFolderNum");
// 検索・置換ダイアログの位置を記憶する
static const auto sSectionSaveRepDlgPos		= TEXT("SaveRepDlgPos");
static const auto sSectionSaveRepDlgPosX	= TEXT("SaveRepDlgPosX");
static const auto sSectionSaveRepDlgPosY	= TEXT("SaveRepDlgPosY");
// 下方向に連番を追加ダイアログの設定 /* Baja 159 */
static const auto sSectionAddNumberWidth	= TEXT("AddNumberWidth");
static const auto sSectionAddNumberPos		= TEXT("AddNumberPos");
static const auto sSectionAddNumberSep		= TEXT("AddNumberSep");
static const auto sSectionAddNumberBef		= TEXT("AddNumberBef"); /* Conspiracy 194 */
static const auto sSectionAddNumberAft		= TEXT("AddNumberAft"); /* Conspiracy 194 */
// Audio Listに表示されるフォルダ名などの分類をセルサイズを無視して(実際は隣３セル分まで)表示する
static const auto sSectionAudioListShow	= TEXT("AudioListShow"); /* Conspiracy 199 */

// フォント設定
static const auto sSectionFont				= TEXT("Font");
static const auto sKeyFontFace				= TEXT("Face");
static const auto sKeyFontHeight			= TEXT("Height");
static const auto sKeyFontWidth				= TEXT("Width");
static const auto sKeyFontEscapement		= TEXT("Escapement");
static const auto sKeyFontOrientation		= TEXT("Orientation");
static const auto sKeyFontWeight			= TEXT("Weight");
static const auto sKeyFontItalic			= TEXT("Italic");
static const auto sKeyFontUnderline			= TEXT("Underline");
static const auto sKeyFontStrikeOut			= TEXT("StrikeOut");
static const auto sKeyFontCharSet			= TEXT("CharSet");
static const auto sKeyFontOutPrecision		= TEXT("OutPrecision");
static const auto sKeyFontClipPrecision		= TEXT("ClipPrecision");
static const auto sKeyFontQuality			= TEXT("Quality");
static const auto sKeyFontPitchAndFamily	= TEXT("PitchAndFamily");

// リスト出力書式
static const auto sSectionWriteFormat	= TEXT("WriteFormat");
static const auto sKeyWriteFormName		= TEXT("Name");
static const auto sKeyWriteFileName		= TEXT("FileName");
static const auto sKeyWriteExtName		= TEXT("ExtName");
static const auto sKeyWriteSelected		= TEXT("WriteSelected");
static const auto sKeyWriteCurrentFile	= TEXT("CurrentFile");
static const auto sKeyWriteIsHtml		= TEXT("IsHtml");
static const auto sKeyWriteHtml			= TEXT("WriteHtml"); /* BeachMonster5 120 */

static const auto sSectionRepFileName	= TEXT("RepFileName");
static const auto sKeyRepCharAfter		= TEXT("RepCharAfter");
static const auto sKeyRepCharBefore		= TEXT("RepCharBefore");	/* FreeFall 050 */

static const auto sSectionSort		= TEXT("Sort");
static const auto sKeySortColumn	= TEXT("Column");
static const auto sKeySortType		= TEXT("Type");

static const auto sSectionClass		= TEXT("Class");
static const auto sKeyClassType		= TEXT("Type");
static const auto sKeyClassColumn	= TEXT("Column");

static const auto sSectionWindow	= TEXT("Window");
static const auto sKeyWinZoomed		= TEXT("Zoomed");
static const auto sKeyWinIconic		= TEXT("Iconic");
static const auto sKeyMainWindow	= TEXT("MainWindow");
static const auto sKeySplitSize		= TEXT("SplitSize");

static const auto sSectionKeyConfig	= TEXT("KeyConfig");

static const auto sSectionGenreList		= TEXT("GenreList");
static const auto sSectionUserGenreList	= TEXT("UserGenreList");
static const auto sKeyUserGenreAddList	= TEXT("AddList");
static const auto sKeyUserGenreNo		= TEXT("No");
static const auto sKeyUserGenreName		= TEXT("Name");

// 文字種統一 /* StartInaction 054 */
static const auto sKeyUnifyAlpha		= TEXT("UnifyAlpha");
static const auto sKeyUnifyHiraKata		= TEXT("UnifyHiraKata");
static const auto sKeyUnifyKata			= TEXT("UnifyKata");
static const auto sKeyUnifyKigou		= TEXT("UnifyKigou");
static const auto sKeyUnifySuji			= TEXT("UnifySuji");
static const auto sKeyUnifyUpLow		= TEXT("UnifyUpLow");
static const auto sKeyUnifyFixedUpLow	= TEXT("UnifyFixedUpLow"); /* STEP 040 */

// 拡張子統一 /* STE 007 */
static const auto sKeyFileExtChange = TEXT("FileExtChange");

// ファイル名文字種統一 /* LastTrain 058 */
static const auto sKeyFileUnifyAlpha	= TEXT("FileUnifyAlpha");
static const auto sKeyFileUnifyHiraKata	= TEXT("FileUnifyHiraKata");
static const auto sKeyFileUnifyKata		= TEXT("FileUnifyKata");
static const auto sKeyFileUnifyKigou	= TEXT("FileUnifyKigou");
static const auto sKeyFileUnifySuji		= TEXT("FileUnifySuji");
static const auto sKeyFileUnifyUpLow	= TEXT("FileUnifyUpLow");

// プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
static const auto sKeyShowLoadPlaylistDlg = TEXT("ShowLoadPlaylistDlg");

// お気に入りのフォルダ /* RockDance 129 */
static const auto sSectionFavorites			= TEXT("Favorites");

static const auto sKeyFirstUpperIgnoreWord			= TEXT("FistUpperIgnoreWord");
static const auto sKeyFirstUpperIgnoreWords			= TEXT("FistUpperIgnoreWords");
static const auto sKeyFirstUpperSentenceSeparator	= TEXT("FistUpperSentenceSeparator");
static const auto sKeyUserConvAddMenu				= TEXT("UserConvAddMenu");
static const auto sKeyZenHanKigouKana				= TEXT("ZenHanKigouKana");

void CSuperTagEditorApp::ReadWindowStatus(LPCTSTR sKey, RECT *rect)
{
	const auto sDefault = TEXT("0 0 0 0");
	CString	str;
	LPCTSTR	sBuffer;
	CString strINI = m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	str = MyGetProfileString(sSectionWindow, sKey, sDefault);
	sBuffer = str;
	_tscanf(
		sBuffer,
		TEXT("%d %d %d %d"),
		&rect->left,
		&rect->top,
		&rect->right,
		&rect->bottom);
	//Profile_Free();
}

void CSuperTagEditorApp::WriteWindowStatus(LPCTSTR sKey, RECT *rect)
{
	CString	str;
	str.Format(
		TEXT("%d %d %d %d"),
		rect->left,
		rect->top,
		rect->right,
		rect->bottom);
	CString strINI = m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	MyWriteProfileString(sSectionWindow, sKey, str);
	//Profile_Free();
}

void CSuperTagEditorApp::ReadRegistry(void)
{
	CString strINI = m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	int		i;

	// 各種設定を読み込む
	CString	strVersion;
	strVersion = MyGetProfileString(sSectionOption, sKeyVersion, TEXT(""));
	g_bIsVersionUp = lstrcmp(strVersion, PROG_VERSION) ? true : false;

	g_strCurrentDirectory	= MyGetProfileString(sSectionOption, sKeyCurrentDir, TEXT(""));
	g_strCurrentPlayList	= MyGetProfileString(sSectionOption, sKeyCurrentPlayList, TEXT("*.m3u"));

	// 一般 - ユーザーインターフェース
	g_bOptESCEditCancel			= MyGetProfileInt(sSectionOption, sKeyESCEditCancel, 0) ? true : false;
	g_bOptEnableEditCursorExit	= MyGetProfileInt(sSectionOption, sKeyEnableEditCursorExit, 1) ? true : false;
	g_bOptEditOkDown		= MyGetProfileInt(sSectionOption, sKeyEditOkDown, 1) ? true : false;
	g_bOptEnterBeginEdit	= MyGetProfileInt(sSectionOption, sKeyEnterBeginEdit, 0) ? true : false;

	// 一般 - 動作設定
	g_bOptKeepTimeStamp		= MyGetProfileInt(sSectionOption, sKeyKeepTimeStamp, 0) ? true : false;
	g_bOptSyncCreateTime	= MyGetProfileInt(sSectionOption, sKeySyncCreateTime, 0) ? true : false;
	g_bOptChangeFileExt		= MyGetProfileInt(sSectionOption, sKeyChangeFileExt, 0) ? true : false;
	g_bOptAutoOpenFolder	= MyGetProfileInt(sSectionOption, sKeyAutoOpenFolder, 0) ? true : false;
	g_bOptLoadFileAdjustColumn	= MyGetProfileInt(sSectionOption, sKeyLoadFileAdjustColumn, 1) ? true : false;
	g_bOptLoadFileChecked	= MyGetProfileInt(sSectionOption, sKeyLoadFileChecked, 1) ? true : false;
	g_bOptHideMP3ListFile	= MyGetProfileInt(sSectionOption, sKeyHideMP3ListFile, 0) ? true : false;

	// 一般 - 歌詞ファイル
	g_bOptChangeTextFile		= MyGetProfileInt(sSectionOption, sKeyChangeTextFile, 1) ? true : false;
	g_bOptSetLyricsDir			= MyGetProfileInt(sSectionOption, sKeySetLyricsDir, 0) ? true : false;
	g_bOptSearchLyricsSubDir	= MyGetProfileInt(sSectionOption, sKeySearchLyricsSubDir, 0) ? true : false;
	g_strOptLyricsPath			= MyGetProfileString(sSectionOption, sKeyLyricsPath, TEXT(""));

	// 
	g_bOptEditFieldSIF		= MyGetProfileInt(sSectionOption, sKeyEditFiledSIF, 1) ? true : false;
	g_nOptCheckFileName		= MyGetProfileInt(sSectionOption, sKeyCheckFileName, FILENAME_CONV_MULTIBYTE);
	g_bEnableSearchSubDir	= MyGetProfileInt(sSectionOption, sKeyEnableSearchSubDir, 1) ? true : false;

	// フォルダの同期
	g_bEnableFolderSync		= MyGetProfileInt(sSectionFolderSync, sKeyEnableFolderSync, 0) ? true : false;
	g_strRootFolder			= MyGetProfileString(sSectionFolderSync, sKeySyncRootFolder, TEXT(""));
	g_bSyncSelectAlways		= MyGetProfileInt(sSectionFolderSync, sKeySyncSelectAlways, 0) ? true : false;
	g_bSyncDeleteFolder		= MyGetProfileInt(sSectionFolderSync, sKeySyncDeleteFolder, 1) ? true : false;
	g_bSyncLyricsFileMove	= MyGetProfileInt(sSectionFolderSync, sKeySyncLyricsFileMove, 1) ? true : false;

	// 確認メッセージ表示
	g_bConfDeleteFile	= MyGetProfileInt(sSectionConfMessage, sKeyConfDeleteFile, 1) ? true : false;
	g_bConfDeleteList	= MyGetProfileInt(sSectionConfMessage, sKeyConfDeleteList, 1) ? true : false;
	g_bConfEditModify	= MyGetProfileInt(sSectionConfMessage, sKeyConfEditModify, 1) ? true : false;
	g_bConfFolderSync	= MyGetProfileInt(sSectionConfMessage, sKeyConfFolderSync, 1) ? true : false;

	// プレイリスト
	g_bPlayListClearList	= MyGetProfileInt(sSectionLoadPlayList, sKeyClearList, 0) ? true : false;
	g_bPlayListClearCheck	= MyGetProfileInt(sSectionLoadPlayList, sKeyClearCheck, 1) ? true : false;
	g_bPlayListAddList		= MyGetProfileInt(sSectionLoadPlayList, sKeyAddList, 0) ? true : false;
	g_bPlayListFileCheck	= MyGetProfileInt(sSectionLoadPlayList, sKeyFileCheck, 1) ? true : false;

	// 条件チェックの状態
	for (i = 0; i < CHECK_STATE_MAX; i++) {
		CHECK_WORD_STATE	*pState = &g_chkWord[i];
		const auto sSectionName = sSectionCheckWord[i];
		pState->strSearchWord	= MyGetProfileString(sSectionName, sKeySearchWord, TEXT(""));
		pState->strReplaceWord	= MyGetProfileString(sSectionName, sKeyReplaceWord, TEXT(""));
		pState->nTargetColumn	= MyGetProfileInt(sSectionName, sKeyTargetColumn, -1);
		pState->bCheckDiffUL	= MyGetProfileInt(sSectionName, sKeyCheckDiffUL, 0) ? true : false;
		pState->bRegExp			= MyGetProfileInt(sSectionName, sKeyRegExp, 0) ? true : false;
		pState->bRangeSelected	= MyGetProfileInt(sSectionName, sKeyRangeSelected, 0) ? true : false;
		pState->bMatchComplete	= MyGetProfileInt(sSectionName, sKeyMatchComplete, 0) ? true : false;
		pState->bMatchSelected	= MyGetProfileInt(sSectionName, sKeyMatchSelected, 0) ? true : false;
	}

	// ユーザー変換書式
	auto sDefFormatTag2File = TEXT("%ARTIST_NAME%-%ALBUM_NAME%-%TRACK_NAME%");
	auto sDefFormatFile2Tag = sDefFormatTag2File;
	//g_nUserConvFormatType	= MyGetProfileInt(sSectionUserConvFormat, sKeyConvFormatType, 0);
	{ /* 数を増やしたので本家併用時に影響を与えないように LastTrain 057 */
		CString		strSectionName;
		strSectionName = TEXT("haseta\\");
		strSectionName += sSectionUserConvFormat;
		g_nUserConvFormatType	= MyGetProfileInt(strSectionName, sKeyConvFormatType, 0);
	}
	for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
		CString		strKeyName;
		// 名称
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormName, i);
		g_userConvFormat[i].strName = MyGetProfileString(sSectionUserConvFormat, strKeyName, TEXT("名称未設定"));
		// タグ情報 => ファイル名
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormTag2File, i);
		g_userConvFormat[i].strTag2File = MyGetProfileString(sSectionUserConvFormat, strKeyName, sDefFormatTag2File);
		// ファイル名 => タグ情報
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormFile2Tag, i);
		g_userConvFormat[i].strFile2Tag = MyGetProfileString(sSectionUserConvFormat, strKeyName, sDefFormatFile2Tag);
		// ２つ目以降はデフォルト文字列はクリア
		sDefFormatTag2File = sDefFormatFile2Tag = TEXT("");
	}

	// 拡張版ユーザー変換書式
	for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("%s%d"), sSectionConvFormatEx, i);
		// 名称
		g_userConvFormatEx[i].strName = MyGetProfileString(strSectionName, sKeyUserFormName, TEXT("名称未設定"));
		// 書式
		g_userConvFormatEx[i].strFormat = MyGetProfileString(strSectionName, sKeyUserFormat, TEXT(""));
		// 固定文字列
		g_userConvFormatEx[i].strFixString = MyGetProfileString(strSectionName, sKeyUserFixString, TEXT(""));
		// 連番：初期値
		g_userConvFormatEx[i].nInitNumber = MyGetProfileInt(strSectionName, sKeyUserInitNumber, 1);
		// 連番：加算値
		g_userConvFormatEx[i].nAddNumber = MyGetProfileInt(strSectionName, sKeyUserAddNumber, 1);
		// 連番：桁数
		g_userConvFormatEx[i].nColumnCount = MyGetProfileInt(strSectionName, sKeyUserColumnCount, 1);
		// 書き込み不可のセルで連番クリア
		g_userConvFormatEx[i].bSpaceInitNumber = MyGetProfileInt(strSectionName, sKeyUserSpaceInit, 0) ? true : false;
	}

	// 移動先フォルダ書式
	for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionMoveFolder, i);
		// 名称
		g_userMoveFolder[i].strName = MyGetProfileString(strSectionName, sKeyUserFormName, TEXT("名称未設定"));
		// 書式
		g_userMoveFolder[i].strFormat = MyGetProfileString(strSectionName, sKeyMoveFolderFormat, TEXT(""));
		// 固定文字列
		g_userMoveFolder[i].strFixString = MyGetProfileString(strSectionName, sKeyMoveFolderFixString, TEXT(""));
		// コピー
		g_userMoveFolder[i].bCopy = MyGetProfileInt(strSectionName, sKeyMoveFolderCopy, 0) ? true : false;
		// 初期フォルダ
		g_userMoveFolder[i].strInitFolder = MyGetProfileString(strSectionName, sKeyMoveFolderInitFolder, TEXT("")); /* STEP 022 */
	}

	// 書式コピー /* FunnyCorn 175 */
	for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionCopyFormat, i);
		// 名称
		g_userCopyFormat[i].strName = MyGetProfileString(strSectionName, sKeyUserFormName, TEXT("名称未設定"));
		// 書式
		g_userCopyFormat[i].strFormat = MyGetProfileString(strSectionName, sKeyCopyFormatFormat, TEXT(""));
		// 固定文字列
		g_userCopyFormat[i].strFixString = MyGetProfileString(strSectionName, sKeyCopyFormatFixString, TEXT(""));
	}

	// タグ情報変換 /* STEP 034 */
	for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionConvFormatTag2Tag, i);
		// 名称
		g_userConvFormatTag2Tag[i].strName = MyGetProfileString(strSectionName, sKeyUserTag2TagFormName, TEXT("名称未設定"));
		// 書式
		g_userConvFormatTag2Tag[i].strFormat = MyGetProfileString(strSectionName, sKeyUserTagTagFormat, TEXT(""));
	}

	{
		CString		sSectionOption;
		sSectionOption = TEXT("haseta");
		//g_bOptID3v2GenreAddNumber	= MyGetProfileInt(sSectionOption, sKeyID3v2GenreAddNumber, 1) ? true : false;
		g_bConfFileNameMaxCheck	= MyGetProfileInt(sSectionOption, sKeyFileNameMaxCheck, 0) ? true : false;
		g_bFileNameMaxCellColor	= MyGetProfileInt(sSectionOption, sKeyFileNameMaxCellColor, 0) ? true : false; /* SeaKnows 036 */
		g_nConfFileNameMaxChar	= MyGetProfileInt(sSectionOption, sKeyFileNameMaxChar, 255);
		g_bOptDropSearchSubFolder	= MyGetProfileInt(sSectionOption, sKeyDropSearchSubFolder, 0) ? true : false;	/* TyphoonSwell 026 */
		g_bOptShowZenSpace	= MyGetProfileInt(sSectionOption, sKeyShowZenSpace, 1) ? true : false;	/* BeachMonster 107 */
		g_sOptShowOtherChar = MyGetProfileString(sSectionOption, sKeyShowOtherChar, TEXT(""));
		g_bOptSortIgnoreCase	= MyGetProfileInt(sSectionOption, sKeySortIgnoreCase, 0) ? true : false;	/* BeachMonster4 114 */
		g_bOptSortIgnoreZenHan	= MyGetProfileInt(sSectionOption, sKeySortIgnoreZenHan, 0) ? true : false;	/* BeachMonster4 114 */
		g_bOptSortIgnoreKataHira= MyGetProfileInt(sSectionOption, sKeySortIgnoreKataHira, 0) ? true : false;	/* FunnyCorn 179 */
		g_bOptShowTotalParent	= MyGetProfileInt(sSectionOption, sKeyShowTotalParent, 0) ? true : false;	/* RockDance 128 */
		g_bOptShowTips	= MyGetProfileInt(sSectionOption, sKeyShowTips, 1) ? true : false;	/* Rumble 188 */
	}
	// 定型文貼り付け /* SeaKnows 030 *//* FreeFall 046 */
	for (int k=0;k<3;k++) {
		CString		sSectionOption;
		CString		strSectionName;
		sSectionOption = "haseta";
		strSectionName.Format(TEXT("%s%d"), sSectionTeikeiGroupName, k);
		g_strTeikeiGroupName[k] = MyGetProfileString(sSectionOption, strSectionName, TEXT("名称未設定"));
		for (i = 0; i < 10; i++) {
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikei, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikei, k, i);
			}
			g_teikeiInfo[i+k*10].strTeikei/* STEP 035 */ = MyGetProfileString(sSectionOption, strSectionName, TEXT(""));
			/* STEP 035 */
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiPaste, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiPaste, k, i);
			}
			g_teikeiInfo[i+k*10].nTeikeiPaste = MyGetProfileInt(sSectionOption, strSectionName, 0);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiAddSpace, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiAddSpace, k, i);
			}
			g_teikeiInfo[i+k*10].bAddSpace = MyGetProfileInt(sSectionOption, strSectionName, 0) ? true : false;
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiAddChar, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiAddChar, k, i);
			}
			g_teikeiInfo[i+k*10].bAddChar = MyGetProfileInt(sSectionOption, strSectionName, 0) ? true : false;
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiFront, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiFront, k, i);
			}
			g_teikeiInfo[i+k*10].strFront = MyGetProfileString(sSectionOption, strSectionName, TEXT(""));
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiBack, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiBack, k, i);
			}
			g_teikeiInfo[i+k*10].strBack = MyGetProfileString(sSectionOption, strSectionName, TEXT(""));
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiShowDialog, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiShowDialog, k, i);
			}
			g_teikeiInfo[i+k*10].bShowDialog = MyGetProfileInt(sSectionOption, strSectionName, 1) ? true : false;
		}
	}
	// ソフトウェア他 /* SeaKnows 031 */
	{
		CString		strSectionName;
		strSectionName = TEXT("haseta");
		g_bValidFolderSelect = MyGetProfileInt(strSectionName, sSectionValidFolderSelect, 0) ? true : false;
		g_bValidDupExec = MyGetProfileInt(strSectionName, sSectionValidDupExec, 0) ? true : false; /* FreeFall 045 */
		g_nRecentFolder = MyGetProfileInt(strSectionName, sSectionRecentFolderNum, 5); /* MyWriteProfileInt 053 */
		g_bSaveRepDlgPos = MyGetProfileInt(strSectionName, sSectionSaveRepDlgPos, 0) ? true : false; /* WildCherry4 086 */
		g_nSaveRepDlgPosX = MyGetProfileInt(strSectionName, sSectionSaveRepDlgPosX, -1); /* WildCherry4 086 */
		g_nSaveRepDlgPosY = MyGetProfileInt(strSectionName, sSectionSaveRepDlgPosY, -1); /* WildCherry4 086 */
		g_nAddNumberWidth = MyGetProfileInt(strSectionName, sSectionAddNumberWidth, 2); /* Baja 159 */
		g_nAddNumberPos = MyGetProfileInt(strSectionName, sSectionAddNumberPos, 0); /* Baja 159 */
		g_strAddNumberSep = MyGetProfileString(strSectionName, sSectionAddNumberSep, TEXT("")); /* Baja 159 */
		g_strAddNumberBef = MyGetProfileString(strSectionName, sSectionAddNumberBef, TEXT("")); /* Conspiracy 194 */
		g_strAddNumberAft = MyGetProfileString(strSectionName, sSectionAddNumberAft, TEXT("")); /* Conspiracy 194 */
		g_bAudioListShow = MyGetProfileInt(strSectionName, sSectionAudioListShow, 0) ? true : false; /* Conspiracy 199 */
	}

	// リスト出力書式
	for (i = 0; i < WRITE_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("%s%d"), sSectionWriteFormat, i);
		// 名称
		g_writeFormat[i].strName = MyGetProfileString(strSectionName, sKeyWriteFormName, TEXT("名称未設定"));
		// 書式ファイル名
		g_writeFormat[i].strFileName = MyGetProfileString(strSectionName, sKeyWriteFileName, TEXT(""));
		// 拡張子
		g_writeFormat[i].strExtName = MyGetProfileString(strSectionName, sKeyWriteExtName, TEXT(".txt"));
		// 選択ファイルのみ出力
		g_writeFormat[i].bWriteSelected = MyGetProfileInt(strSectionName, sKeyWriteSelected, 0) ? true : false;
		// カレントファイル名
		g_writeFormat[i].strCurrentFile = MyGetProfileString(strSectionName, sKeyWriteCurrentFile, TEXT(""));
		// HTML ファイル出力用(空の場合に全角スペースを出力)
		g_writeFormat[i].bIsHtml = MyGetProfileInt(strSectionName, sKeyWriteIsHtml, 0) ? true : false;
		{ /* BeachMonster5 120 */
			g_writeFormat[i].bWriteHtml = MyGetProfileInt("haseta\\" + strSectionName, sKeyWriteHtml, 0) ? true : false;
		}
	}

	// ファイル名置換文字
	for (i = 0; i < FILENAME_REPLACE_MAX; i++) {
		FILENAME_REPLACE	*pRep = &g_fileNameReplace[i];
		CString		strKeyName;
		// 置換後文字列
		strKeyName.Format(TEXT("%s%d"), sKeyRepCharAfter, i);
		pRep->strBefore = g_sRepTable[i][0];
		pRep->strAfter = MyGetProfileString(sSectionRepFileName, strKeyName, g_sRepTable[i][1]);
	}
	// ユーザファイル名置換文字 /* FreeFall 050 */
	{
		CString		strSectionName;
		strSectionName = TEXT("haseta\\User");
		strSectionName += sSectionRepFileName;
		for (i = 0; i < USER_FILENAME_REPLACE_MAX; i++) {
			FILENAME_REPLACE	*pRep = &g_userFileNameReplace[i];
			CString		strKeyName;
			// 置換後文字列
			strKeyName.Format(TEXT("%s%d"), sKeyRepCharBefore, i);
			pRep->strBefore = MyGetProfileString(strSectionName, strKeyName, TEXT(""));
			strKeyName.Format(TEXT("%s%d"), sKeyRepCharAfter, i);
			pRep->strAfter = MyGetProfileString(strSectionName, strKeyName, TEXT(""));
		}
	}

	// 分類設定情報
	g_classInfo.nType = MyGetProfileInt(sSectionClass, sKeyClassType, 0);
	for (i = 0; i < CLASS_MAX; i++) {
		CString		strKeyName;
		// カラム番号
		strKeyName.Format(TEXT("%s%d"), sKeyClassColumn, i);
		g_classInfo.nColumn[i] = MyGetProfileInt(sSectionClass, strKeyName, -1);
	}

	// ソート情報
	for (i = 0; i < SORT_KEY_MAX; i++) {
		CString		strKeyName;
		SORT_STATE	*state = &g_sortState[i];
		// キー(カラム)
		strKeyName.Format(TEXT("%s%d"), sKeySortColumn, i);
		state->nKeyColumn = MyGetProfileInt(sSectionSort, strKeyName, -1);
		// タイプ
		strKeyName.Format(TEXT("%s%d"), sKeySortType, i);
		state->nType = MyGetProfileInt(sSectionSort, strKeyName, 0);
	}

	// ウィンドウのフォントを読み込む
	CString	strFont;
	strFont = MyGetProfileString(sSectionFont, sKeyFontFace, TEXT(""));
	lstrcpy(g_fontReport.lfFaceName, strFont);
	g_fontReport.lfHeight			= MyGetProfileInt(sSectionFont, sKeyFontHeight, 0);
	g_fontReport.lfWidth			= MyGetProfileInt(sSectionFont, sKeyFontWidth, 0);
	g_fontReport.lfEscapement		= MyGetProfileInt(sSectionFont, sKeyFontEscapement, 0);
	g_fontReport.lfOrientation		= MyGetProfileInt(sSectionFont, sKeyFontOrientation, 0);
	g_fontReport.lfWeight			= MyGetProfileInt(sSectionFont, sKeyFontWeight, 0);
	g_fontReport.lfItalic			= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontItalic, 0);
	g_fontReport.lfUnderline		= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontUnderline, 0);
	g_fontReport.lfStrikeOut		= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontStrikeOut, 0);
	g_fontReport.lfCharSet			= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontCharSet, 0);
	g_fontReport.lfOutPrecision		= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontOutPrecision, 0);
	g_fontReport.lfClipPrecision	= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontClipPrecision, 0);
	g_fontReport.lfQuality			= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontQuality, 0);
	g_fontReport.lfPitchAndFamily	= (unsigned char)MyGetProfileInt(sSectionFont, sKeyFontPitchAndFamily, 0);

	//g_nOptPlayerType		= MyGetProfileInt(sSectionOption, sKeyPlayerType, PLAYER_WINAMP);
	//g_sOptWinAmpPath		= MyGetProfileString(sSectionOption, sKeyWinAmpPath, TEXT(""));
	{ /* 数を増やしたので本家併用時に影響を与えないように WildCherry 070 */
		CString		strSectionName;
		strSectionName = "haseta\\";
		strSectionName += sSectionOption;
		g_nOptPlayerType	= MyGetProfileInt(strSectionName, sKeyPlayerType, PLAYER_WINAMP);
		g_sOptWinAmpPath		= MyGetProfileString(strSectionName, sKeyWinAmpPath, TEXT(""));
	}
	if (g_sOptWinAmpPath.IsEmpty()) {
		// WinAmp のパスを取得
		GetWinampPath();
	}

	g_bMainFrameZoomed		= MyGetProfileInt(sSectionWindow, sKeyWinZoomed, FALSE);
	g_bMainFrameIconic		= MyGetProfileInt(sSectionWindow, sKeyWinIconic, FALSE);
	ReadWindowStatus(sKeyMainWindow, &g_rectMainWindow);

	// ユーザ指定ジャンル
	if (g_genreListUSER == NULL) {
		g_genreListUSER = new USER_GENRE_LIST[USER_GENRE_LIST_MAX];
	}

	// 文字種の統一 /* StartInaction 054 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		g_nUnifyAlpha = MyGetProfileInt(strSectionName, sKeyUnifyAlpha, 0);
		g_nUnifyHiraKata = MyGetProfileInt(strSectionName, sKeyUnifyHiraKata, 0);
		g_nUnifyKata = MyGetProfileInt(strSectionName, sKeyUnifyKata, 0);
		g_nUnifyKigou = MyGetProfileInt(strSectionName, sKeyUnifyKigou, 0);
		g_nUnifySuji = MyGetProfileInt(strSectionName, sKeyUnifySuji, 0);
		g_nUnifyUpLow = MyGetProfileInt(strSectionName, sKeyUnifyUpLow, 0);
		g_nUnifyFixedUpLow = MyGetProfileInt(strSectionName, sKeyUnifyFixedUpLow, 0); /* STEP 040 */
	}

	// ファイル名文字種の統一 /* LastTrain 058 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		g_nFileUnifyAlpha = MyGetProfileInt(strSectionName, sKeyFileUnifyAlpha, 0);
		g_nFileUnifyHiraKata = MyGetProfileInt(strSectionName, sKeyFileUnifyHiraKata, 0);
		g_nFileUnifyKata = MyGetProfileInt(strSectionName, sKeyFileUnifyKata, 0);
		g_nFileUnifyKigou = MyGetProfileInt(strSectionName, sKeyFileUnifyKigou, 0);
		g_nFileUnifySuji = MyGetProfileInt(strSectionName, sKeyFileUnifySuji, 0);
		g_nFileUnifyUpLow = MyGetProfileInt(strSectionName, sKeyFileUnifyUpLow, 0);
	}

	// 拡張子の統一 /* STEP 006 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		g_nFileExtChange = MyGetProfileInt(strSectionName, sKeyFileExtChange, 0);
	}

	// プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		g_bShowLoadPlaylistDlg = MyGetProfileInt(strSectionName, sKeyShowLoadPlaylistDlg, 1) ? true : false;
	}

	// お気に入りのフォルダ /* RockDance 129 */
	{
		CString		sSectionOption;
		CString		strSectionName;
		sSectionOption = "haseta";
		for (i = 0; i < 10; i++) {
			strSectionName.Format(TEXT("%s%d"), sSectionFavorites, i);
			g_strFavorite[i] = MyGetProfileString(sSectionOption, strSectionName, TEXT(""));
		}
	}

	{ /* STEP 026 */
		CString		strSectionName;
		strSectionName = "haseta";
		g_bFirstUpperIgnoreWord = MyGetProfileInt(strSectionName, sKeyFirstUpperIgnoreWord, 0) ? true : false;
		g_strFirstUpperIgnoreWords = MyGetProfileString(sSectionOption, sKeyFirstUpperIgnoreWords, TEXT("a,an,and,at,by,for,in,into,of,on,or,the,to,with"));
		g_strFirstUpperSentenceSeparator = MyGetProfileString(sSectionOption, sKeyFirstUpperSentenceSeparator, TEXT("."));
		g_bUserConvAddMenu = MyGetProfileInt(strSectionName, sKeyUserConvAddMenu, 0) ? true : false;
		g_bZenHanKigouKana = MyGetProfileInt(strSectionName, sKeyZenHanKigouKana, 0) ? true : false;
	}

	//Profile_Free();
}

void CSuperTagEditorApp::WriteRegistry(void)
{
	CString strINI = m_pszProfileName;
	InitProfile();


	int		i;

	// 各種設定を書き込む

	MyWriteProfileString(sSectionOption, sKeyVersion        , CString(PROG_VERSION) + CString("M"));
	MyWriteProfileString(sSectionOption, sKeyCurrentDir     , g_strCurrentDirectory);
	MyWriteProfileString(sSectionOption, sKeyCurrentPlayList, g_strCurrentPlayList);
	//dlgLoadProgress.SetPos(5);

	// 一般 - ユーザーインターフェース
	MyWriteProfileInt(sSectionOption, sKeyESCEditCancel       , g_bOptESCEditCancel ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyEnableEditCursorExit, g_bOptEnableEditCursorExit ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyEditOkDown          , g_bOptEditOkDown ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyEnterBeginEdit      , g_bOptEnterBeginEdit ? 1 : 0);
	//dlgLoadProgress.SetPos(10);
	
	// 一般 - 動作設定
	MyWriteProfileInt(sSectionOption, sKeyKeepTimeStamp       , g_bOptKeepTimeStamp ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeySyncCreateTime      , g_bOptSyncCreateTime ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyChangeFileExt       , g_bOptChangeFileExt ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyAutoOpenFolder      , g_bOptAutoOpenFolder ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyLoadFileAdjustColumn, g_bOptLoadFileAdjustColumn ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyLoadFileChecked     , g_bOptLoadFileChecked ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyHideMP3ListFile     , g_bOptHideMP3ListFile ? 1 : 0);
	//dlgLoadProgress.SetPos(15);

	// 一般 - 歌詞ファイル
	MyWriteProfileInt(sSectionOption, sKeyChangeTextFile    , g_bOptChangeTextFile ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeySetLyricsDir      , g_bOptSetLyricsDir ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeySearchLyricsSubDir, g_bOptSearchLyricsSubDir ? 1 : 0);
	MyWriteProfileString(sSectionOption, sKeyLyricsPath     , g_strOptLyricsPath);
	//dlgLoadProgress.SetPos(20);

	// 
	MyWriteProfileInt(sSectionOption, sKeyEditFiledSIF, g_bOptEditFieldSIF ? 1 : 0);
	MyWriteProfileInt(sSectionOption, sKeyCheckFileName, g_nOptCheckFileName);
	MyWriteProfileInt(sSectionOption, sKeyEnableSearchSubDir, g_bEnableSearchSubDir ? 1 : 0);
	//dlgLoadProgress.SetPos(25);

	// フォルダの同期
	MyWriteProfileInt(sSectionFolderSync, sKeyEnableFolderSync, g_bEnableFolderSync ? 1 : 0);
	MyWriteProfileString(sSectionFolderSync, sKeySyncRootFolder, g_strRootFolder);
	MyWriteProfileInt(sSectionFolderSync, sKeySyncSelectAlways, g_bSyncSelectAlways ? 1 : 0);
	MyWriteProfileInt(sSectionFolderSync, sKeySyncDeleteFolder, g_bSyncDeleteFolder ? 1 : 0);
	MyWriteProfileInt(sSectionFolderSync, sKeySyncLyricsFileMove, g_bSyncLyricsFileMove ? 1 : 0);
	//dlgLoadProgress.SetPos(30);

	// 確認メッセージ表示
	MyWriteProfileInt(sSectionConfMessage, sKeyConfDeleteFile, g_bConfDeleteFile ? 1 : 0);
	MyWriteProfileInt(sSectionConfMessage, sKeyConfDeleteList, g_bConfDeleteList ? 1 : 0);
	MyWriteProfileInt(sSectionConfMessage, sKeyConfEditModify, g_bConfEditModify ? 1 : 0);
	MyWriteProfileInt(sSectionConfMessage, sKeyConfFolderSync, g_bConfFolderSync ? 1 : 0);
	//dlgLoadProgress.SetPos(35);

	// プレイリスト
	MyWriteProfileInt(sSectionLoadPlayList, sKeyClearList , g_bPlayListClearList ? 1 : 0);
	MyWriteProfileInt(sSectionLoadPlayList, sKeyClearCheck, g_bPlayListClearCheck ? 1 : 0);
	MyWriteProfileInt(sSectionLoadPlayList, sKeyAddList   , g_bPlayListAddList ? 1 : 0);
	MyWriteProfileInt(sSectionLoadPlayList, sKeyFileCheck , g_bPlayListFileCheck ? 1 : 0);
	//dlgLoadProgress.SetPos(40);

	// 条件チェックの状態
	for (i = 0; i < CHECK_STATE_MAX; i++) {
		CHECK_WORD_STATE	*pState = &g_chkWord[i];
		const auto  sSectionName = sSectionCheckWord[i];
		MyWriteProfileString(sSectionName, sKeySearchWord, pState->strSearchWord);
		MyWriteProfileString(sSectionName, sKeyReplaceWord, pState->strReplaceWord);
		MyWriteProfileInt(sSectionName, sKeyTargetColumn , pState->nTargetColumn);
		MyWriteProfileInt(sSectionName, sKeyCheckDiffUL  , pState->bCheckDiffUL ? 1 : 0);
		MyWriteProfileInt(sSectionName, sKeyRegExp       , pState->bRegExp ? 1 : 0);
		MyWriteProfileInt(sSectionName, sKeyRangeSelected, pState->bRangeSelected ? 1 : 0);
		MyWriteProfileInt(sSectionName, sKeyMatchComplete, pState->bMatchComplete ? 1 : 0);
		MyWriteProfileInt(sSectionName, sKeyMatchSelected, pState->bMatchSelected ? 1 : 0);
	}
	//dlgLoadProgress.SetPos(45);

	// ファイル名置換文字
	for (i = 0; i < FILENAME_REPLACE_MAX; i++) {
		FILENAME_REPLACE	*pRep = &g_fileNameReplace[i];
		CString		strKeyName;
		// 置換後文字列
		strKeyName.Format(TEXT("%s%d"), sKeyRepCharAfter, i);
		MyWriteProfileString(sSectionRepFileName, strKeyName, pRep->strAfter);
	}
	//dlgLoadProgress.SetPos(50);
	// ユーザファイル名置換文字 /* FreeFall 050 */
	{
		CString		strSectionName;
		strSectionName = "haseta\\User";
		strSectionName += sSectionRepFileName;
		for (i = 0; i < USER_FILENAME_REPLACE_MAX; i++) {
			FILENAME_REPLACE	*pRep = &g_userFileNameReplace[i];
			CString		strKeyName;
			// 置換後文字列
			strKeyName.Format(TEXT("%s%d"), sKeyRepCharBefore, i);
			MyWriteProfileString(strSectionName, strKeyName, pRep->strBefore);
			strKeyName.Format(TEXT("%s%d"), sKeyRepCharAfter, i);
			MyWriteProfileString(strSectionName, strKeyName, pRep->strAfter);
		}
	}
	//dlgLoadProgress.SetPos(55);

	// ユーザー変換書式
	//MyWriteProfileInt(sSectionUserConvFormat, sKeyConvFormatType, g_nUserConvFormatType);
	{ /* 数を増やしたので本家併用時に影響を与えないように LastTrain 057 */
		CString		strSectionName;
		strSectionName = "haseta\\";
		strSectionName += sSectionUserConvFormat;
		MyWriteProfileInt(strSectionName, sKeyConvFormatType, g_nUserConvFormatType);
	}
	//dlgLoadProgress.SetPos(60);
	for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
		CString		strKeyName;
		// 名称
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormName, i);
		MyWriteProfileString(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strName);
		// タグ情報 => ファイル名
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormTag2File, i);
		MyWriteProfileString(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strTag2File);
		// ファイル名 => タグ情報
		strKeyName.Format(TEXT("%s%d"), sKeyUserFormFile2Tag, i);
		MyWriteProfileString(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strFile2Tag);
	}
	//dlgLoadProgress.SetPos(65);

	// 拡張版ユーザー変換書式
	for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("%s%d"), sSectionConvFormatEx, i);
		// 名称
		MyWriteProfileString(strSectionName, sKeyUserFormName, g_userConvFormatEx[i].strName);
		// 書式
		MyWriteProfileString(strSectionName, sKeyUserFormat, g_userConvFormatEx[i].strFormat);
		// 固定文字列
		MyWriteProfileString(strSectionName, sKeyUserFixString, g_userConvFormatEx[i].strFixString);
		// 連番：初期値
		MyWriteProfileInt(strSectionName, sKeyUserInitNumber, g_userConvFormatEx[i].nInitNumber);
		// 連番：加算値
		MyWriteProfileInt(strSectionName, sKeyUserAddNumber, g_userConvFormatEx[i].nAddNumber);
		// 連番：桁数
		MyWriteProfileInt(strSectionName, sKeyUserColumnCount, g_userConvFormatEx[i].nColumnCount);
		// 書き込み不可のセルで連番クリア
		MyWriteProfileInt(strSectionName, sKeyUserSpaceInit, g_userConvFormatEx[i].bSpaceInitNumber ? 1 : 0);
	}
	//dlgLoadProgress.SetPos(70);

	// 移動先フォルダ書式
	for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionMoveFolder, i);
		// 名称
		MyWriteProfileString(strSectionName, sKeyUserFormName, g_userMoveFolder[i].strName);
		// 書式
		MyWriteProfileString(strSectionName, sKeyMoveFolderFormat, g_userMoveFolder[i].strFormat);
		// 固定文字列
		MyWriteProfileString(strSectionName, sKeyMoveFolderFixString, g_userMoveFolder[i].strFixString);
		// コピー
		MyWriteProfileInt(strSectionName, sKeyMoveFolderCopy, g_userMoveFolder[i].bCopy ? 1 : 0);
		// 書式
		MyWriteProfileString(strSectionName, sKeyMoveFolderInitFolder, g_userMoveFolder[i].strInitFolder); /* STEP 022 */
	}

	// タグ情報変換 /* STEP 034 */
	for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionConvFormatTag2Tag, i);
		// 名称
		MyWriteProfileString(strSectionName, sKeyUserTag2TagFormName, g_userConvFormatTag2Tag[i].strName);
		// 書式
		MyWriteProfileString(strSectionName, sKeyUserTagTagFormat, g_userConvFormatTag2Tag[i].strFormat);
	}

	//dlgLoadProgress.SetPos(75);
	// 書式コピー /* FunnyCorn 175 */
	for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s%d"), sSectionCopyFormat, i);
		// 名称
		MyWriteProfileString(strSectionName, sKeyUserFormName, g_userCopyFormat[i].strName);
		// 書式
		MyWriteProfileString(strSectionName, sKeyCopyFormatFormat, g_userCopyFormat[i].strFormat);
		// 固定文字列
		MyWriteProfileString(strSectionName, sKeyCopyFormatFixString, g_userCopyFormat[i].strFixString);
	}
	//dlgLoadProgress.SetPos(80);
	{
		CString		sSectionOption;
		sSectionOption = TEXT("haseta");
//		MyWriteProfileInt(sSectionOption, sKeyID3v2GenreAddNumber, g_bOptID3v2GenreAddNumber ? 1 : 0);
		MyWriteProfileInt(sSectionOption, sKeyFileNameMaxCheck, g_bConfFileNameMaxCheck ? 1 : 0);
		MyWriteProfileInt(sSectionOption, sKeyFileNameMaxCellColor, g_bFileNameMaxCellColor ? 1 : 0);
		MyWriteProfileInt(sSectionOption, sKeyFileNameMaxChar, g_nConfFileNameMaxChar);
		MyWriteProfileInt(sSectionOption, sKeyDropSearchSubFolder, g_bOptDropSearchSubFolder ? 1 : 0); /* TyphoonSwell 026 */
		MyWriteProfileInt(sSectionOption, sKeyShowZenSpace, g_bOptShowZenSpace ? 1 : 0); /* BeachMonster 107 */
		//MyWriteProfileInt(sSectionOption, sKeyShowZenSpace, g_bOptShowZenSpace ? 1 : 0); /* BeachMonster 107 */
		MyWriteProfileInt(sSectionOption, sKeySortIgnoreCase, g_bOptSortIgnoreCase ? 1 : 0); /* BeachMonster4 114 */
		MyWriteProfileInt(sSectionOption, sKeySortIgnoreZenHan, g_bOptSortIgnoreZenHan ? 1 : 0); /* BeachMonster4 114 */
		MyWriteProfileInt(sSectionOption, sKeySortIgnoreKataHira, g_bOptSortIgnoreKataHira ? 1 : 0); /* FunnyCorn 179 */
		MyWriteProfileInt(sSectionOption, sKeyShowTotalParent, g_bOptShowTotalParent ? 1 : 0); /* RockDance 128 */
		MyWriteProfileInt(sSectionOption, sKeyShowTips, g_bOptShowTips ? 1 : 0); /* Rumble 188 */
	}
	//dlgLoadProgress.SetPos(85);
	// 定型文貼り付け /* SeaKnows 030 */
	for (int k=0;k<3;k++) {
		CString		sSectionOption;
		CString		strSectionName;
		sSectionOption = "haseta";
		strSectionName.Format(TEXT("%s%d"), sSectionTeikeiGroupName, k);
		MyWriteProfileString(sSectionOption, strSectionName, g_strTeikeiGroupName[k]);
		for (i = 0; i < 10; i++) {
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikei, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikei, k, i);
			}
			MyWriteProfileString(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strTeikei/* STEP 035 */);
			/* STEP 035 */
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiPaste, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiPaste, k, i);
			}
			MyWriteProfileInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].nTeikeiPaste);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiAddSpace, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiAddSpace, k, i);
			}
			MyWriteProfileInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bAddSpace ? 1 : 0);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiAddChar, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiAddChar, k, i);
			}
			MyWriteProfileInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bAddChar ? 1 : 0);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiFront, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiFront, k, i);
			}
			MyWriteProfileString(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strFront);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiBack, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiBack, k, i);
			}
			MyWriteProfileString(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strBack);
			if (k == 0) {
				strSectionName.Format(TEXT("%s%d"), sSectionTeikeiShowDialog, i);
			} else {
				strSectionName.Format(TEXT("%s%d-%d"), sSectionTeikeiShowDialog, k, i);
			}
			MyWriteProfileInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bShowDialog ? 1 : 0);
		}
	}

	// その他 /* SeaKnows 031,033 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sSectionValidFolderSelect, g_bValidFolderSelect ? 1 : 0);
		MyWriteProfileInt(strSectionName, sSectionValidDupExec, g_bValidDupExec ? 1 : 0); /* FreeFall 045 */
		MyWriteProfileInt(strSectionName, sSectionRecentFolderNum, g_nRecentFolder); /* MyWriteProfileInt 053 */
		MyWriteProfileInt(strSectionName, sSectionSaveRepDlgPos, g_bSaveRepDlgPos ? 1 : 0); /* WildCherry4 086 */
		MyWriteProfileInt(strSectionName, sSectionSaveRepDlgPosX, g_nSaveRepDlgPosX); /* WildCherry4 086 */
		MyWriteProfileInt(strSectionName, sSectionSaveRepDlgPosY, g_nSaveRepDlgPosY); /* WildCherry4 086 */
		MyWriteProfileInt(strSectionName, sSectionAddNumberWidth, g_nAddNumberWidth); /* Baja 159 */
		MyWriteProfileInt(strSectionName, sSectionAddNumberPos, g_nAddNumberPos); /* Baja 159 */
		MyWriteProfileString(strSectionName, sSectionAddNumberSep, g_strAddNumberSep); /* Baja 159 */
		MyWriteProfileString(strSectionName, sSectionAddNumberBef, g_strAddNumberBef); /* Conspiracy 194 */
		MyWriteProfileString(strSectionName, sSectionAddNumberAft, g_strAddNumberAft); /* Conspiracy 194 */
		MyWriteProfileInt(strSectionName, sSectionAudioListShow, g_bAudioListShow ? 1 : 0); /* Conspiracy 199 */
	}

	// リスト出力書式
	for (i = 0; i < WRITE_FORMAT_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("%s%d"), sSectionWriteFormat, i);
		// 名称
		MyWriteProfileString(strSectionName, sKeyWriteFormName, g_writeFormat[i].strName);
		// 書式ファイル名
		MyWriteProfileString(strSectionName, sKeyWriteFileName, g_writeFormat[i].strFileName);
		// 拡張子
		MyWriteProfileString(strSectionName, sKeyWriteExtName, g_writeFormat[i].strExtName);
		// 選択ファイルのみ出力
		MyWriteProfileInt(strSectionName, sKeyWriteSelected, g_writeFormat[i].bWriteSelected ? 1 : 0);
		// カレントファイル名
		MyWriteProfileString(strSectionName, sKeyWriteCurrentFile, g_writeFormat[i].strCurrentFile);
		// HTML ファイル出力用(空の場合に全角スペースを出力)
		MyWriteProfileInt(strSectionName, sKeyWriteIsHtml, g_writeFormat[i].bIsHtml ? 1 : 0);
		{ /* BeachMonster5 120 */
			MyWriteProfileInt("haseta\\" + strSectionName, sKeyWriteHtml, g_writeFormat[i].bWriteHtml ? 1 : 0);
		}
	}
	//dlgLoadProgress.SetPos(90);

	// 分類設定情報
	MyWriteProfileInt(sSectionClass, sKeyClassType, g_classInfo.nType);
	for (i = 0; i < CLASS_MAX; i++) {
		CString		strKeyName;
		// カラム番号
		strKeyName.Format(TEXT("%s%d"), sKeyClassColumn, i);
		MyWriteProfileInt(sSectionClass, strKeyName, g_classInfo.nColumn[i]);
	}

	// ソート情報
	for (i = 0; i < SORT_KEY_MAX; i++) {
		CString		strKeyName;
		SORT_STATE	*state = &g_sortState[i];
		// キー(カラム)
		strKeyName.Format(TEXT("%s%d"), sKeySortColumn, i);
		MyWriteProfileInt(sSectionSort, strKeyName, state->nKeyColumn);
		// タイプ
		strKeyName.Format(TEXT("%s%d"), sKeySortType, i);
		MyWriteProfileInt(sSectionSort, strKeyName, state->nType);
	}

	// ウィンドウのフォントを書き込む
	MyWriteProfileString(sSectionFont, sKeyFontFace, g_fontReport.lfFaceName);
	MyWriteProfileInt(sSectionFont, sKeyFontHeight		, g_fontReport.lfHeight);
	MyWriteProfileInt(sSectionFont, sKeyFontWidth 		, g_fontReport.lfWidth);
	MyWriteProfileInt(sSectionFont, sKeyFontEscapement	, g_fontReport.lfEscapement);
	MyWriteProfileInt(sSectionFont, sKeyFontOrientation	, g_fontReport.lfOrientation);
	MyWriteProfileInt(sSectionFont, sKeyFontWeight		, g_fontReport.lfWeight);
	MyWriteProfileInt(sSectionFont, sKeyFontItalic		, g_fontReport.lfItalic);
	MyWriteProfileInt(sSectionFont, sKeyFontUnderline 	, g_fontReport.lfUnderline);
	MyWriteProfileInt(sSectionFont, sKeyFontStrikeOut 	, g_fontReport.lfStrikeOut);
	MyWriteProfileInt(sSectionFont, sKeyFontCharSet		, g_fontReport.lfCharSet);
	MyWriteProfileInt(sSectionFont, sKeyFontOutPrecision	, g_fontReport.lfOutPrecision);
	MyWriteProfileInt(sSectionFont, sKeyFontClipPrecision , g_fontReport.lfClipPrecision);
	MyWriteProfileInt(sSectionFont, sKeyFontQuality		, g_fontReport.lfQuality);
	MyWriteProfileInt(sSectionFont, sKeyFontPitchAndFamily, g_fontReport.lfPitchAndFamily);

	//MyWriteProfileInt(sSectionOption, sKeyPlayerType, (int)g_nOptPlayerType);
	//MyWriteProfileString(sSectionOption, sKeyWinAmpPath, g_sOptWinAmpPath);
	{ /* 数を増やしたので本家併用時に影響を与えないように WildCherry 070 */
		CString		strSectionName;
		strSectionName = "haseta\\";
		strSectionName += sSectionOption;
		MyWriteProfileInt(strSectionName, sKeyPlayerType, (int)g_nOptPlayerType);
		MyWriteProfileString(strSectionName, sKeyWinAmpPath, g_sOptWinAmpPath);
	}
	//dlgLoadProgress.SetPos(95);

	// ウィンドウの状態を保存
	MyWriteProfileInt(sSectionWindow, sKeyWinZoomed, (int)g_bMainFrameZoomed);
	MyWriteProfileInt(sSectionWindow, sKeyWinIconic, (int)g_bMainFrameIconic);
	WriteWindowStatus(sKeyMainWindow, &g_rectMainWindow);

	// 文字種の統一 /* StartInaction 054 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sKeyUnifyAlpha, g_nUnifyAlpha);
		MyWriteProfileInt(strSectionName, sKeyUnifyHiraKata, g_nUnifyHiraKata);
		MyWriteProfileInt(strSectionName, sKeyUnifyKata, g_nUnifyKata);
		MyWriteProfileInt(strSectionName, sKeyUnifyKigou, g_nUnifyKigou);
		MyWriteProfileInt(strSectionName, sKeyUnifySuji, g_nUnifySuji);
		MyWriteProfileInt(strSectionName, sKeyUnifyUpLow, g_nUnifyUpLow);
		MyWriteProfileInt(strSectionName, sKeyUnifyFixedUpLow, g_nUnifyFixedUpLow); /* STEP 040 */
	}

	// ファイル名文字種の統一 /* LastTrain 058 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sKeyFileUnifyAlpha, g_nFileUnifyAlpha);
		MyWriteProfileInt(strSectionName, sKeyFileUnifyHiraKata, g_nFileUnifyHiraKata);
		MyWriteProfileInt(strSectionName, sKeyFileUnifyKata, g_nFileUnifyKata);
		MyWriteProfileInt(strSectionName, sKeyFileUnifyKigou, g_nFileUnifyKigou);
		MyWriteProfileInt(strSectionName, sKeyFileUnifySuji, g_nFileUnifySuji);
		MyWriteProfileInt(strSectionName, sKeyFileUnifyUpLow, g_nFileUnifyUpLow);
	}

	// 拡張子の統一 /* STEP 006 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sKeyFileExtChange, g_nFileExtChange);
	}
	// プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
	{
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sKeyShowLoadPlaylistDlg, g_bShowLoadPlaylistDlg ? 1 : 0);
	}
	//dlgLoadProgress.SetPos(100);
	// お気に入りのフォルダ /* RockDance 129 */
	{
		CString		sSectionOption;
		CString		strSectionName;
		sSectionOption = "haseta";
		for (i = 0; i < 10; i++) {
			strSectionName.Format(TEXT("%s%d"), sSectionFavorites, i);
			MyWriteProfileString(sSectionOption, strSectionName, g_strFavorite[i]);
		}
	}

	{ /* STEP 026 */
		CString		strSectionName;
		strSectionName = "haseta";
		MyWriteProfileInt(strSectionName, sKeyFirstUpperIgnoreWord, g_bFirstUpperIgnoreWord ? 1 : 0);
		MyWriteProfileString(sSectionOption, sKeyFirstUpperIgnoreWords, g_strFirstUpperIgnoreWords);
		MyWriteProfileString(sSectionOption, sKeyFirstUpperSentenceSeparator, g_strFirstUpperSentenceSeparator);
		MyWriteProfileInt(strSectionName, sKeyUserConvAddMenu, g_bUserConvAddMenu ? 1 : 0);
		MyWriteProfileInt(strSectionName, sKeyZenHanKigouKana, g_bZenHanKigouKana ? 1 : 0);
	}

	/*
	if (dlgLoadProgress.GetSafeHwnd() != NULL) {
		dlgLoadProgress.DestroyWindow();
		dlgLoadProgress.SetCanceled(FALSE);
	}
	*/
	SaveProfile();
	FreeProfile();
//	m_pRecentFileList->WriteList();
}

// キー割り当ての読み込み
void CSuperTagEditorApp::ReadKeyConfig(bool bUpdate)
{
	CString strINI = m_pszProfileName;
	InitProfile();
	int		i;
	int		nTableMax = 0;
	for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
		KEY_CONFIG	*pKey = &g_listKeyConfig[i];
		if (pKey->wCmdID != 0x0000) {
			pKey->dwKeyCode = (DWORD)MyGetProfileInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
			//pKey->dwKeyCode = (DWORD)Profile_GetInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
			if (pKey->dwKeyCode != 0) nTableMax++;
		}
	}

	extern CPlugin plugins;
	for (i=0;i<plugins.arPluginKey.GetSize();i++) {
		KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
		if (pKey->wCmdID != 0x0000) {
			pKey->dwKeyCode = (DWORD)MyGetProfileInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
			//pKey->dwKeyCode = (DWORD)Profile_GetInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
			if (pKey->dwKeyCode != 0) nTableMax++;
		}
	}
	FreeProfile();

	// アクセラレータキーテーブルの更新
	if (bUpdate) UpdateAccelerator(nTableMax);
}

// キー割り当ての保存
void CSuperTagEditorApp::WriteKeyConfig(bool bUpdate)
{
	CString strINI = m_pszProfileName;
	InitProfile();
	int		i;
	int		nTableMax = 0;
	//CString sValue;
	for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
		KEY_CONFIG	*pKey = &g_listKeyConfig[i];
		if (pKey->wCmdID != 0x0000) {
			//sValue.Format("%d", pKey->dwKeyCode);
			//WritePrivateProfileString(sSectionKeyConfig, pKey->sRegName, sValue, strINI);
			//Profile_WriteInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
			MyWriteProfileInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
			if (pKey->dwKeyCode != 0) nTableMax++;
		}
	}

	extern CPlugin plugins;
	for (i=0;i<plugins.arPluginKey.GetSize();i++) {
		KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
		if (pKey->wCmdID != 0x0000) {
			//sValue.Format("%ld", pKey->dwKeyCode);
			//WritePrivateProfileString(sSectionKeyConfig, pKey->sRegName, sValue, strINI);
			//Profile_WriteInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
			MyWriteProfileInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
			if (pKey->dwKeyCode != 0) nTableMax++;
		}
	}

	SaveProfile();
	FreeProfile();

	// アクセラレータキーテーブルの更新
	if (bUpdate) UpdateAccelerator(nTableMax);
}

// アクセラレータテーブルの解放
void CSuperTagEditorApp::DestroyAccelerator(void)
{
	if (m_hAccel != 0) {
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = 0;
	}
	if (m_accelTable != NULL) {
		delete[]	m_accelTable;		// アクセラレータテーブル
		m_accelTable = NULL;
		m_nAccelTable = 0;
	}
}

// アクセラレータテーブルの更新
CMenu convSubMenuT2F;
CMenu convSubMenuF2T;
CString convSubMenuT2FTitle;
CString convSubMenuF2TTitle;
void CSuperTagEditorApp::UpdateAccelerator(int nTableMax)
{
	if (nTableMax == -1) {
		nTableMax = m_nAccelTable;
	}

	// アクセラレータテーブルの解放
	DestroyAccelerator();

	CMainFrame/* STEP 030 */	*pMainWnd = (CMainFrame*)/* STEP 030 */AfxGetMainWnd();
	CMenu	*pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;

	/* STEP 030 */
	pMenu->DestroyMenu();
	CMenu newMenu;
	newMenu.LoadMenu(IDR_MAINFRAME);
	pMainWnd->SetMenu(NULL);
	pMainWnd->SetMenu(&newMenu);
	pMainWnd->m_hMenuDefault = newMenu.m_hMenu;
	newMenu.Detach();

	/* STEP 030 */
	convSubMenuT2F.DestroyMenu();
	convSubMenuF2T.DestroyMenu();
	pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
	if (pMenu != NULL && g_bUserConvAddMenu) {
		CMenu* pUConvMenu = NULL;
		int nCount = pMenu->GetMenuItemCount();
		int j; for (int j=0;j<nCount;j++) {
			CString strTitle;
			pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "変換(&C)") {
				pMenu = pMenu->GetSubMenu(j);
				break;
			}
		}
		nCount = pMenu->GetMenuItemCount();
		for (j=0;j<nCount;j++) {
			CString strTitle;
			pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "ユーザー指定変換(&U)") {
				pUConvMenu = pMenu->GetSubMenu(j);
				break;
			}
		}
		convSubMenuT2F.LoadMenu(IDR_MENU_CONV_FORMAT_USER_T2F);
		pUConvMenu->GetMenuString(ID_CONV_TAG2FILE_USER, convSubMenuT2FTitle, MF_BYCOMMAND);
		pUConvMenu->ModifyMenu(ID_CONV_TAG2FILE_USER, MF_BYCOMMAND | MF_POPUP, (UINT)convSubMenuT2F.GetSubMenu(0)->GetSafeHmenu(), convSubMenuT2FTitle);
		//pUConvMenu->ModifyMenu(ID_CONV_TAG2FILE_USER, MF_BYCOMMAND | MF_STRING, ID_CONV_TAG2FILE_USER, convSubMenuT2FTitle);

		convSubMenuF2T.LoadMenu(IDR_MENU_CONV_FORMAT_USER_F2T);
		pUConvMenu->GetMenuString(ID_CONV_FILE2TAG_USER, convSubMenuF2TTitle, MF_BYCOMMAND);
		pUConvMenu->ModifyMenu(ID_CONV_FILE2TAG_USER, MF_BYCOMMAND | MF_POPUP, (UINT)convSubMenuF2T.GetSubMenu(0)->GetSafeHmenu(), convSubMenuF2TTitle);
		//pUConvMenu->ModifyMenu(ID_CONV_FILE2TAG_USER, MF_BYCOMMAND | MF_STRING, ID_CONV_FILE2TAG_USER, convSubMenuF2TTitle);
	}
	extern void OnLoadMainMenu();
	OnLoadMainMenu();

	pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
	// アクセラレータテーブル作成
	m_accelTable = new ACCEL[nTableMax];
	m_nAccelTable = nTableMax;
	int		i, nCount = 0;
	for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
		KEY_CONFIG	*pKey = &g_listKeyConfig[i];
		if (pKey->wCmdID != 0x0000) {
			// アクセラレータの作成
			ACCEL	*pAccel = NULL;
			WORD	wModifiers = 0x0000;
			if (pKey->dwKeyCode != 0) {
				wModifiers = HIWORD(pKey->dwKeyCode);
				pAccel = &m_accelTable[nCount];
				pAccel->fVirt = FNOINVERT | FVIRTKEY;
				if (wModifiers & HOTKEYF_ALT    ) pAccel->fVirt |= FALT;
				if (wModifiers & HOTKEYF_CONTROL) pAccel->fVirt |= FCONTROL;
				if (wModifiers & HOTKEYF_SHIFT  ) pAccel->fVirt |= FSHIFT;
				pAccel->key = LOWORD(pKey->dwKeyCode);
				pAccel->cmd = pKey->wCmdID;
				nCount++;
			}

			// メニュー項目の書き換え
			if (pMenu != NULL) {
				CString	strName;
				// メニュー項目名を取得
				switch(pKey->wCmdID) {
				case ID_CONV_FORMAT_EX_01:
					strName.Format(TEXT("01：%s"), g_userConvFormatEx[0].strName);
					break;
				case ID_CONV_FORMAT_EX_02:
					strName.Format(TEXT("02：%s"), g_userConvFormatEx[1].strName);
					break;
				case ID_CONV_FORMAT_EX_03:
					strName.Format(TEXT("03：%s"), g_userConvFormatEx[2].strName);
					break;
				case ID_CONV_FORMAT_EX_04:
					strName.Format(TEXT("04：%s"), g_userConvFormatEx[3].strName);
					break;
				case ID_CONV_FORMAT_EX_05:
					strName.Format(TEXT("05：%s"), g_userConvFormatEx[4].strName);
					break;
				case ID_CONV_FORMAT_EX_06:
					strName.Format(TEXT("06：%s"), g_userConvFormatEx[5].strName);
					break;
				case ID_CONV_FORMAT_EX_07:
					strName.Format(TEXT("07：%s"), g_userConvFormatEx[6].strName);
					break;
				case ID_CONV_FORMAT_EX_08:
					strName.Format(TEXT("08：%s"), g_userConvFormatEx[7].strName);
					break;
				case ID_CONV_FORMAT_EX_09:
					strName.Format(TEXT("09：%s"), g_userConvFormatEx[8].strName);
					break;
				case ID_CONV_FORMAT_EX_10:
					strName.Format(TEXT("10：%s"), g_userConvFormatEx[9].strName);
					break;
				case ID_WRITE_LIST1:
					strName.Format(TEXT("01：%s"), g_writeFormat[0].strName);
					break;
				case ID_WRITE_LIST2:
					strName.Format(TEXT("02：%s"), g_writeFormat[1].strName);
					break;
				case ID_WRITE_LIST3:
					strName.Format(TEXT("03：%s"), g_writeFormat[2].strName);
					break;
				case ID_WRITE_LIST4:
					strName.Format(TEXT("04：%s"), g_writeFormat[3].strName);
					break;
				case ID_WRITE_LIST5:
					strName.Format(TEXT("05：%s"), g_writeFormat[4].strName);
					break;
				case ID_MOVE_FOLDER_01:
					strName.Format(TEXT("01：%s"), g_userMoveFolder[0].strName);
					break;
				case ID_MOVE_FOLDER_02:
					strName.Format(TEXT("02：%s"), g_userMoveFolder[1].strName);
					break;
				case ID_MOVE_FOLDER_03:
					strName.Format(TEXT("03：%s"), g_userMoveFolder[2].strName);
					break;
				case ID_MOVE_FOLDER_04:
					strName.Format(TEXT("04：%s"), g_userMoveFolder[3].strName);
					break;
				case ID_MOVE_FOLDER_05:
					strName.Format(TEXT("05：%s"), g_userMoveFolder[4].strName);
					break;
				case ID_CONV_FORMAT_USER_01: /* TyphoonSwell 027 */
				case ID_CONV_FORMAT_USER_T2F_01: /* STEP 030 */
				case ID_CONV_FORMAT_USER_F2T_01: /* STEP 030 */
					strName.Format(TEXT("01：%s"), g_userConvFormat[0].strName);
					break;
				case ID_CONV_FORMAT_USER_02: /* TyphoonSwell 027 */
				case ID_CONV_FORMAT_USER_T2F_02: /* STEP 030 */
				case ID_CONV_FORMAT_USER_F2T_02: /* STEP 030 */
					strName.Format(TEXT("02：%s"), g_userConvFormat[1].strName);
					break;
				case ID_CONV_FORMAT_USER_03: /* TyphoonSwell 027 */
				case ID_CONV_FORMAT_USER_T2F_03: /* STEP 030 */
				case ID_CONV_FORMAT_USER_F2T_03: /* STEP 030 */
					strName.Format(TEXT("03：%s"), g_userConvFormat[2].strName);
					break;
				case ID_CONV_FORMAT_USER_04: /* LastTrain 057 */
				case ID_CONV_FORMAT_USER_T2F_04: /* STEP 030 */
				case ID_CONV_FORMAT_USER_F2T_04: /* STEP 030 */
					strName.Format(TEXT("04：%s"), g_userConvFormat[3].strName);
					break;
				case ID_CONV_FORMAT_USER_05: /* LastTrain 057 */
				case ID_CONV_FORMAT_USER_T2F_05: /* STEP 030 */
				case ID_CONV_FORMAT_USER_F2T_05: /* STEP 030 */
					strName.Format(TEXT("05：%s"), g_userConvFormat[4].strName);
					break;
				case ID_TEIKEI_01: /* SeaKnows 030 */
					strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[0].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_02: /* SeaKnows 030 */
					strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[1].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_03: /* SeaKnows 030 */
					strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[2].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_04: /* SeaKnows 030 */
					strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[3].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_05: /* SeaKnows 030 */
					strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[4].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_06: /* SeaKnows 030 */
					strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[5].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_07: /* SeaKnows 030 */
					strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[6].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_08: /* SeaKnows 030 */
					strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[7].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_09: /* SeaKnows 030 */
					strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[8].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_10: /* SeaKnows 030 */
					strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[9].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_01: /* FreeFall 046 */
					strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[10].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_02: /* FreeFall 046 */
					strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[11].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_03: /* FreeFall 046 */
					strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[12].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_04: /* FreeFall 046 */
					strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[13].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_05: /* FreeFall 046 */
					strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[14].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_06: /* FreeFall 046 */
					strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[15].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_07: /* FreeFall 046 */
					strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[16].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_08: /* FreeFall 046 */
					strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[17].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_09: /* FreeFall 046 */
					strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[18].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_2_10: /* FreeFall 046 */
					strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[19].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_01: /* FreeFall 046 */
					strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[20].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_02: /* FreeFall 046 */
					strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[21].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_03: /* FreeFall 046 */
					strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[22].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_04: /* FreeFall 046 */
					strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[23].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_05: /* FreeFall 046 */
					strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[24].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_06: /* FreeFall 046 */
					strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[25].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_07: /* FreeFall 046 */
					strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[26].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_08: /* FreeFall 046 */
					strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[27].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_09: /* FreeFall 046 */
					strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[28].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_TEIKEI_3_10: /* FreeFall 046 */
					strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[29].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
					break;
				case ID_FAVORITE_FOLDER_01: /* RockDance 129 */
					strName.Format(TEXT("&1 %s"), shortFavorite(g_strFavorite[0]));
					break;
				case ID_FAVORITE_FOLDER_02: /* RockDance 129 */
					strName.Format(TEXT("&2 %s"), shortFavorite(g_strFavorite[1]));
					break;
				case ID_FAVORITE_FOLDER_03: /* RockDance 129 */
					strName.Format(TEXT("&3 %s"), shortFavorite(g_strFavorite[2]));
					break;
				case ID_FAVORITE_FOLDER_04: /* RockDance 129 */
					strName.Format(TEXT("&4 %s"), shortFavorite(g_strFavorite[3]));
					break;
				case ID_FAVORITE_FOLDER_05: /* RockDance 129 */
					strName.Format(TEXT("&5 %s"), shortFavorite(g_strFavorite[4]));
					break;
				case ID_FAVORITE_FOLDER_06: /* RockDance 129 */
					strName.Format(TEXT("&6 %s"), shortFavorite(g_strFavorite[5]));
					break;
				case ID_FAVORITE_FOLDER_07: /* RockDance 129 */
					strName.Format(TEXT("&7 %s"), shortFavorite(g_strFavorite[6]));
					break;
				case ID_FAVORITE_FOLDER_08: /* RockDance 129 */
					strName.Format(TEXT("&8 %s"), shortFavorite(g_strFavorite[7]));
					break;
				case ID_FAVORITE_FOLDER_09: /* RockDance 129 */
					strName.Format(TEXT("&9 %s"), shortFavorite(g_strFavorite[8]));
					break;
				case ID_FAVORITE_FOLDER_10: /* RockDance 129 */
					strName.Format(TEXT("&0 %s"), shortFavorite(g_strFavorite[9]));
					break;
				case ID_EDIT_COPY_FORMAT_01: /* FunnyCorn 175 */
					strName.Format(TEXT("01：%s"), g_userCopyFormat[0].strName);
					break;
				case ID_EDIT_COPY_FORMAT_02: /* FunnyCorn 175 */
					strName.Format(TEXT("02：%s"), g_userCopyFormat[1].strName);
					break;
				case ID_EDIT_COPY_FORMAT_03: /* FunnyCorn 175 */
					strName.Format(TEXT("03：%s"), g_userCopyFormat[2].strName);
					break;
				case ID_EDIT_COPY_FORMAT_04: /* FunnyCorn 175 */
					strName.Format(TEXT("04：%s"), g_userCopyFormat[3].strName);
					break;
				case ID_EDIT_COPY_FORMAT_05: /* FunnyCorn 175 */
					strName.Format(TEXT("05：%s"), g_userCopyFormat[4].strName);
					break;
				case ID_CONV_TAG_TO_TAG_01: /* STEP 034 */
					strName.Format(TEXT("01：%s"), g_userConvFormatTag2Tag[0].strName);
					break;
				case ID_CONV_TAG_TO_TAG_02: /* STEP 034 */
					strName.Format(TEXT("02：%s"), g_userConvFormatTag2Tag[1].strName);
					break;
				case ID_CONV_TAG_TO_TAG_03: /* STEP 034 */
					strName.Format(TEXT("03：%s"), g_userConvFormatTag2Tag[2].strName);
					break;
				case ID_CONV_TAG_TO_TAG_04: /* STEP 034 */
					strName.Format(TEXT("04：%s"), g_userConvFormatTag2Tag[3].strName);
					break;
				case ID_CONV_TAG_TO_TAG_05: /* STEP 034 */
					strName.Format(TEXT("05：%s"), g_userConvFormatTag2Tag[4].strName);
					break;
				default:
					pMenu->GetMenuString((UINT)pKey->wCmdID, strName, MF_BYCOMMAND);
					break;
				}

				// 現在のアクセラレータキー部分を削除
				int		nPos;
				if ((nPos = strName.Find('\t')) != -1) {
					strName = strName.Left(nPos);
				}

				if (pAccel != NULL) {
					// 新しいアクセラレータキー部分を追加
					strName += '\t';
					if (wModifiers & HOTKEYF_ALT    ) strName += _T("Alt+");
					if (wModifiers & HOTKEYF_CONTROL) strName += _T("Ctrl+");
					if (wModifiers & HOTKEYF_SHIFT  ) strName += _T("Shift+");
					// キー部分を追加
					if (pAccel->key <= 0x91) {
						strName += g_sKeyName[pAccel->key];
					} else {
						strName += _T("?");
					}
				}

				// メニュー文字列を更新
				pMenu->ModifyMenu((UINT)pKey->wCmdID, MF_BYCOMMAND, pKey->wCmdID, (LPCTSTR)strName);
			}
		}
	}

	extern CPlugin plugins;
	for (i=0;i<plugins.arPluginKey.GetSize();i++) {
		KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
		if (pKey->wCmdID != 0x0000) {
			// アクセラレータの作成
			ACCEL	*pAccel = NULL;
			WORD	wModifiers = 0x0000;
			if (pKey->dwKeyCode != 0) {
				wModifiers = HIWORD(pKey->dwKeyCode);
				pAccel = &m_accelTable[nCount];
				pAccel->fVirt = FNOINVERT | FVIRTKEY;
				if (wModifiers & HOTKEYF_ALT    ) pAccel->fVirt |= FALT;
				if (wModifiers & HOTKEYF_CONTROL) pAccel->fVirt |= FCONTROL;
				if (wModifiers & HOTKEYF_SHIFT  ) pAccel->fVirt |= FSHIFT;
				pAccel->key = LOWORD(pKey->dwKeyCode);
				pAccel->cmd = pKey->wCmdID;
				nCount++;
			}

			// メニュー項目の書き換え
			if (pMenu != NULL) {
				CString	strName;
				pMenu->GetMenuString((UINT)pKey->wCmdID, strName, MF_BYCOMMAND);

				// 現在のアクセラレータキー部分を削除
				int		nPos;
				if ((nPos = strName.Find('\t')) != -1) {
					strName = strName.Left(nPos);
				}

				if (pAccel != NULL) {
					// 新しいアクセラレータキー部分を追加
					strName += '\t';
					if (wModifiers & HOTKEYF_ALT    ) strName += _T("Alt+");
					if (wModifiers & HOTKEYF_CONTROL) strName += _T("Ctrl+");
					if (wModifiers & HOTKEYF_SHIFT  ) strName += _T("Shift+");
					// キー部分を追加
					if (pAccel->key <= 0x91) {
						strName += g_sKeyName[pAccel->key];
					} else {
						strName += _T("?");
					}
				}

				// メニュー文字列を更新
				pMenu->ModifyMenu((UINT)pKey->wCmdID, MF_BYCOMMAND, pKey->wCmdID, (LPCTSTR)strName);
			}
		}
	}

	// アクセラレータテーブルの登録
	m_hAccel = CreateAcceleratorTable(m_accelTable, nCount);

	pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
	if (pMenu != NULL) {
		CMenu* pTeikeiMenu = NULL;
		int nCount = pMenu->GetMenuItemCount();
		int j; for (int j=0;j<nCount;j++) {
			CString strTitle;
			pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "編集(&E)") {
				pMenu = pMenu->GetSubMenu(j);
				break;
			}
		}
		nCount = pMenu->GetMenuItemCount();
		for (j=0;j<nCount;j++) {
			CString strTitle;
			pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
			if (strTitle == "定型文貼り付け") {
				pTeikeiMenu = pMenu->GetSubMenu(j);
				break;
			}
		}
		for (int k = 0; k < 3 && pTeikeiMenu; k++) {
			CMenu* pSubMenu = pTeikeiMenu->GetSubMenu(0);
			pTeikeiMenu->RemoveMenu(0, MF_BYPOSITION);
			pTeikeiMenu->AppendMenu(MF_POPUP , (UINT)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
		}
	}
}

//---------------------------------------------------------------------------
// func:	レジストリからWinampのパスを取り出す
//---------------------------------------------------------------------------
void CSuperTagEditorApp::GetWinampPath(void)
{
	HKEY	hKey;
	CString strKeyString = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Winamp.exe";

	g_sOptWinAmpPath = "";

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyString, 0, KEY_EXECUTE, &hKey) == ERROR_SUCCESS) {
		DWORD	dwValueType;
		DWORD	dwValueSize = FILENAME_MAX;
		unsigned char	sValue[FILENAME_MAX];

		/* レジストリ値の取得 */
		RegQueryValueEx(hKey, TEXT(""), NULL, &dwValueType, &sValue[0], &dwValueSize);

		/* 結果の表示 */
		if (dwValueType == REG_SZ) g_sOptWinAmpPath = sValue;

		/* 終了 */
		RegCloseKey(hKey);
	}
}

// ジャンルリストの設定をレジストリから読みこむ
void CSuperTagEditorApp::ReadGenreList(void)
{
	CString strINI = m_pszProfileName;
	InitProfile();
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		CString	strName;
		strName.Format(TEXT("%d"), pGenre->byGenre);
		pGenre->bAddList = (DWORD)MyGetProfileInt(sSectionGenreList, strName, 1) ? true : false;
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		CString		strSectionName;
		strSectionName.Format(TEXT("haseta\\%s\\%d"), sSectionUserGenreList, i);
		g_genreListUSER[i].bAddList = MyGetProfileInt(strSectionName, sKeyUserGenreAddList, 0) == 0 ? false : true;
		g_genreListUSER[i].byGenre = (BYTE)MyGetProfileInt(strSectionName, sKeyUserGenreNo, 255);
		g_genreListUSER[i].sName = MyGetProfileString(strSectionName, sKeyUserGenreName, NULL);
		if (g_genreListUSER[i].sName.GetLength() == 0) {
			g_genreListUSER[i].bUse = false;
		} else {
			g_genreListUSER[i].bUse = true;
		}
	}
	FreeProfile();
}

// ジャンルリストの設定をレジストリに書き出す
void CSuperTagEditorApp::WriteGenreList(void)
{
	CString strINI = m_pszProfileName;
	InitProfile();
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		CString	strName;
		strName.Format(TEXT("%d"), pGenre->byGenre);
		MyWriteProfileInt(sSectionGenreList, strName, pGenre->bAddList ? 1 : 0);
	}
	if (g_genreListUSER != NULL) {
		int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
			CString		strSectionName;
			strSectionName.Format(TEXT("haseta\\%s\\%d"), sSectionUserGenreList, i);
			MyWriteProfileInt(strSectionName, sKeyUserGenreAddList, g_genreListUSER[i].bAddList == true ? 1 : 0);
			MyWriteProfileInt(strSectionName, sKeyUserGenreNo, g_genreListUSER[i].byGenre);
			MyWriteProfileString(strSectionName, sKeyUserGenreName, g_genreListUSER[i].sName);
		}
	}
	SaveProfile();
	FreeProfile();
}

void CSuperTagEditorApp::LoadStdProfileSettings(UINT nMaxMRU)  /* StartInaction 053 */
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	if (nMaxMRU != 0) {
		m_pRecentFileList = new CMyRecentFileList(0, TEXT("haseta\\MRU"), TEXT("MRUFolder%d"), nMaxMRU, 60);
		m_pRecentFileList->ReadList();
	}
}

void CSuperTagEditorApp::OnUpdateFileMruFile(CCmdUI* pCmdUI) /* StartInaction 053 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	m_pRecentFileList->UpdateMenu(pCmdUI);
}

void CSuperTagEditorApp::AddToRecentFileList(LPCTSTR lpszPathName) /* StartInaction 053 */
{
	CString strPath = lpszPathName; /* WildCherry2 079 */
	if (IsFolderName(lpszPathName) == true) {
		strPath.Delete(strPath.GetLength()-1);
	}
	m_pRecentFileList->Add(strPath);
}

BOOL CSuperTagEditorApp::OnOpenRecentFile(UINT nID) /* BeachMonster 104 */
{
	// 他のやり方が判らなかったので、OnOpenRecentFile()をオーバーライドして自前で処理した
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pRecentFileList)[nIndex]);

	POSITION pos = GetFirstDocTemplatePosition();
	if (pos != NULL) {
		CDocTemplate* pDocTemp = GetNextDocTemplate(pos);
		if (pDocTemp != NULL) {
			POSITION docPos = pDocTemp->GetFirstDocPosition();
			if (docPos != NULL) {
				CSuperTagEditorDoc* pDoc = (CSuperTagEditorDoc*)pDocTemp->GetNextDoc(docPos);
				if (pDoc != NULL) {
					pDoc->OnOpenDocument((*m_pRecentFileList)[nIndex]);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CSuperTagEditorApp::OnFavoriteFolder(CString& strFolder)
{
	POSITION pos = GetFirstDocTemplatePosition();
	if (pos != NULL) {
		CDocTemplate* pDocTemp = GetNextDocTemplate(pos);
		if (pDocTemp != NULL) {
			POSITION docPos = pDocTemp->GetFirstDocPosition();
			if (docPos != NULL) {
				CSuperTagEditorDoc* pDoc = (CSuperTagEditorDoc*)pDocTemp->GetNextDoc(docPos);
				if (pDoc != NULL) {
					pDoc->OnOpenDocument(strFolder);
					return;
				}
			}
		}
	}
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder01(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[0].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder01() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[0]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder02(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[1].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder02() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[1]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder03(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[2].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder03() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[2]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder04(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[3].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder04() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[3]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder05(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[4].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder05() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[4]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder06(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[5].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder06() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[5]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder07(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[6].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder07() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[6]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder08(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[7].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder08() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[7]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder09(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[8].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder09() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[8]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder10(CCmdUI* pCmdUI) /* RockDance 124 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!g_strFavorite[9].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder10() /* RockDance 124 */
{
	OnFavoriteFolder(g_strFavorite[9]);
}

void CSuperTagEditorApp::ReadFixedWordList()
{
	TCHAR   drive[_MAX_DRIVE];
	TCHAR   dir[_MAX_DIR];
	TCHAR   buff[_MAX_PATH] = {'\0'};

	//自己アプリのパス所得（大小文字識別付き）
	GetModuleFileName(m_hInstance, buff, _MAX_PATH);
	_tsplitpath(buff, drive, dir, NULL, NULL);
	_tmakepath(buff, drive, dir, TEXT("FixedWordList"), TEXT("ini"));

	g_arFixedWords.RemoveAll();
	TRY {
		CFile	file;
		if (file.Open(buff, CFile::modeRead|CFile::shareDenyNone)) {
			CArchive	ar(&file, CArchive::load);
			CString strLine;
			while(ar.ReadString(strLine)) {
				g_arFixedWords.Add(strLine);
			}
		}
	}
	CATCH( CFileException, e) {
		CString	str;
		str.Format(TEXT("%s の読み込みに失敗しました"), buff);
		MessageBox(NULL, str, TEXT("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
	}
	END_CATCH
}

void CSuperTagEditorApp::WriteFixedWordList()
{
	TCHAR   drive[_MAX_DRIVE];
	TCHAR   dir[_MAX_DIR];
	TCHAR   buff[_MAX_PATH] = {'\0'};

	//自己アプリのパス所得（大小文字識別付き）
	GetModuleFileName(m_hInstance, buff, _MAX_PATH);
	_tsplitpath(buff, drive, dir, NULL, NULL);
	_tmakepath(buff, drive, dir, TEXT("FixedWordList"), TEXT("ini"));

	TRY {
		CFile	file;
		if (file.Open(buff, CFile::modeCreate|CFile::modeWrite)) {
			CArchive	ar(&file, CArchive::store);
			CString strLine;
			for (int i=0;i<g_arFixedWords.GetSize();i++) {
				ar.WriteString(g_arFixedWords.GetAt(i) + "\n");
			}
		}
	}
	CATCH( CFileException, e) {
		CString	str;
		str.Format(TEXT("%s の書き込みに失敗しました"), buff);
		MessageBox(NULL, str, TEXT("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
	}
	END_CATCH
}
