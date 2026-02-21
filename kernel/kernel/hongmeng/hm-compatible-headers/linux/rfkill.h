/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/rfkill.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:09:15 2021
 */

#ifndef LINUX_RFKILL_H
#define LINUX_RFKILL_H

#include <linux/types.h>

enum rfkill_type {
	RFKILL_TYPE_ALL = 0,
	RFKILL_TYPE_WLAN,
	RFKILL_TYPE_BLUETOOTH,
	RFKILL_TYPE_UWB,
	RFKILL_TYPE_WIMAX,
	RFKILL_TYPE_WWAN,
	RFKILL_TYPE_GPS,
	RFKILL_TYPE_FM,
	RFKILL_TYPE_NFC,
	NUM_RFKILL_TYPES,
};

enum rfkill_operation {
	RFKILL_OP_ADD = 0,
	RFKILL_OP_DEL,
	RFKILL_OP_CHANGE,
	RFKILL_OP_CHANGE_ALL,
};

struct rfkill_event {
	__u32 idx;
	__u8  type;
	__u8  op;
	__u8  soft, hard;
} __attribute__((packed));

#define RFKILL_EVENT_SIZE_V1	8

#endif
