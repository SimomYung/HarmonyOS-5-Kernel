/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Memory cgroup control intapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 17 11:19:59 2023
 */
#ifndef SYSMGR_INTAPI_MEM_MEMCG_H
#define SYSMGR_INTAPI_MEM_MEMCG_H

#include <intapi/res/memory.h>

struct process_s;

#ifdef CONFIG_RESMGR_MEMORY
void memcg_set_oom_memcg(struct memcg_s *mcg);
struct memcg_s *memcg_fetch_oom_memcg(void);
void memcg_clear_oom_memcg(void);

int memcg_meta_map(uintptr_t *rvaddr);
int memcg_meta_new(const struct process_s *proc);
void memcg_meta_del(const struct process_s *proc);
int memcg_meta_update(struct memcg_s *dst, const struct process_s *proc);
void memcg_meta_update_rollback(struct memcg_s *dst, const struct process_s *proc);

int memcg_fs_charge(struct memcg_s *memcg, unsigned int count, unsigned int *charged);
int memcg_fs_uncharge(struct memcg_s *memcg, unsigned int count);
uint64_t memcg_read_pagetables(struct memcg_s *memcg);
uint64_t memcg_read_file_mapped(struct memcg_s *memcg);
uint64_t memcg_read_zswapped(struct memcg_s *mcg);
uint64_t memcg_read_zswap(struct memcg_s *mcg);
#ifdef CONFIG_MEM_PURGEABLE
size_t memcg_reclaim_purgeable(struct memcg_s *memcg_root, struct scan_control *sc);
#endif

#else /* !CONFIG_RESMGR_MEMORY */

#include <libstrict/strict.h>
#include <hongmeng/errno.h>

static inline void memcg_set_oom_memcg(struct memcg_s *mcg)
{
	UNUSED(mcg);
}

static inline struct memcg_s *memcg_fetch_oom_memcg(void)
{
	return NULL;
}

static inline void memcg_clear_oom_memcg(void) {}

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

static inline int memcg_fs_charge(struct memcg_s *memcg, unsigned int count,
				  unsigned int *charged)
{
	UNUSED(memcg, count, charged);
	return E_HM_OK;
}

static inline int memcg_fs_uncharge(struct memcg_s *memcg, unsigned int count)
{
	UNUSED(memcg, count);
	return E_HM_OK;
}

static inline uint64_t memcg_read_pagetables(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}

static inline uint64_t memcg_read_file_mapped(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}

static inline uint64_t memcg_read_zswapped(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}

static inline uint64_t memcg_read_zswap(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}

#endif /* CONFIG_RESMGR_MEMORY */
#ifdef CONFIG_HYPERHOLD
uint64_t memcg_read_zram(struct memcg_s *memcg);
uint64_t memcg_read_eswap(struct memcg_s *memcg);
#else /* !CONFIG_HYPERHOLD */
static inline uint64_t memcg_read_zram(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}
static inline uint64_t memcg_read_eswap(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}
#endif /* CONFIG_HYPERHOLD */
#endif /* SYSMGR_INTAPI_MEM_MEMCG_H */
