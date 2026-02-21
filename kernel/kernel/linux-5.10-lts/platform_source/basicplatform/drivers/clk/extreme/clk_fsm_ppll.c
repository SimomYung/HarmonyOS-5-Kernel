/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * clk_fsm_ppll.c
 *
 * fsm ppll API
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

#include "clk_fsm_ppll.h"

static int get_fsm_ppll_state(struct hi3xxx_fsm_ppll_clk *ppll_clk)
{
	u32 val;

	/* en need 1 */
	val = readl(ppll_clk->addr + ppll_clk->fsm_en_ctrl[0]);
	if (!(val & BIT(ppll_clk->fsm_en_ctrl[1])))
		return 0;
	return 1;
}

static void set_fsm_ppll_en_vote(struct hi3xxx_fsm_ppll_clk *ppll_clk)
{
	u32 val;

	val = BIT(ppll_clk->fsm_en_ctrl[1] + FSM_PLL_MASK_OFFSET);
	val |= BIT(ppll_clk->fsm_en_ctrl[1]);
	writel(val, ppll_clk->addr + ppll_clk->fsm_en_ctrl[0]); /* fsm en 1'b1 */
}

static void wait_fsm_ppll_stat(struct hi3xxx_fsm_ppll_clk *ppll_clk)
{
	unsigned int reg_data1, reg_data2;
	int delay_time = 0;

	/*
	 * reads back twice as 1
	 * before it can be judged as working
	 */
	do {
		delay_time++;
		reg_data1= readl(ppll_clk->addr + ppll_clk->fsm_stat_ctrl[0]);
		reg_data2= readl(ppll_clk->addr + ppll_clk->fsm_stat_ctrl[0]);
		udelay(1);
		if (delay_time > AP_FSM_PPLL_STABLE_TIME) {
			pr_err("%s: ppll-%u enable is timeout\n",
				__func__, ppll_clk->pll_id);
			return;
		}
	} while (!(reg_data1 & BIT(ppll_clk->fsm_stat_ctrl[1])) ||
		!(reg_data2 & BIT(ppll_clk->fsm_stat_ctrl[1])));
}

static int fsm_ppll_enable(struct clk_hw *hw)
{
	struct hi3xxx_fsm_ppll_clk *ppll_clk = container_of(hw,
		struct hi3xxx_fsm_ppll_clk, hw);
	int ret;

	/* enable count */
	ppll_clk->ref_cnt++;

	if (ppll_clk->pll_id == PPLL0)
		return 0;
	if (ppll_clk->ref_cnt == 1) {
		ret = get_fsm_ppll_state(ppll_clk);
		if (ret != 0)
			return 0;
		set_fsm_ppll_en_vote(ppll_clk);
		udelay(30);
		wait_fsm_ppll_stat(ppll_clk);
	}
	return 0;
}

static void fsm_ppll_disable(struct clk_hw *hw)
{
	struct hi3xxx_fsm_ppll_clk *ppll_clk = container_of(hw, struct hi3xxx_fsm_ppll_clk, hw);
	unsigned int val;

	/* enable count */
	ppll_clk->ref_cnt--;
	if (ppll_clk->pll_id == PPLL0)
		return;
#ifndef CONFIG_CLK_ALWAYS_ON
	if (!ppll_clk->ref_cnt) {
		/* ~en */
		val = BIT(ppll_clk->fsm_en_ctrl[1] + FSM_PLL_MASK_OFFSET);
		writel(val, ppll_clk->addr + ppll_clk->fsm_en_ctrl[0]); /* en 1'b0 */
	}
#endif
}

static const struct clk_ops fsm_ppll_ops = {
	.enable = fsm_ppll_enable,
	.disable = fsm_ppll_disable,
};

static struct clk *__plat_clk_register_fsm_pll(
	const struct fsm_pll_clock *fsm_pll,
	struct clock_data *data)
{
	struct hi3xxx_fsm_ppll_clk *fsm_ppll_clk = NULL;
	struct clk_init_data init;
	struct clk *clk = NULL;
	unsigned int i;
	struct hs_clk *hs_clk = get_hs_clk_info();

	fsm_ppll_clk = kzalloc(sizeof(*fsm_ppll_clk), GFP_KERNEL);
	if (IS_ERR_OR_NULL(fsm_ppll_clk)) {
		pr_err("[%s] fail to alloc pclk!\n", __func__);
		return clk;
	}
	/* initialize the reference count */
	fsm_ppll_clk->ref_cnt = 0;

	init.name = fsm_pll->name;
	init.ops = &fsm_ppll_ops;
	init.flags = CLK_SET_RATE_PARENT | CLK_IGNORE_UNUSED;
	init.parent_names = &(fsm_pll->parent_name);
	init.num_parents = 1;

	fsm_ppll_clk->pll_id = fsm_pll->pll_id;
	fsm_ppll_clk->lock = &hs_clk->lock;
	fsm_ppll_clk->hw.init = &init;
	fsm_ppll_clk->addr = data->base;

	for (i = 0; i < FSM_PLL_REG_NUM; i++) {
		fsm_ppll_clk->fsm_en_ctrl[i] = fsm_pll->fsm_en_ctrl[i];
		fsm_ppll_clk->fsm_stat_ctrl[i] = fsm_pll->fsm_stat_ctrl[i];
	}

	clk = clk_register(NULL, &fsm_ppll_clk->hw);
	if (IS_ERR(clk)) {
		pr_err("[%s] fail to reigister clk %s!\n",
			__func__, fsm_pll->name);
		goto err_register;
	}

	/* init is local variable, need set NULL before exit func */
	fsm_ppll_clk->hw.init = NULL;

	return clk;
err_register:
	kfree(fsm_ppll_clk);
	return clk;
}

void plat_clk_register_fsm_pll(const struct fsm_pll_clock *clks,
	int nums, struct clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = __plat_clk_register_fsm_pll(&clks[i], data);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

#ifdef CONFIG_CLK_DEBUG
		debug_clk_add(clk, CLOCK_FSM_PPLL);
#endif

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}

static int alter_fsm_ppll_determine_rate(struct clk_hw *hw,
	struct clk_rate_request *req)
{
	struct hi3xxx_fsm_ppll_clk *ppll_clk = container_of(hw,
		struct hi3xxx_fsm_ppll_clk, hw);
	struct hi3xxx_alter_fsm_ppll_clk *alter_fsm_ppll = NULL;
	int i;

	alter_fsm_ppll = container_of(ppll_clk,
		struct hi3xxx_alter_fsm_ppll_clk, fsm_pll);
	for (i = alter_fsm_ppll->freq_nums - 1; i >= 0; i--)
		if (req->rate >= alter_fsm_ppll->freq_tables[i].freq)
			break;

	if (i < 0)
		req->rate = alter_fsm_ppll->freq_tables[0].freq;
	else
		req->rate = alter_fsm_ppll->freq_tables[i].freq;

	return 0;
}

static unsigned long alter_fsm_ppll_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct hi3xxx_fsm_ppll_clk *ppll_clk = container_of(hw,
		struct hi3xxx_fsm_ppll_clk, hw);
	struct hi3xxx_alter_fsm_ppll_clk *alter_fsm_ppll = NULL;
	unsigned int ctrl0_cfg, ctrl1_cfg, ctrl0_mask, ctrl1_mask;
	unsigned int ctrl0_val, ctrl1_val;
	int i;

	alter_fsm_ppll = container_of(ppll_clk,
		struct hi3xxx_alter_fsm_ppll_clk, fsm_pll);
	ctrl0_val = readl(ppll_clk->addr + alter_fsm_ppll->ctrl0_addr);
	ctrl1_val = readl(ppll_clk->addr + alter_fsm_ppll->ctrl1_addr);

	for (i = 0; i < alter_fsm_ppll->freq_nums; i++) {
		ctrl0_cfg = alter_fsm_ppll->freq_tables[i].ctrl0_cfg;
		ctrl0_mask = alter_fsm_ppll->freq_tables[i].ctrl0_mask;
		ctrl1_cfg = alter_fsm_ppll->freq_tables[i].ctrl1_cfg;
		ctrl1_mask = alter_fsm_ppll->freq_tables[i].ctrl1_mask;
		if (((ctrl0_val & ctrl0_mask) == (ctrl0_cfg & ctrl0_mask)) &&
			((ctrl1_val & ctrl1_mask) == (ctrl1_cfg & ctrl1_mask)))
			break;
	}

	if (i == alter_fsm_ppll->freq_nums) {
		pr_err("%s pll-%d recalc rate failed!\n", __func__, ppll_clk->pll_id);
		return 0;
	}

	return alter_fsm_ppll->freq_tables[i].freq;
}

static int alter_fsm_ppll_set_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long parent_rate)
{
	struct hi3xxx_fsm_ppll_clk *ppll_clk = container_of(hw,
		struct hi3xxx_fsm_ppll_clk, hw);
	struct hi3xxx_alter_fsm_ppll_clk *alter_fsm_ppll = NULL;
	const struct pll_freq_info *pll_info;
	int i;

	if (ppll_clk->ref_cnt > 0) {
		pr_err("%s pll-%d must be disabled before set rate!\n", __func__, ppll_clk->pll_id);
		return -EINVAL;
	}

	alter_fsm_ppll = container_of(ppll_clk,
		struct hi3xxx_alter_fsm_ppll_clk, fsm_pll);
	for (i = alter_fsm_ppll->freq_nums - 1; i >= 0; i--)
		if (rate >= alter_fsm_ppll->freq_tables[i].freq)
			break;

	if (i < 0)
		pll_info = &(alter_fsm_ppll->freq_tables[0]);
	else
		pll_info = &(alter_fsm_ppll->freq_tables[i]);

	writel(pll_info->ctrl0_cfg, ppll_clk->addr + alter_fsm_ppll->ctrl0_addr);
	writel(pll_info->ctrl1_cfg, ppll_clk->addr + alter_fsm_ppll->ctrl1_addr);

	return 0;
}

static const struct clk_ops alter_fsm_ppll_ops = {
	.recalc_rate = alter_fsm_ppll_recalc_rate,
	.set_rate = alter_fsm_ppll_set_rate,
	.determine_rate = alter_fsm_ppll_determine_rate,
	.enable = fsm_ppll_enable,
	.disable = fsm_ppll_disable,
};

static struct clk *__plat_clk_register_alter_fsm_pll(
	const struct alter_fsm_pll_clock *fsm_pll,
	struct clock_data *data)
{
	struct hi3xxx_alter_fsm_ppll_clk *alter_fsm_ppll = NULL;
	struct hi3xxx_fsm_ppll_clk *fsm_ppll_clk = NULL;
	struct clk_init_data init;
	struct clk *clk = NULL;
	unsigned int i;
	struct hs_clk *hs_clk = get_hs_clk_info();

	alter_fsm_ppll = kzalloc(sizeof(*alter_fsm_ppll), GFP_KERNEL);
	if (IS_ERR_OR_NULL(alter_fsm_ppll)) {
		pr_err("[%s] fail to alloc pclk!\n", __func__);
		return NULL;
	}

	/* fsm pll frequence config init */
	alter_fsm_ppll->ctrl0_addr = fsm_pll->ctrl0_addr;
	alter_fsm_ppll->ctrl1_addr = fsm_pll->ctrl1_addr;
	alter_fsm_ppll->freq_nums = fsm_pll->freq_nums;
	alter_fsm_ppll->freq_tables = fsm_pll->freq_tables;

	/* initialize the reference count */
	fsm_ppll_clk = &(alter_fsm_ppll->fsm_pll);
	fsm_ppll_clk->ref_cnt = 0;

	init.name = fsm_pll->name;
	init.ops = &alter_fsm_ppll_ops;
	init.flags = CLK_IS_ROOT | CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE;
	init.num_parents = 0;

	fsm_ppll_clk->pll_id = fsm_pll->pll_id;
	fsm_ppll_clk->lock = &hs_clk->lock;
	fsm_ppll_clk->hw.init = &init;
	fsm_ppll_clk->addr = data->base;

	for (i = 0; i < FSM_PLL_REG_NUM; i++) {
		fsm_ppll_clk->fsm_en_ctrl[i] = fsm_pll->fsm_en_ctrl[i];
		fsm_ppll_clk->fsm_stat_ctrl[i] = fsm_pll->fsm_stat_ctrl[i];
	}

	clk = clk_register(NULL, &fsm_ppll_clk->hw);
	if (IS_ERR(clk)) {
		pr_err("[%s] fail to reigister clk %s!\n",
			__func__, fsm_pll->name);
		goto err_register;
	}

	/* init is local variable, need set NULL before exit func */
	fsm_ppll_clk->hw.init = NULL;

	return clk;
err_register:
	kfree(alter_fsm_ppll);
	return clk;
}

void plat_clk_register_alter_fsm_pll(const struct alter_fsm_pll_clock *clks,
	int nums, struct clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = __plat_clk_register_alter_fsm_pll(&clks[i], data);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

#ifdef CONFIG_CLK_DEBUG
		debug_clk_add(clk, CLOCK_FSM_PPLL);
#endif

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}