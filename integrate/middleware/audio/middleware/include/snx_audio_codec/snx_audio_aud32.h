#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SNX_AUD32_TYPE_ENCODER	1	// type for encoder
#define SNX_AUD32_TYPE_DECODER	2	// type for decoder

enum SNX_AUD32_FORMAT {
	SNX_AUD32_FMT8_8KBPS,		// 8Ksps, 8Kbps
	SNX_AUD32_FMT8_8p8KBPS,		// 8Ksps, 8.8Kbps
	SNX_AUD32_FMT8_9p6KBPS,
	SNX_AUD32_FMT8_10p4KBPS,
	SNX_AUD32_FMT8_11p2KBPS,
	SNX_AUD32_FMT8_12KBPS,
	SNX_AUD32_FMT8_12p8KBPS,
	SNX_AUD32_FMT8_13p6KBPS,
	SNX_AUD32_FMT8_14p4KBPS,
	SNX_AUD32_FMT8_15p2KBPS,
	SNX_AUD32_FMT8_16KBPS		// 8Ksps, 16bps
};

struct SNX_AUD32_CONTEXT {
	int32_t type;			// encoder or decoder
	int32_t aud32_bytes_per_frame;	// bytes/frame in encoded data
	int32_t pcm_bytes_per_frame;	// bytes/frame in pcm S16LE data
	int32_t samples_per_frame;	// samples
	void *priv;			// codec meta-data
};

struct SNX_AUD32_CONTEXT *snx_aud32_open (int32_t type,
				enum SNX_AUD32_FORMAT format);
int32_t snx_aud32_close (struct SNX_AUD32_CONTEXT *aud32);
int32_t snx_aud32_encode (struct SNX_AUD32_CONTEXT *aud32, 
				int8_t *p_src, int8_t *p_dst, 
				int32_t src_bytes, int32_t *p_dst_bytes);

int32_t snx_aud32_decode (struct SNX_AUD32_CONTEXT *aud32, 
				int8_t *p_src, int8_t *p_dst, 
				int32_t src_bytes, int32_t *p_dst_bytes);

#ifdef __cplusplus
}
#endif

