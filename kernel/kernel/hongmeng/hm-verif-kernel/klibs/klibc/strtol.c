/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Strtol
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 13:51:50 2018
 */
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#define HEX	16
#define DEC	10
#define OCT	8
#define HEX_FLAG_0_INDEX	0
#define HEX_FLAG_X_INDEX	1
#define HEX_DIGIT_INDEX		2
#define OCT_FLAG_0_INTEX	0
#define OCT_DIGIT_INDEX		1

static const char *
parse_prefix(const char *s, int *base)
{
	const char *p_ret = s;

	if (*base == 0) {
		if (s[HEX_FLAG_0_INDEX] == '0' &&
		    tolower((int)s[HEX_FLAG_X_INDEX]) == (int)'x' &&
		    isxdigit((int)s[HEX_DIGIT_INDEX]) != 0) {
			*base = HEX;
			p_ret = s + HEX_DIGIT_INDEX;
		} else if (s[OCT_FLAG_0_INTEX] == '0' &&
			   isodigit((int)s[OCT_DIGIT_INDEX]) != 0) {
			*base = OCT;
			p_ret = s + OCT_DIGIT_INDEX;
		} else {
			*base = DEC;
			p_ret = s;
		}
	} else {
		if (*base == HEX && s[HEX_FLAG_0_INDEX] == '0' &&
		    tolower((int)s[HEX_FLAG_X_INDEX]) == (int)'x' &&
		    isxdigit((int)s[HEX_DIGIT_INDEX]) != 0) {
			p_ret = s + HEX_DIGIT_INDEX;
		} else if (*base == OCT && s[OCT_FLAG_0_INTEX] == '0' &&
			   isodigit((int)s[OCT_DIGIT_INDEX]) != 0) {
			p_ret = s + OCT_DIGIT_INDEX;
		} else {
			/* do nothing */
		}
	}

	return p_ret;
}

static const char *
parse_digits(const char *src, int base, unsigned long long *p)
{
	unsigned long long res = 0ULL;
	const char *s = src;

	while (1) {
		int v;

		if (isdigit((int)s[0]) != 0) {
			v = (int)s[0] - (int)'0';
		} else if (isxdigit((int)s[0]) != 0) {
			/* number 10 here is used to translate [a-f] to [10-15] */
			v = (tolower((int)s[0]) - (int)'a') + 10;
		} else {
			break;
		}

		if (v >= base) {
			break;
		}

		/* overflow? is your own fault */
		res = res * (unsigned int)base + (unsigned int)v;
		s++;
	}

	*p = res;
	return s;
}

static inline char *
parse_int(const char *src, int b, unsigned long long *p, int *neg)
{
	const char *s = src;
	int negative = (s[0] == '-' ? (s++, 1) : 0);
	int base = b;

	s = parse_prefix(s, &base);
	s = parse_digits(s, base, p);
	if (neg != NULL) {
		*neg = negative;
	}
	return (char *)s;
}

long int
strtol(const char *s, char **t, int b)
{
	int neg = 0;
	unsigned long long resll = 0ULL;
	char *tail = parse_int(s, b, &resll, &neg);
	long int res = (long int)(resll);

	if (res < 0) {
		res = -res;
	}
	if (neg != 0) {
		res = -res;
	}
	if (t != NULL) {
		*t = (char *)tail;
	}
	return res;
}

unsigned long int
strtoul(const char *s, char **t, int b)
{
	unsigned long long resll = 0ULL;
	char *tail = parse_int(s, b, &resll, NULL);
	unsigned long int res = (unsigned long int)(resll);

	if (t != NULL) {
		*t = tail;
	}
	return res;
}
