/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fusion device headers
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#ifndef __UDK_FUSION_DEVICE_H__
#define __UDK_FUSION_DEVICE_H__

#include <libdevhost/compatible.h>
#include <libdevhost/uobject.h>

#include <devhost/device.h>
#include <udk/device.h>

struct device_private {
	struct uobject obj;
	uint32_t devt;
	int vfs_mode;
	struct device_info *devinfo;
	devnode_fn_t devnode_fn;
	struct devhost_fops *fops;
	struct devhost_fops_ex *ex_fops;
	struct devhost_dev_pm_ops *pmops;
	struct devhost_block_ops *blkops;
	unsigned int flags;
	struct {
		char *value;
		size_t length;
	} compatible;
};

struct hm_actv_buf;

int udk_fusion_devid_acquire(uint32_t devt, int *devid);
int udk_fusion_device_acquire(int devid, struct device_info **info);
int udk_fusion_device_register(struct device_info *info);
int udk_fusion_device_unregister(struct device_info *info);
int udk_fusion_hdr_device_create(struct hm_actv_buf devname,
				 struct hm_actv_buf dev_compat,
				 const struct device_create_args *args,
				 int *devid_out);
int udk_fusion_hdr_device_bind(int devid, struct hm_actv_buf dev_compat);

int udk_fusion_driver_init(struct udk_driver *udkdrv);
int udk_fusion_driver_register(struct udk_driver *udkdrv);
int udk_fusion_driver_acquire(const char *compat, struct udk_driver **udkdrv);
struct device_info *udk_fusion_devinfo_get(struct device_info *devinfo);
void udk_fusion_devinfo_put(struct device_info *devinfo);

#endif
