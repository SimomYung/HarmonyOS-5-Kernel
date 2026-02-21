/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of debug_chck
 * Author: RTOS
 * Create: Fri Jun 18 14:28:57 2021
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *__memset_chk (void * dest, int c, size_t n, size_t dst_len)
{
	if (dst_len < n) {
		printf("buffer overflow detected\n");
		abort();
	}

	return memset(dest, c, n);
}
