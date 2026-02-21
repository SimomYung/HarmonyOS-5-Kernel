/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
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

#include <linux/slab.h>
#include <linux/init.h>
#include "bsp_relay.h"
#include <securec.h>
#include <bsp_print.h>
#include <product_config.h>
#include "vdev.h"

#define THIS_MODU mod_vcom

void vcom_relay_acm_write_donecb(char *virt_addr, char *phy_addr, int size)
{
    struct vcom_buf *vb = NULL;
    struct vcom_hd *hd = NULL;

    vb = (struct vcom_buf *)(virt_addr - sizeof(struct vcom_buf));
    if (vb->dev_type > VCOM_DEV_MAX || vb->magic != VCOM_BUF_MAGIC) {
        VCOM_PRINT_ERROR("buf is err dev_type:%u, magic:%u\n", vb->dev_type, vb->magic);
        return;
    }

    hd = vcom_get_hd(vb->id);
    if (hd == NULL) {
        VCOM_PRINT_ERROR("hd is NULL\n");
        return;
    }

    kfree(vb);
    hd->map->stax.succ_free++;
}

int vcom_relay_acm_read_cb(void *private, unsigned int type, void *addr, int size)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;
    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("vdev:%s, size:%d\n", hd->name, size);
    if (addr == NULL) {
        VCOM_PRINT("addr is NULL\n");
        return -1;
    }

    switch (type) {
        case RELAY_ACM_ACK_ASYNC:
            vcom_write(hd, addr, size, WRITE_ASYNC);
            break;
        case RELAY_ACM_NOTIFY_READY:
            vcom_write(hd, addr, size, NOTIFY_READY);
            break;
        case RELAY_ACM_ACK_MSC:
            vcom_write(hd, addr, size, WRITE_MSC);
            break;
        case RELAY_ACM_NOTIFY_SWITCH_MODE:
            vcom_write(hd, addr, size, NOTIFY_ESCAPE);
            break;
        default:
            return -1;
    }
    return 0;
}

void vcom_relay_acm_event_cb(void *private, enum relay_evt port_state)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;
    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("vdev:%s, relay_evt:%d\n", hd->name, port_state);

    vcom_write(hd, &port_state, sizeof(port_state), NOTIFY_READY);
}

void vcom_relay_acm_enable_cb(struct vcom_hd *hd)
{
    int state = VCOM_DEV_READY;

    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("acm dev:%s\n", hd->name);
    bsp_relay_open(hd->relay_type);
    bsp_relay_ioctl(hd->relay_type, RELAY_ACM_REG_WRITE_CB,
                    vcom_relay_acm_write_donecb, sizeof(relay_acm_write_donecb));
    bsp_relay_ioctl(hd->relay_type, RELAY_ACM_REG_READ_CB,
                    vcom_relay_acm_read_cb, sizeof(relay_acm_read_cb));
    vcom_write(hd, &state, sizeof(state), NOTIFY_READY);
}

int vcom_acm_op(struct vcom_buf *vb)
{
    int ret = -1;
    struct vcom_hd *hd = NULL;

    VCOM_TRACE(g_vcom_debug);
    hd = vcom_get_hd(vb->id);

    VCOM_PRINT("vdev:%s\n", hd->name);

    switch (vb->payload_type) {
        case WRITE_ASYNC: {
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_ACM_WRITE_ASYNC, (void*)vb->data, vb->payload_len);
            if (ret < 0) {
                kfree(vb);
                hd->map->stax.fail_free++;
            }
            return 0;
        }
        case QUERY_READY:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_ACM_QUERY_READY, (void*)vb->data, vb->payload_len);
            break;
        case SET_TRANS_MODE:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_ACM_SET_TRANS_MODE, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_MSC:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_ACM_WRITE_MSC, (void*)vb->data, vb->payload_len);
            break;
        case ENABLE_ESCAPE_DETECT:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_ACM_ENABLE_ESCAPE_DETECT, (void*)vb->data, vb->payload_len);
            break;
        default:
            VCOM_PRINT_ERROR("payload type error\n");
    }

    if (ret < 0) {
        hd->map->stax.fail_free++;
    } else {
        hd->map->stax.succ_free++;
    }
    kfree(vb);
    return 0;
}

#if defined(CONFIG_DIALUP_UART) || defined(CONFIG_DIALUP_PL011_UART)
void vcom_relay_uart_write_donecb(char *addr)
{
    struct vcom_buf *vb = NULL;
    struct vcom_hd *hd = NULL;

    vb = (struct vcom_buf *)(addr - sizeof(struct vcom_buf));
    if (vb->dev_type > VCOM_DEV_MAX || vb->magic != VCOM_BUF_MAGIC) {
        VCOM_PRINT_ERROR("buf is err dev_type:%u, magic:%u\n", vb->dev_type, vb->magic);
        return;
    }

    hd = vcom_get_hd(vb->id);
    if (hd == NULL) {
        VCOM_PRINT_ERROR("hd is NULL\n");
        return;
    }

    kfree(vb);
    hd->map->stax.succ_free++;
}

int vcom_relay_uart_read_cb(void *private, unsigned int type, void *addr, int size)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;
    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("vdev:%s, size:%d\n", hd->name, size);
    if (addr == NULL) {
        VCOM_PRINT("addr is NULL\n");
        return -1;
    }

    switch (type) {
        case RELAY_UART_ACK_ASYNC:
            vcom_write(hd, addr, size, WRITE_ASYNC);
            break;
        case RELAY_UART_ACK_MSC:
            vcom_write(hd, addr, size, WRITE_MSC);
            break;
        case RELAY_UART_NOTIFY_READY:
            vcom_write(hd, addr, size, NOTIFY_READY);
            break;
        case RELAY_UART_NOTIFY_SWITCH_MODE:
            vcom_write(hd, addr, size, NOTIFY_ESCAPE);
            break;
        case RELAY_UART_NOTIFY_WATER_LINE:
            vcom_write(hd, addr, size, NOTIFY_WATER_LINE);
            break;
        default:
            return -1;
    }
    return 0;
}

void vcom_relay_uart_enable_cb(struct vcom_hd *hd)
{
    int state = VCOM_DEV_READY;

    VCOM_TRACE(g_vcom_debug);
    bsp_err("acm dev:%s\n", hd->name);
    bsp_relay_open(hd->relay_type);
    bsp_relay_ioctl(hd->relay_type, RELAY_UART_REG_WRITE_CB,
                    vcom_relay_uart_write_donecb, sizeof(mci_uart_free_cb));
    bsp_relay_ioctl(hd->relay_type, RELAY_UART_REG_READ_CB,
                    vcom_relay_uart_read_cb, sizeof(mci_uart_read_cb));
    vcom_write(hd, &state, sizeof(state), NOTIFY_READY);
}

int vcom_uart_op(struct vcom_buf *vb)
{
    int ret;
    struct vcom_hd *hd = NULL;

    hd = vcom_get_hd(vb->id);

    switch (vb->payload_type) {
        case WRITE_ASYNC: {
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_ASYNC, (void*)vb->data, vb->payload_len);
            if (ret < 0) {
                kfree(vb);
                hd->map->stax.fail_free++;
            }
            return 0;
        }
        case WRITE_FLOW_CTRL:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_FLOW_CTRL, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_STP:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_STP, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_MSC:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_MSC, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_EPS:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_EPS, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_WLEN:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_WLEN, (void*)vb->data, vb->payload_len);
            break;
        case WRITE_BAUD:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_WRITE_BAUD, (void*)vb->data, vb->payload_len);
            break;
        case QUERY_READY:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_QUERY_READY, (void*)vb->data, vb->payload_len);
            break;
        case ENABLE_CMUX:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_ENABLE_CMUX, (void*)vb->data, vb->payload_len);
            break;
        case SET_CMUX_INFO:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_SET_CMUX_INFO, (void*)vb->data, vb->payload_len);
            break;
        case SET_CMUX_DLC:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_SET_CMUX_DLC, (void*)vb->data, vb->payload_len);
            break;
        case SET_TRANS_MODE:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_SET_TRANS_MODE, (void*)vb->data, vb->payload_len);
            break;
        case ENABLE_ESCAPE_DETECT:
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_UART_ENABLE_ESCAPE_DETECT, (void*)vb->data, vb->payload_len);
            break;
        default:
            ret = -1;
    }

    if (ret < 0) {
        hd->map->stax.fail_free++;
    } else {
        hd->map->stax.succ_free++;
    }
    kfree(vb);
    return 0;
}
#endif
#ifdef CONFIG_IOT_GNET
int vcom_relay_net_read_cb(void *private, unsigned int type,
    void *buf, unsigned int len)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;
    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("vdev:%s, len:%d\n", hd->name, len);
    if (buf == NULL) {
        VCOM_PRINT("buf is NULL\n");
        return -1;
    }

    switch (type) {
        case RELAY_NET_AT_RECV: {
            vcom_write(hd, buf, len, WRITE_ASYNC);
            break;
        }
        case RELAY_NET_STAT_CHG: {
            vcom_write(hd, buf, len, NET_STAT_CHG);
            break;
        }
        case RELAY_NET_ACK_CFG: {
            vcom_write(hd, buf, len, NET_ACK_CFG);
            break;
        }
        case RELAY_NET_NOTIFY_READY: {
            vcom_write(hd, buf, len, NOTIFY_READY);
            break;
        }
        default: {
            return -1;
        }
    }
    return 0;
}

void vcom_relay_net_enable_cb(struct vcom_hd *hd)
{
    int state = VCOM_DEV_READY;

    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("net dev:%s\n", hd->name);
    bsp_relay_open(hd->relay_type);
    bsp_relay_ioctl(hd->relay_type, RELAY_NET_REG_READ_CB,
                    vcom_relay_net_read_cb, sizeof(relay_net_read_cb));
    vcom_write(hd, &state, sizeof(state), NOTIFY_READY);
}

int vcom_net_op(struct vcom_buf *vb)
{
    int ret;
    struct vcom_hd *hd = NULL;

    VCOM_TRACE(g_vcom_debug);
    hd = vcom_get_hd(vb->id);

    switch (vb->payload_type) {
        case NET_WRITE_CFG: {
            ret= bsp_relay_ioctl(hd->relay_type, RELAY_NET_WRITE_CFG, (void*)vb->data, vb->payload_len);
            break;
        }
        case WRITE_ASYNC: {
            ret= bsp_relay_ioctl(hd->relay_type, RELAY_NET_AT_RESPONSE, (void*)vb->data, vb->payload_len);
            break;
        }
        case QUERY_READY: {
            ret = bsp_relay_ioctl(hd->relay_type, RELAY_NET_QUERY_READY, (void*)vb->data, vb->payload_len);
            break;
        }
        default:
            ret = -1;
    }

    if (ret) {
        hd->map->stax.fail_free++;
    } else {
        hd->map->stax.succ_free++;
    }

    kfree(vb);
    return 0;
}
#endif

void vcom_relay_enable_cb(void *private)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;

#ifdef CONFIG_PCDEV_UDC
    if (hd->dev_type == VCOM_DEV_ACM || hd->dev_type == VCOM_DEV_PCIE_VDEV) {
#else
    if (hd->dev_type == VCOM_DEV_ACM) {
#endif
        vcom_relay_acm_enable_cb(hd);
    } else if (hd->dev_type == VCOM_DEV_HSUART) {
#if defined(CONFIG_DIALUP_UART) || defined(CONFIG_DIALUP_PL011_UART)
        vcom_relay_uart_enable_cb(hd);
#endif
    }
#ifdef CONFIG_IOT_GNET
     else if (hd->dev_type == VCOM_DEV_UNET_AT || hd->dev_type == VCOM_DEV_NETIF_CTRL) {
        vcom_relay_net_enable_cb(hd);
    }
#endif
    return;
}

void vcom_relay_reg_disablecb(void *private)
{
    struct vcom_hd *hd = (struct vcom_hd *)private;
    int state = VCOM_DEV_NOT_READY;

    VCOM_TRACE(g_vcom_debug);
    vcom_write(hd, &state, sizeof(state), NOTIFY_READY);
    bsp_relay_close(hd->relay_type);
}

int vcom_hd_relay_init(struct vcom_hd *hd)
{
    VCOM_TRACE(g_vcom_debug);
    VCOM_PRINT("vdev:%s, relay_type:%d\n", hd->name, hd->relay_type);
    bsp_relay_reg_enablecb(hd->relay_type, vcom_relay_enable_cb, hd);
    bsp_relay_reg_disablecb(hd->relay_type, vcom_relay_reg_disablecb);
    return 0;
}
