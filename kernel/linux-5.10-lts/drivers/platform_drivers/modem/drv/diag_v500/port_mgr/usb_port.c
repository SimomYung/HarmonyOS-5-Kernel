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
#include "usb_port.h"
#include <product_config.h>
#include "bsp_print.h"
#include <bsp_udi.h>
#include <mdrv_usb.h>
#include <mdrv_udi.h>
#include <osl_sem.h>
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 前置函数声明，g_usb_ctrl依赖以下函数定义，函数内实现又依赖g_usb_ctrl
void cnf_coder_chn_enable(void);
void cnf_coder_chn_disable(void);
void cnf_coder_chn_readcb(void);
void cnf_coder_chn_writecb(char *vaddr, char *paddr, int len);
void cnf_coder_chn_eventcb(ACM_EVT_E state);
void ind_coder_chn_enable(void);
void ind_coder_chn_disable(void);
void ind_coder_chn_readcb(void);
void ind_coder_chn_writecb(char *vaddr, char *paddr, int len);
void ind_coder_chn_eventcb(ACM_EVT_E state);
void debugging_chn_enable(void);
void debugging_chn_disable(void);
void debugging_chn_readcb(void);
void debugging_chn_writecb(char *vaddr, char *paddr, int len);
void debugging_chn_eventcb(ACM_EVT_E state);

typedef struct {
    // 维护与下层udi交互的设备号和本层回调函数
    unsigned int devid;
    usb_udi_enable_cb enable;
    usb_udi_disable_cb disable;
    void (*readcb)(void);
    void (*writecb)(char *vaddr, char *paddr, int len);
    void (*eventcb)(ACM_EVT_E state);
    unsigned long long fd; // homi里usb和socket归一使用了u64,这里继承
    unsigned int recv_buf_len;
    unsigned int recv_buf_num;

    // 维护上层注册进来的回调
    spinlock_t lock;
    enum usb_state state;
    usb_send_cmp_cb send_cmp_cb;
    usb_recv_cb recv_cb;
    usb_event_cb event_notify;
} usb_port_chn_ctrl_s;

typedef struct {
    usb_port_chn_ctrl_s chn[USB_PORT_CHAN_BUTT];
} diag_usb_ctrl_s;

#define PPM_USB_CNF_BUFF_SIZE (8 * 1024)
#define PPM_USB_CNF_BUFF_CNT 8
#define PPM_USB_IND_BUFF_SIZE (2 * 1024)
#define PPM_USB_IND_BUFF_CNT 2

// enable、disable、readcb、writecb、eventcb都可以归一函数处理，只需要识别是哪个通道的回调
// udi注册接口限制，不能传入用户args，所以必须注册不同函数来区分
diag_usb_ctrl_s g_usb_ctrl = {
    .chn[USB_CNF_CODER_CHAN] = {
        .devid = UDI_ACM_GPS_ID,
        .enable = cnf_coder_chn_enable,
        .disable = cnf_coder_chn_disable,
        .readcb = cnf_coder_chn_readcb,
        .writecb = cnf_coder_chn_writecb,
        .eventcb = cnf_coder_chn_eventcb,
        .recv_buf_len = PPM_USB_CNF_BUFF_SIZE,
        .recv_buf_num = PPM_USB_CNF_BUFF_CNT,
    },
    .chn[USB_IND_CODER_CHAN] = {
        .devid = UDI_ACM_LTE_DIAG_ID,
        .enable = ind_coder_chn_enable,
        .disable = ind_coder_chn_disable,
        .readcb = ind_coder_chn_readcb,
        .writecb = ind_coder_chn_writecb,
        .eventcb = ind_coder_chn_eventcb,
        .recv_buf_len = PPM_USB_IND_BUFF_SIZE,
        .recv_buf_num = PPM_USB_IND_BUFF_CNT,
    },
    .chn[USB_DEBUGGING_CODER_CHAN] = {
#ifdef BSP_CONFIG_PHONE_TYPE
        .devid = UDI_ACM_CDMA_LOG_ID,
#else
        .devid = UDI_ACM_MPROBE_ID,
#endif
        .enable = debugging_chn_enable,
        .disable = debugging_chn_disable,
        .readcb = debugging_chn_readcb,
        .writecb = debugging_chn_writecb,
        .eventcb = debugging_chn_eventcb,
        .recv_buf_len = PPM_USB_IND_BUFF_SIZE,
        .recv_buf_num = PPM_USB_IND_BUFF_CNT,
    },
};

#define USB_MAX_DATA_LEN (1024 * 1024)
int usb_send(usb_chan_e chn, unsigned char *virt_addr, unsigned char *phy_addr, unsigned int len)
{
    int ret;
    struct acm_wr_async_info wrinfo = {0};
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];
    unsigned long flags;

    if (chn >= USB_PORT_CHAN_BUTT || virt_addr == NULL || phy_addr == NULL) {
        diag_err("usb send para invalid chn:%d\n", chn);
        return -1;
    }

    if (len > USB_MAX_DATA_LEN) {
        len = USB_MAX_DATA_LEN;
    }

    g_diag_dbg.mntn_data.usb_dbg[chn].usb_send_expected_len += len;
    spin_lock_irqsave(&chn_ctrl->lock, flags);
    if (chn_ctrl->state != USB_CONNECT) {
        spin_unlock_irqrestore(&chn_ctrl->lock, flags);
        g_diag_dbg.mntn_data.usb_dbg[chn].usb_send_fail_disconn_len += len;
        return -1;
    }
    spin_unlock_irqrestore(&chn_ctrl->lock, flags);

    wrinfo.virt_addr = (char *)virt_addr;
    wrinfo.phy_addr = (char *)phy_addr;
    wrinfo.size = len;
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_WRITE_ASYNC, &wrinfo);
    if (ret) {
        g_diag_dbg.mntn_data.usb_dbg[chn].usb_send_fail_len += len;
        diag_err("chn %d usb send data fail, len=0x%x ret:%d\n", chn, len, ret);
        return -1;
    }
    g_diag_dbg.mntn_data.usb_dbg[chn].usb_send_len += len;
    return 0;
}

int usb_register_send_cb(usb_chan_e chn, usb_send_cmp_cb cb)
{
    if (chn >= USB_PORT_CHAN_BUTT) {
        diag_err("usb register sendcb fail, chn:%d invalid\n", chn);
        return -1;
    }
    g_usb_ctrl.chn[chn].send_cmp_cb = cb;
    return 0;
}

int usb_register_recv_cb(usb_chan_e chn, usb_recv_cb cb)
{
    if (chn >= USB_PORT_CHAN_BUTT) {
        diag_err("usb register recvcb fail, chn:%d invalid\n", chn);
        return -1;
    }
    g_usb_ctrl.chn[chn].recv_cb = cb;
    return 0;
}

int usb_register_event_cb(usb_chan_e chn, usb_event_cb cb)
{
    if (chn >= USB_PORT_CHAN_BUTT) {
        diag_err("usb register eventcb fail, chn:%d invalid\n", chn);
        return -1;
    }
    g_usb_ctrl.chn[chn].event_notify = cb;
    return 0;
}

int usb_get_state(usb_chan_e chn)
{
    if (chn >= USB_PORT_CHAN_BUTT) {
        diag_err("usb register eventcb fail, chn:%d invalid\n", chn);
        return -1;
    }
    return g_usb_ctrl.chn[chn].state;
}

void udi_chn_enable(usb_chan_e chn)
{
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];
    int ret;
    struct acm_read_buff_info acm_buf = {0};
    struct udi_open_param param;
    unsigned long flags;

    diag_debug("chn %d udi enable begin\n", chn);
    param.devid = chn_ctrl->devid;
    chn_ctrl->fd = (unsigned long long)(uintptr_t)mdrv_udi_open(&param);
    if (chn_ctrl->fd == 0) {
        diag_err("mdrv_udi_open chn %d fail\n", chn);
        return;
    }

    acm_buf.buff_size = chn_ctrl->recv_buf_len;
    acm_buf.buff_num = chn_ctrl->recv_buf_num;
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_RELLOC_READ_BUFF, &acm_buf);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_RELLOC_READ_BUFF fail, ret:%d buff_size: %d, buf_num:%d\n",
            chn, ret, chn_ctrl->recv_buf_len, chn_ctrl->recv_buf_num); // 讨论一下是不是冗余设计:1.homi未做维测，2.当前udi内部实现是参数合法性检查 + ioctrl_cb返回值
        return;
    }
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_SET_READ_CB, chn_ctrl->readcb);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_SET_READ_CB fail, ret:%d\n",chn, ret);
        return;
    }
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_SET_WRITE_CB, chn_ctrl->writecb);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_SET_WRITE_CB fail, ret:%d\n",chn, ret);
        return;
    }
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_SET_EVT_CB, chn_ctrl->eventcb);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_SET_EVT_CB fail, ret:%d\n",chn, ret);
        return;
    }
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_WRITE_DO_COPY, NULL);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_WRITE_DO_COPY fail, ret:%d\n",chn, ret);
        return;
    }

    spin_lock_irqsave(&chn_ctrl->lock, flags);
    chn_ctrl->state = USB_CONNECT;
    spin_unlock_irqrestore(&chn_ctrl->lock, flags);
    if (chn_ctrl->event_notify != NULL) {
        chn_ctrl->event_notify((unsigned int)chn_ctrl->state);
    }

    diag_debug("chn %d udi enable success\n", chn);
    return;
}

void udi_chn_disable(usb_chan_e chn)
{
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];
    unsigned long flags;

    diag_err("chn %d udi disable begin\n", chn);
    if (chn_ctrl->fd != 0) {
        mdrv_udi_close(chn_ctrl->fd);
        chn_ctrl->fd = 0;
    }
    spin_lock_irqsave(&chn_ctrl->lock, flags);
    chn_ctrl->state = USB_DISCONNECT;
    spin_unlock_irqrestore(&chn_ctrl->lock, flags);
    if (chn_ctrl->event_notify != NULL) {
        chn_ctrl->event_notify(chn_ctrl->state);
    }

    diag_err("chn %d udi disable success\n", chn);
    return;
}

void udi_chn_readcb(usb_chan_e chn)
{
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];
    struct acm_wr_async_info wrinfo = {0};
    int ret;

    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_GET_RD_BUFF, &wrinfo);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_GET_RD_BUFF fail, ret:%d\n",chn, ret);
        return;
    }
    if (chn_ctrl->recv_cb != NULL) {
        g_diag_dbg.diag_msp_proc_dbg.usb_dl_cnf_coder_cnt++;
        chn_ctrl->recv_cb(wrinfo.virt_addr, wrinfo.size);
    }
    ret = mdrv_udi_ioctl(chn_ctrl->fd, ACM_IOCTL_RETURN_BUFF, &wrinfo);
    if (ret) {
        diag_err("chn %d ACM_IOCTL_RETURN_BUFF fail, ret:%d\n",chn, ret);
        return;
    }
    return;
}

void udi_chn_writecb(usb_chan_e chn, char *vaddr, int len)
{
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];

    if (chn_ctrl->send_cmp_cb != NULL) {
        g_diag_dbg.mntn_data.usb_dbg[chn].usb_send_complete += len;
        chn_ctrl->send_cmp_cb(vaddr, len);
    }
    return;
}

void udi_chn_eventcb(usb_chan_e chn, ACM_EVT_E state)
{
    usb_port_chn_ctrl_s *chn_ctrl = &g_usb_ctrl.chn[chn];
    int change_flag = 1;
    unsigned long flags;

    switch (state) {
        case ACM_EVT_DEV_SUSPEND:
            spin_lock_irqsave(&chn_ctrl->lock, flags);
            chn_ctrl->state = USB_DISCONNECT;
            spin_unlock_irqrestore(&chn_ctrl->lock, flags);
            break;
        case ACM_EVT_DEV_READY:
            spin_lock_irqsave(&chn_ctrl->lock, flags);
            chn_ctrl->state = USB_CONNECT;
            spin_unlock_irqrestore(&chn_ctrl->lock, flags);
            break;
        default:
            change_flag = 0;
            diag_err("chn 0x%x udi invalid stat: 0x%x\n", chn, state);
    }
    if ((chn_ctrl->event_notify != NULL) && (change_flag == 1)) {
        chn_ctrl->event_notify(chn_ctrl->state);
        diag_err("chn 0x%x udi event cb begin, end: 0x%x\n", chn, state);
    }
    return;
}

void cnf_coder_chn_enable(void)
{
    return udi_chn_enable(USB_CNF_CODER_CHAN);
}

void cnf_coder_chn_disable(void)
{
    return udi_chn_disable(USB_CNF_CODER_CHAN);
}

void cnf_coder_chn_readcb(void)
{
    return udi_chn_readcb(USB_CNF_CODER_CHAN);
}

void cnf_coder_chn_writecb(char *vaddr, char *paddr, int len)
{
    return udi_chn_writecb(USB_CNF_CODER_CHAN, vaddr, len);
}

void cnf_coder_chn_eventcb(ACM_EVT_E state)
{
    return udi_chn_eventcb(USB_CNF_CODER_CHAN, state);
}

void ind_coder_chn_enable(void)
{
    return udi_chn_enable(USB_IND_CODER_CHAN);
}

void ind_coder_chn_disable(void)
{
    return udi_chn_disable(USB_IND_CODER_CHAN);
}

void ind_coder_chn_readcb(void)
{
    return udi_chn_readcb(USB_IND_CODER_CHAN);
}

void ind_coder_chn_writecb(char *vaddr, char *paddr, int len)
{
    return udi_chn_writecb(USB_IND_CODER_CHAN, vaddr, len);
}

void ind_coder_chn_eventcb(ACM_EVT_E state)
{
    return udi_chn_eventcb(USB_IND_CODER_CHAN, state);
}

void debugging_chn_enable(void)
{
    return udi_chn_enable(USB_DEBUGGING_CODER_CHAN);
}

void debugging_chn_disable(void)
{
    return udi_chn_disable(USB_DEBUGGING_CODER_CHAN);
}

void debugging_chn_readcb(void)
{
    return udi_chn_readcb(USB_DEBUGGING_CODER_CHAN);
}

void debugging_chn_writecb(char *vaddr, char *paddr, int len)
{
    return udi_chn_writecb(USB_DEBUGGING_CODER_CHAN, vaddr, len);
}

void debugging_chn_eventcb(ACM_EVT_E state)
{
    return udi_chn_eventcb(USB_DEBUGGING_CODER_CHAN, state);
}

int usb_init(void)
{
    int ret, i;
    diag_usb_ctrl_s *ctrl = &g_usb_ctrl;

    for (i = USB_CNF_CODER_CHAN; i < USB_PORT_CHAN_BUTT; i++) {
        spin_lock_init(&ctrl->chn[i].lock);
    }

    for (i = USB_CNF_CODER_CHAN; i < USB_PORT_CHAN_BUTT; i++) {
        ret = mdrv_usb_reg_enablecb(ctrl->chn[i].enable);
        if (ret) {
            diag_err("usb reg enablecb fail, ret: %d\n", ret);
            return -1;
        }
        ret = mdrv_usb_reg_disablecb(ctrl->chn[i].disable);
        if (ret) {
            diag_err("usb reg disablecb fail, ret: %d\n", ret);
            return -1;
        }
    }
    diag_err("usb init success\n");
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
