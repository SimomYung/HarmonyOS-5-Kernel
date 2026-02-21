/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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
#include <linux/module.h>
#include "uksm_debug.h"
#include "ukmd_log.h"

uint32_t g_ukmd_log_level = UKMD_LOG_LVL_INFO;
uint32_t g_debug_ukmd_fence = 0;
int32_t g_cmdlist_dump_enable = 0;

