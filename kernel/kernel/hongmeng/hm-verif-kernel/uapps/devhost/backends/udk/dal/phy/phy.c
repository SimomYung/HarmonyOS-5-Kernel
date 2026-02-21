/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK physical layer framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 16 10:32:02 2021
 */
#include <udk/phy.h>

#include <limits.h>

#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>
#include <udk/lib/string.h>
#include <internal/phy.h>
#include <internal/init.h>

struct udk_phy {
	struct udk_phy_desc *desc;
	unsigned int refcnt;
	struct dlist_node node;
	struct udk_mutex lock;
};

struct udk_phy_manager {
	struct dlist_node head;
	struct udk_mutex lock;
};

static struct udk_phy_manager phy_manager;

static inline void phy_mgr_lock(void)
{
	udk_mutex_lock(&phy_manager.lock);
}

static inline void phy_mgr_unlock(void)
{
	udk_mutex_unlock(&phy_manager.lock);
}

void phy_lock(struct udk_phy *phy)
{
	udk_mutex_lock(&phy->lock);
}

void phy_unlock(struct udk_phy *phy)
{
	udk_mutex_unlock(&phy->lock);
}

static struct udk_phy *phy_lookup(const char *name)
{
	struct udk_phy *pos = NULL;

	dlist_for_each_entry(pos, &phy_manager.head, struct udk_phy, node) {
		if ((pos->desc->name != NULL) &&
		    (strcmp(pos->desc->name, name)) == 0) {
			return pos;
		}
	}

	return NULL;
}

static inline bool is_phy_valid(const struct udk_phy *phy)
{
	return ((phy != NULL) && (phy->desc != NULL));
}

struct udk_phy *udk_phy_get(const char *name)
{
	struct udk_phy *phy = NULL;

	if ((name == NULL) || (name[0] == '\0')) {
		return NULL;
	}
	phy_mgr_lock();
	phy = phy_lookup(name);
	if (phy == NULL) {
		phy_mgr_unlock();
		return NULL;
	}
	phy_lock(phy);
	if (phy->refcnt >= INT_MAX) {
		udk_warn("phy references overflow\n");
		phy_unlock(phy);
		phy_mgr_unlock();
		return NULL;
	}
	phy->refcnt++;
	phy_unlock(phy);
	phy_mgr_unlock();

	return phy;
}

int udk_phy_put(struct udk_phy *phy)
{
	if (!is_phy_valid(phy)) {
		return -EINVAL;
	}
	phy_lock(phy);
	if (phy->refcnt == 0) {
		udk_warn("put no references phy\n");
		phy_unlock(phy);
		return -EINVAL;
	}
	phy->refcnt--;
	phy_unlock(phy);

	return 0;
}

int udk_phy_register(struct udk_phy_desc *desc)
{
	struct udk_phy *phy = NULL;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("invalid phy\n");
		return -EINVAL;
	}
	phy_mgr_lock();
	if (phy_lookup(desc->name) != NULL) {
		udk_warn("phy is already exist\n");
		phy_mgr_unlock();
		return -EEXIST;
	}
	phy = (struct udk_phy *)udk_malloc(sizeof(struct udk_phy));
	if (phy == NULL) {
		udk_error("phy alloc failed\n");
		phy_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*phy);
	phy->desc = desc;
	udk_mutex_init(&phy->lock);
	dlist_insert(&phy_manager.head, &phy->node);
	phy_mgr_unlock();

	return 0;
}

int udk_phy_unregister(const struct udk_phy_desc *desc)
{
	struct udk_phy *phy = NULL;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("invalid phy\n");
		return -EINVAL;
	}
	phy_mgr_lock();
	phy = phy_lookup(desc->name);
	if ((phy == NULL) || (phy->desc != desc)) {
		phy_mgr_unlock();
		return -EINVAL;
	}
	phy_lock(phy);
	if (phy->refcnt > 0) {
		phy_unlock(phy);
		phy_mgr_unlock();
		return -EBUSY;
	}
	dlist_delete(&phy->node);
	phy_unlock(phy);
	udk_free(phy);
	phy_mgr_unlock();

	return 0;
}

#define PHY_OPS_COMMON_DEFINE(name)		\
int udk_phy_##name(struct udk_phy *phy)		\
{						\
	int err;				\
						\
	if (!is_phy_valid(phy)) {		\
		return -EINVAL;			\
	}					\
	if (phy->desc->ops.name == NULL) {	\
		return -ENOSYS;			\
	}					\
	phy_lock(phy);				\
	err = phy->desc->ops.name(phy->desc);	\
	if (err < 0) {				\
		phy_unlock(phy);		\
		return err;			\
	}					\
	phy_unlock(phy);			\
						\
	return 0;				\
}

PHY_OPS_COMMON_DEFINE(init);
PHY_OPS_COMMON_DEFINE(exit);
PHY_OPS_COMMON_DEFINE(reset);
PHY_OPS_COMMON_DEFINE(start);
PHY_OPS_COMMON_DEFINE(stop);

int udk_phy_read(struct udk_phy *phy, unsigned int reg)
{
	int val;

	if (!is_phy_valid(phy)) {
		return -EINVAL;
	}
	if (phy->desc->ops.read == NULL) {
		return -ENOSYS;
	}
	phy_lock(phy);
	val = phy->desc->ops.read(phy->desc, reg);
	phy_unlock(phy);

	return val;
}

int udk_phy_write(struct udk_phy *phy, unsigned int reg, unsigned int val)
{
	int err = 0;

	if (!is_phy_valid(phy)) {
		return -EINVAL;
	}
	if (phy->desc->ops.write == NULL) {
		return -ENOSYS;
	}
	phy_lock(phy);
	err = phy->desc->ops.write(phy->desc, reg, val);
	if (err < 0) {
		phy_unlock(phy);
		return err;
	}
	phy_unlock(phy);

	return 0;
}

int udk_phy_set_mode(struct udk_phy *phy, enum udk_phy_mode mode,
		     int submode)
{
	int err = 0;

	if (!is_phy_valid(phy)) {
		return -EINVAL;
	}
	if (phy->desc->ops.set_mode == NULL) {
		return -ENOSYS;
	}
	phy_lock(phy);
	err = phy->desc->ops.set_mode(phy->desc, mode, submode);
	if (err < 0) {
		phy_unlock(phy);
		return err;
	}
	phy_unlock(phy);

	return 0;
}

struct udk_phy_desc *udk_phy_desc_of(struct udk_phy *phy)
{
	if (phy == NULL) {
		return NULL;
	}

	return phy->desc;
}

static int phy_init(void)
{
	udk_mutex_init(&phy_manager.lock);
	dlist_init(&phy_manager.head);

	return 0;
}
udk_init_call(phy_init);
