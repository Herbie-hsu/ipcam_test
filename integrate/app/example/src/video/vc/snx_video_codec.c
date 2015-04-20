/**
 *
 * SONiX SDK Example Code
 * Category: Video Encode
 * File: video_codec.c
 *
 * NOTE:
 *       
 */
 
#include "snx_video_codec.h"

/*
	Before capture stream starts, make sure one m2m stream is started.
	The capture stream is related to the m2m stream, including 
	frame rate 
	scaling down (1 1/2 1/4)
	format (H264, MJPEG, RAW)
*/
void snx_cap_flow(void *arg)
{
	long long unsigned int data_size = 0;					//Count the total datasize
	stream_conf_t *stream = (stream_conf_t *)arg;
	struct snx_m2m *m2m = &stream->m2m;
	int fd = stream->fd;
	
	struct timeval tv;
	unsigned long long start_t =0, end_t =0, period=0, pre_period=0;
	int count_num = 0;
	int ret;
	
	if (m2m->m2m) {
		printf("[Example] m2m = 1, should run m2m thread!\n");
		pthread_exit(NULL); 
	}

	if (stream->mutex) {
		/* Wait for the m2m stream has been start */
		pthread_mutex_lock(stream->mutex);
		(*stream->ref)++;
		pthread_cond_wait(stream->cond, stream->mutex);
		pthread_mutex_unlock(stream->mutex);
	}

	printf("============snx_cap_flow============\n");

	/* 1. Open Video Encode device */
	m2m->codec_fd = snx_open_device(m2m->codec_dev);	

	/* 2. Initialize Video Encode */
	ret = snx_codec_init(m2m);
	if(ret != 0) goto err_init;

	/* 3. Set Codec GOP */
	snx_codec_set_gop(m2m);
	
	/* 4. Start Video Encode */
	ret = snx_codec_start(m2m);
	if(ret != 0) goto err_start;

	/* Start M2M Video Fetech and Record */
	gettimeofday(&tv,NULL);
	start_t = tv.tv_sec * 1000000 + tv.tv_usec;
	
	while(1) {
		/* 5. Read from Video Codec */
		ret = snx_codec_read(m2m);
		/* 6. Check if any frame encodec */
		if(m2m->cap_bytesused != 0) {
			count_num ++;
			gettimeofday(&tv,NULL);
			end_t = tv.tv_sec * 1000000 + tv.tv_usec;
			period = end_t - start_t;
			
			data_size += m2m->cap_bytesused;
			
			if (fps_debug != 0) {
				if (period - pre_period >= 1000000) {
					pre_period = period;
					
					printf("snx_record %d x %d Real fps = %lld,(real_frames= %d)\n"
					,(m2m->width/m2m->scale)
					,(m2m->height/m2m->scale) 
					,(((long long unsigned int)count_num * 1000000) / period)
					,count_num);
				}
			}
			
			/* Handle the encoded frame */
			if(stream->frame_num > 0) {
				if (fd) {
					/* Save the encoded frame to SD card */
					//fwrite(m2m->cap_buffers[m2m->cap_index].start,  m2m->cap_bytesused, 1, fd);
						int has_written = 0;
						int leng;
						while(1){
							leng = write(fd, m2m->cap_buffers[m2m->cap_index].start + has_written, m2m->cap_bytesused - has_written);
							if(leng <= 0);
							else if(leng <  m2m->cap_bytesused - has_written){
								has_written += leng;
							}else  if(leng ==  m2m->cap_bytesused - has_written){
								break;
							}else{
								printf("Write error");
								break;
							}
						}
				}

				if(!stream->live)
					stream->frame_num--;	//recording 
			}  
			
			
		} //if(m2m->cap_bytesused != 0)
		
		/* 8. Reset Codec for the next frame */
		ret = snx_codec_reset(m2m);
		
		/* Save the frames until the frame number has been reached */
	    if(stream->frame_num == 0) {
			if (stream->mutex) {
				pthread_mutex_lock(stream->mutex);
				/* reference - 1 when this capture stream is done */
				(*stream->ref)--;
				pthread_mutex_unlock(stream->mutex);
			}
			break;
		}
	}

	/* To finish the m2m stream properly, call the apis in order */
err_start:
	/* 9. Stop Video codec */
	snx_codec_stop(m2m);
err_init:
	/* 10. un-initialize Video codec */
	snx_codec_uninit(m2m);
	/* 11. Close video encode device */
	close(m2m->codec_fd);
	
	printf("============snx_cap_flow============End\n");

}

/*
	The m2m flow starts to encode video and save to SD card
*/
void snx_m2m_flow(void *arg)
{
	long long unsigned int data_size = 0;					//Count the total datasize
	stream_conf_t *stream = (stream_conf_t *)arg;		
	struct snx_m2m *m2m = &stream->m2m;
	int fd = stream->fd;
	
	struct timeval tv;					//for time calculation
	unsigned long long start_t =0, end_t =0, period=0, pre_period=0;
	
	int count_num = 0;					//Encoded frame count
	int ret;

	if (!m2m->m2m) {
		printf("[Example] m2m = 0, should run cap thread!\n");
		pthread_exit(NULL); 
	}

	printf("============snx_m2m_flow============\n");

	if(m2m->m2m) {
		/* 1. Open ISP device */
		m2m->isp_fd = snx_open_device(m2m->isp_dev);
		
		/* 2. Initialize ISP */
		ret = snx_isp_init(m2m);
		if(ret != 0) goto err_init;

		/* 3. Start ISP */
		snx_isp_start(m2m);
	}

	/* 4. Open Video Encode Device */
	m2m->codec_fd = snx_open_device(m2m->codec_dev);	

	/* 5. Initialize Video Encode */
	ret = snx_codec_init(m2m);
	if(ret != 0) goto err_init;

	/* 6. Set Codec GOP */
	snx_codec_set_gop(m2m);
	
	/* 7. Start Video Encode */
	ret = snx_codec_start(m2m);
	if(ret != 0) goto err_start;
	
	/* Boardcast cond to let all capture streams start */
	if (stream->mutex) {
		pthread_mutex_lock(stream->mutex);
		pthread_cond_broadcast(stream->cond);
		pthread_mutex_unlock(stream->mutex);
	}
	
	/* Start M2M Video Fetech and Record */
	gettimeofday(&tv,NULL);
	start_t = tv.tv_sec * 1000000 + tv.tv_usec;
	
	while(1) {
		/* 8. Read from Video Codec */
		ret = snx_codec_read(m2m);
		/* 9. Check if any frame encodec */
		if(m2m->cap_bytesused != 0) {
			count_num ++;
			gettimeofday(&tv,NULL);
			end_t = tv.tv_sec * 1000000 + tv.tv_usec;
			period = end_t - start_t;
			
			data_size += m2m->cap_bytesused;
			
			if (fps_debug != 0) {
				if (period - pre_period >= 1000000) {
					pre_period = period;
					
					printf("snx_record %d x %d Real fps = %lld,(real_frames= %d)\n"
					,(m2m->width/m2m->scale)
					,(m2m->height/m2m->scale) 
					,(((long long unsigned int)count_num * 1000000) / period)
					,count_num);
				}
			}
			
			/* Handle the encoded frame */
			if(stream->frame_num > 0) {
				if (fd) {
					/* Save the encoded frame to SD card */
					//fwrite(m2m->cap_buffers[m2m->cap_index].start,  m2m->cap_bytesused, 1, fd);
						int has_written = 0;
						int leng;
						while(1){
							leng = write(fd, m2m->cap_buffers[m2m->cap_index].start + has_written, m2m->cap_bytesused - has_written);
							if(leng <= 0);
							else if(leng <  m2m->cap_bytesused - has_written){
								has_written += leng;
							}else  if(leng ==  m2m->cap_bytesused - has_written){
								break;
							}else{
								printf("Write error");
								break;
							}
						}
				}

				if(!stream->live)
					stream->frame_num--;	//recording 
			}  
	    	
		} //if(m2m->cap_bytesused != 0)
		
		/* 11. Reset Codec for the next frame */
		ret = snx_codec_reset(m2m);
		
		/* Save the frames until the frame number has been reached */
	    if(stream->frame_num == 0) {
			if (stream->mutex) {
				pthread_mutex_lock(stream->mutex);
				if(*stream->ref == 0) {
					pthread_mutex_unlock(stream->mutex);
					break;
				}
				pthread_mutex_unlock(stream->mutex);
			} else {
				break;
			}
		}
		
	}
	
	/* To finish the m2m stream properly, call the apis in order */
err_start:
	/* 12. Stop Video codec */
	snx_codec_stop(m2m);
err_init:
	/* 13. un-initialize Video codec */
	snx_codec_uninit(m2m);
	if(m2m->m2m) {
		/* 14. Stop ISP */
		snx_isp_stop(m2m);
		/* 15. Un-initialize ISP */
		snx_isp_uninit(m2m);
	}
	/* 16. Close video encode device */
	close(m2m->codec_fd);
	if(m2m->m2m) {
		/* 17. Close ISP device */
		close(m2m->isp_fd);
	}

	printf("============snx_m2m_flow============End\n");	
}

/*
	M2M + Cap : Integrate M2M and Capture stream flow 
*/
void snx_m2m_cap_flow(void *arg)
{
	long long unsigned int data_size = 0;					//Count the total datasize
	stream_conf_t *stream = (stream_conf_t *)arg;		
	struct snx_m2m *m2m = &stream->m2m;
	int fd = stream->fd;
	
	struct timeval tv;					//for time calculation
	unsigned long long start_t =0, end_t =0, period=0, pre_period=0;
	
	int count_num = 0;					//Encoded frame count
	int ret;

	if (m2m->m2m == 0) {
		if (stream->mutex) {
			/* Wait for the m2m stream has been start */
			pthread_mutex_lock(stream->mutex);
			(*stream->ref)++;
			pthread_cond_wait(stream->cond, stream->mutex);
			pthread_mutex_unlock(stream->mutex);
		}
	}

	printf("============snx_m2m_cap_flow============\n");

	if(m2m->m2m) {
		/* Open ISP device */
		m2m->isp_fd = snx_open_device(m2m->isp_dev);
		
		/* Initialize ISP */
		ret = snx_isp_init(m2m);
		if(ret != 0) goto err_init;

		/* Start ISP */
		snx_isp_start(m2m);
	}

	/* Open Video Encode Device */
	m2m->codec_fd = snx_open_device(m2m->codec_dev);	

	/* Initialize Video Encode */
	ret = snx_codec_init(m2m);
	if(ret != 0) goto err_init;

	/* Set Codec GOP */
	snx_codec_set_gop(m2m);
	
	/* Start Video Encode */
	ret = snx_codec_start(m2m);
	if(ret != 0) goto err_start;
	
	/* Boardcast cond to let all capture streams start */
	if (m2m->m2m) {
		if (stream->mutex) {
			pthread_mutex_lock(stream->mutex);
			pthread_cond_broadcast(stream->cond);
			pthread_mutex_unlock(stream->mutex);
		}
	}
	
	/* Start M2M Video Fetech and Record */
	gettimeofday(&tv,NULL);
	start_t = tv.tv_sec * 1000000 + tv.tv_usec;
	
	while(1) {
		/* Read from Video Codec */
		ret = snx_codec_read(m2m);
		/* Check if any frame encodec */
		if(m2m->cap_bytesused != 0) {
			count_num ++;
			gettimeofday(&tv,NULL);
			end_t = tv.tv_sec * 1000000 + tv.tv_usec;
			period = end_t - start_t;
			
			data_size += m2m->cap_bytesused;
			
			if (fps_debug != 0) {
				if (period - pre_period >= 1000000) {
					pre_period = period;
					
					printf("snx_record %d x %d Real fps = %lld,(real_frames= %d)\n"
					,(m2m->width/m2m->scale)
					,(m2m->height/m2m->scale) 
					,(((long long unsigned int)count_num * 1000000) / period)
					,count_num);
				}
			}
			
			/* Handle the encoded frame */
			if(stream->frame_num > 0) {
				if (fd) {
					/* Save the encoded frame to SD card */
					//fwrite(m2m->cap_buffers[m2m->cap_index].start,  m2m->cap_bytesused, 1, fd);
						int has_written = 0;
						int leng;
						while(1){
							leng = write(fd, m2m->cap_buffers[m2m->cap_index].start + has_written, m2m->cap_bytesused - has_written);
							if(leng <= 0);
							else if(leng <  m2m->cap_bytesused - has_written){
								has_written += leng;
							}else  if(leng ==  m2m->cap_bytesused - has_written){
								break;
							}else{
								printf("Write error");
								break;
							}
						}
				}

				if(!stream->live)
					stream->frame_num--;	//recording 
			}  
	    	
		} //if(m2m->cap_bytesused != 0)
		
		/* Reset Codec for the next frame */
		ret = snx_codec_reset(m2m);
		
		/* Save the frames until the frame number has been reached */
	    if(stream->frame_num == 0) {
			if (stream->mutex) {
				if (m2m->m2m) {		//M2M stream
				
					pthread_mutex_lock(stream->mutex);
					if(*stream->ref == 0) {
						pthread_mutex_unlock(stream->mutex);
						break;
					}
					pthread_mutex_unlock(stream->mutex);
					
				} else {
					
					pthread_mutex_lock(stream->mutex);
					/* reference - 1 when this capture stream is done */
					(*stream->ref)--;
					pthread_mutex_unlock(stream->mutex);
					break;
				}
			} else {
				break;
			}
		}
		
	}
	
	/* To finish the m2m stream properly, call the apis in order */
err_start:
	/* Stop Video codec */
	snx_codec_stop(m2m);
err_init:
	/* un-initialize Video codec */
	snx_codec_uninit(m2m);
	if(m2m->m2m) {
		/* Stop ISP */
		snx_isp_stop(m2m);
		/* Un-initialize ISP */
		snx_isp_uninit(m2m);
	}
	/* Close video encode device */
	close(m2m->codec_fd);
	if(m2m->m2m) {
		/* Close ISP device */
		close(m2m->isp_fd);
	}
	printf("============snx_m2m_cap_flow============End\n");	
}

/*
	M2M + Cap + Rc : Integrate M2M and Capture stream flow with Rate Control
*/
void snx_m2m_cap_rc_flow(void *arg)
{
	long long unsigned int data_size = 0;					//Count the total datasize
	stream_conf_t *stream = (stream_conf_t *)arg;		
	struct snx_m2m *m2m = &stream->m2m;
	int fd = stream->fd;
	struct snx_rc *rc = NULL;			//rate control use
	
	struct timeval tv;					//for time calculation
	unsigned long long start_t =0, end_t =0, period=0, pre_period=0;
	
	int count_num = 0;					//Encoded frame count
	int real_fps = 0;					//real_fps frame count
	int ret;

	if (m2m->m2m == 0) {
		if (stream->mutex) {
			/* Wait for the m2m stream has been start */
			pthread_mutex_lock(stream->mutex);
			(*stream->ref)++;
			pthread_cond_wait(stream->cond, stream->mutex);
			pthread_mutex_unlock(stream->mutex);
		}
	}

	printf("============snx_m2m_cap_rc_flow============\n");

	if(m2m->m2m) {
		/* Open ISP device */
		m2m->isp_fd = snx_open_device(m2m->isp_dev);
		
		/* Initialize ISP */
		ret = snx_isp_init(m2m);
		if(ret != 0) goto err_init;

		/* Start ISP */
		snx_isp_start(m2m);
	}

	/* Open Video Encode Device */
	m2m->codec_fd = snx_open_device(m2m->codec_dev);	

	/* Initialize Video Encode */
	ret = snx_codec_init(m2m);
	if(ret != 0) goto err_init;

	/* Set Codec GOP */
	snx_codec_set_gop(m2m);

	/* Bitrate Rate Control is only support for H264 */
	if((m2m->bit_rate) && (m2m->codec_fmt == V4L2_PIX_FMT_H264)) {
		
		rc = malloc(sizeof(struct snx_rc));
		
		/* Initialize rate control arguments */
		rc->width = m2m->width;				//Bit-rate control width
		rc->height = m2m->height;			//Bit rate control height
		rc->codec_fd = m2m->codec_fd;		//point to the codec fd
		rc->Targetbitrate = m2m->bit_rate;  //rate control target bitrate
		rc->framerate = m2m->codec_fps;		//point to the codec frame rate
		rc->gop = m2m->gop;					//codec gop
		//rc->reinit = 1;
		/*Initialize rate control */
//		m2m->qp = snx_codec_rc_init(rc);
		m2m->qp = snx_codec_rc_init(rc, SNX_RC_INIT);
	}
	
	/* Start Video Encode */
	ret = snx_codec_start(m2m);
	if(ret != 0) goto err_start;
	
	/* Data Stamp */
	if(strlen(stream->cds.dev_name))
		snx_vc_data_stamp(DS_SET_ALL, &stream->cds);
	
	/* Boardcast cond to let all capture streams start */
	if (m2m->m2m) {
		if (stream->mutex) {
			pthread_mutex_lock(stream->mutex);
			pthread_cond_broadcast(stream->cond);
			pthread_mutex_unlock(stream->mutex);
		}
	}
	
	/* Start M2M Video Fetech and Record */
	gettimeofday(&tv,NULL);
	start_t = tv.tv_sec * 1000000 + tv.tv_usec;
	
	while(1) {
		/* Read from Video Codec */
		ret = snx_codec_read(m2m);
		/* Check if any frame encodec */
		if(m2m->cap_bytesused != 0) {
			count_num ++;
			real_fps ++;
			gettimeofday(&tv,NULL);
			end_t = tv.tv_sec * 1000000 + tv.tv_usec;
			period = end_t - start_t;
			
			data_size += m2m->cap_bytesused;
			
			if (fps_debug != 0) {
				if (period - pre_period >= 1000000) {
					pre_period = period;
					
					printf("snx_record %d x %d Real fps = %d,(real_frames= %d)\n"
					,(m2m->width/m2m->scale)
					,(m2m->height/m2m->scale) 
					, real_fps
//					,(((long long unsigned int)real_fps * 1000000) / period)
					,count_num);
					
					printf("Datasize %lld bytes,BitRate %d Kbps  QP == %d\n"
					, data_size
//					, (((data_size<<3) * 1000 ) / period )
					, (data_size>>7) 
					, m2m->qp);
					real_fps = 0;
					data_size = 0;
				}
			}
			
			/* 
				Bit Rate Control Flow 
				Update the QP of the next frame to keep the bitrate. (CBR).
			*/
			if((m2m->bit_rate) && (m2m->codec_fmt == V4L2_PIX_FMT_H264)) {
//				m2m->qp = snx_codec_rc_update(m2m->cap_bytesused, rc);
				m2m->qp = snx_codec_rc_update(m2m->isp_fps, m2m->flags , m2m->cap_bytesused, rc);
			        snx_md_drop_fps(rc, &m2m->force_i_frame);

				/*
				if (fps_debug) {
					printf("[RC] frames %d frames_gop = %d QP== %d\n"	
						, rc->frames
						, rc->frames_gop
						,m2m->qp);
				}
				*/
			}
			
			/* Handle the encoded frame */
			if(stream->frame_num > 0) {
				if (fd) {
					/* Save the encoded frame to SD card */
					//fwrite(m2m->cap_buffers[m2m->cap_index].start,  m2m->cap_bytesused, 1, fd);
						int has_written = 0;
						int leng;
						while(1){
							leng = write(fd, m2m->cap_buffers[m2m->cap_index].start + has_written, m2m->cap_bytesused - has_written);
							if(leng <= 0);
							else if(leng <  m2m->cap_bytesused - has_written){
								has_written += leng;
							}else  if(leng ==  m2m->cap_bytesused - has_written){
								break;
							}else{
								printf("Write error");
								break;
							}
						}
				}

				if(!stream->live)
					stream->frame_num--;	//recording 
			}  
	    	
		} //if(m2m->cap_bytesused != 0)

		/* Reset Codec for the next frame */
		ret = snx_codec_reset(m2m);
		
		/* Save the frames until the frame number has been reached */
	    if(stream->frame_num == 0) {
			if (stream->mutex) {
				if (m2m->m2m) {		//M2M stream
				
					pthread_mutex_lock(stream->mutex);
					if(*stream->ref == 0) {
						pthread_mutex_unlock(stream->mutex);
						break;
					}
					pthread_mutex_unlock(stream->mutex);
					
				} else {			//Capture Stream
					
					pthread_mutex_lock(stream->mutex);
					/* reference - 1 when this capture stream is done */
					(*stream->ref)--;
					pthread_mutex_unlock(stream->mutex);
					break;
				}
			} else {
				break;
			}
		}
		
	}
	
	/* To finish the m2m stream properly, call the apis in order */
err_start:
	/* Stop Video codec */
	snx_codec_stop(m2m);
err_init:
	/* un-initialize Video codec */
	snx_codec_uninit(m2m);
	if(m2m->m2m) {
		/* Stop ISP */
		snx_isp_stop(m2m);
		/* Un-initialize ISP */
		snx_isp_uninit(m2m);
	}
	/* Close video encode device */
	close(m2m->codec_fd);
	if(m2m->m2m) {
		/* Close ISP device */
		close(m2m->isp_fd);
	}
	printf("============snx_m2m_cap_rc_flow============End\n");	
}


void snx_vc_data_stamp(int op, void *arg)
{

	struct snx_cds *cds = (struct snx_cds*)arg;
	if(strlen(cds->dev_name)==0) {
		printf("NO Devname for Data stamp\n");
		return;
	}
	
	switch (op) 
	{
		case DS_SET_ALL:
			snx_cds_set_all(cds->dev_name, cds);
			break;
		case DS_GET_ALL:
			snx_cds_get_enable(cds->dev_name, &cds->enable);
			snx_cds_get_color(cds->dev_name, &cds->t_color, &cds->b_color);\
			snx_cds_get_color_attr(cds->dev_name, &cds->attr);
			snx_cds_get_position(cds->dev_name, &cds->pos, &cds->dim);
			//snx_cds_get_datastamp(cds->dev_name, cds->string, 0);
			snx_cds_get_scale(cds->dev_name, &cds->scale);
			break;
		case DS_SET_EN:
			snx_cds_set_enable(cds->dev_name, cds->enable);
			break;
		case DS_GET_EN:
			snx_cds_get_enable(cds->dev_name, &cds->enable);
			break;
		case DS_SET_COLOR:
			snx_cds_set_color(cds->dev_name, &cds->t_color, &cds->b_color);
			break;
		case DS_GET_COLOR:
			snx_cds_get_color(cds->dev_name, &cds->t_color, &cds->b_color);
			break;
		case DS_SET_COLOR_ATTR:
			snx_cds_set_color_attr(cds->dev_name, &cds->attr);
			break;
		case DS_GET_COLOR_ATTR:
			snx_cds_get_color_attr(cds->dev_name, &cds->attr);
			break;
		case DS_SET_POS:
			snx_cds_set_position(cds->dev_name, &cds->pos, &cds->dim);
			break;
		case DS_GET_POS:
			snx_cds_get_position(cds->dev_name, &cds->pos, &cds->dim);
			break;
		case DS_SET_STRING:
			snx_cds_get_scale(cds->dev_name, &cds->scale);
			snx_cds_set_string(cds->dev_name, cds->string, cds->scale);
			break;
		case DS_SET_DATA:
			snx_cds_set_datastamp(cds->dev_name, cds->string, strlen(cds->string));
			break;
		case DS_GET_DATA:
			snx_cds_get_datastamp(cds->dev_name, cds->string, 0);
			break;
		case DS_SET_BMP:
			snx_cds_get_scale(cds->dev_name, &cds->scale);
//			snx_cds_set_bmp(cds->dev_name, cds->string, cds->scale);
			snx_cds_set_bmp(cds);
			break;
		case DS_SET_SCALE:
			snx_cds_set_scale(cds->dev_name, cds->scale);
			break;
		case DS_GET_SCALE:
			snx_cds_get_scale(cds->dev_name, &cds->scale);
			break;
	}
	

}
