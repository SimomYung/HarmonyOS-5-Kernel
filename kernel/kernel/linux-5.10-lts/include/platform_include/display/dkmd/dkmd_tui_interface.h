/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
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

#ifndef DKMD_TUI_INTERFACE_H
#define DKMD_TUI_INTERFACE_H

#include "dkmd_dpu.h"
#include <linux/types.h>

enum tui_rotate_type {
	TUI_DISPLAY_ROT_0 = 0,
	TUI_DISPLAY_ROT_180 = 1
};

// TUI_LEVEL0: hardware replace fake layer with trusted layer, secure buffer. use as secure_pay
// TUI_LEVLE1: software replace fake layer with secure layer, normal buffer with right control. use as indicator
enum dpu_sec_event {
    DPU_SEC_EXIT_ALL = 0,
    DPU_SEC_ENTER_LEVEL0 = 1, // use such as secure_pay
    DPU_SEC_ENTER_LEVEL1 = 2, // use such as indicator
	DPU_SEC_RESTART_ACK = 255 // ack from teeos to confirm restart power off status update in teeos
};

struct dpu_secure_info {
	uint32_t panel_type;
	uint32_t panel_xres;
	uint32_t panel_yres;
};

#ifdef CONFIG_TEE_TUI
bool dpu_tui_is_power_on(void);
int32_t dpu_tui_get_active_display_rect(struct dkmd_rect *active_rect);
int32_t dpu_tui_get_rotation(void);
int32_t dpu_tui_get_panel_info(struct dpu_secure_info *panel_info);
#else
inline bool dpu_tui_is_power_on(void)
{
	return false;
}
inline int32_t dpu_tui_get_active_display_rect(struct dkmd_rect *active_rect)
{
	return 0;
}
inline int32_t dpu_tui_get_rotation(void)
{
	return 0;
}
inline int32_t dpu_tui_get_panel_info(struct dpu_secure_info *panel_info)
{
	(void)panel_info;
	return 0;
}
#endif

#endif /* DKMD_TUI_INTERFACE_H */
