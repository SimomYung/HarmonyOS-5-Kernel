/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Direct memory access interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <udk/mm.h>

#include <limits.h>

#include <libmem/utils.h>
#include <libalgo/rbtree.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmkernel/mm/pgtable.h>
#include <libdevhost/mm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#include <devhost/memory.h>

#include <udk/log.h>
#include <udk/cache.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/bitops.h>
#include <udk/rwlock.h>
#include <udk/mempool.h>
#include <internal/init.h>

#define DMA_MEMPOOL_DEFAULT_BITMAP		CONFIG_UDK_DMA_MEMPOOL_BITMAP
#define DMA_MEMPOOL_ORDER_MAX			16

struct dma_mem_rbt {
	struct rbt_tree tree;
	struct udk_mutex mutex;
};

struct dma_mem_entry {
	const void *vaddr;
	unsigned long long paddr;
	unsigned long size;
	int data;
	struct rbt_node node;
};

struct dma_mempool {
	int refcnt;
	struct udk_mempool *mempool;
	struct dma_mem_rbt mp_rbt_root;
};

struct dma_mempool_center {
	unsigned int bitmap;
	struct dma_mempool *mempools[DMA_MEMPOOL_ORDER_MAX];
	struct udk_mutex lock;
};

static struct dma_mempool_center dma_mempools;
static struct dma_mem_rbt dma_mem_rbt_root;

static inline bool is_dma_mempool_fixed(unsigned int order)
{
	return udk_test_bit(dma_mempools.bitmap, order);
}

static inline void dma_mem_entry_lock(struct dma_mem_rbt *root)
{
	udk_mutex_lock(&root->mutex);
}

static inline void dma_mem_entry_unlock(struct dma_mem_rbt *root)
{
	udk_mutex_unlock(&root->mutex);
}

static inline void dma_mempools_lock_init(void)
{
	udk_mutex_init(&dma_mempools.lock);
}

static inline void dma_mempools_lock(void)
{
	udk_mutex_lock(&dma_mempools.lock);
}

static inline void dma_mempools_unlock(void)
{
	udk_mutex_unlock(&dma_mempools.lock);
}

static inline bool vaddr_in_range(const struct dma_mem_entry *entry,
				  const void *vaddr)
{
	unsigned long va = ptr_to_ulong(vaddr);
	unsigned long start = ptr_to_ulong(entry->vaddr);
	unsigned long end = start + entry->size;
	BUG_ON(end < start);

	return ((va >= start) && (va < end));
}

static inline bool paddr_in_range(const struct dma_mem_entry *entry,
				  unsigned long long paddr)
{
	unsigned long long start = entry->paddr;
	unsigned long long end = start + entry->size;
	BUG_ON(end < start);

	return ((paddr >= start) && (paddr < end));
}

static struct dma_mem_entry *dma_mem_entry_lookup(struct dma_mem_rbt *root,
						  const void *vaddr,
						  struct rbt_node **parent,
						  bool *is_right)
{
	bool right = false;
	struct rbt_node *m = root->tree.root;
	struct rbt_node *n = NULL;
	struct dma_mem_entry *pos = NULL;

	while (m != NULL) {
		pos = rbt_entry(m, struct dma_mem_entry, node);
		if (vaddr_in_range(pos, vaddr)) {
			return pos;
		} else if (ptr_to_ulong(vaddr) < ptr_to_ulong(pos->vaddr)) {
			if (m->rbt_left == NULL) {
				n = m;
				right = false;
			}
			m = m->rbt_left;
		} else {
			if (m->rbt_right == NULL) {
				n = m;
				right = true;
			}
			m = m->rbt_right;
		}
	}
	if (parent != NULL) {
		*parent = n;
	}
	if (is_right != NULL) {
		*is_right = right;
	}

	return NULL;
}

static int dma_mem_entry_add(struct dma_mem_rbt *root, const void *vaddr,
			     unsigned long long paddr, unsigned long size,
			     int data)
{
	bool is_right = false;
	struct rbt_node *parent = NULL;
	struct dma_mem_entry *entry = NULL;

	entry = dma_mem_entry_lookup(root, vaddr, &parent, &is_right);
	if (entry != NULL) {
		udk_error("dma mem entry exist\n");
		return -EEXIST;
	}
	entry = (struct dma_mem_entry *)udk_malloc(sizeof(*entry));
	if (entry == NULL) {
		udk_error("dma mem entry malloc falied\n");
		return -ENOMEM;
	}
	mem_zero_s(*entry);
	entry->vaddr = vaddr;
	entry->paddr = paddr;
	entry->size = size;
	entry->data = data;
	rbt_insert(&root->tree, &entry->node, parent, is_right);

	return 0;
}

static void dma_mem_entry_remove(struct dma_mem_rbt *root,
				 struct dma_mem_entry *entry)
{
	entry->vaddr = NULL;
	entry->size = 0UL;
	rbt_remove(&root->tree, &entry->node);
	udk_free(entry);
}

static void *dma_mem_alloc(struct dma_mem_rbt *rbt_root, unsigned long size,
			   uint32_t flag, unsigned long long *paddr)
{
	UNUSED(flag);

	int err;
	int shm_id = 0;
	unsigned long long pa;
	void *vaddr = NULL;
	uint32_t mprot = MPROT_READ | MPROT_WRITE | MPROT_NOCACHE | MPROT_DMA;

	vaddr = devhost_backend_mem_alloc(size, mprot, &pa, &shm_id);
	if (vaddr == MAP_FAILED) {
		udk_error("backend mem allocate failed\n");
		return NULL;
	}
	if (!PAGE_ALIGNED(vaddr)) {
		udk_error("vaddr is not page aligned\n");
		devhost_backend_mem_free(shm_id, vaddr, size);
		return NULL;
	}
	dma_mem_entry_lock(rbt_root);
	err = dma_mem_entry_add(rbt_root, vaddr, pa, size, shm_id);
	if (err < 0) {
		udk_error("dma mempool add failed: %s\n", strerror(-err));
		dma_mem_entry_unlock(rbt_root);
		devhost_backend_mem_free(shm_id, vaddr, size);
		return NULL;
	}
	dma_mem_entry_unlock(rbt_root);
	if (paddr != NULL) {
		*paddr = pa;
	}
	/* invalidate cache */
	udk_dma_sync_dev_to_cpu(ptr_to_ulong(vaddr),
				ptr_to_ulong(vaddr) + size);

	return vaddr;
}

static int dma_mem_free(struct dma_mem_rbt *rbt_root, const void *vaddr)
{
	struct dma_mem_entry *entry = NULL;

	dma_mem_entry_lock(rbt_root);
	entry = dma_mem_entry_lookup(rbt_root, vaddr, NULL, NULL);
	if (entry == NULL) {
		udk_warn("dma mempool entry not found\n");
		dma_mem_entry_unlock(rbt_root);
		return -ENOENT;
	}
	devhost_backend_mem_free(entry->data, (void *)entry->vaddr,
				 entry->size);
	dma_mem_entry_remove(rbt_root, entry);
	dma_mem_entry_unlock(rbt_root);

	return 0;
}

static inline void dma_mem_rbt_init(struct dma_mem_rbt *rbt_root)
{
	rbt_tree_init(&rbt_root->tree);
	udk_mutex_init(&rbt_root->mutex);
}

static int dma_mempool_create(unsigned int order)
{
	struct dma_mempool *dma_mp = NULL;
	struct udk_mempool *mempool = NULL;

	if (dma_mempools.mempools[order] != NULL) {
		udk_error("dma mempool[%u] is occupied\n", order);
		return -EEXIST;
	}
	mempool = udk_mempool_create(udk_order_to_size(order));
	if (mempool == NULL) {
		udk_error("dma mempool create failed\n");
		return -ENOENT;
	}
	dma_mp = (struct dma_mempool *)udk_malloc(sizeof(*dma_mp));
	if (dma_mp == NULL) {
		udk_error("dma mempool malloc failed\n");
		udk_mempool_destroy(mempool);
		return -ENOMEM;
	}
	mem_zero_s(*dma_mp);
	dma_mp->refcnt = 0;
	dma_mp->mempool = mempool;
	dma_mem_rbt_init(&dma_mp->mp_rbt_root);
	dma_mempools.mempools[order] = dma_mp;

	return 0;
}

static inline struct dma_mempool *dma_mempool_lookup(unsigned int order)
{
	return dma_mempools.mempools[order];
}

static void dma_mempool_destroy(unsigned int order, bool fixed)
{
	struct dma_mempool *dma_mp = NULL;

	/* do not destroy default mempools */
	if (is_dma_mempool_fixed(order) == fixed) {
		/* make sure it is unlock before */
		dma_mp = dma_mempool_lookup(order);
		if (dma_mp == NULL) {
			return;
		}
		/* mempool is busy, do not destroy mempool */
		if (dma_mp->refcnt > 0) {
			return;
		}
		udk_mempool_destroy(dma_mp->mempool);
		dma_mempools.mempools[order] = NULL;
		udk_free(dma_mp);
	}
}

static void *dma_mempool_add(struct dma_mempool *dma_mp, unsigned long size,
			     uint32_t flag)
{
	int err;
	void *vaddr = NULL;
	unsigned long long paddr;

	if (dma_mp->refcnt >= INT_MAX) {
		udk_error("dma mempool refcnt overflow\n");
		return NULL;
	}
	vaddr = dma_mem_alloc(&dma_mp->mp_rbt_root, size, flag, &paddr);
	if (vaddr == NULL) {
		udk_error("dma mem alloc failed\n");
		return NULL;
	}
	err = udk_mempool_insert(dma_mp->mempool, vaddr, paddr, size);
	if (err < 0) {
		udk_error("dma mempool insert failed: %s\n", strerror(-err));
		(void)dma_mem_free(&dma_mp->mp_rbt_root, vaddr);
		return NULL;
	}
	dma_mp->refcnt++;

	return vaddr;
}

static int dma_mempool_remove(struct dma_mempool *dma_mp, const void *vaddr,
			      unsigned long size)
{
	int err;

	/* mempool remove may fail, since someone still use it */
	err = udk_mempool_remove(dma_mp->mempool, vaddr, size);
	if (err < 0) {
		return err;
	}
	(void)dma_mem_free(&dma_mp->mp_rbt_root, vaddr);
	if (dma_mp->refcnt > 0) {
		dma_mp->refcnt--;
	}

	return 0;
}

static void *dma_mempool_mem_alloc(struct dma_mempool *dma_mp,
				   unsigned long size, uint32_t flag)
{
	int err;
	void *tmp = NULL;
	void *vaddr = NULL;
	unsigned long aligned_size = PAGE_ALIGN_UP(size);

	vaddr = udk_mempool_mem_alloc(dma_mp->mempool, size);
	if (vaddr == NULL) {
		tmp = dma_mempool_add(dma_mp, aligned_size, flag);
		if (tmp == NULL) {
			udk_error("dma mempool add failed\n");
			return NULL;
		}
		vaddr = udk_mempool_mem_alloc(dma_mp->mempool, size);
		if (vaddr == NULL) {
			udk_error("mempool mem alloc failed\n");
			err = dma_mempool_remove(dma_mp, tmp, aligned_size);
			BUG_ON(err < 0);
			return NULL;
		}
	}

	return vaddr;
}

static void dma_mempool_mem_free(struct dma_mempool *dma_mp, const void *vaddr,
				 unsigned long size)
{
	int err;
	struct dma_mem_entry *entry = NULL;
	struct rbt_node *tmp = NULL;

	udk_mempool_mem_free(dma_mp->mempool, vaddr, size);
	/* try to free mempool */
	rbt_for_each_entry_safe(entry, tmp, &dma_mp->mp_rbt_root.tree,
				struct dma_mem_entry, node) {
		if (vaddr_in_range(entry, vaddr)) {
			err = dma_mempool_remove(dma_mp, entry->vaddr,
						 entry->size);
			if (err < 0) {
				return;
			}
			break;
		}
	}
}

static void *dma_alloc_aligned(unsigned int order, unsigned long size,
			       uint32_t flag, unsigned long long *paddr)
{
	int err;
	void *vaddr = NULL;
	void *tmp = NULL;
	unsigned long long pa = 0ULL;
	struct dma_mempool *dma_mp = NULL;

	dma_mp = dma_mempool_lookup(order);
	if (dma_mp == NULL) {
		err = dma_mempool_create(order);
		if (err < 0) {
			udk_error("dma mempool create failed\n");
			return NULL;
		}

		dma_mp = dma_mempool_lookup(order);
		if (dma_mp == NULL) {
			udk_error("dma mempool lookup failed\n");
			return NULL;
		}
		tmp = dma_mempool_add(dma_mp, PAGE_ALIGN_UP(size), flag);
		if (tmp == NULL) {
			udk_error("dma mempool add failed\n");
			return NULL;
		}
	}
	vaddr = dma_mempool_mem_alloc(dma_mp, size, flag);
	if (vaddr == NULL) {
		return NULL;
	}
	if (paddr != NULL) {
		err = udk_mempool_paddr_from_vaddr(dma_mp->mempool, vaddr,
						   (uint64_t *)(uintptr_t)&pa);
		if (err < 0) {
			udk_error("dma paddr from vaddr failed: %s\n",
				  strerror(-err));
			dma_mempool_mem_free(dma_mp, vaddr, size);
			return NULL;
		}
		*paddr = pa;
	}

	return vaddr;
}

static void dma_free(unsigned int order, const void *vaddr, unsigned long size)
{
	struct dma_mempool *dma_mp = NULL;

	dma_mempools_lock();
	dma_mp = dma_mempool_lookup(order);
	if (dma_mp == NULL) {
		dma_mempools_unlock();
	} else {
		dma_mempool_mem_free(dma_mp, vaddr, size);
		/* try to destroy mempool */
		dma_mempool_destroy(order, false);
		dma_mempools_unlock();
	}
}

void *udk_dma_alloc_aligned(unsigned long size, uint32_t flag,
			    unsigned long long *paddr, unsigned long align)
{
	int err;
	unsigned int order = (unsigned int)udk_size_to_order(align);
	unsigned long aligned_size;
	unsigned long long pa = 0ULL;
	void *vaddr = NULL;

	if (order >= DMA_MEMPOOL_ORDER_MAX) {
		udk_warn("invalid order according to align\n");
		return NULL;
	}
	/* mempool must ensure size is aligned */
	aligned_size = IS_ALIGNED(size, align) ? size : ALIGN_UP(size, align);
	dma_mempools_lock();
	vaddr = dma_alloc_aligned(order, aligned_size, flag, &pa);
	if (vaddr == NULL) {
		udk_error("dma alloc aligned failed\n");
		dma_mempool_destroy(order, false);
		dma_mempools_unlock();
		return NULL;
	}
	dma_mempools_unlock();
	dma_mem_entry_lock(&dma_mem_rbt_root);
	err = dma_mem_entry_add(&dma_mem_rbt_root, vaddr, pa, aligned_size,
				(int)order);
	if (err < 0) {
		udk_error("dma mem entry add failed: %s\n", strerror(-err));
		dma_mem_entry_unlock(&dma_mem_rbt_root);
		dma_free(order, vaddr, aligned_size);
		return NULL;
	}
	dma_mem_entry_unlock(&dma_mem_rbt_root);
	if (paddr != NULL) {
		*paddr = pa;
	}

	return vaddr;
}

void *udk_dma_alloc(unsigned long size, uint32_t flag,
		    unsigned long long *paddr)
{
	return udk_dma_alloc_aligned(size, flag, paddr, 1UL);
}

int udk_dma_free(const void *vaddr)
{
	unsigned int order;
	struct dma_mem_entry *entry = NULL;

	if (vaddr == NULL) {
		return -EINVAL;
	}
	dma_mem_entry_lock(&dma_mem_rbt_root);
	entry = dma_mem_entry_lookup(&dma_mem_rbt_root, vaddr, NULL, NULL);
	if (entry == NULL) {
		udk_warn("dma mem entry not found\n");
		dma_mem_entry_unlock(&dma_mem_rbt_root);
		return -ENOENT;
	}
	order = (unsigned int)entry->data;
	dma_free(order, entry->vaddr, entry->size);
	dma_mem_entry_remove(&dma_mem_rbt_root, entry);
	dma_mem_entry_unlock(&dma_mem_rbt_root);

	return 0;
}

unsigned long long udk_dma_va_to_pa(const void *vaddr)
{
	struct dma_mem_entry *entry = NULL;

	if (vaddr == NULL) {
		return 0ULL;
	}
	dma_mem_entry_lock(&dma_mem_rbt_root);
	entry = dma_mem_entry_lookup(&dma_mem_rbt_root, vaddr, NULL, NULL);
	if (entry == NULL) {
		udk_warn("vaddr is invalid\n");
		dma_mem_entry_unlock(&dma_mem_rbt_root);
		return 0ULL;
	}
	dma_mem_entry_unlock(&dma_mem_rbt_root);

	return entry->paddr + (unsigned long long)ptr_diff(vaddr, entry->vaddr);
}

void *udk_dma_pa_to_va(unsigned long long paddr)
{
	unsigned int order;
	void *vaddr = NULL;
	struct dma_mempool *dma_mp = NULL;
	struct dma_mem_entry *entry = NULL;
	struct rbt_node *tmp = NULL;

	if (paddr == 0ULL) {
		return NULL;
	}
	for (order = 0; order < DMA_MEMPOOL_ORDER_MAX; order++) {
		dma_mempools_lock();
		dma_mp = dma_mempool_lookup(order);
		if (dma_mp == NULL) {
			dma_mempools_unlock();
			continue;
		}
		rbt_for_each_entry_safe(entry, tmp, &dma_mp->mp_rbt_root.tree,
					struct dma_mem_entry, node) {
			if (paddr_in_range(entry, paddr)) {
				vaddr = ptr_add(entry->vaddr,
						paddr - entry->paddr);
				break;
			}
		}
		dma_mempools_unlock();
	}

	return vaddr;
}

static void dma_mempool_exit(unsigned int len)
{
	unsigned int order;

	for (order = 0; order < len; order++) {
		dma_mempool_destroy(order, true);
	}
}

static int dma_mempool_init(void)
{
	int err;
	unsigned int order;

	dma_mempools_lock_init();
	dma_mempools.bitmap = DMA_MEMPOOL_DEFAULT_BITMAP;
	dma_mem_rbt_init(&dma_mem_rbt_root);
	for (order = 0U; order < DMA_MEMPOOL_ORDER_MAX; order++) {
		if (is_dma_mempool_fixed(order)) {
			err = dma_mempool_create(order);
			if (err < 0) {
				dma_mempool_exit(order);
				return err;
			}
		} else {
			dma_mempools.mempools[order] = NULL;
		}
	}

	return 0;
}

static int udk_dma_init(void)
{
	int err;

	err = dma_mempool_init();
	if (err < 0) {
		udk_error("dma mempool init failed: %s\n", strerror(-err));
		return err;
	}

	return 0;
}
udk_init_call(udk_dma_init);
