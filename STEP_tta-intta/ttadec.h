/*
 * ttadec.h
 *
 * Description:	 TTAv1 decoder definitions
 * Developed by: Alexander Djourik <ald@true-audio.com>
 *               Pavel Zhilin <pzh@true-audio.com>
 *
 * Copyright (c) 2005 Alexander Djourik. All rights reserved.
 *
 */

/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Please see the file COPYING in this directory for full copyright
 * information.
 */

#ifndef TTADEC_H_
#define TTADEC_H_

#pragma pack(1)

#define	TTA_VERSION		"3.0"
#define TTA_LEVEL		1
#define TTA1_SIGN		0x31415454
#define FRAME_TIME		1.04489795918367346939

// return codes
#define OPEN_ERROR		1		// Can't open file
#define FORMAT_ERROR	2		// Unknown TTA format version
#define PLAYER_ERROR	3		// Not supported file format
#define FILE_ERROR		4		// File is corrupted
#define READ_ERROR		5		// Can't read from file
#define WRITE_ERROR		6		// Can't write to file
#define MEMORY_ERROR	7		// Memory allocation error
#define THREAD_ERROR	8		// Error killing thread

#define ISO_BUFFER_SIZE	(1024*1024)
#define FRAME_TIME		1.04489795918367346939
#define SEEK_STEP		(int)(FRAME_TIME * 1000)
#define MAX_BSIZE		4
#define MAX_BPS			(MAX_BSIZE*8)
#define MAX_NCH			8
#define MAX_ORDER		8
#define BUFFER_LENGTH	576
#define BUFFER_SIZE		(BUFFER_LENGTH*MAX_BSIZE*MAX_NCH)
#define OUT_BPS			24

#define WAVE_FORMAT_PCM	1
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE

typedef struct {
	HANDLE			HFILE;		// file handle
	unsigned short	NCH;		// number of channels
	unsigned short	BPS;		// bits per sample
	unsigned short	BSIZE;		// byte size
	unsigned short	FORMAT;		// audio format
	unsigned long	SAMPLERATE;	// samplerate (sps)
	unsigned long	DATALENGTH;	// data length in samples
	unsigned long	FRAMELEN;	// frame length
	unsigned long	LENGTH;		// playback time (sec)
	unsigned long	FILESIZE;	// file size (byte)
	float			COMPRESS;	// compression ratio
	unsigned long	BITRATE;	// bitrate (kbps)
	unsigned long	STATE;		// return code
	unsigned char	filename[MAX_PATH];
	id3v1_data		id3v1;
	id3v2_data		id3v2;
} tta_info;

typedef struct {
    unsigned long TTAid;
    unsigned short AudioFormat;
    unsigned short NumChannels;
    unsigned short BitsPerSample;
    unsigned long SampleRate;
    unsigned long DataLength;
    unsigned long CRC32;
} tta_hdr;

typedef struct {
	unsigned long k0;
	unsigned long k1;
	unsigned long sum0;
	unsigned long sum1;
} adapt;

typedef struct {
	long shift;
	long round;
	long error;
	long mutex;
	long qm[MAX_ORDER+1];
	long dx[MAX_ORDER+1];
	long dl[MAX_ORDER+1];
} fltst;

typedef struct {
	fltst fst;
	adapt rice;
	long last;
} decoder;

#endif /* TTADEC_H_ */

