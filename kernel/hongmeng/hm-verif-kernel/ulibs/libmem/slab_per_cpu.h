/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file for per-cpu slab feature
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 25 11:49:27 2023
 */
#ifndef _ULIBS_LIBMEM_SLAB_PER_CPU_H_
#define _ULIBS_LIBMEM_SLAB_PER_CPU_H_

#include <stdbool.h>
#include <stdint.h>

#include "slab.h"

void *slab_alloc_per_cpu(struct slab_ator *s_ator);
void free_slab_obj_per_cpu(struct slab_ator *s_ator, void *obj);

void slab_ator_cache_setup(struct slab_ator *s_ator, uintptr_t *cache_pool, uint32_t limit);
size_t slab_cache_pool_size(const uint32_t *array, size_t array_len);
uint32_t slab_ator_cache_limit(uint32_t obj_size);
void slab_cpu_num_setup(uint32_t num_cpus);
void ator_cache_clear(struct slab_ator *s_ator);

#endif
