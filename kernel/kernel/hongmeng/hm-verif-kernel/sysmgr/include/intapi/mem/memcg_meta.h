/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Memory cgroup control intapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 17 16:32:31 2025
 */
#ifndef SYSMGR_INTAPI_MEM_MEMCG_META_H
#define SYSMGR_INTAPI_MEM_MEMCG_META_H

#include <intapi/res/memory.h>

struct process_s;
struct memcg_s;

#ifdef CONFIG_RESMGR_MEMORY
int memcg_meta_map(uintptr_t *rvaddr);

int memcg_meta_new(const struct process_s *proc);

void memcg_meta_del(const struct process_s *proc);

int memcg_meta_update(struct memcg_s *dst, const struct process_s *proc);

void memcg_meta_update_rollback(struct memcg_s *dst, const struct process_s *proc);
#else
static inline int memcg_meta_map(uintptr_t *rvaddr)
{
	UNUSED(rvaddr);
	return E_HM_NOSYS;
}

static inline int memcg_meta_new(const struct process_s *proc)
{
	UNUSED(proc);
	return E_HM_OK;
}

static inline void memcg_meta_del(const struct process_s *proc)
{
	UNUSED(proc);
}

static inline int memcg_meta_update(struct memcg_s *dst, const struct process_s *proc)
{
	UNUSED(dst, proc);
	return E_HM_OK;
}

static inline void memcg_meta_update_rollback(struct memcg_s *dst, const struct process_s *proc)
{
	UNUSED(dst, proc);
}
#endif
#endif
