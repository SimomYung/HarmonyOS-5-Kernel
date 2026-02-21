

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

static sip2326_als_para_table sip2326_als_para_diff_tp_color_table[] = {
	{ GRL, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		0, 7211, 20382, -8385, 1000,       // screen-off para1: c/r/g/b/dgf
		1,                                 // dgf / ratio
		-2031, 3476, 20137, -4718, 2000,   // screen-off para2(ir): c/r/g/b/dgf (unused)
		200,                               // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		24, 13659, 2837, 2214, 1256, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ TGR, V3, DEFAULT_TPLCD, BLACK,
		{2048, // normalized gain
		6, -22, 75, -1, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		6, -22, 75, -1, 1,
		100,                        // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ TGR, V3, DEFAULT_TPLCD, BLACK2,
		{2048, // normalized gain
		6, -22, 75, -1, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		6, -22, 75, -1, 1,
		100,                        // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ TGR, V3, DEFAULT_TPLCD, GRAY,
		{2048, // normalized gain
		6, -22, 75, -1, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		6, -22, 75, -1, 1,
		100,                        // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ TGR, V3, DEFAULT_TPLCD, BROWN,
		{2048, // normalized gain
		6, -22, 75, -1, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		6, -22, 75, -1, 1,
		100,                        // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ MRO, V3, DEFAULT_TPLCD, BLACK,
		{2048, // normalized gain
		2, 12, -15, 22, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		-7, 23, 14, 4, 1,
		100,                        // normalized atime
		1, 56, 13128, 0, 0, -2604, 6546, 0, 0, 1380, 1, 56,
		6, 10562, 5986, 5182, 4585, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ MRO, V3, DEFAULT_TPLCD, BLACK2,
		{2048, // normalized gain
		5, -47, 69, -3, 1000, // screen-off para: c/r/g/b/dgf
		1000,                              // dgf / ratio
		-6, 10, 31, -2, 1,
		100,                        // normalized atime
		1, 54, 11773, 0, 0, -1748, 6409, 0, 0, 1400, 1, 59,
		6, 10562, 5986, 5182, 4585, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ BKY, V3, BOE_TPLCD, BLACK,
		{2048, // normalized gain
		2, -16, 58, -4, 1000, // screen-off para: c/r/g/b/dgf
		1000,                  // dgf / ratio
		-14, 15, 64, -14, 1,
		100,                        // normalized atime
		1, 53, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ BKY, V3, BOE_TPLCD, BLACK2,
		{2048, // normalized gain
		2, -16, 58, -4, 1000, // screen-off para: c/r/g/b/dgf
		1000,                  // dgf / ratio
		-14, 15, 64, -14, 1,
		100,                        // normalized atime
		1, 53, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ BKY, V3, CSOT_TPLCD, BLACK,
		{2048, // normalized gain
		2, -16, 58, -4, 1000, // screen-off para: c/r/g/b/dgf
		1000,                  // dgf / ratio
		-14, 15, 64, -14, 1,
		100,                        // normalized atime
		1, 53, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ BKY, V3, CSOT_TPLCD, BLACK2,
		{2048, // normalized gain
		11, 10, -26, 35, 1000, // screen-off para: c/r/g/b/dgf
		1000,                  // dgf / ratio
		1, 1, 29, 15, 1,
		100,                        // normalized atime
		1, 53, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ VDE, V3, BOE_TPLCD, OTHER,
		{512, // normalized gain
		1052, -18834, 27945, -10528, 3000, // screen-off para: c/r/g/b/dgf
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                          // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		1, 24020, 5044, 5386, 2836, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ VDE, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		995, -10770, 25689,-12334, 1000, // screen-off para: c/r/g/b/dgf
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                          // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		10, 3956, 842, 910, 484, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ HPR, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		805, 560, 738, -21975, 1000, // screen-off para: c/r/g/b/dgf
		10,                              // dgf / ratio
		-133, 116, 583, 4125, 100,
		100,                        // normalized atime
		0, 38, 13813, 0, 0, -2123, 7508, 0, 0, 952, 0, 38,
		30, 28919, 4966, 3566, 1066, // ratio/(target/ratio)c/r/g/b
		5000, 200,
		449, 5, 3248, 6, 28749, 6, -14557, 7, 0, 1, // high X_RGBCW & ratio
		19081, 7, 6998, 6, 381, 4, -18524, 7, 0, 1, // Y_RGBCW & ratio
		-12129, 6, 28068, 6, 9157, 5, -4425, 6, 0, 1, // Z_RGBCW & ratio
		24253, 7, 20597, 7, -23495, 5, 1305, 5, 0, 1, // low X_RGBCW & ratio
		22354, 7, 229, 5, -229, 3, 12824, 6, 0, 1, // Y_RGBCW & ratio
		17593, 7, 5612, 7, -12824, 5, 7454, 6, 0, 1, // Z_RGBCW & ratio
		2593, 5 // algorithm & point
		}
	},
	{ MRDI, V3, DEFAULT_TPLCD, BLACK,
		{ 2048, // normalized gain
		3, 9, -7, 12, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		4, -4, 10, 9, 1,
		100, // normalized atime
		1, 47, 13128, 0, 0, -2604, 6546, 0, 0, 1380, 1, 56,
		6, 10562, 5986, 5182, 4585, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ MRDI, V3, DEFAULT_TPLCD, BLACK2,
		{ 2048, // normalized gain
		2, 8, -4, 17, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		2, -6, 25, 2, 1,
		100, // normalized atime
		1, 48, 11773, 0, 0, -1748, 6409, 0, 0, 1400, 1, 59,
		6, 10562, 5986, 5182, 4585, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, BOE_TPLCD, BLACK,
		{2048, // normalized gain
		6, -2, 13, 22, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		1, 9, 16, 20, 1,
		100, // normalized atime
		1, 51, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, BOE_TPLCD, BLACK2,
		{2048, // normalized gain
		7, -26, 45, 15, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		2, -13, 48, 10, 1,
		100, // normalized atime
		1, 51, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, CSOT_TPLCD, BLACK,
		{2048, // normalized gain
		5, 18, -18, 32, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		-1, 28, -13, 38, 1,
		100, // normalized atime
		1, 52, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, CSOT_TPLCD, BLACK2,
		{2048, // normalized gain
		11, 10, -26, 35, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		1, 1, 29, 15, 1,
		100, // normalized atime
		1, 52, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, BOE_TPLCD, GRAY,
		{2048, // normalized gain
		1, 8, 23, 11, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		1, 8, 23, 11, 1,
		100, // normalized atime
		1, 0, 12152, 0, 0, -416, 7800, 0, 0, 1337, 1, 46,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ LRT, V3, CSOT_TPLCD, GRAY,
		{2048, // normalized gain
		1, 8, 23, 11, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		1, 8, 23, 11, 1,
		100, // normalized atime
		1, 0, 12152, 0, 0, -416, 7800, 0, 0, 1337, 1, 46,
		10, 6106, 3969, 3202, 2605, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ SLG, V3, DEFAULT_TPLCD, BLACK,
		{2048, // normalized gain
		6, -22, 75, -1, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		6, -22, 75, -1, 1,
		100, // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ SLG, V3, DEFAULT_TPLCD, BLACK2,
		{2048, // normalized gain
		-2, -6, 83, -11, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		-2, -6, 83, -11, 1,
		100, // normalized atime
		1, 52, 13813, 0, 0, -2123, 7508, 0, 0, 952, 1, 52,
		6, 15155, 9448, 6125, 5496, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ GLB, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		1107, -2999, 19910, -6124, 1000,       // screen-off para1: c/r/g/b/dgf
		1,                                 // dgf / ratio
		-2031, 3476, 20137, -4718, 2000,   // screen-off para2(ir): c/r/g/b/dgf (unused)
		200,                               // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		24, 6799, 1171, 1105, 472, // ratio/(target/ration)c/r/g/b
		1800, 250
		}
	},
	{ QXS, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		14, 86, -56, 75, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf/ratio
		-9, -104, 322, -117, 1,
		100, // normalized atime
		1, 47, 13709, 0, 0, -1443, 7512, 0, 0, 1329, 1, 47,
		6, 12479, 2593, 2281, 1578, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ TXZ, V3, DEFAULT_TPLCD, BLACK,
		{2048, // normalized gain
		0, -3, 31, 7, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		0, -3, 33, 1, 1,
		100, // normalized atime
		1, 51, 15577, 0, 0, -2502, 8348, 0, 0, 835, 1, 51,
		7, 18232, 9321, 8562, 4992, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ TXZ, V3, DEFAULT_TPLCD, BLACK2,
		{2048, // normalized gain
		0, -3, 31, 7, 1000, // screen-off para: c/r/g/b/dgf
		1000, // dgf / ratio
		0, -3, 33, 1, 1,
		100, // normalized atime
		1, 51, 15577, 0, 0, -2502, 8348, 0, 0, 835, 1, 51,
		7, 18232, 9321, 8562, 4992, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
};

static sip2326_als_para_table sip2326_als1_para_diff_tp_color_table[] = {
	{ GRL, V3, DEFAULT_TPLCD, OTHER,
		{2048,
		0, 5067, 22887, -8326, 1000,
		1,
		-2673, 8652, 19568, -9322, 2000,
		200,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		24, 11910, 2535, 1983, 1064,
		4000, 250
		}
	},
	{ ICL, V3, BOE_TPLCD_B12, OTHER,
		{512,
		0, 2251, 5821, -2083, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		3, 26126, 5561, 5856, 3094,
		1800, 600
		}
	},
	{ ICL, V3, BOE_TPLCD, OTHER,
		{512,
		135, -3652, 12797, -4222, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		3, 22716, 4655, 4616, 1627,
		1800, 600
		}
	},
	{ ICL, V3, DEFAULT_TPLCD, OTHER,
		{512,
		0, 4910, 4258, -2935, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		2, 29110, 7097, 7434, 3478,
		1800, 600
		}
	},
	{ LRT, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		-6, 32, 29, 1, 1000, // screen-off para: c/r/g/b/dgf
		1000,                  // dgf / ratio
		-6, 32, 29, 1, 1,
		100,                        // normalized atime
		1, 53, 8162, 0, 0, 1624, 6351, 0, 0, 1549, 1, 53,
		10, 8398, 3311, 3371, 4181, // ratio/(target/ratio)c/r/g/b
		5000, 200
		}
	},
	{ GLB, V3, DEFAULT_TPLCD, OTHER,
		{2048, // normalized gain
		1308, -5000, 20000, -21000, 1000,         // screen-off para1: c/r/g/b/dgf
		1,                                  // dgf / ratio
		-2673, 8652, 19568, -9322, 2000,    // screen-off para2(ir): c/r/g/b/dgf (unused)
		200,                                // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		24, 6520, 1053, 1162, 450, // ratio/(target/ration)c/r/g/b
		1800, 250
		}
	},
	{ DEL, V3, BOE_TPLCD_B12, OTHER,
		{512,
		0, 2251, 5821, -2083, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		3, 26126, 5561, 5856, 3094,
		4000, 250
		}
	},
	{ DEL, V3, BOE_TPLCD, OTHER,
		{512,
		861, -12067, 21315, -7626, 2000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		1, 25648, 7054, 6219, 2571,
		4000, 250
		}
	},
	{ DEL, V3, DEFAULT_TPLCD, OTHER,
		{512,
		0, 4910, 4258, -2935, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		2, 29110, 7097, 7434, 3478,
		4000, 250 // TO
		}
	},
};

// als
sip2326_als_para_table *als_get_sip2326_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(sip2326_als_para_diff_tp_color_table))
		return NULL;
	return &(sip2326_als_para_diff_tp_color_table[id]);
}

sip2326_als_para_table *als_get_sip2326_first_table(void)
{
	return &(sip2326_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_sip2326_table_count(void)
{
	return ARRAY_SIZE(sip2326_als_para_diff_tp_color_table);
}

// als1
sip2326_als_para_table *als1_get_sip2326_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(sip2326_als1_para_diff_tp_color_table))
		return NULL;
	return &(sip2326_als1_para_diff_tp_color_table[id]);
}

sip2326_als_para_table *als1_get_sip2326_first_table(void)
{
	return &(sip2326_als1_para_diff_tp_color_table[0]);
}

uint32_t als1_get_sip2326_table_count(void)
{
	return ARRAY_SIZE(sip2326_als1_para_diff_tp_color_table);
}
