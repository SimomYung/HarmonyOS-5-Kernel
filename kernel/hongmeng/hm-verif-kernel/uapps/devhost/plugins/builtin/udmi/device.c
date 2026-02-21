/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementations for device APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 09:52:08 2020
 */
#include <devhost/device.h>

#include <vfs.h>
#include <devhost/plugin.h>
#include <libsysif/devmgr/types.h>
#include <libhmsrv_sys/hm_mem.h>

#include "types.h"
#include <devhost/log.h>

static void devhost_devinfo_release(struct uobject *obj)
{
	struct device_private *p = container_of(obj, struct device_private, obj);
	struct device_info *devinfo = NULL;

	devinfo = p->devinfo;
	if (p->compatible.value != NULL) {
		free(p->compatible.value);
	}
	if (p->bd_queue_info != NULL) {
		free(p->bd_queue_info);
	}
	free(p);
	if (devinfo->release != NULL) {
		devinfo->release(devinfo);
	}
}

/* device APIs */
PUBLIC_SYMBOL
int devhost_device_init(struct device_info *devinfo)
{
	if (devinfo == NULL) {
		return -EINVAL;
	}
	if (devinfo->p != NULL) {
		dh_error("repeatedly inited\n");
		return -EINVAL;
	}

	devinfo->p = (struct device_private *)malloc(sizeof(*devinfo->p));
	if (devinfo->p == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*devinfo->p);
	devinfo->p->devinfo = devinfo;
	devinfo->release = NULL;

	uobject_init(&devinfo->p->obj, 1, devhost_devinfo_release);

	return 0;
}

PUBLIC_SYMBOL
void devhost_device_destroy(struct device_info *devinfo)
{
	if (devinfo == NULL) {
		return;
	}

	devinfo_put(devinfo);
}

PUBLIC_SYMBOL
void devhost_device_set_autoload(struct device_info *devinfo)
{
	devinfo->p->flags |= DEVICE_AUTO_LOAD;
}

PUBLIC_SYMBOL
void devhost_device_set_isolate(struct device_info *devinfo, const char *isolate_group)
{
	devinfo->p->flags |= DEVICE_MUST_ISOLATE;
	devinfo->p->isolate_group = isolate_group;
}

PUBLIC_SYMBOL
int devhost_device_set_probeable(struct device_info *devinfo,
				const char *compatible, size_t size)
{
	BUG_ON((compatible == NULL) && (size != 0));
	BUG_ON((compatible != NULL) && (size == 0));

	devinfo->p->flags |= DEVICE_CAN_PROBE;
	if (size != 0) {
		devinfo->p->compatible.value = malloc(size);
		if (devinfo->p->compatible.value == NULL) {
			return -ENOMEM;
		}
		NOFAIL(memcpy_s(devinfo->p->compatible.value, size, compatible, size));
		devinfo->p->compatible.length = size;
	}
	return 0;
}

PUBLIC_SYMBOL
void devhost_device_capable_devfs(struct device_info *devinfo,
				  uint32_t devt, int vfs_mode,
				  devnode_fn_t devnode_fn,
				  struct devhost_fops *fops)
{
	BUG_ON((devinfo->p->flags & DEVICE_CAN_PROBE) != __U(0));

	devinfo->p->devt = devt;
	devinfo->p->vfs_mode = vfs_mode;
	devinfo->p->devnode_fn = devnode_fn;
	devinfo->p->fops = fops;
}

PUBLIC_SYMBOL
void devhost_device_capable_devfs_ex(struct device_info *devinfo,
				     struct devhost_fops_ex *ex_fops)
{
	if (devinfo->p->fops == NULL) {
		dh_warn("fops must be initialized first\n");
		return;
	}
	devinfo->p->ex_fops = ex_fops;
}

PUBLIC_SYMBOL
void devhost_device_capable_blk(struct device_info *devinfo,
				struct devhost_block_ops *blkops)
{
	BUG_ON((devinfo->p->flags & DEVICE_CAN_PROBE) != __U(0));
	BUG_ON(devinfo->p->fops == NULL);

	devinfo->p->blkops = blkops;
}

PUBLIC_SYMBOL
void devhost_device_capable_directmap_shm(struct device_info *devinfo)
{
	devinfo->p->flags |= DEVICE_CAN_DIRECTMAP_SHM;
}

PUBLIC_SYMBOL
void devhost_device_capable_pm(struct device_info *devinfo,
			       struct devhost_dev_pm_ops *pmops)
{
	devinfo->p->pmops = pmops;
}

PUBLIC_SYMBOL
void devhost_device_capable_net(struct device_info *devinfo,
				struct devhost_net_ops *netops)
{
	BUG_ON((devinfo->p->flags & DEVICE_CAN_PROBE) != __U(0));

	devinfo->p->netops = netops;
}

PUBLIC_SYMBOL
void devhost_device_capable_pclk(struct device_info *devinfo,
				 struct devhost_posix_clock_ops *clkops)
{
	BUG_ON((devinfo->p->flags & DEVICE_CAN_PROBE) != __U(0));
	BUG_ON((devinfo->p->vfs_mode != (int)MODE_CHR) || (devinfo->p->devt == 0));

	devinfo->p->clkops = clkops;
}

PUBLIC_SYMBOL
int devhost_device_open(struct device_info *dev, void **ctx, unsigned int flags)
{
	return dev->p->fops->open(dev, ctx, flags);
}

PUBLIC_SYMBOL
void devhost_device_set_anonymous(struct device_info *devinfo)
{
	devinfo->p->flags |= DEVICE_ANONYMOUS;
}

int devhost_device_is_anonymous(const struct device_info *devinfo)
{
	return ((devinfo->p->flags & DEVICE_ANONYMOUS) != __U(0));
}

PUBLIC_SYMBOL
unsigned int devhost_device_get_fops_flags(struct device_info *devinfo, void *filp)
{
	unsigned int ret = 0;

	if (devinfo == NULL)
		return ret;

	if (devinfo->p->ex_fops != NULL ) {
		if (devinfo->p->ex_fops->flush != NULL) {
			 if ((devinfo->p->ex_fops->get_fops_flags != NULL)
			     && (filp != NULL)) {
				ret = devinfo->p->ex_fops->get_fops_flags(filp);
			}
		} else {
			ret |= DH_IO_CTX_DEVFOPS_NO_FLUSH_FLAG;
		}
	} else {
		ret |= DH_IO_CTX_DEVFOPS_NO_FLUSH_FLAG;
	}

	return ret;
}
