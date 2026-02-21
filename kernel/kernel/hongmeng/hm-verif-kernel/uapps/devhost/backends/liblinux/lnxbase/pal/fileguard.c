/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Adapt fileguard for liblinux
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */
#include <liblinux/pal.h>
#include <libstrict/strict.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

/* for fileguard */
int liblinux_pal_sec_fileguard_cfg(const void *ctx, size_t len, size_t *ret)
{
#ifdef CONFIG_FILEGUARD
	int err;
	struct __actvret_hmcall_sec_fileguard_cfg fileguard_cfg_ret;
	mem_zero_s(fileguard_cfg_ret);
	err = actvcall_hmcall_sec_fileguard_cfg(ctx, len, &fileguard_cfg_ret);
	*ret = fileguard_cfg_ret.ret;

	hm_info("[fg] actvcall sec fileguard cfg len %lu, ret %lu, err %d\n",
		len, (uint64_t)fileguard_cfg_ret.ret, err);
	return err;
#else
	UNUSED(ctx, len, ret);
	return E_HM_NOSYS;
#endif
}
