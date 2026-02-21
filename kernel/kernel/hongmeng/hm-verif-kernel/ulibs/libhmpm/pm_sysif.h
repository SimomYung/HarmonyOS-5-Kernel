/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for power manager common sysif wrapper
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 12:48:34 2019
 */
#ifndef ULIBS_LIBHMPM_PM_SYSIF_H
#define ULIBS_LIBHMPM_PM_SYSIF_H

#include <hmkernel/capability.h>

rref_t pm_sysif_get_rref(void);
rref_t fm_sysif_get_rref(void);

#endif
