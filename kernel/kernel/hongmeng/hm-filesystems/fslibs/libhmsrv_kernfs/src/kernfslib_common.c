/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of kernfslib common functions
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 13:35:20 2020
 */

#include "kernfslib_common.h"

#include <vfs.h>
#include <unistd.h>
#include <inttypes.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/atomic.h>
#include <libhmsrv_sys/hm_path.h>

raw_static_assert(sizeof(rref_t) == sizeof(uint64_t),
		  Atomic_cmpxchg_size_not_match_rref);
int kernfslib_get_rref(const char *path, rref_t *fs_rref)
{
	struct sysarg_cnode_statrref statr;
	rref_t rref = 0;
	rref_t old;
	int err;
	int err2;

	if (*fs_rref != 0) {
		return E_HM_OK;
	}

	err = hm_path_acquire(path, &rref);
	if (err < 0) {
		hm_debug("acquire rref failed, path=%s, err=%s\n",
			 path, hmstrerror(err));
		return err;
	}

	mem_zero_s(statr);
	err = sysfast_cnode_stat_rref(&statr, rref,
				      CNODE_STATRREF_SRC_CNODE_IDX |
				      CNODE_STATRREF_USLOT_IDX);
	if (err < 0) {
		hm_debug("statrref failed, err=%s\n", hmstrerror(err));
		goto err_out;
	}

	err = actvcapcall_fscall_vfs_client_init(rref, 0, NULL, NULL, 0);
	if (err < 0) {
		hm_debug("client_init failed, err=%s\n", hmstrerror(err));
		goto err_out;
	}

	old = raw_atomic_cmpxchg64(fs_rref, 0, rref);
	if (old != 0) {
		err = hm_path_release(path, rref);
		if (err != E_HM_OK) {
			hm_warn("release rref failed, rc=%s\n",
				hmstrerror(err));
		}
	}
	return E_HM_OK;

err_out:
	err2 = hm_path_release(path, rref);
	if (err2 != E_HM_OK) {
		hm_warn("release rref failed, err=%s\n", hmstrerror(err2));
	}

	return err;
}
