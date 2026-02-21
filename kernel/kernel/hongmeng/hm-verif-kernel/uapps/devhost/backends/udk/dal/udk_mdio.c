/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK Management Data Input/Output bus framework
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 15 14:45:34 2021
 */

#include <udk/mdio.h>

#include <limits.h>

#include <udk/log.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>
#include <udk/lib/string.h>
#include <internal/init.h>

struct udk_mdio_manager {
	struct dlist_node head;
	struct udk_mutex lock;
};

struct udk_mdio {
	struct udk_mdio_desc *desc;
	int refcnt;
	struct udk_mutex lock;
	struct dlist_node node;
};

static struct udk_mdio_manager mdio_manager;

static inline void mdio_mgr_lock(void)
{
	udk_mutex_lock(&mdio_manager.lock);
}

static inline void mdio_mgr_unlock(void)
{
	udk_mutex_unlock(&mdio_manager.lock);
}

static inline void mdio_lock(struct udk_mdio *mdio)
{
	udk_mutex_lock(&mdio->lock);
}

static inline void mdio_unlock(struct udk_mdio *mdio)
{
	udk_mutex_unlock(&mdio->lock);
}

static struct udk_mdio *mdio_lookup(const char *name)
{
	struct udk_mdio *pos = NULL;

	dlist_for_each_entry(pos, &mdio_manager.head, struct udk_mdio, node) {
		if (pos->desc->name != NULL &&
		    strcmp(pos->desc->name, name) == 0) {
			return pos;
		}
	}

	return NULL;
}

struct udk_mdio *udk_mdio_get(const char *name)
{
	struct udk_mdio *mdio = NULL;

	if (name == NULL || name[0] == '\0') {
		udk_warn("invalid mdio name\n");
		return NULL;
	}
	mdio_mgr_lock();
	mdio = mdio_lookup(name);
	if (mdio == NULL) {
		udk_warn("can not find mdio\n");
		mdio_mgr_unlock();
		return NULL;
	}
	mdio_lock(mdio);
	if (mdio->refcnt >= INT_MAX) {
		udk_warn("mdio count overflow\n");
		mdio_unlock(mdio);
		mdio_mgr_unlock();
		return NULL;
	}
	mdio->refcnt++;
	mdio_unlock(mdio);
	mdio_mgr_unlock();

	return mdio;
}

int udk_mdio_put(struct udk_mdio *mdio)
{
	if (mdio == NULL) {
		udk_warn("invalid mdio\n");
		return -EINVAL;
	}
	mdio_lock(mdio);
	if (mdio->refcnt > 0) {
		mdio->refcnt--;
	}
	mdio_unlock(mdio);

	return 0;
}

static int mdio_reset(struct udk_mdio_desc *desc)
{
	if (desc->ops.reset == NULL) {
		return -ENOSYS;
	}

	return desc->ops.reset(desc);
}

int udk_mdio_reset(struct udk_mdio *mdio)
{
	int err;

	if (mdio == NULL || mdio->desc == NULL) {
		udk_warn("invalid mdio\n");
		return -EINVAL;
	}
	mdio_lock(mdio);
	err = mdio_reset(mdio->desc);
	mdio_unlock(mdio);

	return err;
}

int udk_mdio_read(struct udk_mdio *mdio, unsigned int id,
		  unsigned int reg)
{
	int err = -ENOSYS;

	if (mdio == NULL || mdio->desc == NULL) {
		udk_warn("invalid mdio\n");
		return -EINVAL;
	}
	mdio_lock(mdio);
	if (mdio->desc->ops.read != NULL) {
		err = mdio->desc->ops.read(mdio->desc, id, reg);
	}
	mdio_unlock(mdio);

	return err;
}

int udk_mdio_write(struct udk_mdio *mdio, unsigned int id,
		   unsigned int reg, uint16_t val)
{
	int err = -ENOSYS;

	if (mdio == NULL || mdio->desc == NULL) {
		udk_warn("invalid mdio\n");
		return -EINVAL;
	}
	mdio_lock(mdio);
	if (mdio->desc->ops.write != NULL) {
		err = mdio->desc->ops.write(mdio->desc, id, reg, val);
	}
	mdio_unlock(mdio);

	return err;
}

int udk_mdio_register(struct udk_mdio_desc *desc)
{
	struct udk_mdio *mdio = NULL;
	int err;

	if (desc == NULL ||
	    desc->name == NULL || desc->name[0] == '\0') {
		udk_warn("invalid descriptor\n");
		return -EINVAL;
	}
	mdio_mgr_lock();
	if (mdio_lookup(desc->name) != NULL) {
		udk_warn("mdio is already exist\n");
		mdio_mgr_unlock();
		return -EEXIST;
	}
	err = mdio_reset(desc);
	if (err < 0) {
		udk_error("mdio reset failed:%s \n", strerror(-err));
		mdio_mgr_unlock();
		return err;
	}
	mdio = (struct udk_mdio *)udk_malloc(sizeof(*mdio));
	if (mdio == NULL) {
		udk_error("mdio malloc failed\n");
		mdio_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*mdio);
	mdio->desc = desc;
	udk_mutex_init(&mdio->lock);
	dlist_insert(&mdio_manager.head, &mdio->node);
	mdio_mgr_unlock();

	return 0;
}

int udk_mdio_unregister(struct udk_mdio_desc *desc)
{
	struct udk_mdio *mdio = NULL;

	if (desc == NULL ||
	    desc->name == NULL || desc->name[0] == '\0') {
		return -EINVAL;
	}
	mdio_mgr_lock();
	mdio = mdio_lookup(desc->name);
	if (mdio == NULL || mdio->desc != desc) {
		mdio_mgr_unlock();
		return -EINVAL;
	}
	mdio_lock(mdio);
	if (mdio->refcnt > 0) {
		mdio_unlock(mdio);
		mdio_mgr_unlock();
		return -EBUSY;
	}
	dlist_delete(&mdio->node);
	mdio_unlock(mdio);
	udk_free(mdio);
	mdio_mgr_unlock();

	return 0;
}

static int udk_mdio_init(void)
{
	udk_mutex_init(&mdio_manager.lock);
	dlist_init(&mdio_manager.head);

	return 0;
}
udk_init_call(udk_mdio_init);
