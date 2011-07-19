#ifndef _FILEFLAC_H_
#define _FILEFLAC_H_

#include "STEPlugin.h"

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "metadata.h"

typedef size_t (*vcedit_read_func)(void *, size_t, size_t, void *);
typedef size_t (*vcedit_write_func)(const void *, size_t, size_t, void *);

typedef struct {
	ogg_sync_state		*oy;
	ogg_stream_state	*os;

	vorbis_comment		*vc;
	vorbis_info         *vi;

	vcedit_read_func read;
	vcedit_write_func write;

	void		*in;
	long		serial;
	unsigned char	*mainbuf;
	unsigned char	*bookbuf;
	int		mainlen;
	int		booklen;
	char 	    *lasterror;
	char   *vendor;
	int prevW;
	int extrapage;
	int eosin;
} vcedit_state;

typedef struct _File_Tag File_Tag;
struct _File_Tag
{
    unsigned int key;             /* Incremented value */
    boolean saved;        /* Set to TRUE if this tag had been saved */
    char *title;          /* Title of track */
    char *artist;         /* Artist name */
    char *album;          /* Album name */
    char *year;           /* Year of track */
    char *track;          /* Position of track in the album */
    char *track_total;    /* The number of tracks for the album (ex: 12/20) */
	char *str_track;      /* For write tag (ex. 12/20) */
    char *genre;          /* Genre of song */
    char *comment;        /* Comment */
	char *composer;       /* Composer */
	char *performer;      /* Performer */
    CPtrArray *other;     /* List of unsupported fields (used for ogg only) */

    int samplerate;       /* Samplerate (Hz) */
    int mode;             /* Stereo, ... or channels for ogg */
    int size;             /* The size of file (in bytes) */
    int duration;         /* The duration of file (in seconds) */
    int bitrate;          /* Bitrate (kb/s) */

};

const char *flac_error_msg;

#ifndef MAX
#    define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#    define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

typedef enum
{
    FMT_U8, FMT_S8, FMT_U16_LE, FMT_U16_BE, FMT_U16_NE, FMT_S16_LE, FMT_S16_BE, FMT_S16_NE
}
AFormat;

typedef struct {
    FLAC__bool abort_flag;
    FLAC__bool is_playing;
    FLAC__bool eof;
    FLAC__bool play_thread_open; /* if true, is_playing must also be true */
    unsigned total_samples;
    unsigned bits_per_sample;
    unsigned channels;
    unsigned sample_rate;
    unsigned length_in_msec;
    AFormat sample_format; // Note : defined in XMMS devel
    int seek_to_in_sec;
} file_info_struct;

#ifdef __cplusplus
}
#endif 

#endif /* _FILEFLAC_H_ */