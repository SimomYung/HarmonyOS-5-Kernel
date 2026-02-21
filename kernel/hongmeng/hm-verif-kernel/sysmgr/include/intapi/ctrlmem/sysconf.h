/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Ctrlmem sysconf in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 05 03:26:26 2023
 */

#ifndef INTAPI_CTRLMEM_SYSCONF_H
#define INTAPI_CTRLMEM_SYSCONF_H

#include <hmasm/memory.h>
#include <hmkernel/ctrlmem.h>
#include <hmkernel/ctrlmem/sysconf.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

#include <stdbool.h>
#include <intapi/vfs/file.h>

#ifdef CONFIG_CTRLMEM_SYSCONF_FAST_DIRECT_SPINCALL
#define CTRLMEM_ARG_LIST0
#define CTRLMEM_ARG_LIST1	, unsigned long arg1
#define CTRLMEM_ARG_LIST2	CTRLMEM_ARG_LIST1, unsigned long arg2
#define CTRLMEM_ARG_LIST3	CTRLMEM_ARG_LIST2, unsigned long arg3
#define CTRLMEM_ARG_LIST4	CTRLMEM_ARG_LIST3, unsigned long arg4
#define CTRLMEM_ARG_LIST5	CTRLMEM_ARG_LIST4, unsigned long arg5
#define CTRLMEM_ARG_LIST6	CTRLMEM_ARG_LIST5, unsigned long arg6

#define CTRLMEM_TYPE_LIST0
#define CTRLMEM_TYPE_LIST1	, unsigned long
#define CTRLMEM_TYPE_LIST2	CTRLMEM_TYPE_LIST1, unsigned long
#define CTRLMEM_TYPE_LIST3	CTRLMEM_TYPE_LIST2, unsigned long
#define CTRLMEM_TYPE_LIST4	CTRLMEM_TYPE_LIST3, unsigned long
#define CTRLMEM_TYPE_LIST5	CTRLMEM_TYPE_LIST4, unsigned long
#define CTRLMEM_TYPE_LIST6	CTRLMEM_TYPE_LIST5, unsigned long

#define CTRLMEM_PARAM_LIST0
#define CTRLMEM_PARAM_LIST1	, arg1
#define CTRLMEM_PARAM_LIST2	CTRLMEM_PARAM_LIST1, arg2
#define CTRLMEM_PARAM_LIST3	CTRLMEM_PARAM_LIST2, arg3
#define CTRLMEM_PARAM_LIST4	CTRLMEM_PARAM_LIST3, arg4
#define CTRLMEM_PARAM_LIST5	CTRLMEM_PARAM_LIST4, arg5
#define CTRLMEM_PARAM_LIST6	CTRLMEM_PARAM_LIST5, arg6

#define declare_ctrlmem_sysconf_spincall_fast(name, x) \
extern long ctrlmem_sysconf_spincalls_##name(unsigned long CTRLMEM_TYPE_LIST##x)

declare_ctrlmem_sysconf_spincall_fast(map_pg_fast, 3);
declare_ctrlmem_sysconf_spincall_fast(map_batch_fast, 4);
declare_ctrlmem_sysconf_spincall_fast(cas_map_pg, 4);
declare_ctrlmem_sysconf_spincall_fast(clean_set_kobjvs, 3);
declare_ctrlmem_sysconf_spincall_fast(remap_pgva_kobjvs, 5);
declare_ctrlmem_sysconf_spincall_fast(tlbgather_unmap, 4);
declare_ctrlmem_sysconf_spincall_fast(clear_abit, 0);
declare_ctrlmem_sysconf_spincall_fast(channelpg_unlock, 0);
declare_ctrlmem_sysconf_spincall_fast(unmap_range_fast, 4);
#endif

void ctrlmem_sysconf_init(const __u64 *meta);

extern long ctrlmem_sysconf_spincalls_write_v32(unsigned long callno, void *ptr, __u32 val, unsigned int argc, ...);
extern long ctrlmem_sysconf_spincalls_write_v64(unsigned long callno, void *ptr, __u64 val, unsigned int argc, ...);
extern long ctrlmem_sysconf_spincalls_cas_v32(unsigned long callno, void *ptr,
					      __u32 orig_val, __u32 val, unsigned int argc, ...);
extern long ctrlmem_sysconf_spincalls_cas_v64(unsigned long callno, void *ptr,
					      __u64 orig_val, __u64 val, unsigned int argc, ...);

#define ctrlmem_sysconf_spincalls_write_v32_arg0(callno, ptr, val)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 0)
#define ctrlmem_sysconf_spincalls_write_v32_arg1(callno, ptr, val, arg1)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 1, arg1)
#define ctrlmem_sysconf_spincalls_write_v32_arg2(callno, ptr, val, arg1, arg2)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 2, arg1, arg2)
#define ctrlmem_sysconf_spincalls_write_v32_arg3(callno, ptr, val, arg1, arg2, arg3)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 3, arg1, arg2, arg3)
#define ctrlmem_sysconf_spincalls_write_v32_arg4(callno, ptr, val, arg1, arg2, arg3, arg4)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 4, arg1, arg2, arg3, arg4)
#define ctrlmem_sysconf_spincalls_write_v32_arg5(callno, ptr, val, arg1, arg2, arg3, arg4, arg5)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 5, arg1, arg2, arg3, arg4, arg5)
#define ctrlmem_sysconf_spincalls_write_v32_arg6(callno, ptr, val, arg1, arg2, arg3, arg4, arg5, arg6)	\
			ctrlmem_sysconf_spincalls_write_v32(callno, ptr, val, 6, arg1, arg2, arg3, arg4, arg5, arg6)

#define ctrlmem_sysconf_spincalls_write_v64_arg0(callno, ptr, val)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 0)
#define ctrlmem_sysconf_spincalls_write_v64_arg1(callno, ptr, val, arg1)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 1, arg1)
#define ctrlmem_sysconf_spincalls_write_v64_arg2(callno, ptr, val, arg1, arg2)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 2, arg1, arg2)
#define ctrlmem_sysconf_spincalls_write_v64_arg3(callno, ptr, val, arg1, arg2, arg3)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 3, arg1, arg2, arg3)
#define ctrlmem_sysconf_spincalls_write_v64_arg4(callno, ptr, val, arg1, arg2, arg3, arg4)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 4, arg1, arg2, arg3, arg4)
#define ctrlmem_sysconf_spincalls_write_v64_arg5(callno, ptr, val, arg1, arg2, arg3, arg4, arg5)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 5, arg1, arg2, arg3, arg4, arg5)
#define ctrlmem_sysconf_spincalls_write_v64_arg6(callno, ptr, val, arg1, arg2, arg3, arg4, arg5, arg6)	\
			ctrlmem_sysconf_spincalls_write_v64(callno, ptr, val, 6, arg1, arg2, arg3, arg4, arg5, arg6)

#define ctrlmem_sysconf_spincalls_cas_v32_arg0(callno, ptr, orig_val, val)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 0)
#define ctrlmem_sysconf_spincalls_cas_v32_arg1(callno, ptr, orig_val, val, arg1)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 1, arg1)
#define ctrlmem_sysconf_spincalls_cas_v32_arg2(callno, ptr, orig_val, val, arg1, arg2)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 2, arg1, arg2)
#define ctrlmem_sysconf_spincalls_cas_v32_arg3(callno, ptr, orig_val, val, arg1, arg2, arg3)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 3, arg1, arg2, arg3)
#define ctrlmem_sysconf_spincalls_cas_v32_arg4(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 4, arg1, arg2, arg3, arg4)
#define ctrlmem_sysconf_spincalls_cas_v32_arg5(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4, arg5)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 5, arg1, arg2, arg3, arg4, arg5)
#define ctrlmem_sysconf_spincalls_cas_v32_arg6(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4, arg5, arg6)	\
			ctrlmem_sysconf_spincalls_cas_v32(callno, ptr, orig_val, val, 6, \
							  arg1, arg2, arg3, arg4, arg5, arg6)

#define ctrlmem_sysconf_spincalls_cas_v64_arg0(callno, ptr, orig_val, val)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 0)
#define ctrlmem_sysconf_spincalls_cas_v64_arg1(callno, ptr, orig_val, val, arg1)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 1, arg1)
#define ctrlmem_sysconf_spincalls_cas_v64_arg2(callno, ptr, orig_val, val, arg1, arg2)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 2, arg1, arg2)
#define ctrlmem_sysconf_spincalls_cas_v64_arg3(callno, ptr, orig_val, val, arg1, arg2, arg3)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 3, arg1, arg2, arg3)
#define ctrlmem_sysconf_spincalls_cas_v64_arg4(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 4, arg1, arg2, arg3, arg4)
#define ctrlmem_sysconf_spincalls_cas_v64_arg5(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4, arg5)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 5, arg1, arg2, arg3, arg4, arg5)
#define ctrlmem_sysconf_spincalls_cas_v64_arg6(callno, ptr, orig_val, val, arg1, arg2, arg3, arg4, arg5, arg6)	\
			ctrlmem_sysconf_spincalls_cas_v64(callno, ptr, orig_val, val, 6, \
							  arg1, arg2, arg3, arg4, arg5, arg6)

struct __ctrlmem_sysconf_global_s;
extern struct __ctrlmem_sysconf_global_s *ctrlmem_sysconf_global(void);

struct __ctrlmem_sysconf_sysproc_s;
extern struct __ctrlmem_sysconf_sysproc_s *ctrlmem_sysconf_sysproc(void);

static inline bool ctrlmem_sysconf_global_is_ctrlmem_pgfault_enabled(void)
{
	return (ctrlmem_sysconf_global()->is_ctrlmem_pgfault_enabled == 0) ? false : true;
}

static inline bool ctrlmem_sysconf_global_is_spincall_cocall_enabled(void)
{
	return (ctrlmem_sysconf_global()->is_ctrlmem_spincall_cocall_enabled == 0) ? false : true;
}

static inline bool ctrlmem_sysconf_global_is_pte_access_flag_enabled(void)
{
	return (ctrlmem_sysconf_global()->is_ctrlmem_pte_access_flag_enabled == 0) ? false : true;
}

static inline bool ctrlmem_sysconf_global_is_pgtbl_fragile_enabled(void)
{
	return (ctrlmem_sysconf_global()->is_pgtbl_fragile_enabled == 0) ? false : true;
}

static inline unsigned long ctrlmem_sysconf_sysproc_initrd_size(void)
{
	return ctrlmem_sysconf_sysproc()->sysproc_initrd_size;
}

extern struct __ctrlmem_sysconf_phymem_s *ctrlmem_sysconf_phymem(void);

extern struct __ctrlmem_sysconf_klayout_s *ctrlmem_sysconf_klayout(void);
extern void *ctrlmem_sysconf_klayout_pa_to_va(__paddr_t pa);
extern bool ctrlmem_sysconf_klayout_is_va_linear(__vaddr_t _va);
extern bool ctrlmem_sysconf_klayout_is_pa_linear(__paddr_t pa);
__paddr_t ctrlmem_sysconf_klayout_low_pa_start(void);
__paddr_t ctrlmem_sysconf_klayout_low_pa_end(void);
bool ctrlmem_is_paddr_needref(__paddr_t pa);


extern int ctrlmem_sysconf_set_emrg_ufs_flush(void);

#ifdef CONFIG_HMPERF_NG
extern int ctrlmem_perf_read_max_sample_rate(void);
extern int ctrlmem_perf_write_max_sample_rate(int max_sample_rate);
extern int ctrlmem_perf_read_max_cpu_percent(void);
extern int ctrlmem_perf_write_max_cpu_percent(int max_cpu_percent);
#else
static inline int ctrlmem_perf_read_max_sample_rate(void)
{
	return 0;
}

static inline int ctrlmem_perf_write_max_sample_rate(int max_sample_rate)
{
	UNUSED(max_sample_rate);
	return E_HM_NOSYS;
}

static inline int ctrlmem_perf_read_max_cpu_percent(void)
{
	return 0;
}

static inline int ctrlmem_perf_write_max_cpu_percent(int max_cpu_percent)
{
	UNUSED(max_cpu_percent);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_CTRLMEM_SYSCONF_BOOTUP_KEYPOINT
extern int ctrlmem_bootup_keypoint_write_stage(int bootup_stage);
#else
static inline int ctrlmem_bootup_keypoint_write_stage(int bootup_stage)
{
	UNUSED(bootup_stage);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_CPU_UPDATE_TICK_INTERVAL
extern int ctrlmem_pcptick_write_interval(unsigned int cpu, __u32 tick_interval_ms);
#else
static inline int ctrlmem_pcptick_write_interval(unsigned int cpu, __u32 tick_interval_ms)
{
	UNUSED(cpu, tick_interval_ms);
	return E_HM_NOSYS;
}
#endif

struct __ctrlmem_sysconf_cpuinfo_s;
extern struct __ctrlmem_sysconf_cpuinfo_s *ctrlmem_sysconf_cpuinfo(void);

extern int ctrlmem_sysconf_reserved_memory_read_config(unsigned int type, __u64 *paddr, __u32 *size);

extern int ctrlmem_sysconf_spinhdlr_cpu_support_compat_num(void);
#endif
