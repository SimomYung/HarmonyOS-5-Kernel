/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk module related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 05 09:30:56 2019
 */
#include <internal/init.h>

#include <stdio.h>
#include <dlfcn.h>
#include <limits.h>

#include <udk/log.h>
#include <udk/timer.h>
#include <udk/module.h>
#include <udk/workqueue.h>
#include <devhost/backend.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmsync/raw_mutex.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>
#include <libdevhost/uobject.h>

#include <devhost/backend.h>

enum udk_module_param_type udk_module_param_str_to_type(const char *type)
{
	const char *type_str[] = {"int", "uint", "long", "ulong", "charp"};
	enum udk_module_param_type type_enum[] = {UDK_PARAM_INT,
						  UDK_PARAM_UINT,
						  UDK_PARAM_LONG,
						  UDK_PARAM_ULONG,
						  UDK_PARAM_CHARP};
	if (type == NULL) {
		return UDK_PARAM_INVALID;
	}
	for (unsigned int i = 0; i < sizeof(type_str) / sizeof(char *); i++) {
		if (strcmp(type, type_str[i]) == 0) {
			return type_enum[i];
		}
	}
	return UDK_PARAM_INVALID;
}

static int udk_module_param_set_value(struct udk_module_param_node *node, const char *value)
{
	char *tmp = NULL;
	long lv;
	unsigned long ulv;
	errno = 0;
	switch (node->type) {
	case UDK_PARAM_INT:
		lv = strtol(value, &tmp, 0);
		if (tmp[0] != '\0' || errno != 0 || lv > INT_MAX || lv < INT_MIN) {
			return E_HM_INVAL;
		}
		*(int *)node->ptr = (int)lv;
		break;
	case UDK_PARAM_UINT:
		ulv = strtoul(value, &tmp, 0);
		if (tmp[0] != '\0' || errno != 0 || ulv > UINT_MAX) {
			return E_HM_INVAL;
		}
		*(unsigned int *)node->ptr = (unsigned int)ulv;
		break;
	case UDK_PARAM_LONG:
		lv = strtol(value, &tmp, 0);
		if (tmp[0] != '\0' || errno != 0) {
			return E_HM_INVAL;
		}
		*(long *)node->ptr = lv;
		break;
	case UDK_PARAM_ULONG:
		ulv = strtoul(value, &tmp, 0);
		if (tmp[0] != '\0' || errno != 0) {
			return E_HM_INVAL;
		}
		*(unsigned long *)node->ptr = ulv;
		break;
	case UDK_PARAM_CHARP:
		tmp = strdup(value);
		if (tmp == NULL) {
			return E_HM_INVAL;
		}
		*(char **)node->ptr = tmp;
		break;
	case UDK_PARAM_INVALID:
		/* fall through */
	default:
		return E_HM_INVAL;
	}
	node->inited = true;
	return 0;
}

static int udk_compatible(struct devhost_loader *loader,
			  const char *libname, const void *data, size_t size)
{
	const char elf_magic_number[] = {0x7F, 'E', 'L', 'F'};
	const char *p = (const char *)data;
	const size_t magic_len = sizeof(elf_magic_number) / sizeof(elf_magic_number[0]);
	UNUSED(loader, libname);
	if (size < magic_len) {
		return 0;
	}
	for (unsigned int i = 0; i < magic_len; i++) {
		if (p[i] != elf_magic_number[i]) {
			return 0;
		}
	}
	return 1;
}

static struct uobject_set g_udk_module;
struct udk_module {
	char *libname;
	void *handle;
	struct dlist_node param_list;
	unsigned int nr_opener;
	struct raw_mutex mutex;
	struct uobject obj;
};

int udk_module_get(struct udk_module *module)
{
	int ret;
	if (module == NULL) {
		return -EINVAL;
	}
	ret = uobject_get(&module->obj);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

void udk_module_put(struct udk_module *module)
{
	if (module == NULL) {
		return;
	}
	(void)uobject_put(&module->obj);
}

static void udk_module_uobject_release(struct uobject *obj)
{
	struct udk_module *module = container_of(obj, struct udk_module, obj);
	free(module);
}

static bool udk_module_uobject_cmp(const struct uobject *obj, void *arg)
{
	struct udk_module *module = container_of(obj, struct udk_module, obj);
	char *libname = (char *)arg;
	int ret;

	if (libname == NULL) {
		return false;
	}

	raw_mutex_lock(&module->mutex);
	ret = strcmp(module->libname, libname);
	raw_mutex_unlock(&module->mutex);

	return (ret == 0);
}

#define MODULE_MAXLEN 256
static struct udk_module* alloc_udk_module(const char *libname)
{
	struct udk_module *module = NULL;
	size_t sz;
	int ret;

	sz = sizeof(struct udk_module) + strlen(libname) + 1;
	if (sz >= MODULE_MAXLEN) {
		return NULL;
	}
	module = (struct udk_module *)malloc(sz);
	if (module == NULL) {
		return NULL;
	}
	mem_zero_s(*module);
	module->libname = (char *)module + sizeof(struct udk_module);
	ret = strncpy_s(module->libname, sz - sizeof(struct udk_module), libname, strlen(libname));
	if (ret != 0) {
		free(module);
		return NULL;
	}
	module->handle = NULL;
	dlist_init(&module->param_list);
	raw_mutex_init(&module->mutex);
	uobject_init(&module->obj, 1, udk_module_uobject_release);

	return module;
}

static int udk_load_module_deps(const char *libname, const void *data)
{
	void *handle = NULL;
	char (*module_deps)[UDK_MODULE_STR_MAX_LEN] = NULL;
	int *module_len = NULL;
	int err = 0;

	/* Open this module with RTLD_LAZY to resolve dependencies */
	handle = dlopen_from_mem(data, libname, RTLD_LAZY | RTLD_LOCAL);
	if (handle == NULL) {
		udk_warn("Driver module open for dependency failed, libname=%s, err: %s\n",
			 libname, dlerror());
		return E_HM_INVAL;
	}

	module_deps = (char (*)[UDK_MODULE_STR_MAX_LEN])dlsym(handle, "udk_module_deps");
	module_len = (int *)dlsym(handle, "udk_module_len");
	/* We allow module to not specify dependecies */
	if (module_deps != NULL && module_len != NULL) {
		for (int i = 0; i < *module_len; i++) {
			int ret = udk_request_module(module_deps[i]);
			if (ret < 0) {
				err = posix2hmerrno(-ret);
				udk_warn("Driver dependency load failed:, "
					 "libname=%s, dependency=%s, err: %s\n",
					 libname, module_deps[i], hmstrerror(err));
				break;
			}
		}
	}

	(void)dlclose(handle);
	return err;
}

typedef int (*init_type)(void);
typedef void (*init_id_type)(const struct udk_module *);
static int init_module(const struct udk_module *module, const char *libname, void *handle)
{
	init_type func = NULL;
	init_id_type set_id_func = NULL;

	set_id_func = (init_id_type)(uintptr_t)dlsym(handle, "udk_set_this_module");
	/* We allow a module to not have set_id_func */
	if (set_id_func != NULL) {
		set_id_func(module);
	}

	func = (init_type)(uintptr_t)dlsym(handle, "udk_init_module");
	/* We allow a module to not have an init function */
	if (func != NULL) {
		int ret = func();
		if (ret < 0) {
			udk_warn("Driver init failed, libname=%s, err: %s\n", libname, strerror(-ret));
			return posix2hmerrno(-ret);
		}
	}
	return 0;
}

/*
 * The args passed from dmctl is well-formed as name1="value1" name2="value2".
 * So we don't need to handle special charactor here.
 * This function will return the start ptr to the next arg on success,
 * and NULL on error.
 */
/* the length of \0 and \", or the length of \0 and space */
#define ARG_SKIP_LEN 2
static char* udk_parse_one_param(char *args, char **name, char **value)
{
	char *arg = args;
	char *equal_char = NULL;
	char *end_char = NULL;

	if (arg[0] == '\0') {
		return arg;
	}

	equal_char = strchr(arg, '=');
	if (equal_char == NULL) {
		return NULL;
	}
	equal_char[0] = '\0';
	*name = arg;

	if (equal_char[1] != '"') {
		return NULL;
	}
	arg = equal_char + ARG_SKIP_LEN;

	end_char = strchr(arg, '"');
	if (end_char == NULL) {
		return NULL;
	}
	end_char[0] = '\0';
	*value = arg;

	if (end_char[1] == ' ') {
		return end_char + ARG_SKIP_LEN;
	} else if (end_char[1] == '\0') {
		return end_char + 1;
	}
	return NULL;
}

static int udk_set_param(const struct dlist_node *param_list, const char *name, const char *value)
{
	struct udk_module_param_node *node = NULL;
	if (name == NULL) {
		return E_HM_INVAL;
	}
	dlist_for_each_entry(node, param_list, struct udk_module_param_node, list) {
		if (node->name != NULL && strcmp(name, node->name) == 0) {
			int ret = udk_module_param_set_value(node, value);
			if (ret < 0) {
				udk_warn("Driver set param %s failed, invalid value=%s\n", name, value);
				return ret;
			}
			break;
		}
	}
	return 0;
}

typedef struct dlist_node *(*param_type)(void);
static int udk_init_params(struct udk_module *module, void *handle, char *args)
{
	int ret = 0;
	char *arguments = args;
	struct dlist_node *param_list = NULL;
	param_type func = NULL;

	if (arguments == NULL) {
		return 0;
	}
	func = (param_type)(uintptr_t)dlsym(handle, "udk_get_param_list");
	if (func == NULL) {
		return 0;
	}
	param_list = func();
	if (param_list == NULL) {
		return 0;
	}
	if (!dlist_empty(param_list)) {
		/* set module->param_list as new head. */
		dlist_replace(param_list, &module->param_list);
	}

	while (arguments[0] != '\0') {
		char *name = NULL;
		char *value = NULL;

		arguments = udk_parse_one_param(arguments, &name, &value);
		if (arguments == NULL) {
			ret = E_HM_INVAL;
			break;
		}

		ret = udk_set_param(&module->param_list, name, value);
		if (ret < 0) {
			break;
		}
	}
	return ret;
}

static void udk_free_params(const struct udk_module *module)
{
	struct udk_module_param_node *node = NULL;
	dlist_for_each_entry(node, &module->param_list, struct udk_module_param_node, list) {
		if (node->type == UDK_PARAM_CHARP && node->inited) {
			free(*(char **)node->ptr);
		}
	}
	return;
}

static int udk_load_module(struct devhost_loader *loader,
			   const char *libname, const void *data, size_t size, char *args)
{
	struct udk_module *module = NULL;
	void *handle = NULL;
	int ret;
	int err = E_HM_INVAL;
	UNUSED(loader, size);

	/* Register module and save handle for dlclose() later */
	module = alloc_udk_module(libname);
	if (module == NULL) {
		return E_HM_NOMEM;
	}
	/* insertion fails when driver is already loaded */
	ret = uobject_set_insert(&g_udk_module, &module->obj, module->libname);
	if (ret < 0) {
		if (ret == E_HM_OBJEXIST) {
			udk_warn("Driver is already loaded, libname=%s\n", libname);
		}
		err = E_HM_POSIX_EXIST;
		goto insert_err;
	}

	/* Load module dependencies */
	ret = udk_load_module_deps(libname, data);
	if (ret < 0) {
		goto init_err;
	}

	/* Open this module with RTLD_NOW to resolve all symbols */
	handle = dlopen_from_mem(data, libname, RTLD_NOW | RTLD_GLOBAL);
	if (handle == NULL) {
		udk_warn("Driver module open failed, libname=%s, err: %s\n", libname, dlerror());
		goto init_err;
	}
	/* Set module parameters */
	if ((ret = udk_init_params(module, handle, args)) < 0) {
		goto init_err;
	}
	/* Run module's init function */
	if ((ret = init_module(module, libname, handle)) < 0) {
		goto init_err;
	}
	raw_mutex_lock(&module->mutex);
	/* allow unload after this line */
	module->handle = handle;
	raw_mutex_unlock(&module->mutex);

	(void)uobject_put(&module->obj);
	return E_HM_OK;

init_err:
	if (handle != NULL) {
		(void)dlclose(handle);
	}
	/* allow load the same module after this line */
	(void)uobject_set_delete(&g_udk_module, &module->obj);
insert_err:
	(void)uobject_release(&module->obj);
	return err;
}

typedef int (*exit_type)(void);
static int exit_module(const char *libname, void *handle)
{
	exit_type func = NULL;

	func = (init_type)(uintptr_t)dlsym(handle, "udk_exit_module");
	/* We allow a module to not have an exit function */
	if (func != NULL) {
		int ret = func();
		if (ret < 0) {
			udk_warn("Driver exit failed, libname=%s, err: %s\n", libname, strerror(-ret));
			return posix2hmerrno(-ret);
		}
	}

	return 0;
}

static int udk_unload_module(struct devhost_loader *loader, const char *libname)
{
	struct uobject *obj = NULL;
	struct udk_module *module = NULL;
	void *handle = NULL;
	int ret;
	UNUSED(loader);

	obj = uobject_set_acquire(&g_udk_module, (void *)libname);
	if (obj == NULL) {
		return E_HM_INVAL;
	}
	module = container_of(obj, struct udk_module, obj);

	raw_mutex_lock(&module->mutex);
	/*
	 * if module->handle is NULL, then the module is in the process
	 * of initialization, or it's already cleaned by udk_load_module,
	 * or it's being cleaned by another udk_unload_module.
	 * if module->nr_opener larger than 0, reject this unload request.
	 */
	if (module->handle == NULL || module->nr_opener != 0) {
		raw_mutex_unlock(&module->mutex);
		(void)uobject_put(&module->obj);
		return E_HM_INVAL;
	}
	handle = module->handle;
	/* block other concurrent unload */
	module->handle = NULL;
	raw_mutex_unlock(&module->mutex);

	ret = exit_module(libname, handle);
	if (ret < 0) {
		raw_mutex_lock(&module->mutex);
		module->handle = handle;
		raw_mutex_unlock(&module->mutex);
		(void)uobject_put(&module->obj);
		return ret;
	}
	udk_free_params(module);
	(void)dlclose(handle);
	/* allow load same module after this line */
	(void)uobject_set_delete(&g_udk_module, &module->obj);

	(void)uobject_release(&module->obj);
	return 0;
}

static int udk_do_initcall_func(const struct dlist_node *node)
{
	udk_initcall_t *fn = NULL;
	int ret;

	dlist_for_each_entry(fn, node, udk_initcall_t, list) {
		if (fn == NULL || fn->func == NULL) {
			return -ENOENT;
		}
		ret = fn->func();
		if (ret < 0) {
			udk_error("initcall failed: name=%s\n", fn->name);
			return ret;
		}
	}

	return 0;
}

/*
 * udk_initcall_t and udk_initcall_register are called in serial by dlopen.
 * So we don't need a lock here.
 */
static DLIST_HEAD(udk_initcall_list);
void udk_initcall_register(udk_initcall_t *fn)
{
	if (fn == NULL || fn->func == NULL) {
		return;
	}
	dlist_insert_tail(&udk_initcall_list, &fn->list);
}

static int udk_do_initcalls(void)
{
	return udk_do_initcall_func(&udk_initcall_list);
}

static DLIST_HEAD(udk_postinit_list);
void udk_post_initcall_register(udk_initcall_t *fn)
{
	if (fn == NULL || fn->func == NULL) {
		return;
	}
	dlist_insert_tail(&udk_postinit_list, &fn->list);
}

static int udk_postinit(void *arg)
{
	UNUSED(arg);

	return udk_do_initcall_func(&udk_postinit_list);
}

int udk_module_open(struct udk_module *module)
{
	int ret;
	if (module == NULL) {
		return 0;
	}
	if ((ret = udk_module_get(module)) < 0) {
		return ret;
	}
	raw_mutex_lock(&module->mutex);
	if (module->handle == NULL) {
		raw_mutex_unlock(&module->mutex);
		udk_module_put(module);
		return -EINVAL;
	}
	module->nr_opener++;
	raw_mutex_unlock(&module->mutex);

	return 0;
}

void udk_module_close(struct udk_module *module)
{
	if (module == NULL) {
		return;
	}
	raw_mutex_lock(&module->mutex);
	module->nr_opener--;
	raw_mutex_unlock(&module->mutex);
	udk_module_put(module);
}

/* Built-in so loader */
static struct devhost_loader udk_loader = {
	.is_compatible = udk_compatible,
	.load_module = udk_load_module,
	.unload_module = udk_unload_module,
};

static void init_udk_module_loader(void)
{
	uobject_set_init(&g_udk_module, udk_module_uobject_cmp);
	(void)devhost_register_loader(&udk_loader);
}

int dh_backend_init(struct dh_backend_ctor *ctor, const char *backend_cmdline)
{
	int ret;
	UNUSED(ctor, backend_cmdline);

	init_udk_module_loader();

	ret = udk_do_initcalls();
	if (ret < 0) {
		return ret;
	}
	ret = devhost_postinit(udk_postinit, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}
