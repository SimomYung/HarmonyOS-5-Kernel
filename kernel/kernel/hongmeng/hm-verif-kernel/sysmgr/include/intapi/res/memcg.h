/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Public memcg headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 29 19:31:05 2023
 */

#ifndef SYSMGR_INTAPI_RES_MEMCG_H
#define SYSMGR_INTAPI_RES_MEMCG_H
#include <libmem/utils.h>
#include <libstrict/strict.h>

struct page_s;
struct memcg_s;

#ifdef CONFIG_RESMGR_MEMORY
int memcg_get(struct memcg_s *memcg);
void memcg_put(struct memcg_s *memcg);
#else
static inline int memcg_get(struct memcg_s *memcg)
{
	UNUSED(memcg);
	return E_HM_NOSYS;
}

static inline void memcg_put(struct memcg_s *memcg)
{
	UNUSED(memcg);
}
#endif
/*
 * Do not include any CONFIG, it's used in vfs.
 */
int memcg_fs_charge_by_id(unsigned int memcg_id, unsigned int nr_pages,
			  unsigned int *nr_charged_out);
int memcg_fs_uncharge_by_id(unsigned int memcg_id, unsigned int nr_pages);
int memcg_fs_charge_page(unsigned int cnode_idx, struct page_s *page);

int memcg_fs_charge(struct memcg_s *memcg, unsigned int count, unsigned int *charged);
int memcg_fs_uncharge(struct memcg_s *memcg, unsigned int count);

#ifdef CONFIG_HYPERHOLD
unsigned int get_swap_full_ratio(void);
unsigned int get_zswap_wm_ratio(void);
unsigned int get_compress_ratio(void);
unsigned int hp_inactive_file_ratio_get(void);
unsigned int hp_active_file_ratio_get(void);
#ifdef CONFIG_HYPERHOLD_NC
bool is_nocompress_enable_by_ratio(void);
#else
static inline bool is_nocompress_enable_by_ratio(void)
{
	return false;
}
#endif
#else /* CONFIG_HYPERHOLD */
static inline unsigned int get_swap_full_ratio(void)
{
	return 0U;
}
static inline unsigned int get_zswap_wm_ratio(void)
{
	return 0U;
}
static inline unsigned int get_compress_ratio(void)
{
	return 0U;
}
inline unsigned int hp_inactive_file_ratio_get(void)
{
	return 0U;
}

inline unsigned int hp_active_file_ratio_get(void)
{
	return 0U;
}
#endif /* !CONFIG_HYPERHOLD */

#endif
