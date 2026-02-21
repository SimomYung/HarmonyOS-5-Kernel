/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Basic implementation related with raw_thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#ifndef ULIBS_LIBHMSYNC_THREAD_RAW_THREAD_IMPL_H
#define ULIBS_LIBHMSYNC_THREAD_RAW_THREAD_IMPL_H

#include <time.h>
#include <unistd.h>
#include <hmasm/page.h>
#include <hongmeng/errno.h>
#include <libhmsync/thread.h>
#include <libalgo/rbtree.h>
#include <libmem/utils.h>
#include <libhmsrv_sys/hm_futex.h>
#include <hmkernel/ipc/hmsignal.h>
#include <vsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libstrict/strict_base.h>

#include "raw_tls.h"
#include "compatible_pthread.h"
#include "lockdep_impl.h"

/* max tsd key supported, follow musl's definition */
#define RAW_THREAD_KEYS_MAX 32

#define RAW_THREAD_STACK_ALIGN 16UL

struct __block_param_s;
/*
 * NOTE:
 *  To keep compatible with musl pthread, we insert a compatible pthread structure
 *  at the end of raw_thread.
 *  hmlibc has exported pthread_impl.h out to the include folder.
 *
 *  Check musl/src/internal/pthread_impl.h
 *
 *  So the layout for struct raw_thread and struct pthread is as below in compatible mode:
 *
 *   'raw thread layout'                 'pthread layout'
 *     |---------| <- low addr
 *     |  stack  |
 *     |---------| <- raw_thread_self()
 *     |   raw   |
 *     | thread  |                         |---------| <- low addr
 *     |specific |    (compatible)         |  stack  |
 *     |---------| <- pthread_self()       |---------| <- pthread_self()
 *     | struct  |                         | struct  |
 *     | pthread |                         | pthread |
 *     |---------| <-  raw thread          |---------| <- pthread
 *                     TLS reg pointer                    TLS reg pointer
 *
 *  -- raw thread implement detailed attribute
 *  Compatible Rules:
 *  1. Keep a 'struct pthread' at the end of raw_thread, we can use pthread_self() in raw thread to get
 *     the compatible 'struct pthread'.
 *
 *     So if modify libc 'struct pthread', ulibs should be re-compiled.
 *
 *  2. Add padding bytes between 'pthread section' and raw thread implement in 'struct raw_thread'.
 *
 *     This is used to keep binary ABI compatible. For example, when we must modify libc pthread
 *     for downstream software, but we don't want to publish a new ulib for downstream. We can modify
 *     libc first(less than 64bytes), and later we could modify struct raw_thread in ulibs.
 *
 */
struct raw_thread {
	union {
		cref_t tcb_cref;
		cref_t actv_cref;
	} cref;

	/*
	 * Adaption with musl v1.1.22, because struct pthread in musl v1.1.22 does not contain
	 * 'start_arg' and 'start' anymore. raw_thread can not access these two members through
	 * 'compatible_pthread', so add them in raw_thread.
	 */
	void *start_arg;
	void *(*start)(void *);

	/* used by creator to store some specific data */
	void *private;

#ifdef CONFIG_LOCKDEP
	int lockdep_depth;
	struct held_lock held_locks[MAX_LOCK_DEPTH];
#endif

	/* for actv: link to actvpool rbtree */
	struct rbt_node node;

	/* Stack base used for harden usercopy check */
	unsigned long stack_base;
	unsigned long stack_size;

	/* ---->  can't put raw thread specific attribute below  <----*/
#if !defined __HOST_LLT__ && defined CONFIG_RAW_THREAD_COMPATIBLE_WITH_PTHREAD
	/* padding for raw thread and pthread binary compatible */
	char padding[RAW_THREAD_PADDING];
#endif
	/*
	 * Add a aligned long here, struct pthread will be aligned in libhmc.
	 * If we not aligned here, the pthread address maybe not correct.
	 *
	 * 8 bytes align is valid for both 32bits and 64bits.
	 */
	union {
		long align;
	} __aligned(8);

	/* compatible pthread, __aligned(1) keep the size equals to libc struct pthread. */
	struct pthread compatible_pthread __aligned(1);
};

#define CMPT_THREAD_REG(self, seg) (self)->compatible_pthread.seg
#define CMPT_THREAD_REF(self, seg) (&(self)->compatible_pthread.seg)
#define RAW_THREAD_TO_PTHREAD(th) (&(th)->compatible_pthread)

#define TLS_OFFSET_TSD \
	((sizeof(struct raw_thread)	\
	- offsetof(struct raw_thread, compatible_pthread)) \
	- offsetof(struct pthread, THREAD_TSD))

#ifdef CONFIG_LOCKDEP
static inline void raw_thread_lockdep_depth_init(struct raw_thread *thread)
{
	thread->lockdep_depth = 0;
	mem_zero_a(thread->held_locks);
}
#else
static inline void raw_thread_lockdep_depth_init(struct raw_thread *thread)
{
	UNUSED(thread);
}
#endif

static inline struct raw_thread *__raw_thread_self(void)
{
	return TLS_TO_THREAD(raw_get_tls());
}

static inline void *__raw_thread_private_of(struct raw_thread *thread)
{
	return thread->private;
}

static inline void __raw_thread_set_private(struct raw_thread *thread, void *priv)
{
	thread->private = priv;
}

struct raw_thread *raw_tls_to_thread(unsigned char *_mem);
void *raw_tls_copy(unsigned char *mem);

/* tsd destructors, must be called at thread exit */
void __raw_thread_tsd_run_dtors(struct raw_thread *_thread);

#define RAW_THREAD_STACK_MIN PAGE_SIZE
#define DEFAULT_RAW_THREAD_STACKSIZE (4 * PAGE_SIZE)
#define DEFAULT_RAW_THREAD_GUARDSIZE PAGE_SIZE
extern struct __libc *plibc;
extern void *__raw_tsd_main[RAW_THREAD_KEYS_MAX];

static inline size_t __raw_thread_tls_size(void)
{
	size_t size = plibc->tls_size + sizeof(struct raw_thread) - sizeof(struct pthread);
	return ALIGN_UP(size, RAW_THREAD_STACK_ALIGN);
}

static inline void raw_spin(void)
{
	vatomic32_t tmp = VATOMIC_INIT(0);
	(void)vatomic32_cmpxchg_rlx(&tmp, 0, 0);
}

static inline int raw_timedwait_ex(volatile int *futex, int val,
				__u64 *timeout, struct __hmsigmask *sigmask,
				unsigned int flags,
				unsigned long bitset, unsigned long bitset2, unsigned long data,
				struct __block_param_s *block_param)
{
	int err;

	err = hm_futex_timedwait_ex(futex, val, timeout, sigmask, flags, bitset, bitset2, data, block_param);

	err = (err == E_HM_AGAIN) ? 0 : err;
	err = (err == E_HM_HMSIGINTR) ? E_HM_INTR : err;

	return err;
}

static inline int raw_futex_wait_ex(volatile int *futex, int val, unsigned int flags,
				    unsigned long bitset, unsigned long bitset2, unsigned long data,
				    struct __block_param_s *block_param)
{
	int err;
	struct __hmsigmask hmsig_mask = {(__u64)-1};

	do {
		err = raw_timedwait_ex(futex, val, NULL, &hmsig_mask, flags, bitset, bitset2, data, block_param);
	} while (err == E_HM_INTR);

	return err;
}

static inline int raw_futex_wake_ex(volatile int *futex, int cnt, int priv,
				    unsigned long bitset, unsigned long bitset2)
{
	unsigned int futex_type;

	futex_type = (priv != 0) ? FUTEX_TYPE_PRIVATE : FUTEX_TYPE_SHARED;
	return hm_futex_wake_ex(futex, (unsigned int)cnt, futex_type, bitset, bitset2);
}

static inline int raw_timedwait(volatile int *futex, int val,
				__u64 *timeout, struct __hmsigmask *sigmask,
				unsigned int flags)
{
	return raw_timedwait_ex(futex, val, timeout, sigmask, flags, FUTEX_BITSET_MATCH_ANY, 0ULL, 0ULL, NULL);
}

static inline int raw_futex_wait(volatile int *futex, int val, unsigned int flags)
{
	return raw_futex_wait_ex(futex, val, flags, FUTEX_BITSET_MATCH_ANY, 0ULL, 0ULL, NULL);
}

static inline int raw_futex_wake(volatile int *futex, int cnt, int priv)
{
	return raw_futex_wake_ex(futex, cnt, priv, FUTEX_BITSET_MATCH_ANY, 0ULL);
}

static inline int raw_futex_requeue(volatile int *futex, unsigned int val,
				    unsigned int req, volatile int *r)
{
	int err;
	do {
		err = hm_futex_requeue(futex, r, val, req, 0);
	} while (err == E_HM_INTR);
	return err;
}

int raw_futex_timedwait(volatile int *futex, int val, clockid_t clk,
			const struct timespec *to, struct __hmsigmask *mask);
struct raw_mutex *raw_thread_lock(void);

void raw_thread_link_to_global(struct raw_thread *new, struct raw_thread *prev_thread);
void raw_thread_remove_from_global(struct raw_thread *thread);

#ifdef CONFIG_NEED_SHADOW_STACK
void *raw_mmap_shadow_stack(void);
#endif

#endif
