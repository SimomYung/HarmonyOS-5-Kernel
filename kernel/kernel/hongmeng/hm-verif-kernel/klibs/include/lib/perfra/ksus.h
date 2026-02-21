/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: The Kernel-Shared-User-Shared memory for DFX only
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 29 06:23:51 2021
 */

#ifndef KLIBS_PERFRA_KSUS_H
#define KLIBS_PERFRA_KSUS_H

#include <hmasm/types.h>

/*
 * The memoy chunk related infrastructure for ksus
 */
struct ksus_chunk_s {
	/* The base address of the chunk */
	__u64 base;
	/* The size of each chunk */
	__u32 size;
	/* The number of chunks */
	__u32 number;
};

#define KSUS_CHUNK_NR		0x2U
struct ksus_desc_s {
	struct ksus_chunk_s chunk[KSUS_CHUNK_NR];
};
struct ksus_desc_s *get_ksus_desc(void);

#endif
