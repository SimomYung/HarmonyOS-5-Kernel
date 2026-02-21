/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Kconsole interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Wen Jul 21 08:57:02 2021
 */
#ifndef __UDK_KCONSOLE_H__
#define __UDK_KCONSOLE_H__

#include <stddef.h>

int udk_kconsole_print(const char* str, size_t size);
int udk_kconsole_enable(void);
int udk_kconsole_disable(void);

#endif /* __UDK_KCONSOLE_H__ */
