// Id3tagv1.h: CId3tagv1 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_)
#define AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER -1
#endif /* PlatformSDK���Â��̂ˁ` */

#include <array>
#include <cstdint>

class CId3tagv1  
{
public:
	CId3tagv1(BOOL bScmpxGenre = FALSE);
	virtual ~CId3tagv1();
	void SetScmpxGenre(BOOL bSwitch);
	BOOL IsEnable(){return m_bEnable;};

	CString GetTitle() const;
	void SetTitle(const char *title);
	CString GetArtist() const;
	void SetArtist(const char *artist);
	CString GetAlbum() const;
	void SetAlbum(const char *album);
	CString GetYear() const;
	void SetYear(const char *year);
	unsigned char GetGenreNum();
	CString GetGenre();
	void SetGenre(unsigned char cGenre);
	void SetGenre(const char *szGenre);
	unsigned char GetTrackNoNum();
	CString GetTrackNo();
	void SetTrackNo(unsigned char cTrackNo);
	void SetTrackNo(const char *szTrackNo);
	CString GetComment() const;
	void SetComment(const char *comment);

	CString GenreNum2String(unsigned char cGenre);
	long GenreString2Num(const char *szGenre);

	DWORD Load(LPCTSTR szFileName);
	DWORD LoadMulti(LPCTSTR szFileName);
	DWORD Save(HWND hWnd, LPCTSTR szFileName);
	DWORD DelTag(HWND hWnd, LPCTSTR szFileName);
	void GetId3tagString(char(&szTag)[128]);
	DWORD MakeTag(HWND hWnd, LPCTSTR szFileName);

private:
	void Release();
	BOOL m_bEnable;			// ID3TAG�������ꍇ��FALSE
	BOOL m_bScmpxGenre;		// SCMPX�g���W���������g�p����
	std::array<std::int8_t, 30 + 1> m_szTitle;		// �^�C�g��
	std::array<std::int8_t, 30 + 1> m_szArtist;		// �A�[�e�B�X�g
	std::array<std::int8_t, 30 + 1> m_szAlbum;		// �A���o��
	std::array<std::int8_t, 4 + 1> m_szYear;		// ����
	std::uint8_t m_cGenre;							// �W������(0xFF=N/A)
	std::array<std::int8_t, 30 + 1> m_szComment;	// �R�����g
	std::uint8_t m_cTrackNo;						// �g���b�N�ԍ�(1�`255 0=N/A)

};

#endif // !defined(AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_)
