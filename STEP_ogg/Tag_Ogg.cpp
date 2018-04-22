// Tag_Ogg.cpp: CTag_Ogg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include <fcntl.h>
#include "GlobalCommand.h"

#include "Tag_Ogg.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vector>

/* STEP */
//#if _DEBUG
//#pragma comment(lib,"ogg_static_d.lib")
//#pragma comment(lib,"vorbis_static_d.lib")
//#pragma comment(lib,"vorbisfile_static_d.lib")
//#else
//#pragma comment(lib,"ogg_static.lib")
//#pragma comment(lib,"vorbis_static.lib")
//#pragma comment(lib,"vorbisfile_static.lib")
//#endif
#pragma comment(lib,"libogg_static.lib")
#pragma comment(lib,"libvorbis_static.lib")
#pragma comment(lib,"libvorbisfile_static.lib")

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CTag_Ogg::CTag_Ogg()
{

}

CTag_Ogg::~CTag_Ogg()
{

}

void CTag_Ogg::Release()
{
	m_strAudioFormat = "";
	m_strTime = "";

	m_comments.clear();
}

BOOL CTag_Ogg::AddComment(const char *name, const CString& value)
{
	CString _name(name);
	_name.MakeUpper();
	m_comments.insert(pair<CString,CString>(_name, value));
	
	return TRUE;
}

BOOL CTag_Ogg::DelComment(const char *name,int index)
{
	//nameのなかからdwIndexの値を取得
    pair<multimap<CString,CString>::iterator,multimap<CString,CString>::iterator> itp = m_comments.equal_range(CString(name));
	
	int i = 0;
	while(itp.first != itp.second)
	{
		if(i == index)
		{
			m_comments.erase(itp.first);
			return TRUE;
		}
		itp.first++;
		i++;
	}
	
	return FALSE;
}

BOOL CTag_Ogg::GetComment(const char *name,int index,CString &strValue)
{
	strValue = "";
	//nameのなかからdwIndexの値を取得
    pair<multimap<CString,CString>::iterator,multimap<CString,CString>::iterator> itp = m_comments.equal_range(CString(name));
	
	int i = 0;
	while(itp.first != itp.second)
	{
		if(i == index)
		{
			strValue = (itp.first)->second;
			return TRUE;
		}
		itp.first++;
		i++;
	}
	
	return FALSE;
}

void CTag_Ogg::GetCommentNames(CStringArray &strArray)
{
	//nameリストを返す
	multimap<CString,CString>::iterator it = m_comments.begin();
	
	CString strName;
	while(it != m_comments.end())
	{
		if(strName.Compare(it->first))
		{
			strArray.Add(it->first);
		}
		strName = it->first;
		it++;
	}
}

DWORD CTag_Ogg::Load(LPCTSTR szFileName)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;
	Release();

	//ファイルをオープン
	FILE *fp = _tfopen(szFileName, TEXT("rb"));
	if(!fp)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}

	ogg_sync_state   oy; /* sync and verify incoming physical bitstream */
	ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */
	ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
	ogg_packet       op; /* one raw packet of data for decode */
	vorbis_info      vi; /* struct that stores all the static vorbis bitstream settings */
	vorbis_comment   vc; /* struct that stores all the bitstream user comments */

	char			*buffer;
	int				bytes;

	/********** Decode setup ************/
	ogg_sync_init(&oy); /* Now we can read pages */
	
	while(1)
	{ /* we repeat if the bitstream is chained */
		int i;
		/* submit a 4k block to libvorbis' Ogg layer */
		buffer = ogg_sync_buffer(&oy,4096);
		bytes=fread(buffer,1,4096,fp);
		ogg_sync_wrote(&oy,bytes);

		/* Get the first page. */
		if(ogg_sync_pageout(&oy,&og) != 1)
		{
			fclose(fp);
			dwWin32errorCode = -1;
			return dwWin32errorCode;
		}

		/* Get the serial number and set up the rest of decode. */
		/* serialno first; use it to set up a logical stream */
		ogg_stream_init(&os,ogg_page_serialno(&og));

		/* extract the initial header from the first page and verify that the
		Ogg bitstream is in fact Vorbis data */

		/* I handle the initial header first instead of just having the code
		read all three Vorbis headers at once because reading the initial
		header is an easy way to identify a Vorbis bitstream and it's
		useful to see that functionality seperated out. */
		vorbis_info_init(&vi);
		vorbis_comment_init(&vc);

		if(ogg_stream_pagein(&os,&og)<0)
		{ 
			/* error; stream version mismatch perhaps */
//			fprintf(stderr,"Error reading first page of Ogg bitstream data.\n");
			dwWin32errorCode = -1;
			break;
		}

		if(ogg_stream_packetout(&os,&op)!=1)
		{ 
			/* no page? must not be vorbis */
//			fprintf(stderr,"Error reading initial header packet.\n");
			dwWin32errorCode = -1;
			break;
		}

		if(vorbis_synthesis_headerin(&vi,&vc,&op)<0)
		{ 
			/* error case; not a vorbis header */
//			fprintf(stderr,"This Ogg bitstream does not contain Vorbis "
//			  "audio data.\n");
			dwWin32errorCode = -1;
			break;
		}

		/* At this point, we're sure we're Vorbis.  We've set up the logical
		(Ogg) bitstream decoder.  Get the comment and codebook headers and
		set up the Vorbis decoder */

		/* The next two packets in order are the comment and codebook headers.
		They're likely large and may span multiple pages.  Thus we reead
		and submit data until we get our two pacakets, watching that no
		pages are missing.  If a page is missing, error out; losing a
		header page is the only place where missing data is fatal. */

		i=0;
		while(i<2)
		{
			while(i<2)
			{
				int result=ogg_sync_pageout(&oy,&og);
				if(result==0)break; /* Need more data */
				/* Don't complain about missing or corrupt data yet.  We'll
				catch it at the packet output phase */
				if(result==1)
				{
					ogg_stream_pagein(&os,&og); /* we can ignore any errors here
								 as they'll also become apparent
								 at packetout */
					while(i<2)
					{
						result=ogg_stream_packetout(&os,&op);
						if(result==0)
						{
							break;
						}
						if(result<0)
						{
							/* Uh oh; data at some point was corrupted or missing!
							We can't tolerate that in a header.  Die. */
//							fprintf(stderr,"Corrupt secondary header.  Exiting.\n");
							i = 2;
							dwWin32errorCode = -1;
							break;
						}
						vorbis_synthesis_headerin(&vi,&vc,&op);
						i++;
					}
				}
			}
			/* no harm in not checking before adding more */
			buffer=ogg_sync_buffer(&oy,4096);
			bytes=fread(buffer,1,4096,fp);
			if(bytes==0 && i<2)
			{
				i = 2;
				dwWin32errorCode = -1;
				break;
			}
			ogg_sync_wrote(&oy,bytes);
		}
		m_lChannels = vi.channels;
		m_lSampleRate = vi.rate;
		m_iVersion = vi.version;
		m_strVendor = vc.vendor;
		m_lBitrate_upper = vi.bitrate_upper;
		m_lBitrate_nominal = vi.bitrate_nominal;
		m_lBitrate_lower = vi.bitrate_lower;
		m_lBitrate_window = vi.bitrate_window;

		char **ptr=vc.user_comments;
		while(*ptr)
		{
			CStringA str(*ptr);
			int index = str.Find("=");
			if(index != -1)
			{
				AddComment(CStringA(str, index), Utf8ToUcs2(&((*ptr)[index+1])));
			}
			++ptr;
		}
		/* ogg_page and ogg_packet structs always point to storage in
		libvorbis.  They're never freed or manipulated directly */
		vorbis_comment_clear(&vc);
		vorbis_info_clear(&vi);  /* must be called last */
		ogg_stream_clear(&os);

		break;
	}
	/* OK, clean up the framer */
	ogg_sync_clear(&oy);

	//演奏時間の取得
	rewind(fp);
	while(1)	//dummy
	{
		OggVorbis_File ov;
		/* open the file/pipe on stdin */
		if(ov_open(fp,&ov,NULL,-1)<0)
		{
			break;
		}

		/* print details about each logical bitstream in the input */
		if(ov_seekable(&ov))
		{
			m_strTime.Format(TEXT("%02ld:%02ld (%ldsec)"),
						(DWORD )ov_time_total(&ov,-1)/60,
						(DWORD )ov_time_total(&ov,-1)%60,
						(DWORD )ov_time_total(&ov,-1));
		}

		//シリアル番号の取得
		m_lSerial = ov_serialnumber(&ov,-1);

		//平均ビットレートの取得
		m_lBitrate_avg = ov_bitrate(&ov,-1);

		ov_clear(&ov);
		break;
	}
	
	m_strAudioFormat.Format(TEXT("Ogg Vorbis, %ldHz, %ldKbits/s%s, %ldch"),
		m_lSampleRate,
		m_lBitrate_avg / 1000,
		((m_lBitrate_nominal == m_lBitrate_lower) && (m_lBitrate_nominal == m_lBitrate_upper)) ? TEXT("") : TEXT("(VBR)"),
		m_lChannels);
	fclose(fp);

	return dwWin32errorCode;
}

static void _v_writestring(oggpack_buffer *o,char *s, int bytes)
{
	while(bytes--)
	{
		oggpack_write(o,*s++,8);
	}
}
static int _commentheader_out(vorbis_comment *vc, char *vendor, ogg_packet *op)
{
	oggpack_buffer opb;

	oggpack_writeinit(&opb);

	/* preamble */  
	oggpack_write(&opb,0x03,8);
	_v_writestring(&opb,"vorbis", 6);

	/* vendor */
	oggpack_write(&opb,strlen(vendor),32);
	_v_writestring(&opb,vendor, strlen(vendor));

	/* comments */
	oggpack_write(&opb,vc->comments,32);
	if(vc->comments)
	{
		int i;
		for(i=0;i<vc->comments;i++)
		{
			if(vc->user_comments[i])
			{
				oggpack_write(&opb,vc->comment_lengths[i],32);
				_v_writestring(&opb,vc->user_comments[i], 
						vc->comment_lengths[i]);
			}
			else
			{
				oggpack_write(&opb,0,32);
			}
		}
	}
	oggpack_write(&opb,1,1);

	op->packet = (unsigned char *)_ogg_malloc(oggpack_bytes(&opb));
	memcpy(op->packet, opb.buffer, oggpack_bytes(&opb));

	op->bytes=oggpack_bytes(&opb);
	op->b_o_s=0;
	op->e_o_s=0;
	op->granulepos=0;

	free(opb.buffer);
	return 0;
}

DWORD CTag_Ogg::Save(LPCTSTR szFileName)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;

	//ファイルをオープン
	FILE *fp = _tfopen(szFileName, TEXT("rb"));
	if(!fp)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}

	//==================テンポラリを作成==================
	//テンポラリ名を取得
	TCHAR szTempPath[MAX_PATH];
	TCHAR szTempFile[MAX_PATH];
	lstrcpy(szTempPath,szFileName);
	cutFileName(szTempPath);
	if(!GetTempFileName(szTempPath, TEXT("tms"), 0, szTempFile))
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	
	FILE *fp_out = _tfopen(szTempFile, TEXT("wb"));
	if(!fp_out)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}

	ogg_sync_state		_oy; /* sync and verify incoming physical bitstream */
	ogg_stream_state	_os; /* take physical pages, weld into a logical stream of packets */
	vorbis_info			_vi; /* struct that stores all the static vorbis bitstream settings */
	vorbis_comment		_vc; /* struct that stores all the bitstream user comments */
	int _mainlen = 0;
	int _booklen = 0;
	unsigned char *_mainbuf = NULL;
	unsigned char *_bookbuf = NULL;
	int _serial = 0;
	int i,j;
	CStringArray strArray;
	CStringA strVendor;

	while(1)
	{
		ogg_sync_init(&_oy);

		//[file -> ogg_sync_state]
		char *buffer;
		int bytes;
		buffer = ogg_sync_buffer(&_oy,4096);
		bytes = fread(buffer,1,4096,fp);
		ogg_sync_wrote(&_oy,bytes);

		//[ogg_sync_state -> ogg_page]
		ogg_page og;
		if(ogg_sync_pageout(&_oy,&og) != 1)
		{
			ogg_sync_clear(&_oy);
			fclose(fp);
			fclose(fp_out);
			return -1;
		}

		//シリアル番号の取得
		_serial = ogg_page_serialno(&og);
		
		//init ogg stream
		ogg_stream_init(&_os,_serial);

		//init vorbis info
		vorbis_info_init(&_vi);

		//init vorbis comment
		vorbis_comment_init(&_vc);

		//[ogg page -> ogg stream]
		if(ogg_stream_pagein(&_os,&og) < 0)
		{
//			state->lasterror = _("Error reading first page of Ogg bitstream.");
			dwWin32errorCode = GetLastError();
			goto loaderr;
		}

		//[ogg stream -> header_main]
		ogg_packet header_main;
		if(ogg_stream_packetout(&_os,&header_main) != 1)
		{
//			state->lasterror = _("Error reading initial header packet.");
			dwWin32errorCode = GetLastError();
			goto loaderr;
		}

		if(vorbis_synthesis_headerin(&_vi,&_vc,&header_main) < 0)
		{
//			state->lasterror = _("Ogg bitstream does not contain vorbis data.");
			dwWin32errorCode = GetLastError();
			goto loaderr;
		}

		_mainlen = header_main.bytes;
		_mainbuf = (unsigned char *)malloc(_mainlen);
		if(!_mainbuf)
		{
			dwWin32errorCode = GetLastError();
			goto loaderr;
		}
		memcpy(_mainbuf,header_main.packet,header_main.bytes);

		int i = 0;
		ogg_packet header_comments;
		ogg_packet header_codebooks;
		ogg_packet *header = &header_comments;
		while(i<2)
		{
			while(i<2)
			{
				int result = ogg_sync_pageout(&_oy,&og);
				if(result == 0)
				{
					break; /* Too little data so far */
				}
				else if(result == 1)
				{
					ogg_stream_pagein(&_os,&og);
					while(i<2)
					{
						result = ogg_stream_packetout(&_os,header);
						if(result == 0)
						{
							break;
						}
						if(result == -1)
						{
//							state->lasterror = _("Corrupt secondary header.");
							dwWin32errorCode = -1;
							goto loaderr;
						}
						vorbis_synthesis_headerin(&_vi,&_vc,header);
						if(i == 1)
						{
							_booklen = header->bytes;
							_bookbuf = (unsigned char *)malloc(_booklen);
							if(!_bookbuf)
							{
								dwWin32errorCode = -1;
								goto loaderr;
							}
							memcpy(_bookbuf,header->packet,header->bytes);
						}
						i++;
						header = &header_codebooks;
					}
				}
			}

			buffer = ogg_sync_buffer(&_oy,4096);
			bytes = fread(buffer,1,4096,fp);
			if(bytes == 0 && i < 2)
			{
//				state->lasterror = _("EOF before end of vorbis headers.");
				dwWin32errorCode = -1;
				goto loaderr;
			}
			ogg_sync_wrote(&_oy,bytes);
		}
		break;
	}

	strVendor = _vc.vendor;
	vorbis_comment_clear(&_vc);
	
	GetCommentNames(strArray);
	for(i=0; i<strArray.GetSize(); i++)
	{
		j = 0;
		CStringA strName = strArray.GetAt(i);
		CString strValue;
		while(1)
		{
			if(!GetComment(strName,j++,strValue))
			{
				break;
			}
			vorbis_comment_add_tag(&_vc, strName, Ucs2ToUtf8(strValue).c_str());
			TRACE("vorbis_comment_add_tag(%s=%s)\n", strName, Ucs2ToUtf8(strValue));
		}
	}

	while(1)
	{
		ogg_page ogout, ogin;
		ogg_packet op;
		int result;
		char *buffer;
		int bytes, eosin=0, eosout=0;

		//
		ogg_packet header_main;
		header_main.bytes = _mainlen;
		header_main.packet = _mainbuf;
		header_main.b_o_s = 1;
		header_main.e_o_s = 0;
		header_main.granulepos = 0;

		//
		ogg_packet header_codebooks;
		header_codebooks.bytes = _booklen;
		header_codebooks.packet = _bookbuf;
		header_codebooks.b_o_s = 0;
		header_codebooks.e_o_s = 0;
		header_codebooks.granulepos = 0;

		//
		ogg_stream_state streamout;
		ogg_stream_init(&streamout,_serial);

		//
		ogg_packet header_comments;
//		vorbis_commentheader_out(&_vc,&header_comments);
		_commentheader_out(&_vc, const_cast<char*>((LPCSTR)strVendor), &header_comments);

		ogg_stream_packetin(&streamout,&header_main);
		ogg_stream_packetin(&streamout,&header_comments);
		ogg_stream_packetin(&streamout,&header_codebooks);
		
		while((result = ogg_stream_flush(&streamout,&ogout)))
		{
			if(fwrite(ogout.header,1,ogout.header_len,fp_out) != (size_t )ogout.header_len)
			{
				dwWin32errorCode = -1;
				goto cleanup;
			}
			if(fwrite(ogout.body,1,ogout.body_len,fp_out) != (size_t )ogout.body_len)
			{
				dwWin32errorCode = -1;
				goto cleanup;
			}
		}
		
		while(!eosout)
		{
			while(!eosout)
			{
				result = ogg_sync_pageout(&_oy, &ogin);
				if(result == 0)
				{
					break; /* Need more data... */
				}
				else if(result == -1)
				{
					continue;
				}
				else
				{
					ogg_stream_pagein(&_os, &ogin);
					while(1)
					{
						result = ogg_stream_packetout(&_os, &op);
						if(result == 0)
						{
							break;
						}
						else if(result == -1)
						{
							continue;
						}
						else
						{
							ogg_stream_packetin(&streamout,&op);
							while(!eosout)
							{
								int result = ogg_stream_pageout(&streamout, &ogout);
								if(result==0)
								{
									break;
								}
								if(fwrite(ogout.header,1,ogout.header_len,fp_out) != (size_t )ogout.header_len)
								{
									dwWin32errorCode = -1;
									goto cleanup;
								}
								if(fwrite(ogout.body,1,ogout.body_len,fp_out) != (size_t )ogout.body_len)
								{
									dwWin32errorCode = -1;
									goto cleanup;
								}
								if(ogg_page_eos(&ogout))
								{
									eosout=1;
								}
							}
						}
					}
					if(ogg_page_eos(&ogin))
					{
						eosin = 1;
					}
				}
			}
			if(!eosin)
			{
				buffer = ogg_sync_buffer(&_oy,4096);
				bytes = fread(buffer,1,4096,fp);
				ogg_sync_wrote(&_oy, bytes);
				if(bytes == 0) 
				{
					eosin = 1;
					break;
				}
			}
		}
		eosin=0; /* clear it, because not all paths to here do */
		while(!eosin) /* We reached eos, not eof */
		{
			/* We copy the rest of the stream (other logical streams)
			 * through, a page at a time. */
			while(1)
			{
				result = ogg_sync_pageout(&_oy, &ogout);
				if(result==0)
				{
					break;
				}
				if(result<0)
				{
//					state->lasterror = _("Corrupt or missing data, continuing...");
				}
				else
				{
					/* Don't bother going through the rest, we can just 
					 * write the page out now */
					if(fwrite(ogout.header,1,ogout.header_len,fp_out) != (size_t )ogout.header_len)
					{
						dwWin32errorCode = -1;
						goto cleanup;
					}
					if(fwrite(ogout.body,1,ogout.body_len,fp_out) != (size_t )ogout.body_len)
					{
						dwWin32errorCode = -1;
						goto cleanup;
					}
				}
			}
			buffer = ogg_sync_buffer(&_oy,4096);
			bytes = fread(buffer,1,4096,fp);
			ogg_sync_wrote(&_oy,bytes);
			if(bytes == 0) 
			{
				eosin = 1;
				break;
			}
		}

cleanup:
		ogg_packet_clear(&header_comments);
		ogg_stream_clear(&streamout);
		break;
	}

loaderr:
	vorbis_info_clear(&_vi);
	vorbis_comment_clear(&_vc);
	ogg_stream_clear(&_os);
	ogg_sync_clear(&_oy);
	
	if(_mainbuf)
	{
		free(_mainbuf);
	}
	if(_bookbuf)
	{
		free(_bookbuf);
	}
	fclose(fp_out);
	fclose(fp);

	if(dwWin32errorCode == ERROR_SUCCESS)
	{
		//オリジナルファイルを退避(リネーム)
		TCHAR szPreFile[MAX_PATH];
		if(!GetTempFileName(szTempPath, TEXT("tms"), 0, szPreFile))
		{
			dwWin32errorCode = GetLastError();
			DeleteFile(szTempFile);
			return dwWin32errorCode;
		}
		DeleteFile(szPreFile);//手抜き(^^;
		if(!MoveFile(szFileName,szPreFile))
		{
			dwWin32errorCode = GetLastError();
			DeleteFile(szTempFile);
			return dwWin32errorCode;
		}

		//完成品をリネーム
		if(!MoveFile(szTempFile,szFileName))
		{
			dwWin32errorCode = GetLastError();
			MoveFile(szPreFile,szFileName);
			DeleteFile(szTempFile);
			return dwWin32errorCode;
		}
		//オリジナルを削除
		DeleteFile(szPreFile);
	}
	else
	{
		DeleteFile(szTempFile);
	}

	return dwWin32errorCode;
}

std::string CTag_Ogg::Ucs2ToUtf8(const CStringW& str)
{
	if (!str.GetLength()) {
		return "";
	}

	std::vector<char> wc;
	wc.resize(WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, &wc[0], wc.size(), NULL, NULL);

	return wc.data();
}

CStringW CTag_Ogg::Utf8ToUcs2(const char *str)
{
	if(!str || str[0] == '\0')
	{
		return TEXT("");
	}

	std::vector<WCHAR> buf;
	buf.resize(MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, buf.data(), buf.size());

	return buf.data();
}
