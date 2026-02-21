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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/regmap.h>

#include <linux/platform_drivers/usb/hisi_tcpc_ops.h>
#include "hi6526.h"

static int hi6526_irq_gpio;

static struct hisi_tcpc_reg_ops *hi6526_tcpc_reg_ops = NULL;
static bool hi6526_vusb_uv_det_en_status = false;
static unsigned int g_hi6526_version = 0;

int hi6526_get_irq_gpio(void)
{
	return hi6526_irq_gpio;
}

/*
 * Return 0 on sucess, others for failed.
 */
int hisi_tcpc_block_read(struct regmap *map,        u32 reg, int len, void *dst)
{
	if (hi6526_tcpc_reg_ops && hi6526_tcpc_reg_ops->block_read)
		return hi6526_tcpc_reg_ops->block_read((u16)reg,
			(u8 *)dst, (unsigned int)len);

	return regmap_bulk_read(map, reg, dst, len);
}

/*
 * Return 0 on sucess, others for failed.
 */
int hisi_tcpc_block_write(struct regmap *map, u32 reg, int len, void *src)
{
	if (hi6526_tcpc_reg_ops && hi6526_tcpc_reg_ops->block_write)
		return hi6526_tcpc_reg_ops->block_write((u16)reg,
			(u8 *)src, (unsigned int)len);

	return regmap_bulk_write(map, reg, src, len);
}

s32 hisi_tcpc_regmap_read8(struct regmap *map, u32 reg)
{
	unsigned int value = 0;
	int ret;

	ret = regmap_read(map, reg, &value);
	if (ret < 0)
		return (s32)ret;
	else
		return (s32)value;
}

int hisi_tcpc_regmap_write8(struct regmap *map, u32 reg, u8 value)
{
	unsigned int data = value;

	return regmap_write(map, reg, data);
}

int hisi_tcpc_regmap_update_bits(struct regmap *map, u32 reg, u8 mask, u8 val)
{
	return regmap_update_bits(map, reg, mask, val);
}

s32 hisi_tcpc_regmap_read16(struct regmap *map, u32 reg)
{
	unsigned int value = 0;
	int ret;

	if (hi6526_tcpc_reg_ops && hi6526_tcpc_reg_ops->block_read)
		ret = hi6526_tcpc_reg_ops->block_read((u16)reg, (u8 *)&value, 2); /* 2: buf */
	else
		ret = regmap_bulk_read(map, reg, &value, 2);
	if (ret < 0)
		return (s32)ret;
	else
		return (s32)(value);
}

int hisi_tcpc_regmap_write16(struct regmap *map, u32 reg, u16 value)
{
	unsigned int data = value;

	if (hi6526_tcpc_reg_ops && hi6526_tcpc_reg_ops->block_write)
		return hi6526_tcpc_reg_ops->block_write((u16)reg, (u8 *)&data, 2); /* 2: buf */

	return regmap_bulk_write(map, reg, (u8 *)&data, 2);
}

void hisi_tcpc_reg_ops_register(struct hisi_tcpc_reg_ops *reg_ops)
{
	hi6526_tcpc_reg_ops = reg_ops;
}

void hisi_tcpc_irq_gpio_register(int irq_gpio)
{
	hi6526_irq_gpio = irq_gpio;
}

void hisi_tcpc_set_vusb_uv_det_sts(bool en)
{
	hi6526_vusb_uv_det_en_status = en;
}

bool hisi_tcpc_get_vusb_uv_det_sts(void)
{
	return hi6526_vusb_uv_det_en_status;
}

void hisi_tcpc_notify_chip_version(unsigned int version)
{
	g_hi6526_version = version;
}

unsigned int hisi_tcpc_get_chip_version(void)
{
	return g_hi6526_version;
}

int hisi_tcpc_get_reg_info(struct device_node *np, struct hisi_tcpc_reg *reg_info)
{
	struct device_node *regs_np = of_get_child_by_name(np, "hisi_tcpc_regs");
	int ret;

	if (regs_np == NULL)
		return -1;

	if (of_property_read_u32(regs_np, "cdr_cfg_0", &reg_info->cdr_cfg_0))
		return -1;
	if (of_property_read_u32(regs_np, "dbg_cfg_1", &reg_info->dbg_cfg_1))
		return -1;
	if (of_property_read_u32(regs_np, "sc_buck_en", &reg_info->sc_buck_en))
		return -1;
	if (of_property_read_u32(regs_np, "irq_gbl_msk", &reg_info->irq_gbl_msk))
		return -1;
	if (of_property_read_u32(regs_np, "irq_msk_pd_bit", &reg_info->irq_msk_pd_bit))
		return -1;
	if (of_property_read_u32(regs_np, "abnormal_irq_sts", &reg_info->abnormal_irq_sts))
		return -1;
	if (of_property_read_u32(regs_np, "abnormal_irq_msk", &reg_info->abnormal_irq_msk))
		return -1;
	if (of_property_read_u32(regs_np, "cc_ovp_irq", &reg_info->cc_ovp_irq))
		return -1;
	if (of_property_read_u32(regs_np, "cfg_reg_1", &reg_info->cfg_reg_1))
		return -1;
	if (of_property_read_u32(regs_np, "aux_sbu_en", &reg_info->aux_sbu_en))
		return -1;
	if (of_property_read_u32(regs_np, "jtag_sbu_en", &reg_info->jtag_sbu_en))
		return -1;

	ret = of_property_read_u32(regs_np, "cc_ocp_irq", &reg_info->cc_ocp_irq);
 
	ret = of_property_read_u32(regs_np, "cc_uvp_irq", &reg_info->cc_uvp_irq);

	ret = of_property_read_u32(regs_np, "sbu1_ovp_irq", &reg_info->sbu1_ovp_irq);

	ret = of_property_read_u32(regs_np, "sbu2_ovp_irq", &reg_info->sbu2_ovp_irq);

	return 0;
}

