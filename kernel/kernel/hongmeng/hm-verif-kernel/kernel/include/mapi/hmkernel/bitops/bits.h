/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Bit shift
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 27 14:51:06 2022
 */
#ifndef MAPI_HMKERNEL_BITOPS_BITS_H
#define MAPI_HMKERNEL_BITOPS_BITS_H

#include <hmkernel/const.h>

#define BIT(nr)		(UL(1) << (nr))
#define BIT64(nr)	(ULL(1) << (nr))

#endif
