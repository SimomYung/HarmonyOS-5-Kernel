/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Author: Huawei OS Kernel Lab
* Create: Thu Oct 25 08:35:20 2021
*/

#include <stdio.h>
int main(void)
{
	printf("This is cflags target dir test\n");
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO\n");
#endif
	return 0;
}
