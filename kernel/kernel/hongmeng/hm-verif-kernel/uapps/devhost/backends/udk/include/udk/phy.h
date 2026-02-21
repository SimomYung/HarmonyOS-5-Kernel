/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK physical layer framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 16 10:32:02 2021
 */
#ifndef __UDK_PHY_H__
#define __UDK_PHY_H__

#include <udk/errno.h>
#include <libstrict/strict.h>

struct udk_phy;
struct udk_phy_desc;

enum udk_phy_mode {
	UDK_PHY_MODE_INVALID = -1,
	UDK_PHY_MODE_ETHERNET,
	UDK_PHY_MODE_MAX,
};

struct udk_phy_ops {
	int (*init)(struct udk_phy_desc *desc);
	int (*exit)(struct udk_phy_desc *desc);
	int (*stop)(struct udk_phy_desc *desc);
	int (*start)(struct udk_phy_desc *desc);
	int (*reset)(struct udk_phy_desc *desc);
	int (*read)(struct udk_phy_desc *desc, unsigned int reg);
	int (*write)(struct udk_phy_desc *desc, unsigned int reg,
		     unsigned int val);
	int (*set_mode)(struct udk_phy_desc *desc, enum udk_phy_mode mode,
			int submode);
};

struct udk_phy_desc {
	const char *name;
	struct udk_phy_ops ops;
};

static inline bool is_phy_desc_valid(const struct udk_phy_desc *desc)
{
	return ((desc != NULL) && (desc->name != NULL) &&
		(desc->name[0] != '\0'));
}

#ifdef CONFIG_UDK_PHY

int udk_phy_register(struct udk_phy_desc *desc);
int udk_phy_unregister(const struct udk_phy_desc *desc);

struct udk_phy *udk_phy_get(const char *name);
int udk_phy_put(struct udk_phy *phy);
int udk_phy_init(struct udk_phy *phy);
int udk_phy_exit(struct udk_phy *phy);
int udk_phy_reset(struct udk_phy *phy);
int udk_phy_start(struct udk_phy *phy);
int udk_phy_stop(struct udk_phy *phy);
int udk_phy_read(struct udk_phy *phy, unsigned int reg);
int udk_phy_write(struct udk_phy *phy, unsigned int reg,
		  unsigned int val);
int udk_phy_set_mode(struct udk_phy *phy, enum udk_phy_mode mode,
		     int submode);

#else

static inline int udk_phy_register(struct udk_phy_desc *desc)
{
	UNUSED(desc);

	return -ENOSYS;
}

static inline int udk_phy_unregister(const struct udk_phy_desc *desc)
{
	UNUSED(desc);

	return -ENOSYS;
}

static inline struct udk_phy *udk_phy_get(const char *name)
{
	UNUSED(name);

	return NULL;
}

static inline int udk_phy_put(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_init(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_exit(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_reset(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_start(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_stop(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

static inline int udk_phy_read(struct udk_phy *phy, unsigned int reg)
{
	UNUSED(phy, reg);

	return -ENOSYS;
}

static inline int udk_phy_write(struct udk_phy *phy, unsigned int reg,
				unsigned int val)
{
	UNUSED(phy, reg, val);

	return -ENOSYS;
}

static inline int udk_phy_set_mode(struct udk_phy *phy, enum udk_phy_mode mode,
				   int submode)
{
	UNUSED(phy, mode, submode);

	return -ENOSYS;
}

#endif

#endif
