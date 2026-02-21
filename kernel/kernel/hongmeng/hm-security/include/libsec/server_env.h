/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Provide interfaces for struct sec_server_env
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:21:57 2022
 */
#ifndef ULIBS_LIBSEC_SERVER_ENV_H
#define ULIBS_LIBSEC_SERVER_ENV_H
#include <lib/dlist.h>
#include <stdlib.h>
#include <string.h>

struct sec_server_env {
	struct dlist_node head; /* must align with "dnode" member of "sec_server_env_item" as for type coversion */
};

struct sec_server_env_item {
	struct dlist_node dnode; /* must align with "head" member of "sec_server_env" as for type coversion */
	const char *key;
	const void *value;
};

static inline
void sec_server_env_init(struct sec_server_env *env)
{
	dlist_init(&env->head);
	return;
}

static inline void sec_server_env_item_init(struct sec_server_env_item *item, const char *key, const void *value)
{
	item->key = key;
	item->value = value;
	dlist_init(&item->dnode);
}

static inline
void sec_server_env_insert(struct sec_server_env *env, struct sec_server_env_item *item)
{
	dlist_insert_head(&env->head, &item->dnode);
}

static inline
const void *sec_server_env_find(const struct sec_server_env *env, const char *key)
{
	const void *value = NULL;
	struct sec_server_env_item *pos;
	dlist_for_each_entry(pos, &env->head, struct sec_server_env_item, dnode)
	{
		if(strcmp(pos->key, key) == 0) {
			value = pos->value;
			break;
		}
	}
	return value;
}
#endif
