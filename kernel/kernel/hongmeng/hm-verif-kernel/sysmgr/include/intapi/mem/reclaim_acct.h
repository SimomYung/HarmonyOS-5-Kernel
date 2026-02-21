/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Memory reclaim delay accouting
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 18 15:19:12 2023
 */
#ifndef SYSMGR_RECLAIM_ACCT_H
#define SYSMGR_RECLAIM_ACCT_H

#include <hongmeng/types.h>

#ifdef CONFIG_RECLAIM_ACCT
int reclaimacct_module_init(void);
int reclaimacct_disable_read(char *data, size_t data_len);
int reclaimacct_disable_write(int value);
int reclaimacct_timeout_threshold_read(char *data, size_t data_len);
int reclaimacct_timeout_threshold_write(int value);
#else
static inline int reclaimacct_module_init(void) { return E_HM_OK; }
#endif

#endif
