/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override device interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#include <libstrict/strict.h>
#include <libdevhost/devhost.h>
#include <libsysif/devmgr/types.h>

#include <udk/log.h>
#include <devhost/backend.h>

#include "device.h"
#include "file.h"
#include "of/of_dev.h"

/* override UDK_THIS_MODULE */
struct udk_module *g_udk_this_module = NULL;

static void __devinfo_release(struct uobject *obj)
{
	struct device_private *p = container_of(obj, struct device_private, obj);
	struct device_info *devinfo = NULL;

	devinfo = p->devinfo;
	if (p->compatible.value != NULL) {
		free(p->compatible.value);
		p->compatible.value = NULL;
	}

	free(p);
	devinfo->p = NULL;
	if (devinfo->release != NULL) {
		devinfo->release(devinfo);
	}
}

int devhost_device_init(struct device_info *info)
{
	if (info == NULL) {
		return -EINVAL;
	}
	if (info->p != NULL) {
		udk_error("repeatedly inited\n");
		return -EINVAL;
	}

	info->p = (struct device_private *)malloc(sizeof(*info->p));
	if (info->p == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*info->p);
	info->p->devinfo = info;

	uobject_init(&info->p->obj, 1, __devinfo_release);

	return 0;
}

void devhost_device_capable_devfs(struct device_info *info,
				  uint32_t devt, int vfs_mode,
				  devnode_fn_t devnode_fn,
				  struct devhost_fops *fops)
{
	info->p->devt = devt;
	info->p->vfs_mode = vfs_mode;
	info->p->devnode_fn = devnode_fn;
	info->p->fops = fops;
}

void devhost_device_capable_devfs_ex(struct device_info *info,
				     struct devhost_fops_ex *ex_fops)
{
	info->p->ex_fops = ex_fops;
}

void devhost_device_capable_blk(struct device_info *info,
				struct devhost_block_ops *blkops)
{
	BUG_ON(info->p->fops == NULL);
	info->p->blkops = blkops;
}

void devhost_device_capable_pm(struct device_info *info,
			       struct devhost_dev_pm_ops *pmops)
{
	info->p->pmops = pmops;
}

int devhost_register_device(struct device_info *info)
{
	return -hmerrno2posix(udk_fusion_device_register(info));
}

int devhost_unregister_device(struct device_info *info)
{
	return -hmerrno2posix(udk_fusion_device_unregister(info));
}

void devhost_device_destroy(struct device_info *info)
{
	if (info == NULL) {
		return;
	}

	udk_fusion_devinfo_put(info);
}

void devhost_device_set_autoload(struct device_info *info)
{
	info->p->flags |= DEVICE_AUTO_LOAD;
}

void devhost_device_set_isolate(struct device_info *info, const char *isolate_group)
{
	UNUSED(isolate_group);
	info->p->flags |= DEVICE_MUST_ISOLATE;
}

int devhost_device_set_probeable(struct device_info *info,
				 const char *compatible, size_t size)
{
	if ((compatible == NULL) || (size == 0)) {
		return -EINVAL;
	}
	info->p->flags |= DEVICE_CAN_PROBE;
	info->p->compatible.value = malloc(size);
	if (info->p->compatible.value == NULL) {
		return -ENOMEM;
	}
	NOFAIL(memcpy_s(info->p->compatible.value, size, compatible, size));
	info->p->compatible.length = size;

	return 0;
}

/* override driver interfaces */
int devhost_driver_init(hm_driver_t *drv, const struct hm_driver_ops *ops, void *ctx)
{
	struct udk_driver *udkdrv;
	UNUSED(ops, ctx);

	if ((drv == NULL) || (drv->libname == NULL)) {
		return -EINVAL;
	}
	udkdrv = container_of(drv, struct udk_driver, drv);

	return -hmerrno2posix(udk_fusion_driver_init(udkdrv));
}

int devhost_driver_add_compat(hm_driver_t *drv, const char *compat, size_t compat_len)
{
	struct udk_driver *udkdrv;

	if ((drv == NULL) || (compat == NULL)) {
		return -EINVAL;
	}
	udkdrv = container_of(drv, struct udk_driver, drv);

	return -hmerrno2posix(compatible_string_concat(&udkdrv->compat, compat, compat_len));
}

int devhost_driver_register(hm_driver_t *drv)
{
	struct udk_driver *udkdrv;

	if ((drv == NULL)) {
		return -EINVAL;
	}
	udkdrv = container_of(drv, struct udk_driver, drv);

	return -hmerrno2posix(udk_fusion_driver_register(udkdrv));
}

void devhost_drv_put(struct hm_driver *drv)
{
	/*
	 * Fusion udk doesn't support driver lifecycle management,
	 * so there is no need to do anything.
	 */
	UNUSED(drv);
}

void devhost_driver_destroy(hm_driver_t *drv)
{
	/*
	 * Fusion udk doesn't support to destroy drivers.
	 */
	UNUSED(drv);
}

int devhost_acquire_driver(const char *compat, hm_driver_t **pdrv)
{
	/*
	 * This function is used in udk_unregister_driver.
	 * Fusion udk doesn't support to unregister driver,
	 * so there is no need to do anything.
	 */
	UNUSED(compat, pdrv);

	return 0;
}

const struct devhost_backend_args *devhost_get_backend_args(void)
{
	return NULL;
}

void devhost_device_set_anonymous(struct device_info *info)
{
	info->p->flags |= DEVICE_ANONYMOUS;
}

int devhost_device_is_anonymous(const struct device_info *info)
{
	return ((info->p->flags & DEVICE_ANONYMOUS) != __U(0));
}

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
	if (devinfo->p->devt == udk_zerodev_devt()) {
		ret |= DH_IO_CTX_DEV_ZERO_FLAG;
	}

	return ret;
}

struct filp_node *devhost_alloc_filp_node_fast(struct device_info *info)
{
	return udk_fusion_filp_node_alloc(0U, info);
}
