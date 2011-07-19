#ifndef STEP_API_H
#define STEP_API_H

#define STEP_API __declspec(dllexport)

#define STEP_API_VERSION 0x0001

/* 処理結果 */
enum {
	STEP_SUCCESS,			// 正常
	STEP_UNKNOWN_FORMAT,	// 未知のファイル形式
	STEP_ERROR				// エラー
};

/* メニュー種別 */
enum {
	MENU_FILE,
	MENU_EDIT,
	MENU_DISP,
	MENU_CONV,
	MENU_PLAYLIST,
	MENU_PLAYER,
	MENU_OPTION,
	/*MENU_OPTION_PLUGIN,*/
	MENU_HELP,
	// ここから下は STEPGetMenu() では取得できない STEPOnLoadMenu() で使う
	MENU_CELL_RANGE,
	MENU_FILE_EDIT_OK,
	MENU_FILE_RANGE,
	MENU_SELECT_FOLDER,
};

/* ファイル形式 */
enum	{
	FILE_FORMAT_UNKNOWN = 1,	// ファイル形式：未知の形式
	// その他はプラグイン毎に定義
};

/* セル毎の入力用コントロール種別 */
enum CONTROLTYPE {edit/*default*/, combobox, invalid, multiedit, multieditreadonly};
#define _NULL			invalid
#define _EDIT			edit
#define _CBOX			combobox
#define _MEDIT			multiedit
#define _MEDITR			multieditreadonly

/* 表示カラム */
enum COLUMNTYPE {
	COLUMN_TREE_ITEM = 0,
	COLUMN_FULL_PATH_NAME = 0,
	COLUMN_FILE_NAME,
	COLUMN_TRACK_NAME,		// 曲名
	COLUMN_ARTIST_NAME,		// アーティスト名
	COLUMN_ALBUM_NAME,		// アルバム名／製品名
	COLUMN_TRACK_NUMBER,	// トラック番号(ID3 tag v1.1のみ)
	COLUMN_YEAR,			// リリース年号
	COLUMN_GENRE,			// ジャンル
	COLUMN_COMMENT,			// コメント文字列
	COLUMN_COPYRIGHT,		// 著作権
	COLUMN_ENGINEER,		// エンジニア(製作者)
	COLUMN_SOURCE,			// ソース
	COLUMN_SOFTWARE,		// ソフトウェア
	COLUMN_KEYWORD,			// キーワード
	COLUMN_TECHNICIAN,		// 技術者
	COLUMN_LYRIC,			// 歌詞
	COLUMN_COMMISSION,		// コミッション
	COLUMN_COMPOSER,		// 作曲
	COLUMN_ORIG_ARTIST,		// Orig.アーティスト
	COLUMN_URL,				// URL
	COLUMN_ENCODEST,		// エンコードした人
	COLUMN_FILE_SIZE,		// ファイルサイズ
	COLUMN_FILE_TIME,		// ファイルタイムスタンプ
	COLUMN_FILE_PATH,		// ファイルパス名
	COLUMN_PLAY_TIME,		// 演奏時間
	COLUMN_LYRIC_FILE,		// 歌詞ファイル
	COLUMN_FILE_TYPE,		// ファイルの種類
	COLUMN_FORMAT,			// フォーマット
	COLUMN_OTHER,			// その他
	COLUMN_MAX,
};
#define COLUMN_EDIT_MAX COLUMN_OTHER
/*
case COLUMN_FULL_PATH_NAME:	// フルパス名
case COLUMN_FILE_NAME:		// ファイル名
case COLUMN_TRACK_NAME:		// トラック名
case COLUMN_ARTIST_NAME:	// アーティスト名
case COLUMN_ALBUM_NAME:		// アルバム名
case COLUMN_TRACK_NUMBER:	// トラック番号
case COLUMN_YEAR:			// リリース年号
case COLUMN_GENRE:			// ジャンル番号
case COLUMN_COMMENT:		// コメント
case COLUMN_COPYRIGHT:		// 著作権
case COLUMN_ENGINEER:		// エンジニア(製作者)
case COLUMN_SOURCE:			// ソース
case COLUMN_SOFTWARE:		// ソフトウェア
case COLUMN_KEYWORD:		// キーワード
case COLUMN_TECHNICIAN:		// 技術者
case COLUMN_LYRIC:			// 歌詞
case COLUMN_COMMISSION:		// コミッション
case COLUMN_FILE_SIZE:		// ファイルサイズ
case COLUMN_FILE_TIME:		// ファイル更新日
case COLUMN_FILE_PATH:		// ファイルパス名
case COLUMN_PLAY_TIME:		// 演奏時間
case COLUMN_LYRIC_FILE:		// 歌詞ファイル
case COLUMN_FILE_TYPE:		// ファイルの種類
case COLUMN_FORMAT:			// フォーマット
case COLUMN_COMPOSER:		// 作曲
case COLUMN_ORIG_ARTIST:	// Orig.アーティスト
case COLUMN_URL:			// URL
case COLUMN_ENCODEST:		// エンコードした人
case COLUMN_OTHER:			// その他
*/

/* タグ情報取得／設定用のフィールド名 */
enum FIELDTYPE {
	FIELD_FULL_PATH_NAME,			// ファイル名(フルパス)
	FIELD_FILE_NAME,				// strFullPathName のファイル名部分
	FIELD_FILE_PATH,				// strFullPathName のパス部分
	FIELD_VISUAL_FORMAT,			// 映像フォーマット
	FIELD_AUDIO_FORMAT,				// 音声フォーマット
	FIELD_TRACK_NAME,				// トラック名
	FIELD_ARTIST_NAME,				// アーティスト名
	FIELD_ALBUM_NAME,				// アルバム名
	FIELD_COMMENT,					// コメント
	FIELD_YEAR,						// リリース年号
	FIELD_TRACK_NUMBER,				// トラック番号(文字列編集用)
	FIELD_GENRE,					// ジャンル名
	FIELD_TRACK_NAME_SI,			// 曲名
	FIELD_ARTIST_NAME_SI,			// アーティスト名
	FIELD_ALBUM_NAME_SI,			// 製品名
	FIELD_COMMENT_SI,				// コメント文字列
	FIELD_YEAR_SI,					// リリース年号
	FIELD_GENRE_SI,					// ジャンル名
	FIELD_COPYRIGHT_SI,				// 著作権
	FIELD_ENGINEER_SI,				// エンジニア
	FIELD_SOURCE_SI,				// ソース
	FIELD_SOFTWARE_SI,				// ソフトウェア
	FIELD_KEYWORD_SI,				// キーワード
	FIELD_TECHNICIAN_SI,			// 技術者
	FIELD_LYRIC_SI,					// 歌詞
	FIELD_COMMISSION_SI,			// コミッション
	FIELD_COMPOSER_SI,				// 作曲
	FIELD_ORIG_ARTIST_SI,			// Orig.アーティスト
	FIELD_URL_SI,					// URL
	FIELD_ENCODEST,					// エンコードした人
	FIELD_OTHER,					// その他
	FILED_FILE_TYPE_NAME,			// ファイルタイプ文字列
	FIELD_MAX
};

/* ファイル情報 */
typedef struct _FILE_INFO {
	void*	pFileMP3;
} FILE_INFO;

/**--------------------------------------------------------------------**/
/* プラグインに必ず実装しなければいけない関数 */
extern "C" {
STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder);
STEP_API UINT WINAPI STEPGetAPIVersion(void);
STEP_API LPCTSTR WINAPI STEPGetPluginName(void);
STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void);
}

/* プラグインがタグの読み書きをサポートする場合に実装しなければいけない関数 */
extern "C" {
STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT, COLUMNTYPE, bool);
STEP_API UINT WINAPI STEPGetColumnMax(UINT, UINT, bool);
STEP_API UINT WINAPI STEPLoad(FILE_INFO*, LPCTSTR);
STEP_API UINT WINAPI STEPSave(FILE_INFO*);
}

/* プラグインに実装できる関数 */
extern "C" {
STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID);
STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID);
STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID);
STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd);
STEP_API bool WINAPI STEPSupportSIF(UINT nFormat);
STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat);
STEP_API void WINAPI STEPShowOptionDialog(HWND);
STEP_API void WINAPI STEPOnLoadMenu(HMENU, UINT);
STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT, COLUMNTYPE);
STEP_API bool WINAPI STEPHasSpecificOriginalColumnName(UINT);
}

// ファイル毎に行うコールバック関数 */
typedef bool WINAPI STEPProcessSelectedFilesCB(FILE_INFO*, int, HWND);

#endif