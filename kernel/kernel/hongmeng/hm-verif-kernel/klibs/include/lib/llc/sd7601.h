/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Last level cache interface header for sd7601
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 02 17:08:03 2021
 */

#ifndef KLIBS_LLC_SD7601_H
#define KLIBS_LLC_SD7601_H

#include <hmasm/memory.h>

extern void sd7601_invalidate_llc_all(void __iomem *base_addr);
extern void sd7601_clean_llc_all(void __iomem *base_addr);
extern void sd7601_flush_llc_all(void __iomem *base_addr);

#endif
