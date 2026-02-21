/*
* npu_pm_ffap1.c
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

#include "npu_pm_ffap1.h"

#include <linux/random.h>

#include "npu_atf_subsys.h"
#include "npu_pm_internal.h"
#include "npu_log.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_common.h"

#define NPU_MAX_TRYTIME 1000

int npu_ffap1_connect(u32 work_mode)
{
	uint64_t ffts_flag = 1;
	int ret = 0;
	npu_ffap1_s status;
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	int trytimes = NPU_MAX_TRYTIME;
	npu_drv_warn("ffap1 connect start\n");
	while(trytimes-- > 0) {
		status = atomic_cmpxchg(&dev_ctx->ffap1_status_manager, DISCONNECTED, CONNECTING);
		switch (status) {
		case CONNECTED:
		case CONNECTED_HOLD:
			npu_drv_warn("ffap1 already connected\n");
			return 0;
		case CONNECTING:
		case DISCONNECTING:
		case DISCONNECTED_HOLD:
			mdelay(1);
			break;
		case DISCONNECTED:
			ret = atfd_service_npu_smc(NPU_CONNECT_FFRT_MODE_FFAP1, 0, work_mode, ffts_flag);
			if (ret == 0) {
				atomic_set(&dev_ctx->ffap1_status_manager, CONNECTED);
				npu_drv_warn("ffap1 connect succ.");
			} else {
				atomic_set(&dev_ctx->ffap1_status_manager, DISCONNECTED);
				npu_drv_err("ffap1 connect failed, ret = %d", ret);
			}
			return ret;
		default:
			npu_drv_err("invalid ffap1 status %d", status);
			return -1;
		}
	}

	npu_drv_err("proc block is not finished");
	return -1;
}

int npu_ffap1_disconnect(u32 work_mode)
{
	uint64_t ffts_flag = 1;
	int ret = 0;
	npu_ffap1_s status;
	int trytimes = NPU_MAX_TRYTIME;
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);

	npu_drv_warn("ffap1 disconnect start\n");
	while(trytimes-- > 0) {
		status = atomic_cmpxchg(&dev_ctx->ffap1_status_manager, CONNECTED, DISCONNECTING);
		switch (status) {
		case CONNECTED:
			ret = atfd_service_npu_smc(NPU_DISCONNECT_FFRT_MODE_FFAP1, 0, work_mode, ffts_flag);
			if (ret == 0) {
				atomic_set(&dev_ctx->ffap1_status_manager, DISCONNECTED);
				npu_drv_warn("ffap1 disconnect succ.");
			} else {
				atomic_set(&dev_ctx->ffap1_status_manager, CONNECTED);
				npu_drv_err("ffap1 disconnect failed, ret= %d", ret);
			}
			return ret;
		case CONNECTED_HOLD:
		case CONNECTING:
		case DISCONNECTING:
			mdelay(1);
			break;
		case DISCONNECTED:
		case DISCONNECTED_HOLD:
			npu_drv_warn("ffap1 already disconnected\n");
			return 0;
		default:
			npu_drv_err("invalid ffap1 status %d", status);
			return -1;
		}
	}
	npu_drv_err("proc block is not finished");
	return -1;
}

int npu_ffap1_status_hold(void)
{
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	npu_ffap1_s status;
	int trytimes = NPU_MAX_TRYTIME;

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");

	while (trytimes-- > 0) {
		status = atomic_read(&dev_ctx->ffap1_status_manager);
		switch (status) {
		case CONNECTED:
			status = atomic_cmpxchg(&dev_ctx->ffap1_status_manager, CONNECTED, CONNECTED_HOLD);
			if (status == CONNECTED) {
				return 0;
			} else {
				udelay(10);
			}
			break;
		case DISCONNECTED:
			status = atomic_cmpxchg(&dev_ctx->ffap1_status_manager, DISCONNECTED, DISCONNECTED_HOLD);
			if (status == DISCONNECTED) {
				return 0;
			} else {
				udelay(10);
			}
			break;
		case CONNECTED_HOLD:
		case DISCONNECTED_HOLD:
			npu_drv_err("ffap1 already hold");
			return -1;
		case CONNECTING:
		case DISCONNECTING:
			udelay(10);
			break;
		default:
			npu_drv_err("invalid ffap1 status %d", status);
			return -1;
		}
	}

	if (trytimes == 0) {
		npu_drv_err("ffap1 status cant hold");
		return -1;
	}
	return 0;
}