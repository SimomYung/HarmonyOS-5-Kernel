/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: foo.c
* Author: Huawei OS Kernel Lab
* Create: Tue Oct 26 03:05:36 2021
*/

#include <stdio.h>
#include <foo.h>
int foo(void)
{
	printf("This is foo\n");
#if defined(HAVE_SRC) && defined(HAVE_SRC_MUTI)
	printf("SUCCESS HAVE_SRC\n");
#endif
	return 0;
}
