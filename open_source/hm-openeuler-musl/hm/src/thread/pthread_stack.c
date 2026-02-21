/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thr May 07 14:47:19 2020
 */
#include "pthread_impl.h"
#include <hmpthread.h>

void *pthread_stack(const pthread_t t)
{
	return (t != NULL) ? t->stack : NULL;
}
