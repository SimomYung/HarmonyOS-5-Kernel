/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: internal flush cache api by cache.S implement
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 12 09:40:43 2023
 */
#ifndef ULIBS_HM_FLUSH_CHCHE_INTERNAL_H
#define ULIBS_HM_FLUSH_CHCHE_INTERNAL_H

int clean_dcache_range_el0(unsigned long vstart, unsigned long vend);
int flush_dcache_range_el0(unsigned long vstart, unsigned long vend);

#endif
