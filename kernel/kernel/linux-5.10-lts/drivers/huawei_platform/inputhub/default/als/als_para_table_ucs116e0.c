/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: als para table ucs116e0 source file 
 * Author: wangmenghui 30062392
 * Create: 2024-10-23
 */
 
#include "als_para_table_ucs116e0.h"
 
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
 
static ucs116e0_als_para_table ucs116e0_als_para_diff_tp_color_table[] = {
	{ MNT, V3, DEFAULT_TPLCD, OTHER,
		{128, 147, 2394, 0, 0, 1, 
		1, 
		490, 2334, 0, 0, 1,
	    1,
		6, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 279, // r c
	    378, 0, 0, 5000, 200
		}
	},
};
 
ucs116e0_als_para_table *als_get_ucs116e0_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(ucs116e0_als_para_diff_tp_color_table))
		return NULL;
	return &(ucs116e0_als_para_diff_tp_color_table[id]);
}
 
ucs116e0_als_para_table *als_get_ucs116e0_first_table(void)
{
	return &(ucs116e0_als_para_diff_tp_color_table[0]);
}
 
uint32_t als_get_ucs116e0_table_count(void)
{
	return ARRAY_SIZE(ucs116e0_als_para_diff_tp_color_table);
}