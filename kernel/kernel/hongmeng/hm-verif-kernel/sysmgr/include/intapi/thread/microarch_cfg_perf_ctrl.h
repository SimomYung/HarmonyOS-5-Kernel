/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: microarch_cfg_perf_ctrl.h
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 20 11:20:00 2024
 */
#ifndef SYSMGR_INTAPI_MICROARCH_CFG_PERF_CTRL_H
#define SYSMGR_INTAPI_MICROARCH_CFG_PERF_CTRL_H

#ifdef CONFIG_MICROARCH_CFG_CTRL
struct microarch_cfg_scene_data {
    int pid;
    int scene_id;
};
#endif

#endif
