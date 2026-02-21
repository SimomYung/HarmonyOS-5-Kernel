/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: SMT cookie mechanism for security and energy-efficiency.
 */
#ifndef UAPI_HMKERNEL_SCHED_SMT_COOKIE_H
#define UAPI_HMKERNEL_SCHED_SMT_COOKIE_H

#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif

#include <hmkernel/tcb.h>
#include <hmkernel/cpuset.h>

typedef struct smt_refcnt {
	vatomic32_t cnt;
} smt_refcnt_t;

typedef struct smt_secure_cookie {
	smt_refcnt_t refcnt;
	bool is_freeable;
} smt_secure_cookie_t;

typedef struct smt_prefer_cookie {
	smt_refcnt_t refcnt;
	__cpuset_t active_mask;
	vatomic32_t nr_active[CONFIG_NR_CPUS];
	vatomic32_t total_active;
} smt_prefer_cookie_t;

static inline void smt_refcnt_init(smt_refcnt_t *r, int val)
{
	vatomic32_init(&r->cnt, val);
}

static inline void smt_refcnt_inc(smt_refcnt_t *r)
{
	vatomic32_inc(&r->cnt);
}

static inline bool smt_refcnt_dec(smt_refcnt_t *r)
{
	if (vatomic32_dec_get(&r->cnt) == 0) {
		return true;
	}
	return false;
}

#endif
