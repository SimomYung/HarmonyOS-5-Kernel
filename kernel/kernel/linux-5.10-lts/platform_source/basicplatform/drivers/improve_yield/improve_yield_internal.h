/*
 * improve_yield_internal.h
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#ifndef __IMPROVE_YIELD_INTERNAL_H__
#define __IMPROVE_YIELD_INTERNAL_H__
#include <linux/mutex.h>

#define INFO_MAX_LEN 1024

int get_enable_info_str(char *buf, unsigned int *len);
int get_enable_mask(char *buf, size_t count, unsigned int *module_mask, unsigned int *value_mask);
void free_improve_yield_info_list(void);
int get_improve_yield_info(char *info, unsigned int *len);

extern struct mutex g_improve_yield_nv_access_mutex;
extern struct mutex g_improve_yield_info_mutex;
#endif /* __IMPROVE_YIELD_INTERNAL_H__ */
