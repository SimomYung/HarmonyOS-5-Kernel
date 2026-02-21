/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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

#ifndef DKMD_CMDS_INFO_H
#define DKMD_CMDS_INFO_H

#include <linux/types.h>

enum dkmd_cmds_type {
	DKMD_CMDS_TYPE_BL,         /* backlight */
	DKMD_CMDS_TYPE_PT_RECT,    /* partial tx rectangle */
	DKMD_CMDS_TYPE_PPU_RECT,   /* panel partial update rectangle */
	DKMD_CMDS_TYPE_PPU_ENTER,  /* panel partial update mode enter */
	DKMD_CMDS_TYPE_PPU_EXIT,   /* panel partial update mode exit */
	DKMD_CMDS_TYPE_SFR,        /* panel safe frame rate */
	DKMD_CMDS_TYPE_COMMON,     /* no constraint type */
	DKMD_CMDS_TYPE_MAX
};

#define DKMD_CMDS_DATA_LEN 20
struct dkmd_cmds_info {
	enum dkmd_cmds_type cmds_type;
	int32_t data[DKMD_CMDS_DATA_LEN];
	int32_t data_len;
};

#endif
