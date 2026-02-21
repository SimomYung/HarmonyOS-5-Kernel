/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
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

#include <bsp_dump.h>
#include "ipcmsg_mntn.h"

struct ipcmsg_mntn_ctrl g_ipcmsg_mntn = {0};

/* lock free & allow data coverage */
void ipcmsg_mntn_trace(u32 res)
{
    u32 idx;
    struct ipcmsg_mntn_ctrl *mntn_ctrl = &g_ipcmsg_mntn;

    if (mntn_ctrl->init_status != IPCMSG_MNTN_INITED) {
        return;
    }
    idx = mntn_ctrl->mntn_info->node_idx % IPCMSG_MNTN_INFO_NUM;
    mntn_ctrl->mntn_info->node[idx].timestamp = bsp_get_slice_value();
    mntn_ctrl->mntn_info->node[idx].res = res;
    mntn_ctrl->mntn_info->node_idx++;
}

int ipcmsg_mntn_init(device_node_s *dev_node)
{
    int ret;
    u32 dump_size, data_len;
    struct ipcmsg_mntn_ctrl *mntn_ctrl = &g_ipcmsg_mntn;

    mntn_ctrl->init_status = IPCMSG_MNTN_UNINIT;
    ret = bsp_dt_property_read_u32(dev_node, "dump_size", &dump_size);
    if (ret) {
        ipcmsg_print_err("ipcmsg doesn't support dump\n");
        return IPCMSG_OK;
    }

    data_len = sizeof(struct ipcmsg_mntn_info) + IPCMSG_MNTN_INFO_NUM * sizeof(struct ipcmsg_mntn_node);
    if (data_len > dump_size) {
        ipcmsg_print_err("dump space isn't enough\n");
        return IPCMSG_ERROR;
    }

    mntn_ctrl->mntn_info = (struct ipcmsg_mntn_info *)bsp_dump_register_field(DUMP_MDMAP_IPCMSG, "IPCMSG", data_len, 0);
    if (mntn_ctrl->mntn_info == NULL) {
        ipcmsg_print_err("regist field err!\n");
        return IPCMSG_ERROR;
    }
    (void)memset_s(mntn_ctrl->mntn_info, data_len, 0, data_len);
    mntn_ctrl->mntn_info->hdr.len = data_len - sizeof(struct ipcmsg_mntn_info);
    mntn_ctrl->mntn_info->hdr.version = 0;

    mntn_ctrl->init_status = IPCMSG_MNTN_INITED;
    return IPCMSG_OK;
}
