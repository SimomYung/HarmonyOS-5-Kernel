/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Pseudo spinlock operations for ICES
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 03 17:00:10 CST 2019
 */
#ifndef MODULES_SPINLOCK_H
#define MODULES_SPINLOCK_H

#include <hmkernel/strict.h>

struct spinlock_t { };

static inline void
spin_lock_init(struct spinlock_t *lock)
{
	UNUSED(lock);
}

static inline void
spin_lock(struct spinlock_t *lock)
{
	UNUSED(lock);
}

static inline void
spin_unlock(struct spinlock_t *lock)
{
	UNUSED(lock);
}

static inline void
spinlock_none(struct spinlock_t *lock)
{
	UNUSED(lock);
}
#endif
