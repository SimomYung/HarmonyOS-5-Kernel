/*
* npu_ffts_exception_ldk.c
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
#include "npu_ffts_exception_ldk.h"

#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>

#include "npu_arch_exception.h"
#include "npu_except_mgr.h"
#include "npu_log.h"
#include "npu_ipc.h"
#include "npu_ipc_msg.h"
#include "npu_pm_internal.h"
#include "npu_soc_defines.h"
#include "soc_ffap1_interface.h"
#include "npu_hwts_plat.h"
#include "npu_shm.h"
#include "hwts_driver.h"
#include "npu_pm_framework.h"
#include "npu_platform_exception.h"
#ifdef CONFIG_NPU_SINK_UDK
#include "npu_udk.h"
#else
#include "ffts_hw_config.h"
#include "npu_ffts_exception_udk.h"
#endif
#include "bbox/npu_dfx_black_box.h"
#include "npu_common_resource.h"
#include "npu_except_mgr.h"


int npu_ffts_clear_entry_by_taskidx(uint16_t task_idx)
{
	uint16_t i;
	int ret = -1;
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	npu_drv_warn("task %u clear entry start", task_idx);
	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		npu_pm_safe_call_with_return(dev_ctx, NPU_IP_TSCPU, npu_ffts_clear_entry(task_idx, i), ret);
		if (ret == 0) {
			npu_drv_warn("ffrt entry %u has task %u and clear success", i, task_idx);
			return ret;
		}
	}

	npu_drv_warn("non ffrt entry need clear");
	return ret;
}

int npu_proc_block_resource_clear(u16 uid)
{
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	int ret = 0;
	npu_ffap1_s status;

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	npu_drv_warn("npu resource clear begin");

	status = atomic_read(&dev_ctx->ffap1_status_manager);
	switch (status) {
	case DISCONNECTED_HOLD:
		atomic_set(&dev_ctx->ffap1_status_manager, DISCONNECTED);
		break;

	case CONNECTED_HOLD:
		ret = npu_proc_block_kill_task(uid);
		if (ret != 0) {
			npu_drv_err("npu proc block kill task is not finished");
			npu_set_exception_bits();
		}
		atomic_set(&dev_ctx->ffap1_status_manager, CONNECTED);
		break;

	default:
		npu_drv_err("Failed to clear resources for process blocking, please hold ffap1 status first. ffap1 status=%d",
			status);
		return -1;
	}

	npu_drv_warn("npu resource clear end");
	return ret;
}