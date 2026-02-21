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