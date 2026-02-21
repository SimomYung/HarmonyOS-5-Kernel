/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ULWT kinfo UAPI
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 3 17:33:23 2024
 */
#ifndef UAPI_HMKERNEL_ULWT_KINFO_H
#define UAPI_HMKERNEL_ULWT_KINFO_H

#include <hmkernel/types.h>

enum __ulwt_kinfo_thread_state_s {
	__ULWT_KINFO_TCB_STATE_UNREGISTERED = 0,
	__ULWT_KINFO_TCB_STATE_RUNNING,
	__ULWT_KINFO_TCB_STATE_SLEEPING,
	__ULWT_KINFO_TCB_STATE_BLOCKED,
	__ULWT_KINFO_NR_TCB_STATES,
};

struct __ulwt_kinfo_dlist_node_padding {
	unsigned long padding0;
	unsigned long padding1;
};

struct __ulwt_kinfo_thread_data_s {
	__u32 padding0;
	enum __ulwt_kinfo_thread_state_s padding1;
	struct __ulwt_kinfo_dlist_node_padding padding2;
};

#define __ULWT_BLOCKAWARE_HM_PRCTL_OPS			0x534b4241
#define __ULWT_BLOCKAWARE_SUBOPS_NONE			0x00
#define __ULWT_BLOCKAWARE_SUBOPS_INIT			0x01
#define __ULWT_BLOCKAWARE_SUBOPS_REG			0x02
#define __ULWT_BLOCKAWARE_SUBOPS_UNREG			0x03
#define __ULWT_BLOCKAWARE_SUBOPS_WAIT			0x04
#define __ULWT_BLOCKAWARE_SUBOPS_WAKE			0x05
#define __ULWT_BLOCKAWARE_SUBOPS_MONITORFD		0x06

#endif
