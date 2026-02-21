/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Allocator for actv slot metadata
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 12 19:57:58 2020
 */
#ifndef LIBUCAP_ACTV_KOBJPOOL_H
#define LIBUCAP_ACTV_KOBJPOOL_H

#include <stdint.h>
#include <stdlib.h>
#include <libhmucap/ucap.h>
#include <hmkernel/capability.h>

#ifndef __hmbuild__
# ifdef CONFIG_SHADOW_STACK
#  ifndef __hmfeature_shadowstack__
#   define __hmfeature_shadowstack__
#  endif
# endif
#endif

#ifndef __hmbuild__
# ifdef CONFIG_SFI
#  ifndef __hmfeature_sfi__
#   define __hmfeature_sfi__
#  endif
# endif
#endif

/*
 * Push it inner definitions into ulibs/libhmucap/kobjpool.h: it is not part of API.
 */
struct actv_kobj_slot;

struct actv_kobj_pool {
	uint32_t nr_actvs;
	void *slot_pool;
};

struct actv_kobj_pool *actv_kobj_pool_init(uint32_t nr_actvs);
struct actv_kobj_slot *actv_kobj_slot_alloc(struct actv_kobj_pool *pool, void *kobj_res);
struct actv_kobj_slot *actv_kobj_slot_search(struct actv_kobj_pool *pool, uint32_t inner_idx);
void actv_kobj_slot_free(struct actv_kobj_pool *pool, uint32_t inner_idx);
void actv_kobj_pool_free_one_slot(struct actv_kobj_pool *pool, struct actv_kobj_slot *slot);
void actv_kobj_res_set(void *actv_kobj_pool, cref_t a_cref, void *kobj_res);

/*
 * This is a simple 'slot->kobj_res'. If we bring struct actv_kobj_slot to API
 * we can make it inline.
 */
void *actv_kobj_pool_res_of(struct actv_kobj_slot *slot);

#if (defined __hmfeature_shadowstack__) || (defined __hmfeature_sfi__)
void actv_kobj_sstack_set(void *actv_kobj_pool, cref_t a_cref, void *sstack);

/*
 * It is a simple slot->sstack.
 */
void *actv_kobj_pool_sstack_of(struct actv_kobj_slot *slot);
#endif

#endif
