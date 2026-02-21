/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: page alloc and page fault latency accounting feature
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 11:55:05 2024
 */
#ifndef SYSMGR_ALLOC_ACCT_H
#define SYSMGR_ALLOC_ACCT_H

#include <hongmeng/types.h>

#ifdef CONFIG_ALLOC_ACCT
int allocacct_module_init(void);
int allocacct_disable_read(char *data, size_t data_len);
int allocacct_disable_write(int value);
int allocacct_timeout_threshold_read(char *data, size_t data_len);
int allocacct_timeout_threshold_write(int value);
#else
static inline int allocacct_module_init(void) { return E_HM_OK; };
#endif

#endif