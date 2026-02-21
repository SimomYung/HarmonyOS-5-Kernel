/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Futex related definition
 * Author: Huawei OS Kernel Lab
 * Create: Sun Dec 13 17:36:19 2020
 */
#ifndef UAPI_HMKERNEL_FUTEX_H
#define UAPI_HMKERNEL_FUTEX_H

#include <hmkernel/const.h>

#define FUTEX_MASK_TYPE_USER		__UL(0)
#define FUTEX_MASK_TYPE_SIMPLE_NONE	__UL(1)
#define FUTEX_MASK_TYPE_SIMPLE_ALL	__UL(2)
#define FUTEX_MASK_TYPE_SIMPLE_EMRG	__UL(3)
#define FUTEX_MASK_TYPE_LSYSCALL	__UL(4)
#define FUTEX_MASK_TYPE_BITLOCK		__UL(5)

#define FUTEX_TYPE_PRIVATE		(0x0U)
#define FUTEX_TYPE_SHARED		(0x1U)
#define FUTEX_TYPE_MASK			(0xfU)

#define __FUTEX_BITSET2_WR			(0x0001UL)
#define __FUTEX_BITSET2_RD			(0x0002UL)
#define __FUTEX_BITSET2_XDEP_UNBOOST_ONLY	(0x0004UL)
#define __FUTEX_BITSET2_XDEP_RWLOCK_MULTI	(0x0008UL)
#define __FUTEX_BITSET2_XDEP_RWLOCK_SINGLE	(0x0010UL)
#define __FUTEX_BITSET2_XDEP_MUTEX		(0x0020UL)
#define __FUTEX_BITSET2_XDEP_SINGLE_MASK	(__FUTEX_BITSET2_XDEP_RWLOCK_SINGLE | __FUTEX_BITSET2_XDEP_MUTEX)
#define __FUTEX_BITSET2_XDEP_MULTI_MASK		(__FUTEX_BITSET2_XDEP_RWLOCK_MULTI)
#define __FUTEX_BITSET2_XDEP_MASK		(__FUTEX_BITSET2_XDEP_SINGLE_MASK | __FUTEX_BITSET2_XDEP_MULTI_MASK | \
						 __FUTEX_BITSET2_XDEP_UNBOOST_ONLY)
#define __FUTEX_BITSET2_RWLOCK_RW_MASK 		(__FUTEX_BITSET2_WR | __FUTEX_BITSET2_RD)

#define __FUTEX_XDEP_VAL_SHIFT_OWNER_ID		(3U)
#define __FUTEX_XDEP_MAX_BUCKET_HOLDERS		(16U)
#define __FUTEX_XDEP_MAX_KOBJ_HOLDERS		__FUTEX_XDEP_MAX_BUCKET_HOLDERS

/* Indicates that waker is about to be blocked */
#define FUTEX_FLAG_YIELD_SOON		(0x20000000UL)
/* Indicates that waker is about to synchronous wakeup and load aggregation. */
#define FUTEX_FLAG_SYNC_TRANS		(0x40000000UL)

#define FUTEX_FLAG_HAS_TIMER		(0x80000000UL)

#define FUTEX_NORMAL_BITSET				(0xffffffff)
/* used to distinguish between the top 5 and bottom 27 bits of a 32-bit num */
#define FUTEX_BITLOCK_MASK				(0x0000003f)
#define MAX_BITS_OF_INT				(32)
#define LEFT_OP_NUM					(16)

#ifdef CONFIG_MODULE_TIMER_RBTREE
#define FUTEX_REALTIME_CLOCK		__UL(0x80)
#define FUTEX_BOOTTIME_CLOCK		__UL(0x100)
#define FUTEX_ALARM_CLOCK		__UL(0x200)
#define FUTEX_CLOCK_MASK		(FUTEX_REALTIME_CLOCK | FUTEX_BOOTTIME_CLOCK | FUTEX_ALARM_CLOCK)
#endif

/*
 * If FUTEX_BLOCKED_HAS_LOCK_OWNER is set,
 * snapshot will print lock owner,
 * when the thread is blocked for lock
 */
#define FUTEX_BLOCKED_HAS_LOCK_OWNER	__UL(0x800)

/*
 * If FUTEX_BLOCKED_TIMEDOUT_DETECT is set, hguard will check if
 * the waiter is waiting for a long time.
 */
#define FUTEX_BLOCKED_TIMEDOUT_DETECT	__UL(0x1000)

#define __FUTEX_FLAG_USER_NOT		(0x2000U)
#define __FUTEX_FLAG_USER_MASK	(__FUTEX_FLAG_USER_NOT)

/*
 * __FUTEX_FLAG_BITLOCK modifies how the uaddr value check is implemented.
* Instead of (*uaddr == val) it's ((*uaddr & val) == val)
 */
#define __FUTEX_FLAG_BITLOCK		(0x4000U)
#define __IS_FUTEX_FLAG_BITLOCK(x)	(((x) & __FUTEX_FLAG_BITLOCK) != 0U)

/* Indicates the futex is lock_wait in ldk */
#define FUTEX_FLAG_LOCK_IN_LDK		(0x8000U)

#define __FUTEX_FLAG_IOWAIT		(0x10000U)
#define __IS_FUTEX_FLAG_IOWAIT(x)		(((x) & __FUTEX_FLAG_IOWAIT) != 0U)

#define __FUTEX_FLAG_PAGELOCK		(0x40000U)
#define __IS_FUTEX_FLAG_PAGELOCK(x)	(((x) & __FUTEX_FLAG_PAGELOCK) != 0U)

/*
 * @option in futex_wake_op is encoded as:
 *  +---+---+-----------+-----------+
 *  |op |cmp|   oparg   |  cmparg   |
 *  +---+---+-----------+-----------+
 *    4   4       12          12    <== # of bits
 */
#define __FUTEX_OP_OP_SHIFT 28U
#define __FUTEX_OP_CMP_SHIFT 24U
#define __FUTEX_OP_OPARG_SHIFT 12U

#define __FUTEX_OP_SIGN_BIT 11U

#define __FUTEX_OP_OP_MASK (0x7U << 28)
#define __FUTEX_OP_CMP_MASK (0xfU << 24)
#define __FUTEX_OP_OPARG_MASK (0xfffU << 12)
#define __FUTEX_OP_CMPARG_MASK (0xfffU)

#define __FUTEX_OP_SET 0U /* *(int *)uaddr2 = oparg; */
#define __FUTEX_OP_ADD 1U /* *(int *)uaddr2 += oparg; */
#define __FUTEX_OP_OR 2U /* *(int *)uaddr2 |= oparg; */
#define __FUTEX_OP_ANDN 3U /* *(int *)uaddr2 &= ~oparg; */
#define __FUTEX_OP_XOR 4U /* *(int *)uaddr2 ^= oparg; */

/*
 * bit-wise ORing the following value into op
 * causes (1 << oparg) to be used as the operand
 */
#define __FUTEX_OP_OPARG_FLAG 8U /* Use (1 << oparg) as operand */

#define __FUTEX_OP_CMP_EQ 0U /* if (oldval == cmparg) wake */
#define __FUTEX_OP_CMP_NE 1U /* if (oldval != cmparg) wake */
#define __FUTEX_OP_CMP_LT 2U /* if (oldval < cmparg) wake */
#define __FUTEX_OP_CMP_LE 3U /* if (oldval <= cmparg) wake */
#define __FUTEX_OP_CMP_GT 4U /* if (oldval > cmparg) wake */
#define __FUTEX_OP_CMP_GE 5U /* if (oldval >= cmparg) wake */

/* To be compabitable with linux futex, define the same macros as Linux here */
#define __FUTEX_OP_WAIT 0
#define __FUTEX_OP_WAKE 1
#define __FUTEX_OP_FD 2
#define __FUTEX_OP_REQUEUE 3
#define __FUTEX_OP_CMP_REQUEUE 4
#define __FUTEX_OP_WAKE_OP 5
#define __FUTEX_OP_LOCK_PI 6
#define __FUTEX_OP_UNLOCK_PI  7
#define __FUTEX_OP_TRYLOCK_PI 8
#define __FUTEX_OP_WAIT_BITSET 9
#define __FUTEX_OP_WAKE_BITSET 10

#define __FUTEX_PRIVATE_FLAG 128
#define __FUTEX_CLOCK_REALTIME 256
#define __FUTEX_CMD_MASK (~(__FUTEX_PRIVATE_FLAG | __FUTEX_CLOCK_REALTIME))

#define BIT_IOWAIT_SHIFT	16
#define BIT_IOWAIT_MASK		(1UL << BIT_IOWAIT_SHIFT)
#define BIT_IOWAIT(x)		(((x) >> BIT_IOWAIT_SHIFT) & 0x1U)

#ifdef CONFIG_USER_FUTEX

#ifndef __ASSEMBLY__
struct __ufutex_unblock_target_s {
	__u64 waiter_cref;
	__u64 gencnt_addr;
	__u32 gencnt_expected;
};
#endif

#define __EXT_FASTCALL_GLOBAL_UFUTEX_BLOCK_THREAD 19
#define __EXT_FASTCALL_GLOBAL_UFUTEX_UNBLOCK_THREADS 20

#define __UFUTEX_MAX_UNBLOCK_TARGET 32
#endif /* CONFIG_USER_FUTEX */

#ifndef __ASSEMBLY__
struct __block_param_s {
	const char *fname;
	const char *sname;
	unsigned int fname_size;
	unsigned int sname_size;
	unsigned long offset;
	unsigned long size;
};
#endif /* __ASSEMBLY__ */

#endif
