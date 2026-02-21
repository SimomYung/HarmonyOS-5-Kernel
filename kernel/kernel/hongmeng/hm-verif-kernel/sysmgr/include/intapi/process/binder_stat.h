/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Binder stat declaration
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 15:48:10 2024
 */
#ifndef SYSMGR_INTAPI_PROCESS_BINDER_STAT_H
#define SYSMGR_INTAPI_PROCESS_BINDER_STAT_H

#ifdef CONFIG_PROCESS_BINDER_STAT

void binder_stat_alive_log(int reply, uid_t c_uid, int c_pid, uid_t s_uid, int s_pid);

#else

#include <libstrict/strict.h>

static inline void binder_stat_alive_log(int reply, uid_t c_uid, int c_pid, uid_t s_uid, int s_pid)
{
	UNUSED(reply, c_uid, c_pid, s_uid, s_pid);
}
#endif /* CONFIG_PROCESS_BINDER_STAT */

#endif /* SYSMGR_INTAPI_PROCESS_BINDER_STAT_H */
