/*
* npu_ffts_exception_udk.c
*
* about npu aicore plat
*
* Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include "npu_ffts_exception_udk.h"

#include "ffts_hw_config.h"
#include "hwts_driver.h"
#include "npu_common.h"
#include "npu_common_resource.h"
#include "npu_platform_resource.h"
#include "npu_exception.h"
#include "npu_except_mgr.h"
#include "npu_hwts_plat.h"
#include "npu_log.h"
#include "npu_platform_exception.h"
#include "npu_soc_defines.h"
#ifdef CONFIG_HISI_NPU_UDK
#include "npu_interfaces.h"
#else
#include "bbox/npu_dfx_black_box.h"
#endif

static bool is_entry_id_valid(uint16_t entry_id)
{
	if (entry_id >= MAX_NPU_ENTRY) {
		npu_drv_err("invalid param entry_id = %u\n", entry_id);
		return false;
	}
	return true;
}

static bool is_rtsq_idx_valid(uint16_t rtsq_idx)
{
	if (rtsq_idx >= NPU_FFRT_MODEL_STREAM_NUM_MAX) {
		npu_drv_err("invalid param rtsq_idx = %u\n", rtsq_idx);
		return false;
	}
	return true;
}

int npd_dump_exception_ffts(struct ffts_exception_report_info *report)
{
	if (report == NULL) {
		npu_drv_warn("exception form ts has no report");
		return NPU_EXCEPTION_CONTINUE;
	}

	npu_hwts_exception_dump(report->exception_type, 0, report->channel_id);
	return NPU_EXCEPTION_CONTINUE;
}

static enum npu_hwts_sqe_type npu_get_hwts_sqe_type_by_acsq(uint64_t hwts_base, uint16_t acsq_id)
{
	SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_UNION acsq_fsm_dfx_ctrl0;

	acsq_fsm_dfx_ctrl0.value =
		readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_ACSQ_FSM_DFX_CTRL0_ADDR(hwts_base, acsq_id));
	return (enum npu_hwts_sqe_type)acsq_fsm_dfx_ctrl0.reg.sqe_type;
}

static int npu_handle_ffts_task_kill(uint16_t rtsq, uint32_t *need_reset)
{
	uint16_t acsq_id;
	uint64_t hwts_base = npu_hwts_get_base_addr();
	uint64_t timeout = 100000;
	int ret = 0;
	SOC_NPU_HWTS_HWTS_P0_TASK_CTRL_UNION task_ctrl_0;

	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");

	// send task kill cmd
	task_ctrl_0.value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_TASK_CTRL_ADDR(hwts_base, rtsq));
	task_ctrl_0.reg.p0_task_kill = 0x1;
	writel(task_ctrl_0.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_P0_TASK_CTRL_ADDR(hwts_base, rtsq));

	// polling P0_RTSQ_DFX0
	ret = npu_hwts_polling_rtsq_fsm_state(rtsq, timeout);
	cond_return_error(ret != 0, -1, "polling P0_RTSQ_DFX0 failed\n");

	// check rtsq swapped in or not + check sqe
	acsq_id = hwts_get_acsq_id_by_rtsq_id(rtsq);
	if (acsq_id == UINT16_MAX || npu_get_hwts_sqe_type_by_acsq(hwts_base, acsq_id) == NPU_HWTS_SQE_HTS_EVENT_WAIT) {
		npu_drv_warn("not swapin or hts event wait task, no need to cancel aicore, acsq = %u\n", acsq_id);
		goto disable_sq;
	}

	// make sure acsq is ready to handle exception
	ret = npu_hwts_polling_acsq_fsm_state(acsq_id, timeout);

	// polling owned ip until it becomes 0: no need to do software aic/aiv reset
	timeout = 250000;
	ret += npu_hwts_polling_acsq_ip_own_state(acsq_id, timeout, need_reset);
	cond_return_error(*need_reset != 0, ret, "aic has excp\n");

disable_sq:
	hwts_driver_clear_channel_sq_en(rtsq);
	hwts_driver_hwts_task_terminate(rtsq);
	return ret;
}

static int npu_handle_ffts_exception_hwts(uint16_t rtsq)
{
	// check rtsq swapped in or not
	uint16_t acsq_id = hwts_get_acsq_id_by_rtsq_id(rtsq);
	uint32_t ret = 0;
	if (acsq_id == UINT16_MAX) {
		npu_drv_warn("the rtsq %u is not swapped in\n", rtsq);
		goto disable_sq;
	}

	// polling state and reset aicore
	ret = (uint32_t)npu_exception_aicore_reset(rtsq);
	if (ret != 0) {
		npu_drv_warn("aicore reset failed, need reset npu to handle exception");
		return -1;
	}

disable_sq:
	hwts_driver_clear_channel_sq_en(rtsq);
	hwts_driver_hwts_task_terminate(rtsq);
	return 0;
}

static void npu_read_entry_info(SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION *ctrl0,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION *info1, SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION *info2,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION *dfx0, int entry_num)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();

	cond_return_void(hwts_base == 0ULL, "hwts_base is NULL\n");

	ctrl0->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_num));
	info1->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_ADDR(hwts_base, entry_num));
	info2->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_ADDR(hwts_base, entry_num));
	dfx0->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_ADDR(hwts_base));
}

static int npu_find_pushtask_entry(SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION *ctrl0,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION *info1, SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION *info2,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION *dfx0, int type)
{
	int i;
	int tasksync_entry = 0xff;
	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		npu_read_entry_info(ctrl0, info1, info2, dfx0, i);
		if (type == PUSHTASKSYNCALL && ctrl0->reg.task_vld == 1 && info1->reg.opcode == 1 && info2->reg.ssidv == 0) {
			tasksync_entry = i;
			break;
		} else if (type == PUSHTASKSYNC &&
			ctrl0->reg.task_vld == 1 && info1->reg.opcode == 1 && info2->reg.ssidv == 1 && dfx0->value == 0) {
			tasksync_entry = i;
			break;
		}
	}
	npu_drv_warn("tasksync_entry is %d", tasksync_entry);
	return tasksync_entry;
}

static void npu_read_entry_info_by_id(uint16_t *task_idx, uint32_t *rtsq0_vld,
	uint32_t *rtsq0_err, uint16_t entry_id)
{
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION info1 = { 0 };
	uint64_t hwts_base = npu_hwts_get_base_addr();

	cond_return_void(hwts_base == 0ULL, "hwts_base is NULL\n");

	ctrl0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
	info1.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_ADDR(hwts_base, entry_id));

	*task_idx = info1.reg.taskidx;
	*rtsq0_vld = ctrl0.reg.rtsq0_vld;
	*rtsq0_err = ctrl0.reg.rtsq0_err;
}

int npu_ffts_clear_entry(uint16_t task_idx, uint16_t entry_id)
{
	uint16_t tmp_task_idx = 0;
	uint32_t rtsq0_vld = 0;
	uint32_t rtsq0_err = 0;
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	uint64_t hwts_base = npu_hwts_get_base_addr();

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");
	if(!is_entry_id_valid(entry_id))
		return -1;

	mutex_lock(&dev_ctx->ffrt_entry_mutex);
	npu_read_entry_info_by_id(&tmp_task_idx, &rtsq0_vld, &rtsq0_err, entry_id);
	if (tmp_task_idx != task_idx) {
		mutex_unlock(&dev_ctx->ffrt_entry_mutex);
		return -1;
	}
	if (rtsq0_vld == 0 && rtsq0_err == 1) {
		writel(0x1, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
		mutex_unlock(&dev_ctx->ffrt_entry_mutex);
		return 0;
	}

	mutex_unlock(&dev_ctx->ffrt_entry_mutex);
	return -1;
}

static int npu_check_and_handle_rtsq(uint16_t entry_id, uint16_t rtsq_id, uint32_t rtsq_err)
{
	HWTS_RTSQ_TABLE_BITMAP_INIT();
	uint64_t channel_bitmap = NPU_HWTS_RTSQ_FFRT_BITMAP;
	uint64_t rtsq_low_limit = find_first_bit((unsigned long *)(void *)&channel_bitmap, NPU_HWTS_CHANNEL_NUM);
	uint64_t rtsq_high_limit = find_last_bit((unsigned long *)(void *)&channel_bitmap, NPU_HWTS_CHANNEL_NUM);
	uint32_t need_reset = 0;
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	uint64_t hwts_base = npu_hwts_get_base_addr();

	if (rtsq_id < rtsq_low_limit || rtsq_id > rtsq_high_limit)
		npu_drv_err("the entry %u rtsq %u invalid", entry_id, rtsq_id);
	if (rtsq_err == 1) {
		npu_drv_err("the entry %u rtsq %u has err", entry_id, rtsq_id);
		if (npu_handle_ffts_exception_hwts(rtsq_id) != 0)
			return -1;
	} else {
		npu_drv_err("the entry %u need kill rtsq %u", entry_id, rtsq_id);
		if (npu_handle_ffts_task_kill(rtsq_id, &need_reset) != 0) {
			if (need_reset != 0) {
				ctrl0.value = readl(
					(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
				// if find excep after kill, the excep will not report to hwts
				if (ctrl0.reg.rtsq0_err == 0 && ctrl0.reg.rtsq1_err == 0)
					return npu_handle_ffts_exception_hwts(rtsq_id);
				npu_drv_warn("excep has been reported, proc block dont reset aicore");
				return 0;
			}
			return -1;
		}
	}
	return 0;
}

static int npu_handle_ffrt_entry(uint16_t entry_id, bool proc_block)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO0_UNION info0 = { 0 };
	int trytimes = MAX_NPU_ENTRY;
	int ret = 0;
	npu_drv_warn("npu handle ffrt entry begin");
	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");

	ctrl0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));

	// polling
	while (trytimes-- > 0) {
		if (ctrl0.reg.rtsq_assign == 0) {
			break;
		}
		mdelay(10);
		ctrl0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
	}
	cond_return_error(trytimes == 0, -1, "rtsq assign timeout");

	if (proc_block == true && (ctrl0.reg.rtsq0_err == 1 || ctrl0.reg.rtsq1_err == 1)) {
		// Avoid accessing power-off aic registers during dump in local exception handler
		npu_drv_warn("proc block dont reset aicore");
		return 0;
	}

	if (ctrl0.reg.rtsq0_vld == 0 && ctrl0.reg.rtsq1_vld == 0) {
		// rtsq not occupied
		if (ctrl0.reg.rtsq0_err == 0  && ctrl0.reg.rtsq1_err == 0) {
			npu_drv_err("the entry %u has no error", entry_id);
		} else if (ctrl0.reg.rtsq0_err == 1  && ctrl0.reg.rtsq1_err == 1) {
			npu_drv_err("the entry %u check fail", entry_id);
		} else {
			npu_drv_err("the entry %u mdc err", entry_id);
		}
	} else {
		// at least one rtsq is occupied
		info0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO0_ADDR(hwts_base, entry_id));
		if (ctrl0.reg.rtsq0_vld == 1) {
			ret = npu_check_and_handle_rtsq(entry_id, info0.reg.rtsqid_0, ctrl0.reg.rtsq0_err);
		}
		if (ctrl0.reg.rtsq1_vld == 1) {
			ret = npu_check_and_handle_rtsq(entry_id, info0.reg.rtsqid_1, ctrl0.reg.rtsq1_err);
		}
	}
	// clear ffrt entry
	writel(0x1, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
	npu_drv_warn("npu handle ffrt entry end");
	return ret;
}

int npu_proc_block_kill_task(u16 uid)
{
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION info1 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION info2 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION dfx0 = { 0 };
	int trytimes = HWTS_RETRY_TIMES;
	int tasksync_entry;
	int ret = 0;
	uint16_t i;

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");
	npu_drv_warn("npu proc block kill task begin");

	mutex_lock(&dev_ctx->ffrt_entry_mutex);
	// polling until receive PushTaskSync
	while (trytimes-- > 0) {
		tasksync_entry = npu_find_pushtask_entry(&ctrl0, &info1, &info2, &dfx0, PUSHTASKSYNC);
		if (tasksync_entry != 0xff) break;
	}
	if (tasksync_entry == 0xff) {
		ret = -1;
		npu_drv_err("npu don't receive pushtasksync signal");
		npu_rdr_exception_report(EXC_TYPE_WAIT_FFTS_EXCEPTION, 0);
		mutex_unlock(&dev_ctx->ffrt_entry_mutex);
		return ret;
	}

	// release entries
	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		if (i != tasksync_entry) {
			npu_read_entry_info(&ctrl0, &info1, &info2, &dfx0, i);
			if (ctrl0.reg.task_vld == 1 && info2.reg.ssid == uid) {
				npu_drv_warn("ffrt entry %u has task, ctrl0 0x%x, info1 0x%x, info2 0x%x, dfx0 0x%x",
					i, ctrl0.value, info1.value, info2.value, dfx0.value);
				ret = npu_handle_ffrt_entry(i, true);
				if (ret != 0)
					npu_drv_err("npu handle ffrt %u entry is not finished", i);
			}
		} else {
			continue;
		}
	}
	writel(0x62, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, tasksync_entry));
	mutex_unlock(&dev_ctx->ffrt_entry_mutex);
	npu_drv_warn("npu proc block kill task end");
	return ret;
}

static int npu_handle_local_task_exception_for_ffts(uint16_t entry_id, uint16_t rtsq_idx, uint16_t channel_id)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
	int ret = 0;
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO0_UNION info0 = { 0 };
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);

	cond_return_error(dev_ctx == NULL, NPU_EXCEPTION_CONTINUE, "dev_ctx is null\n");
	cond_return_error(hwts_base == 0ULL, NPU_EXCEPTION_CONTINUE, "hwts_base is NULL\n");
	if ((!is_channel_id_valid(channel_id)) || (!is_entry_id_valid(entry_id)) || (!is_rtsq_idx_valid(rtsq_idx)))
		return NPU_EXCEPTION_CONTINUE;

	// skip the entry handled by e.g. proc block
	mutex_lock(&dev_ctx->ffrt_entry_mutex);
	ctrl0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_id));
	info0.value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO0_ADDR(hwts_base, entry_id));
	if ((rtsq_idx == 0 && ctrl0.reg.rtsq0_vld == 1 && ctrl0.reg.rtsq0_err == 1 && info0.reg.rtsqid_0 == channel_id)
	|| (rtsq_idx == 1 && ctrl0.reg.rtsq1_vld == 1 && ctrl0.reg.rtsq1_err == 1 && info0.reg.rtsqid_1 == channel_id)) {
		ret = npu_handle_ffrt_entry(entry_id, false);
	} else {
		npu_drv_warn("entry %u is handled by others", entry_id);
	}
	mutex_unlock(&dev_ctx->ffrt_entry_mutex);

	if (ret != 0) {
		npu_drv_err("npu process task exception with entry %u failed", entry_id);
		return NPU_EXCEPTION_CONTINUE;
	}
	npu_drv_warn("npu process task exception with entry %u done", entry_id);
	return NPU_EXCEPTION_HANDLED;
}

int npu_handle_ffts_exception(struct ffts_exception_report_info *report)
{
	int ret = NPU_EXCEPTION_CONTINUE;
	npu_drv_warn("ffts report message type:%u", report->exception_type);

	switch (report->exception_type) {
	case NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION:
	case NPU_EXCEPTION_TYPE_HWTS_TASK_TIMEOUT:
	case NPU_EXCEPTION_TYPE_HWTS_TRAP_EXCEPTION:
		ret = npu_handle_local_task_exception_for_ffts(report->entry_id, report->rtsq_idx, report->channel_id);
		break;
	case NPU_EXCEPTION_TYPE_HWTS_SQE_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_BUS_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_ECC_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_POOL_CONFLICT_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_TIMEOUT:
		break;
	default:
		npu_drv_err("invalid ts report message type:%u", report->exception_type);
	}

	return ret;
}

int npu_ffts_entry_is_clear(void)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	uint16_t i;

	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");

	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		ctrl0.value = readl(
			(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, i));
		if (ctrl0.reg.task_vld == 1)
			return -1;
	}
	return 0;
}
