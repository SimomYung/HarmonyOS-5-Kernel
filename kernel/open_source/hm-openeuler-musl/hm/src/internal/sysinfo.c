/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 18 11:06:42 2019
 */

#include "libc.h"

/**
 * We need to use __libc to setup tls for raw thread. This function is used by ulibs
 * to get struct __libc.
 *
 * If we use __libc variable directly in ulibs, it will use a local variable from ulibs
 * not from libc.so. So we provide a function to get __libc pointer.
 */
struct __libc* __get_libc(void);
struct __libc* __get_libc(void)
{
	return &__libc;
}
