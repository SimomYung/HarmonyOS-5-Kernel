/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/ethtool.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 15:16:23 2021
 */

#ifndef _LINUX_ETHTOOL_H
#define _LINUX_ETHTOOL_H

#include <linux/types.h>

struct ethtool_cmd {
	__u32	cmd;
	__u32	supported;
	__u32	advertising;
	__u16	speed;
	__u8	duplex;
	__u8	port;
	__u8	phy_address;
	__u8	transceiver;
	__u8	autoneg;
	__u8	mdio_support;
	__u32	maxtxpkt;
	__u32	maxrxpkt;
	__u16	speed_hi;
	__u8	eth_tp_mdix;
	__u8	eth_tp_mdix_ctrl;
	__u32	lp_advertising;
	__u32	reserved[2];
};

#define ETHTOOL_GSET		0x00000001

#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01

#endif
