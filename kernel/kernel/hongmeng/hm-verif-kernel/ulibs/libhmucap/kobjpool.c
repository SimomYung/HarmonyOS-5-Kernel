/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of ulibs kobjpool
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 25 17:08:54 2019
 */
#include <libhmucap/kobjpool.h>

#include <stdlib.h>
#include <hmasm/page.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libmem/utils.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex_guard.h>

struct kobj_allocator {
	struct dlist_node head;
	struct dlist_node head_used;
	struct raw_mutex mutex;
	uint64_t default_size;
	uint64_t total_size;
	uint32_t free_pmem;
	uint32_t pmem_water;
};

#define DEFINE_KOBJ_ALLOCATOR(type, name, size, water)	\
static struct kobj_allocator name##_allocator = {		\
	.head = {						\
		&name##_allocator.head,				\
		&name##_allocator.head,				\
	},							\
	.head_used = { 						\
		&name##_allocator.head_used, 			\
		&name##_allocator.head_used, 			\
	}, 							\
	.mutex = RAW_MUTEX_INITIALIZER,				\
	.default_size = (size),					\
	.pmem_water = (water),					\
	.total_size = 0,					\
	.free_pmem = 0,						\
};

#include "kobjs_list.h"

#undef DEFINE_KOBJ_ALLOCATOR

static struct kobj_allocator *allocators[(int)hmobj_MAX] = {
#define DEFINE_KOBJ_ALLOCATOR(type, name, size, water)	\
	[type] = &name##_allocator,
#include "kobjs_list.h"
#undef DEFINE_KOBJ_ALLOCATOR
};

struct kobj_pool {
	struct rbt_tree tree;
	struct raw_mutex mutex;
};

static struct kobj_pool self_pmem_pool = {
	.tree = RBT_ROOT,
	.mutex = RAW_MUTEX_INITIALIZER
};

DEFINE_rbt_search(kobj_slot, struct kobj_slot, node, kobj)

size_t kobj_slot_size(void)
{
	return sizeof(struct kobj_slot);
}

static struct kobj_pmem *
alloc_kobj_pmem(cref_t cnode, uint64_t size)
{
	struct kobj_pmem *mem;

	mem = malloc(sizeof(struct kobj_pmem));
	if (mem != NULL) {
		mem->mem.page = NULL;
		mem->mem.pmem = ucap_default_ops.alloc_pmem(cnode, size, &mem->mem);
		if (IS_REF_ERR(mem->mem.pmem)) {
			hm_warn("failed to alloc pmem, err=%s\n", hmstrerror(REF_TO_ERR(mem->mem.pmem)));
			free(mem);
			mem = NULL;
		} else {
			mem->total_size = size;
			mem->remain_size = size;
			dlist_init(&mem->node);
		}
	}

	return mem;
}

static bool __has_a_kobj_pmem(cref_t cnode, const struct kobj_allocator *allocator,
			      struct kobj_pmem **pmem, uint64_t size)
{
	bool found = false;
	struct kobj_pmem *mem = NULL;

	dlist_for_each_entry(mem, &allocator->head, struct kobj_pmem, node) {
		if (mem->mem.cnode != cnode) {
			continue;
		}
		if (mem->slot_size == size && mem->remain_size >= size) {
			found = true;
			*pmem = mem;
			break;
		}
	}

	return found;
}

static void do_alloc_slot_from_pmem(struct kobj_allocator *allocator, struct kobj_pmem *mem, uint64_t size)
{
	if (mem->total_size == mem->remain_size) {
		BUG_ON(allocator->free_pmem == 0);
		allocator->free_pmem--;
	}
	mem->remain_size -= size;
	if (mem->remain_size == 0) {
		/* when there is no remaining slot, put the node in the used list */
		dlist_delete(&mem->node);
		dlist_insert(&allocator->head_used, &mem->node);
	}
}

static void free_kobj_pmem(cref_t cnode, struct kobj_pmem *mem);
static struct kobj_pmem *
alloc_slot_from_pmem(cref_t cnode, struct kobj_allocator *allocator, uint64_t size)
{
	bool found = true;
	struct kobj_pmem *mem = NULL;
	struct kobj_pmem *tmp = NULL;
	uint64_t alloc_size = allocator->default_size;

	raw_mutex_lock(&allocator->mutex);
	found = __has_a_kobj_pmem(cnode, allocator, &mem, size);
	if (found) {
		do_alloc_slot_from_pmem(allocator, mem, size);
	} else {
		BUG_ON(allocator->free_pmem != 0);
	}
	raw_mutex_unlock(&allocator->mutex);

	if (!found) {
		if (size > alloc_size) {
			alloc_size = SIZE_TO_POWEROF2_PAGE_SIZE(size);
		}

		tmp = alloc_kobj_pmem(cnode, alloc_size);
		if (tmp == NULL) {
			mem = NULL;
		} else {
			tmp->allocator = allocator;
			tmp->slot_size = size;
			tmp->mem.cnode = cnode;

			raw_mutex_lock(&allocator->mutex);
			found = __has_a_kobj_pmem(cnode, allocator, &mem, size);
			if (unlikely(found)) {
				free_kobj_pmem(cnode, tmp);
			} else {
				mem = tmp;
				allocator->free_pmem++;
				dlist_insert(&allocator->head, &mem->node);
				allocator->total_size += mem->total_size;
			}

			do_alloc_slot_from_pmem(allocator, mem, size);
			raw_mutex_unlock(&allocator->mutex);
		}
	}

	return mem;
}

/* size should be aligned with power of 2 */
struct kobj_pmem *
kobj_alloc_pmem_slot(cref_t cnode, enum hmobj type, uint64_t size)
{
	return alloc_slot_from_pmem(cnode, allocators[type], size);
}

struct kobj_slot *
kobj_alloc_slot(cref_t cnode, enum hmobj type, uint64_t size)
{
	bool err = false;
	uint64_t aligned_size;
	struct kobj_slot *slot = NULL;
	cref_t real_cnode = cnode;

	if (type >= hmobj_MAX || allocators[type] == NULL) {
		err = true;
	}

	if (!err && size == 0ULL) {
		err = true;
	}

	if (!err) {
		slot = (struct kobj_slot *)kobj_slot_malloc();
		if (slot == NULL) {
			err = true;
		}
	}

	if (!err && cnode == 0ULL) {
		real_cnode = hm_ucap_self_cnode_cref();
	}

	if (!err) {
		/* align kernel object size  */
		aligned_size = (uint64_t)__CAP_ALIGN_KOBJ((unsigned long)size);

		slot->pmem = alloc_slot_from_pmem(real_cnode, allocators[type],
						  aligned_size);
		if (slot->pmem == NULL) {
			hm_warn("failed to alloc pmem slot\n");
			kobj_slot_free(slot);
			slot = NULL;
		} else {
			slot->kobj = ERR_TO_REF(E_HM_INVAL);
		}
	}
	return slot;
}

static struct kobj_slot *
remove_kobj_slot(struct kobj_pool *pool, cref_t kobj)
{
	struct kobj_slot *slot = NULL;

	RAW_MUTEX_GUARD(_, &pool->mutex);

	slot = kobj_slot_rbt_search(&pool->tree, kobj, NULL);
	if (slot != NULL) {
		rbt_remove(&pool->tree, &slot->node);
	}

	return slot;
}

#ifdef __HOST_LLT__
void llt_clean_kobj_pool_tree(void)
{
	struct kobj_pool *pool = NULL;
	struct kobj_slot *slot = NULL;
	struct rbt_node *rbt_node = NULL;

	pool = &self_pmem_pool;

	RAW_MUTEX_GUARD(_, &pool->mutex);

	/* Remove all slot node in pool */
	rbt_for_each_entry_safe(slot, rbt_node, &pool->tree,
				struct kobj_slot, node) {
		rbt_remove(&pool->tree, &slot->node);
		kobj_free_slot(0, slot);
	}
}
#endif

static struct kobj_slot *
search_kobj_slot(struct kobj_pool *pool, cref_t kobj)
{
	struct kobj_slot *slot = NULL;

	RAW_MUTEX_GUARD(_, &pool->mutex);
	slot = kobj_slot_rbt_search(&pool->tree, kobj, NULL);
	return slot;
}

static int
insert_kobj_slot(struct kobj_pool *pool, struct kobj_slot *slot)
{
	int err = E_HM_OK;
	struct rbt_node *parent_node = NULL;
	struct kobj_slot *old = NULL;
	bool is_rchild = false;

	RAW_MUTEX_GUARD(_, &pool->mutex);

	old = kobj_slot_rbt_search(&pool->tree, slot->kobj, &parent_node);
	if (old != NULL) {
		hm_warn("failed to insert kobj pmem, kobj has existed, kobj=%"PRIxref"\n", slot->kobj);
		err = E_HM_INVAL;
	} else {
		if (parent_node != NULL) {
			struct kobj_slot *parent = rbt_entry(parent_node, struct kobj_slot, node);

			if (parent->kobj < slot->kobj) {
				is_rchild = true;
			}
		}

		rbt_insert(&pool->tree, &slot->node, parent_node, is_rchild);
	}

	return err;
}

int kobj_record_slot(cref_t kobj, struct kobj_slot *slot)
{
	int err = E_HM_OK;

	if (slot != NULL) {
		slot->kobj = kobj;
		err = insert_kobj_slot(&self_pmem_pool, slot);
	}

	return err;
}

static void free_kobj_pmem(cref_t cnode, struct kobj_pmem *mem)
{
	int err;

	err = ucap_default_ops.free_pmem(cnode, &mem->mem);
	if (err < 0) {
		hm_warn("failed to free pmem, lead to memory leak, err=%s\n", hmstrerror(err));
	}

	free(mem);
}

void kobj_free_pmem_slot(cref_t cnode, struct kobj_pmem *mem)
{
	struct kobj_allocator *allocator = mem->allocator;

	raw_mutex_lock(&allocator->mutex);
	if (mem->remain_size == 0u) {
		/* memory is available again, put the node back in the main list */
		dlist_delete(&mem->node);
		dlist_insert(&allocator->head, &mem->node);
	}
	mem->remain_size += mem->slot_size;
	if (mem->remain_size == mem->total_size) {
		if (++allocator->free_pmem > allocator->pmem_water) {
			dlist_delete(&mem->node);
			allocator->total_size -= mem->total_size;
			allocator->free_pmem--;
			raw_mutex_unlock(&allocator->mutex);
			free_kobj_pmem(cnode, mem);
		} else {
			raw_mutex_unlock(&allocator->mutex);
		}
	} else {
		raw_mutex_unlock(&allocator->mutex);
	}
}

static void free_slot_to_pmem(cref_t cnode, struct kobj_slot *slot)
{
	struct kobj_pmem *mem = slot->pmem;

	kobj_free_pmem_slot(cnode, mem);
}

void kobj_free_slot(cref_t cnode, struct kobj_slot *slot)
{
	cref_t real_cnode = (cnode == 0ULL ? hm_ucap_self_cnode_cref() : cnode);

	if (slot == NULL) {
		hm_warn("bad args, slot is NULL\n");
	} else if(slot->pmem == NULL) {
		hm_warn("bad args, slot->pmem is NULL\n");
	} else if (slot->pmem->allocator == NULL) {
		hm_warn("bad args, slot->pmem->allocator is NULL\n");
	} else {
		free_slot_to_pmem(real_cnode, slot);
		kobj_slot_free(slot);
	}
}

/* remove slot from pool */
struct kobj_slot *kobj_remove_slot(cref_t kobj)
{
	return remove_kobj_slot(&self_pmem_pool, kobj);
}

struct kobj_slot *kobj_search_slot(cref_t kobj)
{
	return search_kobj_slot(&self_pmem_pool, kobj);
}

uint64_t kobj_alloc_size_dump(void)
{
	unsigned int i;
	uint64_t total = 0;
	struct kobj_allocator *allocator = NULL;

	for (i = 0; i < (unsigned int)hmobj_MAX; i++) {
		allocator = allocators[i];
		if (allocator == NULL) {
			continue;
		}
		RAW_MUTEX_GUARD(_, &allocator->mutex);
		total += allocator->total_size;
	}
	return total;
}

static uint64_t kobj_alloc_size_dump_no_mutex(void)
{
	unsigned int i;
	uint64_t total = 0;
	struct kobj_allocator *allocator = NULL;

	for (i = 0; i < (unsigned int)hmobj_MAX; i++) {
		allocator = allocators[i];
		if (allocator == NULL) {
			continue;
		}
		total += allocator->total_size;
	}

	return total;
}

uint64_t hm_ucap_kobj_alloc_size_dump_oom(void)
{
	const uint64_t result = kobj_alloc_size_dump_no_mutex();
	return result;
}

void kobj_allocators_lock(void)
{
	unsigned int i;
	struct kobj_allocator *allocator = NULL;

	for (i = 0; i < (unsigned int)hmobj_MAX; i++) {
		allocator = allocators[i];
		if (allocator == NULL) {
			continue;
		}

		raw_mutex_lock(&allocator->mutex);
	}
	raw_mutex_lock(&self_pmem_pool.mutex);
}

void kobj_allocators_unlock(void)
{
	int i;
	struct kobj_allocator *allocator = NULL;

	raw_mutex_unlock(&self_pmem_pool.mutex);

	for (i = (int)hmobj_MAX - 1; i >= 0; i--) {
		allocator = allocators[i];
		if (allocator == NULL) {
			continue;
		}

		raw_mutex_unlock(&allocator->mutex);
	}
}
