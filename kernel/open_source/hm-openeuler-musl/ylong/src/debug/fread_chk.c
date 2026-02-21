/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: __fread_chk
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stdio_impl.h"

size_t __fread_chk(void *__restrict ptr, size_t ptrlen, size_t size, size_t n, FILE *__restrict stream)
{
	size_t total_bytes = size * n;
	if (n == 0 || size == 0 || size > (SIZE_MAX / n) || total_bytes > ptrlen) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}

	return fread(ptr, size, n, stream);
}
