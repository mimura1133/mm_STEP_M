// ======================================================
// �T  �v    �F �l�o�R�t�@�C���Ǘ��N���X
// �^�[�Q�b�g�F Windows95/98/NT
// �����n    �F Visual C++ Ver 6.0
// �쐬��    �F MERCURY
// �쐬��    �F 00/01/18(��)
// ���쌠�\�L�F Copyright(C) 2000 MERCURY.
// ======================================================

#ifndef __CFILEMP3_H__
#define __CFILEMP3_H__


// ======================================
// =====   �C���N���[�h�t�@�C��     =====
// ======================================
//#include "vbmp3.h"
#include "STEP_api.h"

// ======================================
// =====           ��  ��           =====
// ======================================

//#define RMP_SIF_LENGTH			2048/*LIST_SIZE VMMP3�̒�` */	// RIFF MP3 �� SIF �̍ő�̒���

// ======================================
// =====           �}�N��           =====
// ======================================
// CString �e�L�X�g�̕������𐧌�����
#define LIMIT_TEXT_LENGTH(sText, nLen)	{	\
	TCHAR	sWorkBuffer[LIST_SIZE+1];		\
	StringCopyN(sWorkBuffer, sText, nLen);	\
	sWorkBuffer[nLen] = '\0';				\
	sText = sWorkBuffer;					\
}



// ======================================
// =====       typedef�^enum        =====
// ======================================



// ======================================
// =====       �\���́^���p��       =====
// ======================================
struct	FILE_MP3	{
	bool	bModifyFlag;				// �O������ҏW���ꂽ���ǂ����̃t���O
	bool	bCheckFlag;					// �`�F�b�N���
	long	lFileSize;					// �t�@�C���T�C�Y
	CTime	time;						// �X�V��
	CTime	timeCreate;					// �쐬��
	int		nFormat;					// �t�@�C���t�H�[�}�b�g(FILE_FORMAT_xxxxx)
	CString	strFullPathName;			// �t�@�C����(�t���p�X)
	CString	strFileName;				// strFullPathName �̃t�@�C��������
	CString	strFilePath;				// strFullPathName �̃p�X����
	int		nPlayTime;					// ���t����(�P�ʁF�b)
	bool	bExistLyricFile;			// �̎��t�@�C���̑���
///* Conspiracy 198 */	CString strBitRate;					// �r�b�g���[�g
	CString strAudioFormat;				// �����t�H�[�}�b�g
	CString strVisualFormat;			// �f���t�H�[�}�b�g

	// ID3 tag ���
	CString	strTrackName;				// �g���b�N��
	CString	strArtistName;				// �A�[�e�B�X�g��
	CString	strAlbumName;				// �A���o����
	CString	strComment;					// �R�����g
	CString	strYear;					// �����[�X�N��
	BYTE	byTrackNumber;				// �g���b�N�ԍ�(���l�ҏW�p)
	CString	strTrackNumber;				// �g���b�N�ԍ�(������ҏW�p)
	BYTE	byDiskNumber;				// �f�B�X�N�ԍ�(���l�ҏW�p)
	CString	strDiskNumber;				// �f�B�X�N�ԍ�(������ҏW�p)
	BYTE	byGenre;					// �W�������ԍ�
	CString	strGenre;					// �W��������

	// RIFF MP3 �� SI �t�B�[���h(ID3v2 �Ȃǂł��g��)
	CString	strTrackNameSI;				// �Ȗ�
	CString	strArtistNameSI;			// �A�[�e�B�X�g��
	CString	strAlbumNameSI;				// ���i��
	CString	strCommentSI;				// �R�����g������
	CString	strYearSI;					// �����[�X�N��
	CString	strTrackNumberSI;			// �g���b�N�ԍ�
	CString strDiskNumberSI;			// �f�B�X�N�ԍ�
	CString	strGenreSI;					// �W��������
	CString	strCopyrightSI;				// ���쌠
	CString	strEngineerSI;				// �G���W�j�A
	CString	strSourceSI;				// �\�[�X
	CString	strSoftwareSI;				// �\�t�g�E�F�A
	CString	strKeywordSI;				// �L�[���[�h
	CString	strTechnicianSI;			// �Z�p��
	CString	strLyricSI;					// �̎�
	CString	strCommissionSI;			// �R�~�b�V����
	CString strWriterSI;				// �쎌 /* ADD MIMURA */
	CString strComposerSI;				// ��� /* Baja 154 */
	CString strAlbmArtistSI;			// Albm.�A�[�e�B�X�g /* ADD MIMURA */
	CString strOrigArtistSI;			// Orig.�A�[�e�B�X�g /* Baja 154 */
	CString strURLSI;					// URL /* Baja 154 */
	CString strEncodest;				// �G���R�[�h�����l /* Baja 154 */
	CString	strOther;					// ���̑�(OggVorbis�̔�W���^�O) /* Conspiracy 196 */
	CString strEngineer;				// �G���W�j�A�i�o�Łj/* ADD MIMURA */

	void*	fileSpecificInfo;			// �t�@�C���ŗL���

	CString strFileTypeName;			// �t�@�C���^�C�v������

	void *pTreeItem;
};

struct	GENRE_LIST	{		// �W���������X�g
	bool	bAddList;		// ���X�g�ɒǉ����邩�ǂ���
	BYTE	byGenre;		// �W�������ԍ�
	LPCTSTR	sName;			// �W��������(NULL:���X�g�I�[)
};
struct	USER_GENRE_LIST	{	// ���[�U�W���������X�g
	bool	bAddList;		// ���X�g�ɒǉ����邩�ǂ���
	BYTE	byGenre;		// �W�������ԍ�
	CString	sName;			// �W��������
	bool	bUse;
};
extern	GENRE_LIST	g_genreListSCMPX[];
extern	USER_GENRE_LIST*	g_genreListUSER;
#define USER_GENRE_LIST_MAX 50


// ======================================
// =====         const �ϐ�         =====
// ======================================



// ======================================
// =====        extern �ϐ�         =====
// ======================================



// ======================================
// =====        static �ϐ�         =====
// ======================================



// ======================================
// =====     �֐��v���g�^�C�v       =====
// ======================================
extern	const char *GetGenreName(BYTE);
extern	LPCTSTR GetGenreNameSIF(BYTE byGenre);
extern	int		GetGenreCode(LPCTSTR);
extern bool IsUserGenre(LPCTSTR sGenre);
extern	void	StringCopyN(LPTSTR, LPCTSTR, int, BOOL = TRUE);
extern	bool	IsFolderName(LPCTSTR);



// ======================================
// =====      �v���O�����̈�        =====
// ======================================



#ifdef __cplusplus

class	CFileMP3	{
public:
			FILE_MP3	m_fileMP3;
public:
	CFileMP3();
	virtual	~CFileMP3();

public:		// ======================================
			// =====      public �����o�֐�     =====
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
			// =====     private �����o�֐�     =====
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
			// =====    protected �����o�֐�    =====
			// ======================================
/* RockDance 133 */
//	static	bool	ReadTagAPE(LPCSTR, FILE_MP3 *);
//	static	bool	WriteTagAPE(FILE_MP3 *);
};

#endif

#endif
