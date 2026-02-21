/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: PAL implement for sd index operations
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 9 15:08:18 2024
 */
#include <libsysif/devmgr/api.h>
#include <devhost/log.h>
#include <libdevhost/devhost.h>
#include <liblinux/pal.h>

static int devhost_mgmt_device_index(unsigned int type, unsigned int cmd, int index)
{
	int err;
	struct __actvret_drvcall_devmgr_device_mgmt_index ret;

	mem_zero_s(ret);
	err = actvcapcall_drvcall_devmgr_device_mgmt_index(libdh_get_devmgr_ap(), type, cmd, index, &ret);
	if (err < 0) {
		dh_error("devhost type-%u cmd-%u index-%d failed, err=%s\n", type, cmd, index, hmstrerror(err));
		return -hmerrno2posix(err);
	}

	return ret.index;
}

int liblinux_pal_sd_ida_alloc(void)
{
	return devhost_mgmt_device_index(DEVMGR_DEVICE_ID_SD, DEVMGR_DEVICE_ID_ALLOC, 0);
}

void liblinux_pal_sd_ida_free(int sd_index)
{
	(void)devhost_mgmt_device_index(DEVMGR_DEVICE_ID_SD, DEVMGR_DEVICE_ID_FREE, sd_index);
}
