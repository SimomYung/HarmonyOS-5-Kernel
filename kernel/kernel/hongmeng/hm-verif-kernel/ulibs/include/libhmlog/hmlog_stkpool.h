/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: Definitions of hongmeng kernel log api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 19 03:43:55 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMLOG_HMLOG_STKPOOL_H
#define ULIBS_INCLUDE_LIBHMLOG_HMLOG_STKPOOL_H

#include <hmasm/page.h>
#include <hongmeng/types.h>
#include <hmkernel/compiler.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_mutex.h>

#define HMLOG_STKPOOL_STACK_SIZE PAGE_SIZE
#define HMLOG_STKPOOL_STACK_GUARD PAGE_SIZE
#define HMLOG_STKPOOL_STACK_COUNT (16U)

/* located at the top of each hmlog stack */
struct __hmlog_stk_meta {
	unsigned long bitmap_id;
	unsigned long reserved;
} __aligned(16U);

struct __hmlog_stkpool {
	DEFINE_RAW_BITMAP(stkmap, HMLOG_STKPOOL_STACK_COUNT);

	unsigned int last;
	/*
	 * each element of stkpool is the address of stack top
	 * and point to the __hmlog_stk_meta
	 *
	 * +------------------+
	 * | __hmlog_stk_meta |
	 * +------------------+ <-- stkpool[i], 0 <= i < HMLOG_STKPOOL_STACK_COUNT
	 * |  availabe stack  |
	 * +------------------+
	 * |  possible guard  |
	 * +------------------+
	 */
	void *stkpool[HMLOG_STKPOOL_STACK_COUNT];
	struct raw_mutex mutex;
};

void hmlog_stkpool_set(struct __hmlog_stkpool *stkpool);

#endif /* ULIBS_INCLUDE_LIBHMLOG_HMLOG_STKPOOL_H */
