/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Base driver for Crypto Core, including initialization.
 * Create: 2019/11/11
 */

#include "crypto_core_internal.h"
#include <platform_include/see/efuse_driver.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <platform_include/basicplatform/linux/partition/partition_ap_kernel.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <linux/kthread.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_reserved_mem.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <securec.h>
#include "crypto_core_at.h"
#include "crypto_core_errno.h"
#include "crypto_core_factory.h"
#include "crypto_core_flash.h"
#include "crypto_core_fs.h"
#include "crypto_core.h"
#include "crypto_core_power.h"
#include "crypto_core_smc.h"
#ifdef CONFIG_CRYPTO_CORE_FFA_SUPPORT
#include <platform_include/see/ffa/ffa_rpmbmspc.h>
#endif

#define MSPC_ERRCODE_MAXLEN                 12

/* Begin of msp core file conctrol define. */
#ifdef CONFIG_DFX_DEBUG_FS
static DEVICE_ATTR(mspc_power, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP),
		   mspc_powerctrl_show, mspc_powerctrl_store);
static DEVICE_ATTR(mspc_check_ready, (S_IRUSR | S_IRGRP),
		   mspc_check_ready_show, NULL);
#endif /* CONFIG_DFX_DEBUG_FS */

static DEVICE_ATTR(mspc_at_ctrl, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP),
		   mspc_at_ctrl_show, mspc_at_ctrl_store);

/* End of msp core file conctrol define. */

struct mspc_module_data g_mspc_data;

int32_t mspc_filectrl_preprocess(const int8_t *buf, size_t count)
{
	if (unlikely(!buf || count > MSPC_CMD_MAX_LEN)) {
		pr_err("%s:Invalid param:%lu\n", __func__, count);
		return	MSPC_INVALID_PARAM_ERROR;
	}

	return MSPC_OK;
}

void mspc_lock(void)
{
	mutex_lock(&g_mspc_data.mspc_mutex);
}

void mspc_unlock(void)
{
	mutex_unlock(&g_mspc_data.mspc_mutex);
}

void mspc_record_errno(int32_t error)
{
	atomic_set(&g_mspc_data.errno, error);
}

int32_t mspc_get_recorded_errno(void)
{
	return atomic_read(&g_mspc_data.errno);
}

struct device *mspc_get_device(void)
{
	return g_mspc_data.device;
}

uint32_t crypto_core_get_sm_efuse_pos(void)
{
	return g_mspc_data.sm_efuse_pos;
}

static int32_t mspc_remove(struct platform_device *pdev)
{
	if (g_mspc_data.device) {
#ifdef CONFIG_DFX_DEBUG_FS
		device_remove_file(g_mspc_data.device, &dev_attr_mspc_power);
		device_remove_file(g_mspc_data.device,
				   &dev_attr_mspc_check_ready);
#endif
		device_remove_file(g_mspc_data.device,
				   &dev_attr_mspc_at_ctrl);
		of_reserved_mem_device_release(g_mspc_data.device);
	}
	return 0;
}

static int32_t mspc_create_files(struct device *pdevice)
{
	int32_t ret = MSPC_OK;

	if (device_create_file(pdevice, &dev_attr_mspc_at_ctrl) != 0) {
		ret = MSPC_AT_CTRL_NODE_CREATE_ERROR;
		pr_err("%s:create mspc_at_ctrl file failed!\n", __func__);
		goto err_exit;
	}

#ifdef CONFIG_DFX_DEBUG_FS
	if (device_create_file(pdevice, &dev_attr_mspc_power) != 0) {
		ret = MSPC_POWER_NODE_CREATE_ERROR;
		pr_err("%s:create mspc_power file failed!\n", __func__);
		goto err_device_remove_file1;
	}

	if (device_create_file(pdevice, &dev_attr_mspc_check_ready) != 0) {
		ret = MSPC_POWER_NODE_CREATE_ERROR;
		pr_err("%s:create mspc_check_ready file failed!\n", __func__);
		goto err_device_remove_file2;
	}

	return ret;
err_device_remove_file2:
	device_remove_file(pdevice, &dev_attr_mspc_power);
err_device_remove_file1:
	device_remove_file(pdevice, &dev_attr_mspc_at_ctrl);
#endif
err_exit:
	return ret;
}

typedef union {
	uint16_t result;
	struct {
		uint8_t module_id;
		uint8_t err_code;
	} record;
} mspc_dft_record_info;

#define MSPC_RECORD_NUM_MAX       32
#define MSPC_BL2_RECORD_SUCCESS   0xA5A5
static void mspc_check_bl2_record(void)
{
	int32_t ret, i, size, num;
	mspc_dft_record_info *mspc_dft_record = NULL;

	num = mspc_send_smc_no_ack((uint64_t)MSPC_FN_MAIN_SERVICE_CMD,
					   (uint64_t)MSPC_SMC_GET_BL2_RECORD_NUM, 0, 0);
	if (num > MSPC_RECORD_NUM_MAX || num <= 0) {
		pr_err("%s get record num, %d\n", __func__, num);
		return;
	}

	size = num * sizeof(mspc_dft_record_info);
	if (size < 0) {
		pr_err("%s size err, %d\n", __func__, size);
		return;
	}

	mspc_dft_record = kzalloc(size, GFP_KERNEL);
	if (!mspc_dft_record) {
		pr_err("%s kmalloc fail\n", __func__);
		return;
	}

#ifdef CONFIG_CRYPTO_CORE_FFA_SUPPORT
	ret = mspc_send_smc_with_shmem_no_ack((uint64_t)MSPC_FN_MAIN_SERVICE_CMD,
					   (uint64_t)MSPC_SMC_GET_BL2_RECORD, (char *)mspc_dft_record,
					   size, SHMEM_OUT);
	if (ret != MSPC_OK) {
		pr_err("%s get record failed, 0x%x\n", __func__, ret);
		goto out;
	}
#else
	ret = memset_s(mspc_dft_record, size, 0, size);
	if (ret != EOK) {
		pr_err("%s undefined ffa_support, memset fail, 0x%x\n", __func__, ret);
		goto out;
	}
#endif

	for (i = 0; i < num; i++) {
		if (mspc_dft_record[i].result == 0)
			continue;

		if (mspc_dft_record[i].result != MSPC_BL2_RECORD_SUCCESS) {
			pr_err("%s %d failed\n", __func__, i);
			bbox_diaginfo_record(MSPC_DIAG_INFO, NULL,
					"MSPC %d failed, module:0x%x, err_code: 0x%x", i,
					mspc_dft_record[i].record.module_id,
					mspc_dft_record[i].record.err_code);
		}
	}

out:
	kfree(mspc_dft_record);
}

static int32_t __init mspc_probe(struct platform_device *pdev)
{
	struct device *pdevice = &(pdev->dev);
	int32_t ret;

	(void)memset_s((void *)&g_mspc_data,
		       sizeof(g_mspc_data), 0, sizeof(g_mspc_data));

	g_mspc_data.device = pdevice;

	/* Initialize mutex, semaphore etc.. */
	mutex_init(&(g_mspc_data.mspc_mutex));
	mspc_init_smc();
	mspc_init_power();
	mspc_factory_init();

	/* Aassign a reserved CMA memory region to mspc. */
	ret = of_reserved_mem_device_init(pdevice);
	if (ret != 0) {
		pr_err("%s:Register mspc shared cma failed!\n", __func__);
		mspc_record_errno(MSPC_CMA_DEVICE_INIT_ERROR);
		return ret;
	}
	pr_err("mspc shared cma registere successful!\n");

	/* Create mspc files. */
	ret = mspc_create_files(pdevice);
	if (ret != MSPC_OK) {
		pr_err("%s:create files failed!\n", __func__);
		goto err_mem_release;
	}

	mspc_flash_register_func(mspc_flash_debug_switchs);
	pr_err("mspc module init successful!\n");
	mspc_record_errno(ret);
	mspc_check_bl2_record();
	return 0;

err_mem_release:
	of_reserved_mem_device_release(g_mspc_data.device);

	mspc_record_errno(ret);
	return ret;
}

static const struct of_device_id mspc_of_match[] = {
	{.compatible = "hisilicon,mspc-device" },
	{ }
};
MODULE_DEVICE_TABLE(of, mspc_of_match);

static struct platform_driver mspc_driver = {
	.probe = mspc_probe,
	.remove = mspc_remove,
	.driver = {
		.name  = MSPC_DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(mspc_of_match),
	},
};

static int32_t __init mspc_module_init(void)
{
	int32_t ret = 0;

	ret = platform_driver_register(&mspc_driver);

	if (ret)
		pr_err("register mspc driver failed.\n");

	return ret;
}
static void __exit mspc_module_exit(void)
{
	platform_driver_unregister(&mspc_driver);
}
module_init(mspc_module_init);
module_exit(mspc_module_exit);

MODULE_DESCRIPTION("MSP CORE KERNEL DRIVER");
MODULE_LICENSE("GPL V2");
