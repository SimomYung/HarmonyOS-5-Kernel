/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Function declaration of raw completion
 * Author: Huawei CBG OS Lab
 * Create: Wed Mar 20 2024
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAWCOMPLETION_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAWCOMPLETION_H

#include <hongmeng/panic.h>
#include <libhmsync/raw_counter.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct raw_completion {
	struct raw_counter done;
};

/* Initialize a new completion */
static inline void raw_completion_init(struct raw_completion *comp)
{
	BUG_ON_D(comp == NULL);
	raw_counter_init(&comp->done, 1, 0);
}

/* Wake up a waiter */
static inline void raw_completion_done(struct raw_completion *comp)
{
	BUG_ON_D(comp == NULL);
	raw_counter_dec_wake(&comp->done);
}

/* Wait until completion is complete */
int raw_completion_wait(struct raw_completion *comp);

#ifdef __cplusplus
}
#endif

#endif /* ULIBS_INCLUDE_LIBHMSYNC_RAWCOMPLETION_H */
