/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi device driver
 *
 * This file is released under the GPLv2.
 */
#include "idi2axi_internal.h"
#include <linux/bits.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/kconfig.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-subdev.h>
#include <media/videobuf2-core.h>
#include <securec.h>
#include "cam_intf.h"
#include "cam_log.h"
#include "idi2axi_intf.h"

#define sd_2_idi2axi(sd) container_of(sd, struct idi2axi_device, subdev)
#define dev_2_platform_dev(dev) container_of(dev, struct platform_device, dev)

static int idi2axi_subdev_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	(void)sd;
	(void)fh;
	return 0;
}

static int idi2axi_subdev_close(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	(void)fh;
	if (!sd) {
		cam_err("sd is NULL\n");
		return -1;
	}

	struct idi2axi_device *device = sd_2_idi2axi(sd);
	int rc = 0;
	if (device && device->intf && device->intf->vtbl && device->intf->vtbl->close)
		rc = device->intf->vtbl->close(device->intf);
	cam_info("%s idi2axi close rc =%d", __func__, rc);
	return rc;
}

static void *idi2axi_get_info(struct idi2axi_device *i2a, i2a_kinfo_t *info)
{
	char *name = NULL;
	size_t min_len;

	if (!i2a || !info) {
		cam_err("func %s pointer is NULL", __func__);
		return NULL;
	}

	if (!i2a->intf || !i2a->intf->vtbl || !i2a->intf->vtbl->get_name) {
		cam_err("func %s intf pointer is NULL", __func__);
		return NULL;
	}
	i2a_intf_t *intf = i2a->intf;
	name = (char *)i2a->intf->vtbl->get_name(intf);
	if (name) {
		errno_t err = memset_s(info->name, IDI2AXI_NAME_SIZE, 0,
							   IDI2AXI_NAME_SIZE);
		if (err != EOK)
			cam_err("func %s: memset_s fail", __func__);

		min_len = strlen(name);
		if (min_len > IDI2AXI_NAME_SIZE - 1)
			min_len = IDI2AXI_NAME_SIZE - 1;

		err = memcpy_s(info->name, IDI2AXI_NAME_SIZE, name, min_len);
		if (err != EOK) {
			cam_err("%s: memcpy_s idi2axi name fail", __func__);
			return NULL;
		}
		return info->name;
	} else {
		cam_err("%s: idi2axi_get_info() is NULL", __func__);
		return NULL;
	}
}

void idi2axi_notify_event(struct device *dev)
{
	struct v4l2_event ev;
	struct platform_device *pdev = dev_2_platform_dev(dev);
	struct idi2axi_device * idev = platform_get_drvdata(pdev);
	if (!idev) {
		pr_err("func %s: subdev is null", __func__);
		return;
	}
	struct video_device *vdev = idev->subdev.devnode;
	idi2axi_event_t *req = (idi2axi_event_t *)ev.u.data;
	errno_t err = memset_s(&ev, sizeof(struct v4l2_event), 0,
						   sizeof(struct v4l2_event));
	if (err != EOK) {
		pr_err("func %s: memset_s fail", __func__);
		return;
	}

	ev.type = IDI2AXI_V4L2_EVENT_TYPE;
	ev.id = IDI2AXI_HIGH_PRIO_EVENT;
	req->kind = IDI2AXI_MSG_CB;
	pr_info("%s v4l2_event_queue idi2axi event dev=%pK", __func__, vdev);
	v4l2_event_queue(vdev, &ev);
}

static long idi2axi_subdev_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	(void)arg;
	long ret = -1;
	struct idi2axi_device *device = NULL;
	if (!sd) {
		cam_err("sd is NULL\n");
		return ret;
	}
	cam_info("%s cmd:0x%x", __func__, cmd);
	device = sd_2_idi2axi(sd);
	size_t len = _IOC_SIZE(cmd);
	switch (cmd) {
	case IDI2AXI_IOCTL_CFG:
		if (device && device->intf && device->intf->vtbl && device->intf->vtbl->config)
			ret = device->intf->vtbl->config(device->intf, arg);
		break;
	case IDI2AXI_IOCTL_GET_INFO:
		if (idi2axi_get_info(device, (i2a_kinfo_t *)arg))
			ret = 0;
		break;
	case IDI2AXI_IOCTL_SEND_MSG:
		if (device && device->intf && device->intf->vtbl && device->intf->vtbl->on_msg)
			ret = device->intf->vtbl->on_msg(device->intf, arg);
		break;
	case IDI2AXI_IOCTL_RECV_MSG:
		if (device && device->intf && device->intf->vtbl && device->intf->vtbl->ack_msg)
			ret = device->intf->vtbl->ack_msg(device->intf, arg);
		break;
	case IDI2AXI_IOCTL_GET_ERR_INFO:
	    if(device && device->intf && device->intf->vtbl && device->intf->vtbl->get_err_info)
		    ret = device->intf->vtbl->get_err_info(device->intf, arg);
		break;
	default:
		cam_err("invalid IOCTL CMD = 0x%x\n", cmd);
		break;
	}
	return ret;
}

static int idi2axi_subdev_subscribe_event(struct v4l2_subdev *sd,
									   struct v4l2_fh *fh, struct v4l2_event_subscription *sub)
{
	(void)sd;
	cam_info("enter %s", __func__);
	/* 128:size */
	return v4l2_event_subscribe(fh, sub, 128, NULL);
}

static int idi2axi_subdev_unsubscribe_event(struct v4l2_subdev *sd,
										 struct v4l2_fh *fh, struct v4l2_event_subscription *sub)
{
	(void)sd;
	cam_info("enter %s", __func__);
	return v4l2_event_unsubscribe(fh, sub);
}

static struct v4l2_subdev_internal_ops s_subdev_internal_ops_idi2axi = {
	.open = idi2axi_subdev_open,
	.close = idi2axi_subdev_close,
};

static struct v4l2_subdev_core_ops s_subdev_core_ops_idi2axi = {
	.ioctl = idi2axi_subdev_ioctl,
	.subscribe_event = idi2axi_subdev_subscribe_event,
	.unsubscribe_event = idi2axi_subdev_unsubscribe_event,
};

static struct v4l2_subdev_ops s_subdev_ops_idi2axi = {
	.core = &s_subdev_core_ops_idi2axi,
};

int32_t idi2axi_register(struct platform_device *pdev, i2a_intf_t *intf)
{
	if (!intf || !pdev) {
		pr_err("func %s input para intf or pdev is NULL", __func__);
		return -1;
	}

	struct v4l2_subdev *subdev = NULL;
	struct idi2axi_device *idev = devm_kzalloc(&pdev->dev, sizeof(*idev), GFP_KERNEL);
	if (!idev)
		return -ENOMEM;

	subdev = &idev->subdev;
	idev->dev = &pdev->dev;
	if (!subdev) {
		pr_err("func %s: subdev is NULL", __func__);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		kzfree(idev);
#else
		kfree_sensitive(idev);
#endif
		return -1;
	}

	mutex_init(&idev->lock);
	v4l2_subdev_init(subdev, &s_subdev_ops_idi2axi);
	subdev->internal_ops = &s_subdev_internal_ops_idi2axi;
	int ret = snprintf_s(subdev->name, sizeof(subdev->name),
					 sizeof(subdev->name) - 1, "%s", "cam-cfg-idi2axi");
	if (ret < 0)
		cam_err("func %s: snprintf_s fail", __func__);
	subdev->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	subdev->flags |= V4L2_SUBDEV_FL_HAS_EVENTS;
	init_subdev_media_entity(subdev, CAM_SUBDEV_IDI2AXI);
	cam_cfgdev_register_subdev(subdev, CAM_SUBDEV_IDI2AXI);
	subdev->devnode->lock = &idev->lock;
	platform_set_drvdata(pdev, idev);
	idev->intf = intf;

	return 0;
}
