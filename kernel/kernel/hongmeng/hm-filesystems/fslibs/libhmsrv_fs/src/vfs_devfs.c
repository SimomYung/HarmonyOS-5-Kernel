/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Support devfs remove kshm
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 05 14:18:48 2023
 */
#include <vfs_devfs.h>
#include <vfs.h>

int vfs_devfs_remove_kshm(unsigned long long kshm_key)
{
	int err;
	struct server_info *server = NULL;

	err = get_fsmgr_si(&server);
	if (err != E_HM_OK) {
		return err;
	}

	err = actvx_fscall(vfs_devfs_remove_kshm, server, false, true, kshm_key);
	put_server_info(server);
	if (err != E_HM_OK) {
		hm_error("fscall devfs remove kshm failed, err=%s\n", hmstrerror(err));
	}

	return err;
}
