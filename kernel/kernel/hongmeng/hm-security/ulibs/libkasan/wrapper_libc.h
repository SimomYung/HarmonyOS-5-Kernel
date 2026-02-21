/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Declaration of wrapped libc function
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */

#ifndef __WRAPPER_LIBC_H_
#define __WRAPPER_LIBC_H_
#include <stddef.h>
#include <stdint.h>
#include <lib/dlist.h>

/* chunk should be at least 64 bytes */
#define MALLOC_CHUNK 64

struct delay_free_node {
        struct dlist_node node;
        uintptr_t real_free_addr;
};

typedef struct _shadow_meta {
	struct delay_free_node free_node;
	intptr_t rtu; /* real buf to user buf offset */
	size_t real_sz; /* real size allocated */
	size_t user_sz; /* size of user's request */
	union {
		uint8_t magic; /* magic value for alloc */
		uintptr_t free_addr; /* address of the code for releasing memory */
	};
} shadow_meta;

#include "wrapper_libc_decl.h"
#endif
