/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VBUCKET_H
#define VBUCKET_H

#include <vsync/common/dbg.h>
#include <vsync/map/listset_lf.h>
#include <vsync/common/macros.h>
#include <vsync/common/compiler.h>
#include <vsync/utils/math.h>

typedef struct vbucket_node_s {
	vlistset_node_t
		lst_node; /* Important: this field must stay the top field,
                     container bucket node must have the same
                     address as the embeded listset node
                     According to the standard:
                     "Within a structure object, the non-bit-field members and
                     the units in which bit-fields reside have addresses that
                     increase in the order in which they are declared. A pointer
                     to a structure object, suitably converted, points to its
                     initial member (or if that member is a bit-field, then to
                     the unit in which it resides), and vice versa. There may be
                     unnamed padding within a structure object, but not at its
                     beginning."
                     */
	vuint64_t timestamp;
} vbucket_entry_t;

typedef struct vbucket_s {
	vlistset_t lst;
	vatomic64_t min_stamp;
	vatomic64_t max_stamp;
	vlistset_handle_node_t evict_cb;
	void *evict_cb_arg;
} vbucket_t;

/**
 * Eviction callback propagates retirement and increments min timestamp.
 *
 * @param lst_node address of vlistset_node_t object.
 * @param args address of vbucket_t object.
 */
static inline void _vbucket_detach_callback(vlistset_node_t *lst_node,
					    void *args)
{
	ASSERT(args);
	vbucket_t *bkt = args;
	bkt->evict_cb(lst_node, bkt->evict_cb_arg);
	vatomic64_inc_rlx(&bkt->min_stamp);
}

static inline void vbucket_init(vbucket_t *bkt, vlistset_handle_node_t evict_cb,
				void *evict_cb_arg, vlistset_cmp_key_t cmp_cb)
{
	vlistset_t *lst = &bkt->lst;
	bkt->evict_cb = evict_cb;
	bkt->evict_cb_arg = evict_cb_arg;
	vlistset_init(lst, _vbucket_detach_callback, bkt, cmp_cb);
	vatomic64_write_rlx(&bkt->min_stamp, 0);
	vatomic64_write_rlx(&bkt->max_stamp, 0);
}

static inline vbool_t vbucket_insert(vbucket_t *bkt, vlistset_key_t key,
				     vbucket_entry_t *val)
{
	vbool_t success = false;
	vlistset_t *lst = &bkt->lst;
	vuint64_t timestamp = vatomic64_read_rlx(&bkt->max_stamp);
	val->timestamp = timestamp;
	success = vlistset_add(lst, key, &val->lst_node);
	if (success) {
		vatomic64_inc_rlx(&bkt->max_stamp);
	}

	return success;
}

static inline vbucket_entry_t *vbucket_get(vbucket_t *bkt, vlistset_key_t key)
{
	return (vbucket_entry_t *)vlistset_get(&bkt->lst, key);
}

static inline vbool_t vbucket_remove(vbucket_t *bkt, vlistset_key_t key)
{
	return vlistset_remove(&bkt->lst, key);
}
/**
 * Returns the number of entries.
 *
 * @param bkt address of vbucket_t object.
 * @return vuint64_t number of entries.
 * @note this is an approximation.
 */
static inline vuint64_t vbucket_get_length(vbucket_t *bkt)
{
	vuint64_t min = vatomic64_read_rlx(&bkt->min_stamp);
	vuint64_t max = vatomic64_read_rlx(&bkt->max_stamp);

	if (max >= min) {
		return max - min;
	}
	/* max can be less than min, because of relaxed accesses */
	return 0;
}
/**
 * Destroys all nodes in the bucket.
 *
 * @param bkt address of vbucket_t object
 */
static inline void vbucket_destroy(vbucket_t *bkt)
{
	vlistset_destroy(&bkt->lst);
}
/**
 * Evicts the oldest `count` number of entries if possible.
 *
 * @param bkt address of vbucket_t object
 * @param count number of entries to evict
 * @return vuint64_t number of entries actually evicted
 */
static inline vuint64_t vbucket_evict(vbucket_t *bkt, vuint64_t count)
{
	vlistset_t *lst = NULL;
	vlistset_node_t *pred = NULL;
	vlistset_node_t *curr = NULL;
	vlistset_node_t *succ = NULL;
	vlistset_node_t *tail = NULL;
	vbucket_entry_t *entry = NULL;
	vuint64_t detach_count = 0;
	vuint64_t min = 0;
	vuint64_t max = 0;
	vuint64_t range_min = 0;
	vuint64_t range_max = 0;
	vbool_t marked = false;
	vbool_t snip = false;

	ASSERT(bkt);

	if (unlikely(count == 0)) {
		return 0;
	}

	min = vatomic64_read_rlx(&bkt->min_stamp);
	max = vatomic64_read_rlx(&bkt->max_stamp);
	if (min >= max) {
		/* bucket is empty */
		return 0;
	}

	range_min = 0;
	range_max = min + count - 1;

	lst = &bkt->lst;
	tail = &lst->tail_sentinel;
RETRY:
	pred = &lst->head_sentinel;
	curr = vatomicptr_markable_get_pointer(&pred->next);

	while (curr != tail && detach_count < count) {
		succ = vatomicptr_markable_get(&curr->next, &marked);
		/* check if current is marked */
		while (marked) {
			snip = vatomicptr_markable_cmpxchg(&pred->next, curr,
							   false, succ, false);
			verification_assume(snip);
			if (!snip) {
				goto RETRY;
			}

			_vbucket_detach_callback(curr, bkt);
			detach_count++;

			/* if we already reached the target or finished the whole list quit
             */
			if (detach_count >= count || succ == tail) {
				return detach_count;
			}

			curr = succ;
			succ = vatomicptr_markable_get(&curr->next, &marked);
		} /* as long as we encounter marked nodes */
		entry = V_CONTAINER_OF(curr, vbucket_entry_t, lst_node);
		/* check if entry is within eviction range timestamp in
         * [range_min, range_max] */
		if (VIN_RANGE(range_min, entry->timestamp, range_max)) {
			/* logically mark curr as removed (curr->next = succ:true) */
			marked = vatomicptr_markable_cmpxchg(&curr->next, succ,
							     false, succ, true);
			if (marked) {
				/* physically detach curr (pred->next = succ) */
				snip = vatomicptr_markable_cmpxchg(
					&pred->next, curr, false, succ, false);
				if (snip) {
					_vbucket_detach_callback(curr, bkt);
					detach_count++;
					/* since we succeeded in removing the node the pred shall
                     * stay the same and the curr should become the succ */
					curr = succ;
					continue;
				}
			}
			/* within range, but either the marking failed (marked = false) or
             * the physical detachment failed (snip = false). In both cases we
             * want to retry and for verification we can ignore because neither
             * changes memory */
			verification_ignore();
			goto RETRY;
		} /* if within range */
		/* proceed to the next node */
		pred = curr;
		curr = succ;
	}
	/* as long as we did not reach the end of the list nor reached target
         eviction count */
	return detach_count;
}

#endif
