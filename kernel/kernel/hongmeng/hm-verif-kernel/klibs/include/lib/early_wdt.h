/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Early watchdog header
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 19 10:26:10 2018
 */
#ifndef KLIBS_EARLY_WDT_H
#define KLIBS_EARLY_WDT_H

#include <hmkernel/compiler.h>

extern void early_watchdog_init_hisi_hi1210(void __iomem *base_addr);
extern int reset_early_watchdog_hisi_hi1210(void);

extern void early_watchdog_init_hisi_sd6219e(void __iomem *base_addr);
extern int reset_early_watchdog_hisi_sd6219e(void);

#endif
