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
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS"
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

#ifndef GENL_PORT_H
#define GENL_PORT_H

#include <osl_types.h>
#include <linux/netdevice.h>
#include <net/genetlink.h>

#define GENL_MSG_CMD 1
#define GENL_MSG_DATA 2

typedef enum {
    GENL_CNF_NOCODER_CHAN,  /* 信令消息&实时上报消息（不进行HDLC解码） */
    GENL_IND_NOCODER_CHAN, /*单板内部解析消息（不进行HDLC解码） */
    GENL_IND_CODER_CHAN, /* 普通OAM消息（经过HDLC解码） */
    GENL_PORT_CHAN_BUTT,
} genl_chan_e;

enum genl_state {
    GENL_DISCONNECT,
    GENL_CONNECT,
    GENL_SWITCH_PORT,
};

enum genl_msg_type{
    GENL_MSG_BASE = 0,
    GENL_MSG_BIND_PORT,

    GENL_MSG_SWITCH_PORT = GENL_MSG_BASE + 0x65,
    GENL_MSG_GET_PORT_TYPE,
    GENL_MSG_SET_LOG_MODE,
    GENL_MSG_GET_LOG_MODE,
    GENL_MSG_SET_LITE_MODE,
    GENL_MSG_GET_LITE_MODE,

    GENL_MSG_HANDLE_CMD = GENL_MSG_BASE + 0x12D,

    GENL_MSG_HANDLE_DATA = GENL_MSG_BASE + 0x1F5,
};

typedef struct {
    unsigned short mode;
    unsigned short timeout;
} genl_logmode_s;

typedef void (*genl_recv_cb)(unsigned char *data, unsigned int len);
typedef void (*genl_event_cb)(unsigned int state);
typedef int (*genl_port_switch_cb)(unsigned char *data, unsigned int len);

int genl_init(void);
int genl_send(genl_chan_e chn, unsigned char *buf, unsigned int len);
int genl_register_recv_cb(genl_chan_e chn, genl_recv_cb cb);
int genl_register_event_cb(genl_event_cb cb);
int genl_register_port_switch_cb(genl_port_switch_cb cb);
#endif
