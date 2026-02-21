/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Devhost buddy management
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 23 13:32:18 2022
 */

#ifndef ULIBS_INCLUDE_LIBDEVHOST_BUDDY_H
#define ULIBS_INCLUDE_LIBDEVHOST_BUDDY_H

#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/param.h>
#include <libalgo/bitops.h>
#include <libalgo/rbtree.h>
#include <hongmeng/compiler.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/bitops/generic.h>

#define LIBDEVHOST_BUDDY_MAX_ORDER		20U

/*
 * The reachable maximum buddy order is no more than BUDDY_MAX_ORDER,
 * so here defines BUDDY_ORDER_CNT to initialize the buddy page lists
 * and stat arraies in which each order page is calculated repectively.
 */
#define LIBDEVHOST_BUDDY_ORDER_CNT		(LIBDEVHOST_BUDDY_MAX_ORDER + 1U)
#define LIBDEVHOST_PAGE_T_NONE		0
#define LIBDEVHOST_PAGE_T_BUDDY		1
#define LIBDEVHOST_BUDDY_HASH_BITS 6
#define LIBDEVHOST_BUDDY_HASH_SIZE (1 << LIBDEVHOST_BUDDY_HASH_BITS)

struct libdh_buddy_stat_info_s {
	unsigned long nr_free_pages;
	unsigned long page_count[LIBDEVHOST_BUDDY_ORDER_CNT];
};

struct libdh_buddy_stat_s {
	raw_atomic_ullong_t nr_free_pages;
	unsigned long page_count[LIBDEVHOST_BUDDY_ORDER_CNT];
};

struct libdh_buddy_page_s {
	unsigned int type;
	unsigned int order;
	unsigned int zone_type;
	struct rbt_node tnode;
	struct dlist_node dnode_shrink;
};

struct libdh_buddy_free_hash {
	DEFINE_RAW_BITMAP(hashmap, LIBDEVHOST_BUDDY_HASH_SIZE);
	struct rbt_tree hash[LIBDEVHOST_BUDDY_HASH_SIZE];
	unsigned int hash_count[LIBDEVHOST_BUDDY_HASH_SIZE];
};

struct libdh_buddy_s {
	struct raw_mutex mutex;
	unsigned int freemap;
	struct libdh_buddy_free_hash free[LIBDEVHOST_BUDDY_ORDER_CNT];
	struct libdh_buddy_stat_s stat;
	unsigned int page_sizeorder;
	struct dlist_node dnode_shrink;
};

#define LIBDEVHOST_BUDDY_ORDER_LIMIT 32U
raw_static_assert(LIBDEVHOST_BUDDY_MAX_ORDER <= LIBDEVHOST_BUDDY_ORDER_LIMIT, LIBDEVHOST_BUDDY_MAX_ORDER_too_large);

/*
 * Buddy allocator init.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @return void
 */
void libdh_buddy_init(struct libdh_buddy_s *buddy);

/*
 * Buddy allocator status get.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @param outbuf	[O] outbuf
 *
 * @param outbuf_size	[I] size of outbuf
 *
 * @param rsize		[O] size of output in outbuf
 *
 * @return E_HM_OK if operation succeeds.
 */
int libdh_buddy_stat(struct libdh_buddy_s *buddy, char *outbuf,
		     unsigned long outbuf_size, unsigned long *rsize);

/*
 * Buddy allocator info dump.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @return void.
 */
void libdh_buddy_dump(struct libdh_buddy_s *buddy);

/*
 * Buddy allocator alloc mem.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @param order		[I] 2's order of alloc size
 *
 * @param type		[I] buddy memory type
 *
 * @return memory vaddr if operation succeeds.
 */
struct libdh_buddy_page_s *libdh_buddy_alloc(struct libdh_buddy_s *buddy,
			   unsigned int order, unsigned int type);

/*
 * Buddy allocator extend and alloc mem.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @param extend_page	[I] extend buddy page
 *
 * @param alloc_order	[I] 2's order of alloc size
 *
 * @return memory vaddr if operation succeeds.
 */
struct libdh_buddy_page_s *libdh_buddy_extend_alloc(struct libdh_buddy_s *buddy,
						    struct libdh_buddy_page_s *extend_page,
						    unsigned int alloc_order);

/*
 * Buddy allocator free mem.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @param page		[I] memory vaddr to free
 *
 * @return void
 */
void libdh_buddy_free(struct libdh_buddy_s *buddy, struct libdh_buddy_page_s *page);

/*
 * Buddy allocator get page numbers.
 *
 * @param buddy		[I] buddy info ptr
 *
 * @return page numbers
 */
unsigned long libdh_buddy_get_nr_pages(struct libdh_buddy_s *buddy);

typedef int (*buddy_do_shrink_t)(struct libdh_buddy_s *buddy,
				 void *vaddr, unsigned int order);
/*
 * Buddy allocator shrink mem.
 */
unsigned long libdh_buddy_shrinked(struct libdh_buddy_s *buddy, int size,
			 unsigned int page_sizeorder,
			 buddy_do_shrink_t func);

static inline unsigned int libdh_buddy_order_of(unsigned long pfn)
{
	unsigned int pfn_order;

	pfn_order = bit_ctzll(pfn);

	return MIN(pfn_order, LIBDEVHOST_BUDDY_MAX_ORDER);
}
#endif
