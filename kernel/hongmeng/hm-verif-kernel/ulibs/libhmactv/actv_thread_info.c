/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of actv thread info
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 30 14:57:55 2023
 */

#include <libhmactv/actv.h>

pid_t hm_actv_local_thread_vtid_of(void)
{
	return __hm_actv_local_thread_vtid_of();
}

pid_t hm_actv_local_thread_vpid_of(void)
{
	return __hm_actv_local_thread_vpid_of();
}

pid_t hm_actv_local_thread_tid_of(void)
{
	return __hm_actv_local_thread_tid_of();
}

pid_t hm_actv_local_thread_pid_of(void)
{
	return __hm_actv_local_thread_pid_of();
}

const char *hm_actv_local_thread_name_of(void)
{
	return __hm_actv_local_thread_name_of();
}

unsigned long long hm_actv_local_thread_cref_of(void)
{
	return __hm_actv_local_thread_cref_of();
}

unsigned long long hm_actv_local_sched_thread_cref_of(void)
{
	return __hm_actv_local_sched_thread_cref_of();
}

unsigned int hm_actv_local_thread_priority_of(void)
{
	return __hm_actv_local_thread_priority_of();
}

unsigned long long hm_actv_local_thread_vskey_of(void)
{
	return __hm_actv_local_thread_vskey_of();
}
