// ======================================================
// 概  要    ： ＭＰ３ファイル管理クラス
// ターゲット： Windows95/98/NT
// 処理系    ： Visual C++ Ver 6.0
// 作成者    ： MERCURY
// 作成日    ： 00/01/18(火)
// 著作権表記： Copyright(C) 2000 MERCURY.
// ======================================================

#ifndef __CFILEMP3_H__
#define __CFILEMP3_H__


// ======================================
// =====   インクルードファイル     =====
// ======================================
//#include "vbmp3.h"
#include "STEP_api.h"

// ======================================
// =====           定  数           =====
// ======================================

//#define RMP_SIF_LENGTH			2048/*LIST_SIZE VMMP3の定義 */	// RIFF MP3 の SIF の最大の長さ

// ======================================
// =====           マクロ           =====
// ======================================
// CString テキストの文字数を制限する
#define LIMIT_TEXT_LENGTH(sText, nLen)	{	\
	TCHAR	sWorkBuffer[LIST_SIZE+1];		\
	StringCopyN(sWorkBuffer, sText, nLen);	\
	sWorkBuffer[nLen] = '\0';				\
	sText = sWorkBuffer;					\
}



// ======================================
// =====       typedef／enum        =====
// ======================================



// ======================================
// =====       構造体／共用体       =====
// ======================================
struct	FILE_MP3	{
	bool	bModifyFlag;				// 外部から編集されたかどうかのフラグ
	bool	bCheckFlag;					// チェック状態
	long	lFileSize;					// ファイルサイズ
	CTime	time;						// 更新日
	CTime	timeCreate;					// 作成日
	int		nFormat;					// ファイルフォーマット(FILE_FORMAT_xxxxx)
	CString	strFullPathName;			// ファイル名(フルパス)
	CString	strFileName;				// strFullPathName のファイル名部分
	CString	strFilePath;				// strFullPathName のパス部分
	int		nPlayTime;					// 演奏時間(単位：秒)
	bool	bExistLyricFile;			// 歌詞ファイルの存在
///* Conspiracy 198 */	CString strBitRate;					// ビットレート
	CString strAudioFormat;				// 音声フォーマット
	CString strVisualFormat;			// 映像フォーマット

	// ID3 tag 情報
	CString	strTrackName;				// トラック名
	CString	strArtistName;				// アーティスト名
	CString	strAlbumName;				// アルバム名
	CString	strComment;					// コメント
	CString	strYear;					// リリース年号
	BYTE	byTrackNumber;				// トラック番号(数値編集用)
	CString	strTrackNumber;				// トラック番号(文字列編集用)
	BYTE	byDiskNumber;				// ディスク番号(数値編集用)
	CString	strDiskNumber;				// ディスク番号(文字列編集用)
	BYTE	byGenre;					// ジャンル番号
	CString	strGenre;					// ジャンル名

	// RIFF MP3 の SI フィールド(ID3v2 などでも使う)
	CString	strTrackNameSI;				// 曲名
	CString	strArtistNameSI;			// アーティスト名
	CString	strAlbumNameSI;				// 製品名
	CString	strCommentSI;				// コメント文字列
	CString	strYearSI;					// リリース年号
	CString	strTrackNumberSI;			// トラック番号
	CString strDiskNumberSI;			// ディスク番号
	CString	strGenreSI;					// ジャンル名
	CString	strCopyrightSI;				// 著作権
	CString	strEngineerSI;				// エンジニア
	CString	strSourceSI;				// ソース
	CString	strSoftwareSI;				// ソフトウェア
	CString	strKeywordSI;				// キーワード
	CString	strTechnicianSI;			// 技術者
	CString	strLyricSI;					// 歌詞
	CString	strCommissionSI;			// コミッション
	CString strWriterSI;				// 作詞 /* ADD MIMURA */
	CString strComposerSI;				// 作曲 /* Baja 154 */
	CString strAlbmArtistSI;			// Albm.アーティスト /* ADD MIMURA */
	CString strOrigArtistSI;			// Orig.アーティスト /* Baja 154 */
	CString strURLSI;					// URL /* Baja 154 */
	CString strEncodest;				// エンコードした人 /* Baja 154 */
	CString	strOther;					// その他(OggVorbisの非標準タグ) /* Conspiracy 196 */
	CString strEngineer;				// エンジニア（出版）/* ADD MIMURA */

	void*	fileSpecificInfo;			// ファイル固有情報

	CString strFileTypeName;			// ファイルタイプ文字列

	void *pTreeItem;
};

struct	GENRE_LIST	{		// ジャンルリスト
	bool	bAddList;		// リストに追加するかどうか
	BYTE	byGenre;		// ジャンル番号
	LPCTSTR	sName;			// ジャンル名(NULL:リスト終端)
};
struct	USER_GENRE_LIST	{	// ユーザジャンルリスト
	bool	bAddList;		// リストに追加するかどうか
	BYTE	byGenre;		// ジャンル番号
	CString	sName;			// ジャンル名
	bool	bUse;
};
extern	GENRE_LIST	g_genreListSCMPX[];
extern	USER_GENRE_LIST*	g_genreListUSER;
#define USER_GENRE_LIST_MAX 50


// ======================================
// =====         const 変数         =====
// ======================================



// ======================================
// =====        extern 変数         =====
// ======================================



// ======================================
// =====        static 変数         =====
// ======================================



// ======================================
// =====     関数プロトタイプ       =====
// ======================================
extern	const char *GetGenreName(BYTE);
extern	LPCTSTR GetGenreNameSIF(BYTE byGenre);
extern	int		GetGenreCode(LPCTSTR);
extern bool IsUserGenre(LPCTSTR sGenre);
extern	void	StringCopyN(LPTSTR, LPCTSTR, int, BOOL = TRUE);
extern	bool	IsFolderName(LPCTSTR);



// ======================================
// =====      プログラム領域        =====
// ======================================



#ifdef __cplusplus

class	CFileMP3	{
public:
			FILE_MP3	m_fileMP3;
public:
	CFileMP3();
	virtual	~CFileMP3();

public:		// ======================================
			// =====      public メンバ関数     =====
			// ======================================
	static	void	InitData(FILE_MP3 *);
	static	void	InitDataSIF(FILE_MP3 *);
	static	void	InitDataID3(FILE_MP3 *); /* STEP 029 */
			bool	Attach(LPCTSTR);
			void	Detach(void);
			bool	LoadFile(const char *, const char *);
	static	bool	CopyFile(FILE_MP3 *, LPCTSTR, bool = false);
	static	bool	WriteTag(FILE_MP3 *, bool = true);
	static	bool	WriteFileTag(FILE_MP3 *fileMP3);
	static	bool	ConvFileFormat(FILE_MP3 *, int);
//	static	bool	AutoConvFileFormatID3(FILE_MP3 *fileMP3);
//	static	bool	DeleteTagID3(FILE_MP3 *fileMP3/*const char **//* Baja 153 */);
//	static	bool	IsCreateID3v2(FILE_MP3 *fileMP3);
//	static	bool	MakeTagID3v2(const char *);
//	static	bool	DeleteTagID3v2(const char *);
//	static	bool	GetMpegInfo(const char *, MPEG_INFO *);
	static	CString SearchLyricsFile(FILE_MP3 *);
	static	CString	CFileMP3::GetIntTrackNo(const CString &no);
	static	CString	CFileMP3::GetIntDiskNo(const CString &no);
	static	bool	IsNumeric(const CString &str);
	static	bool	ConvFileName(FILE_MP3 *);

private:	// ======================================
			// =====     private メンバ関数     =====
			// ======================================
			void	Initialize(void);
	static	void	DeleteLineEndSpace(TCHAR *);
	static	CString SearchFileReent(LPCTSTR, LPCTSTR);


//	static	bool	ReadTagID3(LPCSTR, FILE_MP3 *, char * = NULL);
//	static	bool	WriteTagID3(FILE_MP3 *);
//	static	void	DeleteFrameID3v2(ID3Tag *pTag, ID3_FrameID frame_id);
//	static	void	UpdateTagFieldText(ID3Tag *pTag, ID3_FrameID frame_id, const char *sText);
//	static	void	UpdateTagFieldComment(ID3Tag *pTag, const char *sText);
//	static	bool	ReadTagID3v2(LPCSTR, FILE_MP3 *);
//	static	bool	WriteTagID3v2(FILE_MP3 *);
//	static	bool	ReadTagSIF(LPCSTR, FILE_MP3 *);
//	static	bool	WriteTagSIF(FILE_MP3 *);

protected:	// ======================================
			// =====    protected メンバ関数    =====
			// ======================================
/* RockDance 133 */
//	static	bool	ReadTagAPE(LPCSTR, FILE_MP3 *);
//	static	bool	WriteTagAPE(FILE_MP3 *);
};

#endif

#endif
