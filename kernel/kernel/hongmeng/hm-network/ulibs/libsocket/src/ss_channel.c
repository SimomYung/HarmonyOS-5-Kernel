/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of channel
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include <hmkernel/capability.h>
#include <libsysif/sysmgr/api.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_procmgr.h>

#include "hm_socket.h"
#include "ss_channel.h"

#include "ss_shmem.h"

static rref_t hmnet_actv_rref = 0;
static struct raw_mutex hmnet_init_lock = RAW_MUTEX_INITIALIZER;

int hm_net_ap_rref_and_cnode(rref_t* rref, uint32_t* cnode_idx)
{
	int ret;
	struct sysarg_cnode_statrref statrref = { 0 };

	ret = hm_path_acquire(HMNET_CHANNEL, rref);
	if (ret < 0) {
		return ret;
	}

	ret = sysfast_cnode_stat_rref(&statrref, *rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret < 0) {
		(void)hm_path_release(HMNET_CHANNEL, *rref);
	} else {
		*cnode_idx = statrref.src_cnode_idx;
	}

	return ret;
}

int hm_net_ap_rref_noput(rref_t* rref)
{
	int ret;

	if (hmnet_actv_rref != 0) {
		*rref = hmnet_actv_rref;
		return E_HM_OK;
	}

	raw_mutex_lock(&hmnet_init_lock);
	if (hmnet_actv_rref != 0) {
		*rref = hmnet_actv_rref;
		ret = E_HM_OK;
		goto unlock;
	}

	ret = hm_path_acquire(HMNET_CHANNEL, &hmnet_actv_rref);
	if (ret < 0) {
		hmnet_actv_rref = 0;
		goto unlock;
	}
	*rref = hmnet_actv_rref;
	ret = E_HM_OK;

unlock:
	raw_mutex_unlock(&hmnet_init_lock);
	return ret;
}
