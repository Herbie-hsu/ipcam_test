/**
 *
 * SONiX SDK Example Code
 * Category: Audio Codec
 * File: snx_audio_record.c
 * Usage: 
 *		 1. The record result would be saved in /tmp folder.
 *		 2. Execute snx_audio_record (/usr/bin/example/snx_audio_record)
 *       3. The encoding would start and write to the sd card.
 *       4. It would stop after 10 seconds encoding.
 *		 5. Copy the audio data from /tmp folder to the sdcard 
 * NOTE:
 */
#include "snx_audio_codec.h"

/*-----------------------------------------------------------------------------
 * Example Code Configuration
 *----------------------------------------------------------------------------*/

#define SNX_AUDIO_CAP_FORMAT		SND_PCM_FORMAT_S16
									/*  support the following encode format
										SND_PCM_FORMAT_S8, SND_PCM_FORMAT_U8
										SND_PCM_FORMAT_S16, SND_PCM_FORMAT_U18
										SND_PCM_FORMAT_A_LAW, SND_PCM_FORMAT_MU_LAW
										SND_PCM_FORMAT_G726_16, SND_PCM_FORMAT_G726_24
										SND_PCM_FORMAT_G726_32, SND_PCM_FORMAT_G726_40
										SND_PCM_FORMAT_G722
									*/
#define SNX_AUDIO_CAP_FILENAME		"/tmp/audio1"

/*-----------------------------------------------------------------------------
 * Example Code Flow
 *----------------------------------------------------------------------------*/

/* 
  This example reads from the specifiend PCM device 
  and writes to SD card for 10 seconds of data. 
 */  
  
int main()   
{

	int ret = 0;
	snx_audio_stream_conf_t *cap = NULL;
	cap = malloc(sizeof(snx_audio_stream_conf_t));

	cap->format = SNX_AUDIO_CAP_FORMAT;
	
	ret = audio_cap_format_check(cap);
	if (ret < 0) {
		printf("[SNX-AUDIO] audio format check error\n");
		goto FREE_CAP;
	}
	
	cap->open_mode = SND_PCM_NONBLOCK;
	cap->channel = 1;
	cap->sample_rate = 8000;
	
	cap->fd = fopen(SNX_AUDIO_CAP_FILENAME,"wb");
	if (cap->fd <=0 ) {
		printf("[SNX-AUDIO] capture file open failed\n");
		return 0;
	}

	ret = pthread_create(&cap->stream_thread, NULL, (void *)snx_audio_cap_flow, cap);
	if(ret != 0) {
		perror_exit(1, "exit thread creation failed");   
	}
	
	pthread_join(cap->stream_thread,NULL);

	fclose(cap->fd);
	
FREE_CAP:
	free(cap);
  
    return 0;
}