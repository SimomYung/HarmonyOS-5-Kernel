/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Author: Huawei OS Kernel Lab
* Create: Thu Oct 25 10:35:20 2021
*/

#include <stdio.h>
int main(void)
{
	printf("This is multi_cflags test\n");
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO\n");
#endif
#ifdef HAVE_MUTI
	printf("SUCCESS HAVE_MUTI\n");
#endif
	return 0;
}
