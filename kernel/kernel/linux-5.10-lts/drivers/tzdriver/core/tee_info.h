/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: check compatibility between tzdriver and teeos.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef TEE_INFO_H
#define TEE_INFO_H

#include <linux/types.h>
#include <linux/fs.h>
#include "teek_ns_client.h"

int32_t tc_ns_get_tee_info(struct file *file, void __user *argp);
#endif
