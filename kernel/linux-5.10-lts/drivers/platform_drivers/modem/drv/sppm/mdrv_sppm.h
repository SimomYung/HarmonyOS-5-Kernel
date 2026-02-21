/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef __SPPM_INTERFACE_H__
#define __SPPM_INTERFACE_H__

#define SPPM_SRC_NAME_LEN 32
#define SPPM_SRC_PRIV_LEN 32

#include <osl_list.h>
#include <bsp_print.h>
#include "sppm_soc_interface.h"

#define sppm_err(fmt, ...) (bsp_err(fmt, ##__VA_ARGS__))
#define sppm_info(fmt, ...) (bsp_info(fmt, ##__VA_ARGS__))

#define SPPM_OK 0
#define SPPM_ERROR (-1)

typedef struct {
    unsigned int *events_list; /* 单个通道事件id */
    unsigned int nums;         // 当前通道配置的寄存器个数
    char *private_data;        // 当前通道配置私有数据
} sppm_event_entry;

typedef struct {
    unsigned long uregs[SPPM_MAX_REGS_EACH_CHAN]; /* 14：单个通道寄存器最大个数值 */
    unsigned char msb_4bit;
} sppm_rchan_regs;

typedef enum { SPPM_CLIENT_OFF, SPPM_CLIENT_ON } client_pm_state_e;

typedef int (*sppm_client_pd_notify)(unsigned int chan_id, client_pm_state_e pw_state);

/*
 * sppm register interface.
 */
typedef struct {
    char *name;
    int (*init)(sppm_client_pd_notify pdfunc);
    int (*deinit)(void);
    int (*start)(void);
    int (*stop)(void);
    int (*config)(sppm_event_entry *pentry, sppm_rchan_regs *pregs);
} sppm_reg_interface;

// 注册成功返回有效通道号，否则返回负值
int bsp_sppm_client_reg(sppm_reg_interface *, unsigned int *chan);
#endif /* mdrv_sppm.h */
