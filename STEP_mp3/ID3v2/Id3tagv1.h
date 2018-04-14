// Id3tagv1.h: CId3tagv1 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_)
#define AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER -1
#endif /* PlatformSDKが古いのね～ */

#include <array>
#include <cstdint>

class CId3tagv1  
{
public:
	CId3tagv1(BOOL bScmpxGenre = FALSE);
	virtual ~CId3tagv1();
	void SetScmpxGenre(BOOL bSwitch);
	BOOL IsEnable(){return m_bEnable;};

	CStringA GetTitle() const;
	void SetTitle(const char *title);
	CStringA GetArtist() const;
	void SetArtist(const char *artist);
	CStringA GetAlbum() const;
	void SetAlbum(const char *album);
	CStringA GetYear() const;
	void SetYear(const char *year);
	unsigned char GetGenreNum();
	CString GetGenre();
	void SetGenre(unsigned char cGenre);
	void SetGenre(const char *szGenre);
	unsigned char GetTrackNoNum();
	CString GetTrackNo();
	void SetTrackNo(unsigned char cTrackNo);
	void SetTrackNo(const char *szTrackNo);
	CStringA GetComment() const;
	void SetComment(const char *comment);

	CString GenreNum2String(unsigned char cGenre);
	long GenreString2Num(const char *szGenre);

	DWORD Load(LPCTSTR szFileName);
	DWORD LoadMulti(LPCTSTR szFileName);
	DWORD Save(HWND hWnd,const char *szFileName);
	DWORD DelTag(HWND hWnd, LPCTSTR szFileName);
	void GetId3tagString(char *szTag);
	DWORD MakeTag(HWND hWnd, LPCTSTR szFileName);

private:
	void Release();
	BOOL m_bEnable;			// ID3TAGが無い場合はFALSE
	BOOL m_bScmpxGenre;		// SCMPX拡張ジャンルを使用する
	std::array<std::int8_t, 30 + 1> m_szTitle;		// タイトル
	std::array<std::int8_t, 30 + 1> m_szArtist;		// アーティスト
	std::array<std::int8_t, 30 + 1> m_szAlbum;		// アルバム
	std::array<std::int8_t, 4 + 1> m_szYear;		// 西暦
	std::uint8_t m_cGenre;							// ジャンル(0xFF=N/A)
	std::array<std::int8_t, 30 + 1> m_szComment;	// コメント
	std::uint8_t m_cTrackNo;						// トラック番号(1～255 0=N/A)

};

#endif // !defined(AFX_ID3TAGV1_H__81689DF7_1DE7_47C6_A34A_41AFEECC3184__INCLUDED_)
