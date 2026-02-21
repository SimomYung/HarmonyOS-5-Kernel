/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Author: Huawei OS Kernel Lab
* Create: Thu Oct 21 09:35:20 2021
*/

#include <stdio.h>
#include <foo.h>

int main(void)
{
	printf("This is group cflags tests\n");
#if defined(HAVE_TARGET) && defined(HAVE_TARGET_MUTI)
	printf("SUCCESS HAVE_TARGET\n");
#endif
#if defined(HAVE_DIR) && defined(HAVE_DIR_MUTI)
	printf("SUCCESS HAVE_DIR\n");
#endif
	foo();
	return 0;
}
