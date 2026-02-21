/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: microarch cfg ctrl externel func
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 20 11:00:00 CST 2024
 */
#ifndef HMKERNEL_MICROARCH_CFG_CTRL_API_H
#define HMKERNEL_MICROARCH_CFG_CTRL_API_H

#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <hmkernel/power/energy_model.h>


#define MAX_CLUSTER_NUM 3
 
#ifdef CONFIG_MICROARCH_CFG_CTRL
void set_microarch_cfg_ctrl(unsigned int data_id, unsigned int cluster);
int set_task_microarch_cfg(struct tcb_s *tcb, unsigned int scene_id);
#else
static inline void set_microarch_cfg_ctrl(unsigned int profile_id, unsigned int cluster) {}
static inline int set_task_microarch_cfg(struct tcb_s *tcb __maybe_unused, unsigned int scene_id __maybe_unused)
{
	return E_HM_INVAL;
}
#endif

#endif /* HMKERNEL_MICROARCH_CFG_CTRL_API_H */
