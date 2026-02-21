/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK internal initialization routines
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 06 16:11:39 2019
 */

#ifndef __UDK_INTERNAL_INIT_H__
#define __UDK_INTERNAL_INIT_H__

#include <udk/init.h>
#include <udk/compiler.h>
#include <udk/lib/dlist.h>

/* For udk components other than drivers */
typedef struct {
	int (*func)(void);
	const char *name;
	struct dlist_node list;
} udk_initcall_t;
void udk_initcall_register(udk_initcall_t *);
void udk_post_initcall_register(udk_initcall_t *);

#define udk_init_call_body(fn, callname)                               \
	do {                                                           \
		static udk_initcall_t __initcall_##fn = {              \
			.func = (fn),                                  \
			.name = #fn,                                   \
			.list = { NULL, NULL },                        \
		};                                                     \
		udk_##callname##_register(&__initcall_##fn);           \
	} while (0)

#define udk_init_call(fn)                                              \
static UDK_CONSTRUCTOR void __register_initcall_##fn(void)             \
{                                                                      \
	udk_init_call_body(fn, initcall);                              \
}

#define udk_post_init_call(fn)                                         \
static UDK_CONSTRUCTOR void __register_post_initcall_##fn(void)        \
{                                                                      \
	udk_init_call_body(fn, post_initcall);                         \
}

#ifdef UDK_FUSION
#undef udk_init_call
#define udk_init_call(fn)                                              \
static UDK_CONSTRUCTOR void __register_initcall_##fn(void)             \
{                                                                      \
	udk_init_call_body(fn, initcall);                              \
}                                                                      \
int udk_fusion_##fn(void);                                             \
int udk_fusion_##fn(void) { return fn(); }                             \

#endif

#endif /* __UDK_INTERNAL_INIT_H__ */
