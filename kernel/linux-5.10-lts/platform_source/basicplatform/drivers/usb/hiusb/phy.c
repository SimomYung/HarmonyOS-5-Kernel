/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: phy driver for HIUSB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/platform_drivers/usb/hiusb_phy.h>
#ifdef CONFIG_FMEA_FAULT_INJECTION
#include <pmic_interface.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <debug_fault_injection.h>
#endif

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_PHY]%s: " fmt, __func__

int hiusb_usb3phy_init(struct phy *phy)
{
	int ret;

	pr_info("+\n");
	if (!phy)
		return 0;

	ret = phy_init(phy);
	if (ret) {
		pr_err("init failed %d\n", ret);
		return ret;
	}
	pr_info("-\n");
	return 0;
}

int hiusb_usb3phy_exit(struct phy *phy)
{
	int ret;
	pr_info("+\n");

	if (!phy)
		return 0;

	ret = phy_exit(phy);
	if (ret) {
		pr_err("exit failed %d\n", ret);
		return ret;
	}

	pr_info("-\n");
	return 0;
}

int hiusb_usb3phy_set_orien(struct phy *phy, enum hiusb_phy_orien orien)
{
	struct hiusb_usb3phy *usb3phy = NULL;

	if (!phy)
		return 0;

	usb3phy = phy_get_drvdata(phy);
	if (usb3phy && usb3phy->set_orien)
		return usb3phy->set_orien(usb3phy, orien);

	return 0;
}

int hiusb_usb3phy_set_dpmode(struct phy *phy, enum hiusb_phy_dp_mode dpmode, uint32_t usb_role)
{
	struct hiusb_usb3phy *usb3phy = NULL;

	if (!phy)
		return 0;

	usb3phy = phy_get_drvdata(phy);
	if (usb3phy && usb3phy->set_dpmode)
		return usb3phy->set_dpmode(usb3phy, dpmode, usb_role);

	return 0;
}

int hiusb_usb2phy_poweron(struct phy *phy)
{
	int ret;

	pr_info("+\n");
	if (!phy)
		return 0;

	ret = phy_power_on(phy);
	if (ret) {
		pr_err("usb2 phy poweron failed\n");
		return ret;
	}

	pr_info("-\n");
	return ret;
}

int hiusb_usb2phy_poweroff(struct phy *phy)
{
	int ret;

	pr_info("+\n");
	if (!phy)
		return 0;

	ret = phy_power_off(phy);
	if (ret) {
		pr_err("usb2 phy poweroff failed\n");
		return ret;
	}

	pr_info("-\n");
	return ret;
}

#ifdef CONFIG_FMEA_FAULT_INJECTION
static int stub_phy_power_on(void)
{
	if (pmic_read_reg(FAULT_INJECT_REG) == KERNEL_FAULT_USB_POWER)
		return -ENODEV;

	return 0;
}
#endif

int hiusb_usb2phy_init(struct phy *phy)
{
	int ret;

	pr_info("+\n");
	if (!phy)
		return 0;

	if (phy->power_count == 0) {
		ret = phy_power_on(phy);
#ifdef CONFIG_FMEA_FAULT_INJECTION
		ret = stub_phy_power_on();
#endif
		if (ret) {
			pr_err("usb2 phy poweron failed\n");
			return ret;
		}
	}

	ret = phy_init(phy);
	if (ret) {
		pr_err("init failed %d\n", ret);
		if (phy_power_off(phy))
			pr_err("usb2 phy poweroff failed\n");
	}

	pr_info("-\n");
	return ret;
}

int hiusb_usb2phy_setmode(struct phy *phy, enum phy_mode mode)
{
	int ret;

	if (!phy)
		return 0;

	ret = phy_set_mode(phy, mode);
	if (ret) {
		pr_err("set mode failed\n");
		if (phy_power_off(phy))
			pr_err("phy poweroff failed\n");
		return ret;
	}

	return ret;
}

int hiusb_usb2phy_exit(struct phy *phy)
{
	int ret;
	pr_info("+\n");

	if (!phy) {
		pr_info("no usb 2.0 phy\n");
		return 0;
	}
	ret = phy_exit(phy);
	if (ret) {
		pr_err("exit failed %d\n", ret);
		return ret;
	}

	ret = phy_power_off(phy);
	if (ret) {
		pr_err("usb2 phy poweroff failed\n");
		if (phy_init(phy))
			pr_err("usb2 phy init failed\n");
	}
	pr_info("-\n");

	return 0;
}

enum chip_charger_type hiusb_usb2phy_detect_charger_type(struct phy *phy)
{
	struct hiusb_usb2phy *usb2phy = NULL;
	pr_info("+\n");

	if (!phy)
		return 0;

	usb2phy = phy_get_drvdata(phy);
	if (usb2phy && usb2phy->detect_charger_type)
		return usb2phy->detect_charger_type(usb2phy);

	return 0;
}

int hiusb_usb2phy_set_dpdm_pulldown(struct phy *phy, bool pulldown)
{
	struct hiusb_usb2phy *usb2phy = NULL;

	if (!phy)
		return 0;

	usb2phy = phy_get_drvdata(phy);
	if (usb2phy && usb2phy->set_dpdm_pulldown)
		return usb2phy->set_dpdm_pulldown(usb2phy, pulldown);

	return 0;
}

int hiusb_usb2phy_set_vdp_src(struct phy *phy, bool enable)
{
	struct hiusb_usb2phy *usb2phy = NULL;

	if (!phy)
		return 0;

	usb2phy = phy_get_drvdata(phy);
	if (usb2phy && usb2phy->set_vdp_src)
		return usb2phy->set_vdp_src(usb2phy, enable);

	return 0;
}

int hiusb_usb2phy_suspend(struct phy *phy)
{
	struct hiusb_usb2phy *usb2phy = NULL;

	if (!phy)
		return 0;

	usb2phy = phy_get_drvdata(phy);
	if (usb2phy && usb2phy->phy_suspend)
		return usb2phy->phy_suspend(usb2phy);

	return 0;
}
int hiusb_usb2phy_resume(struct phy *phy)
{
	struct hiusb_usb2phy *usb2phy = NULL;

	if (!phy)
		return 0;

	usb2phy = phy_get_drvdata(phy);
	if (usb2phy && usb2phy->phy_resume)
		return usb2phy->phy_resume(usb2phy);

	return 0;
}

int hiusb_usb2phy_calibrate(struct phy *phy)
{
	int ret;

	pr_info("+\n");
	if (!phy)
		return 0;

	ret = phy_calibrate(phy);
	if (ret) {
		pr_err("usb2phy calibrate failed %d\n", ret);
		return ret;
	}
	pr_info("-\n");
	return 0;
}

