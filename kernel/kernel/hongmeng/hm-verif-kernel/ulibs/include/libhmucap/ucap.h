/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of ulibs ucap
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 30 15:43:05 2018
 */
#ifndef LIBHMUCAP_UCAP_H
#define LIBHMUCAP_UCAP_H

#include <stdbool.h>
#include <stdint.h>
#include <hmkernel/rq.h>
#include <hmkernel/capability.h>
#include <hmkernel/drivers/ec.h>

#ifndef __hmbuild__
# ifdef CONFIG_SFI
#  ifndef __hmfeature_sfi__
#   define __hmfeature_sfi__
#  endif
# endif

# ifdef CONFIG_SHADOW_STACK
#  ifndef __hmfeature_shadowstack__
#   define __hmfeature_shadowstack__
#  endif
# endif
#endif

#define RETRY_TIMES    100

#define CTABLE_SLOTS_WATER_UAPP        32
#define CTABLE_SLOTS_WATER_NEWCAP_ACTV 1
#define CTABLE_SLOTS_WATER_NEWCAP_PMEM 1
#define CTABLE_SLOTS_WATER_NEWCAP_CPUCONTEXT 1
#define UCAP_NEW_SERVERPOOL_MAX_REQUESTS (32 * 4096U)
#define UCAP_NEW_CHANNEL_SIZE (PAGE_SIZE * 2)

struct kobj_self_mng;
cref_t
hm_ucap_new_cap(cref_t cnode, cref_t base_pmem,
		enum hmobj captype, __u8 right,
		unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4);

cref_t
hm_ucap_new_cap_noproxy(cref_t cnode, cref_t base_pmem,
		enum hmobj captype, __u8 right,
		unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4);

int hm_ucap_delete_cap(cref_t cnode, cref_t kobj);

/*
 * Create a new pmem capability for extending ctable.
 * When creating, no water is considered: it may take the last slot,
 * so don't use it directly.
 */
cref_t
ucap_new_ctable_mem(cref_t cnode, const uint64_t *pfn, uint64_t nrpfn);

int ucap_extend_ctable(cref_t cnode, unsigned int water);

int ucap_cslots_water(cref_t cnode, enum hmobj cap_type);

#define __ucap_for_each_ref(cnode, type, ref, err, table)		\
	for ((err) = syscap_CNodeIterate##table##Start((cnode), ((unsigned int)type), CNODE_ITERATE_STATE_ALL, &(ref)); \
	     (err) >= 0;						\
	     (err) = syscap_CNodeIterate##table##Next((cnode), ((unsigned int)type), CNODE_ITERATE_STATE_ALL, &(ref)))
/*
 * Iterate all @type capability in @cnode.
 * The user must provide variables for @cref (holding the result of each
 * step) and @err (holding the possible error), the loop will break if
 * an error occurs and the error value is stored in @err
 */
#define hm_ucap_for_each_cap(cnode, type, cref, err)			\
	__ucap_for_each_ref(cnode, type, cref, err, CTable)

/*
 * Iterate all @type rref in @cnode.
 * The user must provide variables for @rref (holding the result of each
 * step) and @err (holding the possible error), the loop will break if
 * an error occurs and the error value is stored in @err
 */
#define hm_ucap_for_each_rref(cnode, type, rref, err)			\
	__ucap_for_each_ref(cnode, type, rref, err, RTable)

/*
 * Iterate all @type uref in @cnode.
 * The user must provide variables for @uref (holding the result of each
 * step) and @err (holding the possible error), the loop will break if
 * an error occurs and the error value is stored in @err
 */
#define hm_ucap_for_each_uref(cnode, type, uref, err)			\
	__ucap_for_each_ref(cnode, type, uref, err, UTable)

/* get the cnode cref of the current cnode */
cref_t hm_ucap_self_cnode_cref(void);
/* get the cnode index of the current cnode */
__u32 hm_ucap_self_cnode_idx(void);

/* Inner parameter in libhmucap, for alloc pmem and free pmem */
struct ucap_mem {
	/*
	 * Indicate pmem owned by which cnode,
	 * this can be zero/cref/rref.
	 */
	cref_t cnode;
	cref_t pmem;
	void *page;
};

/* Inner operations in libhmucap */
struct ucap_ops {
	/* Alloc pmem memory for @cnode. */
	cref_t (*alloc_pmem)(cref_t cnode, uint64_t size, struct ucap_mem *mem);
	/* Free pmem memory in @cnode. */
	int (*free_pmem)(cref_t cnode, const struct ucap_mem *mem);

	/* Get cslots water for new cap. */
	int (*cslots_water)(cref_t cnode, enum hmobj cap_type);
	/* Alloc memory and extend ctable when freeslots is lower than water. */
	int (*extend_ctable)(cref_t cnode, unsigned int water);

	/* Record table pmem. */
	int (*record_table_pmem)(cref_t cnode, const struct ucap_mem *mem);
};

extern struct ucap_ops ucap_default_ops;

rref_t hm_ucap_require(cref_t tgt_cnode, uint32_t src_cnode_idx,
		       unsigned long flags);
uref_t hm_ucap_grant(cref_t src_cnode, rref_t tgt_rref, unsigned long flags,
		     cref_t cref, uint8_t rights, unsigned long args);

uref_t hm_ucap_export(cref_t src_cnode, uint32_t tgt_cnode_idx,
		      unsigned long flags, cref_t cref, uint8_t rights,
		      unsigned long args);

rref_t hm_ucap_accept_grant(cref_t tgt_cnode, uref_t src_uref, unsigned long flags);

int hm_ucap_revoke(cref_t src_cnode, uref_t uref);
int hm_ucap_reject(cref_t tgt_cnode, rref_t rref);

int hm_ucap_grant_move(cref_t src_cnode, rref_t dst_rref, rref_t src_rref);
int hm_ucap_export_move(cref_t src_cnode, rref_t src_rref, uint32_t dst_cnode_idx);
rref_t hm_ucap_accept_move(cref_t dst_cnode, rref_t src_rref, unsigned long flags);

int hm_ucap_extend_utable(cref_t cnode, unsigned int water);
int hm_ucap_extend_rtable(cref_t cnode, unsigned int water);

int kobj_check_captype(cref_t cnode, cref_t cref, enum hmobj captype);

/* The caller must ensure that the name is a string ending with '/0' */
cref_t hm_ucap_new_actvpool_with_flags(const char *name, unsigned long flags);
/* The caller must ensure that the name is a string ending with '/0' */
cref_t hm_ucap_new_actvpool(const char *name);

int hm_ucap_delete_actvpool(cref_t ap_cref);

int hm_ucap_new_actv(cref_t actv_pool_cref, struct kobj_self_mng *obj);
int hm_ucap_delete_actv(struct kobj_self_mng *obj);

cref_t hm_ucap_new_actv_pure(cref_t actv_pool_cref,
			     void *actv_kobj_pool, void *p_actv);
void *hm_ucap_actv_pure_res_of(cref_t a_cref, void *actv_kobj_pool);
int hm_ucap_delete_actv_pure(cref_t a_cref, void *actv_kobj_pool);
#if (defined __hmfeature_shadowstack__) || (defined __hmfeature_sfi__)
void *hm_ucap_actv_pure_sstack_of(cref_t a_cref, void *actv_kobj_pool);
#endif

/* Create new channel cap */
cref_t hm_ucap_new_channel(void);
int hm_ucap_delete_channel(cref_t chn_cref);

cref_t hm_ucap_new_livepatch(unsigned long cap_size, unsigned long func_nr);
int hm_ucap_delete_livepatch(cref_t lp_cref);
/* Crteate new cpu context cap */
enum hm_ucap_cpuctx_type {
	HM_UCAP_CPUCTX_ARM_PMU,
	HM_UCAP_CPUCTX_ARM_DEBUG,
	HM_UCAP_CPUCTX_MAX,
};

cref_t hm_ucap_new_cpuctx(enum hm_ucap_cpuctx_type type);
int hm_ucap_delete_cpuctx(cref_t cpu_ctx);

cref_t hm_ucap_new_dsuctx(void);
int hm_ucap_delete_dsuctx(cref_t dsu_ctx);

cref_t hm_ucap_new_permission(uint64_t permission);
int hm_ucap_delete_permission(cref_t cref);

int hm_ucap_bind_permission(cref_t tgt_cnode, cref_t permission_cref);
int hm_ucap_unbind_permission(cref_t tgt_cnode);

int ucap_spawn_cnode(struct sysarg_cnode_spawn_req *spawn_req,
		     struct sysarg_cnode_spawn_res *spawn_res);

uint64_t hm_ucap_kobj_alloc_size_dump(void);
uint64_t hm_ucap_kobj_alloc_size_dump_oom(void);

void hm_ucap_kobj_allocators_lock_parent(void);
void hm_ucap_kobj_allocators_unlock_parent(void);
void hm_ucap_kobj_allocators_unlock_child(void);

cref_t hm_ucap_new_ec_ep(cref_t cnode, struct __ec_ep_args *args,
			 cref_t vspace_cref);
int hm_ucap_delete_ec_ep(cref_t cnode, cref_t ep_cref);

cref_t hm_ucap_new_ec_chn(cref_t cnode);
int hm_ucap_delete_ec_chn(cref_t cnode, cref_t chn_cref);

int hm_ucap_delete_epoll(cref_t cnode, cref_t tcb_cref);
cref_t hm_ucap_new_epoll(cref_t cnode, const struct __sysarg_epoll_info *info);

int hm_ucap_new_fdtable(cref_t cnode, __u64 rlimit, struct kobj_self_mng *obj, __u64 process_cnode_idx);
int hm_ucap_delete_fdtable(cref_t cnode, struct kobj_self_mng *obj);

static inline int do_delete_cap(cref_t cnode, cref_t kobj)
{
	return hm_ucap_delete_cap(cnode, kobj);
}

#endif
