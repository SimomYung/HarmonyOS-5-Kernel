/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: cpuset interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 23 10:12:03 2022
 */
#include <stdlib.h>

#include <udk/cpuset.h>
#include <hmkernel/cpuset.h>
#include <internal/cpuset.h>
#include <libstrict/strict.h>

raw_static_assert(sizeof(udk_cpuset_t) == sizeof(__cpuset_t),
		  udk_cpuset_t_size_is_wrong);

udk_cpuset_t udk_cpuset_empty(void)
{
	return cpuset_to_udk_cpuset(__cpuset_empty);
}

udk_cpuset_t udk_cpuset_add(udk_cpuset_t uset, unsigned int cpu)
{
	__cpuset_t kset = udk_cpuset_to_cpuset(uset);

	kset = __cpuset_add(kset, cpu);

	return cpuset_to_udk_cpuset(kset);
}

bool udk_cpuset_equal(udk_cpuset_t a, udk_cpuset_t b)
{
	int ret;
	__cpuset_t kseta = udk_cpuset_to_cpuset(a);
	__cpuset_t ksetb = udk_cpuset_to_cpuset(b);

	ret = __cpuset_equal(kseta, ksetb);

	return (ret == 0) ? false : true;
}
