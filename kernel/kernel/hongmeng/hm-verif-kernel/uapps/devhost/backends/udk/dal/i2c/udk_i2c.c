/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK i2c framework
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 29 17:31:45 2022
 */

#include <udk/i2c.h>
#include <udk/errno.h>
#include <libstrict/strict.h>

#ifdef CONFIG_UDK_I2C

#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/lib/idr.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>
#include <internal/init.h>
#include <hongmeng/errno.h>

#define I2C_7BITS_ADDRESS_MAX		0x7F
#define I2C_MSG_LEN			2

struct udk_i2c_manager {
	struct idr cores;
	struct udk_mutex mutex;
};

struct udk_i2c_core {
	struct udk_i2c_adapter *adapter;
	struct dlist_node clients;
	struct udk_mutex mutex;
};

struct udk_i2c_client {
	unsigned int address;
	struct udk_i2c_core *core;
	struct dlist_node node;
};

static struct udk_i2c_manager i2c_manager;

static inline void i2c_mgr_lock(void)
{
	udk_mutex_lock(&i2c_manager.mutex);
}

static inline void i2c_mgr_unlock(void)
{
	udk_mutex_unlock(&i2c_manager.mutex);
}

static inline void i2c_core_lock(struct udk_i2c_core *core)
{
	udk_mutex_lock(&core->mutex);
}

static inline void i2c_core_unlock(struct udk_i2c_core *core)
{
	udk_mutex_unlock(&core->mutex);
}

int udk_i2c_add_adapter(struct udk_i2c_adapter *adapter)
{
	int err;
	struct udk_i2c_core *core = NULL;

	if ((adapter == NULL) || (adapter->ops == NULL) ||
	    (adapter->ops->adapter_transfer == NULL)) {
		udk_warn("invalid i2c adapter\n");
		return -EINVAL;
	}
	core = (struct udk_i2c_core *)udk_malloc(sizeof(*core));
	if (core == NULL) {
		udk_error("i2c core malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*core);
	dlist_init(&core->clients);
	udk_mutex_init(&core->mutex);
	core->adapter = adapter;
	i2c_mgr_lock();
	err = idr_set(&i2c_manager.cores, (void *)core, adapter->id);
	if (err != 0) {
		i2c_mgr_unlock();
		udk_warn("i2c core %d set idr failed\n", adapter->id);
		udk_free(core);
		return -hmerrno2posix(err);
	}
	i2c_mgr_unlock();

	return 0;
}

int udk_i2c_remove_adapter(const struct udk_i2c_adapter *adapter)
{
	struct udk_i2c_core *core = NULL;

	if (adapter == NULL) {
		udk_warn("invalid i2c adapter\n");
		return -EINVAL;
	}
	/* prevent misdelete */
	i2c_mgr_lock();
	core = (struct udk_i2c_core *)idr_find(&i2c_manager.cores, adapter->id);
	if (core == NULL) {
		i2c_mgr_unlock();
		udk_warn("i2c %d not found\n", adapter->id);
		return -ENOENT;
	}
	if (core->adapter != adapter) {
		i2c_mgr_unlock();
		udk_warn("invalid i2c adapter\n");
		return -EINVAL;
	}
	if (!dlist_empty(&core->clients)) {
		i2c_mgr_unlock();
		udk_warn("i2c client list is not empty\n");
		return -EBUSY;
	}
	(void)idr_remove(&i2c_manager.cores, adapter->id);
	udk_free(core);
	i2c_mgr_unlock();

	return 0;
}

static struct udk_i2c_client *i2c_add_client(struct udk_i2c_core *core,
					     unsigned int address)
{
	struct udk_i2c_client *client = NULL;

	client = (struct udk_i2c_client *)udk_malloc(sizeof(*client));
	if (client == NULL) {
		udk_error("i2c client malloc failed\n");
		return NULL;
	}
	mem_zero_s(*client);
	client->core = core;
	client->address = address;
	i2c_core_lock(core);
	dlist_insert(&core->clients, &client->node);
	i2c_core_unlock(core);

	return client;
}

static bool is_address_exist(struct udk_i2c_core *core,
			    unsigned int address)
{
	struct udk_i2c_client *pos = NULL;

	dlist_for_each_entry(pos, &core->clients, struct udk_i2c_client, node) {
		if (pos->address == address) {
			return true;
		}
	}

	return false;
}

struct udk_i2c_client *udk_i2c_add_client(unsigned int i2c_id,
					  unsigned int address)
{
	struct udk_i2c_core *core = NULL;
	struct udk_i2c_client *client = NULL;

	if (address == 0 || address > I2C_7BITS_ADDRESS_MAX) {
		udk_warn("i2c client address check failed\n");
		return NULL;
	}
	i2c_mgr_lock();
	core = (struct udk_i2c_core *)idr_find(&i2c_manager.cores, i2c_id);
	if (core == NULL) {
		i2c_mgr_unlock();
		udk_warn("could not find i2c core %d\n", i2c_id);
		return NULL;
	}
	if (is_address_exist(core, address)) {
		i2c_mgr_unlock();
		udk_warn("i2c client address already exists\n");
		return NULL;
	}
	client = i2c_add_client(core, address);
	i2c_mgr_unlock();

	return client;
}

static int i2c_remove_client(struct udk_i2c_client *client)
{
	struct udk_i2c_core *core = NULL;
	struct udk_i2c_client *pos = NULL;
	struct udk_i2c_client *next = NULL;

	core = client->core;
	i2c_core_lock(core);
	dlist_for_each_entry_safe(pos, next, &core->clients,
				  struct udk_i2c_client, node) {
		if (pos == client) {
			client->core = NULL;
			dlist_delete(&client->node);
			udk_free(client);
			i2c_core_unlock(core);
			return 0;
		}
	}
	i2c_core_unlock(core);

	return -EFAULT;
}

int udk_i2c_remove_client(struct udk_i2c_client *client)
{
	if ((client == NULL) || (client->core == NULL)) {
		udk_warn("invalid client\n");
		return -EINVAL;
	}

	return i2c_remove_client(client);
}

static inline void i2c_msg_set(struct udk_i2c_msg *msg,
			       unsigned int address,
			       unsigned short flags,
			       unsigned char *buf,
			       unsigned int len)
{
	msg->address = address;
	msg->flags = flags;
	msg->len = len;
	msg->buf = buf;
}

static int i2c_transfer(const struct udk_i2c_client *client, unsigned char *wbuf,
			unsigned int wlen, unsigned char *rbuf, unsigned int rlen)
{
	int err;
	int n = 0;
	struct udk_i2c_msg msgs[I2C_MSG_LEN];
	struct udk_i2c_core *core = client->core;
	struct udk_i2c_adapter *adapter = NULL;

	if (core == NULL) {
		return -EINVAL;
	}
	adapter = core->adapter;
	if (adapter == NULL || adapter->ops == NULL ||
	    adapter->ops->adapter_transfer == NULL) {
		return -ENOSYS;
	}
	mem_zero_a(msgs);
	if (wlen > 0) {
		i2c_msg_set(&msgs[n++], client->address, UDK_I2C_FLAG_NONE,
			    wbuf, wlen);
	}
	if (rlen > 0) {
		i2c_msg_set(&msgs[n++], client->address, UDK_I2C_FLAG_READ,
			    rbuf, rlen);
	}
	i2c_core_lock(core);
	err = adapter->ops->adapter_transfer(adapter, msgs, n);
	i2c_core_unlock(core);
	if (err < 0) {
		udk_warn("i2c transfer failed: %s\n", strerror(err));
		return err;
	}
	if (err != n) {
		udk_warn("i2c transfer is not expected: %d/%d\n", err, n);
		return -EIO;
	}

	return 0;
}

int udk_i2c_write(const struct udk_i2c_client *client,
		  unsigned char *wbuf, unsigned int wlen)
{
	if (client == NULL) {
		return -EINVAL;
	}
	if (wbuf == NULL || wlen == 0) {
		return -EINVAL;
	}

	return i2c_transfer(client, wbuf, wlen, NULL, 0);
}

int udk_i2c_read(const struct udk_i2c_client *client, unsigned char *wbuf,
		 unsigned int wlen, unsigned char *rbuf, unsigned int rlen)
{
	if (client == NULL) {
		return -EINVAL;
	}
	if (wbuf == NULL || wlen == 0 || rbuf == NULL || rlen == 0) {
		return -EINVAL;
	}

	return i2c_transfer(client, wbuf, wlen, rbuf, rlen);
}

static int udk_i2c_init(void)
{
	idr_init(&i2c_manager.cores);
	udk_mutex_init(&i2c_manager.mutex);

	return 0;
}
udk_init_call(udk_i2c_init);

#else /* CONFIG_UDK_I2C */

int udk_i2c_add_adapter(struct udk_i2c_adapter *adapter)
{
	UNUSED(adapter);

	return -ENOSYS;
}

int udk_i2c_remove_adapter(const struct udk_i2c_adapter *adapter)
{
	UNUSED(adapter);

	return -ENOSYS;
}

struct udk_i2c_client *udk_i2c_add_client(unsigned int i2c_id,
					  unsigned int address)
{
	UNUSED(i2c_id, address);

	return NULL;
}

int udk_i2c_remove_client(struct udk_i2c_client *client)
{
	UNUSED(client);

	return -ENOSYS;
}

int udk_i2c_write(const struct udk_i2c_client *client,
		  unsigned char *wbuf, unsigned int wlen)
{
	UNUSED(client, wbuf, wlen);

	return -ENOSYS;
}

int udk_i2c_read(const struct udk_i2c_client *client, unsigned char *wbuf,
		 unsigned int wlen, unsigned char *rbuf, unsigned int rlen)
{
	UNUSED(client, wbuf, wlen, rbuf, rlen);

	return -ENOSYS;
}

#endif /* CONFIG_UDK_I2C */
