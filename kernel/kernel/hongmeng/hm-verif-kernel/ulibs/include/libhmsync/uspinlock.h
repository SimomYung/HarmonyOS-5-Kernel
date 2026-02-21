/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: User space spinlock structure definition and api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 29 21:12:20 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_USPIN_H
#define ULIBS_INCLUDE_LIBHMSYNC_USPIN_H

#include <hongmeng/types.h>
#include <vsync/spinlock.h>

struct uspinlock_s {
	struct spinlock_s s;
};

#define USPINLOCK_INITIALIZER			\
{						\
	.s = SPINLOCK_INIT(),			\
}

static inline void uspinlock_init(struct uspinlock_s *usl)
{
	spinlock_init(&usl->s);
}

/* return cpuid */
int uspinlock_lock(struct uspinlock_s *usl);
void uspinlock_unlock(struct uspinlock_s *usl);

void uspinlock_lock_lite(struct uspinlock_s *usl);
void uspinlock_unlock_lite(struct uspinlock_s *usl);
/*
 * Trylock API usage similar to mutex:
 * on success, return E_HM_OK; on failure, return < 0
 */
int uspinlock_trylock(struct uspinlock_s *usl);

void disable_uspinlock(void);
void enable_uspinlock(void);
_Bool is_uspinlock_disabled(void);

#endif
