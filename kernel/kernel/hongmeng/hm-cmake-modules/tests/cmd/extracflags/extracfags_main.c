/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Author: Huawei OS Kernel Lab
* Create: Thu Oct 21 09:35:18 2021
*/

#include <stdio.h>

int main(void)
{
#ifdef HAVE_FLAGS_FOO
	printf("SUCCESS HAVE_FLAGS_FOO\n");
#endif
	printf("This is extracflags test\n");
	return 0;
}
