/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <linux/of.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/of_fdt.h>
#include <linux/version.h>
#include <linux/irqdomain.h>
#include <linux/completion.h>
#include <linux/of_platform.h>
#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <devhost/memory.h>
#include <lnxbase/lnxbase.h>
#include <linux_device.h>

#include "devhost/devhost.h"
#include "devhost/notifier.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jiuyue Ma");
MODULE_DESCRIPTION("root:of driver");
MODULE_VERSION("0.01");

#define DTB_POPULATE_TIMEOUT (120 * HZ) /* 120-seconds for dtb populate */

static DECLARE_COMPLETION(probe_done);

static int of_platdrv_probe_dtb_prepare(struct platform_device *pdev)
{
	void *dtb = NULL;
	int ret;

	dtb = lnxbase_acquire_bootfdt(NULL);
	if (dtb == NULL) {
		pr_err("load kernel firmware data failed\n");
		return -EINVAL;
	}

	ret = liblinux_setup_fdt(dtb);
	if (ret < 0) {
		pr_err("setup fdt failed, err=%d\n", ret);
	}
	return ret;
}

#ifdef CONFIG_MDC_GODEL_HACK
static void __of_legacy_populate_msi(struct device_node *node)
{
	int ret;
	struct device_node *child = NULL;

	for_each_child_of_node(node, child) {
		if (of_find_property(child, "msi-controller", NULL) != NULL) {
			ret = devhost_vitschip_add(of_node_to_fwnode(child),
						   node->offset);
			if (ret < 0) {
				pr_err("of: failed to add virt itschip \"%s\", err=%d\n",
					child->name, ret);
			}
		}
	}
}
#endif

static int __of_legacy_populate(struct device_node *root)
{
	int ret;
	struct device_node *child = NULL;

	for_each_child_of_node(root, child) {
		if (of_find_property(child, "interrupt-controller", NULL) != NULL) {
			ret = devhost_virqchip_add(of_node_to_fwnode(child),
						   child->offset);
			if (ret < 0) {
				pr_err("of: failed to add virt irqchip \"%s\", err=%d\n",
				       child->name, ret);
			}
#ifdef CONFIG_MDC_GODEL_HACK
			__of_legacy_populate_msi(child);
#endif
			continue;
		}
	}

	return 0;
}

static int of_platdrv_probe(struct platform_device *pdev)
{
	int ret;
	struct device_node *nod = NULL;

	if (completion_done(&probe_done)) {
		return -EBUSY;
	}
	if (of_have_populated_dt()) {
		complete(&probe_done);
		return 0;
	}

	ret = of_platdrv_probe_dtb_prepare(pdev);
	if (ret < 0) {
		pr_err("legacy of node populate failed, err=%d\n", ret);
		return ret;
	}

	/* populate legacy node from "/hmmicrokernel" (e.g. intr, msi, smmu) */
	nod = of_find_node_by_path("/hmmicrokernel");
	if (nod != NULL) {
		ret = __of_legacy_populate(nod);
		if (ret < 0) {
			pr_err("legacy of node populate failed, err=%d\n", ret);
			return ret;
		}
		/* populate device from "/soc" if exists, else from "/" */
		nod = of_find_node_by_path("/soc");
		if (nod == NULL) {
			nod = of_root;
		}
	} else {
		nod = of_root;
	}

	ret = of_platform_default_populate(nod, NULL, pdev->dev.parent);

	/* complete probe */
	complete(&probe_done);

	return ret;
}

static struct platform_driver platdrv = {
	.probe = of_platdrv_probe,
	.driver = {
		.name = "root:of",
	}
};

static int of_bind(void* ctx, struct device_info *info)
{
	struct linux_device_object *obj = NULL;
	int ret;

	if (info == NULL) {
		return -EINVAL;
	}
	obj = container_of(info, struct linux_device_object, info);

	ret = device_attach(obj->dev);
	if (ret == 1) {
		return ret;
	} else if (ret == 0) {
		return -ENOENT;
	} else {
		return ret;
	}
}

static
int of_create(void *ctx, const char *name,
	      const struct device_create_args *args,
	      struct device_info **pinfo)
{
	struct platform_device *pdev = NULL;
	struct linux_device_object *obj = NULL;
	int ret;

	ret = -ENOMEM;
	if ((pdev = platform_device_alloc(name, PLATFORM_DEVID_NONE)) == NULL) {
		goto err_alloc_pdev;
	}
	if ((ret = linux_device_init(&pdev->dev)) < 0) {
		goto err_init_dev;
	}
	obj = linux_device_object_of(&pdev->dev);

	/* set probeable when ldk don't do devices_autoprobe */
	if (!lnxbase_support_device_autoprobe()) {
		ret = devhost_device_set_probeable(&obj->info, "of", sizeof("of"));
		if (ret < 0) {
			goto err_add_pdev;
		}
	}

	ret = platform_device_add(pdev);
	if (ret < 0) {
		goto err_add_pdev;
	}

	/* wait for device finish probe */
	if (wait_for_completion_timeout(&probe_done, DTB_POPULATE_TIMEOUT) == 0) {
		/* timeout, panic */
		panic("of.ko: wait dtb populate timeout\n");
	}

	/*
	 * after of is successfully probed, notify devhost.ko
	 * that dtb is populated
	 */
	ret = dh_post_dev_populate_call();
	if (ret < 0) {
		panic("of.ko: post populate init failed, ret=%d\n", ret);
	}

	if (pinfo != NULL) {
		*pinfo = &obj->info;
	}

	return 0;

err_add_pdev:
	devhost_device_destroy(&obj->info);
err_init_dev:
	platform_device_put(pdev);
err_alloc_pdev:
	return ret;
}

static struct hm_driver_ops of_ops __read_mostly = {
	.bind = of_bind,
	.create = of_create,
};

static int __init of_init(void)
{
	int ret;

	ret = platform_driver_register(&platdrv);
	if (ret != 0) {
		return ret;
	}
	return devhost_register_driver("of.ko", &of_ops, NULL);
}
module_init(of_init);
