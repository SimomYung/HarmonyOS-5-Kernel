/*
 * QIC V300 init functions source file.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "dfx_sec_qic.h"
#include "dfx_sec_qic_err_probe.h"
#include "dfx_sec_qic_dump.h"
#include "dfx_sec_qic_init.h"


#define EM_ER_SIZE 0x50
static int dfx_sec_qic_get_sub_intr_detail_info(struct dfx_sec_qic_device *qic_dev, struct platform_device *pdev,
				   u32 sub_intr_number, void *property_value)
{
	u32 i;
	u32 j;
	u32 addr;
	size_t cnt, size;
	__be32 *val = property_value;

	qic_dev->sub_intr_info = devm_kzalloc(&pdev->dev, sub_intr_number * sizeof(*(qic_dev->sub_intr_info)), GFP_KERNEL);
	if (unlikely(!(qic_dev->sub_intr_info))) {
		dev_err(&pdev->dev, "[%s] alloc sub interrupt info fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < sub_intr_number; i++) {
		(qic_dev->sub_intr_info)[i].qic_sub_intr_index = be32_to_cpup(val++);
		(qic_dev->sub_intr_info)[i].qic_em_num = be32_to_cpup(val++);
		if ((qic_dev->sub_intr_info)[i].qic_em_num == 0)
			continue;

		(qic_dev->sub_intr_info)[i].em_info = devm_kzalloc(&pdev->dev,
				(qic_dev->sub_intr_info)[i].qic_em_num * sizeof(struct qic_em_info), GFP_KERNEL);
		if (unlikely(!((qic_dev->sub_intr_info)[i].em_info))) {
			dev_err(&pdev->dev, "[%s] alloc em_info fail\n", __func__);
			return -ENOMEM;
		}
		for (j = 0; j < (qic_dev->sub_intr_info)[i].qic_em_num; j++) {
			((qic_dev->sub_intr_info)[i].em_info)[j].qic_bus_id = be32_to_cpup(val++);
			((qic_dev->sub_intr_info)[i].em_info)[j].em_base_addr = be32_to_cpup(val++);
			addr = be32_to_cpup(val++);
			size = sizeof(u32);
			if (qic_dev->qic_ath) {
				addr = ((qic_dev->sub_intr_info)[i].em_info)[j].em_base_addr;
				size = EM_ER_SIZE;
			}
			if (addr != REG_ADDR_INVALID) {
				((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_addr = ioremap(addr, size);
				if (!((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_addr) {
					pr_err("[%s] em_intr_addr map failed\n", __func__);
					return -ENOMEM;
				}
			}
			((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_mask = be32_to_cpup(val++);
			((qic_dev->sub_intr_info)[i].em_info)[j].em_name = (char *)val;
			cnt = strlen((const char *)val);
			val = (__be32 *)((char *)val + cnt + 1);
		}
	}
	return 0;
}

static int get_tb_info(struct platform_device *pdev, struct qic_access_info *qic_access_info, __be32 *property_value)
{
	u32 tb_num = qic_access_info->tb_num;
	__be32 *val = property_value;
	u32 i;
	size_t cnt;

	for (i = 0; i < tb_num; i++) {
		val++;
		(qic_access_info->tb_info)[i].tb_name = (char *)val;
		cnt = strlen((const char *)val);
		val = (__be32 *)((char *)val + cnt + 1);
		(qic_access_info->tb_info)[i].mmidx_num = be32_to_cpup(val++);
		(qic_access_info->tb_info)[i].start_addr_head = val;
		val += 2 * (qic_access_info->tb_info)[i].mmidx_num;
	}

	return 0;
}

static int get_ib_info(struct platform_device *pdev, struct qic_access_info *qic_access_info, __be32 *property_value)
{
	u32 ib_num = qic_access_info->ib_num;
	__be32 *val = property_value;
	u32 i;
	size_t cnt;

	for (i = 0; i < ib_num; i++) {
		val++;
		(qic_access_info->ib_name)[i] = (char *)val;
		cnt = strlen((const char *)val);
		val = (__be32 *)((char *)val + cnt + 1);
	}
	return 0;
}

static struct qic_access_info *get_bus_access_info (struct platform_device *pdev, phandle phandle)
{
	struct device_node *np = NULL;
	struct qic_access_info *qic_access_info = NULL;
	const struct property *prop = NULL;
	int ret;

	np = of_find_node_by_phandle(phandle);
	if (!np)
		return NULL;

	qic_access_info = devm_kzalloc(&pdev->dev, sizeof(struct qic_access_info), GFP_KERNEL);
	if (qic_access_info  == NULL) {
		pr_err("[%s] alloc qic_access_info fail\n", __func__);
		return NULL;
	}
	if (of_property_read_u32(np, "tb_num", &qic_access_info->tb_num) != 0) {
		pr_err("[%s] get tb_num from DTS error.\n", __func__);
		return NULL;
	}
	qic_access_info->tb_info =
			devm_kzalloc(&pdev->dev, qic_access_info->tb_num *sizeof(struct qic_tb_info), GFP_KERNEL);
	if (qic_access_info->tb_info  == NULL) {
		pr_err("[%s] alloc tb_info fail\n", __func__);
		return NULL;
	}

	prop = of_find_property(np, "tb_info", NULL);
	if (!prop || !(prop->value)) {
		pr_err("[%s] get tb_info fail\n", __func__);
		return NULL;
	}

	ret = get_tb_info(pdev, qic_access_info, prop->value);
	if (ret) {
		pr_err("[%s] get tb_info from DTS error.\n", __func__);
		return NULL;
	}

	if (of_property_read_u32(np, "ib_num", &qic_access_info->ib_num) != 0) {
		pr_err("[%s] get ib_num from DTS error.\n", __func__);
		return NULL;
	}
	qic_access_info->ib_name =
			devm_kzalloc(&pdev->dev, qic_access_info->ib_num *sizeof(char *), GFP_KERNEL);
	if (qic_access_info->ib_name == NULL) {
		pr_err("[%s] alloc ib_name fail\n", __func__);
		return NULL;
	}
	prop = of_find_property(np, "ib_info", NULL);
	if (!prop || !(prop->value)) {
		pr_err("[%s] get ib_info fail\n", __func__);
		return NULL;
	}

	ret = get_ib_info(pdev, qic_access_info, prop->value);
	if (ret) {
		pr_err("[%s] get ib_info from DTS error.\n", __func__);
		return NULL;
	}
	return qic_access_info;
}

static int dfx_sec_qic_get_mid_detail_info(struct dfx_sec_qic_device *qic_dev, struct platform_device *pdev,
				   u32 mid_num, __be32 *property_value)
{
	u32 i;
	__be32 *val = property_value;
	size_t cnt;

	qic_dev->mid_info = devm_kzalloc(&pdev->dev, mid_num * sizeof(*(qic_dev->mid_info)), GFP_KERNEL);
	if (unlikely(!(qic_dev->mid_info))) {
		dev_err(&pdev->dev, "[%s] alloc mid info fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < mid_num; i++) {
		(qic_dev->mid_info)[i].start = be32_to_cpup(val++);
		(qic_dev->mid_info)[i].end = be32_to_cpup(val++);
		(qic_dev->mid_info)[i].mid_name = (char *)val;
		cnt = strlen((const char *)val);
		val = (__be32 *)((char *)val + cnt + 1);
	}

	return 0;
}

static int dfx_sec_qic_get_bus_detail_info(struct dfx_sec_qic_device *qic_dev, struct platform_device *pdev,
				   u32 bus_num, __be32 *property_value)
{
	u32 i;
	__be32 *val = property_value;
	size_t cnt;

	qic_dev->bus_info = devm_kzalloc(&pdev->dev, bus_num * sizeof(*(qic_dev->bus_info)), GFP_KERNEL);
	if (unlikely(!(qic_dev->bus_info))) {
		dev_err(&pdev->dev, "[%s] alloc bus info fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < bus_num; i++) {
		(qic_dev->bus_info)[i].bus_id = be32_to_cpup(val++);
		(qic_dev->bus_info)[i].bus_name = (char *)val;
		cnt = strlen((const char *)val);
		val = (__be32 *)((char *)val + cnt + 1);
		(qic_dev->bus_info)[i].access_info = get_bus_access_info(pdev, be32_to_cpup(val++));
	}

	return 0;
}

static int dfx_sec_qic_get_list_info(const struct device_node *qic_node, u32 *list_num,
				   void **list_value, const char *name)
{
	const struct device_node *np = NULL;
	const struct property *prop = NULL;

	np = of_find_compatible_node((struct device_node *)qic_node, NULL, name);
	if (unlikely(!np)) {
		pr_err("[%s] cannot get %s node\n", __func__, name);
		return -ENODEV;
	}

	if (of_property_read_u32(np, "target_num", list_num) != 0) {
		pr_err("[%s] get %s target_num from DTS error.\n", __func__, name);
		return -ENODEV;
	}

	prop = of_find_property(np, "target_list", NULL);
	if (!prop || !(prop->value)) {
		pr_err("get target_list fail\n");
		return -ENODEV;
	}

	*list_value = prop->value;

	return 0;
}

static int dfx_sec_qic_get_mid_info(const struct device_node *qic_node, struct platform_device *pdev)
{
	int ret;
	void *mid_property_value;
	struct dfx_sec_qic_device *qic_dev = platform_get_drvdata(pdev);

	if (unlikely(!qic_dev)) {
		dev_err(&pdev->dev, "cannot get qic dev\n");
		return -ENODEV;
	}

	ret = dfx_sec_qic_get_list_info(qic_node, &(qic_dev->mid_info_num),
				&mid_property_value, "dfx,sec_qic_mid_info");
	if (ret) {
		dev_err(&pdev->dev, "cannot get dfx,sec_qic_mid_info property\n");
		return ret;
	}

	return dfx_sec_qic_get_mid_detail_info(qic_dev, pdev, qic_dev->mid_info_num, mid_property_value);
}

static int dfx_sec_qic_get_bus_info(const struct device_node *qic_node, struct platform_device *pdev)
{
	int ret;
	void *bus_property_value;
	struct dfx_sec_qic_device *qic_dev = platform_get_drvdata(pdev);

	if (unlikely(!qic_dev)) {
		dev_err(&pdev->dev, "cannot get qic dev\n");
		return -ENODEV;
	}

	ret = dfx_sec_qic_get_list_info(qic_node, &(qic_dev->bus_info_num),
				&bus_property_value, "dfx,sec_qic_bus_info");
	if (ret) {
		dev_err(&pdev->dev, "cannot get dfx,sec_qic_bus_info property\n");
		return ret;
	}

	return dfx_sec_qic_get_bus_detail_info(qic_dev, pdev, qic_dev->bus_info_num, bus_property_value);
}

static int dfx_sec_qic_get_sub_intr_info(const struct device_node *qic_node, struct platform_device *pdev)
{
	int ret;
	void *sub_intr_property_value;
	struct dfx_sec_qic_device *qic_dev = platform_get_drvdata(pdev);

	if (unlikely(!qic_dev)) {
		dev_err(&pdev->dev, "cannot get qic dev\n");
		return -ENODEV;
	}

	ret = dfx_sec_qic_get_list_info(qic_node, &(qic_dev->sub_intr_number),
				&sub_intr_property_value, "dfx,sec_qic_sub_intr_info");
	if (ret) {
		dev_err(&pdev->dev, "cannot get dfx,sec_qic_sub_intr_info property\n");
		return ret;
	}

	return dfx_sec_qic_get_sub_intr_detail_info(qic_dev, pdev, qic_dev->sub_intr_number, sub_intr_property_value);
}

static int dfx_sec_qic_get_irq_chk(const struct device_node *qic_node, struct platform_device *pdev)
{
	u32 i, reg_num;
	const struct device_node *np = NULL;
	struct dfx_sec_qic_device *qic_dev = platform_get_drvdata(pdev);

	if (unlikely(!qic_dev)) {
		pr_err("[%s] cannot get qic dev\n", __func__);
		return -ENODEV;
	}

	qic_dev->qic_ath = of_property_read_bool(qic_node, "qic_ath");
	if (!qic_dev->qic_ath) {
		pr_err("[%s] nothing to do\n", __func__);
		qic_dev->irq_chk = NULL;
		return 0;
	}

	np = of_find_compatible_node((struct device_node *)qic_node, NULL, "dfx,sec_qic_irq_chk");
	if (unlikely(!np)) {
		pr_err("[%s] cannot get dfx,sec_qic_irq_chk node\n", __func__);
		return -ENODEV;
	}

	if (of_property_read_u32(np, "reg_num", &reg_num) != 0) {
		pr_err("[%s] get reg_num from DTS error\n", __func__);
		return -ENODEV;
	}

	qic_dev->irq_chk = devm_kzalloc(&pdev->dev,
		reg_num * sizeof(*(qic_dev->irq_chk)), GFP_KERNEL);
	if (unlikely(!(qic_dev->irq_chk))) {
		pr_err("[%s] alloc fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < reg_num; i++) {
		if (of_property_read_u32_index(np, "clk_addr", i, &qic_dev->irq_chk[i].clk_addr)) {
			pr_err("[%s] get clk_addr:%u from DTS error\n", __func__, i);
			return -ENODEV;
		}
		if (of_property_read_u32_index(np, "clk_mask", i, &qic_dev->irq_chk[i].clk_mask)) {
			pr_err("[%s] get clk_mask:%u from DTS error\n", __func__, i);
			return -ENODEV;
		}
		if (of_property_read_u32_index(np, "rst_addr", i, &qic_dev->irq_chk[i].rst_addr)) {
			pr_err("[%s] get rst_addr:%u from DTS error\n", __func__, i);
			return -ENODEV;
		}
		if (of_property_read_u32_index(np, "rst_mask", i, &qic_dev->irq_chk[i].rst_mask)) {
			pr_err("[%s] get rst_mask:%u from DTS error\n", __func__, i);
			return -ENODEV;
		}
		qic_dev->irq_chk[i].clk_reg = ioremap(qic_dev->irq_chk[i].clk_addr, sizeof(u32));
		if (!(qic_dev->irq_chk[i].clk_reg)) {
			pr_err("[%s] clk_reg:%u map failed\n", __func__, i);
			return -ENOMEM;
		}
		qic_dev->irq_chk[i].rst_reg = ioremap(qic_dev->irq_chk[i].rst_addr, sizeof(u32));
		if (!(qic_dev->irq_chk[i].rst_reg)) {
			pr_err("[%s] rst_reg:%u map failed\n", __func__, i);
			return -ENOMEM;
		}
	}

	return 0;
}

int dfx_sec_qic_common_init(const struct device_node *np, struct platform_device *pdev)
{
	int ret;

	ret = dfx_sec_qic_dump_init();
	if (ret)
		dev_err(&pdev->dev, "dfx_sec_qic_dump_init fail!");

	ret = dfx_sec_qic_get_mid_info(np, pdev);
	if (ret)
		goto qic_init_err;

	ret = dfx_sec_qic_get_bus_info(np, pdev);
	if (ret)
		goto qic_init_err;

	ret = dfx_sec_qic_get_irq_chk(np, pdev);
	if (ret)
		goto qic_init_err;

	ret = dfx_sec_qic_get_sub_intr_info(np, pdev);
	if (ret)
		goto qic_init_err;

	return 0;
qic_init_err:
	dev_err(&pdev->dev, "qic common init fail!");
	return ret;
}

static void dfx_sec_qic_unmap_reg(struct qic_src_reg *qic_reg, u32 reg_num)
{
	u32 i;

	for (i = 0; i < reg_num; i++) {
		if (qic_reg[i].reg_base) {
			iounmap(qic_reg[i].reg_base);
			qic_reg[i].reg_base = NULL;
		}
	}
}

static void dfx_sec_qic_unmap_chk(struct qic_chk_reg *irq_chk, u32 reg_num)
{
	u32 i;

	if (!irq_chk)
		return;

	for (i = 0; i < reg_num; i++) {
		if (irq_chk[i].clk_reg) {
			iounmap(irq_chk[i].clk_reg);
			irq_chk[i].clk_reg = NULL;
		}
		if (irq_chk[i].rst_reg) {
			iounmap(irq_chk[i].rst_reg);
			irq_chk[i].rst_reg = NULL;
		}
	}
}

void dfx_sec_qic_free_source(struct dfx_sec_qic_device *qic_dev)
{
	u32 i;
	u32 j;

	if (!qic_dev) {
		pr_err("no need to free\n");
		return;
	}

	dfx_sec_qic_unmap_reg(qic_dev->irq_reg, qic_dev->irq_reg_num);
	dfx_sec_qic_unmap_chk(qic_dev->irq_chk, qic_dev->irq_reg_num);
	for (i = 0; i < qic_dev->sub_intr_number; i++) {
		for (j = 0; j < (qic_dev->sub_intr_info)[i].qic_em_num; j++) {
			if (((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_addr) {
				iounmap(((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_addr);
				((qic_dev->sub_intr_info)[i].em_info)[j].em_intr_addr = NULL;
			}
		}
	}
}
