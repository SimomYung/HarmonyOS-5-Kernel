/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_spmi.c
 *
 * scharger spmi driver
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, pmicstributed, and mopmicfied under those terms.
 *
 * This program is pmicstributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/regmap.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <platform_include/basicplatform/linux/spmi_platform.h>
#include "scharger_spmi.h"
#include <securec.h>
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif

#define INTERRUPT_STORM_CHECK_TIME 50
#define INTERRUPT_STORM_CNT 100
#define DSM_BUFF_SIZE_MAX       1024
#define INTERRUPT_STORM_INFO_BUFF_SIZE_MAX 768
#define SCHARGERV800_IRQ_NUM 26
#define TEMP_BUFF_SIZE 50

/* schargerv800 all irq flag reg */
static const unsigned int irq_flag[SCHARGERV800_IRQ_NUM] = {0x10, 0x11, 0x1f, 0x119, 0x11a, 0x11d, 0x130, 0x131, 0x501,
														0x502, 0x802, 0x803, 0x804, 0x805, 0x806, 0x807, 0x808, 0x809,
														0x80a, 0x80b, 0x80c, 0x80d, 0x80e, 0x80f, 0x2027, 0x2028};

static struct scharger_spmi_priv *g_scharger = NULL;
static int g_scharger_test = 0;

static int scharger_regmap_read(void *context, unsigned int reg, unsigned int *val)
{
	int ret;
	struct spmi_device *pdev = NULL;
	struct scharger_spmi_priv *scharger = context;

	if (!scharger || !val) {
		pr_err("could not find dev or val, %s failed!", __func__);
		return -ENODEV;
	}
	*val = 0;

	pdev = to_spmi_device(scharger->dev);
	if (pdev == NULL) {
		pr_err("%s:pdev get failed!\n", __func__);
		return -ENODEV;
	}

	ret = vendor_spmi_ext_register_readl(
		pdev->ctrl, pdev->usid, reg, (unsigned char *)val, 1);
	if (ret)
		pr_err("%s:spmi_ext_register_readl failed!\n", __func__);
	return ret;
}

static int scharger_regmap_write(void *context, unsigned int reg, unsigned int val)
{
	int ret;
	u8 value;
	struct spmi_device *pdev = NULL;
	struct scharger_spmi_priv *scharger = context;

	if (!scharger) {
		pr_err("could not find dev, %s failed!", __func__);
		return -ENODEV;
	}

	pdev = to_spmi_device(scharger->dev);
	if (pdev == NULL) {
		pr_err("%s:sub pdev get failed!\n", __func__);
		return -ENODEV;
	}

	if (g_scharger_test)
		pr_err("%s:reg = 0x%x, val = 0x%x!\n", __func__, reg, val);

	value = val & 0xff;

	ret = vendor_spmi_ext_register_writel(
		pdev->ctrl, pdev->usid, reg, &value, 1);
	if (ret)
		pr_err("%s:spmi_ext_register_writel failed!\n", __func__);
	return ret;
}

static const struct regmap_config scharger_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.max_register = 0x7fff,
	.fast_io = true,
	.reg_read = scharger_regmap_read,
	.reg_write = scharger_regmap_write,
};

static void _scharger_irq_mask(
	struct irq_data *d, int maskflag)
{
	struct scharger_spmi_priv *scharger = irq_data_get_irq_chip_data(d);
	u32 data = 0;
	u32 offset;
	unsigned long flags;
	int ret;

	if (scharger == NULL) {
		pr_err("irq_mask pmic is NULL\n");
		return;
	}
	/* Convert interrupt data to interrupt offset */
	offset = (irqd_to_hwirq(d) / ONE_IRQ_GROUP_NUM);
	offset = scharger->irq_mask_addr_arry[offset];
	spin_lock_irqsave(&scharger->lock, flags);

	ret = regmap_read(scharger->regmap, offset, &data);
	if (ret < 0) {
		pr_err("%s regmap read interrupt offset 0x%x failed\n",
			__func__, offset);
		spin_unlock_irqrestore(&scharger->lock, flags);
		return;
	}

	if (maskflag == MASK)
		data |= (1 << (irqd_to_hwirq(d) & 0x07));
	else
		data &= ~(u32)(1 << (irqd_to_hwirq(d) & 0x07));

	ret = regmap_write(scharger->regmap, offset, data);
	if (ret < 0)
		pr_err("%s regmap write interrupt offset 0x%x failed\n",
			__func__, offset);

	spin_unlock_irqrestore(&scharger->lock, flags);
}

static void scharger_irq_mask(struct irq_data *d)
{
	_scharger_irq_mask(d, MASK);
}

static void scharger_irq_unmask(struct irq_data *d)
{
	_scharger_irq_mask(d, UNMASK);
}

static struct irq_chip scharger_spmi_irqchip = {
	.name = "scharger-spmi-irq",
	.irq_mask = scharger_irq_mask,
	.irq_unmask = scharger_irq_unmask,
	.irq_disable = scharger_irq_mask,
	.irq_enable = scharger_irq_unmask,
};

static int scharger_irq_map(
	struct irq_domain *d, unsigned int virq, irq_hw_number_t hw)
{
	struct scharger_spmi_priv *scharger = d->host_data;
	int ret;

	if (!scharger)
		return -ENOMEM;

	irq_set_chip_and_handler_name(
		virq, &scharger_spmi_irqchip, handle_simple_irq, "scharger");
	irq_set_chip_data(virq, scharger);
	ret = irq_set_irq_type(virq, IRQ_TYPE_NONE);
	if (ret < 0)
		pr_err("irq set type fail\n");

	return 0;
}

static const struct irq_domain_ops scharger_domain_ops = {
	.map = scharger_irq_map,
	.xlate = irq_domain_xlate_twocell,
};

static void scharger_set_clock_mode(struct scharger_spmi_priv *scharger, unsigned int mode)
{
	int ret;

	ret = regmap_update_bits(scharger->regmap, CHG_SYS_LOGIC_CFG_REG_5_REG, CHG_CLOCK_MODE_MSK,
		 mode << CHG_CLOCK_MODE_SHIFT);
	if (ret)
		pr_err("%s %d error\n", __func__, mode);
}

static int scharger_dsm_report_interrupt_storm_info(char *buf)
{
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	char dsm_buf[DSM_BUFF_SIZE_MAX] = {0};
	int ret = 0;

	if (buf == NULL) {
		pr_err("%s, buf is NULL\n", __func__);
		return -1;
	}
	ret = sprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, "interrupt storm happened, %s\n", buf);
	if (ret < 0)
		pr_err("%s, sprintf_s error\n", __func__);

	return power_dsm_report_dmd(POWER_DSM_BATTERY, DSM_SC0_ERROR, dsm_buf);
#else
	return 0;
#endif
}

static void interrupt_storm_check(struct scharger_spmi_priv *scharger)
{
	unsigned long now_time;
	int i;
	unsigned int pending;
	char buf[INTERRUPT_STORM_INFO_BUFF_SIZE_MAX] = {0};
	size_t len = 0;
	int ret;

	now_time = jiffies;
	if (scharger->irq_cnt != 0 &&
		time_before(now_time, scharger->first_irq_time + msecs_to_jiffies(INTERRUPT_STORM_CHECK_TIME))) {
		scharger->irq_cnt++;
	} else {
		scharger->irq_cnt = 1;
		scharger->first_irq_time = jiffies;
	}

	if (scharger->irq_cnt >= INTERRUPT_STORM_CNT) {
		scharger->irq_cnt = 0;
		for (i = 0 ; i < SCHARGERV800_IRQ_NUM ; i++) {
			(void)regmap_read(scharger->regmap, irq_flag[i], &pending);
			pr_err("%s reg=0x%x, pending=0x%x\n", __func__, irq_flag[i], pending);
			len = strlen(buf);
			ret = snprintf_s(buf + len, INTERRUPT_STORM_INFO_BUFF_SIZE_MAX - len,
			INTERRUPT_STORM_INFO_BUFF_SIZE_MAX - len -1, "reg=0x%x val=0x%x, ", irq_flag[i], pending);
			if (ret < 0)
				pr_err("%s, sprintf_s error\n", __func__);
			(void)regmap_write(scharger->regmap, irq_flag[i], pending);
		}
		scharger_dsm_report_interrupt_storm_info(buf);
	}
}

static irqreturn_t scharger_spmi_irq_handler(int irq, void *data)
{
	unsigned int pending = 0;
	unsigned int mask = 0;
	unsigned long pending_s;
	unsigned int i, offset;
	int ret;
	struct scharger_spmi_priv *scharger = (struct scharger_spmi_priv *)data;

	interrupt_storm_check(scharger);

	scharger_set_clock_mode(scharger, CLOCK_1M);
	for (i = 0; i < scharger->irqarray_num; i++) {
		ret = regmap_read(scharger->regmap, scharger->irq_addr_arry[i], &pending);
		if (ret < 0) {
			pr_err("%s regmap read 0x%x failed\n", __func__,
				scharger->irq_addr_arry[i]);
			break;
		}

		ret = regmap_read(scharger->regmap, scharger->irq_mask_addr_arry[i], &mask);
		if (ret < 0) {
			pr_err("%s regmap read 0x%x failed\n", __func__,
				scharger->irq_mask_addr_arry[i]);
			break;
		}

		pr_info("[%s] pending 0x%x, mask 0x%x, action: 0x%x\n", __func__, pending, mask, pending & (~mask));
		pr_info("[%s] 0:FCP, 1:Flash, 2:UFCS, 3:BC12\n", __func__);
		pr_info("[%s] 4:PD,  5:SC,    6:CHG,  7:COUL\n", __func__);

		pending &= (~mask);
		pending_s = (unsigned long)pending;
		if (pending_s) {
			for_each_set_bit(offset, &pending_s, SCHARGER_BITS)
				generic_handle_irq(scharger->irqs[offset + i * SCHARGER_BITS]);
		}
	}
	scharger_set_clock_mode(scharger, CLOCK_AUTO);

	return IRQ_HANDLED;
}

static int scharger_spmi_irq_create_mapping(struct scharger_spmi_priv *scharger)
{
	int i;
	unsigned int virq;

	for (i = 0; i < scharger->irqnum; i++) {
		virq = irq_create_mapping(scharger->domain, i);
		if (virq == NO_IRQ) {
			dev_err(scharger->dev, "Failed mapping hwirq\n");
			return -ENOSPC;
		}
		scharger->irqs[i] = virq;
		dev_info(scharger->dev, "scharger->irqs[%d] = %d\n", i, scharger->irqs[i]);
	}
	return 0;
}

static int scharger_spmi_irq_prc(struct scharger_spmi_priv *scharger)
{
	int i;
	unsigned int pending;
	int ret;

	for (i = 0; i < scharger->irqarray_num; i++) {
		ret = regmap_write(scharger->regmap, scharger->irq_mask_addr_arry[i], SCHARGER_MASK_STATE);
		if (ret < 0) {
			dev_err(scharger->dev, "%s regmap write 0x%x failed\n", __func__,
				scharger->irq_mask_addr_arry[i]);
			return ret;
		}
	}

	for (i = 0; i < scharger->irqarray_num; i++) {
		ret = regmap_read(scharger->regmap, scharger->irq_addr_arry[i], &pending);
		if (ret < 0) {
			dev_err(scharger->dev, "%s regmap read 0x%x failed\n", __func__,
				scharger->irq_addr_arry[i]);
			return ret;
		}

		ret = regmap_write(scharger->regmap, scharger->irq_addr_arry[i], SCHARGER_MASK_STATE);
		if (ret < 0) {
			dev_err(scharger->dev, "%s regmap write 0x%x failed\n", __func__,
				scharger->irq_addr_arry[i]);
			return ret;
		}

		pr_debug("Scharger IRQ address value:irq[0x%x] = 0x%x\n",
			scharger->irq_addr_arry[i], pending);
	}
	return 0;
}

static int scharger_spmi_irq_register(
	struct spmi_device *pdev, struct scharger_spmi_priv *scharger)
{
	int ret;
	enum of_gpio_flags flags;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	scharger->gpio = of_get_gpio_flags(np, 0, &flags);
	if (scharger->gpio < 0) {
		dev_err(dev, "failed to get_gpio_flags:%d\n", scharger->gpio);
		return scharger->gpio;
	}

	if (!gpio_is_valid(scharger->gpio))
		return -EINVAL;

	ret = gpio_request_one(scharger->gpio, GPIOF_IN, "scharger-spmi");
	if (ret) {
		dev_err(dev, "failed to request gpio%d, %d\n",
			scharger->gpio, ret);
		return -ENODEV;
	}

	scharger->irq = gpio_to_irq(scharger->gpio);
	dev_info(dev, "[%s] gpio=%d, irq=%d\n", __func__, scharger->gpio, scharger->irq);

	/* mask && clear IRQ status */
	ret = scharger_spmi_irq_prc(scharger);
	if (ret) {
		dev_err(dev, "failed to mask && clear irq!\n");
		goto fail;
	}

	scharger->irqs = (unsigned int *)devm_kmalloc(
		dev, scharger->irqnum * sizeof(int), GFP_KERNEL);
	if (!scharger->irqs)
		goto fail;

	scharger->domain = irq_domain_add_simple(
		np, scharger->irqnum, 0, &scharger_domain_ops, scharger);
	if (!scharger->domain) {
		dev_err(dev, "failed irq domain add simple!\n");
		goto fail;
	}

	ret = scharger_spmi_irq_create_mapping(scharger);
	if (ret)
		goto fail;

	ret = request_threaded_irq(scharger->irq, scharger_spmi_irq_handler, NULL,
		IRQF_TRIGGER_LOW | IRQF_NO_SUSPEND, "scharger-irq", scharger);
	if (ret) {
		dev_err(dev, "request irq fail %d\n", ret);
		goto fail;
	}

	return 0;
fail:
	gpio_free(scharger->gpio);
	return -ENODEV;
}

static int opt_regs_set(struct scharger_spmi_priv *scharger)
{
	unsigned int i;
	int ret;
	struct opt_regs *opt = scharger->opt_regs_set;

	for (i = 0; i < scharger->opt_regs_num; i++) {
		ret = regmap_update_bits(scharger->regmap, opt[i].reg, opt[i].mask,
					opt[i].val);
		if (ret) {
			pr_err("[%s] error to set regs, reg=0x%x, mask=0x%x, val=0x%x\n",
			__func__, opt[i].reg, opt[i].mask, opt[i].val);
			return -ENOMEM;
		}
		pr_debug("[%s] reg=0x%x, mask=0x%x, val=0x%x\n",
			__func__, opt[i].reg, opt[i].mask, opt[i].val);
	}
	return 0;
}

static int set_opt_regs_data(struct scharger_spmi_priv *scharger)
{
	int ret;
	int num = 0;
	struct device *dev = scharger->dev;
	struct device_node *np = dev->of_node;

	(void)to_spmi_device(dev);

	num = of_property_count_u32_elems(np, "common-option-regs-set");
	if (num <= 0 || num % OPT_SET_SIZE != 0) {
		dev_info(dev, "invalid common-option-regs data num=%d\n", num);
		return 0;
	}
	scharger->opt_regs_num = num / OPT_SET_SIZE;

	scharger->opt_regs_set = devm_kzalloc(dev,
		sizeof(struct opt_regs) * scharger->opt_regs_num, GFP_KERNEL);
	if (!scharger->opt_regs_set)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "common-option-regs-set",
		(u32 *)scharger->opt_regs_set, num);
	if (ret) {
		dev_err(scharger->dev, "failed to get common-option-regs-set\n");
		return -ENODEV;
	}
	return opt_regs_set(scharger);
}

static int get_scharger_spmi_irq_data(struct scharger_spmi_priv *scharger)
{
	int ret;
	struct device *dev = scharger->dev;
	struct device_node *np = dev->of_node;
	struct spmi_device *pdev = NULL;

	pdev = to_spmi_device(dev);
	if (!pdev)
		return -ENODEV;

	/* get scharger irq num */
	ret = of_property_read_u32(np, "scharger-irq-num", &(scharger->irqnum));
	if (ret) {
		dev_err(dev, "no scharger-irq-num property set\n");
		return -ENODEV;
	}

	/* get scharger irq array number */
	ret = of_property_read_u32(np, "scharger-irq-array-num", &(scharger->irqarray_num));
		if (ret) {
		dev_err(dev, "no scharger-irq-array property set\n");
		return -ENODEV;
	}

	scharger->irq_mask_addr_arry = (int *)devm_kzalloc(dev,
		sizeof(int) * scharger->irqarray_num, GFP_KERNEL);
	if (!scharger->irq_mask_addr_arry)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "scharger-irq-mask-addr",
		(int *)scharger->irq_mask_addr_arry, scharger->irqarray_num);
	if (ret) {
		dev_err(dev, "no scharger-irq-mask-addr property set\n");
		return -ENODEV;
	}

	scharger->irq_addr_arry = (int *)devm_kzalloc(dev,
		sizeof(int) * scharger->irqarray_num, GFP_KERNEL);
	if (!scharger->irq_addr_arry)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "scharger-irq-addr",
		(int *)scharger->irq_addr_arry, scharger->irqarray_num);
	if (ret) {
		dev_err(dev, "no scharger-irq-addr property set\n");
		return -ENODEV;
	}
	return 0;
}

/* 0-success or others-fail */
static int device_check(struct scharger_spmi_priv *scharger)
{
	int ret = 0;
	u32 chip_id_reg_num = 0;
	u32 version_reg_num = 0;
	u32 chip_id_reg_addr = 0;
	u32 version_reg_addr = 0;
	struct device *dev = scharger->dev;
	struct device_node *np = dev->of_node;

	ret += of_property_read_u32(np, "chip-id-reg-num", &chip_id_reg_num);
	ret += of_property_read_u32(np, "version-reg-num", &version_reg_num);
	ret += of_property_read_u32(np, "chip-id-reg-addr", &chip_id_reg_addr);
	ret += of_property_read_u32(np, "version-reg-addr", &version_reg_addr);
	if (ret) {
		dev_err(dev, "[$s] parse version reg data failed!\n", __func__);
		return -ENODEV;
	}

	ret = regmap_bulk_read(scharger->regmap, chip_id_reg_addr,
		&scharger->chip_id, chip_id_reg_num);
	if (ret) {
		dev_err(dev, "[%s]:read chip_id fail\n", __func__);
		return CHARGE_IC_BAD;
	}
	dev_info(dev, "chip id is 0x%x\n", scharger->chip_id);

	ret = regmap_bulk_read(scharger->regmap, version_reg_addr,
		&scharger->scharger_version, version_reg_num);
	if (ret) {
		dev_err(dev, "[%s]:read chip version fail\n", __func__);
		return CHARGE_IC_BAD;
	}
	dev_info(dev, "chip version is 0x%x\n", scharger->scharger_version);

	return CHARGE_IC_GOOD;
}

#ifdef CONFIG_DFX_DEBUG_FS
void scharger_spmi_test_print(unsigned int enable)
{
	unsigned int en;

	en = !!enable;
	g_scharger_test = en;
}

unsigned int scharger_spmi_test_read(unsigned int reg)
{
	struct scharger_spmi_priv *scharger = g_scharger;
	unsigned int val = 0;
	int ret;

	if (!scharger) {
		pr_err("%s scharger is null\n", __func__);
		return -1;
	}

	ret = regmap_read(scharger->regmap, reg, &val);
	if (ret)
		pr_err("%s regmap_read 0x%x failed, ret %d\n", __func__, reg, ret);

	pr_err("%s reg 0x%x = 0x%x\n", __func__, reg, val);
	return val;
}
EXPORT_SYMBOL_GPL(scharger_spmi_test_read);

unsigned int scharger_spmi_test_write(unsigned int reg, unsigned int val)
{
	struct scharger_spmi_priv *scharger = g_scharger;
	int ret;

	if (!scharger) {
		pr_err("%s scharger is null\n", __func__);
		return -1;
	}

	pr_err("%s reg 0x%x = 0x%x\n", __func__, reg, val);
	ret = regmap_write(scharger->regmap, reg, val);
	if (ret < 0) {
		pr_err("%s regmap write 0x%x failed\n", __func__, reg);
		return ret;
	}
	return scharger_spmi_test_read(reg);
}
EXPORT_SYMBOL_GPL(scharger_spmi_test_write);
#endif

static void coul_check(struct scharger_spmi_priv *scharger)
{
	int ret;
	struct device_node *np = scharger->dev->of_node;
	struct device_node *coul_np = NULL;
	unsigned int coul_ctrl_onoff_reg = 0;
	const char *status = NULL;

	coul_np = of_find_compatible_node(np, NULL, "hisilicon,scharger_coul");
	if (coul_np == NULL)
		return;

	ret = of_property_read_u32(np, "coul-ctrl-onoff-reg", &coul_ctrl_onoff_reg);
	if (ret) {
		pr_info("[%s] do not ctrl coul\n", __func__);
		return;
	}

	ret = of_property_read_string(coul_np, "status", &status);
	if (ret)
		return;

	if (strcmp(status, "ok") == 0)
		return;

	pr_info("[%s] set coul ctrl off\n", __func__);
	regmap_write(scharger->regmap, DEBUG_WRITE_PRO_ADDR, DEBUG_WRITE_PRO_UNLOCK);
	regmap_write(scharger->regmap, coul_ctrl_onoff_reg, COUL_DISABLE);
	regmap_write(scharger->regmap, DEBUG_WRITE_PRO_ADDR, DEBUG_WRITE_PRO_LOCK);
}

static int scharger_spmi_probe(struct spmi_device *pdev)
{
	int ret;
	struct scharger_spmi_priv *scharger = NULL;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	pr_info("[%s]scharger *********\n", __func__);
	scharger = devm_kzalloc(dev, sizeof(*scharger), GFP_KERNEL);
	if (!scharger) {
		dev_err(dev, "[%s]scharger is null\n", __func__);
		return -ENOMEM;
	}
	scharger->dev = dev;

	ret = of_property_read_u32(np, "slave_id", (u32 *)&(pdev->usid));
	if (ret) {
		dev_err(dev, "failed to get slave_id\n");
		return -ENODEV;
	}

	scharger->regmap = devm_regmap_init(dev, NULL, scharger, &scharger_regmap_config);
	if (IS_ERR(scharger->regmap)) {
		ret = PTR_ERR(scharger->regmap);
		dev_err(dev, "regmap init failed, ret=%d\n", ret);
		return ret;
	}

	ret = device_check(scharger);
	if (ret)
	 	return ret;

	ret = set_opt_regs_data(scharger);
	if (ret) {
		dev_err(dev, "[%s] failed to opt data\n", __func__);
		return ret;
	}

	coul_check(scharger);

	ret = get_scharger_spmi_irq_data(scharger);
	if (ret) {
		dev_err(dev, "[%s] failed to get spmi_irq_data\n", __func__);
		return ret;
	}

	spin_lock_init(&scharger->lock);
	ret = scharger_spmi_irq_register(pdev, scharger);
	if (ret) {
		dev_err(dev, "[%s] irq register fail ret %d\n",
			__func__, ret);
		return ret;
	}

	ret = devm_of_platform_populate(&pdev->dev);
	if (ret) {
		dev_err(dev, "%s populate fail ret %d\n", __func__, ret);
		gpio_free(scharger->gpio);
		return ret;
	}

	g_scharger = scharger;
	dev_info(dev, "[%s] succ\n", __func__);
	return ret;
}

static void scharger_spmi_shutdown(struct spmi_device *pdev)
{
}

static const struct of_device_id of_spmi_scharger_spmi_match_tbl[] = {
	{ .compatible = "hisilicon,scharger-v800", },
	{  }   /* end */
};

static const struct spmi_device_id scharger_spmi_id[] = {
	{ "scharger_spmi_driver", 0 },
	{ }   /* end */
};

MODULE_DEVICE_TABLE(spmi, scharger_spmi_id);

static struct spmi_driver scharger_spmi_driver = {
	.driver = {
			.name = "scharger_spmi_driver",
			.owner = THIS_MODULE,
			.of_match_table = of_spmi_scharger_spmi_match_tbl,
		},
	.id_table = scharger_spmi_id,
	.probe = scharger_spmi_probe,
	.shutdown = scharger_spmi_shutdown,
};

static int __init scharger_spmi_init(void)
{
	return spmi_driver_register(&scharger_spmi_driver);
}

static void __exit scharger_spmi_exit(void)
{
	spmi_driver_unregister(&scharger_spmi_driver);
}

subsys_initcall_sync(scharger_spmi_init);
module_exit(scharger_spmi_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("scharger spmi driver");
