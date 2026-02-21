/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Ctype
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 13:51:50 2018
 */
#ifndef KLIBS_CTYPES_H
#define KLIBS_CTYPES_H

static inline int isupper(int c)
{
	return (int)'A' <= c && c <= (int)'Z' ? 1 : 0;
}

static inline int tolower(int c)
{
	int lc = c;
	if (isupper(c) != 0) {
		lc = c - ((int)'A' - (int)'a');
	}
	return lc;
}

static inline int
isdigit(int c)
{
	return (int)'0' <= c && c <= (int)'9' ? 1 : 0;
}

static inline __attribute__((always_inline))
int isxdigit(int c)
{
	int lc;
	int ret;

	if (isdigit(c) != 0) {
		ret = 1;
	} else {
		lc = tolower(c);
		ret = (int)'a' <= lc && lc <= (int)'f' ? 1 : 0;
	}
	return ret;
}

static inline int
isodigit(int c)
{
	return (int)'0' <= c && c <= (int)'7' ? 1 : 0;
}

#endif
