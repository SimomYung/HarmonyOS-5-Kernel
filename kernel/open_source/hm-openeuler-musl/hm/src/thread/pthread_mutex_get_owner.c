/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 31 17:11:55 2019
 */
#include <errno.h>
#include <hmpthread.h>
#include "pthread_impl.h"

pid_t pthread_mutex_get_owner(const pthread_mutex_t *m)
{
	int rc = 0;
	pid_t own;

	if (m == NULL) {
		errno = EINVAL;
		rc = -1;
	}
	if (rc == 0) {
		own = (pid_t)((unsigned int)m->_m_lock & (unsigned int)0x3fffffff);
		if (own == 0x3fffffff) {
			errno = ENOTRECOVERABLE;
			rc = -1;
		}
	}

	return (rc == 0) ? own : rc;
}
