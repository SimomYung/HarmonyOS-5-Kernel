/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: internal guardmgr interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 9 14:11:05 2024
 */

#ifndef SYSMGR_INCLUDE_SYSRQ_H
#define SYSMGR_INCLUDE_SYSRQ_H

typedef int (*dump_freeze)(void);

void sysrq_register_dump_freeze(dump_freeze dump_func);

#endif /* SYSMGR_INCLUDE_SYSRQ_H */
