/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: Kbox lib interfaces of kbox_snapshot
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 24 15:54:16 2024
 */
#ifndef ULIBS_LIBKBOX_SNAPSHOT_H
#define ULIBS_LIBKBOX_SNAPSHOT_H

#include <stdint.h>

/* max size for one compress unit */
#define SNAPSHOT_COMP_UNIT (128 * 1024)

typedef int (*decomp_func_t)(const void *in, size_t in_size, void *out, size_t out_size);
typedef int (*dump_func_t)(const void *log, unsigned int len, uintptr_t dump_arg);

int kbox_decompress_snapshot(const void *input_buf, size_t input_len,
			     decomp_func_t decomp_func,
			     dump_func_t dump_func, uintptr_t dump_arg);

#endif