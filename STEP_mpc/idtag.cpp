#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "in_mpc.h"
#include "idtag.h"
#include "mpc_dec.h"

const char ListSeparator[] = { "; " };

// list of possible naming schemes
// the order is important, first match is used
// T: Title
// A: Artist
// L: Album
// C: Comment
// N: Track
// Y: Year
// G: Genre
// X: Discard

const char *SchemeList[] = {
    "%L -- [%N] -- %A -- %T",
    "%L/[%N] -- %A -- %T",
    "%L -- [%N] %A -- %T",
    "%L/[%N] %A -- %T",
    "%A -- %L -- [%N] %T",
    "%A/%L -- [%N] %T",
    "%A/%L/[%N] - %T",
    "%A/%L/[%N] %T",
    "%L/%A -- %T",
    "%A_-_%L_-_%Y\\%N_-_%A_-_%T",
    "%A_-_%L-%Y\\%N_-_%A_-_%T",
    "%A_-_%L\\%N_-_%A_-_%T",
    "%L-%Y\\%N_-_%A_-_%T",
    "%L\\%N_-_%A_-_%T",
    "%A_%L\\%A_%N_%T",
    "%L\\%A_%N_%T",
    "%L\\%N_%A - %T",
    "%A_%L\\%A_%T",
    "%L\\%A_%T",
    "(%Y) %L\\%A - %L - [%N] - %T",
    "(%Y) %L\\%A - %L - (%N) - %T",
    "(%Y) %L\\%A - %L - %N - %T",
    "(%Y) %L\\%A - %L [%N] - %T",
    "(%Y) %L\\%A - %L (%N) - %T",
    "(%Y) %L\\%A - %L - [%N] %T",
    "(%Y) %L\\%A - %L - (%N) %T",
    "(%Y) %L\\%A - %L [%N] %T",
    "(%Y) %L\\%A - %L (%N) %T",
    "%L (%Y)\\%L - [%N] - %A - %T",
    "%L (%Y)\\%L - (%N) - %A - %T",
    "%L (%Y)\\%L - %N - %A - %T",
    "%L (%Y)\\%L [%N] - %A - %T",
    "%L (%Y)\\%L (%N) - %A - %T",
    "%L (%Y)\\%L - [%N] %A %T",
    "%L (%Y)\\%L - (%N) %A %T",
    "%L (%Y)\\%L [%N] %A - %T",
    "%L (%Y)\\%L (%N) %A - %T",
    "%A - %L - [%N] - %T",
    "%A - %L - (%N) - %T",
    "%A - %L - %N - %T",
    "%A - %L [%N] - %T",
    "%A - %L (%N) - %T",
    "%A - %L - [%N] %T",
    "%A - %L - (%N) %T",
    "%A - %L [%N] %T",
    "%A - %L (%N) %T",
    "%A - %L\\%N - %A - %T",
    "%A - %L\\%A - %N - %T",
    "%A - %L\\%N- %A - %T",
    "%L\\%A - [%N] - %T",
    "%L\\%A - (%N) - %T",
    "%L\\%A - [%N] %T",
    "%L\\%A - (%N) %T",
    "%L\\%A [%N] - %T",
    "%L\\%A (%N) - %T",
    "%L\\%A (%N) %T",
    "%L\\%A [%N] %T",
    "%L\\%N - %A - %T",
    "%L\\%A - %N - %T",
    "%L\\%N- %A - %T",
    "%A - %L\\%N %A - %T",
    "%L\\%N %A - %T",
    "%A - %L\\%N - %T",
    "%A\\%L\\%N - %T",
    "%A - %L\\%A - %T",
    "%L\\%A - %T",
    "%A\\%L\\%N. %T",
};

/* V A R I A B L E S */
const char*  GenreList [] = {
    "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
    "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies",
    "Other", "Pop", "R&B", "Rap", "Reggae", "Rock",
    "Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
    "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk",
    "Fusion", "Trance", "Classical", "Instrumental", "Acid", "House",
    "Game", "Sound Clip", "Gospel", "Noise", "AlternRock", "Bass",
    "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
    "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk",
    "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta",
    "Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native American", "Cabaret",
    "New Wave", "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo-Fi",
    "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical",
    "Rock & Roll", "Hard Rock", "Folk", "Folk/Rock", "National Folk", "Swing",
    "Fast-Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde",
    "Gothic Rock", "Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band",
    "Chorus", "Easy Listening", "Acoustic", "Humour", "Speech", "Chanson",
    "Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus",
    "Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba",
    "Folklore", "Ballad", "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet",
    "Punk Rock", "Drum Solo", "A capella", "Euro-House", "Dance Hall",
    "Goa", "Drum & Bass", "Club House", "Hardcore", "Terror",
    "Indie", "BritPop", "NegerPunk", "Polsk Punk", "Beat",
    "Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary C",
    "Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop",
    "SynthPop",
};

// free tagdata from memory
void StreamInfo::Clear()
{
    size_t  i;

    if ( tagitems != NULL ) {
        for ( i = 0; i < tagitem_count; i++ ) {
            if ( tagitems[i].Item  ) free ( tagitems[i].Item  );
            if ( tagitems[i].Value ) free ( tagitems[i].Value );
        }

        free ( tagitems );
    }

    tagitem_count = 0;
    tagtype       = (tag_t)0;
    tagitems      = NULL;
	if (filename) {free(filename);filename=0;}
}

StreamInfo::StreamInfo()
{
	memset(&simple,0,sizeof(simple));
    tagitem_count = 0;
    tagtype       = (tag_t)0;
    tagitems      = NULL;
	filename=0;
}

void StreamInfo::SetFilename(const char *fn)
{
	if (filename) free(filename);
	filename=strdup(fn);
}

const char * StreamInfo::GetFilename()
{
	return filename ? filename : "";
}

// copy value from tag field to dest, returns length
int CopyTagValue ( char* dest, const char* item, const StreamInfo* Info, size_t count )
{
    size_t  i;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        // Are items case sensitive?
        if ( _stricmp ( item, Info->tagitems[i].Item ) == 0 ) {
            if ( count > Info->tagitems[i].ValueSize + 1 ) count = Info->tagitems[i].ValueSize + 1;
            memcpy ( dest, Info->tagitems[i].Value, count );
            return count;
        }
    }

    return 0;
}

// return pointer to value in tag field
char* TagValue ( const char* item, const StreamInfo* Info )
{
    size_t  i;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        // Are items case sensitive?
        if ( _stricmp ( item, Info->tagitems[i].Item ) == 0 )
            return (char*)Info->tagitems[i].Value;
    }

    return 0;
}

// add new field to tagdata
int NewTagField ( const char* item, const size_t itemsize, const char* value, const size_t valuesize, const unsigned int flags, StreamInfo* Info, const enum tag_rel reliability )
{
    size_t itemnum = Info->tagitem_count++;

    // no real error handling yet
    Info->tagitems = (TagItem*) realloc ( Info->tagitems, Info->tagitem_count * sizeof (TagItem) );
    if ( Info->tagitems == NULL ) {
        Info->tagitem_count = 0;
        return 1;
    }
    Info->tagitems[itemnum].Item  = (char*)malloc ( itemsize  + 1 );
    if ( Info->tagitems[itemnum].Item == NULL ) {                           // couldn't allocate memory, revert to original items
        Info->tagitems = (TagItem*) realloc ( Info->tagitems, --Info->tagitem_count * sizeof (TagItem) );
        return 1;
    }
    Info->tagitems[itemnum].Value = (char*)malloc ( valuesize + 1 );
    if ( Info->tagitems[itemnum].Value == NULL ) {                          // couldn't allocate memory, revert to original items
        free ( Info->tagitems[itemnum].Item );
        Info->tagitems = (TagItem*) realloc ( Info->tagitems, --Info->tagitem_count * sizeof (TagItem) );
        return 1;
    }
    memcpy ( Info->tagitems[itemnum].Item , item , itemsize  );
    memcpy ( Info->tagitems[itemnum].Value, value, valuesize );

    Info->tagitems[itemnum].Item [itemsize]  = '\0';
    Info->tagitems[itemnum].Value[valuesize] = '\0';

    Info->tagitems[itemnum].ItemSize         = itemsize;
    Info->tagitems[itemnum].ValueSize        = valuesize;
    Info->tagitems[itemnum].Flags            = flags;
    Info->tagitems[itemnum].Reliability      = reliability;

    return 0;
}

// replace old value in tagdata field
int ReplaceTagField ( const char* value, const size_t valuesize, const unsigned int flags, StreamInfo* Info, size_t itemnum, const enum tag_rel reliability )
{
    // no real error handling yet
    free ( Info->tagitems[itemnum].Value );
    Info->tagitems[itemnum].Value = (char*)malloc ( valuesize + 1 );
    if ( Info->tagitems[itemnum].Value == NULL )                            // couldn't allocate memory
        return 1;
    memcpy ( Info->tagitems[itemnum].Value, value, valuesize );

    Info->tagitems[itemnum].Value[valuesize] = '\0';
    Info->tagitems[itemnum].ValueSize        = valuesize;
    Info->tagitems[itemnum].Flags            = flags;
    Info->tagitems[itemnum].Reliability      = reliability;
    return 0;
}

// insert item to tagdata. calls either new or replace
int InsertTagField ( const char* item, size_t itemsize, const char* value, size_t valuesize, const unsigned int flags, StreamInfo* Info )
{
    size_t  i;

    if ( itemsize  == 0 ) itemsize  = strlen ( item  );                     // autodetect size
    if ( valuesize == 0 ) valuesize = strlen ( value );                     // autodetect size

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        // are items case sensitive?
        if ( _stricmp ( item, Info->tagitems[i].Item ) == 0 )               // replace value of first item found
            return ReplaceTagField ( value, valuesize, flags, Info, i, (tag_rel)0 );
    }

    return NewTagField ( item, itemsize, value, valuesize, flags, Info, (tag_rel)0 );// insert new field
}

// insert item to tagdata but only if reliability > previous tag's reliability
int InsertTagFieldLonger ( const char* item, size_t itemsize, const char* value, size_t valuesize, const unsigned int flags, StreamInfo* Info, const enum tag_rel reliability )
{
    size_t  i;

    if ( itemsize  == 0 ) itemsize  = strlen ( item  );                     // autodetect size
    if ( valuesize == 0 ) valuesize = strlen ( value );                     // autodetect size

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        // are items case sensitive?
        if ( _stricmp ( item, Info->tagitems[i].Item ) == 0 ) {
            if ( reliability > Info->tagitems[i].Reliability )              // replace value of first item found
                return ReplaceTagField ( value, valuesize, flags, Info, i, reliability );
            else
                return 0;
        }
    }

    return NewTagField ( item, itemsize, value, valuesize, flags, Info, reliability );  // insert new field
}

// Convert UNICODE to UTF-8
// Return number of bytes written
int unicodeToUtf8 ( const WCHAR* lpWideCharStr, char* lpMultiByteStr, int cwcChars )
{
    const unsigned short*   pwc = (unsigned short *)lpWideCharStr;
    unsigned char*          pmb = (unsigned char  *)lpMultiByteStr;
    const unsigned short*   pwce;
    size_t  cBytes = 0;

    if ( cwcChars >= 0 ) {
        pwce = pwc + cwcChars;
    } else {
        pwce = (unsigned short *)((size_t)-1);
    }

    while ( pwc < pwce ) {
        unsigned short  wc = *pwc++;

        if ( wc < 0x00000080 ) {
            *pmb++ = (char)wc;
            cBytes++;
        } else
        if ( wc < 0x00000800 ) {
            *pmb++ = (char)(0xC0 | ((wc >>  6) & 0x1F));
            cBytes++;
            *pmb++ = (char)(0x80 |  (wc        & 0x3F));
            cBytes++;
        } else
        if ( wc < 0x00010000 ) {
            *pmb++ = (char)(0xE0 | ((wc >> 12) & 0x0F));
            cBytes++;
            *pmb++ = (char)(0x80 | ((wc >>  6) & 0x3F));
            cBytes++;
            *pmb++ = (char)(0x80 |  (wc        & 0x3F));
            cBytes++;
        }
        if ( wc == L'\0' )
            return cBytes;
    }

    return cBytes;
}

// Convert UTF-8 coded string to UNICODE
// Return number of characters converted
int utf8ToUnicode ( const char* lpMultiByteStr, WCHAR* lpWideCharStr, int cmbChars )
{
    const unsigned char*    pmb = (unsigned char  *)lpMultiByteStr;
    unsigned short*         pwc = (unsigned short *)lpWideCharStr;
    const unsigned char*    pmbe;
    size_t  cwChars = 0;

    if ( cmbChars >= 0 ) {
        pmbe = pmb + cmbChars;
    } else {
        pmbe = (unsigned char *)((size_t)-1);
    }

    while ( pmb < pmbe ) {
        char            mb = *pmb++;
        unsigned int    cc = 0;
        unsigned int    wc;

        while ( (cc < 7) && (mb & (1 << (7 - cc)))) {
            cc++;
        }

        if ( cc == 1 || cc > 6 )                    // illegal character combination for UTF-8
            continue;

        if ( cc == 0 ) {
            wc = mb;
        } else {
            wc = (mb & ((1 << (7 - cc)) - 1)) << ((cc - 1) * 6);
            while ( --cc > 0 ) {
                if ( pmb == pmbe )                  // reached end of the buffer
                    return cwChars;
                mb = *pmb++;
                if ( ((mb >> 6) & 0x03) != 2 )      // not part of multibyte character
                    return cwChars;
                wc |= (mb & 0x3F) << ((cc - 1) * 6);
            }
        }

        if ( wc & 0xFFFF0000 )
            wc = L'?';
        *pwc++ = wc;
        cwChars++;
        if ( wc == L'\0' )
            return cwChars;
    }

    return cwChars;
}

// convert Windows ANSI to UTF-8
int ConvertANSIToUTF8 ( const char* ansi, char* utf8 )
{
    WCHAR*  wszValue;          // Unicode value
    size_t  ansi_len;
    size_t  len;

    *utf8 = '\0';
    if ( ansi == NULL )
        return 0;

    ansi_len = strlen ( ansi );

    if ( (wszValue = (WCHAR *)malloc ( (ansi_len + 1) * 2 )) == NULL )
        return 1;

    // Convert ANSI value to Unicode
    if ( (len = MultiByteToWideChar ( CP_ACP, 0, ansi, ansi_len + 1, wszValue, (ansi_len + 1) * 2 )) == 0 ) {
        free ( wszValue );
        return 1;
    }

    // Convert Unicode value to UTF-8
    if ( (len = unicodeToUtf8 ( wszValue, utf8, -1 )) == 0 ) {
    // if ( (len = WideCharToMultiByte ( CP_UTF8, 0, wszValue, -1, utf8, (ansi_len + 1) * 2, NULL, NULL )) == 0 ) {
        free ( wszValue );
        return 1;
    }

    free ( wszValue );

    return 0;
}

// convert UTF-8 to Windows ANSI
int ConvertUTF8ToANSI ( const char* utf8, char* ansi )
{
    WCHAR*  wszValue;          // Unicode value
    size_t  utf8_len;
    size_t  len;

    *ansi = '\0';
    if ( utf8 == NULL )
        return 0;

    utf8_len = strlen ( utf8 );

    if ( (wszValue = (WCHAR *)malloc ( (utf8_len + 1) * 2 )) == NULL )
        return 1;

    // Convert UTF-8 value to Unicode
    if ( (len = utf8ToUnicode ( utf8, wszValue, utf8_len + 1 )) == 0 ) {
    // if ( (len = MultiByteToWideChar ( CP_UTF8, 0, utf8, utf8_len + 1, wszValue, (utf8_len + 1) * 2 )) == 0 ) {
        free ( wszValue );
        return 1;
    }

    // Convert Unicode value to ANSI
    if ( (len = WideCharToMultiByte ( CP_ACP, 0, wszValue, -1, ansi, (utf8_len + 1) * 2, NULL, NULL )) == 0 ) {
        free ( wszValue );
        return 1;
    }

    free ( wszValue );

    return 0;
}

int ConvertTagFieldToUTF8 ( StreamInfo* Info, size_t itemnum )
{
    WCHAR*  wszValue;          // Unicode value
    char*   uszValue;          // UTF-8 value
    size_t  len;
    int     error;

    if ( Info->tagitems[itemnum].Flags & 1<<1 )                             // data in binary
        return 0;

    if ( Info->tagitems[itemnum].Value     == NULL ||                       // nothing to do
         Info->tagitems[itemnum].ValueSize == 0 )
        return 0;

    wszValue = (WCHAR *)malloc ( (Info->tagitems[itemnum].ValueSize + 1) * 2 );
    if ( wszValue == NULL )
        return 1;
    uszValue = (char  *)malloc ( (Info->tagitems[itemnum].ValueSize + 1) * 3 );
    if ( uszValue == NULL ) {
        free ( wszValue );
        return 1;
    }

    // Convert ANSI value to Unicode
    len = MultiByteToWideChar ( CP_ACP, 0, Info->tagitems[itemnum].Value,
        Info->tagitems[itemnum].ValueSize + 1, wszValue,
        (Info->tagitems[itemnum].ValueSize + 1) * 2 );

    if ( len == 0 ) {
        free ( wszValue );
        free ( uszValue );
        return 1;
    }

    // Convert Unicode value to UTF-8
    // len = WideCharToMultiByte ( CP_UTF8, 0, wszValue, -1, uszValue, (Info->tagitems[itemnum].ValueSize + 1) * 2, NULL, NULL );
    len = unicodeToUtf8 ( wszValue, uszValue, -1 );

    if ( len == 0 ) {
        free ( wszValue );
        free ( uszValue );
        return 1;
    }

    error = ReplaceTagField ( uszValue, len-1, Info->tagitems[itemnum].Flags, Info, itemnum, Info->tagitems[itemnum].Reliability );

    free ( wszValue );
    free ( uszValue );

    return error;
}

int ConvertTagFieldToANSI ( StreamInfo* Info, size_t itemnum )
{
    WCHAR*  wszValue;          // Unicode value
    char*   Value;             // ANSI Value
    size_t  len;
    int     error;

    if ( Info->tagitems[itemnum].Flags & 1<<1 )                             // data in binary
        return 0;

    if ( Info->tagitems[itemnum].Value     == NULL ||                       // nothing to do
         Info->tagitems[itemnum].ValueSize == 0 )
        return 0;

    wszValue = (WCHAR *)malloc ( (Info->tagitems[itemnum].ValueSize + 1) * 2 );
    if ( wszValue == NULL )
        return 1;
    Value    = (char  *)malloc ( (Info->tagitems[itemnum].ValueSize + 1) * 2 );
    if ( Value    == NULL ) {
        free ( wszValue );
        return 1;
    }

    // Convert UTF-8 value to Unicode
    // len = MultiByteToWideChar ( CP_UTF8, 0, Info->tagitems[itemnum].Value, Info->tagitems[itemnum].ValueSize + 1, wszValue, (Info->tagitems[itemnum].ValueSize + 1) * 2 );
    len = utf8ToUnicode ( Info->tagitems[itemnum].Value, wszValue, Info->tagitems[itemnum].ValueSize + 1 );

    if ( len == 0 ) {
        free ( wszValue );
        free ( Value );
        return 1;
    }

    // Convert Unicode value to ANSI
    len = WideCharToMultiByte ( CP_ACP, 0, wszValue, -1,
        Value, (Info->tagitems[itemnum].ValueSize + 1) * 2, NULL, NULL );

    if ( len == 0 ) {
        free ( wszValue );
        free ( Value );
        return 1;
    }

    error = ReplaceTagField ( Value, len-1, Info->tagitems[itemnum].Flags, Info, itemnum, Info->tagitems[itemnum].Reliability );

    free ( wszValue );
    free ( Value );

    return error;
}

// replace list separator characters in tag field
int ReplaceListSeparator ( const char* old_sep, const char* new_sep, StreamInfo* Info, size_t itemnum )
{
    unsigned char*  new_value;
    unsigned char*  p;
    size_t          os_len;
    size_t          ns_len;
    size_t          count;
    size_t          new_len;
    size_t          i;
    int             error;

    if ( Info->tagitems[itemnum].Flags & 1<<1 )                             // data in binary
        return 0;

    os_len = strlen ( old_sep );
    ns_len = strlen ( new_sep );
    if ( os_len == 0 ) os_len = 1;                                          // allow null character
    if ( ns_len == 0 ) ns_len = 1;

    if ( Info->tagitems[itemnum].Value     == NULL ||                       // nothing to do
         Info->tagitems[itemnum].ValueSize == 0 )
        return 0;

    count = 0;
    for ( i = 0; i < Info->tagitems[itemnum].ValueSize - os_len + 1; i++ ) {
        if ( memcmp ( Info->tagitems[itemnum].Value+i, old_sep, os_len ) == 0 )
            count++;
    }

    if ( count == 0 )
        return 0;

    new_len = Info->tagitems[itemnum].ValueSize - (count * os_len) + (count * ns_len);
    if ( (new_value = (unsigned char *)malloc ( new_len )) == NULL )
        return 1;

    p = new_value;
    for ( i = 0; i < Info->tagitems[itemnum].ValueSize; i++ ) {
        if ( i + os_len - 1 >= Info->tagitems[itemnum].ValueSize ||
             memcmp ( Info->tagitems[itemnum].Value+i, old_sep, os_len ) != 0 ) {
            *p++ = Info->tagitems[itemnum].Value[i];
        } else {
            memcpy ( p, new_sep, ns_len );
            p += ns_len;
            i += os_len - 1;
        }
    }

    error = ReplaceTagField ( (const char*)new_value, new_len, Info->tagitems[itemnum].Flags, Info, itemnum, Info->tagitems[itemnum].Reliability );

    free ( new_value );

    return error;
}

int
GenreToInteger ( const char* GenreStr)
{
    size_t  i;

    for ( i = 0; i < sizeof(GenreList) / sizeof(*GenreList); i++ ) {
        if ( 0 == _stricmp ( GenreStr, GenreList [i] ) )
            return i;
    }

    return -1;
}

void
GenreToString ( char* GenreStr, const unsigned int genre )
{
    GenreStr [0] = '\0';
    if ( genre < NO_GENRES )
        strcpy ( GenreStr, GenreList [genre] );
}

/* F U N C T I O N S */

/*
 *  Copies src to dst. Copying is stopped at `\0' char is detected or if
 *  len chars are copied.
 *  Trailing blanks are removed and the string is `\0` terminated.
 */

static void
memcpy_crop ( char* dst, const char* src, size_t len )
{
    size_t  i;

    for ( i = 0; i < len; i++ )
        if  ( src[i] != '\0' )
            dst[i] = src[i];
        else
            break;

    // dst[i] points behind the string contents
    while ( i > 0  &&  dst [i-1] == ' ' )
        i--;

    dst [i] = '\0';
}

// replaces % sequences with real characters
void fix_percentage_sequences ( char* string )
{
    char temp[_MAX_PATH];
    char* t = temp;
    char* s = string;
    int value;
    int b1, b2; //, b3, b4;
    int v1, v2; //, v3, v4;

    do {
        value = *s++;

        if ( value == '%' ) {
            if ( *s != '\0' )   b1 = *s++;
            else                b1 = '\0';
            if ( *s != '\0' )   b2 = *s++;
            else                b2 = '\0';

            if ( ((b1 >= '0' && b1 <= '9') || (b1 >= 'A' && b1 <= 'F')) &&
                 ((b2 >= '0' && b2 <= '9') || (b2 >= 'A' && b2 <= 'F')) ) {

                if ( b1 <= '9' )  v1 = b1 - '0';
                else              v1 = b1 - 'A' + 10;
                if ( b2 <= '9' )  v2 = b2 - '0';
                else              v2 = b2 - 'A' + 10;

                if ( v1 == 0 && v2 == 0 ) {         // %00xx
                    /*
                    if ( *s != '\0' )   b1 = *s++;
                    else                b1 = '\0';
                    if ( *s != '\0' )   b2 = *s++;
                    else                b2 = '\0';
                    if ( *s != '\0' )   b3 = *s++;
                    else                b2 = '\0';
                    if ( *s != '\0' )   b4 = *s++;
                    else                b4 = '\0';

                    if ( b1 <= '9' )  v1 = b1 - '0';
                    else              v1 = b1 - 'A' + 10;
                    if ( b2 <= '9' )  v2 = b2 - '0';
                    else              v2 = b2 - 'A' + 10;
                    if ( b3 <= '9' )  v3 = b3 - '0';
                    else              v3 = b3 - 'A' + 10;
                    if ( b4 <= '9' )  v4 = b4 - '0';
                    else              v4 = b4 - 'A' + 10;

                    if ( v1 != 0 || v2 != 0 ) {
                        *t++ = ' ';                 // no multibyte support, unknown character
                    } else {                        // %0000+xxxx+
                        *t++ = ' ';

                        while ( *s && ((*s >= '0' && *s <= '9') || (*s >= 'A' && *s <= 'F')) )
                            s++;
                    }
                    */

                    *t++ = ' ';

                    while ( *s && ((*s >= '0' && *s <= '9') || (*s >= 'A' && *s <= 'F')) )
                        s++;
                } else {                            // %xx
                    *t++ = (v1 << 4) + v2;
                }
            } else {                                // %aa
                *t++ = '%';
                *t++ = b1;
                *t++ = b2;
            }
        } else {
            *t++ = value;
        }
    } while ( value != '\0' );

    strcpy ( string, temp );
}

// gets tag information from filename
int GuessTagFromName ( const char* filename, const char* naming_scheme, StreamInfo* Info )
{
    char scheme  [_MAX_PATH];

    char name    [_MAX_PATH];
    char dir     [_MAX_DIR];
    char fname   [_MAX_FNAME];

    char Title   [_MAX_PATH];
    char Artist  [_MAX_PATH];
    char Album   [_MAX_PATH];
    char Track   [_MAX_PATH];
    char Genre   [_MAX_PATH];
    char Year    [_MAX_PATH];
    char Comment [_MAX_PATH];
    char discard [_MAX_PATH];

    char utf8    [_MAX_PATH * 3];

    char *dest = NULL;      // destination
    char *sep  = NULL;      // field separator
    unsigned int s_pos = 0; // position in scheme
    unsigned int f_pos = 0; // position in filename
    unsigned int d_len = 0; // length of destination
    unsigned int s_len = 0; // length of source
    unsigned long len;      // length of name
    int paths = 0;
    unsigned int i;

    strcpy ( name, filename );
    for ( i = 0; i < strlen (name) - 5; i++ ) {
        if ( name[i] != '\\' && name[i] != '/' ) continue;

        if ( _strnicmp ( (char *)(name+i+1), "(CD", 3 ) == 0 || _strnicmp ( (char *)(name+i+1), "(DVD", 4 ) == 0 )
            name[i] = ' ';
    }

    _splitpath( name, NULL, dir, fname, NULL );

    if ( strlen ( fname ) == 0 || strlen (naming_scheme) >= _MAX_PATH )
        return 1;

    for ( i = 0; i <= strlen (naming_scheme); i++ ) {
        char c = naming_scheme[i];
        if ( c == '\\' || c == '/' ) {
            c = '\\';
            if ( scheme[i + 1] == '\\' ) i++;
            paths++;
        }
        scheme[i] = c;
    }

    if ( paths > 0 ) {
        int path_pos = strlen ( dir ) - 1;

        while ( paths > 0 && path_pos >= 0 ) {
            if ( dir[--path_pos] == '\\' ) paths--;
        }
        path_pos++;

        strcpy ( name, (char *)(dir+path_pos) );
        strcat ( name, fname );
        len = strlen ( name );
    } else {
        strcpy ( name, fname );
        len = strlen ( name );
    }

    Title  [0] = '\0';
    Artist [0] = '\0';
    Album  [0] = '\0';
    Track  [0] = '\0';
    Genre  [0] = '\0';
    Year   [0] = '\0';
    Comment[0] = '\0';

    for ( s_pos = 0; s_pos < strlen (scheme) + 1; s_pos++ ) {
        if ( scheme[s_pos] == '%' ) {
            s_pos++;
            sep = (char *)(scheme+s_pos+1);
            s_len = 0;
            while ( (sep < (char *)(scheme+strlen(scheme)+1)) && ((*(sep+s_len) != '%' && *(sep+s_len) != '\0')) ) s_len++;
            if ( s_len == 0 ) s_len = 1;

            switch ( scheme[s_pos] ) {
            case 't':   // title
            case 'T':
                dest  = Title;
                d_len = sizeof (Title);
                break;
            case 'a':   // artist
            case 'A':
                dest  = Artist;
                d_len = sizeof (Artist);
                break;
            case 'l':   // album
            case 'L':
                dest  = Album;
                d_len = sizeof (Album);
                break;
            case 'c':   // comment
            case 'C':
                dest  = Comment;
                d_len = sizeof (Comment);
                break;
            case 'n':   // track
            case 'N':
                dest  = Track;
                d_len = sizeof (Track);
                break;
            case 'y':   // year
            case 'Y':
                dest  = Year;
                d_len = sizeof (Year);
                break;
            case 'g':   // genre
            case 'G':
                dest  = Genre;
                d_len = sizeof (Genre);
                break;
            case 'x':   // disrecard
            case 'X':
                dest  = discard;
                d_len = sizeof (discard);
                break;
            default:
                dest  = NULL;
                d_len = 0;
                break;
            }
        }

        if ( dest != NULL && sep != NULL ) {
            char *bak = dest;
            char *end = (char *)(dest + d_len);

            while ( f_pos < len && strncmp ((char*)(name+f_pos), sep, s_len) != 0 ) {
                if ( dest < end ) {
                    char c = name[f_pos++];
                    if ( c == '_' ) c = ' ';
                    if ( c == '\'' && f_pos < len && name[f_pos] == '\'' ) {    // '' -> "
                        f_pos++;
                        c = '"';
                    }
                    if ( c == ' ' && f_pos < len && name[f_pos] == ' ' ) {      // '  ' -> ' '
                        f_pos++;
                    }
                    *dest++ = c;
                } else {
                    f_pos++;
                }
            }

            if ( dest < end ) *dest++ = '\0';
            while ( dest != bak && *dest == ' ' ) *dest-- = '\0';
            if ( name[f_pos] == sep[0] ) f_pos += s_len;
            while ( name[f_pos] == ' ' ) f_pos++;

            dest = NULL;
        } else {
            if ( name[f_pos] == scheme[s_pos] ) f_pos++;
        }
    }

    fix_percentage_sequences ( Title   );
    fix_percentage_sequences ( Artist  );
    fix_percentage_sequences ( Album   );
    fix_percentage_sequences ( Track   );
    fix_percentage_sequences ( Genre   );
    fix_percentage_sequences ( Year    );
    fix_percentage_sequences ( Comment );

    if ( strlen ( Album ) > 6 && Year[0] == '\0' ) {
        int l = strlen ( Album );
        if ( (Album[l-6] == '(' || Album[l-6] == '[') && (Album[l-1] == ')' || Album[l-1] == ']') &&
            ((Album[l-5] == '1' && Album[l-4] == '9') || (Album[l-5] == '2' && Album[l-4] == '0')) &&
            (Album[l-3] >= '0' && Album[l-3] <= '9' && Album[l-2] >= '0' && Album[l-2] <= '9') ) {

            memcpy ( Year, (char *)(Album+l-5), 4 );
            Year[4] = '\0';
            if ( Album[l-7] == ' ' )
                Album[l-7] = '\0';
            else
                Album[l-6] = '\0';
        }
    }

    if ( Title[0] != '\0' ) {
        ConvertANSIToUTF8 ( Title,   utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_TITLE,   0, utf8, 0, 0, Info, guess );
    }
    if ( Artist[0] != '\0' ) {
        ConvertANSIToUTF8 ( Artist,  utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_ARTIST,  0, utf8, 0, 0, Info, guess );
    }
    if ( Album[0] != '\0' ) {
        ConvertANSIToUTF8 ( Album,   utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_ALBUM,   0, utf8, 0, 0, Info, guess );
    }
    if ( Track[0] != '\0' ) {
        ConvertANSIToUTF8 ( Track,   utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_TRACK,   0, utf8, 0, 0, Info, guess );
    }
    if ( Genre[0] != '\0' ) {
        ConvertANSIToUTF8 ( Genre,   utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_GENRE,   0, utf8, 0, 0, Info, guess );
    }
    if ( Year[0] != '\0' ) {
        ConvertANSIToUTF8 ( Year,    utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_YEAR,    0, utf8, 0, 0, Info, guess );
    }
    if ( Comment[0] != '\0' ) {
        ConvertANSIToUTF8 ( Comment, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_COMMENT, 0, utf8, 0, 0, Info, guess );
    }

    Info->tagtype = guessed_tag;

    return 0;
}

// gets pattern of filename scheme
int GuessTagFromName_Test ( const char* filename, const char* naming_scheme, char* pattern )
{
    char scheme  [_MAX_PATH];

    char name    [_MAX_PATH];
    char dir     [_MAX_DIR];
    char fname   [_MAX_FNAME];

    char *dest = NULL;      // destination
    char *sep  = NULL;      // field separator
    unsigned int s_pos = 0; // position in scheme
    unsigned int f_pos = 0; // position in filename
    unsigned int d_len = 0; // length of destination
    unsigned int s_len = 0; // length of source
    unsigned int pat_p = 0; // position in pattern
    unsigned long len;      // length of name
    int paths = 0;
    unsigned int i;
    return 0;
    strcpy ( name, filename );
    for ( i = 0; i < strlen (name) - 5; i++ ) {
        if ( name[i] != '\\' && name[i] != '/' ) continue;

        if ( _strnicmp ( (char *)(name+i+1), "(CD", 3 ) == 0 || _strnicmp ( (char *)(name+i+1), "(DVD", 4 ) == 0 )
            name[i] = ' ';
    }

    _splitpath( name, NULL, dir, fname, NULL );

    if ( strlen ( fname ) == 0 || strlen (naming_scheme) >= _MAX_PATH )
        return 1;

    for ( i = 0; i <= strlen (naming_scheme); i++ ) {
        char c = naming_scheme[i];
        if ( c == '\\' || c == '/' ) {
            c = '\\';
            if ( scheme[i + 1] == '\\' ) i++;
            paths++;
        }
        scheme[i] = c;
    }

    if ( paths > 0 ) {
        int path_pos = strlen ( dir ) - 1;

        while ( paths > 0 && path_pos >= 0 ) {
            if ( dir[--path_pos] == '\\' ) paths--;
        }
        path_pos++;

        strcpy ( name, (char *)(dir+path_pos) );
        strcat ( name, fname );
        len = strlen ( name );
    } else {
        strcpy ( name, fname );
        len = strlen ( name );
    }

    *pattern = '\0';
    while ( scheme[s_pos] != '%' ) {
        pattern[pat_p++] = name[f_pos++];
        pattern[pat_p  ] = '\0';
        s_pos++;
    }

    for ( ; s_pos < strlen (scheme) + 1; s_pos++ ) {
        if ( scheme[s_pos] == '%' ) {
            s_pos++;
            sep = (char *)(scheme+s_pos+1);
            s_len = 0;
            while ( (sep < (char *)(scheme+strlen(scheme)+1)) && ((*(sep+s_len) != '%' && *(sep+s_len) != '\0')) ) s_len++;
            if ( s_len == 0 ) s_len = 1;

            switch ( scheme[s_pos] ) {
            case 't':   // title
            case 'T':
            case 'a':   // artist
            case 'A':
            case 'l':   // album
            case 'L':
            case 'c':   // comment
            case 'C':
            case 'n':   // track
            case 'N':
            case 'y':   // year
            case 'Y':
            case 'g':   // genre
            case 'G':
            case 'x':   // disrecard
            case 'X':
                dest  = (char *)(pattern + pat_p);
                d_len = 1;
                break;
            default:
                dest  = NULL;
                d_len = 0;
                break;
            }
        }

        if ( dest != NULL && sep != NULL ) {
            char *bak = dest;
            char *end = (char *)(dest + d_len);

            while ( f_pos < len && strncmp ((char*)(name+f_pos), sep, s_len) != 0 ) {
                if ( dest < end ) {
                    char c = name[f_pos++];
                    if ( c == '_' ) c = ' ';
                    if ( c == '\'' && f_pos < len && name[f_pos] == '\'' ) {    // '' -> "
                        f_pos++;
                        c = '"';
                    }
                    if ( c == ' ' && f_pos < len && name[f_pos] == ' ' ) {      // '  ' -> ' '
                        f_pos++;
                    }
                    *dest++ = c;
                } else {
                    f_pos++;
                }
            }

            *dest++ = '\0';
            if ( name[f_pos] == sep[0] ) f_pos += s_len;
            while ( name[f_pos] == ' ' ) f_pos++;
            strncat ( pattern, sep, s_len );
            pat_p = strlen ( pattern );
            dest = NULL;
        }
    }

    return 0;
}

// finds best matching naming scheme
int FindBestGuess ( const char* filename )
{
    static const char  identifiers [] = "ATLCGNYX";

    char               scheme  [_MAX_PATH];
    char               test    [_MAX_PATH];
    int                closest_match = -1;
    unsigned int       i, j, k;

    for ( i = 0; i < sizeof (SchemeList)/sizeof (*SchemeList); i++ ) {
        GuessTagFromName_Test ( filename, SchemeList[i], test );

        k = 0;
        for ( j = 0; j <= strlen ( SchemeList[i]); j++ ) {
            if ( SchemeList[i][j] == '%' ) continue;
            scheme[k++] = SchemeList[i][j];
        }

        if ( strlen ( test ) == strlen ( scheme ) ) {
            int match = 1;

            for ( j = 0; j < strlen ( test ); j++ ) {
                int is_identifier = 0;
                int tc = test[j];
                int sc = scheme[j];

                if ( sc >= 'a' && sc <= 'z' ) sc -= 'a'-'A';
                for ( k = 0; k < sizeof (identifiers); k++ ) {
                    if ( sc == identifiers[k] ) {
                        is_identifier = 1;
                        break;
                    }
                }

                if ( !is_identifier && sc == tc )   // field separator
                    continue;

                switch (sc) {
                case '/':   // path separator
                case '\\':
                    sc = '/';
                    break;
                case 'N':   // numerical
                case 'Y':
                    sc = 'N';
                    break;
                default:    // text
                    sc = 'A';
                    break;
                }

                if ( tc == '/' || tc == '\\' ) {
                    tc = '/';
                } else
                if ( tc >= '0' && tc <= '9' ) {
                    tc = 'N';
                } else {
                    tc = 'A';
                }

                if ( closest_match == -1 && (sc == 'A' && tc == 'N') )
                    closest_match = (int)i;

                if ( sc != tc ) {
                    if ( sc == 'N' && tc == 'A' && closest_match == (int)i ) {
                        closest_match = -1;
                    }
                    match = 0;
                }
            }

            if ( match ) return i;
        }
    }

    return closest_match;
}

// guess tag information from filename
int StreamInfo::GuessTag ( const char* filename )
{
    int best;

    if ( (best = FindBestGuess ( GetFilename() )) < 0 )
        return 1;

    return GuessTagFromName ( GetFilename(), SchemeList[best], this );
}

// searches and reads a ID3v1 tag
int StreamInfo::ReadID3v1Tag ( Reader* fp)
{
/*    unsigned */char   tmp [128];
/*    unsigned */char   value [32];
    char            utf8[32*2];

    if ( fp->seek ( simple.TagOffset - sizeof (tmp), SEEK_SET ) != 0 )
        return 0;
    if ( fp->read  ( tmp, sizeof (tmp) ) != sizeof (tmp) )
        return 0;
    // check for id3-tag
    if ( 0 != memcmp (tmp, "TAG", 3) )
        return 0;

    memcpy_crop ( value, tmp +  3, 30 );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_TITLE  , 0, utf8, 0, 0, this, ID3v1 );
    }
    memcpy_crop ( value, tmp + 33, 30 );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_ARTIST , 0, utf8, 0, 0, this, ID3v1 );
    }
    memcpy_crop ( value, tmp + 63, 30 );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_ALBUM  , 0, utf8, 0, 0, this, ID3v1 );
    }
    memcpy_crop ( value, tmp + 93,  4 );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_YEAR   , 0, utf8, 0, 0, this, ID3v1 );
    }
    memcpy_crop ( value, tmp + 97, 30 );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_COMMENT, 0, utf8, 0, 0, this, ID3v1 );
    }

    if ( tmp[125] == 0 ) {
        sprintf ( value, "%d", tmp[126] );
        if ( value[0] != '\0' && atoi (value) != 0 ) {
            ConvertANSIToUTF8 ( value, utf8 );
            InsertTagFieldLonger ( APE_TAG_FIELD_TRACK, 0, utf8, 0, 0, this, ID3v1 );
        }
    }
    GenreToString ( value, tmp[127] );
    if ( value[0] != '\0' ) {
        ConvertANSIToUTF8 ( value, utf8 );
        InsertTagFieldLonger ( APE_TAG_FIELD_GENRE  , 0, utf8, 0, 0, this, ID3v1 );
    }

    simple.TagOffset -= 128;
    tagtype = ID3v1_tag;

    return 0;
}


struct APETagFooterStruct {
    unsigned char   ID       [8];    // should equal 'APETAGEX'
    unsigned char   Version  [4];    // 1000 = version 1.0, 2000 = version 2.0
    unsigned char   Length   [4];    // complete size of the tag, including footer, excluding header
    unsigned char   TagCount [4];    // number of fields in the tag
    unsigned char   Flags    [4];    // tag flags (none currently defined)
    unsigned char   Reserved [8];    // reserved for later use
};


static unsigned long
Read_LE_Uint32 ( const unsigned char* p )
{
    return ((unsigned long)p[0] <<  0) |
           ((unsigned long)p[1] <<  8) |
           ((unsigned long)p[2] << 16) |
           ((unsigned long)p[3] << 24);
}

static void
Write_LE_Uint32 ( unsigned char* p, const unsigned long value )
{
    p[0] = (unsigned char) (value >>  0);
    p[1] = (unsigned char) (value >>  8);
    p[2] = (unsigned char) (value >> 16);
    p[3] = (unsigned char) (value >> 24);
}

int
StreamInfo::ReadAPE1Tag ( Reader* fp  )
{
    unsigned long               vsize;
    unsigned long               isize;
    unsigned long               flags;
    /*unsigned */char*              buff;
    /*unsigned */char*              p;
    /*unsigned */char*              end;
    char*                       utf8;
    struct APETagFooterStruct   T;
    unsigned long               TagLen;
    unsigned long               TagCount;

    if ( fp->seek ( simple.TagOffset - sizeof T, SEEK_SET ) != 0 )
        return 0;
    if ( fp->read ( &T, sizeof T ) != sizeof T )
        return 0;
    if ( memcmp ( T.ID, "APETAGEX", sizeof T.ID ) != 0 )
        return 0;
    if ( Read_LE_Uint32 (T.Version) != 1000 )
        return 0;
    TagLen = Read_LE_Uint32 (T.Length);
    if ( TagLen <= sizeof T )
        return 0;
    if ( fp->seek ( simple.TagOffset - TagLen, SEEK_SET ) != 0 )
        return 0;
    buff = (char *)malloc ( TagLen );
    if ( buff == NULL )
        return 1;
    if ( fp->read ( buff, TagLen - sizeof T ) != TagLen - sizeof T ) {
        free ( buff );
        return 0;
    }

    TagCount = Read_LE_Uint32 (T.TagCount);
    end = buff + TagLen - sizeof (T);
    for ( p = buff; p < end /*&& *p */ &&  TagCount--; ) {
        vsize = Read_LE_Uint32 ( (unsigned char *) p ); p += 4;
        flags = Read_LE_Uint32 ( (unsigned char *) p ); p += 4;
        isize = strlen (p);

        if ( vsize > 0 ) {
            if ( (utf8 = (char*)malloc ( (vsize + 1) * 3 )) == NULL ) {
                free ( buff );
                return 1;
            }
            ConvertANSIToUTF8 ( p + isize + 1, utf8 );
            InsertTagFieldLonger ( p, isize, utf8, 0, 0, this, APE1 );  // flags not used with APE 1.0
            free ( utf8 );
        }
        p += isize + 1 + vsize;
    }

    simple.TagOffset -= TagLen;
    tagtype = APE1_tag;

    free ( buff );

    return 0;
}


int
StreamInfo::ReadAPE2Tag ( Reader* fp )
{
    unsigned long               vsize;
    unsigned long               isize;
    unsigned long               flags;
    /*unsigned */char*              buff;
    /*unsigned */char*              p;
    /*unsigned */char*              end;
    struct APETagFooterStruct   T;
    unsigned long               TagLen;
    unsigned long               TagCount;
    size_t                      i;

    if ( fp->seek ( simple.TagOffset - sizeof T, SEEK_SET ) != 0 )
        return 0;
    if ( fp->read ( &T, sizeof T ) != sizeof T )
        return 0;
    if ( memcmp ( T.ID, "APETAGEX", sizeof T.ID ) != 0 )
        return 0;
    if ( Read_LE_Uint32 (T.Version) != 2000 )
        return 0;
    TagLen = Read_LE_Uint32 (T.Length);
    if ( TagLen <= sizeof T )
        return 0;
    if ( fp->seek ( simple.TagOffset - TagLen, SEEK_SET ) != 0 )
        return 0;
    buff = (char *)malloc ( TagLen );
    if ( buff == NULL )
        return 1;
    if ( fp->read ( buff, TagLen - sizeof T ) != TagLen - sizeof T ) {
        free ( buff );
        return 0;
    }

    TagCount = Read_LE_Uint32 (T.TagCount);
    end = buff + TagLen - sizeof (T);
    for ( p = buff; p < end /*&& *p */ &&  TagCount--; ) {
        vsize = Read_LE_Uint32 ( (unsigned char*)p ); p += 4;
        flags = Read_LE_Uint32 ( (unsigned char*)p ); p += 4;
        isize = strlen (p);

        if ( vsize > 0 )
            InsertTagFieldLonger ( p, isize, p + isize + 1, vsize, flags, this, APE2 );
        p += isize + 1 + vsize;
    }

    simple.TagOffset -= TagLen;
    tagtype = APE2_tag;

    free ( buff );

    for ( i = 0; i < tagitem_count; i++ ) {
        if ( ReplaceListSeparator ( "\0", ListSeparator, this, i ) != 0 )
            return 2;
    }

    if ( Read_LE_Uint32 (T.Flags) & 1<<31 ) {       // Tag contains header
        simple.TagOffset -= sizeof (T);
    } else {                                        // Check if footer was incorrect
        if ( fp->seek ( simple.TagOffset - sizeof T, SEEK_SET ) != 0 )
            return 0;
        if ( fp->read ( &T, sizeof T ) != sizeof T )
            return 0;
        if ( memcmp ( T.ID, "APETAGEX", sizeof T.ID ) != 0 )
            return 0;
        if ( Read_LE_Uint32 (T.Version) != 2000 )
            return 0;
        if ( Read_LE_Uint32 (T.Flags) & 1<<29 )     // This is header
            simple.TagOffset -= sizeof T;
    }

    return 0;
}

BOOL _validForID3 ( const char* string, int maxsize )
{
    const unsigned char* p = (unsigned char *)string;

    while ( *p ) {
        if ( *p++ >= 0x80 ) return FALSE;           // only ASCII characters are reliable in ID3v1
    }

    return ( ((char*)p - string) <= maxsize );
}

// checks if ID3v1 tag can store all tag information
// return 0 if can, 1 if can't
int CheckID3V1TagDataLoss ( const StreamInfo* Info )
{
    const char* id3v1_items[] = {
        APE_TAG_FIELD_TITLE,        // 0
        APE_TAG_FIELD_ARTIST,       // 1
        APE_TAG_FIELD_ALBUM,        // 2
        APE_TAG_FIELD_YEAR,         // 3
        APE_TAG_FIELD_COMMENT,      // 4
        APE_TAG_FIELD_TRACK,        // 5
        APE_TAG_FIELD_GENRE         // 6
    };
    char*   value;
    size_t  i, j;

    if ( (value = TagValue ( APE_TAG_FIELD_TRACK, Info )) ) {
        char* p = value;
        while ( *p ) {
            if ( *p < '0' || *p > '9' ) return 1;                   // not a number
            p++;
        }
        if ( atoi (value) <   0 ||                                  // number won't fit in byte
             atoi (value) > 255 )
            return 1;
    }

    if ( ( value = TagValue ( APE_TAG_FIELD_GENRE, Info )) ) {
        if ( value[0] != '\0' && (unsigned int)GenreToInteger (value) >= NO_GENRES ) // non-standard genre
            return 1;
    }

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        BOOL bFound = FALSE;
        for ( j = 0; j < sizeof (id3v1_items) / sizeof (*id3v1_items); j++ ) {
            if ( _stricmp ( Info->tagitems[i].Item, id3v1_items[j] ) == 0 ) {
                int maxlen;
                switch ( j ) {
                case 3:     // Year
                    maxlen =  4;
                    break;
                case 4:     // Comment
                    value = TagValue ( APE_TAG_FIELD_TRACK, Info );
                    if ( value && strcmp ( value, "" ) != 0 )
                        maxlen = 28;
                    else
                        maxlen = 30;
                    break;
                default:
                    maxlen = 30;
                    break;
                }
                if ( !_validForID3 ( Info->tagitems[i].Value, maxlen ) )
                    return 1;
                bFound = TRUE;
                break;
            }
        }
        if ( !bFound ) return 1;                                    // field not possible in ID3v1
    }

    return 0;
}

// remove all tags
int WriteNoTag ( Reader* fp, StreamInfo* Info )
{
    if ( fp->seek (  0L, SEEK_END ) != 0 )
        return 2;
    if ( fp->tell () != Info->simple.TotalFileLength )                       // file size does not match
        return 2;
    if ( fp->seek ( Info->simple.TagOffset, SEEK_SET ) != 0)
        return 2;
	fp->seteof(Info->simple.TagOffset);


    Info->simple.TotalFileLength = Info->simple.TagOffset;                  // store new file size

    return 0;
}

// write ID3v1.0 / 1.1 tag
int WriteID3V1Tag ( Reader* fp, StreamInfo* Info )
{
    /*unsigned */char  tmp [128];
    size_t         i;

    if ( fp->seek (  0L, SEEK_END ) != 0 )
        return 2;
    if ( fp->tell () != Info->simple.TotalFileLength )                       // file size does not match
        return 2;
    if ( fp->seek ( Info->simple.TagOffset, SEEK_SET ) != 0 )
        return 2;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( ConvertTagFieldToANSI ( Info, i ) != 0 )
            return 2;
    }

    memset ( tmp, 0, sizeof (tmp) );
    memcpy ( tmp +   0, "TAG", 3 );
    CopyTagValue ( tmp +   3, APE_TAG_FIELD_TITLE,  Info, 30 );
    CopyTagValue ( tmp +  33, APE_TAG_FIELD_ARTIST, Info, 30 );
    CopyTagValue ( tmp +  63, APE_TAG_FIELD_ALBUM,  Info, 30 );
    CopyTagValue ( tmp +  93, APE_TAG_FIELD_YEAR,   Info,  4 );

    // if track# is used, write ID3v1.1 format
    if ( TagValue ( APE_TAG_FIELD_TRACK, Info ) && TagValue ( APE_TAG_FIELD_TRACK, Info )[0] != '\0' ) {
        CopyTagValue ( tmp +  97, APE_TAG_FIELD_COMMENT, Info, 28 );
        tmp[126] = atoi ( TagValue ( APE_TAG_FIELD_TRACK, Info ) );
    } else {
        CopyTagValue ( tmp +  97, APE_TAG_FIELD_COMMENT, Info, 30 );
    }

    if ( TagValue ( APE_TAG_FIELD_GENRE, Info ) )
        tmp[127] = GenreToInteger ( TagValue ( APE_TAG_FIELD_GENRE, Info ) );
    else
        tmp[127] = (unsigned char)255;

    if ( fp->write ( tmp,  sizeof (tmp) ) != sizeof (tmp) )
        return 2;

	fp->seteof(fp->tell());
//    if ( _chsize ( _fileno (fp), ftell (fp) ) != 0 )
//        return 1;

    Info->simple.TotalFileLength = Info->simple.TagOffset + sizeof (tmp);

    return CheckID3V1TagDataLoss ( Info );
}

// write APE tag version 1.0
int WriteAPE1Tag ( Reader* fp, StreamInfo* Info )
{
    unsigned char               temp[4];
    struct APETagFooterStruct   T;
    size_t                      TagCount;
    size_t                      TagSize;
    size_t                      i;

    if ( fp->seek (  0, SEEK_END ) != 0 )
        return 2;
    if ( fp->tell () != Info->simple.TotalFileLength )                       // file size does not match
        return 2;
    if ( fp->seek ( Info->simple.TagOffset, SEEK_SET ) != 0 )
        return 2;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( ConvertTagFieldToANSI ( Info, i ) != 0 )
            return 2;
    }

    TagCount = 0;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( Info->tagitems[i].Value != NULL && Info->tagitems[i].ValueSize > 0 ) { // don't write empty items
            Write_LE_Uint32 ( temp, Info->tagitems[i].ValueSize + 1 );
            if ( fp->write ( temp, 4 ) != 4 )
                return 2;
            memset ( temp, 0, sizeof (temp) );
            if ( fp->write ( temp, 4 ) != 4 )
                return 2;
            if ( fp->write ( Info->tagitems[i].Item ,Info->tagitems[i].ItemSize  + 1 ) != Info->tagitems[i].ItemSize  + 1 )
                return 2;
            if ( fp->write ( Info->tagitems[i].Value, Info->tagitems[i].ValueSize + 1 ) != Info->tagitems[i].ValueSize + 1 )
                return 2;
            TagCount++;
        }
    }

    TagSize = fp->tell () - Info->simple.TagOffset + sizeof (T);

    memcpy ( T.ID, "APETAGEX", sizeof (T.ID) );     // ID String
    Write_LE_Uint32 ( T.Version , 1000     );       // Version 1.000
    Write_LE_Uint32 ( T.Length  , TagSize  );       // Tag size
    Write_LE_Uint32 ( T.TagCount, TagCount );       // Number of fields
    memset ( T.Flags   , 0, sizeof (T.Flags   ) );  // Flags
    memset ( T.Reserved, 0, sizeof (T.Reserved) );  // Reserved

    if ( fp->write ( &T, sizeof (T)) != sizeof (T) )
        return 2;

	fp->seteof(fp->tell());

    Info->simple.TotalFileLength = Info->simple.TagOffset + TagSize;

    return 0;   // no data loss
}

// write APE tag version 2.0
int WriteAPE2Tag ( Reader* fp, StreamInfo* Info )
{
    unsigned char               temp[4];
    unsigned char*              buff;
    unsigned char*              p;
    struct APETagFooterStruct   T;
    unsigned int                flags;
    size_t                      TagCount;
    size_t                      TagSize;
    size_t                      i;

    if ( fp->seek ( 0L, SEEK_END ) != 0 )
        return 2;
    if ( fp->tell () != Info->simple.TotalFileLength )                       // file size does not match
        return 2;
    if ( fp->seek ( Info->simple.TagOffset, SEEK_SET ) != 0 )
        return 2;

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( ReplaceListSeparator ( ListSeparator, "\0", Info, i ) != 0 )
            return 2;
    }

    TagCount = 0;

    TagSize = sizeof (T);                                                   // calculate size of buffer needed
    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( Info->tagitems[i].Value != NULL && Info->tagitems[i].ValueSize > 0 ) {
            TagSize += 8 + Info->tagitems[i].ItemSize + 1 + Info->tagitems[i].ValueSize;
            TagCount++;
        }
    }

    if ( (buff = (unsigned char*)malloc ( TagSize + sizeof (T) )) == NULL )
        return 2;

    p = buff;

    flags  = 1<<31;                                     // contains header
    flags |= 1<<29;                                     // this is the header
    memcpy ( T.ID, "APETAGEX", sizeof (T.ID) );         // ID String
    Write_LE_Uint32 ( T.Version , 2000     );           // Version 2.000
    Write_LE_Uint32 ( T.Length  , TagSize  );           // Tag size
    Write_LE_Uint32 ( T.TagCount, TagCount );           // Number of fields
    Write_LE_Uint32 ( T.Flags   , flags    );           // Flags
    memset ( T.Reserved, 0, sizeof (T.Reserved) );      // Reserved
    memcpy ( p, &T, sizeof (T) );   p += sizeof (T);                        // insert header

    for ( i = 0; i < Info->tagitem_count; i++ ) {
        if ( Info->tagitems[i].Value != NULL && Info->tagitems[i].ValueSize > 0 ) { // don't write empty items
            Write_LE_Uint32 ( temp, Info->tagitems[i].ValueSize );
            memcpy ( p, temp, 4 );  p += 4;
            Write_LE_Uint32 ( temp, Info->tagitems[i].Flags );
            memcpy ( p, temp, 4 );  p += 4;

            memcpy ( p, Info->tagitems[i].Item , Info->tagitems[i].ItemSize + 1 );
            p += Info->tagitems[i].ItemSize + 1;

            memcpy ( p, Info->tagitems[i].Value, Info->tagitems[i].ValueSize );
            p += Info->tagitems[i].ValueSize;
        }
    }

    // TagSize = ftell (fp) - Info->simple.TagOffset + sizeof (T);

    flags  = 1<<31;                                     // contains header
    memcpy ( T.ID, "APETAGEX", sizeof (T.ID) );         // ID String
    Write_LE_Uint32 ( T.Version , 2000     );           // Version 2.000
    Write_LE_Uint32 ( T.Length  , TagSize  );           // Tag size - header
    Write_LE_Uint32 ( T.TagCount, TagCount );           // Number of fields
    Write_LE_Uint32 ( T.Flags   , flags    );           // Flags
    memset ( T.Reserved, 0, sizeof (T.Reserved) );      // Reserved
    memcpy ( p, &T, sizeof (T) );                                           // insert footer

    if ( fp->write ( buff, TagSize + sizeof (T)) != TagSize + sizeof (T) ) {
        free ( buff );
        return 2;
    }

	fp->seteof(fp->tell());

    Info->simple.TotalFileLength = Info->simple.TagOffset + TagSize + sizeof (T);
    free ( buff );

    return 0;   // no data loss
}

// scan file for all supported tags and read them
int StreamInfo::ReadTags ( Reader* fp )
{
    off_t TagOffs;

    do {
        TagOffs = simple.TagOffset;
        ReadAPE1Tag  ( fp );
        ReadAPE2Tag  ( fp );
        ReadID3v1Tag ( fp );
    } while ( TagOffs != simple.TagOffset );

    if ( tagtype == no_tag ) {
        GuessTag ( GetFilename() );
    }

    return 0;
}

// decide best tag to write
int WriteAutoTag ( Reader* fp, StreamInfo* Info )
{
    if ( CheckID3V1TagDataLoss ( Info ) == 0 )
        return WriteID3V1Tag ( fp, Info );
    else
        return WriteAPE2Tag  ( fp, Info );
}

// write tag specified in StreamInfo
int WriteTag ( Reader* fp, StreamInfo* Info )
{
    switch ( Info->tagtype ) {
    case ID3v1_tag:
        return WriteID3V1Tag ( fp, Info );

    case APE1_tag:
        return WriteAPE1Tag  ( fp, Info );

    case APE2_tag:
        return WriteAPE2Tag  ( fp, Info );

    case auto_tag:
        return WriteAutoTag  ( fp, Info );

    default:
        return 0;
    }
}


// searches for a ID3v2-tag and reads the length (in bytes) of it
// -1 on errors of any kind

long
JumpID3v2 ( Reader* fp )
{
    unsigned char  tmp [10];
    unsigned int   Unsynchronisation;   // ID3v2.4-flag
    unsigned int   ExtHeaderPresent;    // ID3v2.4-flag
    unsigned int   ExperimentalFlag;    // ID3v2.4-flag
    unsigned int   FooterPresent;       // ID3v2.4-flag
    long           ret;

    fp->read  ( tmp, sizeof(tmp) );

    // check id3-tag
    if ( 0 != memcmp ( tmp, "ID3", 3) )
        return 0;

    // read flags
    Unsynchronisation = tmp[5] & 0x80;
    ExtHeaderPresent  = tmp[5] & 0x40;
    ExperimentalFlag  = tmp[5] & 0x20;
    FooterPresent     = tmp[5] & 0x10;

    if ( tmp[5] & 0x0F )
        return -1;              // not (yet???) allowed
    if ( (tmp[6] | tmp[7] | tmp[8] | tmp[9]) & 0x80 )
        return -1;              // not allowed

    // read HeaderSize (syncsave: 4 * $0xxxxxxx = 28 significant bits)
    ret  = tmp[6] << 21;
    ret += tmp[7] << 14;
    ret += tmp[8] <<  7;
    ret += tmp[9]      ;
    ret += 10;
    if ( FooterPresent )
        ret += 10;

    return ret;
}
