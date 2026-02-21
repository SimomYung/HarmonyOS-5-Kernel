/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Defs for syspa
 * Author: Huawei OS Kernel Lab
 * Created: Mon May 29 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPA_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPA_H

#include <hmkernel/types.h>
#include <hmasm/memory.h>

struct __ctrlmem_syspa32_s {
	__u32 val;
};

struct __ctrlmem_syspa64_s {
	__u64 val;
};

typedef struct __ctrlmem_syspa32_s __ctrlmem_syspa32_t;
typedef struct __ctrlmem_syspa64_s __ctrlmem_syspa64_t;

/*
 * Helper type for sysmgr encode its pointer to syspa
 */
#if __SIZEOF_POINTER__ == 4
typedef __ctrlmem_syspa32_t __ctrlmem_syspa_t;
#else
typedef __ctrlmem_syspa64_t __ctrlmem_syspa_t;
#endif

#define __CTRLMEM_SYSPARULE_UNSET		0
#define __CTRLMEM_SYSPARULE_PTWALK		1
#define __CTRLMEM_SYSPARULE_LINEAR		2
#define __CTRLMEM_SYSPARULE_SPIN		3

struct __ctrlmem_sysparule_s {
	__u32 type;
	__u32 padding;
	union {
		struct __ctrlmem_sysparule_pgtblwalk_data_s {
			__u64 kobjvs_ptr;
		} pgtblwalk_data;
		struct __ctrlmem_sysparule_linear_data_s {
			__u64 pvoffset;
		} linear_data;
		struct __ctrlmem_sysparule_spin_data_s {
		} spin_data;
	};
};

#endif
