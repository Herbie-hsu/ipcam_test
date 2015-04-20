/**
 *
 * SONiX SDK Example Code
 * Category: Video Encode
 * File: video_codec.h
 * NOTE:
 *       
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>        
#include <pthread.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <sys/stat.h>
#include <malloc.h>
#include <linux/videodev2.h>
#include "snx_common.h"
#include "snx_vc_lib.h"
#include "snx_rc_lib.h"

#define DS_BUFFER_SIZE			128

#define fps_debug				1			// [0, 1] debug message print
#define DS_RED					"81 90 240"
#define DS_GREEN				"144 53 34"
#define DS_BLUE					"40 240 109"
#define DS_YELLOW				"210 16 146"
#define DS_MAGENTA				"106 202 221"
#define DS_CYAN					"91 146 73"
#define DS_BLACK				"16 128 128"
#define DS_WHITE				"255 128 128"

#define DS_SET_ALL				0
#define DS_GET_ALL				1
#define DS_SET_EN				2
#define DS_GET_EN				3
#define DS_SET_COLOR			4
#define DS_GET_COLOR			5
#define DS_SET_COLOR_ATTR		6
#define DS_GET_COLOR_ATTR		7
#define DS_SET_POS				8
#define DS_GET_POS				9
#define DS_SET_DATA				10
#define DS_GET_DATA				11
#define DS_SET_SCALE			12
#define DS_GET_SCALE			13
#define DS_SET_STRING			14
#define DS_SET_BMP				15


/* 
	stream_conf_t is used for stream configuration, including m2m config, 
	device config, filename config, frame number setup, and the thread control
*/
typedef struct stream_conf_s {
	/* Video encoder control structure */
	struct snx_m2m m2m;					//For stream config
	struct snx_cds cds;					//Data stamp config
	
	int fd;								//filedescriptor 
	int frame_num;						//Frame number
	
	/* m2m or capture thread control */
	pthread_t stream_thread;
	
	/* Used for multi-thread control: One M2M thread with multi capture threads */
	pthread_cond_t	*cond;				//Point to the m2m stream cond
	pthread_mutex_t	*mutex;				//Point to the m2m stream mutex
	int 			*ref;				//Point to the m2m stream ref variable
	int				live;				//live streaming or not
} stream_conf_t;

//Before capture stream starts, make sure one m2m stream is started.
void snx_cap_flow(void *arg);

//The m2m flow starts to encode video and save to SD card
void snx_m2m_flow(void *arg);

//M2M + Cap 
void snx_m2m_flow(void *arg);

//M2M + Cap + Rate control
void snx_m2m_cap_rc_flow(void *arg);

//Data stamp
void snx_vc_data_stamp(int op, void *arg);
