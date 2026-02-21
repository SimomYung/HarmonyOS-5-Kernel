/**
 * @file cmdlist_drv.c
 * @brief Cmdlist device driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/of_device.h>
#include <linux/module.h>
#include <linux/genalloc.h>

#include "ukmd_log.h"
#include "chash.h"
#include "cmdlist_node.h"
#include "cmdlist_dev.h"
#include "cmdlist_drv.h"

#define CMDLIST_DRV_NAME "cmdlist"
#define DTS_DPU_CMDLIST "ukmd,dpu_cmdlist"
#define DTS_AAE_CMDLIST "ukmd,aae_cmdlist"

struct cmdlist_private g_cmdlist_privs[CMDLIST_DEV_MAX_NUM];

static int32_t cmdlist_of_device_probe(struct platform_device *pdev)
{
	int32_t ret = 0;
	struct cmdlist_private *cmd_priv = (struct cmdlist_private *)platform_get_drvdata(pdev);

	if (!cmd_priv)
		return -1;

	cmd_priv->of_dev = &pdev->dev;
	
	ret = dma_set_mask_and_coherent(cmd_priv->of_dev, DMA_BIT_MASK(64));
	if (unlikely(ret != 0)) {
		ukmd_pr_err("dma set mask and coherent failed %d!", ret);
		return -1;
	}

	cmd_priv->client_mgr.cmd_priv = cmd_priv;
	if (unlikely(cmdlist_client_mgr_probe(&cmd_priv->client_mgr, cmd_priv->of_dev->of_node))) {
		ukmd_pr_err("cmdlist_client_mgr_probe failed!");
		return -1;
	}

	cmd_priv->mem_mgr.cmd_priv = cmd_priv;
	if (unlikely(cmdlist_mem_mgr_probe(&cmd_priv->mem_mgr, cmd_priv->of_dev,
		cmd_priv->client_mgr.sum_node_size))) {
		ukmd_pr_err("cmdlist_mem_mgr_probe failed!");
		cmdlist_client_mgr_remove(&cmd_priv->client_mgr);
		cmd_priv->client_mgr.cmd_priv = NULL;
		cmd_priv->mem_mgr.cmd_priv = NULL;
		return -1;
	}

	return 0;
}

static void cmdlist_of_device_remove(struct platform_device *pdev)
{
	struct cmdlist_private *cmd_priv = (struct cmdlist_private *)platform_get_drvdata(pdev);
	if (unlikely(!cmd_priv))
		return;

	cmdlist_client_mgr_remove(&cmd_priv->client_mgr);
	cmd_priv->client_mgr.cmd_priv = NULL;

	cmdlist_mem_mgr_remove(&cmd_priv->mem_mgr);
	cmd_priv->mem_mgr.cmd_priv = NULL;
}

static struct cmdlist_match_data cmdlist_basic_dpu = {
	.version = GENERIC_DEVICETREE_CMDLIST,
	.dev_id = CMDLIST_DEV_ID_DPU,
	.of_device_probe = cmdlist_of_device_probe,
	.of_device_remove = cmdlist_of_device_remove
};

static struct cmdlist_match_data cmdlist_basic_aae = {
	.version = GENERIC_DEVICETREE_CMDLIST,
	.dev_id = CMDLIST_DEV_ID_AAE,
	.of_device_probe = cmdlist_of_device_probe,
	.of_device_remove = cmdlist_of_device_remove
};

static const struct of_device_id device_match_table[] = {
	{
		.compatible = DTS_DPU_CMDLIST,
		.data = &cmdlist_basic_dpu,
	},
	{
		.compatible = DTS_AAE_CMDLIST,
		.data = &cmdlist_basic_aae,
	},
	{},
};
MODULE_DEVICE_TABLE(of, device_match_table);

static int32_t cmdlist_probe(struct platform_device *pdev)
{
	const struct cmdlist_match_data *data = NULL;
	struct cmdlist_private *cmd_priv;
	uint32_t dev_id = 0;

	if (!pdev)
		return -EINVAL;

	ukmd_pr_info("++++");
	data = of_device_get_match_data(&pdev->dev);
	if (!data) {
		ukmd_pr_err("get device data failed!");
		return -EINVAL;
	}

	dev_id = data->dev_id;
	if (dev_id >= CMDLIST_DEV_MAX_NUM) {
		ukmd_pr_err("dev_id=%u is over %d!", dev_id, CMDLIST_DEV_MAX_NUM);
		return -EINVAL;
	}

	cmd_priv = &g_cmdlist_privs[dev_id];
	if (cmd_priv->device_initialized != 0) {
		ukmd_pr_info("already initailed!");
		return 0;
	}

	memset(cmd_priv, 0, sizeof(*cmd_priv));
	cmd_priv->pdev = pdev;
	platform_set_drvdata(pdev, cmd_priv);

	if (data->of_device_probe(pdev)) {
		ukmd_pr_err("Device initialization is failed!");
		return -EINVAL;
	}

	if (cmdlist_device_probe(cmd_priv) != 0) {
		data->of_device_remove(pdev);
		ukmd_pr_err("Device setup failed!");
		return -EINVAL;
	}

	cmd_priv->dev_id = dev_id;
	atomic_set(&cmd_priv->ref_count, 0);
	cmd_priv->device_initialized = 1;
	ukmd_pr_info("---- dev_id=%u", dev_id);

	return 0;
}

/**
 * Clear resource when device removed but not for devicetree device
 */
static int32_t cmdlist_remove(struct platform_device *pdev)
{
	const struct cmdlist_match_data *data = NULL;
	struct cmdlist_private *cmd_priv = (struct cmdlist_private *)platform_get_drvdata(pdev);

	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return -EINVAL;
	}

	if (cmd_priv->device_initialized == 0) {
		ukmd_pr_err("has no initialization!");
		return 0;
	}
	
	cmdlist_device_remove(cmd_priv);

	data = of_device_get_match_data(&pdev->dev);
	if (data != NULL)
		data->of_device_remove(pdev);

	cmd_priv->device_initialized = 0;
	return 0;
}

static struct platform_driver cmdlist_platform_driver = {
	.probe  = cmdlist_probe,
	.remove = cmdlist_remove,
	.driver = {
		.name  = CMDLIST_DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(device_match_table),
	}
};

static int32_t __init cmdlist_register(void)
{
	return platform_driver_register(&cmdlist_platform_driver);
}

static void __exit cmdlist_unregister(void)
{
	platform_driver_unregister(&cmdlist_platform_driver);
}

module_init(cmdlist_register);
module_exit(cmdlist_unregister);

MODULE_AUTHOR("Graphics Display");
MODULE_DESCRIPTION("Cmdlist Module Driver");
MODULE_LICENSE("GPL");
