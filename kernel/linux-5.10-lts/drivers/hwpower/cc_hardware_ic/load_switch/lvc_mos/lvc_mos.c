// SPDX-License-Identifier: GPL-2.0
/*
 * lvc_mos.c
 *
 * lvc mos driver
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

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/platform_device.h>
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_supply.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_adapter.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_ic.h>
#include <chipset_common/hwpower/hardware_ic/lvc_mos.h>
#include <chipset_common/hwpower/common_module/power_log.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>

#define HWLOG_TAG lvc_mos
HWLOG_REGIST();

struct lvc_mos_dev {
	struct device *dev;
	struct notifier_block connect_nb;
	u32 ic_role;
	int device_id;
	bool enable_flag;
};

static struct lvc_mos_ic_ops *g_lvc_mos_ic_ops;
static struct lvc_mos_dev *g_lvc_mos_dev;

int lvc_mos_ic_ops_register(struct lvc_mos_ic_ops *ops)
{
	if (!ops) {
		hwlog_err("%s ops is null\n", __func__);
		return -EINVAL;
	}

	g_lvc_mos_ic_ops = ops;
	hwlog_info("%s ok\n", __func__);
	return 0;
}

static int lvc_mos_charge_init(void *dev_data)
{
	if (dc_adpt_is_iwatt_adapter()) {
		hwlog_err("is iwatt adapter, can not open lvc mos\n");
		return -EINVAL;
	}

	hwlog_info("%s\n", __func__);
	return 0;
}

static int lvc_mos_charge_exit(void *dev_data)
{
	struct lvc_mos_dev *di = dev_data;

	if (!di)
		return -EINVAL;

	di->enable_flag = false;
	hwlog_info("%s\n", __func__);
	return 0;
}

static int lvc_mos_charge_enable(int enable, void *dev_data)
{
	int ret;
	struct lvc_mos_dev *di = dev_data;
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;

	if (!di || !ops || !ops->ic_enable)
		return -EINVAL;

	ret = ops->ic_enable(!!enable);
	if (ret)
		return ret;

	di->enable_flag = !!enable;
	hwlog_info("%s enable=%d enable_flag=%u ret=%d\n", __func__, enable, di->enable_flag, ret);
	return 0;
}

static int lvc_mos_discharge(int enable, void *dev_data)
{
	hwlog_info("%s\n", __func__);
	return 0;
}

static int lvc_mos_is_device_close(void *dev_data)
{
	return 0;
}

static int lvc_mos_get_device_id(void *dev_data)
{
	hwlog_info("%s\n", __func__);
	return 0;
}

static int lvc_mos_config_watchdog_ms(int time, void *dev_data)
{
	hwlog_info("%s\n", __func__);
	return 0;
}

static int lvc_mos_kick_watchdog_ms(void *dev_data)
{
	return 0;
}

static struct dc_ic_ops lvc_mos_ops = {
	.dev_name = "lvc_mos",
	.ic_init = lvc_mos_charge_init,
	.ic_exit = lvc_mos_charge_exit,
	.ic_enable = lvc_mos_charge_enable,
	.irq_enable = NULL,
	.ic_adc_enable = NULL,
	.ic_discharge = lvc_mos_discharge,
	.is_ic_close = lvc_mos_is_device_close,
	.ic_enable_prepare = NULL,
	.get_ic_id = lvc_mos_get_device_id,
	.get_ic_status = NULL,
	.set_ic_buck_enable = NULL,
	.config_ic_watchdog = lvc_mos_config_watchdog_ms,
	.kick_ic_watchdog = lvc_mos_kick_watchdog_ms,
	.ic_reg_reset_and_init = NULL,
	.set_ic_freq = NULL,
	.get_ic_freq = NULL,
	.set_ic_thld = NULL,
	.get_max_ibat = NULL,
};

static int lvc_mos_batinfo_init(void *dev_data)
{
	return 0;
}

static int lvc_mos_batinfo_exit(void *dev_data)
{
	return 0;
}

static int lvc_mos_get_vbat_mv(void *dev_data)
{
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;

	if (!ops || !ops->get_bat_btb_voltage)
		return -EINVAL;

	return ops->get_bat_btb_voltage(ops->dev_data);
}

static int lvc_mos_get_vbus_mv(int *vbus, void *dev_data)
{
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;

	if (!vbus || !ops || !ops->get_vbus_voltage)
		return -EINVAL;

	return ops->get_vbus_voltage(vbus, ops->dev_data);
}

static int lvc_mos_get_ibat_ma(int *ibat, void *dev_data)
{
	if (!ibat)
		return -EINVAL;

	return power_supply_get_int_property_value(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_CURRENT_NOW, ibat);
}

static int lvc_mos_get_ibus(int *ibus, void *dev_data)
{
	int iadapter = 0;
	int ibat = 0;
	int ibus_other = 0;
	int ret;
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;
	struct lvc_mos_dev *di = dev_data;

	if (!di || !ops || !ops->get_ic_ibus)
		return -EINVAL;

	ret = ops->get_ic_ibus(&ibus_other, ops->dev_data);
	if (di->enable_flag)
		ret += dc_get_adapter_current(&iadapter);
	if (ret)
		return -EINVAL;

	*ibus = iadapter - ibus_other;
	if (*ibus > 0)
		return 0;

	power_supply_get_int_property_value(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_CURRENT_NOW, &ibat);
	*ibus = (ibat - ibus_other > 0) ? (ibat - ibus_other) : 0;
	return 0;
}

static int lvc_mos_get_device_temp(int *temp, void *dev_data)
{
	return 0;
}

static int lvc_mos_get_vout_mv(int *vout, void *dev_data)
{
	return 0;
}

static int lvc_mos_get_vusb_mv(int *vusb, void *dev_data)
{
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;

	if (!vusb || !ops || !ops->get_ic_vusb)
		return -EINVAL;

	return ops->get_ic_vusb(vusb, ops->dev_data);
}

static struct dc_batinfo_ops lvc_mos_batinfo_ops = {
	.init = lvc_mos_batinfo_init,
	.exit = lvc_mos_batinfo_exit,
	.get_bat_btb_voltage = lvc_mos_get_vbat_mv,
	.get_bat_package_voltage = lvc_mos_get_vbat_mv,
	.get_vbus_voltage = lvc_mos_get_vbus_mv,
	.get_bat_current = lvc_mos_get_ibat_ma,
	.get_ic_ibus = lvc_mos_get_ibus,
	.get_ic_temp = lvc_mos_get_device_temp,
	.get_ic_vout = lvc_mos_get_vout_mv,
	.get_ic_vusb = lvc_mos_get_vusb_mv,
};

static int lvc_mos_get_register_head(char *buffer, int size, void *dev_data)
{
	struct lvc_mos_dev *di = dev_data;

	if (!buffer || !di)
		return -EPERM;

	if (snprintf_s(buffer, size, size - 1, "lvc_mos iadap  ibus0  ibus1  ibat   ") < 0)
		return -EINVAL;
	return 0;
}

static int lvc_mos_value_dump(char *buffer, int size, void *dev_data)
{
	int iadapter = 0;
	int ibus0 = 0;
	int ibus1 = 0;
	int ibat = 0;
	struct lvc_mos_dev *di = dev_data;
	struct lvc_mos_ic_ops *ops = g_lvc_mos_ic_ops;

	if (!buffer || !di || !ops || !ops->get_ic_ibus)
		return -EPERM;

	if (di->enable_flag) {
		dc_get_adapter_current(&iadapter);
		ops->get_ic_ibus(&ibus0, ops->dev_data);
		lvc_mos_get_ibus(&ibus1, di);
		lvc_mos_get_ibat_ma(&ibat, di);
	}
	if (snprintf_s(buffer, size, size - 1, "%s %-7d%-7d%-7d%-7d",
		(di->enable_flag ? "  on   " : "  off  "), iadapter, ibus0, ibus1, ibat) < 0)
		return -EINVAL;
	return 0;
}

static struct power_log_ops lvc_mos_log_ops = {
	.dev_name = "lvc_mos",
	.dump_log_head = lvc_mos_get_register_head,
	.dump_log_content = lvc_mos_value_dump,
};

static void lvc_mos_parse_dts(struct device_node *np, struct lvc_mos_dev *di)
{
	/* 2: lvc mos as the aux lvc chip ic */
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "ic_role", &(di->ic_role), 2);
}

static int lvc_mos_ops_register(struct lvc_mos_dev *di)
{
	int ret;

	lvc_mos_ops.dev_data = di;
	lvc_mos_batinfo_ops.dev_data = di;
	ret = dc_ic_ops_register(LVC_MODE, di->ic_role, &lvc_mos_ops);
	ret += dc_batinfo_ops_register(di->ic_role, &lvc_mos_batinfo_ops, di->device_id);
	if (ret)
		return ret;

	lvc_mos_log_ops.dev_data = di;
	power_log_ops_register(&lvc_mos_log_ops);
	return 0;
}

static int lvc_mos_event_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct lvc_mos_dev *di = g_lvc_mos_dev;

	if (!di)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_USB_DISCONNECT:
		di->enable_flag = false;
		hwlog_info("usb disconnect, set enable_flag false\n");
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int lvc_mos_probe(struct platform_device *pdev)
{
	int ret;
	struct lvc_mos_dev *di = NULL;
	struct device_node *np = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &pdev->dev;
	np = pdev->dev.of_node;
	lvc_mos_parse_dts(np, di);
	lvc_mos_ops_register(di);

	di->connect_nb.notifier_call = lvc_mos_event_notifier_call;
	(void)power_event_bnc_register(POWER_BNT_CONNECT, &di->connect_nb);

	g_lvc_mos_dev = di;
	platform_set_drvdata(pdev, di);
	return 0;
}

static int lvc_mos_remove(struct platform_device *pdev)
{
	struct lvc_mos_dev *di = platform_get_drvdata(pdev);

	if (!di)
		return -ENODEV;

	power_event_bnc_unregister(POWER_BNT_CONNECT, &di->connect_nb);
	kfree(di);
	platform_set_drvdata(pdev, NULL);
	g_lvc_mos_dev = NULL;
	return 0;
}

static const struct of_device_id lvc_mos_match_table[] = {
	{
		.compatible = "huawei,lvc_mos",
		.data = NULL,
	},
	{},
};

static struct platform_driver lvc_mos_driver = {
	.probe = lvc_mos_probe,
	.remove = lvc_mos_remove,
	.driver = {
		.name = "huawei,lvc_mos",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lvc_mos_match_table),
	},
};

static int __init lvc_mos_init(void)
{
	return platform_driver_register(&lvc_mos_driver);
}

static void __exit lvc_mos_exit(void)
{
	platform_driver_unregister(&lvc_mos_driver);
}

module_init(lvc_mos_init);
module_exit(lvc_mos_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("lvc mos driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
