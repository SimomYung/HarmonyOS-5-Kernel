/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Function declaration of raw counter
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 01 11:07:20 2020
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAWCOUNTER_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAWCOUNTER_H

#include <libhmsync/atomic.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define COUNTER_WAITER_BIT (30)
#define COUNTER_MAX  (0x3fffffff)
#define COUNTER_MASK (0x3fffffffU)

/*
* struct raw_counter - raw counter structure
* @count:	counter val
* @futex_val:	futex val
* @wa_val:	wa_val is the value of count when you need to wait or wake up.
* However, the value of count may be increased or decreased concurrently when
* the value of count is compared with that of wa_val. In the current scenario,
* raw_counter is not required to process the concurrency. Therefore, this function
* is not supported currently. Attention, please.
*/
struct raw_counter {
	raw_atomic_int_t count;
	volatile int futex_val;
	/* wait or wake val */
	volatile int wa_val;
};

static inline void raw_counter_init(struct raw_counter *cnter, int count, int wa_val)
{
	raw_atomic_int_init(&cnter->count,
			    (int)((unsigned int)count & COUNTER_MASK));
	cnter->wa_val = wa_val;
	cnter->futex_val = 0;
}

static inline int raw_counter_val(struct raw_counter *cnter)
{
	return (int)((unsigned int)raw_atomic_int_read(&cnter->count) &
		     COUNTER_MASK);
}

int raw_counter_inc(struct raw_counter *cnter);
int raw_counter_dec_wake(struct raw_counter *cnter);
int raw_counter_wait(struct raw_counter *cnter);

#ifdef __cplusplus
}
#endif

#endif
