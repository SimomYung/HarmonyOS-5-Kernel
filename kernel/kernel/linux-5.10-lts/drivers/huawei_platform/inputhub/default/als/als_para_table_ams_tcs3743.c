
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: als para table tcs3410 source file
 * Author: chenjianchun 30063743
 * Create: 2024-8-19
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
	{ HED, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		323, 372, 19871, 7899, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0
		}
	},
	{ LMR, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		323, 372, 19871, 7899, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0
		}
	},
	{ LMU, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		323, 372, 19871, 7899, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-4887, 7, 2474, 7, -9912, 7, 136, 5, -3475, 8, // cct matrix 1 x c/r/g/b/w
		-7582, 7, 1736, 7, -7764, 7, 157, 5, -5859, 8, // cct matrix 1 y c/r/g/b/w
		-184, 5, -106, 5, 182, 5, 135, 5, 8767, 8, // cct matrix 1 z c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 x c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 y c/r/g/b/w
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cct matrix 2 z c/r/g/b/w
		0, 0
		}
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

