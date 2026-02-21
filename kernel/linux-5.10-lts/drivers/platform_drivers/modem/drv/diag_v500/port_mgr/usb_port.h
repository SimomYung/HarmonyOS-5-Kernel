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

#ifndef DIAG_USB_PORT_H
#define DIAG_USB_PORT_H

typedef enum {
    USB_CNF_CODER_CHAN,
    USB_IND_CODER_CHAN,
    USB_DEBUGGING_CODER_CHAN,
    USB_PORT_CHAN_BUTT,
} usb_chan_e;

enum usb_state {
    USB_DISCONNECT,
    USB_CONNECT,
};

typedef void (*usb_send_cmp_cb)(unsigned char *data, unsigned int len);
typedef void (*usb_recv_cb)(unsigned char *data, unsigned int len);
typedef void (*usb_event_cb)(unsigned int state);

int usb_init(void);
int usb_send(usb_chan_e chn, unsigned char *virt_addr, unsigned char *phy_addr, unsigned int len);
int usb_register_send_cb(usb_chan_e chn, usb_send_cmp_cb cb);
int usb_register_recv_cb(usb_chan_e chn, usb_recv_cb cb);
// 连接状态发生变更时触发回调，入参是更新后状态，也可以调用usb_get_state获取连接状态更新。用户不同，每个通道单独申请cb
int usb_register_event_cb(usb_chan_e chn, usb_event_cb cb);
// ret=0: 断开连接； ret=1 连接； 参考enum usb_state
int usb_get_state(usb_chan_e chn);

#endif
