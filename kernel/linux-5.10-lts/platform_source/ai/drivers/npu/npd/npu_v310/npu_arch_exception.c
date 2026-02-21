/*
* npu_arch_exception.c
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

#include "npu_arch_exception.h"

#include <linux/kthread.h>
#include <linux/delay.h>

#include "npu_common_resource.h"
#include "npu_except_mgr.h"
#include "npu_log.h"
#include "npu_hwts_plat.h"
#include "npu_platform_exception.h"
#include "npu_sink_task_verify.h"
#include "npu_sink_sqe_fmt.h"
#include "npu_ipc.h"
#include "npu_ipc_msg.h"
#include "npu_shm.h"
#include "npu_pool.h"
#include "npu_pm_internal.h"
#ifdef CONFIG_NPU_FFRT
#include "npu_ffts_exception.h"
#endif
#include "hwts_driver.h"
#include "npu_pm_framework.h"
#ifdef CONFIG_NPU_OHOS
#include "npu_udk.h"
#endif

#define NPU_EXCEPTION_TIMEOUT_IN_MS 100

#ifndef CONFIG_NPU_OHOS
bool is_channel_id_valid(uint16_t channel_id)
{
	if (channel_id >= NPU_HWTS_CHANNEL_NUM) {
		npu_drv_err("invalid param channel_id = %u\n", channel_id);
		return false;
	}
	return true;
}
#endif

static int32_t handle_exception_svc(void *arg)
{
	struct npu_dev_ctx *dev_ctx = (struct npu_dev_ctx *)arg;

	npu_drv_warn("in exception thread");

	do {
		msleep(NPU_EXCEPTION_TIMEOUT_IN_MS);
		npu_raise_global_reg_exception(dev_ctx);
	} while (!kthread_should_stop());
	return 0;
}

int32_t npu_exception_svc_init(struct npu_dev_ctx *dev_ctx)
{
	if (dev_ctx == NULL) {
		npu_drv_err("dev_ctx is null");
		return -1;
	}
	npu_drv_debug("enter");

	dev_ctx->except_manager.status = NPU_STATUS_NORMAL;

	npu_drv_warn("start npu exception thread");
	dev_ctx->except_manager.thread = kthread_run(handle_exception_svc, dev_ctx, "npu_excp");
	if (dev_ctx->except_manager.thread == NULL) {
		npu_drv_err("start exception thead fail");
		return -1;
	}
	npu_drv_warn("start npu exception thread succ");

	return 0;
}

void npu_exception_svc_deinit(struct npu_dev_ctx *dev_ctx)
{
	if (dev_ctx == NULL) {
		npu_drv_err("dev_ctx is null");
		return;
	}

	npu_drv_warn("end npu exception thread");
	kthread_stop(dev_ctx->except_manager.thread);
	dev_ctx->except_manager.thread = NULL;
	npu_drv_warn("end npu exception thread succ");

	return;
}

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

static int npu_handle_task_exception(struct npu_dev_ctx *dev_ctx, u16 channel_id)
{
	int ret = 0;
	enum npu_subip subip = npu_get_subip_by_channel_id(dev_ctx, channel_id);
	if (subip == NPU_SUBIP_MAX) {
		return NPU_EXCEPTION_HANDLED;
	}

	npu_pm_safe_call_with_return(dev_ctx, subip, npu_exception_aicore_reset(channel_id), ret);
	if (ret != 0) {
		npu_drv_warn("aicore reset failed, need reset npu to handle exception");
		return NPU_EXCEPTION_CONTINUE;
	}

	// reset succ; inform ts to disable sq, send cq, free channel
	ret = send_ipc_msg_to_ts(IPCDRV_TS_HWTS_TASK_EXCEPTION, IPCDRV_MSG_ASYNC,
		(u8 *)&channel_id, sizeof(channel_id), IPCDRV_SEND_WITH_ACK);
	if (ret != 0) {
		npu_drv_warn("send ipc to ts failed, need reset npu to handle exception");
		return NPU_EXCEPTION_CONTINUE;
	}

	return NPU_EXCEPTION_HANDLED;
}

static int npu_handle_hwts_exception(struct npu_dev_ctx *dev_ctx, struct hwts_exception_report_info *report)
{
	npu_drv_warn("ts report message type:%u", report->exception_type);

	switch (report->exception_type) {
	case NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION:
	case NPU_EXCEPTION_TYPE_HWTS_TASK_TIMEOUT:
	case NPU_EXCEPTION_TYPE_HWTS_TRAP_EXCEPTION:
		return npu_handle_task_exception(dev_ctx, report->channel_id);
	case NPU_EXCEPTION_TYPE_HWTS_BUS_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_ECC_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_POOL_CONFLICT_ERROR:
	case NPU_EXCEPTION_TYPE_HWTS_TIMEOUT:
		break;
	default:
		npu_drv_err("invalid ts report message type:%u", report->exception_type);
	}

	return NPU_EXCEPTION_CONTINUE;
}

int npd_handle_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	int ret = NPU_EXCEPTION_CONTINUE;
#ifdef CONFIG_NPU_FFRT
	npu_ffap1_s status;
#endif

	switch (er->ex_source) {
	case NPU_EXCEPTION_SRC_TS:
		ret = npu_handle_hwts_exception(dev_ctx, er->u.ex_hwts_report);
		break;
#ifdef CONFIG_NPU_FFRT
	case NPU_EXCEPTION_SRC_FFTS:
		npu_pm_safe_call_with_return(dev_ctx, NPU_IP_TSCPU, 
			npu_handle_ffts_exception(er->u.ex_ffts_report), ret);
		break;
#endif
	case NPU_EXCEPTION_SRC_GLOBAL_REG:
	case NPU_EXCEPTION_SRC_QIC:
	case NPU_EXCEPTION_SRC_TASK_TIMEOUT:
	case NPU_EXCEPTION_SRC_SERVER:
	default:
		break;
	}
#ifdef CONFIG_NPU_FFRT
	status = atomic_read(&dev_ctx->ffap1_status_manager);
	npu_drv_warn("ffap1_status_manager is %d", status);
	if (ret == NPU_EXCEPTION_CONTINUE && status != DISCONNECTED && status != DISCONNECTED_HOLD)
		npu_pm_safe_call_with_return(dev_ctx, NPU_IP_TSCPU, npu_handle_global_exception_for_ffts(), ret);
#endif
	return ret;
}