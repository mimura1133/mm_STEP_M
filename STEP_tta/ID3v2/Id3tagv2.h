// Id3tagv2.h: CId3tagv2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)
#define AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

static WORD ExtractI2(unsigned char buf[2])
{
	WORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	return x;
}

static DWORD ExtractI4(unsigned char buf[4])
{
	DWORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	x <<= 8;
	x |= buf[2];
	x <<= 8;
	x |= buf[3];
	return x;
}

class CId3Frame  
{
public:
	CId3Frame()
	{
		m_data = NULL;
		m_dwId = 0;
		m_dwSize = 0;
		m_wFlags = 0;
	};
	virtual ~CId3Frame()
	{
		Release();
	};
	void Release()
	{
		if(m_data)
		{
			free(m_data);
		}
		m_data = NULL;
		m_dwId = 0;
		m_dwSize = 0;
		m_wFlags = 0;
	}
	CId3Frame(const CId3Frame &obj)	//コピーコンストラクタ
	{
		m_data = (unsigned char *)malloc(obj.m_dwSize+1/* STEP */);
		ZeroMemory(m_data, obj.m_dwSize+1); /* STEP */
		memcpy(m_data,obj.m_data,obj.m_dwSize);
		m_dwId = obj.m_dwId;
		m_dwSize = obj.m_dwSize;
		m_wFlags = obj.m_wFlags;
	};
	DWORD LoadFrame2_4(unsigned char *pData,DWORD dwSize)
	{
		Release();
		if(dwSize < 10)
		{
			return 0;	//フレームヘッダがない場合は終了
		}
		DWORD size = (((DWORD )(pData[4]&0x7f)<<21) | ((DWORD )(pData[5]&0x7f)<<14) | ((DWORD )(pData[6]&0x7f)<<7) | ((DWORD )(pData[7]&0x7f)));
		if((size+10) > dwSize)
		{
			return 0;	//ヘッダサイズが入力データを超過している
		}
		DWORD dwId;
		memcpy(&dwId,pData,sizeof(dwId));
		if(!dwId)
		{
			return 0;	//無効なフレームID
		}
		m_data = (unsigned char *)malloc(size+1/* STEP */);
		if(!m_data)
		{
			return 0;	//メモリを確保できなかった
		}
		ZeroMemory(m_data, size+1); /* STEP */
		m_dwSize = size;
		m_dwId = dwId;
		m_wFlags = ExtractI2(&pData[8]);
		memcpy(m_data,&pData[10],size);
		return (size + 10);
	};
	DWORD LoadFrame2_3(unsigned char *pData,DWORD dwSize)
	{
		Release();
		if(dwSize < 10)
		{
			return 0;	//フレームヘッダがない場合は終了
		}
		DWORD size = (((DWORD )pData[4]<<24) | ((DWORD )pData[5]<<16) | ((DWORD )pData[6]<<8) | (DWORD )pData[7]);
		if((size+10) > dwSize)
		{
			return 0;	//ヘッダサイズが入力データを超過している
		}
		DWORD dwId;
		memcpy(&dwId,pData,sizeof(dwId));
//	BYTE id[5];
//	memcpy(id,pData,sizeof(dwId));
//	id[4] = '\0';
//	TRACE("id=%s (size=%d)\n",id,size);
		if(!dwId)
		{
			return 0;	//無効なフレームID
		}
		m_data = (unsigned char *)malloc(size+1/* STEP */);
		if(!m_data)
		{
			return 0;	//メモリを確保できなかった
		}
		ZeroMemory(m_data, size+1); /* STEP */
//		memcpy(&m_dwId,pData,sizeof(m_dwId));
		m_dwSize = size;
		m_dwId = dwId;
		m_wFlags = ExtractI2(&pData[8]);
		memcpy(m_data,&pData[10],size);
		return (size + 10);
	};
	DWORD LoadFrame2_2(unsigned char *pData,DWORD dwSize)
	{
		Release();
		if(dwSize < 6)
		{
			return 0;	//フレームヘッダがない場合は終了
		}
		DWORD size = (((DWORD )pData[3]<<16) | ((DWORD )pData[4]<<8) | (DWORD )pData[5]);
		if((size+6) > dwSize)
		{
			return 0;	//ヘッダサイズが入力データを超過している
		}
		BYTE id[3+1];
		memcpy(&id,pData,sizeof(id));
		id[3] = '\0';
		TRACE("id=%s (size=%d)\n",id,size);
		//v2.2からv2.3へフレームIDを変換
		if(memcmp(id,"TT2",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TIT2",sizeof(m_dwId));
		}
		else if(memcmp(id,"TRK",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TRCK",sizeof(m_dwId));
		}
		else if(memcmp(id,"TP1",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPE1",sizeof(m_dwId));
		}
		else if(memcmp(id,"TAL",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TALB",sizeof(m_dwId));
		}
		else if(memcmp(id,"TYE",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TYER",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCO",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCON",sizeof(m_dwId));
		}
		else if(memcmp(id,"COM",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"COMM",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCM",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCOM",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOA",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TOPE",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCR",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCOP",sizeof(m_dwId));
		}
		else if(memcmp(id,"WXX",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"WXXX",sizeof(m_dwId));
		}
		else if(memcmp(id,"TSS",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TSSE",sizeof(m_dwId));
		}
		else if(memcmp(id,"TEN",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TENC",sizeof(m_dwId));
		}
		else if(memcmp(id,"PIC",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"APIC",sizeof(m_dwId));
		}
		/* STEP start */
		else if(memcmp(id,"TBP",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TBPM",sizeof(m_dwId));
		}
		else if(memcmp(id,"TPA",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPOS",sizeof(m_dwId));
		}
		else if(memcmp(id,"TDA",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TDAT",sizeof(m_dwId));
		}
		else if(memcmp(id,"TDY",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TDLY",sizeof(m_dwId));
		}
		else if(memcmp(id,"TFT",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TFLT",sizeof(m_dwId));
		}
		else if(memcmp(id,"TIM",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TIME",sizeof(m_dwId));
		}
		else if(memcmp(id,"TKE",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TKEY",sizeof(m_dwId));
		}
		else if(memcmp(id,"TLA",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TLAN",sizeof(m_dwId));
		}
		else if(memcmp(id,"TLE",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TLEN",sizeof(m_dwId));
		}
		else if(memcmp(id,"TMT",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TMED",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOF",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TOFN",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOL",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TOLY",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOR",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TORY",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOT",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TOAL",sizeof(m_dwId));
		}
		else if(memcmp(id,"TP2",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPE2",sizeof(m_dwId));
		}
		else if(memcmp(id,"TP3",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPE3",sizeof(m_dwId));
		}
		else if(memcmp(id,"TP4",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPE4",sizeof(m_dwId));
		}
		else if(memcmp(id,"TPB",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPUB",sizeof(m_dwId));
		}
		else if(memcmp(id,"TRC",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TSRC",sizeof(m_dwId));
		}
		else if(memcmp(id,"TRD",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TRDA",sizeof(m_dwId));
		}
		else if(memcmp(id,"TSI",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TSIZ",sizeof(m_dwId));
		}
		else if(memcmp(id,"TT1",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TIT1",sizeof(m_dwId));
		}
		else if(memcmp(id,"TT3",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TIT3",sizeof(m_dwId));
		}
		else if(memcmp(id,"TXT",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TEXT",sizeof(m_dwId));
		}
		else if(memcmp(id,"TXX",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TXXX",sizeof(m_dwId));
		}
		/* STEP end */
		else
		{
			return 0;
		}

		m_data = (unsigned char *)malloc(size+1/* STEP */);
		if(!m_data)
		{
			return 0;	//メモリを確保できなかった
		}
		ZeroMemory(m_data, size+1); /* STEP */
		m_dwSize = size;
		m_wFlags = 0;	//v2.2
		memcpy(m_data,&pData[6],size);
		return (size + 6);
	};
	DWORD GetId(){return m_dwId;};
	void SetId(DWORD id){m_dwId = id;};
	DWORD GetSize(){return m_dwSize;};
	void SetSize(DWORD size){m_dwSize = size;};
	WORD GetFlags(){return m_wFlags;};
	void SetFlags(WORD flags){m_wFlags = flags;};
	unsigned char *GetData(){return m_data;};
	void SetData(unsigned char *data,DWORD size)
	{
		if(m_data)
		{
			free(m_data);
		}
		m_data = (unsigned char *)malloc(size+1/* STEP */);
		if(!m_data)
		{
			return;	//メモリを確保できなかった
		}
		ZeroMemory(m_data, size+1); /* STEP */
		memcpy(m_data,data,size);
		m_dwSize = size;
	};

private:
	unsigned char	*m_data;
	DWORD	m_dwId;
	DWORD	m_dwSize;
	WORD	m_wFlags;
};

#pragma warning(disable:4786)
#include <map>
using namespace std;

class CId3tagv2  
{
public:
#pragma pack(1)
	typedef struct _ID3HEAD
	{
		char id3des[3];
		char ver[2];
		unsigned char flag;
		unsigned char size[4];
	}ID3HEAD;
#pragma pack()
//	CId3tagv2(WORD defaultVersion = 0x0300/* ID3v2.3 = 0x0300/ID3v2.4 = 0x0400*/);
	CId3tagv2();
	virtual ~CId3tagv2();
	void SetEncDefault(const char *szDefaultEnc);
	BOOL IsEnable(){return m_bEnable;};
//	void SetDefaultId3v2Version(DWORD version){m_wDefaultId3TagVersion = (WORD )version;};/* ID3v2.3 = 0x0300/ID3v2.4 = 0x0400*/
	void SetVer(WORD ver){m_wVer = ver;};
	WORD GetVer(){return m_wVer;};
	void SetCharEncode(int encode){m_encode = encode;};
	int GetCharEncode(){return m_encode;};
	void SetUnSynchronization(BOOL bEnable){m_bUnSynchronization = bEnable;};
	BOOL GetUnSynchronization(){return m_bUnSynchronization;};
	
	static const int	ID3V2CHARENCODE_ISO_8859_1;
	static const int	ID3V2CHARENCODE_UTF_16;
	static const int	ID3V2CHARENCODE_UTF_16BE;
	static const int	ID3V2CHARENCODE_UTF_8;
	
	CString GetTitle();	//TIT2
	void SetTitle(const char *title);
	CString GetTrackNo();
	void SetTrackNo(const char *szTrackNo);
	CString GetArtist();
	void SetArtist(const char *artist);
	CString GetAlbum();
	void SetAlbum(const char *album);
	CString GetYear();
	void SetYear(const char *year);
	CString GetGenre();
	void SetGenre(const char *szGenre);
	CString GetComment();
	void SetComment(const char *comment);
	CString GetComposer();
	void SetComposer(const char *composer);
	CString GetOrigArtist();
	void SetOrigArtist(const char *origArtist);
	CString GetCopyright();
	void SetCopyright(const char *copyright);
	CString GetUrl();
	void SetUrl(const char *url);
	CString GetEncoder();
	void SetEncoder(const char *encoder);
	CString GetEncodest();
	void SetEncodest(const char *encoder);

	DWORD Load(const char *szFileName);
	DWORD Save(const char *szFileName);
	DWORD DelTag(const char *szFileName);
	DWORD MakeTag(const char *szFileName);

private:
	DWORD DecodeUnSynchronization(unsigned char *data,DWORD dwSize);
	DWORD EncodeUnSynchronization(unsigned char *srcData,DWORD dwSize,unsigned char *dstData);
	DWORD ExtractV2Size(const unsigned char size[4]);
	void MakeV2Size(DWORD dwSize,unsigned char size[4]);
	CString GetId3String(const char szId[]);
	void SetId3String(const char szId[],const char *szString,const char *szDescription = NULL);
	DWORD GetTotalFrameSize();
	void v23IDtov22ID(char *v23ID,char *v22ID);
	void UTF16toUTF16BE(WCHAR *str,int len);

	void Release();
	BOOL m_bEnable;					//ID3v2が無い場合はFALSE
	int	m_encode;					// 文字エンコードタイプ (0=ISO-8859-1/1=UTF-16/2=UTF-16BE/3=UTF-8)
//	BOOL m_bUnicodeEncode;			//文字コードエンコードにUnicodeを使用する
	BOOL m_bUnSynchronization;		//非同期化する
	ID3HEAD m_head;
	WORD m_wVer;
	multimap<DWORD,CId3Frame> m_frames; /* STEP map -> multimap */
	CString m_strDefaultEnc;		//TENCのデフォルト値
//	WORD m_wDefaultId3TagVersion;	// ID3V2の初期値(新規作成時にこのバージョンとなる)

public: /* STE改で追加 */
	//void SetGenre(const char *szGenre, bool addNumber);
	//static bool IsNumeric(const CString &str);
	//static CString GetIntTrackNo(const CString &no);
	//BOOL IsSafeVer(){return TRUE;};
};

#endif // !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)
