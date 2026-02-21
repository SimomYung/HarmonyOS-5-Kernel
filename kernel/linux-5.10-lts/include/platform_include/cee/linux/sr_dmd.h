/*
 * sr_dmd.h
 *
 * trans tp from cmdline to dmd
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
 
#ifndef __H_PM_MNTN_SR_DMD_H__
#define __H_PM_MNTN_SR_DMD_H__

#include <linux/types.h>

#ifdef CONFIG_SR_MNTN_DMD
int get_sr_info_for_dmd(const void *bbox_base, size_t size, char *out_buf, size_t buf_size);
#else
static inline int get_sr_info_for_dmd(const void *bbox_base, size_t size, char *out_buf, size_t buf_size)
{
	return 0;
}
#endif
 
#endif /* __H_PM_MNTN_SR_DMD_H__ */