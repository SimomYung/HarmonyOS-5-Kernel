/*
 * vcodec_vdec_plat_v800.c
 *
 * This is for vdec platform
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/of.h>
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec_regulator.h"

const char *pg_chip_bypass_vdec[] = {
	"cpu_normal",
	"hsdt_cdc",
	"unknown",
};

static bool bypass_vdec_by_soc_spec(void)
{
	int32_t ret;
	const char *soc_spec = NULL;
	struct device_node *np = of_find_compatible_node(NULL, NULL, "hisilicon, soc_spec");
	size_t i;
	if (!np) {
		dprint(PRN_ERROR, "of_find_compatible_node fail or normal type chip\n");
		return false;
	}

	ret = of_property_read_string(np, "soc_spec_set", &soc_spec);
	if (ret) {
		dprint(PRN_ERROR, "of_property_read_string fail\n");
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(pg_chip_bypass_vdec); i++) {
		ret = strcmp(soc_spec, pg_chip_bypass_vdec[i]);
		if (!ret) {
			dprint(PRN_ALWS, "is pg chip : %s, need bypass vdec\n", pg_chip_bypass_vdec[i]);
			return true;
		}
	}

	dprint(PRN_ALWS, " no need to bypss vdec\n");
	return false;
}

static bool need_cfg_tbu_max_tok_trans(void)
{
	return false;
}

// 下电时set bus idle, set为true, 上电的时候clear bus idle, set为false;
// target_value为写入值, expect_value为读预期值, 该寄存器写低位读高位, 高位为只读属性;
static void config_bus_idle(bus_idle_cfg* bus_idle, bool set)
{
	bus_idle->offset = 0x406040;
	bus_idle->expect_value = set ? 0x303 : 0x0;
	bus_idle->target_value = set ? 0x3 : 0x0;
}
 
static void pxp_config_bus_idle(bus_idle_cfg* bus_idle, bool set)
{
	bus_idle->offset = 0x406040;
	bus_idle->expect_value = set ? 0x303 : 0x0;
	bus_idle->target_value = set ? 0x3 : 0x0;
}

static void pxp_reset_config_subctrl_rst(U_VDEC_RST* reg_rst, bool enable)
{
	reg_rst->bits.core_qic_clk_pxphw_soft_rst = enable ? 1: 0;
}
 
static void pxp_reset_config_subctrl_crg(U_VDEC_CRG* reg_crg, bool enable)
{
	reg_crg->bits.core_qic_clk_pxphw_en = enable ? 1: 0;
}

static void pxp_powerup_softrst_bit(U_VDEC_RST* reg_rst) {
	reg_rst->bits.pxpc_soft_rst = 0;
}

static vdec_plat_ops g_vdec_plat_v800_ops = {
	.vcodec_vdec_device_bypass = bypass_vdec_by_soc_spec,
	.smmu_need_cfg_max_tok_trans = need_cfg_tbu_max_tok_trans,
	.config_bus_idle = config_bus_idle,
	.pxp_config_bus_idle = pxp_config_bus_idle,
	.pxp_reset_config_subctrl_rst = pxp_reset_config_subctrl_rst,
	.pxp_reset_config_subctrl_crg = pxp_reset_config_subctrl_crg,
	.pxp_powerup_softrst_bit = pxp_powerup_softrst_bit,
};

vdec_plat_ops *get_vdec_plat_ops(void)
{
	return &g_vdec_plat_v800_ops;
}
