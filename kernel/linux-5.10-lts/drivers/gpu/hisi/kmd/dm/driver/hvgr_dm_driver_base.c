/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_dm_driver_base.h"

#include <linux/io.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <securec.h>

#include "hvgr_pm_api.h"
#include "hvgr_gpu_id.h"
#include "hvgr_regmap.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_sch_api.h"
#include "hvgr_dm_hwconfig_issues.h"
#include "hvgr_hts_api.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif

#define THREAD_FEATURES_MAX_REGISTERS_MASK		0xFFFFF
#define THREAD_FEATURES_MAX_TASKQUEUE_MASK		0xFFF00000
#define GPU_VERSION_EU_NUMBER_MASK				0XF


static char *hvgr_irq_name[] = {
	"JOB",
	"MMU",
	"GPU",
	"FCP",
	"CQ",
	"CC",
};

char *hvgr_get_irq_name(int i)
{
	return hvgr_irq_name[i];
}

uint32_t hvgr_get_chipid_reg(void)
{
	return GPU_CONTROL_BASE;
}

void hvgr_get_info_from_reg(struct hvgr_device *gdev)
{
	struct hvgr_reg_info *reg_val = &gdev->dm_dev.dev_reg;

	reg_val->gpu_version = hvgr_read_reg(gdev, gpu_control_reg(gdev, GPU_VERSION));
	reg_val->gpc_present = hvgr_read_reg(gdev, gpu_control_reg(gdev, GPC_PRESENT));
	reg_val->l2_features = hvgr_read_reg(gdev, gpu_control_reg(gdev, L2RW_FEATURES));
	reg_val->mmu_features = hvgr_read_reg(gdev, mmu_control_reg(gdev, MMU_FEATURES));
	reg_val->thread_features = hvgr_read_reg(gdev, misc_control_reg(gdev, MAX_FEATURES));
	reg_val->gpc_cores = reg_val->gpc_present;
#if hvgr_version_ge(HVGR_V350)
	/*
	 * for v350, MAX_FEATURES.max_registers is per EU size.
	 * to make it uniform, we change this to per GPC size.
	 */
	reg_val->thread_features = (reg_val->thread_features & THREAD_FEATURES_MAX_TASKQUEUE_MASK) |
		((reg_val->thread_features & THREAD_FEATURES_MAX_REGISTERS_MASK) *
		(reg_val->gpu_version & GPU_VERSION_EU_NUMBER_MASK));
#endif
	reg_val->thread_max_workgroup_size =
		hvgr_read_reg(gdev, misc_control_reg(gdev, MAX_WORKGROUP_SIZE));
	if (gdev->pm_dev.pm_pwr.gpc_present_mask != 0)
		reg_val->gpc_present &= gdev->pm_dev.pm_pwr.gpc_present_mask;
}

void hvgr_disable_interrupts(struct hvgr_device *gdev)
{
	hvgr_write_reg(gdev, gpu_control_reg(gdev, CC_IRQ_MASK), 0);
	hvgr_write_reg(gdev, gpu_control_reg(gdev, CC_IRQ_CLEAR), CC_IRQ_REG_ALL);

	hvgr_write_reg(gdev, misc_control_reg(gdev, GPU_IRQ_MASK), 0);
	hvgr_write_reg(gdev, misc_control_reg(gdev, GPU_IRQ_CLEAR), 0xFFFFFFFFU);

#ifdef CONFIG_HVGR_MMU_HM
	(void)liblinux_ext_hvgr_mmu_irq_disable();
#else
	hvgr_write_reg(gdev, mmu_control_reg(gdev, MMU_IRQ_MASK), MMU_IRQ_MASK_DISABLE_ALL);
	hvgr_write_reg(gdev, mmu_control_reg(gdev, MMU_IRQ_CLEAR), 0xFFFFFFFFU);
#endif
	hvgr_sch_intr_switch(gdev, HVGR_SCH_INTRDISABLE);
	hvgr_hts_no_consumer_irq_disable(gdev);
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_mcu_lowpa_irq_disable(gdev);
#endif

#if hvgr_version_ge(HVGR_V500)
	hvgr_write_reg(gdev, fcp_mb0_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x0);
	hvgr_write_reg(gdev, fcp_mb0_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
	hvgr_write_reg(gdev, fcp_mb1_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x0);
	hvgr_write_reg(gdev, fcp_mb1_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
	hvgr_write_reg(gdev, fcp_mb2d_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x0);
	hvgr_write_reg(gdev, fcp_mb2d_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
#endif
}

void hvgr_enable_interrupts(struct hvgr_device *gdev)
{
	hvgr_write_reg(gdev, gpu_control_reg(gdev, CC_IRQ_CLEAR), CC_IRQ_REG_ALL);
	hvgr_write_reg(gdev, gpu_control_reg(gdev, CC_IRQ_MASK), CC_IRQ_REG_ALL);

	hvgr_write_reg(gdev, misc_control_reg(gdev, GPU_IRQ_CLEAR), 0xFFFFFFFFU);
	hvgr_write_reg(gdev, misc_control_reg(gdev, GPU_IRQ_MASK), GPU_IRQ_MASK_CONFIG);

#ifdef CONFIG_HVGR_MMU_HM
	(void)liblinux_ext_hvgr_mmu_irq_enable();
#else
	hvgr_write_reg(gdev, mmu_control_reg(gdev, MMU_IRQ_CLEAR), 0xFFFFFFFFU);
	hvgr_write_reg(gdev, mmu_control_reg(gdev, MMU_IRQ_MASK), MMU_IRQ_MASK_ENABLE_ALL);
#endif
	hvgr_sch_intr_switch(gdev, HVGR_SCH_INTRENABLE);

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_info(gdev, HVGR_DM, "stb start in hvgr_enable_interrupts");
	hvgr_info(gdev, HVGR_DM, "stb reg addr 0x%llx is 0x%llx", CQ_IRQ_TOP_CLEAR,cq_control_reg(gdev, CQ_IRQ_TOP_CLEAR));
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_CLEAR), 0x3);
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_MASK), 0x3);
#endif

	hvgr_hts_no_consumer_irq_enable(gdev);
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_mcu_lowpa_irq_enable(gdev);
#endif

#if hvgr_version_ge(HVGR_V500)
	hvgr_write_reg(gdev, fcp_mb0_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
	hvgr_write_reg(gdev, fcp_mb0_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x1);
	hvgr_write_reg(gdev, fcp_mb1_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
	hvgr_write_reg(gdev, fcp_mb1_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x1);
	hvgr_write_reg(gdev, fcp_mb2d_reg(gdev, FCP_MB_MCU_IRQ_CLEAR), 0x1);
	hvgr_write_reg(gdev, fcp_mb2d_reg(gdev, FCP_MB_MCU_IRQ_MASK), 0x1);
#endif
}

uint32_t hvgr_get_core_nums(struct hvgr_device *gdev)
{
	return hweight32(gdev->dm_dev.dev_reg.gpc_present);
}

uint32_t hvgr_get_cores(struct hvgr_device *gdev)
{
	return gdev->dm_dev.dev_reg.gpc_cores;
}

uint32_t hvgr_get_gpu_id(struct hvgr_device *gdev)
{
	return hvgr_read_reg(gdev, gpu_control_reg(gdev, hvgr_get_chipid_reg()));
}

uint32_t hvgr_read_reg(struct hvgr_device * const gdev, uint64_t addr)
{
	return readl((void *)addr);
}

void hvgr_write_reg(struct hvgr_device * const gdev, uint64_t addr, uint32_t value)
{
	writel(value, (void *)addr);
}

void hvgr_fast_pwr_write_reg(struct hvgr_device * const gdev, uint64_t addr, uint32_t value)
{
#if hvgr_version_ge(HVGR_V350)
	uint64_t offset;

	offset = addr - (uint64_t)((gdev)->dm_dev.dev_reg.reg_base[2]) + HVGR_REG_BASE2_LEN;
	if (hvgr_pm_fast_pwr_is_enable(gdev) && gdev->pm_dev.pm_pwr.entire_booting)
		hvgr_pm_fast_pwr_add_reg(gdev, (uint32_t)offset, value);
	else
		hvgr_write_reg(gdev, addr, value);
#else
	hvgr_write_reg(gdev, addr, value);
#endif
}

struct hvgr_reg_base_cfg {
	uint32_t gpu_id;
	struct hvgr_register_module_base reg_base;
};

#include "hvgr_register_cfg.h"

static void hvgr_set_control_baseaddr(struct hvgr_device * const gdev,
	struct hvgr_reg_base_cfg const * const cfg)
{
	gdev->dm_dev.reg_base.gpu_reg_base = cfg->reg_base.gpu_reg_base;
	gdev->dm_dev.reg_base.job_reg_base = cfg->reg_base.job_reg_base;
	gdev->dm_dev.reg_base.mmu_stage1_reg_base = cfg->reg_base.mmu_stage1_reg_base;
	gdev->dm_dev.reg_base.cq_reg_base = cfg->reg_base.cq_reg_base;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_base = cfg->reg_base.fcp_base.fcp_reg_base;
	gdev->dm_dev.reg_base.fcp_base.fcp_itcm_offset = cfg->reg_base.fcp_base.fcp_itcm_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_dtcm_offset = cfg->reg_base.fcp_base.fcp_dtcm_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_itcm_cq_offset =
		cfg->reg_base.fcp_base.fcp_itcm_cq_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_dtcm0_cq_offset =
		cfg->reg_base.fcp_base.fcp_dtcm0_cq_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_dtcm1_cq_offset =
		cfg->reg_base.fcp_base.fcp_dtcm1_cq_offset;

#if hvgr_version_ge(HVGR_V500)
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb0_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb0_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb1_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb1_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb2d_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb2d_offset;
#elif hvgr_version_eq(HVGR_V350)
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb0_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb0_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb1_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb1_offset;
#else
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_mb_offset =
		cfg->reg_base.fcp_base.fcp_reg_mb_offset;
#endif
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_dma_offset =
		cfg->reg_base.fcp_base.fcp_reg_dma_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_timer_offset =
		cfg->reg_base.fcp_base.fcp_reg_timer_offset;
	gdev->dm_dev.reg_base.fcp_base.fcp_reg_uart_offset =
		cfg->reg_base.fcp_base.fcp_reg_uart_offset;
}

static const struct hvgr_hw_product hvgr_hw_products[] = {
	{ GPU_ID2_PRODUCT_TV200,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv200_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV210,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv210_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV211,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv211_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV300,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv300_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV310,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv300_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV350,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv350_r1p0 },
			{ U32_MAX, NULL } } },
	{ GPU_ID2_PRODUCT_TV500,
		{ { gpu_id2_version_make(1, 0, 0), hvgr_hw_issues_tv500_r1p0 },
			{ U32_MAX, NULL } } },
};

static const struct hvgr_hw_product *hvgr_hw_get_product(
	struct hvgr_device * const gdev)
{
	u32 gpu_id = gdev->dm_dev.dev_reg.gpu_id;
	const u32 product_model = gpu_id & GPU_ID1_PRODUCT_MODEL;
	const struct hvgr_hw_product *product = NULL;
	size_t i;

	for (i = 0; i < ARRAY_SIZE(hvgr_hw_products); ++i) {
		if (product_model == hvgr_hw_products[i].product_model) {
			product = &hvgr_hw_products[i];
			break;
		}
	}
	return product;
}

static const enum hvgr_hw_issue *hvgr_hw_get_issues_for_new_id(
	struct hvgr_device * const gdev)
{
	const enum hvgr_hw_issue *issue = NULL;
	u32 gpu_id = gdev->dm_dev.dev_reg.gpu_id;
	const struct hvgr_hw_product *product = NULL;
	const u32 gpu_version = gpu_id & GPU_ID1_VERSION;
	size_t i;

	product = hvgr_hw_get_product(gdev);
	if (product == NULL)
		return issue;

	for (i = 0; product->map[i].version != U32_MAX; ++i) {
		if (gpu_version == product->map[i].version) {
			issue = product->map[i].issues;
			break;
		}
	}
	return issue;
}

int hvgr_hw_set_issues_mask(struct hvgr_device * const gdev)
{
	const enum hvgr_hw_issue *issues = NULL;
	u32 gpu_id;

	gpu_id = gdev->dm_dev.dev_reg.gpu_id;

	issues = hvgr_hw_get_issues_for_new_id(gdev);
	if (issues == NULL) {
		hvgr_err(gdev, HVGR_DM, "Unknown GPU ID %x", gpu_id);
		return -EINVAL;
	}

	hvgr_info(gdev, HVGR_DM,
		"GPU identified as 0x%x arch %u.%u.%u r%up%u status %u",
		(gpu_id & GPU_ID1_PRODUCT_MAJOR) >> GPU_ID1_PRODUCT_MAJOR_SHIFT,
		(gpu_id & GPU_ID1_ARCH_MAJOR) >> GPU_ID1_ARCH_MAJOR_SHIFT,
		(gpu_id & GPU_ID1_ARCH_MINOR) >> GPU_ID1_ARCH_MINOR_SHIFT,
		(gpu_id & GPU_ID1_ARCH_REV) >> GPU_ID1_ARCH_REV_SHIFT,
		(gpu_id & GPU_ID1_VERSION_MAJOR) >> GPU_ID1_VERSION_MAJOR_SHIFT,
		(gpu_id & GPU_ID1_VERSION_MINOR) >> GPU_ID1_VERSION_MINOR_SHIFT,
		(gpu_id & GPU_ID1_VERSION_STATUS) >>
			GPU_ID1_VERSION_STATUS_SHIFT);

	for (; *issues < HVGR_HW_ISSUE_END; issues++)
		set_bit(*issues, &gdev->hw_issues_mask[0]);

	return 0;
}


int hvgr_init_control_baseaddr(struct hvgr_device * const gdev)
{
	struct hvgr_reg_info * const reg_info = &gdev->dm_dev.dev_reg;
	uint32_t idx;
	uint32_t gpu_id;

	if (reg_info->reg_base[0] == NULL ||
		reg_info->reg_base[1] == NULL ||
		reg_info->reg_base[2] == NULL) {
		hvgr_err(gdev, HVGR_DM, "%s reg_base is NULL.", __func__);
		return -1;
	}

	gdev->dm_dev.dev_reg.gpu_id = hvgr_get_gpu_id(gdev);
	gpu_id = gdev->dm_dev.dev_reg.gpu_id & GPU_ID1_PRODUCT_MODEL;
	for (idx = 0; idx < ARRAY_SIZE(product_reg_cfg); idx++) {
		if (gpu_id == product_reg_cfg[idx].gpu_id) {
			hvgr_set_control_baseaddr(gdev, &product_reg_cfg[idx]);
			return 0;
		}
	}
	hvgr_err(gdev, HVGR_DM, "%s not match cfg.", __func__);
	return -1;
}

irqreturn_t hvgr_gpu_irq(int irq, void *data)
{
	struct hvgr_device *gdev = data;
	u32 val;

	if (hvgr_pm_get_gpu_status(gdev) == HVGR_PM_POWER_OFF) {
		hvgr_info(gdev, HVGR_DM, "gpu pm status 0x%x.", hvgr_pm_get_gpu_status(gdev));
		return IRQ_NONE;
	}

	val = hvgr_read_reg(gdev, misc_control_reg(gdev, GPU_IRQ_STATUS));
	hvgr_info(gdev, HVGR_DM, "GPU_IRQ_STATUS 0x%x.", val);
	if (val == 0)
		return IRQ_NONE;

	if (val & GPU_FAULT_SET)
		hvgr_gpu_fault_proc(gdev, val);

	if (val & PRFCNT_SAMPLE_COMPLETED)
		gdev->datan_dev.datan_ct_dev.int_done = true;

	hvgr_write_reg(gdev, misc_control_reg(gdev, GPU_IRQ_CLEAR), val);
	return IRQ_HANDLED;
}

uint64_t hvgr_dm_get_soc_timestamp(struct hvgr_device * const gdev)
{
	uint64_t soc_cnt = 0;
	uint32_t hi1, hi2;

	/* Read hi, lo, hi to ensure that overflow from lo to hi is handled correctly */
	do {
		hi1 = hvgr_read_reg(gdev, misc_control_reg(gdev, SYSCNT_HI));
		soc_cnt = hvgr_read_reg(gdev, misc_control_reg(gdev, SYSCNT_LO));
		hi2 = hvgr_read_reg(gdev, misc_control_reg(gdev, SYSCNT_HI));
		soc_cnt |= (((uint64_t)hi1) << 32);
	} while (hi1 != hi2);
	return soc_cnt;
}

bool hvgr_is_fpga(struct hvgr_device * const gdev)
{
	unsigned long gpu_version = gdev->dm_dev.dev_reg.gpu_version;

	/* bit5 is fpga flag. */
	return test_bit(5, &gpu_version);
}

bool hvgr_is_fake_hang(struct hvgr_device * const gdev)
{
	uint32_t index;

	if (gdev->datan_dev.gpu_fault_msg.gpu_fault_id != DMD_JOB_HANG)
		return false;

	for (index = 0; index < CQ_CHANNEL_NUM; index++) {
		if (hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(index))) != 0)
			return false;
	}

	return true;
}

bool hvgr_is_report_dmd(struct hvgr_device * const gdev)
{
	if (hvgr_is_fake_hang(gdev)) {
		hvgr_debug(gdev, HVGR_DM, "Fake hang detected.");
		return false;
	}

	if (gdev->datan_dev.gpu_fault_msg.gpu_fault_id == DMD_ENTRY_TIMEOUT)
		return false;

	return true;
}
