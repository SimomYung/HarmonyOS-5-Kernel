/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for random operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 14:58:12 2024
 */
#include <liblinux/pal.h>

#include <hongmeng/panic.h>
#include <devhost/log.h>
#include <devhost/crypto.h>

#define MAX_RETRY_TIMES 10

int liblinux_pal_get_random_u64(uint64_t *value)
{
	int ret = E_HM_FAULT;
	int retry = 0;

	while (retry < MAX_RETRY_TIMES && ret != E_HM_OK) {
		retry++;
		ret = devhost_get_random_u64(value);
	}

	return -hmerrno2posix(ret);
}
