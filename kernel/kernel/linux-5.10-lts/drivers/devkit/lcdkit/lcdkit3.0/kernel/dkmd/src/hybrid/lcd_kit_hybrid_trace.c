// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_trace.c
 *
 * source file for trace
 *
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
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
#define CREATE_TRACE_POINTS
#include "lcd_kit_hybrid_trace.h"

void lcd_hybrid_trace_begin(const char *str)
{
	if (!str)
		return;
	LCD_HYBRID_TRACE_BEGIN(str);
}

void lcd_hybrid_trace_end(const char *str)
{
	if (!str)
		return;
	LCD_HYBRID_TRACE_END(str);
}
