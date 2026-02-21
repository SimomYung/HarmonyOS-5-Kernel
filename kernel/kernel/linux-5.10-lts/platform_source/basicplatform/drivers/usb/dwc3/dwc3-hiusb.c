/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Glue layer for dwc3 HIUSB
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
#include "dwc3_hook.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/extcon.h>
#include <linux/extcon-provider.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_drivers/usb/usb_reg_cfg.h>
#include <linux/platform_drivers/usb/hiusb_client.h>
#include <linux/dma-mapping.h>
#include <linux/platform_drivers/usb/usb_misc_ctrl.h>
#include <linux/platform_drivers/usb/hiusb.h>

#include "core.h"
#include "io.h"

#undef pr_fmt
#define pr_fmt(fmt) "[DWC3_HIUSB]%s: " fmt, __func__

#define DWC3_GLOBALS_REGS_START		0xc100

struct dwc3_hiusb {
	struct device		*dev;
	struct device		*dwc3_dev;
	enum hiusb_port_usb_mode current_mode;
	int portid;
	void __iomem		*regs;
	struct clk		**clks;
	int			num_clocks;
	u32	dma_mask_bit;
	int init_times;
	struct extcon_dev	*edev;
	struct chip_usb_reg_cfg	*reset;
	struct chip_usb_reg_cfg	*unreset;
	struct chip_usb_reg_cfg	*sel_self;
	struct chip_usb_reg_cfg	*intr_usb_hifi_disable;
	struct hiusb_client	*client;
};

static struct hiusb_client_desc dwc3_hiusb_client_desc = {
	.type = HIUSB_CLIENT_TYPE_DEVICE | HIUSB_CLIENT_TYPE_HOST,
	.name = "hidwc3-drd",
	.owner = THIS_MODULE,
};

static const unsigned int usb_extcon_cable[] = {
	EXTCON_USB, EXTCON_USB_HOST, EXTCON_NONE
};

static void dwc3_hiusb_disable_pipe_clock(struct dwc3_hiusb *hiusb_dwc)
{
	u32 value;
	struct dwc3 *dwc3 = NULL;

	if ((hiusb_dwc == NULL) || (hiusb_dwc->dwc3_dev == NULL)) {
		pr_err("disable pipe clock failed, para null!\n");
		return;
	}

	dwc3 = dev_get_drvdata(hiusb_dwc->dwc3_dev);
	if (dwc3 == NULL) {
		pr_err("disable pipe clock failed, dwc3 null!\n");
		return;
	}

	value = dwc3_readl(dwc3->regs, DWC3_GUCTL1);
	value |= DWC3_GUCTL1_DISREFCLKGTNG_BIT | DWC3_GUCTL1_RESERVED26;
	dwc3_writel(dwc3->regs, DWC3_GUCTL1, value);
}

static int dwc3_hiusb_start_controller(struct dwc3_hiusb *hiusb_dwc, struct device *parent)
{
	struct device_node *node = hiusb_dwc->dev->of_node;
	struct platform_device *pdev = NULL;

	pr_info("+\n");
	if (parent == NULL)
		return -EINVAL;

	if (node) {
		node = of_find_compatible_node(node, NULL, "snps,dwc3");
		if (node)
			pdev = of_platform_device_create(node, NULL, parent);
	}

	if (!pdev) {
		pr_err("create usbc dev failed!\n");
		return -ENODEV;
	}

	hiusb_dwc->dwc3_dev = &pdev->dev;
	pr_info("-\n");
	return 0;
}

static int dwc3_hiusb_stop_controller(struct dwc3_hiusb *hiusb_dwc)
{
	int ret;
	pr_info("+\n");

	ret = of_platform_device_destroy(hiusb_dwc->dwc3_dev, NULL);
	if (ret)
		pr_err("dwc3 destroy error (ret %d)\n", ret);

	hiusb_dwc->dwc3_dev = NULL;
	pr_info("-\n");
	return ret;
}

static int dwc3_hiusb_init_hw(struct hiusb_client *client, struct dwc3_hiusb *hiusb_dwc)
{
	int ret;
	pr_info("+\n");

	ret = hiusb_client_init_usb2(client);
	if (ret) {
		pr_err("client init usb2 fail %d\n", ret);
		return ret;
	}

	ret = hiusb_client_init_usb3(client);
	if (ret){
		pr_err("client init usb3 fail %d\n", ret);
		goto init_usb3_fail;
	}

	ret = chip_usb_init_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		goto init_clks_fail;
	}

	if (hiusb_dwc->intr_usb_hifi_disable) {
		ret = chip_usb_reg_write(hiusb_dwc->intr_usb_hifi_disable);
		if (ret)
			pr_err("intr_usb_hifi_disable failed %d\n", ret);
	}

	if (hiusb_dwc->unreset) {
		ret = chip_usb_reg_write(hiusb_dwc->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}
	msleep(10);
	hiusb_client_set_port_power_flag(client, HIUSB_POWER_ON);

	if (hiusb_client_is_usb3_phy(client) &&
	    hiusb_client_is_dp_mode(client))
		dwc3_hiusb_disable_pipe_clock(hiusb_dwc);

	pr_info("-\n");
	return ret;
reset_fail:
	chip_usb_shutdown_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);
init_clks_fail:
	hiusb_client_deinit_usb3(client);
init_usb3_fail:
	hiusb_client_deinit_usb2(client, 0);
	return ret;
}

void dwc3_hiusb_deinit_hw(struct hiusb_client *client, struct dwc3_hiusb *hiusb_dwc)
{
	int ret;
	pr_info("+\n");
	hiusb_client_set_port_power_flag(client, HIUSB_POWER_OFF);

	if (hiusb_dwc->reset) {
		ret = chip_usb_reg_write(hiusb_dwc->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	chip_usb_shutdown_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);

	ret = hiusb_client_deinit_usb3(client);
	if (ret)
		pr_err("deinit usb3 fail %d\n", ret);

	ret = hiusb_client_deinit_usb2(client, 0);
	if (ret)
		pr_err("deinit usb2 fail %d\n", ret);
	pr_info("-\n");
}

static int init_hw_first_time(struct hiusb_client *client, struct dwc3_hiusb *hiusb_dwc, struct device *parent)
{
	int ret;
	pr_info("+\n");
	if(hiusb_dwc->init_times == 0) {
		ret = dwc3_hiusb_init_hw(client, hiusb_dwc);
		if (ret)
			return ret;

		ret = dwc3_hiusb_start_controller(hiusb_dwc, parent);
		if (ret)
			goto start_controller_fail;

		dwc3_hiusb_deinit_hw(client, hiusb_dwc);
		hiusb_dwc->init_times++;
	}
	return 0;

start_controller_fail:
	dwc3_hiusb_deinit_hw(client, hiusb_dwc);
	return ret;
}
static int dwc3_hiusb_port_device_init(struct hiusb_client *client, struct dwc3_hiusb *hiusb_dwc, struct device *parent)
{
	int ret;
	pr_info("+\n");

	ret = init_hw_first_time(client, hiusb_dwc, parent);
	if (ret) {
		pr_err("init hw first time fail\n");
		return ret;
	}
	ret = dwc3_hiusb_init_hw(client, hiusb_dwc);
	if (ret) {
		pr_err("device init hw fail\n");
		return ret;
	}

	ret = hiusb_client_detect_charger_type(client);
	if (ret)
		pr_err("hiusb_client_detect_charger_type fail %d\n", ret);

	if(!hiusb_client_enumerate_allowed(client)) {
		pr_info("not allow enum\n");
		return 0;
	}

	ret = hiusb_client_entry_device_mode(client);
	if(ret){
		goto entry_device_fail;
	}

	/* start device */
	ret = extcon_set_state_sync(hiusb_dwc->edev,
			EXTCON_USB, true);
	if (ret) {
		pr_err("start device error\n");
		goto extcon_set_state_fail;
	}

	pr_info("-\n");
	return 0;

extcon_set_state_fail:
	hiusb_client_exit_device_mode(client);
entry_device_fail:
	dwc3_hiusb_stop_controller(hiusb_dwc);
	dwc3_hiusb_deinit_hw(client, hiusb_dwc);
	hiusb_dwc->init_times= 0;
	return ret;
}

static int dwc3_hiusb_port_assign(struct hiusb_client *client, enum hiusb_port_usb_mode mode, struct device *parent)
{
	struct dwc3_hiusb *hiusb_dwc = NULL;
	int ret;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	pr_info("mode %d\n",mode);

	if (mode == HIUSB_PORT_USB_MODE_HOST) {
		ret = init_hw_first_time(client, hiusb_dwc, parent);
		if (ret) {
			pr_err("host init hw first time fail\n");
			return ret;
		}
		ret = dwc3_hiusb_init_hw(client, hiusb_dwc);
		if (ret) {
			pr_err("host init hw fail\n");
			return ret;
		}
		/* start host */
		ret = extcon_set_state_sync(hiusb_dwc->edev,
				EXTCON_USB_HOST, true);
		if (ret) {
			pr_err("start host error\n");
			goto extcon_set_state_fail;
		}
	} else if (mode == HIUSB_PORT_USB_MODE_DEVICE) {
		ret = dwc3_hiusb_port_device_init(client, hiusb_dwc, parent);
		if (ret)
			return ret;
	} else {
		pr_err("mode %d error\n", mode);
		return -EINVAL;
	}
	hiusb_dwc->current_mode = mode;
	pr_info("-\n");
	return 0;
extcon_set_state_fail:
	dwc3_hiusb_stop_controller(hiusb_dwc);
	dwc3_hiusb_deinit_hw(client, hiusb_dwc);
	hiusb_dwc->init_times= 0;
	return ret;
}

static int dwc3_hiusb_port_deassign(struct hiusb_client *client)
{
	int ret = 0;
	struct dwc3_hiusb *hiusb_dwc = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	if(hiusb_dwc->current_mode == HIUSB_PORT_USB_MODE_HOST){
		/* stop host */
		ret = extcon_set_state_sync(hiusb_dwc->edev,
				EXTCON_USB_HOST, false);
		if (ret)
			pr_err("stop host error\n");
	} else {
		if(hiusb_client_is_device_mode(client)) {
			/* stop device */
			ret = extcon_set_state_sync(hiusb_dwc->edev,
					EXTCON_USB, false);
			if (ret)
				pr_err("stop device error\n");
			hiusb_client_exit_device_mode(client);
		}
	}

	hiusb_client_handle_no_charger(client);
	hiusb_dwc->current_mode = HIUSB_PORT_USB_MODE_NONE;

	dwc3_hiusb_deinit_hw(client, hiusb_dwc);
	pr_info("-\n");
	return ret;
}

static int dwc3_hiusb_port_suspend(struct hiusb_client *client, bool remote_wakeup)
{
	int ret = 0;
	struct dwc3_hiusb *hiusb_dwc = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_OFF);

	if (hiusb_dwc->reset) {
		ret = chip_usb_reg_write(hiusb_dwc->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	chip_usb_shutdown_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);

	pr_info("-\n");
	return ret;
}

static int dwc3_hiusb_port_resume(struct hiusb_client *client, bool remote_wakeup)
{
	int ret = 0;
	struct dwc3_hiusb *hiusb_dwc = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	ret = chip_usb_init_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		return ret;
	}

	if (hiusb_dwc->unreset) {
		ret = chip_usb_reg_write(hiusb_dwc->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_ON);

	pr_info("-\n");
	return ret;

reset_fail:
	chip_usb_shutdown_clks(hiusb_dwc->clks, hiusb_dwc->num_clocks);
	return ret;
}

static int dwc3_hiusb_port_bind(struct hiusb_client *client)
{
	return 0;
}

static int dwc3_hiusb_port_unbind(struct hiusb_client *client)
{
	return 0;
}

static int dwc3_hiusb_get_portid(struct hiusb_client *client)
{
	struct dwc3_hiusb *hiusb_dwc = NULL;

	if (!client)
		return -EINVAL;

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	return hiusb_dwc->portid;
}

static int dwc3_hiusb_port_ioctrl(struct hiusb_client *client,
	enum hiusb_port_client_operation operation_code, void* data)
{
	int ret = -ESRCH;
	struct dwc3_hiusb *hiusb_dwc = NULL;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusb_dwc = (struct dwc3_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_dwc == NULL)
		return -ENODEV;

	if(hiusb_dwc->current_mode == HIUSB_PORT_USB_MODE_NONE)
		return -ENODEV;

	switch (operation_code) {
	case HIUSB_CLIENT_IOCTRL_STOP_PERIPHERIAL:
		/* stop device */
		ret = extcon_set_state_sync(hiusb_dwc->edev,
				EXTCON_USB, false);
		if (ret)
			pr_err("stop device error\n");
		break;
	case HIUSB_CLIENT_IOCTRL_START_PERIPHERIAL:
		ret = hiusb_client_entry_device_mode(client);
		if (ret)
			pr_err("already in device mode\n");

		/* start device */
		ret = extcon_set_state_sync(hiusb_dwc->edev,
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

static struct hiusb_client_ops dwc3_hiusb_client_ops = {
	.bind = dwc3_hiusb_port_bind,
	.unbind = dwc3_hiusb_port_unbind,
	.assign = dwc3_hiusb_port_assign,
	.deassign = dwc3_hiusb_port_deassign,
	.suspend = dwc3_hiusb_port_suspend,
	.resume = dwc3_hiusb_port_resume,
	.get_portid = dwc3_hiusb_get_portid,
	.ioctl = dwc3_hiusb_port_ioctrl,
};
static void dwc3_hiusb_dma_mask_config(struct dwc3_hiusb *hiusb_dwc)
{
	struct device *dev = hiusb_dwc->dev;

	hiusb_dwc->dma_mask_bit = hiusb_dwc->dma_mask_bit > 64 ?
			64 : hiusb_dwc->dma_mask_bit; /* 64-bit machine */
	dev->coherent_dma_mask = DMA_BIT_MASK(hiusb_dwc->dma_mask_bit);
	dev->dma_mask = &dev->coherent_dma_mask;
}

static int dwc3_hiusb_get_resource(struct platform_device *pdev, struct dwc3_hiusb *hiusb_dwc)
{
	int ret;
	struct device *dev = &pdev->dev;

	ret = of_property_read_u32(dev->of_node, "portid",
				&(hiusb_dwc->portid));
		if (ret) {
			pr_err("get portid failed %d\n", ret);
			return -ENODEV;
		}

	pr_info("portid %d\n", hiusb_dwc->portid);

	if (of_property_read_u32(dev->of_node, "dma_mask_bit",
				&(hiusb_dwc->dma_mask_bit)))
		hiusb_dwc->dma_mask_bit = 32; /* DMA MASK */

	pr_info("dma_mask_bit %d\n", hiusb_dwc->dma_mask_bit);

	ret = devm_chip_usb_get_clks(dev, &hiusb_dwc->clks, &hiusb_dwc->num_clocks);
	if (ret) {
		pr_err("get controller clks failed %d\n", ret);
		return -ENODEV;
	}

	hiusb_dwc->reset = of_get_chip_usb_reg_cfg(dev->of_node, "reset");
	if (!hiusb_dwc->reset) {
		pr_err("controller reset fail\n");
		return -ENODEV;
	}

	hiusb_dwc->unreset = of_get_chip_usb_reg_cfg(dev->of_node, "unreset");
	if (!hiusb_dwc->unreset) {
		ret = -ENODEV;
		pr_err("controller unreset fail\n");
		goto unreset_fail;
	}

	hiusb_dwc->intr_usb_hifi_disable = of_get_chip_usb_reg_cfg(dev->of_node, "intr_usb_hifi_disable");
	if (!hiusb_dwc->intr_usb_hifi_disable)
		pr_err("controller intr_usb_hifi_disable fail\n");

	return 0;
unreset_fail:
	of_remove_chip_usb_reg_cfg(hiusb_dwc->reset);
	return ret;
}

static int dwc3_hiusb_put_resource(struct platform_device *pdev)
{
	struct dwc3_hiusb *hiusb_dwc = platform_get_drvdata(pdev);

	of_remove_chip_usb_reg_cfg(hiusb_dwc->reset);
	of_remove_chip_usb_reg_cfg(hiusb_dwc->unreset);
	if (hiusb_dwc->intr_usb_hifi_disable)
		of_remove_chip_usb_reg_cfg(hiusb_dwc->intr_usb_hifi_disable);
	return 0;
}

static int dwc3_hiusb_probe(struct platform_device *pdev)
{
	int ret = -ENOMEM;
	struct dwc3_hiusb *hiusb_dwc = NULL;
	struct hiusb_client	*client = NULL;
	struct device *dev = &pdev->dev;

	pr_info("+\n");
	client = hiusb_client_create(dev, &dwc3_hiusb_client_desc, &dwc3_hiusb_client_ops);
	if (IS_ERR_OR_NULL(client))
		return -EPROBE_DEFER;

	hiusb_dwc = devm_kzalloc(dev, sizeof(*hiusb_dwc), GFP_KERNEL);
	if (!hiusb_dwc)
		goto destroy_client;

	ret = dwc3_hiusb_get_resource(pdev, hiusb_dwc);
	if (ret)
		goto destroy_client;

	hiusb_dwc->client = client;
	hiusb_dwc->dev = dev;
	hiusb_dwc->init_times = 0;

	dwc3_hiusb_dma_mask_config(hiusb_dwc);
	hiusb_client_set_drvdata(client, hiusb_dwc);

	hiusb_dwc->edev = devm_extcon_dev_allocate(dev, usb_extcon_cable);
	if (IS_ERR(hiusb_dwc->edev)) {
		pr_err("failed to allocate extcon device\n");
		ret = PTR_ERR(hiusb_dwc->edev);
		goto put_resource;
	}

	ret = devm_extcon_dev_register(dev, hiusb_dwc->edev);
	if (ret < 0) {
		pr_err("failed to register extcon device\n");
		goto put_resource;
	}
	hiusb_dwc->current_mode = HIUSB_PORT_USB_MODE_NONE;

	platform_set_drvdata(pdev, hiusb_dwc);

	ret = hiusb_runtime_enable(dev);
	if (ret < 0)
		pr_err("hiusb_runtime_enable failed %d\n", ret);

	dwc3_register_hooks();
	pr_info("dwc3_register_hooks\n");

	pr_info("-\n");
	return ret;
put_resource:
	dwc3_hiusb_put_resource(pdev);
destroy_client:
	hiusb_client_destroy(client);
	return ret;
}

static int dwc3_hiusb_remove(struct platform_device *pdev)
{
	int ret;
	struct dwc3_hiusb *hiusb_dwc = platform_get_drvdata(pdev);

	if (!hiusb_dwc) {
		pr_err("hiusb_dwc NULL\n");
		return -ENODEV;
	}

	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	ret = dwc3_hiusb_put_resource(pdev);
	if (ret)
		return ret;

	hiusb_client_destroy(hiusb_dwc->client);

	return ret;
}

static const struct of_device_id of_dwc3_hiusb_match[] = {
	{ .compatible = "hiusb,dwc3" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_dwc3_hiusb_match);

static struct platform_driver dwc3_hiusb_driver = {
	.probe		= dwc3_hiusb_probe,
	.remove		= dwc3_hiusb_remove,
	.driver		= {
		.name	= "dwc3-hiusb",
		.of_match_table = of_dwc3_hiusb_match,
	},
};

module_platform_driver(dwc3_hiusb_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DesignWare USB3 Layer of HIUSB");
