
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: als para table tcs3410 source file
 * Author: tianyunhang 30013707
 * Create: 2025-3-10
 */

#include "als_para_table.h"

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

static ams_tcs3743_als_para_table ams_tcs3743_als_para_diff_tp_color_table[] = {
	{ MLR, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		3430, -2702, 7408, -5816, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		3430, -2702, 7408, -5816, 100,
		100, // normalized atime
		1, 32, 31022, 0, 0, -5606, 14823, 0, 0, 123, 1, 32,
		12, 7434, 3866, 2690, 1604, // ratio/(target/ration)c/r/g/b
		10000, 0}
	},
};

ams_tcs3743_als_para_table *als_get_ams_tcs3743_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(ams_tcs3743_als_para_diff_tp_color_table))
		return NULL;
	return &(ams_tcs3743_als_para_diff_tp_color_table[id]);
}

ams_tcs3743_als_para_table *als_get_ams_tcs3743_first_table(void)
{
	return &(ams_tcs3743_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_ams_tcs3743_table_count(void)
{
	return ARRAY_SIZE(ams_tcs3743_als_para_diff_tp_color_table);
}

