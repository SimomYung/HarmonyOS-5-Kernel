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
#include <hmkernel/futex.h>
#include <hmkernel/ctrlmem/kobjs/vregion.h>
#include <lib/dlist.h>
#include <hmkernel/ipc/hmsignal.h> /* for struct __hmsigmask */
#include <hmkernel/ctrlmem/kobjs/thread/sched.h>
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
#define __AMUDATA_KOBJ_THREAD_SIZE 64
#define __GPDATA_KOBJ_THREAD_SIZE 48

struct __sched_rtsbw_s {
	struct dlist_node restricted_list;
	struct dlist_node downgraded_list;
};

struct deadline_group_s;
struct __kobj_thread_sched_s {
	struct __sched_rtsbw_s rtsbw;
	struct __hmlt_load_s hmlt_load;
	__lt_t lt;
	struct deadline_group_s *dl_grp;
	struct dlist_node dl_link;
	__tcb_ext_sched_t ext_sched;
	struct __tcb_stat_s stat;
};

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
	__uatomic64 win_start_time;
	__uatomic64 time;
	__uatomic64 energy;
#ifdef CONFIG_HISI_CPU_INTELLISENSE
	u8 padding_for_cpu_intellisense[CONFIG_CPU_INTELLISENSE_PADDING_SIZE];
#endif
	__u8 padding_for_amu_info[__AMUDATA_KOBJ_THREAD_SIZE];
	__u8 padding_for_gp_info[__GPDATA_KOBJ_THREAD_SIZE];

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
	unsigned int seccomp_refresh_sc_data;
	struct __kobj_thread_sched_s kobj_sched;
#ifdef CONFIG_IPI_INFO_DEBUG
	struct __tcb_ipi_info_s ipi_info;
#endif
	__bool jitfort_enabled;

	unsigned int prev_cpu;
	__u64 last_cputype_change_time;

	__u64 fetch_runtime;
	__u64 acc_runtime;
	__u64 last_block_time;
	/* Bitmap for hm's signals */
	struct __hmsigmask _pending_hmsignals;

	/* cpu timer rbtree */
	unsigned long cpu_timer_base;
};

#define __CTRLMEM_KOBJCTNR_THREAD_HEADER_SZ sizeof(struct __ctrlmem_kobj_thread_s)
#define __CTRLMEM_KOBJ_THREAD_SZ_IN_PIECES (1 + ((__CTRLMEM_KOBJCTNR_THREAD_HEADER_SZ - 1) \
	/ __CTRLMEM_KOBJCTNR_PIECE_SZ))

#endif
