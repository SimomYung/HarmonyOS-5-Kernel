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
#include <linux/types.h>
#include "dkmd_aod_itf.h"
#if defined(CONFIG_DKMD_DPU_AOD)
#include "dpu_aod_device_itf.h"
#endif

bool dpu_aod_get_lcd_always_on(void)
{
#if defined(CONFIG_DKMD_DPU_AOD)
	return get_lcd_always_on() > 0;
#else
	return 0;
#endif
}

int dpu_aod_peri_handle(uint32_t panel_id, int blank_mode)
{
#if defined(CONFIG_DKMD_DPU_AOD)
	return dpu_aod_panel_handle(panel_id, blank_mode);
#else
	(void)panel_id;
	(void)blank_mode;
	return 0;
#endif
}