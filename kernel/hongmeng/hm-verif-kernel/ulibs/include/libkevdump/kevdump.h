/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Kevdump header structure definition
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 14 12:01:33 2020
 */

#ifndef ULIBS_INCLUDE_LIBKEVDUMP_KEVDUMP_H
#define ULIBS_INCLUDE_LIBKEVDUMP_KEVDUMP_H

#include <stdint.h>

#define KEVDUMP_MEM_MAGIC	0x4B455644U /* hex ascii code for KEVD */
#define LEVDUMP_MEM_MAGIC	0x454C3254U /* hex ascii code for EL2T */

enum kevdump_endian_type {
	KEVDUMP_ENDIAN_BIG     =  0U,
	KEVDUMP_ENDIAN_LITTLE  =  1U,
	KEVDUMP_ENDIAN_INVALID = -1U,
};

struct kevdump_memory {
	uint32_t magic; /* magic number, always stored in little endian */
	uint32_t endian; /* endian type, always stored in little endian */
	uint32_t version; /* version for kevdump */
	uint32_t size; /* kevdump memory size */
	uint32_t dump_size; /* size of the data dumped */
	char data[0]; /* start address of the dumped data */
};

static inline enum kevdump_endian_type kevdump_read_endian_type(void)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	return KEVDUMP_ENDIAN_LITTLE;
#elif __BYTE_ORDER == __BIG_ENDIAN
	return KEVDUMP_ENDIAN_BIG;
#else
	return KEVDUMP_ENDIAN_INVALID;
#endif
}

#endif
