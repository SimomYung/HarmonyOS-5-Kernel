
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
	{ HED, V3, BOE_TPLCD, OTHER,
		{4, // normalized gain
		3521, -3332, 22711, -7226, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11243, 6547, 3952, 1791, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-6164, 5, 8250, 5, -19131, 5, 10528, 5, 0, 0,
		-7518, 5, 10280, 5, -11747, 5, 9670, 5, 0, 0,
		-17330, 5, -8378, 5, 4113, 4, 12490, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ HED, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		-656, 487, 3631, -783, 10, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 10591, 5822, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-8291, 5, 5423, 5, -21439, 5, 14095, 5, 0, 0,
		-8747, 5, 8991, 5, -15155, 5, 11975, 5, 0, 0,
        -14753, 5, -4203, 5, 29195, 5, 10610, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMR, V3, BOE_TPLCD, OTHER,
		{4, // normalized gain
		1904, -1535, 20657, -6342, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-8722, 5, 8385, 5, -18521, 5, 11898, 5, 0, 0,
		-9835, 5, 10746, 5, -11372, 5, 10451, 5, 0, 0,
		-19112, 5, -4533, 5, 3930, 4, 10368, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMR, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		1807, -831, 23799, -9571, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-9491, 5, 3776, 5, -21228, 5, 14743, 5, 0, 0,
		-10051, 5, 6315, 5, -15251, 5, 13222, 5, 0, 0,
		-14919, 5, -5847, 5, 29331, 5, 10883, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMU, V3, BOE_TPLCD, OTHER,
		{4, // normalized gain
		1904, -1535, 20657, -6342, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-8772, 5, 7274, 5, -18355, 5, 12202, 5, 0, 0,
		-9888, 5, 9390, 5, -11159, 5, 10814, 5, 0, 0,
		-19221, 5, -5890, 5, 3978, 4, 10725, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMU, V4_D, BOE_TPLCD, OTHER,
		{4, // normalized gain
		1904, -1535, 20657, -6342, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-8772, 5, 7274, 5, -18355, 5, 12202, 5, 0, 0,
		-9888, 5, 9390, 5, -11159, 5, 10814, 5, 0, 0,
		-19221, 5, -5890, 5, 3978, 4, 10725, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMU, V3, DEFAULT_TPLCD, OTHER, // < V4H4
		{4, // normalized gain
		-449, 885, 23810, -6487, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-7121, 5, 5573, 5, -14809, 5, 11089, 5, 0, 0,  // Color temperature matrix before light source change
		-7451, 5, 8316, 5, -10558, 5, 9654, 5, 0, 0,
		-12067, 5, -4765, 5, 21557, 5, 9238, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ LMU, V4_D, DEFAULT_TPLCD, OTHER, // >= V4H4
		{4, // normalized gain
		-449, 885, 23810, -6487, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-9020, 5, 6801, 5, -20684, 5, 12603, 5, 0, 0,  // Color temperature matrix after the light source is changed
		-9146, 5, 10659, 5, -14896, 5, 10406, 5, 0, 0,
		-14377, 5, -23584, 6, 28385, 5, 9348, 5, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ SGP, V3, BOE_TPLCD, OTHER,
		{4, // normalized gain
		10826, -8152, 11074, -7809, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		5, 28646, 17717, 9711, 4369, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-700, 4, 1076, 4, -2551, 4, 1166, 4, 0, 0,
		-741, 4, 1394, 4, -1807, 4, 960, 4, 0, 0,
		-1251, 4, 752, 6, 3107, 4, 742, 4, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ SGP, V3, BOE_TPLCD_B12, OTHER,
		{4, // normalized gain
		-2703, 987, 29661, -7100, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		6, 27578, 15795, 9378, 5121, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-700, 4, 1076, 4, -2551, 4, 1166, 4, 0, 0,
		-741, 4, 1394, 4, -1807, 4, 960, 4, 0, 0,
		-1251, 4, 752, 6, 3107, 4, 742, 4, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ SGP, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		-2865, 3007, 26020, -4288, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-700, 4, 1076, 4, -2551, 4, 1166, 4, 0, 0, 
		-741, 4, 1394, 4, -1807, 4, 960, 4, 0, 0, 
		-1251, 4, 752, 6, 3107, 4, 742, 4, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ SGT, V3, BOE_TPLCD, OTHER,
		{4, // normalized gain
		1904, -1535, 20657, -6342, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 11608, 6299, 4067, 2118, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-700, 4, 1076, 4, -2551, 4, 1166, 4, 0, 0,
		-741, 4, 1394, 4, -1807, 4, 960, 4, 0, 0,
		-1251, 4, 752, 6, 3107, 4, 742, 4, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ SGT, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		138, 161, 24696, -5974, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		5, 27257, 14732, 10357, 4762, // ratio/(target/ration)c/r/g/b
		4000, 250,
		-700, 4, 1076, 4, -2551, 4, 1166, 4, 0, 0, 
		-741, 4, 1394, 4, -1807, 4, 960, 4, 0, 0, 
		-1251, 4, 752, 6, 3107, 4, 742, 4, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
		}
	},
	{ MLR, V3, DEFAULT_TPLCD, OTHER,
		{4, // normalized gain
		-2865, 3007, 26020, -4288, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 7434, 3866, 2690, 1604, // ratio/(target/ration)c/r/g/b
		10000, 0,
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

