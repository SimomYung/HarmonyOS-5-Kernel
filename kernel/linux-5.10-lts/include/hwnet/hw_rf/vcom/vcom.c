/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to start vcom between ril and kernel
 */

#include <securec.h>
#include <asm/ioctl.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/string.h>

#include <log/hw_log.h>

#include "vcom.h"
#include "rf_plat_driver.h"
#include "vcom_nodes.h"
#include "satellite/rf_satellite_ctrl.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG HW_RF_VCOM
HWLOG_REGIST();

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define THIS_MODU mod_rfvcom
#define BASE_MONITOR_START 100

struct vcom_map_s {
	char* name;
	struct cdev cdev;
	dev_t devt;
	struct class* class;
	struct file_operations* ops;
	wait_queue_head_t wait;
	unsigned int iccid;
	unsigned int flag;
	unsigned int format;
	int (*idle)(struct vcom_map_s *map);
};

static struct file_operations vcom_dev_fops;
static struct vcom_map_s* g_vcom_map;

static struct vcom_map_s* get_map(struct inode *inode)
{
	if (inode->i_cdev) {
		return container_of(inode->i_cdev, struct vcom_map_s, cdev);
	}
	return (struct vcom_map_s *)PDE_DATA(inode);
}

static long vcom_ioctl(struct file *filp, unsigned int cmd, unsigned long data)
{
	int32_t ret = -1;

	hwlog_info("%s enter, cmd:%d, data:%d, cmd type:%c\n", __func__, cmd, data, _IOC_TYPE(cmd));
	switch (_IOC_TYPE(cmd)) {
	case RF_IOCTL_MAGIC_FAC:
		ret = rf_fac_ioctl_proc(cmd, data);
		break;
	case RF_IOCTL_MAGIC_RELEASE:
		ret = rf_release_ioctl_proc(cmd, data);
		break;
	default:
		hwlog_info("%s invalid cmd type\n", __func__);
		break;
	}

	return ret;
}

static int vcom_open(struct inode *inode, struct file *filp)
{
	struct vcom_map_s *map = NULL;

	hwlog_info("%s enter\n", __func__);

	if (inode == NULL)
		return -1;
	if (filp == NULL)
		return -1;
	map = get_map(inode);
	hwlog_info("%s done \n", __func__);
	return 0;
}

static int vcom_release(struct inode *inode, struct file *filp)
{
	struct vcom_map_s *map = NULL;
	hwlog_info("%s enter\n", __func__);
	if (filp == NULL)
		return -1;
	map = get_map(inode);
	hwlog_info("%s done\n", __func__);
	return 0;
}

static struct file_operations vcom_dev_fops = {
	.owner = THIS_MODULE,
	.open = vcom_open,
	.unlocked_ioctl = vcom_ioctl,
	.compat_ioctl = vcom_ioctl,
	.release = vcom_release,
};

static struct file_operations* vcom_ops_setup(unsigned int idx)
{
	switch (com_name_id_map[idx].flag) {
	case VCOM_SYNCCOM:
		return &vcom_dev_fops;
	default:
		hwlog_info("%s, vcom_ops_setup: idx = %x, flag = %x\n", __func__, idx, com_name_id_map[idx].flag);
		return NULL;
	}
}

int hw_rfvcom_init(void)
{
	int ret;
	unsigned int i;
	dev_t devt;
	unsigned int size;
	struct device* dev = NULL;

	size = sizeof(com_name_id_map) / sizeof(*com_name_id_map);
	g_vcom_map = kvzalloc(size * sizeof(*g_vcom_map), GFP_KERNEL);
	if (g_vcom_map == NULL)
		return -1;
	ret = alloc_chrdev_region(&devt, BASE_MONITOR_START,
		sizeof(com_name_id_map) / sizeof(*com_name_id_map), "rfvcom");
	if (ret) {
		goto err_rfvcom_init;
	}
	for (i = 0; i < size; i++) {
		g_vcom_map[i].name = com_name_id_map[i].name;
		g_vcom_map[i].iccid = i;
		g_vcom_map[i].format = com_name_id_map[i].format;
		if (g_vcom_map[i].format != VCOM_STREAM) {
			goto err_rfvcom_init;
		}
		g_vcom_map[i].ops = vcom_ops_setup(i);
		g_vcom_map[i].flag = com_name_id_map[i].flag;
		if (VCOM_TYPE_VIRTUAL != com_name_id_map[i].type) {
			goto err_rfvcom_init;
		}
		g_vcom_map[i].devt = devt + i;
		cdev_init(&g_vcom_map[i].cdev, g_vcom_map[i].ops);
		ret = cdev_add(&g_vcom_map[i].cdev, g_vcom_map[i].devt, 1);
		if (ret) {
			goto err_rfvcom_init;
		}
		g_vcom_map[i].class = class_create(THIS_MODULE, g_vcom_map[i].name);
		dev = device_create(g_vcom_map[i].class, NULL, g_vcom_map[i].devt, NULL, g_vcom_map[i].name);
		if (IS_ERR(dev)) {
			goto err_rfvcom_init;
		}
	}
	return 0;

err_rfvcom_init:
	kfree(g_vcom_map);
	g_vcom_map = NULL;
	return -1;
}

void hw_rfvcom_exit(void)
{
	int i;
	unsigned int com_size = sizeof(com_name_id_map) / sizeof(*com_name_id_map);

	hwlog_info("%s enter com_size:%d\n", __func__, com_size);
	for (i = 0; i < com_size; i++) {
		cdev_del(&(g_vcom_map[i].cdev));
		if (g_vcom_map) {
			kfree(g_vcom_map);
			g_vcom_map = NULL;
		}
		unregister_chrdev_region(g_vcom_map[i].devt, 1);
	}
	hwlog_info("%s end\n", __func__);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */