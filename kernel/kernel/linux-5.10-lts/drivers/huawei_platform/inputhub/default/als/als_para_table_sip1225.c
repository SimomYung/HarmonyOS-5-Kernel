/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: als para table sip1225 source file
 * Author: lisheng
 * Create: 2024-01-19
 */

#include "als_para_table_sip1225.h"

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

static als_para_normal_table sip1225_als_para_diff_tp_color_table[] = {
	/* add normalization gain issued from the kernel */
	{ PSD, V3, VISI_TPLCD, SIP1225_PARA_SIZE,
		{ 1024, 400, 550, 1000, 2000, 3978, 4326, 4363, 4114, 11000, 5900, 4000, 250, 0, 0, 0 }
	},
	{ PSD, V3, BOE_TPLCD_B12, SIP1225_PARA_SIZE,
		{ 1024, 420, 550, 1000, 2000, 6025, 6503, 6571, 6247, 7988, 4159, 4000, 250, 0, 0, 0 }
	},
	{ PSD, V3, CSOT_TPLCD, SIP1225_PARA_SIZE,
		{ 1024, 380, 590, 950, 2000, 1911, 2096, 2108, 1997, 18390, 9712, 4000, 250, 0, 0, 0 }
	},
	{ PSD, V3, TS_PANEL_UNKNOWN, SIP1225_PARA_SIZE,
		{ 1024, 420, 550, 1000, 2000, 6330, 7008, 7054, 6712, 7988, 4159, 4000, 250, 0, 0, 0 }
	},
	{ TLR, V3, VISI_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 380, 580, 946, 2000, 10134, 11098, 11257, 10468, 3800, 1900, 4000, 250, 0, 0, 0 }
	},
	{ TLR, V3, BOE_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 440, 660, 1050, 2000, 15428, 15849, 15646, 15788, 2920, 1360, 4000, 250, 0, 0, 0 }
	},
	{ TLR, V3, CSOT_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 434, 666, 1081, 2000, 15026, 16097, 16173, 15495, 2660, 6500, 4000, 250, 0, 0, 0 }
	},
	{ TLR, V3, TS_PANEL_UNKNOWN, SIP1225_PARA_SIZE,
		{ 128, 450, 690, 1100, 2000, 12454, 13646, 13678, 13225, 2920, 1360, 4000, 250, 0, 0, 0 }
	},
	{ GAI, V3, VISI_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 380, 580, 946, 2000, 10134, 11098, 11257, 10468, 3800, 1900, 4000, 250, 0, 0, 0 }
	},
	{ GAI, V3, BOE_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 440, 660, 1050, 2000, 15428, 15849, 15646, 15788, 2920, 1360, 4000, 250, 0, 0, 0 }
	},
	{ GAI, V3, CSOT_TPLCD, SIP1225_PARA_SIZE,
		{ 128, 434, 666, 1081, 2000, 15026, 16097, 16173, 15495, 2660, 6500, 4000, 250, 0, 0, 0 }
	},
	{ GAI, V3, TS_PANEL_UNKNOWN, SIP1225_PARA_SIZE,
		{ 128, 450, 690, 1100, 2000, 12454, 13646, 13678, 13225, 2920, 1360, 4000, 250, 0, 0, 0 }
	},
};

als_para_normal_table *als_get_sip1225_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(sip1225_als_para_diff_tp_color_table))
		return NULL;
	return &(sip1225_als_para_diff_tp_color_table[id]);
}

als_para_normal_table *als_get_sip1225_first_table(void)
{
	return &(sip1225_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_sip1225_table_count(void)
{
	return ARRAY_SIZE(sip1225_als_para_diff_tp_color_table);
}
