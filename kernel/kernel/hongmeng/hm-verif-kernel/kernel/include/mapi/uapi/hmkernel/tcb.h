/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for tcb uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 13:36:28 2019
 */
#ifndef UAPI_TCB_H
#define UAPI_TCB_H

enum __tcb_state {
	__TCB_STATE_READY,
	__TCB_STATE_RUNNING,
	__TCB_STATE_BLOCKED,
	__TCB_STATE_DEAD,
	__TCB_STATE_UNKNOWN,
};

enum __tcb_block_reason {
	__TCB_BLOCK_FUTEX,
	__TCB_BLOCK_LOCK,
	__TCB_BLOCK_RECV,
	__TCB_BLOCK_TRIGGER,
	__TCB_BLOCK_EVENTPOLL,
	__TCB_BLOCK_ACTVPOOL,
	__TCB_BLOCK_ULWT_MON,
	__TCB_BLOCK_ULWT_KINFO,
	__TCB_BLOCK_PAUSE,
	__TCB_BLOCK_SLEEP,
	__TCB_BLOCK_AT_ADDR,
	__TCB_NO_BLOCK,
};

enum __tcb_attach_type {
	__TCB_ATTACH_TYPE_THREAD,
	__TCB_DETACH_TYPE_THREAD,
	__TCB_ATTACH_TYPE_ACTV,
	__TCB_DETACH_TYPE_ACTV,
	__TCB_DETACH_TYPE_UNKONWN,
};

#define __IS_TCB_WAIT_WITH_LOCKOWNER_INFO(reason)	\
	((reason) == __TCB_BLOCK_LOCK ||		\
	 (reason) == __TCB_BLOCK_FUTEX ||		\
	 (reason) == __TCB_BLOCK_AT_ADDR)

#define TCB_SUSPEND_CMP_EQ	(0U)	 /* if *addr == expected, suspend */
#define TCB_SUSPEND_CMP_NE	(1U)	 /* if *addr != expected, suspend */
#define TCB_SUSPEND_CMP_LT	(2U)	 /* if *addr < expected, suspend */
#define TCB_SUSPEND_CMP_LE	(3U)	 /* if *addr <= expected, suspend */
#define TCB_SUSPEND_CMP_GT	(4U)	 /* if *addr > expected, suspend */
#define TCB_SUSPEND_CMP_GE	(5U)	 /* if *addr >= expected, suspend */

#define TCB_SUSPEND_MODE_BLOCK	(1U)	 /* suspend through tcb blcok, must be current tcb */

/*
 * TCB_SUSPEND expected value is a 32bits integer(unsigned int) value:
 * +---+----+---+-----------------------+
 * |rsv|mode|cmp|         value         |
 * +---+----+---+-----------------------+
 *   3    1  4         24
 */
#define TCB_SUSPEND_CMP_SHIFT	(24)
#define TCB_SUSPEND_CMP_NBITS	(4)
#define TCB_SUSPEND_MODE_NBITS	(1)
#define TCB_SUSPEND_CMP_MASK	((1U << TCB_SUSPEND_CMP_NBITS) - 1)
#define TCB_SUSPEND_MODE_MASK	((1U << TCB_SUSPEND_MODE_NBITS) - 1)
#define TCB_SUSPEND_MODE_SHIFT	(TCB_SUSPEND_CMP_SHIFT + TCB_SUSPEND_CMP_NBITS)
#define TCB_SUSPEND_RSV_SHIFT	(TCB_SUSPEND_MODE_SHIFT + TCB_SUSPEND_MODE_NBITS)
#define TCB_SUSPEND_VALUE_MASK	((1U << TCB_SUSPEND_CMP_SHIFT) - 1)

#define TCB_SUSPEND_CMP(n)						\
	(((n) >> TCB_SUSPEND_CMP_SHIFT) & TCB_SUSPEND_CMP_MASK)
#define TCB_SUSPEND_MODE(n)						\
	(((n) >> TCB_SUSPEND_MODE_SHIFT) & TCB_SUSPEND_CMP_MASK)
#define TCB_SUSPEND_VALUE(n)		((n) & TCB_SUSPEND_VALUE_MASK)
#define TCB_SUSPEND_RSV(n)		((n) >> TCB_SUSPEND_RSV_SHIFT)

#define TCB_SUSPEND_CMPARG(cmp, val)					\
	(((TCB_SUSPEND_CMP_MASK & (cmp)) << TCB_SUSPEND_CMP_SHIFT) |	\
	 ((TCB_SUSPEND_VALUE_MASK) & (val)))

#define TCB_SUSPEND_MODEARG(mode, cmp, val)					\
	(((TCB_SUSPEND_MODE_MASK & (mode)) << TCB_SUSPEND_MODE_SHIFT) |	\
	 ((TCB_SUSPEND_CMP_MASK & (cmp)) << TCB_SUSPEND_CMP_SHIFT) |	\
	 ((TCB_SUSPEND_VALUE_MASK) & (val)))

#endif
