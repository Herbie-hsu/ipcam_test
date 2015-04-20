#ifndef __SNX_RC_LIB_H__
#define __SNX_RC_LIB_H__

#ifdef __cplusplus
extern "C"{
#endif

#define SNX_RC_INIT	0
#define SNX_RC_REINIT	1

struct snx_rc
{
	int codec_fd;
	size_t width;
	size_t height;
	unsigned int gop;
	int frames_gop;
	int Targetbitrate;	
	int rtn_quant;
	int Previous_frame_size;
	int BitRemain;
	int IFrameAve;
	int IntraPreDeviation;
	int FirstPQP;
	int IntraQP;
	int FrameCntRemain;
	int framerate;
	int total_quant_gop;
	int QpAdjEnable;
	int QpAdjStopCnt;
	int QpDifSum;
	int NFirstGOP;
	int IFrameCnt;
	int CurSecHaveIFrame;
	int TotalBitPerSec;
	int frames;
	int current_frame_size;

//initial value
	int UpBoundUp;
	int UpBoundDw;
	int ConvergenceRate;
	int QPStopCnt;
	int SumQPBound;
	int UpBoundBytesPerSec;
	int BitExceed;
	int RealBytesPerSec;
	int SumQPCnt;
	int I_frame_size[4];
	int bufsize;
	
// Ryan add 2014/04/07
	int Pre_Framerate;
	int reinit;
	int md_flag;
};

//extern int snx_codec_rc_init(struct snx_rc *rc);
extern int snx_codec_rc_init(struct snx_rc *rc, int reinit);
extern int snx_codec_rc_update(int isp_fps, int flags, int current_frame_size, struct snx_rc *rc);
extern int snx_codec_rc_reinit(int isp_fps, int flags, struct snx_rc *rc);
extern int snx_420line_to_420(char *p_in, char *p_out, unsigned int width, unsigned int height);

extern int snx_md_corner (int idx, unsigned int m, unsigned int n, int count);
extern int snx_isp_denoise(int level);
extern int snx_md_drop_fps (struct snx_rc *rate_ctl, int *force_i_frame);
extern int snx_get_file_value(char *path, int *value, int base);

#ifdef __cplusplus
}
#endif

#endif //__SNX_RC_LIB_H__
