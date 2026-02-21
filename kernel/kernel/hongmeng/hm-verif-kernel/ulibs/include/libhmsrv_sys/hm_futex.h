/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of sysif server futex
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 21 16:32:46 2018
 */
#ifndef _LIBVSYSCALL_HM_FUTEX_H
#define _LIBVSYSCALL_HM_FUTEX_H

#include <hmasm/types.h>
#include <hmkernel/futex.h>
#include <hmkernel/activation/local_area.h>
#include <stdbool.h>

#if defined(CONFIG_RT_FUTEX) || defined(CONFIG_USER_FUTEX)
#include <hmkernel/capability.h>
#include <hongmeng/ext_fast_syscall.h>
#endif

#ifdef CONFIG_RT_FUTEX
#include <hmkernel/rt_futex.h>
#endif

#define FUTEX_CLOCK_REALTIME 256

#define FUTEX_BLOCKED_DETECT_FLAG	0x1000

#define FUTEX_BITSET_MATCH_ANY	0xffffffffUL

#define FUTEX_PRIVATE_FLAG __FUTEX_PRIVATE_FLAG

#define FUTEX_LOCK_PI_PRIVATE	(__FUTEX_OP_LOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_UNLOCK_PI_PRIVATE	(__FUTEX_OP_UNLOCK_PI | FUTEX_PRIVATE_FLAG)

struct thread_futex_wait_args {
	__u64 uaddr;
	__s32 val;
	union {
		struct {
			__u32 is_rtfutex:1;
		} flags;
		__u32 padding;
	};
	__u64 timeout;
	__u64 sigmask;
	__u64 bitset;
};

struct thread_futex_wake_args {
	__u64 uaddr;
	__u32 num;
	union {
		struct {
			__u32 is_rtfutex:1;
		} flags;
		__u32 padding;
	};
	__u64 bitset;
	__u64 bitset2; /* bitset2 for shared rwlock */
};

struct thread_futex_requeue_args {
	__u64 src_uaddr;
	__u64 dst_uaddr;
	__u32 deq_num;
	__u32 req_num;
};

struct thread_futex_args {
	int val1;
	int val3;
	int *uaddr1;
	int *uaddr2;
	unsigned long ts_or_val2; /* depends on futex_op */
	int futex_op;
};

#ifdef CONFIG_RT_FUTEX
#define EXT_FASTCALL_GLOBAL_RT_FUTEX_WAIT_IDX 0
#define EXT_FASTCALL_GLOBAL_RT_FUTEX_WAKE_IDX 1

__EXT_FASTCALL_GLOBAL(6, rt_futex_wait, EXT_FASTCALL_GLOBAL_RT_FUTEX_WAIT_IDX,
		      volatile const int *, uaddr, unsigned long, flags, __u64 *,
		      utimeout, const cref_t *, owner_cref, unsigned long, owner_id,
		      unsigned long, caller_id)
__EXT_FASTCALL_GLOBAL(2, rt_futex_wake, EXT_FASTCALL_GLOBAL_RT_FUTEX_WAKE_IDX,
		      volatile const int *, uaddr, unsigned long, flags)
#endif

struct timespec;
struct __hmsigmask;
int hm_futex_wait(volatile int *uaddr, int val,
		  const struct timespec *ts, unsigned int flags);
int hm_futex_timedwait(volatile int *uaddr, int val, unsigned long long *timeout,
		       struct __hmsigmask *mask, unsigned int flags);
int hm_futex_timedwait_ex(volatile int *uaddr, int val, unsigned long long *timeout,
			  struct __hmsigmask *mask, unsigned int flags,
			  unsigned long bitset, unsigned long bitset2, unsigned long data,
			  struct __block_param_s *block_param);
int hm_futex_wait_bitset(volatile int *uaddr, int val,
			 const struct timespec *ts, unsigned int flags, unsigned long bitset);
int hm_futex_wait_interruptible(volatile int *uaddr, int val,
				const struct timespec *ts, unsigned int flags);
int hm_futex_wake(volatile int *uaddr, unsigned int num, unsigned int flags);
int hm_futex_wake_ex(volatile int *uaddr, unsigned int num, unsigned int flags,
		     unsigned long bitset, unsigned long bitset2);
int hm_futex_wake_bitset(volatile int *uaddr, unsigned int num, unsigned int flags, unsigned long bitset);
int hm_futex_wake_bitset_ex(volatile int *uaddr, unsigned int num, unsigned int flags,
			    unsigned long bitset, unsigned long bitset2);
int hm_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
		     unsigned int deq_num, unsigned int req_num, unsigned int flags);
int hm_set_robust_list(const void *list_head, unsigned long size);

#ifdef CONFIG_RT_FUTEX
int hm_futex_wait_pi(volatile int *uaddr, unsigned int flags,
		     const struct timespec *ts);
int hm_futex_wake_pi(volatile const int *uaddr, unsigned int flags);
#endif

int hm_futex_wake_op(volatile int *uaddr1, volatile int *uaddr2,
		     unsigned int nr_wake1, unsigned int nr_wake2,
		     int option, unsigned int flags);

/*
 * If replace_futex is true, all the original hm futexes in a given process are replaced by
 * by user futex in hm_futex.c.
 */
void hm_user_futex_init(bool replace_futex);

#ifdef CONFIG_USER_FUTEX
__EXT_FASTCALL_GLOBAL(7, block_thread, __EXT_FASTCALL_GLOBAL_UFUTEX_BLOCK_THREAD,
				const int *, uaddr, struct __hmsigmask *, sigmask,
				unsigned int, flag, __u64 *, utimeout,
				const unsigned int *, gencnt_addr, unsigned int, gencnt_expected,
				const struct __block_param_s *, block_param_uaddr)

__EXT_FASTCALL_GLOBAL(4, unblock_threads, __EXT_FASTCALL_GLOBAL_UFUTEX_UNBLOCK_THREADS,
				const struct __ufutex_unblock_target_s *, target_list, unsigned long, list_size,
				const int *, uaddr, unsigned long, flags)

int hm_thread_block(volatile int *uaddr, int val, unsigned long long *ptimeout, struct __hmsigmask *mask,
		    unsigned int flags, const struct __block_param_s *block_param);
int hm_thread_unblock(unsigned long long tcb_cref, volatile int *uaddr, unsigned int flags);

int hm_user_futex_wait(volatile int *uaddr, int val,
						unsigned long bitset, unsigned int flags,
						struct __hmsigmask *mask, unsigned long long *ptimeout);

int hm_user_futex_wake_ex(volatile int *uaddr, unsigned int num,
							unsigned long bitset, unsigned long bitset2, unsigned int flags);

int hm_user_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
							unsigned int deq_num, unsigned int req_num);

bool hm_user_futex_allowed(unsigned int flags);
#endif /* CONFIG_USER_FUTEX */

void hm_set_block_param(struct __block_param_s *block_param, unsigned long pc);
void hm_set_block_info(struct __block_info_s *block_info, unsigned long pc);
#endif
