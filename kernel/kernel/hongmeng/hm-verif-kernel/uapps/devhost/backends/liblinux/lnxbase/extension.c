/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Init func for dh-liblinux extension lib
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 17 02:24:01 2020
 */
#include "internal.h"

#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <devhost/log.h>
#include <devhost/backend.h>
#include <lnxbase/lnxbase.h>
#include <hongmeng/errno.h>

static struct extension_node *__find_extensionlib(const struct lnxbase_ctx *ctx,
						  const char *extralib)
{
	struct extension_node *pos = NULL;
	dlist_for_each_entry(pos, &ctx->extensions, struct extension_node, node) {
		if (strcmp(extralib, pos->extralib_name) == 0) {
			return pos;
		}
	}
	return NULL;
}

static struct extension_node *__new_one_extensionlib(struct lnxbase_ctx *ctx, const char *extralib)
{
	struct extension_node *new_node = NULL;

	new_node = __find_extensionlib(ctx, extralib);
	if (new_node != NULL) {
		if (new_node->extralib_sohandle == NULL) {
			return new_node;
		}
		return NULL; /* loaded */
	}

	new_node = (struct extension_node *)malloc(sizeof(struct extension_node));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->extralib_sohandle = NULL;
	new_node->extralib_name = strdup(extralib);
	if (new_node->extralib_name == NULL) {
		free(new_node);
		return NULL;
	}
	dlist_insert_tail(&ctx->extensions, &new_node->node);
	return new_node;
}

static int __load_one_extensionlib(struct extension_node *lib, struct lnxbase_ctx *ctx)
{
	void *dlhandler = NULL;
	dh_extension_init_fn_t init_func= NULL;
	int ret;

	dlhandler = dlopen(lib->extralib_name, RTLD_NOW);
	if (dlhandler == NULL) {
		hm_info("%s\n", dlerror());
		return E_HM_INVAL;
	}

	init_func = (dh_extension_init_fn_t)(uintptr_t)dlsym(dlhandler, DH_EXTRALIB_INIT);
	if (init_func == NULL) {
		(void)dlclose(dlhandler);
		hm_error("invalid extension lib\n");
		return E_HM_INVAL;
	}

	ret = init_func(ctx);
	if (ret < 0) {
		(void)dlclose(dlhandler);
		hm_error("init dso extension failed, %d\n", ret);
		return posix2hmerrno(-ret);
	}
	lib->extralib_sohandle = dlhandler;
	hm_debug("load dso extension %s success\n", lib->extralib_name);
	return E_HM_OK;
}

int lnxbase_load_extension(struct lnxbase_ctx *ctx, const char *ext_so)
{
	struct extension_node *pos = NULL;
	int ret;

	pos = __new_one_extensionlib(ctx, ext_so);
	if (pos == NULL) {
		hm_warn("plug in extension failed\n");
		return E_HM_NOMEM;
	}

	ret = __load_one_extensionlib(pos, ctx);
	if (ret < 0) {
		hm_info("loader extension %s, ret = %s\n", pos->extralib_name, hmstrerror(ret));
		return ret;
	}

	return E_HM_OK;
}
