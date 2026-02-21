/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: HMbuild for udk hufs
 * Author: Kirin Platform Solution Dept, Hisilicon
 * Create: Mon Jan 23 17:23:00 UTC 2024
 */
 
#ifndef __UDK_INCLUDE_UDK_COMPLETION_H__
#define __UDK_INCLUDE_UDK_COMPLETION_H__
 
#include <stdio.h>
#include <udk/timer.h>
#include <udk/param.h>
#include <udk/mutex.h>
#include <udk/types.h>
#include <udk/thread_cond.h>
 
struct udk_completion {
	struct udk_thread_cond cond;
	struct udk_mutex mutex;
	int done;
};
 
void udk_completion_init(struct udk_completion *comp);
static inline void udk_completion_reinit(struct udk_completion *comp)
{
		comp->done = 0;
}
 
void udk_completion_wait(struct udk_completion *comp);
unsigned long udk_completion_wait_timeout(struct udk_completion *comp, unsigned long timeout);
void udk_complete(struct udk_completion *comp);
bool udk_completion_done(struct udk_completion *comp);
 
#endif // __UDK_INCLUDE_UDK_COMPLETION_H__
