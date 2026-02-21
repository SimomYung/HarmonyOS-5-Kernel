/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __strcat_chk
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *__strcat_chk(char *dst, const char *src, size_t dstlen)
{
	/* buffer length judge, take '\0' into consideration. */
	if (strlen(dst) + strlen(src) >= dstlen) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}

	return strcat(dst, src);
}
