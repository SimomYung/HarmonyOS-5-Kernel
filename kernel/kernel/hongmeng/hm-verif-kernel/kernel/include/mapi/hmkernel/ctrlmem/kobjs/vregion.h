/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for vregion object
 * Author: Huawei OS Kernel Lab
 * Created: Fri Apr 21 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_VREGION_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_VREGION_H

#include <uapi/hmkernel/ctrlmem/kobjs/vregion.h>
#include <hmkernel/ctrlmem/kobjs/vrcache.h>
#include <hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>
#include <hmkernel/atomic.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>
#include <lib/rbtree.h>

struct ctrlmem_kobj_vregion_node_s {
#if __SIZEOF_LONG__ == 4
	u32 padding;
#else
	u64 padding;
#endif
	ctrlmem_syspa_t left;
	ctrlmem_syspa_t right;
};

struct ctrlmem_kobj_vregion_data_anonbk_s {
#if __SIZEOF_LONG__ == 4
	u32 vrgptr;
	u32 padding[2];
#else
	u64 vrgptr;
	u64 padding[2];
#endif
};

struct ctrlmem_kobj_vregion_data_filebk_fscache_s {
#if __SIZEOF_LONG__ == 4
	u32 vrgptr;
#else
	u64 vrgptr;
#endif

#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
#if __SIZEOF_LONG__ == 4
	u32 mapping;
#else
	u64 mapping;
#endif
#endif
};

struct ctrlmem_kobj_vregion_data_filebk_s {
#if __SIZEOF_LONG__ == 4
	u32 vrgptr;
	ctrlmem_syspa_t fcache_ptr;
	bool is_dax;
	bool is_seal_future_write;
	bool hhp;
	u32 cnode_idx;
	u32 fid;
#else
	u64 vrgptr;
	ctrlmem_syspa_t fcache_ptr;
	bool is_dax;
	bool is_seal_future_write;
	bool hhp;
	u32 cnode_idx;
	u64 fid;
#endif
};

union ctrlmem_kobj_vregion_data_backend_u {
	struct ctrlmem_kobj_vregion_data_anonbk_s anonbk;
	struct ctrlmem_kobj_vregion_data_filebk_s filebk;
	struct {
		u32 padding1;
		u64 padding2;
		u64 padding3;
		u64 padding4;
		u64 padding5;
	} iomem_padding;
};

struct ctrlmem_kobj_vregion_s {
	struct ctrlmem_kobj_vregion_node_s node;
	vaddr_t start;
	vaddr_t end;
	u64 flags;
	u32 prot;
	u64 pos;
	union ctrlmem_kobj_vregion_data_backend_u bkdata;
};

struct ctrlmem_kobj_phymem_stat_s {
	u64 counter[__CTRLMEM_NR_MEM_TYPES];
};

/*
 * a per vspace structure, usage: store rbt root, kernel/sysmgr lock, statistic data, etc.
*/
struct ctrlmem_kobj_vregion_shared_s {
	ctrlmem_syspa_t vmem_list_prev;
	ctrlmem_syspa_t vmem_list_next;
	ctrlmem_syspa_t rbt_root;
	ctrlmem_syspa_t sumry_upd_cb;
	struct ctrlmem_kobj_rwlock_s rwlock;
	struct ctrlmem_kobj_vrcache_s vrcache;
	bool mlockall;
	bool mlockall_onfault;
	bool swapless;
	vatomic32_t kpfskip;
	struct ctrlmem_kobj_phymem_stat_s phymem_stat;
	struct ctrlmem_kobj_fspgcache_fscache_stat_s *fscache_stat;
};

struct ctrlmem_kobj_vspace_s;

static inline int ctrlmem_kobj_vregion_shared_trylock(struct ctrlmem_kobj_vregion_shared_s *vrshared)
{
	return ctrlmem_kobj_tryrdlock(&vrshared->rwlock);
}

static inline void ctrlmem_kobj_vregion_shared_unlock(struct ctrlmem_kobj_vregion_shared_s *vrshared)
{
	ctrlmem_kobj_rdunlock(&vrshared->rwlock);
}

#define INVALID_VREGION_POS (0xffffffffffffffffULL)
#define file_offset_to_index(pos, order) ((pos) >> ((order) + (u32)PAGE_SHIFT))

static inline u64 vregion_addr_to_pos(const struct ctrlmem_kobj_vregion_s *vr, vaddr_t abort_addr)
{
	u64 pos = INVALID_VREGION_POS;
	int rc = E_HM_OK;

	/* NOTE: end of vregion is valid for this function */
	if ((vr == NULL) || (abort_addr < vr->start) || (abort_addr > vr->end)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		pos = (u64)(abort_addr - vr->start) + vr->pos;
	}
	return pos;
}

struct ctrlmem_kobj_vspace_searchvr_state_s;
extern int ctrlmem_kobj_vregion_shared_searchvr_acquire(struct ctrlmem_kobj_vregion_shared_s *vrshared,
							const struct ctrlmem_sysparule_s *sysparule,
							struct ctrlmem_kobj_vspace_searchvr_state_s *state,
							void __user *fault_ptr);

extern void ctrlmem_kobj_vregion_shared_add_phymem_stat(struct ctrlmem_kobj_vregion_shared_s *vrshared,
							enum __ctrlmem_kobj_phymem_type type);
#endif
