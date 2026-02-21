/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
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
#ifndef GP_OPS_H
#define GP_OPS_H
#include "tc_ns_client.h"
#include "teek_ns_client.h"

int write_to_client(void __user *dest, size_t dest_size,
	const void *src, size_t size, uint8_t kernel_api);
int read_from_client(void *dest, size_t dest_size,
	const void __user *src, size_t size, uint8_t kernel_api);
int tc_client_call(const struct tc_call_params *call_params);
bool is_tmp_mem(uint32_t param_type);
bool is_ref_mem(uint32_t param_type);
bool is_val_param(uint32_t param_type);

#endif
