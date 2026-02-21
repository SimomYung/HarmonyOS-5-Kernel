/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition for process scopedptr
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 11 10:50:29 2020
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_SCOPEDPTR_H
#define SYSMGR_INTAPI_PROCESS_PROC_SCOPEDPTR_H

#ifndef SYSMGR_INTAPI_PROCESS_PROCESS_H
# error "Not for directly included by C file. Include intapi/process/process.h instead."
#endif

#define SYSMGR_SCOPEDPTR

#include <libhmsync/sysmgr_scopedptr.h>
#include <intapi/process/proc_info.h>
#include <intapi/nsmgr/ns_pid.h>

#define PROCESS_LOCK_GUARD RAW_RWLOCK_RD_GUARD(scopedptr, procpool_rwlock())

declare_scopeptr1(process, offset_of(struct process_s, refcnt), PROCESS_LOCK_GUARD,
		  procpool_lookup_cnode_idx_nolock, uint32_t, cnode_idx);

#define acquire_process(name, cnode_idx) \
	acquire_scopedptr(process, name, offset_of(struct process_s, refcnt), cnode_idx)

#define acquire_process_start(name, cnode_idx) \
	{\
	acquire_process(name, cnode_idx);

#define acquire_process_end(name)  }

declare_scopeptr1(process_by_cnode_hdlr, offset_of(struct process_s, refcnt), PROCESS_LOCK_GUARD,
		  procpool_lookup_cnode_hdlr_nolock, rref_t, cnode_hdlr);

#define acquire_process_by_cnode_hdlr(name, cnode_hdlr) \
	acquire_scopedptr(process_by_cnode_hdlr, name, offset_of(struct process_s, refcnt), cnode_hdlr)

#define acquire_process_by_cnode_hdlr_no_auto(cnode) \
		acquire_process_by_cnode_hdlr_ptr_ref(cnode)
#define release_process_ref(process) (void)raw_refcnt_put(&(process)->refcnt)

#define process_init_refcount(ptr) \
	raw_refcnt_init(ptr, 1, process_delete_from_ref)

#define process_cnode_ref_init(ref, res, rref) \
do { \
	(ref)->resource = (res); \
	(ref)->cnode_rref = (rref); \
} while (__false)

#define process_vspace_ref_init(ref, vs) \
{ \
	(ref)->vspace = (vs); \
}

#define acquire_vspace(vspace, process) \
	struct process_vspace_ref __scopedptr_to_##vspace##_##process \
		__attribute__((cleanup(__resource_vspace_release))) = {NULL, NULL}; \
	vspace = process_acquire_vspace(process); \
	process_vspace_ref_init(&__scopedptr_to_##vspace##_##process, vspace) \

#define acquire_vspace_start(vspace, process) \
{ \
	acquire_vspace(vspace, process);

#define acquire_vspace_end(vspace) }

#define acquire_cnode_rref(cnode_rref, process) \
	struct process_cnode_ref __scopedptr_to_##cnode_rref##_##process \
		__attribute__((cleanup(__resource_cnode_release))) = {0, NULL}; \
	cnode_rref = process_acquire_cnode_rref(process_resource_of(process)); \
	process_cnode_ref_init(&__scopedptr_to_##cnode_rref##_##process, \
			       process_resource_of(process), cnode_rref)

#define process_resource_init_refcount(ptr) \
	raw_refcnt_nr_init(ptr, 1)

#define process_vspace_init_refcount(ptr) \
	raw_refcnt_nr_init(ptr, 1)

#endif
