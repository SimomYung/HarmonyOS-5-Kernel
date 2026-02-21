/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Asan output log header
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 03 20:17:45 2022
 */
#ifndef ASAN_PRINT_H
#define ASAN_PRINT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DEFAULT_OUTPUT_BUF_SIZE 512

void asan_print_with_len(const char *output, size_t len);

static inline void asan_print(const char *output)
{
	asan_print_with_len(output, 0);
}

void asan_handle_error(void);
void asan_report_address(uintptr_t addr, size_t size, bool write, uintptr_t ret_addr);
void asan_report_alloc_mismatch(const char *free_type_name, const char *real_type_name);

#endif
