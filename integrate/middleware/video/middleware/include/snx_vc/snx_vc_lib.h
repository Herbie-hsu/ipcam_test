#ifndef __SNX_VC_LIB_H__
#define __SNX_VC_LIB_H__

#ifdef __cplusplus
extern "C"{
#endif

// Unit: 16 pixels.
typedef	struct snx_cds_position
{
	unsigned int	start_x;
	unsigned int	start_y;
}POSITION;

// 
// Font	dimension.
// For picture:	
// dim_x = pic_width /16;
// dim_y = pic_height /	16;

typedef	struct snx_cds_dimension
{
	unsigned int	dim_x;
	unsigned int	dim_y;
}DIMENSION;

//
// [txty   txtcb   txtcr   bgy	 bgcb	bgcr]	
// Foreground Y, Cb, Cr. Background Y, Cb, Cr 
//
typedef	struct snx_cds_color
{
	unsigned int	color_Y;
	unsigned int	color_Cb;
	unsigned int	color_Cr;	
}COLOR;

//
// Color attr: [weighting mode]	 
// Weighting between background	and foreground.	
// Mode	is used	to select foreground or	background color.
//
// (1) if mode is 0 or 1, weight will be used,	
// and 0 stands	for background is transparent,	
// and 1 stands	for background is not transparent.
// (2) if mode is 2 or 3, weight will not be used,
// and 2 stands	for background is transparent,
// and 3 stands	for backgound is not transparent.
//

typedef	struct snx_cds_color_attr
{
	int	weight;			    // range: 0~7 //
	int	mode;			   // range: 0~3 //
}COLOR_ATTR;

/// 
// scaling_up_ratio  0:	1x1;   1: 2x2;	 2: 4x4	
///
typedef	enum snx_cds_scaling_ratio
{
	SCALING_1X1 = 0,
	SCALING_2X2,
	SCALING_4X4
}SCALING_UP_RATIO;

struct snx_cds
{
	char dev_name[64];
	char bmp_file[64];
	char *string;
	struct snx_cds_position pos;
	struct snx_cds_dimension dim;
	struct snx_cds_color t_color;
	struct snx_cds_color b_color;
	struct snx_cds_color bmp_threshold;

	struct snx_cds_color_attr attr;
	unsigned int enable;
	unsigned int scale;

};

struct buffer
{
    void *start;
    size_t length;
};

struct snx_m2m
{
	char isp_dev[12];
	char codec_dev[12];
	char ds_dev_name[64];

	int isp_fd;
	int codec_fd;
	unsigned int last;

	// Sonix Set
	unsigned int m2m;

	int socket_fd;
	int thread_num;
	unsigned int scale;
	unsigned int ds_font_num;

	int dyn_fps_en;
	struct timeval timestamp;


	// Basic Set
	size_t width;
	size_t height;
	int isp_fps;
	int codec_fps;
    
	int bit_rate;
	int qp;
	int gop;
	int m2m_buffers;
	unsigned int flags;
	int force_i_frame;

	// V4L2 Set
	struct buffer *isp_buffers;
	struct buffer *cap_buffers;

	int isp_index;
	int cap_index;
	int cap_bytesused;
	unsigned int isp_fmt;
	unsigned int codec_fmt;

	unsigned int cap_mem;
	unsigned int out_mem;  
	unsigned int isp_mem;


};


int snx_open_device(char *dev_name);

int snx_isp_init(struct snx_m2m *m2m);
int snx_isp_start(struct snx_m2m *m2m);
int snx_isp_read(struct snx_m2m *m2m);
int snx_isp_reset(struct snx_m2m *m2m);

int snx_isp_stop(struct snx_m2m *m2m);
int snx_isp_uninit(struct snx_m2m *m2m);

int snx_codec_init(struct snx_m2m *m2m);
int snx_codec_start(struct snx_m2m *m2m);
int snx_codec_read(struct snx_m2m *m2m);
int snx_codec_reset(struct snx_m2m *m2m);
int snx_codec_stop(struct snx_m2m *m2m);
int snx_codec_uninit(struct snx_m2m *m2m);
int snx_codec_set_qp(struct snx_m2m *m2m, unsigned int type);
int snx_codec_set_gop(struct snx_m2m *m2m);
int snx_codec_dyn_fps(struct snx_m2m *m2m);


//int snx_cds_set_bmp(char *dev_name, char *bmp_file, SCALING_UP_RATIO scaling_up_ratio);
int snx_cds_set_bmp(struct snx_cds *cds);

int snx_cds_set_string(char *dev_name, char *data, SCALING_UP_RATIO *scaling_up_ratio);
int snx_cds_get_datastamp(char *dev_name, char *data, int len);
int snx_cds_set_datastamp(char *dev_name, char *data, int len);
int snx_cds_get_position(char *dev_name, POSITION *position, DIMENSION *dimension);
int snx_cds_set_position(char *dev_name, POSITION *position, DIMENSION *dimension);
int snx_cds_get_color(char *dev_name, COLOR *forg_color, COLOR *bkg_color);
int snx_cds_set_color(char *dev_name, COLOR *forg_color, COLOR *bkg_color);
int snx_cds_get_color_attr(char *dev_name, COLOR_ATTR *color_attr);
int snx_cds_set_color_attr(char *dev_name, COLOR_ATTR *color_attr);
int snx_cds_get_scale(char *dev_name, SCALING_UP_RATIO *scaling_up_ratio);
int snx_cds_set_scale(char *dev_name, SCALING_UP_RATIO scaling_up_ratio);
int snx_cds_get_enable(char *dev_name, int *enable);
int snx_cds_set_enable(char *dev_name, int enable);

int snx_cds_set_all(char *dev_name, struct snx_cds *cds);
int snx_scale_down_bmppicture (char *in_bmp_file, char* out_bmp_file);
int snx_scale_up_bmppicture (char *in_bmp_file,char *out_bmp_file);

#ifdef __cplusplus
}
#endif

#endif //__SNX_VC_LIB_H__
