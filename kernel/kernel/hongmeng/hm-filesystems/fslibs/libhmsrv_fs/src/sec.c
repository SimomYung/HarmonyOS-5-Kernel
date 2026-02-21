/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Set and get secure label functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 16:04:27 2019
 */

#include <vfs.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include "internal.h"

int vfs_set_hkip_label(int fd)
{
	return actvxcap_fscall(vfs_set_hkip_label, false, true, fd);
}

int vfs_set_label(int fd, uint64_t label)
{
	int err;

	err = actvxcap_fscall(vfs_set_label, false, true, fd, label);
	if (err < 0) {
		hm_error("FS set label failed %d.\n", err);
	}

	return err;
}

int vfs_set_label_by_path(const char *path, int flag, uint64_t label)
{
	return actvxcap_fscall(vfs_set_label_by_path, false, true, path, flag, label, -1, 0, 0);
}

int vfs_get_label(int fd, uint64_t *label)
{
	struct __actvret_fscall_vfs_get_label ret;
	int err;

	if (label == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(ret);
	err = actvxcap_fscall(vfs_get_label, false, true, fd, &ret);
	if (err < 0) {
		hm_error("FS get label failed %d.\n", err);
		return err;
	}

	*label = ret.label;

	return err;
}

int vfs_pac_fault_dfi(void)
{
	return actvxcap_fscall(vfs_pac_fault_dfi, false, true);
}

int vfs_pac_fault_cfi_func_table(void)
{
	return actvxcap_fscall(vfs_pac_fault_cfi_func_table, false, true);
}

int vfs_proc_unauthoried_files(void)
{
	return actvxcap_fscall(vfs_proc_unauthoried_files, true, true);
}

int vfs_lock_in(unsigned int user_id)
{
	return actvxcap_fscall(vfs_lock_in, false, true, user_id);
}

int vfs_unlock_in(unsigned int user_id, unsigned int file, unsigned char *iv, unsigned int iv_len)
{
	struct __actvret_fscall_vfs_unlock_in ret;
	int err;

	mem_zero_s(ret);
	err = actvxcap_fscall(vfs_unlock_in, false, true, user_id, file, iv, iv_len, &ret);
	if (err != E_HM_OK) {
		hm_error("vfs_unlock_in fail %d.\n", err);
		return err;
	}

	return ret.ret;
}
