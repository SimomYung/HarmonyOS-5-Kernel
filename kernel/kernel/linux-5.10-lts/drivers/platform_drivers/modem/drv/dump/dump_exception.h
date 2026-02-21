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

#ifndef _DUMP_EXCEPTION_H_
#define _DUMP_EXCEPTION_H_

#include <osl_types.h>
#include <bsp_dump.h>
#include <dump_infra.h>

typedef struct {
    u32 num;
    /* 按优先级排序，优先级越高的放在前面；支持小region覆盖大region，大region放在后面；子系统上报的src modid找不到时，默认使用最后一个cfg */
    const dump_exc_cfg_s *cfg_list;
} dump_subsys_exc_cfg_s;

typedef struct {
    const dump_exc_info_s *exc_info;
    u64 dump_done_subsys; /* 已完成dump的subsys mask */
    char log_path[RDR_DUMP_FILE_PATH_LEN];
} dump_exc_log_desc_s;

enum dump_exc_rpt_ret {
    DUMP_EXC_REJECT_NOMEM = -3,
    DUMP_EXC_REJECT_INVALID = -2,
    DUMP_EXC_REJECT_REENTRY = -1,
    DUMP_EXC_ACCEPT_NOW = 0,
    DUMP_EXC_ACCEPT_LATER,
    DUMP_EXC_RET_BUTT,
};

typedef enum {
    DUMP_RESET_NOT_SUPPORT = 0,
    DUMP_RESET_NOT_SUCCESS = 1,
    DUMP_RESET_SUCCESS = 2,
} dump_reset_result_e;

#endif