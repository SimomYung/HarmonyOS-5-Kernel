/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als gamma config header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __ALS_GAMMA_CONFIG_H__
#define __ALS_GAMMA_CONFIG_H__

#if ((defined DSS_ALSC_NOISE) || (defined DKMD_ALSC_NOISE))
#include <linux/types.h>

bool als_gamma_param_config(uint32_t tag, uint32_t mipi);
bool threshold_by_product(uint8_t phone_type, uint8_t lcd_type, int32_t tag);
#ifdef DSS_ALSC_NOISE
void als_gamma_set_alpha(uint32_t alpha);
#elif defined DKMD_ALSC_NOISE
void als_gamma_set_alpha(uint32_t alpha, uint32_t panel_id);
#endif
int als_gamma_update_with_alpha(int tag);
#endif

enum als_current_support_gamma {
	GAMMA_12 = 12,
	GAMMA_13,
	GAMMA_14,
	GAMMA_15,
	GAMMA_16,
	GAMMA_17,
	GAMMA_18,
	GAMMA_19,
	GAMMA_20,
	GAMMA_21,
	GAMMA_22,
	GAMMA_23,
	GAMMA_24,
};

#define R_TYPE            0
#define G_TYPE            1
#define B_TYPE            2
#define GAMMA_TYPE_NUM    3

extern unsigned char g_als_manual_gamma[GAMMA_TYPE_NUM];

#endif
