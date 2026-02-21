/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VHASHTABLE_EVICTABLE_H
#define VHASHTABLE_EVICTABLE_H
/*******************************************************************************
 * @file hashtable_evictable.h
 * @ingroup requires_smr lock_free
 * @brief This is a lock-free listset based hashtable with eviction support.
 *
 * This hashtable uses a lock-free listset per bucket. The listset is ordered by
 * keys, and keys are expected to be unique.
 *
 * In addition to the standard hashtable operations, this implementation offers
 * `vhashtable_evict` API. Which allows for evicting/retiring hashtable
 * entries. The eviction policy favors longer buckets and oldest entries within
 * those buckets. Note that the eviction FIFO is not global to the hashtable,
 * but local to the buckets.
 *
 * This feature is useful for controlling the buckets growth and can be useful
 * in cache applications where dropping entries does not affect the soundness of
 * applications.
 *
 * The table consists of `VHASHTABLE_BUCKET_COUNT` number of buckets with a
 * default value of `1024`. This can be overwritten by the user by compiling
 * with `-DVHASHTABLE_BUCKET_COUNT=N`. User control the mapping between keys and
 * buckets by passing `hash_idx` to the APIs. It is expected that users use
 * their own hash functions that map a key to a certain bucket index. Thus
 * the given `hash_idx` is expected to be `< VHASHTABLE_BUCKET_COUNT `.
 *
 * This is a concurrent lock-free implementation and it requires users to deploy
 * a safe memory reclamation scheme in order to free/recycle evicted/detached
 * nodes safely.
 *
 * @example
 * @include eg_hashtable_evictable.c
 ******************************************************************************/

#include <vsync/utils/math.h>
#include <vsync/map/internal/hashtable/bucket_evictable.h>
#include <vsync/map/internal/hashtable/hashtable_config.h>

typedef vbucket_entry_t vhashtable_entry_t;
typedef vlistset_key_t vhashtable_key_t;
typedef int (*vhashtable_cmp_key_t)(vhashtable_entry_t *, vhashtable_key_t);
typedef void (*vhashtable_entry_retire_t)(vhashtable_entry_t *, void *);

typedef struct vhashtable_s {
	vbucket_t buckets[VHASHTABLE_BUCKET_COUNT];
} vhashtable_t;

/**
 * Initializes the hashtable.
 *
 * @param table address of vhashtable_t object.
 * @param evict_cb address of a callback function of type
 * `vhashtable_entry_retire_t`. The function is called whenever an entry is
 * evicted from the hashtable.
 * @param evict_cb_arg address of an extra argument of `evict_cb`.
 * @param cmp_cb address of callback function of type `vhashtable_cmp_key_t`.
 * Used for comparing an entry key to a given key.
 * @note must be called before threads start accessing the hashtable.
 */
static inline void vhashtable_init(vhashtable_t *table,
				   vhashtable_entry_retire_t evict_cb,
				   void *evict_cb_arg,
				   vhashtable_cmp_key_t cmp_cb)
{
	ASSERT(table);
	for (vsize_t i = 0; i < VHASHTABLE_BUCKET_COUNT; i++) {
		/* the cast of the following functions must be safe, as the types are
         * mapped one to one
         * vlistset_key_t is vhashtable_key_t
         * vlistset_node_t is the first member of vbucket_entry_t which is
         * vhashtable_entry_t
         */
		vbucket_init(&table->buckets[i],
			     (vlistset_handle_node_t)evict_cb, evict_cb_arg,
			     (vlistset_cmp_key_t)cmp_cb);
	}
}
/**
 * Evicts all entries in the hashtable.
 *
 * @param table address of vhashtable_t object.
 * @note this function is not thread-safe, can be called iff all threads are
 * done accessing the hashtable.
 */
static inline void vhashtable_destroy(vhashtable_t *table)
{
	ASSERT(table);
	for (vsize_t i = 0; i < VHASHTABLE_BUCKET_COUNT; i++) {
		vbucket_destroy(&table->buckets[i]);
	}
}
/**
 * Inserts the given `entry` into the hashtable.
 *
 * @param table address of vhashtable_t object.
 * @param key value of the key object.
 * @param entry address of vhashtable_entry_t object.
 * @param hash_idx a hash value of the key used as a bucket index. must be
 * `< VHASHTABLE_BUCKET_COUNT`
 * @return true successful insertion.
 * @return false failed insertion, an entry associated with the given `key`
 * already exists.
 * @note this function must be called inside an SMR critical section.
 */
static inline vbool_t vhashtable_insert(vhashtable_t *table,
					vhashtable_key_t key,
					vhashtable_entry_t *entry,
					vsize_t hash_idx)
{
	ASSERT(table);
	ASSERT(entry);
	ASSERT(hash_idx < VHASHTABLE_BUCKET_COUNT);
	return vbucket_insert(&table->buckets[hash_idx], key, entry);
}
/**
 * Removes the entry associated with the given key from the the hashtable.
 *
 * @param table address of vhashtable_t object.
 * @param key value of the key object.
 * @param hash_idx a hash value of the key used as a bucket index. must be
 * `< VHASHTABLE_BUCKET_COUNT`.
 * @return true successful remove, the entry associated with the given key was
 * removed.
 * @return false failed remove, no entry associated with the given key exists.
 */
static inline vbool_t vhashtable_remove(vhashtable_t *table,
					vhashtable_key_t key, vsize_t hash_idx)
{
	ASSERT(table);
	ASSERT(hash_idx < VHASHTABLE_BUCKET_COUNT);
	return vbucket_remove(&table->buckets[hash_idx], key);
}
/**
 * Looks for the entry associated with the given `key`.
 *
 * @param table address of vhashtable_t object.
 * @param key value of the key object.
 * @param hash_idx a hash value of the key used as a bucket index. must be
 * `< VHASHTABLE_BUCKET_COUNT`
 * @return vhashtable_entry_t* address of the
 * `vhashtable_entry_t` object associated with the given key, if found.
 * @return `NULL` if key is not found.
 * @note this function must be called inside an SMR critical section.
 */
static inline vhashtable_entry_t *
vhashtable_get(vhashtable_t *table, vhashtable_key_t key, vsize_t hash_idx)
{
	ASSERT(table);
	ASSERT(hash_idx < VHASHTABLE_BUCKET_COUNT);
	return vbucket_get(&table->buckets[hash_idx], key);
}
/**
 * Tries to evict given `count` number of entries.
 *
 * Eviction from a bucket will favor FIFO, i.e. oldest entries in a bucket will
 * be evicted first. However, the policy is local to the bucket. Buckets with
 * length greater than average will be choosen to evict first. After that there
 * is no guarantee which bucket shall evict first.
 * @param table address of vhashtable_t object.
 * @param count number of entries to evict.
 * @return vuint64_t number of entries actually evicted by the calling thread.
 * @note the return value can be `<= count` this can happen if the hashtable
 * does not have that number of entries. Or a parallel insert/evict operation is
 * running and helped with this eviction. In that case only the number of
 * entries evicted by the calling thread are counted.
 * @note Evicted entries are handed back to the user through the callback
 * registered via `vhashtable_init`. And they are not safe to recycle/free
 * immediately. It is expected that the user deploys an SMR scheme and retires
 * the nodes to the SMR in the said callback.
 * @note this function must be called inside an SMR critical section.
 */
static inline vuint64_t vhashtable_evict(vhashtable_t *table, vuint64_t count)
{
	vuint64_t evict_sum = 0;
	vuint64_t evict_req = 0;
	vuint64_t evict_real = 0;
	vuint64_t len[VHASHTABLE_BUCKET_COUNT] = { 0 };
	vuint64_t sum = 0;
	vuint64_t avg_len = 0;
	vsize_t i = 0;

	ASSERT(table);

	/* record current length of each bucket */
	for (i = 0; i < VHASHTABLE_BUCKET_COUNT; i++) {
		/* len_i = max_i - min_i */
		len[i] = vbucket_get_length(&table->buckets[i]);
		sum += len[i];
	}

	if (sum == 0) {
		/* empty */
		return 0;
	}

	avg_len = sum / VHASHTABLE_BUCKET_COUNT;
	vsize_t last_evicting_idx = 0;
	if (avg_len != 0) {
		/* evict entries from buckets proportional to their current size  */
		for (i = 0; i < VHASHTABLE_BUCKET_COUNT && evict_sum < count;
		     i++) {
			if (len[i] <=
			    avg_len) { /* skip buckets without enough entries */
				continue;
			}

			/* determine how much to take from this bucket */
			evict_req = VMIN(len[i] - avg_len, count - evict_sum);
			evict_real =
				vbucket_evict(&table->buckets[i], evict_req);
			ASSERT(evict_real <= evict_req);
			evict_sum += evict_real;
			last_evicting_idx = i;
		}
	}
	vsize_t idx = last_evicting_idx + 1;
	// at this point we must have truncated all buckets that have length > avg
	vsize_t empty_bucket_count = 0;
	while (evict_sum < count &&
	       empty_bucket_count < VHASHTABLE_BUCKET_COUNT) {
		i = idx++ % VHASHTABLE_BUCKET_COUNT;
		evict_req = 1;
		evict_real = vbucket_evict(&table->buckets[i], evict_req);
		if (evict_real == 0) {
			empty_bucket_count++;
		} else {
			ASSERT(evict_real <= evict_req);
			evict_sum += evict_real;
			/* reset */
			empty_bucket_count = 0;
		}
	}

	return evict_sum;
}
/**
 * Returns the count of entries currently available in the hashtable.
 *
 * @param table address of vhashtable_t object.
 * @return vuint64_t an approximate count of the entries available in the
 * hashtable.
 */
static inline vuint64_t vhashtable_get_entries_count(vhashtable_t *table)
{
	vuint64_t len = 0;
	ASSERT(table);
	for (vsize_t i = 0; i < VHASHTABLE_BUCKET_COUNT; i++) {
		len += vbucket_get_length(&table->buckets[i]);
	}
	return len;
}
#endif
