/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for core sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include "udmi.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <libmem/utils.h> /* for PAGE_ALIGN_UP() */
#include <hmkernel/errno.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>
#include "devhost_loader.h"
#include "utils/utils.h"

int hdr_devhost_core_load_driver(struct hm_actv_buf name,
				 struct hm_actv_buf args,
				 uint64_t drv_shm_key,
				 uint32_t owner_cnode_idx,
				 size_t size)
{
	char libname[DRIVER_NAME_LEN];
	char drvargs[DRIVER_ARGS_LEN];
	void *data = NULL;
	size_t shm_rlen;
	int shm_id;
	int ret = E_HM_OK;

	if (name.size == 0U) {
		return E_HM_INVAL;
	}

	ret = hm_actv_read_caller_buf(libname, sizeof(libname), &name);
	if (ret != 0) {
		return E_HM_INVAL;
	}
	if (!is_valid_driver_name(libname)) {
		return E_HM_INVAL;
	}
	dh_info("load driver [%s]\n", libname);

	ret = hm_actv_read_caller_buf(drvargs, sizeof(drvargs), &args);
	if (ret != 0) {
		return E_HM_INVAL;
	}

	shm_id = hm_ashm_open_auth(drv_shm_key, owner_cnode_idx, 0, SHM_O_RDWR, &shm_rlen);
	if (shm_id < 0) {
		ret = shm_id;
		goto err_shm_open;
	}

	data = hm_mem_mmap_posix(NULL, shm_rlen, MPROT_READ | MPROT_WRITE,
				 MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
				 shm_id, 0, &ret);
	if (ret < 0) {
		goto err_mem_mmap;
	}
	/* load driver modules */
	ret = devhost_load_module(libname, data, size, drvargs);
	if (ret == E_HM_POSIX_EXIST) {
		goto err_init_module;
	}

	if (ret < 0) {
		dh_error("init module failed, libname=%s, err=%s\n",
			 libname, hmstrerror(ret));
		goto err_init_module;
	}

err_init_module:
	(void)hm_mem_munmap(data, shm_rlen);
err_mem_mmap:
	(void)hm_ashm_close(shm_id);
err_shm_open:
	return ret;
}

int hdr_devhost_core_unload_driver(struct hm_actv_buf drvname)
{
	char driver_name[DRIVER_NAME_LEN];
	int ret = E_HM_OK;

	if (drvname.size == 0U) {
		return E_HM_INVAL;
	}

	ret = hm_actv_read_caller_buf(driver_name, sizeof(driver_name), &drvname);
	if (ret < 0) {
		return ret;
	}
	if (!is_valid_driver_name(driver_name)) {
		return E_HM_INVAL;
	}
	dh_info("unload driver [%s]\n", driver_name);

	/* unload driver modules */
	ret = devhost_unload_module(driver_name);
	if (ret < 0) {
		dh_error("unload module failed, libname=%s, err=%s\n",
			 driver_name, hmstrerror(ret));
		return ret;
	}

	return 0;
}

int hdr_devhost_core_create_device(struct hm_actv_buf devname,
				   struct hm_actv_buf dev_compat,
				   const struct device_create_args *args,
				   int *devid_out)
{
	hm_driver_t *drv = NULL;
	char device_name[DEVICE_NAME_LEN] = { 0 };
	char compat[DRIVER_NAME_LEN] = { 0 };
	struct device_info *info = NULL;
	int ret = E_HM_OK;

	if (devname.size == 0U) {
		return E_HM_INVAL;
	}

	ret = hm_actv_read_caller_buf(device_name, sizeof(device_name), &devname);
	if (ret != 0) {
		return E_HM_INVAL;
	}
	ret = hm_actv_read_caller_buf(compat, sizeof(compat), &dev_compat);
	if (ret != 0) {
		return E_HM_INVAL;
	}
	if (!is_valid_device_name(device_name)) {
		return E_HM_INVAL;
	}

	if ((ret = devhost_acquire_driver(compat, &drv)) < 0) {
		return posix2hmerrno(-ret);
	}
	if (!drv->p->ops->create) {
		driver_put(drv);
		return E_HM_NOSYS;
	}
	if ((ret = drv->p->ops->create(drv->p->ctx, device_name,
				       args, &info)) < 0) {
		driver_put(drv);
		return posix2hmerrno(-ret);
	}
	driver_put(drv);

	if (devid_out != NULL) {
		*devid_out = info->devid;
	}

	dh_info("device created, device=%s, id=%d\n", info->devname, info->devid);

	return E_HM_OK;
}

int hdr_devhost_core_bind_device(int devid,
				 struct hm_actv_buf dev_compat)
{
	hm_driver_t *drv = NULL;
	struct device_info *info = NULL;
	char compat[DRIVER_NAME_LEN];
	int ret = E_HM_OK;

	ret = hm_actv_read_caller_buf(compat, sizeof(compat), &dev_compat);
	if (ret != 0) {
		return E_HM_INVAL;
	}
	devhost_wait_register_device();
	if ((ret = devhost_acquire_device(devid, &info)) < 0) {
		return posix2hmerrno(-ret);
	}
	/* bind to selected driver */
	if ((ret = devhost_acquire_driver(compat, &drv)) < 0) {
		devinfo_put(info);
		return posix2hmerrno(-ret);
	}
	if (!drv->p->ops->bind) {
		driver_put(drv);
		devinfo_put(info);
		return E_HM_NOSYS;
	}
	if ((ret = drv->p->ops->bind(drv->p->ctx, info)) < 0) {
		driver_put(drv);
		devinfo_put(info);
		return posix2hmerrno(-ret);
	}
	driver_put(drv);
	devinfo_put(info);

	return E_HM_OK;
}
