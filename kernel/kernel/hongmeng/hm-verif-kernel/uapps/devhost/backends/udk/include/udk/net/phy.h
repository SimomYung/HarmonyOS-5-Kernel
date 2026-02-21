/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Network physical layer management interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 05 15:18:56 2022
 */
#ifndef __UDK_NET_PHY_H__
#define __UDK_NET_PHY_H__

#include <stdbool.h>
#include <udk/phy.h>
#include <udk/mdio.h>
#include <udk/errno.h>
#include <udk/net/netdev.h>
#include <libstrict/strict.h>

struct udk_netphy;

enum udk_netphy_mode {
	UDK_NETPHY_MODE_NA = 0,
	UDK_NETPHY_MODE_MII,
	UDK_NETPHY_MODE_GMII,
	UDK_NETPHY_MODE_SGMII,
	UDK_NETPHY_MODE_MAX,
};

struct udk_netphy_ops {
	int (*config_init)(struct udk_netphy *phy);
};

struct udk_netphy {
	struct udk_phy_desc desc;
	unsigned int id;
	int speed;
	int duplex;
	unsigned autoneg:1;
	unsigned int supported;
	unsigned int advertising;
	unsigned int lp_advertising;
	struct udk_mdio *mdio;
	struct udk_netphy_ops ops;
	enum udk_netphy_mode mode;
};

typedef void (*udk_netphy_adjust_link_func)(struct udk_net_device *);

#ifdef CONFIG_UDK_NETPHY

int udk_netphy_register(struct udk_netphy *phy);
int udk_netphy_unregister(struct udk_netphy *phy);
struct udk_phy *udk_netphy_connect(struct udk_net_device *netdev,
				   const char *name,
				   udk_netphy_adjust_link_func adjust_link,
				   enum udk_netphy_mode mode);
int udk_netphy_disconnect(struct udk_phy *phy);
bool udk_netphy_is_linked(struct udk_phy *phy);
int udk_netphy_set_link_settings(struct udk_phy *phy, const struct udk_ethtool_link_ksettings *ksetting);
int udk_netphy_get_link_settings(struct udk_phy *phy, struct udk_ethtool_link_ksettings *ksetting);

#else

static inline int udk_netphy_register(struct udk_netphy *phy)
{
	UNUSED(phy);
	return -ENOSYS;
}

static inline int udk_netphy_unregister(struct udk_netphy *phy)
{
	UNUSED(phy);
	return -ENOSYS;
}

static inline
struct udk_phy *udk_netphy_connect(struct udk_net_device *netdev,
				   const char *name,
				   udk_netphy_adjust_link_func adjust_link,
				   enum udk_netphy_mode mode)
{
	UNUSED(netdev, name, adjust_link, mode);

	return NULL;
}

static inline int udk_netphy_disconnect(struct udk_phy *phy)
{
	UNUSED(phy);

	return -ENOSYS;
}

bool udk_netphy_is_linked(struct udk_phy *phy)
{
	UNUSED(phy);

	return false;
}

static inline int
udk_netphy_set_link_settings(struct udk_phy *phy, const struct udk_ethtool_link_ksettings *ksetting)
{
	UNUSED(phy, ksetting);

	return -ENOSYS;
}

static inline int
udk_netphy_get_link_settings(struct udk_phy *phy, const struct udk_ethtool_link_ksettings *ksetting)
{
	UNUSED(phy, ksetting);

	return -ENOSYS;
}
#endif

#endif /* __UDK_NET_PHY_H__ */
