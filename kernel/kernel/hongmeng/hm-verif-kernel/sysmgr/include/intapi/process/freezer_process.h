/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Process-related interfaces on which the freezer depends
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 09 10:31:44 CST 2024
 */

#ifndef H_SYSMGR_INCLUDE_PROCESS_FREEZER_PROCESS_H
#define H_SYSMGR_INCLUDE_PROCESS_FREEZER_PROCESS_H

bool resmgr_freezer_is_process_thawed(uint32_t cnode_idx);

#endif /* H_SYSMGR_INCLUDE_PROCESS_FREEZER_PROCESS_H */
