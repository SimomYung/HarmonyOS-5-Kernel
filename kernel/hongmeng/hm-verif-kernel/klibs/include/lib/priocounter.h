/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prio counter function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 14 11:19:23 2020
 */
#ifndef KLIBS_PRIOCOUNTER_H
#define KLIBS_PRIOCOUNTER_H

#include <lib/errno.h>
#include <lib/atomic.h>
#include <hmkernel/bitops/bitops.h>

#define PRIOCOUNTER_MAX       0xffff

#define __DECLARE_PRIOCOUNTER_NAME(name, len)		\
struct name##_priocounter {				\
	u16 cnt[len];					\
	DEFINE_BITMAP(map, len);			\
};

#define __DECLARE_PRIOCOUNTER_OPS_INIT(name, len)				\
static inline void name##_priocounter_init(struct name##_priocounter *counter)	\
{										\
	unsigned int i = 0;							\
										\
	while (i < (len)) {							\
		counter->cnt[i] = (u16)0;					\
		i++;								\
	}									\
	bitmap_clear_all(counter->map, len);					\
}

/*
 * original highest priority is returned through *old_orio*,
 * old_prio >= PRIOCOUNTER_MAX indicates that the bitmap is empty, caller of
 * this function should deal with it
 */
#define __DECLARE_PRIOCOUNTER_OPS_ADD(name, len)				\
static inline unsigned int name##_priocounter_add(struct name##_priocounter *counter, \
						  unsigned int *old_prio, unsigned int new_prio) \
{										\
	unsigned int ret = 0u;							\
										\
	*old_prio = bitmap_find_first_one_msb(counter->map, len);		\
										\
	if (counter->cnt[new_prio] < PRIOCOUNTER_MAX) {				\
		if (counter->cnt[new_prio] == 0) {				\
			__set_bit(counter->map, new_prio);			\
		}								\
		counter->cnt[new_prio]++;					\
		ret = bitmap_find_first_one_msb(counter->map, len);		\
	} else {								\
		ret = (unsigned int)(-1);					\
	}									\
	return ret;								\
}

#define __DECLARE_PRIOCOUNTER_OPS_DEL(name)					\
static inline int name##_priocounter_del(struct name##_priocounter *counter,	\
					unsigned int prio)			\
{										\
	int ret = E_HM_OK;							\
										\
	if (counter->cnt[prio] == 0) {						\
		ret = E_HM_OVERFLOW;						\
	} else {								\
		counter->cnt[prio]--;						\
		if (counter->cnt[prio] == 0) {					\
			__clear_bit(counter->map, prio);			\
		}								\
	}									\
	return ret;								\
}

#define __DECLARE_PRIOCOUNTER_OPS_GET_HIGHEST_PRIO(name, len)			\
static inline unsigned int							\
name##_priocounter_get_highest_prio(struct name##_priocounter *counter)		\
{										\
	return bitmap_find_first_one_msb(counter->map, len);			\
}


#define DECLARE_PRIOCOUNTER(name, len)				\
	__DECLARE_PRIOCOUNTER_NAME(name, len)			\
	__DECLARE_PRIOCOUNTER_OPS_INIT(name, len)		\
	__DECLARE_PRIOCOUNTER_OPS_ADD(name, len)		\
	__DECLARE_PRIOCOUNTER_OPS_DEL(name)			\
	__DECLARE_PRIOCOUNTER_OPS_GET_HIGHEST_PRIO(name, len)	\

#endif
