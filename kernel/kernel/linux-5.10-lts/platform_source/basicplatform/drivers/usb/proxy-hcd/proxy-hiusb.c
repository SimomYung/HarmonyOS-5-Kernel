/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Glue layer for hifi
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
#include "hifi-usb.h"

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/phy/phy.h>
#include <hifi-usb-hibernate.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_drivers/usb/hiusb_client.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include "../hiusb/core.h"

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_PROXY]%s: " fmt, __func__

enum usb_clk_freq_select {
	HIGH_FREQUENCY = 0,
	LOW_FREQUENCY
};

struct proxy_hiusb {
	struct device		*dev;
	struct clk		**clks;
	int			num_clocks;
	struct clk_freq_config *clk_freq_cfg;
	int			portid;
	/*
	 support_suspend: Whether the hifi supports suspend and resume.
	 If the value is true, stop hifi during suspend and start hifi during resume.
	 */
	bool			support_suspend;
	bool		is_hibernate;
	struct chip_usb_reg_cfg	*reset;
	struct chip_usb_reg_cfg	*unreset;
	struct chip_usb_reg_cfg	**qos_set;
	int num_qos_set;
	struct chip_usb_reg_cfg	**qos_reset;
	int num_qos_reset;
	struct chip_usb_reg_cfg	*intr_usb_hifi_enable;
	struct chip_usb_reg_cfg	*intr_usb_hifi_disable;
	struct hiusb_client	*client;
	struct notifier_block usbplug_nb;
};

static struct hiusb_client_desc hiusb_proxy_client_desc = {
	.type = HIUSB_CLIENT_TYPE_HIFI,
	.name = "hiusb-proxy",
	.owner = THIS_MODULE,
};

static int proxy_hiusb_clk_freq_cfg(struct proxy_hiusb *hiusb_proxy, enum usb_clk_freq_select select)
{
	unsigned int freq[2] = {0};
	int ret, i;

	if (!hiusb_proxy)
		return -EINVAL;

	for (i = 0; i < hiusb_proxy->num_clocks; i++) {
		if (hiusb_proxy->clk_freq_cfg && hiusb_proxy->clk_freq_cfg[i].clk) {
			freq[0] = hiusb_proxy->clk_freq_cfg[i].high_freq;
			freq[1] = hiusb_proxy->clk_freq_cfg[i].low_freq;

			pr_info("clk before set:fre=%d rate= %lu\n", select, clk_get_rate(hiusb_proxy->clk_freq_cfg[i].clk));
			ret = clk_set_rate(hiusb_proxy->clk_freq_cfg[i].clk, freq[select]);
			if (ret) {
				pr_err("[clk-index:%d]Failed to set clk rate\n", i);
				return ret;
			}
			pr_info("clk after set:fre=%d rate= %lu\n", select, clk_get_rate(hiusb_proxy->clk_freq_cfg[i].clk));
		}
	}

	return 0;
}

static int hifi_enhance_utmi_clk_if_necessary(struct hiusb_client *client) 
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

static int proxy_hiusb_init_hw(struct hiusb_client *client, struct proxy_hiusb *hiusb_proxy)
{
	int ret;
	pr_info("+\n");

	if (client->port == NULL)
		return -EINVAL;

	if (client->port->usb_init_flag_switch && client->port->init_flag != 0) {
		pr_info("hiusb port has inited. \n");
		return 0;
	}

	ret = hiusb_client_init_usb2(client);
	if (ret) {
		pr_err("client init usb2 fail %d\n", ret);
		return ret;
	}

	ret = proxy_hiusb_clk_freq_cfg(hiusb_proxy, LOW_FREQUENCY);
	if (ret)
		pr_info("clk_freq_cfg failed%d\n", ret);

	ret = chip_usb_init_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		goto init_clks_fail;
	}

	if (hiusb_proxy->unreset) {
		ret = chip_usb_reg_write(hiusb_proxy->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}

	if (hiusb_proxy->intr_usb_hifi_enable) {
		ret = chip_usb_reg_write(hiusb_proxy->intr_usb_hifi_enable);
		if (ret) {
			pr_err("set intr_usb_hifi_enable failed %d\n", ret);
			goto reset_fail;
		}
	}

	msleep(10);
	/* add utmi clk enhance process [begin] */
	ret = hifi_enhance_utmi_clk_if_necessary(client);
	if (ret) {
		pr_err("enhance utmi failed %d, will abort hifi enum!!!\n", ret);
		goto reset_fail;
	}
	client->port->init_flag = 1;
	/* add utmi clk enhance process [end] */
	pr_info("-\n");
	return ret;

reset_fail:
	chip_usb_shutdown_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);
init_clks_fail:
	proxy_hiusb_clk_freq_cfg(hiusb_proxy, HIGH_FREQUENCY);
	hiusb_client_deinit_usb2(client, 0);
	client->port->init_flag = 0;
	return ret;
}

void proxy_hiusb_deinit_hw(struct hiusb_client *client, struct proxy_hiusb *hiusb_proxy, unsigned int keep_power)
{
	int ret;

	if (client->port == NULL)
		return;

	if (client->port->usb_init_flag_switch && client->port->init_flag == 0) {
		pr_err("hiusb port no need deinit. \n");
		return;
	}
	if (hiusb_proxy->intr_usb_hifi_disable) {
		ret = chip_usb_reg_write(hiusb_proxy->intr_usb_hifi_disable);
		if (ret)
			pr_err("set intr_usb_hifi_disable failed %d\n", ret);
	}

	if (hiusb_proxy->reset) {
		ret = chip_usb_reg_write(hiusb_proxy->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	ret = proxy_hiusb_clk_freq_cfg(hiusb_proxy, HIGH_FREQUENCY);
	if (ret)
		pr_info("clk_freq_cfg failed%d\n", ret);

	chip_usb_shutdown_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);

	ret = hiusb_client_deinit_usb2(client, keep_power);
	if (ret)
		pr_err("deinit usb2 fail %d\n", ret);
	client->port->init_flag = 0;
}

static int proxy_hiusb_port_init_hw(struct hiusb_client *client)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	int ret;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	ret = proxy_hiusb_init_hw(client, hiusb_proxy);
	if (ret)
		pr_err("init hw fail\n");

	pr_info("-n");
	return ret;
}

static int proxy_hiusb_port_deinit_hw(struct hiusb_client *client, unsigned int keep_power)
{
	struct proxy_hiusb *hiusb_proxy = NULL;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	proxy_hiusb_deinit_hw(client, hiusb_proxy, keep_power);

	pr_info("-\n");
	return 0;
}

static void hifi_usb_set_qos(struct proxy_hiusb *proxy, bool set)
{
	if (!proxy)
		return;

	/*
	 * qos register need usb domain poweron,
	 * access should after check hibeernation_state
	 */
	if (proxy->qos_set && proxy->qos_reset) {
		if (set) {
			if (chip_usb_reg_write_array(proxy->qos_set, proxy->num_qos_set))
				pr_err("set controller qos failed\n");
		} else {
			if (chip_usb_reg_write_array(proxy->qos_reset, proxy->num_qos_reset))
				pr_err("reset controller qos failed\n");
		}
	}
}

static int proxy_hiusb_port_assign(struct hiusb_client *client, enum hiusb_port_usb_mode mode, struct device *parent)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	int ret;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	if (mode != HIUSB_PORT_USB_MODE_HIFI)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	pr_info("mode %d\n",mode);

	ret = proxy_hiusb_init_hw(client, hiusb_proxy);
	if (ret)
		return ret;

	ret = set_hifi_usb_portid(hiusb_proxy->portid);
	if (ret){
		pr_err("set portid fail %d\n",ret);
		goto start_hifi_usb_fail;
	}

	hifi_usb_set_qos(hiusb_proxy, true);
	ret = start_hifi_usb();
	if (ret) {
		ret = -EBUSY;
		goto set_hifi_usb_portid_fail;
	}
	pr_info("-\n");
	return 0;
start_hifi_usb_fail:
	set_hifi_usb_portid(-ENODEV);
set_hifi_usb_portid_fail:
	hifi_usb_set_qos(hiusb_proxy, false);
	proxy_hiusb_deinit_hw(client, hiusb_proxy, 0);
	return ret;
}

static int proxy_hiusb_port_deassign(struct hiusb_client *client)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	int ret;

	if (!client)
		return -EINVAL;

	pr_info("+\n");

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	stop_hifi_usb();
	hifi_usb_set_qos(hiusb_proxy, false);
	ret = set_hifi_usb_portid(-ENODEV);
	if (ret)
		pr_err("set portid fail %d\n",ret);

	proxy_hiusb_deinit_hw(client, hiusb_proxy, 0);
	pr_info("-\n");
	return 0;
}

static int proxy_hiusb_port_bind(struct hiusb_client *client)
{
	return 0;
}

static int proxy_hiusb_port_unbind(struct hiusb_client *client)
{
	return 0;
}

static int proxy_hiusb_get_portid(struct hiusb_client *client)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	pr_info("-\n");
	return hiusb_proxy->portid;
}

static int proxy_hiusb_port_hibernate(struct hiusb_client *client)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	int ret;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	ret = hifi_usb_hibernate();
	if (ret) {
		pr_err("set hifi_usb_hibernate fail %d\n",ret);
		return ret;
	}

	ret = proxy_hiusb_port_deinit_hw(client, 1);
	if (ret) {
		pr_err("proxy_hiusb_port_deinit_hw fail %d\n",ret);
		return ret;
	}

	pr_info("-n");
	return ret;
}

int proxy_hiusb_port_wakeup(struct hiusb_client *client)
{
	struct proxy_hiusb *hiusb_proxy = NULL;
	int ret;

	pr_info("+\n");
	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	ret = proxy_hiusb_port_init_hw(client);
	if (ret) {
		pr_err("set proxy_hiusb_port_init_hw fail %d\n",ret);
		return ret;
	}
	ret = hifi_usb_revive();
	if (ret) {
		pr_err("hifi_usb_revive fail %d\n",ret);
		goto err_usb_revive;
	}

	hifi_usb_complete_wakeup();

	pr_info("-n");
	return ret;
err_usb_revive:
	proxy_hiusb_port_deinit_hw(client, 0);
	return ret;
}

static int proxy_hiusb_port_ioctrl(struct hiusb_client *client,
								enum hiusb_port_client_operation operation_code, void* data)
{
	pr_info("+\n");
	if (!client)
		return -EINVAL;

	switch (operation_code) {
	case HIUSB_CLIENT_IOCTRL_RESET_HIFI:
		reset_hifi_usb();
		pr_info("reset_hifi_usb\n");
		break;

	case HIUSB_CLIENT_IOCTRL_STOP_HIFI:
		stop_hifi_usb();
		pr_info("stop_hifi_usb\n");
		break;

	default:
		pr_info("un support operation_code %d\n", operation_code);
		break;
	}

	pr_info("-\n");
	return 0;
}

static int proxy_hiusb_port_suspend(struct hiusb_client *client, bool remote_wakup)
{
	int ret = 0;
	struct proxy_hiusb *hiusb_proxy = NULL;

	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	if (!hiusb_proxy->support_suspend)
		return 0;

	pr_info("+ keep_remote_wakup:%d\n", (int)remote_wakup);
	stop_hifi_usb();

	hiusb_client_set_port_power_flag(client, HIUSB_POWER_OFF);

	if (hiusb_proxy->reset) {
		ret = chip_usb_reg_write(hiusb_proxy->reset);
		if (ret)
			pr_err("reset fail %d\n", ret);
	}

	chip_usb_shutdown_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);

	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;
}

static int proxy_hiusb_port_resume(struct hiusb_client *client, bool remote_wakup)
{
	int ret = 0;
	struct proxy_hiusb *hiusb_proxy = NULL;

	if (!client)
		return -EINVAL;

	hiusb_proxy = (struct proxy_hiusb *)hiusb_client_get_drvdata(client);
	if (hiusb_proxy == NULL)
		return -ENODEV;

	if (!hiusb_proxy->support_suspend)
		return 0;

	pr_info("+ keep_remote_wakup:%d\n", (int)remote_wakup);
	ret = chip_usb_init_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);
	if (ret) {
		pr_err("clks init failed %d\n", ret);
		return ret;
	}

	if (hiusb_proxy->unreset) {
		ret = chip_usb_reg_write(hiusb_proxy->unreset);
		if (ret) {
			pr_err("unreset failed %d\n", ret);
			goto reset_fail;
		}
	}

	start_hifi_usb();
	hiusb_client_set_port_power_flag(client, HIUSB_POWER_ON);

	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;

reset_fail:
	chip_usb_shutdown_clks(hiusb_proxy->clks, hiusb_proxy->num_clocks);
	pr_info("- keep_remote_wakup:%d\n", (int)remote_wakup);
	return ret;
}

static struct hiusb_client_ops proxy_hiusb_client_ops = {
	.bind = proxy_hiusb_port_bind,
	.unbind = proxy_hiusb_port_unbind,
	.assign = proxy_hiusb_port_assign,
	.deassign = proxy_hiusb_port_deassign,
	.suspend = proxy_hiusb_port_suspend,
	.resume = proxy_hiusb_port_resume,
	.get_portid = proxy_hiusb_get_portid,
	.init_hw = proxy_hiusb_port_init_hw,
	.deinit_hw = proxy_hiusb_port_deinit_hw,
	.hibernate = proxy_hiusb_port_hibernate,
	.wakeup = proxy_hiusb_port_wakeup,
	.ioctl = proxy_hiusb_port_ioctrl,
};

static int proxy_hiusb_get_resource(struct platform_device *pdev, struct proxy_hiusb *hiusb_proxy)
{
	int ret;
	struct device *dev = &pdev->dev;

	if (of_property_read_u32(dev->of_node, "portid",
				&(hiusb_proxy->portid))) {
		pr_err("get portid failed\n");
		return -ENODEV;
	}

	pr_info("portid %d\n", hiusb_proxy->portid);
	hiusb_proxy->support_suspend = of_property_read_bool(dev->of_node, "hifi_support_suspend");
	ret = devm_chip_usb_get_clks(dev, &hiusb_proxy->clks, &hiusb_proxy->num_clocks);
	if (ret) {
		pr_err("get controller clks failed %d\n", ret);
		return -ENODEV;
	}

	ret = chip_usb_get_clk_freq(dev->of_node, &hiusb_proxy->clk_freq_cfg, hiusb_proxy->num_clocks);
	if (ret)
		pr_info("get clk freq fail\n");

	hiusb_proxy->reset = of_get_chip_usb_reg_cfg(dev->of_node, "reset");
	if (!hiusb_proxy->reset) {
		pr_err("controller reset fail\n");
		chip_usb_put_clk_freq(hiusb_proxy->clk_freq_cfg, hiusb_proxy->num_clocks);
		hiusb_proxy->clk_freq_cfg = NULL;
		return -ENODEV;
	}

	hiusb_proxy->unreset = of_get_chip_usb_reg_cfg(dev->of_node, "unreset");
	if (!hiusb_proxy->unreset) {
		ret = -ENODEV;
		pr_err("controller unreset fail\n");
		goto resume_start;
	}

	if (get_chip_usb_reg_cfg_array(dev, "qos-set", &hiusb_proxy->qos_set, &hiusb_proxy->num_qos_set)) {
		pr_info("no qos_set\n");
		hiusb_proxy->qos_set = NULL;
		hiusb_proxy->num_qos_set = 0;
	}

	if (get_chip_usb_reg_cfg_array(dev, "qos-reset", &hiusb_proxy->qos_reset, &hiusb_proxy->num_qos_reset)) {
		pr_info("no qos_reset\n");
		hiusb_proxy->qos_reset = NULL;
		hiusb_proxy->num_qos_reset = 0;
	}

	hiusb_proxy->intr_usb_hifi_enable = of_get_chip_usb_reg_cfg(dev->of_node, "intr_usb_hifi_enable");
	if (!hiusb_proxy->intr_usb_hifi_enable)
		pr_err("intr usb hifi enable fail\n");

	hiusb_proxy->intr_usb_hifi_disable = of_get_chip_usb_reg_cfg(dev->of_node, "intr_usb_hifi_disable");
	if (!hiusb_proxy->intr_usb_hifi_disable)
		pr_err("intr usb hifi disable fail\n");
	return 0;
resume_start:
	of_remove_chip_usb_reg_cfg(hiusb_proxy->reset);
	chip_usb_put_clk_freq(hiusb_proxy->clk_freq_cfg, hiusb_proxy->num_clocks);
	hiusb_proxy->clk_freq_cfg = NULL;
	return ret;
}

static int proxy_hiusb_put_resource(struct proxy_hiusb *hiusb_proxy)
{
	of_remove_chip_usb_reg_cfg(hiusb_proxy->reset);
	of_remove_chip_usb_reg_cfg(hiusb_proxy->unreset);
	if (hiusb_proxy->qos_set)
		free_chip_usb_reg_cfg_array(hiusb_proxy->qos_set, hiusb_proxy->num_qos_set);

	if (hiusb_proxy->qos_reset)
		free_chip_usb_reg_cfg_array(hiusb_proxy->qos_reset, hiusb_proxy->num_qos_reset);

	if (hiusb_proxy->intr_usb_hifi_enable)
		of_remove_chip_usb_reg_cfg(hiusb_proxy->intr_usb_hifi_enable);
	if (hiusb_proxy->intr_usb_hifi_disable)
		of_remove_chip_usb_reg_cfg(hiusb_proxy->intr_usb_hifi_disable);
	if (hiusb_proxy->clk_freq_cfg)
		chip_usb_put_clk_freq(hiusb_proxy->clk_freq_cfg, hiusb_proxy->num_clocks);
	return 0;
}

static int proxy_hiusb_notifier_fn(struct notifier_block *nb,
			unsigned long action, void *data)
{
	pr_info("+\n");
	switch (action) {
	case HIUSB_PROP_HOST_PLUG_OUT:
		reset_hifi_usb();
		pr_info("reset_hifi_usb\n");
		break;

	case HIUSB_PROP_WAKEUP_CALLBACK:
		hifi_usb_complete_wakeup();
		pr_info("hifi_usb_complete_wakeup\n");
		break;

	default:
		pr_info("unsupport action %lu\n", action);
		break;
	}
	pr_info("-\n");
	return 0;
}

static int proxy_hiusb_probe(struct platform_device *pdev)
{
	int ret = -ENOMEM;
	struct proxy_hiusb *hiusb_proxy = NULL;
	struct hiusb_client	*client = NULL;
	struct device *dev = &pdev->dev;

	pr_info("+\n");
	client = hiusb_client_create(dev, &hiusb_proxy_client_desc, &proxy_hiusb_client_ops);
	if (IS_ERR_OR_NULL(client))
		return -EPROBE_DEFER;

	hiusb_proxy = devm_kzalloc(dev, sizeof(*hiusb_proxy), GFP_KERNEL);
	if (!hiusb_proxy)
		goto destroy_client;

	ret = proxy_hiusb_get_resource(pdev, hiusb_proxy);
	if (ret)
		goto destroy_client;

	hiusb_proxy->client = client;
	hiusb_proxy->dev = dev;

	hiusb_client_set_drvdata(client, hiusb_proxy);

	platform_set_drvdata(pdev, hiusb_proxy);

	if (hiusb_proxy->intr_usb_hifi_disable) {
		ret = chip_usb_reg_write(hiusb_proxy->intr_usb_hifi_disable);
		if (ret) {
			pr_err("set intr_usb_hifi_disable failed %d\n", ret);
			goto put_resource;
		}
	}

	hiusb_proxy->usbplug_nb.notifier_call = proxy_hiusb_notifier_fn;
	ret = hiusb_plug_event_notifier_register(&hiusb_proxy->usbplug_nb);
	if (ret) {
		hiusb_proxy->usbplug_nb.notifier_call = NULL;
		pr_err("hiusb_runtime_enable failed %d\n", ret);
		goto put_resource;
	}
	ret = hiusb_runtime_enable(dev);
	if (ret < 0)
		pr_err("hiusb_runtime_enable failed %d\n", ret);

	pr_info("-\n");
	return ret;
put_resource:
	proxy_hiusb_put_resource(hiusb_proxy);
destroy_client:
	hiusb_client_destroy(client);
	return ret;
}

static int proxy_hiusb_remove(struct platform_device *pdev)
{
	int ret;
	struct proxy_hiusb *hiusb_proxy = platform_get_drvdata(pdev);

	if (!hiusb_proxy) {
		pr_err("hiusb_proxy NULL\n");
		return -ENODEV;
	}

	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	if (hiusb_proxy->usbplug_nb.notifier_call) {
		hiusb_plug_event_notifier_unregister(&hiusb_proxy->usbplug_nb);
		hiusb_proxy->usbplug_nb.notifier_call = NULL;
	}

	ret = proxy_hiusb_put_resource(hiusb_proxy);
	if (ret)
		pr_err("proxy_hiusb_put_resource fail\n");

	hiusb_client_destroy(hiusb_proxy->client);

	return ret;
}

static const struct of_device_id of_proxy_hiusb_client_match[] = {
	{ .compatible = "hisilicon,proxy-hiusb" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_proxy_hiusb_client_match);

static struct platform_driver proxy_hiusb_driver = {
	.probe		= proxy_hiusb_probe,
	.remove		= proxy_hiusb_remove,
	.driver		= {
		.name	= "proxy_hiusb_driver",
		.of_match_table = of_proxy_hiusb_client_match,
	},
};

module_platform_driver(proxy_hiusb_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("proxy hiusb driver");
