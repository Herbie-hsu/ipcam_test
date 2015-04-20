
#ifndef __ISP_LIB_API_H__
#define __ISP_LIB_API_H__

#ifdef __cplusplus
extern "C"{
#endif

enum{
	ISP_CH_0 = 0x0,
	ISP_CH_1 = 0x1,
};

enum{
	ISP_DISABLE = 0,
	ISP_ENABLE = 1,
};

/*isp iq tunning tool*/
int snx_isp_iq_write(void *b, int s);
int snx_isp_iq_read(void *b, int s);
int snx_isp_iq_enable_get(int *enable);
int snx_isp_iq_enable_set(int enable);
int snx_isp_iq_firmware_reload(void);

/*motion detection*/
int snx_isp_md_enable_get(int *enable);
int snx_isp_md_enable_set(int enable);
int snx_isp_md_threshold_get(int *threshold);
int snx_isp_md_threshold_set(int threshold);
int snx_isp_md_int_get(int *status);
int snx_isp_md_int_timeout_set(int ms);
int snx_isp_md_int_timeout_get(int *ms);
int snx_isp_md_int_threshold_get(int *threshold);
int snx_isp_md_int_threshold_set(int threshold);
int snx_isp_md_x_step_size_get(int *step);
int snx_isp_md_x_step_size_set(int step);
int snx_isp_md_y_step_size_get(int *step);
int snx_isp_md_y_step_size_set(int step);
int snx_isp_md_block_mask_get(unsigned int mask[]);
int snx_isp_md_block_mask_set(unsigned int mask[]);
int snx_isp_md_block_report_get(unsigned int report[]);

/*osd*/
#ifndef TIMESTAMP_TEMP
#define TIMESTAMP_TEMP "0123456789:/."
#define SPACE_TEMP " "
#endif

int snx_isp_osd_enable_set(int ch, int enable);
int snx_isp_osd_data_str_set(int ch, const char *str);
int snx_isp_osd_data_str_get(int ch, char *str);
int snx_isp_osd_motion_str_set(int ch, const char *str);
int snx_isp_osd_rec_str_set(int ch, const char *str);
int snx_isp_osd_recshow_set(int ch, int enable);
int snx_isp_osd_timestamp_set(int ch, int enable);
int snx_isp_osd_timestamp_get(int ch, int *enable);
int snx_isp_osd_gain_set(int ch, int gain);
int snx_isp_osd_gain_get(int ch, int *gain);
int snx_isp_osd_txt_color_set(int ch, int color);
int snx_isp_osd_bg_color_set(int ch, int color);
int snx_isp_osd_txt_transp_set(int ch, int transp);
int snx_isp_osd_bg_transp_set(int ch, int transp);
int snx_isp_osd_position_set(int ch, int x, int y);
int snx_isp_osd_position_get(int ch, int *x, int *y);
int snx_isp_osd_template_set(int ch, char *str);
int snx_isp_osd_font_set(int ch, char *font);
int snx_isp_osd_width_get(int ch, int *width);
int snx_isp_osd_height_get(int ch, int *height);
/*isp mirror flip*/
enum{
	ISP_MIRROR_FLIP_OFF = 0x0,
	ISP_FLIP_ON,
	ISP_MIRROR_ON,
	ISP_MIRROR_FLIP_ON,
};
int snx_isp_mirror_flip_mode_set(int mode);
/*sensor mirror*/
int snx_isp_sensor_mirror_set(int mode);
/*sensor flip*/
int snx_isp_sensor_flip_set(int mode);
int snx_isp_sensor_aec_set(int mode);
int snx_isp_sensor_exposure_set(int mode);
int snx_isp_sensor_gain_set(int mode);
int snx_isp_aec_enable_set(int enable);
int snx_isp_light_frequency_set(int frq);
int snx_isp_light_frequency_get(int *frq);
int snx_isp_light_exposure_time_set(int val);
int snx_isp_sensor_awb_set(int val);
int snx_isp_sensor_redGain_set(int gain);
int snx_isp_sensor_greenGain_set(int gain);
int snx_isp_sensor_blueGain_set(int gain);
int snx_isp_awb_enable_set(int enable);
int snx_isp_drc_status_get(int *status);//0 is off, 1 is on,-1 is error
int snx_isp_drc_status_set(int status);
int snx_isp_drc_value_set(int val);
int snx_isp_drc_value_get(int *val);
int snx_isp_pm_enable_set(int enable);
int snx_isp_pm_color_set(int color);
int snx_isp_pm_area_set(int *area);
int snx_isp_hdr_enable_set(int enable);
int snx_isp_filter_contrast_set(int val);
int snx_isp_filter_sharpness_set(int val);
int snx_isp_filter_saturation_set(int val);
int snx_isp_filter_hue_set(int val);
int snx_isp_filter_brightness_set(int val);
int snx_isp_sensor_name_get(char *name);



/*motion detection osd*/
int snx_isp_md_osd_enable_set(int ch, int enable);
int snx_isp_md_osd_enable_get(int ch, int *enable);

/*record osd*/
int snx_isp_record_osd_enable_set(int ch, int enable);
int snx_isp_record_osd_enable_get(int ch, int *enable);

#ifdef __cplusplus
}
#endif



#endif /*__ISP_LIB_API_H__*/
