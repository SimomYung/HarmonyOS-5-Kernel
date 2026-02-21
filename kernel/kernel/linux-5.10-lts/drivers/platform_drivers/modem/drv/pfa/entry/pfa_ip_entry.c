/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/jhash.h>
#include <linux/kernel.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netdevice.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_tuple.h>

#include <securec.h>
#include <bsp_pfa.h>

#include "pfa_direct_fw.h"
#include "pfa_ip_entry.h"
#include "pfa_dbg.h"
#include "pfa_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * debug code for pfa_ip_fw_add, the IP_FW_ADD_ERR_REC_MAX is the max code line
 * of function: pfa_ip_fw_add
 */
#define IP_FW_ADD_ERR_REC_MAX 300
#define IP_FW_ADD_REC_MASK (IP_FW_ADD_ERR_REC_MAX - 1)
#define IP_FW_ADD_ERR_REC_INIT() do { \
    /* idx 0 is record the base line */                                              \
    if (unlikely(NULL == g_sg_ip_fw_err_line)) {                                     \
        g_sg_ip_fw_err_line = kzalloc(g_sg_ip_fw_dft_max * sizeof(int), GFP_ATOMIC); \
        if (g_sg_ip_fw_err_line != NULL)                                             \
            g_sg_ip_fw_err_line[0] = __LINE__;                                       \
    }                                                                                \
} while (0)

#define IP_FW_ADD_ERR_REC_START() do { \
    int idx;                                                                   \
    /* the room of "err line rec" is ok */                                     \
    if (likely(g_sg_ip_fw_err_line != NULL)) {                                 \
        idx = __LINE__ - g_sg_ip_fw_err_line[0];                               \
        /* the room of "err line rec" is not enough */                         \
        if (unlikely(idx >= g_sg_ip_fw_dft_max)) {                             \
            /* expand the max and remalloc */                                  \
            int *rec_alloc;                                                    \
            g_sg_ip_fw_dft_max = idx + 100;                                    \
            rec_alloc = kzalloc(g_sg_ip_fw_dft_max * sizeof(int), GFP_ATOMIC); \
            if (rec_alloc != NULL) {                                           \
                /* kfree the old one and use the new room */                   \
                rec_alloc[0] = g_sg_ip_fw_err_line[0];                         \
                kfree((void *)g_sg_ip_fw_err_line);                            \
                g_sg_ip_fw_err_line = rec_alloc;                               \
                g_sg_ip_fw_err_line[idx]++;                                    \
            }                                                                  \
        } /* ok, record it */                                                  \
        else {                                                                 \
            g_sg_ip_fw_err_line[idx]++;                                        \
        }                                                                      \
    }                                                                          \
} while (0)

#ifdef PFA_DEBUGER
/* move to global, so we can modify them by debuger */
char g_usb0_pc_ip[IPV4_ADDR_LEN] = { 2, 4, 10, 1 };
char g_usb0_soc_ip[IPV4_ADDR_LEN] = { 2, 4, 10, 150 };
char g_eth0_pc_ip[IPV4_ADDR_LEN] = { 192, 168, 1, 10 };

#endif

#ifdef CONFIG_PFA_FW

static int *g_sg_ip_fw_err_line = NULL;
static u32 g_sg_ip_fw_dft_max = IP_FW_ADD_ERR_REC_MAX;

void pfa_aging_timer_set(void)
{
    struct pfa *pfa = &g_pfa;
    pfa_ageing_timer_t aging_timer;

    aging_timer.bits.pfa_ip_aging_time = pfa->ipfw.ipfw_timeout;
    aging_timer.bits.pfa_mac_aging_time = pfa->macfw.macfw_timeout;

    pfa_writel(pfa->regs, PFA_AGING_TIME, aging_timer.u32);
}

void updata_only_timer_set(unsigned int mac, unsigned ip)
{
    struct pfa *pfa = &g_pfa;
    pfa_ageing_timer_t aging_timer;
    aging_timer.u32 = pfa_readl(pfa->regs, PFA_AGING_TIME);
    if (ip) {
        aging_timer.bits.pfa_ip_aging_time = ip;
    }

    if (mac) {
        aging_timer.bits.pfa_mac_aging_time = mac;
    }

    pfa_writel(pfa->regs, PFA_AGING_TIME, aging_timer.u32);
}

void hash_baddr_set(dma_addr_t dma)
{
    struct pfa *pfa = &g_pfa;

    pfa_writel(pfa->regs, PFA_HASH_BADDR_H, upper_32_bits(dma));
    pfa_writel(pfa->regs, PFA_HASH_BADDR_L, lower_32_bits(dma));
}

static void pfa_ip_fw_add_tuple(struct pfa_ip_fw_entry *entry, struct nf_conn *ct, int32_t dir)
{
    if (nf_ct_l3num(ct) == NFPROTO_IPV4 ) {
        entry->iptype = 0;
    } else {
        entry->iptype = 1;
    }

    if (ct->status & IPS_SRC_NAT) {
        if (dir == IP_CT_DIR_ORIGINAL) {
            entry->nat_src_ip = ct->tuplehash[!dir].tuple.dst.u3.ip;
            entry->nat_src_port = ct->tuplehash[!dir].tuple.dst.u.tcp.port;
            entry->action = PFA_NF_NAT_MANIP_SRC;
        } else {
            entry->nat_dst_ip = ct->tuplehash[!dir].tuple.src.u3.ip;
            entry->nat_dst_port = ct->tuplehash[!dir].tuple.src.u.tcp.port;
            entry->action = PFA_NF_NAT_MANIP_DST;
        }
    } else if (ct->status & IPS_DST_NAT) {
        if (dir == IP_CT_DIR_ORIGINAL) {
            entry->nat_dst_ip = ct->tuplehash[!dir].tuple.src.u3.ip;
            entry->nat_dst_port = ct->tuplehash[!dir].tuple.src.u.tcp.port;
            entry->action = PFA_NF_NAT_MANIP_DST;
        } else {
            entry->nat_src_ip = ct->tuplehash[!dir].tuple.dst.u3.ip;
            entry->nat_src_port = ct->tuplehash[!dir].tuple.dst.u.tcp.port;
            entry->action = PFA_NF_NAT_MANIP_SRC;
        }
    } else {
        entry->action = PFA_NF_NAT_MANIP_NONE;
        entry->nat_src_ip = 0;
        entry->nat_src_port = 0;
        entry->nat_dst_ip = 0;
        entry->nat_dst_port = 0;
    }

    return;
}

// pfa_etry.c
int32_t pfa_ip_fw_show(void)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *head = NULL;
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *pos_priv = NULL;
    unsigned long flags = 0;
    int32_t i = 1;
    int32_t k = 0;

    PFA_ERR("pfa ip fw entry show begin \n");

    for (k = 0; k < PFA_IPFW_HTABLE_SIZE; k++) {
        head = &((struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket))[k];
        i = 1;

        if (head->priv == NULL) {
            PFA_ERR("hbucket[%d] priv NULL\n", k);
            continue;
        }

        /* check if ip_fw exist */
        if (head->valid || head->next_higher != 0 || head->next_lower != 0) {
            PFA_ERR("hbucket[%d], head->valid:%d, next:0x%x%x\n", k, head->valid, head->next_higher, head->next_lower);
            PFA_ERR("add cnt %u; del cnt %u; hkey %u; hash 0x%x \n", head->priv->add_cnt, head->priv->remove_cnt,
                    head->priv->hkey, head->priv->hash);

            spin_lock_irqsave(&pfa->ipfw.lock, flags);

            /* if head is valid dump the tuple */
            if (head->valid) {
                iptable_dump_entry(0, head);
            }

            list_for_each_entry(pos_priv, &head->priv->list, list)
            {
                pos = pos_priv->entry;
                iptable_dump_entry(i, pos);
                i++;
            }
            spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
        }
    }

    PFA_ERR("pfa ip fw entry show end \n");

    return 0;
}

void pfa_ip_fw_del_cb(struct pfa *pfa, struct pfa_ip_fw_entry *entry)
{
    struct pfa_del_entry_info info;

    if (pfa->ops.pfa_del_entry_cb) {
        info.stream_id = entry->stream_id;
        pfa->ops.pfa_del_entry_cb(&info);
    }
}

void pfa_ipfw_list_del_tuple(struct nf_conntrack_tuple *tuple, uint32_t hkey)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *pos = NULL;

    unsigned long flags;

    spin_lock_irqsave(&pfa->ipfw.lock, flags);

    if ((pos = ip_fw_entry_exist(pfa, tuple, hkey))) {
        if (pos->priv && pos->priv->contrack) {
            pos->priv->contrack->status &= ~IPS_PFA_CONFIRMED;
        }
        pfa_ipfw_list_del_entry(pfa, pos, hkey);
        spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
        pfa_ip_fw_del_cb(pfa, pos);
        ip_fw_node_put(pfa, pos, false);
        pfa->stat.ipfw_del++;
    } else {
        spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
    }

    return;
}

void pfa_ip_fw_write_wan_info(struct pfa *pfa, struct wan_dev_info_s *wan_dev_info, struct pfa_ip_fw_entry *entry)
{
    struct wan_info_s wan_info = {0};

    if (wan_dev_info->get_fw_info) {
        wan_dev_info->get_fw_info(wan_dev_info->dev, entry->iptype, &wan_info);
        entry->pdu_ssid = wan_info.info.pdu_session_id;
        entry->fc_head = wan_info.info.fc_head;
        entry->modem_id = wan_info.info.modem_id;
        entry->htab_usrfield_lower16 = (unsigned short)(wan_info.userfield0);
        entry->htab_usrfield_higher16 = (unsigned short)(wan_info.userfield0 >> 16); /* right shit 16 bit */
    }
    dev_hold(wan_dev_info->dev);
    entry->priv->wan_dev = wan_dev_info->dev;
    dev_put(wan_dev_info->dev);

    if (pfa->msg_level & PFA_MSG_TUPLE) {
        PFA_ERR("pdu_ssid 0x%x fc_head 0x%x modem_id 0x%x userfield0 0x%x \n", wan_info.info.pdu_session_id,
                wan_info.info.fc_head, wan_info.info.modem_id, wan_info.userfield0);
    }
}

static int pfa_ip_fw_cpy_org_entry_info(struct pfa_ip_fw_entry *entry, struct neighbour *n)
{
    int ret;
    unsigned int seq;

    entry->portno = n->dev->pfa_portno;
    if (n->hh.hh_len) {
        do {
            seq = read_seqbegin(&n->hh.hh_lock);
            ret = memcpy_s(entry->shost, sizeof(entry->shost),
                           (char *)n->hh.hh_data + HH_DATA_OFF(sizeof(struct ethhdr)) + ETH_ALEN, ETH_ALEN);
            if (ret) {
                return -ENOMEM;
            }
            ret = memcpy_s(entry->dhost, sizeof(entry->dhost),
                           (char *)n->hh.hh_data + HH_DATA_OFF(sizeof(struct ethhdr)), ETH_ALEN);
            if (ret) {
                return -ENOMEM;
            }
        } while (read_seqretry(&n->hh.hh_lock, seq));
    } else {
        ret = memcpy_s(entry->shost, sizeof(entry->shost), n->dev->dev_addr, ETH_ALEN);
        if (ret) {
            return -ENOMEM;
        }
        ret = memcpy_s(entry->dhost, sizeof(entry->dhost), n->ha, ETH_ALEN);
        if (ret) {
            return -ENOMEM;
        }
    }
    return 0;
}
static int pfa_ip_fw_cpy_reply_entry_info(struct pfa_ip_fw_entry *entry, struct net_device *indev, struct sk_buff *skb)
{
    int ret;
    unsigned char *mac_hd = NULL;

    entry->portno = (unsigned int)indev->pfa_portno;
    if (!skb_mac_header_was_set(skb)) {
        return -ENOMEM;
    }
    mac_hd = skb_mac_header(skb);
    ret = memcpy_s(entry->shost, sizeof(entry->shost), mac_hd, ETH_ALEN);  // shost is skb dhost
    if (ret) {
        return -ENOMEM;
    }
    ret = memcpy_s(entry->dhost, sizeof(entry->dhost), mac_hd + ETH_ALEN, ETH_ALEN);  // dhost is skb shost
    if (ret) {
        return -ENOMEM;
    }
    return 0;
}
int pfa_ip_fw_cpy_entry_info(struct pfa_ip_fw_entry *entry, struct neighbour *n, struct nf_conn *ct,
                             struct net_device *indev, struct sk_buff *skb, int dir)
{
    int ret;
    enum ip_conntrack_info ctinfo;
    struct nf_conntrack_tuple *tuple = NULL;
    struct nf_conn *skb_ct = nf_ct_get(skb, &ctinfo);

    if (skb_ct == NULL) {
        return -EINVAL;
    }

    if (CTINFO2DIR(ctinfo) == dir) {
        ret = pfa_ip_fw_cpy_org_entry_info(entry, n);
        if (ret) {
            return ret;
        }
        entry->priv->in = indev;
        entry->priv->out = n->dev;
    } else {
        ret = pfa_ip_fw_cpy_reply_entry_info(entry, indev, skb);
        if (ret) {
            return ret;
        }
        entry->priv->in = n->dev;
        entry->priv->out = indev;
    }

    /* explicit initialize unused field */
    entry->vid = 0;
    entry->priv->dir = dir;
    tuple = &entry->tuple;
    ret = memcpy_s(tuple, sizeof(*tuple), &ct->tuplehash[dir].tuple, sizeof(ct->tuplehash[dir].tuple));
    if (ret) {
        return -ENOMEM;
    }

    return 0;
}

void pfa_upadte_multiple_port_net_id(struct pfa *pfa, struct pfa_ip_fw_entry *entry)
{
    unsigned int portno;
    unsigned int net_id;
    struct net_device *out;

    portno = PFA_GET_PHY_PORT(entry->portno);
    if (portno >= PFA_PORT_NUM) {
        return;
    }
    if (pfa->ports[portno].ctrl.port_multiple_en) {
        out = entry->priv->out;
        net_id = PFA_NET_ID_BY_VIR_PORT(out->pfa_portno);
        entry->net_id = net_id;
        entry->portno = portno;
    }
}

int pfa_ip_fw_add_entry(struct neighbour *n, struct net_device *indev, struct nf_conn *ct, struct sk_buff *skb, int dir)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *entry = NULL;
    struct wan_dev_info_s *wan_dev_info = NULL;
    unsigned int hkey;
    unsigned int hash;
    unsigned long flags;
    int ret;

    entry = ip_fw_node_get(pfa);
    if (entry == NULL) {
        return -ENOMEM;
    }

    ret = pfa_ip_fw_cpy_entry_info(entry, n, ct, indev, skb, dir);
    if (ret) {
        return ret;
    }

    pfa_ip_fw_add_tuple(entry, ct, dir);
    entry->timestamp = pfa_readl(pfa->regs, PFA_TIME);
    entry->priv->contrack = ct;

    /* get hash */
    hash = pfa_ipfw_get_hash(&entry->tuple, pfa->ipfw.hzone);
    hkey = reciprocal_scale(hash, pfa->ipfw.hlist_size);
    entry->priv->hash = hash;
    entry->priv->hkey = hkey;
    entry->htab_usrfield_higher16 = (unsigned short)hkey;

    if (pfa->wanport.get_wan_info) {
        wan_dev_info = pfa->wanport.get_wan_info(indev);
        if (wan_dev_info != NULL) {
            if (pfa->wanport.get_wan_info(n->dev)) {
                PFA_ERR_ONCE("pfa tuple in and out dev both wan.\n");
                ip_fw_node_put(pfa, entry, false);
                return -EINVAL;
            }
        } else {
            wan_dev_info = pfa->wanport.get_wan_info(n->dev);
        }

        if ((wan_dev_info != NULL) && (wan_dev_info->dev != NULL)) {
            pfa_ip_fw_write_wan_info(pfa, wan_dev_info, entry);
            spin_lock_irqsave(&pfa->ipfw.lock, flags);
            list_add(&entry->priv->wan_list, &pfa->ipfw.wan_entry_list); /* add entry after first node */
            spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
        }
    } else {
        PFA_ERR_ONCE("pfa pfa->wanport.get_wan_info not reg \n");
    }
    pfa_upadte_multiple_port_net_id(pfa, entry);

    pfa_add_ipfw_entry_cb(pfa, skb, entry, dir, hkey);
    /* add node to list */
    ip_fw_list_add(pfa, entry, hkey);

    return 0;
}

void pfa_hash_cache_set(unsigned int setting)
{
    struct pfa *pfa = &g_pfa;

    if (setting) {
        pfa->hal->config_hash_cache_en(pfa, 1);
    } else {
        pfa->hal->config_hash_cache_en(pfa, 0);
    }
}

void pfa_add_err_show(void)
{
    int k = 0;
    IP_FW_ADD_ERR_REC_INIT();
    if (g_sg_ip_fw_err_line != NULL) {
        PFA_ERR("dump pfa_ip_fw_add retrun count, max(%d), base(%d)\n", g_sg_ip_fw_dft_max, g_sg_ip_fw_err_line[0]);
        for (k = 1; k < g_sg_ip_fw_dft_max; k++) {
            if (g_sg_ip_fw_err_line[k]) {
                PFA_ERR("pfa_ip_fw_add err line:%d, return count:%d\n", k + g_sg_ip_fw_err_line[0],
                        g_sg_ip_fw_err_line[k]);
            }
        }
    }
    PFA_ERR("dump pfa_ip_fw_add retrun count end\n\n");
}

static inline void __pfa_ip_fw_add_rec_init(void)
{
    IP_FW_ADD_ERR_REC_INIT();
}

int __pfa_ip_fw_check_ct(struct pfa *pfa, struct neighbour *n, struct sk_buff *skb, struct nf_conn *ct)
{
    if (!(ct->status & IPS_PFA_FORWARD)) {
        IP_FW_ADD_ERR_REC_START();
        return -ENOPROTOOPT;
    }

    if (ct->status & IPS_PFA_CONFIRMED) {
        IP_FW_ADD_ERR_REC_START();
        return -ENOPROTOOPT;
    }

    if ((ct->status & IPS_DST_NAT) && (ct->status & IPS_SRC_NAT)) {
        IP_FW_ADD_ERR_REC_START();
        return -ENOPROTOOPT;
    }

    /* only for reachable skb */
    if (!(n->nud_state & (NUD_CONNECTED | NUD_DELAY | NUD_PROBE))) {
        IP_FW_ADD_ERR_REC_START();
        return -ENOPROTOOPT;
    }

    return 0;
}

int __pfa_ip_fw_check_l4_type(struct pfa *pfa, struct sk_buff *skb, struct nf_conn *ct, enum ip_conntrack_info ctinfo)
{
    unsigned char protocol;

    /* only for tcp udp */
    if (AF_INET == nf_ct_l3num(ct)) {
        protocol = ip_hdr(skb)->protocol;
    } else if (AF_INET6 == nf_ct_l3num(ct)) {
        protocol = ipv6_hdr(skb)->nexthdr;
    } else {
        IP_FW_ADD_ERR_REC_START();
        return -ENOPROTOOPT;
    }

    if (IPPROTO_TCP == protocol) {
        if ((ctinfo == IP_CT_ESTABLISHED || ctinfo == IP_CT_ESTABLISHED_REPLY)) {
            if (ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) {
                return 0;
            } else {
                IP_FW_ADD_ERR_REC_START();
                return -ENOPROTOOPT;
            }
        } else {
            IP_FW_ADD_ERR_REC_START();
            return -ENOPROTOOPT;
        }
    }

    if (IPPROTO_UDP == protocol) {
        return 0;
    }

    IP_FW_ADD_ERR_REC_START();
    return -ENOPROTOOPT;
}

int __pfa_ip_fw_check_dir(struct pfa *pfa, struct sk_buff *skb, struct nf_conn *ct, enum ip_conntrack_info ctinfo)
{
    if (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL) {
        return 0;
    }

    if (CTINFO2DIR(ctinfo) == IP_CT_DIR_REPLY) {
        return 0;
    }

    IP_FW_ADD_ERR_REC_START();
    return -ENOPROTOOPT;
}

int pfa_ip_fw_check_entry_info(struct pfa *pfa, struct neighbour *n, struct sk_buff *skb, struct nf_conn *ct,
                                enum ip_conntrack_info ctinfo, struct net_device **indev)
{
    struct nf_conn_help *help = NULL;
    struct net *net_ctx = NULL;

    if (__pfa_ip_fw_check_ct(pfa, n, skb, ct)) {
        return -EINVAL;
    }

    if (__pfa_ip_fw_check_l4_type(pfa, skb, ct, ctinfo)) {
        return -EINVAL;
    }

    if (__pfa_ip_fw_check_dir(pfa, skb, ct, ctinfo)) {
        return -EINVAL;
    }

    /*
     * do not add master conntrack for pfa,
     * *for ppp ftp sip it will make error
     */
    help = nfct_help(ct);
    if ((help != NULL) && help->helper) {
        IP_FW_ADD_ERR_REC_START();
        return -EINVAL;
    }

    if (!n->dev) {
        IP_FW_ADD_ERR_REC_START();
        return -EINVAL;
    }
    /* find indev */
    if (skb->dev && skb->skb_iif) {
        dev_hold(skb->dev);
        net_ctx = dev_net(skb->dev);
        *indev = dev_get_by_index(net_ctx, skb->skb_iif);
        dev_put(skb->dev);
    }

    if (*indev == NULL) {
        IP_FW_ADD_ERR_REC_START();
        return -EINVAL;
    }
    return 0;
}

void pfa_add_ipfw_entry(struct neighbour *n, struct sk_buff *skb, struct nf_conn *ct, enum ip_conntrack_info ctinfo,
                         struct net_device *indev)
{
    int ret;
#ifdef CONFIG_PFA_XFRM
    if (skb->sp) {
        ret = pfa_ip_fw_add_entry(n, indev, ct, skb, CTINFO2DIR(ctinfo));
        if (ret) {
            IP_FW_ADD_ERR_REC_START();
        }
    } else {
        ret = pfa_ip_fw_add_entry(n, indev, ct, skb, CTINFO2DIR(ctinfo));
        if (ret) {
            IP_FW_ADD_ERR_REC_START();
        }
        ret = pfa_ip_fw_add_entry(n, indev, ct, skb, !CTINFO2DIR(ctinfo));
        if (ret) {
            IP_FW_ADD_ERR_REC_START();
        }
    }
#else
    ret = pfa_ip_fw_add_entry(n, indev, ct, skb, CTINFO2DIR(ctinfo));
    if (ret) {
        IP_FW_ADD_ERR_REC_START();
    }
    ret = pfa_ip_fw_add_entry(n, indev, ct, skb, !CTINFO2DIR(ctinfo));
    if (ret) {
        IP_FW_ADD_ERR_REC_START();
    }
#endif
    return;
}
/*
 * the entry must adds in orginial dir, otherwise causes error.
 * we choose adding pfa entry in dst_neigh_output, but it is
 * to be late. the dev is replaced by dst->dev, the outdev.
 * so we must find out indev.
 */
int __pfa_ip_fw_add_entry(struct neighbour *n, struct sk_buff *skb)
{
    struct pfa *pfa = &g_pfa;
    struct nf_conn *ct = NULL;
    enum ip_conntrack_info ctinfo;
    struct net_device *indev = NULL;
    int ret;

    if (pfa->mask_flags) {
        return 0;
    }

    __pfa_ip_fw_add_rec_init();
    pfa->stat.ipfw_add_enter++;

    ct = nf_ct_get(skb, &ctinfo);
    if (ct == NULL) {
        IP_FW_ADD_ERR_REC_START();
        goto err;
    }

    ret = pfa_ip_fw_check_entry_info(pfa, n, skb, ct, ctinfo, &indev);
    if (ret) {
        goto out;
    }
    /*
     * *only recv forward packet
     * *confirm once enough
     */
    pfa_add_ipfw_entry(n, skb, ct, ctinfo, indev);
    ct->status |= IPS_PFA_CONFIRMED;
    dsb(sy);

    dev_put(indev); /* dev_hold(indev) inside dev_get_by_index */

    return 0;

out:
    ct->status &= ~IPS_PFA_FORWARD;

err:
    pfa->stat.ipfw_add_leave++;

    return -EINVAL;
}

void pfa_ip_fw_add(void *n, void *skb)
{
    int ret;
    struct pfa *pfa = &g_pfa;

    if (n == NULL || skb == NULL) {
        return;
    }

    clear_bit(PFA_EVENT_IPFW_ADD_STOPED_BIT, (void *)&pfa->event);
    if (test_bit(PFA_EVENT_RESETTING_BIT, (void *)&pfa->event)) {
        set_bit(PFA_EVENT_IPFW_ADD_STOPED_BIT, (void *)&pfa->event);
        wake_up(&pfa->wqueue);
        return;
    }
    if (pfa->ops.pfa_ip_fw_add_filter && pfa->ops.pfa_ip_fw_add_filter(skb)) {
        pfa->stat.ipfw_filter++;
        return;
    }

    ret = __pfa_ip_fw_add_entry((struct neighbour *)n, (struct sk_buff *)skb);
    if (ret) {
        pfa->ipfw.ip_fw_not_add++;
    }

    set_bit(PFA_EVENT_IPFW_ADD_STOPED_BIT, (void *)&pfa->event);
    wake_up(&pfa->wqueue);
    return;
}

void pfa_ip_fw_del_one(struct pfa *pfa, struct nf_conntrack_tuple *tuple)
{
    u32 hkey;
    u32 hash;

    hash = pfa_ipfw_get_hash(tuple, pfa->ipfw.hzone);
    hkey = reciprocal_scale(hash, pfa->ipfw.hlist_size);

    pfa_ipfw_list_del_tuple(tuple, hkey);
    pfa->hal->config_hash_value(pfa, hash);
    pfa_writel(pfa->regs, PFA_TAB_CTRL, PFA_TAB_CTRL_RM_HCACHE);

    return;
}

void pfa_ip_fw_del(void *ct)
{
    struct pfa *pfa = &g_pfa;
    struct nf_conntrack_tuple *orig = NULL;
    struct nf_conntrack_tuple *repl = NULL;
    struct nf_conn *pfa_ct = (struct nf_conn *)ct;

    pfa->stat.ipfw_del_enter++;

    orig = &pfa_ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
    repl = &pfa_ct->tuplehash[IP_CT_DIR_REPLY].tuple;

    /*
     * If there are no packets over this connection for timeout period
     * delete the entries.
     */
    clear_bit(PFA_EVENT_IPFW_DEL_STOPED_BIT, (void *)&pfa->event);
    if (test_bit(PFA_EVENT_RESETTING_BIT, (void *)&pfa->event)) {
        set_bit(PFA_EVENT_IPFW_DEL_STOPED_BIT, (void *)&pfa->event);
        wake_up(&pfa->wqueue);
        return;
    }
    pfa_ip_fw_del_one(pfa, orig);
    pfa_ip_fw_del_one(pfa, repl);
    pfa_ct->status &= ~IPS_PFA_CONFIRMED;

    pfa->stat.ipfw_del_leave++;

    dsb(sy);

    set_bit(PFA_EVENT_IPFW_DEL_STOPED_BIT, (void *)&pfa->event);
    wake_up(&pfa->wqueue);

    return;
}

static void pfa_ip_fw_del_one_new(struct pfa *pfa,
    struct nf_conntrack_tuple *tuple, struct pfa_ip_fw_entry *pos)
{
    u32 hkey;
    u32 hash;

    hash = pfa_ipfw_get_hash(tuple, pfa->ipfw.hzone);
    hkey = reciprocal_scale(hash, pfa->ipfw.hlist_size);

    if (likely(pos->priv && pos->priv->contrack))
        pos->priv->contrack->status &= ~IPS_PFA_CONFIRMED;

    pfa_ipfw_list_del_entry(pfa, pos, hkey);
    pfa_ip_fw_del_cb(pfa, pos);
    ip_fw_node_put(pfa, pos, false);
    pfa_writel_relaxed(pfa->regs, PFA_HASH_VALUE, hash);
    pfa_writel(pfa->regs, PFA_TAB_CTRL, PFA_TAB_CTRL_RM_HCACHE);
}

static void pfa_ip_fw_del_one_entry(struct pfa *pfa, int i)
{
    struct pfa_ip_fw_entry *head = NULL;
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *n = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;
    struct pfa_ip_fw_entry_priv *priv_head = NULL;

    head = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket +
            i * sizeof(struct pfa_ip_fw_entry));
    priv_head = head->priv;
    list_for_each_entry_safe(priv_pos, n, &priv_head->list, list) {
        pos = priv_pos->entry;
        pfa_ip_fw_del_one_new(pfa, &pos->tuple, pos);
    }
}

void pfa_ip_fw_del_all_entry(void)
{
    int i;
    unsigned long flags;
    struct pfa *pfa = &g_pfa;

    spin_lock_irqsave(&pfa->ipfw.lock, flags);
    for (i = 0; i < pfa->ipfw.hlist_size; i++)
        pfa_ip_fw_del_one_entry(pfa, i);
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
}

void mdrv_pfa_ipfw_del_by_tuple(struct nf_conntrack_tuple *tuple)
{
    struct pfa *pfa = &g_pfa;

    pfa->stat.ipfw_del_enter++;

    pfa_ip_fw_del_one(pfa, tuple);

    pfa->stat.ipfw_del_leave++;

    return;
}

int mdrv_pfa_ipfw_find_info_by_tuple(struct nf_conntrack_tuple *tuple, struct pfa_ipfw_entry_info *info)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *pos = NULL;
    unsigned long flags;
    u32 hkey;
    u32 hash;
    int ret = -1;

    if (unlikely(!tuple || !info)) {
        return -1;
    }

    hash = pfa_ipfw_get_hash(tuple, pfa->ipfw.hzone);
    hkey = reciprocal_scale(hash, pfa->ipfw.hlist_size);

    spin_lock_irqsave(&pfa->ipfw.lock, flags);
    pos = ip_fw_entry_exist(pfa, tuple, hkey);
    if (pos) {
        info->in = pos->priv->in;
        info->out = pos->priv->out;
        ret = 0;
    }
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
    return ret;
}

struct pfa_ip_fw_entry *ip_fw_entry_exist_hkey(struct pfa *pfa, uint32_t hkey)
{
    struct pfa_ip_fw_entry *entry = pfa->ipfw.hbucket;
    struct list_head *head = &entry[hkey].priv->list;
    struct pfa_ip_fw_entry *first = &entry[hkey];
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;

    /* check if first header exist */
    if (first->valid && (first->priv->hkey == hkey)) {
        return first;
    }

    /* check if ip_fw exist */
    list_for_each_entry(priv_pos, head, list)
    {
        pos = priv_pos->entry;
        if (pos->valid && (pos->priv->hkey == hkey)) {
            return pos;
        }
    }

    return NULL;
}

int mdrv_pfa_update_streamid_by_hkey(unsigned int hkey, unsigned int stream_id)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *pos = NULL;
    unsigned long flags;
    int ret = -1;

    if (hkey >= pfa->ipfw.hlist_size) {
        PFA_ERR("hkey:%u more than hlist_size:%u\n", hkey, pfa->ipfw.hlist_size);
        return -1;
    }

    spin_lock_irqsave(&pfa->ipfw.lock, flags);
    pos = ip_fw_entry_exist_hkey(pfa, hkey);
    if (pos) {
          pos->stream_id = stream_id;
          ret = 0;
    }
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
    return ret;
}

EXPORT_SYMBOL(mdrv_pfa_update_streamid_by_hkey);

bool pfa_del_ipfw_check_condition(struct pfa_ip_fw_entry *pos, const struct net_device *dev, const unsigned char *mac)
{
    if (dev) {
        return (pos->priv->in == dev || pos->priv->out == dev);
    }
    if (mac) {
        return pfa_ip_fw_check_dhost(mac, pos->shost) || pfa_ip_fw_check_dhost(mac, pos->dhost);
    }
    return false;
}

static void pfa_ip_fw_del_one_entry_by_condition(struct pfa *pfa, int i, const struct net_device *dev, const unsigned char *mac)
{
    struct pfa_ip_fw_entry *head = NULL;
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *n = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;
    struct pfa_ip_fw_entry_priv *priv_head = NULL;

    head = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket +
        i * sizeof(struct pfa_ip_fw_entry));
    priv_head = head->priv;
    list_for_each_entry_safe(priv_pos, n, &priv_head->list, list) {
        pos = priv_pos->entry;
        if ((pfa_del_ipfw_check_condition(pos, dev, mac)) && pos->valid) {
            pfa->stat.ipfw_del_enter++;
            pfa_ip_fw_del_one_new(pfa, &pos->tuple, pos);
            pfa->stat.ipfw_del_leave++;
        }
    }
}

void mdrv_pfa_ipfw_del_by_dev(struct net_device *dev)
{
    int i;
    unsigned long flags;
    struct pfa *pfa = &g_pfa;

    if (unlikely(!dev))
        return;

    spin_lock_irqsave(&pfa->ipfw.lock, flags);
    for (i = 0; i < pfa->ipfw.hlist_size; i++) {
        pfa_ip_fw_del_one_entry_by_condition(pfa, i, dev, NULL);
    }
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
}

void mdrv_pfa_ipfw_del_by_mac(const unsigned char *mac)
{
    int i;
    unsigned long flags;
    struct pfa *pfa = &g_pfa;

    if (unlikely(!mac))
        return;

    spin_lock_irqsave(&pfa->ipfw.lock, flags);
    for (i = 0; i < pfa->ipfw.hlist_size; i++) {
        pfa_ip_fw_del_one_entry_by_condition(pfa, i, NULL, mac);
    }
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);
}
EXPORT_SYMBOL(mdrv_pfa_ipfw_del_by_mac);

int bsp_pfa_update_ipfw_entry(struct wan_dev_info_s *wan_dev_info)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *entry = NULL;
    struct pfa_ip_fw_entry_priv *entry_priv = NULL;
    unsigned long flags = 0;
    unsigned int tab_clean;
    int ret = 0;

    if (wan_dev_info == NULL) {
        PFA_ERR_ONCE("update_entry param error\n");
        return ret;
    }

    if (!pfa->direct_fw.dfw_flags.enable && wan_dev_info->dev == NULL) {
        PFA_ERR_ONCE("update_entry dev NULL \n");
        return ret;
    }

    pfa_writel(pfa->regs, PFA_TAB_CLR, 0);

    spin_lock_irqsave(&pfa->ipfw.lock, flags);

    list_for_each_entry(entry_priv, &pfa->ipfw.wan_entry_list, wan_list)
    {
        entry = entry_priv->entry;
        if ((entry->valid) && (entry->priv->wan_dev == wan_dev_info->dev)) {
            pfa_ip_fw_write_wan_info(pfa, wan_dev_info, entry);
        }
    }

    tab_clean = BIT(4);  // clean all hash cache, bit 4
    pfa_writel(pfa->regs, PFA_TAB_CLR, tab_clean);
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);

    if (pfa->direct_fw.dfw_flags.enable) {
        ret = pfa_direct_fw_record_wandev(wan_dev_info);
    }

    return ret;
}

void pfa_add_entry_update_info_out_dev(struct pfa_ip_fw_entry *entry, struct pfa_add_entry_info *info)
{
    if (info->out) {
        entry->priv->out = info->out;
        entry->net_id = PFA_NET_ID_BY_VIR_PORT(info->out->pfa_portno);
        entry->portno = PFA_GET_PHY_PORT(info->out->pfa_portno);
    }
}

#else
int bsp_pfa_update_ipfw_entry(struct wan_dev_info_s *wan_dev_info)
{
    struct pfa *pfa = &g_pfa;
    int ret = 0;

    if (pfa->direct_fw.dfw_flags.enable) {
        ret = pfa_direct_fw_record_wandev(wan_dev_info);
    }
    return ret;
}
void mdrv_pfa_ipfw_del_by_mac(const unsigned char *mac)
{}
EXPORT_SYMBOL(mdrv_pfa_ipfw_del_by_mac);

void pfa_add_entry_update_info_out_dev(struct pfa_ip_fw_entry *entry, struct pfa_add_entry_info *info)
{}

#endif

void pfa_ip_fw_htab_set(void)
{
    struct pfa *pfa = &g_pfa;
    unsigned int size;

    size = sizeof(struct pfa_ip_fw_entry);
    size /= 0x4;

    pfa_writel(pfa->regs, PFA_HASH_BADDR_L, lower_32_bits(pfa->ipfw.hbucket_dma));
    pfa_writel(pfa->regs, PFA_HASH_BADDR_H, upper_32_bits(pfa->ipfw.hbucket_dma));
    pfa_writel(pfa->regs, PFA_HASH_WIDTH, size);
    pfa_writel(pfa->regs, PFA_HASH_DEPTH, pfa->ipfw.hlist_size);
    pfa_writel(pfa->regs, PFA_HASH_ZONE, pfa->ipfw.hzone);
    pfa_writel(pfa->regs, PFA_HASH_RAND, pfa->ipfw.hrand);
    // we are going to use default reg setting on PFA_HASH_L3_PROTO_OFFSET;
}
static void pfa_ip_empty_entry_free(struct pfa *pfa, unsigned int size_ipfw)
{
    if (pfa->ipfw.hbucket_empty) {
        dma_free_coherent(pfa->dev, size_ipfw * PFA_IPFW_HTABLE_EMPTY_SIZE, pfa->ipfw.hbucket_empty,
                          pfa->ipfw.hbucket_dma_empty);
        pfa->ipfw.hbucket_empty = NULL;
    }
    return;
}
static void pfa_ip_entry_free(struct pfa *pfa, unsigned int size_ipfw)
{
    int i = 0;
    struct pfa_ip_fw_entry *entry = NULL;

    while (i < pfa->ipfw.hlist_size) {
        entry = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket + i * size_ipfw);
        if (entry->priv != NULL) {
            kfree(entry->priv);
        }
        i++;
    }

    if (pfa->ipfw.hbucket) {
        dma_free_coherent(pfa->dev, size_ipfw * pfa->ipfw.hlist_size, pfa->ipfw.hbucket, pfa->ipfw.hbucket_dma);

        pfa->ipfw.hbucket = NULL;
    }
    return;
}
int pfa_ip_empty_entry_init(struct pfa *pfa, unsigned int size_ipfw)
{
    struct pfa_ip_fw_entry *empty_entry = NULL;
    int ret;

    /* alloc pfa hash bucket empty for pfa switch of func */
    empty_entry = dma_alloc_coherent(pfa->dev, size_ipfw * PFA_IPFW_HTABLE_EMPTY_SIZE, &pfa->ipfw.hbucket_dma_empty,
                                     GFP_KERNEL);
    if (empty_entry == NULL) {
        PFA_ERR("alloc ip fw hbucket empty 1 entry failed\n");
        return -ENOMEM;
    }
    pfa->ipfw.hbucket_empty = empty_entry;

    ret = memset_s(empty_entry, sizeof(*empty_entry) * PFA_IPFW_HTABLE_EMPTY_SIZE, 0,
                   size_ipfw * PFA_IPFW_HTABLE_EMPTY_SIZE);
    if (ret) {
        PFA_ERR("memset_s ip fw hbucket empty  entry failed\n");
        pfa_ip_empty_entry_free(pfa, size_ipfw);
    }
    return ret;
}

static inline void pfa_ip_entry_defcfg(struct pfa *pfa, unsigned int size_ipfw)
{
    if (!pfa->ipfw.hlist_size) {
        pfa->ipfw.hlist_size = PFA_IPFW_HTABLE_SIZE;
    }

    pfa->ipfw.hrand = PFA_IPFW_HTABLE_RAND;
    pfa->ipfw.hzone = PFA_IPFW_HTABLE_ZONE;
    pfa->ipfw.hitem_width = size_ipfw; /* jiffies */
    return;
}

int pfa_ip_entry_table_init(struct pfa *pfa)
{
    int ret, i;
    unsigned int size_ipfw = sizeof(struct pfa_ip_fw_entry);

    pfa_ip_entry_defcfg(pfa, size_ipfw);
    ret = pfa_ip_empty_entry_init(pfa, size_ipfw);
    if (ret) {
        return ret;
    }

    /* alloc pfa hash bucket */
    pfa->ipfw.hbucket = dma_alloc_coherent(pfa->dev, size_ipfw * pfa->ipfw.hlist_size, &pfa->ipfw.hbucket_dma,
                                           GFP_KERNEL);
    if (!pfa->ipfw.hbucket) {
        PFA_ERR("alloc ip fw hbucket of %d entrys failed\n", pfa->ipfw.hlist_size);
        goto err_alloc_hbucket;
    }

    ret = memset_s(pfa->ipfw.hbucket, sizeof(struct pfa_ip_fw_entry) * pfa->ipfw.hlist_size, 0,
                   size_ipfw * pfa->ipfw.hlist_size);
    if (ret) {
        goto err_alloc_hbucket;
    }

    for (i = 0; i < pfa->ipfw.hlist_size; i++) {
        struct pfa_ip_fw_entry *entry = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket + i * size_ipfw);

        entry->dead_timestamp = jiffies;
        entry->valid = 0;
        entry->priv = kzalloc(sizeof(struct pfa_ip_fw_entry_priv), GFP_KERNEL);
        if (entry->priv == NULL) {
            PFA_ERR("alloc ip fw hbucket priv failed\n");
            goto err_alloc_hslab;
        }
        entry->priv->entry = entry;
        entry->priv->dma = pfa->ipfw.hbucket_dma + i * size_ipfw;
        INIT_LIST_HEAD(&entry->priv->list);
    }

    /* alloc dma pool for ip fw entry */
    pfa->ipfw.hslab = (struct dma_pool *)dma_pool_create(dev_name(pfa->dev), pfa->dev,
                                                         sizeof(struct pfa_ip_fw_entry_dma), 4, PAGE_SIZE); /* 4 byte alignment */
    if (pfa->ipfw.hslab == NULL) {
        goto err_alloc_hslab;
    }

    INIT_LIST_HEAD(&pfa->ipfw.free_list);
    INIT_LIST_HEAD(&pfa->ipfw.wan_entry_list);

    spin_lock_init(&pfa->ipfw.lock);
    spin_lock_init(&pfa->ipfw.free_lock);

    pfa->ipfw.deadtime = 2; /* jiffies, 2 deadtime */
    return 0;

err_alloc_hslab:
    pfa_ip_entry_free(pfa, size_ipfw);
err_alloc_hbucket:
    pfa_ip_empty_entry_free(pfa, size_ipfw);
    return -1;
}

void pfa_ip_entry_res_free(struct pfa *pfa)
{
    if (pfa->ipfw.hslab != NULL) {
        dma_pool_destroy(pfa->ipfw.hslab);
        pfa->ipfw.hslab = NULL;
    }

    if (pfa->ipfw.hbucket_empty != NULL) {
        dma_free_coherent(pfa->dev, sizeof(struct pfa_ip_fw_entry) * pfa->ipfw.hlist_size, pfa->ipfw.hbucket,
                          pfa->ipfw.hbucket_dma);
        pfa->ipfw.hbucket_empty = NULL;
    }

    if (pfa->ipfw.hbucket != NULL) {
        dma_free_coherent(pfa->dev, sizeof(struct pfa_ip_fw_entry) * PFA_IPFW_HTABLE_EMPTY_SIZE,
                          pfa->ipfw.hbucket_empty, pfa->ipfw.hbucket_dma_empty);
        pfa->ipfw.hbucket = NULL;
    }
    return;
}

void pfa_ip_entry_table_exit(struct pfa *pfa)
{
    int i;
    unsigned long flags;
    dma_addr_t dma;
    struct pfa_ip_fw_entry *head = NULL;
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *n = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;
    struct pfa_ip_fw_entry_priv *priv_head = NULL;
    spin_lock_irqsave(&pfa->ipfw.lock, flags);

    /* free node in free queue */
    for (i = 0; i < pfa->ipfw.hlist_size; i++) {
        head = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket + i * sizeof(struct pfa_ip_fw_entry));
        priv_head = head->priv;
        list_for_each_entry_safe(priv_pos, n, &priv_head->list, list)
        {
            pos = priv_pos->entry;
            list_del_init(&pos->priv->list);
            list_del_init(&pos->priv->wan_list);
            dma = pos->priv->dma;
            kfree(pos->priv);
            dma_pool_free(pfa->ipfw.hslab, container_of(pos, struct pfa_ip_fw_entry_dma, entry), PFA_IP_FW_TO_DMA_OFFSET(dma));
        }
    }

    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);

    spin_lock_irqsave(&pfa->ipfw.free_lock, flags);

    list_for_each_entry_safe(priv_pos, n, &pfa->ipfw.free_list, list)
    {
        pos = priv_pos->entry;
        dma = pos->priv->dma;
        kfree(pos->priv);
        dma_pool_free(pfa->ipfw.hslab, container_of(pos, struct pfa_ip_fw_entry_dma, entry), PFA_IP_FW_TO_DMA_OFFSET(dma));
    }

    spin_unlock_irqrestore(&pfa->ipfw.free_lock, flags);

    pfa_ip_entry_res_free(pfa);
    return;
}

MODULE_LICENSE("GPL");

#ifdef CONFIG_PFA_FW

EXPORT_SYMBOL(pfa_ip_fw_show);
EXPORT_SYMBOL(pfa_add_err_show);
EXPORT_SYMBOL(bsp_pfa_update_ipfw_entry);
EXPORT_SYMBOL(pfa_ip_fw_del_one);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
