/**
 *
 * SONiX SDK Example Code
 * Category: Audio Codec
 * File: snx_audio_playback.c
 * Usage: 
 *		 1. Make sure the nokia.raw is in the same path with snx_audio_playback
 *		 2. Execute snx_audio_playback (/usr/bin/example/snx_audio_playback)
 *       3. The audio data (SNX_AUDIO_PLAYBACK_FILENAME)
 *  		would be loaded and decoded.
 *       4. The playback would be stop when all data have been decoded.
 * NOTE:
 */
#include "snx_audio_codec.h"

/*-----------------------------------------------------------------------------
 * Example Code Configuration
 *----------------------------------------------------------------------------*/

#define SNX_AUDIO_PLAYBACK_FORMAT		SND_PCM_FORMAT_S8

										/*  support the following encode format
										SND_PCM_FORMAT_S8, SND_PCM_FORMAT_U8
										SND_PCM_FORMAT_S16, SND_PCM_FORMAT_U16
										
										modify alsa conf for more format support
										SND_PCM_FORMAT_A_LAW, SND_PCM_FORMAT_MU_LAW
										SND_PCM_FORMAT_G726_16, SND_PCM_FORMAT_G726_24
										SND_PCM_FORMAT_G726_32, SND_PCM_FORMAT_G726_40
										SND_PCM_FORMAT_G722
										*/
#define SNX_AUDIO_PLAYBACK_FILENAME		"Nokia.raw"

/*-----------------------------------------------------------------------------
 * Example Code Flow
 *----------------------------------------------------------------------------*/

/* 
  This example reads from the default PCM device 
  and writes to standard output for 5 seconds of data. 
 */  
  
int main()
{    
    int ret = 0;
	snx_audio_stream_conf_t *pb = NULL;
	pb = malloc(sizeof(snx_audio_stream_conf_t));

	pb->format = SNX_AUDIO_PLAYBACK_FORMAT;

	ret = audio_pb_format_check(pb);
	if (ret < 0) {
		printf("[SNX-AUDIO] audio format check error\n");
		goto FREE_PB;
	}

	pb->open_mode = 0;
	pb->channel = 1;
	pb->sample_rate = 8000;
	
	pb->fd = fopen(SNX_AUDIO_PLAYBACK_FILENAME,"rb");
	if (pb->fd <=0 ) {
		printf("[SNX-AUDIO] playback file open failed\n");
		return 0;
	}

	ret = pthread_create(&pb->stream_thread, NULL, (void *)snx_audio_pb_flow, pb);
	if(ret != 0) {
		perror_exit(1, "exit thread creation failed");   
	}
	
	pthread_join(pb->stream_thread,NULL);

	fclose(pb->fd);
FREE_PB:
	free(pb);
  
    return 0;  
}