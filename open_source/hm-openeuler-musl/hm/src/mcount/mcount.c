/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wednesday Aug 28, 2019 17:14:56 CST
 */

#include <mcount.h>
#include <stdio.h>

MCOUNT_INTERNAL(frompc, selfpc)
{
	/* just a stub for gcc -pg option, do nothing */
}

MCOUNT_ENTRY
