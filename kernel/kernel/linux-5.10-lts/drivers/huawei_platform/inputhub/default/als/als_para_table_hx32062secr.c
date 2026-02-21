/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: als para table hx32062secr source file
 * Author: huangjiaqing <huangjiaqing4@huawei.com>
 * Create: 2023-06-12
 */

#include "als_para_table_hx32062secr.h"

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

static hx32062se_als_para_table hx32062secr_als_para_diff_tp_color_table[] = {
	// Historical Parameters
	{ VDE, V3, DEFAULT_TPLCD, OTHER,
		{ 1546,
		-7265, -30621, 7444, 0, 8000,
		1,
		-9583, 4331, 19797, 0, 2000,
		0, 1, 2, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		2, 14061, 16406, 10507, 0,
		4000, 10
		}
	},
};

hx32062se_als_para_table *als_get_hx32062secr_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(hx32062secr_als_para_diff_tp_color_table))
		return NULL;
	return &(hx32062secr_als_para_diff_tp_color_table[id]);
}

hx32062se_als_para_table *als_get_hx32062secr_first_table(void)
{
	return &(hx32062secr_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_hx32062secr_table_count(void)
{
	return ARRAY_SIZE(hx32062secr_als_para_diff_tp_color_table);
}
