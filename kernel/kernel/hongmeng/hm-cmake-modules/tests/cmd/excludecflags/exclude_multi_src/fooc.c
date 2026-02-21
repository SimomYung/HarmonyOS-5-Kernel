/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: fooc.c for testcase exclude_multi_src
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 25 22:50:26 2022
 */

#include <stdio.h>
#include "fooc.h"

int fooc(void)
{
#ifdef HAVE_FOO
	printf("fooc HAVE_FOO\n");
#else
	printf("fooc NO_FOO\n");
#endif
	return 0;
}
