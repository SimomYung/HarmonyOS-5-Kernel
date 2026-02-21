/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: vnotify uapi header file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 08 15:59:15 2021
 */

#ifndef HMAPI_HMKERNEL_VNOTIFY_COMMON_H
#define HMAPI_HMKERNEL_VNOTIFY_COMMON_H

#include <hmkernel/types.h>

#define __VNOTIFY_CHN_NUM_MAX 4096

/* intr bit: interrupt and enqueue bit */
#define __VNOTIFY_INTR 0x1U

/* notifying bit: vnotify has events to be processed */
#define __VNOTIFY_NOTIFYING   0x2U

/* closed bit: vnotify closed bit */
#define __VNOTIFY_CLOSED 0x4U

/* epollout bit: vnotify can send revent(EPOLLOUT) */
#define __VNOTIFY_EPOLLOUT 0x8U

/* epollin bit: vnotify can send revent(EPOLLIN) */
#define __VNOTIFY_EPOLLIN 0x10U

/* sysfast_read_status masks and bits */
#define __VNOTIFY_EPOLL_MASK (__VNOTIFY_EPOLLIN | __VNOTIFY_EPOLLOUT)

#define __VNOTIFY_CHN_MSG_SIZE 1024
#define __VNOTIFY_CHN_MSG_PAYLOAD 1016

#define VNOTIFY_HOST_CTRL_CHANNEL 0
#define VNOTIFY_GUEST_CTRL_CHANNEL 1
#define VNOTIFY_RESERVED_CHANNEL 2

#define VNOTIFY_CONFIG_BIND_CHANNEL 	0U
#define VNOTIFY_CONFIG_UNBIND_CHANNEL 	1U
#define VNOTIFY_CONFIG_HOST_CLOSED	2U
#define VNOTIFY_CONFIG_GUEST_CLOSED	4U

#define VNOTIFY_INJECT_EVENT_NOTIFY	0U
#define VNOTIFY_INJECT_EVENT_INTER	1U
#define VNOTIFY_INJECT_EVENT_EPOLLOUT	2U
#define VNOTIFY_INJECT_EVENT_EPOLLIN	3U

/*----------- CHANNEL MASKS -----------------------*/
/*
 * Only host will set closed bit.
 * When host close this channel actively, tx chn
 * will be set this bit, otherwise, rx will be set.
 */
#define CHN_MASK_CLOSED		0x1U
#define CHN_MASK_OCCUPIED	0x2U

struct __vnotify_info_s {
	__u32 irq;
	__u64 addr;
	__u64 size;
};

struct __vnotify_args_s {
	__u32 *__futex;
	__u32  __id;
};

struct __vnotify_config_bind_s {
	__u32 __id;
	__u32 __binder_idx;
};

struct __vnotify_config_unbind_s {
	__u32 __id;
};

struct __vnotify_chn_msg_s {
	__u8 __modid; /* module ID */
	__u8 __type; /* msg type */
	__u16 __len;  /* msg len */
	__u32 __rsvd;  /* rsvd bits */
	__u8  __msg[__VNOTIFY_CHN_MSG_PAYLOAD];
};

#endif
