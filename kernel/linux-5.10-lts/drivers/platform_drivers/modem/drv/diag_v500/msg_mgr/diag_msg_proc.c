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
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_malloc.h>
#include <bsp_slice.h>
#include <bsp_odt.h>
#include "msg_chan.h"
#include "soft_decode.h"
#include "diag_msp_msg.h"
#include "diag_msg_proc.h"
#include "diag_debug.h"
#include "ppm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

int diag_msg_dispatch(msg_frame_s *msg_frame, unsigned int msg_size)
{
    diag_debug("recv msglite, msg_id:0x%x", msg_frame->msg_id);
    switch (msg_frame->msg_id) {
        case DIAG_MSG_DIAGTOOL_CONN_REQ:
            diagtool_conn_msg_proc();
            break;

        case DIAG_MSG_DIAGTOOL_DISCONN_REQ:
            diagtool_disconn_msg_proc();
            break;

        case DIAG_MSG_DIAGTOOL_CONN_AUTH:
            diag_auth_msg_proc(DIAG_TOOL_TYPE_DIAGTOOL);
            break;

        case DIAG_MSG_HMISRV_CONN_REQ:
            hmisrv_conn_msg_proc();
            break;

        case DIAG_MSG_HMISRV_LOGCAT_CONN_REQ:
            hmisrv_logcat_conn_msg_proc();
            break;

        case DIAG_MSG_HMISRV_DISCONN_REQ:
            hmisrv_disconn_msg_proc();
            break;

        case DIAG_MSG_HMISRV_LOGCAT_DISCONN_REQ:
            hmisrv_logcat_disconn_msg_proc();
            break;

        case DIAG_MSG_HMISRV_CONN_AUTH:
            diag_auth_msg_proc(DIAG_TOOL_TYPE_HMISRV);
            break;

        case DIAG_MSG_MXT_CONN_REQ:
            mxt_conn_msg_proc();
            break;

        case DIAG_MSG_MXT_DISCONN_REQ:
            mxt_disconn_msg_proc();
            break;

        case DIAG_MSG_MXT_CONN_AUTH:
            diag_auth_msg_proc(DIAG_TOOL_TYPE_MXT);
            break;

        case DIAG_MSG_RX_READY:
            diagtool_rxready_msg_proc();
            break;

        case DIAG_MSG_POWERON_LOG:
            diag_poweron_log_cfg_proc((diag_log_cfg_s *)msg_frame->data, msg_size - sizeof(msg_frame_s));
            break;

        case DIAG_MSG_MSP_REQ:
            diag_msp_msg_proc((unsigned char *)msg_frame->data, msg_size - sizeof(msg_frame_s));
            break;

        case DIAG_MSG_PORT_CFG:
            ppm_cfg_phy_port_proc((unsigned int *)msg_frame->data, msg_size - sizeof(msg_frame_s));
            break;

        default:
            diag_crit("receive msg id invalid, msg_id=0x%x\n", msg_frame->msg_id);
            break;
    }
    return 0;
}

int diag_msg_mgr_init(void)
{
    diag_msp_msg_init();
    // 处理C核消息
    diag_msg_chan_data_dispatch_cb_regitster(diag_msg_dispatch);

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
