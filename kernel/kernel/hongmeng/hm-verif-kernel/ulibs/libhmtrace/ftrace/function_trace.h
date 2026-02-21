/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of data struct and declaration of inner interface of function trace
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 15:15:41 2020
 */
#ifndef FUNCTION_TRACE_H
#define FUNCTION_TRACE_H

#include <libhmtrace/hmtrace.h>

#define FUNC_TRACE_DEFAULT_RB_SIZE	0x00010000

extern struct symbol_desc *elf_sym_desc;
extern enum symbol_mode sym_mode;
#endif
