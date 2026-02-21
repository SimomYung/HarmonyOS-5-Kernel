/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef GDUMP_CACHE_H
#define GDUMP_CACHE_H

#include <vsync/common/assert.h>
#include <vsync/vtypes.h>
#include <vsync/smr/internal/gdump_types.h>

#ifndef GDUMP_CACHE_CAPACITY
#if defined(VSYNC_VERIFICATION)
#define GDUMP_CACHE_CAPACITY 1U
#else
#define GDUMP_CACHE_CAPACITY 10U
#endif
#endif

typedef struct gdump_cache_entry_s {
	vuint64_t timestamp;
	gdump_thread_t *thread;
} gdump_cache_entry_t;

typedef struct gdump_cache_s {
	gdump_cache_entry_t entries[GDUMP_CACHE_CAPACITY];
	vsize_t len;
	gdump_thread_t *successor;
} gdump_cache_t;

/**
 * Returns true if thread is stored in the cache.
 *
 * @param cache address of gdump_cache_t object.
 * @param thread address of gdump_thread_t object, to look for.
 * @return true thread is stored in cache.
 * @return false thread is not stored in cache.
 */
static inline vbool_t _gdump_cache_exists(gdump_cache_t *cache,
					  gdump_thread_t *thread)
{
	if (thread == cache->successor) {
		return true;
	}

	for (vsize_t i = 0; i < cache->len; i++) {
		if (cache->entries[i].thread == thread) {
			return true;
		}
	}

	return false;
}

static inline void _gdump_cache_init(gdump_cache_t *cache)
{
	ASSERT(cache);
	cache->len = 0;
	cache->successor = NULL;
}

static inline vbool_t _gdump_cache_is_full(gdump_cache_t *cache)
{
	ASSERT(cache->len <= GDUMP_CACHE_CAPACITY);
	return (cache->len >= GDUMP_CACHE_CAPACITY);
}

static inline vbool_t _gdump_cache_is_empty(gdump_cache_t *cache)
{
	ASSERT(cache);
	ASSERT(cache->len <= GDUMP_CACHE_CAPACITY);
	return cache->len == 0;
}

static inline vsize_t _gdump_cache_entry_count(gdump_cache_t *cache)
{
	ASSERT(cache);
	ASSERT(cache->len <= GDUMP_CACHE_CAPACITY);
	return cache->len;
}

static inline void _gdump_copy_entry(gdump_cache_entry_t *des,
				     gdump_cache_entry_t *src)
{
	ASSERT(des);
	ASSERT(src);
	/* copy one by one because of verification bug */
	des->thread = src->thread;
	des->timestamp = src->timestamp;
}

static inline vbool_t _gdump_cache_eject(gdump_cache_t *cache, vsize_t idx)
{
	ASSERT(cache);
	if (cache->len > 0 && idx < cache->len) {
		_gdump_copy_entry(&cache->entries[idx],
				  &cache->entries[cache->len - 1]);
		cache->len--;
		return true;
	}
	return false;
}

static inline vbool_t _gdump_cache_insert(gdump_cache_t *cache,
					  gdump_cache_entry_t *entry)
{
	ASSERT(cache);
	if (cache->len < GDUMP_CACHE_CAPACITY) {
		_gdump_copy_entry(&cache->entries[cache->len], entry);
		cache->len++;
		return true;
	} else {
		return false;
	}
}

static inline gdump_cache_entry_t *_gdump_cache_get(gdump_cache_t *cache,
						    vsize_t idx)
{
	ASSERT(cache);
	ASSERT(idx < GDUMP_CACHE_CAPACITY);
	if (idx < cache->len) {
		return &cache->entries[idx];
	}
	return NULL;
}

#undef GDUMP_CACHE_CAPACITY
#endif
