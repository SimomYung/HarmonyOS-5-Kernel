/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * pmic_vbus.c
 *
 * Device driver for PMU VBUS interrupts
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/of_irq.h>
#include <platform_include/basicplatform/linux/mfd/pmic_vbus.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>

static char *g_vbus_irq_names[] = {
	[VBUS_PLUGIN] = "Plug_in_vbus_irq",
	[VBUS_PLUGOUT] = "Plug_out_vbus_irq",
	[VBUS_PLUGIN_AND_OUT] = "Plug_in_and_out_vbus_irq",
	[VBUS_PLUGEOF] = "EOF"
};

static RAW_NOTIFIER_HEAD(vbus_notify_chain);
#ifdef CONFIG_DFX_DEBUG_FS
static int pmic_vbus_test_notifier_call(
	struct notifier_block *nb, unsigned long val, void *data)
{
	pr_err("%s, %s happen.\n", __func__, g_vbus_irq_names[val]);
	return 0;
}
static struct notifier_block vbus_test_chain_nb = {
	.notifier_call = pmic_vbus_test_notifier_call,
	.priority = INT_MIN,
};
#endif

int pmic_vbus_register_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(
		&vbus_notify_chain, nb);
}
int pmic_vbus_unregister_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(
		&vbus_notify_chain, nb);
}
static int pmic_vbus_call_notifiers(int val, void *v)
{
	return raw_notifier_call_chain(
		&vbus_notify_chain, (unsigned long)val, v);
}

static irqreturn_t pmic_vbus_irq_handle(int irq_id, void *data)
{
	u32 val = 0;
	enum vbus_irq_type event = VBUS_PLUGEOF;
	struct pmic_vbus *pmic_vbus_p = (struct pmic_vbus *)data;
	struct device *dev = pmic_vbus_p->dev;
	
	regmap_read(pmic_vbus_p->reg_map, pmic_vbus_p->event_reg, &val);
	// vbus_event_reg_clr
	if (val)
		regmap_write(pmic_vbus_p->reg_map, pmic_vbus_p->event_reg, val);

	if (val & (1 << pmic_vbus_p->event_reg_bits[VBUS_PLUGIN]) &&
		val & (1 << pmic_vbus_p->event_reg_bits[VBUS_PLUGOUT]))
		event = VBUS_PLUGIN_AND_OUT;
	else if(val & (1 << pmic_vbus_p->event_reg_bits[VBUS_PLUGIN]))
		event = VBUS_PLUGIN;
	else if (val & (1 << pmic_vbus_p->event_reg_bits[VBUS_PLUGOUT]))
		event = VBUS_PLUGOUT;

	pmic_vbus_call_notifiers(event, NULL);
	dev_err(dev, "%s vbus event reg: 0x%x = 0x%x, %s happen!\n",
		__func__, pmic_vbus_p->event_reg, val, g_vbus_irq_names[event]);

	return IRQ_HANDLED;
}

static int vbus_dts_init(
		struct pmic_vbus *pmic_vbus_p)
{
	int i;
	int ret;
	struct device *dev = pmic_vbus_p->dev;
	struct device_node *np = dev->of_node;

	ret = of_property_read_string(np,"interrupt-names", 
		(const char **)(&pmic_vbus_p->parent_irq_name));
	pmic_vbus_p->irq_id = of_irq_get_byname(dev->of_node, 
		pmic_vbus_p->parent_irq_name);

	ret = of_property_read_u32(np, 
		"pmic-vbus-event-reg", &pmic_vbus_p->event_reg);
	if (ret) {
		dev_err(dev, "pmic-vbus-event-reg read fail\n");
		return ret;
	}

	pmic_vbus_p->event_reg_bits = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * VBUS_IRQ_TYPE_NUM, GFP_KERNEL);
	ret = of_property_read_u32_array(np, "pmic-vbus-event-reg-bits", 
		pmic_vbus_p->event_reg_bits, VBUS_IRQ_TYPE_NUM);
	if (ret) {
		dev_err(dev, "pmic-vbus-event-reg-bits read fail\n");
		return ret;
	}

	return ret;
}

static int pmic_vbus_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pmic_vbus *pmic_vbus_p = NULL;
	int ret;

	pmic_vbus_p = devm_kzalloc(dev, sizeof(*pmic_vbus_p), GFP_KERNEL);
	if (pmic_vbus_p == NULL)
		return -ENOMEM;

	pmic_vbus_p->dev = dev;
	pmic_vbus_p->reg_map = dev_get_regmap(dev->parent, NULL);

	ret = vbus_dts_init(pmic_vbus_p);
	if (ret) {
		dev_err(dev, "[%s]vbus_dts_para_init failed\n", __func__);
		return -ENODEV;
	}

	ret = devm_request_irq(dev, pmic_vbus_p->irq_id, 
		pmic_vbus_irq_handle, IRQF_NO_SUSPEND,
		pmic_vbus_p->parent_irq_name, pmic_vbus_p);
	if (ret < 0) {
		dev_err(dev, "failed to request pmic_vbus_irq_handle.\n");
		return -ENOENT;
	}

#ifdef CONFIG_DFX_DEBUG_FS
	ret = pmic_vbus_register_notifier(&vbus_test_chain_nb);
	if (ret < 0) {
		dev_err(dev, "failed to regist vbus_test_chain_nb\n");
		return -ENOENT;
	}
#endif

	dev_info(dev, "[%s] succ\n", __func__);
	return 0;
}

static int pmic_vbus_remove(struct platform_device *pdev)
{
#ifdef CONFIG_DFX_DEBUG_FS
	return pmic_vbus_unregister_notifier(&vbus_test_chain_nb);
#endif
	return 0;
}

const static struct of_device_id pmic_vbus_match_tbl[] = {
	{
		.compatible = "pmic-vbus"
	},
	{}  /* end */
};

static struct platform_driver pmic_vbus_driver = {
	.driver = {
		.name = "pmic-vbus-driver",
		.owner  = THIS_MODULE,
		.of_match_table = pmic_vbus_match_tbl,
	},
	.probe = pmic_vbus_probe,
	.remove = pmic_vbus_remove,
};

static int __init pmic_vbus_init(void)
{
	return platform_driver_register(&pmic_vbus_driver);
}

static void __exit pmic_vbus_exit(void)
{
	platform_driver_unregister(&pmic_vbus_driver);
}

module_init(pmic_vbus_init);
module_exit(pmic_vbus_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SPMI PMU VBUS Driver");
