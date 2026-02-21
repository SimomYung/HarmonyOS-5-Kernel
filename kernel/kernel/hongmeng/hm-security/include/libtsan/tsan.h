/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Header file for tsan enable
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 15 11:11:11 2022
 */

#ifndef TSAN_H
#define TSAN_H

#include <stdbool.h>

void tsan_enable(void);
void tsan_disable(void);
bool tsan_set_delay(unsigned int t);
bool tsan_set_consecutive(unsigned int t);

typedef void (* tsan_backtrace_callback_type)(unsigned int);
void tsan_set_backtrace_callback(tsan_backtrace_callback_type back);
#endif
