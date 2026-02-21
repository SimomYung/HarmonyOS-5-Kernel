/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Extract and Save Sensorhub Dump Log
 * Author: Shengzexuan Shengzexuan@huawei.com
 * Create: 2024-05-13
 */

#include <linux/stat.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include "myprintf.h"

#define LOG_LOWEST_LEVEL DEBUG_LEVEL
static char log_prompts[LOG_LOWEST_LEVEL + 1][32] = {
	"EMG",
	"ERR",
	"WARN",
	"INFO",
	"DEBUG",
};

typedef struct
{
	uint32_t base;
	uint32_t size;
	char log_strings[];
} sh_dict_info;

typedef struct {
	uint32_t offset;
	uint32_t log_size;
	uint32_t addr_start;
} origin_dict_info;

int load_sh_dict(void);
int extract_sh_dump_log(uint8_t *sensorhub_dump_buff, union dump_info dump_info, uint32_t dump_index);