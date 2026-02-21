/*
 * ffts_proc_block.h
 *
 * ffts proc block
 *
 * Copyright (c) 2024-2030 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef _FFTS_PROC_BLOCK_H_
#define _FFTS_PROC_BLOCK_H_

#include <linux/types.h>
#include "ffts_types.h"

typedef void (*CallbackType)(void *data, int32_t tgid);
 
struct ffts_proc_rel_regis_info {
	ffts_hw_t hw_type;
    CallbackType callback;
	void *data;
	int32_t tgid;
};

void ffts_proc_block_enable(int uid);
int ffts_proc_release_register(ffts_hw_t hw_type, void (*callback)(void *data, int32_t tgid), void *data, int32_t tgid);

#endif