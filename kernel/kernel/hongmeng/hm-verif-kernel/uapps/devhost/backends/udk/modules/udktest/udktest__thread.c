/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Desciption: Udk thread releated interface test
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 17:16:07 2022
 */
#include "udktests.h"

#include <string.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/thread.h>
#include <libstrict/strict.h>

static int test_udk_exited = 0;
static void *test_udk_threadfn(void *args)
{
	UNUSED(args);
	while (test_udk_exited == 0) {
		(void)udk_thread_yield();
	}

	return NULL;
}

static void test_udk_thread_exit(udk_thread_t thread)
{
	void *ret_thread = NULL;

	/* exit udk thread */
	test_udk_exited = 1;
	(void)udk_thread_join(thread, &ret_thread);
}

static int test_udk_thread_affinity(void)
{
	int ret;
	udk_thread_t thread;
	udk_cpuset_t cpus_set = udk_cpuset_empty();
	udk_cpuset_t cpus_get = udk_cpuset_empty();

	/* create udk thread */
	ret = udk_thread_create(&thread, NULL, test_udk_threadfn, NULL);
	if (ret < 0) {
		udk_error("udk thread create failed\n");
		return ret;
	}

	cpus_set = udk_cpuset_add(cpus_set, 0);
	ret = udk_thread_setaffinity(thread, cpus_set);
	if (ret < 0) {
		udk_error("udk thread setaffinity failed\n");
		test_udk_thread_exit(thread);
		return ret;
	}
	ret = udk_thread_getaffinity(thread, &cpus_get);
	if (ret < 0) {
		udk_error("udk thread getaffinity failed\n");
		test_udk_thread_exit(thread);
		return ret;
	}
	if (!udk_cpuset_equal(cpus_set, cpus_get)) {
		udk_error("udk thread affinity cpus set and get dismatch\n");
		test_udk_thread_exit(thread);
		return -EINVAL;
	}
	test_udk_thread_exit(thread);
	udk_info("udk thread affinity test success\n");

	return 0;
}

int udktest__thread(void)
{
	int ret;

	ret = test_udk_thread_affinity();
	if (ret < 0) {
		return ret;
	}

	return 0;
}
