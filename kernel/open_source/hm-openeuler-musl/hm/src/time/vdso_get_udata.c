/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implement vdso_get_udata
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 2 15:49:23 2021
 */
#include <errno.h>
#include <stdint.h>
#include <stddef.h>

#include "atomic.h"
#include "syscall.h"

void *vdso_get_udata(void)
{
#ifdef VDSO_GET_DATA_SYM
	void *r = NULL;
	static void *(*func)(void) = (void *(*)(void))NULL;

	if (func == NULL) {
		void *p = __vdsosym(VDSO_GET_DATA_VER, VDSO_GET_DATA_SYM);
		if (p == NULL) {
			errno = ENOSYS;
			return NULL;
		}
		(void)a_cas_p(&func, NULL, p);
	}

	r = func();
	if (r == NULL) {
		errno = EINVAL;
	}

	return r;
#else
	errno = ENOSYS;
	return NULL;
#endif
}
