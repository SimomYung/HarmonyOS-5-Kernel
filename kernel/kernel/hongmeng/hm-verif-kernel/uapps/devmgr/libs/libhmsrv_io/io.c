/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service library core
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#include <hongmeng/errno.h>

#include <libsysif/devmgr/api.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_io/io.h>
#include <libhmucap/ucap.h>
#include <libhmlog/hmlog.h>

#include "internal.h"

static __u32 g_devmgr_cnode_idx = (__u32)CNODE_IDX_INVALD_ZERO;
rref_t g_devmgr_ap_rref = (rref_t)0;

rref_t hmsrv_io_require(void)
{
	return hm_ucap_require(0, g_devmgr_cnode_idx, 0);
}

int hmsrv_io_init(void)
{
	rref_t devmgr_rref = ERR_TO_REF(E_HM_INVAL);
	struct sysarg_cnode_statrref stat;
	int ret;
	int err;

	/* already inited, return success */
	if (!IS_REF_ERR(g_devmgr_ap_rref)) {
		if (sysfast_cnode_stat_rref(&stat, g_devmgr_ap_rref, CNODE_STATRREF_SRC_CNODE_IDX) >= 0) {
			return E_HM_OK;
		}
	}

	/* query devmgr actvpool */
	ret = hm_path_acquire(DEVMGR_PATH, &devmgr_rref);
	if  (ret < 0) {
		return ret;
	}
	if (IS_REF_ERR(devmgr_rref)) {
		return E_HM_INVAL;
	}

	ret = sysfast_cnode_stat_rref(&stat, devmgr_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret < 0) {
		err = hm_path_release(DEVMGR_PATH, devmgr_rref);
		if (err != E_HM_OK) {
			hm_warn("hm_path_release failed %s\n", hmstrerror(err));
		}
		return ret;
	}

	g_devmgr_cnode_idx = stat.src_cnode_idx;
	g_devmgr_ap_rref = devmgr_rref;

	return E_HM_OK;
}
