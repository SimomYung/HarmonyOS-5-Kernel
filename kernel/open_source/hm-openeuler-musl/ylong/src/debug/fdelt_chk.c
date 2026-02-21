/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __fdelt_chk
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

/* Same with glibc. Helper functions to issue warnings and errors when needed.  */
long int __fdelt_chk(long int d)
{
	if (d < 0 || d >= FD_SETSIZE) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}

	return (d / (8 * (int)sizeof(long int))); /* Same with glibc : __NFDBITS */
}
