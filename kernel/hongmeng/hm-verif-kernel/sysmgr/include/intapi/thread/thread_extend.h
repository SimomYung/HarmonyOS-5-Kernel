/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Extend interfaces of operation on thread in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 8 11:16:28 2023
 */

#ifndef SYSMGR_INCLUDE_THREAD_EXTEND_H
#define SYSMGR_INCLUDE_THREAD_EXTEND_H

#include <intapi/thread/thread.h>

static inline struct thread_s *spinhdlr_caller_thread_of(void)
{
	raw_thread_t raw_thread = raw_thread_self();
	return ptr_from_void(raw_thread_private_of(raw_thread), struct thread_s);
}

static inline unsigned long spinhdlr_caller_thread_cred_of(void)
{
	return sysif_actv_current_callinfo()->credential;
}
#endif /* SYSMGR_INCLUDE_THREAD_EXTEND_H */
