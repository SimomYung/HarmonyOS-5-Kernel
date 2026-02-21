/*
 * npu_aicore_plat.c
 *
 * about npu aicore plat
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
#include "npu_aicore_plat.h"

#include "npu_platform.h"
#include "npu_log.h"
#include "soc_npu_aicore_interface.h"

u64 npu_aicore_get_base_addr(u8 core_id)
{
	u64 aicore_base = 0ULL;
	u64 base = 0ULL;
	struct npu_platform_info *plat_info = npu_plat_get_info();

	cond_return_error(plat_info == NULL, 0ULL, "npu_plat_get_info failed\n");
	base = (u64)(uintptr_t)plat_info->dts_info.reg_vaddr[NPU_REG_AIC0_BASE];
	if (core_id == NPU_AIC_0)
		aicore_base = base;
	else if (core_id == NPU_AIV_0)
		aicore_base = base + NPU_AIV_ADDR_OFFSET;
	else if (core_id == NPU_AIC_EFF)
		aicore_base = base + NPU_AIC_EFF_ADDR_OFFSET;
	else
		npu_drv_err("core_id=%u is invalid\n", core_id);

	npu_drv_debug("core_id = %u, aicore_base = 0x%016llx\n", core_id, aicore_base);
	return aicore_base;
}

static int npu_aicore_dbg_busy_wait(u64 addr, u32 *val)
{
	u32 busy_wait_loop = 0;

	while (busy_wait_loop < NPU_AICORE_DBG_WAIT_LOOP_MAX) {
		*val = readl((const volatile void *)(uintptr_t)addr);
		if (*val != 0)
			break;
		busy_wait_loop++;
	}

	if (*val == 0 && busy_wait_loop >= NPU_AICORE_DBG_WAIT_LOOP_MAX) {
		npu_drv_warn("busy wait timed out\n");
		return -1;
	}

	npu_drv_debug("busy wait successfully\n");
	return 0;
}

static int npu_aicore_read_dbg_register(u8 core_id, u64 debug_addr_id, u64 *read_buf)
{
	int ret;
	SOC_NPU_AICORE_SC_DBG_ADDR_L_UNION dbg_addr_l = {0};
	SOC_NPU_AICORE_SC_DBG_ADDR_H_UNION dbg_addr_h = {0};
	SOC_NPU_AICORE_SC_DBG_RW_CTRL_UNION dbg_rw_ctrl = {0};
	SOC_NPU_AICORE_SC_DBG_STATUS_UNION dbg_status = {0};
	SOC_NPU_AICORE_SC_DBG_DATA_0_UNION dbg_data0 = {0};
	SOC_NPU_AICORE_SC_DBG_DATA_1_UNION dbg_data1 = {0};
	u64 base = npu_aicore_get_base_addr(core_id);

	cond_return_error(base == 0ULL, -1, "aicore base is NULL\n");
	npu_drv_debug("base = 0x%pK\n", base);

	/* step 2 */
	dbg_addr_l.reg.dbg_addr_l = debug_addr_id & UINT32_MAX;
	writel(dbg_addr_l.value, (volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_DBG_ADDR_L_ADDR(base));
	npu_drv_debug("dbg_addr_l.value = 0x%016lx\n", dbg_addr_l.value);
	dbg_addr_h.value = (u32)(debug_addr_id >> 32);
	writel(dbg_addr_h.value, (volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_DBG_ADDR_H_ADDR(base));
	npu_drv_debug("dbg_addr_1.value = 0x%016lx\n", dbg_addr_h.value);

	/* step 3 */
	dbg_rw_ctrl.reg.dbg_ctrl_rd = 0x1;
	writel(dbg_rw_ctrl.value, (volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_DBG_RW_CTRL_ADDR(base));
	npu_drv_debug("dbg_rw_ctrl.value = 0x%016lx\n", dbg_rw_ctrl.value);

	/* step 4 */
	ret = npu_aicore_dbg_busy_wait(
		SOC_NPU_AICORE_SC_DBG_STATUS_ADDR(base), &dbg_status.value);
	/* step 6 */
	if (ret != 0) {
		if (dbg_status.reg.dbg_read_err == 0x1)
			npu_drv_warn("register read error\n");
		else
			npu_drv_warn("read register timed out\n");
		return -1;
	}

	/* step 7 */
	dbg_data0.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_DBG_DATA_0_ADDR(base));
	dbg_data1.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_DBG_DATA_1_ADDR(base));
	*read_buf = ((u64)dbg_data1.reg.dbg_data_1 << 32) | dbg_data0.reg.dbg_data_0;
	npu_drv_debug("*read_buf = 0x%016llx\n", *read_buf);

	return 0;
}

int npu_aicore_query_exception_dbg_info(
	u8 core_id, struct aicore_exception_dbg_info *aic_info)
{
	u64 read_buf = 0;
	int ret;
	u32 reg_id;

	cond_return_error(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	/* ifu current */
	ret = npu_aicore_read_dbg_register(core_id, 0x10000000000040, &read_buf);
	if (ret == 0) {
		aic_info->ifu_current = read_buf;
		npu_drv_warn("ifu current is at 0x%016llx\n", read_buf);
	}

	/* x0-x31 */
	for (reg_id = 0; reg_id < 32; reg_id++) {
		ret = npu_aicore_read_dbg_register(core_id, 0x10000000000000 | reg_id, &read_buf);
		if (ret == 0) {
			aic_info->general_register[reg_id] = read_buf;
			npu_drv_warn("general purpose register x%u = 0x%016llx\n",
					reg_id, read_buf);
		}
	}

	return 0;
}

static int npu_aiv_query_exception_info(void)
{
	SOC_NPU_AICORE_VEC_ERR_INFO_T0_0_UNION vec_err_info_t0_0 = {0};
	SOC_NPU_AICORE_VEC_ERR_INFO_T0_1_UNION vec_err_info_t0_1 = {0};
	SOC_NPU_AICORE_VEC_ERR_INFO_T0_2_UNION vec_err_info_t0_2 = {0};
	SOC_NPU_AICORE_VEC_ERR_INFO_T0_3_UNION vec_err_info_t0_3 = {0};
	SOC_NPU_AICORE_VEC_ERR_INFO_T0_4_UNION vec_err_info_t0_4 = {0};

	u64 base = npu_aicore_get_base_addr(NPU_AIV_0);
	cond_return_error(base == 0ULL, -EINVAL, "aicore base is NULL\n");

	vec_err_info_t0_0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_0_ADDR(base));
	npu_drv_warn("AIC_VEC_ERR_INFO_T0_0.value = 0x%016llx\n", vec_err_info_t0_0.value);
	vec_err_info_t0_1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_1_ADDR(base));
	npu_drv_warn("AIC_VEC_ERR_INFO_T0_1.value = 0x%016llx\n", vec_err_info_t0_1.value);
	vec_err_info_t0_2.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_2_ADDR(base));
	npu_drv_warn("AIC_VEC_ERR_INFO_T0_2.value = 0x%016llx\n", vec_err_info_t0_2.value);
	vec_err_info_t0_3.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_3_ADDR(base));
	npu_drv_warn("AIC_VEC_ERR_INFO_T0_3.value = 0x%016llx\n", vec_err_info_t0_3.value);
	vec_err_info_t0_4.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_4_ADDR(base));
	npu_drv_warn("AIC_VEC_ERR_INFO_T0_4.value = 0x%016llx\n", vec_err_info_t0_4.value);
	return 0;
}

int npu_aicore_query_exception_info(
	u8 core_id, struct aicore_exception_info *aic_info)
{
	SOC_NPU_AICORE_SU_KIS_START_PC_L_UNION su_kis_start_l = {0};
	SOC_NPU_AICORE_SU_KIS_START_PC_H_UNION su_kis_start_h = {0};
	SOC_NPU_AICORE_SC_AIC_CORE_INT_UNION sc_aic_core_int = {0};
	u32 error_flag;
	u64 base = npu_aicore_get_base_addr(core_id);

	cond_return_error(base == 0ULL, -EINVAL, "aicore base is NULL\n");
	cond_return_error(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	su_kis_start_l.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_KIS_START_PC_L_ADDR(base));
	su_kis_start_h.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_KIS_START_PC_H_ADDR(base));
	aic_info->ifu_start = ((u64)su_kis_start_h.reg.kis_su_start_pc_h << 32) | su_kis_start_l.value;
	npu_drv_warn("ifu start = %016llx\n", aic_info->ifu_start);

	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_ERROR_T0_0_ADDR(base));
	aic_info->aic_error = (u64)error_flag;
	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_ERROR_T0_0_ADDR(base));
	aic_info->aic_error |= ((u64)error_flag << 32);
	npu_drv_warn("AIC_ERROR.value(SU|SC) = 0x%016llx\n", aic_info->aic_error);

	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_MTE_ERROR_T0_0_ADDR(base));
	aic_info->aic_error1 = (u64)error_flag;
	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERROR_T0_0_ADDR(base));
	aic_info->aic_error1 |= ((u64)error_flag << 32);
	npu_drv_warn("AIC_ERROR1.value(VEC|MTE) = 0x%016llx\n", aic_info->aic_error1);

	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_CUBE_ERROR_T0_0_ADDR(base));
	aic_info->aic_error2 = (u64)error_flag;
	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_L1_ERROR_T0_0_ADDR(base));
	aic_info->aic_error2 |= ((u64)error_flag << 32);
	npu_drv_warn("AIC_ERROR2.value(L1_T0|CUBE) = 0x%016llx\n", aic_info->aic_error2);

	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_L1_ERROR_T0_1_ADDR(base));
	aic_info->aic_error3 = (u64)error_flag;
	npu_drv_warn("AIC_ERROR3.value(L1) = 0x%016llx\n", aic_info->aic_error3);

	sc_aic_core_int.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_AIC_CORE_INT_ADDR(base));
	aic_info->aic_core_int = sc_aic_core_int.value;
	npu_drv_warn("AI_CORE_INT.value = 0x%x\n", sc_aic_core_int.value);

	if (core_id == NPU_AIV_0)
		return npu_aiv_query_exception_info();
	return 0;
}

int npu_aicore_query_exception_error_info(
	u8 core_id, struct aicore_exception_error_info *aic_info)
{
	u64 base = npu_aicore_get_base_addr(core_id);
	u32 error_info_l;
	u32 error_info_h;
	cond_return_error(base == 0ULL, -EINVAL, "aicore base is NULL\n");
	cond_return_error(aic_info == NULL, -EINVAL, "aic_info is NULL\n");

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_ERR_INFO_T0_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_ERR_INFO_T0_1_ADDR(base));
	aic_info->ccu_err_info = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn("SU_ERR_INFO.value = 0x%016llx\n", aic_info->ccu_err_info);
	aic_info->su_error_pc = ((error_info_l & 0x0000FFFF) << 2);
	npu_drv_warn("su_error_pc = 0x%lx\n", aic_info->su_error_pc);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_ERR_INFO_T0_2_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_ERR_INFO_T0_3_ADDR(base));
	npu_drv_warn("SU_ERR_INFO_T0_2.value = 0x%016lx\n", error_info_l);
	npu_drv_warn("SU_ERR_INFO_T0_3.value = 0x%016lx\n", error_info_h);
	aic_info->illegal_instr = error_info_h;
	npu_drv_warn("illegal_instr = 0x%lx\n", aic_info->illegal_instr);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_CUBE_ERR_INFO_T0_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_CUBE_ERR_INFO_T0_1_ADDR(base));
	aic_info->cube_err_info = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn("CUBE_ERR_INFO.value = 0x%x\n", aic_info->cube_err_info);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_MTE_ERR_INFO_T0_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_MTE_ERR_INFO_T0_1_ADDR(base));
	aic_info->mte_err_info = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn("MTE_ERR_INFO.value = 0x%016llx\n", aic_info->mte_err_info);

	/* valid when mte error occurs */
	aic_info->mte_error_pc = ((error_info_l & 0x000000FF) << 2) + ((error_info_h & 0x000000FF) << 10);
	npu_drv_warn("MTE_ERR_PC.value = 0x%lx\n", aic_info->mte_error_pc);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_VEC_ERR_INFO_T0_1_ADDR(base));
	aic_info->vec_err_info = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn("VEC_ERR_INFO.value = 0x%016llx\n", aic_info->vec_err_info);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_MTE_1BIT_ECC_ERR_ADDR_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_MTE_1BIT_ECC_ERR_ADDR_1_ADDR(base));
	aic_info->mte_ecc_1bit_err = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn(
			"MTE_ECC_1BIT_ERR.value = 0x%016llx\n", aic_info->mte_ecc_1bit_err);

	error_info_l = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_L1_1BIT_ECC_ERR_ADDR_0_ADDR(base));
	error_info_h = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SU_1BIT_ECC_ERR_PB_ADDR_ADDR(base));
	aic_info->vec_cube_ecc_1bit_err = ((u64)error_info_h << 32) | error_info_l;
	npu_drv_warn("VEC_CUBE_ECC_1BIT_ERR.value = 0x%016llx\n",
			aic_info->vec_cube_ecc_1bit_err);

	return 0;
}

int npu_aicore_query_timeout_info(u8 core_id, struct aicore_timeout_info *aic_info)
{
	SOC_NPU_AICORE_SC_PENDING_TASK_STATUS_UNION sc_pending_task_st = {0};
	u64 base = npu_aicore_get_base_addr(core_id);
	u32 error_flag;
	u32 i;

	cond_return_error(base == 0ULL, -EINVAL, "aicore base is NULL\n");

	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_CONTEXT_SWITCH_CFG_ST_ADDR(base));
	aic_info->sc_csw_st = (u64)error_flag;
	error_flag = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_INNER_STATUS_ADDR(base));
	aic_info->sc_csw_st |= ((u64)error_flag << 32);
	npu_drv_warn("aic_info->sc_csw_st = %016llx\n", aic_info->sc_csw_st);

	sc_pending_task_st.value = readl((const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_PENDING_TASK_STATUS_ADDR(base));
	npu_drv_warn("sc_pending_task_st.value = %lx\n", sc_pending_task_st.value);

	for (i = 0; i < 8; i++) {
		error_flag =
			readl((const volatile void *)(uintptr_t)(SOC_NPU_AICORE_BIU_STATUS0_0_ADDR(base) + (0x8 * i)));
		aic_info->biu_st[i] =  (u64)error_flag;
		error_flag =
			readl((const volatile void *)(uintptr_t)(SOC_NPU_AICORE_BIU_STATUS0_0_ADDR(base) + (0x8 * i) + 0x4));
		aic_info->biu_st[i] |= ((u64)error_flag << 32);
		npu_drv_warn("biu_st%u = %016llx\n", i, aic_info->biu_st[i]);
	}

	return 0;
}

int npu_aicore_query_run_stall(u8 core_id, u64 timeout)
{
	SOC_NPU_AICORE_SC_RUN_STALL_UNION run_stall = {0};
	u64 base = npu_aicore_get_base_addr(core_id);
	cond_return_error(base == 0ULL, -EINVAL, "aicore base is NULL\n");

	do {
		run_stall.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_AICORE_SC_RUN_STALL_ADDR(base));
		if (run_stall.reg.psq_stop == 1) {
			npu_drv_warn("aicore %u polling success", core_id);
			return 0;
		}
		usleep_range(1, 1);
		timeout--;
	} while (timeout > 0);

	npu_drv_err("aicore polling timeout, core_id = %u state = %x", core_id, run_stall.value);
	return -1;
}
