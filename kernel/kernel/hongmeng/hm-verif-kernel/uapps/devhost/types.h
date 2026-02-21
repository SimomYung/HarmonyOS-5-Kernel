/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Type definations and helper prototypes
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_TYPES_H__
#define __DEVHOST_TYPES_H__

#include <unistd.h>
#include <stdint.h>
#include <lib/dlist.h>
#include <libdevhost/uobject.h>
#include <libdevhost/compatible.h>

#include <devhost/device.h>
#ifdef CONFIG_SEHARMONY
#include <devhost/security_seharmony.h>
#endif

struct hm_driver_ops;

struct driver_private {
	struct uobject obj;
	struct compatible_string compat;
	hm_driver_t *drv;
	const hm_driver_ops_t *ops;
	void *ctx;
	struct dlist_node node;
};

struct devhost_fops;
struct devhost_fops_ex;
struct devhost_block_ops;
struct devhost_dev_pm_ops;
struct devhost_net_ops;
struct devhost_posix_clock_ops;
struct bd_queue_info;

/* device private info */
struct device_private {
	struct uobject obj;
	struct device_info *devinfo;
	unsigned int flags;
	uint32_t devt;
	int vfs_mode;
	devnode_fn_t devnode_fn;
#ifdef CONFIG_SEHARMONY
	struct device_info_sec devinfo_cred;
	struct device_info_sec devinfo_cred_aoco;
#endif
	struct devhost_fops *fops;
	struct devhost_fops_ex *ex_fops;
	struct devhost_block_ops *blkops;
	struct devhost_dev_pm_ops *pmops;
	struct devhost_net_ops *netops;
	struct devhost_posix_clock_ops *clkops;
	const char *isolate_group; /* valid when flags & DEVICE_MUST_ISOLATE */
	struct {
		char *value;
		size_t length;
	} compatible;

	/*
	 * queue(s) to process bio, each vfs has one dlist_node to
	 * store it's rings in the device.
	 */
	struct bd_queue_info *bd_queue_info;
};

static inline struct hm_driver *driver_get(struct hm_driver *drv)
{
	(void)uobject_get(&drv->p->obj);
	return drv;
}

static inline void driver_put(struct hm_driver *drv)
{
	(void)uobject_put(&drv->p->obj);
}

#endif /* __DEVHOST_TYPES_H__ */
