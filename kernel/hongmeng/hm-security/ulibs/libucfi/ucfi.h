/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Header file for ucfi initialization
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 8 11:29:11 2022
 */
#ifndef __UCFI_H_
#define __UCFI_H_

#include <stddef.h>

int ucfi_init(void);
void ucfi_update(void *dso, bool is_open);
__attribute__((visibility("default"))) int ucfi_fail(unsigned long addr);
#endif
