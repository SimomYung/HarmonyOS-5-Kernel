/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UAPI of internal structure of kobj process
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 18 11:00:00 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_PROCESS_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_PROCESS_H

#include <hmkernel/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/vregion.h>

#define __CTRLMEM_KOBJ_PROCESS_STATE_LOCKED	0xffffffff

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_kobj_process_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobj_process_uatomic32_s __ctrlmem_kobj_process_uatomic32_t;
# define __uatomic32 __ctrlmem_kobj_process_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif /* __KERNEL__ */
#endif /* __uatomic32 */

struct __ctrlmem_kobj_process_s {
	__uatomic32 state_refcnt;
	__u32 pid;
	__u32 init_load_pct;
	__u32 boost;
	union {
		/*
		 * it turns out to be vatomicptr(struct capability_s *) cap_cnode;
		 * user mode don`t use cap_cnode, so we hide it.
		 */
		__u64 padding;
	};
	union {
		struct __ctrlmem_cosched_s *cosched;
		__u64 padding;
	} csp;
	union {
		uintptr_t cosched;
		__u64 padding;
	} csp_uva;
	__bool load_chk;
	__uatomic32 win_idx;
	__uatomic32 win_load;
	/* Total running time of the process */
	__uatomic64 time;
	/*
	 * Total energy of the process, which is the sum of
	 * running time multiplies the power at the cpu frequency
	 */
	__uatomic64 energy;
	__uatomic64 utime;
	__uatomic64 stime;
	__uatomic64 prev_running_time_ms;
	__uatomic64 load;
	__uatomic64 prev_load;

	uintptr_t kobj_ulwt_padding;
	uintptr_t ulwt_mon_page_padding;
	uintptr_t ulwt_infopage_padding;
	__uptr_t ulwt_kinfo_lock_wrapper_padding[2];
	__uptr_t kobj_ulwt_kinfo_padding;
	__uptr_t ulwt_kinfopage_padding;

	__uatomic64 itimer_prof_expire;
	__uatomic64 itimer_prof_interval;
	__uatomic64 itimer_virt_expire;
	__uatomic64 itimer_virt_interval;
	__uatomic64 jitfort_status;
	unsigned long cpu_timer_base;
};

#endif
