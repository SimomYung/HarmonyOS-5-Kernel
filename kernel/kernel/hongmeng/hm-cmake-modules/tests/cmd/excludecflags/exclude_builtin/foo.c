/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: foo.c
* Author: Huawei OS Kernel Lab
* Create: Fri Oct 22 01:24:11 2021
*/

#include <stdio.h>
#include <foo.h>

int foo(void)
{
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO\n");
#else
	printf("SUCCESS NO_FOO\n");
#endif
#ifdef HAVE_BUILTIN
	printf("SUCCESS HAVE_BUILTIN\n");
#endif
	return 0;
}
