/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implement dump functions in ulibs that can be overwritten by sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 17 11:30:47 2024
 */
#include <libhmsrv_sys/hm_dump.h>
#include <hmsysmgr/dumpstack.h>
#include <libhmlog/hmlog.h>

int dump_current_thread_stack(unsigned int flags)
{
	return hm_dump_thread_to_klog(0, flags);
}

int dump_thread_stack(pid_t tid, unsigned int flags)
{
	return hm_dump_thread_to_klog(tid, flags);
}
