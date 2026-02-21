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

#include <securec.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <osl_types.h>
#include <osl_malloc.h>
#include <bsp_mprobe.h>
#include <bsp_print.h>
#include "coresight_trace_mgr.h"
#include "coresight_mprobe.h"
#include "mdrv_diag.h"
#include <bsp_diag.h>
#define DIAG_MXT_TRACE_ID(msg_id) DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_OM, DIAG_DRV_MODULE_TRACE, msg_id)

#undef THIS_MODU
#define THIS_MODU mod_coresight
struct work_struct g_cs_mprobe_work;

void coresight_mprobe_send(struct work_struct *work)
{
    unsigned int i, cnt, left, cmd_id;
    struct coresight_trace_s trace_info = coresight_get_trace_info();
    u8 *data = (u8 *)(uintptr_t)trace_info.virt_ddr_buffer;
    u32 size = trace_info.ddr_buffer_len;
    if (data == NULL || size == 0) {
        return;
    }
    cnt = size / CORESIGHT_MPROBE_MAX_PACKAGE;
    left = size % CORESIGHT_MPROBE_MAX_PACKAGE;

    cmd_id = DIAG_MXT_TRACE_ID(0x1);
    for (i = 0; i < cnt; i++) {
        (void)bsp_diag_mxt_report(cmd_id, data, CORESIGHT_MPROBE_MAX_PACKAGE);
        data = data + CORESIGHT_MPROBE_MAX_PACKAGE;
    }
    cmd_id = DIAG_MXT_TRACE_ID(0x2);
    (void)bsp_diag_mxt_report(cmd_id, data, left);
    bsp_err("save complete\n");
}

static void coresight_func_reg(unsigned char *data)
{
    u32 ddr_size = 0;
    int i;
    diag_frame_head_s *head = (diag_frame_head_s *)data;
    if (head == NULL) {
        return;
    }
    if (DIAG_GET_MSG_ID_FROM_CMD_ID(head->cmd_id) == 0) {
        if (head->msg_len == sizeof(ddr_size)) {
            for (i = 0; i < head->msg_len; i++) {
                ddr_size = ddr_size * 0x100 + head->data[i];
            }
            coresight_mprob_set(ddr_size);
            return;
        }
    } else {
        schedule_work(&g_cs_mprobe_work);
    }
    return;
}

void coresight_mprobe_init(void)
{
    unsigned int cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_OM, DIAG_DRV_MODULE_TRACE, 0); // 0x9FA02480
    INIT_WORK(&g_cs_mprobe_work, coresight_mprobe_send);
    bsp_diag_drv_cfg_fn_register(cmd_id, coresight_func_reg);
}