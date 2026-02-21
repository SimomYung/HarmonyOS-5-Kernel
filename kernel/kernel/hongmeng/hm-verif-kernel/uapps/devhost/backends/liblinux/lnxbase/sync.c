/*
* Copyright (c) Huawei Technologies Co., Ltd. 2022- All rights reserved.
* Description: Implement
* Author: Huawei OS Kernel Lab
* Create: Thu Mar 17 17:26:07 2022
*/
#include <devhost/log.h>
#include <lnxbase/lnxbase.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/devhost.h>

#include "../../../utils/utils.h"

int lnxbase_dev_synchronize(void)
{
	int err;

	err = actvcapcall_drvcall_devmgr_mgmt_sync(libdh_get_devmgr_ap());
	if (err < 0) {
		dh_error("cpustat failed err: %s\n", hmstrerror(err));
	}

	return err;
}

void lnxbase_invoke_on_cpu(unsigned int cpu,
			   void (*callback)(void *), void *data)
{
	devhost_smpcall_wq_schedule(cpu, callback, data);
}
