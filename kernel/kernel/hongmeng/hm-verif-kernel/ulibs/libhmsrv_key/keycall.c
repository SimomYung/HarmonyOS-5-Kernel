/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Prepare actvcall for native interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 10 20:54:39 2021
 */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_key/key.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_key/keymgr.h>

rref_t key_get_keymgr_rref(void)
{
	rref_t rref;
	int err = E_HM_OK;
	err = hm_path_acquire(KEYMGR_PATH, &rref);
	if (err != E_HM_OK) {
		rref = ERR_TO_REF(err);
	}
	return rref;
}

int key_release_keymgr_rref(rref_t rref)
{
	return hm_path_release(KEYMGR_PATH, rref);
}
