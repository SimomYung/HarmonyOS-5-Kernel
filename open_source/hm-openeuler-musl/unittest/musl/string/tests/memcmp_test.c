/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 05 20:22:46 2019
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hm_unittest.h"

#include <string.h>



static void UT_string_memcmp(void **state)
{
	int ret;
	char s1[30]="Hello, Programmers!";
	char s2[40]="Hello, programmers!";
	char s3[30]="Hello, Programmers!";

	ret = memcmp(s1, s2, 18);
	assert_true(ret < 0);

	ret = memcmp(s1, s3, 18);
	assert_true(ret == 0);
}


const struct HMUnitTest MEMCMP_MOCKTEST[] = {
	cmocka_unit_test(UT_string_memcmp),
};

int memcmp_test(void);
int memcmp_test(void)
{
	return hm_run_group_tests(MEMCMP_MOCKTEST, NULL, NULL);
}
