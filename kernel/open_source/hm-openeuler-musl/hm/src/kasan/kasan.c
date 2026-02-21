/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Kasan hook function
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 15:10:47 2022
 */
#include <stddef.h>
#include "kasanhook.h"

static void dummy0(void)
{
}

static void dummy1(unsigned long bgn)
{
	(void)bgn;
}

static void dummy2(unsigned long bgn, size_t size)
{
	(void)bgn;
	(void)size;
}

static void dummy3(struct kasan_global *glbls, size_t size)
{
	(void)glbls;
	(void)size;
}

#define ASAN_LOADSTORE_ASSIGN(size)		\
	.__asan_load##size = dummy1,		\
	.__asan_load##size##_noabort = dummy1,	\
	.__asan_store##size = dummy1,		\
	.__asan_store##size##_noabort = dummy1,

static struct asan_callback g_asan_callback = {
	ASAN_LOADSTORE_ASSIGN(1)
	ASAN_LOADSTORE_ASSIGN(2)
	ASAN_LOADSTORE_ASSIGN(4)
	ASAN_LOADSTORE_ASSIGN(8)
	ASAN_LOADSTORE_ASSIGN(16)
	.__asan_loadN = dummy2,
	.__asan_loadN_noabort = dummy2,
	.__asan_storeN = dummy2,
	.__asan_storeN_noabort = dummy2,
	.__asan_register_globals = dummy3,
	.__asan_unregister_globals = dummy3,
	.__asan_handle_no_return = dummy0
};


#define DEFINE_ASAN_LOADSTORE(size)				\
	void __asan_load##size(unsigned long bgn)		\
	{							\
		g_asan_callback.__asan_load##size(bgn);			\
	}							\
	void __asan_load##size##_noabort(unsigned long bgn)	\
	{							\
		g_asan_callback.__asan_load##size##_noabort(bgn);		\
	}							\
	void __asan_store##size(unsigned long bgn)		\
	{							\
		g_asan_callback.__asan_store##size(bgn);			\
	}							\
	void __asan_store##size##_noabort(unsigned long bgn)	\
	{							\
		g_asan_callback.__asan_store##size##_noabort(bgn);		\
	}

DEFINE_ASAN_LOADSTORE(1)
DEFINE_ASAN_LOADSTORE(2)
DEFINE_ASAN_LOADSTORE(4)
DEFINE_ASAN_LOADSTORE(8)
DEFINE_ASAN_LOADSTORE(16)

void __asan_loadN(unsigned long bgn, size_t size)
{
	g_asan_callback.__asan_loadN(bgn, size);
}

void __asan_loadN_noabort(unsigned long bgn, size_t size)
{
	g_asan_callback.__asan_loadN_noabort(bgn, size);
}

void __asan_storeN(unsigned long bgn, size_t size)
{
	g_asan_callback.__asan_storeN(bgn, size);
}

void __asan_storeN_noabort(unsigned long bgn, size_t size)\
{
	g_asan_callback.__asan_storeN_noabort(bgn, size);
}

void __asan_register_globals(struct kasan_global *glbls, size_t size)
{
	g_asan_callback.__asan_register_globals(glbls, size);
}

void __asan_unregister_globals(struct kasan_global *glbls, size_t size)

{
	g_asan_callback.__asan_unregister_globals(glbls, size);
}

void __asan_handle_no_return(void)
{
	g_asan_callback.__asan_handle_no_return();
}

int set_asan_callback(const struct asan_callback *new_ac)
{
	if (new_ac == NULL) {
		return -1;
	}
	/* Avoid using memcpy (gcc optimization) */
	unsigned int i = 0;
	for (; i < sizeof(struct asan_callback); i++) {
		*((char *)&g_asan_callback + i) = *((char *)new_ac + i);
	}
	return 0;
}
