/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Event Channel uapi interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 22 16:33:09 2022
 */

#ifndef UAPI_HMKERNEL_DRIVERS_EC_H
#define UAPI_HMKERNEL_DRIVERS_EC_H

#include <hmasm/types.h>
#include <hmkernel/capability.h>

/*
 * event channel endpoint's flags is defined as follow:
 * bit  0     : has communication source or not
 * bit  1     : has communication destination or not
 * bit  2     : created by guest or host
 * bit  3 - 15: reserved
 * bit 16 -   : communication type
 */

/* communication type */
#define __EC_EP_TYPE_SHMEC	(0)

#define __EC_EP_SRC		(0x1ULL)
#define __EC_EP_DST		(0x2ULL)
#define __EC_EP_GUEST		(0x4ULL)

#define __ec_ep_is_src(flags)			(((flags) & __EC_EP_SRC) != 0U)
#define __ec_ep_is_dst(flags)			(((flags) & __EC_EP_DST) != 0U)
#define __ec_ep_is_created_by_guest(flags)	(((flags) & __EC_EP_GUEST) != 0U)

#define __ec_ep_flags_to_type(flags)		(((flags) >> 16U) & ((1U << 16U) - 1U))
#define __ec_ep_type_to_flags(type)		(((type) & ((1U << 16U) - 1U)) << 16U)

struct __shmec_args {
	__uptr_t buf;
	__u32 len;
	union {
		int *futex;
		struct {
			cref_t vm_cref;
			int idx; /* identity an ep related to the owner vm */
			unsigned int irq;
		};
	};
};

struct __ec_ep_args {
	unsigned int flags;
	union {
		struct __shmec_args shmec_args;
	};
};

#endif /* UAPI_HMKERNEL_DRIVERS_EC_H */
