/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Error handling functions of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 24 16:50:41 2019
 */
#ifndef __UDK_PANIC_H__
#define __UDK_PANIC_H__

#include <udk/compiler.h>
#include <hongmeng/panic.h>

/* DEPRECATED */
void __noreturn
udk_bug(const char *assertion, const char *file, const char *function, int line);

#endif /* __UDK_PANIC_H__ */
