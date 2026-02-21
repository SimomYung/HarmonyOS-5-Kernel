/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: pgtblpool related API
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 11:40:39 2024
 */

#ifndef HMKERNEL_DRIVERS_BOOTPGTBLPOOL_H
#define HMKERNEL_DRIVERS_BOOTPGTBLPOOL_H

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/boot/pgtblpool.h>

#ifndef CONFIG_BOOTPGTBLPOOL_ENABLE
static inline void bootpgtblpool_allocator_init(struct bootpgtblpool_s *pool)
{
	UNUSED(pool);
}
static inline void bootpgtblpool_allocator_mkrdonly(const struct bootpgtblpool_s *pool)
{
	UNUSED(pool);
}
#else
void bootpgtblpool_allocator_init(struct bootpgtblpool_s *pool);
void bootpgtblpool_allocator_mkrdonly(const struct bootpgtblpool_s *pool);
#endif

#endif
