/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 05 20:07:01 2019
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hm_unittest.h"

extern int strcmp_test(void);
extern int memcmp_test(void);

int main(void)
{
	printf("HMUT musl-string test start...\n");
	int ret = 0;
	ret = strcmp_test();
	ret |= memcmp_test();

	return ret;
}
