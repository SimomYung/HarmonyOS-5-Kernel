

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

static tcs3410_als_para_table tcs3410_als_para_diff_tp_color_table[] = {
	{ GRL, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		18820, 0, -767, -14847, 100, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 27450, 0, 8394, 23481, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ TGR, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		-22, 15, 705, -215, 1,
		100,                        // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ TGR, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		-22, 15, 705, -215, 1,
		100,                        // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ TGR, V3, DEFAULT_TPLCD, GRAY,
		{486, // normalized gain
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		-22, 15, 705, -215, 1,
		100,                        // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ TGR, V3, DEFAULT_TPLCD, BROWN,
		{486, // normalized gain
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		-22, 15, 705, -215, 1,
		100,                        // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ MRO, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		52, -46, 598, -305, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		52, -46, 598, -305, 1,
		100,                        // normalized atime
		1, 27, 30257, 0, 0, -3230, 16828, 0, 0, 285, 1, 27,
		1, 4798, 2713, 1670, 1046, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ MRO, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		196, -194, 525, -474, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		196, -194, 525, -474, 1,
		100,                        // normalized atime
		1, 27, 27615, 1, 0, -2250, 16337, 0, 0, -1679, 3, 41,
		1, 4798, 2713, 1670, 1046, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ BKY, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		-125, 209, 676, -104, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                       // dgf / ratio
		-240, 222, 1335, -443, 1,
		100,                        // normalized atime
		1, 30, 19905, 0, 0, -345, 14871, 0, 0, 497, 1, 30,
		1, 7716, 3034, 2412, 1301, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ BKY, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		-125, 209, 676, -104, 1, // screen-off para:c/r/g/b/dgf * 1000
		1,                       // dgf / ratio
		-240, 222, 1335, -443, 1,
		100,                        // normalized atime
		1, 30, 19905, 0, 0, -345, 14871, 0, 0, 497, 1, 30,
		1, 7716, 3034, 2412, 1301, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ VDE, V3, VISI_TPLCD, OTHER,
		{512, // normalized gain
		1000, 0, 618, -1079, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 30038, 0, 11264, 22387, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ VDE, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		582, 0, 2074, -827, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 20261, 0, 6833, 14649, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ HPR, V3, DEFAULT_TPLCD, OTHER,
		{486, // normalized gain
		2082, -2172, 7761, -893, 1000, // screen-off para: c/r/g/b/dgf * 1000
		10,                           // dgf / ratio
		2082, -2172, 7761, -893, 100,
		100,                        // normalized atime
		0, 22, 32000, 0, 0, -1650, 32000, 0, 0, -2620, 0, 22,
		5, 10365, 8145, 2618, 1128, // ratio/(target/ration)c/r/g/b
		5000, 200,
		-13392, 6, 5781, 5, -6305, 6, 18133, 6, 0, 1, // high X_RGBCW & ratio
		-21715, 6, 7761, 5, -8927, 6, 20817, 6, 0, 1, // Y_RGBCW & ratio
		-7083, 5, 19083, 5, -24922, 6, 3676, 5, 0, 1, // Z_RGBCW & ratio
		32589, 6, 15763, 6, -7179, 7, 13038, 7, 0, 1, // low X_RGBCW & ratio
		3286, 5, 15894, 6, -7238, 7, 13146, 7, 0, 1, // Y_RGBCW & ratio
		20668, 6, 9997, 6, -4553, 7, 8269, 7, 0, 1, // Z_RGBCW & ratio
		42, 4  // algorithm & point
		}
	},
	{ MRDI, V3, DEFAULT_TPLCD, BLACK,
		{ 486, // normalized gain
		52, -46, 598, -305, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		52, -46, 598, -305, 1,
		100, // normalized atime
		1, 27, 30257, 0, 0, -3230, 16828, 0, 0, 285, 1, 27,
		1, 4798, 2713, 1670, 1046, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ MRDI, V3, DEFAULT_TPLCD, BLACK2,
		{ 486, // normalized gain
		196, -194, 525, -474, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		196, -194, 525, -474, 1,
		100, // normalized atime
		1, 27, 27615, 1, 0, -2250, 16337, 0, 0, -1679, 3, 41,
		1, 4798, 2713, 1670, 1046, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ LRT, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		-125, 209, 676, -104, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-240, 222, 1335, -443, 1,
		100, // normalized atime
		1, 30, 19905, 0, 0, -345, 14871, 0, 0, 497, 1, 30,
		1, 7716, 3034, 2412, 1301, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ LRT, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		-125, 209, 676, -104, 1, // screen-off para:c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-240, 222, 1335, -443, 1,
		100, // normalized atime
		1, 30, 19905, 0, 0, -345, 14871, 0, 0, 497, 1, 30,
		1, 7716, 3034, 2412, 1301, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ LRT, V3, BOE_TPLCD, GRAY,
		{486, // normalized gain
		-201, 219, 604, 80, 1, // screen-off para:c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-201, 219, 604, 80, 1,
		100, // normalized atime
		1, 30, 21915, 0, 0, -1734, 15303, 0, 0, 226, 1, 31,
		1, 11183, 4763, 4054, 2112, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ LRT, V3, CSOT_TPLCD, GRAY,
		{486, // normalized gain
		-201, 219, 604, 80, 1, // screen-off para:c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-201, 219, 604, 80, 1,
		100, // normalized atime
		1, 30, 23313, 0, 0, -2116, 14638, 0, 0, 424, 1, 30,
		1, 11183, 4763, 4054, 2112, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ SLG, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-22, 15, 705, -215, 1,
		100, // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ SLG, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		19, -29, 588, -187, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		19, -29, 588, -187, 1,
		100, // normalized atime
		1, 29, 26728, 0, 0, -2661, 16798, 0, 0, 26, 1, 29,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ GLB, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		403, 0, 1879, -495, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                            // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 24016, 0, 7305, 19849, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ QXS, V3, DEFAULT_TPLCD, OTHER,
		{486, // normalized gain
		535, 1458, -7538, 7719, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf/ratio
		585, -983, 3709, -4017, 1,
		100, // normalized atime
		1, 26, 24994, 0, 0, -1225, 17292, 0, 0, 376, 1, 26,
		1, 4053, 2212, 1062, 626, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ QXS, V4, DEFAULT_TPLCD, OTHER,
		{1024, // normalized gain
		100, -117, 830, -392, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf/ratio
		149, -264, 1579, -1401, 1,
		100, // normalized atime
		1, 24, 25037, 2, 23, -814, 18345, 31950, -2784, 277, 1, 28,
		1, 10430, 6276, 2601, 1674, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ TXZ, V3, DEFAULT_TPLCD, BLACK,
		{486, // normalized gain
		-59, 43, 715, -190, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-59, 43, 715, -190, 1,
		100, // normalized atime
		1, 31, 24130, 0, 0, -2014, 15200, 0, 0, 353, 1, 31,
		1, 16206, 8875, 4655, 3331, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ TXZ, V3, DEFAULT_TPLCD, BLACK2,
		{486, // normalized gain
		-59, 43, 715, -190, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-59, 43, 715, -190, 1,
		100, // normalized atime
		1, 31, 24130, 0, 0, -2014, 15200, 0, 0, 353, 1, 31,
		1, 16206, 8875, 4655, 3331, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
};

static tcs3410_als_para_table tcs3410_als1_para_diff_tp_color_table[] = {
	{ GRL, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		20446, 0, -2783, -16157, 100, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 25021, 0, 7633, 20939, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ ICL, V3, BOE_TPLCD, OTHER,
		{128,
		813, 0, -128, -609, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		5, 26873, 0, 8059, 22724,
		1800, 600
		}
	},
	{ ICL, V3, BOE_TPLCD_B12, OTHER,
		{128,
		596, 0, -282, -456, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		6, 30800, 0, 9393, 22469,
		1800, 600
		}
	},
	{ ICL, V3, DEFAULT_TPLCD, OTHER,
		{64,
		709, 0, -376, -468, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		3, 23373, 0, 8089, 17668,
		1800, 600
		}
	},
	{ LRT, V3, DEFAULT_TPLCD, OTHER,
		{486, // normalized gain
		46, -164, 1332, -900, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                       // dgf / ratio
		-91, 73, 973, -454, 1,
		100,                        // normalized atime
		1, 25, 19905, 0, 0, -345, 14871, 0, 0, 497, 1, 30,
		1, 11382, 5155, 2967, 2369, // ratio/(target/ration)c/r/g/b
		5000, 200 }
	},
	{ GLB, V3, DEFAULT_TPLCD, OTHER,
		{512, // normalized gain
		84, 0, 1758, -155, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		100,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		12, 23516, 0, 7382, 19885, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ DEL, V3, BOE_TPLCD, OTHER,
		{128,
		1192, 0, -564, -912, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		6, 30800, 0, 9393, 22469,
		4000, 250 // TO
		}
	},
	{ DEL, V3, DEFAULT_TPLCD, OTHER,
		{64,
		5, 0, 2300, -216, 1000,
		1,
		5690, -4716, 2529, -5526, 100,
		0,
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		1, 30355, 0, 10246, 24961,
		4000, 250 // TO
		}
	},
};

tcs3410_als_para_table *als_get_tcs3410_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tcs3410_als_para_diff_tp_color_table))
		return NULL;
	return &(tcs3410_als_para_diff_tp_color_table[id]);
}

tcs3410_als_para_table *als_get_tcs3410_first_table(void)
{
	return &(tcs3410_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_tcs3410_table_count(void)
{
	return ARRAY_SIZE(tcs3410_als_para_diff_tp_color_table);
}

tcs3410_als_para_table *als1_get_tcs3410_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tcs3410_als1_para_diff_tp_color_table))
		return NULL;
	return &(tcs3410_als1_para_diff_tp_color_table[id]);
}

tcs3410_als_para_table *als1_get_tcs3410_first_table(void)
{
	return &(tcs3410_als1_para_diff_tp_color_table[0]);
}

uint32_t als1_get_tcs3410_table_count(void)
{
	return ARRAY_SIZE(tcs3410_als1_para_diff_tp_color_table);
}
