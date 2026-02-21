

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
		3, 6, 186, -320, 1, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		3, 6, 186, -320, 1,
		100,                        // normalized atime
		0, 22, 32000, 0, 0, -1650, 32000, 0, 0, -2620, 0, 22,
		5, 10365, 8145, 2618, 1128, // ratio/(target/ration)c/r/g/b
		30000, 0 }
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
		-22, 15, 705, -215, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		-22, 15, 705, -215, 1,
		100, // normalized atime
		1, 30, 24472, 0, 0, -2165, 16183, 0, 0, 169, 1, 30,
		1, 16918, 8956, 4675, 3195, // ratio/(target/ration)c/r/g/b
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
		{128, // normalized gain
		813, 0, -128, -609, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		0,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		5, 26873, 0, 8059, 22724, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ ICL, V3, BOE_TPLCD_B12, OTHER,
		{128, // normalized gain
		596, 0, -282, -456, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		0,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		6, 30800, 0, 9393, 22469, // ratio/(target/ration)c/r/g/b
		4000, 250
		}
	},
	{ ICL, V3, DEFAULT_TPLCD, OTHER,
		{64, // normalized gain
		709, 0, -376, -468, 1000, // screen-off para: c/r/g/b/dgf * 1000
		1,                           // dgf / ratio
		5690, -4716, 2529, -5526, 100,
		0,                        // normalized atime
		1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20,
		3, 23373, 0, 8089, 17668, // ratio/(target/ration)c/r/g/b
		4000, 250
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
