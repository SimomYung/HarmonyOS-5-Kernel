/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Internal interfaces for backtrace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 30 09:14:35 2020
 */
#ifndef HM_INCLUDE_MEM_DUMP_H
#define HM_INCLUDE_MEM_DUMP_H

#include <stdint.h>

typedef int (*dump_stack_func_t)(uintptr_t *buf, int max);

int dump_init(void);

int dump_stack(uintptr_t *buf, int max);

int load_symbol(const char *name);

int unload_symbol(void);

char *addr2sym(uintptr_t addr, char *sym, int max);

extern dump_stack_func_t __dump_stack_func;

#endif
