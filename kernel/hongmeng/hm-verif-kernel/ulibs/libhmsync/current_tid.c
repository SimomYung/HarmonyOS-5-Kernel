/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Get current thread tid
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 16:55:30 2019
 */
#include <libhmsync/raw_thread.h>
#include <libhmsync/thread.h>
#include "raw_tls.h"

int thread_get_current_tid(void)
{
	pid_t tid = -1;
	/*
	 * if uapp tries to call this function before tls is set, for example,
	 * use recursive mutex before calling raw_tls_init() in crt hook,
	 * then -1 is returned to indicate this special case, this will make
	 * recursive mutex degrade into a normal mutex.
	 */
	if (raw_get_tls() != 0UL) {
		tid = raw_thread_tid_of(raw_thread_self());
	}
	return tid;
}
