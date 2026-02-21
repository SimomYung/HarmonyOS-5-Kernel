/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2021. All rights reserved.
 * Description: hhee exception driver source file
 * Create: 2024/05/07
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/kthread.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/util.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <linux/compiler.h>
#include <linux/version.h>
#include <linux/debugfs.h>
#include "hhee.h"
#include <linux/atomic.h>

struct rdr_exception_info_s hhee_excetption_info[] = {
	{ .e_modid            = MODID_AP_S_HHEE_PANIC,
	  .e_modid_end        = MODID_AP_S_HHEE_PANIC,
	  .e_process_priority = RDR_ERR,
	  .e_reboot_priority  = RDR_REBOOT_NOW,
	  .e_notify_core_mask = RDR_AP,
	  .e_reset_core_mask  = RDR_AP,
	  .e_from_core        = RDR_AP,
	  .e_reentrant        = (u32)RDR_REENTRANT_DISALLOW,
	  .e_exce_type        = AP_S_HHEE_PANIC,
	  .e_upload_flag      = (u32)RDR_UPLOAD_YES,
	  .e_from_module      = "RDR HHEE PANIC",
	  .e_desc             = "RDR HHEE PANIC" }
};

static int hhee_panic_handle(unsigned int len, void *buf)
{
	pr_err("hhee panic trigger system_error\n");
	(void)len;
	if (buf != NULL)
		pr_err("El2 trigger panic, buf should be null\n");
	rdr_syserr_process_for_ap((u32)MODID_AP_S_HHEE_PANIC, 0ULL, 0ULL);
	return 0;
}

/* check hhee enable */
static int g_hhee_enable = HHEE_ENABLE;
int hhee_check_enable(void)
{
	return g_hhee_enable;
}
EXPORT_SYMBOL(hhee_check_enable);

static int hkip_hhee_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;

	if (HHEE_DISABLE == hhee_check_enable())
		return 0;

	/* rdr struct register */
	ret = (s32)rdr_register_exception(&hhee_excetption_info[0]);
	if (!ret)
		dev_err(dev, "register hhee exception fail\n");

#ifdef CONFIG_HM_HHEE_DEBUG
	ret = hhee_init_debugfs();
	if (ret)
		dev_err(dev, "hhee debugfs init error\n");

	/*
	 * The hhee log init should not affect the hhee probe init,
	 * and hhee log is not available in user mode.
	 */
	ret = hhee_logger_init();
	if (ret < 0)
		dev_err(dev, "hhee logger init error\n");
#endif

	pr_info("hhee probe done\n");
	return 0;
}

static int hkip_hhee_remove(struct platform_device *pdev)
{
	(void)pdev;
#ifdef CONFIG_HM_HHEE_DEBUG
	hhee_cleanup_debugfs();
#endif
	return 0;
}

static struct platform_driver hkip_hhee_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "hhee",
	},
	.probe = hkip_hhee_probe,
	.remove = hkip_hhee_remove,
};

static int __init hkip_hhee_cmd_get(char *arg)
{
	if (!arg)
		return -EINVAL;

	if (!strncmp(arg, "false", strlen("false"))) {
		g_hhee_enable = HHEE_DISABLE;
	}

	pr_err("hhee enable = %d.\n", g_hhee_enable);
	return 0;
}
early_param("hhee_enable", hkip_hhee_cmd_get);

static int __init hkip_hhee_device_init(void)
{
	int ret;

	pr_info("hhee probe init\n");
	ret = platform_driver_register(&hkip_hhee_driver);
	if (ret)
		pr_err("register hhee driver fail\n");

	return ret;
}

static void __exit hkip_hhee_device_exit(void)
{
	platform_driver_unregister(&hkip_hhee_driver);
}

module_init(hkip_hhee_device_init);
module_exit(hkip_hhee_device_exit);

MODULE_DESCRIPTION("hhee driver");
MODULE_ALIAS("hhee module");
MODULE_LICENSE("GPL");
