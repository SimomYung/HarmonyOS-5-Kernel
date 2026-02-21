/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Futex  and futex hash key
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 16 16:10:19 2023
 */

#ifndef MAPI_HMKERNEL_FUTEX_KEY_H
#define MAPI_HMKERNEL_FUTEX_KEY_H
#include <hmkernel/memory.h>
#include <lib/atomic.h>

struct capability_s;

struct futex_key_s {
	struct capability_s *vspace;
	vaddr_t vaddr;
} __packed;

struct futex_s {
	struct futex_key_s hash_key;
	unsigned long flags;
	u32 bitset;
	vatomic32_t on_queue;
	int val;
};
#endif
