/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Plugin of udmi
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 21 10:09:19 2020
 */
#include <devhost/plugin.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/server.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmucap/ucap.h>
#include <libsysif/utils.h>

#include "devhost.h"
#include <devhost/log.h>
#include "devhost_loader.h"

#include "udmi.h"

static int dh_udmi_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret;
	UNUSED(plugin);

	/* initialize devhost file mapping */
	ret = devhost_file_mapping_init();
	if (ret < 0) {
		dh_error("setup file mapping failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	devhost_loader_init();

	return E_HM_OK;
}

/* core */
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_core_load_driver, dh_core_load_driver_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, name, struct hm_actv_buf, args,
			 unsigned long long, drv_shm_key, unsigned long, size
			)
{
	UNUSED(credential);

	return hdr_devhost_core_load_driver(name, args, drv_shm_key, __RPC_SENDER_DECODE_SRC_CIDX(sender),
					    size);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_core_unload_driver, dh_core_unload_driver_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, drvname
			)
{
	UNUSED(sender, credential);
	return hdr_devhost_core_unload_driver(drvname);
}

static
int __prepare_resource(struct device_create_args *args,
		       unsigned int resource_count,
		       const void *data, unsigned long size)
{
	if (resource_count == 0) {
		return E_HM_OK;
	}

	if (resource_count > RESOURCE_COUNT_MAX ||
	    size != sizeof(uint64_t) * resource_count) {
		return E_HM_INVAL;
	}

	args->resource = data;
	args->resource_size = size;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_core_create_device, dh_core_create_device_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, devname, struct hm_actv_buf, dev_compat,
			 int, parent_devid, unsigned int, resource_count
			)
{
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct __actvret_drvcall_devhost_core_create_device *pret =
		actvhdlr_drvhandler_devhost_core_create_device_prepare_ret(sender,
									   credential);
	BUG_ON_D(pret == NULL);
	struct device_create_args args;
	int ret;

	mem_zero_s(args);
	ret = __prepare_resource(&args, resource_count, recv.data, recv.size);
	if (ret < 0) {
		return ret;
	}
	args.parent_devid = parent_devid;

	return hdr_devhost_core_create_device(devname, dev_compat, &args,
					      &pret->devid);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_core_bind_device, dh_core_bind_device_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, devid, struct hm_actv_buf, dev_compat
			)
{
	UNUSED(sender, credential);
	return hdr_devhost_core_bind_device(devid, dev_compat);
}

/* fops */
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_open, dh_fops_open_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, u_cidx, int, devid, rref_t, tgt_rref, unsigned int, vfs_flags
			)
{
	struct __actvret_drvcall_devhost_fops_open *pret =
		actvhdlr_drvhandler_devhost_fops_open_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);

	return hdr_devhost_fops_open(u_cidx, devid, tgt_rref, vfs_flags, pret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_close, dh_fops_close_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id
			)
{
	return hdr_devhost_fops_close(sender, credential, fastpath, file_id);
}

DEFINE_UPCALL_ACTVHDLR_ALS(handle_rpccnt_put)
{
	int ret;
	struct filp_node *node = NULL;
	uintptr_t common_file = args.rpccnt.common_file;

	if (is_common_file_overflow(common_file)) {
		common_file = clear_common_file_overflow(common_file);
		node = (struct filp_node *)common_file;
		NOFAIL(raw_compounded_rpccnt_balance(&node->refcnt.refcnt_nr,
						     MAX_FD_RPCCNT, NULL));
		ret = __ACTV_WFERET_RPCCNT_PUT_REWIND;
	} else {
		node = (struct filp_node *)common_file;
		devhost_put_file_rpccnt(node);
		ret = __ACTV_WFERET_RPCCNT_PUT_RET;
	}

	return ret;
}

/*
 * udmi = Unified&Userspace Device Model Instrumentation.
 * This is core module for devhost. It defines the unified base behaviour of device & driver
 * and implement the core functions -- `load & unload & open & close` of device.
 * Also it is middle layer between the driver of backend and the user.
 *
 * extension point:
 *  - advice-ep: `devhost_register_device`
 * 		 Record device information to devhost.
 *  - advice-ep: `devhost_unregister_device`
 * 		 Remove device information from devmgr and devhost, delete device node.
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_core_load_driver`
 *  - target sysif-ep: `drvhandler_devhost_core_unload_driver`
 *  - target sysif-ep: `drvhandler_devhost_core_create_device`
 *  - target sysif-ep: `drvhandler_devhost_core_bind_device`
 *  - target sysif-ep: `drvhandler_devhost_fops_open`
 *  - target sysif-ep: `drvhandler_devhost_fops_close`
 *
 * export symbol:
 *  - func: `devhost_register_loader`
 *  - func: `acquire_filp_node_ptr_ref`
 *  - func: `devhost_devinfo_put`
 *  - func: `devhost_drv_put`
 *  - func: `devhost_register_driver`
 *  - func: `devhost_acquire_driver`
 *  - func: `devhost_register_device`
 *  - func: `devhost_unregister_device`
 *  - func: `devhost_acquire_device`
 *  - func: `devhost_driver_register`
 *  - func: `devhost_driver_destroy`
 *  - func: `devhost_driver_add_compat`
 *  - func: `devhost_driver_init`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(udmi, dh_udmi_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_core_load_driver, dh_core_load_driver_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_core_unload_driver, dh_core_unload_driver_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_core_create_device, dh_core_create_device_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_core_bind_device, dh_core_bind_device_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_open, dh_fops_open_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_close, dh_fops_close_ext)
);
