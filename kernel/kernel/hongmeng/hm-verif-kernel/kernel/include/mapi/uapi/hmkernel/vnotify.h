/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: vnotify uapi for dfx header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 19 16:11:24 2021
 */
#ifndef HMAPI_HMKERNEL_VNOTIFY_H
#define HMAPI_HMKERNEL_VNOTIFY_H

#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif
#include <hmkernel/vnotify_common.h>

#define __NR_EXT_FASTCALL_VNOTIFY				12
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_CHANNEL		5
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_CTRL_CHANNEL	6
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_READ_CHN_MSG	7
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_UPDATE_FUTEX	8
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_READ_STATUS	9
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_INJECT_EVENT	10
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_NOTIFY_CLEAR_VSPACE	11
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_QUERY_CAPACITY		14
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_ATTACH_FD			15
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_BIND_EPOLL		16
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_SELECT			17
#define __EXT_FASTCALL_GLOBAL_VNOTIFY_PPOLL			18

struct __chn_s {
	vatomic32_t flags;
	vatomic32_t status;
};

struct vnotify_mem_s {
	struct __chn_s *chns;
	struct __vnotify_chn_msg_s *host_msg;
	struct __vnotify_chn_msg_s *guest_msg;
};

#endif
