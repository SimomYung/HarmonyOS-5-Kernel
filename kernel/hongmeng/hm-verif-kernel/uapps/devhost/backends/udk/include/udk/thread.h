/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Thread interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 16:44:03 2021
 */
#ifndef __UDK_THREAD_H__
#define __UDK_THREAD_H__

#include <udk/cpuset.h>

struct udk_thread;
typedef struct udk_thread *udk_thread_t;
typedef struct {
	unsigned int  inherit;
	unsigned int  policy;
	unsigned int  prio;
	unsigned int  flags;
	unsigned int  mode;
	unsigned long stacksize;
	unsigned long guardsize;
	unsigned long stackaddr;
} udk_thread_attr_t;

int udk_thread_create(udk_thread_t *thread,
			     const udk_thread_attr_t *attr,
			     void *(*func)(void *args), void *args);
int udk_thread_join(udk_thread_t thread, void **ret);
int udk_thread_terminate(const struct udk_thread* thread);
int udk_thread_yield(void);
udk_thread_t udk_thread_self(void);
int udk_thread_setname(udk_thread_t thread, const char *new_name, char *old_name);
int udk_thread_setaffinity(udk_thread_t thread, udk_cpuset_t cpuset);
int udk_thread_getaffinity(udk_thread_t thread, udk_cpuset_t *cpuset);

/*
 * udk_lock_task and udk_unlock_task are temporary
 * and subject to change in future versions.
 */
void udk_lock_task(void);
void udk_unlock_task(void);

#endif /* __UDK_THREAD_H__ */
