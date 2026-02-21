/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: als para table stk37670  file
 * Author: tianyunhang2@huawei.com
 * Create: 2024-05-13
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

static stk37670_als_para_table pad_stk37670_als_para[] = {
	{ WEB, V3, BOE_TPLCD, BLACK,
		{128, // normalized gain
		1, 3, 32, 126, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		1, 3, 32, 126, 1,
		100, // normalized atime
		1, 13, 27822, 0, 0, 634, 20355, 0, 0, 1764, 1, 13,
		5, 20040, 4816, 7897, 1067, // ratio/(target/ration)c/r/g/b
		5000, 200
		}
	},
	{ WEB, V3, BOE_TPLCD, BLACK2,
		{128, // normalized gain
		1, 3, 32, 126, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		1, 3, 32, 126, 1,
		100, // normalized atime
		1, 13, 27822, 0, 0, 634, 20355, 0, 0, 1764, 1, 13,
		5, 20040, 4816, 7897, 1067, // ratio/(target/ration)c/r/g/b
		5000, 200
		}
	},
	{ WEB, V3, VISI_TPLCD, BLACK,
		{128, // normalized gain
		1, 3, 32, 126, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		1, 3, 32, 126, 1,
		100, // normalized atime
		1, 13, 27822, 0, 0, 634, 20355, 0, 0, 1764, 1, 13,
		5, 20040, 4816, 7897, 1067, // ratio/(target/ration)c/r/g/b
		5000, 200
		}
	},
	{ WEB, V3, VISI_TPLCD, BLACK2,
		{128, // normalized gain
		1, 3, 32, 126, 1, // screen-off para: c/r/g/b/dgf * 1000
		1, // dgf / ratio
		1, 3, 32, 126, 1,
		100, // normalized atime
		1, 13, 27822, 0, 0, 634, 20355, 0, 0, 1764, 1, 13,
		5, 20040, 4816, 7897, 1067, // ratio/(target/ration)c/r/g/b
		5000, 200
		}
	},
};


stk37670_als_para_table *als_get_pad_stk37670_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(pad_stk37670_als_para))
		return NULL;
	return &(pad_stk37670_als_para[id]);
}

stk37670_als_para_table *als_get_pad_stk37670_first_table(void)
{
	return &(pad_stk37670_als_para[0]);
}

uint32_t als_get_pad_stk37670_table_count(void)
{
	return ARRAY_SIZE(pad_stk37670_als_para);
}
