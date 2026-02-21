/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef AVS_PERCHIP_H
#define AVS_PERCHIP_H
#include <bsp_avs.h>
#include "avs_base.h"

typedef struct {
    unsigned char perchip_valid_flag[AVS_PROFILE_MAX]; /* 分档位perchip 电压有效标记 0:无效，1:有效 */
    unsigned int  vmin[AVS_PROFILE_MAX]; /* AVS各档perchip_vmin值，单位100uv */
    unsigned int  temp[AVS_PROFILE_MAX]; /* AVS各档perchip_vmin场景的温度值，单位摄氏度 */
} PERCHIP_MEASURE_S;

typedef struct {
    PERCHIP_MEASURE_S perchip_measure_info[POWER_DOMAIN_NUM];
} AVS_PERCHIP_MEASURE_S;

typedef struct {
    u8 perchip_valid_flag[AVS_PROFILE_MAX];
    signed char perchip_measure_temp[AVS_PROFILE_MAX];
    u16 perchip_measure_vmin[AVS_PROFILE_MAX];
} perchip_measure_record_s;

typedef struct {
    u8 perchip_vmin_test_flag[POWER_DOMAIN_NUM][AVS_PROFILE_MAX];
    perchip_measure_record_s perchip_measure[POWER_DOMAIN_NUM];
} perchip_nve_info_s;

int perchip_avs_init(void);

#endif