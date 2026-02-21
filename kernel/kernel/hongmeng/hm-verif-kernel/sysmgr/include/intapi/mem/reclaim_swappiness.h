/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Memory reclaim swappiness
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 17 18:06:14 2023
 */

#ifndef SYSMGR_INTAPI_MEM_RECLAIM_SWAPPINESS_H
#define SYSMGR_INTAPI_MEM_RECLAIM_SWAPPINESS_H

#include <hongmeng/types.h>

int reclaim_swappiness_get(void);
int reclaim_swappiness_set(int val);
int reclaim_swappiness_read(char *data, size_t data_len);
int reclaim_swappiness_write(uint64_t pos, const char *src, size_t size, size_t *wsize);

#endif /* SYSMGR_INTAPI_MEM_RECLAIM_SWAPPINESS_H */
