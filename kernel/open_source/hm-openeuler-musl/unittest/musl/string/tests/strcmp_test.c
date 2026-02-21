/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 29 20:44:44 2019
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hm_unittest.h"

#include <string.h>

static void null_test_success(void **state)
{
	(void) state;
}

static void UT_string_strcmp(void **state)
{
	int ret;
	char s1[30]="Hello, Programmers!";
	char s2[40]="Hello, Programmers!";

	ret = strcmp(s1, s2);
	assert_true(ret == 0);
}

const struct HMUnitTest tests[] = {
	cmocka_unit_test(null_test_success),
	cmocka_unit_test(UT_string_strcmp),
};

int strcmp_test(void);
int strcmp_test(void)
{
	return hm_run_group_tests(tests, NULL, NULL);
}
