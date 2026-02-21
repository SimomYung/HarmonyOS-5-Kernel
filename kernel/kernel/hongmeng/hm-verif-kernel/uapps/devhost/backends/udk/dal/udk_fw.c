/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Firmware udk framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 22 10:55:04 2021
 */

#include <udk/fw.h>
#include <udk/log.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/securec.h>
#include <udk/lib/idr.h>
#include <internal/init.h>
#include <hongmeng/errno.h>

struct udk_fw_manager {
	struct idr fw_idr;
	struct udk_mutex mutex;
};

struct fw_adapter_core {
	struct udk_fw_adapter *adapter;
	struct dlist_node client_list;
};

struct udk_fw_client {
	struct udk_fw_adapter *adapter;
	struct dlist_node node;
};

static struct udk_fw_manager fw_manager;

static inline void fw_manager_lock(void)
{
	udk_mutex_lock(&fw_manager.mutex);
}

static inline void fw_manager_unlock(void)
{
	udk_mutex_unlock(&fw_manager.mutex);
}

#define DEFINE_FW_PROPERTY(operation)						\
int udk_fw_##operation##_property(struct udk_fw_client *client, void *property)	\
{										\
	int err;								\
	struct udk_fw_adapter *adapter = NULL;					\
										\
	if (client == NULL || client->adapter == NULL ||			\
	    client->adapter->ops.operation##_property == NULL) {		\
		udk_warn("invalid client\n");					\
		return -EINVAL;							\
	}									\
	if (property == NULL) {							\
		udk_warn("invalid property\n");					\
		return -EINVAL;							\
	}									\
	fw_manager_lock();							\
	adapter = client->adapter;						\
	err = adapter->ops.operation##_property(adapter->ctx, property);	\
	if (err < 0) {								\
		udk_error("fw "#operation" property failed: %s\n",		\
			  strerror(-err));					\
		fw_manager_unlock();						\
		return err;							\
	}									\
	fw_manager_unlock();							\
										\
	return 0;								\
}

DEFINE_FW_PROPERTY(get);
DEFINE_FW_PROPERTY(set);

int udk_fw_register(struct udk_fw_adapter *adapter)
{
	int err;
	struct fw_adapter_core *core = NULL;

	if (adapter == NULL) {
		udk_warn("invalid adapter\n");
		return -EINVAL;
	}
	core = (struct fw_adapter_core *)udk_malloc(sizeof(*core));
	if (core == NULL) {
		udk_error("malloc fw core failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*core);
	core->adapter = adapter;
	dlist_init(&core->client_list);
	fw_manager_lock();
	err = idr_set(&fw_manager.fw_idr, (void *)core, adapter->id);
	if (err < 0) {
		udk_error("fw idr set failed: %s\n", strerror(-err));
		udk_free(core);
		fw_manager_unlock();
		return -hmerrno2posix(err);
	}
	fw_manager_unlock();

	return 0;
}

void udk_fw_unregister(struct udk_fw_adapter *adapter)
{
	unsigned int id;
	struct udk_fw_client *pos = NULL;
	struct udk_fw_client *next = NULL;
	struct fw_adapter_core *core = NULL;

	if (adapter == NULL) {
		udk_warn("invalid adapter\n");
		return;
	}
	id = adapter->id;
	fw_manager_lock();
	core = (struct fw_adapter_core *)idr_remove(&fw_manager.fw_idr, id);
	if (core == NULL) {
		udk_warn("firmware %d not found\n", id);
		fw_manager_unlock();
		return;
	}
	dlist_for_each_entry_safe(pos, next, &core->client_list,
				  struct udk_fw_client, node) {
		dlist_delete(&pos->node);
		pos->adapter = NULL;
		udk_free(pos);
	}
	udk_free(core);
	fw_manager_unlock();
}

struct udk_fw_client *udk_fw_client_request(unsigned int id)
{
	struct fw_adapter_core *core = NULL;
	struct udk_fw_client *client = NULL;

	fw_manager_lock();
	core = (struct fw_adapter_core *)idr_find(&fw_manager.fw_idr, id);
	if (core == NULL) {
		udk_warn("fw core %d not found\n", id);
		fw_manager_unlock();
		return NULL;
	}
	client = (struct udk_fw_client *)udk_malloc(sizeof(*client));
	if (client == NULL) {
		udk_error("malloc client failed\n");
		fw_manager_unlock();
		return NULL;
	}
	client->adapter = core->adapter;
	dlist_insert_tail(&core->client_list, &client->node);
	fw_manager_unlock();

	return client;
}

int udk_fw_client_free(struct udk_fw_client *client)
{
	unsigned int id;
	struct udk_fw_client *pos = NULL;
	struct udk_fw_client *next = NULL;
	struct fw_adapter_core *core = NULL;

	if (client == NULL || client->adapter == NULL) {
		udk_warn("invalid client\n");
		return -EINVAL;
	}
	fw_manager_lock();
	id = client->adapter->id;
	core = (struct fw_adapter_core *)idr_find(&fw_manager.fw_idr, id);
	if (core == NULL) {
		udk_error("fw core %d not found\n", id);
		fw_manager_unlock();
		return -ENOENT;
	}
	dlist_for_each_entry_safe(pos, next, &core->client_list,
				  struct udk_fw_client, node) {
		if (pos == client) {
			client->adapter = NULL;
			dlist_delete(&client->node);
			udk_free(client);
		}
	}
	fw_manager_unlock();

	return 0;
}

static int udk_fw_init(void)
{
	idr_init(&fw_manager.fw_idr);
	udk_mutex_init(&fw_manager.mutex);

	return 0;
}
udk_init_call(udk_fw_init);
