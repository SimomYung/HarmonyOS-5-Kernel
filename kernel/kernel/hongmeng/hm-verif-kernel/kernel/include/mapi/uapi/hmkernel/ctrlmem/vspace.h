/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: vspace control structures - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 08 00:08:28 2021
 */
#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_VSPACE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_VSPACE_H

#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/capability.h>

#include <hmasm/ctrlmem/vspace.h>

#define __CTRLMEM_VSPACE_FLAG_PRIVILEGED	1
#define __CTRLMEM_VSPACE_FLAG_NOREVOKE		2
#define __CTRLMEM_VSPACE_FLAG_SUPERVS		4

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_vspace_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_vspace_uatomic32_s __ctrlmem_vspace_uatomic32_t;
#  define __uatomic32 __ctrlmem_vspace_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif
#endif

/*
 * 4 * u64 for each slot (32 bytes)
 */
struct __ctrlmem_vspace_slot_s {
	/*
	 * An identifier for vspace. Should be moved to
	 * physical space of level 0 page table or content of TTBR
	 * when needed.
	 */
	__u64			vspace_cref;
	__u64			kobj_vspace;
	__u32			flags;
	__u32			padding;
	/*
	 * State transision rule of refcnt_state:
	 *   FREE <--> Updating <--> Occupied/Offline <--> Occupied/Online(1) <--> Occupied/Online(2) ...
	 * Updating state is important. It gives a safe state to flush tlb.
	 * If directly moving from Offline to FREE, it would be impossible to flush tlb correctly.
	 * Thinking following event ordering:
	 * CPU0:  Online(1) --> Assigned --> flush tlb ---------> FREE
	 *                            \                    /
	 *                             |                   |
	 *                             V                   |
	 * CPU1:                       Online(1) --> Assigned
	 *
	 * Without Updating state, CPU1 is able to online the vspace after flush tlb, give
	 * new vspace a dirty tlb state.
	 *
	 * Updating state is a transient state. The one who move a slot to Updating state
	 * "own" this slot, but must move it to Offline or FREE in fixed number of instructions.
	 * So other observers use a loop to wait its result.
	 */
#define __CTRLMEM_VSPACE_STATE_FREE	0
#define __CTRLMEM_VSPACE_STATE_UPDATING	0x20000000
	/*
	 * ASSIGNED is originally named OFFLINE, which means slot is assigned to a vspace but
	 * not onlined. In current design we don't trace how many cores are using this vspace
	 * to avoid atomic refcnt inc/dec, so rename it from OFFLINE to ASSIGNED.
	 */
#define __CTRLMEM_VSPACE_STATE_ASSIGNED	0x40000000
	/*
	 * PROBING state natually take an reference count with it.
	 * We can directly define __CTRLMEM_VSPACE_STATE_REVOKE_PROBING
	 * to 0x60000001 but then we must remember we should use
	 * (v & __CTRLMEM_VSPACE_STATE_MASK_STATE) ==
	 *     (__CTRLMEM_VSPACE_STATE_REVOKE_DECIDED & __CTRLMEM_VSPACE_STATE_MASK_STATE)
	 */
#define __CTRLMEM_VSPACE_STATE_REVOKE_PROBING	0x60000000
#define __CTRLMEM_VSPACE_STATE_REVOKE_DECIDED	0x80000000

#define __CTRLMEM_VSPACE_STATE_MASK_STATE	0xe0000000
#define __CTRLMEM_VSPACE_STATE_MASK_REFCNT	0x1fffffff
	/*
	 * NOTE that this macro should only be used in LLT. In kernel we should never directly
	 * set state explicitly to an reference number.
	 */
#define __ctrlmem_vspace_state_refcnt(x)	((typeof(x))(__CTRLMEM_VSPACE_STATE_ASSIGNED) | (x))
	__uatomic32		refcnt_state;
	/*
	 * A bitmask to record online cpuset. Internal structure is same as cpuset_t.
	 */
};

struct __ctrlmem_vspace_online_vs_s {
	__u64 vspace_cref;

	/*
	 * Record busy idx in each CPU.
	 * Before updating TTBR0, should set next busy idx in higher 32 bits, so
	 * revokeer is able to see it.
	 * No need to use atomic type: CPU online it should use normal load and
	 * store to update this variable. Following slot->state should use
	 * cmpxchg_rel, which make sure change to __busy_idx_pair should be seen
	 * to revokeer.
	 */
#define __CTRLMEM_VSPACE_ONLINE_VS_BUSY_IDX_PAIR_SHIFT	32
#define __ctrlmem_vspace_online_vs_curr_busy_idx(x)	(__u32)((x) & (__u64)0xffffffff)
#define __ctrlmem_vspace_online_vs_next_busy_idx(x)	(__u32)(((x) >> \
								__CTRLMEM_VSPACE_ONLINE_VS_BUSY_IDX_PAIR_SHIFT) & \
								(__u64)0xffffffff)
	__u64 __busy_idx_pair;
};

struct __ctrlmem_vspace_online_pcp_s {
	/*
	 * un: unprivileged
	 * pr: privileged
	 */
	struct __ctrlmem_vspace_online_vs_s un;
	struct __ctrlmem_vspace_online_vs_s pr;
};

#define __CTRLMEM_VSCTRL_VERSION		0x1
#define __CTRLMEM_VSCTRL_MAX_CPU_CNT		32
#define __CTRLMEM_VSCTRL_NR_SYSTEM_SERVICES	24

struct __ctrlmem_vspace_version_s {
	__u64 meta;
	__u64 version;
	__u32 nr_slots;
	__u32 nr_cpus;
	__u32 nr_supervss;
	__u32 padding;
};

struct __ctrlmem_vspace_supervs_s {
	__u64 paddr;
};

struct __ctrlmem_vspace_array_s {
	struct __ctrlmem_vspace_version_s	version;
	struct __ctrlmem_vspace_slot_s		slots[__UL(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)];
	struct __ctrlmem_vspace_supervs_s	supervss[__UL(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)];
	/*
	 * CPUS must be the last one because __CTRLMEM_VSCTRL_MAX_CPU_CNT
	 * is possible !=CONFIG_NR_CPUS
	 */
	struct __ctrlmem_vspace_online_pcp_s	cpus[__UL(__CTRLMEM_VSCTRL_MAX_CPU_CNT)];
};

/*
 * Encode a unsigned idx to signed retval.
 * if idx is invalid, return __ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR, else
 * return E_HM_AGAIN when idx is invalid.
 */
#define __actv_wferet_vsfault_encode_idx(idx)	\
	(((unsigned int)(idx) >= __U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)) ? \
	 (int)(E_HM_AGAIN) : (int)((unsigned int)(idx) + 1u))

#define ____actv_wferet_vsfault_decode_idx_pos_valid(x)	\
	((unsigned int)(x) - 1u)
#define ____actv_wferet_vsfault_decode_idx_pos_inval(x)	\
	__U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)
#define ____actv_wferet_vsfault_decode_idx_nz(x) \
	__U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)
#define ____actv_wferet_vsfault_decode_idx_pos(x)	\
	(((unsigned int)(x) > __U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR)) ? \
	 ____actv_wferet_vsfault_decode_idx_pos_inval(x) : \
	 ____actv_wferet_vsfault_decode_idx_pos_valid(x))
#define __actv_wferet_vsfault_decode_idx(x)		\
	(((int)(x) <= (int)(0)) ? \
	 ____actv_wferet_vsfault_decode_idx_nz(x) : \
	 ____actv_wferet_vsfault_decode_idx_pos(x))

/*
 * Sysmgr should avoid assign slot in supervs area.
 */
static inline __bool
__ctrlmem_vspace_is_idx_supervs(unsigned int busy_idx, const struct __ctrlmem_vspace_version_s *v)
{
	__bool result = __false;

	if (busy_idx >= v->nr_slots) {
		result = __false;
	} else if (busy_idx < (v->nr_slots - v->nr_supervss)) {
		result = __false;
	} else {
		result = __true;
	}
	return result;
}

#define __CTRLMEM_VSPACE_INVALID_BUSYIDX	__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR
#define __CTRLMEM_VSPACE_DEAD_BUSYIDX		(__CTRLMEM_VSPACE_INVALID_BUSYIDX + 1U)

#endif
