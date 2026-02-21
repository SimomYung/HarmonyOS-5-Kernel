/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * pmic_oc_level.c
 *
 * Device driver for regulators in PMIC IC
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
#include <linux/err.h>
#include <platform_include/basicplatform/linux/pr_log.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/regmap.h>
#include <securec.h>
#include "pmic_oc_level.h"

#define PR_LOG_TAG PMIC_TAG

static int delay_time = 300*1000;
#ifdef CONFIG_DFX_DEBUG_FS
void pmic_ocl_work_time(int val) {
	delay_time = val;
}
#endif

static int get_oc_level_tree_data(struct platform_device *pdev, struct pmic_oc_level *pmic)
{
	int ret, n, count;
	int nums;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	/* Get the oc_level configuration register value */
	count = of_property_count_elems_of_size(np, "pmic-oc-level-enable-regs", sizeof(int));
	if (count <= 0 || count % OC_ARR_COUNT) {
		pr_err("%s no pmic-oc-level-enable-regs property set\n", __func__);
		return -ENODEV;
	}

	nums = count / OC_ARR_COUNT;

	pmic->oc_arr = devm_kzalloc(dev, sizeof(struct oc_level_arrinf) * nums, GFP_KERNEL);
	if(!pmic->oc_arr)
		return -ENOMEM;

	for (n = 0; n < nums; n++) {
		ret = of_property_read_u32_index(np, "pmic-oc-level-enable-regs",
						 n * 3 + 0,
						 &pmic->oc_arr[n].oc_reg);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "pmic-oc-level-enable-regs",
						 n * 3 + 1,
						 &pmic->oc_arr[n].oc_mask);
		if (ret)
			return ret;

		ret = of_property_read_u32_index(np, "pmic-oc-level-enable-regs",
						 n * 3 + 2,
						 &pmic->oc_arr[n].oc_enb);
		if (ret)
			return ret;
	}

	pmic->oc_level_cnt = nums;

    /* Get the number of oc_level record registers */
	count = of_property_count_elems_of_size(np, "pmic-oc-level-record-regs", sizeof(int));
	if (count <= 0) {
		pr_err("%s no pmic-oc-record-num property set\n", __func__);
		return -ENODEV;
	}

	pmic->oc_level_record_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * count, GFP_KERNEL);
	if (pmic->oc_level_record_regs == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-oc-level-record-regs", pmic->oc_level_record_regs, count);
	if(ret < 0)
	{
		pr_err("%s status property read failed!\n", __func__);
		return -1;
	}

	pmic->oc_level_record_num = count;

	return ret;
}

static void pmic_oc_level_onoff(struct pmic_oc_level *pmic) {
	int i, val;

	for (i = 0; i < pmic->oc_level_cnt; i++) {
		regmap_read(pmic->regmap, pmic->oc_arr[i].oc_reg, &val);
		regmap_write(pmic->regmap, pmic->oc_arr[i].oc_reg, val & (~(pmic->oc_arr[i].oc_mask) | OC_LEVEL_DISABLE));
		
		regmap_read(pmic->regmap, pmic->oc_arr[i].oc_reg, &val);
		regmap_write(pmic->regmap, pmic->oc_arr[i].oc_reg, 
							(val & (~ (pmic->oc_arr[i].oc_mask))) | (OC_LEVEL_ENABLE & pmic->oc_arr[i].oc_mask));
	}
}

static void oc_level_detection_work(struct work_struct *work) {
	unsigned int val = 0;
	int i;
	struct pmic_oc_level * pmic = container_of(work, struct pmic_oc_level,
			oc_level_get_work.work);
   
	/* 2.Read two oc level event registers */
	for (i = 0; i < pmic->oc_level_record_num; i++) {
		regmap_read(pmic->regmap, pmic->oc_level_record_regs[i], &val);
	/* 3.Print two oc level event registers */
		if(val)
		pr_err("[%s] SUBPMU OC_Level event happened\n", __func__);
		dev_err(pmic->dev, "[%s] SUBPMU oc event record reg 0x%x, value 0x%x\n", __func__,
			pmic->oc_level_record_regs[i], val);

	/* 5.Clear oc level event register */
		regmap_write(pmic->regmap, pmic->oc_level_record_regs[i], val);
	}
	pmic_oc_level_onoff(pmic);

	schedule_delayed_work(&pmic->oc_level_get_work, msecs_to_jiffies(delay_time));
}

static int pmic_oc_level_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pmic_oc_level *pmic = NULL;

	dev_err(dev, "[%s] +\n", __func__);
	pmic = (struct pmic_oc_level *)devm_kzalloc(
		dev, sizeof(*pmic), GFP_KERNEL);
	if (pmic == NULL)
		return -ENOMEM;

	pmic->dev = dev;

	pmic->regmap = dev_get_regmap(dev->parent, NULL);
	if (!pmic->regmap)
		return -ENODEV;

	get_oc_level_tree_data(pdev, pmic);
	INIT_DELAYED_WORK(&pmic->oc_level_get_work, oc_level_detection_work);
	schedule_delayed_work(&pmic->oc_level_get_work, msecs_to_jiffies(delay_time));

	pr_err("[%s] oc level dev, succ\n", __func__);

	return 0;
}

const static struct of_device_id pmic_oc_level_match_tbl[] = {
	{
		.compatible = "oc-level",
	},
	{ }    /* end */
};

static struct platform_driver pmic_oc_level_driver = {
	.driver = {
			.name = "oc-level",
			.owner = THIS_MODULE,
			.of_match_table = pmic_oc_level_match_tbl,
		},
	.probe = pmic_oc_level_probe,
};

static int __init pmic_oc_level_init(void)
{
	return platform_driver_register(&pmic_oc_level_driver);
}

static void __exit pmic_oc_level_exit(void)
{
	platform_driver_unregister(&pmic_oc_level_driver);
}

module_init(pmic_oc_level_init);
module_exit(pmic_oc_level_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PMU OC LEVEL Driver");
