/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */


#ifndef HVGR_HTS_H
#define HVGR_HTS_H

#define HTS_EVENT_REF_LOAD                      (0u << 1)
#define HTS_EVENT_REF_STORE                     (1u << 1)
#define HTS_EVENT_REF_CLEAR                     (2u << 1)
#define HTS_EVENT_CMD_START                      1

#define HVGR_HTS_EVENT_PAGES                     1
#define HTS_EVENT_REGISTER_MAX                   255
#define DEV_GPU                                  2
#define HVGR_CLEAN_GPU_REF_RAM                   1
#define HVGR_CLEAN_HTS_EVENT                     2
#define HVGR_HTS_TRY_TIMES_MAX                   1000

#endif