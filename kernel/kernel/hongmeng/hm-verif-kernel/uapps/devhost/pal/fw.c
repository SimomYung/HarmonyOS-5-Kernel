/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: PAL implement for firmware operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 19:27:55 2020
 */
#include <devhost/backend.h>

#include <errno.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libsysif/devmgr/api.h>
#include <libhmactv/actv.h>

#include "../devhost.h"
#include <devhost/log.h>

int devhost_pal_fw_acquire_contents(const char *filename, void *data,
				    unsigned int size, unsigned int *rsize)
{
	int ret = E_HM_OK;
	struct __actvret_drvcall_devmgr_fw_acquire_contents fret;

	if ((filename == NULL) || (filename[0] == '\0') || (rsize == NULL)) {
		return -EINVAL;
	}
	if (data == NULL && size != 0) {
		return -EINVAL;
	}

	mem_zero_s(fret);
	hm_actv_set_accessible_mem_range(ptr_to_ulong(data), (unsigned long)size);
	ret = actvxactcapcall_drvcall_devmgr_fw_acquire_contents(false, true, libdh_get_devmgr_ap(),
								 ptr_to_ulong(data), size,
								 ptr_to_ulong(filename),
								 (unsigned int)strlen(filename),
								 &fret);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	*rsize = fret.rsize;

	return 0;
}
