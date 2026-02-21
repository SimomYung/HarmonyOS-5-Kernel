/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Devhost buddy management
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 23 13:30:15 2022
 */
#include <hmasm/page.h>
#include <devhost/buddy.h>
#include <libhwsecurec/securec.h>
#include <libalgo/hash.h>
#include <libhmsync/raw_mutex_guard.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>

/*
 * buddy algorithm: buddy figure in mem
 *
 * There are many pages with different orders in the buddy
 * ranging from idx_start to idx_end
 * every page and its buddy page have a distinct index based on idx
 * and page and its buddy page can be merge into a order + 1 page when they are free

 * dlist_node free[BUDDY_ORDER_CNT]
 * -----------------    ---    ---    ---    ---    ---
 * |       0       |--->| |--->| |--->| |--->| |--->| |   2^0 page sized block
 * -----------------    ---    ---    ---    ---    ---
 * |       1       |
 * -----------------
 * |       2       |
 * -----------------
 * |       3       |
 * -----------------
 * |       4       |
 * -----------------    --------    --------    --------
 * |       5       |--->|      |--->|      |--->|      |  2^5 page sized block
 * -----------------    --------    --------    --------
 * |       6       |
 * -----------------
 * |       7       |
 * -----------------
 * |       8       |
 * -----------------
 * |      ...      |
 * -----------------
 * |BUDDY_MAX_ORDER|
 * -----------------
 */

static struct libdh_buddy_page_s *
__buddy_tree_find(const struct rbt_tree *tree, const struct libdh_buddy_page_s *page)
{
	struct rbt_node *root = tree->root;
	struct libdh_buddy_page_s *cur_page = NULL;

	while (root != NULL) {
		cur_page = rbt_entry(root, struct libdh_buddy_page_s, tnode);
		if (ptr_to_ulong(page) > ptr_to_ulong(cur_page)) {
			if (root->rbt_right == NULL) {
				break;
			}
			root = root->rbt_right;
		} else if (ptr_to_ulong(page) < ptr_to_ulong(cur_page)) {
			if (root->rbt_left == NULL) {
				break;
			}
			root = root->rbt_left;
		} else {
			return cur_page;
		}
	}

	return NULL;
}

/*
 * Insert a page into tree sorted by its address.
 */
static int __buddy_tree_insert(struct rbt_tree *tree,
			       struct libdh_buddy_page_s *page)
{
	struct rbt_node *root = tree->root;
	struct libdh_buddy_page_s *cur_page = NULL;
	bool is_right_child = false;

	while (root != NULL) {
		cur_page = rbt_entry(root, struct libdh_buddy_page_s, tnode);
		if (ptr_to_ulong(page) > ptr_to_ulong(cur_page)) {
			if (root->rbt_right == NULL) {
				is_right_child = true;
				break;
			}
			root = root->rbt_right;
		} else if (ptr_to_ulong(page) < ptr_to_ulong(cur_page)) {
			if (root->rbt_left == NULL) {
				break;
			}
			root = root->rbt_left;
		} else {
			return -EEXIST;
		}
	}

	rbt_insert(tree, &page->tnode, root, is_right_child);

	return 0;
}

void libdh_buddy_init(struct libdh_buddy_s *buddy)
{
	unsigned int idx;
	unsigned int hash_idx;

	BUG_ON(buddy == NULL);
	raw_mutex_init(&buddy->mutex);
	(void)raw_atomic_ullong_set_relaxed(&buddy->stat.nr_free_pages, 0ULL);
	buddy->freemap = 0U;
	for (idx = 0U; idx < LIBDEVHOST_BUDDY_ORDER_CNT; idx++) {
		buddy->stat.page_count[idx] = 0U;
		mem_zero_a(buddy->free[idx].hashmap);
		for (hash_idx = 0U; hash_idx < LIBDEVHOST_BUDDY_HASH_SIZE; hash_idx++) {
			rbt_tree_init(&buddy->free[idx].hash[hash_idx]);
			buddy->free[idx].hash_count[hash_idx] = 0;
		}
	}
	dlist_init(&buddy->dnode_shrink);
}

#define libdh_page_to_idx(buddy, page) ((unsigned long)((ptr_to_ulong(page)) >> (buddy)->page_sizeorder))
#define libdh_idx_to_page(buddy, idx)  (ulong_to_ptr(((idx) << (buddy)->page_sizeorder), struct libdh_buddy_page_s))

static void libdh_buddy_enqueue_page(struct libdh_buddy_s *buddy, struct libdh_buddy_page_s *page)
{
	unsigned int order = page->order;
	unsigned long long idx;
	uint32_t hash_idx;
	int ret;

	idx = (unsigned long long)libdh_page_to_idx(buddy, page);
	hash_idx = hash_64(idx, LIBDEVHOST_BUDDY_HASH_BITS);

	ret = __buddy_tree_insert(&buddy->free[order].hash[hash_idx], page);
	BUG_ON(ret != 0, "buddy page insert failed.");

	buddy->free[order].hash_count[hash_idx]++;
	(void)raw_bitmap_set_bit(buddy->free[order].hashmap, LIBDEVHOST_BUDDY_HASH_SIZE, hash_idx);

	buddy->freemap |= 1U << order;
	buddy->stat.page_count[order]++;
}

static void libdh_buddy_dequeue_page(struct libdh_buddy_s *buddy, struct libdh_buddy_page_s *page)
{
	unsigned int order = page->order;
	unsigned long long idx;
	uint32_t hash_idx;

	idx = (unsigned long long)libdh_page_to_idx(buddy, page);
	hash_idx = hash_64(idx, LIBDEVHOST_BUDDY_HASH_BITS);

	rbt_remove(&buddy->free[order].hash[hash_idx], &page->tnode);
	buddy->free[order].hash_count[hash_idx]--;
	if (buddy->free[order].hash_count[hash_idx] == 0) {
		(void)raw_bitmap_clear_bit(buddy->free[order].hashmap, LIBDEVHOST_BUDDY_HASH_SIZE, hash_idx);
	}

	buddy->stat.page_count[order]--;
	if (buddy->stat.page_count[order] == 0) {
		buddy->freemap &= ~(1U << order);
	}
}

static void libdh_page_split(struct libdh_buddy_s *buddy, unsigned int high_order,
		       unsigned int order, struct libdh_buddy_page_s *page)
{
	unsigned int cur_order = high_order;
	size_t offset = 1UL << high_order;
	struct libdh_buddy_page_s *split_page = NULL;
	unsigned long split_idx;

	while (cur_order > order) {
		cur_order--;
		offset = offset >> 1U;
		split_idx = libdh_page_to_idx(buddy, page) + offset;
		split_page = libdh_idx_to_page(buddy, split_idx);
		split_page->zone_type = page->zone_type;
		split_page->type = LIBDEVHOST_PAGE_T_BUDDY;
		split_page->order = cur_order;
		libdh_buddy_enqueue_page(buddy, split_page);
		dlist_insert_tail(&buddy->dnode_shrink, &split_page->dnode_shrink);
	}
	page->order = order;
}

/* Caller ensures parameter validity */
static void libdh_buddy_alloc_from_page(struct libdh_buddy_s *buddy, struct libdh_buddy_page_s *page,
			   unsigned int order, unsigned int type)
{
	libdh_buddy_dequeue_page(buddy, page);
	dlist_delete(&page->dnode_shrink);
	libdh_page_split(buddy, page->order, order, page);
	page->type = type;
	(void)raw_atomic_ullong_sub_relaxed(&buddy->stat.nr_free_pages,
					    1ULL << order);
}

struct libdh_buddy_page_s *libdh_buddy_extend_alloc(struct libdh_buddy_s *buddy,
						    struct libdh_buddy_page_s *extend_page,
						    unsigned int alloc_order)
{
	unsigned int extend_order;
	struct libdh_buddy_page_s *alloc_page = NULL;

	BUG_ON(buddy == NULL || extend_page == NULL);
	BUG_ON(extend_page->order >= LIBDEVHOST_BUDDY_ORDER_CNT);

	extend_order = extend_page->order;
	if (alloc_order == extend_order) {
		alloc_page = extend_page;
	} else if (alloc_order > extend_order) {
		libdh_buddy_free(buddy, extend_page);
	} else {
		alloc_page = extend_page;
		RAW_MUTEX_GUARD(_, &buddy->mutex);
		libdh_page_split(buddy, extend_order, alloc_order, alloc_page);
		(void)raw_atomic_ullong_add_relaxed(&buddy->stat.nr_free_pages,
						    ((1ULL << extend_order) - (1ULL << alloc_order)));
	}

	return alloc_page;
}

struct libdh_buddy_page_s *libdh_buddy_alloc(struct libdh_buddy_s *buddy,
			   unsigned int order, unsigned int type)
{
	struct rbt_tree *free_tree = NULL;
	struct libdh_buddy_page_s *page = NULL;
	unsigned int curr_order;
	unsigned int freemap;
	unsigned long hash_idx;

	BUG_ON(buddy == NULL);
	BUG_ON(order >= LIBDEVHOST_BUDDY_ORDER_CNT);

	RAW_MUTEX_GUARD(_, &buddy->mutex);
	/* Clear bits less than order */
	freemap = buddy->freemap & ~((1U << order) - 1U);
	curr_order = (unsigned int)__builtin_ffs((int)freemap);
	if (likely(curr_order != 0U)) {
		hash_idx = raw_bitmap_find_first_one(buddy->free[curr_order - 1U].hashmap,
						     LIBDEVHOST_BUDDY_HASH_SIZE);
		if (likely(hash_idx != LIBDEVHOST_BUDDY_HASH_SIZE)) {
			free_tree =
			    &buddy->free[curr_order - 1U].hash[hash_idx];
			BUG_ON(free_tree->root == NULL);
			page = rbt_entry(free_tree->root,
					 struct libdh_buddy_page_s, tnode);
			libdh_buddy_alloc_from_page(buddy, page, order, type);
		}
	}

	return page;
}

static bool libdh_check_page_combine(const struct libdh_buddy_page_s *page,
				     const struct libdh_buddy_page_s *buddy_page)
{
	bool can_merge = true;

	/* check buddy index invalid */
	if (page->zone_type != buddy_page->zone_type) {
		can_merge = false;
	} else if (buddy_page->type != LIBDEVHOST_PAGE_T_BUDDY) {
		can_merge = false;
	} else if (page->order != buddy_page->order) {
		can_merge = false;
	} else {
		/* Do nothing */
	}

	return can_merge;
}

static bool libdh_find_page(const struct libdh_buddy_s *buddy,
			    unsigned int order,
			    const struct libdh_buddy_page_s *buddy_page,
			    unsigned long idx)
{
	struct libdh_buddy_page_s *page;
	uint32_t hash_idx;

	hash_idx = hash_64(idx, LIBDEVHOST_BUDDY_HASH_BITS);
	page =
	    __buddy_tree_find(&buddy->free[order].hash[hash_idx], buddy_page);

	return page != NULL;
}

static struct libdh_buddy_page_s *libdh_find_buddy_page(const struct libdh_buddy_s *buddy,
							const struct libdh_buddy_page_s *page)
{
	struct libdh_buddy_page_s *buddy_page = NULL;
	unsigned long pg_idx, buddy_pg_idx;

	pg_idx = libdh_page_to_idx(buddy, page);
	buddy_pg_idx = (pg_idx ^ (1UL << page->order));
	buddy_page = libdh_idx_to_page(buddy, buddy_pg_idx);
	if (libdh_find_page(buddy, page->order, buddy_page, buddy_pg_idx) &&
	    libdh_check_page_combine(page, buddy_page)) {
		return buddy_page;
	}
	return NULL;
}

static struct libdh_buddy_page_s *libdh_combine_buddy_page(struct libdh_buddy_s *buddy,
							   struct libdh_buddy_page_s *page,
							   struct libdh_buddy_page_s *buddy_page)
{
	struct libdh_buddy_page_s *combine_page = NULL;
	unsigned long pg_idx, combine_idx;

	/* remove buddy page from dlist */
	libdh_buddy_dequeue_page(buddy, buddy_page);
	dlist_delete(&buddy_page->dnode_shrink);
	/* get combine page */
	pg_idx = libdh_page_to_idx(buddy, page);
	combine_idx = (pg_idx & (~(1ULL << page->order)));
	combine_page = libdh_idx_to_page(buddy, combine_idx);
	/* update combine page stat */
	page->type = LIBDEVHOST_PAGE_T_NONE;
	buddy_page->type = LIBDEVHOST_PAGE_T_NONE;
	combine_page->order = page->order + 1;
	combine_page->type = LIBDEVHOST_PAGE_T_BUDDY;

	return libdh_idx_to_page(buddy, combine_idx);
}

void libdh_buddy_free(struct libdh_buddy_s *buddy, struct libdh_buddy_page_s *page)
{
	struct libdh_buddy_page_s *buddy_page = NULL;
	unsigned int type;

	BUG_ON(page == NULL);
	type = page->type;
	BUG_ON(buddy == NULL);
	BUG_ON(type != LIBDEVHOST_PAGE_T_BUDDY);
	BUG_ON(page->order >= LIBDEVHOST_BUDDY_ORDER_CNT,
	       "order: 0x%x\n", page->order);
	RAW_MUTEX_GUARD(_, &buddy->mutex);
	(void)raw_atomic_ullong_add_relaxed(&buddy->stat.nr_free_pages,
					    1ULL << page->order);
	page->type = LIBDEVHOST_PAGE_T_BUDDY;
	while (page->order < LIBDEVHOST_BUDDY_ORDER_CNT) {
		buddy_page = libdh_find_buddy_page(buddy, page);
		if (buddy_page == NULL) {
			break;
		}
		page = libdh_combine_buddy_page(buddy, page, buddy_page);
	}
	libdh_buddy_enqueue_page(buddy, page);
	dlist_insert_tail(&buddy->dnode_shrink, &page->dnode_shrink);
}

#define SHRINK_ALL_SIZE (-1)
unsigned long libdh_buddy_shrinked(struct libdh_buddy_s *buddy, int size,
			 unsigned int page_sizeorder,
			 buddy_do_shrink_t func)
{
	struct libdh_buddy_page_s *cur = NULL;
	unsigned long cur_order;
	unsigned long target_size = 0;
	unsigned long work_size = 0;
	int ret = 0;

	BUG_ON(buddy == NULL);
	BUG_ON(page_sizeorder != PAGE_SHIFT);
	BUG_ON(size < 0 && size !=  SHRINK_ALL_SIZE);

	if (size == SHRINK_ALL_SIZE) {
		work_size = (unsigned long)ULONG_MAX;
	} else {
		work_size = (unsigned long)size;
	}

	while (target_size < work_size) {
		raw_mutex_lock(&buddy->mutex);
		if (dlist_empty(&buddy->dnode_shrink)) {
			raw_mutex_unlock(&buddy->mutex);
			break;
		}

		cur = dlist_first_entry(&buddy->dnode_shrink, struct libdh_buddy_page_s, dnode_shrink);
		libdh_buddy_dequeue_page(buddy, cur);
		dlist_delete(&cur->dnode_shrink);
		cur_order = cur->order;
		raw_mutex_unlock(&buddy->mutex);
		ret = func(buddy, (void *)cur, cur_order);
		if (ret == 0) {
			(void)raw_atomic_ullong_sub_relaxed(&buddy->stat.nr_free_pages,
							    1ULL << cur_order);
			target_size += 1UL << (cur_order + page_sizeorder);
		} else {
			/* back to buddy free */
			raw_mutex_lock(&buddy->mutex);
			libdh_buddy_enqueue_page(buddy, cur);
			raw_mutex_unlock(&buddy->mutex);
		}
	}

	return target_size;
}

unsigned long libdh_buddy_get_nr_pages(struct libdh_buddy_s *buddy)
{
	unsigned long nr_pages;

	BUG_ON(buddy == NULL);

	nr_pages = (unsigned long)raw_atomic_ullong_read_relaxed(&buddy->stat.nr_free_pages);
	return nr_pages;
}

int libdh_buddy_stat(struct libdh_buddy_s *buddy, char *outbuf, unsigned long outbuf_size, unsigned long *rsize)
{
	unsigned int index;
	unsigned long nr_free_pages;
	unsigned long page_count;
	int ret;

	BUG_ON(buddy == NULL);
	*rsize = 0;

	ret = sprintf_s(outbuf + *rsize, outbuf_size - *rsize, "%s\n", "buddy info:");
	if (ret < 0) {
		return E_HM_FBIG;
	}
	*rsize += (unsigned long)(unsigned int)ret;
	nr_free_pages = (unsigned long)raw_atomic_ullong_read_relaxed(&buddy->stat.nr_free_pages);
	ret = sprintf_s(outbuf + *rsize, outbuf_size - *rsize, "%-16s%-20lu\n%-16s%lu B\n",
				" nr_free_pages:", nr_free_pages,
				" free size:", nr_free_pages * (1UL << buddy->page_sizeorder));
	if (ret < 0) {
		 return E_HM_FBIG;
	}
	*rsize += (unsigned long)(unsigned int)ret;
	for (index = 0; index < LIBDEVHOST_BUDDY_ORDER_CNT; index++) {
		page_count = buddy->stat.page_count[index];
		if (page_count == 0) continue;
		ret = sprintf_s(outbuf + *rsize, outbuf_size - *rsize, "%-16s%-4u%-20lu\n",
					" page_count:", index, page_count);
		if (ret < 0) {
			return E_HM_FBIG;
		}
		*rsize += (unsigned long)(unsigned int)ret;
	}
	ret = sprintf_s(outbuf + *rsize, outbuf_size - *rsize, "\n");
	if (ret < 0) {
		return E_HM_FBIG;
	}
	*rsize += (unsigned long)(unsigned int)ret;
	return 0;
}
void libdh_buddy_dump(struct libdh_buddy_s *buddy)
{
	unsigned int index;
	unsigned long nr_free_pages;
	unsigned long page_count;
	BUG_ON(buddy == NULL);

	RAW_MUTEX_GUARD(_, &buddy->mutex);
	hm_info("%s\n", "buddy info:");
	nr_free_pages = (unsigned long)raw_atomic_ullong_read_relaxed(&buddy->stat.nr_free_pages);
	hm_info("%-16s%#-20lx\n%-16s%#-20lx\n", " nr_free_pages:", nr_free_pages,
						" free size:", nr_free_pages * (1UL << buddy->page_sizeorder));
	for (index = 0; index < LIBDEVHOST_BUDDY_ORDER_CNT; index++) {
		page_count = buddy->stat.page_count[index];
		if (page_count == 0) continue;
		hm_info("%-16s%-4u%#-20lx\n", " page_count:", index, page_count);
	}
	hm_info("\n");
}
