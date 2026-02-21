/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: AARCH64 Cache Model
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 09 19:12:45 2021
 */

#ifndef AARCH64_ASM_CACHEMODEL_H
#define AARCH64_ASM_CACHEMODEL_H

/*
 * AARCH64 Cache Model
 * ===================
 *
 * AARCH64 cache maintain methods.
 * These methods are implemented in arch/aarch/lib/cache_asm.S.
 *
 * a64_cache_d_invalidate_all_poc()
 * a64_cache_d_clean_all_poc()
 * a64_cache_d_flush_all_poc()
 *
 *   Invalidate/clean/flush(clean+invalidate) the entire data cache or unified
 *   cache to the Point of Coherency. Note these methods cannot be used to ensure
 *   coherence based on the following reasons: a. they only affect caches local
 *   to a CPU (not other CPUs or system caches) b. they are not atomic. They are
 *   limited to use for power-up and power-down cache management.
 */
extern void a64_cache_d_invalidate_all_poc(void);
extern void a64_cache_d_clean_all_poc(void);
extern void a64_cache_d_flush_all_poc(void);

#endif
