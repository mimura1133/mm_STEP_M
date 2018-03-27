#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "imm.h"

#ifndef iskanji
#define iskanji(c)		((c) >= 0x81 && (c) <= 0x9f || (c) >= 0xe0 && (c) <= 0xfc)
#endif

enum	{CONV_SUJI=1, CONV_ALPHA=2, CONV_KATA=4, CONV_KIGOU=8, CONV_ALL=15};

#ifdef __cplusplus
extern "C" {
#endif

extern	int conv_han2zens(unsigned char *, const unsigned char *, int);
extern	int conv_zen2hans(unsigned char *, const unsigned char *, int);
extern	void conv_kata2hira(unsigned char *);
extern	void conv_kata_erase_dakuon(unsigned char *);
extern	void conv_hira2kata(unsigned char *);
extern	void conv_upper(unsigned char *);
extern	void conv_lower(unsigned char *);
extern	void conv_first_upper(unsigned char *, const char *, const char *, bool);
//extern	DWORD conv_kan2hira(HWND, unsigned char *, DWORD);
//extern	void conv_romaji(HWND hwnd, unsigned char *str, unsigned char *sRomaji);
#ifdef __cplusplus
}
#endif

unsigned short han2zen(int c, int flag)
{
	if (c <= 0x19 || (c >= 0x80 && c <= 0xa0) || c >= 0xe0) {
		return c;					// 制御未定義
	}
	if (flag & CONV_ALPHA) {
		if (c >= 'A' && c <= 'Z') return(0x8260U + (c - 'A'));	// 英大文字
		if (c >= 'a' && c <= 'z') return(0x8281U + (c - 'a'));	// 英小文字
	}
	if (flag & CONV_SUJI) {
		if (c >= '0' && c <= '9') return(0x824FU + (c - '0'));	// 数字
	}

	int		i;
	static const unsigned char *s1 = (const unsigned char *)
		/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
		  "   ,. :;?!ﾞﾟ   "		/* 212X:　　、。，．・：；？！゛゜´｀¨ */
		  /* STEP 016 *///" ､｡,.･:;?!ﾞﾟ   "		/* 212X:　　、。，．・：；？！゛゜´｀¨ */
		  /* BeachMonster 102 *///"^~_         ｰ- /"	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  /* Baja 169 *///"^~_          - /"	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  "^~_            /"	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  "   |  `\' \"()  []"	/* 214X:＼～∥｜…‥‘’“”（）〔〕［］ */
		  "{}          +-   "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  /* STEP 016 *///"{}    ｢｣    +-   "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  " = <>          \\"	/* 216X:÷＝≠＜＞≦≧∞∴♂♀°′″℃￥ */
		  "$  %#&*@        ";	/* 217X:＄￠￡％＃＆＊＠§☆★○●◎◇   */
	static const unsigned char *s2 = (const unsigned char *)
		/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
		  "ｧｱｨｲｩｳｪｴｫｵｶﾞｷﾞｸ"		/* 252X:　ァアィイゥウェエォオカガキギク */
		  "ﾞｹﾞｺﾞｻﾞｼﾞｽﾞｾﾞｿﾞﾀ"	/* 253X:グケゲコゴサザシジスズセゼソゾタ */
		  "ﾞﾁﾞｯﾂﾞﾃﾞﾄﾞﾅﾆﾇﾈﾉﾊ"	/* 254X:ダチヂッツヅテデトドナニヌネノハ */
		  "ﾞﾟﾋﾞﾟﾌﾞﾟﾍﾞﾟﾎﾞﾟﾏﾐ "	/* 255X:バパヒビピフブプヘベペホボポマミ7F */
		  "ﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛ ﾜ"	/* 256X:ムメモャヤュユョヨラリルレロヮワ */
		  "  ｦﾝ";				/* 257X:ヰヱヲンヴヵヶ */
	static const unsigned char *s3 = (const unsigned char *) /* BeachMonster 102 */
		/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
		  "                "	/* 212X:　　、。，．・：；？！゛゜´｀¨ */
		  "            ｰ   "	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  "                "	/* 214X:＼～∥｜…‥‘’“”（）〔〕［］ */
		  "                "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  "                "	/* 216X:÷＝≠＜＞≦≧∞∴♂♀°′″℃￥ */
		  "                ";	/* 217X:＄￠￡％＃＆＊＠§☆★○●◎◇   */
		 ;
	static const unsigned char *s4 = (const unsigned char *) /* STEP 016 */
		/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
		  "  ､｡  ･         "	/* 212X:　　、。，．・：；？！゛゜´｀¨ */
		  "                "	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  "                "	/* 214X:＼～∥｜…‥‘’“”（）〔〕［］ */
		  "      ｢｣        "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  "                "	/* 216X:÷＝≠＜＞≦≧∞∴♂♀°′″℃￥ */
		  "                ";	/* 217X:＄￠￡％＃＆＊＠§☆★○●◎◇   */
		 ;
	if (flag & CONV_KIGOU) {
		for (i = 0; s1[i]; i++) if (c == s1[i]) return(0x8140U+i);	// 記号
	}
	if (flag & CONV_KATA) {
		for (i = 0; s2[i]; i++) if (c == s2[i]) return(0x8340U+i);	// カタカナ
	}
	if (flag & CONV_KATA) { /* BeachMonster 102 */
		for (i = 0; s3[i]; i++) if (c == s3[i]) return(0x8140U+i-1/* STEP 016*/);	// 記号だけどカタカナ扱い
	}
	extern bool g_bZenHanKigouKana; /* STEP 016 */
	if ((!g_bZenHanKigouKana && flag & CONV_KIGOU) 
		|| (g_bZenHanKigouKana && flag & CONV_KATA)) { /* STEP 016 */
		for (i = 0; s4[i]; i++) if (c == s4[i]) return(0x8140U+i-1);
	}
	return(c);
}

int conv_han2zens(unsigned char *zen, const unsigned char *han, int flag)
{
	int		i;
	int		pzen = 0;		// 全角位置
	int		l = strlen((const char *)han);
	zen[0] = '\0';

	for (i = 0; i < l; i++, pzen++, zen[pzen] = '\0') {
		if (han[i] == ' ') {		// 空白
			if (flag & CONV_KIGOU) {
				zen[pzen] = 0x81U;
				pzen++;
				zen[pzen] = 0x40U;
			} else {
				zen[pzen] = ' ';
			}
			continue;
		}

		if (iskanji(han[i])) {		// 漢字
			zen[pzen] = han[i];
			pzen++;
			i++;
			zen[pzen] = han[i];
			continue;
		}

		if (flag & CONV_KATA) {
			unsigned int	x;
			static const unsigned char *s1 = (const unsigned char *)
			/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
			             "ｶ ｷ ｸ"	/* 252X:　ァアィイゥウェエォオカガキギク */
			  " ｹ ｺ ｻ ｼ ｽ ｾ ｿ ﾀ"	/* 253X:グケゲコゴサザシジスズセゼソゾタ */
			  " ﾁ ｯﾂ ﾃ ﾄ ﾅﾆﾇﾈﾉﾊ"	/* 254X:ダチヂッツヅテデトドナニヌネノハ */
			  "  ﾋ  ﾌ  ﾍ  ﾎ";		/* 255X:バパヒビピフブプヘベペホボポ     */
			static const unsigned char *s2 = (const unsigned char *)
			/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
			                 "ﾊ"	/* 254X:                              ハ */
			  "  ﾋ  ﾌ  ﾍ  ﾎ";		/* 255X:バパヒビピフブプヘベペホボポ     */

			// [ｳﾞ] => [ヴ] の変換
			if (han[i] == (unsigned char)'ｳ' && han[i+1] == (unsigned char)'ﾞ') {
				zen[pzen] = 0x83U;
				pzen++;
				i++;
				zen[pzen] = 0x94U;
				goto cont;
			}

			// 濁音の変換
			for (x = 0; s1[x]; x++) {
				if (han[i] == s1[x] && han[i+1] == (unsigned char)'ﾞ') {
					zen[pzen] = 0x83U;
					pzen++;
					i++;
					zen[pzen] = 0x4BU + x;
					goto cont;
				}
			}

			// 半濁音の変換
			for (x = 0; s2[x]; x++) {
				if (han[i] == s2[x] && han[i+1] == (unsigned char)'ﾟ') {
					// 半濁音
					zen[pzen] = 0x83U;
					pzen++;
					i++;
					zen[pzen] = 0x70U + x;
					goto cont;
				}
			}
		}

		// その他の変換
		{
			unsigned short z = han2zen(han[i], flag);
			if (z >= 0x100) {
				zen[pzen] = (unsigned char)(z >> 8);
				pzen++;
				zen[pzen] = (unsigned char)z;
			} else {
				zen[pzen] = (unsigned char)z;
			}
		}
cont:	;
	}

	return(pzen);
}

unsigned short zen2han(unsigned short c, int flag)
{
	if (c <= 0x100) return(c);				// 非漢字
	if (flag & CONV_ALPHA) {
		if (c == 0x8140U) return(' ');			// 空白
		if (c >= 0x8260U && c <= 0x8279U) {		// Ａ－Ｚ
			return('A' + (c - 0x8260U));
		}
		if (c >= 0x8281U && c <= 0x829AU) {		// ａ－ｚ
			return('a' + (c - 0x8281U));
		}
	}
	if (flag & CONV_SUJI) {
		if (c >= 0x824FU && c <= 0x8258U) {		// ０－９
			return('0' + (c - 0x824FU));
		}
	}

	int		i;
	if (flag & CONV_KIGOU) {
		// なんで"～"を"-"にしてるのか覚えてないので削除 041018
		static const unsigned char *han = (const unsigned char *)
			",.:;?!ﾞﾟ^~_-/|`\'\"()[]{}+-=<>\\$%#&*@"; /* BeachMonster 102 *//* STEP 016 */
		static const unsigned char *zen = (const unsigned char *)
			"，．：；？！゛゜＾￣＿－／｜｀’”（）［］｛｝＋－＝＜＞￥＄％＃＆＊＠\0"; /* BeachMonster 102 *//* STEP 016 */
		for (i = 0; zen[i+1]; i+=2) {
			if (c == ((zen[i] << 8) | zen[i+1])) return(han[i/2]);
		}
		extern bool g_bZenHanKigouKana; /* STEP 016 */
		if (!g_bZenHanKigouKana) { /* STEP 016 */
			static const unsigned char *han2 = (const unsigned char *)
				"･､｡｢｣";
			static const unsigned char *zen2 = (const unsigned char *)
				"・、。「」\0";
			for (i = 0; zen2[i+1]; i+=2) {
				if (c == ((zen2[i] << 8) | zen2[i+1])) return(han2[i/2]);
			}
		}
	}
	if (flag & CONV_KATA) {
		static const unsigned char *han = (const unsigned char *)
			"ｧｱｨｲｩｳｪｴｫｵｶｷｸｹｺｻｼｽｾｿﾀﾁｯﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛﾜｦﾝｰ";
		static const unsigned char *kata = (const unsigned char *)
			"ァアィイゥウェエォオカキクケコサシスセソタチッツテトナニヌネノ"
			"ハヒフヘホマミムメモャヤュユョヨラリルレロワヲンー\0";
		// カタカナ
		for (i = 0; kata[i+1]; i+=2) {
			if (c == ((kata[i] << 8) | kata[i+1])) return(han[i/2]);
		}
		extern bool g_bZenHanKigouKana; /* STEP 016 */
		if (g_bZenHanKigouKana) { /* STEP 016 */
			static const unsigned char *han2 = (const unsigned char *)
				"･､｡｢｣";
			static const unsigned char *kata2 = (const unsigned char *)
				"・、。「」\0";
			for (i = 0; kata2[i+1]; i+=2) {
				if (c == ((kata2[i] << 8) | kata2[i+1])) return(han2[i/2]);
			}
		}
	}

	// 変換できなかった
	return(c);
}

int conv_zen2hans(unsigned char *han, const unsigned char *zen, int flag)
{
	int		i;
	int		phan = 0;		// 半角位置
	int		l = strlen((const char *)zen);
	han[0] = '\0';

	for (i = 0; i < l; i++, phan++, han[phan] = '\0') {
		if (!iskanji(zen[i])) {		// 非漢字
			han[phan] = zen[i];
			continue;				// 無変換
		}

		if (flag & CONV_KATA) {
			int		j;
			static const unsigned char *k1 = (const unsigned char *)"ガギグゲゴザジズゼゾダヂヅデドバビブベボヴ\0";
			static const unsigned char *h1 = (const unsigned char *)"がぎぐげござじずぜぞだぢづでどばびぶべぼ\0\0\0";
			static const unsigned char *k2 = (const unsigned char *)"パピプペポ\0";
			static const unsigned char *h2 = (const unsigned char *)"ぱぴぷぺぽ\0";
			// 濁音の変換
			for (j = 0; k1[j+1]; j+=2) {
#if 1
				if (zen[i] == k1[j] && zen[i+1] == k1[j+1]) {	// カタカナ
#else
				if ((zen[i] == k1[j] && zen[i+1] == k1[j+1])	// カタカナ
				||  (zen[i] == h1[j] && zen[i+1] == h1[j+1])) {	// ひらがな
#endif
					static const unsigned char *h = (const unsigned char *)"ｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾊﾋﾌﾍﾎｳ";
					han[phan] = h[j/2];
					phan++;
					han[phan] = (unsigned char)'ﾞ';
					i++;
					goto cont;
				}
			}

			// 半濁音の変換
			for (j = 0; k2[j+1]; j+=2) {
#if 1
				if (zen[i] == k2[j] && zen[i+1] == k2[j+1]) {	// カタカナ
#else
				if ((zen[i] == k2[j] && zen[i+1] == k2[j+1])	// カタカナ
				||  (zen[i] == h2[j] && zen[i+1] == h2[j+1])) {	// ひらがな
#endif
					static const unsigned char *h = (const unsigned char *)"ﾊﾋﾌﾍﾎ";
					han[phan] = h[j/2];
					phan++;
					han[phan] = (unsigned char)'ﾟ';
					i++;
					goto cont;
				}
			}
		}

		// その他の変換
		{
			unsigned short c;
			c = zen2han((zen[i] << 8) | zen[i+1], flag);
			i++;
			if (c >= 0x100) {
				han[phan] = (unsigned char)(c >> 8);
				phan++;
				han[phan] = (unsigned char)c;
			} else {
				han[phan] = (unsigned char)c;
			}
		}

cont:	;
	}

	return(phan);
}

static const unsigned char *kata = (const unsigned char *)
	"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾ"
	"タダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポ"
	"マミムメモャヤュユョヨラリルレロヮワヲン";
static const unsigned char *hira = (const unsigned char *)
	"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞ"
	"ただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽ"
	"まみむめもゃやゅゆょよらりるれろゎわをん";
static const unsigned char *alphaS = (const unsigned char *)
	"ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ’０１２３４５６７８９" /* STEP 026 */;
static const unsigned char *alphaL = (const unsigned char *)
	"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ’０１２３４５６７８９" /* STEP 026 */;

static const unsigned char *kata_dakuon = (const unsigned char *) /* FunnyCorn 179 */
	"ァアィイゥウェエォオカカキキククケケココササシシススセセソソ"
	"タタチチッツツテテトトナニヌネノハハハヒヒヒフフフヘヘヘホホホ"
	"マミムメモャヤュユョヨラリルレロヮワヲン";

void conv_table(const unsigned char *before, const unsigned char *after, unsigned char *str)
{
	while(*str != '\0') {
		if (iskanji(*str)) {		// 漢字
			int i; for (i = 0; before[i]; i+=2) {
				if (before[i] == str[0] && before[i+1] == str[1]) {
					str[0] = after[i];
					str[1] = after[i+1];
					break;
				}
			}
			str++;
		}
		str++;
	}
}
// [カタカナ]の濁音、半濁音をなくす
void conv_kata_erase_dakuon(unsigned char *str)
{
	conv_table(kata, kata_dakuon, str);
}
// [カタカナ]=>[ひらがな]に変換
void conv_kata2hira(unsigned char *str)
{
	conv_table(kata, hira, str);
}

// [ひらがな]=>[カタカナ]に変換
void conv_hira2kata(unsigned char *str)
{
	conv_table(hira, kata, str);
}

// [小文字]=>[大文字]に変換
void conv_upper(unsigned char *str)
{
	_mbsupr(str);
	conv_table(alphaS, alphaL, str);
}

// [大文字]=>[小文字]に変換
void conv_lower(unsigned char *str)
{
	_mbslwr(str);
	conv_table(alphaL, alphaS, str);
}

void lower_suffix_word(unsigned char *str, int len, CString suffixs) /* STEP 026*/
{
	CString strZWord((LPCTSTR)str, len);
	CString strWord;
	conv_zen2hans((unsigned char *)strWord.GetBuffer(len*2+1), (const unsigned char *)(const char *)strZWord, CONV_ALL);
	strWord.ReleaseBuffer();
	strWord.MakeLower();
	while (1) {
		CString suffix = suffixs.SpanExcluding(",");
		if (suffix == strWord) {
			if (iskanji(*str)) {		// 漢字
				int i; for (i = 0; alphaS[i]; i+=2) {
					if ((alphaS[i] == str[0] && alphaS[i+1] == str[1])
					||  (alphaL[i] == str[0] && alphaL[i+1] == str[1])) {
						str[0] = alphaS[i];
						str[1] = alphaS[i+1];
						break;
					}
				}
			} else {
				*str = tolower(*str);
			}
			return;
		}
		suffixs.TrimLeft(suffix);
		suffixs.TrimLeft(',');
		if (suffixs.GetLength() == 0) {
			return;
		}
	}
}

bool isSentenceSeparate(unsigned char* str, int len, CString separator) /* STEP 026 */
{
	CString strZWord((LPCTSTR)str, len);
	CString strChar;
	conv_zen2hans((unsigned char *)strChar.GetBuffer(2+1), (const unsigned char *)(const char *)strZWord, CONV_ALL);
	strChar.ReleaseBuffer();
	for (int i=0;i<separator.GetLength();i++) {
		unsigned char c = separator.GetAt(i);
		if (iskanji(c)) {		// 漢字
			char	kanji[3];
			kanji[0] = separator.GetAt(i);
			kanji[1] = separator.GetAt(i+1);
			kanji[2] = '\0';
			if (strChar == kanji) {
				return true;
			}
		} else {
			if (strChar == separator.GetAt(i)) {
				return true;
			}
		}
	}
	return false;
}

// 単語の１文字目のみ、[小文字]=>[大文字]に変換
void conv_first_upper(unsigned char *str, const char *suffixs, const char* separator , bool bUseSuffix)
{
	bool	bFirst = true;
	unsigned char*	pFirstPos = NULL; /* STEP 026 */
	unsigned char*	pEndPos = NULL; /* STEP 026 */
	bool	bFirstWord = true; /* STEP 026 */
	while(TRUE) {
		if (iskanji(*str)) {		// 漢字
			int i; for (i = 0; alphaS[i]; i+=2) {
				if ((alphaS[i] == str[0] && alphaS[i+1] == str[1])
				||  (alphaL[i] == str[0] && alphaL[i+1] == str[1])) {
					if (bFirst) {
						str[0] = alphaL[i];
						str[1] = alphaL[i+1];
						pFirstPos = str; /* STEP 026 */
					}
					bFirst = false;
					break;
				}
			}
			if (alphaS[i] == '\0') {	// 単語の区切り
				pEndPos = str-1; /* STEP 026 */
				if (pFirstPos != NULL && pEndPos != NULL && !bFirstWord && bUseSuffix) { /* STEP 026 */
					lower_suffix_word(pFirstPos, pEndPos - pFirstPos + 1, suffixs);
				}
				pFirstPos = NULL; /* STEP 026 */
				pEndPos = NULL; /* STEP 026 */
				if (!bFirst) bFirstWord = FALSE; /* STEP 026 */
				if (bUseSuffix) { /* STEP 026 */
					if (isSentenceSeparate(str, 2, separator)) {
						bFirstWord = TRUE;
					}
				}
				bFirst = true;
			}
			if (*str == '\0')	break; /* STEP 026 */
			str++;
		} else {					// 非漢字
			if ((*str >= 'a' && *str <= 'z')
			||  (*str >= 'A' && *str <= 'Z')
			||  (*str >= '0' && *str <= '9') /* STEP 026 2005.12.02 */
			||  (*str == '\'')) {
				if (bFirst) {
					*str = toupper(*str);
					bFirst = false;
					pFirstPos = str; /* STEP 026 */
				}
			} else {
				// 単語の区切り
				pEndPos = str-1;
				if (pFirstPos != NULL && pEndPos != NULL && !bFirstWord && bUseSuffix) { /* STEP 026 */
					lower_suffix_word(pFirstPos, pEndPos - pFirstPos + 1, suffixs);
				}
				pFirstPos = NULL; /* STEP 026 */
				pEndPos = NULL; /* STEP 026 */
				if (!bFirst) bFirstWord = FALSE; /* STEP 026 */
				if (bUseSuffix) { /* STEP 026 */
					if (isSentenceSeparate(str, 1, separator)) {
						bFirstWord = TRUE;
					}
				}
				if (*str == '\n') {
					bFirstWord = TRUE;
				}
				bFirst = true;
			}
		}
		if (*str == '\0')	break; /* STEP 026 */
		str++;
	}
}

bool isKigou(char ch) {
	static const unsigned char *s1 = (const unsigned char *)
		/* 0123456789ABCDEF		        ０１２３４５６７８９ＡＢＣＤＥＦ */
		  "   ,. :;?!ﾞﾟ   "		/* 212X:　　、。，．・：；？！゛゜´｀¨ */
		  "^~_            /"	/* 213X:＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／ */
		  "   |  `\' \"()  []"	/* 214X:＼～∥｜…‥‘’“”（）〔〕［］ */
		  "{}          +-   "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  /* STEP 016 *///"{}    ｢｣    +-   "	/* 215X:｛｝〈〉《》「」『』【】＋－±×7F */
		  " = <>          \\"	/* 216X:÷＝≠＜＞≦≧∞∴♂♀°′″℃￥ */
		  "$  %#&*@        ";	/* 217X:＄￠￡％＃＆＊＠§☆★○●◎◇   */
	for (int i=0;s1[i]!='\0';i++) {
		if (ch == s1[i]) {
			return true;
		}
	}
	return false;
}

unsigned char* fixed_upper_lower(unsigned char *str, CStringArray& fixedWords) /* STEP 040*/
{
	CString strZWord((LPCTSTR)str);
	CString strWord;
	conv_zen2hans((unsigned char *)strWord.GetBuffer(strlen((char*)str)+1), (const unsigned char *)(const char *)strZWord, CONV_ALL);
	strWord.ReleaseBuffer();
	strWord.MakeLower();
	for (int i=0;i<fixedWords.GetSize();i++) {
		CString fixed = fixedWords.GetAt(i);
		CString strFixZWord(fixed);
		CString strFixWord;
		conv_zen2hans((unsigned char *)strFixWord.GetBuffer(strFixZWord.GetLength()+1), (const unsigned char *)(const char *)strFixZWord, CONV_ALL);
		strFixWord.ReleaseBuffer();
		fixed = strFixWord;
		fixed.MakeLower();
//		if (fixed == strWord) {
		if (strWord.Find(fixed, 0) == 0 && (strWord == fixed || (strWord.GetLength() >= fixed.GetLength() && isKigou(strWord.GetAt(strlen(fixed)))) )) {
			unsigned char* pos = str;
			for (int j=0;j<fixed.GetLength();j++) {
				if (iskanji(*pos)) {		// 漢字
					for (int k = 0; alphaS[k]; k+=2) {
						if ((alphaS[k] == pos[0] && alphaS[k+1] == pos[1])
						||  (alphaL[k] == pos[0] && alphaL[k+1] == pos[1])) {
							if (isupper(strFixWord.GetAt(j))) {
								pos[0] = alphaL[k];
								pos[1] = alphaL[k+1];
							} else {
								pos[0] = alphaS[k];
								pos[1] = alphaS[k+1];
							}
							break;
						}
					}
					pos += 2;
				} else {
					*pos = strFixWord.GetAt(j);
					pos++;
				}
			}
			return pos;
		}
	}
	return NULL;
}

void conv_fixed_upper_lower(unsigned char *str, CStringArray& fixedWords) /* STEP 040 */
{
	unsigned char* current = str;
	bool bConv = false;
	while (TRUE) {
		if ((current = fixed_upper_lower(str, fixedWords))) {
			bConv = true;
			str = current;
		} else {
			bConv = false;
			current = str;
		}
		if (*str == '\0')	break;
		if (bConv)	continue;
		//if (iskanji(*str)) {		// 漢字
		//	str++;
		//}
		//str++;
		bool bBreak;
		if (current == str)	{
			bBreak = TRUE;
		}
		while(TRUE) {
			if (*str == '\0')	break;
			if (iskanji(*str)) {		// 漢字
				int i; for (i = 0; alphaS[i]; i+=2) {
					if ((alphaS[i] == str[0] && alphaS[i+1] == str[1])
					||  (alphaL[i] == str[0] && alphaL[i+1] == str[1])) {
						break;
					}
				}
				if (alphaS[i] == '\0') {	// 単語の区切り
					str += 2;
					continue;
				} else {
					if (!bBreak) {
						break;
					}
					str += 2;
				}
			} else {					// 非漢字
				if ((*str >= 'a' && *str <= 'z')
				||  (*str >= 'A' && *str <= 'Z')
				||  (*str >= '0' && *str <= '9')
				||  (*str == '\'')) {
					if (!bBreak) {
						break;
					}
					str++;
				} else {
					str++;
					if (bBreak) {
						break;
					}
					continue;
				}
			}
		}
	}
}
