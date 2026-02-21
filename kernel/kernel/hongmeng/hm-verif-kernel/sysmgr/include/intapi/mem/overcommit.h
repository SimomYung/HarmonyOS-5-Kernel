/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memory overcommit
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 10:33:53 2019
 */
#ifndef SYSMGR_MEMMGR_MEM_OVERCOMMIT_H
#define SYSMGR_MEMMGR_MEM_OVERCOMMIT_H

#include <hongmeng/types.h>

#include <stdint.h>
#include <stdbool.h>

#define __OVERCOMMIT_MEMORY CONFIG_OVERCOMMIT_MEMORY
#ifdef CONFIG_OVERCOMMIT_RATIO
#	define __OVERCOMMIT_RATIO CONFIG_OVERCOMMIT_RATIO
#else
#	define __OVERCOMMIT_RATIO 50
#endif
#define OVM_POLICY_MAX_SIZE 2
#define OVM_RATIO_MAX_SIZE 11 /* max numbers of the overcommit ratio */
enum overcommit_policy_e {
	/* calculate the free mem in buddy and page allocator */
	OVERCOMMIT_GUESS,
	/* always return true when doing overcommit_check */
	OVERCOMMIT_ALWAYS,
	/* calculate the specific ratio of total ram */
	OVERCOMMIT_NEVER
};

struct vspace_s;
uint64_t overcommit_limit_size(void);
enum overcommit_policy_e overcommit_policy_get(void);
void overcommit_policy_set(enum overcommit_policy_e policy);
int overcommit_ratio_get(void);
void overcommit_ratio_set(int ratio);
bool overcommit_check(const struct vspace_s *vspace, size_t length);
#endif
