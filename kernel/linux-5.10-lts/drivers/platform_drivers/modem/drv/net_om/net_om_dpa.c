/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include <bsp_msg.h>
#include <msg_id.h>
#include "bsp_dpa.h"
#include "net_om.h"

#undef THIS_MODU
#define THIS_MODU mod_netom

struct net_om_frame_s{
    unsigned int report_swt;
    unsigned int report_period;
};

msg_chn_t g_net_om_msg_hdl;

int net_om_send_cfg_to_dpa(net_om_diag_cmd_s *net_om_diag_cmd)
{
    int ret;
    struct msg_addr dst;
    struct net_om_frame_s net_om_frame;

    net_om_frame.report_swt = net_om_diag_cmd->net_om_switch_dpa;
    net_om_frame.report_period = net_om_diag_cmd->report_log_period_dpa;

    dst.core = MSG_CORE_DPA;
    dst.chnid = MSG_CHN_NET_OM;

    ret = bsp_msg_sendto_dpa(g_net_om_msg_hdl, &dst, (void *)&net_om_frame, sizeof(struct net_om_frame_s));

    return ret;
}

static int net_om_msg_rcv_proc(const struct msg_addr *src, void *msg, unsigned int len)
{
    return 0;
}

int net_om_msg_init(void)
{
    int ret = 0;
    struct msgchn_attr lite_attr = { 0 };

    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_NET_OM;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_DPA);
    lite_attr.lite_notify = net_om_msg_rcv_proc;
    ret = bsp_msg_lite_open(&g_net_om_msg_hdl, &lite_attr);
    if (unlikely(ret)) {
        bsp_err("net om msg open err, ret=%d\n", ret);
        return ret;
    }

    return ret;
}