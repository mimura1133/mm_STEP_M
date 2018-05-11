#pragma once

// strcnv.cpp �̐錾
enum { CONV_SUJI = 1, CONV_ALPHA = 2, CONV_KATA = 4, CONV_KIGOU = 8, CONV_ALL = 15 };
#ifdef __cplusplus
extern "C" {
#endif
	extern	int conv_han2zens(LPTSTR, LPCTSTR, int);
	extern	int conv_zen2hans(LPTSTR, LPCTSTR, int);
	extern	void conv_kata2hira(LPTSTR);
	extern	void conv_kata_erase_dakuon(LPTSTR);
	extern	void conv_hira2kata(LPTSTR);
	extern	void conv_upper(LPTSTR);
	extern	void conv_lower(LPTSTR);
	extern	void conv_first_upper(LPTSTR, LPCTSTR, LPCTSTR, bool);

	//extern	DWORD conv_kan2hira(HWND, unsigned char *, DWORD);
	//extern	void conv_romaji(HWND hwnd, unsigned char *str, unsigned char *sRomaji);

	extern	void conv_fixed_upper_lower(LPTSTR, CStringArray&); /* STEP 040 */
#ifdef __cplusplus
}
#endif
