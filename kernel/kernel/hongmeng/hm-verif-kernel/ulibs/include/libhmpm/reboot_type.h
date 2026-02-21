/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 * Description: Get reboot type
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 18 19:36:32 2021
 */
#ifndef TPPMGR_PWRMGR_REBOOT_TYPE_H
#define TPPMGR_PWRMGR_REBOOT_TYPE_H

#include <hongmeng/errno.h>
#include <libhmpm/shutdown.h>

int hm_reboot_cmd_to_type(const char *cmd, unsigned int *ret_type);
#endif
