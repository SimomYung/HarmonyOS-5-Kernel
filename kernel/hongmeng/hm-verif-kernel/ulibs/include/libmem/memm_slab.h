/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Memm slab function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 26 22:33:34 2019
 */
#ifndef ULIBS_INCLUDE_LIBMEM_MEMM_SLAB_H
#define ULIBS_INCLUDE_LIBMEM_MEMM_SLAB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <libmem/slab_utils.h>
#include <libmem/types.h>
#include <libstrict/strict.h>

#define MEMM_SLAB_EXTEND_ORDER	SIZE_TO_MEM_ORDER(SLAB_PAGE_SIZE)

#define MEMM_SLAB_LOWER_MARK	(0U * SLAB_PAGE_SIZE)
/* extend number of SLAB PAGE */
#define MEMM_SLAB_EXTEND_MARK	(8U * SLAB_PAGE_SIZE)
/* shrink number of SLAB PAGE */
#define MEMM_SLAB_SHRINK_MARK	(16U * SLAB_PAGE_SIZE)

/*
 * number of max supported slab allocator index
 * the index of number is used to g_size_to_ator_table
 */
#define MEMM_ROOT_SLAB_ATOR_NUM 27U

/* NOTE: This is the used for sysdump */
#define MEMM_SLAB_ALL_ATOR_INFO_NUM	MEMM_ROOT_SLAB_ATOR_NUM
/*
 * vregion is frequently allocated(freed). According to its aligned slot size(224),
 * the slab_ator with size 224 is responsible for it. Therefore, we set larger
 * cache volume for this slab_ator.
 */
#ifdef CONFIG_MEM_PURGEABLE
#define MEMM_SLAB_SIZE_OF_VREGION 296
#else
#define MEMM_SLAB_SIZE_OF_VREGION 288
#endif

struct memm_slab_ator_info {
	struct slab_ator_info_s ator_info;
};

struct memm_slab_info {
	struct slab_info_s slab_info;
	struct slab_slot_info_s slot_info;
	struct memm_slab_ator_info sized_ator_info[MEMM_SLAB_ALL_ATOR_INFO_NUM];
};

typedef void (*slab_cb_s)(uintptr_t slab_desc);
/*
 * Callback function when slab error found
 * slab_desc: the buggy slab
 */
void memm_slab_error_cb(uintptr_t slab_desc);
void memm_slab_init(uintptr_t start, size_t size, size_t num_cpus, slab_cb_s cb);
#ifdef CONFIG_SLAB_FREELIST_RANDOM
void memm_slab_update_random(unsigned long random_value);
#else
static inline void memm_slab_update_random(unsigned long random_value)
{
	UNUSED(random_value);
}
#endif
bool memm_slab_report(size_t size);
void *memm_slab_alloc(size_t size);
void memm_slab_free(const void *obj);

size_t memm_slab_alloc_usable_size(const void *obj);

size_t memm_slab_size_type_nolock(enum slab_size_type type);
size_t memm_slab_size_type(enum slab_size_type type);

int memm_slab_extend(uintptr_t start, size_t size);
uintptr_t memm_slab_shrink(void);

void memm_slab_dump(struct memm_slab_info *info);
void memm_slab_dump_pool(struct memm_slab_info *info);
void memm_slab_dump_pool_nolock(struct memm_slab_info *info);
size_t memm_slab_get_ator_free_size(void);

void memm_slab_extend_pool_lock(void);
void memm_slab_extend_pool_unlock(void);
void memm_slab_shrink_pool_lock(void);
int memm_slab_shrink_pool_trylock(void);
void memm_slab_shrink_pool_unlock(void);
#endif	/* ULIBS_INCLUDE_LIBMEM_MEMM_SLAB_H */
