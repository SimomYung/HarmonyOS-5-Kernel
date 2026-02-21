/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table ams tmd3702 source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include "als_para_table_ams_tsl2540.h"

#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <securec.h>

#include "als_tp_color.h"
#include "contexthub_boot.h"
#include "contexthub_route.h"
/*Parameter Configuration Description
	{phone_type,phone_version,tp_lcd,tp_color,
		{
			again,lux_coef_h_clear,lux_coef_h_red,lux_coef_h_green,lux_coef_h_blue,lux_coef_h_dgf,
			lux_coef_h_offset,lux_coef_l_clear,lux_coef_l_red,lux_coef_l_green,lux_coef_l_blue,
			lux_coef_l_dgf,lux_coef_l_offset,lux_ir_comp,lux_ir_div_point,
			cct_coef_h_blue_red,cct_coef_h_blue_green,cct_coef_h_green_red,cct_coef_h_offset,
			cct_coef_l_blue_red,cct_coef_l_blue_green,cct_coef_l_green_red,cct_coef_l_offset,
			cct_ir_comp,cct_ir_div_point,cct_cal,
			target_clear,target_red,target_green,target_blue,threshold_max,threshold_min
		}
	}
*/
static sip3642_als_para_table sip3642_als_para_diff_tp_color_table[] = {
	{ ADY, V3, BOE_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ ADY, V3, BOE_TPLCD_B12, OTHER,
		{ 64, 0, -4424, 16012, -2256, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ ADY, V3, DEFAULT_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ HBN, V3, BOE_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ HBN, V3, DEFAULT_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ PLA, V3, BOE_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ PLA, V3, DEFAULT_TPLCD, OTHER,
		{ 64, 0, -11238, 19879, 2597, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8740, 10287, 15684, 11804, 4000, 250 }
	},
	{ HED, V3, DEFAULT_TPLCD, OTHER,
		{ 64, -4135, 21977, -14444, 19991, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9034, 10278, 13771, 9310, 4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0 }
	},
	{ LMR, V3, BOE_TPLCD, OTHER,
		{ 64, -1233, 7102, 1183, 7702, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10710, 12712, 15217, 9754, 4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0 }
	},
	{ LMR, V3, DEFAULT_TPLCD, OTHER,
		{ 64, -4135, 21977, -14444, 19991, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9034, 10278, 13771, 9310, 4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0 }
	},
	{ LMU, V3, BOE_TPLCD, OTHER,
		{ 64, -1233, 7102, 1183, 7702, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10710, 12712, 15217, 9754, 4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0 }
	},
	{ LMU, V3, DEFAULT_TPLCD, OTHER,
		{ 64, -4135, 21977, -14444, 19991, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9034, 10278, 13771, 9310, 4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0 }
	},
};

sip3642_als_para_table *als_get_sip3642_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(sip3642_als_para_diff_tp_color_table))
		return NULL;
	return &(sip3642_als_para_diff_tp_color_table[id]);
}

sip3642_als_para_table *als_get_sip3642_first_table(void)
{
	return &(sip3642_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_sip3642_table_count(void)
{
	return ARRAY_SIZE(sip3642_als_para_diff_tp_color_table);
}
