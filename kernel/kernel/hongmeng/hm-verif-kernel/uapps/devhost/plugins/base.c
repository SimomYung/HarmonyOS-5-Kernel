/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Plugin system base framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 11:10:51 2020
 */
#include "plugin.h"

#include <string.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>

static raw_atomic_int_t __inited;
static struct extension_point *ep_start;
static const struct extension_point *ep_end;
static struct libdh_plugin *plugin_start;
static const struct libdh_plugin *plugin_end;

static struct extension_point __libdh_none_ep;
static struct libdh_plugin __libdh_none_plugin;
/* symbol from app. keep consistent with dh_plugin.lds */
extern struct extension_point __libdh_extpoint_start __attribute__((weak, alias("__libdh_none_ep"))) ;
extern const struct extension_point __libdh_extpoint_end __attribute__((weak, alias("__libdh_none_ep")));
extern struct libdh_plugin __libdh_plugin_start __attribute__((weak, alias("__libdh_none_plugin")));
extern const struct libdh_plugin __libdh_plugin_end __attribute__((weak, alias("__libdh_none_plugin")));

static struct raw_mutex ep_list_lock = RAW_MUTEX_INITIALIZER;
static struct raw_mutex plugin_list_lock = RAW_MUTEX_INITIALIZER;
static DLIST_HEAD(g_ep_list);
static DLIST_HEAD(g_plugin_list);

static int __libdh_plugin_ep_init(struct extension_point *ep);

static int libdh_plugin_info_init(void)
{
	uintptr_t dur;
	if (raw_atomic_int_cas(&__inited, 0, 1) != 0) {
		return E_HM_INVAL;
	}

	ep_start = &__libdh_extpoint_start;
	ep_end = &__libdh_extpoint_end;
	plugin_start = &__libdh_plugin_start;
	plugin_end = &__libdh_plugin_end;

	if (ep_start > ep_end || plugin_start > plugin_end) {
		return E_HM_INVAL;
	}
	dur = (uintptr_t)ep_end - (uintptr_t)ep_start;
	if (dur != 0U && dur < sizeof(struct extension_point)) {
		return E_HM_INVAL;
	}
	dur = (uintptr_t)plugin_end - (uintptr_t)plugin_start;
	if (dur != 0U && dur < sizeof(struct libdh_plugin)) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int libdh_plugin_pre_init(void)
{
	int ret;
	struct extension_point *ep = ep_start;
	struct libdh_plugin *plugin = NULL;
	while (ep < ep_end) {
		plugin = ep->owner();
		if (plugin == NULL) { /* elf extension point */
			ret = libdh_plugin_ep_init(ep); /* base init */
			if (ret < 0) {
				return ret;
			}
		} else { /* plugin extension point */
			/* gather extension point info of 'plugin' */
			dlist_insert_tail(&plugin->ext_points, &ep->owner_dnode);
		}

		ep = (struct extension_point *)((uintptr_t)ep + ep->size);
	}
	return E_HM_OK;
}

int libdh_plugin_sys_init(void)
{
	int ret;

	ret = libdh_plugin_info_init();
	if (ret < 0) {
		return ret;
	}
	mem_zero_s(__libdh_none_ep);
	mem_zero_s(__libdh_none_plugin);
	if (((&__libdh_none_ep) == (&__libdh_extpoint_end)) &&
	    ((&__libdh_none_plugin) == (&__libdh_plugin_end))) {
		/* nothing to do */
		return E_HM_OK;
	}

	ret = libdh_plugin_pre_init();
	if (ret < 0) {
		return ret;
	}

	ret = libdh_builtin_plugins_sys_init(plugin_start, plugin_end);
	if (ret < 0) {
		hm_error("builtin plugins init failed\n");
	}
	return ret;
}

/* plugin */
static struct libdh_plugin *__find_plugin_nolock(const char *name)
{
	struct dlist_node *pos = NULL;
	struct libdh_plugin *plugin = NULL;

	dlist_for_each(pos, &g_plugin_list) {
		plugin = dlist_entry(pos, struct libdh_plugin, dnode);
		if (strcmp(plugin->name, name) == 0) {
			return plugin;
		}
	}
	return NULL;
}

static void __auto_release_plugin(struct raw_refcnt *ref)
{
	struct libdh_plugin *plugin = container_of(ref, struct libdh_plugin, refcnt);
	hm_panic("%s: not support release plugin now\n", plugin->name);
}

#define PLUGIN_LIST_GUARD	RAW_MUTEX_GUARD(_, &plugin_list_lock)
int libdh_plugin_insert(struct libdh_plugin *plugin)
{
	struct libdh_plugin *plugin_ptr = NULL;
	if (plugin == NULL || plugin->name == NULL) {
		return E_HM_INVAL;
	}
	PLUGIN_LIST_GUARD;
	plugin_ptr = __find_plugin_nolock(plugin->name);
	if (plugin_ptr != NULL) {
		return E_HM_POSIX_EXIST;
	}
	dlist_insert_tail(&g_plugin_list, &plugin->dnode);
	raw_refcnt_init(&plugin->refcnt, 1, __auto_release_plugin);
	return E_HM_OK;
}

int libdh_plugin_delete(struct libdh_plugin *plugin)
{
	struct libdh_plugin *plugin_ptr = NULL;
	if (plugin == NULL || plugin->name == NULL) {
		return E_HM_INVAL;
	}
	PLUGIN_LIST_GUARD;
	plugin_ptr = __find_plugin_nolock(plugin->name);
	if (plugin_ptr != NULL) {
		return E_HM_INVAL;
	}
	dlist_delete(&plugin->dnode);
	return E_HM_OK;
}

int libdh_plugin_get(struct libdh_plugin *plugin)
{
	if (plugin != NULL) {
		return raw_refcnt_get(&plugin->refcnt);
	}
	return E_HM_INVAL;
}

int libdh_plugin_put(struct libdh_plugin *plugin)
{
	if (plugin != NULL) {
		return raw_refcnt_put(&plugin->refcnt);
	}
	return E_HM_INVAL;
}

/* extension point */
static struct extension_point *__find_ep_nolock(const char *name)
{
	struct dlist_node *pos = NULL;
	struct extension_point *ep = NULL;

	dlist_for_each(pos, &g_ep_list) {
		ep = dlist_entry(pos, struct extension_point, dnode);
		if (strcmp(ep->name, name) == 0) {
			return ep;
		}
	}
	return NULL;
}

#define EP_LIST_GUARD	RAW_MUTEX_GUARD(_, &ep_list_lock)
define_refcnt_function1(ep_node, offset_of(struct extension_point, refcnt),
			EP_LIST_GUARD, __find_ep_nolock, const char *, name)

int libdh_plugin_ep_acquire(const char *name, struct extension_point **ep_out)
{
	struct extension_point *ep = NULL;

	if (name == NULL || ep_out == NULL) {
		return E_HM_INVAL;
	}

	ep = acquire_ep_node_ptr_ref(name);
	if (ep == NULL) {
		*ep_out = NULL;
		return E_HM_POSIX_NOENT;
	}
	*ep_out = ep;
	return E_HM_OK;
}

static void __auto_release_ep(struct raw_refcnt *ref)
{
	struct extension_point *ep = container_of(ref, struct extension_point, refcnt);

	hm_panic("%s: not support release ep now\n", ep->name);
}

int libdh_plugin_ep_insert(struct extension_point *ep)
{
	struct extension_point *ep_ptr = NULL;
	if (ep == NULL || ep->name == NULL) {
		return E_HM_INVAL;
	}
	EP_LIST_GUARD;
	ep_ptr = __find_ep_nolock(ep->name);
	if (ep_ptr != NULL) {
		return E_HM_POSIX_EXIST;
	}
	dlist_insert_tail(&g_ep_list, &ep->dnode);
	raw_refcnt_init(&ep->refcnt, 1, __auto_release_ep);
	return E_HM_OK;
}

int libdh_plugin_ep_delete(struct extension_point *ep)
{
	struct extension_point *ep_ptr = NULL;
	if (ep == NULL || ep->name == NULL) {
		return E_HM_INVAL;
	}
	EP_LIST_GUARD;
	ep_ptr = __find_ep_nolock(ep->name);
	if (ep_ptr == NULL) {
		return E_HM_INVAL;
	}
	dlist_delete(&ep->dnode);
	return E_HM_OK;
}

int libdh_plugin_ep_get(struct extension_point *ep)
{
	if (ep != NULL) {
		return raw_refcnt_get(&ep->refcnt);
	}
	return E_HM_INVAL;
}

int libdh_plugin_ep_put(struct extension_point *ep)
{
	if (ep != NULL) {
		return raw_refcnt_put(&ep->refcnt);
	}
	return E_HM_INVAL;
}

static int __libdh_plugin_ep_init(struct extension_point *ep)
{
	struct libdh_plugin *owner = NULL;
	int ret;

	ret = libdh_plugin_ep_insert(ep);
	if (ret < 0) {
		return ret;
	}

	raw_rwlock_init(&ep->rwlock, NULL);
	owner = ep->owner();
	/* when ep is located at elf, ep->owner() is NULL */
	if (owner != NULL) {
		if (libdh_plugin_get(owner) <= 1) {
			hm_error("owner of ep \"%s\" is invalid\n", ep->name);
			(void)libdh_plugin_ep_delete(ep);
			return E_HM_INVAL;
		}
	}
	return E_HM_OK;
}

int libdh_plugin_ep_init(struct extension_point *ep)
{
	int ret;
	if (ep == NULL || ep->name == NULL) {
		return E_HM_INVAL;
	}
	ret = ep->init(ep);
	if (ret < 0) {
		hm_error("extension point \"%s\" init failed\n", ep->name);
			return ret;
	}
	ret = __libdh_plugin_ep_init(ep); /* base init */
	if (ret < 0) {
		if (ep->deinit != NULL) {
			ep->deinit(ep);
		}
		return ret;
	}
	return E_HM_OK;
}

/* extension */
int libdh_plugin_ext_attach(struct libdh_extension *ext)
{
	int ret;
	struct extension_point *ep = NULL;
	struct libdh_plugin *plugin = NULL;
	if (ext == NULL || ext->name == NULL) {
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &ext->ext_lock);

	ret = libdh_plugin_ep_acquire(ext->name, &ep);
	if (ret < 0) {
		hm_error("bad ep \"%s\": %s\n", ext->name, hmstrerror(ret));
		return ret;
	}
	BUG_ON(ep == NULL);

	plugin = ext->owner();
	ret = libdh_plugin_get(plugin);
	if (ret <= 1) {
		hm_error("plugin is invalid");
		(void)libdh_plugin_ep_put(ep);
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&ep->rwlock);
	ret = ep->attach(ep, ext);
	(void)raw_rwlock_unlock(&ep->rwlock);
	if (ret < 0) {
		hm_error("extend ep \"%s\" failed: %s\n", ep->name, hmstrerror(ret));
		(void)libdh_plugin_ep_put(ep);
		(void)libdh_plugin_put(plugin);
		return ret;
	}
	return ret;
}

int libdh_plugin_exts_init(struct libdh_plugin *plugin)
{
	struct libdh_extension *ext = NULL;
	int ret;

	if (plugin == NULL) {
		return E_HM_INVAL;
	}
	ext = plugin->extensions;
	if (ext == NULL) {
		return E_HM_OK;
	}

	while (ext->name != NULL) {
		ret = libdh_plugin_ext_attach(ext);
		if (ret < 0) {
			return ret;
		}
		ext++;
	}
	return E_HM_OK;
}
