/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Ethtool operations in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 22 10:45:12 2020
 */

#include <net/if.h>
#include <udk/net/ethtool.h>
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/security.h>
#include <libhmactv/actv.h>
#include <hongmeng/errno.h>
#include <libsec/posix_cap.h>

static int get_ethtool_drvinfo(struct udk_net_device *dev, void *data)
{
	struct udk_ethtool_drvinfo info;
	const struct udk_ethtool_ops *ops = dev->ethtool_ops;
	int ret;

	mem_zero_s(info);
	info.cmd = ETHTOOL_GET_DRVINFO;
	if (ops != NULL && ops->get_driver_info != NULL) {
		ops->get_driver_info(dev, &info);
	} else {
		return -EOPNOTSUPP;
	}

	if (ops != NULL && ops->get_info_nums != NULL) {
		ret = ops->get_info_nums(dev, (int)SELFTEST_NAME);
		if (ret >= 0) {
			info.selftest_num = (uint32_t)ret;
		}
		ret = ops->get_info_nums(dev, (int)STATS_NAME);
		if (ret >= 0) {
			info.stats_num = (uint32_t)ret;
		}
		ret = ops->get_info_nums(dev, (int)PRIVFLAG_NAME);
		if (ret >= 0) {
			info.privflag_num = (uint32_t)ret;
		}
	}

	if (ops != NULL && ops->get_regs_datalen != NULL) {
		ret = ops->get_regs_datalen(dev);
		if (ret > 0) {
			info.regs_datalen = (uint32_t)ret;
		}
	}

	if (ops != NULL && ops->get_eeprom_datalen != NULL) {
		ret = ops->get_eeprom_datalen(dev);
		if (ret >= 0) {
			info.eeprom_datalen = (uint32_t)ret;
		}
	}

	ret = hm_actv_write_xact_vm(data, &info, sizeof(info));
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static void convert_ksetting_to_cmd(struct udk_ethtool_link_ksettings *ksettings,
				   struct udk_ethtool_cmd *cmd)
{
	mem_zero_s(*cmd);

	udk_ethtool_set_cmd_speed(cmd, ksettings->base.speed);
	cmd->duplex = ksettings->base.duplex;
	cmd->port = ksettings->base.port;
	cmd->phy_address = ksettings->base.phy_address;
	cmd->autoneg = ksettings->base.autoneg;

	/* not use bitmap now in udk */
	cmd->advertising = ksettings->link_modes.advertising[0];
	cmd->supported = ksettings->link_modes.supported[0];
	cmd->lp_advertising = ksettings->link_modes.lp_advertising[0];
}

static void convert_cmd_to_ksetting(struct udk_ethtool_link_ksettings *ksettings,
				    struct udk_ethtool_cmd *cmd)
{
	mem_zero_s(*ksettings);

	ksettings->base.speed = udk_ethtool_speed_of_cmd(cmd);
	ksettings->base.duplex = cmd->duplex;
	ksettings->base.port = cmd->port;
	ksettings->base.phy_address = cmd->phy_address;
	ksettings->base.autoneg = cmd->autoneg;

	/* not use bitmap now in udk */
	ksettings->link_modes.advertising[0] = cmd->advertising;
	ksettings->link_modes.supported[0] = cmd->supported;
	ksettings->link_modes.lp_advertising[0] = cmd->lp_advertising;
}

static int get_ethtool_link_settings(struct udk_net_device *dev, void *data)
{
	struct udk_ethtool_cmd cmd;
	struct udk_ethtool_link_ksettings ksettings;
	const struct udk_ethtool_ops *ops = dev->ethtool_ops;
	int ret;

	if (ops == NULL || ops->get_link_ksettings == NULL) {
		return -EOPNOTSUPP;
	}
	mem_zero_s(ksettings);
	ret = ops->get_link_ksettings(dev, &ksettings);
	if (ret < 0) {
		return ret;
	}

	convert_ksetting_to_cmd(&ksettings, &cmd);
	ret = udk_copy_to_user(data, &cmd, sizeof(cmd));
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static int set_ethtool_link_settings(struct udk_net_device *dev, void *data)
{
	struct udk_ethtool_cmd cmd;
	struct udk_ethtool_link_ksettings ksettings;
	const struct udk_ethtool_ops *ops = dev->ethtool_ops;
	int ret;

	if (ops == NULL || ops->set_link_ksettings == NULL) {
		return -EOPNOTSUPP;
	}
	ret = udk_copy_from_user(&cmd, data, sizeof(cmd));
	if (ret < 0) {
		return ret;
	}

	convert_cmd_to_ksetting(&ksettings, &cmd);
	ret = ops->set_link_ksettings(dev, &ksettings);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static int ethtool_nway_reset(struct udk_net_device *dev)
{
	const struct udk_ethtool_ops *ops = dev->ethtool_ops;
	int ret;

	if (ops == NULL || ops->nway_reset == NULL) {
		return -EOPNOTSUPP;
	}

	ret = ops->nway_reset(dev);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static bool ethtool_sec_chk(unsigned int cmd)
{
	switch(cmd) {
	case ETHTOOL_SSET:
		return udk_capable(POSIX_CAP_NET_ADMIN);
	default:
		return true;
	}

	return true;
}

int udk_net_call_ethtool(struct udk_net_device *dev, struct ifreq *ifr)
{
	int ret;
	uint32_t cmd = 0;
	void *ptr = NULL;

	if ((dev == NULL || ifr == NULL || ifr->ifr_data == NULL)) {
		return -EINVAL;
	}

	ptr = (void*)(ifr->ifr_data);

	ret = hm_actv_read_xact_vm(&cmd, ptr, sizeof(cmd));
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	if (!ethtool_sec_chk(cmd)) {
		udk_warn("caller has no permission to call command %u\n", cmd);
		return -EPERM;
	}
	if (dev->ethtool_ops != NULL && dev->ethtool_ops->pre != NULL) {
		ret = dev->ethtool_ops->pre(dev);
		if (ret < 0) {
			return ret;
		}
	}

	switch (cmd) {
	case ETHTOOL_GET_DRVINFO:
		ret = get_ethtool_drvinfo(dev, ptr);
		break;
	case ETHTOOL_GSET:
		ret = get_ethtool_link_settings(dev, ptr);
		break;
	case ETHTOOL_SSET:
		ret = set_ethtool_link_settings(dev, ptr);
		break;
	case ETHTOOL_NWAY_RST:
		ret = ethtool_nway_reset(dev);
		break;
	default:
		ret = -EOPNOTSUPP;
	}

	if (dev->ethtool_ops != NULL && dev->ethtool_ops->done != NULL) {
		dev->ethtool_ops->done(dev);
	}

	return ret;
}
