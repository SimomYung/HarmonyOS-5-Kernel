/*
 * ffts_types.h
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
#ifndef _FFTS_TYPES_H_
#define _FFTS_TYPES_H_

typedef enum ffts_hw_type {
	FFTS_HW_GPU,
	FFTS_HW_NPU,
    FFTS_HW_AAE,
	FFTS_HW_MAX
} ffts_hw_t;

#endif