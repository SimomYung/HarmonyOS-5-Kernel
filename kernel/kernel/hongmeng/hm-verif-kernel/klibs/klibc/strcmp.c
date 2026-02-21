/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strcmp
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <string.h>
#endif

#ifndef __KERNEL__
int strcmp(const char *s1, const char *s2);
#endif

/*
 * strcmp behaves like the standard version.
 * it compares s1 with s2 lexicographically.
 *
 * describe: if there exists an integer n>=0,
 * 	if forall i>=0 and i<n it has s1[i] == s2[i] and s1[n-1]=0, strcmp returns 0;
 *      if forall i>=0 and i<n it has s1[i] == s2[i] and s1[n] > s2[n], strcmp returns a positive value;
 *	if forall i>=0 and i<n it has s1[i] == s2[i] and s1[n] < s2[n], strcmp returns a negative value.
 */
int strcmp(const char *s1, const char *s2)
{
	const char *c1 = (const char *)s1;
	const char *c2 = (const char *)s2;
	int d = 0;

	while (1) {
		d = (int)*c1 - (int)*c2;
		if (d != 0 || *c1 == '\0') {
			break;
		}

		c1++;
		c2++;
	}
	return d;
}
