/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Devhost key
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 30 18:43:51 2021
 */
#include "devhost.h"

#include <linux/key.h>
#include <linux/slab.h>
#include <linux/key-type.h>
#include <keys/user-type.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#include <keys/request_key_auth-type.h>
#endif
#include <linux/errno.h>

static int devhost_key_request(struct key *k, void *aux)
{
	struct user_key_payload *payload = NULL;
	struct request_key_auth *rka = dereference_key_rcu(k);
	struct key *key = rka->target_key;
	int tmp_data;
	int ret;
	long size = 0;
	long id;

	id = liblinux_key_request(key->index_key.type->name, key->index_key.description, NULL, 0);
	if (id < 0) {
		return (int)id;
	}

	size = liblinux_key_read(id, (unsigned long)(uintptr_t)&tmp_data, sizeof(int));
	if (size < 0) {
		return (int)size;
	}

	payload = (struct user_key_payload *)kzalloc(sizeof(*payload) + size, GFP_KERNEL);
	if (payload == NULL) {
		return -ENOMEM;
	}
	payload->datalen = size;

	size = liblinux_key_read(id, (unsigned long)(uintptr_t)payload->data, payload->datalen);
	if (size != payload->datalen) {
		kfree(payload);
		return -EFAULT;
	}

	ret = key_instantiate_and_link(key, payload, size, NULL, NULL);
	kfree(payload);
	return ret;
}

int __init devhost_key_init(void)
{
	key_type_user.request_key = devhost_key_request;
	key_type_logon.request_key = devhost_key_request;

	return 0;
}
