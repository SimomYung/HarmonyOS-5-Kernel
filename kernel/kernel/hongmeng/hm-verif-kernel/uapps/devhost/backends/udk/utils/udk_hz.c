/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: parameters interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 28 15:22:41 2022
 */
#include <udk/param.h>
#include <hmasm/param.h>

unsigned int udk_hz(void)
{
	return HZ;
}