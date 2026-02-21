/*
 * mad extension driver.
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#ifndef __MAD_EXTENSION_H__
#define __MAD_EXTENSION_H__

void mad_ex_set_pinctrl_state(unsigned int mode);
int mad_ex_request_pinctrl_state(unsigned int mode);
int mad_ex_release_pinctrl_state(unsigned int mode);

#endif

