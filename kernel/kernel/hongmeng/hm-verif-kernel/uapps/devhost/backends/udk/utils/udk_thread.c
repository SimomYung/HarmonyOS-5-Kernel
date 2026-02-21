/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of thread in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <sched.h>
#include <udk/thread.h>

#include <hongmeng/errno.h>
#include <hmkernel/cpuset.h>
#include <internal/cpuset.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_ct_task.h>

raw_static_assert(sizeof(raw_thread_attr_t) == sizeof(udk_thread_attr_t),
		  udk_thread_attr_t_size_is_wrong);

int udk_thread_create(udk_thread_t *thread,
		      const udk_thread_attr_t *attr,
		      void *(*func)(void *args),
		      void *args)
{
	return -hmerrno2posix(raw_thread_create((raw_thread_t *)(uintptr_t)thread,
						(raw_thread_attr_t *)(uintptr_t)attr,
						func, args));
}

int udk_thread_join(udk_thread_t thread, void **ret)
{
	return -hmerrno2posix(raw_thread_join((raw_thread_t)thread, ret));
}

int udk_thread_terminate(const struct udk_thread* thread)
{
	UNUSED(thread);
	return -ENOSYS;
}

int udk_thread_yield(void)
{
	return -hmerrno2posix(hm_thread_yield());
}

udk_thread_t udk_thread_self(void)
{
	return (udk_thread_t)raw_thread_self();
}

int udk_thread_setname(udk_thread_t thread, const char *new_name, char *old_name)
{
	cref_t cref = raw_thread_cref_of((raw_thread_t)thread);

	return -hmerrno2posix(hm_thread_setname(cref, new_name, old_name));
}

int udk_thread_setaffinity(udk_thread_t thread, udk_cpuset_t cpuset)
{
	int ret;
	int tid;

	tid = raw_thread_tid_of((raw_thread_t)thread);
	if (tid < 0) {
		return -EINVAL;
	}
	cpu_set_t affinity = {0};
	affinity.__bits[0] = (unsigned long)cpuset.__bits;
	ret = hm_thread_sched_setaffinity(tid, sizeof(affinity), &affinity);

	return -hmerrno2posix(ret);
}

int udk_thread_getaffinity(udk_thread_t thread, udk_cpuset_t *cpuset)
{
	int ret;
	int tid;

	if (cpuset == NULL) {
		return -EINVAL;
	}
	tid = raw_thread_tid_of((raw_thread_t)thread);
	if (tid < 0) {
		return -EINVAL;
	}
	cpu_set_t affinity = {0};
	ret = hm_thread_sched_getaffinity(tid, sizeof(affinity), &affinity);
	cpuset->__bits = (__u32)affinity.__bits[0];
	if (ret > 0) {
		ret = 0;
	}

	return -hmerrno2posix(ret);
}

/*
 * udk_lock_task and udk_unlock_task are temporary
 * and subject to change in future versions.
 */
void udk_lock_task(void)
{
	(void)hm_ct_lock_task();
}

void udk_unlock_task(void)
{
	(void)hm_ct_unlock_task();
}
