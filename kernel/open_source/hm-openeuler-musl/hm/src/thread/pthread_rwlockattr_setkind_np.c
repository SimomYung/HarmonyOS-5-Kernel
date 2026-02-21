/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei RTOS
 * Create: Thu June 24 09:34:00 2021
 */


#include "pthread_impl.h"

int pthread_rwlockattr_setkind_np (pthread_rwlockattr_t *attr, int pref)
{
	// No requirement. This function does not need to be implemented currently.
	return EINVAL;
}
