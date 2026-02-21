/*
 * Copyright (c) 2024-2027 Huawei Technologies Co., Ltd.
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

#include "clk-comb-dvfs.h"
#include "securec.h"

static struct hi3xxx_comb_dvfs *g_comb_dvfs_tbl[MAX_COMB_DVFS_NUM];

#ifdef CONFIG_CLK_DEBUG
static void comb_dvfs_debug_level_show(struct seq_file *s,
	struct hi3xxx_comb_dvfs *comb_dvfs)
{
	const struct level_freq *level_info;
	u8 i, j;

	seq_puts(s, "\n----- Level Configure START ----\n");

	for (i = 0; i < comb_dvfs->level_num; i++) {
		level_info = &(comb_dvfs->sensitive_level_freqs[i]);
		seq_printf(s, "  level         : %u\n", i);
		seq_printf(s, "  pvp_id        : %u\n", level_info->pvp_id);
		seq_printf(s, "  volt          : %u\n", level_info->volt);
		for (j = 0; j < comb_dvfs->clk_num; j++)
			seq_printf(s, "  sensitive_freq[%u]  : %lu\n",
				j, level_info->sensitive_freq[j]);
	}

	seq_puts(s, "\n----- Level Clock Configure END ----\n");
}

static void comb_dvfs_debug_clk_show(struct seq_file *s,
	struct hi3xxx_comb_dvfs *comb_dvfs)
{
	const struct comb_clock *comb_clk;
	u8 i, j;

	seq_puts(s, "\n----- Combine Clock Configure START ----\n");

	for (i = 0; i < comb_dvfs->clk_num; i++) {
		comb_clk = &(comb_dvfs->comb_clks[i]);
		seq_printf(s, "  clk_id        : %u\n", i);
		seq_printf(s, "  clk_name      : %s\n", comb_clk->clk_name);
		seq_printf(s, "  default_freq  : %u(kHz)\n", comb_clk->pwoff_freq);
		seq_printf(s, "  current_freq  : %lu\n", comb_dvfs->cur_freqs[i]);
		for (j = 0; j < comb_dvfs->level_num; j++)
			seq_printf(s, "  sensitive_freq[%u]  : %u\n",
				j, comb_clk->sensitive_freqs[j]);
	}

	seq_puts(s, "\n----- Combine Clock Configure END ----\n");
}

static void comb_dvfs_debug_show(struct seq_file *s,
	struct hi3xxx_comb_dvfs *comb_dvfs)
{
	seq_printf(s, "  id        : %u\n", comb_dvfs->id);
	seq_printf(s, "  name      : %s\n", comb_dvfs->name);
	seq_printf(s, "  levels    : %u\n", comb_dvfs->level_num);
	seq_printf(s, "  clks      : %u\n", comb_dvfs->clk_num);
	seq_printf(s, "  cur_level : %u\n", comb_dvfs->cur_level);

	comb_dvfs_debug_clk_show(s, comb_dvfs);
	comb_dvfs_debug_level_show(s, comb_dvfs);
}

static int comb_dvfs_summary_show(struct seq_file *s, void *data)
{
	unsigned int i;

	if (!s)
		return 0;

	seq_puts(s, "\n<---- Show Combine DVFS Configure START --->\n");

	for (i = 0; i < MAX_COMB_DVFS_NUM; i++) {
		if (g_comb_dvfs_tbl[i] == NULL)
			continue;
		comb_dvfs_debug_show(s, g_comb_dvfs_tbl[i]);
	}

	seq_puts(s, "<---- Show Combine DVFS Configure END --->\n");
	return 0;
}

static int comb_dvfs_summary_open(struct inode *inode, struct file *file)
{
	return single_open(file, comb_dvfs_summary_show, inode->i_private);
}

static const struct file_operations comb_dvfs_summary_fops = {
	.open    = comb_dvfs_summary_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int __init comb_dvfs_debug_init(void)
{
#ifdef CONFIG_DFX_DEBUG_FS
	struct dentry *dvs_parent = NULL;
	struct dentry *dvs_info = NULL;

	dvs_parent = debugfs_create_dir("comb_dvfs", NULL);
	if (!dvs_parent) {
		pr_err("create comb_dvfs debugfs dir failed\n");
		return -ENOMEM;
	}

	dvs_info = debugfs_create_file("dvfs_info", S_IRUGO, dvs_parent,
			NULL, &comb_dvfs_summary_fops);
	if (!dvs_info) {
		pr_err("create dvfs_info debugfs file failed\n");
		return -ENOMEM;
	}

	return 0;
#endif /* CONFIG_DFX_DEBUG_FS */
}
late_initcall(comb_dvfs_debug_init);
#endif

#ifdef CONFIG_PERI_DVFS
static inline struct hi3xxx_comb_dvfs *get_com_dvfs(u32 dvfs_clk_id)
{
	if (dvfs_clk_id >= MAX_COMB_DVFS_NUM)
		return NULL;
	return g_comb_dvfs_tbl[dvfs_clk_id];
}

static inline void comb_dvfs_preset(struct hi3xxx_comb_dvfs *comb_dvfs,
	u32 clk_id, unsigned long rate)
{
	comb_dvfs->tmp_freq = comb_dvfs->cur_freqs[clk_id];
	comb_dvfs->cur_freqs[clk_id] = rate;
}

static inline void comb_dvfs_recovery(struct hi3xxx_comb_dvfs *comb_dvfs, u32 clk_id)
{
	comb_dvfs->cur_freqs[clk_id] = comb_dvfs->tmp_freq;
}

static bool is_all_clock_disable(struct hi3xxx_comb_dvfs *comb_dvfs)
{
	u32 i;

	for (i = 0; i < comb_dvfs->clk_num; i++)
		if (comb_dvfs->cur_freqs[i] != 0)
			break;

	if (i == comb_dvfs->clk_num)
		return true;
	return false;
}

static int comb_dvfs_channel_vote(u32 pvp_id, u32 endis, u32 volt)
{
	struct peri_volt_poll *pvp = NULL;
	int ret;

	pvp = peri_volt_poll_get(pvp_id, NULL);
	if (IS_ERR_OR_NULL(pvp)) {
		pr_err("[%s] pvp-%u get failed!\n", __func__, pvp_id);
		return -EINVAL;
	}

	/* set new channel valid */
	ret = peri_set_avs(pvp, endis);
	if (ret < 0) {
		pr_err("[%s] pvp-%u set valid-%u failed!\n",
			__func__, pvp_id, endis);
		return ret;
	}

	/* set new volt */
	ret = peri_set_volt(pvp, volt);
	if (ret < 0) {
		pr_err("[%s] pvp-%u set valid-%u volt-%u failed ret = %d!\n",
			__func__, pvp_id, endis, volt, ret);
		return ret;
	}

	return 0;
}

static int comb_dvfs_change_volt(struct hi3xxx_comb_dvfs *comb_dvfs, u8 new_level)
{
	struct level_freq *old_level_freq = NULL;
	struct level_freq *new_level_freq = NULL;
	int ret;

	/* level not updated, return */
	if (new_level == comb_dvfs->cur_level)
		return 0;

	if (new_level != CLOCK_DISABLE_LEVEL)
		new_level_freq = &(comb_dvfs->sensitive_level_freqs[new_level]);
	if (comb_dvfs->cur_level != CLOCK_DISABLE_LEVEL)
		old_level_freq = &(comb_dvfs->sensitive_level_freqs[comb_dvfs->cur_level]);

	/* same pvp means same channel, return */
	if (new_level_freq && old_level_freq &&
		(new_level_freq->pvp_id == old_level_freq->pvp_id)) {
		comb_dvfs->cur_level = new_level;
		return 0;
	}

	/* new channel vote */
	if (new_level_freq) {
		ret = comb_dvfs_channel_vote(new_level_freq->pvp_id,
				PERI_AVS_ENABLE, new_level_freq->volt);
		if (ret < 0)
			return ret;
	}

	/* old channel unvote */
	if (old_level_freq) {
		ret = comb_dvfs_channel_vote(old_level_freq->pvp_id,
				PERI_AVS_DISABLE, PERI_VOLT_0);
		if (ret < 0)
			return ret;
	}

	comb_dvfs->cur_level = new_level;
	return 0;
}

static u32 get_comb_dvfs_level(struct hi3xxx_comb_dvfs *comb_dvfs)
{
	struct level_freq *level_freqs = NULL;
	u32 level = 0;
	u32 i, j;

	if (comb_dvfs->level_num == 0)
		return 0;

	for (i = 0; i < comb_dvfs->level_num; i++) {
		level_freqs = &(comb_dvfs->sensitive_level_freqs[i]);
		for (j = 0; j < comb_dvfs->clk_num; j++) {
			if (comb_dvfs->cur_freqs[j] > level_freqs->sensitive_freq[j])
				break;
		}

		if (j == comb_dvfs->clk_num) {
			level = i;
			break;
		}
	}

	if (i == comb_dvfs->level_num)
		level = comb_dvfs->level_num - 1;
	return level;
}

static int comb_dvfs_set_rate(u32 dvfs_clk_id, u32 clk_id, unsigned long rate)
{
	struct hi3xxx_comb_dvfs *comb_dvfs = NULL;
	int level, ret;

	comb_dvfs = get_com_dvfs(dvfs_clk_id);
	if (comb_dvfs == NULL) {
		pr_err("[%s] comb_dvfs-%u had not init!\n", __func__, dvfs_clk_id);
		return -EINVAL;
	}

	if (clk_id >= comb_dvfs->clk_num) {
		pr_err("[%s] %s invalid clk_id:%u clk_num:%u!\n",
			__func__, comb_dvfs->name, clk_id, comb_dvfs->clk_num);
		return -EINVAL;
	}

	comb_dvfs_preset(comb_dvfs, clk_id, rate);

	/* get comb dvfs new level */
	level = get_comb_dvfs_level(comb_dvfs);

	ret = comb_dvfs_change_volt(comb_dvfs, level);
	if (ret < 0) {
		comb_dvfs_recovery(comb_dvfs, clk_id);
		pr_err("[%s] change volt failed ret = %d!\n", __func__, ret);
		return ret;
	}

	return 0;
}

static int comb_dvfs_unprepare(u32 dvfs_clk_id, u32 clk_id)
{
	struct hi3xxx_comb_dvfs *comb_dvfs = NULL;
	int ret;

	comb_dvfs = get_com_dvfs(dvfs_clk_id);
	if (comb_dvfs == NULL) {
		pr_err("[%s] comb_dvfs-%u had not init!\n", __func__, dvfs_clk_id);
		return -EINVAL;
	}

	if (clk_id >= comb_dvfs->clk_num) {
		pr_err("[%s] %s invalid clk_id:%u clk_num:%u!\n",
			__func__, comb_dvfs->name, clk_id, comb_dvfs->clk_num);
		return -EINVAL;
	}

	comb_dvfs_preset(comb_dvfs, clk_id, 0);

	if (is_all_clock_disable(comb_dvfs)) {
		ret = comb_dvfs_change_volt(comb_dvfs, CLOCK_DISABLE_LEVEL);
		if (ret < 0)
			/* clock disable failed, no recovery required */
			pr_err("[%s] change volt failed ret = %d!\n", __func__, ret);
	}

	return 0;
}

static void comb_dvfs_info_init(struct hi3xxx_comb_dvfs *comb_item,
	const struct comb_dvfs *item)
{
	struct level_freq *level_freqs = NULL;
	u8 i, j;

	comb_item->id = item->id;
	comb_item->name = item->name;
	comb_item->tmp_freq = 0;
	comb_item->cur_level = CLOCK_DISABLE_LEVEL;
	comb_item->clk_num = item->num_clks;
	comb_item->level_num = item->num_levels;
	comb_item->comb_clks = item->comb_clks;
	level_freqs = comb_item->sensitive_level_freqs;
	memset_s(comb_item->cur_freqs, sizeof(comb_item->cur_freqs),
		0, sizeof(comb_item->cur_freqs));

	for (i = 0; i < item->num_levels; i++) {
		level_freqs[i].pvp_id = item->level_infos[i].pvp_id;
		level_freqs[i].volt = item->level_infos[i].volt;
		for (j = 0; j < item->num_clks; j++)
			level_freqs[i].sensitive_freq[j] =
				item->comb_clks[j].sensitive_freqs[i] * FREQ_CONVERSION;
	}

	return;
}

static int clk_register_comb_dvfs(const struct comb_dvfs *item)
{
	struct hi3xxx_comb_dvfs *comb_item = NULL;

	if (!item->comb_clks || !item->level_infos) {
		pr_err("[%s] NULL pointer!\n", __func__);
		return -EINVAL;
	}

	if (item->id >= MAX_COMB_DVFS_NUM) {
		pr_err("[%s] invalid clk_id:%u!\n", __func__, item->id);
		return -EINVAL;
	}
	if ((item->num_clks == 0) || (item->num_clks > MAX_CLK_NUM)) {
		pr_err("[%s] invalid clk_num:%u!\n", __func__, item->num_clks);
		return -EINVAL;
	}
	if ((item->num_levels == 0) || (item->num_levels > MAX_LEVEL_NUM)) {
		pr_err("[%s] invalid level_num:%u!\n", __func__, item->num_levels);
		return -EINVAL;
	}

	comb_item = kzalloc(sizeof(*comb_item), GFP_KERNEL);
	if (IS_ERR_OR_NULL(comb_item))
		return -ENOMEM;

	comb_dvfs_info_init(comb_item, item);
	g_comb_dvfs_tbl[comb_item->id] = comb_item;

	return 0;
}

static void comb_dvfs_table_init(void)
{
	int i;
	for (i = 0; i < MAX_COMB_DVFS_NUM; i++)
		g_comb_dvfs_tbl[i] = NULL;
}

static void comb_dvfs_table_deinit(void)
{
	int i;

	for (i = 0; i < MAX_COMB_DVFS_NUM; i++) {
		if (g_comb_dvfs_tbl[i] != NULL) {
			kfree(g_comb_dvfs_tbl[i]);
			g_comb_dvfs_tbl[i] = NULL;
		}
	}
}

static int peri_temperature(struct hi3xxx_comb_dvfs_clock *pclk)
{
	struct peri_volt_poll *pvp = NULL;

	if (pclk->low_temperature_property != LOW_TEMPERATURE_PROPERTY)
		return 0;

	pvp = peri_volt_poll_get(pclk->pvp_id, NULL);
	if (pvp == NULL) {
		pr_err("[%s]pvp get failed, pvp_id = %u!\n", __func__, pclk->pvp_id);
		return -EINVAL;
	}

	if (peri_get_temperature(pvp) != NORMAL_TEMPRATURE)
		return LOW_TEMPERATURE;

	return 0;
}

static int __comb_dvfs_clk_prepare(struct hi3xxx_comb_dvfs_clock *dfclk)
{
	unsigned long cur_rate;
	unsigned int mul_ret;
	int ret;

	cur_rate = __clk_get_rate(dfclk->hw.clk);
	if (!cur_rate)
		pr_err("[%s]soft rate: must not be 0,please check!\n", __func__);

	if (peri_temperature(dfclk)) {
		mul_ret = mul_valid_cal(dfclk->low_temperature_freq, FREQ_CONVERSION);
		if (!mul_ret)
			return -EINVAL;
		if (cur_rate > (dfclk->low_temperature_freq * FREQ_CONVERSION)) {
			pr_err("[%s]: cur_freq-%lu > low_temperature_freq-%lu!\n",
				__func__, cur_rate, dfclk->low_temperature_freq);
			return -ELOW_TEMPERATURE;
		}
	}

	ret = comb_dvfs_set_rate(dfclk->dvfs_clk_id, dfclk->clk_vote_id, cur_rate);
	if (ret) {
		pr_err("[%s] comb_dvfs prepare faild, ret = %d!\n", __func__, ret);
		return ret;
	}

	return 0;
}

static void __comb_dvfs_clk_unprepare(struct hi3xxx_comb_dvfs_clock *dfclk)
{
	comb_dvfs_unprepare(dfclk->dvfs_clk_id, dfclk->clk_vote_id);
}

static int __comb_dvfs_clk_set_rate(struct hi3xxx_comb_dvfs_clock *dfclk,
	struct clk *friend_clk, unsigned long rate)
{
	unsigned long cur_rate;
	int ret;

	cur_rate = __clk_get_rate(friend_clk);
	if (!cur_rate)
		pr_err("[%s]soft rate: must not be 0, please check!\n", __func__);

	if (rate > cur_rate) {
		ret = comb_dvfs_set_rate(dfclk->dvfs_clk_id, dfclk->clk_vote_id, rate);
		if (ret) {
			pr_err("[%s] fail to raise volt, ret = %d!\n", __func__, ret);
			return ret;
		}

		ret = clk_set_rate_nolock(friend_clk, rate);
		if (ret) {
			pr_err("[%s] fail to updata rate, ret = %d!\n", __func__, ret);
			return ret;
		}
	} else {
		ret = clk_set_rate_nolock(friend_clk, rate);
		if (ret) {
			pr_err("[%s] fail to updata rate, ret = %d!\n", __func__, ret);
			return ret;
		}

		ret = comb_dvfs_set_rate(dfclk->dvfs_clk_id, dfclk->clk_vote_id, rate);
		if (ret) {
			pr_err("[%s] fail to reduce volt, ret = %d!\n", __func__, ret);
			return ret;
		}
	}

	return 0;
}
#endif

/* func: get cur freq */
static unsigned long comb_dvfs_clk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *clk_friend = NULL;

	clk_friend = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(clk_friend)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return 0;
	}

	return __clk_get_rate(clk_friend);
}

static int comb_dvfs_clk_determine_rate(struct clk_hw *hw, struct clk_rate_request *req)
{
#ifdef CONFIG_PERI_DVFS
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	u32 mul_ret;

	if (peri_temperature(dfclk)) {
		mul_ret = mul_valid_cal(dfclk->low_temperature_freq, FREQ_CONVERSION);
		if (!mul_ret)
			return -EINVAL;

		if (req->rate > (dfclk->low_temperature_freq * FREQ_CONVERSION)) {
			pr_err("[%s]: clk name =%s, cur_freq-%lu > low_temperature_freq-%lu!\n",
				__func__, clk_hw_get_name(hw), req->rate,
				dfclk->low_temperature_freq);
			return -ELOW_TEMPERATURE;
		}
	}
#endif
	return 0;
}

/* func: dvfs set rate main func */
static int comb_dvfs_clk_set_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long parent_rate)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *friend_clk = NULL;
	int ret;
#ifdef CONFIG_PERI_DVFS
	u32 mul_ret;
#endif

	friend_clk = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(friend_clk)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return -ENODEV;
	}

#ifdef CONFIG_PERI_DVFS
	if (peri_temperature(dfclk)) {
		mul_ret = mul_valid_cal(dfclk->low_temperature_freq, FREQ_CONVERSION);
		if (!mul_ret)
			return -EINVAL;
		if (rate > (dfclk->low_temperature_freq * FREQ_CONVERSION)) {
			pr_err("[%s]: cur_rate-%lu > low_temperature_freq-%lu!\n",
				__func__, rate, dfclk->low_temperature_freq);
			return -ELOW_TEMPERATURE;
		}
	}

	if (__clk_get_enable_count(friend_clk)) {
		ret = __comb_dvfs_clk_set_rate(dfclk, friend_clk, rate);
		if (ret < 0)
			pr_err("[%s] fail to set rate, ret = %d, %d!\n",
				__func__, ret, __LINE__);
		return ret;
	}
#endif

	ret = clk_set_rate_nolock(friend_clk, rate);
	if (ret < 0)
		pr_err("[%s] fail to set rate, ret = %d, %d!\n",
			__func__, ret, __LINE__);
	return ret;
}

static int comb_dvfs_clk_prepare(struct clk_hw *hw)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *friend_clk = NULL;
	unsigned long cur_rate;
	int ret;

	friend_clk = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(friend_clk)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return -ENODEV;
	}

	ret = plat_core_prepare(friend_clk);
	if (ret) {
		pr_err("[%s] clock %s prepare faild!\n", __func__, dfclk->link);
		return ret;
	}

#ifdef CONFIG_PERI_DVFS
	ret = __comb_dvfs_clk_prepare(dfclk);
	if (ret) {
		plat_core_unprepare(friend_clk);
		pr_err("[%s] clock %s prepare dvfs faild, ret = %d!\n",
			__func__, clk_hw_get_name(hw), ret);
		return ret;
	}
#endif

	return 0;
}

static void comb_dvfs_clk_unprepare(struct clk_hw *hw)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *friend_clk = NULL;

	friend_clk = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(friend_clk)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return;
	}

	plat_core_unprepare(friend_clk);

#ifdef CONFIG_PERI_DVFS
	__comb_dvfs_clk_unprepare(dfclk);
#endif
	return;
}

/* func: dvfs clk enable func */
static int comb_dvfs_clk_enable(struct clk_hw *hw)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *friend_clk = NULL;
	int ret;

	friend_clk = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(friend_clk)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return -ENODEV;
	}

	ret = __clk_enable(friend_clk);
	if (ret) {
		pr_err("[%s], friend clock enable faild!", __func__);
		return ret;
	}

	return ret;
}

/* func: dvfs clk disable func */
static void comb_dvfs_clk_disable(struct clk_hw *hw)
{
	struct hi3xxx_comb_dvfs_clock *dfclk = container_of(hw,
		struct hi3xxx_comb_dvfs_clock, hw);
	struct clk *friend_clk = NULL;

	friend_clk = __clk_lookup(dfclk->link);
	if (IS_ERR_OR_NULL(friend_clk)) {
		pr_err("[%s] %s get failed!\n", __func__, dfclk->link);
		return;
	}

	__clk_disable(friend_clk);
}

static const struct clk_ops comb_comb_dvfs_clk_ops = {
	.recalc_rate = comb_dvfs_clk_recalc_rate,
	.set_rate = comb_dvfs_clk_set_rate,
	.determine_rate = comb_dvfs_clk_determine_rate,
	.prepare = comb_dvfs_clk_prepare,
	.unprepare = comb_dvfs_clk_unprepare,
	.enable = comb_dvfs_clk_enable,
	.disable = comb_dvfs_clk_disable,
};

static struct clk *clk_register_comb_dvfs_clk(const struct comb_dvfs_clock *dvfs,
	struct clock_data *data)
{
	struct clk *clk = NULL;
	struct clk_init_data init;
	struct hi3xxx_comb_dvfs_clock *devfreq_clk = NULL;

	devfreq_clk = kzalloc(sizeof(*devfreq_clk), GFP_KERNEL);
	if (IS_ERR_OR_NULL(devfreq_clk))
		return NULL;

	init.name = dvfs->name;
	init.ops = &comb_comb_dvfs_clk_ops;
	init.parent_names = NULL;
	init.num_parents = 0;
	init.flags = CLK_IS_ROOT | CLK_GET_RATE_NOCACHE;

	devfreq_clk->hw.init = &init;
	devfreq_clk->link = dvfs->link;
	devfreq_clk->dvfs_clk_id = dvfs->dvfs_clk_id;
	devfreq_clk->clk_vote_id = dvfs->vote_id;
	devfreq_clk->pvp_id = dvfs->pvp_id;
	devfreq_clk->low_temperature_property = dvfs->low_temperature_property;
	devfreq_clk->low_temperature_freq = dvfs->low_temperature_freq;

	clk = clk_register(NULL, &devfreq_clk->hw);
	if (IS_ERR(clk)) {
		pr_err("[%s] fail to register devfreq_clk %s!\n",
			__func__, dvfs->name);
		goto err_init;
	}

	devfreq_clk->hw.init = NULL;
	return clk;

err_init:
	kfree(devfreq_clk);
	return clk;
}

void plat_clk_register_comb_dvfs(const struct comb_dvfs *clks, int nums)
{
#ifdef CONFIG_PERI_DVFS
	int i, ret;

	comb_dvfs_table_init();

	for (i = 0; i < nums; i++) {
		ret = clk_register_comb_dvfs(&clks[i]);
		if (ret) {
			pr_err("%s: failed to register comb_dvfs %s\n",
				   __func__, clks[i].name);
			goto err_init;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);
	}
	return;

err_init:
	comb_dvfs_table_deinit();
	return;
#endif
}

void plat_clk_register_comb_dvfs_clk(const struct comb_dvfs_clock *clks,
	int nums, struct clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = clk_register_comb_dvfs_clk(&clks[i], data);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
				   __func__, clks[i].name);
			continue;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}
