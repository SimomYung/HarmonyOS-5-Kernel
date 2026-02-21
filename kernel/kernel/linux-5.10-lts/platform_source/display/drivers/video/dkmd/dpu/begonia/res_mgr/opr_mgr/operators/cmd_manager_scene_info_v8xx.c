/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "cmd_manager_impl.h"
#include <linux/slab.h>
#include <dpu/dpu_dm.h>
#include <ukmd_cmdlist.h>
#include "dkmd_base_frame.h"
#include "dkmd_network.h"
#include "opr_cmd_data_interface.h"
#include "cmdlist_interface.h"
#include "dkmd_log.h"
#include "config/dpu_config_utils.h"

void set_dm_scene_info(uint32_t scene_id, uint32_t scene_mode, struct dpu_dm_param *dm_param)
{
	dm_param->scene_info.srot_number.reg.scene_id = scene_id;
	dm_param->ov_info.scene_mode.reg.scene_mode = scene_mode;
	dm_param->ddic_info.ddic_layer_id.reg.ddic_demura_lut_layer_id = 0x1f;
	dm_param->ddic_info.ddic_layer_id.reg.ddic_deburnin_lut_layer_id = 0x1f;
	dm_param->ddic_info.ddic_layer_id.reg.ddic_od_lut_layer_id = 0x1f;
}