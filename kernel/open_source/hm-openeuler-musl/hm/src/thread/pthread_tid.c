/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 08 14:47:19 2019
 */
#include "pthread_impl.h"
#include <hmpthread.h>

int pthread_tid(const pthread_t t)
{
	return (t != NULL) ? t->tid : -EINVAL;
}
