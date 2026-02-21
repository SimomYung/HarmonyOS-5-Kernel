/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: polllist metadata as kobject - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 14 11:12:02 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_POLLLIST_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_POLLLIST_H

#include <hmkernel/types.h>
#include <lib/dlist.h>

#define __CTRLMEM_KOBJ_POLLLIST_STATE_RELEASED	0xffffffff
#define __CTRLMEM_U32_MAX_VAL 0xFFFFFFFF

#define __POLLLIST_NOTIFY_CLEANUP  0x0U
#define __POLLLIST_NOTIFY_WAKEUP   0x1U
#define __POLLLIST_SPECIFY_EVENTS  0x0U
#define __POLLLIST_ENTIRE_EVENTS   0x1U
#define __POLLLIST_CLEANUP_REVENTS ((__POLLLIST_NOTIFY_CLEANUP << 4U) | __POLLLIST_SPECIFY_EVENTS) // 0x00
#define __POLLLIST_WAKEUP_REVENTS  ((__POLLLIST_NOTIFY_WAKEUP << 4U) | __POLLLIST_SPECIFY_EVENTS)  // 0x10
#define __POLLLIST_UPDATE_REVENTS  ((__POLLLIST_NOTIFY_WAKEUP << 4U) | __POLLLIST_ENTIRE_EVENTS)  // 0x11

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_kobj_polllist_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobj_polllist_uatomic32_s __ctrlmem_kobj_polllist_uatomic32_t;
# define __uatomic32 __ctrlmem_kobj_polllist_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif
#endif

#define __POLLLIST_PADDING_SIZE 5
struct __ctrlmem_kobj_polllist_s {
	__uatomic32 listener_cnt;
	__uatomic32 revents;
	__u32 status;
	/* how many fds is referencing this ctrlmem */
	__uatomic32 refcnt;
	/* used by struct fd_s for pollable fds */
	union {
		void *file;
		void *socket;
		void *kernel_file;
		unsigned long value;
	} common;
	/* wait queue, and wait_queue lock, revent, user space don't care about them yet */
	__u64 padding[__POLLLIST_PADDING_SIZE];
};
#endif
