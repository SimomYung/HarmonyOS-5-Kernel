/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * pmic_die_crack_check.c
 *
 * Device driver for regulators in PMIC DIE CRACK CHECK
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

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/workqueue.h>
#include <platform_include/basicplatform/linux/pr_log.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <securec.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif
#include "pmic_die_crack_check.h"

#define PR_LOG_TAG PMIC_TAG

#define DELAY_FIVE_MINUTES (5*60*1000)
#define DELAY_SEVEN_DAYS (7*1440*60*1000)

static int ioctl_init_flag = 0;
#define DIE_CRACK_MAX_DEVICES 10
static int pmic_cnt = 0;
static struct pmic_die_crack_check *pmics[DIE_CRACK_MAX_DEVICES] = {0};

#define HISI_AT_DIE_CRACK_BUF 500
#define DIE_CRACK_BUF 40

#define DIE_CRACK_CHECK_DEV "die_crack_check_dev"

static DEFINE_MUTEX(pmic_die_crack_lock);

#define CMD_IOC_MAGIC 'd'
#define CMD_IOC_R	_IOR(CMD_IOC_MAGIC, 0, unsigned int)

static int dts_parse(struct platform_device *pdev, struct pmic_die_crack_check *pmic)
{
	int ret, n, count, nums;
	struct device *dev = &pdev->dev;
	struct device_node *fnp = NULL;
	struct device_node *np = dev->of_node;

	fnp = of_get_parent(np);
	if (!fnp) {
		pr_err("%s No fnp device node\n", __func__);
		return -ENODEV;
	}

	ret = of_property_read_u32(fnp, "slave_id", &pmic->slave_id);
	if (ret)
		pr_err("%s slave_id not set\n", __func__);
	pr_err("%s slave_id is %d\n", __func__, pmic->slave_id);

	ret = of_property_read_string(np, "pmic-name", &pmic->pmu_name);
	if (ret) {
		pr_err("%s pmic_name not set\n", __func__);
		return -ENODEV;
	}
	/* Get the number of die_crack_check record register */
	count = of_property_count_elems_of_size(np, "check-record-regs", sizeof(int));
	if (count <= 0 || count % RESULT_CNT) {
		pr_err("%s no pmic-die-crack-check-record-reg property set\n", __func__);
		return -ENODEV;
	}
	nums = count / RESULT_CNT;

	pmic->result_regs = devm_kzalloc(dev, sizeof(struct result_regs_info) * nums, GFP_KERNEL);
	if (!pmic->result_regs)
		return -ENOMEM;

	for (n = 0; n < nums; n++) {
		ret = of_property_read_u32_index(np, "check-record-regs",
						 n * 3 + 0,
						 &pmic->result_regs[n].result_reg);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "check-record-regs",
						 n * 3 + 1,
						 &pmic->result_regs[n].expect_val);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "check-record-regs",
						 n * 3 + 2,
						 &pmic->result_regs[n].result_mask);
		if (ret)
			return ret;
	}
	pmic->result_regs_num = nums;

	/* Get the number of die_crack_check reg register */
	count = of_property_count_elems_of_size(np, "enable-regs", sizeof(int));
	if (count <= 0 || count % ENB_CNT) {
		pr_err("%s no enable_regs property set\n", __func__);
		return -ENODEV;
	}
	nums = count / ENB_CNT;

	pmic->enable_regs= devm_kzalloc(dev, sizeof(struct enable_regs_info) * nums, GFP_KERNEL);
	if (!pmic->enable_regs)
		return -ENOMEM;

	for (n = 0; n < nums; n++) {
		ret = of_property_read_u32_index(np, "enable-regs",
						 n * 4 + 0,
						 &pmic->enable_regs[n].enable_reg);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "enable-regs",
						 n * 4 + 1,
						 &pmic->enable_regs[n].enable_mask);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "enable-regs",
						 n * 4 + 2,
						 &pmic->enable_regs[n].enable_val);
		if (ret)
			return ret;
		ret = of_property_read_u32_index(np, "enable-regs",
						 n * 4 + 3,
						 &pmic->enable_regs[n].disable_val);
		if (ret)
			return ret;
	}
	pmic->enable_regs_num = nums;

	return ret;
}

static void enable_check(struct pmic_die_crack_check *pmic_check)
{
	/* Print die_crack val event registers */
	int ret, i;
	for (i = 0; i < pmic_check->enable_regs_num; i++) {
		regmap_read(pmic_check->regmap, pmic_check->enable_regs[i].enable_reg, &pmic_check->enable_regs[i].default_val);
		pr_err("[%s]die_crack event before enable reg 0x%x, default value 0x%x, slave_id is %d\n", __func__,
			pmic_check->enable_regs[i].enable_reg,
			pmic_check->enable_regs[i].default_val,
			pmic_check->slave_id);
		ret = regmap_update_bits(pmic_check->regmap, pmic_check->enable_regs[i].enable_reg,
			pmic_check->enable_regs[i].enable_mask, pmic_check->enable_regs[i].enable_val);
		if (ret < 0)
			pr_err("[%s]:write_enable_val_%d is error!\n", __func__, i);
		pr_err("[%s]die_crack event enable reg 0x%x, value 0x%x, slave_id is %d\n", __func__,
			pmic_check->enable_regs[i].enable_reg,
			pmic_check->enable_regs[i].enable_val,
			pmic_check->slave_id);
	}
	mdelay(1);
}

static int get_check_result(struct pmic_die_crack_check *pmic_check)
{
	/* Read die_crack val event registers */
	int i;
	unsigned int val, mask_val;

	for (i = 0; i < pmic_check->result_regs_num; i++) {
		regmap_read(pmic_check->regmap, pmic_check->result_regs[i].result_reg, &val);
		pr_err("[%s]die_crack event result reg 0x%x, read_value 0x%x, slave_id is %d\n", __func__,
			pmic_check->result_regs[i].result_reg, val, pmic_check->slave_id);
		mask_val = val & (pmic_check->result_regs[i].result_mask);
		if (mask_val != pmic_check->result_regs[i].expect_val) {
			pr_err("[%s]: pmic_check->result_regs.expect_val is 0x%x read_val = 0x%x, %d, failed, slav_id is %d\n",
				__func__, pmic_check->result_regs[i].expect_val, val, i, pmic_check->slave_id);
			return 1;
		}
	}
	return 0;
}

static void disable_check(struct pmic_die_crack_check *pmic_check)
{
	/* Print default die_crack val event register */
	int ret, i;
	for (i = 0; i < pmic_check->enable_regs_num; i++) {
		ret = regmap_update_bits(pmic_check->regmap, pmic_check->enable_regs[i].enable_reg,
			pmic_check->enable_regs[i].enable_mask, pmic_check->enable_regs[i].disable_val);
		if (ret < 0)
			pr_err("[%s]:write_default_val_%d is error!\n", __func__, i);
		regmap_read(pmic_check->regmap, pmic_check->enable_regs[i].enable_reg, &pmic_check->enable_regs[i].default_val);
		pr_err("[%s]die_crack event after enable reg 0x%x, default value 0x%x, slave_id is %d\n", __func__,
			pmic_check->enable_regs[i].enable_reg,
			pmic_check->enable_regs[i].default_val,
			pmic_check->slave_id);
	}
}

// 0:The test is PASS, others:The test is FAIL
int die_crack_check(struct pmic_die_crack_check *pmic_info)
{
	int ret;

	enable_check(pmic_info);
	ret = get_check_result(pmic_info);
	disable_check(pmic_info);

	if (ret) {
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
		power_dsm_report_format_dmd(POWER_DSM_PMU_OCP, DSM_PMU_DIE,
			"pmic_die_crack_check error happened, slave_id is %d, pmu_judgment is FAIL please pay attention!\n", pmic_info->slave_id);
#endif
		pr_err("pmic_die_crack_check error happened, slave_id is %d\n", pmic_info->slave_id);
	}
	return ret;
}

static int die_crack_check_func(char *die_crack_buf, unsigned int len)
{
	int i, ret;
    int length = 0;
	int used_len = 0;

	for (i = 0; i < pmic_cnt; i++) {
		ret = die_crack_check(pmics[i]);
		if (len > 0) {
			length = snprintf_s(die_crack_buf + used_len, len, len - 1,
				"\r\n%s, slave_id %d, %s\r\n", pmics[i]->pmu_name,
				pmics[i]->slave_id, ((ret == 0) ? "PASS" : "FAIL"));
			if(length < 0) {
				pr_err("[%s]:snprintf_s is error!\n", __func__);
                break;
            }
			len -= length;
			used_len += length;
		}
		else {
			return -EFAULT;
		}
	}
	return 0;
}

#ifdef CONFIG_DFX_DEBUG_FS 
static long die_crack_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;
	void __user *argp = (void __user *)arg;
	char at_die_crack_buf[HISI_AT_DIE_CRACK_BUF] = {0};
	unsigned int buf_length;

	pr_info("%s: %d: cmd=0x%x\n", __func__, __LINE__, cmd);
	
	if (pmics == NULL)
		return -ENODEV;
	
	mutex_lock(&pmic_die_crack_lock);

	switch (cmd) {
	case CMD_IOC_R: 
		if (!arg) {
			ret = -EFAULT;
			goto out_unlock;
		}
		ret = die_crack_check_func(at_die_crack_buf, HISI_AT_DIE_CRACK_BUF - 1);
		if (ret < 0) {
			pr_err("%s:get_die_crack_check is error!\n", __func__);
			ret = -EFAULT;
			goto out_unlock;
		}
		pr_err("%s", at_die_crack_buf);
		/* send back to user */
		if (copy_to_user(argp, at_die_crack_buf, HISI_AT_DIE_CRACK_BUF)) {
			ret = -EFAULT;
			goto out_unlock;
		}
		break;

	default:
		pr_info("[DIE_CRACK][%s] Unknow command!\n", __func__);
		ret = -ENOTTY;
		break;
	}
	mutex_unlock(&pmic_die_crack_lock);
	return (long)ret;

out_unlock:
    mutex_unlock(&pmic_die_crack_lock);
    return (long)ret;
}

const static struct file_operations pmic_die_crack_dev_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = die_crack_ioctl,
};
#endif

static void die_crack_check_detection_work(struct work_struct *work)
{
	struct pmic_die_crack_check *pmic = container_of(work, struct pmic_die_crack_check,
			die_crack_check_work.work);

	die_crack_check(pmic);
	schedule_delayed_work(&pmic->die_crack_check_work, msecs_to_jiffies(DELAY_SEVEN_DAYS));
	pr_err("%s:delayed_work is done!\n", __func__);
}

#ifdef CONFIG_DFX_DEBUG_FS
unsigned int die_crack_check_test(void)
{
	char at_die_crack_buf[HISI_AT_DIE_CRACK_BUF] = {0};
	int ret;

	ret = die_crack_check_func(at_die_crack_buf, HISI_AT_DIE_CRACK_BUF - 1);
	if (ret < 0) {
			pr_err("%s:die_crack_check_test error!\n", __func__);
			return -EFAULT;
		}
	pr_err("%s", at_die_crack_buf);
	return 0;
}
#endif

#ifdef CONFIG_DFX_DEBUG_FS
static int die_crack_major = 0;
static struct class *pmic_die_crack_check_class;
static int ioctl_init(struct pmic_die_crack_check *pmic)
{
	struct device *pdevice = NULL;
	int minor = 0;

	mutex_lock(&pmic_die_crack_lock);

	die_crack_major = register_chrdev(0, DIE_CRACK_CHECK_DEV, &pmic_die_crack_dev_fops);
	if (die_crack_major < 0) {
		pr_err("[%s] unable to get die_crack_check_die_crack_major\n",__func__);
		mutex_unlock(&pmic_die_crack_lock);
		return -EFAULT;
	}
	pmic_die_crack_check_class = class_create(THIS_MODULE, DIE_CRACK_CHECK_DEV);
	if (IS_ERR(pmic_die_crack_check_class)) {
		pr_err("pmic_die_crack_check: class_create error\n");
		goto error1;
	}

	pdevice = device_create(pmic_die_crack_check_class, NULL, MKDEV(die_crack_major, minor), NULL, DIE_CRACK_CHECK_DEV);
	if (IS_ERR(pdevice)) {
		pr_err("pmic_die_crack_check: device_create error\n");
		goto error2;
	}

	pr_err("[%s] die_crack check create dev, succ\n", __func__);
	mutex_unlock(&pmic_die_crack_lock);
	return 0;

error2:
	class_destroy(pmic_die_crack_check_class);
	pmic_die_crack_check_class = NULL;
	pmic = NULL;
	goto error1;
error1:
	unregister_chrdev(die_crack_major, DIE_CRACK_CHECK_DEV);
	die_crack_major = 0;
	mutex_unlock(&pmic_die_crack_lock);
	return -ENODEV;
}
#endif

static int pmic_die_crack_check_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pmic_die_crack_check *pmic = NULL;
	int ret;

	dev_err(dev, "[%s] +\n", __func__);
	pmic = (struct pmic_die_crack_check *)devm_kzalloc(
		&pdev->dev, sizeof(*pmic), GFP_KERNEL);
	if (pmic == NULL) {
		pr_err("%s failed to alloc pmic struct\n", __func__);
		return -ENOMEM;
	}

	pmic->dev = dev;

	pmic->regmap = dev_get_regmap(dev->parent, NULL);
	if (!pmic->regmap)
		return -ENODEV;

	dts_parse(pdev, pmic);
	INIT_DELAYED_WORK(&pmic->die_crack_check_work, die_crack_check_detection_work);
	/* power_on first die_crack_check worker delay (5*60*1000) */
	schedule_delayed_work(&pmic->die_crack_check_work, msecs_to_jiffies(DELAY_FIVE_MINUTES));
	
#ifdef CONFIG_DFX_DEBUG_FS 
	if (ioctl_init_flag == 0) {
		ret = ioctl_init(pmic);
		if (ret < 0) {
			pr_err("pmic_die_crack: ioctl_init error\n");
		return ret;
		}
		ioctl_init_flag = 1; 
	}
	pmics[pmic_cnt++] = pmic;
#endif

	pr_err("[%s] pmic_die_crack dev, succ\n", __func__);
	return 0;
}

const static struct of_device_id pmic_die_crack_check_match_tbl[] = {
	{
		.compatible = "die-crack-check",
	},
	{ },    /* end */
};

static struct platform_driver pmic_die_crack_check_driver = {
	.driver = {
			.name = "die-crack-check",
			.owner = THIS_MODULE,
			.of_match_table = pmic_die_crack_check_match_tbl,
		},
	.probe = pmic_die_crack_check_probe,
};

static int __init pmic_die_crack_check_init(void)
{
	return platform_driver_register(&pmic_die_crack_check_driver);
}

static void __exit pmic_die_crack_check_exit(void)
{
	platform_driver_unregister(&pmic_die_crack_check_driver);
}

module_init(pmic_die_crack_check_init);
module_exit(pmic_die_crack_check_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PMU DIE CRACK CHECK Driver");