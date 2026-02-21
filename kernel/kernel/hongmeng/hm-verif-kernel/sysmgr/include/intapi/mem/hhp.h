/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interface hybrid huge pages
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 05 17:57:42 2024
 */

#ifndef SYSMGR_MEMMGR_MEM_HHP_H
#define SYSMGR_MEMMGR_MEM_HHP_H

int hhp_policy(void);
int hhp_toggle(long policy);

#endif /* SYSMGR_MEMMGR_MEM_HHP_H */
