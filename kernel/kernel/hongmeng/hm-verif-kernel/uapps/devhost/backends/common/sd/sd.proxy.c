/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 13 16:05:44 2019
 */
#include <liblinux/pal.h>
#include <linux/kmod.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/libfdt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux_device.h>
#include <devhost/rpc.h>
#include <devhost/interrupt.h>
#include <linux/blkdev.h>

#include "api.h"
#ifdef CONFIG_CGROUP_IOLIMIT_HIBERN8
#include "platform_include/basicplatform/linux/ufs/hufs_hibern8.h"
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei OS Kernel Lab");
MODULE_DESCRIPTION("sd proxy driver");
MODULE_VERSION("0.01");

#define UNUSED(x) ((void)(x))
#define SD_PROXY_ISOLATE	"sdgrp"
#define SD_PROXY_COMPAT		"sd.proxy"
#define SD_PROXY_DEV		"sd_proxy"
#define SD_BUS_DEV		"sd_bus"

#define SD_ADDRESS_CELLS	2
#define SD_SIZE_CELLS		2
#define SD_IRQCTRL_ID_DEFAULT	0x10000

static struct device_info sd_proxy_info;
static struct sd_proxy_dev_s *sd_proxy_dev;

static int sd_proxy_dev_add(int parent)
{
	int ret;
	struct device_info *info = &sd_proxy_info;
	ret = devhost_device_init(info);
	if (ret < 0) {
		goto err_init;
	}
	info->devname = SD_PROXY_DEV;
	info->devid = -1;
	info->parent = parent;

	devhost_device_set_isolate(info, SD_PROXY_ISOLATE);
	devhost_device_set_probeable(info, SD_PROXY_COMPAT, sizeof(SD_PROXY_COMPAT));

	ret = devhost_register_device(info);
	if (ret < 0) {
		goto err_register;
	}

	pr_info("sd bus create proxy dev p=%d devid=%d\n", info->parent, info->devid);

	return 0;

err_register:
	devhost_device_destroy(info);
err_init:
	return ret;
}

static
void __fill_parent_property(char *buf)
{
	(void)fdt_property_cell(buf, "#address-cells", SD_ADDRESS_CELLS);
	(void)fdt_property_cell(buf, "#size-cells", SD_SIZE_CELLS);
	(void)fdt_property_cell(buf, "irqctrl_id", SD_IRQCTRL_ID_DEFAULT);
}

static
int __serialize_of_device_node(const struct device_node *node,
			       void *wr, size_t wr_size)
{
	struct property *prop = NULL;
	char *buf = NULL;
	size_t buf_size = 0;
	int ret = 0;

	buf = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (buf == NULL) {
		return -ENOMEM;
	}
	if ((fdt_create(buf, PAGE_SIZE) < 0) ||
	    (fdt_finish_reservemap(buf) < 0)) {
		goto err_overflow;
	}
	if (fdt_begin_node(buf, "/") < 0) {
		goto err_overflow;
	}
	__fill_parent_property(buf);
	if (fdt_begin_node(buf, node->name) < 0) {
		goto err_overflow;
	}
	for_each_property_of_node(node, prop) {
		/* skip "interrupt-parent" property */
		if (strcmp(prop->name, "interrupt-parent") == 0) {
			continue;
		}
		if (fdt_property(buf, prop->name,
				 prop->value, prop->length) < 0) {
			goto err_overflow;
		}
	}
	if (fdt_end_node(buf) < 0) {
		goto err_overflow;
	}
	if (fdt_end_node(buf) < 0) {
		goto err_overflow;
	}
	if (fdt_finish(buf) < 0) {
		goto err_overflow;
	}

	buf_size = fdt_totalsize(buf);
	if (buf_size > wr_size) {
		goto err_overflow;
	}

	ret = copy_to_user(wr, buf, buf_size);
	kfree(buf);

	return ret;

err_overflow:
	kfree(buf);
	return -EOVERFLOW;
}


static int sd_proxy_queryinfo_cb(unsigned int cmd, void *rd, size_t rd_size,
				 void *wr, size_t wr_size)
{
	UNUSED(cmd);
	UNUSED(rd);
	UNUSED(rd_size);

	if ((sd_proxy_dev == NULL) || ( sd_proxy_dev->dev == NULL)) {
		return -ENOSYS;
	}

	return __serialize_of_device_node(sd_proxy_dev->dev->of_node, wr, wr_size);
}

static int sd_proxy_querydma_cb(unsigned int cmd, void *rd, size_t rd_size,
				void *wr, size_t wr_size)
{
	UNUSED(cmd);
	UNUSED(rd);
	UNUSED(rd_size);

	if (sd_proxy_dev == NULL) {
		return -ENOSYS;
	}

	struct sd_proxy_dma_s dma = { 0 };
	dma.paddr = sd_proxy_dev->dma_paddr;
	dma.size = sd_proxy_dev->dma_size;

	if (wr_size != sizeof(dma)) {
		return -EINVAL;
	}

	return copy_to_user(wr, (void *)&dma, sizeof(dma));
}

static int sd_proxy_querystatus_cb(unsigned int cmd, void *rd, size_t rd_size,
				   void *wr, size_t wr_size)
{
	UNUSED(cmd);
	UNUSED(rd);
	UNUSED(rd_size);
	UNUSED(wr);
	UNUSED(wr_size);

	if ((sd_proxy_dev == NULL) || (sd_proxy_dev->query_status == NULL)) {
		return -ENOSYS;
	}

	/* read cmd from req in the future */
	return sd_proxy_dev->query_status((unsigned int)SD_PROXY_STATUS_SUSPEND);
}

#ifdef CONFIG_MAS_BLK_HM
static int sd_proxy_bkops_cb(unsigned int cmd, void *rd, size_t rd_size,
	void *wr, size_t wr_size)
{
	UNUSED(cmd);
	int ret = -ENOSYS;
	void *info = NULL;

	if (sd_proxy_dev == NULL) {
		return -ENOSYS;
	}

	if ((rd == NULL) || (rd_size <= 0) || (rd_size > PAGE_SIZE)) {
		pr_err("rd is null or size is invalid\n");
		return -ENOSYS;
	}

	info = (void *)kmalloc(rd_size, GFP_KERNEL | __GFP_ZERO);
	if (info == NULL) {
		return -ENOSYS;
	}

	ret = copy_from_user(info, rd, rd_size);
	if (ret) {
		pr_err("copy err %d\n", ret);
		kfree(info);
		return -ENOSYS;
	}

	ret = mas_bkops_hm_nofity(info, rd_size);
	kfree(info);

	return ret;
}
#endif

#ifdef CONFIG_CGROUP_IOLIMIT_HIBERN8
static int sd_proxy_update_ufshcd_hibern8_cb(unsigned int cmd, void *rd, size_t rd_size,
	void *wr, size_t wr_size)
{
	UNUSED(cmd);
	int ret = -ENOSYS;
	void *info = NULL;
	int dynamic_h8 = 0;

	if (sd_proxy_dev == NULL) {
		return -ENOSYS;
	}

	if ((rd == NULL) || (rd_size <= 0) || (rd_size > PAGE_SIZE)) {
		pr_err("rd is null or size is invalid\n");
		return -ENOSYS;
	}

	info = (void *)kmalloc(rd_size, GFP_KERNEL | __GFP_ZERO);
	if (info == NULL) {
		return -ENOSYS;
	}

	ret = copy_from_user(info, rd, rd_size);
	if (ret) {
		pr_err("copy err %d\n", ret);
		kfree(info);
		return -ENOSYS;
	}
	dynamic_h8 = *(int *)info;
	if (dynamic_h8) {
		ufshcd_iolimit_wakeup_and_update_h8();
	} else {
		ufshcd_iolimit_reset_h8();
	}
	kfree(info);

	return ret;
}
#endif

static int sd_proxy_start_error_handler_cb(unsigned int cmd, void *rd, size_t rd_size,
					   void *wr, size_t wr_size)
{
	struct sd_error_handler_s eh;

	UNUSED(cmd);
	UNUSED(wr);
	UNUSED(wr_size);

	if ((sd_proxy_dev == NULL) || (sd_proxy_dev->start_error_handler == NULL)) {
		return -ENOSYS;
	}

	if (rd_size != sizeof(eh)) {
		return -EINVAL;
	}

	if (copy_from_user(&eh, rd, rd_size)) {
		return -EINVAL;
	}

	return sd_proxy_dev->start_error_handler(&eh);
}

int sd_proxy_ops_register(int cmd, sd_proxy_func func)
{
	UNUSED(cmd);
	UNUSED(func);

	return -ENOSYS;
}
EXPORT_SYMBOL(sd_proxy_ops_register);

static int sd_proxy_rpc_invoke(int devid, int cmd,
			       struct devhost_rpc_req *req,
			       struct devhost_rpc_resp *resp)
{
	UNUSED(devid);
	uint64_t req_buf = 0UL, req_size = 0UL;
	uint64_t resp_buf = 0UL, resp_size = 0UL;
	req->acquire_buf(req, &req_buf, &req_size);
	resp->acquire_buf(resp, &resp_buf, &resp_size);

	return liblinux_pal_proxy_handler(cmd, (void *)req_buf, (size_t)req_size,
					  (void *)resp_buf, (size_t)resp_size);
}

static int dma_buf_copy(unsigned int cmd, void *rd, size_t rd_size,
			void *wr, size_t wr_size)
{
	int ret = 0;
	int ret1 = 0;
	int ret2 = 0;
	int w_test = 0xaaaa;
	int r_test = 0;

	switch (PAL_PROXY_MINOR_CMD_OF(cmd)) {
		case DMA_BUF_MINOR_CMD_COPY:
			if (wr_size != sizeof(int)) {
				return -EINVAL;
			} else {
				ret1 = copy_to_user(wr, (void *)&w_test, sizeof(int));
			}

			if (rd_size != sizeof(int)) {
				return -EINVAL;
			} else {
				ret2 = copy_from_user((void *)&r_test, rd, sizeof(int));
			}
			break;
		default:
			ret = -ENOSYS;
			break;
	}

	return ret;
}

static void sd_proxy_free_dev_res(void)
{
	unsigned int irq = sd_proxy_dev->reserved_irq_start;
	unsigned int irq_cnt = sd_proxy_dev->reserved_irq_cnt;
	unsigned int i;

	pr_info("reserved_irq_start: %u, cnt: %u\n", irq, irq_cnt);
	for (i = 0; i < irq_cnt; i++) {
		devhost_free_irq(irq + i);
	}
}

static void sd_proxy_free_dev_irq(int linux_irq, int cnt)
{
	pr_info("reserved_irq_start: %d, cnt: %d\n", linux_irq, cnt);
	for (int i = 0; i < cnt; i++) {
		devhost_free_irq(linux_irq + i);
	}
}

static void sd_proxy_set_its_base(uint64_t its_base)
{
	pr_info("sd proxy set its base\n");
	devhost_irq_set_its_base(its_base);
}

static struct devhost_rpc_ops sd_proxy_rpc_ops __read_mostly = {
	.invoke = sd_proxy_rpc_invoke,
};

/* udk export rref to ldk */
static int sd_proxy_init_rpc_key(unsigned int cmd, void *rd, size_t rd_size,
				 void *wr, size_t wr_size)
{
	UNUSED(cmd);
	UNUSED(wr);
	UNUSED(wr_size);

	int ret = 0;
	uint64_t uref = 0ULL;
	uint64_t sys_rref = 0ULL;

	if (sd_proxy_dev == NULL) {
		pr_err("sd proxy dev uninitialized\n");
		return -ENOSYS;
	}

	if (rd_size != sizeof(uint64_t)) {
		return -EINVAL;
	}

	ret = copy_from_user((void *)&uref, rd, rd_size);
	if (ret != 0) {
		pr_err("sd proxy copy from user failed, ret=%d\n", ret);
		return -EFAULT;
	}

	ret = devhost_timer_accept_grant(0ULL, uref,
					 0ULL, &sys_rref);
	if (ret != 0) {
		pr_info("sd proxy dev rpc key grant failed, ret=%d\n", ret);
		return ret;
	}

	pr_info("sd proxy dev rpc key updated\n");
	sd_proxy_dev->rpc_key = sys_rref;
	return 0;
}

static int sd_proxy_common(unsigned int cmd, void *rd, size_t rd_size,
			   void *wr, size_t wr_size)
{
	int ret = 0;

	switch (PAL_PROXY_MINOR_CMD_OF(cmd)) {
	case SD_PROXY_MINOR_CMD_QUERY_DEVINFO:
		ret = sd_proxy_queryinfo_cb(cmd, rd, rd_size, wr, wr_size);
		break;
	case SD_PROXY_MINOR_CMD_QUERY_DMA:
		ret = sd_proxy_querydma_cb(cmd, rd, rd_size, wr, wr_size);
		break;
	case SD_PROXY_MINOR_CMD_QUERY_STATUS:
		ret = sd_proxy_querystatus_cb(cmd, rd, rd_size, wr, wr_size);
		break;
#ifdef CONFIG_MAS_BLK_HM
	case SD_PROXY_MINOR_CMD_BKOPS:
		ret = sd_proxy_bkops_cb(cmd, rd, rd_size, wr, wr_size);
		break;
#endif
	case SD_PROXY_MINOR_CMD_START_ERROR_HANDLER:
		ret = sd_proxy_start_error_handler_cb(cmd, rd, rd_size, wr, wr_size);
		break;
	case SD_PROXY_MINOR_CMD_INIT_RPC_KEY:
		ret = sd_proxy_init_rpc_key(cmd, rd, rd_size, wr, wr_size);
		break;
#ifdef CONFIG_CGROUP_IOLIMIT_HIBERN8
	case SD_PROXY_MINOR_CMD_UPDATE_UFSHCD_HIBERN8:
		ret = sd_proxy_update_ufshcd_hibern8_cb(cmd, rd, rd_size, wr, wr_size);
		break;
#endif
	default:
		ret = -ENOSYS;
		break;
	}

	return ret;
}

static int sd_proxy_bind(void *ctx, struct device_info *info)
{
	int ret = 0;
	UNUSED(ctx);

	if (strncmp(info->devname, SD_BUS_DEV, strlen(SD_BUS_DEV)) == 0) {
		ret = sd_proxy_dev_add(info->devid);
		pr_info("sd bus bind dev devid=%d, parent=%d, ret=%d\n", info->devid, info->parent, ret);
	} else {
		ret = devhost_register_rpc(info->devid, &sd_proxy_rpc_ops);
		if (ret == 0) {
			liblinux_pal_proxy_register(PAL_PROXY_MAJOR(SD_PROXY_MAJOR_CMD), sd_proxy_common);
			liblinux_pal_proxy_register(PAL_PROXY_MAJOR(DMA_BUF_MAJOR_CMD), dma_buf_copy);
			sd_proxy_dev = sd_proxy_dev_fetch();
			sd_proxy_free_dev_res();
			if (sd_proxy_dev != NULL) {
				sd_proxy_dev->free_dev_irq = sd_proxy_free_dev_irq;
				sd_proxy_dev->set_its_base = sd_proxy_set_its_base;
			}
		}
		pr_info("sd proxy bind dev devid=%d, parent=%d, ret=%d\n", info->devid, info->parent, ret);
	}

	return ret;
}

static struct hm_driver_ops sd_proxy_ops __read_mostly = {
	.bind = sd_proxy_bind,
	.create = NULL,
};

static int sd_bus_dev_add(void)
{
	struct platform_device *pdev = NULL;
	struct linux_device_object *obj = NULL;
	int ret = -ENOMEM;

	if ((pdev = platform_device_alloc(SD_BUS_DEV, PLATFORM_DEVID_NONE)) == NULL) {
		goto err_alloc_pdev;
	}
	if ((ret = linux_device_init(&pdev->dev)) < 0) {
		goto err_init_dev;
	}
	obj = linux_device_object_of(&pdev->dev);
	ret = devhost_device_set_probeable(&obj->info, SD_PROXY_COMPAT, sizeof(SD_PROXY_COMPAT));
	if (ret < 0) {
		goto err_add_pdev;
	}

	ret = platform_device_add(pdev);
	if (ret < 0) {
		goto err_add_pdev;
	}

	return 0;

err_add_pdev:
	devhost_device_destroy(&obj->info);
err_init_dev:
	platform_device_put(pdev);
err_alloc_pdev:
	return ret;
}

static int __init sd_proxy_init(void)
{
	int ret;
	ret = devhost_register_driver(SD_PROXY_COMPAT".ko", &sd_proxy_ops, NULL);
	if (ret < 0) {
		pr_err("failed to register driver %s\n", SD_PROXY_COMPAT);
		goto err;
	}

	ret = sd_bus_dev_add();
	pr_info("sd bus driver init end, ret=%d\n", ret);
err:
	/* if sd proxy failed, it means system initialization failure
	 * BUG_ON is for easier locating.
	 */
	BUG_ON(ret != 0);

	return ret;
}

module_init(sd_proxy_init);
