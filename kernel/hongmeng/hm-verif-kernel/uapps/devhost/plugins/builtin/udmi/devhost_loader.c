/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Driver module loader support
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 09 17:05:15 2019
 */
#include "devhost.h"
#include "devhost_loader.h"

#include <unistd.h>
#include <lib/dlist.h>
#include <libalgo/idr.h>
#include <hongmeng/errno.h>
#include <devhost/backend.h>
#include <libsysif/devmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include "devhost_api.h"

static DEFINE_IDR(g_loaders);

PUBLIC_SYMBOL
int devhost_register_loader(struct devhost_loader *loader)
{
	int ret;

	if (loader == NULL) {
		return E_HM_INVAL;
	}

	ret = idr_alloc(&g_loaders, loader);
	if (ret < 0) {
		return ret;
	}

	return E_HM_OK;
}

struct devhost_module {
	struct uobject obj;
	char *libname;
	struct devhost_loader *loader;
	struct raw_mutex mutex;
};
static struct uobject_set g_devhost_module;

static bool devhost_module_uobject_cmp(const struct uobject *obj, void *arg)
{
	struct devhost_module *module = container_of(obj, struct devhost_module, obj);
	char *libname = (char *)arg;
	int ret = E_HM_OK;

	if (libname == NULL) {
		return false;
	}

	raw_mutex_lock(&module->mutex);
	ret = strcmp(module->libname, libname);
	raw_mutex_unlock(&module->mutex);

	return (ret == 0);
}

void devhost_loader_init(void)
{
	uobject_set_init(&g_devhost_module, devhost_module_uobject_cmp);
}

static void devhost_module_uobject_release(struct uobject *obj)
{
	struct devhost_module *module = container_of(obj, struct devhost_module, obj);
	free(module);
}

static struct devhost_module *alloc_devhost_module(const char *libname)
{
	struct devhost_module *module = NULL;
	size_t sz;
	int ret = E_HM_OK;

	sz = sizeof(struct devhost_module) + strlen(libname) + 1;
	module = (struct devhost_module *)malloc(sz);
	if (module == NULL) {
		return NULL;
	}
	module->libname = (char *)module + sizeof(struct devhost_module);
	ret = strncpy_s(module->libname, sz - sizeof(struct devhost_module), libname, strlen(libname));
	if (ret != 0) {
		free(module);
		return NULL;
	}
	module->loader = NULL;
	raw_mutex_init(&module->mutex);
	uobject_init(&module->obj, 1, devhost_module_uobject_release);
	return module;
}

struct loader_args {
	/* input */
	const char *libname;
	const void *data;
	size_t size;
	/* output */
	struct devhost_loader *loader;
};

static int __loader_cb(unsigned int id, void *ptr, void *data)
{
	struct devhost_loader *loader = (struct devhost_loader *)ptr;
	struct loader_args *args = (struct loader_args *)data;
	UNUSED(id);
	if (loader->is_compatible(loader,
				  args->libname, args->data, args->size) != 0) {
		args->loader = loader;
		return 1;
	}
	return E_HM_OK;
}

static
struct devhost_loader *devhost_find_loader(const char *libname, const void *data, size_t size)
{
	struct loader_args args = {
		.libname = libname,
		.data = data,
		.size = size,
		.loader = NULL,
	};
	(void)idr_for_each(&g_loaders, __loader_cb, &args);
	return args.loader;
}

int devhost_load_module(const char *libname, const void *data, size_t size, char *args)
{
	struct devhost_loader *loader = NULL;
	struct devhost_module *module = NULL;
	int ret = E_HM_OK;

	if (libname == NULL || data == NULL) {
		return E_HM_INVAL;
	}

	module = alloc_devhost_module(libname);
	if (module == NULL) {
		return E_HM_NOMEM;
	}
	/* insertion fails when driver is already loaded */
	ret = uobject_set_insert(&g_devhost_module, &module->obj, module->libname);
	if (ret < 0) {
		if (ret == E_HM_OBJEXIST) {
			hm_warn("Driver is already loaded, libname=%s\n", libname);
		}
		ret = E_HM_POSIX_EXIST;
		goto insert_err;
	}
	loader = devhost_find_loader(libname, data, size);
	if (loader == NULL) {
		ret = E_HM_INVAL;
		goto load_err;
	}
	ret = loader->load_module(loader, libname, data, size, args);
	if (ret < 0) {
		goto load_err;
	}
	raw_mutex_lock(&module->mutex);
	/* allow unload after this line */
	module->loader = loader;
	raw_mutex_unlock(&module->mutex);

	(void)uobject_put(&module->obj);
	return 0;

load_err:
	/* allow load the same module after this line */
	(void)uobject_set_delete(&g_devhost_module, &module->obj);
insert_err:
	(void)uobject_release(&module->obj);
	return ret;
}

int devhost_unload_module(const char *libname)
{
	struct uobject *obj = NULL;
	struct devhost_module *module = NULL;
	struct devhost_loader *loader = NULL;
	int ret = E_HM_OK;

	if (libname == NULL) {
		return E_HM_INVAL;
	}
	obj = uobject_set_acquire(&g_devhost_module, (void *)libname);
	if (obj == NULL) {
		return E_HM_INVAL;
	}
	module = container_of(obj, struct devhost_module, obj);
	raw_mutex_lock(&module->mutex);
	/*
	 * if module->loader is NULL, then the module is in the process
	 * of initialization, or it's already cleaned by devhost_load_module,
	 * or it's being cleaned by another devhost_unload_module.
	 */
	if (module->loader == NULL) {
		raw_mutex_unlock(&module->mutex);
		(void)uobject_put(&module->obj);
		return E_HM_INVAL;
	}
	loader = module->loader;
	/* block other concurrent unload */
	module->loader = NULL;
	raw_mutex_unlock(&module->mutex);

	if (loader->unload_module != NULL) {
		ret = loader->unload_module(loader, libname);
	} else {
		ret = E_HM_NOSYS;
	}
	if (ret < 0) {
		raw_mutex_lock(&module->mutex);
		module->loader = loader;
		raw_mutex_unlock(&module->mutex);
		(void)uobject_put(&module->obj);
		return ret;
	}
	/* allow load same module after this line */
	(void)uobject_set_delete(&g_devhost_module, &module->obj);
	(void)uobject_release(&module->obj);
	return 0;
}
