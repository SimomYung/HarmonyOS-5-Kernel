/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Robust futex function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 21 15:12:22 2019
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_ROBUST_FUTEX_H
#define H_SYSMGR_INCLUDE_THREAD_ROBUST_FUTEX_H

#include <intapi/thread/thread.h>

/* one bit to indicate if this robust futex has waiters */
#define FUTEX_HAS_WAITERS      0x80000000U
/* one bit to indicate that the target thread holding this futex has died */
#define FUTEX_OWNER_DEAD       0x40000000U
/* reset field of this robust futex is tid */
#define FUTEX_OWNER_MASK       0x3fffffffU
/* limit of loop times handling robust futex to avoid vicious circular robust futex list */
#define ROBUST_LIST_LOOP_LIMIT 1024

struct futex_list {
	struct futex_list *next;
};

struct thread_robust_list {
	/* robust futex list head */
	struct futex_list head;
	/* offset between list node and futex field to examine */
	long futex_off;
	/*
	 * The death of the thread may race with user setting up
	 * a lock's links. To handle this race condition, user should:
	 *   1. set this field to the address of the to-be-taken lock.
	 *   2. acquire the lock.
	 *   3. add itself to the list.
	 *   4. clear this field.
	 * Then sysmgr will always have full knowledge of all locks
	 * that the thread might have taken.
	 */
	struct futex_list *pending;
};

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
/* compatible struct with futex_list */
struct compat_futex_list {
	__u32 next;
};

/* compatible struct with thread_robust_list */
struct compat_thread_robust_list {
	struct compat_futex_list head;
	__s32 futex_off;
	__u32 pending;
};
#endif

struct thread_s;
void handle_exit_robust_list(struct thread_s *thread);
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
void handle_exit_compat_robust_list(struct thread_s *thread);
#endif

#endif
