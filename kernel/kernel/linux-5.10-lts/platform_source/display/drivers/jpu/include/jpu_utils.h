/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpeg jpu utils
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

#ifndef JPU_UTILS_H
#define JPU_UTILS_H

#include "jpu.h"

void jpu_dec_normal_reset(const struct jpu_data_type *jpu_device);
void jpu_dec_error_reset(struct jpu_data_type *jpu_device);
#endif /* JPU_UTILS_H */
