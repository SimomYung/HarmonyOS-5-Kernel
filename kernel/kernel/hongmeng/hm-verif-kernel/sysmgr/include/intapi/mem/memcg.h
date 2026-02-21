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

uint64_t memcg_read_pagetables(struct memcg_s *memcg);
uint64_t memcg_read_file_mapped(struct memcg_s *memcg);
uint64_t memcg_read_zswapped(struct memcg_s *memcg);
uint64_t memcg_read_zswap(struct memcg_s *memcg);
#ifdef CONFIG_MEM_PURGEABLE
size_t memcg_reclaim_purgeable(struct memcg_s *memcg, struct scan_control *sc);
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

#ifdef CONFIG_HYPERHOLD_NC
uint64_t memcg_read_eswap_nc(struct memcg_s *memcg);
uint64_t memcg_read_zram_nc(struct memcg_s *memcg);
void update_memcg_nocompress_limit(struct memcg_s *memcg, bool force_shrink);
void set_no_compress_flag(struct dlist_node *pglist, bool force_shrink);
#else /* !CONFIG_HYPERHOLD_NC */
static inline uint64_t memcg_read_eswap_nc(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}
static inline uint64_t memcg_read_zram_nc(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return 0ULL;
}

static inline void update_memcg_nocompress_limit(struct memcg_s *memcg, bool force_shrink)
{
	UNUSED(memcg, force_shrink);
}
static inline void set_no_compress_flag(struct memcg_s *memcg, bool force_shrink)
{
	UNUSED(memcg, force_shrink);
}
#endif /* CONFIG_HYPERHOLD_NC */

#endif /* SYSMGR_INTAPI_MEM_MEMCG_H */
