/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: This is the implementation of udk ext interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 24 21:24:17 2022
 */
#include <udk/ext.h>
#include <udk/log.h>
#include <internal/init.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/ext_fast_syscall.h>

#ifdef CONFIG_UDK_EXT

#define UDK_EXT_FASTCALL_CNODE_HMEXT_IDX    2

/* sysfast_ext_ctrl_handler */
__EXT_FASTCALL_PER_CNODE(5, udk_ext_ctrl, UDK_EXT_FASTCALL_CNODE_HMEXT_IDX,
			 unsigned long, cmd,
			 unsigned long, arg0, unsigned long, arg1,
			 unsigned long, arg2, unsigned long, arg3)

int udk_ext_ctl(uint32_t cmd, void *cfg, uint32_t cfg_len, void *rlt, uint32_t rlt_len)
{
	int ret;

	/* cfg and rlt might be zero, let micro-kernel decide */
	ret = sysfast_udk_ext_ctrl((unsigned long)cmd,
				   ptr_to_ulong(cfg), (unsigned long)cfg_len,
				   ptr_to_ulong(rlt), (unsigned long)rlt_len);
	if (ret < 0) {
		udk_warn("ext fastcall handler failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int udk_ext_init(void)
{
	int ret;

	ret = syscap_CNodeExtFastcallBind(0, __ext_fastcall_scno_of_udk_ext_ctrl());
	if (ret < 0) {
		udk_warn("ext fastcall bind failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

udk_init_call(udk_ext_init);

#else

static inline int udk_ext_ctl(uint32_t cmd, void *cfg, uint32_t cfg_len,
			      void *rlt, uint32_t rlt_len)
{
	UNUSED(cmd, cfg, cfg_len, rlt, rlt_len);

	return -ENOSYS;
}

#endif /* CONFIG_UDK_EXT */
