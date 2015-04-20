/*
 * Sensor Header File
 *
 * Copyright (C) 2012 qingbin li <qingbin_li@cdmail.sonix.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef	__SIMPLE_I2C_OV971X_H__
#define	__SIMPLE_I2C_OV971X_H__

#define SENSOR_NAME		"ov9715"

/* system control registers */

#define OV9715_PIDH 		0x0a
#define OV9715_PIDL 		0x0b
#define OV9715_COM7 		0x12
#define OV9715_MIDH 		0x1c
#define OV9715_MIDL 		0x1d
/* IDs */
#define VERSION(pid, ver)		(((pid) << 8) | ((ver) & 0xFF))

#define OV9715				0x9711

#define COM7_SCCB_RESET 	(0x1<<7)

struct ov9715_priv_data {
	u32 input_clk;
	u32 output_clk;
	u32 dummy_pixels;
	u32 dummy_lines;

	u32 xclk_hz;
	u32 clk1_hz;
	u32 clk2_hz;
	u32 clk3_hz;
	u32 sys_clk_hz;
	u32 pix_clk_hz;
};


struct sensor_info {
	struct v4l2_subdev subdev;
	int id;
	int model;
	int revision;
	int max_width;
	int max_height;
	struct v4l2_rect  iw; //image window

	u32 frame_rate;
	u32 pix_clock;
	u32 pix_per_line;

	void *priv;
};


#endif	/* __SIMPLE_I2C_OV971X_H__ */
