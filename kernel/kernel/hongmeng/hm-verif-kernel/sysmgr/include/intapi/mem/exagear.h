/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: exagear for mem
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 18 11:38:53 2025
 */

#ifndef INTAPI_MEM_EXAGEAR_H
#define INTAPI_MEM_EXAGEAR_H

#define MMAP_MIN_ADDR 0x10000000

#define __prctl_check_order(__m1, __op, __m2)				\
	((unsigned long)mm->__m1 __op (unsigned long)mm->__m2) ? 0 : E_HM_INVAL

int exagear_set_current_mmap_base(void *arg);
int exagear_set_current_layout(void *arg);

#endif