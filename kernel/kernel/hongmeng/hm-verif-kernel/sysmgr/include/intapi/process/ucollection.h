/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Unified collection declaration
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 16 17:31:21 2023
 */
#ifndef SYSMGR_INCLUDE_PROCESS_UCOLLECTION_H
#define SYSMGR_INCLUDE_PROCESS_UCOLLECTION_H

#ifdef CONFIG_UNIFIED_COLLECTION
int dev_ucollection_ioctl(unsigned int cmd, unsigned long arg);
void fill_ucollection_cap_info(unsigned long cpuset);
#endif

#endif
