/*
* hwts_driver.c
*
* about hwts driver
*
* Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
*/
#include "hwts_driver.h"
#include "npu_hwts_plat.h"
#include "npu_soc_defines.h"
#include "npu_log.h"

static void hwts_driver_aic_aiv_pool_enable(void)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_AIC_POOL_ENABLE_CTRL_UNION aic_pool_en = { 0 };
	SOC_NPU_HWTS_HWTS_AIV_POOL_ENABLE_CTRL_UNION aiv_pool_en = { 0 };

	cond_return_void(hwts_base == 0ULL, "hwts_base is NULL\n");

	aiv_pool_en.reg.aiv_enable_ctrl = 1;
	writel(aiv_pool_en.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIV_POOL_ENABLE_CTRL_ADDR(hwts_base, 0));
	aic_pool_en.reg.aic_enable_ctrl = 1;
	writel(aic_pool_en.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIC_POOL_ENABLE_CTRL_ADDR(hwts_base, 0));
}

void hwts_driver_hwts_task_terminate(uint16_t rtsq)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_P0_TASK_CTRL_UNION task_ctrl = { 0 };

	cond_return_void(hwts_base == 0ULL, "hwts_base is NULL\n");

	task_ctrl.reg.p0_task_terminate = 0x1;
	writel(task_ctrl.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_TASK_CTRL_ADDR(hwts_base, rtsq));
	npu_drv_warn("write hwts task ctrl:value:0x%x", task_ctrl.value);

	hwts_driver_aic_aiv_pool_enable();
}

void hwts_driver_clear_channel_sq_en(uint16_t channel_id)
{
	uint64_t hwts_base_addr = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG3_UNION cq_cfg3 = { 0 };
	uint64_t hwts_reg_addr = SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG3_ADDR(hwts_base_addr, channel_id);
	
	cond_return_void(hwts_base_addr == 0ULL, "hwts_base_addr is NULL\n");

	cq_cfg3.value = readl((const volatile void *)(uintptr_t)hwts_reg_addr);
	cq_cfg3.reg.p0_rtsq_en = 0;
	writel(cq_cfg3.value, (volatile void *)(uintptr_t)hwts_reg_addr);
}

void hwts_driver_reset_cq_head(uint16_t cq_idx)
{
	uint16_t cq_tail;
	SOC_NPU_HWTS_HWTS_P0_RTCQ_CFG1_UNION cq_cfg1 = { 0 };
	SOC_NPU_HWTS_HWTS_P0_RTCQ_CFG0_UNION cq_cfg0 = { 0 };
	uint64_t hwts_base_addr = npu_hwts_get_base_addr();

	cond_return_void(hwts_base_addr == 0ULL, "hwts_base_addr is NULL\n");

	cq_cfg1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTCQ_CFG1_ADDR(hwts_base_addr, cq_idx));
	cq_tail = cq_cfg1.reg.p0_rtcq_tail;

	cq_cfg0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTCQ_CFG0_ADDR(hwts_base_addr, cq_idx));
	cq_cfg0.reg.p0_rtcq_head = cq_tail;
	writel(cq_cfg0.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTCQ_CFG0_ADDR(hwts_base_addr, cq_idx));
	return;
}