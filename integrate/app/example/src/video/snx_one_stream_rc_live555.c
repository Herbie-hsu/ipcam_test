/**
 *
 * SONiX SDK Example Code
 * Category: Video Encode
 * File: snx_m2m_one_stream.c
 * Usage: 
 *		 1. The result video file would be saved in /tmp directory
 *		 2. Execute snx_m2m_one_stream (/usr/bin/snx_m2m_one_stream)
 *       3. The video encoding would start
 *       4. After the frame_num is reached, the encoding would stop
 *       5. Check the file recorded in the SD card by using VLC
 * NOTE:
 *       Recording all streams to SD card would cause the target framerate can
 *       not be reached because of the bandwidth leakage of SD card.
 */
#include "snx_video_codec.h"

#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*-----------------------------------------------------------------------------
 * Example Code Configuration
 *----------------------------------------------------------------------------*/

/*----------------- Stream 1 M2M stream --------------------------------------*/
#define FORMAT			V4L2_PIX_FMT_H264	// V4L2_PIX_FMT_H264 or V4L2_PIX_FMT_MJPEG or V4L2_PIX_FMT_SNX420
											// Video Encode Format

#define WIDTH			1280				// Video Encoded Frame Width
#define HEIGHT			720				// Video Encoded Frame Height
#define FRAME_RATE		15					// Video Encoded Frame Rate
#define IMG_DEV_NAME	"/dev/video0"		// ISP Device Node
#define CAP_DEV_NAME	"/dev/video1"		// Video Codec Device Node for M2M
#define SCALE			1					// 1: 1, 2: 1/2, 4: 1/4 scaling down
#define FILENAME		"/tmp/264_fifo"	// Video Encoded File Path
#define BIT_RATE		1024000				// Bitrate control for H264 ONLY


/*-----------------------------------------------------------------------------
 * GLOBAL Variables
 *----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Example Code Flow
 *----------------------------------------------------------------------------*/

static const char short_options[] = "hW:H:a:R:i:o:B:P:b:l:g:";

static const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"width", required_argument, NULL, 'W'},
    {"height", required_argument, NULL, 'H'},
    {"codec_fps", required_argument, NULL, 'R'},
    {"isp_fps", required_argument, NULL, 'i'},
    {"alone", required_argument, NULL, 'a'},
    {"output", required_argument, NULL, 'o'},
    {"buffer", required_argument, NULL, 'B'},
    {"pipe_size", required_argument, NULL, 'P'},
    {"bit_rate", required_argument, NULL, 'b'},
    {"packet_buffer_size", required_argument, NULL, 'l'},
    {"gop", required_argument, NULL, 'g'},
    {0, 0, 0, 0}
};


static void usage(FILE * fp, int argc, char ** argv) {   
    fprintf(fp, "Usage: %s [options]/n\n"   
        "Options:\n"
        "-h Print this message\n"
        "-W | --width        Frame Width\n"
        "-H | --height       Frame Height\n"
        "-a | --alone        Standalone daemon task exist\n"
        "-R | --codec_fps    		(optional) Codec Frame Rate (default=30)\n"
        "-i | --isp_fps      		(optional) ISP Frame Rate (default=30) \n"
        "-o | --output       		(optional) (default=/dev/video1) \n"
        "-B | --buffer       		(optional) V4L2 buffer (default=4)\n"
	"-P | --pipe_size    		(optional) Pipe size (default=102400, max=1048576)\n"
	"-b | --bite_rate    		(optional) Bit rate (default=1048576 bytes)\n"
	"-l | --packet_buffer_size	(optional) Live555 packet buffer size (default=250000)\n"
	"-g | --gop			(optional) Group of pictures (default=30)\n"
        "", argv[0]);
}

/*
	The entrance of this file.
	One M2M streams would be created.
	The configuration can set on the setting definitions above. 
	After the stream conf are done, the thread of the stream would be created.
	The bitstreams of the stream will be record on the SD card.
*/

int main(int argc, char **argv)
{
	int alone = 0;
	int scale = SCALE, buffer = 4;
	int width = WIDTH;
	int height = HEIGHT;
	int codec_fps =FRAME_RATE;
	int isp_fps =FRAME_RATE;
	char dev_name[1024] = CAP_DEV_NAME;
	int pipe_size = 102400;
	int bit_rate = BIT_RATE;
	int packet_buffer_size = 250000;
	int server_open;
	int gop = FRAME_RATE;
	int ret = 0;
	char live555_command[512];

	sigset_t signal_mask;

	stream_conf_t *stream1 = NULL;
	stream1 = malloc(sizeof(stream_conf_t));
	struct snx_m2m *m2m = &stream1->m2m;

	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	ret = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
	if (ret != 0) {
		printf("block sigpipe error\n");
	} 


	for (;;)
	{   
		int index;   
		int c;  
		c = getopt_long(argc, argv, short_options, long_options, &index);
		if (-1 == c)   
			break;   
		switch (c) {
			case 0: /* getopt_long() flag */   
				break;
			case 'h':   
				usage(stdout, argc, argv);   
				exit(EXIT_SUCCESS);   
			case 'W':
				sscanf(optarg, "%d", &width);
				break;
			case 'H':
				sscanf(optarg, "%d", &height);
				break;		
			case 'R':
				sscanf(optarg, "%d", &codec_fps);
				break;
			case 'i':
				sscanf(optarg, "%d", &isp_fps);
				break;
			case 's':
				sscanf(optarg, "%d", &scale);
				break;
			case 'a':
				sscanf(optarg, "%d", &alone);
				break;
			case 'o':
				strcpy(dev_name,optarg);
				break;
			case 'B':
				sscanf(optarg, "%d", &buffer);
				break;
			case 'P':
				sscanf(optarg, "%d", &pipe_size);
				break;
			case 'b':
				sscanf(optarg, "%d", &bit_rate);
				break;
			case 'l':
				sscanf(optarg, "%d", &packet_buffer_size);
				break;
			case 'g':
				sscanf(optarg, "%d", &gop);
				break;
			default:
				usage(stderr, argc, argv);   
				exit(EXIT_FAILURE);   
		}   
	}

	
	/*--------------------------------------------------------
		stream config setup
	---------------------------------------------------------*/
	m2m->isp_fps = isp_fps;
	m2m->m2m_buffers = buffer;
	m2m->ds_font_num = DS_BUFFER_SIZE;
	m2m->scale = scale;
	m2m->codec_fps = codec_fps;
	m2m->width = width;
	m2m->height = height;
	m2m->gop = gop;
	m2m->codec_fmt = FORMAT;
	m2m->out_mem = V4L2_MEMORY_USERPTR;
    
	if (alone == 0) 
	{
			m2m->m2m = 1;											/* M2M stream */
			m2m->isp_mem = V4L2_MEMORY_MMAP;
		    m2m->isp_fmt = V4L2_PIX_FMT_SNX420;
	}

	m2m->bit_rate = bit_rate;
    m2m->dyn_fps_en = 1;
	
	strcpy(m2m->isp_dev,IMG_DEV_NAME);
	strcpy(m2m->codec_dev,dev_name);
	
	umask(0);
	mknod(FILENAME, S_IFIFO|0666, 0);
	stream1->fd = open(FILENAME, O_RDWR | O_NONBLOCK);

	printf("[EXAMPLE] pipe size = %d\n",pipe_size);

	if ( fcntl(stream1->fd, 1024 + 7, pipe_size) == -1 )  //F_SETPIPE_SZ
		printf("adjust pipe size fail\n");


	stream1->live = 1;			//live streaming 
	stream1->frame_num = 1;  	//set a non-zero frame number for streaming

	/*--------------------------------------------------------
		Live 555 start 
	---------------------------------------------------------*/


	sprintf(live555_command, "./testOnDemandRTSPServer %d &", packet_buffer_size);

	server_open = system("netstat -tln | grep 554");
	if (server_open == 256) // means rtsp server not open
	{
		system(live555_command);
		printf("start live555 rtsp server\n");
	}
	else
	{
		printf("live555 rtsp server has already open\n");
		system("killall testOnDemandRTSPServer");
		printf("wait...\n");
		sleep(1);
		system(live555_command);
		printf("restart live555 rtsp server\n");
	}


	/*--------------------------------------------------------
		Start 
	---------------------------------------------------------*/
	ret = pthread_create(&stream1->stream_thread, NULL, (void *)snx_m2m_cap_rc_flow, stream1);
	if(ret != 0) {
		perror_exit(1, "exit thread creation failed");   
	}
	pthread_join(stream1->stream_thread,NULL);
	
	/*--------------------------------------------------------
		Record End 
	---------------------------------------------------------*/
	close(stream1->fd);
	/* Free the stream configs */
	free(stream1);
	
    return ret;
}
