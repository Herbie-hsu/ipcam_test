/* ov971x.c - a simple device driver for the SN986xx target use i2c to
    connect sensor ov9715 interface , if you want to get more information about
    ov9715 driver support cmd, you can referance 
    st58600/driver/driver/video/src/driver/isp2/sensor/ov9715.c
    The i2c_board_info is defined in
    st58600/kernel/linux-2.6.35.12/src/arch/arm/mach-sn986xx/devices.c  */
    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-common.h>
#include <linux/time.h>
#include <mach/snx_media.h>
#include <linux/proc_fs.h>

#include <media/soc_camera.h>

#include "simple_i2c_ov971x.h"

/* read a register */
static int ov9715_reg_read(struct i2c_client *client, u8 reg, u8 *val)
{
	int ret;
	u8 data = reg;
	struct i2c_msg msg = {
		.addr	= client->addr,
		.flags	= 0,
		.len	= 1,
		.buf	= &data,
	};

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0)
		goto err;

	msg.flags = I2C_M_RD;
	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0)
		goto err;

	*val = data;
	return 0;

err:
	dev_err(&client->dev, "Failed reading register 0x%02x!\n", reg);
	return ret;
}

/* write a register */
static int ov9715_reg_write(struct i2c_client *client, u8 reg, u8 val)
{
	int ret;
	unsigned char data[2] = { reg, val };
	struct i2c_msg msg = {
		.addr	= client->addr,
		.flags	= 0,
		.len	= 2,
		.buf	= data,
	};

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0) {
		dev_err(&client->dev, "Failed writing register 0x%02x!\n", reg);
		return ret;
	}

	return 0;
}
/* Soft reset the camera. This has nothing to do with the RESET pin! */
static int ov9715_reset(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	ret = ov9715_reg_write(client, OV9715_COM7, COM7_SCCB_RESET);
	if (ret)
		dev_err(&client->dev,"An error occured while entering soft reset!\n");

	msleep(20);
  printk ("I2C Write reset cmd to ov9715 ok\n\n\n\n");
	return ret;
}

static int ov9715_video_probe(struct soc_camera_link *icl, struct i2c_client *client)
{
	int ret;
	const char *devname;
	u8 pid, ver, midh, midl;
	struct sensor_info *info = container_of(i2c_get_clientdata(client), struct sensor_info, subdev);
	
  
	/*
	 * check and show product ID and manufacturer ID
	 */

	ret = ov9715_reg_read(client, OV9715_PIDH, &pid);
	if (ret)
		goto err;
  
	ret = ov9715_reg_read(client, OV9715_PIDL, &ver);
	if (ret)
		goto err;
  

	ret = ov9715_reg_read(client, OV9715_MIDH, &midh);
	if (ret)
		goto err;
  
	ret = ov9715_reg_read(client, OV9715_MIDL, &midl);
	if (ret)
		goto err;

  
	switch (VERSION(pid, ver)) {
	case OV9715:
		devname		= SENSOR_NAME;
		info->id = OV9715;
		//info->model	= V4L2_IDENT_OV9715;
		//info->revision	= 2;
		break;
	default:
		dev_err(&client->dev, "Product ID error %x:%x\n", pid, ver);
		ret = -ENODEV;
		goto err;
	}
  
	printk("I2C read %s Product ID 0x%0x Version 0x%0x \ 
  Manufacturer ID 0x%x:0x%x ok\n", devname, pid, ver, midh, midl);

	//reset sensor
	ov9715_reset(&info->subdev, 1);

	return 0;

err:
	printk("ov9715 sensor don't connect\n");
	return ret;
}




static struct v4l2_subdev_ops ov9715_subdev_ops = {
	.core	= NULL,
	.video	= NULL,
};

/*
 * i2c_driver function
 */
static int ov9715_probe(struct i2c_client *client, const struct i2c_device_id *did)
{
	struct sensor_info *info;
  
	struct soc_camera_link *icl = soc_camera_i2c_to_link(client);
	int ret;

	if (!icl) {
		dev_err(&client->dev, "Missing platform_data for driver\n");
		return -EINVAL;
	}

	info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
	if (!info) {
		dev_err(&client->dev,
			"Failed to allocate memory for infoate data!\n");
		return -ENOMEM;
	}

	info->priv = kzalloc(sizeof(struct ov9715_priv_data), GFP_KERNEL);
	if (!info->priv) {
		dev_err(&client->dev,
			"Failed to allocate memory for device info data!\n");
		return -ENOMEM;
	}

  
	v4l2_i2c_subdev_init(&info->subdev, client, &ov9715_subdev_ops);
  
	ret = ov9715_video_probe(icl, client);
	if (ret) {
		kfree(info->priv);
		kfree(info);
		return -EINVAL;
	}

	return ret;
}

static int ov9715_remove(struct i2c_client *client)
{
	struct sensor_info *info = container_of(i2c_get_clientdata(client), struct sensor_info, subdev);

	kfree(info->priv);
	kfree(info);

	return 0;
}

static const struct i2c_device_id ov9715_id[] = {
	{ SENSOR_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ov9715_id);

static struct i2c_driver ov9715_i2c_driver = {
	.driver = {
		.name = "ov9715",
	},
	.probe    = ov9715_probe,
	.remove   = ov9715_remove,
	.id_table = ov9715_id,
};

static int __init ov9715_module_init(void)
{
	printk("load i2c_tov9715_simple_test driver\n"); 
	return i2c_add_driver(&ov9715_i2c_driver);
}

static void __exit ov9715_module_exit(void)
{
	printk("unload i2c_tov9715_simple_test driver\n");   
	i2c_del_driver(&ov9715_i2c_driver);
}

module_init(ov9715_module_init);
module_exit(ov9715_module_exit);

MODULE_DESCRIPTION("I2C driver example to connect OV9715");
MODULE_AUTHOR("alek");
MODULE_LICENSE("GPL v2");
