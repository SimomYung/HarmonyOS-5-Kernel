/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef GDUMP_SYNC_H
#define GDUMP_SYNC_H
/**
 * Initializes the given `gd` object.
 *
 * @param gd address of gdump_t object.
 */
static inline void _gdump_init(gdump_t *gd)
{
	vsize_t i = 0;
	for (i = 0; i < VGDUMP_MAX_SYNCERS; i++) {
		vatomicptr_init(&gd->sync_cache[i], NULL);
	}
}
/**
 * Publishes the calling thread's cache.
 *
 * Finds an empty slot of the communication channels with de-registering
 * threads. Writes the address of the given cache in it.
 *
 * If no empty slot is found, the calling thread spins until one becomes
 * available.
 *
 * @param gd address of gdump_t object.
 * @param cache address of gdump_cache_t object associated with the calling
 * thread.
 * @return vatomicptr_t* address of the reserved slot where the cache address
 * has been published.
 *
 * @note can be blocking, if all slots are occupied, then the calling thread
 * spins till a slot becomes vacant.
 * @note should not be called inside a CS of a lock. Mutual exclusion is not
 * required for cache reservation.
 */
#define GDUMP_YIELD_MAX 500
#define GDUMP_SLEEP_USEC 100
static inline vatomicptr_t *_gdump_sync_cache_reserve(gdump_t *gd,
						      gdump_cache_t *cache,
						      gdump_yield_fun_t yield,
						      gdump_delay_fun_t delay_time)
{
	vsize_t i = 0;
	vbool_t published = false;
	unsigned int cnt = 0U;

	while (true) {
		for (i = 0; i < VGDUMP_MAX_SYNCERS; i++) {
			published = vatomicptr_cmpxchg(&gd->sync_cache[i], NULL,
						       cache) == NULL;
			if (published) {
				return &gd->sync_cache[i];
			}
		}
		if (cnt <= GDUMP_YIELD_MAX) {
			cnt++;
			yield();
		} else {
			delay_time(GDUMP_SLEEP_USEC);
		}
		verification_ignore();
	}
}
/**
 * Takes the previously published cache address offline.
 *
 * @param slot address of the slot where the cache has been previously
 * published.
 */
static inline void _gdump_sync_cache_release(vatomicptr_t *slot)
{
	vatomicptr_write(slot, NULL);
}
/**
 * Finds an active thread starting from curr.
 *
 * @param curr address of gdump_thread_t object.
 * @return gdump_thread_t* address of an active thread.
 * @return NULL there are no active threads left.
 */
static inline gdump_thread_t *_gdump_get_active_thread(gdump_thread_t *curr)
{
	gdump_cache_entry_t dummy_entry;
	gdump_thread_t *succ = NULL;
	vbool_t active = false;

	succ = curr;
	while (succ && !active) {
		curr = succ;
		succ = (gdump_thread_t *)gdump_list_get_next(
			(gdump_list_node_t *)curr);
		/* the dummy entry is discarded here */
		active = _gdump_cache_and_check(curr, &dummy_entry);
		/* as long as we don't find a thread that is active */
	}
	return curr;
}
/**
 * Caches `N` active threads in the given cache, or all active threads if fewer
 * than `N` are active.
 *
 * Where `N` is the predefined cache capacity. It also writes the address of the
 * next active thread in cache->successor.
 *
 * @param gd address of gdump_t object.
 * @param cache address of gdump_cache_t object.
 */
static inline void _gdump_cache_active(gdump_t *gd, gdump_cache_t *cache)
{
	gdump_thread_t *thread = NULL;
	gdump_list_node_t *curr = NULL;
	gdump_cache_entry_t record;
	gdump_list_iter_t iter;

	ASSERT(gd);
	ASSERT(cache);
	/* make sure this is the first time the cache is used! */
	ASSERT(cache->len == 0);
	ASSERT(cache->successor == NULL);

	gdump_list_iter_init(&iter, &gd->threads);
	while (curr = gdump_list_iter_next(&iter), curr) {
		if (_gdump_cache_is_full(cache)) {
			break;
		}
		thread = (gdump_thread_t *)curr;
		if (_gdump_cache_and_check(thread, &record)) {
			_gdump_cache_insert(cache, &record);
		}
	}

	/*
     * Optimization =>
     * Instead of doing:
     * cache->successor = (gdump_thread_t *)curr;
     * We avoid pointing to a thread that is inactive, because
     * that thread might become active later when we
     * `_gdump_scan_update_cache`, making us wait even though
     * we already (right now) observed a quiescent state.
     * Instead we traverse till we find a thread that is active
     * (and hence actually needs waiting for)
     * and keep its address.
     */
	thread = (gdump_thread_t *)curr;
	ASSERT(!thread || !_gdump_cache_exists(cache, thread));
	cache->successor = _gdump_get_active_thread(thread);
}
/**
 * Overwrites the given entry and updates the successor in the
 * cache.
 *
 * Starting from the current successor traverse the registered
 * threads' list. Look for an active thread and cache it
 * overwriting the given entry then update the successor.
 *
 * If no active thread is found, the successor is updated to `NULL`.
 *
 * @param cache address of gdump_cache_t object.
 * @param entry address of gdump_cache_entry_t object to overwrite.
 *
 * @return true an active thread was found, entry is overwritten and
 * successor is updated.
 * @return false no active thread is found, entry is not overwritten and
 * successor was set to `NULL`.
 */
static inline vbool_t _gdump_overwrite_entry(gdump_cache_t *cache,
					     gdump_cache_entry_t *entry)
{
	gdump_thread_t *thread = NULL;
	gdump_list_node_t *curr = NULL;
	gdump_list_node_t *succ = NULL;
	gdump_cache_entry_t record;
	gdump_list_iter_t iter;

	ASSERT(cache);
	ASSERT(entry);

	if (cache->successor) {
		gdump_list_iter_init_from(&iter, &cache->successor->lst_node);
		while (curr = gdump_list_iter_next(&iter), curr) {
			thread = (gdump_thread_t *)curr;
			/* find an active entry to continue from */
			if (_gdump_cache_and_check(thread, &record)) {
				/* found an active entry, we copy it */
				_gdump_copy_entry(entry, &record);
				succ = gdump_list_get_next(&thread->lst_node);
				cache->successor = _gdump_get_active_thread(
					(gdump_thread_t *)succ);
				return true;
			}
		}
		/* no active thread was found */
		cache->successor = NULL;
	}
	return false;
}
/**
 * Removes all ties of the given `thread` object from the given `cache`
 * object.
 *
 * Ejects any cached entry that points to the given thread object, and
 * replaces it if possible.
 *
 * @param cache address of gdump_cache_t object.
 * @param thread address of gdump_thread_t object.
 */
static inline void _gdump_remove_from_cache(gdump_cache_t *cache,
					    gdump_thread_t *thread)
{
	vsize_t i = 0;
	gdump_cache_entry_t *entry = NULL;

	ASSERT(cache);
	ASSERT(thread);
	for (i = 0; i < _gdump_cache_entry_count(cache); i++) {
		entry = _gdump_cache_get(cache, i);
		if (entry->thread == thread) {
			if (!_gdump_overwrite_entry(cache, entry)) {
				/* This entry was not replaced, because there is no active
                 * replacement available. Eject it */
				_gdump_cache_eject(cache, i);
			}
			/* There can be no multiple entries associated with this thread,
             * so we break the loop */
			break;
		}
	}
}
/**
 * Removes all ties of the given `node` object from all published caches in
 * gdump `gd` object.
 *
 * @param gd address of gdump_t object.
 * @param node address of gdump_thread_t object.
 */
static inline void _gdump_remove_from_caches(gdump_t *gd, gdump_thread_t *node)
{
	gdump_cache_t *cache = NULL;
	gdump_list_node_t *snode = NULL;

	for (vsize_t i = 0; i < VGDUMP_MAX_SYNCERS; i++) {
		cache = vatomicptr_read(&gd->sync_cache[i]);
		/* if it is a cache and there is a successor */
		if (cache != NULL) {
			snode = &cache->successor->lst_node;
			if (cache->successor == node) {
				cache->successor =
					(gdump_thread_t *)gdump_list_get_next(
						snode);
			} else {
				_gdump_remove_from_cache(cache, node);
			}
		}
	}
}
/**
 * Checks which of the caches entries has been quiescent.
 *
 * If a thread associated with an entry has been quiescent since it was last
 * observed, its entry is replaced by the active successor. If there is no
 * such successor, the entry is ejected and the cache shrinks.
 *
 * @param cache address of gdump_cache_t object.
 */
static inline void _gdump_scan_update_cache(gdump_cache_t *cache)
{
	vsize_t idx = 0;
	vbool_t overwritten = false;
	gdump_cache_entry_t *entry = NULL;
	vbool_t ghost_progressed = false;

	/* go through each cache entry, and decide whether to keep it or
     * replace it. */
	while (idx < _gdump_cache_entry_count(cache)) {
		/* get cache entry at */
		entry = _gdump_cache_get(cache, idx);
		/* check if the thread has been quiescent if so replace it */
		if (_gdump_has_been_quiescent(entry)) {
			/* The thread has been quiescent, we try to replace it with
             * another active thread from the non-cached list if possible.
             */
			overwritten = _gdump_overwrite_entry(cache, entry);
			if (!overwritten) {
				/* no replacement was found, we eject the current quiescent
                 * entry */
				_gdump_cache_eject(cache, idx);
			}
			ghost_progressed = true;
		} else {
			/* we still need to wait on this thread, however, we can
             * meanwhile check next thread */
			idx++;
		}
	}
	/* we want to consider the executions only where progress have been made
     */
	verification_assume(ghost_progressed);
	V_UNUSED(ghost_progressed);
}
/**
 * Waits for one grace period complete.
 *
 * Keeps polling until all registered threads are witnessed in at least one
 * quiescent state.
 *
 * @param gd address of gdump_t object.
 * @param yield address of a function with the same sematics as
 * `pthread_yield`.
 *
 * @note releases the lock before each yield and reacquiring it after. This
 * helps preventing starvation of de-registering threads. Please note that
 * not yielding/sleeping in between lock-acquisitions can also lead to
 * starvation of de-registering threads.
 */
static inline void gdump_sync(gdump_t *gd, gdump_yield_fun_t yield, gdump_delay_fun_t delay_time)
{
	vatomicptr_t *cache_slot = NULL;
	gdump_cache_t cache;
	unsigned int cnt = 0;

	ASSERT(gd);
	vatomic_fence();
	_gdump_cache_init(&cache);
	/* we reserve our slot outside the CS of the lock. The call can block
     * for a while if there are more parallel syncs than slots. */
	cache_slot = _gdump_sync_cache_reserve(gd, &cache, yield, delay_time);
	/* !!! ACQUIRE !!! */
	gd->lock.read_acq(gd->lock.arg);
	/* cache up to N active threads, N is the maximum gdump cache capacity
     */
	_gdump_cache_active(gd, &cache);
	/* wait on all active threads to be observed in at least on quiescent
     * state (i.e. exited the CS that they were observed in) We keep polling
     * until the cache is empty.
     */
	while (!_gdump_cache_is_empty(&cache)) {
		/* check all cached threads and cache new ones to replace those are
         * done. The cache must be updated inside the lock's critical
         * section */
		_gdump_scan_update_cache(&cache);
		/* we made one full scan we can give it a break, and then we
         * reattempt the scan */
		/* !!! RELEASE !!! */
		gd->lock.read_rel(gd->lock.arg);
		if (cnt <= GDUMP_YIELD_MAX) {
			cnt++;
			yield();
		} else {
			delay_time(GDUMP_SLEEP_USEC);
		}
		/* !!! ACQUIRE !!! */
		gd->lock.read_acq(gd->lock.arg);
	}
	ASSERT(cache.successor == NULL);
	/* must be done under lock protection */
	_gdump_sync_cache_release(cache_slot);
	/* !!! RELEASE !!! */
	gd->lock.read_rel(gd->lock.arg);
}
#endif
