/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Descriptions: interface for livepatch
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 17 16:22:56 2023
 */

#include <lib/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_livepatch.h>
#include <libsysif/sysmgr/api.h>

int hm_livepatch_register_target(void)
{
	int err;
	err = actvcall_hmcall_livepatch_register_target();
	if (err != E_HM_OK) {
		hm_error("register target failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

int hm_livepatch_unregister_target(void)
{
	int err;
	err = actvcall_hmcall_livepatch_unregister_target();
	if (err != E_HM_OK) {
		hm_error("unregister target failed, err=%s\n", hmstrerror(err));
	}
	return err;
}
