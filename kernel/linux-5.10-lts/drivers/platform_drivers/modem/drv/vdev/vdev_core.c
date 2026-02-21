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
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <securec.h>
#include <bsp_relay.h>
#include <bsp_print.h>
#include <mdrv_usb.h>

#ifdef CONFIG_MCI_CUST_CHN
#include <nva_stru.h>
#include <nva_id.h>
#include <bsp_nva.h>
#endif

#include "vdev.h"

#define THIS_MODU mod_vcom

#define VDEV_PRINT_SIZE_MAX 0x400

#ifdef CONFIG_MCI_CUST_CHN
#define BIT_NUM_OF_BYTE 8
#define VCOM_APP_CUST_CHN_ID_MIN VCOM_APP_1
#define VCOM_APP_CUST_CHN_ID_MAX (VCOM_CHN_BOTTOM - 1)
#endif

struct vcom_map_s *g_vdev_map = NULL;
struct vcom_hd **g_vcom_id2hd_map;
static struct vcom_debug_stax g_vcom_stax;
static bool g_vcom_wakeup_flag = false;

int g_vcom_debug = 0;
void vcom_debug_on(int on)
{
    g_vcom_debug = on;
}

struct vcom_map_s *vcom_get_map(unsigned int id)
{
    return g_vcom_id2hd_map[id]->map;
}

static struct vcom_hd g_vcom_hd_map[] = {
    {
        .name = "appvcom", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_0, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom1", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_1, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
#ifdef CONFIG_VDEV_PHONE
    {
        .name = "appvcom2", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_2, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom3", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_3, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom4", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_4, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom5", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_5, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom6", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_6, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom7", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_7, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom8", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_8, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom9", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_9, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom10", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_10, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom11", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_11, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom12", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_12, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom13", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_13, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom14", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_14, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom15", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_15, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom16", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_16, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom17", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_17, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom18", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_18, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom19", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_19, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom20", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_20, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom21", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_21, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom22", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_22, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom23", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_23, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom24", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_24, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom25", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_25, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom26", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_26, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom27", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_27, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom28", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_28, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom29", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_29, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom30", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_30, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom31", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_31, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom32", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_32, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 1,
    },
    {
        .name = "appvcom33", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_33, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom34", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_34, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom35", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_35, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom36", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_36, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom37", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_37, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom38", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_38, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom39", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_39, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom40", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_40, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom41", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_41, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom42", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_42, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom43", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_43, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom44", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_44, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom45", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_45, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom46", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_46, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom47", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_47, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom48", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_48, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom49", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_49, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom50", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_50, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom51", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_51, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcom52", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_52, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcomERRLOG", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_53, .init = vcom_hd_app_init, .enable = 0, .dump_enable = 0,
    },
    {
        .name = "appvcomTLLOG", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_54, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcomLOG", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_55, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "appvcomAMC", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_56, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "act", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_ACT, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "nmctrlvcom", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_NMCTRL, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "bastet_modem", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_APP_BASTET, .init = vcom_hd_app_init, .enable = 0, .dump_enable = 0,
    },
#endif
    {
        .name = "at_pass", .dev_type = VCOM_DEV_APP,
        .dev_id = VCOM_ATP_ID, .init = vcom_hd_app_init, .enable = 1, .dump_enable = 0,
    },
    {
        .name = "acm_ctrl", .dev_type = VCOM_DEV_ACM, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_ACM_CTRL, .relay_type = RELAY_ACM_DEV_CTRL_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "acm_pcui", .dev_type = VCOM_DEV_ACM, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_ACM_AT, .relay_type = RELAY_ACM_DEV_AT_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "acm_om", .dev_type = VCOM_DEV_ACM, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_ACM_OM, .relay_type = RELAY_ACM_DEV_OM_TYPE, .init = vcom_hd_relay_init,
    },
#ifdef CONFIG_USB_IOT_UDC
    {
        .name = "acm_modem", .dev_type = VCOM_DEV_ACM, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_ACM_MODEM, .relay_type = RELAY_ACM_DEV_MODEM_TYPE, .init = vcom_hd_relay_init,
    },
#endif
    {
        .name = "acm_skytone", .dev_type = VCOM_DEV_ACM, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_ACM_SKYTONE, .relay_type = RELAY_ACM_DEV_SKYTONE_TYPE, .init = vcom_hd_relay_init,
    },
#if defined(CONFIG_DIALUP_UART) || defined(CONFIG_DIALUP_PL011_UART)
    {
        .name = "uart_hsuart", .dev_type = VCOM_DEV_HSUART, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_HSUART_ID, .relay_type = RELAY_UART_DEV_HSUART_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "uart_cmux0", .dev_type = VCOM_DEV_HSUART, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CMUX0_ID, .relay_type = RELAY_UART_DEV_CMUX0_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "uart_cmux1", .dev_type = VCOM_DEV_HSUART, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CMUX1_ID, .relay_type = RELAY_UART_DEV_CMUX1_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "uart_cmux2", .dev_type = VCOM_DEV_HSUART, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CMUX2_ID, .relay_type = RELAY_UART_DEV_CMUX2_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "uart_cmux3", .dev_type = VCOM_DEV_HSUART, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CMUX3_ID, .relay_type = RELAY_UART_DEV_CMUX3_TYPE, .init = vcom_hd_relay_init,
    },
#endif
    {
        .name = "pcie_at", .dev_type = VCOM_DEV_PCIE_VDEV, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CHN_PCIE_AT, .relay_type = RELAY_PCIE_DEV_AT_TYPE, .init = vcom_hd_relay_init,
    },
#ifdef CONFIG_IOT_GNET
    {
        .name = "pnet_ctrl", .dev_type = VCOM_DEV_NETIF_CTRL, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CHN_PNET_CTRL, .relay_type = RELAY_NET_DEV_PCIE_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "unet_at", .dev_type = VCOM_DEV_UNET_AT, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CHN_UNET_AT, .relay_type = RELAY_UNET_DEV_AT_TYPE, .init = vcom_hd_relay_init,
    },
    {
        .name = "unet_ctrl", .dev_type = VCOM_DEV_NETIF_CTRL, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CHN_UNET_CTRL, .relay_type = RELAY_NET_DEV_USB_TYPE, .init = vcom_hd_relay_init,
    },
#endif
#ifdef CONFIG_MCI_SOCKET
    {
        .name = "socket at", .dev_type = VCOM_DEV_SOCK, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_SOCK_ID, .relay_type = MCI_SOCKET_AT, .init = vcom_socket_ready_init,
    },
    {
        .name = "socket om", .dev_type = VCOM_DEV_SOCK, .enable = 1, .dump_enable = 0,
        .dev_id = VCOM_CHN_SOCKET_OM, .relay_type = MCI_SOCKET_OM, .init = vcom_socket_ready_init,
    },
#endif
#ifdef CONFIG_VDEV_DEBUG
    {
        .name = "appvcomtest", .dev_type = VCOM_DEV_APP, .dump_enable = 0,
        .dev_id = VCOM_APP_TEST, .enable = 1, .init = vcom_hd_app_init,
    },
#endif
};

static struct vcom_hd g_vcom_hd_ext_map[] = {
    {.name = "getslice", .init = vcom_hd_proc_init, },
#ifdef CONFIG_MCI_ATP
    {.name = "atp", .init = mci_atp_init, },
#endif
#ifdef CONFIG_VDEV_PHONE
    {.name = "bind_pid", .init = vcom_hd_bindpid_init, },
#endif
};

void vcom_print_msg(unsigned char *base, int size)
{
    int ret;
    unsigned char *str = NULL;

    if (!g_vcom_debug) {
        return;
    }

    if (size > VDEV_PRINT_SIZE_MAX) {
        return;
    }

    str = kmalloc(size + 1, GFP_KERNEL);
    if (str == NULL) {
        return;
    }

    str[size] = '\0';
    ret = memcpy_s(str, size + 1, base, size);
    if (!ret) {
        bsp_err("%s\n", str);
    }
    kfree(str);
    str = 0;
}

void vcom_print_vb(struct vcom_buf *vb)
{
    if (!g_vcom_debug) {
        return;
    }

    bsp_err("id: %d\n", vb->id);
    bsp_err("size:%d\n", vb->payload_len);
    bsp_err("type:%d\n", vb->payload_type);
    vcom_print_msg(vb->data, vb->payload_len);
}

struct vcom_hd *vcom_get_hd(unsigned int id)
{
    if (id >= VCOM_CHN_BOTTOM) {
        return NULL;
    }
    return g_vcom_id2hd_map[id];
}

struct vcom_hd *vcom_get_hd_from_set(unsigned int index)
{
    return &g_vcom_hd_map[index];
}

unsigned int vcom_get_hd_num(void)
{
    return sizeof(g_vcom_hd_map) / sizeof(*g_vcom_hd_map);
}

struct vcom_debug_stax *vcom_get_debug_stax(void)
{
    return &g_vcom_stax;
}

void mci_write_record(struct vcom_map_s *map, struct vcom_buf *vb, unsigned int size)
{
    unsigned int record_size;

    record_size = size > MCI_RECORD_LEN - 1 ? MCI_RECORD_LEN - 1 : size;
    if (memcpy_s(map->stax.write_record, MCI_RECORD_LEN - 1, vb->data, record_size) != EOK) {
        map->stax.write_size = 0;
        VCOM_PRINT_ERROR("write record fail\n");
    } else {
        map->stax.write_size = record_size;
    }
}

struct vcom_buf *vcom_build_write_buffer(struct vcom_hd *hd, unsigned int total_len, unsigned int size, unsigned int type)
{
    struct vcom_buf *vb = NULL;

    vb = (struct vcom_buf *)kmalloc(total_len, GFP_KERNEL);
    if (vb == NULL) {
        VCOM_PRINT_ERROR("kmalloc failed\n");
        return NULL;
    }

    vb->id = hd->map->id;
    vb->dev_type = hd->dev_type;
    vb->payload_start = 0;
    vb->payload_len = size;
    vb->payload_type = type;
    return vb;
}

int vcom_ioctl(struct vcom_hd *hd, const void *buf, unsigned int size, unsigned int type)
{
    int ret;
    unsigned int total_len;
    struct vcom_buf *vb = NULL;
    struct vcom_map_s *map = NULL;

    map = hd->map;
#ifdef CONFIG_MCI_CUST_CHN
    if (map->ccore_ready == 0 && type == WRITE_ASYNC) {
#else
    if (map->ccore_ready == 0) {
#endif
        VCOM_PRINT_ERROR("channel %s type %d, size %d, ccore not ready\n", hd->name, type, size);
        return -1;
    }

    total_len = sizeof(struct vcom_buf) + size;
    vb = vcom_build_write_buffer(hd, total_len, size, type);
    if (vb == NULL) {
        return -ENOMEM;
    }

    ret = memcpy_s(vb->data, vb->payload_len, buf, size);
    if (ret) {
        kfree(vb);
        return -EIO;
    }

    ret = map->link_ops->write(map, (unsigned char *)vb, total_len);
    return ret;
}

int mci_msg_send(struct vcom_hd *hd, struct vcom_buf *vb)
{
    int ret;
    struct vcom_map_s *map = hd->map;
    unsigned int total_len = sizeof(struct vcom_buf) + vb->payload_len;

    mci_write_record(map, vb, vb->payload_len);
    ret = map->link_ops->write(map, (unsigned char *)vb, total_len);
    mci_debug_record(hd, vb->data, vb->payload_len, VCOM_KERNEL_TYPE, ret == 0 ? 1 : 0);
    return ret;
}

int vcom_write(struct vcom_hd *hd, const void *buf, unsigned int size, unsigned int type)
{
    int ret;
    struct vcom_buf *vb = NULL;
    unsigned int total_len;
    struct vcom_map_s *map = NULL;
    struct vcom_debug_stax *debug = vcom_get_debug_stax();

    VCOM_TRACE(g_vcom_debug);

    if (size > VCOM_MAX_DATA_SIZE) {
        VCOM_PRINT_ERROR("channel %s, size is too large, size = %d\n", hd->name, size);
        return -1;
    }

    map = hd->map;
#ifdef CONFIG_MCI_CUST_CHN
    if (map->ccore_ready == 0 && type == WRITE_ASYNC) {
#else
    if (map->ccore_ready == 0) {
#endif
        map->stax.write_drop++;
        return -1;
    }


    total_len = sizeof(struct vcom_buf) + size;
    vb = vcom_build_write_buffer(hd, total_len, size, type);
    if (vb == NULL) {
        return -ENOMEM;
    }

    debug->up_kmalloc++;

    if (hd->dev_type == VCOM_DEV_APP && type == WRITE_ASYNC) {
        if (copy_from_user(vb->data, buf, size)) {
            kfree(vb);
            debug->up_kfree++;
            return -EIO;
        }
    } else {
        if (EOK != memcpy_s(vb->data, vb->payload_len, buf, size)) {
            kfree(vb);
            debug->up_kfree++;
            return -EIO;
        }
    }

#ifdef CONFIG_MCI_SWITCH
    if (mci_switch_te_vb_receive(hd, vb) == 0) {
        return 0;
    }
#endif

    ret = mci_msg_send(hd, vb);
    kfree(vb);
    debug->up_kfree++;
    return ret;
}

unsigned char *vcom_read(struct vcom_hd *hd, unsigned int *size)
{
    struct vcom_buf *vb = NULL;
    VCOM_TRACE(g_vcom_debug);
    vb = (struct vcom_buf *)hd->map->link_ops->dequeue(hd->map);
    if (vb == NULL) {
        *size = 0;
        return NULL;
    }
    *size = vb->payload_len;
    return (unsigned char *)vb;
}

int vcom_requeue(struct vcom_hd *hd, struct vcom_buf *vb)
{
    return hd->map->link_ops->queue_head(hd->map, (unsigned char *)vb);
}

int vcom_msg_check(unsigned char *buf, unsigned int total_len)
{
    struct vcom_buf *vb = NULL;

    if (total_len < sizeof(struct vcom_buf)) {
        VCOM_PRINT_ERROR("buf len is error\n");
        return -1;
    }

    vb = (struct vcom_buf *)buf;
    if (vb->payload_len + sizeof(struct vcom_buf) != total_len) {
        VCOM_PRINT_ERROR("msg length check fail\n");
        return -1;
    }

    return 0;
}

int vcom_nortify(struct vcom_buf *vb)
{
    struct vcom_hd *hd = NULL;
    struct vcom_buf vb_send = {0};
    unsigned int i;
    unsigned int hd_num;

    vb_send.magic = VCOM_BUF_MAGIC;
    vb_send.dev_type = VCOM_DEV_NORTIFY;

    hd_num = vcom_get_hd_num();
    for (i = 0; i < hd_num; i++) {
        hd = vcom_get_hd_from_set(i);
        if (hd->enable == 1) {
#ifndef CONFIG_MCI_CUST_CHN
            hd->map->ccore_ready = 1;
#endif
            vb_send.id = hd->map->id;
            hd->map->link_ops->write(hd->map, (unsigned char *)&vb_send, sizeof(struct vcom_buf));
        }
    }

    VCOM_PRINT("recv notify init done\n");
    kfree(vb);
    return 0;
}

#ifdef CONFIG_MCI_CUST_CHN
static int vcom_nortify_recv_ready(struct vcom_buf *vb)
{
    struct vcom_hd *hd = NULL;

    hd = vcom_get_hd(vb->id);
    if (hd->enable == 1) {
        hd->map->ccore_ready = 1;
    }

    VCOM_PRINT("recv notify ready chan %d\n", vb->id);
    kfree(vb);
    return 0;
}
#endif

int vcom_nortify_op(struct vcom_buf *vb)
{
#ifdef CONFIG_MCI_CUST_CHN
    if (vb->payload_type == INIT_DONE) {
        return vcom_nortify(vb);
    } else if (vb->payload_type == NOTIFY_READY) {
        return vcom_nortify_recv_ready(vb);
#else
    if (vb->payload_type == NOTIFY_READY) {
            return vcom_nortify(vb);
#endif
    } else if (vb->payload_type == CHR_REQ) {
        return vcom_app_chr_report(vb);
    } else {
        VCOM_PRINT_ERROR("payload type error\n");
        return -1;
    }
}

void mci_read_record(struct vcom_buf *vb)
{
    struct vcom_map_s *map = NULL;
    unsigned int record_size;

    map = vcom_get_map(vb->id);
    record_size = vb->payload_len > MCI_RECORD_LEN - 1 ? MCI_RECORD_LEN - 1 : vb->payload_len;
    if (memcpy_s(map->stax.read_record, MCI_RECORD_LEN - 1, vb->data, record_size) != EOK) {
        map->stax.read_size = 0;
        VCOM_PRINT_ERROR("write record fail\n");
    } else {
        map->stax.read_size = record_size;
    }
}

void vcom_msg_parse_debug(struct vcom_hd *hd, struct vcom_buf *vb)
{
    hd->wakeup = g_vcom_wakeup_flag ? true : false;
    g_vcom_wakeup_flag = false;

    vcom_print_vb(vb);
    mci_read_record(vb);
    mci_debug_record(hd, vb->data, vb->payload_len, VCOM_KERNEL_TYPE, 1);
}

int mci_msg_dispatch(struct vcom_buf *vb)
{
    struct vcom_debug_stax *debug = vcom_get_debug_stax();

    vb->magic = VCOM_BUF_MAGIC;
    switch (vb->dev_type) {
        case VCOM_DEV_APP:
            debug->parse_app++;
            return vcom_app_op(vb);
#if defined(CONFIG_DIALUP_UART) || defined(CONFIG_DIALUP_PL011_UART)
        case VCOM_DEV_HSUART:
            debug->parse_uart++;
            return vcom_uart_op(vb);
#endif
        case VCOM_DEV_PCIE_VDEV:
            debug->parse_pcdev++;
            /* fall-through */
        case VCOM_DEV_ACM:
            debug->parse_acm++;
            return vcom_acm_op(vb);
#ifdef CONFIG_IOT_GNET
        case VCOM_DEV_NETIF_CTRL:
        case VCOM_DEV_UNET_AT:
            debug->parse_net_ctrl++;
            return vcom_net_op(vb);
#endif
        case VCOM_DEV_SOCK:
            debug->parse_socket++;
            return vcom_socket_op(vb);
        default:
            debug->parse_err++;
            VCOM_PRINT_ERROR("unsuport dev type: %d\n", vb->dev_type);
            return -1;
    }
}

int vcom_msg_parse(unsigned char *buf, unsigned int len)
{
    struct vcom_buf *vb = NULL;
    struct vcom_hd *hd = NULL;
    struct vcom_debug_stax *debug = vcom_get_debug_stax();

    VCOM_TRACE(g_vcom_debug);

    if (vcom_msg_check(buf, len) < 0) {
        debug->check_fail++;
        return -1;
    }

    vb = (struct vcom_buf *)buf;
    if ((hd = vcom_get_hd(vb->id)) == NULL) {
        kfree(buf);
        return 0;
    }

    vcom_msg_parse_debug(hd, vb);
    if (vb->dev_type == VCOM_DEV_NORTIFY) {
        debug->parse_notify++;
        return vcom_nortify_op(vb);
    }

#ifdef CONFIG_MCI_SWITCH
    if (mci_switch_mt_vb_receive(hd, vb) == 0) {
        return 0;
    }
#endif

    return mci_msg_dispatch(vb);
}

void vcom_wakeup_notify(void)
{
    g_vcom_wakeup_flag = true;
}

void vcom_map_init(struct vcom_map_s *map, struct vcom_hd *hd)
{
    VCOM_TRACE(g_vcom_debug);
    memset_s(map, sizeof(struct vcom_map_s), 0, sizeof(struct vcom_map_s));
    INIT_LIST_HEAD(&map->high_list);
    map->high_list_len = 0;
    INIT_LIST_HEAD(&map->low_list);
    spin_lock_init(&map->link_lock);
    map->link_ops = &g_vcom_link_list_ops;
    map->id = hd->dev_id;
    map->hd = hd;
    hd->map = map;
    hd->dump_index = 0;
    map->link_ops->init(map);
    if (hd->init) {
        hd->init(hd);
    }
}

#ifdef CONFIG_MCI_CUST_CHN
static int mci_nv_cust_at_chn_init(nv_mci_chn_cust_feature_s *cust_at_chn)
{
    int ret;

    ret = bsp_nv_read(NVA_ID_DRV_MCI_CHN_CUST_FEATURE, (unsigned char *)cust_at_chn, sizeof(nv_mci_chn_cust_feature_s));
    if (ret) {
        VCOM_PRINT_ERROR("nv at chn cust read fail, app chn closed\n");
        ret = memset_s(cust_at_chn, sizeof(nv_mci_chn_cust_feature_s), 0, sizeof(nv_mci_chn_cust_feature_s));
    }

    return ret;
}

static int mci_get_at_chn_state(int chn_id, nv_mci_chn_cust_feature_s *cust_chn)
{
    int offset_byte;
    int offset_bit;

    offset_byte = chn_id / BIT_NUM_OF_BYTE;
    offset_bit = chn_id - offset_byte * BIT_NUM_OF_BYTE;
    return (cust_chn->chn_state[offset_byte] >> offset_bit) & 0x1;
}
#endif

static int vdev_init(void)
{
    int i;
    int dev_id;
#ifdef CONFIG_MCI_CUST_CHN
    nv_mci_chn_cust_feature_s cust_at_chn;
#endif
    bsp_err("[init] start\n");

#ifdef CONFIG_MCI_CUST_CHN
    if (mci_nv_cust_at_chn_init(&cust_at_chn)) {
        return -1;
    }
#endif

#ifdef CONFIG_MCI_SWITCH
    if (mci_switch_init()) {
        return -1;
    }
#endif

    g_vdev_map = kzalloc(sizeof(struct vcom_map_s) * (sizeof(g_vcom_hd_map) / sizeof(g_vcom_hd_map[0])), GFP_KERNEL);
    if (g_vdev_map == NULL) {
        return -1;
    }

    vdev_cdev_init();
    g_vcom_id2hd_map = kzalloc(sizeof(struct vcom_hd *) * VCOM_CHN_BOTTOM, GFP_KERNEL);
    if (g_vcom_id2hd_map == NULL) {
        g_vdev_map = NULL;
        return -1;
    }
    for (i = 0; i < (sizeof(g_vcom_hd_map) / sizeof(g_vcom_hd_map[0])); i++) {
        dev_id = g_vcom_hd_map[i].dev_id;
#ifdef CONFIG_MCI_CUST_CHN
        if ((dev_id >= VCOM_APP_CUST_CHN_ID_MIN) && (dev_id <= VCOM_APP_CUST_CHN_ID_MAX)) {
            g_vcom_hd_map[i].enable = mci_get_at_chn_state(dev_id, &cust_at_chn);
        }
#endif
        if (g_vcom_hd_map[i].enable) {
            vcom_map_init(&g_vdev_map[i], &g_vcom_hd_map[i]);
            g_vcom_id2hd_map[dev_id] = &g_vcom_hd_map[i];
        }
    }

    for (i = 0; i < (sizeof(g_vcom_hd_ext_map) / sizeof(g_vcom_hd_ext_map[0])); i++) {
        if (g_vcom_hd_ext_map[i].init) {
            g_vcom_hd_ext_map[i].init(&g_vcom_hd_ext_map[i]);
        }
    }

    vdev_msg_init();
    if (mci_debug_init()) {
        kfree(g_vdev_map);
        g_vdev_map = NULL;
        kfree(g_vcom_id2hd_map);
        g_vcom_id2hd_map = NULL;
        return -1;
    }
    bsp_err("[init] ok\n");
    return 0;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
static int vdev_core_init(void)
{
    return vdev_init();
}
#endif

int bsp_vdev_init(void)
{
    return vdev_init();
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(vdev_core_init);
#endif

#ifdef CONFIG_VDEV_DEBUG
void vcom_record_show(unsigned char *buf, unsigned char buf_size)
{
    unsigned char i;
    for (i = 0; i < buf_size; i++) {
        bsp_err("0x%x ", buf[i]);
    }
}

void vcom_channel_show(unsigned int chan_id)
{
    struct vcom_map_s *map = NULL;

    if (vcom_get_hd(chan_id) == NULL) {
        bsp_err("chan id: %d not support\n", chan_id);
        return;
    }

    map = vcom_get_map(chan_id);
    bsp_err("mci channel stats log start\n");
    bsp_err("chan name:    %s\n", map->hd->name);
    bsp_err("enqueue:      %u\n", map->stax.enqueue);
    bsp_err("read drop:    %u\n", map->stax.read_drop);
    bsp_err("write drop:   %u\n", map->stax.write_drop);
    bsp_err("requeue:      %u\n", map->stax.requeue);
    bsp_err("req_fail:     %u\n", map->stax.req_fail);
    bsp_err("dequeue:      %u\n", map->stax.dequeue);
    bsp_err("read:         %u\n", map->stax.read);
    bsp_err("copy_fail:    %u\n", map->stax.copy_fail);
    bsp_err("remain:       %u\n", map->stax.remain);
    bsp_err("succ_free:    %u\n", map->stax.succ_free);
    bsp_err("fail_free:    %u\n", map->stax.fail_free);
    bsp_err("read record:");
    vcom_record_show(map->stax.read_record, map->stax.read_size);
    bsp_err("read record end\n");
    bsp_err("write record:");
    vcom_record_show(map->stax.write_record, map->stax.write_size);
    bsp_err("write record end\n");
    bsp_err("app record:");
    vcom_record_show(map->stax.app_record, map->stax.app_size);
    bsp_err("app record end\n");
    bsp_err("mci channel stats log end\n");
    bsp_err("ccore ready   %u\n", map->ccore_ready);
}

void vcom_channel_help(void)
{
    unsigned int i;
    struct vcom_hd *hd = NULL;

    for (i = 0; i < VCOM_CHN_BOTTOM; i++) {
        hd = vcom_get_hd(i);
        if (hd != NULL && hd->enable == 1) {
            bsp_err("chan name: %s, chan id = %u\n", hd->name, hd->dev_id);
        }
    }
}

int vcom_debug_show(void)
{
    struct vcom_debug_stax *debug = vcom_get_debug_stax();

    bsp_err("msg_rcv:         %u\n", debug->msg_rcv);
    bsp_err("down_kmalloc:    %u\n", debug->down_kmalloc);
    bsp_err("fail_free:       %u\n", debug->fail_free);

    bsp_err("up_kmalloc:      %u\n", debug->up_kmalloc);
    bsp_err("up_kfree:        %u\n", debug->up_kfree);
    bsp_err("write:           %u\n", debug->write);
    bsp_err("write_suc:       %u\n", debug->write_suc);

    bsp_err("check_fail:      %u\n", debug->check_fail);
    bsp_err("parse_app:       %u\n", debug->parse_app);
    bsp_err("parse_acm:       %u\n", debug->parse_acm);
    bsp_err("parse_net_ctrl:  %u\n", debug->parse_net_ctrl);
    bsp_err("parse_notify:    %u\n", debug->parse_notify);
    bsp_err("parse_socket:    %u\n", debug->parse_socket);
    bsp_err("parse_pcdev:     %u\n", debug->parse_pcdev);
    bsp_err("parse_uart:      %u\n", debug->parse_uart);
    bsp_err("parse_err:       %u\n", debug->parse_err);
    return 0;
}
#endif
