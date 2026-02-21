/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: platform csi2if interface
 *
 * This file is released under the GPLv2.
 */
#ifndef PLAT_CSI2IF_H
#define PLAT_CSI2IF_H
#include <linux/types.h>

struct csi_phy_info;

enum idi_phy_workmode_e {
	IDI_PHY_WORKMODE_SINGLE,
	IDI_PHY_WORKMODE_DUAL_SENSORA,
	IDI_PHY_WORKMODE_DUAL_SENSORB,
	IDI_PHY_WORKMODE_MAX,
};

enum idi_phy_mode_e {
	IDI_PHY_MODE_DPHY,
	IDI_PHY_MODE_CPHY,
	IDI_PHY_MODE_MAX,
};

struct csi_phy_cfg_t {
	uint8_t addr;
	uint8_t value;
};

void plat_csi2if_irq_handler(int irq, void *dev_id);
int plat_csi2if_enable(struct csi_phy_info *info);
int plat_csi2if_disable(struct csi_phy_info *info);
void plat_dump_csi_status(struct csi_phy_info *info);
void plat_csi2if_dump_debug_info(struct csi_phy_info *info);
#endif /* end of include guard: PLAT_CSI2IF_H */
