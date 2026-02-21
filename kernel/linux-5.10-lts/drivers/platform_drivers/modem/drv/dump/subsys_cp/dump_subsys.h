/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
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

#ifndef _DUMP_SUBSYS_H_
#define _DUMP_SUBSYS_H_

#include <osl_types.h>
#include <dump_works.h>
#include <dump_infra.h>
#include "dump_exception.h"

/* 注意这个枚举在dts中也用到了(file_attr,见log_strategy.c)，如果调整顺序，需要同步修改dts */
enum dump_subsys {
    DUMP_SUBSYS_MDM_START = 0,
    DUMP_SUBSYS_MDMAP = DUMP_SUBSYS_MDM_START,
    DUMP_SUBSYS_CP, /* C核 */
    DUMP_SUBSYS_DPA,
    DUMP_SUBSYS_MDMLPM, /* PHONE：modem在lpmcu上的驱动，MBB：与DUMP_SUBSYS_LPMCU无差异 */
    DUMP_SUBSYS_COMM,
    DUMP_SUBSYS_MDMBUTT,
    DUMP_SUBSYS_MDMMAX = 31,
    DUMP_SUBSYS_OTHER_START = 32,
    DUMP_SUBSYS_AP = DUMP_SUBSYS_OTHER_START, /* 非modem子系统，对应于ACPU子系统总体 */
    DUMP_SUBSYS_LPMCU, /* 非modem子系统，对应于LPMCU子系统总体 */
    DUMP_SUBSYS_HIFI, /* 非modem子系统，对应于HIFI子系统总体 */
    DUMP_SUBSYS_OTHER_BUTT,
    DUMP_SUBSYS_MAX = 63
};

#define DUMP_SUBSYS_MASK(n) (1ul << (n))
enum dump_subsys_mask {
    DUMP_SUBSYS_MASK_MDM_START = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDM_START),
    DUMP_SUBSYS_MASK_MDMAP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMAP),
    DUMP_SUBSYS_MASK_CP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_CP),
    DUMP_SUBSYS_MASK_DPA = DUMP_SUBSYS_MASK(DUMP_SUBSYS_DPA),
    DUMP_SUBSYS_MASK_MDMLPM = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMLPM),
    DUMP_SUBSYS_MASK_COMM = DUMP_SUBSYS_MASK(DUMP_SUBSYS_COMM),
    DUMP_SUBSYS_MASK_MDMBUTT = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMBUTT),
    DUMP_SUBSYS_MASK_MDMMAX = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMMAX),
    DUMP_SUBSYS_MASK_OTHER_START = DUMP_SUBSYS_MASK(DUMP_SUBSYS_OTHER_START),
    DUMP_SUBSYS_MASK_AP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_AP),
    DUMP_SUBSYS_MASK_LPMCU = DUMP_SUBSYS_MASK(DUMP_SUBSYS_LPMCU),
    DUMP_SUBSYS_MASK_HIFI = DUMP_SUBSYS_MASK(DUMP_SUBSYS_HIFI),
    DUMP_SUBSYS_MASK_OTHER_BUTT = DUMP_SUBSYS_MASK(DUMP_SUBSYS_OTHER_BUTT),
    DUMP_SUBSYS_MASK_MAX = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MAX),
};

#define DUMP_SUBSYS_MDM_VALID_MASK (DUMP_SUBSYS_MASK_MDMBUTT - DUMP_SUBSYS_MASK_MDM_START)
#define DUMP_SUBSYS_OTHER_VALID_MASK (DUMP_SUBSYS_MASK_OTHER_BUTT - DUMP_SUBSYS_MASK_OTHER_START)
#define DUMP_SUBSYS_VALID_MASK ((DUMP_SUBSYS_MDM_VALID_MASK) | (DUMP_SUBSYS_OTHER_VALID_MASK))

/* <= dump_global_head中的module_name_len */
#define DUMP_SUBSYS_NAME_LEN_MAX 16

typedef struct {
    const u32 dump_sys;
    const char *name;
    /* 异常配置信息,num为0时,cfg_list可为NULL */
    dump_subsys_exc_cfg_s exc_cfg;
    /* 不允许为NULL，至少要实现一个subsys_done_done的work，同步异步均可 */
    dump_work_ctrl_s *work_ctrl;

    /* dump interface */
    /* 通知各子系统准备dump，比如发送ipc等，如果没有，需打桩，不能为NULL；
     * 注意：该接口是同步接口，不允许进行异步操作
     * 如果某些场景下，子系统不希望自己的dump work被调度；那么需要在dump_prepare回调中，将done_mask置位
     */
    int (*dump_prepare_handler)(const dump_exc_log_desc_s *exc_desc);

    /* reset interface */
    /* reset_ops：子系统复位回调，当前设计为只支持单个子系统复位
     * 注意：该接口目前需要是同步接口，返回值为dump_reset_result_e
     * 该接口可以为NULL，表示子系统不支持单独复位
     */
    dump_reset_result_e (*reset_ops)(const dump_exc_log_desc_s *exc_desc);

    /* event interface */
    /* 1. DUMP_EVENT_RESET_START: 通知各子系统，某些子系统即将进行reset，用于让各子系统清除dump标记等操作
     * 2. DUMP_EVENT_EXC_HANDLE_DONE: 通知各子系统，当前异常处理完成(不论成功或者失败)
     * 该接口不能为NULL
     */
    int (*event_handler)(int event, const dump_exc_log_desc_s *exc_desc);
} dump_subsys_drv_s;

#endif