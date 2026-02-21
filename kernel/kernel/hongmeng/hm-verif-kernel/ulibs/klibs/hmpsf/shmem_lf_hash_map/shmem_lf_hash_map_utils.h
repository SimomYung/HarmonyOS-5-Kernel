/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: headers for implementaion of shared-memory lock-free hash maps
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 14 21:08:25 2023
 */

#ifndef ULIBS_KLIBS_HMPSF_XVM_SHMEM_LF_HASH_MAP_H
#define ULIBS_KLIBS_HMPSF_XVM_SHMEM_LF_HASH_MAP_H

#include <hongmeng/compiler.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libhwsecurec/securec.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <lib/errno.h>
#include <lib/utils.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <vsync/atomic.h>

#define hmpsf_debug		hm_debug
#define hmpsf_info		hm_info
#define hmpsf_warn		hm_warn
#define hmpsf_error		hm_error

static inline void hmpsf_lock_init(void *lock)
{
	raw_mutex_init((struct raw_mutex *)lock);
}

static inline void hmpsf_lock_acquire(void *lock)
{
	raw_mutex_lock((struct raw_mutex *)lock);
}

static inline void hmpsf_lock_release(void *lock)
{
	raw_mutex_unlock((struct raw_mutex *)lock);
}

static inline unsigned int hmpsf_lock_size(void)
{
	return sizeof(struct raw_mutex);
}

unsigned int hmpsf_test_rand(void);

#endif
