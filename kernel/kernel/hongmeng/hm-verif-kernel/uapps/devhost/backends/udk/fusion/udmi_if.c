/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK device sysif
 * Author: Huawei OS Kernel Lab
 * Create: Thur Jul 6 10:03:27 2023
 */

#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>

#include "device.h"
#include "fusion.h"

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_core_create_device,
		    unsigned long long, sender, unsigned long, credential,
		    struct hm_actv_buf, devname, struct hm_actv_buf, dev_compat,
		    int, parent_devid, unsigned int, resource_count)
{
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct __actvret_drvcall_devhost_core_create_device *pret =
		actvhdlr_drvhandler_devhost_core_create_device_prepare_ret(sender,
									   credential);
	struct device_create_args args;

	BUG_ON(pret == NULL);
	if (!udk_fusion_cnode_is_devmgr()) {
		return E_HM_PERM;
	}
	mem_zero_s(args);
	args.resource = recv.data;
	args.resource_size = recv.size;
	args.parent_devid = parent_devid;

	if (args.resource_size != sizeof(uint64_t) * resource_count) {
		return E_HM_INVAL;
	}

	return udk_fusion_hdr_device_create(devname, dev_compat, &args, &pret->devid);
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_core_bind_device,
		    unsigned long long, sender, unsigned long, credential,
		    int, devid, struct hm_actv_buf, dev_compat)
{
	UNUSED(sender, credential);
	if (!udk_fusion_cnode_is_devmgr()) {
		return E_HM_PERM;
	}

	return udk_fusion_hdr_device_bind(devid, dev_compat);
}
