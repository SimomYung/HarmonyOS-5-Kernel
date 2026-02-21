/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: foob.c for testcase exclude_multi_src
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 25 12:56:56 2022
 */

#include <stdio.h>
#include "foob.h"

int foob(void)
{
#ifdef HAVE_FOO
	printf("foob HAVE_FOO\n");
#else
	printf("foob NO_FOO\n");
#endif
	return 0;
}
