/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of proc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 27 14:26:43 2020
 */

#include <vfs.h>

#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmlog/hmlog.h>
#include <libmem/utils.h>
#include <libhmsync/raw_rwlock.h>

#include <fd.h>
#include <fslib_common.h>

#include "internal.h"

/* Only sysmgr can call this function */
int vfs_revoke_process(rref_t rref, uint32_t cnode_idx)
{
	int err;

	err = actvxactcapcall_fscall_vfs_revoke_process(false, true, rref, cnode_idx);
	if (err < 0) {
		hm_warn("FS revoke_process failed %s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

/* Only sysmgr can call this function */
int vfs_setrlimit(uint32_t cnode_idx, unsigned int type,
		  rlim_t rlim_cur, rlim_t rlim_max)
{
	int err;

	err = actvcap_fscall(vfs_setrlimit, cnode_idx, rlim_cur, rlim_max, type);
	if (err < 0) {
		hm_debug("FS setrlimit failed %s\n", hmstrerror(err));
	}

	return err;
}
