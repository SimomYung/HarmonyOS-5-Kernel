/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020 All rights reserved.
 * Description: Key and data map helper
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 21 18:14:22 2020
 */

#include <libdevhost/map.h>

#include <string.h>
#include <stdbool.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>

struct map_data {
	struct rbt_node node;
	char *key;
	int value;
};

static void map_rbt_init(struct rbt_tree *rbt)
{
	rbt->root = NULL;
}

int libdh_str_map_init(struct libdh_map_context *map_ctx)
{
	int ret = E_HM_OK;

	if (map_ctx == NULL) {
		ret = E_HM_INVAL;
	} else {
		raw_mutex_init(&map_ctx->mutex);
		map_rbt_init(&map_ctx->rbt);
	}
	return ret;
}

static int map_key_cmp(const struct rbt_node *map_node, const char *key)
{
	return strcmp(rbt_entry(map_node, struct map_data, node)->key, key);
}

static void *map_data_rbt_search(struct rbt_tree *tree, const char *key,
				 struct rbt_node **pparent_node)
{
	struct rbt_node *node = tree->root;
	struct rbt_node *parent = NULL;
	/* tree is NULL */
	if (node == NULL) {
		parent = NULL;
	}
	while (node != NULL) {
		if (map_key_cmp(node, key) == 0) {
			parent = rbt_parent(node);
			break;
		}
	        /* not equal, continue to find correct one */
		if (map_key_cmp(node, key) < 0) {
			/* in right subtree */
			if (node->rbt_right == NULL) {
				/* no right child */
				parent = node;
				node = NULL;
				break;
			} else {
				/* has right child, continue */
				node = node->rbt_right;
			}
		} else {
			/* in left subtree */
			if (node->rbt_left == NULL) {
				/* no left child */
				parent = node;
				node = NULL;
				break;
			} else {
				/* has left child, continue */
				node = node->rbt_left;
			}
		}
	}
	if (pparent_node != NULL) {
		*pparent_node = parent;
	}
	return (node != NULL) ? rbt_entry(node, struct map_data, node) : NULL;
}

static struct map_data *alloc_map_data(const char *key, int value)
{
	struct map_data *map = NULL;

	map = (struct map_data *)malloc(sizeof(*map));
	if (map != NULL) {
		NOFAIL(memset_s(map, sizeof(*map), 0, sizeof(*map)));

		map->value = value;
		map->key = strdup(key);
		if (map->key == NULL) {
			free(map);
			map = NULL;
		}
	}
	return map;
}

static void destroy_map_data(struct rbt_node *node)
{
	struct map_data *map = rbt_entry(node, struct map_data, node);
	if (map != NULL) {
		free(map->key);
		free(map);
	}
}

void libdh_str_map_destroy(struct libdh_map_context *map_ctx)
{
	if (map_ctx == NULL) {
		return;
	}

	rbt_destroy_tree(&map_ctx->rbt, destroy_map_data);
}

bool libdh_str_map_key_exist(struct libdh_map_context *map_ctx, const char *key)
{
	struct map_data *map = NULL;

	if (map_ctx == NULL) {
		return false;
	}

	RAW_MUTEX_GUARD(_, &map_ctx->mutex);

	map = (struct map_data *)map_data_rbt_search(&map_ctx->rbt, key, NULL);
	if (map != NULL) {
		return true;
	}
	return false;
}

int libdh_str_map_insert(struct libdh_map_context *map_ctx,
			 const char *key, int value)
{
	struct map_data *map = NULL;
	struct rbt_node *parent_node = NULL;
	bool data_is_rchild = false;

	int ret = E_HM_OK;

	if (map_ctx == NULL) {
		ret = E_HM_INVAL;
	} else {
		RAW_MUTEX_GUARD(_, &map_ctx->mutex);

		map = (struct map_data *)map_data_rbt_search(&map_ctx->rbt, key,
							     &parent_node);
		if (map != NULL) {
			ret = E_HM_POSIX_EXIST;
		}

		if (ret == E_HM_OK) {
			map = alloc_map_data(key, value);
			if (map == NULL) {
				ret = E_HM_NOMEM;
			}
		}

		if (ret == E_HM_OK) {
			if (parent_node != NULL) {
				data_is_rchild = map_key_cmp(parent_node, key) < 0 ?
						 true : false;
			}
			rbt_insert(&map_ctx->rbt, &map->node, parent_node, data_is_rchild);
		}
	}
	return ret;
}

static struct rbt_node *
__str_map_delete(struct libdh_map_context *map_ctx, const char *key)
{
	struct map_data *map = NULL;

	if (map_ctx == NULL) {
		return NULL;
	}

	RAW_MUTEX_GUARD(_, &map_ctx->mutex);

	map = (struct map_data *)map_data_rbt_search(&map_ctx->rbt, key, NULL);
	if (map != NULL) {
		rbt_remove(&map_ctx->rbt, &map->node);
		return &map->node;
	}
	return NULL;
}

int libdh_str_map_delete(struct libdh_map_context *map_ctx,
			 const char *key, int *value_out)
{
	struct rbt_node *node = NULL;
	int ret = E_HM_OK;

	if (map_ctx == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		node = __str_map_delete(map_ctx, key);
		if (node == NULL) {
			ret = E_HM_POSIX_NOENT;
		}
	}

	if (ret == E_HM_OK) {
		if (value_out != NULL) {
			*value_out = (rbt_entry(node, struct map_data, node))->value;
		}
		destroy_map_data(node);
	}

	return ret;
}
