/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: fooa.c for testcase exclude_multi_src
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 25 12:53:22 2022
 */

#include <stdio.h>
#include "fooa.h"

int fooa(void)
{
#ifdef HAVE_FOO
	printf("fooa HAVE_FOO\n");
#else
	printf("fooa NO_FOO\n");
#endif
	return 0;
}
