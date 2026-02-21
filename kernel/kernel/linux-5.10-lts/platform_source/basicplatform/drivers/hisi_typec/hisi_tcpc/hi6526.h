/*
 * Copyright (c) 2019 Huawei Technologies Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _HI6526_H_
#define _HI6526_H_

#include <linux/i2c.h>
#include "include/std_tcpci_v10.h"

/*
 * vendor defined
 */
#define REG_PD_VDM_CFG_0		0x7A
#define PD_DA_FRS_ENABLE		(1 << 0)
#define REG_PD_VDM_ENABLE		0x7B
#define REG_PD_VDM_CFG_1		0x7C
#define PD_SNK_DISC_BY_CC		(1 << 4)
#define PD_RX_PHY_SOFT_RESET		(1 << 3)
#define PD_TX_PHY_SOFT_RESET		(1 << 2)
#define PD_FSM_RESET			(1 << 1)
#define PD_FSM_RESET_VAL		(1 << 1)
#define PD_FSM_UNRESET_VAL		(0 << 1)
#define TC_FSM_RESET			1
#define PD_TC_ALL_RESET			(PD_RX_PHY_SOFT_RESET | \
					PD_TX_PHY_SOFT_RESET | \
					PD_FSM_RESET | TC_FSM_RESET)

#define REG_PD_DBG_RDATA_CFG		0x7D

#define PD_DBG_RDATA_MACHINE_STATUS	0x80
#define PD_DBG_RDATA_CC_STATUS		0x82
#define PD_DBG_RDATA_VBUS_STATUS	0x83
#define DA_VBUS_5V_EN_STATUS		(1 << 6)
#define PD_DBG_RDATA_RX_TX_STATUS	0x91

#define PD_DBG_MODULE_SEL(x)		(((x) & 0x7) << 4)
#define PD_DBG_RDATA_SEL(x)		((x) & 0xf)
#define REG_PD_DBG_RDATA		0x7E
#define TCPC_DBG_ACC			(1 << 6)
#define TCPC_PD_STATE			(1 << 5)
#define TCPC_TYPEC_STATE		(1 << 4)
#define PD_PHY_RX_STAT			(0x1 << 3)
#define REG_VDM_PAGE_SELECT 		0x7F
#define MASK_SNK_VBUS_DETECT		(0x1 << 6)
#define MASK_WDT_RST_PD_BIT		(0x1 << 6)
#define BIT_SC_BUCK_EN			(1 << 0)
#define PD_FRS_DETECT			(1 << 6)

#define PD_EXT_OTG_SEL_REG		(0x100 + 0x5A)
#define EXT_OTG_BIT			(0x1 << 0)
#define PD_EXT_OTG_EN_REG		(0x100 + 0x5B)
#define EXT_OTG_EN_BIT			(0x1 << 0)

#define CHIP_VERSION_V610	0xF3F6

enum {
	TCPC_CHIP_VERSION_V600,
	TCPC_CHIP_VERSION_V700,
	TCPC_CHIP_VERSION_V800,
	TCPC_CHIP_VERSION_INVALID,
};

struct hisi_tcpc_reg {
	u32 cdr_cfg_0;
	u32 dbg_cfg_1;
	u32 sc_buck_en;
	u32 irq_gbl_msk;
	u32 irq_msk_pd_bit;
	u32 abnormal_irq_sts;
	u32 abnormal_irq_msk;
	u32 cc_ovp_irq;
	u32 cc_ocp_irq;
	u32 cc_uvp_irq;
	u32 sbu1_ovp_irq;
	u32 sbu2_ovp_irq;
	u32 cfg_reg_1;
	u32 aux_sbu_en;
	u32 jtag_sbu_en;
};

int hisi_tcpc_block_read(struct regmap *map, u32 reg, int len, void *dst);
int hisi_tcpc_block_write(struct regmap *map, u32 reg, int len, void *src);
s32 hisi_tcpc_regmap_read8(struct regmap *map, u32 reg);
int hisi_tcpc_regmap_write8(struct regmap *map, u32 reg, u8 value);
int hisi_tcpc_regmap_update_bits(struct regmap *map, u32 reg, u8 mask, u8 val);
s32 hisi_tcpc_regmap_read16(struct regmap *map, u32 reg);
int hisi_tcpc_regmap_write16(struct regmap *map, u32 reg, u16 value);
bool hisi_tcpc_get_vusb_uv_det_sts(void);
unsigned int hisi_tcpc_get_chip_version(void);
int hi6526_get_irq_gpio(void);
int hisi_tcpc_get_reg_info(struct device_node *np, struct hisi_tcpc_reg *reg_info);

#endif
