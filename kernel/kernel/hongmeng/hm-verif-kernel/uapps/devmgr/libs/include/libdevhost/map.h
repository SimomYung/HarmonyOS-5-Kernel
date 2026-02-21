/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020 All rights reserved.
 * Description: Header of libdh map
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 21 19:03:26 2020
 */

#ifndef ULIBS_LIBDEVHOST_MAP_INTERNAL_H
#define ULIBS_LIBDEVHOST_MAP_INTERNAL_H

#include <libalgo/rbtree.h>
#include <libhmsync/raw_mutex.h>

struct libdh_map_context {
	struct rbt_tree rbt;
	struct raw_mutex mutex;
};

int libdh_str_map_init(struct libdh_map_context *map_ctx);

int libdh_str_map_insert(struct libdh_map_context *map_ctx, const char *key,
			 int value);

int libdh_str_map_delete(struct libdh_map_context *map_ctx,
			 const char *key, int *value_out);

bool libdh_str_map_key_exist(struct libdh_map_context *map_ctx, const char *key);

void libdh_str_map_destroy(struct libdh_map_context *map_ctx);

#endif /* ULIBS_LIBDEVHOST_MAP_INTERNAL_H */
