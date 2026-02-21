/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: main.c for testcase exclude_src_regex
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 24 21:31:59 2022
 */

#include <stdio.h>

int main(void)
{
	printf("This is exclude_src_regex test\n");
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO");
#else
	printf("SUCCESS NO_FOO");
#endif
	return 0;
}
