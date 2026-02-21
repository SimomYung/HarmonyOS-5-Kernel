/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Swap intapi file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 11 14:52:53 2023
 */
#ifndef __SYSMGR_INTAPI_MEM_ZSWAP_H__
#define __SYSMGR_INTAPI_MEM_ZSWAP_H__

#include <hongmeng/compiler.h>
#include <libstrict/strict.h>

#ifdef CONFIG_SWAP_ZSWAP_BACKEND
#define ZSWAP_BITROT_VERIF_MAX 1U
#ifdef CONFIG_ZSWAP_BITROT_STATISTIC
void zswap_bitrot_verif_set(uint32_t policy);
uint32_t zswap_bitrot_verif_get(void);
#else /* CONFIG_ZSWAP_BITROT_STATISTIC */
static inline void zswap_bitrot_verif_set(uint32_t policy)
{
	UNUSED(policy);
}

static inline uint32_t zswap_bitrot_verif_get(void)
{
	return 0U;
}
#endif
struct zswap_group_s;
#ifdef CONFIG_ZSWAP_GROUP_PROTECT
void zswap_group_protect_enable_readonly(struct zswap_group_s *zgrp);
void zswap_group_protect_disable_readonly(struct zswap_group_s *zgrp);
uintptr_t zswap_group_protect_addr_check(uintptr_t vaddr);
#else
static inline void zswap_group_protect_enable_readonly(__maybe_unused struct zswap_group_s *zgrp) { }
static inline void zswap_group_protect_disable_readonly(__maybe_unused struct zswap_group_s *zgrp) { }
static inline uintptr_t zswap_group_protect_addr_check(__maybe_unused uintptr_t vaddr) { return 0UL; }
#endif

#endif /* CONFIG_SWAP_ZSWAP_BACKEND */

#endif /* __SYSMGR_INTAPI_MEM_ZSWAP_H__ */
