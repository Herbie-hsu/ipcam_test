/**
 *
 * SONiX SDK Example Code
 * Category: Video Encode
 * File: snx_m2m_one_stream.c
 * Usage: 
 *		 1. Make sure at least a stream has been run (./snx_m2m_one_stream)
 *       2. Input the correct dev name of the stream (refer to the usage )
 *       3. Input the rest of arguments you want to setup (refer to the usage)
 * NOTE:
 *       Recording all streams to SD card would cause the target framerate can
 *       not be reached because of the bandwidth leakage of SD card.
 */
#include <stdint.h>
#include "snx_video_codec.h"

/*-----------------------------------------------------------------------------
 * Example Code Configuration
 *----------------------------------------------------------------------------*/

/*----------------- Data Stamp config   --------------------------------------*/
#define DS_ENABLE		1					// Data Stamp Enable
#define DS_TEXT_COLOR	DS_BLUE				// Data Stamp Text Color
#define DS_BG_COLOR		DS_YELLOW			// Data Stamp Background Color
#define DS_MODE			2					// Data stamp mode [0~3]
											// 0: text(wighting) + background(transparent)
											// 1: text(wighting) + background
											// 2: text           + background(transparent)
											// 3: text           + background
											
#define DS_WEIGHT		0					// TEXT transparent weight [1~7] Transparent ~ Solid
#define DS_SCALE		0					// [0~2] 0: 1x1 1: 2x2 2: 4x4 Data Stamp Scale 
#define DS_STARTX		0					// Start posision of Data stamp
#define DS_STARTY		0					// Start posision of Data stamp
#define DS_STRING		"Hello World"		// Data Stamp String Data


/*-----------------------------------------------------------------------------
 * GLOBAL Variables
 *----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Example Code Flow
 *----------------------------------------------------------------------------*/


/*
	The entrance of this file.
	One M2M streams would be created.
	The configuration can set on the setting definitions above. 
	After the stream conf are done, the thread of the stream would be created.
	The bitstreams of the stream will be record on the SD card.
*/

static const char short_options[] = "hd:t:b:m:w:i:s:x:y:l:v:e:p";

static const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"device", required_argument, NULL, 'd'},
    {"text", required_argument, NULL, 't'},
    {"background", required_argument, NULL, 'b'},
    {"mode", required_argument, NULL, 'm'},
    {"weighting", required_argument, NULL, 'w'},
    {"string", required_argument, NULL, 'i'},
    {"scale", required_argument, NULL, 's'},
    {"start_x", required_argument, NULL, 'x'},
    {"start_y", required_argument, NULL, 'y'},
    {"dim_h", required_argument, NULL, 'l'},
    {"dim_v", required_argument, NULL, 'v'},
    {"enable", required_argument, NULL, 'e'},
	{"bitmap", no_argument, NULL, 'p'},

    {0, 0, 0, 0}
};


static void usage(FILE * fp, int argc, char ** argv) {   
    fprintf(fp, "Usage: %s [options]/n\n"   
        "Options:\n"
        "-h Print this message\n"
        "-d | --device	     [1_xs] select on /proc/codec/* \n"
        "		       [1] means /dev/video1\n"
        "		       [2] means /dev/video2\n"
        "		       [x] can have two options\n"
        "		           [h] for H264\n"
        "		           [j] for MJPEG\n"
		"			   [s] scaling stream\n"
        "-t | --text	     text color,       R:Red    G:Green   B:Blue\n"
		"-b | --background   background color, Y:Yellow M:Magenta C:Cyan\n"
		"				       W:White  K:Black\n"
		"-m | --mode         0: text(wighting) + background(transparent)\n"
		"                    1: text(wighting) + background\n"
		"                    2: text           + background(transparent)\n"
		"                    3: text           + background\n"
		"-w | --weighting    OUT=(DS_WT*TEXT+(8-DS_WT)*IMG)/8 \n"
		"-i | --string       input string\n"
		"-s | --scale        scale down 0: 1x1 1: 2x2 2: 4x4\n"

		"-x | --start_x      postiton unit is 16 pixel\n"
		"-y | --start_y      postiton unit is 16 pixel\n"
		"-l | --dim_h        font dimension horizontal\n"
		"-v | --dim_v	     font dimension vertical\n"
		"-e | --enable	     Codec Data Stamp enable\n"

	"", argv[0]);   
}

int main(int argc, char **argv)
{
	int ret = 0;
	struct snx_cds *cds = NULL;
	cds = malloc(sizeof(struct snx_cds));
	struct snx_cds_color *cds_color;
	int bitmap_flag = 0;
	
	/*--------------------------------------------------------
		Data Stamp Setup (Default)
	---------------------------------------------------------*/
	memset(cds ,0x0, sizeof(struct snx_cds));

	/*--------------------------------------------------------
		Option Value
	---------------------------------------------------------*/
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
			case 'd':
				if (strlen(optarg) != 0) {
					sprintf(cds->dev_name, "/proc/codec/%s", optarg);
					snx_vc_data_stamp(DS_GET_ALL, cds);
				}
				break;
			case 'e':
				sscanf(optarg, "%d", &cds->enable);
				snx_vc_data_stamp(DS_SET_EN, cds);
				break;
			case 't':
			case 'b':
				if( c == 't')
					cds_color = &cds->t_color;
				else
					cds_color = &cds->b_color;
				
				switch(*optarg) {
					case 'R': 
						sscanf(DS_RED, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'G': 
						sscanf(DS_GREEN, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'B': 
						sscanf(DS_BLUE, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'Y': 
						sscanf(DS_YELLOW, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'M': 
						sscanf(DS_MAGENTA, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'C': 
						sscanf(DS_CYAN, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'K': 
						sscanf(DS_BLACK, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					case 'W': 
						sscanf(DS_WHITE, "%u %u %u", &cds_color->color_Y, &cds_color->color_Cb, &cds_color->color_Cr);
						break;
					default: 
						usage(stderr, argc, argv);   
						exit(EXIT_FAILURE);   
				}
				break;
			case 'm':
				sscanf(optarg, "%d", &cds->attr.mode);
				break;
			case 'w':
				sscanf(optarg, "%d", &cds->attr.weight);
				break;
			case 's':
				sscanf(optarg, "%d", &cds->scale);
				snx_vc_data_stamp(DS_SET_SCALE, cds);
				break;
			case 'i':
				cds->string = malloc(strlen(optarg)+1);
				strcpy(cds->string, optarg);
                printf("[CDS] get string: %s\n", cds->string);
				break;
			case 'x':
				sscanf(optarg, "%d", &cds->pos.start_x);
				break;
			case 'y':
				sscanf(optarg, "%d", &cds->pos.start_y);
				break;
			case 'l':
				sscanf(optarg, "%d", &cds->dim.dim_x);
				break;
			case 'v':
				sscanf(optarg, "%d", &cds->dim.dim_y);
				break;
			case 'p':
				bitmap_flag = 1;
				break;
			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}
	if(cds->string) {
		if(bitmap_flag) {
			snx_vc_data_stamp(DS_SET_BMP, cds);
		} else
			snx_vc_data_stamp(DS_SET_STRING, cds);
	}
	/* By default, we use the length * 1 */
	if ((cds->string) && ((cds->dim.dim_x == 0) || (cds->dim.dim_y == 0))) {
		cds->dim.dim_x = strlen(cds->string);
		cds->dim.dim_y = 1;
	}
	
    if (cds->attr.weight == 0 )
        cds->attr.weight = 1; //default weight value

	snx_vc_data_stamp(DS_SET_POS, cds);
	snx_vc_data_stamp(DS_SET_COLOR_ATTR, cds);
	snx_vc_data_stamp(DS_SET_COLOR, cds);

	/*--------------------------------------------------------
		Record End 
	---------------------------------------------------------*/
	/* Free the stream configs */
	if(cds->string)
		free(cds->string);
	free(cds);
	
    return ret;
}
