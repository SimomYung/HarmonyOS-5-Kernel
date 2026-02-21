/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of kconsole in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <udk/kconsole.h>

#include <devhost/backend.h>

int udk_kconsole_print(const char* str, size_t size)
{
	return devhost_kconsole_print(str, size);
}

int udk_kconsole_enable(void)
{
	return devhost_kconsole_enable();
}

int udk_kconsole_disable(void)
{
	return devhost_kconsole_disable();
}
