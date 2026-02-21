/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 11 15:59:16 2021
 */
#ifndef KLIBS_JITTER_H
#define KLIBS_JITTER_H
#include <hmkernel/types.h>

struct jitter_mem_ctx {
	unsigned char *mem_addr;
	size_t mem_size;
};

#define RNG_ENTROPY_STATE_VALID		0
#define RNG_ENTROPY_STATE_INVALID	1

#define JITTER_MEM_BLOCK 64U
#define JITTER_MEM_BLOCK_SIZE 32U
#define JITTER_MEM_SIZE (JITTER_MEM_BLOCK * JITTER_MEM_BLOCK_SIZE)

int jitter_init(const struct jitter_mem_ctx *ctx);
int jitter_generate_entropy(u64 *random);

#endif
