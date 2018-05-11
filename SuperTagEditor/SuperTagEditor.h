// SuperTagEditor.h : DMTLOGVIEW �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_DMTLOGVIEW_H__D67B6444_BFAB_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DMTLOGVIEW_H__D67B6444_BFAB_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

#define PROG_NAME			TEXT("STEP_O")		// �v���O������
#define PROG_VERSION		TEXT("1.0.0.0")		// �o�[�W����
#define PROG_NAME_ORG		TEXT("STEP_M")		// �v���O������
#define PROG_VERSION_ORG	TEXT("1.054 f")		// �o�[�W����

#define WM_USER_SET_STATUS_POS		WM_USER+1
#define WM_USER_SET_STATUS_SIZE		WM_USER+2
#define WM_USER_SET_STATUS_INPUT_MODE	WM_USER+3 /* Misirlou 150 */

#ifndef iskanji
#define iskanji(c)		((BYTE)(c) >= (BYTE)0x81 && (BYTE)(c) <= (BYTE)0x9f || (BYTE)(c) >= (BYTE)0xe0 && (BYTE)(c) <= (BYTE)0xfc)
#endif

#ifndef shortString	/* FreeFall 051 */
#define shortString(str, count)	str.GetLength() > count ? (str.Left(count) + "...") : str
#endif
#define TEIKEI_MENU_STRING_MAX	30	/* FreeFall 051 */

#ifndef shortFavorite	/* RockDance 129 */
#define shortFavorite(str)	str.GetLength() > 30 ? (str.Left(3) + "..." + str.Right(27)) : str
#endif

#include "regexp.h"
#include "MyRecentFileList.h"	// ClassView �ɂ���Ēǉ�����܂����B

enum	{
	FILENAME_NO_CHECK,				// �`�F�b�N���Ȃ�
	FILENAME_CONV_MULTIBYTE,		// �Q�o�C�g�����ɕϊ�
};

#define SORT_KEY_MAX		5
struct	SORT_STATE	{				// �\�[�g���
	int		nKeyColumn;				// �\�[�g�L�[(�J�����ԍ�)
	int		nType;					// �^�C�v(0:����/1:�~��)
};

#define CLASS_MAX			5		// ���ފK�w�̐[��(�ő�)
struct	CLASS_INFO	{				// ���ޏ��
	int		nType;					// ���ރ^�C�v
	int		nColumn[CLASS_MAX];		// �J�����ԍ�
};

enum	{
	COMMAND_GROUP_FILE,
	COMMAND_GROUP_EDIT,
	COMMAND_GROUP_DISP,
	COMMAND_GROUP_CONV, /* 2006.03.03 */
	COMMAND_GROUP_PLAYLIST,
	COMMAND_GROUP_PLAYER,
	COMMAND_GROUP_PLUGIN,
	COMMAND_GROUP_MAX,
};
struct	KEY_CONFIG	{				// �L�[���蓖��
	WORD	wCmdID;					// ���j���[���\�[�X�h�c
	DWORD	dwKeyCode;				// �z�b�g�L�[
	int		nGroupID;				// �O���[�v��
	char	*sName;					// ���ږ�
	LPTSTR	sRegName;				// ���W�X�g����
};
extern	KEY_CONFIG	g_listKeyConfig[];

#define FILENAME_REPLACE_MAX		12
#define USER_FILENAME_REPLACE_MAX		6	/* FreeFall 050 */
struct	FILENAME_REPLACE {			// �t�@�C�����u��
	CString	strBefore;				// �u���O�̕���
	CString	strAfter;				// �u����̕���
};
extern const LPCTSTR g_sRepTable[FILENAME_REPLACE_MAX][2];

#define USER_CONV_FORMAT_MAX		5 /*3 LastTrain 057 */
struct	USER_CONV_FORMAT	{		// ���[�U�[�ϊ�����
	CString	strName;				// ����
	CString	strTag2File;			// �����F�^�O��� => �t�@�C����
	CString	strFile2Tag;			// �����F�t�@�C���� => �^�O���
};

#define USER_CONV_FORMAT_EX_MAX		10
struct	USER_CONV_FORMAT_EX	{		// �g���Ń��[�U�[�ϊ�����
	CString	strName;				// ����
	CString	strFormat;				// ����
	CString	strFixString;			// �Œ蕶����
	int		nInitNumber;			// �A�ԁF�����l
	int		nAddNumber;				// �A�ԁF���Z�l
	int		nColumnCount;			// �A�ԁF����
	bool	bSpaceInitNumber;		// �������ݕs�̃Z���ŘA�ԃN���A
};

#define USER_MOVE_FODLER_FORMAT_MAX	5
struct	USER_MOVE_FODLER_FORMAT	{	// �ړ���t�H���_����
	CString	strName;				// ����
	CString	strFormat;				// ����
	CString	strFixString;			// �Œ蕶����
	bool	bCopy;					// �R�s�[����
	CString strInitFolder;			// �����t�H���_ /* STEP 022 */
	CString strCurrentMoveDirectory;// �I�����ꂽ�ړ���t�H���_ /* STE 022 */
};

/* STEP 034 */
#define USER_CONV_FORMAT_TAG2TAG_MAX	5
struct USER_CONV_FORMAT_TAG2TAG {	// �^�O�U���ϊ�����
	CString	strName;				// ����
	CString	strFormat;				// ����
};

/* FunnyCorn 175 */
#define USER_COPY_FORMAT_FORMAT_MAX	5
struct	USER_COPY_FORMAT_FORMAT	{	// �����R�s�[
	CString	strName;				// ����
	CString	strFormat;				// ����
	CString	strFixString;			// �Œ蕶����
};

/* STEP 035 */
#define TEIKEI_INFO_MAX	30
struct TEIKEI_INFO {				// ��^��
	CString	strTeikei;				// ��^��
	int		nTeikeiPaste;			// �\��t���ʒu
	BOOL	bAddSpace;				// �X�y�[�X�ǉ�
	BOOL	bAddChar;				// �O��ɕ�����ǉ�
	CString	strFront;				// �O�t������
	CString	strBack;				// ��t������
	BOOL	bShowDialog;			// �\��t�����_�C�A���O�\��
};

#define WRITE_FORMAT_MAX			5
struct	WRITE_FORMAT	{			// �g���Ń��[�U�[�ϊ�����
	CString	strName;				// ����
	CString	strFileName;			// �����t�@�C��
	CString	strExtName;				// �g���q
	bool	bWriteSelected;			// �I���t�@�C���̂ݏ㏑��
	CString	strCurrentFile;			// �J�����g�t�@�C����
	bool	bIsHtml;				// HTML �t�@�C���o�͗p(��̏ꍇ�ɑS�p�X�y�[�X���o��)
	bool	bWriteHtml;				// HTML �t�@�C���Ƃ��ďo�́i�^�O��u������j/* BeachMonster5 120 */
};

struct	CHECK_WORD_STATE	{		// �����`�F�b�N�p�̏��
	CString	strSearchWord;			// ����������
	CString	strReplaceWord;			// �u��������
	int		nTargetColumn;			// �����ΏۃJ����
	BOOL	bCheckDiffUL;			// �啶���^�������̋��
	BOOL	bRegExp;				// ���K�\��
	BOOL	bRangeSelected;			// �I��͈͂̂�
	BOOL	bMatchComplete;			// ���S�Ɉ�v
	BOOL	bMatchSelected;			// �`�F�b�N�����ɑI����Ԃɂ���
	int		nMatchColumn;			// (�o��)�������ʁF��v�����J����
	int		nMatchPos;				// (�o��)�������ʁF��v�����ʒu
	int		nMatchLength;			// (�o��)�������ʁF��v��������
	CRegExp	regexp;					// ���K�\���N���X(���������p)
};
enum	{CHECK_STATE_CHECK, CHECK_STATE_REPLACE, CHECK_STATE_MAX};
extern	CHECK_WORD_STATE	g_chkWord[CHECK_STATE_MAX];

enum	{PLAYER_WINAMP, PLAYER_SCMPX, PLAYER_KBMPLAY, PLAYER_EXTEND, PLAYER_LILITH/* WildCherry 070 */};

extern	RECT		g_rectMainWindow;		// ���C���E�B���h�E�̍��W
extern	BOOL		g_bMainFrameZoomed;
extern	BOOL		g_bMainFrameIconic;
extern	CString		g_strCurrentDirectory;
extern	CString		g_strCurrentMoveDirectory;
extern	CString		g_strCurrentPlayList;
extern	bool		g_bIsVersionUp;
extern	int			g_nUserConvFormatType;
extern	USER_CONV_FORMAT	g_userConvFormat[USER_CONV_FORMAT_MAX];
extern	USER_CONV_FORMAT_EX	g_userConvFormatEx[USER_CONV_FORMAT_EX_MAX];
extern	USER_MOVE_FODLER_FORMAT	g_userMoveFolder[USER_MOVE_FODLER_FORMAT_MAX];
extern	USER_COPY_FORMAT_FORMAT	g_userCopyFormat[USER_COPY_FORMAT_FORMAT_MAX]; /* FunnyCorn 175 */
extern	USER_CONV_FORMAT_TAG2TAG g_userConvFormatTag2Tag[USER_CONV_FORMAT_TAG2TAG_MAX]; /* STEP 034 */
extern	TEIKEI_INFO g_teikeiInfo[TEIKEI_INFO_MAX]; /* STEP 035 */
extern	WRITE_FORMAT		g_writeFormat[WRITE_FORMAT_MAX];
extern	bool		g_bOptESCEditCancel;
extern	bool		g_bOptEnableEditCursorExit;
extern	bool		g_bOptEnterBeginEdit;
extern	bool		g_bOptEditOkDown;
extern	bool		g_bOptKeepTimeStamp;
extern	bool		g_bOptSyncCreateTime;
extern	bool		g_bOptChangeFileExt;
extern	bool		g_bOptChangeTextFile;
extern	int			g_nOptCheckFileName;
extern	bool		g_bOptLoadFileChecked;
extern	bool		g_bOptHideMP3ListFile;
extern	bool		g_bOptDropSearchSubFolder; /* TyphoonSwell 026 */
extern	bool		g_bOptShowZenSpace; /* BeachMonster 107 */
extern	CString		g_sOptShowOtherChar; /* BeachMonster 107 */ // �Ƃ肠��������Ƃ�
extern	bool		g_bOptSortIgnoreCase; /* BeachMonster4 114 */
extern	bool		g_bOptSortIgnoreZenHan; /* BeachMonster4 114 */
extern	bool		g_bOptSortIgnoreKataHira; /* BeachMonster4 114 */
extern	bool		g_bOptShowTotalParent; /* RockDance 128 */
extern	bool		g_bOptShowTips; /* Rumble 188 */
extern	int			g_nOptPlayerType;
extern	CString		g_sOptWinAmpPath;
extern	bool		g_bOptEditFieldSIF;
extern	bool		g_bOptAutoOpenFolder;
extern	bool		g_bOptLoadFileAdjustColumn;
extern	bool		g_bOptSearchLyricFile;
extern	bool		g_bOptSetLyricsDir;
extern	bool		g_bOptSearchLyricsSubDir;
extern	bool		g_bEnableSearchSubDir;
extern	bool		g_bEnableMoveFolderCopy;
extern	CString		g_strOptLyricsPath;
extern	SORT_STATE	g_sortState[SORT_KEY_MAX];
extern	CLASS_INFO	g_classInfo;

//extern	bool		g_bOptID3v2GenreAddNumber;
extern	bool		g_bConfFileNameMaxCheck;
extern	int			g_nConfFileNameMaxChar;
extern	bool		g_bFileNameMaxCellColor; /* SeaKnows 036 */

extern	bool		g_bEnableFolderSync;
extern	CString		g_strRootFolder;
extern	bool		g_bSyncSelectAlways;
extern	bool		g_bSyncDeleteFolder;
extern	bool		g_bSyncLyricsFileMove;

extern	bool		g_bConfDeleteFile;
extern	bool		g_bConfDeleteList;
extern	bool		g_bConfEditModify;
extern	bool		g_bConfFolderSync;

extern	bool		g_bPlayListClearList;
extern	bool		g_bPlayListClearCheck;
extern	bool		g_bPlayListAddList;
extern	bool		g_bPlayListFileCheck;

/* STEP 035 *///extern	CString		g_strTeikei[10+20];	/* SeaKnows 030 *//* FreeFall 046 */
extern	CString		g_strTeikeiGroupName[3];	/* FreeFall 046 */
extern	bool		g_bValidFolderSelect;	/* SeaKnows 033 */
extern	bool		g_bValidDupExec;	/* FreeFall 045 */

extern	int			g_nRecentFolder; /* StartInaction 053 */
extern	bool		g_bSaveRepDlgPos; /* WildCherry4 086 */
extern	int			g_nSaveRepDlgPosX; /* WildCherry4 086 */
extern	int			g_nSaveRepDlgPosY; /* WildCherry4 086 */

// �����퓝�� /* StartInaction 054 */
extern	UINT		g_nUnifyAlpha;
extern	UINT		g_nUnifyAlpha;
extern	UINT		g_nUnifyHiraKata;
extern	UINT		g_nUnifyKata;
extern	UINT		g_nUnifyKigou;
extern	UINT		g_nUnifySuji;
extern	UINT		g_nUnifyUpLow;
extern	UINT		g_nUnifyFixedUpLow; /* STEP 040 */

// �t�@�C���������퓝�� /* LastTrain 058 */
extern	UINT		g_nFileUnifyAlpha;
extern	UINT		g_nFileUnifyHiraKata;
extern	UINT		g_nFileUnifyKata;
extern	UINT		g_nFileUnifyKigou;
extern	UINT		g_nFileUnifySuji;
extern	UINT		g_nFileUnifyUpLow;

// �g���q�ϊ� /* STEP 006 */
extern	UINT		g_nFileExtChange;

extern	bool		g_bEndEditMoveRightCell; /* BeachMonster 091 */

extern	bool		g_bShowLoadPlaylistDlg; /* RockDance 126 */

extern	CString		g_strFavorite[10];	/* RockDance 129 */

extern	int			g_nAddNumberWidth; /* Baja 159 */
extern	int			g_nAddNumberPos; /* Baja 159 */
extern	CString		g_strAddNumberSep; /* Baja 159 */
extern	CString		g_strAddNumberBef; /* Conspiracy 194 */
extern	CString		g_strAddNumberAft; /* Conspiracy 194 */

extern	bool		g_bAudioListShow; /* Conspiracy 199 */

extern bool			g_bFirstUpperIgnoreWord; /* STEP 026 */
extern CString		g_strFirstUpperIgnoreWords; /* STEP 026 */
extern CString		g_strFirstUpperSentenceSeparator; /* STEP 026 */
extern bool			g_bUserConvAddMenu; /* STEP 030 */
extern bool			g_bZenHanKigouKana; /* STEP 016 */

extern	LOGFONT		g_fontReport;			// �t�H���g

extern	FILENAME_REPLACE	g_fileNameReplace[FILENAME_REPLACE_MAX];

extern	FILENAME_REPLACE	g_userFileNameReplace[USER_FILENAME_REPLACE_MAX];	/* FreeFall 050 */


extern	KEY_CONFIG *SearchKeyConfigID(WORD);

extern	CStringArray g_arFixedWords; /* STEP 040 */

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SuperTagEditor.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSuperTagEditorApp : public CWinApp
{
public:
	void WriteFixedWordList();
	CSuperTagEditorApp();
	~CSuperTagEditorApp();

protected:
	BOOL OnOpenRecentFile(UINT nID); /* BeachMonster 104 */
	HANDLE	m_hMutex;			// ���d�N���֎~�����p��Mutex
	HACCEL	m_hAccel;			// �A�N�Z�����[�^�e�[�u���̃n���h��
	ACCEL	*m_accelTable;		// �A�N�Z�����[�^�e�[�u��
	int		m_nAccelTable;		// �A�N�Z�����[�^�e�[�u���� /* STEP 030 }\

public:
	void AddToRecentFileList(LPCTSTR lpszPathName);
	inline	HACCEL	GetAccelHandle(void) {return(m_hAccel);}
	tstring	MakeFileName(TCHAR *) const;
	tstring	MakeFileName(LPCTSTR, LPCTSTR) const;
	void	ReadWindowStatus(LPCTSTR, RECT *);
	void	WriteWindowStatus(LPCTSTR, RECT *);
	void	ReadRegistry(void);
	void	WriteRegistry(void);
	void	ReadKeyConfig(bool = false);
	void	WriteKeyConfig(bool = false);
	void	ReadGenreList(void);
	void	WriteGenreList(void);
	void	UpdateAccelerator(int = -1);
	void	DestroyAccelerator(void);
	void	GetWinampPath(void);
	void	ReadFixedWordList(void);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSuperTagEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CSuperTagEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateFileMruFile(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder01();
	afx_msg void OnUpdateFavoriteFolder01(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder02();
	afx_msg void OnUpdateFavoriteFolder02(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder03();
	afx_msg void OnUpdateFavoriteFolder03(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder04();
	afx_msg void OnUpdateFavoriteFolder04(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder05();
	afx_msg void OnUpdateFavoriteFolder05(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder06();
	afx_msg void OnUpdateFavoriteFolder06(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder07();
	afx_msg void OnUpdateFavoriteFolder07(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder08();
	afx_msg void OnUpdateFavoriteFolder08(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder09();
	afx_msg void OnUpdateFavoriteFolder09(CCmdUI* pCmdUI);
	afx_msg void OnFavoriteFolder10();
	afx_msg void OnUpdateFavoriteFolder10(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnFavoriteFolder(CString& strFolder);
	void LoadStdProfileSettings(UINT nMaxMRU);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMTLOGVIEW_H__D67B6444_BFAB_11D3_9459_00402641B29B__INCLUDED_)
