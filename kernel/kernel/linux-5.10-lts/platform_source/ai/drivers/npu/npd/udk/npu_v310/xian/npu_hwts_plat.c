/*
 * npu_hwts_plat.c
 *
 * about npu hwts plat
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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
#include "npu_hwts_plat.h"

#include "npu_osal.h"
#include "npu_log.h"
#include "npu_aicore_plat.h"
#include "npu_common_resource.h"
#include "npu_exception.h"
#include "soc_npu_hwts_interface.h"
#include "npu_soc_defines.h"

u64 npu_hwts_get_base_addr(void)
{
	u64 hwts_base = 0ULL;
	struct npu_platform_info *plat_info = npu_plat_get_info();

	cond_return_error(plat_info == NULL, 0ULL, "get platform info failed\n");
	hwts_base = (u64)(uintptr_t)plat_info->dts_info.reg_vaddr[NPU_REG_HWTS_BASE];
	npu_drv_debug("hwts_base = 0x%pK\n", hwts_base);
	return hwts_base;
}

u16 hwts_get_acsq_id_by_rtsq_id(u16 channel_id)
{
	SOC_NPU_HWTS_HWTS_P0_RTSQ_TO_ACSQ_MAP0_UNION map0 = {0};
	SOC_NPU_HWTS_HWTS_P0_RTSQ_TO_ACSQ_MAP1_UNION map1 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, UINT16_MAX, "hwts_base is NULL\n");

	map0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_TO_ACSQ_MAP0_ADDR(hwts_base, channel_id));
	map1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_TO_ACSQ_MAP1_ADDR(hwts_base, channel_id));

	if (map1.reg.p0_rtsq_to_acsqid_map_vld == 0) {
		npu_drv_info("rtsq id %u is not activated", channel_id);
		return UINT16_MAX;
	}

	return map0.reg.p0_rtsq_to_acsqid_map;
}

int npu_hwts_query_aic_pool_status(
	u8 index, u8 *reg_val)
{
	SOC_NPU_HWTS_HWTS_AIC_POOL_STATUS_UNION aic_pool_status = {0};

	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	aic_pool_status.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIC_POOL_STATUS_ADDR(hwts_base, 0));
	*reg_val = aic_pool_status.reg.aic_enabled_status;

	npu_drv_warn("HWTS_AICORE_POOL_STATUS%u_NS = 0x%x\n", index, *reg_val);

	return 0;
}

int npu_hwts_query_aiv_pool_status(
	u8 index, u8 *reg_val)
{
	SOC_NPU_HWTS_HWTS_AIV_POOL_STATUS_UNION aiv_pool_status = {0};

	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	aiv_pool_status.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIV_POOL_STATUS_ADDR(hwts_base, 0));
	*reg_val = aiv_pool_status.reg.aiv_enabled_status;
	npu_drv_warn("HWTS_VEC_POOL_STATUS%u_NS = 0x%x\n", index, *reg_val);
	return 0;
}

int npu_hwts_query_sw_status(u16 channel_id, u32 *reg_val)
{
	SOC_NPU_HWTS_HWTS_RTSQ_SW_STATUS_UNION rtsq_sw_status = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	rtsq_sw_status.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_RTSQ_SW_STATUS_ADDR(hwts_base, channel_id));
	*reg_val = rtsq_sw_status.reg.rtsq_sw_status;
	npu_drv_warn("HWTS_BUS_ERR_INFO.rtsq_sw_status = 0x%08x\n",
			rtsq_sw_status.reg.rtsq_sw_status);
	return 0;
}

int npu_hwts_query_sq_head(u16 channel_id, u16 *reg_val)
{
	SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG2_UNION rtsq_cfg2 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	rtsq_cfg2.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG2_ADDR(hwts_base, channel_id));
	*reg_val = rtsq_cfg2.reg.p0_rtsq_head;
	npu_drv_warn("HWTS_P0_RTSQ_CFG2.p0_rtsq_head = 0x%04x\n", rtsq_cfg2.reg.p0_rtsq_head);
	return 0;
}

int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val)
{
	u16 acsq_id;
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_UNION acsq_fsm_dfx_ctrl0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");
	if (!is_channel_id_valid(channel_id))
		return -1;

	acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	cond_return_error(acsq_id == UINT16_MAX, 0, "invalid acsq id\n");
	acsq_fsm_dfx_ctrl0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_ADDR(hwts_base, acsq_id));
	*reg_val = acsq_fsm_dfx_ctrl0.reg.sqe_type;
	npu_drv_warn("SQCQ_FSM_MISC_STATE.sqe_type = 0x%x\n",
			acsq_fsm_dfx_ctrl0.reg.sqe_type);
	return 0;
}

static int npu_hwts_query_ip_own_bitmap(u16 channel_id, u8 *reg_val)
{
	u16 acsq_id;
	SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_UNION acsq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	cond_return_error(acsq_id == UINT16_MAX, 0, "invalid acsq id\n");
	acsq_fsm_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_ADDR(hwts_base, acsq_id));
	*reg_val = acsq_fsm_state.reg.ip_own_bitmap;
	npu_drv_warn("SQCQ_FSM_IP_OWN_STATE.ip_own_bitmap = 0x%x\n",
			acsq_fsm_state.reg.ip_own_bitmap);
	return 0;
}

static int npu_hwts_query_ip_exception_bitmap(u16 channel_id, u8 *reg_val)
{
	u16 acsq_id;
	SOC_NPU_HWTS_ACSQ_FSM_IP_EXCEPTION_STATE_UNION acsq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	cond_return_error(acsq_id == UINT16_MAX, 0, "invalid acsq id\n");
	acsq_fsm_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_EXCEPTION_STATE_ADDR(hwts_base, acsq_id));
	*reg_val = acsq_fsm_state.reg.ip_exception_bitmap;
	npu_drv_warn("SQCQ_FSM_IP_EXCEPTION_STATE.ip_exception_bitmap = 0x%x\n",
			acsq_fsm_state.reg.ip_exception_bitmap);
	return 0;
}

static int npu_hwts_query_ip_trap_bitmap(u16 channel_id, u8 *reg_val)
{
	u16 acsq_id;
	SOC_NPU_HWTS_ACSQ_FSM_IP_TRAP_STATE_UNION acsq_fsm_state = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(reg_val == NULL, -EINVAL, "reg_val is NULL\n");

	acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	cond_return_error(acsq_id == UINT16_MAX, 0, "invalid acsq id\n");
	acsq_fsm_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_TRAP_STATE_ADDR(hwts_base, acsq_id));
	*reg_val = acsq_fsm_state.reg.ip_trap_bitmap;
	npu_drv_warn("SQCQ_FSM_IP_TRAP_STATE.aic_trap_bitmap = 0x%x\n",
			acsq_fsm_state.reg.ip_trap_bitmap);
	return 0;
}

int npu_hwts_query_aicore_own_bitmap(u16 channel_id, u8 *reg_val)
{
	return npu_hwts_query_ip_own_bitmap(channel_id, reg_val);
}

int npu_hwts_query_aicore_exception_bitmap(u16 channel_id, u8 *reg_val)
{
	return npu_hwts_query_ip_exception_bitmap(channel_id, reg_val);
}

int npu_hwts_query_aicore_trap_bitmap(u16 channel_id, u8 *reg_val)
{
	return npu_hwts_query_ip_trap_bitmap(channel_id, reg_val);
}

int npu_hwts_query_aic_task_config(void)
{
	u64 value;
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");

	value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIC_FSM0_DFX0_ADDR(hwts_base, 0));
	npu_drv_warn("HWTS_AIC_BLK_FSM_STATE = 0x%llx\n", value);

	return 0;
}

int npu_hwts_query_aiv_task_config(void)
{
	u64 value;
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0, -EINVAL, "hwts_base is NULL\n");

	value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIV_FSM0_DFX0_ADDR(hwts_base, 0));
	npu_drv_warn("HWTS_AIV_BLK_FSM_STATE = 0x%llx\n", value);

	return 0;
}

int npu_hwts_query_interrupt_info(
	struct hwts_interrupt_info *interrupt_info)
{
	SOC_NPU_HWTS_HWTS_GLB_CTRL3_UNION glb_ctrl3 = {0};
	SOC_NPU_HWTS_HWTS_GLB_CTRL4_UNION glb_ctrl4 = {0};
	SOC_NPU_HWTS_HWTS_L1_NORMAL_NS_INTERRUPT_UNION l1_normal_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_DEBUG_NS_INTERRUPT_UNION l1_debug_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_ERROR_NS_INTERRUPT_UNION l1_error_ns_interrupt = {0};
	SOC_NPU_HWTS_HWTS_L1_DFX_INTERRUPT_UNION l1_dfx_interrupt = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(interrupt_info == NULL,
		-EINVAL, "interrupt_info is NULL\n");

	glb_ctrl3.value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_GLB_CTRL3_ADDR(hwts_base));
	interrupt_info->aic_task_runtime_limit_exp =
		glb_ctrl3.reg.aic_task_runtime_limit_exp;
	interrupt_info->aiv_task_runtime_limit_exp =
		glb_ctrl3.reg.aiv_task_runtime_limit_exp;
	interrupt_info->notify_wait_task_runtime_limit_exp = glb_ctrl3.reg.notify_wait_task_runtime_limit_exp;
	npu_drv_warn("HWTS_GLB_CTRL3.aic_task_runtime_limit_exp = 0x%x\n",
		glb_ctrl3.reg.aic_task_runtime_limit_exp);
	npu_drv_warn("HWTS_GLB_CTRL3.aiv_task_runtime_limit_exp = 0x%x\n",
		glb_ctrl3.reg.aiv_task_runtime_limit_exp);
	npu_drv_warn("HWTS_GLB_CTRL3.wait_task_runtime_limit_exp = 0x%x\n", glb_ctrl3.reg.notify_wait_task_runtime_limit_exp);

		glb_ctrl4.value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_GLB_CTRL4_ADDR(hwts_base));
	interrupt_info->eff_aic_task_runtime_limit_exp =
		glb_ctrl4.reg.eff_aic_task_runtime_limit_exp;
	npu_drv_warn("HWTS_GLB_CTRL4.eff_aic_task_runtime_limit_exp = 0x%x\n",
		glb_ctrl4.reg.eff_aic_task_runtime_limit_exp);

	l1_normal_ns_interrupt.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_L1_NORMAL_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_normal_ns_interrupt = l1_normal_ns_interrupt.value;
	npu_drv_warn("HWTS_L1_NORMAL_NS_INTERRUPT.value = 0x%x\n",
			l1_normal_ns_interrupt.value);

	l1_debug_ns_interrupt.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_L1_DEBUG_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_debug_ns_interrupt = l1_debug_ns_interrupt.value;
	npu_drv_warn("HWTS_L1_DEBUG_NS_INTERRUPT.value = 0x%x\n",
			l1_debug_ns_interrupt.value);

	l1_error_ns_interrupt.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_L1_ERROR_NS_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_error_ns_interrupt = l1_error_ns_interrupt.value;
	npu_drv_warn("HWTS_L1_ERROR_NS_INTERRUPT.value = 0x%x\n",
			l1_error_ns_interrupt.value);

	l1_dfx_interrupt.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_L1_DFX_INTERRUPT_ADDR(hwts_base));
	interrupt_info->l1_dfx_interrupt = l1_dfx_interrupt.value;
	npu_drv_warn("HWTS_L1_DFX_INTERRUPT.value = 0x%x\n",
			l1_dfx_interrupt.value);

	return 0;
}

static void npu_hwts_query_acsq_info(u64 hwts_base,struct sq_exception_info *acsq_info)
{
	SOC_NPU_HWTS_HWTS_RTSQ_CFG4_UNION rtsq_cfg4 = {0};
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL2_UNION acsq_fsm_dfx_ctrl2 = {0};
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL3_UNION acsq_fsm_dfx_ctrl3 = {0};
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_UNION acsq_fsm_dfx_ctrl0 = {0};
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL1_UNION acsq_fsm_dfx_ctrl1 = {0};
	SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_UNION acsq_fsm_ip_own_state = {0};
	u16 acsq_id = acsq_info->acsq_id;
	rtsq_cfg4.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_RTSQ_CFG4_ADDR(hwts_base, acsq_id));
	acsq_info->sq_length = rtsq_cfg4.reg.rtsq_length;

	acsq_fsm_dfx_ctrl2.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL2_ADDR(hwts_base, acsq_id));
	acsq_info->sqcq_fsm_state = acsq_fsm_dfx_ctrl2.reg.acsq_fsm_state;

	acsq_fsm_dfx_ctrl3.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL3_ADDR(hwts_base, acsq_id));
	acsq_info->sq_prefetch_busy = acsq_fsm_dfx_ctrl3.reg.acsq_prefetch_busy;

	acsq_fsm_dfx_ctrl0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_ADDR(hwts_base, acsq_id));
	acsq_info->sqe_type = acsq_fsm_dfx_ctrl0.reg.sqe_type;

	acsq_fsm_dfx_ctrl1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL1_ADDR(hwts_base, acsq_id));
	acsq_info->blk_dim = acsq_fsm_dfx_ctrl1.reg.block_dim;

	acsq_fsm_ip_own_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_ADDR(hwts_base, acsq_id));

	npu_drv_warn("HWTS_RTSQ_CFG4.rtsq_length = 0x%04x\n", rtsq_cfg4.reg.rtsq_length);
	npu_drv_warn("ACSQ_FSM_DFX_CTRL.reg.acsq_fsm_state = 0x%04x\n", acsq_fsm_dfx_ctrl2.reg.acsq_fsm_state);
	npu_drv_warn("HWTS_SQCQ_FSM_STATE1.reg.acsq_prefetch_busy = 0x%04x\n", acsq_fsm_dfx_ctrl3.reg.acsq_prefetch_busy);
	npu_drv_warn("HWTS_SQCQ_FSM_MISC_STATE0.reg.sqe_type = 0x%04x\n", acsq_fsm_dfx_ctrl0.reg.sqe_type);
	npu_drv_warn("HWTS_SQCQ_FSM_MISC_STATE1.reg.block_dim = 0x%04x\n", acsq_fsm_dfx_ctrl1.reg.block_dim);
	npu_drv_warn("acsq_fsm_ip_own_state.reg.ip_own_bitmap = 0x%04x\n", acsq_fsm_ip_own_state.reg.ip_own_bitmap);
}

int npu_hwts_query_sq_info(u16 channel_id, struct sq_exception_info *sq_info)
{
	SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG1_UNION rtsq_cfg1 = {0};
	SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG2_UNION rtsq_cfg2 = {0};
	SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG3_UNION rtsq_cfg3 = {0};
	SOC_NPU_HWTS_HWTS_P0_RTSQ_DFX0_UNION rtsq_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();

	cond_return_error(hwts_base == 0ULL, -EINVAL, "hwts_base is NULL\n");
	cond_return_error(sq_info == NULL, -EINVAL, "interrupt_info is NULL\n");

	rtsq_cfg1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG1_ADDR(hwts_base, channel_id));
	sq_info->sq_tail = rtsq_cfg1.reg.p0_rtsq_tail;
	npu_drv_warn("HWTS_P0_RTSQ_CFG1.sq_tail = 0x%04x\n", rtsq_cfg1.reg.p0_rtsq_tail);

	rtsq_cfg2.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG2_ADDR(hwts_base, channel_id));
	sq_info->sq_head = rtsq_cfg2.reg.p0_rtsq_head;
	npu_drv_warn("HWTS_P0_RTSQ_CFG2.sq_head = 0x%04x\n", rtsq_cfg2.reg.p0_rtsq_head);

	rtsq_cfg3.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_CFG3_ADDR(hwts_base, channel_id));
	npu_drv_warn("Channel %u enable status = %u\n", channel_id, rtsq_cfg3.reg.p0_rtsq_en);

	rtsq_dfx0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_DFX0_ADDR(hwts_base, channel_id));
	npu_drv_warn("Channel %u p0_rtsq_fsm_state = %u\n", channel_id, rtsq_dfx0.reg.p0_rtsq_fsm_state);

	sq_info->acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	if (sq_info->acsq_id != UINT16_MAX) {
		npu_drv_warn("channel id %u is mapped to %u", channel_id, sq_info->acsq_id);
		npu_hwts_query_acsq_info(hwts_base,sq_info);
	} else {
		npu_drv_warn("channel id %u is not activated", channel_id);
	}
	if ((sq_info->sq_head < sq_info->sq_tail) &&
		(rtsq_cfg3.reg.p0_rtsq_en == 1) &&
		(rtsq_dfx0.reg.p0_rtsq_fsm_state != 0x9)) {
		return 1;
	}
	return 0;
}

int npu_hwts_polling_aic_fsm_state(u64 timeout)
{
	SOC_NPU_HWTS_HWTS_AIC_FSM0_DFX0_UNION aic_fsm0_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");

	do {
		aic_fsm0_dfx0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIC_FSM0_DFX0_ADDR(hwts_base, 0));
		if (aic_fsm0_dfx0.reg.aic_fsm0_state == 0x4 && // 4'h4: WAIT_EXCEPTION_HANDLE
			aic_fsm0_dfx0.reg.aic_fsm0_rd_ost_cnt == 0 &&
			aic_fsm0_dfx0.reg.aic_fsm0_pmu_ost_cnt == 0 &&
			aic_fsm0_dfx0.reg.aic_fsm0_log_ost_cnt == 0 &&
			aic_fsm0_dfx0.reg.aic_fsm0_cfg_ost_cnt == 0)
			return 0;

		usleep_range(1, 1);
		timeout--;
	} while (timeout > 0);

	npu_drv_err("polling HWTS_AIC_FSM0_DFX0 TIMEOUT\n");
	return -1;
}

int npu_hwts_polling_aiv_fsm_state(u64 timeout)
{
	SOC_NPU_HWTS_HWTS_AIV_FSM0_DFX0_UNION aiv_fsm0_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");

	do {
		aiv_fsm0_dfx0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIV_FSM0_DFX0_ADDR(hwts_base, 0));
		if (aiv_fsm0_dfx0.reg.aiv_fsm0_state == 0x4 && // 4'h4: WAIT_EXCEPTION_HANDLE
			aiv_fsm0_dfx0.reg.aiv_fsm0_rd_ost_cnt == 0 &&
			aiv_fsm0_dfx0.reg.aiv_fsm0_cxt_sw_ost_cnt == 0 &&
			aiv_fsm0_dfx0.reg.aiv_fsm0_pmu_ost_cnt == 0 &&
			aiv_fsm0_dfx0.reg.aiv_fsm0_log_ost_cnt == 0 &&
			aiv_fsm0_dfx0.reg.aiv_fsm0_cfg_ost_cnt == 0)
			return 0;

		usleep_range(1, 1);
		timeout--;
	} while (timeout > 0);

	npu_drv_err("polling HWTS_AIV_FSM0_DFX0 TIMEOUT\n");
	return -1;
}

int npu_hwts_polling_acsq_fsm_state(u16 acsq_id, u64 timeout)
{
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL2_UNION acsq_fsm_dfx_ctrl2 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");

	do {
		acsq_fsm_dfx_ctrl2.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL2_ADDR(hwts_base, acsq_id));
		if (acsq_fsm_dfx_ctrl2.reg.acsq_fsm_state == 0x14 && // 14:WAIT_TASK_TERMINATE
			acsq_fsm_dfx_ctrl2.reg.acsq_fsm_cq_write_ost_cnt == 0 &&
			acsq_fsm_dfx_ctrl2.reg.acsq_fsm_write_value_ost_cnt == 0 &&
			acsq_fsm_dfx_ctrl2.reg.acsq_fsm_profile_ost_cnt == 0 &&
			acsq_fsm_dfx_ctrl2.reg.acsq_fsm_task_log_ost_cnt == 0)
			return 0;

		usleep_range(1, 1);
		timeout--;
	} while (timeout > 0);

	npu_drv_err("polling acsq fsm_state(0x%x) TIMEOUT\n", acsq_fsm_dfx_ctrl2.value);
	return -1;
}

int npu_hwts_polling_rtsq_fsm_state(u16 channel_id, u64 timeout)
{
	SOC_NPU_HWTS_HWTS_P0_RTSQ_DFX0_UNION rtsq_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");

	do {
		rtsq_dfx0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_RTSQ_DFX0_ADDR(hwts_base, channel_id));
		if (rtsq_dfx0.reg.p0_rtsq_fsm_state == 0x9) // 9: WAIT_FW_RSP
			return 0;

		usleep_range(1, 1);
		timeout--;
	} while (timeout > 0);

	npu_drv_err("polling rtsq fsm_state(0x%x) TIMEOUT\n", rtsq_dfx0.reg.p0_rtsq_fsm_state);
	return -1;
}

static int npu_hwts_aic_fsm_poll_proc(u16 acsq_id, u32 bitmap, u64 timeout)
{
	uint32_t index = 0;
	u64 timeout_tmp;
	SOC_NPU_HWTS_HWTS_AIC_FSM0_DFX0_UNION hwts_aic_fsm0_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");
 
	for (; index < AIC_NUM; index++) {
		if ((bitmap & (0x1 << index)) == 0)
			continue;
		timeout_tmp = timeout;
		do {
			hwts_aic_fsm0_dfx0.value = readl(
				(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIC_FSM0_DFX0_ADDR(hwts_base, index));
			if ((hwts_aic_fsm0_dfx0.reg.aic_fsm0_rd_ost_cnt == 0) &&
				(hwts_aic_fsm0_dfx0.reg.aic_fsm0_pmu_ost_cnt == 0) &&
				(hwts_aic_fsm0_dfx0.reg.aic_fsm0_log_ost_cnt == 0) &&
				(hwts_aic_fsm0_dfx0.reg.aic_fsm0_cfg_ost_cnt == 0)) {
				npu_drv_warn("aic suspend, acsq_id = %u core_id = %u state = %x",
					acsq_id, index, hwts_aic_fsm0_dfx0.value);
				break;
			}
			usleep_range(1, 1);
			timeout_tmp--;
		} while (timeout_tmp > 0);
		if (timeout_tmp == 0) {
			npu_drv_err("polling acsq ip_own_state TIMEOUT, acsq_id = %u core_id = %u state = %x\n",
				acsq_id, index, hwts_aic_fsm0_dfx0.value);
			return -1;
		}
	}

	return 0;
}
 
static int npu_hwts_aiv_fsm_poll_proc(u16 acsq_id, u32 bitmap, u64 timeout)
{
	uint32_t index = 0;
	u64 timeout_tmp;
	SOC_NPU_HWTS_HWTS_AIV_FSM0_DFX0_UNION hwts_aiv_fsm0_dfx0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");
 
	for (; index < AIV_NUM; index++) {
		if ((bitmap & (0x1 << index)) == 0)
			continue;
		timeout_tmp = timeout;
		do {
			hwts_aiv_fsm0_dfx0.value = readl(
				(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_AIV_FSM0_DFX0_ADDR(hwts_base, index));
			if ((hwts_aiv_fsm0_dfx0.reg.aiv_fsm0_cxt_sw_ost_cnt == 0) &&
				(hwts_aiv_fsm0_dfx0.reg.aiv_fsm0_rd_ost_cnt == 0) &&
				(hwts_aiv_fsm0_dfx0.reg.aiv_fsm0_pmu_ost_cnt == 0) &&
				(hwts_aiv_fsm0_dfx0.reg.aiv_fsm0_log_ost_cnt == 0) &&
				(hwts_aiv_fsm0_dfx0.reg.aiv_fsm0_cfg_ost_cnt == 0)) {
				npu_drv_warn("aiv suspend, acsq_id = %u core_id = %u state = %x",
					acsq_id, index, hwts_aiv_fsm0_dfx0.value);
				break;
			}
			usleep_range(1, 1);
			timeout_tmp--;
		} while (timeout_tmp > 0);
		if (timeout_tmp == 0) {
			npu_drv_err("polling acsq ip_own_state TIMEOUT, acsq_id = %u core_id = %u state = %x\n",
				acsq_id, index, hwts_aiv_fsm0_dfx0.value);
			return -1;
		}
	}
 
	return 0;
}

int npu_hwts_polling_acsq_ip_own_state(u16 acsq_id, u64 timeout, uint32_t *need_reset)
{
    SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_UNION acsq_fsm_ip_own_state = {0};
    SOC_NPU_HWTS_ACSQ_FSM_IP_EXCEPTION_STATE_UNION acsq_fsm_ip_exception_state = {0};
    SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_UNION acsq_fsm_dfx_ctrl0 = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	enum npu_core_addr_index core_id = NPU_AIC_0;
	int ret = 0;
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");

	acsq_fsm_ip_own_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_OWN_STATE_ADDR(hwts_base, acsq_id));
	// 1. no aicore own
	if (acsq_fsm_ip_own_state.reg.ip_own_bitmap == 0) {
		npu_drv_warn("no aic own, acsq id = %u", acsq_id);
		return 0;
	}

	// 2. aicore own
	// 2.1 get sqe_type
	acsq_fsm_dfx_ctrl0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_ADDR(hwts_base, acsq_id));
	// 2.2 polling aicore fsm
	switch(acsq_fsm_dfx_ctrl0.reg.sqe_type) {
		case NPU_HWTS_SQE_AICORE:
			ret = npu_hwts_aic_fsm_poll_proc(acsq_id, acsq_fsm_ip_own_state.reg.ip_own_bitmap, timeout);
			if (ret != 0) {
				npu_drv_err("aic fsm polling timeout");
				return -1;
			}
			core_id = NPU_AIC_0;
			break;
		case NPU_HWTS_SQE_VECTOR_CORE:
			ret = npu_hwts_aiv_fsm_poll_proc(acsq_id, acsq_fsm_ip_own_state.reg.ip_own_bitmap, timeout);
			if (ret != 0) {
				npu_drv_err("aiv fsm polling timeout");
				return -1;
			}
			core_id = NPU_AIV_0;
			break;
		default:
			npu_drv_err("invalid sqe type %d", acsq_fsm_dfx_ctrl0.reg.sqe_type);
			return -1;
	}
	// 2.3 polling aicore run stall
	ret = npu_aicore_query_run_stall(core_id, timeout);
	if (ret != 0) {
		npu_drv_err("aic %d run stall polling fail", core_id);
		*need_reset = 1;
		return -1;
	}
	
	// 3. check exception
	acsq_fsm_ip_exception_state.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_IP_EXCEPTION_STATE_ADDR(hwts_base, acsq_id));
	if (acsq_fsm_ip_exception_state.value != 0) {
        npu_drv_err("aicore exception, acsq_id = %u state = %x, nead reset",
			acsq_id, acsq_fsm_ip_exception_state.value);
		*need_reset = 1;
        return -1;
    }
	return 0;
}

int npu_clear_hts_event_tbl(uint32_t event_id)
{
	SOC_NPU_HWTS_HWTS_HTS_EVENT_TABLE_UNION hts_event_tbl = {0};
	u64 hwts_base = npu_hwts_get_base_addr();
	if (event_id >= NPU_HTS_EVENT_MAX_ID) {
		npu_drv_err("invalid eventid:%u\n", event_id);
		return -1;
	}
	cond_return_error(hwts_base == 0, -1, "hwts_base is NULL\n");
	hts_event_tbl.reg.hts_event_table_cnt = 0;
	writel(hts_event_tbl.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_HTS_EVENT_TABLE_ADDR(hwts_base, event_id));
	return 0;
}
