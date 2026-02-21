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

#include <securec.h>
#include <osl_types.h>
#include <bsp_slice.h>
#include <bsp_dump.h>
#include <dump_modid.h>
#include <dump_works.h>
#include <dump_subsys_mgr.h>
#include <dump_infra.h>
#include "dump_comm_log.h"
#include "dump_comm_agent.h"

int dump_comm_prepare_handler(const dump_exc_log_desc_s *exc_desc);
int dump_comm_event_handler(int event, const dump_exc_log_desc_s *exc_desc);

dump_comm_agent_ctrl_s g_dump_comm_agent;
__ro_after_init dump_subsys_drv_s g_dump_comm_subsys = {
    .dump_sys = DUMP_SUBSYS_COMM,
    .name = "MDMCOMM",
    .exc_cfg = {
        .num = 0,
        .cfg_list = NULL,
    },
    .work_ctrl = &g_dump_comm_agent.work_ctrl,
    .dump_prepare_handler = dump_comm_prepare_handler,
    .reset_ops = NULL, /* 不支持单独复位 */
    .event_handler = dump_comm_event_handler,
};

int dump_comm_prepare_handler(const dump_exc_log_desc_s *exc_desc)
{
    UNUSED(exc_desc);
    return DUMP_OK;
}

/******************************* dump_event：dump事件处理 ***************************/
int dump_comm_event_handler(int event, const dump_exc_log_desc_s *exc_desc)
{
    UNUSED(event);
    UNUSED(exc_desc);
    return DUMP_OK;
}

/* 允许多次调用 */
void dump_comm_subsys_drv_prepare(void)
{
    int ret;
    /* 使用专用workqueue，single-thread-wq，顺序执行，不支持并发 */
    ret = dump_work_ctrl_factory(&g_dump_comm_agent.work_ctrl, NULL, DUMP_COMM_WORKQUEUE_NAME);
    if (ret != DUMP_OK) {
        dump_print("comm work ctrl init fail\n");
    }

    dump_register_subsys(&g_dump_comm_subsys);
}

__init void dump_comm_subsys_update_exc_cfg(u64 reset_support_subsysmask)
{
    return;
}

__init void dump_comm_subsys_init(void)
{
    dump_comm_log_init();

    g_dump_comm_agent.init_state = true;

    dump_print("dump mdm comm init ok\n");
}
