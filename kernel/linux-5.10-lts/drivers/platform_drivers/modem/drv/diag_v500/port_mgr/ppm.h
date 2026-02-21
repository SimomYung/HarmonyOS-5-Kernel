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

#ifndef DIAG_PPM_H
#define DIAG_PPM_H

#define PPM_SEND_ASYNC 0

typedef enum {
    PPM_CNF_CODER_CHAN,
    PPM_CNF_NOCODER_CHAN,
    PPM_IND_CODER_CHAN,
    PPM_IND_NOCODER_CHAN,
    PPM_DEBUGGING_CODER_CHAN,
    CHAN_BUTT,
} ppm_chan_e;

typedef enum {
    PPM_HMI_PORT,
    PPM_DIAG_TOOL_PORT,
    PPM_PORT_BUTT,
} ppm_logic_port_e;

// 该状态应该与sock usb genl状态统排并保持一致
typedef enum {
    PPM_STAT_DISCONNECT,
    PPM_STAT_CONNECT,
    PPM_STAT_PORT_SWITCH,
} ppmstate_e;

// 暂时与genl_recv_cb保持一致，如果某phy_port(比如sock)结构变更，则在ppm_core进行封装，在注册到对应phy_port
typedef void (*recv_cb)(unsigned char *data, unsigned int len);
typedef int (*send_cb)(ppm_chan_e chn, unsigned char *data, unsigned int len);
typedef int (*event_cb)(ppm_chan_e chn, ppmstate_e stat);
typedef int (*port_switch_cb)(unsigned char *data, unsigned int len);

// recv_cb回调函数要求支持重入
int ppm_register_recv_cb(ppm_chan_e chn, recv_cb cb);

int ppm_register_send_cb(ppm_chan_e chn, send_cb cb);

int ppm_register_event_cb(ppm_chan_e chn, event_cb cb);

int ppm_register_port_switch_cb(ppm_chan_e chn, port_switch_cb cb);

// 返回值
// 1.ret < 0 表示发送失败； 
// 2.ret = PPM_SEND_ASYNC（0） 表示异步发送，调用者需要在发送完成回调中处理相关资源释放,和业务行为（如开中断）
// 3.ret > 0 标识发送成功，返回实际发送数据长度，理论上实际发送长度应等于len
int ppm_data_send(ppm_chan_e chn, char *data, char *phyaddr, unsigned int len);

// 入参port： 目标切换端口，即将要切换到的端口
// 返回值：ret = 0 表示成功； ret < 0 表示失败；
int ppm_logic_port_switch(ppm_logic_port_e port);

void ppm_cfg_phy_port_proc(unsigned int *port, unsigned int len);

int ppm_init(void);
#endif
