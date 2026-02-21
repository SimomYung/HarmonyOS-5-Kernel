/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Ctrlmem rwlock related defs
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 27 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_RWLOCK_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_RWLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>

#define __CTRLMEM_RWLOCK_RDSHIFT		0
#define __CTRLMEM_RWLOCK_WRSHIFT		16
#define __CTRLMEM_RWLOCK_RDMASK			0x0000ffff
#define __CTRLMEM_RWLOCK_WRMASK			0xffff0000
#define __CTRLMEM_RWLOCK_RD_MAXCNT		((0xffffffff) & __CTRLMEM_RWLOCK_RDMASK)
#define __CTRLMEM_RWLOCK_WR_MAXCNT		(((0xffffffff) & __CTRLMEM_RWLOCK_WRMASK) >> __CTRLMEM_RWLOCK_WRSHIFT)

struct __ctrlmem_kobj_rwlock_s {
	__u32 counter;
};

#endif
