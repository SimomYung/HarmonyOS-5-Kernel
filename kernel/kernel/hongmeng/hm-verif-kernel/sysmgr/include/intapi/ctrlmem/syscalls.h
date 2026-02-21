/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem syscall wrapper
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 09 10:44:12 2023
 */

#ifndef INTAPI_CTRLMEM_SYSCALLS_H
#define INTAPI_CTRLMEM_SYSCALLS_H

#ifdef CONFIG_CTRLMEM_SYSCONF_DIRECT_SPINCALL

#ifndef __ctrlmem_syscall_override__
# define __ctrlmem_syscall_override__
#endif

#include <intapi/ctrlmem/sysconf.h>
#include <intapi/mem/linear_map.h>
/*
 * For privileged services, we have a globally mapped
 * sysconf spincall board which allows us invoke ctrlmem syscall
 * through function call. All arguments are kept as is.
 * function is ctrlmem_sysconf_spincalls_write_v{32,64}, which
 * is provided by intapi/ctrlmem/sysconf.h
 */
# define __sysctrlmem_arg_u64(x)	(__u64)(x)
# define __sysctrlmem_arg_u32(x)	(__u32)(x)
# define __sysctrlmem_arg_ulong(x)	(unsigned long)(x)
# define __sysctrlmem_arg_ptr(x)	(void *)(x)
# define __sysctrlmem_arg_paddr(x)	(__paddr_t)(x)
# define __sysctrlmem_val_u64(v)	(__u64)(v)
# define __sysctrlmem_val_u32(v)	(__u32)(v)
# define __sysctrlmem_fn(v, n)		ctrlmem_sysconf_spincalls_write_v##v##_arg##n
# define __sysctrlmem_cas_fn(v, n)	ctrlmem_sysconf_spincalls_cas_v##v##_arg##n

# ifdef CONFIG_CTRLMEM_SYSCONF_FAST_DIRECT_SPINCALL
# define sysctrlmem_pgtblull_cas_map_pgva __sysctrlmem_pgtblull_cas_map_pgva
# define sysctrlmem_pgtblull_cas_map_pgstrva __sysctrlmem_pgtblull_cas_map_pgstrva
# define sysctrlmem_pgtblull_clean_set_kobjvs __sysctrlmem_pgtblull_clean_set_kobjvs
# define sysctrlmem_pgtblull_cas_remap_pgva_kobjvs __sysctrlmem_pgtblull_cas_remap_pgva_kobjvs
# define sysctrlmem_tlbgather_unmap_gather_page __sysctrlmem_tlbgather_unmap_gather_page
# define sysctrlmem_pgtblull_clear_abit __sysctrlmem_pgtblull_clear_abit
# define sysctrlmem_channelpg_unlock __sysctrlmem_channelpg_unlock
# endif

#endif
#include <libhmsrv_sys/hm_ctrlmem_syscalls_layer.h>

# ifdef CONFIG_CTRLMEM_SYSCONF_FAST_DIRECT_SPINCALL
static inline int sysctrlmem_pgtblull_map_pg_fast(void *pte, const void *pg_pa, void *va,
						  unsigned int modelid)
{
	return ctrlmem_sysconf_spincalls_map_pg_fast(sys_va_to_pa((uintptr_t)pte),
						     (__paddr_t)pg_pa,
						     (uintptr_t)va, modelid);
}

static inline int sysctrlmem_pgtblull_map_batch_fast(void *pte, unsigned long batch_num, void *info,
						  void *va, unsigned int modelid)
{
	return ctrlmem_sysconf_spincalls_map_batch_fast(sys_va_to_pa((uintptr_t)pte),
						     batch_num, info,
						     (uintptr_t)va, modelid);
}

static inline __always_inline
int sysctrlmem_kobj_vspace_rangeop_munmap_urecords_fast(struct __ctrlmem_kobj_vspace_s *kobj_vspace,
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
		err = ctrlmem_sysconf_spincalls_unmap_range_fast((__paddr_t)sys_va_to_pa((uintptr_t)kobj_vspace),
				     __sysctrlmem_val_u32(opval),
				     (uintptr_t)va_st,
				     (uintptr_t)va_ed,
				     (uintptr_t)urecords);
	}
	return err;
}

static inline int sysctrlmemfast_pgtblull_cas_map_pgva(void *pte, const void *pgva, void *va,
						   unsigned int modelid, pte_t old_ptev)
{
	return ctrlmem_sysconf_spincalls_cas_map_pg(sys_va_to_pa((uintptr_t)pte),
						    sys_va_to_pa((uintptr_t)pgva), (uintptr_t)va,
						    old_ptev, modelid);
}

static inline int sysctrlmemfast_pgtblull_cas_map_pgstrva(void *pte, const struct pgstr_s *pgstr, void *va,
						      unsigned long modelid, __paddr_t pa, pte_t old_ptev)
{
	UNUSED(pgstr);
	return ctrlmem_sysconf_spincalls_cas_map_pg(sys_va_to_pa((uintptr_t)pte),
						    pa, (uintptr_t)va, old_ptev, modelid);
}

static inline int sysctrlmemfast_pgtblull_clean_set_kobjvs(void *pte, pte_t ptev,
							   const struct __ctrlmem_kobj_vspace_s *kobjvs,
							   void *va)
{
	return ctrlmem_sysconf_spincalls_clean_set_kobjvs(sys_va_to_pa((uintptr_t)pte),
							  (uintptr_t)kobjvs, (uintptr_t)va, ptev);
}

static inline int sysctrlmemfast_pgtblull_cas_remap_pgva_kobjvs(void *pte, const void *pgva,
							    const struct __ctrlmem_kobj_vspace_s *kobjvs,
							    void *va, unsigned int modelid,
							    pte_t old_ptev)
{
	return ctrlmem_sysconf_spincalls_remap_pgva_kobjvs(sys_va_to_pa((uintptr_t)pte),
							   sys_va_to_pa((uintptr_t)pgva), (uintptr_t)kobjvs, (uintptr_t)va, old_ptev, modelid);
}

static inline int sysctrlmemfast_tlbgather_unmap_gather_page(void *ptr, unsigned long mode, void *pte,
							 const struct __ctrlmem_kobj_vspace_s *kobjvs,
							 void *va)
{
	UNUSED(mode);
	uintptr_t tlbg_pa = sys_va_to_pa((uintptr_t)ptr);
	return ctrlmem_sysconf_spincalls_tlbgather_unmap(sys_va_to_pa((uintptr_t)pte), tlbg_pa,
							 (uintptr_t)kobjvs, (uintptr_t)va, 0UL);
}

static inline int sysctrlmemfast_pgtblull_clear_abit(void *pte, const struct __ctrlmem_kobj_vspace_s *kobjvs, void *va)
{
	UNUSED(kobjvs, va);
	return ctrlmem_sysconf_spincalls_clear_abit(sys_va_to_pa((uintptr_t)pte));
}

struct __kstack_channel_pages_lockable_head_s;
static inline int sysctrlmemfast_channelpg_unlock(struct __kstack_channel_pages_lockable_head_s *head)
{
	return ctrlmem_sysconf_spincalls_channelpg_unlock(sys_va_to_pa((uintptr_t)head));
}

# undef sysctrlmem_pgtblull_cas_map_pgva
# define sysctrlmem_pgtblull_cas_map_pgva sysctrlmemfast_pgtblull_cas_map_pgva
# undef sysctrlmem_pgtblull_cas_map_pgstrva
# define sysctrlmem_pgtblull_cas_map_pgstrva sysctrlmemfast_pgtblull_cas_map_pgstrva
# undef sysctrlmem_pgtblull_clean_set_kobjvs
# define sysctrlmem_pgtblull_clean_set_kobjvs sysctrlmemfast_pgtblull_clean_set_kobjvs
# undef sysctrlmem_pgtblull_cas_remap_pgva_kobjvs
# define sysctrlmem_pgtblull_cas_remap_pgva_kobjvs sysctrlmemfast_pgtblull_cas_remap_pgva_kobjvs
# undef sysctrlmem_tlbgather_unmap_gather_page
# define sysctrlmem_tlbgather_unmap_gather_page sysctrlmemfast_tlbgather_unmap_gather_page
# undef sysctrlmem_pgtblull_clear_abit
# define sysctrlmem_pgtblull_clear_abit sysctrlmemfast_pgtblull_clear_abit
# undef sysctrlmem_channelpg_unlock
# define sysctrlmem_channelpg_unlock sysctrlmemfast_channelpg_unlock
# undef sysctrlmem_kobj_vspace_rangeop_munmap_urecords
# define sysctrlmem_kobj_vspace_rangeop_munmap_urecords sysctrlmem_kobj_vspace_rangeop_munmap_urecords_fast
# endif
#endif
