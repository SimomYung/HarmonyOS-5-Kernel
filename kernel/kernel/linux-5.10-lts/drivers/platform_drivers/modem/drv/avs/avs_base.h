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

#ifndef AVS_BASE_H
#define AVS_BASE_H

#define AVS_PROFILE_MAX 10
#define POWER_DOMAIN_NUM 2

#define avs_err(fmt, ...) bsp_err(fmt, ##__VA_ARGS__)

typedef enum {
    AVS_POWER_DOMAIN_SW,
    AVS_PROFILE_SW,
    AVS_VOLT_LIMIT,
    AVS_OTHER_COMP,
    AVS_TEMP_COMP,
    AVS_AGING_COMP,
    AVS_INITED,
    AVS_BASE_VOLT,
    AVS_PERCHIP_UPDATE_REQ,
    AVS_PERCHIP_UPDATE,
    AVS_MSG_TYPE_MAX,
} avs_msg_type_e;

typedef struct {
    int avs_msg_type;
    unsigned int data_len;
    char data[0];
} avs_msg_s;

typedef int (*avs_msg_cb)(char *data, unsigned int len);

typedef struct {
    avs_msg_cb msg_cb[AVS_MSG_TYPE_MAX];
} avs_msg_cb_s;

int avs_msg_init(void);
int avs_val_notify(int avs_msg_type, char *data, unsigned int len);
void avs_msg_cb_register(avs_msg_type_e msg_type, avs_msg_cb ops);

#endif