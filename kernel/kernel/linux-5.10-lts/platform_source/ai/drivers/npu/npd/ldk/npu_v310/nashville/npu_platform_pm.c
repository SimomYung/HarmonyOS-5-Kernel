/*
 * npu_platform_pm.c
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

#include "npu_log.h"
#include "npu_platform.h"
#include "npu_pm_top_common.h"
#include "npu_pm_top_specify.h"
#include "npu_pm_non_top_common.h"
#include "npu_pm_non_top_specify.h"
#include "npu_pm_framework.h"
#include "npu_platform_pm.h"

enum npu_subip g_powerup_order[NPU_SUBIP_MAX] = {
	NPU_IP_TOP,
	NPU_IP_NON_TOP,
	NPU_IP_AICORE_GE,
	NPU_IP_AICORE_EFF,
	NPU_IP_TSCPU
};

enum npu_subip g_powerdown_order[NPU_SUBIP_MAX] = {
	NPU_IP_TSCPU,
	NPU_IP_AICORE_EFF,
	NPU_IP_AICORE_GE,
	NPU_IP_NON_TOP,
	NPU_IP_TOP
};

struct npu_pm_subip_action g_pm_subip_action_talbe[NPU_SUBIP_MAX] = {
	{NPU_IP_TOP, npu_top_powerup, npu_top_powerdown},
	{NPU_IP_NON_TOP, npu_non_top_powerup, npu_non_top_powerdown},
	{NPU_IP_AICORE_GE, npu_aicore_gene_powerup, npu_aicore_gene_powerdown},
	{NPU_IP_AICORE_EFF, npu_aicore_eff_powerup, npu_aicore_eff_powerdown},
	{NPU_IP_TSCPU, npu_tscpu_powerup, npu_tscpu_powerdown},
};

/*
 * NONSEC = 0              :null;                    support idle powerdown;     need all subips
 * SEC = 1                 :null;                    not support idle powerdown; need all except NPU_IP_TSCPU
 * ISPNN_SEPARATED = 2     :null;                    not support idle powerdown; need all subips
 * ISPNN_SHARED = 3        :null;                    null;                       null
 * INIT = 4                :null;                    not support idle powerdown; need all except NPU_IP_AICORE_GE&EFF
 * NPU_NONSEC_GE = 5       :null;                    null;                       need all except NPU_IP_AICORE_EFF
 * NPU_NONSEC_EFF = 6      :null;                    null;                       need all except NPU_IP_AICORE_GE
 * NPU_NONSEC_TS = 7       :null;                    not support idle powerdown; need all except NPU_IP_AICORE_GE&EFF
 * NPU_FFRT = 8            :null;                    null;                       need all subips
 * NPU_FFRT_GE = 9         :null;                    null;                       need all except NPU_IP_AICORE_EFF
 * NPU_FFRT_EFF = 10       :null;                    null;                       need all except NPU_IP_AICORE_GE
 * NPU_HTS = 11            :null;                    null;                       need all subips
 * NPU_HTS_GE = 12         :null;                    null;                       need all except NPU_IP_AICORE_EFF
 * NPU_HTS_EFF = 13        :null;                    null;                       need all except NPU_IP_AICORE_GE
 */
struct npu_pm_work_strategy g_pm_work_strategy_table[NPU_WORKMODE_MAX] = {
	{
		NPU_NONSEC,
		{0},
		false,
		{NPU_SUBIP_ALL}
	},
	{
		NPU_SEC,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_TSCPU)}
	},
	{
		NPU_ISPNN_SEPARATED,
		{UINT32_MAX},
		0,
		{0}
	},
	{
		NPU_ISPNN_SHARED,
		{0},
		false,
		{NPU_SUBIP_ALL}
	},
	{
		NPU_INIT,
		{0},
		false,
		{npu_bitmap_clear(npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_GE),
		NPU_IP_AICORE_EFF)}
	},
	{
		NPU_NONSEC_GE,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_NONSEC_EFF,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_GE)}
	},
	{
		NPU_NONSEC_TS,
		{0},
		false,
		{npu_bitmap_clear(npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_GE),
		NPU_IP_AICORE_EFF)}
	},
	{
		NPU_FFRT,
		{0},
		false,
		{NPU_SUBIP_ALL}
	},
	{
		NPU_FFRT_GE,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_FFRT_EFF,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_GE)}
	},
	{
		NPU_HTS,
		{0},
		false,
		{NPU_SUBIP_ALL}
	},
	{
		NPU_HTS_GE,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_HTS_EFF,
		{0},
		false,
		{npu_bitmap_clear(NPU_SUBIP_ALL, NPU_IP_AICORE_GE)}
	}
};

/*
 * interframe_idle_work_strategy:
 * NONSEC = 0           :null;  null;     null
 * SEC = 1              :null;  null;     null
 * ISPNN_SEPARATED = 2  :null;  null;     null
 * ISPNN_SHARED = 3     :null;  null;     null
 * INIT = 4             :null;  null;     null
 * NPU_NONSEC_GE = 5    :null;  null;     null
 * NPU_NONSEC_EFF = 6   :null;  null;     null
 * NPU_NONSEC_TS = 7    :null;  null;     null
 * NPU_FFRT = 8         :null;  null;     null
 * NPU_FFRT_GE = 9      :null;  null;     null
 * NPU_FFRT_EFF = 20    :null;  null;     null
 * NPU_HTS = 11         :null;  null;     null
 * NPU_HTS_GE = 12      :null;  null;     null
 * NPU_HTS_EFF = 13     :null;  null;     null
 */
static struct npu_pm_work_strategy g_pm_interframe_idle_work_strategy_table[
	NPU_WORKMODE_MAX] = {
	{
		NPU_NONSEC,
		{0},
		true,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE) | npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_SEC,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE) | npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_ISPNN_SEPARATED,
		{UINT32_MAX},
		false,
		{0}
	},
	{
		NPU_ISPNN_SHARED,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE) | npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_INIT,
		{0},
		0,
		{0}
	},
	{
		NPU_NONSEC_GE,
		{0},
		true,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE)}
	},
	{
		NPU_NONSEC_EFF,
		{0},
		true,
		{npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_NONSEC_TS,
		{0},
		0,
		{0}
	},
	{
		NPU_FFRT,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE) | npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_FFRT_GE,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE)}
	},
	{
		NPU_FFRT_EFF,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_HTS,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE) | npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	},
	{
		NPU_HTS_GE,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_GE)}
	},
	{
		NPU_HTS_EFF,
		{0},
		false,
		{npu_bitmap_set(0, NPU_IP_AICORE_EFF)}
	}
};

u32 g_pm_interfram_idle_subip = npu_bitmap_set(0, NPU_IP_AICORE_EFF);

int npu_pm_init(struct npu_dev_ctx *dev_ctx)
{
	struct npu_power_mgr *power_mgr = NULL;
	struct npu_interframe_idle_manager *interframe_power_manager = NULL;
	struct npu_platform_info *plat_info =npu_plat_get_info();

	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	cond_return_error(plat_info == NULL, -1, "plat_info is null\n");

	power_mgr = &dev_ctx->pm;
	power_mgr->strategy_table_addr =
		(vir_addr_t)(uintptr_t)&g_pm_work_strategy_table[0];
	power_mgr->action_table_addr =
		(vir_addr_t)(uintptr_t)&g_pm_subip_action_talbe[0];
	power_mgr->powerup_order = g_powerup_order;
	power_mgr->powerdown_order = g_powerdown_order;
	power_mgr->power_array_size = ARRAY_SIZE(g_powerup_order);

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_INTERFRAME_IDLE_POWER_DOWN]) {
		interframe_power_manager = &power_mgr->interframe_idle_manager;
		interframe_power_manager->idle_subip = g_pm_interfram_idle_subip;
		interframe_power_manager->strategy_table_addr =
			(vir_addr_t)(uintptr_t)&g_pm_interframe_idle_work_strategy_table[0];
	}

	return 0;
}

int npu_plat_switch_hwts_aicore_pool(struct npu_dev_ctx *dev_ctx,
	npu_work_mode_info_t *work_mode_info, uint32_t power_status)
{
	unused(dev_ctx);
	unused(work_mode_info);
	unused(power_status);

	/* no need to switch hwts aicore pool in this version */
	return 0;
}

int npu_plat_dev_pm_suspend(void)
{
	return 0;
}

int npu_nosec_get_workmode(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *comm_task)
{
	int work_mode;
	u16 model_id;
	int ret;
	npu_model_desc_sub_t model_desc_sub = {0};
	if (comm_task->type == NPU_TASK_MODEL_EXECUTE) {
		model_id = (u16)comm_task->u.model_execute_task.model_id;
		ret = npu_proc_get_model_info(proc_ctx, model_id, &model_desc_sub);
		if (ret > 0)
			npu_drv_err("npu_proc_get_model_info error, ret= %d\n", ret);
		work_mode = (int)model_desc_sub.workmode;
	} else {
		work_mode = (int)NPU_NONSEC_TS;
	}
	npu_drv_debug("work mode set %u\n", work_mode);
	return work_mode;
}

enum npu_subip npu_get_subip_by_sqe_type(u8 sqe_type)
{
	switch (sqe_type) {
	case NPU_HWTS_SQE_AICORE:
		return NPU_IP_AICORE_GE;
	case NPU_HWTS_SQE_VECTOR_CORE:
		return NPU_IP_AICORE_GE;
	case NPU_HWTS_SQE_AICORE_EFF:
		return NPU_IP_AICORE_EFF;
	default:
		npu_drv_warn("get invalid subip, sqe_type = %u\n", sqe_type);
		return NPU_SUBIP_MAX;
	}
}