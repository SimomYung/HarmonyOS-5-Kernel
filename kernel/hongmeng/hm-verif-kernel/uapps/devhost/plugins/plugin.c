/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Init function of builtin plugin
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 16:15:14 2020
 */
#include "plugin.h"

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#include <devhost/plugin.h>

int __libdh_builtin_plugin_init(struct libdh_plugin *plugin)
{
	struct libdh_builtin_plugin *bp = NULL;
	int ret;

	bp = container_of(plugin, struct libdh_builtin_plugin, base);
	if (bp->constructor != NULL) {
		ret = bp->constructor(bp);
		if (ret < 0) {
			hm_error("construct plugin \"%s\" failed\n", plugin->name);
			return ret;
		}
	}
	return E_HM_OK;
}

static int __libdh_plugin_init(struct libdh_plugin *plugin)
{
	int ret;
	struct extension_point *ep = NULL;
	struct dlist_node *pos = NULL;

	/* plugin init first */
	ret = libdh_plugin_insert(plugin);
	if (ret < 0) {
		return ret;
	}
	ret = plugin->init(plugin); /* subclass plugin init */
	if (ret < 0) {
		(void)libdh_plugin_delete(plugin);
		hm_error("plugin \"%s\" init failed\n", plugin->name);
		return ret;
	}

	/* load extension point secondly */
	dlist_for_each(pos, &plugin->ext_points) {
		ep = dlist_entry(pos, struct extension_point, owner_dnode);
		ret = libdh_plugin_ep_init(ep); /* base init */
		if (ret < 0) {
			hm_error("ep \"%s\" init failed\n", ep->name);
			return ret;
		}
	}

	return E_HM_OK;
}

int libdh_builtin_plugins_sys_init(struct libdh_plugin *start, const struct libdh_plugin *end)
{
	struct libdh_builtin_plugin *plugin = NULL;
	size_t plugin_cnt = 0;
	size_t i = 0;
	int ret;
	if (start == NULL || end == NULL || ((uintptr_t)end < (uintptr_t)start)) {
		return E_HM_INVAL;
	}
	plugin_cnt = ((uintptr_t)end - (uintptr_t)start) / sizeof(struct libdh_builtin_plugin);
	plugin = container_of(start, struct libdh_builtin_plugin, base);

	for (i = 0; i < plugin_cnt; ++i) {
		ret = __libdh_plugin_init(&(plugin[i].base));
		if (ret < 0) {
			hm_error("plugin %s init failed\n", plugin[i].base.name);
			return ret;
		}
	}
	for (i = 0; i < plugin_cnt; ++i) {
		/* load plugins extensions finally */
		ret = libdh_plugin_exts_init(&(plugin[i].base)); /* base ext init */
		if (ret < 0) {
			hm_error("plugin %s extension init failed\n", plugin[i].base.name);
			return ret;
		}
	}
	return E_HM_OK;
}

int libdh_dso_plugin_pre_init(struct libdh_plugin *dso_plugin, const struct libdh_plugin *dso_plugin_end,
			      struct extension_point *ep, const struct extension_point *ep_end)
{
	struct libdh_plugin *plugin = NULL;
	size_t plugin_cnt = 0;

	plugin_cnt = ptr_diff(dso_plugin_end, dso_plugin) / sizeof(struct libdh_dso_plugin);
	if ((plugin_cnt != 1U) && (dso_plugin->type != DH_PLUGIN_TYPE_DYNAMIC)) {
		return E_HM_INVAL;
	}

	while (ep < ep_end) {
		plugin = ep->owner();
		if (plugin != dso_plugin) {
			return E_HM_INVAL;
		}
		dlist_insert_tail(&dso_plugin->ext_points, &ep->owner_dnode);
		ep = (struct extension_point *)ptr_add(ep, ep->size);
	}
	return E_HM_OK;
}

int __libdh_dso_plugin_init(struct libdh_plugin *plugin)
{
	struct libdh_dso_plugin *bp = NULL;
	int ret;

	bp = container_of(plugin, struct libdh_dso_plugin, base);
	if (bp->constructor != NULL) {
		ret = bp->constructor(bp);
		if (ret < 0) {
			hm_error("construct plugin \"%s\" failed\n", plugin->name);
			return ret;
		}
	}
	return E_HM_OK;
}

int libdh_dso_plugin_init(struct libdh_plugin *dso_plugin)
{
	struct libdh_dso_plugin *plug = NULL;
	int ret;

	if (dso_plugin == NULL) {
		return E_HM_INVAL;
	}
	plug = container_of(dso_plugin, struct libdh_dso_plugin, base);

	ret = __libdh_plugin_init(&plug->base);
	if (ret < 0) {
		hm_error("dso plugin %s init failed\n", plug->base.name);
		return ret;
	}
	ret = libdh_plugin_exts_init(&plug->base);
	if (ret < 0) {
		hm_error("plugin %s extension init failed\n", plug->base.name);
		return ret;
	}
	return E_HM_OK;
}
