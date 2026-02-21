/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UAPI of internal structure of kobj vspace
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 06 15:50:25 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_H

#include <hmkernel/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/vspace/rangeop.h>
#include <hmkernel/ctrlmem/kobjs/vregion.h>

#define __CTRLMEM_KOBJ_VSPACE_STATE_LOCKED	0xffffffff

struct __ctrlmem_kobj_pcache_s;

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_kobj_vspace_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobj_vspace_uatomic32_s __ctrlmem_kobj_vspace_uatomic32_t;
# define __uatomic32 __ctrlmem_kobj_vspace_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif /* __KERNEL__ */
#endif /* __uatomic32 */

struct __ctrlmem_kobj_vspace_s {
	__uatomic32 state_refcnt;
	__u32 flags;
	union {
		struct __ctrlmem_kobj_vspace_rangeop_op_s rangeop;
		__uatomic32 __rangeop_csstate;
	};
	__uatomic32 busy_idx;
	union {
		/* read */
		void *kptr_cap_vspace;

		/* write */
		__u64 vspace_cref;
	} u;
	__u64 ttbrv;
	struct __ctrlmem_sysparule_s sysparule;
	union {
		__ctrlmem_syspa_t vrshared;
		__u64 padding2;
	} vrs;
	union {
		struct __ctrlmem_kobj_pcache_s *kobj_pcache;
		__u64 padding;
	} spc;
	union {
		__u64 kptr;
	} ext;
	__u64 vs_key;
	void *copgtable_linked_kobjvs;
	__uatomic32 membarrier_state;
	__u32 padding_supervsidx;
};

#define __CTRLMEM_KOBJ_VSPACE_CTOR_FLAG_PRIVILEGED	1
#define __CTRLMEM_KOBJ_VSPACE_CTOR_FLAG_HAS_LINEAR_MAP	2
#define __CTRLMEM_KOBJ_VSPACE_CTOR_FLAG_SUPERVS		4
#define __CTRLMEM_KOBJ_VSPACE_FLAG_SUPERVSABLE		8
#define __CTRLMEM_KOBJ_VSPACE_CTOR_FLAG_COPGTABLEVS		16

#define __CTRLMEM_KOBJ_VSPACE_CTOR_FLAG_SUPERVSABLE	__CTRLMEM_KOBJ_VSPACE_FLAG_SUPERVSABLE

#define __CTRLMEM_KOBJ_VSPACE_INVOKE_FLAG_SUPERVSABLE	__CTRLMEM_KOBJ_VSPACE_FLAG_SUPERVSABLE

#endif
