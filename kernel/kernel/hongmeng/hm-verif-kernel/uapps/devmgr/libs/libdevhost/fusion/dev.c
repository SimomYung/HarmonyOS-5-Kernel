/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: functions for libdh IPC to selfcall
 * Author: Huawei OS Kernel Lab
 * Create: Thur Nov 30 10:24:38 2023
 */
#include <vfs.h>
#include <libhmlog/hmlog.h>
#include <libdevhost/fusion/dev.h>
#include <libstrict/strict.h>
#include <libsysif/devhost/api.h>

/* for /dev/null, zero, full, ..., memory devices */
#define DEV_MEM_MAJOR	1U
#define DEV_NULL_MINOR	3U
#define DEV_ZERO_MINOR	5U
#define DEV_FULL_MINOR	7U
#define DEV_RANDOM_MINOR	8U
#define DEV_URANDOM_MINOR	9U

static bool is_fast_open_dev(int vfs_mode, unsigned int devno)
{
	bool fast_open = vfs_mode == MODE_CHR &&
			 ((devno == MKDEV(DEV_MEM_MAJOR, DEV_NULL_MINOR)) ||
			 (devno == MKDEV(DEV_MEM_MAJOR, DEV_ZERO_MINOR)) ||
			 (devno == MKDEV(DEV_MEM_MAJOR, DEV_FULL_MINOR)) ||
			 (devno == MKDEV(DEV_MEM_MAJOR, DEV_RANDOM_MINOR)) ||
			 (devno == MKDEV(DEV_MEM_MAJOR, DEV_URANDOM_MINOR)));
	return fast_open;
}

int libdh_dev_open(struct hmsrv_io_ctx *ctx, unsigned int devno, int vfs_mode,
		   unsigned int vfs_flags, unsigned int flags)
{
	int ret;
	unsigned int user_cidx = 0U;
	struct __actvret_drvcall_devhost_fops_open devhost_ret;

	if (!is_fast_open_dev(vfs_mode, devno)) {
		return E_HM_NODEV;
	}

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* user_cidx should be fs/sysmgr? */
	user_cidx = hm_ucap_self_cnode_idx();
	mem_zero_s(devhost_ret);
	/* here we just trans devno, devhost_fops_open should use devno to acquire devid */
	ret = actvxactcapcall_drvcall_devhost_fops_open(
			true, true, /* `is fwd` && `allow refwd` */
			0, user_cidx,
			devno, 0, vfs_flags,
			&devhost_ret);
	if (ret < 0) {
		return ret;
	}
	mem_zero_s(*ctx);
	ctx->filp = devhost_ret.filp;
	/* dh_cnode_idx should be fs/sysmgr? */
	ctx->dh_cnode_idx = user_cidx;
	ctx->flags = flags;
	ctx->dev_fops_flags = devhost_ret.dev_fops_flags;

	return E_HM_OK;
}
