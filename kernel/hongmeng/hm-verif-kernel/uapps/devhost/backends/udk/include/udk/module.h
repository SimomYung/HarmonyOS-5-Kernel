/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Driver management interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 08 19:36:14 2019
 */
#ifndef _UDK_MODULE_H
#define _UDK_MODULE_H

#include <stddef.h>
#include <stdbool.h>
#include <lib/dlist.h>
#include <udk/stdint.h>
#include <udk/types.h>
#include <udk/compiler.h>
#include <udk/init.h>

#define udk_module_init(initfn)	\
	int udk_init_module(void) __attribute__((weak, alias(#initfn))); \
	int udk_fusion_##initfn(void); \
	int udk_fusion_##initfn(void) { return initfn(); } \

#define udk_module_exit(exitfn)	\
	int udk_exit_module(void) __attribute__((weak, alias(#exitfn)));

#define UDK_MODULE_STR_MAX_LEN 64
#define udk_module_compat(...) \
	char __attribute__((weak, section(".udk_module_compat"))) udk_module_compat[][UDK_MODULE_STR_MAX_LEN] = {__VA_ARGS__};

#define udk_module_deps_array(...) \
	char __attribute__((weak, section(".udk_module_deps"))) udk_module_deps[][UDK_MODULE_STR_MAX_LEN] = {__VA_ARGS__};

#define UDK_MODULE_DEPS_LEN \
	unsigned int __attribute__((weak)) udk_module_len = sizeof(udk_module_deps) / sizeof(udk_module_deps[0]);

#define udk_module_deps(...) \
	udk_module_deps_array(__VA_ARGS__); UDK_MODULE_DEPS_LEN;

enum udk_module_param_type {
	UDK_PARAM_INT,
	UDK_PARAM_UINT,
	UDK_PARAM_LONG,
	UDK_PARAM_ULONG,
	UDK_PARAM_CHARP,
	UDK_PARAM_INVALID,
};

struct udk_module_param_node {
	const char *name;
	enum udk_module_param_type type;
	void *ptr;
	bool inited;
	struct dlist_node list;
};

void udk_module_param_register(struct udk_module_param_node *node);
enum udk_module_param_type udk_module_param_str_to_type(const char *type);

#define udk_module_param_header(varname, vartype) \
	__attribute__((constructor)) static void __register_param_##varname##_##vartype(void)

#define udk_module_param_body(varname, vartype)	\
	do {													\
		static struct udk_module_param_node __module_param_##varname##_##vartype = {			\
			.name = #varname, .ptr = (void *)&(varname), .inited = false, .list = { NULL, NULL },	\
		};												\
		__module_param_##varname##_##vartype.type = udk_module_param_str_to_type(#vartype);		\
		dlist_insert_tail(&g_udk_param_list, &__module_param_##varname##_##vartype.list);		\
	} while (0)

extern struct dlist_node g_udk_param_list;
#define udk_module_param(varname, vartype)								\
	udk_module_param_header(varname, vartype) { udk_module_param_body(varname, vartype); }

#define DEFINE_UDK_GET_PARAM_LIST_HEADER					\
	__attribute__((weak)) struct dlist_node *udk_get_param_list(void)

#define DEFINE_UDK_GET_PARAM_LIST \
	DEFINE_UDK_GET_PARAM_LIST_HEADER; DEFINE_UDK_GET_PARAM_LIST_HEADER { return &g_udk_param_list; }

#define DEFINE_UDK_MODULE_PARAM_LIST \
	__attribute__((visibility("hidden"))) DLIST_HEAD(g_udk_param_list); DEFINE_UDK_GET_PARAM_LIST;

struct udk_module;
#define DEFINE_UDK_SET_THIS_MODULE_HEADER					\
	__attribute__((weak)) void udk_set_this_module(struct udk_module *this_module)

#define DEFINE_UDK_SET_THIS_MODULE_BODY		\
	do {						\
		g_udk_this_module = this_module;	\
	} while (0)

#define DEFINE_UDK_SET_THIS_MODULE \
	DEFINE_UDK_SET_THIS_MODULE_HEADER; DEFINE_UDK_SET_THIS_MODULE_HEADER { DEFINE_UDK_SET_THIS_MODULE_BODY; }

#define DEFINE_UDK_THIS_MODULE	\
	__attribute__((visibility("hidden"))) struct udk_module *g_udk_this_module; DEFINE_UDK_SET_THIS_MODULE

extern struct udk_module *g_udk_this_module;
#define UDK_THIS_MODULE ((void **)&g_udk_this_module)

int udk_init_module(void);
int udk_exit_module(void);

int udk_request_module(const char *name);

struct udk_module;
int udk_module_open(struct udk_module *module);
void udk_module_close(struct udk_module *module);
int udk_module_get(struct udk_module *module);
void udk_module_put(struct udk_module *module);

#endif /* _UDK_MODULE_H */
