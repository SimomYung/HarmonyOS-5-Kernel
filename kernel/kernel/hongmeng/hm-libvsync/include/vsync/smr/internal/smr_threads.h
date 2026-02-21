/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_SMR_THREADS
#define VSYNC_SMR_THREADS

#include <vsync/vtypes.h>
#include <vsync/common/assert.h>
#include <vsync/common/assert.h>

#ifdef VSYNC_SMR_USE_MYTHREADLIB
#ifdef VSYNC_SMR_TID_TYPE
typedef VSYNC_SMR_TID_TYPE smr_tid_t;
#else
#error "when VSYNC_SMR_USE_MYTHREADLIB is defined, also define VSYNC_SMR_TID_TYPE. E.g. VSYNC_SMR_TID_TYPE=int"
#endif
#else
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#ifdef VSYNC_VERIFICATION
#include <vsync/atomic.h>
typedef vuint32_t smr_tid_t;
static vatomic32_t g_tid = VATOMIC_INIT(1);
#define SMR_TID_FORMAT "%u"
static __thread smr_tid_t __thread_id = 0;
#else
typedef pthread_t smr_tid_t;
#define SMR_TID_FORMAT "%lu"
#endif
#endif

/**
 * The following functions
 *
 */
typedef void *(*smr_thread_fun)(void *);
typedef vbool_t (*smr_thread_eq_fun)(smr_tid_t t1, smr_tid_t t2);
typedef smr_tid_t (*smr_thread_self_fun)(void);
typedef void (*smr_thread_create_fun)(smr_tid_t *tid, smr_thread_fun smr_fun,
				      void *smr_fun_args);
typedef void (*smr_thread_join_fun)(smr_tid_t tid);
typedef void (*smr_thread_yield_fun)(void);
typedef int (*smr_thread_usleep_fun)(unsigned int);

typedef struct smr_thread_lib_s {
	smr_thread_eq_fun thread_eq;
	smr_thread_self_fun thread_self;
	smr_thread_create_fun thread_create;
	smr_thread_join_fun thread_join;
	smr_thread_yield_fun thread_yield;
	smr_thread_usleep_fun thread_usleep;
} smr_thread_lib_t;

static inline void smr_thread_lib_not_null(smr_thread_lib_t *tlib)
{
	ASSERT(tlib);
	ASSERT(tlib->thread_eq);
	ASSERT(tlib->thread_self);
	ASSERT(tlib->thread_create);
	ASSERT(tlib->thread_join);
	ASSERT(tlib->thread_yield);
	ASSERT(tlib->thread_usleep);
	V_UNUSED(tlib);
}

#if !defined(VSYNC_SMR_USE_MYTHREADLIB) || \
	defined(VSYNC_SMR_USE_MYTHREADLIBTEST)

static inline vbool_t smr_thread_eq(smr_tid_t t1, smr_tid_t t2)
{
#if defined(VSYNC_VERIFICATION)
	return t1 == t2;
#else
	return pthread_equal(t1, t2) != 0;
#endif
}

static inline smr_tid_t smr_thread_self(void)
{
#if defined(VSYNC_VERIFICATION)
	if (__thread_id == 0) {
		__thread_id = vatomic32_get_inc(&g_tid);
	}
	return __thread_id;
#else
	return pthread_self();
#endif
}

static inline void smr_thread_create(smr_tid_t *tid, smr_thread_fun smr_fun,
				     void *smr_fun_args)
{
	pthread_create(tid, NULL, smr_fun, smr_fun_args);
}

static inline void smr_thread_join(smr_tid_t tid)
{
	pthread_join(tid, NULL);
}

static inline void smr_thread_yield(void)
{
	sched_yield();
}

static inline int smr_thread_usleep(unsigned int microseconds)
{
#if defined(VSYNC_VERIFICATION)
	(void)microseconds;
	return 0;
#else
	return usleep(microseconds);
#endif
}

/* default functions to use */
#define DEFAULT_SMR_THREAD_LIB()                                            \
	(smr_thread_lib_t)                                                  \
	{                                                                   \
		.thread_eq = smr_thread_eq, .thread_self = smr_thread_self, \
		.thread_create = smr_thread_create,                         \
		.thread_join = smr_thread_join,                             \
		.thread_yield = smr_thread_yield,                           \
		.thread_usleep = smr_thread_usleep                          \
	}

#endif

#endif
