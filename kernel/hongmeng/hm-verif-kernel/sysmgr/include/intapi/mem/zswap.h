/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Swap intapi file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 11 14:52:53 2023
 */
#ifndef __SYSMGR_INTAPI_MEM_ZSWAP_H__
#define __SYSMGR_INTAPI_MEM_ZSWAP_H__

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#ifdef CONFIG_SWAP_ZSWAP_BACKEND
void zswap_disable_item_lock(void);
void zswap_enable_item_lock(void);

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

#endif /* CONFIG_SWAP_ZSWAP_BACKEND */

#endif /* __SYSMGR_INTAPI_MEM_ZSWAP_H__ */
