/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

#include <product_config.h>
#include "ppm.h"
#include "genl_port.h"
#include "sock_port.h"
#include "usb_port.h"
#include "bsp_print.h"
#include <securec.h>
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    USB_PHY,
    GENL_PHY,
    SOCK_PHY,
    ESL_PHY,
    PHY_BUTT
} ppm_phy_e;

typedef struct ppm_ctrl{
    ppmstate_e state;

    ppm_phy_e ind_coder_phy;
    ppm_phy_e cnf_coder_phy;
    ppm_logic_port_e logic_port;
    ppm_phy_e support_diag_tool_phy;

    send_cb cnf_sendcb;
    send_cb ind_sendcb;
    send_cb debugging_sendcb;
    event_cb event_notify[CHAN_BUTT];
} ppm_ctrl_s;
ppm_ctrl_s g_ppm_ctrl = {0};

int ppm_cnf_coder_send(char *data, char *phyaddr, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    ppm_phy_e phy = ctrl->cnf_coder_phy;

    int ret = -1;

    switch (phy) {
        case USB_PHY:
            ret = usb_send(USB_CNF_CODER_CHAN, data, phyaddr, len);
            break;
        case ESL_PHY:
        case SOCK_PHY:
            ret = sock_send(SOCK_CNF_CODER_CHAN, data, len);
            break;
        default:
            diag_err("ppm_cnf_coder_send invlid ,phy:%u\n", phy);
    }
    return ret;
}

int ppm_ind_coder_send(char *data, char *phyaddr, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    ppm_phy_e phy = ctrl->ind_coder_phy;
    int ret = -1;

    switch (phy) {
        case USB_PHY:
            ret = usb_send(USB_IND_CODER_CHAN, data, phyaddr ,len);
            break;
        case GENL_PHY:
            ret = genl_send(GENL_IND_CODER_CHAN, data, len);
            if (ret != 0) {
                diag_err("genl_send fail for GENL_PHY.\n");
            } else {
                ret = len;
            }
            break;
        case ESL_PHY:
        case SOCK_PHY:
            ret = sock_send(SOCK_IND_CODER_CHAN, data, len);
            break;
        default:
            diag_err("ppm_ind_coder_send invlid ,phy:%u\n", phy);
    }
    return ret;
}

int ppm_data_send(ppm_chan_e chn, char *data, char *phyaddr, unsigned int len)
{
    int ret = -1;

    if (chn >= CHAN_BUTT || data == NULL) {
        diag_err("ppm_send para err, chn:%u\n", chn);
        return -1;
    }
    switch (chn) {
        case PPM_CNF_NOCODER_CHAN:
            ret = genl_send(GENL_CNF_NOCODER_CHAN, data, len);
            if (ret != 0) {
                diag_err("genl_send fail for PPM_CNF_NOCODER_CHAN.\n");
            } else {
                ret = len;
            }
            break;
        case PPM_IND_NOCODER_CHAN:
            ret = genl_send(GENL_IND_NOCODER_CHAN, data, len);
            if (ret != 0) {
                diag_err("genl_send fail for PPM_IND_NOCODER_CHAN.\n");
            } else {
                ret = len;
            }
            break;
        case PPM_IND_CODER_CHAN:
            ret = ppm_ind_coder_send(data, phyaddr, len);
            break;
        case PPM_CNF_CODER_CHAN:
            ret = ppm_cnf_coder_send(data, phyaddr, len);
            break;
        case PPM_DEBUGGING_CODER_CHAN:
#ifdef DIAG_VERSION_ENG
            ret = usb_send(USB_DEBUGGING_CODER_CHAN, data, phyaddr, len);
#else
            diag_err("ppm user stub debugging port send\n");
            ret = 0;
#endif
            break;
        default:
            diag_err("ppm_send invlid chn:%u\n", chn);
    }
    return ret;
}

void ppm_cnf_sendcb(unsigned char *data, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    if (ctrl->cnf_sendcb != NULL) {
        ctrl->cnf_sendcb(PPM_CNF_CODER_CHAN, data, len);
    }
}

void ppm_ind_sendcb(unsigned char *data, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    if (ctrl->ind_sendcb != NULL) {
        ctrl->ind_sendcb(PPM_IND_CODER_CHAN, data, len);
    }
}

#ifdef DIAG_VERSION_ENG
void ppm_debugging_sendcb(unsigned char *data, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    if (ctrl->debugging_sendcb != NULL) {
        ctrl->debugging_sendcb(PPM_DEBUGGING_CODER_CHAN, data, len);
    }
}
#endif

int ppm_register_send_cb(ppm_chan_e chn, send_cb cb)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    int ret = -1;

    if (chn >= CHAN_BUTT || cb == NULL) {
        diag_err("ppm_register_send_cb para err, chn:%u\n", chn);
        return -1;
    }
    switch (chn) {
        case PPM_CNF_CODER_CHAN:
            ctrl->cnf_sendcb = cb;
            ret = usb_register_send_cb(USB_CNF_CODER_CHAN, (usb_send_cmp_cb)ppm_cnf_sendcb);
            break;
        case PPM_IND_CODER_CHAN:
            ctrl->ind_sendcb = cb;
            ret = usb_register_send_cb(USB_IND_CODER_CHAN, (usb_send_cmp_cb)ppm_ind_sendcb);
            break;
        case PPM_DEBUGGING_CODER_CHAN:
#ifdef DIAG_VERSION_ENG
            ctrl->debugging_sendcb = cb;
            ret = usb_register_send_cb(USB_DEBUGGING_CODER_CHAN, (usb_send_cmp_cb)ppm_debugging_sendcb);
#else
            diag_err("ppm user stub debugging port send_cb\n");
            ret = 0;
#endif
            break;
        default:
            diag_err("ppm_register_send_cb invlid chn:%u\n", chn);
    }
    return ret;
}

int ppm_cnf_coder_register_recv_cb(recv_cb cb)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    ppm_phy_e phy = ctrl->cnf_coder_phy;

    switch (phy) {
        case USB_PHY:
            diag_debug("USB PHY\n");
            usb_register_recv_cb(USB_CNF_CODER_CHAN, (usb_recv_cb)cb);
            diag_debug("sock PHY\n");
            sock_register_recv_cb(SOCK_CNF_CODER_CHAN, (sock_recv_cb)cb);
            break;
        default:
            diag_err("ppm_cnf_coder_register invlid, phy:%u\n", phy);
    }
    return 0;
}

int ppm_register_recv_cb(ppm_chan_e chn, recv_cb cb)
{
    int ret = 0;

    if (chn >= CHAN_BUTT || cb == NULL) {
        diag_err("ppm_register_recv_cb para err, chn:%u\n", chn);
        return -1;
    }
    switch (chn) {
        case PPM_CNF_NOCODER_CHAN:
            ret = genl_register_recv_cb(GENL_CNF_NOCODER_CHAN, cb);
            break;
        case PPM_CNF_CODER_CHAN:
            ret = ppm_cnf_coder_register_recv_cb(cb);
            break;
        case PPM_DEBUGGING_CODER_CHAN:
#ifdef DIAG_VERSION_ENG
            ret = usb_register_recv_cb(USB_DEBUGGING_CODER_CHAN, (usb_recv_cb)cb);
#else
            diag_err("ppm user stub debugging port recv cb\n");
            ret = 0;
#endif
            break;
        default:
            diag_err("ppm_register_recv_cb invlid chn:%u\n", chn);
    }
    return ret;
}

int ppm_logic_port_switch(ppm_logic_port_e port)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    if (ctrl->logic_port == port || (ctrl->logic_port == PPM_DIAG_TOOL_PORT && ctrl->state == PPM_STAT_CONNECT)) {
        diag_dbg_port_switch_log(ctrl->logic_port, port, ctrl->logic_port);
        diag_err("ppm_logic_port ignore swtich port:%u\n", port);
        return 0;
    }
    if (port == PPM_HMI_PORT) {
        ctrl->ind_coder_phy = GENL_PHY;
    } else {
        ctrl->ind_coder_phy = ctrl->support_diag_tool_phy;
    }
    ctrl->logic_port = port;
    diag_dbg_port_switch_log(ctrl->logic_port, port, port);
    diag_err("ppm_logic_port swtich:%u\n", port);
    return 0;
}

int ppm_register_port_switch_cb(ppm_chan_e chn, port_switch_cb cb)
{
    return genl_register_port_switch_cb((genl_port_switch_cb)cb);
}

int ppm_register_event_cb(ppm_chan_e chn, event_cb cb)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    if (chn >= CHAN_BUTT) {
        return -1;
    }
    ctrl->event_notify[chn] = cb;
    return 0;
}

void ppm_cnf_coder_connect_event(unsigned int event)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    diag_err("ppm_cnf_coder_connect_event event cb\n");
    if (ctrl->event_notify[PPM_CNF_CODER_CHAN] != NULL) {
        ctrl->event_notify[PPM_CNF_CODER_CHAN](PPM_CNF_CODER_CHAN, (ppmstate_e)event);
    }
}

void ppm_ind_coder_connect_event(unsigned int event)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    diag_err("ppm_ind_coder_connect_event event cb, cur phy:%d \n", ctrl->ind_coder_phy);
    if (ctrl->event_notify[PPM_IND_CODER_CHAN] != NULL && ctrl->ind_coder_phy != GENL_PHY) {
        ctrl->event_notify[PPM_IND_CODER_CHAN](PPM_IND_CODER_CHAN, (ppmstate_e)event);
    }
}

#ifdef DIAG_VERSION_ENG
void ppm_debugging_coder_connect_event(unsigned int event)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    diag_err("ppm_debugging_coder_connect_event event cb\n");
    if (ctrl->event_notify[PPM_DEBUGGING_CODER_CHAN] != NULL) {
        ctrl->event_notify[PPM_DEBUGGING_CODER_CHAN](PPM_DEBUGGING_CODER_CHAN, (ppmstate_e)event);
    }
}
#endif

void ppm_genl_event(unsigned int state)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    if (ctrl->event_notify[PPM_CNF_NOCODER_CHAN] != NULL) {
        ctrl->event_notify[PPM_CNF_NOCODER_CHAN](PPM_CNF_NOCODER_CHAN, (ppmstate_e)state);
    }
    if (ctrl->event_notify[PPM_IND_NOCODER_CHAN] != NULL) {
        ctrl->event_notify[PPM_IND_NOCODER_CHAN](PPM_IND_NOCODER_CHAN, (ppmstate_e)state);
    }
    if (ctrl->event_notify[PPM_IND_CODER_CHAN] != NULL && ctrl->ind_coder_phy == GENL_PHY) {
        ctrl->event_notify[PPM_IND_CODER_CHAN](PPM_IND_CODER_CHAN, (ppmstate_e)state);
    }
}

void ppm_usb_port_disable(void)
{
    (void)usb_register_event_cb(USB_CNF_CODER_CHAN, NULL);
    (void)usb_register_event_cb(USB_IND_CODER_CHAN, NULL);
    usb_register_recv_cb(USB_CNF_CODER_CHAN, NULL);
}

void ppm_cfg_phy_port_proc(unsigned int *port, unsigned int len)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    if (len != sizeof(unsigned int)) {
        diag_err("cfg port para invalid, len:%d\n", len);
        return;
    }
    if (*port == ctrl->support_diag_tool_phy) {
        diag_err("cfg phy port: %d(0:usb, 2:socket)\n", ctrl->support_diag_tool_phy);
        return;
    }
    if (*port == SOCK_PHY) {
        if (sock_init()) {
            diag_err("sock init fail\n");
            return;
        }
        ppm_usb_port_disable();
        (void)sock_register_event_cb(SOCK_CNF_CODER_CHAN, (sock_event_cb)ppm_cnf_coder_connect_event);
        (void)sock_register_event_cb(SOCK_IND_CODER_CHAN, (sock_event_cb)ppm_cnf_coder_connect_event);
        ctrl->support_diag_tool_phy = SOCK_PHY;
        ctrl->cnf_coder_phy = SOCK_PHY;
        ctrl->ind_coder_phy = ctrl->support_diag_tool_phy;
        diag_debug("cfg phy port: %d(0:usb, 2:socket)\n", ctrl->support_diag_tool_phy);
    } else {
        diag_err("invalid port:%d\n", *port);
    }
    return;
}

int ppm_init(void)
{
    int ret;
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;

    ret = genl_init();
    if (ret) {
        return ret;
    }
    (void)genl_register_event_cb((genl_event_cb)ppm_genl_event);

    ret = sock_sec_init();
    if (ret) {
        return ret;
    }
    // socket usb esl端口实现后，要注意eventcb的挂载，维护连接状态
    // usb初始化不判返回值，phone版本user形态usb接口会返回-1，但不影响diag初始化
    (void)usb_init();

    (void)usb_register_event_cb(USB_CNF_CODER_CHAN, (usb_event_cb)ppm_cnf_coder_connect_event);
    (void)usb_register_event_cb(USB_IND_CODER_CHAN, (usb_event_cb)ppm_ind_coder_connect_event);
#ifdef DIAG_VERSION_ENG
    (void)usb_register_event_cb(USB_DEBUGGING_CODER_CHAN, (usb_event_cb)ppm_debugging_coder_connect_event);
#endif
    ctrl->support_diag_tool_phy = USB_PHY;
    ctrl->cnf_coder_phy = USB_PHY; // 默认usb上报
    ctrl->ind_coder_phy = ctrl->support_diag_tool_phy; // 默认sock/usb上报
    ctrl->logic_port = PPM_DIAG_TOOL_PORT;

    ctrl->state = PPM_STAT_DISCONNECT; // ppm的state只维护了一个状态，三个工具不够分，并且phy event cb转换时未维护
    diag_crit("diag ppm init success\n");
    return 0;
}

void diag_show_ppm_status(void)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    diag_err("state=%d\n", ctrl->state);
    diag_err("ind_coder_phy=%d\n", ctrl->ind_coder_phy);
    diag_err("cnf_coder_phy=%d\n", ctrl->cnf_coder_phy);
    diag_err("logic_port=%d\n", ctrl->logic_port);
    diag_err("support_diag_tool_phy=%d\n", ctrl->support_diag_tool_phy);
}

int diag_test_set_port_genl(void)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    ctrl->ind_coder_phy = GENL_PHY;
    ctrl->logic_port = PPM_HMI_PORT;
    return 0;
}

int diag_test_set_port_usb(void)
{
    ppm_ctrl_s *ctrl = &g_ppm_ctrl;
    ctrl->ind_coder_phy = USB_PHY;
    ctrl->logic_port = PPM_DIAG_TOOL_PORT;
    return 0;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
