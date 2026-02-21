/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memcg utilities
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 22 11:41:36 2021
 */

#ifndef ULIBS_INCLUDE_LIBMEM_MEMCG_H
#define ULIBS_INCLUDE_LIBMEM_MEMCG_H

#include <limits.h>
#include <stdint.h>
#include <hongmeng/types.h>

#define MEM_CGROUP_ID_MAX	4096
#define MEMCG_ROOT_ID		MEM_CGROUP_ID_MAX

#ifndef CONFIG_PAGE_META_MERGE
struct memcg_share_slot_s {
	union {
		uint32_t cnode_idx;
		/* keep version in head to enable lock-free read */
		unsigned int version;
	} u;
	unsigned int memcg_id;
	uintptr_t next_off;
};

static inline struct memcg_share_slot_s *memcg_share_from_off(uintptr_t addr, uintptr_t off)
{
	return ulong_to_ptr(addr + off, struct memcg_share_slot_s);
}
#endif

/*
 * Initialize memcg utilities. Services that want to access the cnode_idx to
 * memcg mapping should call this function, which would call sysmgr to query
 * the virtual address of the shared memory in the calling service.
 *
 * @return  Return E_HM_OK if succeeded, or E_HM_XX if failed.
 */
int memcg_utils_init(void);

/*
 * Query the memcg_id of the process with the given cnode_idx. The function
 * goes through the shared memory and figures out the memcg_id of a process.
 *
 * @param cnode_idx   [I] the cnode_idx of the process.
 *
 * @param memcg_id    [O] the memcg_id of the process.
 *
 * @return  Return E_HM_OK if the entry of the cnode_idx is found and memcg_id
 *          was set, E_HM_NOENT if no entry is available (possibly in root
 *          memcg), E_HM_FAULT if memcg_utils_init was not called.
 */
int memcg_utils_query_id(uint32_t cnode_idx, unsigned int *memcg_id);

/*
 * Charge a memcg for certain memory usage in number of pages.
 *
 * @param memcg_id    [I] the id of memcg being charged.
 *
 * @param nr_pages    [I] the amount of pages to charge.
 *
 * @param nr_charged  [O] the amount of pages actually charged.
 *
 * @return  Return E_HM_OK if charged successfully, or E_HM_XX if failed.
 */

int memcg_utils_charge(unsigned int memcg_id, unsigned int nr_pages,
		       unsigned int *nr_charged);
/*
 * Uncharge a memcg for certain memory usage in number of pages.
 *
 * @param memcg_id    [I] the id of memcg to be uncharged.
 *
 * @param nr_pages    [I] the ammount of pages to uncharged.
 *
 * @return  Return E_HM_OK if uncharged successfully, or E_HM_XX if failed.
 */
int memcg_utils_uncharge(unsigned int memcg_id, unsigned int nr_pages);

#endif
