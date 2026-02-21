/*
 * improve_yield_module.c
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <platform_include/basicplatform/linux/improve_yield.h>
#include "improve_yield_internal.h"
#include <securec.h>

#define IMPROVE_YIELD_DIR "improve_yield"
#define IMPROVE_YIELD_ENABLE_FILE "enable"
#define IMPROVE_YIELD_INFO_FILE "info"

#define ENBALE_MAX_LEN 2 /* endable value is 0 or 1,an extra char for '\0' */

static char g_improve_yield_info[INFO_MAX_LEN] = "";

static ssize_t enable_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	char buf[ENBALE_MAX_LEN + 1] = "";
	int  len;

	(void)(file);

	len = snprintf_s(buf, sizeof(buf), sizeof(buf), "%d\n", is_rtb_intercept_phase());

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t enable_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	char                          buf[ENBALE_MAX_LEN + 1] = "";
	char                         *newline;
	int                           ret;
	unsigned char                 input_value = 0;
	improve_yield_enabale_nv_info enabale_info;

	(void)(file);
	(void)(ppos);

	if (count >= sizeof(buf) || user_buf == NULL)
		return -EINVAL;

	ret = (int)copy_from_user(buf, user_buf, count);
	if (ret != 0)
		return -EFAULT;

	buf[count] = '\0';

	newline = strchr(buf, '\n');
	if (newline)
		*newline = '\0';

	ret = kstrtou8(buf, 10, &input_value);
	if (ret != 0 || input_value > 1) {
		pr_err("[%s %d]: input invailed, ret:%d\n", __func__, __LINE__, ret);
		goto end;
	}

	ret = get_improve_yield_nv_info(MODULE_ID_ENABLE, (char *)&enabale_info.value, sizeof(enabale_info.value));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield enable status fail, ret:%d\n", __func__, __LINE__, ret);
		goto end;
	}

	enabale_info.bits.enable_flag = input_value;

	ret = set_improve_yield_nv_info(MODULE_ID_ENABLE, (char *)&enabale_info.value, sizeof(enabale_info.value));
	if (ret != 0) {
		pr_err("[%s %d]: set improve yield enable status fail, ret:%d\n", __func__, __LINE__, ret);
		goto end;
	}

end:
	return count;
}

static ssize_t info_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	char         buf[INFO_MAX_LEN] = "";
	unsigned int len               = 0;
	int          ret;

	(void)(file);

	ret = get_improve_yield_info(buf, &len);
	if (ret != 0 || len == 0 || len >= INFO_MAX_LEN)
		pr_err("%s:get improve yield info fail, ret:%d, len:%u\n", __func__, ret, len);

	if ((strlen(g_improve_yield_info) > 0) && (len + strlen(g_improve_yield_info) < INFO_MAX_LEN - 1)) {
		ret = strncat_s(buf + len, INFO_MAX_LEN - len, g_improve_yield_info, strlen(g_improve_yield_info));
		if (ret != 0)
			pr_err("%s:get g_improve_yield_info fail, strncat_s fail, ret:%d\n", __func__, ret);
		else
			len += strlen(g_improve_yield_info);
	}

	buf[len] = '\0';

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}


static const struct file_operations enable_fops = {
	.owner = THIS_MODULE,
	.read  = enable_read,
	.write = enable_write,
};

static const struct file_operations info_fops = {
	.owner = THIS_MODULE,
	.read  = info_read,
};

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
static struct dentry *g_improve_yield_dir;

static int __init improve_yield_init(void)
{
	struct dentry *enable_entry, *info_entry;

	g_improve_yield_dir = debugfs_create_dir(IMPROVE_YIELD_DIR, NULL);
	if (!g_improve_yield_dir)
		return -ENOMEM;

	enable_entry = debugfs_create_file(IMPROVE_YIELD_ENABLE_FILE, 0644, g_improve_yield_dir, NULL, &enable_fops);
	if (!enable_entry)
		goto err_enable;

	info_entry = debugfs_create_file(IMPROVE_YIELD_INFO_FILE, 0644, g_improve_yield_dir, NULL, &info_fops);
	if (!info_entry)
		goto err_info;

	mutex_init(&g_improve_yield_nv_access_mutex);
	mutex_init(&g_improve_yield_info_mutex);
	return 0;

err_info:
	debugfs_remove(enable_entry);
err_enable:
	debugfs_remove(g_improve_yield_dir);
	return -ENOMEM;
}

static void __exit improve_yield_exit(void)
{
	free_improve_yield_info_list();

	debugfs_remove_recursive(g_improve_yield_dir);
}

module_init(improve_yield_init);
module_exit(improve_yield_exit);

MODULE_DESCRIPTION("Hisilicon Improve Yield Module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HISI_DRV");
#endif
