/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Statistics interface for lru
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 09 10:26:20 2023
 */

#ifndef SYSMGR_MEMMGR_STAT_LRU_STAT_H
#define SYSMGR_MEMMGR_STAT_LRU_STAT_H

#include <libmem/mstat.h>
#include <intapi/mem/lru.h>

struct memcg_s;

void pgscan_kswapd_inc(uint64_t nr_pgscan);
uint64_t pgscan_kswapd_read(void);
void pgscan_direct_inc(uint64_t nr_pgscan);
uint64_t pgscan_direct_read(void);

/*
 * Fill lru info together with fs statistics.
 *
 * @param info		[I] fs statistics data, can be fetched by fs_stat_calculate
 *
 * @param lru_size	[O] where stat data is stored, should be lru_size[__NR_LRU_PAGE_TYPES][__NR_LRU_LISTS]
 *
 * @param len		[I] the length of the first dimension of lru_size, must be __NR_LRU_PAGE_TYPES
 *
 * @return		E_HM_OK if success, E_HM_INVAL if len not equal to __NR_LRU_PAGE_TYPES
 */
int lru_stat_fill_info(const struct memstat_fs_stat_s *info,
		       uint64_t lru_size[(int)__NR_LRU_PAGE_TYPES][(int)__NR_LRU_LISTS],
		       size_t len);

size_t lru_anon_count_by_type_with_memcg(struct memcg_s *mcg,
					 enum lru_type_e lru_type);
#endif /* SYSMGR_MEMMGR_STAT_LRU_STAT_H */
