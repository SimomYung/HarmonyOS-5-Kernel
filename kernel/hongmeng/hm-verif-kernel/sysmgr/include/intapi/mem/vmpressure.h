/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file for internal api of vmpressure
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 06 15:11:08 2023
 */

#ifndef INTAPI_MEM_VMPRESSURE_H
#define INTAPI_MEM_VMPRESSURE_H

#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/atomic.h>
#include <lib/dlist.h>
#include <stdint.h>

#define VMPRESSURE_MAX_SIZE 100

struct vmpressure_s {
	raw_atomic_long_t cost;
	raw_atomic_long_t reward;
	struct raw_mutex mutex;
	struct dlist_node events;
};

int vmpressure_level_show(char *data, size_t data_len);
int vmpressure_policy_show(char *data, size_t data_len);
int vmpressure_policy_write(char *src, size_t size);

#endif
