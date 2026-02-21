/*
 * npu_platform_exception.c
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

#include "npu_platform_exception.h"

#include <securec.h>
#include "hwts_driver.h"
#include "npu_soc_defines.h"
#include "npu_shm.h"
#include "npu_sink_sqe_fmt.h"
#include "npu_hwts_plat.h"
#include "npu_aicore_plat.h"
#include "npu_pm_framework.h"
#include "npu_pool.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_proc_ctx.h"
#include "npu_common.h"
#include "npu_arch_exception.h"
#include "npu_atf_subsys.h"

#ifdef CONFIG_NPU_OHOS
#include "npu_udk.h"
#endif

#ifndef CONFIG_NPU_OHOS
static void npu_exception_pool_conflict_proc(u16 channel_id)
{
	unused(channel_id);
}

static void npu_exception_bus_error_proc(u16 channel_id)
{
	unused(channel_id);
}

static void npu_exception_ecc_error_proc(u16 channel_id)
{
	unused(channel_id);
}

static void npu_exception_timeout_proc(u16 channel_id)
{
	unused(channel_id);
}

static void npu_exception_sqe_error_proc(u16 channel_id)
{
	unused(channel_id);
}

static void npu_exception_sw_status_error_proc(u16 channel_id)
{
	u32 status = 0;
	(void)npu_hwts_query_sw_status(channel_id, &status);
	npu_exception_sqe_error_proc(channel_id);
}

static void npu_exception_bbox_dump_aicore(u8 aic_id,
	struct aicore_exception_info *aic_info,
	struct aicore_exception_dbg_info *aic_dbg_info)
{
	char log_buf[NPU_BUF_LEN_MAX + 1] = {0};

	(void)snprintf_s(log_buf, sizeof(log_buf), sizeof(log_buf) - 1,
		"AICORE INFO:\naicore%u\nAIC_ERROR=0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx\nifu offset=0x%016llx\n",
		aic_id, aic_info->aic_error, aic_info->aic_error1, aic_info->aic_error2,
		aic_info->aic_error3, (aic_dbg_info->ifu_current) - (aic_info->ifu_start));
	(void)npu_mntn_copy_reg_to_bbox(log_buf, (unsigned int)strlen(log_buf));
}

static void npu_exception_bbox_dump_aic_timeout(u8 aic_id,
	struct aicore_timeout_info *aic_info)
{
	char log_buf[NPU_BUF_LEN_MAX + 1] = {0};

	(void)snprintf_s(log_buf, sizeof(log_buf), sizeof(log_buf) - 1,
		"AIC TIMEOUT INFO:\naicore%u\nSC_CSW_ST=0x%016llx, "
		"BIU=0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx, 0x%016llx\n",
		aic_id, aic_info->sc_csw_st, aic_info->biu_st[0], aic_info->biu_st[1], aic_info->biu_st[2],
		aic_info->biu_st[3], aic_info->biu_st[4], aic_info->biu_st[5],
		aic_info->biu_st[6], aic_info->biu_st[7]);
	(void)npu_mntn_copy_reg_to_bbox(log_buf, (unsigned int)strlen(log_buf));
}

static void npu_exception_query_aicore_info(u16 channel_id, u32 is_timeout)
{
	struct aicore_exception_info aicore_info = {0};
	struct aicore_exception_error_info aicore_error_info = {0};
	struct aicore_exception_dbg_info aicore_dbg_info = {0};
	struct aicore_timeout_info aic_timeout_info = {0};
	u8 sqe_type = NPU_HWTS_SQE_TYPE_RESV;
	u8 core_id;

	(void)npu_hwts_query_sqe_type(channel_id, &sqe_type);
	if (sqe_type == NPU_HWTS_SQE_AICORE) {
		npu_drv_warn("npu hwts query aicore exception\n");
		core_id = (u8)NPU_AIC_0;
	} else if (sqe_type == NPU_HWTS_SQE_VECTOR_CORE) {
		npu_drv_warn("npu hwts query aiv exception\n");
		core_id = (u8)NPU_AIV_0;
	} else if (sqe_type == NPU_HWTS_SQE_AICORE_EFF) {
		core_id = (u8)NPU_AIC_EFF;
	} else {
		npu_drv_warn("hwts sqe type not supported");
		return;
	}

	(void)npu_aicore_query_exception_info(core_id, &aicore_info);
	(void)npu_aicore_query_exception_error_info(core_id, &aicore_error_info);
	(void)npu_aicore_query_exception_dbg_info(core_id, &aicore_dbg_info);
	if (is_timeout == 1) {
		npu_aicore_query_timeout_info(core_id, &aic_timeout_info);
		npu_exception_bbox_dump_aic_timeout(core_id, &aic_timeout_info);
	}
	npu_exception_bbox_dump_aicore(core_id, &aicore_info, &aicore_dbg_info);
}

static void npu_exception_aicore_exception_proc(u16 channel_id)
{
	int ret = 0;
	u8 aic_own_bitmap = 0;
	u8 aic_exception_bitmap = 0;
	u8 i = 0;
	u8 aic_status = 0;
	u8 aiv_status = 0;

	ret = npu_hwts_query_aicore_own_bitmap(channel_id, &aic_own_bitmap);
	cond_return_void(ret != 0, "cannot get aic_own_bitmap\n");

	ret = npu_hwts_query_aicore_exception_bitmap(channel_id, &aic_exception_bitmap);
	cond_return_void(ret != 0, "cannot get aic_exception_bitmap\n");

	if (aic_exception_bitmap == 0) {
		npu_drv_warn("no aicore get exception\n");
		return;
	}

	(void)npu_hwts_query_aic_task_config();

	(void)npu_hwts_query_aiv_task_config();

	(void)npu_hwts_query_eff_aic_task_config();

	for (i = 0; i < NPU_HWTS_MAX_AICORE_POOL_NUM; i++)
		(void)npu_hwts_query_aic_pool_status(i, &aic_status);

	for (i = 0; i < NPU_HWTS_MAX_AIV_POOL_NUM; i++)
		(void)npu_hwts_query_aiv_pool_status(i, &aiv_status);

	for (i = 0; i < NPU_HWTS_MAX_AICORE_POOL_NUM; i++)
		(void)npu_hwts_query_aic_eff_pool_status(i, &aic_status);

	if (aic_exception_bitmap & (0x1 << 0))
		npu_exception_query_aicore_info(channel_id, 0);
}

static void npu_exception_aicore_trap_proc(u16 channel_id)
{
	int ret = 0;
	u8 aic_own_bitmap = 0;
	u8 aic_trap_bitmap = 0;

	ret = npu_hwts_query_aicore_own_bitmap(channel_id, &aic_own_bitmap);
	cond_return_void(ret != 0, "cannot get aic_own_bitmap\n");

	ret = npu_hwts_query_aicore_trap_bitmap(channel_id, &aic_trap_bitmap);
	cond_return_void(ret != 0, "cannot get aic_trap_bitmap\n");

	cond_return_void(aic_trap_bitmap == 0, "no aicore get trap\n");
	if (aic_trap_bitmap & (0x1 << 0))
		npu_exception_query_aicore_info(channel_id, 0);
}

static void npu_exception_task_trap_proc(u16 channel_id)
{
	npu_exception_aicore_trap_proc(channel_id);
	npu_exception_sqe_error_proc(channel_id);
}

static void npu_exception_aicore_timeout_proc(u16 channel_id)
{
	int ret = 0;
	u8 aic_own_bitmap = 0;

	ret = npu_hwts_query_aicore_own_bitmap(channel_id, &aic_own_bitmap);
	cond_return_void(ret != 0, "cannot get aic_own_bitmap\n");

	if (aic_own_bitmap == 0) {
		npu_drv_warn("no aicore Occupied\n");
		return;
	}

	if (aic_own_bitmap & (0x1 << 0))
		npu_exception_query_aicore_info(channel_id, 1);
}

static void npu_exception_bbox_dump_hwts(u16 channel_id, struct sq_exception_info *sq_info)
{
	char log_buf[NPU_BUF_LEN_MAX + 1] = {0};

	(void)snprintf_s(log_buf, sizeof(log_buf), sizeof(log_buf) - 1,
		"HWTS INFO:\nchannel id=%u, sqe_type=0x%x, blk_dim=%u, rtsq_head=0x%x, "
		"rtsq_tail=0x%x, sq_length=0x%x, acsq_id=%u, sqcq_fsm_state=0x%0x\n",
		channel_id, sq_info->sqe_type, sq_info->blk_dim, sq_info->sq_head,
		sq_info->sq_tail, sq_info->sq_length, sq_info->acsq_id, sq_info->sqcq_fsm_state);
	(void)npu_mntn_copy_reg_to_bbox(log_buf, (unsigned int)strlen(log_buf));
}

void npu_ispnn_exception_dump(u8 exception_type, u16 channel_id)
{
	struct hwts_interrupt_info interrupt_info = {0};
	struct sq_exception_info sq_info = {0};
 
	npu_drv_warn("channel_id = %u, exception_type = %u\n", channel_id, exception_type);
 
	(void)npu_hwts_query_interrupt_info(&interrupt_info);
	(void)npu_hwts_query_sq_info(channel_id, &sq_info);
	npu_exception_bbox_dump_hwts(channel_id, &sq_info);
	if (exception_type == NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION) {
		npu_exception_aicore_exception_proc(channel_id);
	} else if (exception_type == NPU_EXCEPTION_TYPE_HWTS_TASK_TIMEOUT) {
		npu_exception_aicore_timeout_proc(channel_id);
	} else {
		npu_drv_warn("it is not aicore exception or timeout\n");
	}
}

static void npu_exception_task_exception_proc(u16 channel_id)
{
	struct hwts_interrupt_info interrupt_info = {0};
	struct sq_exception_info sq_info = {0};

	(void)npu_hwts_query_interrupt_info(&interrupt_info);
	(void)npu_hwts_query_sq_info(channel_id, &sq_info);
	npu_exception_bbox_dump_hwts(channel_id, &sq_info);
	npu_exception_aicore_exception_proc(channel_id);
}

static void npu_exception_task_timeout_proc(u16 channel_id)
{
	struct hwts_interrupt_info interrupt_info = {0};
	struct sq_exception_info sq_info = {0};

	(void)npu_hwts_query_interrupt_info(&interrupt_info);
	(void)npu_hwts_query_sq_info(channel_id, &sq_info);
	npu_exception_bbox_dump_hwts(channel_id, &sq_info);
	npu_exception_aicore_timeout_proc(channel_id);
}

static bool is_exception_type_valid(uint8_t exception_type)
{
	if (exception_type >= NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION && exception_type < NPU_EXCEPTION_TYPE_MAX) {
		return true;
	}

	npu_drv_err("invalid exception_type %u\n", exception_type);
	return false;
}

void npu_hwts_exception_dump(u8 exception_type, u16 hwts_sq_id, u16 channel_id)
{
	// 1. print exception info
	static void (*exception_func[NPU_EXCEPTION_TYPE_MAX])(u16) = {
		npu_exception_task_exception_proc,
		npu_exception_task_timeout_proc,
		npu_exception_task_trap_proc,
		npu_exception_sqe_error_proc,
		npu_exception_sw_status_error_proc,
		npu_exception_bus_error_proc,
		npu_exception_ecc_error_proc,
		npu_exception_pool_conflict_proc,
		npu_exception_timeout_proc
	};
	unused(hwts_sq_id);

	if (!is_exception_type_valid(exception_type))
		return;
	
	if (exception_func[exception_type] != NULL)
		(exception_func[exception_type])(channel_id);
}

int npu_timeout_exception_dump(void)
{
	int ret = 0;
	int status = 0;
	u16 channel_id = 0;
	uint64_t nonsec_bitmap = NPU_HWTS_RTSQ_LITE_UNSEC_BITMAP | NPU_HWTS_RTSQ_ISPNN_BITMAP |
		NPU_HWTS_RTSQ_FFRT_BITMAP | NPU_HWTS_RTSQ_HTS_BITMAP;
	struct hwts_interrupt_info interrupt_info = {0};
	struct sq_exception_info sq_info = {0};

	ret = npu_hwts_query_interrupt_info(&interrupt_info);
	status = npu_hwts_query_aic_task_config();
	if (status == 0x4) {
		return NPU_EXCEPTION_CONTINUE;
	}
	status = npu_hwts_query_aiv_task_config();
	if (status == 0x4) {
		return NPU_EXCEPTION_CONTINUE;
	}
	status = npu_hwts_query_eff_aic_task_config();
	if (status == 0x4) {
		return NPU_EXCEPTION_CONTINUE;
	}

	/* timeout without hwts exception, check&&dump all channel info */
	for (channel_id = 0; channel_id < NPU_HWTS_CHANNEL_NUM; channel_id++) {
		if (npu_bitmap_get(nonsec_bitmap, channel_id) == 0)
			continue;
		npu_drv_warn("channel_id = %u\n", channel_id);
		status = npu_hwts_query_sq_info(channel_id, &sq_info);
		ret += status;
	}
	if (ret > 0) {
		return NPU_EXCEPTION_HANDLED;
	}
	return NPU_EXCEPTION_CONTINUE;
}

int npu_exception_aicore_reset(u16 channel_id)
{
	int ret;
	u8 sqe_type = NPU_HWTS_SQE_TYPE_RESV;
	u8 aic_own_bitmap = 0;
	u8 aic_exception_bitmap = 0;
	u8 aicore_id;
	u64 timeout = 100;
	u16 acsq_id;

	// 1.2.3. handle in TSFW
	// 4. query sqe_type
	npu_drv_warn("reset aicore start");
	ret = npu_hwts_query_sqe_type(channel_id, &sqe_type);
	cond_return_error(ret != 0, -1, "query sqe_type failed\n");
	cond_return_error(!(sqe_type == NPU_HWTS_SQE_AICORE || sqe_type == NPU_HWTS_SQE_VECTOR_CORE
		|| sqe_type == NPU_HWTS_SQE_AICORE_EFF),
		-1, "hwts sqe type not supported\n");
	
	// 4.1 query ip_exception_bitmap
	ret = npu_hwts_query_aicore_exception_bitmap(channel_id, &aic_exception_bitmap);
	cond_return_error(ret != 0, -1, "cannot get aic_exception_bitmap\n");
	npu_drv_warn("aic_exception_bitmap = %u\n", aic_exception_bitmap);

	// 5.1.1 polling HWTS_AIC/AIV_FSM0_DFX0
	if (sqe_type == NPU_HWTS_SQE_AICORE) {
		ret = npu_hwts_polling_aic_fsm_state(timeout);
		aicore_id = NPU_AIC_0;
	} else if (sqe_type == NPU_HWTS_SQE_VECTOR_CORE) {
		ret = npu_hwts_polling_aiv_fsm_state(timeout);
		aicore_id = NPU_AIV_0;
	} else if (sqe_type == NPU_HWTS_SQE_AICORE_EFF) {
		ret = npu_hwts_polling_eff_aic_fsm_state(timeout);
		aicore_id = NPU_AIC_EFF;
	}
	cond_return_error(ret != 0, -1, "polling HWTS_AIC/AIV_FSM0_DFX0 failed\n");
	// 5.1.2 own_bitmap=1
	ret = npu_hwts_query_aicore_own_bitmap(channel_id, &aic_own_bitmap);
	cond_return_error(ret != 0, -1, "cannot get aic_own_bitmap\n");
	cond_return_error(aic_own_bitmap != 1, -1, "no aicore Occupied\n");

	// 5.2 reset AIC/AIV
	ret = atfd_service_npu_smc(NPU_AICORE_RESET, aicore_id, 0, 0);
	cond_return_error(ret != 0, -1, "reset failed, aicore_id: %u\n", aicore_id);

	// 5.3 polling ACSQ_FSM_DFX_CTRL2
	acsq_id = hwts_get_acsq_id_by_rtsq_id(channel_id);
	cond_return_error(acsq_id == UINT16_MAX, -1, "invalid acsq id\n");
	ret = npu_hwts_polling_acsq_fsm_state(acsq_id, timeout);
	cond_return_error(ret != 0, -1, "polling ACSQ_FSM_DFX_CTRL2 failed\n");

	// 5.4 polling P0_RTSQ_DFX0
	ret = npu_hwts_polling_rtsq_fsm_state(channel_id, timeout);
	cond_return_error(ret != 0, -1, "polling P0_RTSQ_DFX0 failed\n");

	hwts_driver_clear_channel_sq_en(channel_id);
	hwts_driver_hwts_task_terminate(channel_id);
	hwts_driver_reset_cq_head(channel_id);

	npu_drv_warn("reset aicore succ, wait ts proc bottom half");
	return 0;
}
#endif

static enum npu_subip npu_get_subip_by_channel_id(struct npu_dev_ctx *dev_ctx, u16 channel_id)
{
	u8 sqe_type = NPU_HWTS_SQE_TYPE_RESV;
	npu_pm_safe_call(dev_ctx, NPU_IP_NON_TOP, (void)npu_hwts_query_sqe_type(channel_id, &sqe_type));
 
	if (sqe_type == NPU_HWTS_SQE_AICORE) {
		return NPU_IP_AICORE_GE;
	} else if (sqe_type == NPU_HWTS_SQE_VECTOR_CORE) {
		return NPU_IP_AICORE_GE;
	} else if (sqe_type == NPU_HWTS_SQE_AICORE_EFF) {
		return NPU_IP_AICORE_EFF;
	} else {
		npu_drv_warn("get invalid subip, channel_id = %u, sqe_type = %u\n", channel_id, sqe_type);
		return NPU_SUBIP_MAX;
	}
}

int npd_dump_exception_with_power_check(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er, 
	npd_dump_func dump_exception)
{
	int ret = NPU_EXCEPTION_CONTINUE;
	u16 channel_id;
	enum npu_subip subip;

	switch (er->ex_source) {
	case NPU_EXCEPTION_SRC_FFTS:
		channel_id = er->u.ex_ffts_report->channel_id;
		break;
	case NPU_EXCEPTION_SRC_TS:
		channel_id = er->u.ex_hwts_report->channel_id;
		break;
	case NPU_EXCEPTION_SRC_TASK_TIMEOUT: 
		npu_pm_safe_call_with_return(dev_ctx, NPU_IP_NON_TOP, dump_exception(er), ret);
		return ret;
	default: 
		npu_drv_warn("invalid exception source %d\n", er->ex_source);
		return NPU_EXCEPTION_CONTINUE;
	}
 
	subip = npu_get_subip_by_channel_id(dev_ctx, channel_id);
	if (subip == NPU_SUBIP_MAX) {
		return NPU_EXCEPTION_CONTINUE;
	}
 
	npu_pm_safe_call_with_return(dev_ctx, subip, dump_exception(er), ret);
	return ret;
}