#ifndef _MP3INFP_EXPORT_FUNC_H
#define _MP3INFP_EXPORT_FUNC_H

/*
☆mp3infp APIの利用・dllの再配布について　2002-09-16

1.APIを利用する前に必ず mp3infp_GetVer() を呼び出して、必要なバージョンを調べてください
(古いmp3infpはmp3infp_GetVer()をサポートしていないため、エントリポイントのNULLチェックも必要です)
2.mp3infp.dll/mp3infpr.dllのフリーソフトへの同梱はご自由にどうぞ。
・この場合、利用ソフトのカレントDirにmp3infp.dllを置いて利用します(systemとかwindowsとかには置かないでください)
・ドキュメントに同梱したmp3infpのバージョンなどを明記してください
・mp3infpはCライブラリとMFCライブラリをスタティックリンクしてるため、サイズが大きいです
・カレントとWindowsシステム(mp3infpがインストールされているdir)のどちらを優先してロードするのか、よく考えて設計してください
*/

#if __cplusplus
extern "C"{
#endif

/*=======================================================================================
名前：	mp3infp_GetVer
概要：	mp3infpのバージョンを取得する
引数：	なし
戻値：	DWORD	バージョン情報 
			Ver.2.11	= 0x0211
備考：	
対応：	Ver2.11～
*/
DWORD __stdcall mp3infp_GetVer();
typedef DWORD (WINAPI *LPMP3INFP_GETVER)();

/*=======================================================================================
名前：	mp3infp_ViewProp
概要：	mp3infpに対応したファイル形式のプロパティを開く
説明：	指定ファイルのプロパティをmp3infpのタブをアクティブにして開きます
引数：
		IN HWND hWnd		親ウインドウを指定します。無い場合はNULL。
		IN char *szFileName	対象ファイル名をフルパスで指定。
		IN DWORD dwPage		・mp3infpの何ページ目をアクティブにするか指定する(0=ID3v1 / 1=ID3v2 / 2=RiffSIF)
							・タグを含まないmp3の場合のみ有効
							・タグを含む場合はID3v2/RiffSIF/ID3v1の順で検索して、最初に見つかったタグをアクティブにします

戻値：	BOOL 成功=TRUE/失敗=FALSE
対応：	Ver2.00～
*/
BOOL __stdcall mp3infp_ViewProp(HWND hWnd,char *szFileName,DWORD dwDummy);
typedef BOOL (WINAPI *LPMP3INFP_VIEWPROP)(HWND hWnd,char *szFileName,DWORD dwDummy);

/*=======================================================================================
名前：	mp3infp_Load
概要：	タグ情報をロードする
引数：
		IN HWND hWnd		呼び出し元ウインドウを指定します。無い場合はNULL。
		IN char *szFileName	対象ファイル名をフルパスで指定。
戻値：	DWORD 
		-1=ロード失敗
		ERROR_SUCCESS=成功
		(その他)=Win32エラーコード (FormatMessageで文字列を取得できる)
対応：	Ver2.26～
*/
DWORD __stdcall mp3infp_Load(HWND hWnd,char *szFileName);
typedef DWORD (WINAPI *LPMP3INFP_LOAD)(HWND hWnd,char *szFileName);

/*=======================================================================================
////////////////////////////////////////////////////////////////////
表1　mp3infp_GetValue()/mp3infp_SetValue()でszValueNameに指定する名前一覧
////////////////////////////////////////////////////////////////////

	[MP3]				ID3v1		ID3v2		RiffSIF
	フォーマット		"AFMT"		"AFMT"		"AFMT"	
	演奏時間			"TIME"		"TIME"		"TIME"
	タイトル			"INAM_v1"	"INAM_v2"	"INAM_rmp"
	アーティスト		"IART_v1"	"IART_v2"	"IART_rmp"
	アルバム			"IPRD_v1"	"IPRD_v2"	"IPRD_rmp"
	コメント			"ICMT_v1"	"ICMT_v2"	"ICMT_rmp"
	作成日				"ICRD_v1"	"ICRD_v2"	"ICRD_rmp"
	ジャンル			"IGNR_v1"	"IGNR_v2"	"IGNR_rmp"
	(ID3v2/RiffSIF)
	著作権							"ICOP_v2"	"ICOP_rmp"
	ソフトウェア					"ISFT_v2"	"ISFT_rmp"
	(ID3v2)
	作曲							"COMP_v2"
	Orig.アーティスト				"OART_v2"
	URL								"URL_v2"
	(RiffSIF)
	ソース										"ISRC_rmp"
	エンジニア									"IENG_rmp"
	(ID3v1/2)
	トラック番号		"TRACK_v1"	"TRACK_v2"

	[WAV]
	フォーマット		"AFMT"
	演奏時間			"TIME"
	タイトル			"INAM"
	タイトル			"ISBJ"
	アーティスト		"IART"
	アルバム			"IPRD"
	コメント			"ICMT"
	作成日				"ICRD"
	ジャンル			"IGNR"
	著作権				"ICOP"
	ソフトウェア		"ISFT"
	ソース				"ISRC"
	エンジニア			"IENG"

	[AVI]
	音声フォーマット	"AFMT"	
	映像フォーマット	"VFMT"
	時間				"TIME"
	タイトル			"INAM"
	タイトル			"ISBJ"
	アーティスト		"IART"
	コメント			"ICMT"
	作成日				"ICRD"
	ジャンル			"IGNR"
	著作権				"ICOP"
	ソフトウェア		"ISFT"
	ソース				"ISRC"
	エンジニア			"IENG" 

	[VQF]
	フォーマット		"AFMT"
	演奏時間			"TIME"
	タイトル			"INAM"
	アーティスト		"IART"
	コメント			"ICMT"
	著作権				"ICOP"
	保存名				"FILE"

	[WMA]
	音声フォーマット	"AFMT"
	映像フォーマット	"VFMT"
	時間				"TIME"
	タイトル			"INAM"
	トラック			"TRACK"
	アーティスト		"IART"
	アルバム			"IPRD"
	コメント			"ICMT"
	作成日				"ICRD"
	ジャンル			"IGNR"
	著作権				"ICOP"
	URL(Album)			"URL1"
	URL(関連)			"URL2"

	[OGG]
	フォーマット		"AFMT"
	演奏時間			"TIME"
	タイトル			"INAM"
	アーティスト		"IART"
	アルバム			"IPRD"
	コメント			"ICMT"
	作成日				"ICRD"
	ジャンル			"IGNR"
	トラック番号		"TRACK"
*/

#define MP3INFP_FILE_UNKNOWN	0x00
#define MP3INFP_FILE_MP3		0x01
#define MP3INFP_FILE_WAV		0x02
#define MP3INFP_FILE_AVI		0x03
#define MP3INFP_FILE_VQF		0x04
#define MP3INFP_FILE_WMA		0x05
#define MP3INFP_FILE_OGG		0x07
#define MP3INFP_FILE_APE		0x08
/*=======================================================================================
名前：	mp3infp_GetType
概要：	ファイルの種類を取得する
補足：	mp3infp_Load()の後に呼び出してください
引数：	なし
戻値：	DWORD
#define MP3INFP_FILE_UNKNOWN	0x00
#define MP3INFP_FILE_MP3		0x01
#define MP3INFP_FILE_WAV		0x02
#define MP3INFP_FILE_AVI		0x03
#define MP3INFP_FILE_VQF		0x04
#define MP3INFP_FILE_WMA		0x05
#define MP3INFP_FILE_OGG		0x07
#define MP3INFP_FILE_APE		0x08
対応：	Ver2.26～
*/
DWORD __stdcall mp3infp_GetType();
typedef DWORD (WINAPI *LPMP3INFP_GETTYPE)();

/*=======================================================================================
名前：	mp3infp_GetValue
概要：	タグ情報を取得する
補足：	mp3infp_Load()の後に呼び出してください
引数：	
		IN char *szValueName	タグの種類を示す名前(表1を参照)
		OUT char **buf			タグ情報を示すバッファのポインタを受け取るポインタ
戻値：	BOOL 成功=TRUE/失敗=FALSE
対応：	Ver2.26～
*/
BOOL __stdcall mp3infp_GetValue(char *szValueName,char **buf);
typedef BOOL (WINAPI *LPMP3INFP_GETVALUE)(char *szValueName,char **buf);

#define MP3INFP_HAS_MP3_ID3V1	0x01
#define MP3INFP_HAS_MP3_ID3V2	0x02
#define MP3INFP_HAS_MP3_RIFFSIF	0x04
/*=======================================================================================
名前：	mp3infp_mp3_GetTagType
概要：	mp3が持っているタグの種類を取得する
補足：	mp3infp_Load()の後に呼び出してください
引数：	なし
戻値：	DWORD (OR)
#define MP3INFP_HAS_MP3_ID3V1	0x01
#define MP3INFP_HAS_MP3_ID3V2	0x02
#define MP3INFP_HAS_MP3_RIFFSIF	0x04
対応：	Ver2.27～
*/
DWORD __stdcall mp3infp_mp3_GetTagType();
typedef DWORD (WINAPI *LPMP3INFP_MP3_GETTAGTYPE)();

#if 0
/*=======================================================================================
名前：	mp3infp_SetValue
概要：	タグ情報を設定する
補足：	mp3infp_Load()の後に呼び出してください
引数：	
		IN char *szValueName	タグの種類を示す名前(表1を参照)
		OUT char *buf			タグ情報を示すバッファ
		OUT DWORD len			長さ(終端の'\0'を含む)
戻値：	BOOL 成功=TRUE/失敗=FALSE
対応：	未実装
*/
BOOL __stdcall mp3infp_SetValue(char *szValueName,char *buf,DWORD len);
typedef BOOL (WINAPI *LPMP3INFP_SETVALUE)(char *szValueName,char *buf,DWORD len);


BOOL __stdcall mp3infp_Save(HWND hWnd,char *szFileName);

//mp3
BOOL __stdcall mp3infp_mp3_MakeId3v1(HWND hWnd,char *szFileName);
BOOL __stdcall mp3infp_mp3_DelId3v1(HWND hWnd,char *szFileName);
BOOL __stdcall mp3infp_mp3_ConvId3v2(HWND hWnd,char *szFileName);
BOOL __stdcall mp3infp_mp3_ConvId3v2(HWND hWnd,char *szFileName);
BOOL __stdcall mp3infp_mp3_ConvRMP(HWND hWnd,char *szFileName);
BOOL __stdcall mp3infp_mp3_ConvRMP(HWND hWnd,char *szFileName);
#endif

#if __cplusplus
}
#endif

#endif //_MP3INFP_EXPORT_FUNC_H
