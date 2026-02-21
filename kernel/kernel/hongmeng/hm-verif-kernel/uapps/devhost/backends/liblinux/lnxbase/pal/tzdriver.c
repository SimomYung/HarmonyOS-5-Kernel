/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Tzdriver pal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 1 15:22:27 2024
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

int liblinux_pal_get_processname_by_tid(__u32 calling_tid, char *path, uint32_t path_len)
{
#ifdef CONFIG_SUPPORT_GETPROCNAME
	int ret = E_HM_OK;
	struct __actvret_hmcall_proc_gettidpath getpath_ret = {0};
	if (path == NULL || path_len == 0) {
		hm_error("Input parameter error.\n");
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = actvcall_hmcall_proc_gettidpath(calling_tid, &getpath_ret);
		if (ret != 0) {
			hm_warn("actvcall_hmcall_proc_gettidpath failed, err=%s\n", hmstrerror(ret));
		}
	}
	if (ret == 0) {
		ret = strcpy_s(path, path_len, getpath_ret.tid_path.path);
		if (ret != EOK) {
			hm_error("strcpy_s fail\n");
		}
	}
	return ret;
#else
	UNUSED(calling_tid, path, path_len);
	return E_HM_NOSYS;
#endif
}
