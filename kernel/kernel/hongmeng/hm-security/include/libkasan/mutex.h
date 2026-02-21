/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Simple notrace mutex
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 26 11:37:33 2021
 */

#ifndef KASAN_MUTEX_H
#define KASAN_MUTEX_H

#include <libhmsync/raw_mutex.h>

#define CACHE_BLOCK_SIZE 64
#define CACHE_ALIGNED __attribute__((aligned(CACHE_BLOCK_SIZE)))
#define ALWAYS_INLINE __attribute__((always_inline)) static inline

struct kasan_mutex {
	struct raw_mutex mutex;
} CACHE_ALIGNED;

typedef struct kasan_mutex kasan_mutex_t;

ALWAYS_INLINE void kasan_mutex_init(kasan_mutex_t *m)
{
	raw_mutex_init_recursive(&m->mutex);
}

ALWAYS_INLINE void kasan_mutex_lock(kasan_mutex_t *m)
{
	raw_mutex_lock(&m->mutex);
}

ALWAYS_INLINE void kasan_mutex_unlock(kasan_mutex_t *m)
{
	raw_mutex_unlock(&m->mutex);
}

#endif /* KASAN_MUTEX_H */
