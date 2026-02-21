/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: platform dependent idi2axi interface implementations
 *
 * This file is released under the GPLv2.
 */
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include "idi2axi_internal.h"
#include "i2a_atf.h"
#include "plat_idi2axi.h"
#include "plat_csi2if.h"
#include "i2a_helper.h"
#include "i2a_debug.h"
#include "i2a_csi2if.h"
#include "i2a_power.h"
#include "cam_intf.h"
#include "i2a_mgr.h"
#include "idi2axi_intf.h"
#include "idi2axi_channel.h"
#include "cam_log.h"
#include <platform_include/see/efuse_driver.h>
#include <efuse_kernel_def.h>
#include "idi2axi_err.h"

typedef struct _tag_idi2axi300 {
	i2a_intf_t intf;
	char const *name;
	struct platform_device *pdev;
	struct mutex mtx; // for members in struct
	struct tasklet_struct irqtask;
	struct power_setting_group power_group;
	struct csi_phy_info csi_phy_info;
	i2a_mgr_t  i2a_mgr;
	spinlock_t power_lock; // protect power_ref
	struct wakeup_source *ws;
	struct idi2axi_channel channels[I2A_MAX_CHANNELS];
	struct i2a_dev_base_t i2a_base_info;
	uint32_t power_ref;
	uint32_t irq_no;
	unsigned int id;
} idi2axi300_t;


#define intf_2_idi2axi_intf(i) container_of(i, idi2axi300_t, intf)

static char const *i2a300_get_name(i2a_intf_t *i);
static int i2a300_config(i2a_intf_t *i, void *cfg);
static int i2a300_open(i2a_intf_t *i);
static int i2a300_close(i2a_intf_t *i);
static int i2a300_on_msg(i2a_intf_t *i, void *msg);
static int i2a300_ack_msg(i2a_intf_t *i, void *msg);
static int i2a300_get_err_info(i2a_intf_t *i, void *msg);

static i2a_vtbl_t vtbl_i2a300 = {
	.get_name = i2a300_get_name,
	.config = i2a300_config,
	.on_msg = i2a300_on_msg,
	.ack_msg = i2a300_ack_msg,
	.open = i2a300_open,
	.close = i2a300_close,
	.get_err_info = i2a300_get_err_info,
};

static char const *i2a300_get_name(i2a_intf_t *i)
{
	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300) {
		pr_err("%s idi2axi convert NULL", __func__);
		return NULL;
	}
	return i2a300->name;
}

static int i2a300_on_msg(i2a_intf_t *i, void *msg)
{
	int ret = -1;
	idi2axi_msg_t *m = (idi2axi_msg_t*)msg;
	if (!m) {
		pr_err("%s msg is NULL", __func__);
		return ret;
	}
	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300) {
		pr_err("%s idi2axi convert NULL", __func__);
		return ret;
	}

	mutex_lock(&i2a300->mtx);
	if (!i2a300->i2a_base_info.dev) {
		pr_err("%s dev is NULL", __func__);
		mutex_unlock(&i2a300->mtx);
		return ret;
	}
	struct device *dev = i2a300->i2a_base_info.dev;
	if (i2a300->power_ref == 0) {
		dev_err(i2a300->i2a_base_info.dev, "on msg without power on");
		mutex_unlock(&i2a300->mtx);
		return -EINVAL;
	}
	mutex_unlock(&i2a300->mtx);

	switch (m->api_name) {
	case COMMAND_IDI2AXI_REQUEST:
		{
			ret = i2a_queue_buf(i2a300->i2a_base_info.id, i2a300->channels, &i2a300->i2a_mgr, m);
		}
		break;
	case COMMAND_IDI2AXI_ATTACH_CHANNEL:
		{
			dev_info(dev, "COMMAND_IDI2AXI_ATTACH_CHANNEL");
			ret = i2a_attach_channel(i2a300->channels, &i2a300->i2a_mgr, &(m->u.attach_chanel_req));
		}
		break;
	case COMMAND_IDI2AXI_FLUSH_CHANNEL:
		{
			dev_info(dev, "COMMAND_IDI2AXI_FLUSH_CHANNEL");
			ret = i2a_flush(dev, i2a300->channels, &i2a300->i2a_mgr, m);
			if (ret >= 0 && i2a_is_all_channel_idle(i2a300->channels) == 0)
				disable_channel_base_irq(&i2a300->i2a_base_info);
		}
		break;
	case COMMAND_IDI2AXI_USECASE_CONFIG:
		{
			dev_info(dev, "COMMAND_IDI2AXI_USECASE_CONFIG");
			ret = i2a_usecase_config(&i2a300->i2a_base_info, &i2a300->i2a_mgr, m);
		}
		break;
	default:
		dev_info(dev, "%s unsupported cmd:%u", __func__, m->api_name);
		break;
	}
	return ret;
}

static int i2a300_ack_msg(i2a_intf_t *i, void *msg)
{
	idi2axi_msg_t *m = (idi2axi_msg_t*)msg;
	if (!m) {
		pr_err("%s idi2axi_msg_t is NULL", __func__);
		return -EFAULT;
	}
	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300) {
		pr_err("%s idi2axi convert NULL", __func__);
		return -EFAULT;
	}
	mutex_lock(&i2a300->mtx);
	if (!i2a300->i2a_base_info.dev) {
		pr_err("%s dev is NULL", __func__);
		mutex_unlock(&i2a300->mtx);
		return -EFAULT;
	}
	if (i2a300->power_ref == 0) {
		dev_err(i2a300->i2a_base_info.dev, "ack msg without power on");
		mutex_unlock(&i2a300->mtx);
		return -EINVAL;
	}
	mutex_unlock(&i2a300->mtx);
	return i2a_dqueue_buf(&i2a300->i2a_base_info, &(i2a300->i2a_mgr), m);
}


static int i2a300_get_err_info(i2a_intf_t *i, void *msg)
{
	idi2axi_err_info *m = (idi2axi_err_info*)msg;
	if (!m) {
		pr_err("%s idi2axi_err_info is NULL", __func__);
		return -EFAULT;
	}
	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300) {
		pr_err("%s idi2axi convert NULL", __func__);
		return -EFAULT;
	}
	mutex_lock(&i2a300->mtx);
	if (!i2a300->i2a_base_info.dev) {
		pr_err("%s dev is NULL", __func__);
		mutex_unlock(&i2a300->mtx);
		return -EFAULT;
	}
	if (i2a300->power_ref == 0) {
		dev_err(i2a300->i2a_base_info.dev, "get idi2axi_err_info without power on");
		mutex_unlock(&i2a300->mtx);
		return -EINVAL;
	}
	mutex_unlock(&i2a300->mtx);
	return read_err_info(&i2a300->channels[0], m);
}

static bool idi2axi_bypass_csi_init_by_soc_spec(void)
{
	const char *pg_chip_without_csi[] = {
		"desktop_pc",
		"unknown",
	};
	const char *soc_spec = NULL;
	int ret;
	unsigned int i;
	struct device_node *np = of_find_compatible_node(NULL, NULL, "hisilicon, soc_spec");
	if (np == NULL) {
		pr_err("%s: of_find_compatible_node fail or normal type chip\n", __func__);
		return false;
	}

	ret = of_property_read_string(np, "soc_spec_set", &soc_spec);
	if (ret < 0) {
		pr_err("%s: read string fail\n", __func__);
		return false;
	}

	for (i = 0; i < sizeof(pg_chip_without_csi) / sizeof(pg_chip_without_csi[0]); ++i) {
		ret = strncmp(soc_spec, pg_chip_without_csi[i], strlen(pg_chip_without_csi[i]));
		if (ret == 0) {
			pr_info("%s: this is pg chip:%s, need bypass csi\n", __func__, pg_chip_without_csi[i]);
			return true;
		}
	}

	pr_info("%s: no bypass csi\n", __func__);
	return false;
}

static int idi2axi_check_csi_bypass(uint32_t phy_id)
{
	bool is_pg_bypass = false;
	is_pg_bypass = idi2axi_bypass_csi_init_by_soc_spec();
	if (is_pg_bypass)
		return 1;

	return 0;
}

int idi2axi_device_power_on(idi2axi300_t *i2a300)
{
	int rc = EOK;
	unsigned long flags;
	mutex_lock(&i2a300->mtx);
	if (i2a300->power_ref == 0) {
		rc = idi2axi_check_csi_bypass(&i2a300->csi_phy_info.phy_id);
		if (rc)
			goto power_fail;
		rc = enable_power_setting(&i2a300->power_group);
		if (rc)
			goto power_fail;
		rc = plat_idi2axi_enable(&i2a300->i2a_base_info);
		if (rc)
			goto i2a_enable_fail;
		rc = i2a_csi_enable(i2a300->i2a_base_info.dev, &i2a300->csi_phy_info);
		if (rc)
			goto csi_phy_fail;
		__pm_stay_awake(i2a300->ws);
	}
	spin_lock_irqsave(&i2a300->power_lock, flags);
	++i2a300->power_ref;
	spin_unlock_irqrestore(&i2a300->power_lock, flags);
	dev_info(i2a300->i2a_base_info.dev, "%s power on succ cnt = %u", __func__, i2a300->power_ref);
	mutex_unlock(&i2a300->mtx);
	return EOK;

csi_phy_fail:
	plat_idi2axi_disable(&i2a300->i2a_base_info);
i2a_enable_fail:
	disable_power_setting(&i2a300->power_group);
power_fail:
	mutex_unlock(&i2a300->mtx);
	dev_err(i2a300->i2a_base_info.dev, "%s power on failed", __func__);
	return rc;
}

static int idi2axi_device_power_off(idi2axi300_t *i2a300)
{
	unsigned long flags;
	mutex_lock(&i2a300->mtx);
	if (i2a300->power_ref == 0) {
		dev_warn(i2a300->i2a_base_info.dev, "power off without power on");
		__pm_relax(i2a300->ws);
		mutex_unlock(&i2a300->mtx);
		return -EINVAL;
	}
	spin_lock_irqsave(&i2a300->power_lock, flags);
	--i2a300->power_ref;
	spin_unlock_irqrestore(&i2a300->power_lock, flags);
	if (i2a300->power_ref == 0) {
		dev_info(i2a300->i2a_base_info.dev, "power ref now zero");
		i2a_csi_disable(i2a300->i2a_base_info.dev, &i2a300->csi_phy_info);
		plat_idi2axi_disable(&i2a300->i2a_base_info);
		disable_power_setting(&i2a300->power_group);
		__pm_relax(i2a300->ws);
	}
	mutex_unlock(&i2a300->mtx);
	return EOK;
}

static int i2a300_config(i2a_intf_t *i, void *cfg)
{
	int ret = -1;
	idi2axi300_t *i2a300 = NULL;
	idi2axi_cfg_data *icfg = (idi2axi_cfg_data*)cfg;
	if (!icfg) {
		pr_err("%s input NULL", __func__);
		return ret;
	}
	i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300) {
		pr_err("%s convert to idi2axi300 NULL", __func__);
		return ret;
	}
	struct device *dev = i2a300->i2a_base_info.dev;
	if (!dev) {
		pr_err("%s dev is null", __func__);
		return ret;
	}
	switch (icfg->cfgtype) {
	case IDI2AXI_CONFIG_POWER_ON:
		{
			dev_info(dev, "%s cfgtype = IDI2AXI_CONFIG_POWER_ON", __func__);
			ret = idi2axi_device_power_on(i2a300);
			if (!ret) {
				i2a_mgr_init(&i2a300->i2a_mgr);
				i2a_channel_init(i2a300->channels, I2A_MAX_CHANNELS, &i2a300->i2a_base_info);
			}
		}
		break;
	case IDI2AXI_CONFIG_POWER_OFF:
		{
			dev_info(dev, "%s cfgtype = IDI2AXI_CONFIG_POWER_OFF", __func__);
			ret = idi2axi_device_power_off(i2a300);
		}
		break;
	default:
		dev_info(dev, "%s unsupported cfgtype:%#x", __func__, icfg->cfgtype);
		break;
	}

	if (ret < 0)
		dev_err(dev, "%s cmd:%#x fail, ret:%d", __func__, icfg->cfgtype, ret);
	return ret;
}

static int i2a300_open(i2a_intf_t *i)
{
	return 0;
}

static int i2a300_close(i2a_intf_t *i)
{
	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(i);
	if (!i2a300 || !i2a300->i2a_base_info.dev) {
		cam_err("%s idi2axi convert NULL", __func__);
		return -1;
	}

	mutex_lock(&i2a300->mtx);
	if (i2a300->power_ref == 0) {
		dev_warn(i2a300->i2a_base_info.dev, "close idi2axi without power on");
		__pm_relax(i2a300->ws);
		mutex_unlock(&i2a300->mtx);
		return 0;
	}
	mutex_unlock(&i2a300->mtx);

	i2a300->i2a_mgr.close_dev = true;
	i2a_clear(i2a300->i2a_base_info.dev, i2a300->channels);
	i2a300->i2a_mgr.close_dev = false;
	disable_channel_base_irq(&i2a300->i2a_base_info);

	unsigned long flags;
	spin_lock_irqsave(&i2a300->power_lock, flags);
	i2a300->power_ref = 1;
	spin_unlock_irqrestore(&i2a300->power_lock, flags);
	return idi2axi_device_power_off(i2a300);
}

static void idi2axi_dump(idi2axi300_t* i2a300)
{
	if (log_i2a_status()) {
		plat_dump_status(&i2a300->i2a_base_info);
		plat_dump_csi_status(&i2a300->csi_phy_info);
		plat_csi2if_dump_debug_info(&i2a300->csi_phy_info);
	}
}

static irqreturn_t idi2axi_irq_handler(int irq, void *dev_id)
{
	unsigned long flags;
	(void)irq;
	uint32_t eofs = 0;
	struct idi2axi_channel *ch = NULL;
	idi2axi300_t *i2a300 = dev_id;
	if (!i2a300) {
		pr_err("%s i2a300 device is NULL", __func__);
		return IRQ_NONE;
	}
	spin_lock_irqsave(&i2a300->power_lock, flags);
	if (!i2a300->power_ref) { // need power on state to read register
		dev_info(i2a300->i2a_base_info.dev, "irq comes, but power is down");
		spin_unlock_irqrestore(&i2a300->power_lock, flags);
		return IRQ_HANDLED;
	}

	eofs = plat_idi2axi_get_eof_channels(&i2a300->channels[0]);
	idi2axi_dump(i2a300);
	for (unsigned int cid = 0; cid < I2A_MAX_CHANNELS; ++cid) {
		ch = &i2a300->channels[cid];
		if ((BIT(cid) & eofs) && i2a_is_attached(ch))
			idi2axi_channel_eof_handler(ch);
	}
	spin_unlock_irqrestore(&i2a300->power_lock, flags);
	if (eofs)
		tasklet_schedule(&i2a300->irqtask);
	return IRQ_HANDLED;
}

static void idi2axi_irq_task(unsigned long data)
{
	unsigned long flags;
	uint32_t i = 0;
	struct idi2axi_channel *ch = NULL;
	struct buf_node *bn, *tmp = NULL;
	idi2axi300_t *i2a300 = (void *)data;
	if (!i2a300) {
		pr_err("%s i2a300 device is NULL", __func__);
		return;
	}

	for (i = 0; i < I2A_MAX_CHANNELS; ++i) {
		ch = &i2a300->channels[i];
		spin_lock_irqsave(&ch->lock, flags);
		list_for_each_entry_safe(bn, tmp, &ch->done_list, entry) {
			dev_dbg(i2a300->i2a_base_info.dev, "channel:%s eof comes, buf:%#lx",
				ch->requestor, bn->buf_addr);
			list_del(&bn->entry);
			if (ch->eof_cb)
				ch->eof_cb(&i2a300->i2a_base_info, bn, ch->cb_data);
		}
		spin_unlock_irqrestore(&ch->lock, flags);
	}
}

static int32_t idi2axi_parse_dts(struct platform_device *pdev, idi2axi300_t *i2a300)
{
	int rc = 0;
	struct device *dev = &pdev->dev;
	if (!dev) {
		pr_err("%s dev is null", __func__);
		return -1;
	}
	struct device_node *np = dev->of_node;
	if (!np) {
		dev_err(dev, "has no of_node");
		return -1;
	}
	rc = of_property_read_string(np, "idi2axi,name", &i2a300->name);
	dev_info(dev, "%s idi2axi,name %s, rc %d\n", __func__, i2a300->name, rc);
	if (rc < 0) {
		dev_err(dev, "%s failed %d\n", __func__, __LINE__);
		return rc;
	}

	rc = of_property_read_u32(np, "idi2axi,index", (u32 *)(&i2a300->i2a_base_info.id));
	dev_info(dev, "%s idi2axi,index %d, rc %d\n", __func__, i2a300->i2a_base_info.id, rc);
	if (rc < 0) {
		dev_err(dev,"%s failed %d\n", __func__, __LINE__);
		return rc;
	}

	if (get_power_setting(dev, &i2a300->power_group))
		return -1;

	if (i2a_parse_csi_info(pdev, &i2a300->csi_phy_info))
		return -1;

	i2a300->i2a_base_info.i2a_base = devm_ioremap_named_iomem(pdev, "idi2axi");
	dev_info(dev, "i2a_base  addr = %px", i2a300->i2a_base_info.i2a_base);
	if (!i2a300->i2a_base_info.i2a_base) {
		dev_err(dev, "i2a addr base is null");
		return -1;
	}
	return 0;
}

static int32_t idi2axi300_platform_probe(struct platform_device *pdev)
{
	if (!pdev) {
		pr_err("dev is null");
		return -1;
	}
	struct device *dev = &pdev->dev;
	dev_info(dev, "%s enter", __func__);
	idi2axi300_t *i2a300 = devm_kzalloc(dev, sizeof(*i2a300), GFP_KERNEL);
	if (!i2a300) {
		dev_err(dev, "%s() %d  kzalloc i2a300 fail", __func__, __LINE__);
		return -ENOMEM;
	}
	mutex_init(&i2a300->mtx);
	spin_lock_init(&i2a300->power_lock);
	i2a300->power_ref = 0;
	i2a300->pdev = pdev;
	i2a300->i2a_base_info.dev = dev;
	i2a300->intf.vtbl = &vtbl_i2a300;
	int ret = idi2axi_parse_dts(pdev, i2a300);
	if (ret != 0) {
		dev_err(dev, "%s() %d idi2axi_parse_dts failed", __func__, __LINE__);
		return ret;
	}

	ret = idi2axi_register(pdev, &i2a300->intf);
	if (ret != 0) {
		dev_err(dev, "%s() %d idi2axi_register failed with ret %d",
			__func__, __LINE__, ret);
		return ret;
	}

	i2a300->irq_no = platform_get_irq(pdev, 0);
	if (i2a300->irq_no <= 0) {
		dev_err(dev, "failed to get idi2axi irq");
		return -1;
	}

	ret = devm_request_irq(dev, i2a300->irq_no, idi2axi_irq_handler, 0, dev_name(dev), i2a300);
	if (ret < 0) {
		dev_err(dev, "request irq: %d failed", i2a300->irq_no);
		return ret;
	}
	tasklet_init(&i2a300->irqtask, idi2axi_irq_task, (uintptr_t)i2a300);

	i2a300->ws = wakeup_source_register(dev, "idi2axi_power_wakelock");
	if (!i2a300->ws) {
		dev_err(dev, "%s wakeup source register failed", __func__);
		return -1;
	}

	i2a_debugfs_init();
	return EOK;
}

static int32_t idi2axi300_platform_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "%s enter", __func__);
	struct idi2axi_device *idev = platform_get_drvdata(pdev);
	if (!idev)
		return -EINVAL;
	media_entity_cleanup(&(idev->subdev.entity));
	cam_cfgdev_unregister_subdev(&idev->subdev);

	idi2axi300_t *i2a300 = intf_2_idi2axi_intf(idev->intf);
	tasklet_kill(&i2a300->irqtask);
	wakeup_source_unregister(i2a300->ws);
	i2a300->ws = NULL;
	return EOK;
}

static const struct of_device_id idi2axi_of_match[] = {
	{ .compatible = "hisilicon,idi2axi300", },
	{},
};
MODULE_DEVICE_TABLE(of, idi2axi_of_match);

static struct platform_driver idi2axi_driver = {
	.probe = idi2axi300_platform_probe,
	.remove = idi2axi300_platform_remove,
	.driver = {
		.name = "hisilicon,idi2axi300",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(idi2axi_of_match),
	},
};

static int __init idi2axi_mipi_device_init(void)
{
	return platform_driver_register(&idi2axi_driver);
}

static void __exit idi2axi_mipi_device_exit(void)
{
	platform_driver_unregister(&idi2axi_driver);
}

module_init(idi2axi_mipi_device_init);
module_exit(idi2axi_mipi_device_exit);
MODULE_DESCRIPTION("idi2axi driver");
MODULE_LICENSE("GPL v2");