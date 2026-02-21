/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Init func for udk extension lib
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 27 15:24:01 2023
 */
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <hongmeng/errno.h>
#include <internal/init.h>
#include <devhost/backend.h>
#include <udk/extension.h>
#include <udk/module.h>
#include <udk/log.h>

static const struct option long_options[] = {
	{"extensions", required_argument, NULL, (int)'e'},
	{NULL, no_argument, NULL, 0}
};

static const char* udk_extensions_args(void)
{
	const struct devhost_backend_args *args = devhost_get_backend_args();
	int opt;

	if (args != NULL) {
		while ((opt = getopt_long(args->argc, args->argv, "", long_options, NULL)) != -1) {
			if (opt == 'e') {
				return optarg;
			}
		}
	}

	return NULL;
}

static int udk_load_extension(const char* extension)
{
	void *dlhandler = NULL;
	dh_udk_extension_init_fn init_func= NULL;
	int ret;

	dlhandler = udk_dlopen(extension, RTLD_NOW);
	if (dlhandler == NULL) {
		udk_warn("udk dlopen %s failed: %s\n", extension, udk_dlerror());
		return E_HM_INVAL;
	}

	init_func = (dh_udk_extension_init_fn)(uintptr_t)udk_dlsym(dlhandler, DH_UDK_EXTRALIB_INIT);
	if (init_func == NULL) {
		(void)udk_dlclose(dlhandler);
		udk_warn("udk invalid extension lib %s\n", extension);
		return E_HM_INVAL;
	}

	ret = init_func();
	if (ret < 0) {
		(void)udk_dlclose(dlhandler);
		udk_warn("udk init dso extension %s failed, %d\n", extension, ret);
		return posix2hmerrno(-ret);
	}

	udk_info("udk load extension %s success\n", extension);

	return E_HM_OK;
}

static int udk_load_extensions(const char* extensions)
{
	char* duplicate;
	char* pos;
	char *ext;
	int ret;

	duplicate = strdup(extensions);
	if (duplicate == NULL) {
		return E_HM_NOMEM;
	}

	pos = duplicate;
	while ((ext = strsep(&pos, ":")) != NULL) {
		ret = udk_load_extension(ext);
		if (ret < 0) {
			udk_warn("udk load extension %s failed, ret = %d\n", ext, ret);
		}
	}

	free(duplicate);
	return E_HM_OK;
}

static int udk_extensions_init(void)
{
	const char* extensions;
	int ret;

	extensions = udk_extensions_args();
	if (extensions == NULL) {
		return E_HM_OK;
	}

	ret = udk_load_extensions(extensions);
	if (ret != 0) {
		udk_error("udk load extensions(%s) failed, ret = %d\n", extensions, ret);
		return ret;
	}

	return E_HM_OK;
}

udk_post_init_call(udk_extensions_init);
