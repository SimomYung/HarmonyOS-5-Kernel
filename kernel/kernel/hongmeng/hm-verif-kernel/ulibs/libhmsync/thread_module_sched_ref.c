/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implement of thread_module_sched_ref in ulib
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 05 20:49:13 2020
 */
#include <libhmsync/thread.h>
#include <libhmsrv_sys/hmsrv_sys.h>

cref_t thread_module_sched_ref(void)
{
	return hmsrv_module_sched_rref_of();
}
