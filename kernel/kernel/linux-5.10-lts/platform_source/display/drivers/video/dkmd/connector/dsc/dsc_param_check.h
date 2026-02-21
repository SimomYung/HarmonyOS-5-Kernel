/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef DSC_PARAM_CHECK_H
#define DSC_PARAM_CHECK_H

void vesa_dsc_bpc_check(uint32_t version, uint32_t bpc);
void vesa_dsc_version_check(uint32_t version);
void vesa_dsc_native422_check(uint32_t version, uint16_t native_422);

#endif