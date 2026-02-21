/*
 * hufs_hiberb8.c
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "hufs_hibern8.h"

#include <linux/version.h>
#include <linux/proc_fs.h>

#include <scsi/scsi_host.h>
#include "ufs.h"
#include "ufshcd.h"
#include "ufshcd_debug.h"
#include "ufshcd_extend.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
#include "ufs-quirks.h"
#else
#include "ufs_quirks.h"
#endif

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
#include <linux/blkdev.h>
#endif

static int AH8_limited_time_level = 0;
static DEFINE_MUTEX(AH8_limited_time_lock);
static DEFINE_MUTEX(hibern8_hba_lock);
static struct ufs_hba *g_hibern8_hba = NULL;
static int user_hibern8_cmd = 0;

static u32 ufshcd_autoh8_query_ahit(struct ufs_hba *hba)
{
	u32 ahit = 0;

	if (!ufshcd_is_auto_hibern8_supported(hba))
		return ahit;
    pm_runtime_get_sync(hba->dev);
	ufshcd_hold(hba, false);
	ahit = ufshcd_readl(hba, REG_AUTO_HIBERNATE_IDLE_TIMER);
	ufshcd_release(hba);
	pm_runtime_put_sync(hba->dev);
	return ahit;
}

static int hufs_autoh8_limited_time_close(void *hba_param)
{
	int ret = 0;
	u32 ahit = 0;
	u32 set_value = UFS_AHIT_AUTOH8_TIMER;

	struct ufs_hba *hba = NULL;
	if (!hba_param) {
		return -ENOMEM;
	}
	hba = hba_param;
	mutex_lock(&AH8_limited_time_lock);
	if (hba->autoh8_disable_depth > 0) {
		dev_err(hba->dev, "%s: AH8 is disabled autoh8_disable_depth is %d\n",
			__func__,  hba->autoh8_disable_depth);
		ret = -ENOMEM;
		goto out;
	}

	if (AH8_limited_time_level > 0) {
		AH8_limited_time_level--;
	} else {
		dev_err(hba->dev, "%s: AH8_time_level = %d, set Timer fail\n",
			__func__, AH8_limited_time_level);
		ret = -ENOMEM;
		HUFS_BUG(); /*lint !e146*/
		goto out;
	}

#ifdef CONFIG_UFS_H8_IDLE_3MS
	if (strstarts(hba->model, UFS_MODEL_THOR935)) {
		set_value = UFS_AHIT_AUTOH8_OPT_TIMER;
	}
#endif

	ufshcd_auto_hibern8_update(hba, set_value);
	ahit = ufshcd_autoh8_query_ahit(hba);
	dev_err(hba->dev, "%s:after setting value for closing h8 limited is %x\n", __func__, ahit);
	if (ahit != set_value) {
		ret = -ENOMEM;
		dev_err(hba->dev, "%s:limited time close failed, setting value is %x, cur is %x\n",
			__func__, set_value, ahit);
	}
out:
	mutex_unlock(&AH8_limited_time_lock);
	return ret;
}

static int hufs_autoh8_limited_time_open_30ms(void *hba_param)
{
	int ret = 0;
	u32 ahit;
	struct ufs_hba *hba = NULL;

	if (!hba_param) {
		return -ENOMEM;
	}
	hba = hba_param;
	mutex_lock(&AH8_limited_time_lock);
	if (hba->autoh8_disable_depth > 0) {
		dev_err(hba->dev, "%s: AH8 is disabled, autoh8_disable_depth is %d\n",
			__func__,  hba->autoh8_disable_depth);
		ret = -ENOMEM;
		goto out;
	}

	if (AH8_limited_time_level == 0) {
		AH8_limited_time_level++;
	} else {
		dev_err(hba->dev, "%s: AH8_time_level = %d, set Timer fail\n", __func__, 
			AH8_limited_time_level);
		ret = -ENOMEM;
		goto out;
	}

	ufshcd_auto_hibern8_update(hba, UFS_AHIT_AUTOH8_TIMER_30ms);
	ahit = ufshcd_autoh8_query_ahit(hba);
	dev_err(hba->dev, "%s:after setting value for limited h8 30ms  is %x\n", __func__, ahit);
    if (ahit != UFS_AHIT_AUTOH8_TIMER_30ms) {
		ret = -ENOMEM;
		dev_err(hba->dev, "%s: limited time 30ms failed, setting value is %x, cur value is %x\n",
			__func__, UFS_AHIT_AUTOH8_TIMER_30ms, ahit);
	}
out:
	mutex_unlock(&AH8_limited_time_lock);
	return ret;
}

static int hufs_hibern8_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "user_hibern8_cmd:%d\n", user_hibern8_cmd);
	return 0;
}

static int hufs_hibern8_proc_open(
	struct inode *p_inode, struct file *p_file)
{
	return single_open((struct file *)p_file,
		hufs_hibern8_proc_show, NULL);
}

static ssize_t hufs_hibern8_proc_write(struct file *p_file,
	const char __user *userbuf, size_t count, loff_t *ppos)
{
	char buf;
	int ret = 0;

    if (!g_hibern8_hba) {
		pr_err("%s %d: g_hibern8_ops is null.\n", __func__, __LINE__);
		return -EINVAL;
	}
	if (count == 0 || !userbuf) {
		pr_err("%s %d: input error.\n", __func__, __LINE__);
		return -EINVAL;
	}
	if (copy_from_user(&buf, userbuf, sizeof(char))) {
		pr_err("%s %d: input error.\n", __func__, __LINE__);
		return -EFAULT;
	}
	if (buf != '0' && buf != '1') {
		pr_err("%s %d: input error.\n", __func__, __LINE__);
		return -EINVAL;
	}
	mutex_lock(&hibern8_hba_lock);
	user_hibern8_cmd = (buf == '0') ? 0 : 1;
	if (user_hibern8_cmd == LIMITED_TIME_STOP) {
		ret = hufs_autoh8_limited_time_close(g_hibern8_hba);
		if (ret != 0) {
			pr_err("%s: g_hibern8_hba limit_stop failed, ret = %d!\n", __func__, ret);
			goto out;
		}
	} else if (user_hibern8_cmd == LIMITED_TIME_30MS_START) {
		ret = hufs_autoh8_limited_time_open_30ms(g_hibern8_hba);
		if (ret != 0) {
			pr_err("%s: g_hibern8_hba limit_30ms_start failed, ret = %d!\n", __func__, ret);
			goto out;
		}
	}

out:
	mutex_unlock(&hibern8_hba_lock);
	return (ssize_t)count;
}

static const struct proc_ops hufs_hibern8_fops = {
	.proc_open = hufs_hibern8_proc_open,
	.proc_read = seq_read,
	.proc_write = hufs_hibern8_proc_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int hufs_hibern8_proc_init(void)
{
	struct proc_dir_entry *dir = NULL;

	dir = proc_mkdir("block", NULL);
	if (!dir) {
		if (!proc_create("block/hibern8_limited", 0440 | 0220, NULL, &hufs_hibern8_fops)) {
			pr_err("%s %d: proc mk error.\n", __func__, __LINE__);
			return -EINVAL;
		}
		return 0;
	}
	proc_create("hibern8_limited", 0440 | 0220, dir, &hufs_hibern8_fops);
	return 0;
}

int hufs_hibern8_config(struct scsi_device *sdev)
{
	struct ufs_hba *hba = NULL;

	if (g_hibern8_hba) {
		pr_err("hufs_hibern8_config already init!\n");
		return 0;
	}

	if (!sdev) {
        return -EINVAL;
	}
	hba = shost_priv(sdev->host);
    g_hibern8_hba = hba;
	return hufs_hibern8_proc_init();
}
