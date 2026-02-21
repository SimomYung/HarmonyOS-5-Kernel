/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2022. All rights reserved.
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

#include <uapi/linux/if_ether.h>
#include <linux/if_vlan.h>

#include <osl_types.h>
#include <mdrv_udi.h>
#include <mdrv_pfa.h>
#include <bsp_wan_eth_stick.h>
#include <securec.h>

#include "bsp_pfa.h"
#include "bsp_dra.h"

#include "wan_eth_stick.h"
#include "wan_eth_spe.h"
#include "wan_eth_common.h"

#ifdef CONFIG_WETH_OFL_ENABLE
#include "wan_eth_msg.h"
#endif

#define PNET_MAX_NUM 8
#define HEX_NUM 16

#define WETH_STICK_PKTYPE_ETH 0x40
#define WETH_STICK_PKTYPE_VLAN 0x80
#define WETH_VLAN_PARSE_CHK_LEN 58
#define WETH_ETH_PARSE_CHK_LEN 54

enum sess_info_type {
    SESS_INFO_IPV4,
    SESS_INFO_IPV6,
    SESS_INFO_ETH,
    SESS_INFO_MAX,
};

struct weth_pfa_tft_info {
    unsigned int pdu_session_id:8;
    unsigned int fc_head:4;
    unsigned int modem_id:2;
    unsigned int rsv:18;
};

struct stick_session_info {
    struct weth_pfa_tft_info pfa_tft_infos[SESS_INFO_MAX];
    unsigned char iface_id[SESS_INFO_MAX];
    enum net_iface_mode iface_mode;
    unsigned char link_state;
};

struct stick_stat {
    unsigned int ioctl_ctrl;
    unsigned int stick_tx;
    unsigned int stick_tx_fail;
    unsigned int uplink_tx;
    unsigned int ethernet_tx;
    unsigned int ethernet_tx_fail;
    unsigned int ethernet_rx;
};

struct stick_ctx {
    struct stick_session_info sess_info[PNET_MAX_NUM];
    struct stick_stat stat[PNET_MAX_NUM];
    weth_stick_tx_cb_t tx_cb;
    struct sk_buff *skb_queue_tail;
    int skb_queue_len;
    spinlock_t tx_lock;
    int is_tx_lock_init;
    unsigned int invalid_devid_sess;
    unsigned int invalid_devid_data;
    unsigned int invalid_devid_uplink;
    unsigned int rx_complete_submit_fail;
    struct device *dev_for_map;
};

#ifdef CONFIG_WETH_OFL_ENABLE
STATIC int weth_stick_sync_session_ofl(unsigned int dev_id, struct stick_session_info *sess_info);

#endif

struct stick_ctx g_weth_stick_ctx = {0};

int weth_stick_get_session_mode(unsigned int dev_id)
{
    struct stick_ctx *ctx = &g_weth_stick_ctx;
    struct stick_session_info *sess_info = NULL;

    if (dev_id >= PNET_MAX_NUM) {
        ctx->invalid_devid_data++;
        return -1;
    }

    sess_info = &ctx->sess_info[dev_id];

    return sess_info->iface_mode;
}

static void stick_ioctl_backup_session_info(
    struct stick_session_info *sess_info, struct net_packet_label *packet_label, enum sess_info_type type)
{
    struct weth_pfa_tft_info *pfa_tft_info = NULL;

    if (packet_label->valid != 1) {
        return;
    }

    pfa_tft_info = &sess_info->pfa_tft_infos[type];
    pfa_tft_info->pdu_session_id = packet_label->sess_id;
    pfa_tft_info->modem_id = packet_label->modem_id;
    pfa_tft_info->fc_head = packet_label->fc_head;

    weth_pr_dbg("sess type %d\n", type);
    weth_pr_dbg("sess id   %d\n", pfa_tft_info->pdu_session_id);
    weth_pr_dbg("mdm  id   %d\n", pfa_tft_info->modem_id);
    weth_pr_dbg("fc head   %d\n", pfa_tft_info->fc_head);
}

static int stick_ioctl_set_pdusession(struct stick_ctx *ctx, u32 dev_id, void *param)
{
    struct net_iface_info *iface_info = (struct net_iface_info *)param;
    struct net_packet_label *packet_label = NULL;
    struct stick_session_info *sess_info = &ctx->sess_info[dev_id];

    weth_pr_dbg("iface_mode %d\n", iface_info->iface_mode);
    weth_pr_dbg("iface_id   %d\n", iface_info->iface_id);
    weth_pr_dbg("link_state %d\n", iface_info->link_state);

    switch (iface_info->iface_mode) {
        case PPP_NDIS_NET_MODE_IP: {
            /* ipv4 sess info cfg */
            packet_label = &iface_info->v4_label;
            sess_info->iface_id[SESS_INFO_IPV4] = iface_info->iface_id;
            stick_ioctl_backup_session_info(sess_info, packet_label, SESS_INFO_IPV4);
            /* ipv6 sess info cfg */
            packet_label = &iface_info->v6_label;
            sess_info->iface_id[SESS_INFO_IPV6] = iface_info->iface_id;
            stick_ioctl_backup_session_info(sess_info, packet_label, SESS_INFO_IPV6);
            /* ip type wan info cfg func registe */

            sess_info->iface_mode = PPP_NDIS_NET_MODE_IP;
            break;
        }
        case PPP_NDIS_NET_MODE_ETH: {
            /* eth sess info cfg */
            packet_label = &iface_info->eth_label;
            sess_info->iface_id[SESS_INFO_ETH] = iface_info->iface_id;
            stick_ioctl_backup_session_info(sess_info, packet_label, SESS_INFO_ETH);
            /* eth type wan info cfg func registe */
            sess_info->iface_mode = PPP_NDIS_NET_MODE_ETH;
            break;
        }
        default: {
            printk(KERN_ERR "[usb]:%s: session cfg mode error\n", __func__);
            return -EINVAL;
        }
    }
#ifdef CONFIG_WETH_OFL_ENABLE
    sess_info->link_state = iface_info->link_state;
    weth_stick_sync_session_ofl(dev_id, sess_info);
#endif
    return 0;
}

int bsp_pnet_stick_ioctl(u32 dev_id, enum ncm_ioctl_cmd_type cmd, void *param)
{
    int ret = 0;
    struct stick_ctx *ctx = &g_weth_stick_ctx;

    weth_pr_dbg("stick ioctl dev_id %d cmd 0x%x\n", dev_id, cmd);
    if (dev_id >= PNET_MAX_NUM) {
        ctx->invalid_devid_sess++;
        return -1;
    }

    if (cmd == NCM_IOCTL_SET_PDUSESSION) {
        ctx->stat[dev_id].ioctl_ctrl++;
        ret = stick_ioctl_set_pdusession(ctx, dev_id, param);
    }

    if (ctx->is_tx_lock_init == 0) {
        spin_lock_init(&ctx->tx_lock);
        ctx->is_tx_lock_init = 1;
    }

    weth_register_get_session_mode_cb(weth_stick_get_session_mode);
    return ret;
}

int bsp_pnet_stick_open(void)
{
    weth_pr_dbg("stick open\n");
    return 0;
}

int bsp_pnet_stick_close(void)
{
    weth_pr_dbg("stick close\n");
    return 0;
}

void bsp_pcie_stick_rx_complete(void)
{
    weth_pr_dbg("stick rx cmplt\n");
}

int bsp_pcie_stick_single_down_deliver(unsigned int dev_id, struct sk_buff *skb)
{
    struct stick_ctx *ctx = &g_weth_stick_ctx;
    struct stick_stat *stat = NULL;
    struct rx_cb_map_s *map_info = (struct rx_cb_map_s *)skb->cb;
    int ret;

    weth_pr_dbg("stick single down deliver\n");
    if (dev_id >= PNET_MAX_NUM) {
        ctx->invalid_devid_data++;
        return -1;
    }

    stat = &ctx->stat[dev_id];
    stat->stick_tx++;

    if (unlikely(!skb)) {
        weth_pr_err("single down skb null\n");
        return -1;
    }

    if (weth_stick_get_session_mode(dev_id) != PPP_NDIS_NET_MODE_ETH) {
        weth_add_pfa_direct_fw_entry(skb, dev_id);
    }

    map_info->userfield0 = ((struct ethhdr *)skb->data)->h_proto;
    map_info->userfield0 = (map_info->userfield0 << HEX_NUM) | dev_id; /* get high 16 bits */
    ret = weth_send_pkt_to_ofl(true, skb);
    if (ret) {
        dev_kfree_skb_any(skb);
    }

    return ret;
}

void weth_stick_register_tx_cb(weth_stick_tx_cb_t cb)
{
    g_weth_stick_ctx.tx_cb = cb;
}

static u8 weth_stick_eth_can_parse(struct sk_buff *skb)
{
    switch (skb->protocol) {
        case htons(ETH_P_8021Q): {
            if (skb->len >= WETH_VLAN_PARSE_CHK_LEN) {
                return 1;
            }
            break;
        }
        default: {
            if (skb->len >= WETH_ETH_PARSE_CHK_LEN) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

void weth_ethernet_tx(struct sk_buff *skb, unsigned int dev_id, struct stick_session_info *sess_info)
{
    struct wan_info_s *wan_info = NULL;
    struct vlan_ethhdr *vhdr = NULL;
    struct stick_ctx *ctx = &g_weth_stick_ctx;
    struct stick_stat *stat = &ctx->stat[dev_id];

    stat->ethernet_tx++;
    dma_unmap_single(ctx->dev_for_map, virt_to_phys(skb->data), L1_CACHE_BYTES, DMA_FROM_DEVICE);
    wan_info = (struct wan_info_s *)(&skb->cb);
    wan_info->userfield1 = 0;
    wan_info->userfield2 = 0;
    wan_info->info.pkt_ind = 1;

    vhdr = (struct vlan_ethhdr *)(skb->data);
    skb->protocol = vhdr->h_vlan_proto;
    if (skb->protocol == htons(ETH_P_8021Q)) {
        wan_info->userfield0 |= WETH_STICK_PKTYPE_VLAN;
    } else {
        wan_info->userfield0 |= WETH_STICK_PKTYPE_ETH;
    }

    dma_map_single(ctx->dev_for_map, skb->data, L1_CACHE_BYTES, DMA_FROM_DEVICE);
}

int weth_stick_wan_tx(struct sk_buff *skb)
{
    struct stick_ctx *ctx = &g_weth_stick_ctx;
    struct wan_info_s *wan_info = (struct wan_info_s *)skb->cb;
    struct stick_session_info *sess_info = NULL;
    unsigned short protocol;
    unsigned int dev_id;
    struct weth_pfa_tft_info *pfa_tft_info = NULL;
    struct stick_stat *stat = NULL;

    dev_id = wan_info->userfield0 & 0xFFFF;
    if (dev_id >= PNET_MAX_NUM) {
        ctx->invalid_devid_uplink++;
        return -1;
    }
    stat = &ctx->stat[dev_id];
    stat->uplink_tx++;

    protocol = (wan_info->userfield0 >> 16) & 0xFFFF;
    sess_info = &ctx->sess_info[dev_id];
    if (sess_info->iface_mode == PPP_NDIS_NET_MODE_ETH) {
        pfa_tft_info = &sess_info->pfa_tft_infos[SESS_INFO_ETH];
        wan_info->userfield0 = sess_info->iface_id[SESS_INFO_ETH];
    } else if (protocol == htons(ETH_P_IP)) {
        pfa_tft_info = &sess_info->pfa_tft_infos[SESS_INFO_IPV4];
        wan_info->userfield0 = sess_info->iface_id[SESS_INFO_IPV4];
    } else {
        pfa_tft_info = &sess_info->pfa_tft_infos[SESS_INFO_IPV6];
        wan_info->userfield0 = sess_info->iface_id[SESS_INFO_IPV6];
    }

    wan_info->info.pdu_session_id = pfa_tft_info->pdu_session_id;
    wan_info->info.modem_id = pfa_tft_info->modem_id;
    wan_info->info.fc_head = pfa_tft_info->fc_head;
    wan_info->info.pkt_ind = 0;
    wan_info->info.parse_en = !!weth_stick_eth_can_parse(skb);

    if (sess_info->iface_mode == PPP_NDIS_NET_MODE_ETH) {
        weth_ethernet_tx(skb, dev_id, sess_info);
    }

    weth_pr_dbg("sess_mode %d, protocol %d, parse_en %d, pkt_ind %d\n",
        sess_info->iface_mode, protocol, wan_info->info.parse_en, wan_info->info.pkt_ind);

    return mdrv_wan_tx(skb, WAN_DRA_OWN);
}

void weth_stick_set_dev_for_map(struct device *dev)
{
    g_weth_stick_ctx.dev_for_map = dev;
}

#ifdef CONFIG_WETH_OFL_ENABLE
struct weth_msg_sess_info {
    unsigned int dev_id;
    struct stick_session_info session;
};

STATIC int weth_stick_sync_session_ofl(unsigned int dev_id, struct stick_session_info *sess_info)
{
    int ret;
    struct weth_msg_sess_info sess = {0};

    weth_pr_dbg("send sess to ofl\n");
    sess.dev_id = dev_id;
    ret = memcpy_s(&sess.session, sizeof(struct stick_session_info), sess_info, sizeof(struct stick_session_info));
    if (ret) {
        weth_pr_err("msg memcpy error\n");
        return -1;
    }

    return weth_msg_cmd_sendto(WETH_MSG_CMD_ID_SESS_INFO, &sess, sizeof(sess));
}

#endif

#ifdef CONFIG_WETH_DEBUG
int weth_get_pdu_session(unsigned int dev_id)
{
    int i;
    struct stick_session_info *sess_info = NULL;
    struct weth_pfa_tft_info *pfa_tft_info = NULL;

    if (dev_id >= PNET_MAX_NUM) {
        weth_pr_err("dev_id %d error\n", dev_id);
        return -1;
    }

    sess_info = &g_weth_stick_ctx.sess_info[dev_id];
    weth_pr_err("iface_mode  %s\n", (sess_info->iface_mode == 0) ? "ip" : "eth");
    for (i = 0; i < SESS_INFO_MAX; i++) {
        pfa_tft_info = &sess_info->pfa_tft_infos[i];

        weth_pr_err("iface_id  %d\n", sess_info->iface_id[i]);

        weth_pr_err("sess type %s\n", (i == 0) ? "ipv4" : "ipv6");
        weth_pr_err("sess id   %d\n", pfa_tft_info->pdu_session_id);
        weth_pr_err("mdm  id   %d\n", pfa_tft_info->modem_id);
        weth_pr_err("fc head   %d\n", pfa_tft_info->fc_head);
        weth_pr_err("\n");
    }

    return 0;
}
#endif
