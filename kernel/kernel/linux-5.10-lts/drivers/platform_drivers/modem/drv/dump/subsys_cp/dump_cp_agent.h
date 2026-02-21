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
#ifndef __DUMP_CP_AGENT_H__
#define __DUMP_CP_AGENT_H__

#include <osl_types.h>
#include <dump_subsys.h>
#include <dump_works.h>

#define DUMP_CP_REST_TIME_COUNT 8
#define DUMP_CP_REST_TIME_SLICE (60 * 0x8000)
#define RDR_MODEM_CP_RESET_TIMEOUT 30000

typedef struct {
    u64 reset_time[DUMP_CP_REST_TIME_COUNT];
    u32 count;
} dump_cp_reset_s;

typedef struct {
    bool init_done;
    bool cp_exc_flag;
    dump_cp_reset_s reset_info;
    u32 modem_off;
    dump_work_ctrl_s work_ctrl;
} dump_cp_agent_ctrl_s;


/* 内部接口，用于wdt检测到CP异常时，先通知CP，再做后续流程 */
void dump_cp_async_exc_proc(u32 mod_id);
/* 内部接口，根据baseinfo生成subsys_exc信息 */
int dump_fill_cp_exc_info(dump_base_info_s *base_info, dump_subsys_exc_info_s *subsys_exc);

void dump_cp_subsys_drv_prepare(void);
void dump_cp_subsys_init(void);
/* 若某个异常需要子系统A复位，但是子系统A又不支持单独复位，那么就配置成整机复位 */
void dump_cp_subsys_update_exc_cfg(u64 reset_support_subsysmask);
#endif
