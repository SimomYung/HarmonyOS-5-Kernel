/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Exported APIs of devhost
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include "types.h"

#include <vfs.h>
#include <devhost_api.h>
#include <lib/dlist.h>
#include <sys/mman.h>
#include <devhost/plugin/advice_ep.h>
#include <libalgo/idr.h>
#include <libhmpm/shutdown.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libdevhost/compatible.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhwsecurec/securec.h>

#include "udmi.h"
#include "devhost.h"
#include "devhost_bio.h"
#include <devhost/log.h>
#include <devhost/file.h>
#include "utils/utils.h"

static struct raw_mutex g_drivers_mutex = RAW_MUTEX_INITIALIZER;
/* This lock is to for g_devid_idr ops */
static struct raw_mutex __device_idr_mutex = RAW_MUTEX_INITIALIZER;
static DLIST_HEAD(g_drivers);
static DEFINE_IDR(g_devid_idr);

PUBLIC_SYMBOL
void devhost_devinfo_put(struct device_info *info)
{
	devinfo_put(info);
}

PUBLIC_SYMBOL
void devhost_drv_put(struct hm_driver *drv)
{
	driver_put(drv);
}

static void drv_uobject_release(struct uobject *uobj)
{
	struct driver_private *p = container_of(uobj, struct driver_private, obj);
	struct hm_driver *drv = p->drv;

	compatible_string_reset(&p->compat);
	free(p);
	drv->p = NULL;
	if (drv->release != NULL) {
		drv->release(drv);
	}
}

PUBLIC_SYMBOL
int devhost_driver_init(hm_driver_t *drv, const struct hm_driver_ops *ops, void *ctx)
{
	int ret = E_HM_OK;

	if (drv == NULL || drv->p != NULL || drv->libname == NULL) {
		return -EINVAL;
	}

	drv->p = (struct driver_private *)malloc(sizeof(struct driver_private));
	if (drv->p == NULL) {
		return -ENOMEM;
	}

	/* use libname as the first compatible string segment */
	ret = compatible_string_init_from_libname(&drv->p->compat, drv->libname);
	if (ret < 0) {
		free(drv->p);
		drv->p = NULL;
		return -hmerrno2posix(ret);
	}

	drv->p->ops = ops;
	drv->p->ctx = ctx;
	drv->p->drv = drv;
	dlist_init(&drv->p->node);
	drv->release = NULL;

	uobject_init(&drv->p->obj, 1, drv_uobject_release);

	return 0;
}

PUBLIC_SYMBOL
int devhost_driver_add_compat(hm_driver_t *drv, const char *compat, size_t compat_len)
{
	int ret = E_HM_OK;
	if (drv == NULL) {
		return -EINVAL;
	}
	ret = compatible_string_concat(&drv->p->compat, compat, compat_len);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

static void devhost_driver_unregister(hm_driver_t *drv)
{
	raw_mutex_lock(&g_drivers_mutex);
	dlist_delete(&drv->p->node);
	raw_mutex_unlock(&g_drivers_mutex);
	driver_put(drv);
}

PUBLIC_SYMBOL
void devhost_driver_destroy(hm_driver_t *drv)
{
	if (drv == NULL) {
		return;
	}
	devhost_driver_unregister(drv);
	driver_put(drv);
}

PUBLIC_SYMBOL
int devhost_driver_register(hm_driver_t *drv)
{
	struct driver_private *drv_p = NULL;

	if ((drv == NULL) || (drv->p == NULL)) {
		return -EINVAL;
	}

	BUG_ON(drv->p->compat.value == NULL);

	raw_mutex_lock(&g_drivers_mutex);
	dlist_for_each_entry(drv_p, &g_drivers, struct driver_private, node) {
		if ((strcmp(drv->libname, drv_p->drv->libname) == 0) ||
		    (compatible_string_compare(&drv_p->compat,
					       &drv->p->compat) != NULL)) {
			raw_mutex_unlock(&g_drivers_mutex);
			return -EEXIST;
		}
	}
	/* g_drivers hold drv`s refcnt */
	dlist_insert_tail(&g_drivers, &driver_get(drv)->p->node);
	raw_mutex_unlock(&g_drivers_mutex);

	dh_info("driver [%s] registered\n", drv->libname);
	return 0;
}

static void devhost_driver_release(hm_driver_t *drv)
{
	free(drv->libname);
	free(drv);
}

static hm_driver_t *devhost_driver_alloc(const char *libname,
					 const struct hm_driver_ops *ops,
					 void *ctx)
{
	hm_driver_t *drv = NULL;
	int ret = E_HM_OK;

	if (!is_valid_driver_name(libname)) {
		return NULL;
	}
	drv = (hm_driver_t *)malloc(sizeof(hm_driver_t));
	if (drv == NULL) {
		return NULL;
	}
	mem_zero_s(*drv);
	drv->libname = strndup(libname, DRIVER_NAME_LEN - 1);
	if (drv->libname == NULL) {
		free(drv);
		return NULL;
	}
	ret = devhost_driver_init(drv, ops, ctx);
	if (ret < 0) {
		free(drv->libname);
		free(drv);
		return NULL;
	}
	drv->release = devhost_driver_release;
	return drv;
}

PUBLIC_SYMBOL
int devhost_register_driver(const char *libname, const struct hm_driver_ops *ops, void *ctx)
{
	hm_driver_t *drv = NULL;
	int ret = E_HM_OK;

	if (!is_valid_driver_name(libname)) {
		return -EINVAL;
	}

	drv = devhost_driver_alloc(libname, ops, ctx);
	if (drv == NULL) {
		return -ENOMEM;
	}

	ret = devhost_driver_register(drv);
	if (ret < 0) {
		driver_put(drv);
		return ret;
	}

	return 0;
}

PUBLIC_SYMBOL
int devhost_acquire_driver(const char *compat, hm_driver_t **pdrv)
{
	struct driver_private *drv_p = NULL;
	struct compatible_string compat_str;
	int ret = E_HM_OK;

	if (compat == NULL) {
		return -ENOENT;
	}
	ret = compatible_string_init(&compat_str, compat, strlen(compat) + 1U);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	raw_mutex_lock(&g_drivers_mutex);
	dlist_for_each_entry(drv_p, &g_drivers, struct driver_private, node) {
		if (compatible_string_compare(&drv_p->compat, &compat_str) != NULL) {
			if (pdrv != NULL) {
				*pdrv = driver_get(drv_p->drv);
			}
			raw_mutex_unlock(&g_drivers_mutex);
			compatible_string_reset(&compat_str);
			return 0;
		}
	}
	raw_mutex_unlock(&g_drivers_mutex);
	compatible_string_reset(&compat_str);
	return -ENOENT;
}

static int __acquire_devnode(struct device_info *info,
			     char *node_name, size_t size, struct devnode_req *req)
{
	if (info->p->devt == 0) {
		return 0;
	}

	if (info->p->devnode_fn) {
		int ret = info->p->devnode_fn(info, node_name, (unsigned int)size, req);
		if (ret < 0) {
			return ret;
		}
		if (!is_valid_device_name(node_name)) {
			return -EINVAL;
		}
	} else {
		NOFAIL(strncpy_s(node_name, size, info->devname, strlen(info->devname)));
	}

	return (int)strlen(node_name) + 1;
}

static struct devmgr_device_data *__prepare_device_data(const struct devnode_req *req,
							const char *compat, size_t length)
{
	struct devmgr_device_data *data = NULL;
	int ret = 0;

	data = malloc(sizeof(*data) + length);
	if (data == NULL) {
		return NULL;
	}
	data->umode = req->mode;
	data->uid = req->uid;
	data->gid = req->gid;
	if (length != 0) {
		ret = memcpy_s(ptr_add(data, sizeof(*data)), length, compat, length);
		BUG_ON(ret != 0, "ret=%d\n", ret);
	}
	return data;
}

/*
 * register device info to sysmgr
 *
 * @param   info    [I] device info
 *
 * @return  0 on success or posix errno on error
 */
static int
__register_devmgr_device(struct device_info *info)
{
	struct __actvret_drvcall_devmgr_core_device_add drvcall_ret;
	struct hm_actv_buf devname;
	struct hm_actv_buf devnode_name;
	struct hm_actv_buf group;
	char node_name[DEVICE_NAME_LEN] = { 0 };
	struct devnode_req req = { 0 };
	struct devmgr_device_data *data = NULL;
	int ret = 0;

	ret = __acquire_devnode(info, node_name, sizeof(node_name), &req);
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
		hm_actv_buf_init(&devnode_name, node_name, (size_t)(unsigned int)ret);
	}

	hm_actv_buf_init(&devname, info->devname, strlen(info->devname) + 1U);

	if (____IS(info->p->flags & DEVICE_MUST_ISOLATE) &&
	    (info->p->isolate_group != NULL)) {
		hm_actv_buf_init(&group, info->p->isolate_group, strlen(info->p->isolate_group) + 1U);
	} else {
		hm_actv_buf_init(&group, NULL, 0);
	}

	data = __prepare_device_data(&req, info->p->compatible.value, info->p->compatible.length);
	if (data == NULL) {
		return -ENOMEM;
	}

	mem_zero_s(drvcall_ret);
	ret = actvcapcall_drvcall_devmgr_core_device_add(libdh_get_devmgr_ap(),
			devname, devnode_name,
			info->parent, info->p->flags,
			info->p->devt, info->p->vfs_mode,
			group, data,
			(unsigned int)(info->p->compatible.length + sizeof(*data)),
			&drvcall_ret);
	if (ret != E_HM_OK) {
		free(data);
		return -hmerrno2posix(ret);
	}
	info->devid = drvcall_ret.devid;
	info->p->flags |= drvcall_ret.dev_flags;
	free(data);

	return 0;
}

/* This lock is to block bind ops beforce device is ready in devhost_register_device */
static struct raw_mutex __device_reg_mutex = RAW_MUTEX_INITIALIZER;

static int __register_devhost_device(int devid, struct device_info *info);
PUBLIC_SYMBOL
int devhost_register_device(struct device_info *info)
{
	int ret = E_HM_OK;

	/* validate device_info */
	if ((info == NULL) || (info->devname == NULL)) {
		return -EINVAL;
	}

	if (info->p == NULL) {
		dh_error("must inited device info first\n");
		return -EINVAL;
	}

	if (!is_valid_device_name(info->devname)) {
		return -EINVAL;
	}

	if (info->devid != -1) {
		return -EINVAL;
	}

	if (____IS(info->p->flags & DEVICE_MUST_ISOLATE) &&
	    !is_valid_isolate_group_name(info->p->isolate_group)) {
		return -EINVAL;
	}

	/* extra init for block device, SHOULD move to other places */
	if (info->p->vfs_mode == (int)MODE_BLK) {
		info->p->bd_queue_info = malloc(sizeof(struct bd_queue_info));
		if (info->p->bd_queue_info == NULL) {
			return -ENOMEM;
		}
		raw_rwlock_init(&(info->p->bd_queue_info->bd_vfs_queue_rwlock),
				NULL);
		dlist_init(&(info->p->bd_queue_info->bd_vfs_queue_list));
	}

	/* register to devmgr, and record to internal device table */
	RAW_MUTEX_GUARD_START(_, &__device_reg_mutex);
	ret = __register_devmgr_device(info);
	if (ret < 0) {
		goto err_out;
	}
	ret = __register_devhost_device(info->devid, info);
	if (ret < 0) {
		/* need rollback device added to devmgr */
		(void)actvcapcall_drvcall_devmgr_core_device_del(libdh_get_devmgr_ap(),
								 info->devid);
		goto err_out;
	}
	RAW_MUTEX_GUARD_END(_, &__device_reg_mutex);

	return 0;

err_out:
	if ((info->p->vfs_mode == (int)MODE_BLK) && (info->p->bd_queue_info != NULL)) {
		free(info->p->bd_queue_info);
		info->p->bd_queue_info = NULL;
	}
	return ret;
}

void devhost_wait_register_device(void)
{
	raw_mutex_lock(&__device_reg_mutex);
	raw_mutex_unlock(&__device_reg_mutex);
}

static int __devhost_unregister_device(struct device_info *info)
{
	int ret = E_HM_OK;

	/* not care info`s refcnt, notify devmgr to delete device */
	ret = actvcapcall_drvcall_devmgr_core_device_del(
			libdh_get_devmgr_ap(),
			info->devid);
	if (ret != E_HM_OK) {
		dh_error("devmgr delete device failed, name=%s\n", info->devname);
		return ret;
	}

	RAW_MUTEX_GUARD(_, &__device_idr_mutex);
	(void)idr_remove(&g_devid_idr, (unsigned)info->devid);
	devinfo_put(info);

	return 0;
}

DEFINE_ADVICE_EP_PROTOTYPE(devhost_unregister_device, struct device_info *);
PUBLIC_SYMBOL
int devhost_unregister_device(struct device_info *info)
{
	int ret = E_HM_OK;

	if (info == NULL) {
		return -EINVAL;
	}

	DECLARE_ADVICE_EP(devhost_unregister_device, ret, __devhost_unregister_device, info);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

static int __devhost_insert_device(struct device_info *info, int devid)
{
	int ret = E_HM_OK;

	RAW_MUTEX_GUARD(_, &__device_idr_mutex);
	/* g_devid_idr hold devinfo`s refcnt */
	ret = idr_set(&g_devid_idr, devinfo_get(info), (unsigned)devid);
	if (ret < 0) {
		devinfo_put(info);
	}
	if (ret == E_HM_OBJEXIST) {
		ret = E_HM_POSIX_EXIST;
	}

	return ret;
}

DEFINE_ADVICE_EP_PROTOTYPE(devhost_register_device, struct device_info *, int);
static int __register_devhost_device(int devid, struct device_info *info)
{
	int ret = E_HM_OK;

	if ((info == NULL) || (info->devname == NULL)) {
		return -EINVAL;
	}

	if (info->p == NULL) {
		dh_error("must inited device info before create it\n");
		return -EINVAL;
	}

	DECLARE_ADVICE_EP(devhost_register_device, ret, __devhost_insert_device, info, devid);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

PUBLIC_SYMBOL
int devhost_acquire_device(int devid, struct device_info **pinfo)
{
	void *ptr = NULL;

	if (pinfo == NULL) {
		return -EINVAL;
	}

	RAW_MUTEX_GUARD(_, &__device_idr_mutex);
	ptr = idr_find(&g_devid_idr, (unsigned)devid);
	if (ptr == NULL) {
		return -ENOENT;
	}

	/* transfer refcnt to caller */
	*pinfo = devinfo_get((struct device_info *)ptr);

	return 0;
}

PUBLIC_SYMBOL
struct device_info *devinfo_get(struct device_info *devinfo)
{
	(void)uobject_get(&devinfo->p->obj);
	return devinfo;
}

PUBLIC_SYMBOL
void devinfo_put(struct device_info *devinfo)
{
	(void)uobject_put(&devinfo->p->obj);
}
