/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Inter-mgr apis of memory controller
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 27 06:31:05 2021
 */

#ifndef SYSMGR_INTAPI_RES_MEMORY_H
#define SYSMGR_INTAPI_RES_MEMORY_H

#include <libhmsync/atomic.h>
#include <libmem/utils.h>
#include <libmem/memcg_utils.h>
#include <intapi/res/res_def.h>
#include <intapi/mem/lru.h>
#include <intapi/mem/vmpressure.h>
#include <intapi/mem/page.h>

enum memcg_limit_type {
	LIMIT_TYPE_MIN,
	LIMIT_TYPE_LOW,
	LIMIT_TYPE_HIGH,
	LIMIT_TYPE_MAX,
	LIMIT_TYPE_INVALID,
};

#ifdef CONFIG_HYPERHOLD
enum hyperhold_memcg_type {
	TYPE_APP_SCORE,
	TYPE_FORCE_SWAPIN,
	TYPE_FORCE_SWAPOUT,
	TYPE_FORCE_SHRINK_ANON,
	TYPE_NAME,
	TYPE_GPU_RECLAIMED,
	TYPE_MAX,
};

#ifdef CONFIG_HYPERHOLD_NC
enum nc_hyperhold_memcg_type {
	TYPE_NOCMP_LIMIT,
	TYPE_LRU_ANON_NOCMP,
	TYPE_FORCE_SHRINK_NOCMP,
	TYPE_ENABLE_NOCMP_BY_RATIO,
	NC_TYPE_MAX,
};
#endif

enum reclaim_param_type {
	MIN_SCORE,
	MAX_SCORE,
	MEM_ZRAM_RATIO,
	ZRAM_UFS_RATIO,
	REFAULT_THRESHOLD,
};
#endif

#ifdef CONFIG_HYPERHOLD
struct reclaim_param_s {
	int min_score;
	int max_score;
	int ub_mem2zram_ratio;
	int ub_zram2ufs_ratio;
	int refault_threshold;
};
#endif

#ifdef CONFIG_RESMGR_MEMORY
static inline struct memcg_s *robj_to_memcg(const struct res_obj *robj)
{
	return (robj != NULL) ? container_of(robj, struct memcg_s, robj) : NULL;
}

static inline struct memcg_s *group_to_memcg(struct res_group *rgrp)
{
	return robj_to_memcg(group_robj(rgrp, (unsigned int)memory_controller_id));
}

#define MEMCG_ITERATE_CONTINUE 0
#define MEMCG_ITERATE_BREAK   1

bool memcg_root_exists(void);
struct memcg_s *memcg_fetch_root(void);
struct memcg_s *memcg_parent_of(struct memcg_s *memcg);
struct res_group *fetch_adapted_rgrp(struct res_group *rgrp);

struct memcg_s *memcg_of_process(const struct process_s *process);
struct memcg_s *get_memcg_of_process_nolock(const struct process_s *process);
struct memcg_s *get_memcg_of_process(const struct process_s *process);
bool memcg_below_low(struct memcg_s *memcg);
bool memcg_below_min(struct memcg_s *memcg);
int memcg_tree_for_each(struct memcg_s *root,
			int (*memcg_callback)(struct memcg_s *, void *),
			void *args);
#ifdef CONFIG_HYPERHOLD
struct memcg_s *hyperhold_memcg_fetch_root(void);
#else
static inline struct memcg_s *hyperhold_memcg_fetch_root(void)
{
	return NULL;
}
#endif
#else /* CONFIG_RESMGR_MEMORY */
static inline struct memcg_s *robj_to_memcg(const struct res_obj *robj)
{
	UNUSED(robj);
	return NULL;
}

static inline struct memcg_s *group_to_memcg(struct res_group *rgrp)
{
	UNUSED(rgrp);
	return NULL;
}

static inline bool memcg_root_exists(void)
{
	return false;
}

static inline struct memcg_s *memcg_fetch_root(void)
{
	return NULL;
}

static inline struct memcg_s *memcg_parent_of(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return NULL;
}

static inline struct res_group *fetch_adapted_rgrp(struct res_group *rgrp)
{
	UNUSED(rgrp);
	return NULL;
}

static inline struct memcg_s *memcg_of_process(const struct process_s *process)
{
	UNUSED(process);
	return NULL;
}
static inline struct memcg_s *get_memcg_of_process_nolock(const struct process_s *process)
{
	UNUSED(process);
	return NULL;
}

static inline struct memcg_s *get_memcg_of_process(const struct process_s *process)
{
	UNUSED(process);
	return NULL;
}

static inline int memcg_tree_for_each(struct memcg_s *root,
				      int (*memcg_callback)(struct memcg_s *, void *),
				      void *args)
{
	UNUSED(root, memcg_callback, args);
	return E_HM_NOSYS;
}
static inline struct memcg_s *hyperhold_memcg_fetch_root(void)
{
	return NULL;
}
#endif /* CONFIG_RESMGR_MEMORY */

#endif
