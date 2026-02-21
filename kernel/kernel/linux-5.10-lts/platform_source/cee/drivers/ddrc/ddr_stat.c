/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#include "ddr_stat.h"
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <securec.h>
#include <stddef.h>
#include <linux/io.h>
#if defined (CONFIG_PLATDRV_SUPPORT_FFA)
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#endif
#include <ddr/ddr_vote_statistic_struct.h>
#include <ddr/ddr_stat_struct.h>
#include <ddr/ddr_release_message.h>
#include <linux/atomic.h>

static ddr_release_message_str g_ddr_release_message;
atomic_t g_big_model_vote = ATOMIC_INIT(0);

#if defined(CONFIG_DDR_ORION)
static ddr_lat_info g_ddr_lat_info;
#endif

unsigned int get_force_vote(void)
{
	unsigned int value = atomic_read(&g_big_model_vote);
	return value;
}

void set_force_vote(unsigned int value)
{
	atomic_set(&g_big_model_vote, value);
}

int ddr_stat_get_info(ddr_release_message_str *info)
{
#if defined (CONFIG_PLATDRV_SUPPORT_FFA)

	struct ffa_send_direct_data args = {
		.data0 = FID_LP_DDR_STAT_GET,
		.data1 = SHMEM_OUT,
		.data4 = sizeof(ddr_release_message_str),
	};

	return ffa_platdrv_send_msg_with_shmem(&args, (u8 *)info, SHMEM_OUT);
#else
	return -1;
#endif
}

int get_info_from_shmem(void)
{
	int ret;

	memset_s(&g_ddr_release_message, sizeof(ddr_release_message_str), 0, sizeof(ddr_release_message_str));
	ret = ddr_stat_get_info(&g_ddr_release_message);
	if (ret) {
		pr_err("%s: ffa send fail\n", __func__);
		return -EINVAL;
	}

	return ret;
}

int set_info_to_shmem(unsigned int type, unsigned int value)
{
#if defined (CONFIG_PLATDRV_SUPPORT_FFA)
	int ret;
	unsigned int send_data[DDR_STAT_SEND_DATA_SIZE];
	send_data[0] = type;
	send_data[1] = value;
	struct ffa_send_direct_data args = {
		.data0 = FID_LP_DDR_STAT_SET,
		.data1 = SHMEM_IN,
		.data4 = sizeof(send_data),
	};
	ret = ffa_platdrv_send_msg_with_shmem(&args, (u8 *)send_data, SHMEM_IN);
	if (ret != 0) {
		pr_err("%s: ffa send fail\n", __func__);
		return -EINVAL;
	}
	return 0;
#else
	return -1;
#endif
}

static int ddr_stat_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}
	seq_printf(m, "freq id    time(ms)\n");
	ddr_stat_info* stat_info = &(g_ddr_release_message.lp_info.stat_info);
	for (unsigned int i = 0; i <= stat_info->freq_stat.freq_num; i++)
		seq_printf(m, "%d:    %d\n", i, stat_info->freq_stat.stat_ms[i]);
	return 0;
}

static int ddr_stat_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_stat_show, NULL);
}

static const struct proc_ops ddr_stat_proc_fops = {
	.proc_open = ddr_stat_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static int ddr_bandwidth_ctrl_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_bandwidth_ctrl_info* bandwidth_ctrl_info = &(g_ddr_release_message.bandwidth_info.bandwidth_ctrl_info);
	seq_printf(m, "enable:			%d\n", bandwidth_ctrl_info->enable);
	seq_printf(m, "interval:		%d\n", bandwidth_ctrl_info->interval);
	seq_printf(m, "duration:		%d\n", bandwidth_ctrl_info->duration);
	seq_printf(m, "threshold:		%d\n", bandwidth_ctrl_info->threshold);
	for (unsigned int i = 0; i < DDR_BAND_MNTN_MID_MAX; i++)
		seq_printf(m, "m[%d]:	%d\n", i, bandwidth_ctrl_info->master_id_enable[i]);
	return 0;
}

static int ddr_lp_ctrl_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_lp_ctrl_info* lp_ctrl_info = &(g_ddr_release_message.lp_info.lp_ctrl_info);
	ddr_stat_info* stat_info = &(g_ddr_release_message.lp_info.stat_info);
	seq_printf(m, "enable:			%d\n", lp_ctrl_info->enable);
	seq_printf(m, "interval:		%d\n", lp_ctrl_info->interval);
	seq_printf(m, "stat_en:			%d\n", stat_info->enable);
	return 0;
}

static int ddr_bandwidth_info_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_bandwidth_master_info* bandwidth_master_info = &(g_ddr_release_message.bandwidth_info.bandwidth_master_info[0]);
	for (unsigned int master_id = 0; master_id < DDR_BAND_MNTN_MID_MAX; master_id++)
		for (unsigned int i = 0; i < BANKWIDTH_MNTN_TIMES; i++)
			seq_printf(m, "m:%d   t:%d   fi:%d   f:%d   time:%d\n",
				master_id,
				i,
				bandwidth_master_info[master_id].freq_id[i],
				bandwidth_master_info[master_id].flux[i],
				bandwidth_master_info[master_id].timestamp[i]);

	return 0;
}

static int ddr_lp_vote_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_freq_vote_info* freq_vote_info = &(g_ddr_release_message.lp_info.freq_vote_info);
	for (unsigned int i = 0; i < XPU_DDR_FLOOR_VOTE_NUMS; i++)
		for (unsigned int j = 0; j < DDR_STAT_FREQ_NUM_MAX; j++)
			seq_printf(m, "m:%d    id:%d    dur:%d    f:%d\n",
				i,
				freq_vote_info->vote_stat[i].logic_id,
				freq_vote_info->vote_stat[i].stat_ms[j],
				j);
	return 0;
}

static int ddr_lp_statistic_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_lp_statistic_info* lp_statistic_info = &(g_ddr_release_message.lp_info.lp_statistic_info);
	for (unsigned int i = 0; i < DDR_LP_STATISTIC_MAX; i++)
		for (unsigned int j = 0; j < DDR_STAT_FREQ_NUM_MAX; j++)
			seq_printf(m, "i:%d    id:%d    v:%d    f:%d\n",
				i,
				lp_statistic_info->lp_statistic_msg[i].id,
				lp_statistic_info->lp_statistic_msg[i].stat[j],
				j);
	return 0;
}

static int ddr_lp_freq_track_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_freq_track* freq_track_info = &(g_ddr_release_message.lp_info.stat_info.freq_track);
	for (unsigned int i = 0; i < FREQ_TRACK_TIMES; i++)
		seq_printf(m, "f:%d t:%d\n", freq_track_info->freq_id[i], freq_track_info->timestamp[i]);
	return 0;
}

static int ddr_lp_load_track_show(struct seq_file *m, void *v)
{
	(void)v;
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}
	ddr_load_track* load_track_info = &(g_ddr_release_message.lp_info.stat_info.load_track);
	for (unsigned int i = 0; i < FREQ_TRACK_TIMES; i++)
		seq_printf(m, "f:%d s:%d t:%d\n",
			load_track_info->freq_id[i],
			load_track_info->trigger_source_id[i],
			load_track_info->timestamp[i]);
	return 0;
}

static int ddr_bandwidth_ctrl_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_bandwidth_ctrl_show, NULL);
}

static int ddr_bandwidth_info_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_bandwidth_info_show, NULL);
}

static int ddr_lp_ctrl_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_lp_ctrl_show, NULL);
}

static int ddr_lp_vote_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_lp_vote_show, NULL);
}

static int ddr_lp_statistic_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_lp_statistic_show, NULL);
}

static int ddr_lp_freq_track_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_lp_freq_track_show, NULL);
}

static int ddr_lp_load_track_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_lp_load_track_show, NULL);
}

int ddr_bandwidth_set_enable(unsigned int enable)
{
	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}

	if (enable != 0 && enable != 1)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_BAND_ENABLE, enable);
}

int ddr_bandwidth_set_enable_force(unsigned int enable)
{
	if (enable != 0 && enable != 1)
		return -EINVAL;

	set_force_vote(enable);
	return set_info_to_shmem(DDR_MNTN_BAND_ENABLE, 1);
}

static ssize_t ddr_bandwidth_enable_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int enable = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &enable) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_set_enable(enable);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_bandwidth_set_interval(unsigned int interval)
{
	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}

	if ((interval < DDR_STAT_BANDWIDTH_INTERVAL_MIN) || (interval > DDR_STAT_BANDWIDTH_INTERVAL_MAX))
		return -EINVAL;

	if ((interval > DDR_STAT_BANDWIDTH_INTERVAL_10S) && ((interval % DDR_STAT_BANDWIDTH_INTERVAL_10S) != 0))
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_BAND_INTERVAL, interval);
}

static ssize_t ddr_bandwidth_interval_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int interval = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &interval) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_set_interval(interval);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_bandwidth_set_duration_force(unsigned int duration)
{
	if (duration < 1 || duration > DDR_STAT_BANDWIDTH_DURATION_MAX)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_BAND_DURATION, duration);
}

int ddr_bandwidth_set_duration(unsigned int duration)
{
	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}

	return ddr_bandwidth_set_duration_force(duration);
}

static ssize_t ddr_bandwidth_duration_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int duration = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &duration) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_set_duration(duration);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_bandwidth_set_threshold_force(unsigned int threshold)
{
	if (threshold < 1 || threshold > DDR_STAT_BANDWIDTH_THRESHOLD_MAX)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_BAND_THRESHOLD, threshold);
}

int ddr_bandwidth_set_threshold(unsigned int threshold)
{
	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}

	return ddr_bandwidth_set_threshold_force(threshold);
}

static ssize_t ddr_bandwidth_threshold_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int threshold = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &threshold) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_set_threshold(threshold);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_bandwidth_trigger_once_force(void)
{
	return set_info_to_shmem(DDR_MNTN_BAND_TRIGGER_ONCE, 1);
}

int ddr_bandwidth_trigger_once(unsigned int trigger_once)
{
	if (trigger_once != 0 && trigger_once != 1)
		return -EINVAL;

	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}
	return set_info_to_shmem(DDR_MNTN_BAND_TRIGGER_ONCE, trigger_once);
}

static ssize_t ddr_bandwidth_trigger_once_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int trigger_once = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &trigger_once) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_trigger_once(trigger_once);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_bandwidth_set_master_id_force(unsigned int master_id, unsigned int enable)
{
	if ((master_id >= DDR_STAT_BANDWIDTH_MASTER_MAX) || (enable != 0 && enable != 1))
		return -EINVAL;

	set_info_to_shmem(DDR_MNTN_BAND_MASTER_ID, (master_id * DDR_STAT_ONE_HUNDRED + enable));
	return 0;
}

int ddr_bandwidth_set_master_id(unsigned int master_value)
{
	unsigned int master_id = master_value / DDR_STAT_ONE_HUNDRED;
	unsigned int enable = master_value % DDR_STAT_ONE_HUNDRED;

	if (get_force_vote()) {
		pr_err("big model is using band mntn\n");
		return -EINVAL;
	}

	ddr_bandwidth_set_master_id_force(master_id, enable);
	return 0;
}

static ssize_t ddr_bandwidth_master_id_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int master_value = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &master_value) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_bandwidth_set_master_id(master_value);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_lp_set_enable(unsigned int lp_enable)
{
	if (lp_enable != 0 && lp_enable != 1)
		return -EINVAL;

	set_info_to_shmem(DDR_MNTN_LP_ENABLE, lp_enable);
	return 0;
}

static ssize_t ddr_lp_enable_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lp_enable = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &lp_enable) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_lp_set_enable(lp_enable);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

static ssize_t ddr_lp_interval_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lp_interval = 0;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &lp_interval) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}
	if ((lp_interval < DDR_STAT_LP_INTERVAL_MIN ||
		lp_interval > DDR_STAT_LP_INTERVAL_MAX) ||
		((lp_interval % DDR_STAT_LP_INTERVAL_MIN) != 0)) {
		pr_err("%s data is invalid\n", __func__);
		return -EINVAL;
	}
	set_info_to_shmem(DDR_MNTN_LP_INTERVAL, lp_interval);
	return count;
}

static ssize_t ddr_lp_stat_enable_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lp_stat_enable = 0;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%d", &lp_stat_enable) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}
	if (lp_stat_enable != 0 && lp_stat_enable != 1) {
		pr_err("%s data is invalid\n", __func__);
		return -EINVAL;
	}
	set_info_to_shmem(DDR_MNTN_LP_STAT_ENABLE, lp_stat_enable);
	return count;
}

static const struct proc_ops ddr_bandwidth_enable_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_enable_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};
static const struct proc_ops ddr_bandwidth_interval_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_interval_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};
static const struct proc_ops ddr_bandwidth_duration_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_duration_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};
static const struct proc_ops ddr_bandwidth_threshold_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_threshold_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_bandwidth_trigger_once_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_trigger_once_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_bandwidth_master_fops = {
	.proc_open = ddr_bandwidth_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_bandwidth_master_id_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_bandwidth_info_fops = {
	.proc_open = ddr_bandwidth_info_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_enable_fops = {
	.proc_open = ddr_lp_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_lp_enable_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_interval_fops = {
	.proc_open = ddr_lp_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_lp_interval_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_vote_fops = {
	.proc_open = ddr_lp_vote_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_statistic_fops = {
	.proc_open = ddr_lp_statistic_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_stat_enable_fops = {
	.proc_open = ddr_lp_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_lp_stat_enable_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_freq_track_fops = {
	.proc_open = ddr_lp_freq_track_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_lp_load_track_fops = {
	.proc_open = ddr_lp_load_track_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

#if defined(CONFIG_DDR_ORION)
int ull_ddr_stat_get_info(ddr_lat_info *info)
{
#if defined (CONFIG_PLATDRV_SUPPORT_FFA)

	struct ffa_send_direct_data args = {
		.data0 = FID_DDR_ULL_GET,
		.data1 = SHMEM_OUT,
		.data4 = sizeof(ddr_lat_info),
	};

	return ffa_platdrv_send_msg_with_shmem(&args, (u8 *)info, SHMEM_OUT);
#else
	return -1;
#endif
}

int ull_get_info_from_shmem(void)
{
	int ret;

	memset_s(&g_ddr_lat_info, sizeof(ddr_lat_info), 0, sizeof(ddr_lat_info));
	ret = ull_ddr_stat_get_info(&g_ddr_lat_info);
	if (ret) {
		pr_err("%s: ffa send fail\n", __func__);
		return -EINVAL;
	}

	return ret;
}

static int ddr_latency_ctrl_show(struct seq_file *m, void *v)
{
	(void)v;
	if (ull_get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_lat_ctrl_info* latency_ctrl_info = &(g_ddr_lat_info.lat_ctrl_info);
	seq_printf(m, "enable:			%d\n", latency_ctrl_info->enable);
	seq_printf(m, "cpu_enable:		%d\n", latency_ctrl_info->cpu_enable);
	seq_printf(m, "interval:		%d\n", latency_ctrl_info->interval);
	for (unsigned int i = 0; i < ULL_MONITOR_SUM; i++) {
		seq_printf(m, "m[%d]:	%d\n", i, latency_ctrl_info->master_id_enable[i]);
		seq_printf(m, "grp[%d]:	%d %d %d %d\n",
			i,
			latency_ctrl_info->group_id_enable[i][ULL_MON_GROUP0],
			latency_ctrl_info->group_id_enable[i][ULL_MON_GROUP1],
			latency_ctrl_info->group_id_enable[i][ULL_MON_GROUP2],
			latency_ctrl_info->group_id_enable[i][ULL_MON_GROUP3]);
	}
	return 0;
}

static int ddr_latency_info_show(struct seq_file *m, void *v)
{
	(void)v;
	if (ull_get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return 0;
	}

	ddr_lat_info* lat_info = &(g_ddr_lat_info);
	for (unsigned int master_id = 0; master_id < ULL_MONITOR_SUM; master_id++) {
		if (lat_info->lat_ctrl_info.master_id_enable[master_id] == 0)
			continue;
		for (unsigned int grp = 0; grp < ULL_GROUP_NUM_MAX; grp++) {
			if (lat_info->lat_ctrl_info.group_id_enable[master_id][grp] == 0)
				continue;
			if (lat_info->lat_master_info[master_id][grp].trans_count == 0)
				continue;
			seq_printf(m, "m:%d   grp:%d   cnt:%d   cur_latency:%d   min:%d   max:%d   time:%d\n",
				master_id,
				grp,
				lat_info->lat_master_info[master_id][grp].trans_count,
				lat_info->lat_master_info[master_id][grp].avg_latency,
				lat_info->lat_master_info[master_id][grp].min_latency,
				lat_info->lat_master_info[master_id][grp].max_latency,
				lat_info->lat_master_info[master_id][grp].timestamp);
		}
	}
	for (unsigned int core_id = 0; core_id < DDR_ULL_CPU_CORE_MAX; core_id++) {
		if (lat_info->lat_ctrl_info.cpu_core_enable == 0)
			return 0;
		for (unsigned int qos_id = 0; qos_id < DDR_ULL_CPU_QOS_MAX; qos_id++) {
			if (qos_id != 0)
				continue;
			if (lat_info->lat_cpu_info[core_id][qos_id].max_latency == 0)
				continue;
			if (lat_info->lat_cpu_info[core_id][qos_id].min_latency > lat_info->lat_cpu_info[core_id][qos_id].max_latency)
				continue;
			seq_printf(m, "core:%d   qos_grp:%d   cnt:%d   avg_cur:%d   min:%d   max:%d   qos:%d   time:%d\n",
				core_id,
				qos_id,
				lat_info->lat_cpu_info[core_id][qos_id].qos_cnt,
				lat_info->lat_cpu_info[core_id][qos_id].avg_latency,
				lat_info->lat_cpu_info[core_id][qos_id].min_latency,
				lat_info->lat_cpu_info[core_id][qos_id].max_latency,
				lat_info->lat_cpu_info[core_id][qos_id].qos,
				lat_info->lat_cpu_info[core_id][qos_id].timestamp);
		}
	}
	return 0;
}

static int ddr_latency_ctrl_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_latency_ctrl_show, NULL);
}

static int ddr_latency_info_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, ddr_latency_info_show, NULL);
}

int ddr_latency_set_enable(unsigned int lat_enable)
{
	if (lat_enable != 0 && lat_enable != 1)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_LAT_ENABLE, lat_enable);
}

int ddr_latency_set_cpu_enable(unsigned int lat_enable)
{
	if (lat_enable != 0 && lat_enable != 1)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_LAT_CPU_ENABLE, lat_enable);
}

static ssize_t ddr_latency_enable_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lat_enable = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}
	
	if (count > sizeof(buff)) {
		pr_err("%s input fail!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%u", &lat_enable) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_latency_set_enable(lat_enable);
	
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

static ssize_t ddr_latency_cpu_enable_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int cpu_enable = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (count > sizeof(buff)) {
		pr_err("%s input fail!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%u", &cpu_enable) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_latency_set_cpu_enable(cpu_enable);
	
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

static ssize_t ddr_latency_interval_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lat_interval = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (count > sizeof(buff)) {
		pr_err("%s input fail!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%u", &lat_interval) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	if ((lat_interval < DDR_STAT_LAT_INTERVAL_MIN ||
		lat_interval > DDR_STAT_LAT_INTERVAL_MAX) ||
		((lat_interval % DDR_STAT_LAT_INTERVAL_MIN) != 0)) {
		pr_err("%s data is invalid\n", __func__);
		return -EINVAL;
	}

	set_info_to_shmem(DDR_MNTN_LAT_INTERVAL, lat_interval);
	return count;
}

int ddr_latency_trigger_once(unsigned int lat_trigger_once)
{
	if (lat_trigger_once != 0 && lat_trigger_once != 1)
		return -EINVAL;

	return set_info_to_shmem(DDR_MNTN_LAT_TRIGGER_ONCE, lat_trigger_once);
}

static ssize_t ddr_latency_trigger_once_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int lat_trigger_once = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (count > sizeof(buff)) {
		pr_err("%s input fail!\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%u", &lat_trigger_once) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_latency_trigger_once(lat_trigger_once);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

int ddr_latency_set_master_id(unsigned int master_value)
{
	unsigned int master_id = master_value / DDR_STAT_TEN_THOUSAND;
	unsigned int grp_enable = master_value % DDR_STAT_TEN_THOUSAND;
	unsigned int enable = 0;

	if ((master_id >= DDR_STAT_LAT_MASTER_MAX))
		return -EINVAL;

	for (unsigned int i = 0; i < ULL_GROUP_NUM_MAX; i++) {
		enable = (grp_enable >> i) & DATA_BIT0_MASK;
		if (enable != 0 && enable != 1)
			return -EINVAL;
	}
	set_info_to_shmem(DDR_MNTN_LAT_MASTER_ID, master_value);
	return 0;
}

static ssize_t ddr_latency_master_id_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[DDR_STAT_WRITE_DATA_SIZE] = {0};
	unsigned int master_value = 0;
	int ret;

	if (!user_buf) {
		pr_err("%s user_buf is NULL!\n", __func__);
		return -EINVAL;
	}

	if (count > sizeof(buff)) {
		pr_err("%s input fail!\n", __func__);
		return -EINVAL;
	}
	
	if (copy_from_user(buff, user_buf,count)) {
		pr_err("%s copy from user fail!\n", __func__);
		return 0;
	}

	if (sscanf_s(buff, "%u", &master_value) != 1) {
		pr_err("%s sscanf_s fail!\n", __func__);
		return 0;
	}

	ret = ddr_latency_set_master_id(master_value);
	if (ret) {
		pr_err("%s data is invalid\n", __func__);
		return ret;
	}

	return count;
}

static const struct proc_ops ddr_latency_enable_fops = {
	.proc_open = ddr_latency_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_latency_enable_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_latency_cpu_enable_fops = {
	.proc_open = ddr_latency_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_latency_cpu_enable_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};
static const struct proc_ops ddr_latency_interval_fops = {
	.proc_open = ddr_latency_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_latency_interval_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_latency_trigger_once_fops = {
	.proc_open = ddr_latency_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_latency_trigger_once_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_latency_master_fops = {
	.proc_open = ddr_latency_ctrl_open,
	.proc_read = seq_read,
	.proc_write = ddr_latency_master_id_write,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static const struct proc_ops ddr_latency_info_fops = {
	.proc_open = ddr_latency_info_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release_private,
};

static struct proc_dir_entry *proc_latency_entry = NULL;
#endif

static struct proc_dir_entry *proc_ddr_debug_entry = NULL;
static struct proc_dir_entry *proc_bandwidth_entry = NULL;
static struct proc_dir_entry *proc_lp_entry = NULL;
static struct proc_dir_entry *proc_lp_stat_entry = NULL;
static int __init ddr_stat_init(void)
{
	proc_ddr_debug_entry = proc_mkdir("ddr_debug", NULL);
	if (!proc_ddr_debug_entry) {
		pr_err("Unable to create ddr_debug directory\n");
		return -EINVAL;
	}

	proc_bandwidth_entry = proc_mkdir("bandwidth", proc_ddr_debug_entry);
	if (!proc_bandwidth_entry) {
		pr_err("Unable to create bandwidth directory\n");
		return -EINVAL;
	}
	proc_create("enable", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_enable_fops);
	proc_create("interval", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_interval_fops);
	proc_create("duration", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_duration_fops);
	proc_create("threshold", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_threshold_fops);
	proc_create("trigger_once", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_trigger_once_fops);
	proc_create("master", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_master_fops);
	proc_create("info", S_IRUGO, (struct proc_dir_entry *)proc_bandwidth_entry, &ddr_bandwidth_info_fops);

	proc_lp_entry = proc_mkdir("lp", proc_ddr_debug_entry);
	if (!proc_lp_entry) {
		pr_err("Unable to create lp directory\n");
		return -EINVAL;
	}
	proc_create("enable", S_IRUGO, (struct proc_dir_entry *)proc_lp_entry, &ddr_lp_enable_fops);
	proc_create("interval", S_IRUGO, (struct proc_dir_entry *)proc_lp_entry, &ddr_lp_interval_fops);
	proc_create("vote", S_IRUGO, (struct proc_dir_entry *)proc_lp_entry, &ddr_lp_vote_fops);
	proc_create("lp_statistic", S_IRUGO, (struct proc_dir_entry *)proc_lp_entry, &ddr_lp_statistic_fops);

	proc_lp_stat_entry = proc_mkdir("stat", proc_lp_entry);
	if (!proc_lp_stat_entry) {
		pr_err("Unable to create lp_stat directory\n");
		return -EINVAL;
	}
	proc_create("enable", S_IRUGO, (struct proc_dir_entry *)proc_lp_stat_entry, &ddr_lp_stat_enable_fops);
	proc_create("ddr_stat", S_IRUGO, (struct proc_dir_entry *)proc_lp_stat_entry, &ddr_stat_proc_fops);
	proc_create("freq_track", S_IRUGO, (struct proc_dir_entry *)proc_lp_stat_entry, &ddr_lp_freq_track_fops);
	proc_create("load_track", S_IRUGO, (struct proc_dir_entry *)proc_lp_stat_entry, &ddr_lp_load_track_fops);
	
#if defined(CONFIG_DDR_ORION)
	proc_latency_entry = proc_mkdir("latency", proc_ddr_debug_entry);
	if (!proc_latency_entry) {
		pr_err("Unable to create latency directory\n");
		return -EINVAL;
	}
	proc_create("enable", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_enable_fops);
	proc_create("cpu_enable", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_cpu_enable_fops);
	proc_create("interval", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_interval_fops);
	proc_create("trigger_once", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_trigger_once_fops);
	proc_create("master", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_master_fops);
	proc_create("info", S_IRUGO, (struct proc_dir_entry *)proc_latency_entry, &ddr_latency_info_fops);
#endif
	return 0;
}

static void __exit ddr_stat_exit(void)
{
	remove_proc_entry("ddr_stat", NULL);
}

void ddr_stat_bandwidth_mntn(void)
{
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return;
	}
	unsigned int offset = 0;
	unsigned char buf[LOG_BUFFER_SIZE] = {'\0'};
	ddr_bandwidth_master_info* bandwidth_master_info = &(g_ddr_release_message.bandwidth_info.bandwidth_master_info[0]);
	for (unsigned int master_id = 0; master_id < DDR_BAND_MNTN_MID_MAX; master_id++) {
		for (unsigned int i = 0; i < BANKWIDTH_MNTN_TIMES; i++) {
			offset += sprintf_s(buf + offset, LOG_BUFFER_SIZE, "%u %u ", i, bandwidth_master_info[master_id].flux[i]);
			if (offset < 0) {
				pr_err("ddr stat mntn buf over\n");
				return;
			}
		}
		(void)sprintf_s(buf + offset, LOG_BUFFER_SIZE, "m:%u f:%u time:%u",
			master_id,
			bandwidth_master_info[master_id].freq_id[0],
			bandwidth_master_info[master_id].timestamp[0]);
		pr_info("%s\n", buf);
		offset = 0;
		(void)memset_s(buf, sizeof(buf), '\0', LOG_BUFFER_SIZE);
	}
}
EXPORT_SYMBOL(ddr_stat_bandwidth_mntn);

void ddr_stat_bandwidth_record_for_modem_test(void)
{
	unsigned int tmpbuffer[LOG_BUFFER_TEST_SIZE] = {0};
	int ret;
	ret = ddr_stat_bandwidth_record_for_modem(tmpbuffer, sizeof(unsigned int) * LOG_BUFFER_TEST_SIZE);
	if (ret < 0) {
		pr_err("%s failed\n", __func__);
		return;
	}

	for (unsigned int i = 0; i < LOG_BUFFER_TEST_SIZE; i++)
		pr_err("i:%u [%u]\n", i, tmpbuffer[i]);
}

int ddr_stat_bandwidth_record_for_modem(unsigned int* value, unsigned int length)
{
	if (get_info_from_shmem() != 0) {
		pr_err("%s get info failed\n", __func__);
		return -EINVAL;
	}
	if (length < sizeof(unsigned int) * DDR_BAND_MNTN_MID_MAX) {
		pr_err("%s modem buffer too small\n", __func__);
		return -EINVAL;
	}
	unsigned int offset = 0;
	unsigned char buf[LOG_BUFFER_SIZE] = {'\0'};
	ddr_bandwidth_master_info* bandwidth_master_info = &(g_ddr_release_message.bandwidth_info.bandwidth_master_info[0]);
	unsigned int timetamp = 0;
	unsigned int last_index = 0;
	for (unsigned int i = 0; i < BANKWIDTH_MNTN_TIMES; i++) {
		if (bandwidth_master_info[0].timestamp[i] > timetamp) {
			timetamp = bandwidth_master_info[0].timestamp[i];
			last_index = i;
		}
	}

	*(unsigned int*)value++ = bandwidth_master_info[0].timestamp[last_index];
	for (unsigned int master_id = 0; master_id < DDR_BAND_MNTN_MID_MAX; master_id++) {
		*(unsigned int*)value++ = bandwidth_master_info[master_id].flux[last_index];
		offset += sprintf_s(buf + offset, LOG_BUFFER_SIZE, "%u ", bandwidth_master_info[master_id].flux[last_index]);
		if (offset < 0) {
			pr_err("ddr stat record mntn buf over\n");
			return -EINVAL;
		}
	}
	(void)sprintf_s(buf + offset, LOG_BUFFER_SIZE, "size:%u", sizeof(unsigned int) * DDR_BAND_MNTN_MID_MAX);
	pr_info("%s\n", buf);
	return (sizeof(unsigned int) * (DDR_BAND_MNTN_MID_MAX + 1));
}
EXPORT_SYMBOL(ddr_stat_bandwidth_record_for_modem);

module_init(ddr_stat_init);
module_exit(ddr_stat_exit);
