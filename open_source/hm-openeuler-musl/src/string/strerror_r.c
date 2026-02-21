/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The implements for strerror_r
 * Author:  Huawei RTOS
 * Create: 2021-09-13 11:05:50
 */
#include <string.h>
#include <errno.h>

int __xpg_strerror_r(int err, char *buf, size_t buflen)
{
	if (__errno_valid(err) == 0)
		return EINVAL;

	char *msg = strerror(err);
	size_t l = strlen(msg);
	if (l >= buflen) {
		if (buflen) {
			memcpy(buf, msg, buflen-1);
			buf[buflen-1] = 0;
		}
		return ERANGE;
	}
	memcpy(buf, msg, l+1);
	return 0;
}

weak_alias(__xpg_strerror_r, strerror_r);

char *__gnu_strerror_r (int err, char *buf, size_t buflen)
{
	char *msg = strerror(err);
	if (buf != NULL) {
		size_t l = strlen(msg);
		l = (buflen > l + 1) ? l : buflen - 1;
		memcpy(buf, msg, l);
		buf[l] = '\0';
		return buf;
	}
	return msg;
}
