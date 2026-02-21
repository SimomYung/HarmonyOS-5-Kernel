/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __memset_chk
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *__memset_chk(void *dst, int c, size_t len, size_t dstlen)
{
	if (dstlen < len) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}
	return memset (dst, c, len);
}
