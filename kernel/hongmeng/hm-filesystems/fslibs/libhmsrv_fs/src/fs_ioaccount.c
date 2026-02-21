/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of fs ioaccount
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 23:07:01 2020
 */

#include <fs_ioaccount.h>

#include <libsysif/sysmgr/api.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/version.h>
#include <libhmlog/hmlog.h>

#include <vfs.h>
#include <fd.h>

int fs_get_pid_io_stats(uint32_t pid, struct taskstats *stats)
{
	int err;
	rref_t fsmgr_rref;
	struct __actvret_fscall_vfs_pid_io_stats ret = {0};

	if (stats == NULL) {
		hm_debug("stats was null.\n");
		return E_HM_INVAL;
	}

	if (memset_s(stats, sizeof(struct taskstats), 0, sizeof(struct taskstats)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	err = hm_path_acquire(FSMGR_PATH_ACTV, &fsmgr_rref);
	if (err != E_HM_OK) {
		hm_warn("get fsmgr rref failed, err=%s\n", hmstrerror(err));
		return err;
	}
	err = actvcapcall_fscall_vfs_pid_io_stats(fsmgr_rref, pid, &ret);
	(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
	switch (err) {
	case IO_DATA_OK:
		stats->blkio_delay_total = ret.io_ac.blkio_delay_total;
		stats->read_bytes = ret.io_ac.read_bytes;
		stats->write_bytes = ret.io_ac.write_bytes;
		break;
	case IO_NOT_SUP:
		hm_warn("hm version is not support\n");
		return E_HM_INVAL;
	default:
		stats->blkio_delay_total = 1;
	}
	stats->version = IOACCOUNT_VERSION;

	return E_HM_OK;
}
