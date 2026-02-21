/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of ulibs kobjpool
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 29 14:10:51 2019
 */
#ifndef LIBUCAP_KOBJPOOL_H
#define LIBUCAP_KOBJPOOL_H

#include <hmkernel/capability.h>

#include <lib/dlist.h>

#include <libhmucap/ucap.h>
#include <libhmucap/actv_kobjpool.h>
#include <libalgo/rbtree.h>
#include <libhmsync/raw_mutex.h>

#include <stdint.h>

struct kobj_allocator;

struct kobj_pmem {
	struct dlist_node node;
	struct ucap_mem mem;
	uint64_t total_size;
	uint64_t remain_size;
	uint64_t slot_size;
	struct kobj_allocator *allocator;
};

struct kobj_slot {
	struct rbt_node node;
	cref_t kobj; // cref of kernel object
	struct kobj_pmem *pmem;
};

/* mng is short for managed */
struct kobj_self_mng {
	cref_t cref;
	struct kobj_pmem *pmem;
};

/*
 * kobj_alloc/free_slot interface can be invoked in sysmgr or other apps:
 * sysmgr, a valid rref represent sysmgr alloc/free kobj for other apps,
 * 0 represent sysmgr alloc/free kobj for itself.
 * other apps, a valid cnode cref or 0, both represent alloc/free for itself.
 * Notes: kobj's related ops are not async-signal-safe functions. So that the
 * user can't call kobj's related ops at after fork before exec.
 */
size_t kobj_slot_size(void);
void *kobj_slot_malloc(void);
void kobj_slot_free(void *ptr);
struct kobj_slot *kobj_alloc_slot(cref_t cnode, enum hmobj type, uint64_t size);
void kobj_free_slot(cref_t cnode, struct kobj_slot *slot);
int kobj_record_slot(cref_t kobj, struct kobj_slot *slot);
struct kobj_slot *kobj_remove_slot(cref_t kobj);
struct kobj_slot *kobj_search_slot(cref_t kobj);
uint64_t kobj_alloc_size_dump(void);
void kobj_allocators_lock(void);
void kobj_allocators_unlock(void);
struct kobj_pmem *kobj_alloc_pmem_slot(cref_t cnode, enum hmobj type, uint64_t size);
void kobj_free_pmem_slot(cref_t cnode, struct kobj_pmem *mem);
#ifdef __HOST_LLT__
void llt_clean_kobj_pool_tree(void);
#endif

struct actv_kobj_slot {
	uint16_t remain_actv;		/* <=4 */
	uint16_t used;			/* 0 or 1 */
	uint32_t inner_idx;		/* indicate the actv */
	void *kobj_res;
#if (defined __hmfeature_shadowstack__) || (defined __hmfeature_sfi__)
	void *sstack;
#endif
	struct ucap_mem mem;
};

#endif
