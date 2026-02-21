/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Kasan wrapper functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 17 11:43:29 2024
 */
#include "pac_wrapper.h"
#include <lib/pac.h>

/* libhmcrypt depends on the openssl library, but the library pass
 * function pointer type parameters to the following functions with modifier 0.
 * This file is used to fix the unsigned issue.
 */

void __wrap_qsort (void *data, size_t num, size_t size, int (*cmp)(const void *, const void *))
{
	int (*cmp_func)(const void *, const void *) = cmp;

	PAC_FECFI_XPAC_SIGN(cmp_func, int (*)(const void *, const void *));
	__real_qsort(data, num, size, cmp_func);

	return;
}

int __wrap_pthread_once(pthread_once_t *once, void (*init)(void))
{
	int ret;

	void (*fn)(void) = init;
	PAC_FECFI_XPAC_SIGN(fn, void(*)(void));
	ret = __real_pthread_once(once, fn);

	return ret;
}

int __wrap_pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
	int ret;

	void (*fn)(void*) = destructor;
	PAC_FECFI_XPAC_SIGN(fn, void(*)(void*));
	ret = __real_pthread_key_create(key, fn);

	return ret;
}

int __wrap_atexit(void (*func)(void))
{
	void (*func_func)(void) = func;

	PAC_FECFI_XPAC_SIGN(func_func, void(*)(void));
	return __real_atexit(func_func);
}
