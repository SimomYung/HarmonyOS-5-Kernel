/*
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "clk-smc.h"
#include "clk-vote-xpu.h"

#define ELOW_TEMPERATURE	0xE558

static struct vote_xpu_driver_cfg g_driver_cfg;

#ifdef CONFIG_CLK_DEBUG
int clk_vote_xpu_dump(struct clk *clk)
{
	struct clk_hw *hw;
	struct hi3xxx_vote_xpu_clk *pclk;
	int ret = 0;

	if (!clk) {
		pr_err("[%s] clk is NULL point!\n", __func__);
		return -EINVAL;
	}

	hw = __clk_get_hw(clk);
	pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);

	/* SMC into ATF to open the gate */
	ret = atfd_service_clk_smc(CLK_VOTE_XPU, pclk->xpu_clk_id, VOTE_XPU_CLK_DUMP, 0);
	if (ret) {
		pr_err("[%s] dump clk [%s] fail, ret = %d\n",
			__func__, clk_hw_get_name(hw), ret);
		return ret;
	}

	return 0;
}

int clk_vote_xpu_dump_all(void)
{
	int ret = 0;

	/* SMC into ATF to open the gate */
	ret = atfd_service_clk_smc(CLK_VOTE_XPU, 0, VOTE_XPU_CLK_ALL_DUMP, 0);
	if (ret) {
		pr_err("[%s] dump clk fail, ret = %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
#endif

static int low_temperature_config_init(struct device_node *np)
{
	struct device_node *pmctrl_np = NULL;
	uint32_t register_info[LOW_TEMPERATURE_CFG_NUM];
	int ret;

	pmctrl_np = of_find_compatible_node(NULL, NULL, "hisilicon,pmctrl");
	if (IS_ERR_OR_NULL(pmctrl_np)) {
		pr_err("[%s] fail to find pmctrl node\n", __func__);
		return -ENODEV;
	}

	g_driver_cfg.pmc_base_addr = of_iomap(pmctrl_np, 0);
	if (IS_ERR_OR_NULL(g_driver_cfg.pmc_base_addr)) {
		pr_err("[%s] pmc_addr iomap fail\n", __func__);
		return -EINVAL;
	}

	/* parse low temperature ctrl_reg */
	ret = of_property_read_u32_array(np, "low-temperature-attr",
		&register_info[0], LOW_TEMPERATURE_CFG_NUM);
	if (ret) {
		pr_err("[%s] parse low temperature property fail\n", __func__);
		return ret;
	}

	g_driver_cfg.temperature_flag_addr = register_info[0];
	g_driver_cfg.temperature_flag_mask = register_info[1];
	g_driver_cfg.normal_temperature_val = register_info[2];

	return 0;
}

int clk_vote_xpu_driver_init(struct device_node *np)
{
	return low_temperature_config_init(np);
}

static bool is_low_temperature(struct hi3xxx_vote_xpu_clk *pclk)
{
	unsigned int offset = g_driver_cfg.temperature_flag_addr;
	unsigned int mask = g_driver_cfg.temperature_flag_mask;
	unsigned int normal_value = g_driver_cfg.normal_temperature_val;
	unsigned int value;

	if (!(pclk->low_temperature_property))
		return false;

	/* low temperature return true, normal return false */
	value = readl(g_driver_cfg.pmc_base_addr + offset);
	if ((value & mask) != normal_value)
		return true;
	return false;
}

static unsigned long vote_xpu_clk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct hi3xxx_vote_xpu_clk *pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);
	u32 cur_freq = pclk->cur_freq;
	return (unsigned long)cur_freq;
}

static int vote_xpu_clk_set_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long parent_rate)
{
	struct hi3xxx_vote_xpu_clk *pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);
	unsigned int freq_req, freq_final, err_code;
	unsigned int value, freq_result, intr_state;
	unsigned long rate_max;
	int timeout = 0;

	(void)parent_rate;

	if (is_low_temperature(pclk)) {
		if (rate > (pclk->low_temperature_freq * FREQ_CONVERSION)) {
			pr_err("[%s] clk name =%s, cur_freq-%lu > low_temperature_freq-%lu!\n",
				__func__, clk_hw_get_name(hw), rate, pclk->low_temperature_freq);
			return -ELOW_TEMPERATURE;
		}
	}

	rate_max = MHZ_TO_HZ(pclk->vote_reg.freq_mask >> pclk->vote_reg.freq_shift);
	if (rate > rate_max) {
		pr_err("[%s] clk name =%s, rate-%lu > max-%lu!\n",
			__func__, clk_hw_get_name(hw), rate, rate_max);
		return -1;
	}

	/* vote xpu to set rate */
	freq_req = HZ_TO_MHZ(rate);
	value = (freq_req << pclk->vote_reg.freq_shift) & pclk->vote_reg.freq_mask;
	value |= BIT(pclk->vote_reg.mask_bit);
	writel(value, pclk->base_addr + pclk->vote_reg.addr);

	/* waiting for the result of set rate */
	while (timeout < XPU_VOTE_TIMEOUT) {
		freq_result = readl(pclk->base_addr + pclk->result_freq_reg);
		value = readl(pclk->base_addr + pclk->stat_reg.addr);
		intr_state = (readl(pclk->base_addr + pclk->intr_reg.addr) & BIT(pclk->intr_reg.stat_bit));
		freq_final = (value & pclk->stat_reg.freq_mask) >> pclk->stat_reg.freq_shift;
		err_code = (value & pclk->stat_reg.err_code_mask) >> pclk->stat_reg.err_code_shift;
		if ((!intr_state) && (!err_code) && (freq_final >= freq_req) && (freq_result >= freq_req))
			break;

		udelay(1);
		timeout++;
	}

	if (timeout >= XPU_VOTE_TIMEOUT) {
		pr_err("[%s] clk name =%s, set rate-%lu fail ret-%u, cur_rate %lu %lu\n",
			__func__, clk_hw_get_name(hw), rate, err_code,
			MHZ_TO_HZ(freq_final), MHZ_TO_HZ(freq_result));
		return -1;
	}

	pclk->cur_freq = (u32)rate;
	return 0;
}

static int vote_xpu_clk_determine_rate(struct clk_hw *hw, struct clk_rate_request *req)
{
	struct hi3xxx_vote_xpu_clk *pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);

	if (is_low_temperature(pclk)) {
		if (req->rate > (pclk->low_temperature_freq * FREQ_CONVERSION)) {
			pr_err("[%s] clk name =%s, cur_freq-%lu > low_temperature_freq-%lu!\n",
				__func__, clk_hw_get_name(hw), req->rate, pclk->low_temperature_freq);
			return -ELOW_TEMPERATURE;
		}
	}

	return 0;
}

static int vote_xpu_clk_enable(struct clk_hw *hw)
{
	struct hi3xxx_vote_xpu_clk *pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);
	int ret = 0;

	/* SMC into ATF to open the gate */
	ret = atfd_service_clk_smc(CLK_VOTE_XPU, pclk->xpu_clk_id, VOTE_XPU_CLK_ENABLE, 0);
	if (ret) {
		pr_err("[%s] clk name =%s id-%u fail, ret = %d\n",
			__func__, clk_hw_get_name(hw), pclk->xpu_clk_id, ret);
		return ret;
	}

	return 0;
}

static void vote_xpu_clk_disable(struct clk_hw *hw)
{
#ifndef CONFIG_CLK_ALWAYS_ON
	struct hi3xxx_vote_xpu_clk *pclk = container_of(hw, struct hi3xxx_vote_xpu_clk, hw);
	int ret = 0;

	if (pclk->always_on)
		return;

	/* SMC into ATF to disable the gate */
	ret = atfd_service_clk_smc(CLK_VOTE_XPU, pclk->xpu_clk_id, VOTE_XPU_CLK_DISABLE, 0);
	if (ret) {
		pr_err("[%s] clk name =%s id-%u fail, ret = %d\n",
			__func__, clk_hw_get_name(hw), pclk->xpu_clk_id, ret);
		return;
	}
#endif
}

static const struct clk_ops vote_xpu_clk_ops = {
	.recalc_rate = vote_xpu_clk_recalc_rate,
	.set_rate = vote_xpu_clk_set_rate,
	.determine_rate = vote_xpu_clk_determine_rate,
	.enable = vote_xpu_clk_enable,
	.disable = vote_xpu_clk_disable,
};

static void __freq_vote_reg_init(struct hi3xxx_vote_xpu_clk *pclk,
	const struct vote_xpu_clock *vote_xpu_clk)
{
	unsigned int shift, width;

	pclk->vote_reg.addr = vote_xpu_clk->vote_reg.addr;
	pclk->vote_reg.mask_bit = vote_xpu_clk->vote_reg.mask_bit;
	shift = vote_xpu_clk->vote_reg.freq_shift;
	width = vote_xpu_clk->vote_reg.freq_width;
	pclk->vote_reg.freq_shift = shift;
	pclk->vote_reg.freq_mask = WIDTH_MASK(width) << shift;

	pclk->stat_reg.addr = vote_xpu_clk->stat_reg.addr;
	shift = vote_xpu_clk->stat_reg.freq_shift;
	width = vote_xpu_clk->stat_reg.freq_width;
	pclk->stat_reg.freq_shift = shift;
	pclk->stat_reg.freq_mask = WIDTH_MASK(width) << shift;

	shift = vote_xpu_clk->stat_reg.err_code_shift;
	width = vote_xpu_clk->stat_reg.err_code_width;
	pclk->stat_reg.err_code_shift = shift;
	pclk->stat_reg.err_code_mask = WIDTH_MASK(width) << shift;

	pclk->result_freq_reg = vote_xpu_clk->result_freq_reg;

	pclk->intr_reg.addr = vote_xpu_clk->intr_reg.addr;
	pclk->intr_reg.stat_bit = vote_xpu_clk->intr_reg.stat_bit;
}

static struct clk *__clk_register_vote_xpu_clk(const struct vote_xpu_clock *vote_xpu_clk,
	struct clock_data *data)
{
	struct hi3xxx_vote_xpu_clk *pclk = NULL;
	struct clk_init_data init;
	struct clk *clk = NULL;

	pclk = kzalloc(sizeof(*pclk), GFP_KERNEL);
	if (pclk == NULL) {
		pr_err("[%s] fail to alloc pclk!\n", __func__);
		return clk;
	}

	init.name = vote_xpu_clk->name;
	init.ops  = &vote_xpu_clk_ops;
	init.flags = CLK_IS_ROOT | CLK_GET_RATE_NOCACHE;
	init.parent_names = NULL;
	init.num_parents = 0;
	pclk->cur_freq = 0;
	pclk->hw.init = &init;
	pclk->base_addr = data->base; /* must be MEDIA PMCTRL addr */
	pclk->xpu_clk_id = vote_xpu_clk->xpu_clk_id;
	pclk->always_on	= vote_xpu_clk->always_on;
	pclk->low_temperature_property = vote_xpu_clk->low_temperature_property;
	pclk->low_temperature_freq = vote_xpu_clk->low_temperature_freq;

	__freq_vote_reg_init(pclk, vote_xpu_clk);

	clk = clk_register(NULL, &pclk->hw);
	if (IS_ERR(clk)) {
		pr_err("[%s] fail to reigister clk %s!\n",
			__func__, vote_xpu_clk->name);
		goto err_init;
	}

	/* init is local variable, need set NULL before func */
	pclk->hw.init = NULL;
	return clk;

err_init:
	kfree(pclk);
	return clk;
}

void plat_clk_register_vote_xpu_clk(const struct vote_xpu_clock *clks,
	int nums, struct clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = __clk_register_vote_xpu_clk(&clks[i], data);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("[%s] failed to register clock %s\n",
				__func__, clks[i].name);
			continue;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);
		clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}