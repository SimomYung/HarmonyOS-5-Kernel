/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK spi subsystem framework
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 21 9:23:42 2022
 */

#include <udk/spi.h>
#include <udk/errno.h>
#include <libstrict/strict.h>

#ifdef CONFIG_UDK_SPI

#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/lib/idr.h>
#include <udk/lib/dlist.h>
#include <udk/securec.h>
#include <internal/init.h>

struct udk_spi_manager {
	struct idr cores;
	struct udk_mutex lock;
};

struct udk_spi_core {
	struct udk_spi_adapter *adapter;
	struct dlist_node clients;
};

struct udk_spi_client {
	bool is_opened;
	unsigned int adapter_id;
	unsigned int client_id;
	unsigned int speed;
	unsigned int mode;
	struct dlist_node node;
};

static struct udk_spi_manager spi_manager = {
	.cores = IDR_INIT(spi_manager.cores),
};

static inline void udk_spi_mgr_lock(void)
{
	udk_mutex_lock(&spi_manager.lock);
}

static inline void udk_spi_mgr_unlock(void)
{
	udk_mutex_unlock(&spi_manager.lock);
}

static inline struct udk_spi_core *spi_core_lookup(unsigned int adapter_id)
{
	return (struct udk_spi_core *)idr_find(&spi_manager.cores, adapter_id);
}

static inline void spi_core_destroy(unsigned int adapter_id)
{
	(void)idr_remove(&spi_manager.cores, adapter_id);
}

int udk_spi_add_adapter(struct udk_spi_adapter *adapter)
{
	int err;
	struct udk_spi_core *core = NULL;

	if ((adapter == NULL) || (adapter->ops == NULL) ||
	    (adapter->ops->adapter_transfer == NULL)) {
		udk_warn("invalid spi adapter\n");
		return -EINVAL;
	}
	core = (struct udk_spi_core *)udk_malloc(sizeof(*core));
	if (core == NULL) {
		udk_error("spi core malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*core);
	dlist_init(&core->clients);
	core->adapter = adapter;
	err = idr_set(&spi_manager.cores, (void *)core, adapter->id);
	if (err != 0) {
		udk_warn("spi core %d set idr failed\n", adapter->id);
		udk_free(core);
		return err;
	}

	return 0;
}

int udk_spi_remove_adapter(const struct udk_spi_adapter *adapter)
{
	struct udk_spi_core *core = NULL;

	if (adapter == NULL) {
		udk_warn("invalid spi adapter\n");
		return -EINVAL;
	}

	/* prevent misdelete */
	udk_spi_mgr_lock();
	core = spi_core_lookup(adapter->id);
	if (core == NULL) {
		udk_warn("spi %d not found\n", adapter->id);
		udk_spi_mgr_unlock();
		return -ENOENT;
	}
	if (core->adapter != adapter) {
		udk_warn("invalid spi adapter\n");
		udk_spi_mgr_unlock();
		return -EINVAL;
	}

	if (!dlist_empty(&core->clients)) {
		udk_warn("spi client list is not empty\n");
		udk_spi_mgr_unlock();
		return -EBUSY;
	}

	spi_core_destroy(adapter->id);
	udk_free(core);
	udk_spi_mgr_unlock();

	return 0;
}

static inline void spi_set_client_info(struct udk_spi_client *client,
				       unsigned int adapter_id,
				       unsigned int client_id,
				       unsigned int mode, unsigned int speed)
{
	client->adapter_id = adapter_id;
	client->client_id = client_id;
	client->mode = mode;
	client->speed = speed;
}

static bool is_client_id_exist(struct udk_spi_core *core,
			    unsigned int client_id)
{
	struct udk_spi_client *pos = NULL;

	dlist_for_each_entry(pos, &core->clients, struct udk_spi_client, node) {
		if (pos->client_id == client_id) {
			return true;
		}
	}

	return false;
}

int udk_spi_add_client(unsigned int adapter_id,
		       unsigned int mode, unsigned int speed, unsigned int client_id)
{
	int err = 0;
	struct udk_spi_core *core = NULL;
	struct udk_spi_client *client = NULL;

	udk_spi_mgr_lock();
	core = spi_core_lookup(adapter_id);
	if (core == NULL) {
		udk_warn("could not find spi core %d\n", adapter_id);
		udk_spi_mgr_unlock();
		return -EINVAL;
	}

	if (client_id >= core->adapter->client_max) {
		udk_warn("spi client_id check failed\n");
		udk_spi_mgr_unlock();
		return -EINVAL;
	}

	if (is_client_id_exist(core, client_id)) {
		udk_warn("spi client_id already exists\n");
		udk_spi_mgr_unlock();
		return -EINVAL;
	}

	client = (struct udk_spi_client *)udk_malloc(sizeof(*client));
	if (client == NULL) {
		udk_error("spi client malloc failed\n");
		udk_spi_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*client);

	spi_set_client_info(client, adapter_id, client_id, mode, speed);
	dlist_insert(&core->clients, &client->node);

	udk_spi_mgr_unlock();

	return err;
}

int udk_spi_remove_client(unsigned int adapter_id, unsigned int client_id)
{
	struct udk_spi_core *core = NULL;
	struct udk_spi_client *pos = NULL;
	struct udk_spi_client *next = NULL;

	udk_spi_mgr_lock();
	core = spi_core_lookup(adapter_id);
	if (core == NULL) {
		udk_warn("could not find spi core %d\n", adapter_id);
		udk_spi_mgr_unlock();
		return -EINVAL;
	}
	dlist_for_each_entry_safe(pos, next, &core->clients,
				struct udk_spi_client, node) {
		if (pos->client_id == client_id && !pos->is_opened) {
			dlist_delete(&pos->node);
			udk_free(pos);
			udk_spi_mgr_unlock();

			return 0;
		}
	}
	udk_spi_mgr_unlock();

	return -EFAULT;
}

static struct udk_spi_client *find_client(struct udk_spi_core *core,
					  unsigned int client_id)
{
	struct udk_spi_client *pos = NULL;

	dlist_for_each_entry(pos, &core->clients, struct udk_spi_client, node) {
		if (pos->client_id == client_id) {
			return pos;
		}
	}

	return NULL;
}

struct udk_spi_client *udk_spi_open_client(unsigned int adapter_id, unsigned int client_id)
{
	int err = 0;
	struct udk_spi_core *core = NULL;
	struct udk_spi_client *client = NULL;

	udk_spi_mgr_lock();
	core = spi_core_lookup(adapter_id);
	if (core == NULL) {
		udk_warn("could not find spi core %u\n", adapter_id);
		udk_spi_mgr_unlock();
		return NULL;
	}

	client = find_client(core, client_id);
	if (client == NULL || client->is_opened) {
		udk_warn("could not find spi client %u\n", client_id);
		udk_spi_mgr_unlock();
		return NULL;
	}

	err = core->adapter->ops->init(core->adapter);
	if (err != 0) {
		udk_warn("spi adapter %u init fail\n", adapter_id);
		udk_spi_mgr_unlock();

		return NULL;
	}

	client->is_opened = true;
	udk_spi_mgr_unlock();

	return client;
}

int udk_spi_close_client(struct udk_spi_client *client)
{
	int err = 0;
	struct udk_spi_core *core = NULL;

	if (client == NULL || !client->is_opened) {
		udk_warn("could not close spi client\n");
		return -EINVAL;
	}

	udk_spi_mgr_lock();
	core = spi_core_lookup(client->adapter_id);
	if (core == NULL || core->adapter == NULL ||
	    core->adapter->ops == NULL ||
	    core->adapter->ops->deinit == NULL) {
		udk_warn("could not get spi core %u \n", client->adapter_id);
		udk_spi_mgr_unlock();
		return -EINVAL;
	}

	err = core->adapter->ops->deinit(core->adapter);
	if (err != 0) {
		udk_warn("spi adapter %u deinit fail\n", client->adapter_id);
		udk_spi_mgr_unlock();

		return -EFAULT;
	}

	client->is_opened = false;
	udk_spi_mgr_unlock();

	return 0;
}

static inline void spi_set_para(struct udk_spi_trans_para *para,
				unsigned int client_id,
				unsigned int mode,
				unsigned int speed,
				unsigned int flags)
{
	para->client_id = client_id;
	para->mode = mode;
	para->speed = speed;
	para->flags = flags;
}

static int spi_transfer(const struct udk_spi_client *client, unsigned char *wbuf,
			unsigned char *rbuf, unsigned long bit_len, unsigned int flags)
{
	int err;
	struct udk_spi_trans_para para;
	struct udk_spi_core *core = NULL;
	struct udk_spi_adapter *adapter = NULL;

	udk_spi_mgr_lock();
	core = spi_core_lookup(client->adapter_id);
	if (core == NULL) {
		udk_warn("could not find spi core %u\n", client->adapter_id);
		udk_spi_mgr_unlock();
		return -EINVAL;
	}
	adapter = core->adapter;
	if (adapter == NULL || adapter->ops == NULL ||
	    adapter->ops->adapter_transfer == NULL) {
		udk_spi_mgr_unlock();
		return -EINVAL;
	}
	mem_zero_s(para);
	spi_set_para(&para, client->client_id, client->mode, client->speed, flags);
	err = adapter->ops->adapter_transfer(adapter, &para, wbuf, rbuf, bit_len);
	if (err != 0) {
		udk_warn("spi transfer failed: %s\n", strerror(err));
		udk_spi_mgr_unlock();
		return err;
	}
	udk_spi_mgr_unlock();

	return 0;
}

int udk_spi_write(const struct udk_spi_client *client,
		  unsigned char *wbuf, unsigned long bit_len)
{
	if (client == NULL || wbuf == NULL ||
	    bit_len == 0) {
		return -EINVAL;
	}

	return spi_transfer(client, wbuf, NULL, bit_len, 0);
}

int udk_spi_read(const struct udk_spi_client *client, unsigned char *wbuf,
		 unsigned char *rbuf, unsigned long bit_len, unsigned int flags)
{
	if (client == NULL || wbuf == NULL ||
	    rbuf == NULL || bit_len == 0) {
		return -EINVAL;
	}

	return spi_transfer(client, wbuf, rbuf, bit_len, flags);
}

static int udk_spi_init(void)
{
	udk_mutex_init(&spi_manager.lock);

	return 0;
}
udk_init_call(udk_spi_init);

#else /* CONFIG_UDK_SPI */

int udk_spi_add_adapter(struct udk_spi_adapter *adapter)
{
	UNUSED(adapter);

	return -ENOSYS;
}

int udk_spi_remove_adapter(const struct udk_spi_adapter *adapter)
{
	UNUSED(adapter);

	return -ENOSYS;
}

int udk_spi_add_client(unsigned int adapter_id,
		       unsigned int mode, unsigned int speed, unsigned int client_id)
{
	UNUSED(adapter_id, mode, speed, client_id);

	return -ENOSYS;
}

int udk_spi_remove_client(unsigned int adapter_id, unsigned int client_id)
{
	UNUSED(adapter_id, client_id);

	return -ENOSYS;
}

struct udk_spi_client *udk_spi_open_client(unsigned int adapter_id, unsigned int client_id)
{
	UNUSED(adapter_id, client_id);

	return NULL;
}

int udk_spi_close_client(struct udk_spi_client *client)
{
	UNUSED(client);

	return -ENOSYS;
}

int udk_spi_write(const struct udk_spi_client *client,
		  unsigned char *wbuf, unsigned long bit_len)
{
	UNUSED(client, wbuf, bit_len);

	return -ENOSYS;
}

int udk_spi_read(const struct udk_spi_client *client, unsigned char *wbuf,
		 unsigned char *rbuf, unsigned long bit_len, unsigned int flags)
{
	UNUSED(client, wbuf, rbuf, bit_len, flags);

	return -ENOSYS;
}

#endif /* CONFIG_UDK_SPI */
