/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Improper allocation accounting feature
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 14:36:05 2024
 */
#ifndef SYSMGR_MEMMGR_MEM_STAT_IMPROPER_H
#define SYSMGR_MEMMGR_MEM_STAT_IMPROPER_H
#include <libmem/types.h>

#ifdef CONFIG_IMPROPER_STAT
void improper_alloc_hook(uint32_t order, uint32_t paf);
#else
static inline void improper_alloc_hook(uint32_t order, uint32_t paf) {};
#endif

int improper_enable_read(char *data, size_t data_len);
int improper_enable_write(int val);
int improper_interval_read(char *data, size_t data_len);
int improper_interval_write(int val);
int improper_sample_rate_read(char *data, size_t data_len);
int improper_sample_rate_write(int val);

uint64_t improper_stat_high_order_show(void);
uint64_t improper_stat_atomic_show(void);

#endif

