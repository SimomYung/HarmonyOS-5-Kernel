/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Kasan hook function header
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 15:11:46 2022
 */
#ifndef KASAN_HOOK_H
#define KASAN_HOOK_H

#include <stddef.h>

#define DEFINE_ASAN_LOADSTORE_PTR(size)				\
	void (*__asan_load##size)(unsigned long bgn);		\
	void (*__asan_load##size##_noabort)(unsigned long bgn);	\
	void (*__asan_store##size)(unsigned long bgn);		\
	void (*__asan_store##size##_noabort)(unsigned long bgn);

struct kasan_global;

struct asan_callback {
	DEFINE_ASAN_LOADSTORE_PTR(1)
	DEFINE_ASAN_LOADSTORE_PTR(2)
	DEFINE_ASAN_LOADSTORE_PTR(4)
	DEFINE_ASAN_LOADSTORE_PTR(8)
	DEFINE_ASAN_LOADSTORE_PTR(16)
	void (*__asan_loadN)(unsigned long bgn, size_t size);
	void (*__asan_loadN_noabort)(unsigned long bgn, size_t size);
	void (*__asan_storeN)(unsigned long bgn, size_t size);
	void (*__asan_storeN_noabort)(unsigned long bgn, size_t size);
	void (*__asan_register_globals)(struct kasan_global *glbls, size_t size);
	void (*__asan_unregister_globals)(struct kasan_global *glbls, size_t size);
	void (*__asan_handle_no_return)(void);
};


int set_asan_callback(const struct asan_callback *new_ac);

#endif
