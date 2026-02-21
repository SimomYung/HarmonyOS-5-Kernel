/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Futex interfaces exported for module.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 04 11:54:50 2020
 */
#ifndef MAPI_HMKERNEL_FUTEX_H
#define MAPI_HMKERNEL_FUTEX_H

#include <lib/dlist.h>
#include <lib/atomic.h>
#include <hmkernel/cpu.h>
#include <hmkernel/memory.h>
#include <hmkernel/compiler.h>
#include <hmkernel/timer/timer.h>
#include <hmkernel/ipc/hmsignal.h>
#include <uapi/hmkernel/futex.h>
#include <hmkernel/futex_key.h>
#include <uapi/hmkernel/activation/local_area.h>

struct tcb_s;
struct __hmsigmask;
struct capability_s;
struct rec_spinlock_s;

struct futex_block_at_s {
	/* record the cpu id tcb is blocked on */
	unsigned int block_cpu;
#ifdef CONFIG_USER_FUTEX
	union {
		u64 gencnt_addr;
		struct dlist_node list;
	};
#else
	struct dlist_node list;
#endif
	struct futex_s futex;
#if defined(CONFIG_TRACE_BLOCK_INFO) && !defined(CONFIG_TRACE_BLOCK_INFO_ON_KOBJ)
	struct __block_info_s block_info;
#endif
};

struct futex_timer_s {
	struct timer_s timer;
	u64 __user *rmtime; /* Return remaining time to user space */
};

#define FUTEX_HASH_TABLE_SIZE	CONFIG_FUTEX_HASH_TABLE_SIZE

#define __FUTEX_BITSET_MATCH_ANY 0xffffffffU

extern bool
is_futex_on_queue(struct futex_s *futex);
extern void
set_futex_on_queue(struct futex_s *futex);
extern void
unset_futex_on_queue(struct futex_s *futex);

extern bool
is_key_equal(const struct futex_key_s *hash_key1,
	     const struct futex_key_s *hash_key2);

static inline bool
is_bitset_matched_with_mask(unsigned long bitset1,
			    unsigned long bitset2,
			    unsigned long bitset2_mask)
{
	/* 1.bitset2 of sys_futex_wake_ex may have multiple meanings.
	 * For example, the rwlock only uses __FUTEX_BITSET2_WR and
	 * __FUTEX_BITSET2_RD bit on bitset2 to identify that it is
	 * a reader lock or a writer lock.
	 * If we want to determine if there are any usage scenarios
	 * for rwlock, we need to & __FUTEX_BITSET2_RWLOCK_RW_MASK
	 * to distinguish it.
	 *
	 * 2. In userspace, we make rd_bitset & wr_bitset == 0U,
	 * we can use this condition to determine whether
	 * it is a read lock or a write lock. As well as, we can
	 * use this condition to judge whether bitset and bitset2 is valid.
	 *
	 */
	return (bitset1 & bitset2 & bitset2_mask);
}

void futex_wake_sync_mark(unsigned long flags, struct tcb_s **sync_tcb, struct tcb_s *wakee);
unsigned int
futex_get_wake_bitset(const struct futex_block_at_s *block_at,
		      const struct futex_s *futex, unsigned int bitset);
extern unsigned long
futex_hash(const struct futex_key_s *hash_key);
extern bool
futex_intr(struct tcb_s *tcb, int *retval);
extern void
futex_reset_block_at(struct futex_block_at_s *block_at);
void double_lock(struct rec_spinlock_s *l1, struct rec_spinlock_s *l2);
void double_unlock(struct rec_spinlock_s *l1, struct rec_spinlock_s *l2);
#ifdef CONFIG_MODULE_SCHED_CLASS
extern void
module_futex_hash_table_init_hook(void);
extern void
futex_dequeue_hook(struct futex_block_at_s *block_at);
extern void
futex_append_hook(struct futex_block_at_s *block_at);
extern struct rec_spinlock_s *
futex_bucket_lock_hook(const struct futex_s *futex);
extern struct rec_spinlock_s *
futex_bucket_lock_hook_acq(const struct futex_s *futex);
extern void do_futex_requeue_hook(const struct futex_s *src_futex, const struct futex_s *dst_futex,
			   unsigned int req_num);
extern unsigned int futex_wake_hook(const struct futex_s *futex, unsigned int num,
		  struct dlist_node *wake_list, unsigned int bitset1, int *has_waiter);
extern void futex_switch_priority(struct futex_block_at_s *block_at,
				  unsigned int old_prio,
				  unsigned int new_prio);
extern int futex_btimer_try_remove(struct tcb_s *tcb);

#else
#define module_futex_hash_table_init_hook(...)
#define do_futex_requeue_hook(...)
#define futex_dequeue_hook(...)
#define futex_append_hook(...)
static inline unsigned int futex_wake_hook(const struct futex_s *futex, unsigned int num,
		  struct dlist_node *wake_list, unsigned long bitset1, int *has_waiter)
{
	UNUSED(futex, num, bitset1);
	return 0;
}

static inline struct rec_spinlock_s *
futex_bucket_lock_hook_acq(const struct futex_s *futex)
{
	UNUSED(futex);
	return NULL;
}

static inline struct rec_spinlock_s *
futex_bucket_lock_hook(const struct futex_s *futex)
{
	UNUSED(futex);
	return NULL;
}
#endif

extern void futex_inc_iowaits(struct actv_s *actv);
extern void futex_dec_iowaits(struct tcb_s *tcb);

extern int set_futex_block_info(struct tcb_s *tcb,
				const struct __block_param_s __user *block_param_uaddr);
extern struct __block_info_s *futex_block_info_of(struct tcb_s *tcb);

void futex_timer_setup(struct tcb_s *tcb, unsigned long flags,
			u64 timeout,
			struct timer_ret_s (*callback)(struct timer_s *timer, u64 curr_time),
			enum timer_type type);

extern int
futex_convert_usermask(struct tcb_s *caller,
				struct __hmsigmask *knewmask, unsigned long mask_type,
				const struct __hmsigmask __user *newmask);

#ifdef CONFIG_USER_FUTEX
extern bool blockat_addr_intr(struct tcb_s *tcb);
void blockat_addr_priority_update(struct tcb_s *tcb);
#endif

#endif
