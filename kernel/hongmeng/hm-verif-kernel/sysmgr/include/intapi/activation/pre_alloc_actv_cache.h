/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Actv cache pool interface declaration
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 10 15:11:29 2023
 */

#ifndef INTAPI_ACTIVATIONH_ALLOC_ACTV_CACHE_H
#define INTAPI_ACTIVATIONH_ALLOC_ACTV_CACHE_H

#include <stdint.h>

enum cache_reclaim_type {
	TYPE_ALL_RECLAIM,
	TYPE_PARTIAL_RECLAIM,
};

struct actv_cache_reclaim_policy {
	enum cache_reclaim_type reclaim_type;
};

#ifdef CONFIG_ACTV_CACHE_POOL

void pre_alloc_actv_reclaim_cache(struct actv_cache_reclaim_policy *reclaim_policy);
int pre_alloc_actv_get_cache_free_num(void);
int pre_alloc_actv_get_total_num(void);
size_t pre_alloc_actv_get_reclaim_mem(void);
size_t pre_alloc_actv_get_total_mem(void);

#else

static inline void pre_alloc_actv_reclaim_cache(struct actv_cache_reclaim_policy *reclaim_policy)
{
	UNUSED(reclaim_policy);
	return;
}

static inline int pre_alloc_actv_get_cache_free_num(void)
{
	return 0;
}

static inline int pre_alloc_actv_get_total_num(void)
{
	return 0;
}

static inline size_t pre_alloc_actv_get_reclaim_mem(void)
{
	return 0;
}

static inline size_t pre_alloc_actv_get_total_mem(void)
{
	return 0;
}

#endif /* CONFIG_ACTV_CACHE_POOL */

#endif
