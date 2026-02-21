/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fusion entry headers
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#ifndef __UDK_FUSION_H__
#define __UDK_FUSION_H__

#define UDK_DEVHOST_NAME		"sdgrp"
#define UDK_FUSION_MAGIC_RREF		ERR_TO_REF(E_HM_NODEV)

/* drivers */
extern int udk_drivers_builtin_init(void); /* from extmodules */
extern int udk_fusion_sd_proxy_init(void); /* from common/sd */
/* framework */
extern int udk_fusion_udk_pm_init(void);
extern int udk_fusion_udk_timer_process_init(void);
extern int udk_fusion_udk_anon_file_init(void);

extern int udk_fusion_probe_resources_init(void);

extern int udk_fusion_trace_init(void);

extern int udk_fusion_block_workqueue_init(void);

int udk_fusion_entry(int argc, char **argv);
int udk_pm_action(unsigned int action);
bool udk_fusion_cnode_is_devmgr(void);

#endif
