/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem syscall wrapper
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 09 10:44:12 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_LAYER_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_LAYER_H

#include <hmasm/memory.h>
#include <sys/types.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hmkernel/ctrlmem/kobjs/syspgarr.h>
#include <hmkernel/ctrlmem/kobjs/cofile.h>
#include <hmkernel/ctrlmem/kobjs/process.h>
#include <hmkernel/ctrlmem/kobjs/pcache.h>
#include <hmkernel/ctrlmem/kobjs/vspace.h>
#include <hmkernel/ctrlmem/kobjs/thread.h>
#include <hmkernel/ctrlmem/kobjs/vspace/rangeop.h>
#include <hmkernel/ctrlmem/kobjs/polllist.h>
#include <hmkernel/ctrlmem/kobjs/fsverity.h>
#include <hmkernel/ctrlmem/kobjs/actv_binder_context.h>
#include <hmkernel/ctrlmem/kobjs/actv_binder_refer.h>
#include <hmkernel/ctrlmem/iterator.h>
#include <hmkernel/ctrlmem/kobjs/sysevent.h>
#include <hmkernel/ctrlmem/vspace.h>
#include <hmkernel/ctrlmem/syscall.h>
#include <hmkernel/ctrlmem/sysconf.h>
#include <hmkernel/ctrlmem/pgtblull.h>
#include <hmkernel/ctrlmem/pcpinfo.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmkernel/mm/kstack.h>
#include <hmkernel/errno.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls.h>
#include <libhmsrv_sys/hm_ctrlmem_psf_syscalls_layer.h>
#include <hmasm/pgtbl/pgtblhdlr.h>

#ifdef __HMPGTBL_ARM_2LEVELS__
typedef __u32 pte_t;
#else
typedef __u64 pte_t;
#endif

/*
 * kobjctnr and kobjs
 */
struct __ctrlmem_kobjctnr_page_freelist_s;
struct __ctrlmem_kobj_pcache_s;
struct __ctrlmem_kobj_polllist_s;
struct __ctrlmem_kobj_sysevent_s;
struct __ctrlmem_kobj_ulwt_s;

static inline int sysctrlmem_kobjctnr_set_header_ptr(void *header_ptr, void *ptr)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, header_ptr, (unsigned long)(ptr));
}

static inline int sysctrlmem_kobjctnr_set_header_u32(void *header_ptr, __u32 v)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, header_ptr, v);
}

static inline int sysctrlmem_kobjctnr_spawn_process(struct __ctrlmem_kobjctnr_page_freelist_s *fl)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_PROCESS);
}

static inline int sysctrlmem_kobjctnr_spawn_vspace(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
						   void *pages_linear_va, unsigned long flags,
						   struct __ctrlmem_kobj_pcache_s *pcache)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_VSPACE,
			      __sysctrlmem_arg_ptr(pages_linear_va),
			      __sysctrlmem_arg_ulong(flags),
			      __sysctrlmem_arg_ptr(pcache));
}

static inline int sysctrlmem_kobjctnr_spawn_data(struct __ctrlmem_kobjctnr_page_freelist_s *fl, unsigned int sz)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_DATA, sz));
}

static inline int sysctrlmem_kobjctnr_spawn_thread(struct __ctrlmem_kobjctnr_page_freelist_s *fl, unsigned int sz)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_THREAD, sz));
}

static inline int sysctrlmem_kobjctnr_spawn_thread_by_meta(struct __ctrlmem_kobjctnr_meta_s *meta, unsigned int sz)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, meta,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_THREAD, sz));
}

static inline int sysctrlmem_kobj_process_set_jitfort(struct __ctrlmem_kobj_process_s *kobj_process,
						 unsigned int option, struct __ctrlmem_kobj_vspace_s* kobj_vspace)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_process->jitfort_status,
						 __sysctrlmem_arg_ulong(option),
						 __sysctrlmem_arg_ptr(kobj_vspace));
}

static inline int sysctrlmem_kobjctnr_spawn_cofile(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
						   unsigned int type, __u64 size)
{
	int err = E_HM_INVAL;
	if ((type < __KOBJCOF_TYPE_INVALID) && (size > 0)) {
		err = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_COFILE,
				     __sysctrlmem_arg_u32(type),
				     __sysctrlmem_arg_u64(size));
	}
	return err;
}

static inline int sysctrlmem_kobjctnr_spawn_ulwt(struct __ctrlmem_kobjctnr_page_freelist_s *fl)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_ULWT);
}

static inline int sysctrlmem_kobjctnr_spawn_actv_frame(struct __ctrlmem_kobjctnr_page_freelist_s *fl)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_FRAME);
}

static inline int sysctrlmem_kobjctnr_spawn_fsverity(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
						     void *info, __u32 info_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_FSVERITY,
			      __sysctrlmem_arg_ptr(info), __sysctrlmem_arg_u32(info_size));
}

static inline int sysctrlmem_kobjctnr_spawn_actv_binder_context(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
								unsigned int devno, cref_t actv_pool_cref)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_BINDER_CONTEXT,
			      __sysctrlmem_arg_u32(devno), __sysctrlmem_arg_u64(actv_pool_cref));
}

static inline int sysctrlmem_kobjctnr_spawn_actv_binder_refer(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
							      struct __ctrlmem_kobj_actv_binder_context_s *kobj_ab_ctx)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_BINDER_REFER,
			      __sysctrlmem_arg_ptr(kobj_ab_ctx));
}

struct __ctrlmem_kobj_syspgarr_s;
struct __ctrlmem_pcache_s;

static inline int sysctrlmem_kobjctnr_spawn_pcache_spin(struct __ctrlmem_kobjctnr_page_freelist_s *fl, unsigned int sz,
							struct __ctrlmem_pcache_s *pcpg,
							struct __ctrlmem_kobj_syspgarr_s *syspgarr)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_PCACHE, sz),
			      __sysctrlmem_arg_ptr(pcpg),
			      __sysctrlmem_arg_ptr(syspgarr),
			      __sysctrlmem_arg_ulong(__CTRLMEM_SYSPARULE_SPIN));
}

static inline int sysctrlmem_kobjctnr_spawn_pcache_linear(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
							  unsigned int sz, struct __ctrlmem_pcache_s *pcpg,
							  struct __ctrlmem_kobj_syspgarr_s *syspgarr,
							  __u64 pvoffset)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_PCACHE, sz),
			      __sysctrlmem_arg_ptr(pcpg),
			      __sysctrlmem_arg_ptr(syspgarr),
			      __sysctrlmem_arg_ulong(__CTRLMEM_SYSPARULE_LINEAR),
			      __sysctrlmem_arg_u64(pvoffset));
}

static inline int sysctrlmem_kobjctnr_spawn_syspgarr_dpa(struct __ctrlmem_kobjctnr_page_freelist_s *fl, unsigned int sz,
							 unsigned long cellsize, unsigned long l1order,
							 unsigned long page_block_order)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_SYSPGARR, sz),
			      __sysctrlmem_arg_ulong(__CTRLMEM_KOBJ_SYSPGARR_TYPE_DPA),
			      __sysctrlmem_arg_ulong(cellsize),
			      __sysctrlmem_arg_ulong(l1order),
			      __sysctrlmem_arg_ulong(page_block_order));
}

static inline int sysctrlmem_kobjctnr_spawn_syspgarr_sparse(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
							    unsigned int sz, unsigned long cellsize, unsigned long array_start)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_SYSPGARR, sz),
			      __sysctrlmem_arg_ulong(__CTRLMEM_KOBJ_SYSPGARR_TYPE_SPARSE),
			      __sysctrlmem_arg_ulong(cellsize),
			      __sysctrlmem_arg_ulong(array_start));
}

struct __ctrlmem_kobjctnr_page_s;

static inline int sysctrlmem_kobj_destroy(struct __ctrlmem_kobjctnr_page_s *page, unsigned int npiece_kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &page->metas[npiece_kobj], __CTRLMEM_KOBJCTNR_META_FREEPIECE);
}

struct __ctrlmem_kobj_vspace_s;
static inline int sysctrlmem_kobj_vspace_link(struct __ctrlmem_kobj_vspace_s *kobj_vspace,
				cref_t vspace_cref)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_vspace->u.vspace_cref, vspace_cref);
}

static inline int sysctrlmem_kobj_vspace_unlink(struct __ctrlmem_kobj_vspace_s *kobj_vspace)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_vspace->u.vspace_cref, 0ull);
}

static inline int ctrlmem_kobj_process_set_itimer_prof_expire(struct __ctrlmem_kobj_process_s *kobj, __u64 expire)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->itimer_prof_expire, expire);
}

static inline int ctrlmem_kobj_process_set_itimer_prof_interval(struct __ctrlmem_kobj_process_s *kobj, __u64 interval)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->itimer_prof_interval, interval);
}

static inline int ctrlmem_kobj_process_set_itimer_virt_expire(struct __ctrlmem_kobj_process_s *kobj, __u64 expire)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->itimer_virt_expire, expire);
}

static inline int ctrlmem_kobj_process_set_load_chk(struct __ctrlmem_kobj_process_s *kobj, bool load_chk)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->load_chk, __sysctrlmem_arg_u32(load_chk));
}

static inline int ctrlmem_kobj_process_set_itimer_virt_interval(struct __ctrlmem_kobj_process_s *kobj, __u64 interval)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->itimer_virt_interval, interval);
}

static inline int ctrlmem_kobj_thread_set_timerslack(struct __ctrlmem_kobj_thread_s *kobj, __u64 timerslack_ns)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->timerslack_current_ns, timerslack_ns);
}

static inline int sysctrlmem_kobj_thread_seccomp_refresh_sc_data(struct __ctrlmem_kobj_thread_s *kobj,
								 void *data, __u32 data_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->seccomp_refresh_sc_data, 0,
			      __sysctrlmem_arg_ptr(data), __sysctrlmem_arg_u32(data_size));
}

static inline int ctrlmem_kobj_thread_set_load_chk(struct __ctrlmem_kobj_thread_s *kobj, bool load_chk)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->load_chk, __sysctrlmem_arg_u32(load_chk));
}

static inline int sysctrlmem_kobj_data_write32(void *kobj_data, __u32 v)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj_data, v);
}

static inline int sysctrlmem_kobj_data_write64(void *kobj_data, __u64 v)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, kobj_data, v);
}

static inline int sysctrlmem_kobj_cofile_write_pos(struct __ctrlmem_kobj_cofile_s *kobj_cofile, __s64 pos)
{
	return (pos > (__s64)kobj_cofile->size) ? E_HM_INVAL : sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN,
									&kobj_cofile->pos,
									pos);
}

static inline int sysctrlmem_kobj_cofile_write_fdata_status(struct __ctrlmem_kobj_cofile_s *kobj_cofile, __u32 status)
{
	return (status > __KOBJCOF_FDATA_INVALID) ? E_HM_INVAL : sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN,
									       &kobj_cofile->fdata.status,
									       status);
}

static inline int sysctrlmem_kobj_cofile_write_fdata_page(struct __ctrlmem_kobj_cofile_s *kobj_cofile,
							  unsigned int page_idx,
							  __u8 *page_addr)
{
	return (page_idx >= __KOBJCOF_FDATA_PAGE_NR) ? E_HM_INVAL : sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN,
										      &kobj_cofile->fdata.pages_kva[page_idx],
										      page_addr);
}

static inline int sysctrlmem_kobj_fsverity_verify(struct __ctrlmem_kobj_fsverity_s *kobj,
						  void *hbuffer, __u64 data_page_pa, __u32 data_offset)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_FSVERITY_OP_VERIFY),
			      __sysctrlmem_arg_ptr(hbuffer),
			      __sysctrlmem_arg_u64(data_page_pa),
			      __sysctrlmem_arg_u32(data_offset));
}

static inline int sysctrlmem_kobj_fsverity_verify_mb(struct __ctrlmem_kobj_fsverity_s *kobj,
						    void *mb_task)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_FSVERITY_OP_VERIFY_MB),
			      __sysctrlmem_arg_ptr(mb_task));
}

static inline int sysctrlmem_kobj_vspace_link_vrshared(struct __ctrlmem_kobj_vspace_s *kobj, void *vrshared)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->vrs.vrshared, (unsigned long)(uintptr_t)vrshared);
}

static inline int sysctrlmem_kobj_vspace_link_sysparule_linear(struct __ctrlmem_kobj_vspace_s *kobj,
						 __u64 pvoffset)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->sysparule, __CTRLMEM_SYSPARULE_LINEAR,
			      __sysctrlmem_arg_u64(pvoffset));
}

static inline int sysctrlmem_kobj_vspace_link_key(struct __ctrlmem_kobj_vspace_s *kobj, __u64 key)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->vs_key, key);
}

static inline int sysctrlmem_kobj_vspace_link_sysparule_spin(struct __ctrlmem_kobj_vspace_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->sysparule, __CTRLMEM_SYSPARULE_SPIN);
}

/* Only one bit can be set at a time. */
static inline int sysctrlmem_kobj_vspace_link_flags(struct __ctrlmem_kobj_vspace_s *kobj, __u32 flags)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->flags, flags);
}

struct gzeropg_page_s;
static inline int sysctrlmem_kobj_pcache_link_gzeropg(struct __ctrlmem_kobj_pcache_s *kobj,
						      struct gzeropg_page_s *gzeropg)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->gzeropg, __sysctrlmem_arg_ptr(gzeropg));
}

struct __ctrlmem_pcache_oplog_s;
static inline int sysctrlmem_kobj_pcache_link_oplog(struct __ctrlmem_kobj_pcache_s *kobj,
						    struct __ctrlmem_pcache_oplog_s *oplog)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->oplog, __sysctrlmem_arg_ptr(oplog));
}

static inline int sysctrlmem_kobj_process_link_cosched(struct __ctrlmem_kobj_process_s *kobj,
						       struct __ctrlmem_cosched_s *cosched)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->csp.cosched, __sysctrlmem_arg_ptr(cosched));
}

static inline int sysctrlmem_kobj_process_unlink_cosched(struct __ctrlmem_kobj_process_s *kobj)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->csp.cosched, __sysctrlmem_arg_ptr(NULL));
}

/*
 * pgstr
 */
struct pgstr_s;

static inline int sysctrlmem_pgstr_trans(struct pgstr_s *pgstr, __u32 pgstrv_new, __paddr_t pa)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, pgstrv_new, __sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgstr_trans_nml2iofa(struct pgstr_s *pgstr, __u32 pgstrv_new, __paddr_t pa)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_PGSTR_NML2IOFA, pgstr, pgstrv_new, __sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgstr_trans_with_pte(struct pgstr_s *pgstr, __u32 pgstrv_new,
				    void *upte)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, pgstrv_new,  __sysctrlmem_arg_ptr(upte));
}

static inline int sysctrlmem_pgstr_trans_nml2pgd(struct pgstr_s *pgstr, void *upte, void *pgd_va)
{
	__u32 pgstrv_new = __PGSTR_TYPE_PGTBL_FL << __PGSTRV_SHIFT_TYPE;
	int err;

	if ((uintptr_t)upte == ~(uintptr_t)(0UL)) {
		pgstrv_new |= (__PGSTRV_TRANSCTRL_NML2PGT_DIRECT_KLINEAR << __PGSTRV_SHIFT_TRANSCTRL);
		err = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, pgstrv_new,
				     __sysctrlmem_arg_ptr(pgd_va));
	} else {
		err = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, pgstrv_new,
				     __sysctrlmem_arg_ptr(upte),
				     __sysctrlmem_arg_ptr(pgd_va));
	}
	return err;
}

static inline int sysctrlmem_pgstr_trans_pgd2nml(struct pgstr_s *pgstr, void *pgd_va)
{
	__u32 pgstrv_new = __PGSTR_TYPE_NORMALPG << __PGSTRV_SHIFT_TYPE;
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, pgstrv_new,
			      __sysctrlmem_arg_ptr(pgd_va));
}

static inline int sysctrlmem_pgstr_superop_inc(struct pgstr_s *pgstr, __paddr_t pa)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, __PGSTRSUPEROP_INC_FST_REFCNT,
			      __sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgstr_superop_dec(struct pgstr_s *pgstr, __paddr_t pa)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, pgstr, __PGSTRSUPEROP_DEC_LST_REFCNT,
			      __sysctrlmem_arg_paddr(pa));
}

/*
 * pte and page table
 */
struct __ctrlmem_vspace_slot_s;
struct __ctrlmem_kobj_vspace_s;

static inline int sysctrlmem_pgtblull_remap_pgstrva_vsslot(void *pte, const struct pgstr_s *pgstr,
							   const struct __ctrlmem_vspace_slot_s *vsslot,
							   void *va, unsigned int modelid, __paddr_t pa)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val((unsigned long)(pgstr) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGSTRVA),
		__sysctrlmem_arg_ptr(vsslot),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid),
		__sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgtblull_remap_pgstrva_kobjvs(void *pte, const struct pgstr_s *pgstr,
							   const struct __ctrlmem_kobj_vspace_s *kobjvs,
							   void *va, unsigned int modelid, __paddr_t pa)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val((unsigned long)(pgstr) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGSTRVA),
		__sysctrlmem_arg_ptr(kobjvs),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid),
		__sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgtblull_clean_vsslot(void *pte,
						   const struct __ctrlmem_vspace_slot_s *vsslot,
						   void *va)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val(0ul | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_UNMAP),
		__sysctrlmem_arg_ptr(vsslot),
		__sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_pgtblull_clean_kobjvs(void *pte,
						   const struct __ctrlmem_kobj_vspace_s *kobjvs,
						   void *va)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val(0ul | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_UNMAP),
		__sysctrlmem_arg_ptr(kobjvs),
		__sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_pgtblull_clean_set_vsslot(void *pte, pte_t ptev,
						       const struct __ctrlmem_vspace_slot_s *vsslot,
						       void *va)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val(ptev | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_UNMAP),
		__sysctrlmem_arg_ptr(vsslot),
		__sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_pgtblull_clean_set_kobjvs(void *pte, pte_t ptev,
						       const struct __ctrlmem_kobj_vspace_s *kobjvs,
						       void *va)
{
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_CLEAN_SET_KOBJVS, pte,
		__sysctrlmem_pgtbl_val(ptev | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_UNMAP),
		__sysctrlmem_arg_ptr(kobjvs),
		__sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_pgtblull_clear_abit(void *pte, const struct __ctrlmem_kobj_vspace_s *kobjvs, void *va)
{
	unsigned long op_command = __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_REPROTECT |
				   __CTRLMEM_SYSCALL_PGTBLULL_VAL_CMD_ACCESS_FLAG_UNSET;
	return sysctrlmem_pgtbl(__CTRLMEM_SYSCALL_CLEAR_ABIT, pte,
		__sysctrlmem_pgtbl_val(op_command),
		__sysctrlmem_arg_ptr(kobjvs),
		__sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_pgtblull_cas_map_pgstrva(void *pte, const struct pgstr_s *pgstr, void *va,
						      unsigned long modelid, __paddr_t pa, pte_t old_ptev)
{
	return sysctrlmem_pgtbl_cas(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val(old_ptev),
		__sysctrlmem_pgtbl_val((unsigned long)(pgstr) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGSTRVA),
		__sysctrlmem_arg_ptr(NULL),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid),
		__sysctrlmem_arg_paddr(pa));
}

static inline int sysctrlmem_pgtblull_cas_map_pgva(void *pte, const void *pgva, void *va,
						   unsigned int modelid, pte_t old_ptev)
{
	return sysctrlmem_pgtbl_cas(__CTRLMEM_SYSCALL_MAP_PGVA, pte,
		__sysctrlmem_pgtbl_val(old_ptev),
		__sysctrlmem_pgtbl_val((unsigned long)(pgva) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGVA),
		__sysctrlmem_arg_ptr(NULL),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid));
}

static inline int sysctrlmem_pgtblull_cas_remap_pgva_kobjvs(void *pte, const void *pgva,
							    const struct __ctrlmem_kobj_vspace_s *kobjvs,
							    void *va, unsigned int modelid,
							    pte_t old_ptev)
{
	return sysctrlmem_pgtbl_cas(__CTRLMEM_SYSCALL_REMAP_PGVA_KOBJVS, pte,
		__sysctrlmem_pgtbl_val(old_ptev),
		__sysctrlmem_pgtbl_val((unsigned long)(pgva) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGVA),
		__sysctrlmem_arg_ptr(kobjvs),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid));
}

static inline int sysctrlmem_pgtblull_cas_remap_pgva_vsslot(void *pte, const void *pgva,
							    const struct __ctrlmem_vspace_slot_s *vsslot,
							    void *va, unsigned int modelid,
							    pte_t old_ptev)
{
	return sysctrlmem_pgtbl_cas(__CTRLMEM_SYSCALL_UNKNOWN, pte,
		__sysctrlmem_pgtbl_val(old_ptev),
		__sysctrlmem_pgtbl_val(((unsigned long)(pgva) | __CTRLMEM_SYSCALL_PGTBLULL_VAL_OP_MAPPGVA)),
		__sysctrlmem_arg_ptr(vsslot),
		__sysctrlmem_arg_ptr(va),
		__sysctrlmem_arg_ulong(modelid));
}

static inline int sysctrlmem_iterator_handle(void *iter_ptr, unsigned long iter_type)
{
	unsigned long callno;
	if (iter_type == __CTRLMEM_ITERATOR_BATCH_TLBG_UNMAP) {
		callno = __CTRLMEM_SYSCALL_BATCH_TLBG_UNMAP;
	} else {
		callno = __CTRLMEM_SYSCALL_BATCH_MAP_PGVA;
	}
	return sysctrlmem_vulong(callno, iter_ptr, iter_type);
}

static inline int sysctrlmem_tlbgather_flush(void *ptr, unsigned long mode)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_UNKNOWN, ptr, mode);
}

static inline int sysctrlmem_tlbgather_unmap_gather_page(void *ptr, unsigned long mode, void *pte,
							 const struct __ctrlmem_kobj_vspace_s *kobjvs,
							 void *va)
{
	return sysctrlmem_vulong(__CTRLMEM_SYSCALL_TLBGATHER_UNMAP, ptr, mode,
				 __sysctrlmem_arg_ptr(pte), __sysctrlmem_val_ulong(0UL), /* pte and upteval */
				 __sysctrlmem_arg_ptr(kobjvs), __sysctrlmem_arg_ptr(va));
}

static inline int sysctrlmem_kobj_process_set_pid(struct __ctrlmem_kobj_process_s *kobj, pid_t pid)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->pid, __sysctrlmem_arg_u32(pid));
}

static inline int sysctrlmem_kobj_process_set_init_load_pct(struct __ctrlmem_kobj_process_s *kobj, __u32 val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->init_load_pct, __sysctrlmem_arg_u32(val));
}

static inline int sysctrlmem_kobj_process_set_load(struct __ctrlmem_kobj_process_s *kobj, __u64 val)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->load, __sysctrlmem_arg_u64(val));
}

static inline int sysctrlmem_kobj_process_set_prev_running_time_ms(struct __ctrlmem_kobj_process_s *kobj,
		__u64 val)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->prev_running_time_ms, __sysctrlmem_arg_u64(val));
}

static inline int sysctrlmem_kobj_process_set_prev_load(struct __ctrlmem_kobj_process_s *kobj, __u64 val)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->prev_load, __sysctrlmem_arg_u64(val));
}

static inline int sysctrlmem_kobj_process_set_boost(struct __ctrlmem_kobj_process_s *kobj, __u32 val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj->boost, __sysctrlmem_arg_u32(val));
}

static inline int sysctrlmem_kobjctnr_spawn_polllist(struct __ctrlmem_kobjctnr_page_freelist_s *fl, unsigned int sz,
						     unsigned long file, __u32 listener_cnt, vatomic32_t* revent)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_POLLLIST, sz),
			      __sysctrlmem_arg_ulong(file),
			      __sysctrlmem_arg_u32(listener_cnt),
			      __sysctrlmem_arg_ulong((uintptr_t)revent));
}

static inline int sysctrlmem_kobjctnr_spawn_sysevent(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
		unsigned int sz)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, fl,
			      __ctrlmem_kobjctnr_write_meta_mkval(__CTRLMEM_KOBJCTNR_META_KOBJ_SYSEVENT, sz));
}

static inline int sysctrlmem_kobjctnr_sysevent_init_futex(struct __ctrlmem_kobj_sysevent_s *sysevent,
		cref_t vspace_cref, void *wseq_ptr_u)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &sysevent->write_seq,
				     __sysctrlmem_arg_u32(__SYSEVENT_ACTION_INIT),
				     __sysctrlmem_arg_u64(vspace_cref), __sysctrlmem_arg_ptr(wseq_ptr_u));
}

static inline int sysctrlmem_kobjctnr_sysevent_read(struct __ctrlmem_kobj_sysevent_s *sysevent,
		char *buf, unsigned int buf_len)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &sysevent->write_seq,
				     __sysctrlmem_arg_u32(__SYSEVENT_ACTION_READ),
				     __sysctrlmem_arg_ptr(buf), __sysctrlmem_arg_u32(buf_len));
}

static inline int sysctrlmem_kobjctnr_sysevent_write(struct __ctrlmem_kobj_sysevent_s *sysevent,
		const char *buf, unsigned int buf_len)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &sysevent->write_seq,
				     __sysctrlmem_arg_u32(__SYSEVENT_ACTION_WRITE),
				     __sysctrlmem_arg_ptr(buf), __sysctrlmem_arg_u32(buf_len));
}

/*
 * This API is used to synchronize the revents of the FD.
 * The potential side effect of this API is the same as
 * sysctrlmem_kobjctnr_set_polllist_revents: wakeup is triggered.
 */
static inline __always_inline
int sysctrlmem_kobjctnr_sync_polllist_revents(struct __ctrlmem_kobj_polllist_s *polllist, unsigned int revents)
{
	int ret = 0;

	if (polllist != NULL) {
		__u32 listener_cnt = vatomic32_read((struct vatomic32_s *)(void *)(&polllist->listener_cnt));
		if (listener_cnt == __CTRLMEM_U32_MAX_VAL) {
			/*
			 * For the FDs that always synchronizes revents to the ctrlmem kobj,
			 * revents are updated only when the revents changes.
			 */
			__u32 old_revents = vatomic32_read_rlx((struct vatomic32_s *)(void *)(&polllist->revents));
			if (revents != old_revents) {
				ret = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &polllist->revents,
						     __sysctrlmem_val_u32(revents),
						     __sysctrlmem_arg_u32(__POLLLIST_UPDATE_REVENTS));
			}
		} else if (listener_cnt != 0U) {
			ret = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &polllist->revents,
					     __sysctrlmem_val_u32(revents),
					     __sysctrlmem_arg_u32(__POLLLIST_UPDATE_REVENTS));
		}
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

static inline __always_inline
int sysctrlmem_kobjctnr_clean_polllist_revents(struct __ctrlmem_kobj_polllist_s *polllist, unsigned int revents)
{
	int ret = 0;

	if (polllist != NULL) {
		__u32 listener_cnt = vatomic32_read_rlx((struct vatomic32_s *)(void *)(&polllist->listener_cnt));
		if (listener_cnt != 0U) {
			ret = sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &polllist->revents,
					     __sysctrlmem_val_u32(revents),
					     __sysctrlmem_arg_u32(__POLLLIST_CLEANUP_REVENTS));
		}
	} else {
		ret = E_HM_INVAL;
	}
	return ret;
}

static inline __always_inline
int sysctrlmem_kobj_vspace_rangeop_mprotect(struct __ctrlmem_kobj_vspace_s *kobj_vspace,
					    void *va_st, void *va_ed, unsigned int modelid)
{
	__u32 opval = __kobjvs_rangeop_mkval(MPROTECT, 0U, modelid, __KOBJVS_RANGEOP_VAL_NRPTES_MASK);
	int err = sysctrlmem_v32(__CTRLMEM_SYSCALL_RANGE_MPROTECT, &kobj_vspace->rangeop.op,
				 __sysctrlmem_val_u32(opval),
				 __sysctrlmem_arg_ptr(va_st),
				 __sysctrlmem_arg_ptr(va_ed),
				 __sysctrlmem_arg_ptr(NULL));
	return err;
}

static inline __always_inline
int sysctrlmem_kobj_vspace_rangeop_munmap(struct __ctrlmem_kobj_vspace_s *kobj_vspace,
					  void *va_st, void *va_ed)
{
	__u32 opval = __kobjvs_rangeop_mkval(MUNMAP, 0U, 0, __KOBJVS_RANGEOP_VAL_NRPTES_MASK);
	int err = sysctrlmem_v32(__CTRLMEM_SYSCALL_RANGE_MUNMAP, &kobj_vspace->rangeop.op,
				 __sysctrlmem_val_u32(opval),
				 __sysctrlmem_arg_ptr(va_st),
				 __sysctrlmem_arg_ptr(va_ed),
				 __sysctrlmem_arg_ptr(NULL));
	return err;
}

#define UNMAP_RANGEOP_NORMAL		0
#define UNMAP_RANGEOP_MADVFREE		1
#define UNMAP_RANGEOP_COPGTABLE		2
#define UNMAP_RANGEOP_CLEAR_REFS	4
static inline __always_inline
int sysctrlmem_kobj_vspace_rangeop_munmap_urecords(struct __ctrlmem_kobj_vspace_s *kobj_vspace,
						   void *va_st, void *va_ed, unsigned int option,
						   struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s *urecords,
						   unsigned int nrptes)
{
	int err;
	__u32 opval;

	if (nrptes > __KOBJVS_RANGEOP_VAL_NRPTES_MASK) {
		err = E_HM_INVAL;
	} else {
		if (option & UNMAP_RANGEOP_MADVFREE) {
			opval = __kobjvs_rangeop_mkval(MADVFREE, __KOBJVS_RANGEOP_VAL_FLAGS_CHANNELPG, 0, nrptes);
		} else if (option & UNMAP_RANGEOP_CLEAR_REFS) {
			opval = __kobjvs_rangeop_mkval(CLEAR_REFS, __KOBJVS_RANGEOP_VAL_FLAGS_CHANNELPG, 0, nrptes);
		} else {
			/* UNMAP_RANGEOP_NORMAL	*/
			opval = __kobjvs_rangeop_mkval(MUNMAP, __KOBJVS_RANGEOP_VAL_FLAGS_CHANNELPG, 0, nrptes);
		}
		err = sysctrlmem_v32(__CTRLMEM_SYSCALL_RANGE_MUNMAP, &kobj_vspace->rangeop.op,
				     __sysctrlmem_val_u32(opval),
				     __sysctrlmem_arg_ptr(va_st),
				     __sysctrlmem_arg_ptr(va_ed),
				     __sysctrlmem_arg_ptr(urecords));
	}
	return err;
}

static inline __always_inline
int sysctrlmem_kobj_vspace_rangeop_vscpy_urecords(struct __ctrlmem_kobj_vspace_s *kobjvs_pt,
						  struct __ctrlmem_kobj_vspace_s *kobjvs_src,
						  struct __ctrlmem_kobj_vspace_s *kobjvs_dest,
						  void *va_st, void *va_ed,
						  unsigned int modelid, bool pmprotect, bool anon,
						  struct __ctrlmem_kobj_vspace_rangeop_vscpy_urecord_s *urecords,
						  unsigned int nrptes)
{
	int err;
	if (nrptes > __KOBJVS_RANGEOP_VAL_NRPTES_MASK) {
		err = E_HM_INVAL;
	} else {
		__u32 opval = __kobjvs_rangeop_mkval(VSCPY,
						     (__KOBJVS_RANGEOP_VAL_FLAGS_CHANNELPG |
						      (pmprotect ? __KOBJVS_RANGEOP_VAL_FLAGS_PMPROTECT : 0U) |
						      (anon ? __KOBJVS_RANGEOP_VAL_FLAGS_ANON : 0U)),
						     modelid, nrptes);
		err = sysctrlmem_v32(__CTRLMEM_SYSCALL_RANGE_VSCPY, &kobjvs_src->rangeop.op,
				     __sysctrlmem_val_u32(opval),
				     __sysctrlmem_arg_ptr(va_st),
				     __sysctrlmem_arg_ptr(va_ed),
				     __sysctrlmem_arg_ptr(urecords),
				     __sysctrlmem_arg_ptr(kobjvs_pt),
				     __sysctrlmem_arg_ptr(kobjvs_dest));
	}
	return err;
}

static inline __always_inline
int sysctrlmem_kobj_vspace_mkdead(struct __ctrlmem_kobj_vspace_s *kobj_vspace)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_VSMKDEAD, &kobj_vspace->busy_idx,
			      __CTRLMEM_VSPACE_DEAD_BUSYIDX);
}

static inline __always_inline
int sysctrlmem_channelpg_unlock(struct __kstack_channel_pages_lockable_head_s *head)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_CHANNELPG_UNLOCK, &head->locked, 0U);
}

static inline __always_inline
int sysctrlmem_sysconf_page_write_bootup_stage(struct __ctrlmem_sysconf_page_s *page, int bootup_stage)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &page->keypoint.bootup_stage, bootup_stage);
}

static inline __always_inline
int sysctrlmem_sysconf_page_write_track_info(struct __ctrlmem_sysconf_page_s *page, unsigned int pause)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &page->track_info.pause, pause);
}

static inline __always_inline
int sysctrlmem_kobj_actv_binder_refer_link(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer,
					   cref_t actv_pool_cref, unsigned int handle)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_ab_refer->op,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_AB_REFER_LINK),
			      __sysctrlmem_arg_u64(actv_pool_cref),
			      __sysctrlmem_arg_u32(handle));
}

static inline __always_inline
int sysctrlmem_kobj_actv_binder_refer_unlink(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_ab_refer->op,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_AB_REFER_UNLINK));
}

static inline __always_inline
int sysctrlmem_kobj_actv_binder_refer_revoke(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &kobj_ab_refer->op,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_AB_REFER_REVOKE));
}

static inline __always_inline
int sysctrlmem_pcpinfo_clear_self_pa(struct __ctrlmem_pcpinfo_s *pcpinfo, __u32 op)
{
	return sysctrlmem_v64(__CTRLMEM_SYSCALL_UNKNOWN, &pcpinfo->self_pa, __sysctrlmem_arg_u32(op));
}

static inline __always_inline
int sysctrlmem_sysconf_page_write_cpu_support_compat_num(struct __ctrlmem_sysconf_page_s *page)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &page->cpu_support_compat_num.cpu_support_compat_num, 0);
}

#endif
