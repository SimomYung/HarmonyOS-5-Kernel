/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Udmi sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 21 10:09:19 2020
 */
#ifndef DEVHOST_PLUGINS_UDMI_H
#define DEVHOST_PLUGINS_UDMI_H

#include <libhmactv/actv.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>

#include <devhost/device.h>
#include <devhost/file.h>

int hdr_devhost_core_load_driver(struct hm_actv_buf name,
				 struct hm_actv_buf args,
				 uint64_t drv_shm_key,
				 uint32_t owner_cnode_idx,
				 size_t size);
int hdr_devhost_core_unload_driver(struct hm_actv_buf drvname);

int hdr_devhost_core_create_device(struct hm_actv_buf devname,
				   struct hm_actv_buf drvname,
				   const struct device_create_args *args,
				   int *devid_out);
int hdr_devhost_core_bind_device(int devid,
				 struct hm_actv_buf drvname);

int hdr_devhost_fops_open(unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags,
			  struct __actvret_drvcall_devhost_fops_open *pret);
int hdr_devhost_fops_close(unsigned long long sender, unsigned long credential,
			   unsigned int fastpath, unsigned long long file_id);

void devhost_wait_register_device(void);

int hdr_devhost_fops_open_fast(struct device_info *devinfo,
			       unsigned int vfs_flags,
			       struct filp_node **fnode,
			       rref_t tgt_rref,
			       struct hmkobj_ctx *kobj_ctx);
int hdr_devhost_fops_close_fast(unsigned long long filp, unsigned int rpccnt);
#endif /* DEVHOST_PLUGINS_UDMI_H */
