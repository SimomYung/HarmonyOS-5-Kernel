/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of slab_utils
 * Author: Huawei OS Kernel Lab
 * Create: Sun Aug 20 15:57:38 UTC 2023
 */

#ifndef __ULIBS_INCLUDE_LIBMEM_SLAB_UTILS_H__
#define __ULIBS_INCLUDE_LIBMEM_SLAB_UTILS_H__

#include <stdint.h>
#include <stdbool.h>
#include <libmem/types.h>

/* max memory order in slab_desc_range which max size is 1 PAGE_SIZE */
#define SLAB_PAGE_SIZE 	(1 << PAGE_SHIFT)
#define SLAB_NAME_LEN 64U
#define SLAB_MAX_LOWER_BOUND	3920U

#ifdef CONFIG_PER_CPU_SLAB
#define SLAB_CACHE_NUM	CONFIG_SLAB_CACHE_NUM
#endif

struct slab_ator;
struct slab_pool;

/* used for slab info statistics */
struct slab_ator_info_s {
	int in_use; /* if in_use is 0 which means this allocator is not used */
	uint32_t slab_page_size;
	uint32_t obj_size;
	uint32_t max_slot_count;
	unsigned int slab_page_hold;
	uint32_t slot_used;
	uint32_t slot_free;
	unsigned int empty_num;
	unsigned int partial_num;
	unsigned int full_num;
	uint32_t flags;
	char name[SLAB_NAME_LEN];
};

struct slab_slot_info_s {
	size_t all_slot_size;
	size_t free_slot_size;
	size_t used_slot_size;
};

struct slab_info_s {
	size_t total_slab_size;
	size_t free_slab_size;
	size_t grown_slab_size;
	size_t real_total_slab_size;
};

enum slab_size_type {
	SLAB_TOTAL_SIZE,
	SLAB_FREE_SIZE,
	SLAB_GROWN_SIZE,
	SLAB_REAL_SIZE,
	SLAB_USED_SIZE,
	SLAB_RECLAIMABLE_SIZE,
	NR_SLAB_SIZE_TYPE,
};

struct slab_cache_info_s {
	uintptr_t *cache_pool;
	uint32_t limit;
};

/* used for slab ator flags */
#define SLAB_SKIP_DEBUG		0x1U // bit0
#define SLAB_STORE_USER		0x2U // bit1
#define SLAB_STORE_STACK	0x4U // bit2
#define SLAB_RECLAIMABLE	0x8U // bit3

int slab_ator_init(struct slab_pool *s_pool, struct slab_ator *s_ator, uint32_t obj_size,
		   const char *name, struct slab_cache_info_s *info, uint32_t s_flags);
struct slab_ator *slab_ator_create(struct slab_pool *s_pool, uint32_t obj_size,
				   const char *name, struct slab_cache_info_s *info,
				   uint32_t s_flags);
int slab_ator_destroy(struct slab_ator *s_ator);

struct slab_pool *slab_pool_create(void);
void slab_pool_destroy(struct slab_pool *s_pool);
int slab_pool_push_memory(struct slab_pool *s_pool, uintptr_t start, size_t size);
uintptr_t slab_pool_pop_memory(struct slab_pool *s_pool);
void slab_pool_extend_lock(struct slab_pool *s_pool);
void slab_pool_extend_unlock(struct slab_pool *s_pool);
void slab_pool_shrink_lock(struct slab_pool *s_pool);
void slab_pool_shrink_unlock(struct slab_pool *s_pool);
void slab_pool_lock(struct slab_pool *s_pool);
void slab_pool_unlock(struct slab_pool *s_pool);

size_t slab_max_malloc(void);
bool slab_report(struct slab_ator *s_ator);
void *slab_alloc(struct slab_ator *s_ator);
void slab_free(struct slab_ator *s_ator, void *obj);

struct slab_ator *slab_obj_to_ator(const void *obj);
struct slab_pool *slab_ator_to_pool(const struct slab_ator *ator);
uint32_t slab_fetch_ator_obj_size(const struct slab_ator *s_ator);

size_t slab_mem_size(struct slab_pool *s_pool, enum slab_size_type types);

void slab_dump_nolock(struct slab_info_s *info, struct slab_pool *s_pool);
void slab_dump_ator(struct slab_ator *ator, struct slab_ator_info_s *ator_info,
		    struct slab_slot_info_s *slot_info);
size_t slab_get_ator_free_size(struct slab_ator *ator);
void slab_dump_pool_desc(const struct slab_pool *pool);
const char *slab_ator_name_of(const struct slab_ator *ator);

bool slab_reclaimable(const struct slab_ator *ator);

int slab_desc_size_of(void);
#endif /* __ULIBS_INCLUDE_LIBMEM_SLAB_UTILS_H__ */
