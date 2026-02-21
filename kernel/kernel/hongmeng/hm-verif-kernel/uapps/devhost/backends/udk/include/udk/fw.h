/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Firmware udk framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 22 10:59:59 2021
 */
#ifndef __UDK_FW_H__
#define __UDK_FW_H__

#include <udk/errno.h>
#include <libstrict/strict.h>

struct udk_fw_ops {
	int (*get_property)(void *ctx, void *property);
	int (*set_property)(void *ctx, void *property);
};

struct udk_fw_adapter {
	unsigned int id;
	void *ctx;
	struct udk_fw_ops ops;
};

struct udk_fw_client;

#ifdef CONFIG_UDK_FIRMWARE

int udk_fw_register(struct udk_fw_adapter *adapter);
void udk_fw_unregister(struct udk_fw_adapter *adapter);
struct udk_fw_client *udk_fw_client_request(unsigned int id);
int udk_fw_client_free(struct udk_fw_client *client);
int udk_fw_get_property(struct udk_fw_client *client, void *property);
int udk_fw_set_property(struct udk_fw_client *client, void *property);

#else

static inline int udk_fw_register(struct udk_fw_adapter *adapter)
{
	UNUSED(adapter);

	return -ENOSYS;
}

static inline void udk_fw_unregister(struct udk_fw_adapter *adapter)
{
	UNUSED(adapter);
}

static inline struct udk_fw_client *udk_fw_client_request(unsigned int id)
{
	UNUSED(id);

	return NULL;
}

static inline int udk_fw_client_free(struct udk_fw_client *client)
{
	UNUSED(client);

	return -ENOSYS;
}

static inline int udk_fw_get_property(struct udk_fw_client *client,
				      void *property)
{
	UNUSED(client, property);

	return -ENOSYS;
}

static inline int udk_fw_set_property(struct udk_fw_client *client,
				      void *property)
{
	UNUSED(client, property);

	return -ENOSYS;
}

#endif

#endif /* __UDK_FW_H__ */
