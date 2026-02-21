/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __memcpy_chk
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *__memcpy_chk(void *dst, const void *src, size_t srclen, size_t dstlen)
{
	if (dstlen < srclen) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}

    return memcpy(dst, src, srclen);
}
