/*
 * vcodec_vdec_refulator.c
 *
 * This is for vdec regulator
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#include "vcodec_vdec_regulator.h"
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec.h"
#include <linux/device.h>
#include "securec.h"
#include "doze.h"
#include "vdec_tzsp_client.h"

vdec_plat g_vdec_plat_entry;

#define VDEC_CLOCK_NAME "clk_vdec"
#define VDEC_PXPC_CLOCK_NAME "clk_vdec_pxpc"
#define VDEC_CLK_RATE "clk_rate"
#define PXPC_CLK_RATE "pxpc_clk_rate"
#define CLK_LOAD_LIMIT "clk_load_limit"
#define MEDIA_DEFAULT_CLK_NAME "default_clk_rate"

const char *g_regulator_name[MAX_REGULATOR] = { "ldo_media", "ldo_vdec", "ldo_smmu_tcu" };

vdec_plat *vdec_plat_get_entry(void)
{
	return &g_vdec_plat_entry;
}

static int32_t vdec_plat_init_smmu(struct device *dev)
{
	int32_t ret;
	vdec_plat *plt = vdec_plat_get_entry();
	(void)memset_s(&plt->smmu_info, sizeof(struct smmu_tbu_info), 0, sizeof(struct smmu_tbu_info));

	ret = of_property_read_u32(dev->of_node, "mmu_tbu_num", &plt->smmu_info.mmu_tbu_num);
	if (ret) {
		dprint(PRN_FATAL, "read mmu_tbu_num failed set to default\n");
		return ret;
	}

	ret = of_property_read_u32(dev->of_node, "mmu_tbu_offset", &plt->smmu_info.mmu_tbu_offset);
	if (ret) {
		dprint(PRN_FATAL, "read mmu_tbu_offset failed set to default\n");
		return ret;
	}

	ret = of_property_read_u32(dev->of_node, "mmu_sid_offset", &plt->smmu_info.mmu_sid_offset);
	if (ret) {
		dprint(PRN_FATAL, "read mmu_sid_offset failed set to default\n");
		return ret;
	}

	return 0;
}

static int32_t vdec_plat_init_regulator(struct device *dev)
{
	int32_t i;
	int32_t ret;
	vdec_plat *plt = vdec_plat_get_entry();
	uint32_t value = 0;
	(void)memset_s(&plt->regulator_info, sizeof(vdec_regulator), 0, sizeof(vdec_regulator));

	for (i = 0; i < MAX_REGULATOR; i++) {
		plt->regulator_info.regulators[i] = devm_regulator_get(dev, g_regulator_name[i]);
		if (IS_ERR_OR_NULL(plt->regulator_info.regulators[i])) {
			dprint(PRN_FATAL, "get regulator: %s failed\n", g_regulator_name[i]);
			goto regulators_err;
		}
	}

	plt->regulator_info.clk_vdec = devm_clk_get(dev, VDEC_CLOCK_NAME);
	if (IS_ERR_OR_NULL(plt->regulator_info.clk_vdec)) {
		dprint(PRN_FATAL, "get clk failed\n");
		goto clk_vdec_err;
	}

	plt->regulator_info.clk_vdec_pxpc = devm_clk_get(dev, VDEC_PXPC_CLOCK_NAME);
	if (IS_ERR_OR_NULL(plt->regulator_info.clk_vdec_pxpc)) {
		dprint(PRN_FATAL, "get clk failed\n");
		goto clk_pxpc_err;
	}

	for (i = 0; i < VDEC_CLK_RATE_LEVEL_MAX; i++) {
		ret = of_property_read_u32_index(dev->of_node, VDEC_CLK_RATE, (uint32_t)i,
			&plt->regulator_info.clk_values[i]);
		ret += of_property_read_u32_index(dev->of_node, PXPC_CLK_RATE, (uint32_t)i,
			&plt->regulator_info.pxpc_clk_values[i]);
		if (ret) {
			dprint(PRN_FATAL, "read %d clk failed\n", i);
			goto property_err;
		}
	}

	for (i = 0; i < VDEC_CLK_LOAD_LIMIT_MAX; i++) {
		ret = of_property_read_u32_index(dev->of_node, CLK_LOAD_LIMIT, (uint32_t)i, &value);
		if (ret) {
			dprint(PRN_FATAL, "read %d clk load limit failed\n", i);
			goto property_err;
		}
		plt->regulator_info.clk_load_limit_values[i] = (uint64_t)value;
	}

	/* default_clk_rate are used for clk config constraint */
	ret = of_property_read_u32(dev->of_node, MEDIA_DEFAULT_CLK_NAME,
		&plt->regulator_info.default_clk_rate);
	if (ret) {
		dprint(PRN_FATAL, "failed to read default_clk_rate\n");
		goto property_err;
	}

	return 0;

property_err:
	devm_clk_put(dev, plt->regulator_info.clk_vdec_pxpc);
clk_pxpc_err:
	devm_clk_put(dev, plt->regulator_info.clk_vdec);
clk_vdec_err:
regulators_err:
	for (i = 0; i < MAX_REGULATOR; i++) {
		if (!IS_ERR_OR_NULL(plt->regulator_info.regulators[i]))
			devm_regulator_put(plt->regulator_info.regulators[i]);
	}

	(void)memset_s(&plt->regulator_info, sizeof(vdec_regulator), 0, sizeof(vdec_regulator));
	return -EFAULT;
}


static int32_t vdec_plat_init_dts(struct device *dev)
{
	int32_t ret;
	int32_t i;
	struct resource res = {0};
	vdec_plat *plt_info = vdec_plat_get_entry();
	(void)memset_s(&plt_info->dts_info, sizeof(vdec_dts), 0, sizeof(vdec_dts));

	for (i = 0; i < MAX_VDEC_NORM_IRQ; i++) {
		plt_info->dts_info.irq_norm[i] = irq_of_parse_and_map(dev->of_node, i);
		if (plt_info->dts_info.irq_norm[i] == 0) {
			dprint(PRN_ERROR, "irq_of_parse_and_map failed\n");
			return -EFAULT;
		}
	}

	for (i = BSP_SAFE_IRQ; i < MAX_VDEC_SAFE_IRQ; i++) {
		plt_info->dts_info.irq_safe[i] = irq_of_parse_and_map(dev->of_node, i);
		if (plt_info->dts_info.irq_safe[i] == 0) {
			dprint(PRN_ERROR, "irq_of_parse_and_map failed\n");
			return -EFAULT;
		}
	}

	ret = of_address_to_resource(dev->of_node, 0, &res);
	if (ret) {
		dprint(PRN_FATAL, "read dec inner module reg info failed\n");
		return ret;
	}

	ret = of_property_read_u32(dev->of_node, "vdec_reserved_iova_start", &plt_info->dts_info.vdec_reserved_iova_start);
	if (ret) {
		dprint(PRN_FATAL, "get vdec_reserved_iova_start failed");
		return ret;
	}

	ret = of_property_read_u32(dev->of_node, "vdec_reserved_iova_size", &plt_info->dts_info.vdec_reserved_iova_size);
	if (ret) {
		dprint(PRN_FATAL, "get vdec_reserved_iova_size failed");
		return ret;
	}

	plt_info->dts_info.reg_base_info.reg_phy_addr = (UADDR)res.start;
	plt_info->dts_info.reg_base_info.reg_range = (uint32_t)resource_size(&res);

	ret = of_property_read_u32(dev->of_node, "vdec_fpga", &plt_info->dts_info.is_fpga);
	if (ret) {
		dprint(PRN_WARN, "failed to get plat vdec_fpga flag\n");
		plt_info->dts_info.is_fpga = 0;
	}

	ret = of_property_read_u32(dev->of_node, "vdec_es", &plt_info->dts_info.is_es_plat);
	if (ret) {
		dprint(PRN_WARN, "failed to get plat vdec_es flag\n");
		plt_info->dts_info.is_es_plat = 0;
	}

	ret = of_property_read_u32(dev->of_node, "pcie_mmap_host_start_reg", &plt_info->dts_info.pcie_mmap_host_start_reg);
	if (ret) {
		dprint(PRN_WARN, "failed to get plat pcie_mmap_host_start_reg flag\n");
		plt_info->dts_info.pcie_mmap_host_start_reg = 0xE9C00000;
	}

	return 0;
}

static void vdec_plat_deinit_regulator(vdec_regulator *regulator_info)
{
	int32_t i;
	vdec_plat *plt = vdec_plat_get_entry();

	devm_clk_put(plt->dev, plt->regulator_info.clk_vdec_pxpc);
	devm_clk_put(plt->dev, plt->regulator_info.clk_vdec);
	for (i = 0; i < MAX_REGULATOR; i++) {
		if (!IS_ERR_OR_NULL(plt->regulator_info.regulators[i]))
			devm_regulator_put(plt->regulator_info.regulators[i]);
	}

	(void)memset_s(regulator_info, sizeof(vdec_regulator), 0, sizeof(vdec_regulator));
}

static void vdec_plat_deinit_dts(vdec_dts *dts_info)
{
	(void)memset_s(dts_info, sizeof(vdec_dts), 0, sizeof(vdec_dts));
}

static void vdec_plat_deinit_smmu(struct smmu_tbu_info *smm_info)
{
	(void)memset_s(smm_info, sizeof(struct smmu_tbu_info), 0, sizeof(struct smmu_tbu_info));
}

int32_t vdec_plat_init(struct platform_device *plt_dev)
{
	int32_t ret;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_init_mutex(&plt->vdec_plat_mutex);

	ret = vdec_plat_init_regulator(&plt_dev->dev);
	if (ret) {
		dprint(PRN_FATAL, "regulator init failed\n");
		return ret;
	}

	ret = vdec_plat_init_dts(&plt_dev->dev);
	if (ret) {
		dprint(PRN_FATAL, "dts init failed\n");
		return ret;
	}

	ret = vdec_plat_init_smmu(&plt_dev->dev);
	if (ret) {
		dprint(PRN_FATAL, "smmu info init failed\n");
		return ret;
	}

	plt->dev = &plt_dev->dev;
	plt->dts_info.dev = plt->dev;

	plt->plt_init = 1;
	plt->power_flag = 0;
	plt->clk_ctrl.dynamic_clk_level = 0;


	return 0;
}

int32_t vdec_plat_deinit(void)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_plat_deinit_regulator(&plt->regulator_info);

	vdec_plat_deinit_dts(&plt->dts_info);

	vdec_plat_deinit_smmu(&plt->smmu_info);

	plt->power_flag = 0;
	plt->plt_init = 0;

	return 0;
}

static int32_t vdec_plat_switch_clk(uint32_t dst_clk_level, uint32_t dst_clk_rate, uint32_t dst_pxpc_clk_rate)
{
	/* Low Temperature Scenario fail need todo deal with */
	int32_t ret = 0;
	uint32_t current_clk_rate, current_pxpc_clk_rate;
	vdec_plat *plt = vdec_plat_get_entry();
	uint32_t actual_clk_level;
	uint32_t secondary_high_clk_level;
	uint32_t clk_rate_value;

	current_clk_rate = (uint32_t)clk_get_rate(plt->regulator_info.clk_vdec);
	current_pxpc_clk_rate = (uint32_t)clk_get_rate(plt->regulator_info.clk_vdec_pxpc);
	if (dst_clk_rate == current_clk_rate)
		return 0;

	actual_clk_level = dst_clk_level;
	if (dst_pxpc_clk_rate > current_pxpc_clk_rate) { // increase freqency
		ret = clk_set_rate(plt->regulator_info.clk_vdec_pxpc, dst_pxpc_clk_rate);
		if (!ret)
			(void)clk_set_rate(plt->regulator_info.clk_vdec, dst_clk_rate);
	} else { // decrease freqency
		(void)clk_set_rate(plt->regulator_info.clk_vdec, dst_clk_rate);
		(void)clk_set_rate(plt->regulator_info.clk_vdec_pxpc, dst_pxpc_clk_rate);
	}

	if (ret && (dst_clk_level == VDEC_CLK_RATE_LEVEL_MAX - 1)) { // low temperature
		// after the high frequency fails to be set, set the secondary high frequency
		dprint(PRN_ALWS, "low temperature");
		secondary_high_clk_level = VDEC_CLK_RATE_LEVEL_MAX - 2;
		actual_clk_level = secondary_high_clk_level;
		clk_rate_value = plt->regulator_info.pxpc_clk_values[secondary_high_clk_level];

		ret = clk_set_rate(plt->regulator_info.clk_vdec_pxpc, clk_rate_value);
		if (ret) {
			dprint(PRN_ERROR, "failed set clk to level %u", actual_clk_level);
			return ret;
		} else {
			clk_rate_value = plt->regulator_info.clk_values[secondary_high_clk_level];
			(void)clk_set_rate(plt->regulator_info.clk_vdec, clk_rate_value);
		}
	}

	plt->clk_ctrl.current_clk_level = actual_clk_level;
	return 0;
}

static int32_t vdec_plat_set_clk(uint32_t clk_level)
{
	int32_t ret = 0;
	vdec_plat *plt = vdec_plat_get_entry();

	ret = clk_set_rate(plt->regulator_info.clk_vdec_pxpc, plt->regulator_info.pxpc_clk_values[clk_level]);
	if (ret) {
		dprint(PRN_FATAL, "set pxpc clk failed");
		return ret;
	}

	ret = clk_set_rate(plt->regulator_info.clk_vdec, plt->regulator_info.clk_values[clk_level]);
	if (ret) {
		dprint(PRN_FATAL, "set vdec clk failed");
		(void)clk_set_rate(plt->regulator_info.clk_vdec_pxpc, plt->regulator_info.default_clk_rate);
		return ret;
	}
	return 0;
}

static int32_t power_up_set_clk(vdec_plat *plt)
{
	int32_t ret;
	uint32_t clk_level = VDEC_CLK_RATE_LEVEL_MAX - 1;
	uint32_t clk_rate, pxpc_clk_rate;

	if (!plt->clk_ctrl.open_in_high_clk) {
		clk_level = 0;
		if (vdec_plat_set_clk(clk_level) != 0)
			return -EIO;
	} else {
		plt->clk_ctrl.open_in_high_clk = 0;
		clk_rate = plt->regulator_info.clk_values[clk_level];
		pxpc_clk_rate = plt->regulator_info.pxpc_clk_values[clk_level];
		if (vdec_plat_switch_clk(clk_level, clk_rate, pxpc_clk_rate) != 0)
			return -EIO;
	}

	ret = clk_prepare_enable(plt->regulator_info.clk_vdec_pxpc); // clk_vdec_pxpc
	if (ret) {
		dprint(PRN_FATAL, "clk pxpc enable failed");
		goto clk_prepare_enable_pxpc_fail;
	}

	ret = clk_prepare_enable(plt->regulator_info.clk_vdec); // clk_vdec
	if (ret) {
		dprint(PRN_FATAL, "clk enable failed");
		goto clk_prepare_enable_vdec_fail;
	}

	return 0;
clk_prepare_enable_vdec_fail:
	clk_disable_unprepare(plt->regulator_info.clk_vdec_pxpc);
clk_prepare_enable_pxpc_fail:
	(void)clk_set_rate(plt->regulator_info.clk_vdec, plt->regulator_info.default_clk_rate);
	(void)clk_set_rate(plt->regulator_info.clk_vdec_pxpc, plt->regulator_info.default_clk_rate);
	return -EIO;
}

static void power_down_set_clk(vdec_plat *plt)
{
	(void)clk_set_rate(plt->regulator_info.clk_vdec, plt->regulator_info.default_clk_rate);
	(void)clk_set_rate(plt->regulator_info.clk_vdec_pxpc, plt->regulator_info.default_clk_rate);

	clk_disable_unprepare(plt->regulator_info.clk_vdec);
	clk_disable_unprepare(plt->regulator_info.clk_vdec_pxpc);
}

int32_t VCODEC_ATTR_WEEK power_up_vdec_top(void)
{
	int32_t ret;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_check_ret(plt->plt_init, -EINVAL);

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	if (plt->power_flag) {
		dprint(PRN_ALWS, "regulator already enable\n");
		vdec_mutex_unlock(&plt->vdec_plat_mutex);
		return 0;
	}
    vote_disable_doze(DOZE_CLIENT_VDEC);

	ret = regulator_enable(plt->regulator_info.regulators[MEDIA_REGULATOR]);
	if (ret) {
		dprint(PRN_FATAL, "enable media regulator failed\n");
		goto regulator_enable_media1_fail;
	}

	if (power_up_set_clk(plt)) {
		dprint(PRN_FATAL, " set clk rate and enable clk fail\n");
		goto set_clk_fail;
	}
	ret = regulator_enable(plt->regulator_info.regulators[VDEC_REGULATOR]); // vdec
	if (ret) {
		dprint(PRN_FATAL, "enable vdec regulator failed\n");
		goto regulator_enable_vdec_fail;
	}

	plt->power_flag = 1;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	dprint(PRN_ALWS, "success\n");
	return 0;

regulator_enable_vdec_fail:
	power_down_set_clk(plt);
set_clk_fail:
	regulator_disable(plt->regulator_info.regulators[MEDIA_REGULATOR]);
regulator_enable_media1_fail:
    vote_enable_doze(DOZE_CLIENT_VDEC);
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	return -EIO;
}

void VCODEC_ATTR_WEEK power_down_vdec_top_mcu(void)
{
	vdec_plat *plt = vdec_plat_get_entry();

	if (!(plt->plt_init)) {
		dprint(PRN_ERROR, "assert condition not match\n");
		return;
	}
	vdec_mutex_lock(&plt->vdec_plat_mutex);

	if (!plt->power_flag) {
		dprint(PRN_ALWS, "regulator already disable\n");
		vdec_mutex_unlock(&plt->vdec_plat_mutex);
		return;
	}

	(void)regulator_disable(plt->regulator_info.regulators[VDEC_REGULATOR]); // vdec

	power_down_set_clk(plt);
	(void)regulator_disable(plt->regulator_info.regulators[MEDIA_REGULATOR]);

    vote_enable_doze(DOZE_CLIENT_VDEC);

	plt->power_flag = 0;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	dprint(PRN_ALWS, "success\n");
}

int32_t power_up_vdec_cal(void)
{
	vdec_entry *entry = vdec_get_entry();
	vdec_plat *plt = vdec_plat_get_entry();
	vdec_plat_ops *ops = get_vdec_plat_ops();
	bus_idle_cfg bus_idle = {0};
	uint32_t reg_data;

	vdec_check_ret(plt->plt_init, -EINVAL);
	vdec_mutex_lock(&plt->vdec_plat_mutex);

	// step 1 vdec cal module mtcmos on
	writel(0x80008, (entry->media1_ctrl_vir_addr + 0x404));
	udelay(100); // wait 100us

	 // step 2, SUB_CTRL VDEC_ROOT_CRG vdec cal module clk on
	writel(0xFFFFFFFF, (entry->reg_base_vir_addr + 0x40801c));
	udelay(1); // wait 1us

	// step 3 vdec cal module clk off
	writel(0x0, (entry->reg_base_vir_addr + 0x40801c));
	udelay(1); // wait 1us

	// step 4 module isolation disable
	writel(0x40000, (entry->media1_ctrl_vir_addr +  0x400));

	// step 5 memory repair
	writel(0x100000, (entry->bisr_ctrl_vir_addr + 0x30));
	udelay(400); // wait 400us

	// step 5 hardware memory automatic repair finish ack
	reg_data = readl(entry->bisr_ctrl_vir_addr + 0x5c);
	if(!(reg_data & (1 << 14))) {
		dprint(PRN_ERROR, "memory repair failed, reg_data is %lu\n", reg_data);
		goto memory_repair_fail;
	}

	// step 6 disable shut down control signal, the memory exits shutdown
	writel(0x1200000, (entry->media1_ctrl_vir_addr + 0x428));
	// using ffa communicate with tzsp to write 0x10000 to actrl + 0x9a8
	if (vdec_tzsp_client_send_msg(0)) {
		dprint(PRN_ERROR, "step 6 disable shut down control signal failed");
		// reverse the first procedure of step 6
		writel(0x1200020, (entry->media1_ctrl_vir_addr + 0x428));
		goto memory_repair_fail;
	}

	// step 7 vdec module unrst
	writel(0x0, (entry->reg_base_vir_addr + 0x40802c));

	// step 8 vdec module clk enable
	writel(0xFFFFFFFF, (entry->reg_base_vir_addr + 0x40801c));

	// step 9 vdec bus idle clear
	ops->config_bus_idle(&bus_idle, false);
	writel(bus_idle.target_value, (entry->reg_base_vir_addr + bus_idle.offset));
	udelay(100);
	reg_data = readl(entry->reg_base_vir_addr + bus_idle.offset);
	if (reg_data != bus_idle.expect_value) {
		dprint(PRN_ERROR, "step9 vdec bus idle clear failed\n");
		goto bus_idle_clear_fail;
	}
	// step 10 bsp/pxpc auto dfs, sample back not enable
	writel(0x3FBF3FBF, (entry->reg_base_vir_addr + 0x408500));

	plt->power_vdec_cal_flag = 1;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);

	return 0;
bus_idle_clear_fail:
	// step 9 set vdec bus idle
	writel(0x3, (entry->reg_base_vir_addr + bus_idle.offset));
	udelay(100);
	// step 8 vdec cal module clk off
	writel(0x0, (entry->reg_base_vir_addr + 0x40801c));
	// step 7 vdec module reset
	writel(0xFFFFFFFF, (entry->reg_base_vir_addr + 0x40802c));
	udelay(1); // wait 1us
	// step 6
	writel(0x1200020, (entry->media1_ctrl_vir_addr + 0x428));
	// using ffa communicate with tzsp to writel 0x10001 to actrl + 0x9a8
	(void)vdec_tzsp_client_send_msg(1);

memory_repair_fail:
	// step 4 module isolation enable
	writel(0x40004, (entry->media1_ctrl_vir_addr +  0x400));
	// step 1 module mtcmos off
	writel(0x80000, (entry->media1_ctrl_vir_addr + 0x404));
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
	return -EFAULT;
}

void VCODEC_ATTR_WEEK power_down_vdec_cal(bool retention_img)
{
	vdec_entry *entry = vdec_get_entry();
	vdec_plat *plt = vdec_plat_get_entry();
	vdec_plat_ops *ops = get_vdec_plat_ops();
	bus_idle_cfg bus_idle = {0};
	uint32_t reg_data;
	if (!(plt->plt_init)) {
		dprint(PRN_ERROR, "assert condition not match\n");
		return;
	}
	vdec_mutex_lock(&plt->vdec_plat_mutex);

	if (!plt->power_vdec_cal_flag) {
		dprint(PRN_ALWS, "regulator already disable\n");
		vdec_mutex_unlock(&plt->vdec_plat_mutex);
		return;
	}

	// step 1 set vdec bus idle
	ops->config_bus_idle(&bus_idle, true);
	writel(bus_idle.target_value, (entry->reg_base_vir_addr + bus_idle.offset));
	udelay(100);
	reg_data = readl(entry->reg_base_vir_addr + bus_idle.offset);
	if (reg_data != bus_idle.expect_value)
		dprint(PRN_ERROR, "step 1 set vdec bus idle failed\n");

	// step 2 vdec module clk disable
	writel(0x0, (entry->reg_base_vir_addr + 0x40801c));

	// step 3 enable shutdown singnal，memory shutdown
	if (retention_img) {
		writel(0x1200120, (entry->media1_ctrl_vir_addr + 0x428));
		// using ffa communicate with tzsp to write 0x10000 to actrl + 0x9a8
		(void)(vdec_tzsp_client_send_msg(0));
	} else {
		writel(0x1200020, (entry->media1_ctrl_vir_addr + 0x428));
		// using ffa communicate with tzsp to write 0x10001 to actrl + 0x9a8
		(void)(vdec_tzsp_client_send_msg(1));
	}
	udelay(1);

	// step 4 module iso enable
	writel(0x40004, (entry->media1_ctrl_vir_addr +  0x400));

	// step 5
	writel(0x100010, (entry->bisr_ctrl_vir_addr + 0x30));

	// step 6 vdec module rst
	writel(0xFFFFFFFF, (entry->reg_base_vir_addr + 0x40802c));

	// step 7
	writel(0x80000, (entry->media1_ctrl_vir_addr + 0x404));

	plt->power_vdec_cal_flag = 0;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

void vdec_plat_set_dynamic_clk_rate(uint32_t clk_level)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	plt->clk_ctrl.dynamic_clk_level = clk_level;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

void vdec_plat_get_dynamic_clk_rate(uint32_t *clk_level)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	*clk_level = plt->clk_ctrl.dynamic_clk_level;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

void vdec_plat_set_static_clk_rate(uint32_t clk_level)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	plt->clk_ctrl.static_clk_level = clk_level;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

void vdec_plat_get_static_clk_rate(uint32_t *clk_level)
{
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	*clk_level = plt->clk_ctrl.static_clk_level;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

void vdec_plat_get_target_clk_rate(uint32_t *clk_level)
{
	vdec_plat *plt = vdec_plat_get_entry();
	uint32_t target_clk_level;

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	if (*clk_level >= VDEC_CLK_RATE_LEVEL_MAX)
		target_clk_level = (plt->clk_ctrl.static_clk_level >= plt->clk_ctrl.dynamic_clk_level) ?
			plt->clk_ctrl.static_clk_level : plt->clk_ctrl.dynamic_clk_level;
	else
		target_clk_level = (*clk_level >= plt->clk_ctrl.dynamic_clk_level) ? *clk_level : plt->clk_ctrl.dynamic_clk_level;
	*clk_level = target_clk_level;
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
}

int32_t vdec_plat_regulator_set_clk_rate(uint32_t dst_clk_level)
{
	uint32_t dst_clk_rate, dst_pxpc_clk_rate;
	int32_t ret = 0;
	vdec_plat *plt = vdec_plat_get_entry();

	vdec_check_ret(plt->plt_init, -EINVAL);
	vdec_check_ret((dst_clk_level < VDEC_CLK_RATE_LEVEL_MAX), -EINVAL);

	vdec_mutex_lock(&plt->vdec_plat_mutex);
	if (!plt->power_flag) {
		dprint(PRN_WARN, "power down do not set clk rate");
		goto exit;
	}

	if (plt->clk_ctrl.clk_flag && plt->clk_ctrl.current_clk_level == dst_clk_level)
		goto exit;

	dst_clk_rate = plt->regulator_info.clk_values[dst_clk_level];
	dst_pxpc_clk_rate = plt->regulator_info.pxpc_clk_values[dst_clk_level];

	if (vdec_plat_switch_clk(dst_clk_level, dst_clk_rate, dst_pxpc_clk_rate) != 0) {
		ret = -EFAULT;
		goto exit;
	}

	dprint(PRN_ALWS, "set dst_clk_rate value: %u", dst_clk_rate);

exit:
	vdec_mutex_unlock(&plt->vdec_plat_mutex);
	return ret;
}

uint32_t vdec_plat_is_es(void)
{
	return vdec_plat_get_entry()->dts_info.is_es_plat;
}

uint32_t vdec_plat_reserved_ion_base(void)
{
    vdec_plat *plt_info = vdec_plat_get_entry();
    return plt_info->dts_info.vdec_reserved_iova_start;
}

uint32_t vdec_plat_reg_base_addr(void)
{
	vdec_plat *plt_info = vdec_plat_get_entry();
	return plt_info->dts_info.reg_base_info.reg_phy_addr;
}