/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: User API of kobject thread
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 29 06:35:02 2023
 */
#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_THREAD_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_THREAD_H

#include <hmkernel/types.h>
#include <hmasm/registers.h>
#include <hmkernel/tcb_stat.h>
#include <hmkernel/tcb_sched.h>
#include <hmkernel/ctrlmem/kobjs/vregion.h>
#include <lib/dlist.h>
#include <hmkernel/ipc/hmsignal.h> /* for struct __hmsigmask */
#include <hmkernel/ulwt/kinfo.h>
#include <hmkernel/activation/local_area.h>

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_kobj_thread_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobj_thread_uatomic32_s __ctrlmem_kobj_thread_uatomic32_t;
# define __uatomic32 __ctrlmem_kobj_thread_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif /* __KERNEL__ */
#endif /* __uatomic32 */

#define __MISCDATA_KOBJ_THREAD_SIZE 10
#define __BASE_ACTV_FRAME_SZ_NR_UL  8

struct __kobj_thread_sched {
#ifdef CONFIG_RT_SOFT_BANDWIDTH
	struct dlist_node restricted_list;
	struct dlist_node downgraded_list;
#endif
};

#ifdef CONFIG_CPU_INTELLISENSE
struct pmu_feature {
	int mem_bound_flag;
	__u32 sample_count;
	__u64 mem_bound_avg;
};

struct period_info {
	__u32 cnt;
	__u64 sat_val;
	__u64 waterline;
	__u64 last_info;
};
#endif

struct __ctrlmem_kobj_thread_s {
	union {
		struct {
			vatomic32_t refcnt;
		};
		__u64 padding[2]; /* 2 * 64: one piece */
	} __aligned(16);
	union {
		__u64 tcb;
		__u64 padding2;
	};
	struct {} boundary;
	__bool load_chk;
	__uatomic32 win_idx;
	__uatomic32 win_load;
	__uatomic64 time;
	__uatomic64 energy;
#ifdef CONFIG_CPU_INTELLISENSE
	struct period_info period_info;
	struct pmu_feature pmu_feature;
#endif

	struct __ulwt_kinfo_thread_data_s __ulwt_kinfo_data;

	__uatomic64 timerslack_current_ns;
	__uatomic64 timerslack_default_ns;
#ifdef CONFIG_AARCH64_SVE
	union {
		struct arch_fpu_regs fpu_regs;
		struct arch_sve_regs sve_regs;
	} __aligned(16);
#else
	struct arch_fpu_regs fpu_regs;
#endif

	unsigned long actv_frame[__BASE_ACTV_FRAME_SZ_NR_UL];
	unsigned long miscdata[__MISCDATA_KOBJ_THREAD_SIZE];
	struct __kobj_thread_sched rtsbw;
	struct __tcb_stat_s stat;
#ifdef CONFIG_IPI_INFO_DEBUG
	struct __tcb_ipi_info_s ipi_info;
#endif
	struct __tcb_ext_sched_s sched;
	__bool jitfort_enabled;

	unsigned int seccomp_refresh_sc_data;
	unsigned int prev_cpu;
	__u64 last_cputype_change_time;

	__u64 fetch_runtime;
	__u64 acc_runtime;
	__u64 last_block_time;
	/* Bitmap for hm's signals */
	struct __hmsigmask _pending_hmsignals;
	struct __block_info_s block_info;

	/* cpu timer rbtree */
	unsigned long cpu_timer_base;
#if defined(CONFIG_MODULE_MIPS_LOAD_TRACKING)
	struct __tcb_mips_demand tmipsd;
#endif
};

#define __CTRLMEM_KOBJCTNR_THREAD_HEADER_SZ sizeof(struct __ctrlmem_kobj_thread_s)
#define __CTRLMEM_KOBJ_THREAD_SZ_IN_PIECES (1 + ((__CTRLMEM_KOBJCTNR_THREAD_HEADER_SZ - 1) \
	/ __CTRLMEM_KOBJCTNR_PIECE_SZ))

#endif
