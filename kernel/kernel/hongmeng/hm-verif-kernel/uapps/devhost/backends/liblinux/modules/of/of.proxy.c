/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <linux/of.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/libfdt.h>
#include <linux/of_fdt.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <devhost/rpc.h>
#include <lnxbase/lnxbase.h>
#include <linux_device.h>

#include "of_private.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jiuyue Ma");
MODULE_DESCRIPTION("of proxy driver");
MODULE_VERSION("0.01");

#define MAX_BUFSIZE 0x400
#define OF_PROXY_RESOURCE_NR	2

static DEFINE_MUTEX(of_proxy_lock);

static
int __serialize_of_node_property(const struct device_node *node,
				 struct devhost_rpc_resp *resp)
{
	struct property *prop = NULL;
	char *buf = NULL;
	int ret = 0;

	/* alloc serialize buffer */
	buf = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (buf == NULL) {
		return -ENOMEM;
	}

	/* build fdt on buf */
	if ((fdt_create(buf, PAGE_SIZE) < 0) ||
	    (fdt_finish_reservemap(buf) < 0)) {
		goto err_overflow;
	}

	/* build fdt node with name `node->full_name` */
	if (fdt_begin_node(buf, node->full_name) < 0) {
		goto err_overflow;
	}
	/* serialize properties */
	for_each_property_of_node(node, prop) {
		/* skip "isolate" property */
		if (strcmp(prop->name, "isolate") == 0) {
			continue;
		}

		if (fdt_property(buf, prop->name,
				 prop->value, prop->length) < 0) {
			goto err_overflow;
		}
	}
	/* enable devmgr-autoprobe for devices of proxy devhost */
	if (fdt_property(buf, "autoprobe", NULL, 0) < 0) {
		goto err_overflow;
	}
	if (fdt_end_node(buf) < 0) {
		goto err_overflow;
	}

	/* finish fdt build */
	if (fdt_finish(buf) < 0) {
		goto err_overflow;
	}

	/* return */
	ret = resp->write(resp, buf, fdt_totalsize(buf));
	kfree(buf);

	return ret;

err_overflow:
	kfree(buf);
	return -EOVERFLOW;
}

static int of_proxy_rpc_invoke(int devid, int cmd, struct devhost_rpc_req *req,
			       struct devhost_rpc_resp *resp)
{
	struct device_info *info = NULL;
	struct linux_device_object *obj = NULL;
	int ret;

	(void)cmd;
	(void)req;

	ret = devhost_acquire_device(devid, &info);
	if (ret != 0) {
		return ret;
	}
	obj = container_of(info, struct linux_device_object, info);

	ret = __serialize_of_node_property(obj->dev->of_node, resp);
	devhost_devinfo_put(info);

	return ret;
}

static struct devhost_rpc_ops of_proxy_rpc_ops __read_mostly = {
	.invoke = of_proxy_rpc_invoke,
};

static int of_proxy_bind(void* ctx, struct device_info *info)
{
	struct linux_device_object *obj = NULL;
	struct platform_device *pdev = NULL;
	int ret;

	if (info == NULL) {
		return -EINVAL;
	}
	obj = container_of(info, struct linux_device_object, info);

	pdev = to_platform_device(obj->dev);
	if ((pdev->driver_override == NULL) || (strcmp(pdev->driver_override, "of:proxy") != 0)) {
		return -EINVAL;
	}

	ret = devhost_register_rpc(info->devid, &of_proxy_rpc_ops);
	if (ret < 0) {
		return ret;
	}
	if (obj->dev->driver == NULL) {
		(void)device_attach(obj->dev);
	}

	return 0;
}

static int of_proxy_get_node(const struct device_create_args *args, char *buf, struct device_node **pnode)
{
	int ret = -EINVAL;
	int rpc_devid;
	uint64_t rpc_uref;
	static uint64_t rref = 0;
	struct device_node *node = NULL;
	struct rpc_ctx req = {.buf = NULL, .size = 0};
	struct rpc_ctx resp = {.buf = buf, .size = MAX_BUFSIZE};

	if ((args->resource == NULL) ||
	    (args->resource_size != (OF_PROXY_RESOURCE_NR * sizeof(uint64_t)))) {
		goto err_create;
	}
	rpc_uref = ((const uint64_t *)args->resource)[0];
	rpc_devid = (int)((const uint64_t *)args->resource)[1];
	mutex_lock(&of_proxy_lock);
	if (rref == 0) {
		ret = devhost_timer_accept_grant(0, rpc_uref, 0, &rref);
		if (ret < 0) {
			mutex_unlock(&of_proxy_lock);
			goto err_create;
		}
	}
	mutex_unlock(&of_proxy_lock);

	/* when actvchain A->B->C (e.g. user->ldk->udk), C will use A as caller if is_xact=true
	 * always pass is_xact=false if not necessary */
	ret = devhost_rpc_invoke(rref, rpc_devid, 0, &req, &resp, NULL, false);
	if (ret < 0) {
		goto err_create;
	}

	/* unflatten device_node by fdt */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	of_fdt_unflatten_tree((const unsigned long *)buf, &node);
#else
	of_fdt_unflatten_tree((const unsigned long *)buf, NULL, &node);
#endif
	if (node == NULL) {
		ret = -EINVAL;
		goto err_create;
	}
	node = __of_node_dup(node, node->full_name);
	if (node == NULL) {
		ret = -ENOMEM;
		goto err_create;
	}
	node->parent = of_root;

	*pnode = node;
err_create:
	return ret;
}

static
int of_proxy_create(void *ctx, const char *name,
		    const struct device_create_args *args,
		    struct device_info **pinfo)
{
	int ret;
	char *buf = NULL;
	struct of_changeset ocs;
	struct linux_device_object *obj = NULL;
	struct platform_device *pdev = NULL;
	struct device_node *node = NULL;
	struct property _pp_parent, *pp_parent = &_pp_parent;
	u32 parent_devid_be32 = cpu_to_be32(args->parent_devid);

	buf = kmalloc(MAX_BUFSIZE, GFP_KERNEL);
	if (buf == NULL) {
		return -ENOMEM;
	}
	ret = of_proxy_get_node(args, buf, &node);
	if (ret < 0) {
		goto err_create_prepare;
	}

	/* add extra "parent-devid" property */
	pp_parent->name = "parent-devid";
	pp_parent->value = &parent_devid_be32;
	pp_parent->length = (int)sizeof(parent_devid_be32);
	pp_parent = __of_prop_dup(pp_parent, GFP_KERNEL);
	if (pp_parent == NULL) {
		ret = -ENOMEM;
		goto err_alloc_pp_parent;
	}

	/* build and apply ocs */
	of_changeset_init(&ocs);
	if (of_changeset_attach_node(&ocs, node) < 0 ||
	    of_changeset_add_property(&ocs, node, pp_parent) < 0) {
		ret = -ENOMEM;
		goto err_apply_ocs;
	}

	if ((ret = of_changeset_apply(&ocs)) < 0) {
		goto err_apply_ocs;
	}

	/* query platform device by device_node */
	pdev = of_find_device_by_node(node);
	if (pdev == NULL) {
		ret = -EFAULT;
		goto err_find_dev_by_node;
	}

	/* all done, destroy the changeset */
	of_changeset_destroy(&ocs);
	kfree(buf);

	if (pinfo != NULL) {
		obj = linux_device_object_of(&pdev->dev);
		*pinfo = &obj->info;
	}

	return 0;

err_find_dev_by_node:
	(void)of_changeset_revert(&ocs);
	of_changeset_destroy(&ocs);
err_apply_ocs:
	kfree(pp_parent->name);
	kfree(pp_parent->value);
	kfree(pp_parent);
err_alloc_pp_parent:
	of_node_put(node);
err_create_prepare:
	kfree(buf);
	return ret;
}

static int platdrv_probe(struct platform_device *pdev)
{
	/* not a isolate device, can not bind to proxy driver */
	if (of_find_property(pdev->dev.of_node, "isolate", NULL) == NULL) {
		return -EINVAL;
	}
	return 0;
}

static struct platform_driver platdrv = {
	.probe = platdrv_probe,
	.driver = {
		.name = "of:proxy",
	}
};

static struct hm_driver_ops of_proxy_ops __read_mostly = {
	.bind = of_proxy_bind,
	.create = of_proxy_create,
};

static int __populate_dummy_dt_root(const char *provider)
{
	void *fdt = NULL;
	int ret;

	fdt = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (fdt == NULL) {
		return -ENOMEM;
	}

	/*
	 * build fdt, just ignore retvalue.
	 * liblinux_setup_fdt() will validate the fdt blob.
	 */
	(void)fdt_create(fdt, PAGE_SIZE);
	(void)fdt_finish_reservemap(fdt);
	(void)fdt_begin_node(fdt, "/");
	(void)fdt_property_cell(fdt, "#address-cells", 2);
	(void)fdt_property_cell(fdt, "#size-cells", 2);
	(void)fdt_property(fdt, "ranges", NULL, 0);
	(void)fdt_property_string(fdt, "provider", provider);
	(void)fdt_end_node(fdt);
	(void)fdt_finish(fdt);

	/* setup dt root */
	ret = liblinux_setup_fdt(fdt);
	if (ret < 0) {
		pr_err("SETUP FDT failed: ret=%d\n", ret);
		kfree(fdt);
		return ret;
	}

	/* populate dummy dt root */
	ret = of_platform_default_populate(of_root, NULL, NULL);
	if (ret < 0) {
		kfree(fdt);
		return ret;
	}

	return 0;
}

static int __init of_proxy_init(void)
{
	int ret;

	/* populate of_root if required */
	if (!of_have_populated_dt()) {
		ret = __populate_dummy_dt_root("of.proxy");
		if (ret < 0) {
			return ret;
		}
	}

	/* register platform driver */
	ret = platform_driver_register(&platdrv);
	if (ret < 0) {
		/* keep the populated of_root */
		return ret;
	}

	/* register driver to devhost */
	ret = devhost_register_driver("of.proxy.ko", &of_proxy_ops, NULL);
	if (ret < 0) {
		platform_driver_unregister(&platdrv);
		return ret;
	}

	return 0;
}
module_init(of_proxy_init);
