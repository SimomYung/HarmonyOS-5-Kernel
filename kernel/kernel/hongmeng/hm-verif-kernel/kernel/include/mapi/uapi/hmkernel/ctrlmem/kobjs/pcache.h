/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: pcache metadata as kobject - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 01 18:14:02 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_PCACHE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_PCACHE_H

#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>

struct __ctrlmem_kobj_pcache_s {
	__u64 pcache_kptr;
	__u64 syspgarr_kptr;
	/*
	 * NOTE: do not call it gzeropg_pa in uapi:
	 * when writingn, it is gzeropg_va.
	 */
	__u64 gzeropg;
	__u64 gzeropg_pgstr;
	__u64 oplog;
	struct __ctrlmem_sysparule_s sysparule;
};

#endif
