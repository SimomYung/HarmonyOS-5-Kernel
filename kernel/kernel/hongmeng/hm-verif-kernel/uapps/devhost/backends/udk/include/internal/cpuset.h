/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: cpuset interfaces of udk internal
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 25 16:09:03 2022
 */
#ifndef __UDK_INTERNAL_CPUSET_H__
#define __UDK_INTERNAL_CPUSET_H__

#include <udk/cpuset.h>
#include <hmkernel/cpuset.h>

static inline __cpuset_t udk_cpuset_to_cpuset(udk_cpuset_t uset)
{
	__cpuset_t kset = (__cpuset_t){ .__bits = uset.__bits, };

	return kset;
}
static inline udk_cpuset_t cpuset_to_udk_cpuset(__cpuset_t kset)
{
	udk_cpuset_t uset = (udk_cpuset_t){ .__bits = kset.__bits, };

	return uset;
}

#endif /* __UDK_INTERNAL_CPUSET_H__ */
