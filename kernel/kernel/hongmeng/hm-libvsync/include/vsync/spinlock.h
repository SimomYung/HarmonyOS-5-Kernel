/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: Backwards compatibility file
 * Author: Huawei Dresden Research Center
 * Create: 2023-03-31
 */
#ifndef VSYNC_COMPAT_SPINLOCK_H
#define VSYNC_COMPAT_SPINLOCK_H
/**
 * @file spinlock.h
 * @brief Use spinlock/caslock.h
 * @deprecated
 */
#include <vsync/spinlock/caslock.h>

#define spinlock_s caslock_s
typedef caslock_t spinlock_t;
#define SPINLOCK_INIT CASLOCK_INIT

static inline void spinlock_init(spinlock_t *l)
{
	caslock_init(l);
}

static inline void spinlock_acquire(spinlock_t *l)
{
	caslock_acquire(l);
}

static inline vbool_t spinlock_tryacquire(spinlock_t *l)
{
	return caslock_tryacquire(l);
}

static inline void spinlock_release(struct spinlock_s *l)
{
	caslock_release(l);
}

#endif
