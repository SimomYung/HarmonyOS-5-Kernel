/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Vnotify common header defination
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 12 16:00:03 2021
 */

#ifndef ULIBS_INCLUDE_LIBVPIPE_VNOTIFY_COMMON_H
#define ULIBS_INCLUDE_LIBVPIPE_VNOTIFY_COMMON_H

/*
 * NOTE: Now the <libvpipe/vnotify_common.h> might be included when
 * compiling a liblinux module. To support this, cannot include any
 * unsupported-for-liblinux-module header files here.
 */

/* block mode */
#define VNOTIFY_NONBLOCK  0x1U
/* interruptible mode */
#define VNOTIFY_INTR  0x2U

/* vnotify channel create message */
#define VNOTIFY_CREATE_CHANNEL 0x1U
#define VNOTIFY_CREATE_CHANNEL_RESP 0x2U
#define VNOTIFY_DESTROY_CHANNEL 0x3U
#define VNOTIFY_DESTROY_CHANNEL_RESP 0x4U

/* flag of vnotify attach */
#define VNOTIFY_ATTACH_CREATE 0x1U
#define VNOTIFY_ATTACH_SHARED 0x2U
#define VNOTIFY_ATTACH_PERSISTENT 0x4U


/* Add dac check for shared chn
 * There are some rules to use perm flags:
 * 1. Dac check only for host attaching shared chn, not guest attaching chn or private chn.
 *
 * 2. The dac attribute only takes effect the first time the host attaches shared chn,
 *    and doest not become invalid until chn dies(for shared and persistent chn).
 *
 * 3. When the host attaches shared chn first time, the following perm flags must be carried.
 *    If the host attached shared chn does not with any perm flags for the first time, this shared
 *    chn would be limited, only its creator can attach this shared chn.
 *
 * 4. VNOTIFY_CAP_USER: dac check for euid, if process A attaches shared chn N first time
 *    with perm flags VNOTIFY_CAP_USER, it means any processes which's euid is same with 
 *    process A can attach shared chn N.
 */

 /* perm flags */
#define VNOTIFY_CAP_OTHER    0x8U
#define VNOTIFY_CAP_GROUP    0x10U
#define VNOTIFY_CAP_USER     0x20U

#define VNOTIFY_FLAGS_SHARED 0x1U
#define VNOTIFY_FLAGS_ClOSED 0x2U

/* vnotify status */
#define VNOTIFY_HOST_ATTACHED  0x1U
#define VNOTIFY_GUEST_ATTACHED 0x2U
#define VNOTIFY_HOST_DETACHED  0x4U
#define VNOTIFY_GUEST_DETACHED 0x8U

struct timespec;

struct vnotify_s {
	unsigned int chn_id;
	unsigned int flags;
	int futex;
	int *faddr;
};

struct vnotify_msg_header_s {
	unsigned char modid; /* module ID */
	unsigned char type; /* msg type */
	unsigned short len; /* msg len */
	unsigned int rsvd; /* reserved bits */
};

#endif /* !ULIBS_INCLUDE_LIBVPIPE_VNOTIFY_COMMON_H */
