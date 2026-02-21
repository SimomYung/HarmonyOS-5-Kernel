/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Structures and defines of ethtool ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 22 10:46:12 2020
 */

#ifndef __UDK_ETHTOOL_H__
#define __UDK_ETHTOOL_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <udk/net/netdev.h>

/*
 * All structures and macros should be compatible with ethtool-5.4/ethtool-copy.h
 */
#define SIOCETHTOOL	0x8946
#define ETHTOOL_GSET		0x00000001 /* Get settings. */
#define ETHTOOL_SSET		0x00000002 /* Set settings. */
#define ETHTOOL_GET_DRVINFO	0x00000003 /* Get driver info. */
#define ETHTOOL_NWAY_RST        0x00000009 /* Restart autonegotiation. */

enum udk_ethtool_nameset {
	SELFTEST_NAME = 0,
	STATS_NAME,
	PRIVFLAG_NAME,
};

#define SPEED_10                10
#define SPEED_100               100
#define SPEED_1000              1000
#define SPEED_UNKNOWN           -1

/* Duplex, half or full. */
#define DUPLEX_HALF             0x00
#define DUPLEX_FULL             0x01
#define DUPLEX_UNKNOWN          0xff

/* Enable or disable autonegotiation. */
#define AUTONEG_DISABLE         0x00
#define AUTONEG_ENABLE          0x01

#define UDK_ETHTOOL_CMD_SPEED_LOW_MASK		(0xffffU)
#define UDK_ETHTOOL_CMD_SPEED_HIGH_SHIFT	(0x10U)

struct udk_ethtool_cmd {
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

static inline void udk_ethtool_set_cmd_speed(struct udk_ethtool_cmd *cmd, __u32 speed)
{
	cmd->speed = speed & UDK_ETHTOOL_CMD_SPEED_LOW_MASK;
	cmd->speed_hi = speed >> UDK_ETHTOOL_CMD_SPEED_HIGH_SHIFT;
}

static inline __u32 udk_ethtool_speed_of_cmd(struct udk_ethtool_cmd *cmd)
{
	return (((__u32)cmd->speed_hi << UDK_ETHTOOL_CMD_SPEED_HIGH_SHIFT) | cmd->speed);
}

#define ETHTOOL_DRIVER_NAME_LEN		32
#define ETHTOOL_DRIVER_VERSION_LEN	32
#define ETHTOOL_FIRWARE_VERSION_LEN	32
#define ETHTOOL_BUS_ADDR_LEN		32
#define ETHTOOL_EROM_VERSION_LEN	32
#define ETHTOOL_UNUSED_BUF_LEN		12

struct udk_ethtool_drvinfo {
	uint32_t	cmd;
	char		drv_name[ETHTOOL_DRIVER_NAME_LEN];
	char		drv_version[ETHTOOL_DRIVER_VERSION_LEN];
	char		fw_version[ETHTOOL_FIRWARE_VERSION_LEN];
	char		bus_addr[ETHTOOL_BUS_ADDR_LEN];
	char		erom_version[ETHTOOL_EROM_VERSION_LEN];
	char		unused[ETHTOOL_UNUSED_BUF_LEN];
	uint32_t	privflag_num;
	uint32_t	stats_num;
	uint32_t	selftest_num;
	uint32_t	eeprom_datalen;
	uint32_t	regs_datalen;
};

struct udk_ethtool_link_settings {
	__u32	cmd;
	__u32	speed;
	__u8	duplex;
	__u8	port;
	__u8	phy_address;
	__u8	autoneg;
	__u8	mdio_support;
	__u8	eth_tp_mdix;
	__u8	eth_tp_mdix_ctrl;
	__s8	link_mode_masks_nwords;
	__u8	transceiver;
	__u8	reserved1[3];
	__u32	reserved[7];
	__u32	link_mode_masks[];
        /* layout of link_mode_masks fields:
         * __u32 map_supported[link_mode_masks_nwords];
         * __u32 map_advertising[link_mode_masks_nwords];
         * __u32 map_lp_advertising[link_mode_masks_nwords];
         */
};

#define UDK_ETHTOOL_LINK_MODE_SIZE 10

struct udk_ethtool_link_ksettings {
	struct udk_ethtool_link_settings base;
	/* now only supported bit < 31 link_modes */
	struct {
		__u32 supported[UDK_ETHTOOL_LINK_MODE_SIZE];
		__u32 advertising[UDK_ETHTOOL_LINK_MODE_SIZE];
		__u32 lp_advertising[UDK_ETHTOOL_LINK_MODE_SIZE];
	} link_modes;
	__u32	lanes;
};

int udk_net_call_ethtool(struct udk_net_device *pnet_dev, struct ifreq *ifr);
#endif
