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

#ifndef _MSG_CHAN_H_
#define _MSG_CHAN_H_

#include <linux/types.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <osl_types.h>
#include <mdrv_diag.h>
#include <bsp_msg.h>
#include <osl_sem.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    unsigned int port;
} diag_port_info_s;

#define DIAG_MSG_MAX_LEN (4 * 1024)

#define DIAG_MSG_GET_PORT_TYPE(data) (((diag_frame_req_s *)(data))->port_type)
#define DIAG_MSG_SET_PORT_TYPE(data, port_type) (((diag_frame_req_s *)(data))->port_type = (port_type))

typedef enum {
    DIAG_MSG_ID_REQ_DL = 0,             /* 工具下发的命令 ap->cp */
    DIAG_MSG_DIAGTOOL_DISCONN_REQ,      /* 端口断开 ap->cp */
    DIAG_MSG_HMISRV_DISCONN_REQ,        /* 端口断开 ap->cp */
    DIAG_MSG_MXT_DISCONN_REQ,           /* 端口断开 ap->cp */
    DIAG_MSG_HMISRV_LOGCAT_DISCONN_REQ, /* 重通道断连结果 cp->ap */
    DIAG_MSG_DIAGTOOL_CONN_REQ,         /* 连接结果 cp->ap */
    DIAG_MSG_HMISRV_CONN_REQ,           /* 连接结果 cp->ap*/
    DIAG_MSG_HMISRV_LOGCAT_CONN_REQ,    /* 重通道连接结果 cp->ap */
    DIAG_MSG_MXT_CONN_REQ,              /* 连接结果 cp->ap 8 */
    DIAG_MSG_DIAGTOOL_CONN_AUTH,        /* 鉴权结果 cp->ap */
    DIAG_MSG_HMISRV_CONN_AUTH,          /* 鉴权结果 cp->ap */
    DIAG_MSG_MXT_CONN_AUTH,             /* 鉴权结果 cp->ap 11 */
    DIAG_MSG_RX_READY,                  /* Rx Ready处理 cp->ap */
    DIAG_MSG_POWERON_LOG,               /* 开机LOG cp->ap */
    DIAG_MSG_MSP_REQ,                   /* 转发MSP开关 cp->ap->dpa 14 */
    DIAG_MSG_ID_REPORT,                 /*上报给工具 cp->ap */
    DIAG_MSG_PORT_CFG,                  /* 物理端口设置 cp->ap */
    DIAG_MSG_SYNC_CONN_DBG_INFO,        /* 同步状态机维测 cp->ap */
    DIAG_MSG_LOGCAT_DISCONN_REQ,        /* 端口断开 ap->cp */
    DIAG_INNER_MSG_TYPE_BUTT
} diag_msglite_id_e;

typedef struct {
    unsigned int msg_id;
    unsigned char data[0];
} msg_frame_s;

typedef struct {
    MDRV_MSG_HEAD
    unsigned char data[0];
} msg_head_s;

typedef void (*msg_chan_dl_cb)(unsigned int port_type, const unsigned char *data, unsigned int data_len);
typedef int (*msg_chan_data_dispatch_cb)(msg_frame_s *data, unsigned int data_len);
typedef struct {
    msg_chn_t msg_chan_hdl;
    osl_sem_id slicesem;
    struct list_head slicelist;
    struct wakeup_source *wakelock;
    msg_chan_dl_cb dl_data_proc;
    msg_chan_data_dispatch_cb data_dispatch; 
} msg_chan_info_s;

void diag_msg_chan_data_dispatch_cb_regitster(msg_chan_data_dispatch_cb func);

int diag_msglite_send_to_tsp(unsigned char *data, unsigned int len);
int diag_msglite_send_to_dpa(unsigned char *data, unsigned int len);
int diag_msg_send(unsigned char *data, unsigned int len);

int diag_chan_mgr_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
