/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK device & driver management
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#include <string.h>
#include <hongmeng/errno.h>
#include <libalgo/idr.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/devhost.h>
#include <libhmsrv_io/type.h>

#include <devhost/device.h>
#include <udk/log.h>
#include <udk/udk_iommu.h>

#include "device.h"
#include "fusion.h"

static DEFINE_IDR(g_devid_idr);
static DLIST_HEAD(udk_drivers);
static struct raw_mutex udk_drivers_mutex = RAW_MUTEX_INITIALIZER;
/* to protect bind operation, make sure that device is ready */
static struct raw_mutex udk_device_reg_mutex = RAW_MUTEX_INITIALIZER;
static struct raw_rwlock g_fusion_dev_lock = RAW_RDLOCK_INITIALIZER;

static DEFINE_IDR(g_devt_idr);

static inline void udk_fusion_dev_wrlock(void)
{
	(void)raw_rwlock_wrlock(&g_fusion_dev_lock);
}

static inline void udk_fusion_dev_rdlock(void)
{
	(void)raw_rwlock_rdlock(&g_fusion_dev_lock);
}

static inline void udk_fusion_dev_unlock(void)
{
	(void)raw_rwlock_unlock(&g_fusion_dev_lock);
}

static void __udk_fusion_wait_devie_ready(void)
{
	raw_mutex_lock(&udk_device_reg_mutex);
	raw_mutex_unlock(&udk_device_reg_mutex);
}

int udk_fusion_device_acquire(int devid, struct device_info **info)
{
	void *ptr = NULL;

	if (info == NULL) {
		return E_HM_INVAL;
	}

	udk_fusion_dev_rdlock();
	ptr = idr_find(&g_devid_idr, (unsigned)devid);
	if (ptr == NULL) {
		udk_fusion_dev_unlock();
		return E_HM_NOOBJ;
	}
	*info = ptr;
	(void)udk_fusion_devinfo_get(ptr);
	udk_fusion_dev_unlock();

	return 0;
}

static int __acquire_devnode(struct device_info *info,
			     char *name, size_t size, struct devnode_req *req)
{
	int err = 0;

	if (info->p->devt == 0) {
		return 0;
	}

	if (info->p->devnode_fn) {
		err = info->p->devnode_fn(info, name, (unsigned int)size, req);
		if (err < 0) {
			return err;
		}
		if (!is_valid_device_name(name)) {
			return -EINVAL;
		}
	} else {
		NOFAIL(strncpy_s(name, size, info->devname, strlen(info->devname)));
	}

	return (int)strlen(name) + 1;
}

static int __register_device_to_devmgr(struct device_info *info)
{
	struct __actvret_drvcall_devmgr_core_device_add drvcall_ret;
	struct hm_actv_buf devname;
	struct hm_actv_buf devnode_name;
	struct hm_actv_buf group;
	struct devmgr_device_data *data = NULL;
	char name[DEVICE_NAME_LEN] = { 0 };
	struct devnode_req req = { 0 };
	size_t compat_len = info->p->compatible.length;
	size_t data_len = sizeof(*data) + compat_len;
	int ret = 0;

	ret = __acquire_devnode(info, name, sizeof(name), &req);
	if (ret < 0) {
		return ret;
	}

	if (req.mode == 0) {
		/* keep consistent with linux */
		req.mode = S_IRUSR | S_IWUSR;
	}

	if (ret == 0) {
		hm_actv_buf_init(&devnode_name, NULL, 0);
	} else {
		hm_actv_buf_init(&devnode_name, name, (size_t)(unsigned int)ret);
	}
	hm_actv_buf_init(&devname, info->devname, strlen(info->devname) + 1U);
	hm_actv_buf_init(&group, NULL, 0);
	if (____IS(info->p->flags & DEVICE_MUST_ISOLATE)) {
		hm_actv_buf_init(&group, UDK_DEVHOST_NAME, strlen(UDK_DEVHOST_NAME) + 1U);
	}
	if ((data = malloc(data_len)) == NULL) {
		return E_HM_NOMEM;
	}
	mem_zero_p(data, data_len, data_len);
	data->umode = req.mode;
	data->uid = req.uid;
	data->gid = req.gid;

	if (compat_len > 0) {
		NOFAIL(memcpy_s(ptr_add(data, sizeof(*data)), compat_len,
				info->p->compatible.value, compat_len));
	}

	mem_zero_s(drvcall_ret);
	ret = actvcapcall_drvcall_devmgr_core_device_add(libdh_get_devmgr_ap(),
			devname, devnode_name, info->parent, info->p->flags,
			info->p->devt, info->p->vfs_mode, group, data,
			(unsigned int)data_len, &drvcall_ret);
	free(data);
	if (ret == 0) {
		info->devid = drvcall_ret.devid;
		info->p->flags |= drvcall_ret.dev_flags;
	}

	return ret;
}

int udk_fusion_devid_acquire(uint32_t devt, int *devid)
{
	void *ptr = NULL;

	if ((MAJOR(devt) == 0) || (devid == NULL)) {
		return E_HM_INVAL;
	}

	ptr = idr_find(&g_devt_idr, devt);
	if (ptr == NULL) {
		return E_HM_NOOBJ;
	}
	*devid = (int)(uintptr_t)ptr;

	return E_HM_OK;
}

static int create_devt_devid_map(uint32_t devt, int devid)
{
	int ret = E_HM_OK;

	if (MAJOR(devt) != 0) {
		ret = idr_set(&g_devt_idr, (void *)(uintptr_t)devid, devt);
	}

	return ret;
}

static inline int create_devid_devinfo_map(struct device_info *info)
{
	return idr_set(&g_devid_idr, info, (unsigned)info->devid);
}

static int __insert_device(struct device_info *info)
{
	int ret = E_HM_OK;

	/* g_devid_idr hold devinfo`s refcnt */
	(void)udk_fusion_devinfo_get(info);
	ret = create_devid_devinfo_map(info);
	if (ret < 0) {
		(void)udk_fusion_devinfo_put(info);
		return ret;
	}
	if (info->p->vfs_mode == (int)MODE_CHR) {
		ret = create_devt_devid_map(info->p->devt, info->devid);
		if (ret < 0) {
			(void)udk_fusion_devinfo_put(info);
			return ret;
		}
	}
	return 0;
}

static inline int __unregister_device_from_devmgr(struct device_info *info)
{
	int ret;

	ret = actvcapcall_drvcall_devmgr_core_device_del(
			libdh_get_devmgr_ap(),
			info->devid);
	if (ret != E_HM_OK) {
		udk_error("failed to unregister device, name=%s\n", info->devname);
	}

	return ret;
}

static inline void __delete_device(struct device_info *info)
{
	udk_fusion_dev_wrlock();
	(void)idr_remove(&g_devid_idr, (unsigned)info->devid);
	if (info->p->vfs_mode == (int)MODE_CHR) {
		(void)idr_remove(&g_devt_idr, (unsigned)info->p->devt);
	}
	udk_fusion_dev_unlock();
	udk_fusion_devinfo_put(info);
}

int udk_fusion_device_register(struct device_info *info)
{
	int ret = E_HM_INVAL;

	/* validate device_info */
	if ((info == NULL) || (info->devname == NULL) || (info->p == NULL)) {
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &udk_device_reg_mutex);
	ret = __register_device_to_devmgr(info);
	if (ret < 0) {
		udk_warn("register device to devmgr failed\n");
		return ret;
	}
	ret = __insert_device(info);
	if (ret < 0) {
		(void)__unregister_device_from_devmgr(info);
		udk_warn("insert device info to idr map failed\n");
	}

	return ret;
}

int udk_fusion_device_unregister(struct device_info *info)
{
	int ret = E_HM_OK;

	if (info == NULL) {
		return -EINVAL;
	}

	ret = __unregister_device_from_devmgr(info);
	if (ret < 0) {
		return ret;
	}
	__delete_device(info);

	return ret;
}

int udk_fusion_hdr_device_create(struct hm_actv_buf devname,
				 struct hm_actv_buf dev_compat,
				 const struct device_create_args *args,
				 int *devid_out)
{
	char device_name[DEVICE_NAME_LEN] = { 0 };
	char compat[DRIVER_NAME_LEN] = { 0 };
	struct device_info *info;
	struct udk_driver *udkdrv;
	int ret;

	if ((args == NULL) || (devid_out == NULL)) {
		return E_HM_INVAL;
	}
	if ((ret = hm_actv_read_caller_buf(device_name, sizeof(device_name), &devname)) < 0) {
		return ret;
	}
	if ((ret = hm_actv_read_caller_buf(compat, sizeof(compat), &dev_compat)) < 0) {
		return ret;
	}
	if (!is_valid_device_name(device_name)) {
		return E_HM_INVAL;
	}

	if ((ret = udk_fusion_driver_acquire(compat, &udkdrv)) < 0) {
		return ret;
	}
	if (udkdrv->drv_ops->create == NULL) {
		return E_HM_NOSYS;
	}
	if ((ret = udkdrv->drv_ops->create(udkdrv->ctx, device_name,
					   args, &info)) < 0) {
		return posix2hmerrno(-ret);
	}

	*devid_out = info->devid;
	udk_info("device created, device=%s, id=%d\n", info->devname, info->devid);

	return 0;
}

int udk_fusion_hdr_device_bind(int devid, struct hm_actv_buf dev_compat)
{
	struct udk_driver *udkdrv = NULL;
	struct device_info *info;
	int ret = E_HM_OK;

	UNUSED(dev_compat);

	__udk_fusion_wait_devie_ready();
	if ((ret = udk_fusion_device_acquire(devid, &info)) < 0) {
		return ret;
	}
	/* devmgr bind callback, compatiable should not be NULL. */
	BUG_ON(info->p->compatible.value == NULL);
	if ((ret = udk_fusion_driver_acquire(info->p->compatible.value, &udkdrv)) < 0) {
		udk_fusion_devinfo_put(info);
		return ret;
	}
	if (udkdrv->drv_ops->bind == NULL) {
		udk_fusion_devinfo_put(info);
		return E_HM_NOSYS;
	}

	udk_dev_iommu_configure(info);
	if ((ret = udkdrv->drv_ops->bind(udkdrv->ctx, info)) < 0) {
		udk_fusion_devinfo_put(info);
		return posix2hmerrno(-ret);
	}
	udk_fusion_devinfo_put(info);

	return 0;
}

int udk_fusion_driver_init(struct udk_driver *udkdrv)
{
	int ret;

	/* use libname as the first compatible string segment */
	ret = compatible_string_init_from_libname(&udkdrv->compat, udkdrv->drv.libname);
	if (ret < 0) {
		return ret;
	}
	dlist_init(&udkdrv->node);

	return ret;
}

int udk_fusion_driver_register(struct udk_driver *udkdrv)
{
	struct udk_driver *__udkdrv = NULL;

	BUG_ON(udkdrv->compat.value == NULL);

	raw_mutex_lock(&udk_drivers_mutex);
	dlist_for_each_entry(__udkdrv, &udk_drivers, struct udk_driver, node) {
		if ((strcmp(udkdrv->drv.libname, __udkdrv->drv.libname) == 0) ||
		    (compatible_string_compare(&udkdrv->compat,
					       &__udkdrv->compat) != NULL)) {
			raw_mutex_unlock(&udk_drivers_mutex);
			return E_HM_OBJEXIST;
		}
	}
	dlist_insert_tail(&udk_drivers, &udkdrv->node);
	raw_mutex_unlock(&udk_drivers_mutex);

	return 0;
}

int udk_fusion_driver_acquire(const char *compat, struct udk_driver **udkdrv)
{
	struct udk_driver *__udkdrv = NULL;
	struct compatible_string compat_str;
	int ret;

	if (compat == NULL) {
		return E_HM_NOOBJ;
	}
	if (udkdrv == NULL) {
		return E_HM_INVAL;
	}

	ret = compatible_string_init(&compat_str, compat, strlen(compat) + 1U);
	if (ret < 0) {
		return ret;
	}

	raw_mutex_lock(&udk_drivers_mutex);
	dlist_for_each_entry(__udkdrv, &udk_drivers, struct udk_driver, node) {
		if (compatible_string_compare(&__udkdrv->compat, &compat_str) != NULL) {
			*udkdrv = __udkdrv;
			break;
		}
	}
	raw_mutex_unlock(&udk_drivers_mutex);
	compatible_string_reset(&compat_str);

	return (*udkdrv == NULL) ? E_HM_NOOBJ : 0;
}

struct device_info *udk_fusion_devinfo_get(struct device_info *devinfo)
{
	(void)uobject_get(&devinfo->p->obj);
	return devinfo;
}

void udk_fusion_devinfo_put(struct device_info *devinfo)
{
	(void)uobject_put(&devinfo->p->obj);
}
