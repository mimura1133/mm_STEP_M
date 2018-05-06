#include "stdafx.h"
#include "FileFLAC.h"

#include "file_decoder.h"
#include "assert.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

static FLAC__byte reservoir_[FLAC__MAX_BLOCK_SIZE * 2 * 2 * 2]; /* *2 for max bytes-per-sample, *2 for max channels, another *2 for overflow */
static unsigned reservoir_samples_ = 0;

static FLAC__StreamDecoderWriteStatus write_callback_(const FLAC__FileDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
static void metadata_callback_(const FLAC__FileDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
static void error_callback_   (const FLAC__FileDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

FLAC__StreamDecoderWriteStatus write_callback_(const FLAC__FileDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
    file_info_struct *file_info = (file_info_struct *)client_data;
    const unsigned bps = file_info->bits_per_sample, channels = file_info->channels, wide_samples = frame->header.blocksize;
    unsigned wide_sample, sample, channel;
    FLAC__int8 *scbuffer = (FLAC__int8*)reservoir_;
    FLAC__int16 *ssbuffer = (FLAC__int16*)reservoir_;

    (void)decoder;

    if(file_info->abort_flag)
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

    if(bps == 8) {
        for(sample = reservoir_samples_*channels, wide_sample = 0; wide_sample < wide_samples; wide_sample++)
            for(channel = 0; channel < channels; channel++, sample++)
                scbuffer[sample] = (FLAC__int8)buffer[channel][wide_sample];
    }
    else if(bps == 16) {
        for(sample = reservoir_samples_*channels, wide_sample = 0; wide_sample < wide_samples; wide_sample++)
            for(channel = 0; channel < channels; channel++, sample++)
                ssbuffer[sample] = (FLAC__int16)buffer[channel][wide_sample];
    }
    else {
        file_info->abort_flag = true;
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }

    reservoir_samples_ += wide_samples;

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void metadata_callback_(const FLAC__FileDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    file_info_struct *file_info = (file_info_struct *)client_data;
    (void)decoder;
    if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
        //FLAC__ASSERT(metadata->data.stream_info.total_samples < 0x100000000); /* this plugin can only handle < 4 gigasamples */
        file_info->total_samples = (unsigned)(metadata->data.stream_info.total_samples&0xffffffff);
        file_info->bits_per_sample = metadata->data.stream_info.bits_per_sample;
        file_info->channels = metadata->data.stream_info.channels;
        file_info->sample_rate = metadata->data.stream_info.sample_rate;

        if(file_info->bits_per_sample == 8) {
            file_info->sample_format = FMT_S8;
        }
        else if(file_info->bits_per_sample == 16) {
            file_info->sample_format = FMT_S16_NE;
        }
        else {
            file_info->abort_flag = true;
            return;
        }
        file_info->length_in_msec = file_info->total_samples * 10 / (file_info->sample_rate / 100);
    }
}

void error_callback_(const FLAC__FileDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
    file_info_struct *file_info = (file_info_struct *)client_data;
    (void)decoder;
    if(status != FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC)
        file_info->abort_flag = true;
}

char * strndup(char* string, int size) {
	char* buff = (char*)malloc(size+1);
	memset(buff, 0, size+1);
	memcpy(buff, string, size);
	//buff[size] = '0';
	return buff;
}

CString convert_from_utf8(const char* UTF8)
{
	auto size = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, 0, 0);
	std::vector<wchar_t> buff(size);
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, buff.data(), buff.size());
	return buff.data();
}

char* convert_to_utf8(const char* SJIS)
{
	unsigned char* data;
	char* buff = NULL;
	int size1, size2;
	size1 = MultiByteToWideChar(CP_ACP, 0, SJIS, -1, 0, 0);
	size1 = size1*sizeof(WCHAR);
	data = (unsigned char*)malloc(size1);
	if(data) {
		MultiByteToWideChar(CP_ACP, 0, SJIS, -1, (LPWSTR)data,size1/sizeof(WCHAR));

		size2 = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)data, size1/sizeof(WCHAR),0,0,NULL,NULL);
		size2++;
		buff = (char*)malloc(size2);
		if (buff) {
			WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)data, size1/sizeof(WCHAR), buff, size2, NULL, NULL);
			buff[size2-1] = '\0';
		}
		delete data;
	}
	return buff;
}


/***************
 * Declaration *
 ***************/

#define MULTIFIELD_SEPARATOR " - "

/* FLAC uses Ogg Vorbis comments
 * Ogg Vorbis fields names :
 *  - TITLE        : Track name
 *  - VERSION      : The version field may be used to differentiate multiple version of the same track title in a single collection. (e.g. remix info)
 *  - ALBUM        : The collection name to which this track belongs
 *  - TRACKNUMBER  : The track number of this piece if part of a specific larger collection or album
 *  - ARTIST       : Track performer
 *  - ORGANIZATION : Name of the organization producing the track (i.e. the 'record label')
 *  - DESCRIPTION  : A short text description of the contents
 *  - GENRE        : A short text indication of music genre
 *  - DATE         : Date the track was recorded
 *  - LOCATION     : Location where track was recorded
 *  - COPYRIGHT    : Copyright information
 *  - ISRC         : ISRC number for the track; see the ISRC intro page for more information on ISRC numbers.
 *
 * Field names should not be 'internationalized'; this is a concession to simplicity
 * not an attempt to exclude the majority of the world that doesn't speak English.
 * Field *contents*, however, are represented in UTF-8 to allow easy representation
 * of any language.
 */


/*************
 * Functions *
 *************/

/*
 * Read tag data from a FLAC file.
 * Note:
 *  - if field is found but contains no info (strlen(str)==0), we don't read it
 */
boolean Flac_Tag_Read_File_Tag(LPCTSTR filename, File_Tag *FileTag)
{
    FLAC__Metadata_SimpleIterator               *iter;
    FLAC__StreamMetadata                        *vc_block;
    FLAC__StreamMetadata_VorbisComment          *vc;
    FLAC__StreamMetadata_VorbisComment_Entry    *field;
    char                                       *field_value;
    char                                       *string = NULL;
    char                                       *string1 = NULL;
    int                                        field_num;
    int                                        field_len;
    unsigned int                                       i;


    flac_error_msg = NULL;
    
    iter = FLAC__metadata_simple_iterator_new();
    if ( iter == NULL || !FLAC__metadata_simple_iterator_init(iter, filename, true, false) )
    {
        if ( iter == NULL )
        {
            flac_error_msg = FLAC__Metadata_SimpleIteratorStatusString[FLAC__METADATA_SIMPLE_ITERATOR_STATUS_MEMORY_ALLOCATION_ERROR];
        }else
        {
            flac_error_msg = FLAC__Metadata_SimpleIteratorStatusString[FLAC__metadata_simple_iterator_status(iter)];
            FLAC__metadata_simple_iterator_delete(iter);
        }
        
        ////g_print(_("ERROR while opening file: '%s' as FLAC (%s).\n\a"),filename,flac_error_msg);
        return FALSE;
    }

    /* libFLAC is able to detect (and skip) ID3v2 tags by itself */
    
    /* Find the VORBIS_COMMENT block */
    while ( FLAC__metadata_simple_iterator_get_block_type(iter) != FLAC__METADATA_TYPE_VORBIS_COMMENT )
    {
        if ( !FLAC__metadata_simple_iterator_next(iter) )
        {
            /* End of metadata: comment block not found, nothing to read */
            FLAC__metadata_simple_iterator_delete(iter);
            return TRUE;
        }
    }

    /* Get comments from block */
    vc_block = FLAC__metadata_simple_iterator_get_block(iter);
    vc = &vc_block->data.vorbis_comment;

    /*********
     * Title *
     *********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"title")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
      
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->title==NULL)
                FileTag->title = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->title) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->title);
				free(FileTag->title);
				FileTag->title = multi;
                FileTag->title = strcat(FileTag->title, MULTIFIELD_SEPARATOR);
                FileTag->title = strcat(FileTag->title, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /**********
     * Artist *
     **********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"artist")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
      
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->artist==NULL)
                FileTag->artist = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->artist) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->artist);
				free(FileTag->artist);
				FileTag->artist = multi;
                FileTag->artist = strcat(FileTag->artist, MULTIFIELD_SEPARATOR);
                FileTag->artist = strcat(FileTag->artist, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /*********
     * Album *
     *********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"album")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
      
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->album==NULL)
                FileTag->album = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->album) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->album);
				free(FileTag->album);
				FileTag->album = multi;
                FileTag->album = strcat(FileTag->album, MULTIFIELD_SEPARATOR);
                FileTag->album = strcat(FileTag->album, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /********
     * Year *
     ********/
    if ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,0,"date")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            FileTag->year = strdup(string);
            free(string);
            free(field_value);
        }            
    }

    /*************************
     * Track and Total Track *
     *************************/
    if ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,0,"tracknumber")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            string1 = strchr(string,'/');
            if (string1)
            {
                FileTag->track_total = strdup(string1+1);
                *string1 = '\0';
            }
            FileTag->track = strdup(string);
            free(string);
            free(field_value);
        }            
    }

    /*********
     * Genre *
     *********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"genre")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->genre==NULL)
                FileTag->genre = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->genre) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->genre);
				free(FileTag->genre);
				FileTag->genre = multi;
                FileTag->genre = strcat(FileTag->genre, MULTIFIELD_SEPARATOR);
                FileTag->genre = strcat(FileTag->genre, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /***********
     * Comment *
     ***********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"comment"/*"description"*/)) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->comment==NULL)
                FileTag->comment = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->comment) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->comment);
				free(FileTag->comment);
				FileTag->comment = multi;
                FileTag->comment = strcat(FileTag->comment, MULTIFIELD_SEPARATOR);
                FileTag->comment = strcat(FileTag->comment, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /***********
     * Composer*
     ***********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"composer")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->composer==NULL)
                FileTag->composer = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->composer) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->composer);
				free(FileTag->composer);
				FileTag->composer = multi;
                FileTag->composer = strcat(FileTag->comment, MULTIFIELD_SEPARATOR);
                FileTag->composer = strcat(FileTag->comment, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /***********
     * Performer*
     ***********/
    field_num = 0;
    while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(vc_block,field_num,"performer")) >= 0 )
    {
        /* Extract field value */
        field = &vc->comments[field_num++];
        field_value = (char*)memchr(field->entry, '=', field->length);
        
        if ( field_value && strlen(field_value)>0 )
        {
            field_value++;
            field_len = field->length - (field_value - (char*) field->entry);
            field_value = strndup(field_value, field_len);
            string = convert_from_utf8(field_value);
            //Strip_String(string);
            if (FileTag->performer==NULL)
                FileTag->performer = strdup(string);
            else {
				char * multi = (char*)malloc(strlen(FileTag->performer) + strlen(MULTIFIELD_SEPARATOR) + strlen(string) + 1);
				strcpy(multi, FileTag->performer);
				free(FileTag->performer);
				FileTag->performer = multi;
                FileTag->performer = strcat(FileTag->comment, MULTIFIELD_SEPARATOR);
                FileTag->performer = strcat(FileTag->comment, string);
			}
            free(string);
            free(field_value);
        }                
    }

    /***************************
     * Save unsupported fields *
     ***************************/
    for (i=0;i<(unsigned int)vc->num_comments;i++)
    {
        field = &vc->comments[i];
        if ( strnicmp((char*)field->entry,"title=",       MIN(6,  field->length)) != 0
          && strnicmp((char*)field->entry,"artist=",      MIN(7,  field->length)) != 0
          && strnicmp((char*)field->entry,"album=",       MIN(6,  field->length)) != 0
          && strnicmp((char*)field->entry,"date=",        MIN(5,  field->length)) != 0
          && strnicmp((char*)field->entry,"tracknumber=", MIN(12, field->length)) != 0
          && strnicmp((char*)field->entry,"genre=",       MIN(6,  field->length)) != 0
          && strnicmp((char*)field->entry,"composer=",    MIN(9,  field->length)) != 0
          && strnicmp((char*)field->entry,"performer=",   MIN(10, field->length)) != 0
          && strnicmp((char*)field->entry,"comment=",     MIN(8,  field->length)) != 0 )
        {
            ////g_print("custom %*s\n", field->length, field->entry);
            FileTag->other->Add(strndup((char*)field->entry, field->length));
        }
    }

    FLAC__metadata_object_delete(vc_block);
    FLAC__metadata_simple_iterator_delete(iter);

#ifdef SUPPORT_ID3TAG_READ
    /* If no FLAC vorbis tag found : we try to get the ID3 tag if it exists (will be deleted when writing the tag */
    if ( FileTag->title       == NULL
      && FileTag->artist      == NULL
      && FileTag->album       == NULL
      && FileTag->year        == NULL
      && FileTag->track       == NULL
      && FileTag->track_total == NULL
      && FileTag->genre       == NULL
      && FileTag->comment     == NULL)
    {
        int rc;
        
        rc = Id3tag_Read_File_Tag(filename,FileTag);
        
        // If an ID3 tag has been found, we mark the file as unsaved to rewritte a flac tag
        if ( FileTag->title       != NULL
          || FileTag->artist      != NULL
          || FileTag->album       != NULL
          || FileTag->year        != NULL
          || FileTag->track       != NULL
          || FileTag->track_total != NULL
          || FileTag->genre       != NULL
          || FileTag->comment     != NULL)
        {
            FileTag->saved = FALSE;
        }
        
        return rc;
    }
#endif
    
    return TRUE;
}

unsigned long Get_File_Size (LPCTSTR filename)
{
    struct _stat statbuf;
    FLAC__Metadata_SimpleIterator               *iter;
    FLAC__StreamMetadata                        *vc_block;

    if (filename)
    {
        _stat(filename,&statbuf);
        //return statbuf.st_size;
		iter = FLAC__metadata_simple_iterator_new();
		if ( iter == NULL || !FLAC__metadata_simple_iterator_init(iter, filename, true, false) )
			return 0;
		unsigned int tagSize = 0;
		while ((vc_block = FLAC__metadata_simple_iterator_get_block(iter)) != NULL)
		{
			tagSize += vc_block->length;
			if ( !FLAC__metadata_simple_iterator_next(iter) )
			{
				/* End of metadata */
				break;;
			}
		}
	    FLAC__metadata_simple_iterator_delete(iter);
		return statbuf.st_size - tagSize - 20;
    }else
    {
        return 0;
    }
}

boolean Flac_Header_Read_File_Info(LPCTSTR filename, File_Tag *FileTag)
{
    FILE *file;
    double duration = 0;
    unsigned long filesize;

    FLAC__FileDecoder *tmp_decoder;
    file_info_struct tmp_file_info;


    if ( (file= _tfopen(filename, TEXT("r")))==NULL )
    {
        //g_print(_("ERROR while opening file: '%s' (%s)\n\a"),filename,g_strerror(errno));
        return FALSE;
    }
    fclose(file);

    /* Decoding FLAC file */
    tmp_decoder = FLAC__file_decoder_new();
    if(tmp_decoder == 0)
        return FALSE;

    tmp_file_info.abort_flag = false;
    FLAC__file_decoder_set_md5_checking     (tmp_decoder, false);
    FLAC__file_decoder_set_filename         (tmp_decoder, filename);
    FLAC__file_decoder_set_write_callback   (tmp_decoder, write_callback_);
    FLAC__file_decoder_set_metadata_callback(tmp_decoder, metadata_callback_);
    FLAC__file_decoder_set_error_callback   (tmp_decoder, error_callback_);
    FLAC__file_decoder_set_client_data      (tmp_decoder, &tmp_file_info);
    if(FLAC__file_decoder_init(tmp_decoder) != FLAC__FILE_DECODER_OK)
        return FALSE;

    // In FLAC 1.0.3, is used : FLAC__file_decoder_process_metadata
    // In FLAC 1.0.4, is used : FLAC__file_decoder_process_until_end_of_metadata
//    if(!FLAC__file_decoder_process_metadata(tmp_decoder)) // FLAC 1.0.3
    if(!FLAC__file_decoder_process_until_end_of_metadata(tmp_decoder)) // FLAC 1.0.4 (Bastian Kleineidam)
        return FALSE;

    FLAC__file_decoder_finish(tmp_decoder);
    FLAC__file_decoder_delete(tmp_decoder);
    /* End of decoding FLAC file */


    filesize = Get_File_Size(filename);
	struct stat _stat;
	stat(filename, &_stat);
    //duration = (int)tmp_file_info.length_in_msec/1000;

    //if (tmp_file_info.length_in_msec/1000.0 > 0) {
        //FileTag->bitrate = filesize*8.0/tmp_file_info.length_in_msec;
    if (tmp_file_info.length_in_msec > 0) {
        FileTag->bitrate = 
					8.0 * (float)(filesize) / (1000.0 * (float)tmp_file_info.total_samples / (float)tmp_file_info.sample_rate);
					// bitrateがあわないときは、_stat.st_size で計算していると思われる
					// _statでは単にファイルのサイズであり、filesize はタグ分を抜いたサイズ
	}
    FileTag->samplerate  = tmp_file_info.sample_rate;
    FileTag->mode        = tmp_file_info.channels;
    FileTag->size        = filesize;
    FileTag->duration    = tmp_file_info.length_in_msec/1000;

    return TRUE;
}

boolean Flac_Tag_Write_File_Tag (LPCTSTR filename, File_Tag *FileTag)
{
    const char                                       *filename_in = filename;
    FLAC__Metadata_SimpleIterator               *iter;
    FLAC__StreamMetadata                        *vc_block;
    FLAC__StreamMetadata_VorbisComment_Entry    field;
    FLAC__bool                                  write_ok;
    char                                       *string, *string1;

    flac_error_msg = NULL;

    /* libFLAC is able to detect (and skip) ID3v2 tags by itself */

    iter = FLAC__metadata_simple_iterator_new();
    if ( iter == NULL || !FLAC__metadata_simple_iterator_init(iter,filename_in,false,false) )
    {
        if ( iter == NULL )
        {
            flac_error_msg = FLAC__Metadata_SimpleIteratorStatusString[FLAC__METADATA_SIMPLE_ITERATOR_STATUS_MEMORY_ALLOCATION_ERROR];
        }else
        {
            flac_error_msg = FLAC__Metadata_SimpleIteratorStatusString[FLAC__metadata_simple_iterator_status(iter)];
            FLAC__metadata_simple_iterator_delete(iter);
        }
        
        //g_print(_("ERROR while opening file: '%s' as FLAC (%s).\n\a"),filename_in,flac_error_msg);
        return FALSE;
    }
    
    /* Find the VORBIS_COMMENT block */
    while ( FLAC__metadata_simple_iterator_get_block_type(iter) != FLAC__METADATA_TYPE_VORBIS_COMMENT )
    {
        if ( !FLAC__metadata_simple_iterator_next(iter) )
            break;
    }

    /* Write tag */
    if ( FLAC__metadata_simple_iterator_get_block_type(iter) != FLAC__METADATA_TYPE_VORBIS_COMMENT )
	    vc_block = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
	else
		vc_block = FLAC__metadata_simple_iterator_get_block(iter);

    /*********
     * Title *
     *********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "title");
    if ( FileTag->title && strlen(FileTag->title) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "title=");
        strcat(string, FileTag->title);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
	}

    /**********
     * Artist *
     **********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "artist");
    if ( FileTag->artist && strlen(FileTag->artist) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "artist=");
        strcat(string, FileTag->artist);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /*********
     * Album *
     *********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "album");
    if ( FileTag->album && strlen(FileTag->album) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "album=");
        strcat(string, FileTag->album);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /********
     * Year *
     ********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "date");
    if ( FileTag->year && strlen(FileTag->year) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "date=");
        strcat(string, FileTag->year);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /*************************
     * Track and Total Track *
     *************************/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "tracknumber");
    if ( FileTag->str_track /*FileTag->track*/ && strlen(FileTag->str_track) > 0)
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
		/*
        if ( FileTag->track_total && strlen(FileTag->track_total)>0 ) {
            strcpy(string, "tracknumber=");
            strcat(string, FileTag->track);
            strcat(string, "/");
            strcat(string, FileTag->track_total);
		} else {
            strcpy(string, "tracknumber=");
            strcat(string, FileTag->track);
		}
		*/
        strcpy(string, "tracknumber=");
        strcat(string, FileTag->str_track);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /*********
     * Genre *
     *********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "genre");
    if ( FileTag->genre && strlen(FileTag->genre) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "genre=");
        strcat(string, FileTag->genre);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /***********
     * Comment *
     ***********/
    // We write the comment using the "both" format
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "comment");
    if ( FileTag->comment && strlen(FileTag->comment) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "comment=");
        strcat(string, FileTag->comment);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /***********
     * Composer*
     ***********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "composer");
    if ( FileTag->composer && strlen(FileTag->composer) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "composer=");
        strcat(string, FileTag->composer);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /***********
     * Performer*
     ***********/
	FLAC__metadata_object_vorbiscomment_remove_entry_matching(vc_block, "performer");
    if ( FileTag->performer && strlen(FileTag->performer) > 0 )
    {
		string = (char*)malloc(1024+1);//すべてのタグで入力できる最大+1
        strcpy(string, "performer=");
        strcat(string, FileTag->performer);
        string1 = convert_to_utf8(string);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
        free(string);
        free(string1);
    }

    /**************************
     * Set unsupported fields *
     **************************/
	/*
	for (int i=0;i<FileTag->other->GetSize();i++)
    {
        string1 = (char*)FileTag->other->GetAt(i);
        field.entry = (unsigned char*)string1;
        field.length = strlen(string1);
        FLAC__metadata_object_vorbiscomment_insert_comment(vc_block,vc_block->data.vorbis_comment.num_comments,field,true);
    }
	*/
    
    /* Find the VORBIS_COMMENT block */
	/*
    while ( FLAC__metadata_simple_iterator_get_block_type(iter) != FLAC__METADATA_TYPE_VORBIS_COMMENT )
    {
        if ( !FLAC__metadata_simple_iterator_next(iter) )
            break;
    }
	*/

    /* Write tag */
    if ( FLAC__metadata_simple_iterator_get_block_type(iter) != FLAC__METADATA_TYPE_VORBIS_COMMENT )
    {
        /* End of metadata: no comment block, so insert one */
		write_ok = FLAC__metadata_simple_iterator_insert_block_after(iter,vc_block,true);
    }
    else
    {
        write_ok = FLAC__metadata_simple_iterator_set_block(iter,vc_block,true);
    }        

    if ( !write_ok )
    {
        flac_error_msg = FLAC__Metadata_SimpleIteratorStatusString[FLAC__metadata_simple_iterator_status(iter)];
        //g_print(_("ERROR: Failed to write comments to file '%s' (%s).\n"),filename_in,flac_error_msg);
        FLAC__metadata_simple_iterator_delete(iter);
        FLAC__metadata_object_delete(vc_block);
        return FALSE;
    }else
    {
        //g_print(_("Written tag of '%s'\n"),g_basename(filename_in));
    }

    FLAC__metadata_simple_iterator_delete(iter);
    FLAC__metadata_object_delete(vc_block);
    
    // FIX ME! : Remove the ID3 tag if found....

    return TRUE;
}

bool LoadFileFLAC(FILE_INFO *pFile)
{
	File_Tag FileTag;
	memset(&FileTag, 0, sizeof(FileTag));
	FileTag.other = new CPtrArray();
	Flac_Tag_Read_File_Tag(GetFullPath(pFile), &FileTag);
	Flac_Header_Read_File_Info(GetFullPath(pFile), &FileTag);

	SetTrackNameSI(pFile, FileTag.title);
	SetArtistNameSI(pFile, FileTag.artist);
	SetComposerSI(pFile, FileTag.composer);
	SetCommentSI(pFile, FileTag.comment);
	//SetSoftwareSI(pFile, FileTag.);
	SetYearSI(pFile, FileTag.year);
	SetAlbumNameSI(pFile, FileTag.album);
	char trackNo[10] = { 0 };
	if (FileTag.track_total != NULL) {
		if (atoi(FileTag.track_total) > 0) {
			sprintf(trackNo, "%d/%d", atoi(FileTag.track), atoi(FileTag.track_total));
		} else {
			sprintf(trackNo, "%d", atoi(FileTag.track));
		}
	} else {
		if (FileTag.track != NULL) {
			sprintf(trackNo, "%d", atoi(FileTag.track));
		}
	}
	SetTrackNumberSI(pFile, trackNo);
	SetGenreSI(pFile, FileTag.genre);
	//SetKeywordSI(pFile, FileTag.);
	SetOrigArtistSI(pFile, FileTag.performer);

	SetPlayTime(pFile, FileTag.duration);

	char format[30];
	sprintf(format, "FLAC %d kbs %d Hz", FileTag.bitrate, FileTag.samplerate);
	SetAudioFormat(pFile, format);

	free(FileTag.title);
	free(FileTag.artist);
	free(FileTag.composer);
	free(FileTag.comment);
	free(FileTag.year);
	free(FileTag.album);
	free(FileTag.str_track);
	free(FileTag.genre);
	free(FileTag.performer);
	free(FileTag.track);
	free(FileTag.track_total);
	for (int i=0;i<FileTag.other->GetSize();i++) {
		free(FileTag.other->GetAt(i));
	}
	delete FileTag.other;
	return TRUE;
}

bool WriteFileFLAC(FILE_INFO *pFile)
{
	File_Tag FileTag;
	memset(&FileTag, 0, sizeof(FileTag));
	FileTag.other = new CPtrArray();
	//Flac_Tag_Read_File_Tag(GetFullPath(pFile), &FileTag); // FitaTag.other への設定

	free(FileTag.title);
	FileTag.title = (char*)GetTrackNameSI(pFile);
	free(FileTag.artist);
	FileTag.artist = (char*)GetArtistNameSI(pFile);
	free(FileTag.composer);
	FileTag.composer = (char*)GetComposerSI(pFile);
	free(FileTag.comment);
	FileTag.comment = (char*)GetCommentSI(pFile);
	free(FileTag.year);
	FileTag.year = (char*)GetYearSI(pFile);
	free(FileTag.album);
	FileTag.album = (char*)GetAlbumNameSI(pFile);
	free(FileTag.str_track);
	FileTag.str_track = (char*)GetTrackNumberSI(pFile);
	free(FileTag.genre);
	FileTag.genre = (char*)GetGenreSI(pFile);
	free(FileTag.performer);
	FileTag.performer = (char*)GetOrigArtistSI(pFile);

	bool ret = Flac_Tag_Write_File_Tag(GetFullPath(pFile), &FileTag) ? TRUE : FALSE;
	free(FileTag.track);
	free(FileTag.track_total);
	for (int i=0;i<FileTag.other->GetSize();i++) {
		free(FileTag.other->GetAt(i));
	}
	delete FileTag.other;
	return ret;
}