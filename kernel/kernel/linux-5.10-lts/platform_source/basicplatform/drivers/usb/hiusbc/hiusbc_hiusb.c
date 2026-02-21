/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Glue layer for hiusbc HIUSB
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


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/phy/phy.h>
#include <linux/pm_runtime.h>
#include <linux/extcon.h>
#include <linux/extcon-provider.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_drivers/usb/usb_reg_cfg.h>
#include <linux/platform_drivers/usb/hiusb_client.h>
#include <linux/dma-mapping.h>
#include <linux/platform_drivers/usb/usb_misc_ctrl.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include "../hiusb/core.h"

#ifdef CONFIG_FMEA_FAULT_INJECTION
#include <pmic_interface.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <debug_fault_injection.h>
#endif

#undef pr_fmt
#define pr_fmt(fmt) "[hiusbc_chip]%s: " fmt, __func__

struct hiusbc_chip {
	struct device *dev;
	struct device *hiusbc_dev;
	enum hiusb_port_usb_mode current_mode;
	int portid;
	void __iomem *regs;
	struct clk **clks;
	int num_clocks;
	u32 dma_mask_bit;

	u8 init_count;
	struct extcon_dev *edev;
	struct chip_usb_reg_cfg *reset;
	struct chip_usb_reg_cfg *unreset;
	struct chip_usb_reg_cfg *sel_self;
	struct chip_usb_reg_cfg *intr_usb_hifi_disable;
	struct chip_usb_reg_cfg **suspend_clk_config;
	int num_suspend_clk_config;
	struct chip_usb_reg_cfg **resume_clk_config;
	int num_resume_clk_config;
	struct chip_usb_reg_cfg **suspend_wakeup_mode;
	int num_suspend_wakeup_mode;
	struct chip_usb_reg_cfg **resume_wakeup_mode;
	int num_resume_wakeup_mode;
	struct hiusb_client *client;
};

static struct hiusb_client_desc hiusbc_chip_client_desc = {
	.type = HIUSB_CLIENT_TYPE_DEVICE | HIUSB_CLIENT_TYPE_HOST,
	.name = "hiusbc-drd",
	.owner = THIS_MODULE,
};

static const unsigned int usb_extcon_cable[] = {
	EXTCON_USB, EXTCON_USB_HOST, EXTCON_NONE
};

static int hiusbc_chip_start_controller(struct hiusbc_chip *hiusbc_chip, struct device *parent)
{
	struct device_node *node = hiusbc_chip->dev->of_node;
	struct platform_device *pdev = NULL;

	pr_info("+\n");
	if (parent == NULL || node == NULL)
		return -EINVAL;

	if (hiusbc_chip->init_count > 0)
		return 0;

	node = of_find_compatible_node(node, NULL, "hisilicon,hiusbc");
	if (node)
		pdev = of_platform_device_create(node, NULL, parent);

	if (!pdev) {
		pr_err("create usbc dev failed!\n");
		return -ENODEV;
	}

	hiusbc_chip->hiusbc_dev = &pdev->dev;
	hiusbc_chip->init_count++;
	pr_info("-\n");
	return 0;
}

static int hiusbc_chip_stop_controller(struct hiusbc_chip *hiusbc_chip)
{
	int ret;

	pr_info("+\n");
	ret = of_platform_device_destroy(hiusbc_chip->hiusbc_dev, NULL);
	if (ret)
		pr_err("hiusbc destroy error (ret %d)\n", ret);

	hiusbc_chip->hiusbc_dev = NULL;
	hiusbc_chip->init_count = 0;
	pr_info("-\n");
	return ret;
}

#ifdef CONFIG_FMEA_FAULT_INJECTION
static int stub_usb_init_clks(void)
{
	if (pmic_read_reg(FAULT_INJECT_REG) == KERNEL_FAULT_USB_CLK)
		return -ENODEV;

	return 0;
}
#endif

static int hiusbc_chip_get_portid(struct hiusb_client *client)
{
	struct hiusbc_chip *hiusbc_chip = NULL;

	if (!client)
		return -EINVAL;

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	return hiusbc_chip->portid;
}

static int hiusbc_enhance_utmi_clk_if_necessary(struct hiusb_client *client) 
{
	int ret;
	struct hiusb_usb2phy *usb2phy = NULL;
	struct hiusb_port *port = NULL;

	if ((IS_ERR_OR_NULL(client)) || (client->port == NULL))
		return -EINVAL;

	port = client->port;
	if (port->cur_client != client)
		return -EBUSY;
	
	if(port->usb2_phy)
		usb2phy = phy_get_drvdata(port->usb2_phy);

	if (usb2phy && usb2phy->phy_enhance) {
		pr_info("phy enhance process!!!\n");
		ret = usb2phy->phy_enhance(usb2phy);
		return ret;
	}
	return 0;	
}

static int hiusbc_chip_init_hw(struct hiusb_client *client, struct hiusbc_chip *hiusbc_chip)
{
	int ret;
#ifdef CONFIG_TASK_COPROCESSOR
	int portid;
#endif
	pr_err("+\n");

	ret = hiusb_client_init_usb2(client);
	if (ret) {
		pr_err("client init usb2 fail %d\n", ret);
		return ret;
	}
	pr_err("hiusb_client_init_usb2 end\n");
	ret = hiusb_client_init_usb3(client);
	if (ret) {
		pr_err("client init usb3 fail %d\n", ret);
		goto init_usb3_fail;
	}
	pr_err("hiusb_client_init_usb3 end\n");
#ifdef CONFIG_TASK_COPROCESSOR
	portid = hiusbc_chip_get_portid(client);
	if (portid >= 0)
		tcp_set_port_irq_status(portid, true);
#endif
	ret = chip_usb_init_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);
#ifdef CONFIG_FMEA_FAULT_INJECTION
	ret = stub_usb_init_clks();
#endif
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		goto init_clks_fail;
	}

	pr_err("chip_usb_init_clks end\n");

	if (hiusbc_chip->intr_usb_hifi_disable) {
		ret = chip_usb_reg_write(hiusbc_chip->intr_usb_hifi_disable);
		if (ret)
			pr_err("intr_usb_hifi_disable failed %d\n", ret);
	}

	if (hiusbc_chip->unreset) {
		ret = chip_usb_reg_write(hiusbc_chip->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}
	msleep(10);
	/* add utmi clk enhance process [begin] */
	ret = hiusbc_enhance_utmi_clk_if_necessary(client);
	if (ret) {
		pr_err("enhance utmi failed %d, will abort usb enum!!!\n", ret);
		goto reset_fail;
	}
	/* add utmi clk enhance process [end] */

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_ON);
	pr_err("-\n");
	return ret;
reset_fail:
	chip_usb_shutdown_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);
init_clks_fail:
	hiusb_client_deinit_usb3(client);
init_usb3_fail:
	hiusb_client_deinit_usb2(client, 0);
	return ret;
}

static void hiusbc_chip_deinit_hw(struct hiusb_client *client, struct hiusbc_chip *hiusbc_chip)
{
	int ret;
#ifdef CONFIG_TASK_COPROCESSOR
	int portid;
#endif

	pr_info("+\n");
	hiusb_client_set_port_power_flag(client, HIUSB_POWER_OFF);
#ifdef CONFIG_TASK_COPROCESSOR
	portid = hiusbc_chip_get_portid(client);
	if (portid >= 0) {
		tcp_set_port_irq_status(portid, false);
		mdelay(10); /* wait tcp interrut handler finished */
	}
#endif
	if (hiusbc_chip->reset) {
		ret = chip_usb_reg_write(hiusbc_chip->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	chip_usb_shutdown_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);

	ret = hiusb_client_deinit_usb3(client);
	if (ret)
		pr_err("deinit usb3 fail %d\n", ret);

	ret = hiusb_client_deinit_usb2(client, 0);
	if (ret)
		pr_err("deinit usb2 fail %d\n", ret);
	pr_info("-\n");
}

static int hiusbc_chip_port_device_init(struct hiusb_client *client,
	struct hiusbc_chip *hiusbc_chip, struct device *parent)
{
	int ret;
	pr_err("+\n");

	ret = hiusbc_chip_init_hw(client, hiusbc_chip);
	if (ret) {
		pr_err("device init hw fail\n");
		return ret;
	}

	ret = hiusb_client_detect_charger_type(client);
	if (ret)
		pr_err("hiusb_client_detect_charger_type fail %d\n", ret);

	ret = hiusbc_chip_start_controller(hiusbc_chip, parent);
	if (ret) {
		pr_err("start hiusbc controller fail\n");
		goto start_usbc_fail;
	}

	if (!hiusb_client_enumerate_allowed(client)) {
		pr_info("not allow enum\n");
		return 0;
	}

	ret = hiusb_client_entry_device_mode(client);
	if (ret) {
		pr_err("hiusb_client_entry_device_mode fail\n");
		goto entry_device_fail;
	}

	/* start device */
	ret = extcon_set_state_sync(hiusbc_chip->edev,
			EXTCON_USB, true);
	if (ret) {
		pr_err("start device error\n");
		goto extcon_set_state_fail;
	}

	pr_err("-\n");
	return 0;

extcon_set_state_fail:
	hiusb_client_exit_device_mode(client);
entry_device_fail:
	hiusbc_chip_stop_controller(hiusbc_chip);
start_usbc_fail:
	hiusbc_chip_deinit_hw(client, hiusbc_chip);
	return ret;
}

static int hiusbc_chip_port_assign(struct hiusb_client *client, enum hiusb_port_usb_mode mode, struct device *parent)
{
	struct hiusbc_chip *hiusbc_chip = NULL;
	int ret;

	pr_err("+\n");
	if (!client)
		return -EINVAL;

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	pr_info("mode %d\n",mode);

	if (mode == HIUSB_PORT_USB_MODE_HOST) {
		ret = hiusbc_chip_init_hw(client, hiusbc_chip);
		if (ret) {
			pr_err("host init hw fail %d\n", ret);
			return ret;
		}
		ret = hiusbc_chip_start_controller(hiusbc_chip, parent);
		if (ret) {
			pr_err("start hiusbc controller fail %d\n", ret);
			return ret;
		}

		/* start host */
		ret = extcon_set_state_sync(hiusbc_chip->edev,
				EXTCON_USB_HOST, true);
		if (ret) {
			pr_err("start host error\n");
			goto extcon_set_state_fail;
		}
	} else if (mode == HIUSB_PORT_USB_MODE_DEVICE) {
		ret = hiusbc_chip_port_device_init(client, hiusbc_chip, parent);
		if (ret)
			return ret;
	} else {
		pr_err("mode %d error\n", mode);
		return -EINVAL;
	}
	hiusbc_chip->current_mode = mode;
	pr_err("-\n");
	return 0;
extcon_set_state_fail:
	hiusbc_chip_stop_controller(hiusbc_chip);
	hiusbc_chip_deinit_hw(client, hiusbc_chip);
	return ret;
}

static int hiusbc_chip_port_deassign(struct hiusb_client *client)
{
	int ret = 0;
	struct hiusbc_chip *hiusbc_chip = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	if (hiusbc_chip->current_mode == HIUSB_PORT_USB_MODE_HOST) {
		/* stop host */
		ret = extcon_set_state_sync(hiusbc_chip->edev,
				EXTCON_USB_HOST, false);
		if (ret)
			pr_err("stop host error\n");
	} else {
		if (hiusb_client_is_device_mode(client)) {
			/* stop device */
			ret = extcon_set_state_sync(hiusbc_chip->edev,
					EXTCON_USB, false);
			if (ret)
				pr_err("stop device error\n");
			hiusb_client_exit_device_mode(client);
		}
	}

	hiusb_client_handle_no_charger(client);
	hiusbc_chip->current_mode = HIUSB_PORT_USB_MODE_NONE;

	hiusbc_chip_deinit_hw(client, hiusbc_chip);
	pr_info("-\n");
	return ret;
}

static int hiusbc_chip_port_suspend(struct hiusb_client *client, bool remote_wakup)
{
	int ret = 0;
	struct hiusbc_chip *hiusbc_chip = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+ keep_remote_wakup:%d\n", (int)remote_wakup);

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	if (remote_wakup) {
		/* wakeup irq mask */
		ret = chip_usb_reg_write_array(hiusbc_chip->suspend_wakeup_mode,
					hiusbc_chip->num_suspend_wakeup_mode);
		if (ret)
			pr_err("config wakeup mode  fail %d\n", ret);

		/* switch suspend clk */
		ret = chip_usb_reg_write_array(hiusbc_chip->suspend_clk_config, hiusbc_chip->num_suspend_clk_config);
		if (ret)
			pr_err("config suspend clk  fail %d\n", ret);

		/* close usbc clk */
		chip_usb_suspend_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);
		pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
		return ret;
	}

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_OFF);

	if (hiusbc_chip->reset) {
		ret = chip_usb_reg_write(hiusbc_chip->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	chip_usb_shutdown_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);

	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;
}

static int hiusbc_chip_port_resume(struct hiusb_client *client, bool remote_wakup)
{
	int ret = 0;
	struct hiusbc_chip *hiusbc_chip = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+ keep_remote_wakup:%d\n", (int)remote_wakup);

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	if (remote_wakup) {
		chip_usb_resume_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);

		/* switch suspend clk */
		ret = chip_usb_reg_write_array(hiusbc_chip->resume_clk_config, hiusbc_chip->num_resume_clk_config);
		if (ret)
			pr_err("config resume clk  fail %d\n", ret);
		/* wakeup irq mask */
		ret = chip_usb_reg_write_array(hiusbc_chip->resume_wakeup_mode,
					hiusbc_chip->num_resume_wakeup_mode);
		if (ret)
			pr_err("config wakeup mode  fail %d\n", ret);
		
		pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
		return ret;
	}

	ret = chip_usb_init_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		return ret;
	}

	if (hiusbc_chip->unreset) {
		ret = chip_usb_reg_write(hiusbc_chip->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_ON);

	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;

reset_fail:
	chip_usb_shutdown_clks(hiusbc_chip->clks, hiusbc_chip->num_clocks);
	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;
}

static int hiusbc_chip_port_bind(struct hiusb_client *client)
{
	return 0;
}

static int hiusbc_chip_port_unbind(struct hiusb_client *client)
{
	return 0;
}

static int hiusbc_chip_port_ioctrl(struct hiusb_client *client,
	enum hiusb_port_client_operation operation_code, void* data)
{
	int ret = -ESRCH;
	struct hiusbc_chip *hiusbc_chip = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusbc_chip = (struct hiusbc_chip *)hiusb_client_get_drvdata(client);
	if (hiusbc_chip == NULL)
		return -ENODEV;

	if (hiusbc_chip->current_mode == HIUSB_PORT_USB_MODE_NONE)
		return -ENODEV;

	switch (operation_code) {
	case HIUSB_CLIENT_IOCTRL_STOP_PERIPHERIAL:
		/* stop device */
		ret = extcon_set_state_sync(hiusbc_chip->edev,
				EXTCON_USB, false);
		if (ret)
			pr_err("stop device error\n");
		break;
	case HIUSB_CLIENT_IOCTRL_START_PERIPHERIAL:
		ret = hiusb_client_entry_device_mode(client);
		if (ret)
			pr_err("already in device mode\n");

		/* start device */
		ret = extcon_set_state_sync(hiusbc_chip->edev,
				EXTCON_USB, true);
		if (ret)
			pr_err("start device error\n");
		break;
	default:
		pr_info("unsupport operation_code %d\n", operation_code);
		break;
	}

	pr_info("-\n");
	return ret;
}

static struct hiusb_client_ops hiusbc_chip_client_ops = {
	.bind = hiusbc_chip_port_bind,
	.unbind = hiusbc_chip_port_unbind,
	.assign = hiusbc_chip_port_assign,
	.deassign = hiusbc_chip_port_deassign,
	.suspend = hiusbc_chip_port_suspend,
	.resume = hiusbc_chip_port_resume,
	.get_portid = hiusbc_chip_get_portid,
	.ioctl = hiusbc_chip_port_ioctrl,
};
static void hiusbc_chip_dma_mask_config(struct hiusbc_chip *hiusbc_chip)
{
	struct device *dev = hiusbc_chip->dev;

	hiusbc_chip->dma_mask_bit = hiusbc_chip->dma_mask_bit > 64 ?
			64 : hiusbc_chip->dma_mask_bit; /* 64-bit machine */
	dev->coherent_dma_mask = DMA_BIT_MASK(hiusbc_chip->dma_mask_bit);
	dev->dma_mask = &dev->coherent_dma_mask;
}

static void hiusbc_chip_get_sr_resource(struct platform_device *pdev, struct hiusbc_chip *hiusbc_chip)
{
	struct device *dev = &pdev->dev;

	if (get_chip_usb_reg_cfg_array(dev, "suspend_wakeup_mode",
				       &hiusbc_chip->suspend_wakeup_mode,
				       &hiusbc_chip->num_suspend_wakeup_mode)) {
		pr_info("no suspend_wakeup_mode\n");
		hiusbc_chip->suspend_wakeup_mode = NULL;
		hiusbc_chip->num_suspend_wakeup_mode = 0;
	}

	if (get_chip_usb_reg_cfg_array(dev, "resume_wakeup_mode",
				       &hiusbc_chip->resume_wakeup_mode,
				       &hiusbc_chip->num_resume_wakeup_mode)) {
		pr_info("no resume_wakeup_mode\n");
		hiusbc_chip->resume_wakeup_mode = NULL;
		hiusbc_chip->num_resume_wakeup_mode = 0;
	}

	if (get_chip_usb_reg_cfg_array(dev, "suspend_clk_config", &hiusbc_chip->suspend_clk_config, 
			&hiusbc_chip->num_suspend_clk_config)) {
		pr_info("someplate no need suspend_clk_config reg\n");
		hiusbc_chip->suspend_clk_config = NULL;
		hiusbc_chip->num_suspend_clk_config = 0;
	}

	if (get_chip_usb_reg_cfg_array(dev, "resume_clk_config", &hiusbc_chip->resume_clk_config,
			&hiusbc_chip->num_resume_clk_config)) {
		pr_info("someplate no need resume_clk_config reg\n");
		hiusbc_chip->resume_clk_config = NULL;
		hiusbc_chip->num_resume_clk_config = 0;
	}
}

static int hiusbc_chip_get_resource(struct platform_device *pdev, struct hiusbc_chip *hiusbc_chip)
{
	int ret;
	struct device *dev = &pdev->dev;

	ret = of_property_read_u32(dev->of_node, "portid", &(hiusbc_chip->portid));
	if (ret) {
		pr_err("get portid failed %d\n", ret);
		return -ENODEV;
	}

	pr_info("portid %d\n", hiusbc_chip->portid);

	if (of_property_read_u32(dev->of_node, "dma_mask_bit",
				&(hiusbc_chip->dma_mask_bit)))
		hiusbc_chip->dma_mask_bit = 32; /* DMA MASK */

	pr_info("dma_mask_bit %d\n", hiusbc_chip->dma_mask_bit);

	ret = devm_chip_usb_get_clks(dev, &hiusbc_chip->clks, &hiusbc_chip->num_clocks);
	if (ret) {
		pr_err("get controller clks failed %d\n", ret);
		return -ENODEV;
	}

	hiusbc_chip->reset = of_get_chip_usb_reg_cfg(dev->of_node, "reset");
	if (!hiusbc_chip->reset) {
		pr_err("controller reset fail\n");
		return -ENODEV;
	}

	hiusbc_chip->unreset = of_get_chip_usb_reg_cfg(dev->of_node, "unreset");
	if (!hiusbc_chip->unreset) {
		ret = -ENODEV;
		pr_err("controller unreset fail\n");
		goto unreset_fail;
	}

	hiusbc_chip->intr_usb_hifi_disable = of_get_chip_usb_reg_cfg(dev->of_node, "intr_usb_hifi_disable");
	if (!hiusbc_chip->intr_usb_hifi_disable)
		pr_info("hiusbc doesn't support intr_usb_hifi_disable\n");

	hiusbc_chip->sel_self = of_get_chip_usb_reg_cfg(dev->of_node, "sel-self");
	if (!hiusbc_chip->sel_self)
		pr_info("only fpga need sel-self\n");

	hiusbc_chip_get_sr_resource(pdev, hiusbc_chip);

	return 0;
unreset_fail:
	of_remove_chip_usb_reg_cfg(hiusbc_chip->reset);
	return ret;
}

static int hiusbc_chip_put_resource(struct platform_device *pdev)
{
	struct hiusbc_chip *hiusbc_chip = platform_get_drvdata(pdev);

	of_remove_chip_usb_reg_cfg(hiusbc_chip->reset);
	of_remove_chip_usb_reg_cfg(hiusbc_chip->unreset);
	if (hiusbc_chip->intr_usb_hifi_disable)
		of_remove_chip_usb_reg_cfg(hiusbc_chip->intr_usb_hifi_disable);

	if (hiusbc_chip->sel_self)
		of_remove_chip_usb_reg_cfg(hiusbc_chip->sel_self);

	if (hiusbc_chip->suspend_clk_config)
		free_chip_usb_reg_cfg_array(hiusbc_chip->suspend_clk_config,
				hiusbc_chip->num_suspend_clk_config);

	if (hiusbc_chip->resume_clk_config)
		free_chip_usb_reg_cfg_array(hiusbc_chip->resume_clk_config,
				hiusbc_chip->num_resume_clk_config);

	if (hiusbc_chip->suspend_wakeup_mode)
		free_chip_usb_reg_cfg_array(hiusbc_chip->suspend_wakeup_mode,
				hiusbc_chip->num_suspend_wakeup_mode);

	if (hiusbc_chip->resume_wakeup_mode)
		free_chip_usb_reg_cfg_array(hiusbc_chip->resume_wakeup_mode,
				hiusbc_chip->num_resume_wakeup_mode);

	return 0;
}

static int hiusbc_chip_probe(struct platform_device *pdev)
{
	int ret = -ENOMEM;
	struct hiusbc_chip *hiusbc_chip = NULL;
	struct hiusb_client *client = NULL;
	struct device *dev = &pdev->dev;

	pr_info("+\n");
	client = hiusb_client_create(dev, &hiusbc_chip_client_desc, &hiusbc_chip_client_ops);
	if (IS_ERR_OR_NULL(client))
		return -EPROBE_DEFER;

	hiusbc_chip = devm_kzalloc(dev, sizeof(*hiusbc_chip), GFP_KERNEL);
	if (!hiusbc_chip)
		goto destroy_client;

	ret = hiusbc_chip_get_resource(pdev, hiusbc_chip);
	if (ret)
		goto destroy_client;

	hiusbc_chip->client = client;
	hiusbc_chip->dev = dev;

	hiusbc_chip_dma_mask_config(hiusbc_chip);
	hiusb_client_set_drvdata(client, hiusbc_chip);

	hiusbc_chip->edev = devm_extcon_dev_allocate(dev, usb_extcon_cable);
	if (IS_ERR(hiusbc_chip->edev)) {
		pr_err("failed to allocate extcon device\n");
		ret = PTR_ERR(hiusbc_chip->edev);
		goto put_resource;
	}

	ret = devm_extcon_dev_register(dev, hiusbc_chip->edev);
	if (ret < 0) {
		pr_err("failed to register extcon device\n");
		goto put_resource;
	}
	hiusbc_chip->current_mode = HIUSB_PORT_USB_MODE_NONE;

	platform_set_drvdata(pdev, hiusbc_chip);

	ret = hiusb_runtime_enable(dev);
	if (ret < 0)
		pr_err("hiusb_runtime_enable failed %d\n", ret);

	pr_info("-\n");
	return ret;
put_resource:
	hiusbc_chip_put_resource(pdev);
destroy_client:
	hiusb_client_destroy(client);
	return ret;
}

static int hiusbc_chip_remove(struct platform_device *pdev)
{
	int ret;
	struct hiusbc_chip *hiusbc_chip = platform_get_drvdata(pdev);

	if (!hiusbc_chip) {
		pr_err("hiusbc_chip NULL\n");
		return -ENODEV;
	}

	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	ret = hiusbc_chip_put_resource(pdev);
	if (ret)
		return ret;

	hiusb_client_destroy(hiusbc_chip->client);

	return ret;
}

static const struct of_device_id of_hiusbc_chip_match[] = {
	{ .compatible = "hiusb,hiusbc" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_hiusbc_chip_match);

static struct platform_driver hiusbc_chip_driver = {
	.probe		= hiusbc_chip_probe,
	.remove		= hiusbc_chip_remove,
	.driver		= {
		.name	= "hiusbc-hiusb",
		.of_match_table = of_hiusbc_chip_match,
	},
};

module_platform_driver(hiusbc_chip_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hiusbc Layer of HIUSB");
