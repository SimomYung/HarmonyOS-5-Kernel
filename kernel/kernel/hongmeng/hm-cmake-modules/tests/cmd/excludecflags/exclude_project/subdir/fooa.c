/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: fooa.c
* Author: Huawei OS Kernel Lab
* Create: Thu Jan 06 01:52:38 2022
*/

#include <stdio.h>

int fooa(void);
int fooa(void)
{
	printf("this is fooa\n");
#ifdef HAVE_FOO
	printf("DEFINED HAVE_FOO\n");
#else
	printf("NOT DEFINE HAVE_FOO\n");
#endif
#ifdef HAVE_FOOB
	printf("DEFINED HAVE_FOOB\n");
#else
	printf("NOT DEFINE HAVE_FOOB\n");
#endif
	return 0;
}
