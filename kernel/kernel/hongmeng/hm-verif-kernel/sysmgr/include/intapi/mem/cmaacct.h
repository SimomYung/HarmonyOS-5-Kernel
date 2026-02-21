/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: CMA_ALLOC latency accounting feature
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 14:36:05 2024
 */
#ifndef SYSMGR_INTAPI_MEM_CMAACCT_H
#define SYSMGR_INTAPI_MEM_CMAACCT_H

#ifdef CONFIG_CMAACCT
int cmaacct_module_init(void);
int mem_cmaacct_read(char *data, size_t data_len);
int cmaacct_timeout_threshold_read(char *data, size_t data_len);
int cmaacct_timeout_threshold_write(int value);
#else
static inline int cmaacct_module_init(void) { return E_HM_OK; }
#endif
#endif
