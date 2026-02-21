/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: Implement of idi2axi sensor v4l2 device driver
 *
 * This file is released under the GPLv2.
 */

#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/kernel.h>
#include <linux/kconfig.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <linux/timekeeping.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-v4l2.h>
#include <platform_include/camera/native/sensor_cfg.h>
#include "i2a_sensor_common.h"
#include "idi2axi_internal.h"
#include "i2a_hwsensor.h"
#include "cam_intf.h"


/* This structure contains v4l2_subdev and pointers to the interface functions that need to
be implemented by the sensor. It is used as the interface structure for processing ioctl. */
struct i2a_hwsensor_t {
	struct v4l2_subdev subdev;
	struct platform_device *pdev;
	i2a_hwsensor_intf_t *intf;
	struct mutex lock;
	int cam_dev_num;
};

#define sd_to_i2a_sensor(i) container_of(i, struct i2a_hwsensor_t, subdev)
#define intf_to_sensor(i) container_of(i, i2a_sensor_t, intf)

static long i2a_sensor_subdev_get_info(struct i2a_hwsensor_t *i2a_s, hwsensor_info_t *info)
{
	int i;
	int rc;
	i2a_sensor_t *sensor = NULL;

	if (!i2a_s || !info || !i2a_s->intf) {
		pr_err("s or info is null");
		return -1;
	}

	sensor = intf_to_sensor(i2a_s->intf);
	rc = memset_s(info, sizeof(hwsensor_info_t),
		0, sizeof(hwsensor_info_t));
	if (rc != 0) {
		pr_err("%s memset_s return fail\n", __func__);
		return -1;
	}

	rc = strncpy_s(info->name, DEVICE_NAME_SIZE - 1,
		i2a_hwsensor_intf_get_name(i2a_s->intf),
		strlen(i2a_hwsensor_intf_get_name(i2a_s->intf)) + 1);
	if (rc != 0) {
		pr_err("%s sensor device name copy error\n", __func__);
		return -1;
	}

	info->dev_id = i2a_s->cam_dev_num;
	info->mount_position =
		(hwsensor_position_kind_t)sensor->board_info->sensor_index;
	return 0;
}

static long i2a_sensor_subdev_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	pr_info("i2a_sensor_subdev_ioctl in, %p", sd);
	long rc = -EINVAL;
	struct i2a_hwsensor_t *s = NULL;

	if (!sd) {
		pr_err("sd is NULL\n");
		return -EINVAL;
	}

	s = sd_to_i2a_sensor(sd);
	pr_info("i2a sensor cmd = %x", cmd);
	if (!s->intf || !s->intf->vtbl) {
		pr_err("%s sensor intf null", __func__);
		return rc;
	}

	switch (cmd) {
	case HWSENSOR_IOCTL_GET_INFO:
		rc = i2a_sensor_subdev_get_info(s, arg);
		break;
	case HWSENSOR_IOCTL_SENSOR_CFG:
		if (s->intf->vtbl->config)
			rc = s->intf->vtbl->config(s->intf, arg);
		break;
	default:
		pr_err("invalid IOCTL CMD = %d\n", cmd);
		break;
	}
	return rc;
}

static int i2a_hwsensor_subdev_internal_close(struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	struct i2a_hwsensor_t *s = sd_to_i2a_sensor(sd);
	int rc = 0;
	struct sensor_cfg_data cdata = {0};

	cdata.cfgtype = SEN_CONFIG_POWER_OFF;

	if (!s) {
		pr_err("%s get s_strl error", __func__);
		return -1;
	}
	if (!s->intf || !s->intf->vtbl ||
		!s->intf->vtbl->config)
		return rc;

	rc = s->intf->vtbl->config(s->intf, (void *)(&cdata));

	pr_info(" enter %s, return value %d", __func__, rc);
	return rc;
}

static int i2a_hwsensor_subdev_open(
	struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	return 0;
}

static int i2a_hwsensor_subdev_close(
	struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	i2a_hwsensor_subdev_internal_close(sd, fh);
	return 0;
}

static struct v4l2_subdev_core_ops s_subdev_core_ops_i2a_hwsensor = {
	.ioctl = i2a_sensor_subdev_ioctl,
};

static struct v4l2_subdev_ops s_subdev_ops_i2a_hwsensor = {
	.core = &s_subdev_core_ops_i2a_hwsensor,
};

static struct v4l2_subdev_internal_ops s_subdev_internal_ops_i2a_hwsensor = {
	.open  = i2a_hwsensor_subdev_open,
	.close = i2a_hwsensor_subdev_close,
};

int i2a_hwsensor_register(struct platform_device *pdev, i2a_hwsensor_intf_t *intf)
{
	int ret = 0;
	struct v4l2_subdev *subdev = NULL;
	struct i2a_hwsensor_t *sensor = NULL;
	i2a_sensor_t *i2a_sensor = NULL;

	if (!pdev || !intf) {
		pr_err("%s pdev or si is NULL", __func__);
		return -EINVAL;
	}

	i2a_sensor = intf_to_sensor(intf);
	if (!i2a_sensor) {
		dev_err(&pdev->dev, "%s hisensor is NULL", __func__);
		return -EINVAL;
	}

	sensor = devm_kzalloc(&pdev->dev, sizeof(struct i2a_hwsensor_t), GFP_KERNEL);
	if (!sensor) {
		ret = -ENOMEM;
		goto register_fail;
	}

	subdev = &sensor->subdev;
	mutex_init(&sensor->lock);

	v4l2_subdev_init(subdev, &s_subdev_ops_i2a_hwsensor);
	subdev->internal_ops = &s_subdev_internal_ops_i2a_hwsensor;
	if (snprintf_s(subdev->name, sizeof(subdev->name),
		sizeof(subdev->name) - 1,
		"%s", i2a_hwsensor_intf_get_name(intf)) < 0)
		dev_err(&pdev->dev, "%s snprintf_s failed", __func__);

	subdev->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	v4l2_set_subdevdata(subdev, i2a_sensor);
	platform_set_drvdata(pdev, subdev);

	init_subdev_media_entity(subdev, CAM_SUBDEV_IDI2AXI_SENSOR);
	cam_cfgdev_register_subdev(subdev, CAM_SUBDEV_IDI2AXI_SENSOR);
	subdev->devnode->lock = &sensor->lock;

	cam_dev_create(&pdev->dev, &sensor->cam_dev_num);
	sensor->intf = intf;
	sensor->pdev = pdev;

register_fail:
	return ret;
}

void i2a_hwsensor_unregister(struct platform_device *pdev)
{
	struct v4l2_subdev *subdev = platform_get_drvdata(pdev);
	struct i2a_hwsensor_t *sensor = sd_to_i2a_sensor(subdev);

	media_entity_cleanup(&subdev->entity);
	cam_cfgdev_unregister_subdev(subdev);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	kzfree(sensor);
#else
	kfree_sensitive(sensor);
#endif
}