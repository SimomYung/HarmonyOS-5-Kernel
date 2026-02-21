/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: devhost plugin API prototypes
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 11:10:51 2020
 */
#ifndef DEVHOST_PLUGIN_H
#define DEVHOST_PLUGIN_H

#include <devhost/plugin/base.h>

int libdh_plugin_sys_init(void);

int libdh_plugin_ep_acquire(const char *name, struct extension_point **ep_out);
int libdh_plugin_ep_insert(struct extension_point *ep);
int libdh_plugin_ep_delete(struct extension_point *ep);
int libdh_plugin_ep_get(struct extension_point *ep);
int libdh_plugin_ep_put(struct extension_point *ep);
int libdh_plugin_ep_init(struct extension_point *ep);

int libdh_plugin_exts_init(struct libdh_plugin *plugin);
int libdh_plugin_ext_attach(struct libdh_extension *ext);

int libdh_plugin_insert(struct libdh_plugin *plugin);
int libdh_plugin_delete(struct libdh_plugin *plugin);
int libdh_plugin_get(struct libdh_plugin *plugin);
int libdh_plugin_put(struct libdh_plugin *plugin);

int libdh_builtin_plugins_sys_init(struct libdh_plugin *start, const struct libdh_plugin *end);

int libdh_dso_plugin_pre_init(struct libdh_plugin *dso_plugin, const struct libdh_plugin *dso_plugin_end,
			      struct extension_point *ep, const struct extension_point *ep_end);

#endif /* DEVHOST_PLUGIN_H */
