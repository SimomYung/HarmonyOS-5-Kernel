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

int pfa_wan_ip_fw_show(void)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_ip_fw_entry *entry = NULL;
    struct pfa_ip_fw_entry_priv *entry_priv = NULL;
    unsigned long flags = 0;
    int i = 0;
    spin_lock_irqsave(&pfa->ipfw.lock, flags);

    list_for_each_entry(entry_priv, &pfa->ipfw.wan_entry_list, wan_list)
    {
        entry = entry_priv->entry;
        printk("wan_dev %s", entry_priv->wan_dev->name);
        iptable_dump_entry(i, entry);
        i++;
    }

    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);

    return 0;
}
EXPORT_SYMBOL(pfa_wan_ip_fw_show);

void pfa_add_entry_update_info(struct pfa_ip_fw_entry *entry, struct pfa_add_entry_info *info)
{
    if (info->en.bits.stream_id_en) {
        entry->stream_id = info->stream_id;
    }
    if (info->en.bits.vlan_id_en) {
        entry->vid = info->vlan_id;
        entry->vlan_valid = info->en.bits.vlan_id_en;
    }
    if (info->en.bits.userfiledh_en) {
        entry->htab_usrfield_higher16 = info->userfiled_higher16;
    }
    if (info->en.bits.userfiledl_en) {
        entry->htab_usrfield_lower16 = info->userfiled_lower16;
    }

    if (info->in) {
        entry->priv->in = info->in;
    }

    pfa_add_entry_update_info_out_dev(entry, info);
}

void pfa_add_ipfw_entry_cb(struct pfa *pfa, struct sk_buff *skb, struct pfa_ip_fw_entry *entry, int dir, unsigned int hkey)
{
    struct pfa_add_entry_info info = {0};
    struct pfa_entry_flow_info flow_info = {0};
    int ret;

    if (pfa->ops.pfa_add_entry_cb) {
        flow_info.in = entry->priv->in;
        flow_info.out = entry->priv->out;
        flow_info.dir = dir;
        flow_info.hkey = hkey;
        ret = memcpy_s(flow_info.dhost, ETH_ALEN, entry->dhost, ETH_ALEN);
        if (ret) {
            PFA_ERR("memcpy dhost fail\n");
        }
        ret = memcpy_s(flow_info.shost, ETH_ALEN, entry->shost, ETH_ALEN);
        if (ret) {
            PFA_ERR("memcpy shost fail\n");
        }
        pfa->ops.pfa_add_entry_cb(skb, &flow_info, &info);
        pfa_add_entry_update_info(entry, &info);
    }
}

static void ipv4_tuple_show(struct nf_conntrack_tuple *tuple)
{
    unsigned int ip[4]; /* 4 words is ip add len */
    unsigned int ipall;
    unsigned int i;

    PFA_ERR("src_l3_num(%d) dst_l4_protonum(%d) ", tuple->src.l3num, tuple->dst.protonum);

    ipall = tuple->src.u3.ip;
    for (i = 0; i < 4; i++) { /* 4 words is ip add len */
        ip[i] = ipall & 0xff;
        ipall = ipall >> 8;  /* left shift 8  */
    }
    PFA_ERR("src *.*.%u.%u :(%u) \n", ip[2], ip[3], ntohs(tuple->src.u.all));  /* 2,3 ip addr */

    ipall = tuple->dst.u3.ip;
    for (i = 0; i < 4; i++) { /* 4 words is ip add len */
        ip[i] = ipall & 0xff;
        ipall = ipall >> 8;  /* left shift 8  */
    }
    PFA_ERR("dst *.*.%u.%u :(%u) \n", ip[2], ip[3], ntohs(tuple->dst.u.all)); /* 2,3 ip addr */

    return;
}

static void ipv6_tuple_show(struct nf_conntrack_tuple *tuple)
{
    PFA_ERR("l3num(%d) protonum(%d) %pI6:(%u) --> \n %pI6:(%u)\n", tuple->src.l3num, tuple->dst.protonum,
            &(tuple->src.u3.ip6), ntohs(tuple->src.u.all), &(tuple->dst.u3.ip6), ntohs(tuple->dst.u.all));
    return;
}

void tuple_show(struct nf_conntrack_tuple *tuple)
{
    if (AF_INET6 == tuple->src.l3num) {
        ipv6_tuple_show(tuple);
    } else {
        ipv4_tuple_show(tuple);
    }
}

void iptable_dump_entry(int idx, struct pfa_ip_fw_entry *pos)
{
    unsigned int ip[4]; /* 4 is ip add len */
    unsigned int ipall;
    unsigned int i;

    tuple_show(&pos->tuple);
    PFA_ERR("entry[%d]: valid(%u)  adddir(%d) vir(%lx) dma: (%lx) nextdma: (0x%x%x)\n", idx, pos->valid, pos->priv->dir,
            (uintptr_t)pos, (uintptr_t)pos->priv->dma, pos->next_higher, pos->next_lower);
    PFA_ERR("entry->portno %d\n", pos->portno);
    PFA_ERR("entry->net_id %d\n", pos->net_id);
    PFA_ERR("entry->action %d\n", pos->action);
    PFA_ERR("entry->vid %d\n", ntohs(pos->vid));
    PFA_ERR("entry->shost %x:%x:%x:%x:%x:%x\n", (unsigned int)pos->shost[0], (unsigned int)pos->shost[1],
            (unsigned int)pos->shost[2], (unsigned int)pos->shost[3], // 2,3 is addr shift
            (unsigned int)pos->shost[4], (unsigned int)pos->shost[5]); // 4,5 is addr shift

    PFA_ERR("entry->dhost %x:%x:%x:%x:%x:%x\n", (unsigned int)pos->dhost[0], (unsigned int)pos->dhost[1],
            (unsigned int)pos->dhost[2], (unsigned int)pos->dhost[3], // 2,3 is addr shift
            (unsigned int)pos->dhost[4], (unsigned int)pos->dhost[5]); // 4,5 is addr shift

    ipall = pos->nat_src_ip;
    for (i = 0; i < 4; i++) { // 4 is ip addr word nums
        ip[i] = ipall & 0xff;
        ipall = ipall >> 8; // 8 bit, right shift
    }
    PFA_ERR("src_ip: %u.%u.%u.%u src_port %u\n", ip[0], ip[1], ip[2], ip[3], ntohs(pos->nat_src_port)); // 2,3addr shift
    ipall = pos->nat_dst_ip;
    for (i = 0; i < 4; i++) { // 4 is ip addr word nums
        ip[i] = ipall & 0xff;
        ipall = ipall >> 8; // 8 bit, right shift
    }
    PFA_ERR("dst_ip: %u.%u.%u.%u dst_port %u\n", ip[0], ip[1], ip[2], ip[3], ntohs(pos->nat_dst_port)); // 2,3addr shift

    PFA_ERR("entry->pdu_ssid %u\n", pos->pdu_ssid);
    PFA_ERR("entry->fc_head %u\n", pos->fc_head);
    PFA_ERR("entry->modem_id %u\n", pos->modem_id);
    PFA_ERR("entry->htab_usrfield_lower16 %x\n", pos->htab_usrfield_lower16);
    PFA_ERR("entry->htab_usrfield_higher16 %x\n", pos->htab_usrfield_higher16);
    PFA_ERR("entry->timestamp %x\n", pos->timestamp);
    PFA_ERR("entry->dead_timestamp %x\n", pos->dead_timestamp);
    PFA_ERR("entry->stream_id %x\n", pos->stream_id);
    PFA_ERR("entry->iptype %x\n", pos->iptype);
    PFA_ERR("entry->is_static %x\n", pos->is_static);
    PFA_ERR("entry->ttl_ind %x\n", pos->ttl_ind);
    PFA_ERR("entry->valid %x\n", pos->valid);
    PFA_ERR("entry->in %s \n", pos->priv->in ? pos->priv->in->name : "null");
    PFA_ERR("entry->out %s\n", pos->priv->out ? pos->priv->out->name : "null");

    PFA_ERR("priv list: prev %lx next %lx entry %lx hash 0x%x hkey %u  \n\n", (uintptr_t)pos->priv->list.prev,
            (uintptr_t)pos->priv->list.next, (uintptr_t)pos->priv->entry, pos->priv->hash, pos->priv->hkey);
}

void pfa_ipfw_list_del_entry(struct pfa *pfa, struct pfa_ip_fw_entry *pos, uint32_t hkey)
{
    struct pfa_ip_fw_entry *entry = pos;
    struct pfa_ip_fw_entry *prev_entry = NULL;
    struct pfa_ip_fw_entry_priv *prev_entry_priv = NULL;
    struct pfa_ip_fw_entry *head = NULL;
    dma_addr_t entry_dma;

    entry->next_higher = 0;
    entry->next_lower = 0;
    entry->valid = 0;
    entry->dead_timestamp = jiffies;
    prev_entry_priv = list_entry(entry->priv->list.prev, struct pfa_ip_fw_entry_priv, list);
    prev_entry = prev_entry_priv->entry;

    head = (struct pfa_ip_fw_entry *)(pfa->ipfw.hbucket + sizeof(struct pfa_ip_fw_entry) * hkey);
    head->priv->remove_cnt++;
    if (pfa->msg_level & PFA_MSG_TUPLE) {
        PFA_ERR("(DEL)pfa_ipfw_list_del_tuple start hkey =%u\n", hkey);
        PFA_ERR("head priv list: prev %lx next %lx entry %lx hash 0x%x hkey %u  \n", (uintptr_t)head->priv->list.prev,
                (uintptr_t)head->priv->list.next, (uintptr_t)head->priv->entry, head->priv->hash, head->priv->hkey);
        iptable_dump_entry(0, entry);
        PFA_ERR("(DEL)pfa_ipfw_list_del_tuple end\n \n \n");
        WARN_ON(1);
    }

    // check if this is the last entry in list;
    if (list_is_last(&entry->priv->list, &head->priv->list)) {
        prev_entry->next_higher = 0;
        prev_entry->next_lower = 0;
    } else {
        entry_dma = list_entry(entry->priv->list.next, struct pfa_ip_fw_entry_priv, list)->dma;
        prev_entry->next_higher = upper_32_bits(entry_dma);
        prev_entry->next_lower = lower_32_bits(entry_dma);
    }

    list_del_init(&entry->priv->list);
    list_del_init(&entry->priv->wan_list);
    return;
}

static inline int32_t ip_fw_node_recyclable(struct pfa *pfa, struct pfa_ip_fw_entry *ent)
{
    return time_after(jiffies, (unsigned long)ent->dead_timestamp + (unsigned long)pfa->ipfw.deadtime);
}

struct pfa_ip_fw_entry *ip_fw_node_get(struct pfa *pfa)
{
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_dma *pos_dma = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;
    dma_addr_t dma = 0;
    unsigned long flags;

    spin_lock_irqsave(&pfa->ipfw.free_lock, flags);
    if (!list_empty(&pfa->ipfw.free_list)) {
        priv_pos = list_first_entry(&pfa->ipfw.free_list, struct pfa_ip_fw_entry_priv, list);
        pos = priv_pos->entry;
        if (ip_fw_node_recyclable(pfa, pos)) {
            list_del_init(&pos->priv->list);
            list_del_init(&pos->priv->wan_list);
            pfa->ipfw.free_cnt--;
            spin_unlock_irqrestore(&pfa->ipfw.free_lock, flags);
            return pos;
        }
    }
    spin_unlock_irqrestore(&pfa->ipfw.free_lock, flags);

    pos_dma = (struct pfa_ip_fw_entry_dma *)dma_pool_zalloc(pfa->ipfw.hslab, GFP_ATOMIC, &dma);
    if (pos_dma == NULL) {
        return NULL;
    }
    pos = &pos_dma->entry;
    pos->priv = kzalloc(sizeof(struct pfa_ip_fw_entry_priv), GFP_ATOMIC);
    if (pos->priv == NULL) {
        dma_pool_free(pfa->ipfw.hslab, pos_dma, dma);
        return NULL;
    }
    pos->priv->entry = pos;
    pos->priv->dma = PFA_IP_FW_TO_ENTRY_OFFSET(dma);
    pos->priv->dir = IP_CT_DIR_MAX;
    pos->dead_timestamp = INITIAL_JIFFIES;

    INIT_LIST_HEAD(&pos->priv->list);
    INIT_LIST_HEAD(&pos->priv->wan_list);

    return pos;
}

// pfa_etry.c
void ip_fw_node_put(struct pfa *pfa, struct pfa_ip_fw_entry *ent, bool to_pool)
{
    unsigned long flags;
    dma_addr_t dma;
    struct pfa_ip_fw_entry_priv *priv = NULL;
    int ret;

    if (to_pool) {
        dma = ent->priv->dma;
        dma_pool_free(pfa->ipfw.hslab, container_of(ent, struct pfa_ip_fw_entry_dma, entry), PFA_IP_FW_TO_DMA_OFFSET(dma));
    } else {
        spin_lock_irqsave(&pfa->ipfw.free_lock, flags);
        priv = ent->priv;
        ret = memset_s(ent, sizeof(*ent), 0, sizeof(struct pfa_ip_fw_entry));
        if (ret) {
            PFA_ERR("memset_s ip fw entry failed\n");
        }
        ent->priv = priv;
        ent->dead_timestamp = INITIAL_JIFFIES;
        list_add_tail(&ent->priv->list, &pfa->ipfw.free_list);
        pfa->ipfw.free_cnt++;

        if (pfa->ipfw.free_cnt > pfa->ipfw.free_threhold) {
            struct pfa_ip_fw_entry *pos = NULL;
            struct pfa_ip_fw_entry_priv *priv_pos = NULL;
            struct pfa_ip_fw_entry_priv *n = NULL;

            list_for_each_entry_safe(priv_pos, n, &(pfa->ipfw.free_list), list)
            {
                pos = priv_pos->entry;
                if (ip_fw_node_recyclable(pfa, pos)) {
                    list_del_init(&pos->priv->list);
                    list_del_init(&pos->priv->wan_list);
                    pfa->ipfw.free_cnt--;
                    dma = pos->priv->dma;
                    kfree(pos->priv);
                    dma_pool_free(pfa->ipfw.hslab, container_of(pos, struct pfa_ip_fw_entry_dma, entry), PFA_IP_FW_TO_DMA_OFFSET(dma));
                } else {
                    break;
                }
            }
        }

        spin_unlock_irqrestore(&pfa->ipfw.free_lock, flags);
    }
}

static inline unsigned compare_ipall_addr(const uint32_t *a, const uint32_t *b)
{
    return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | (a[2] ^ b[2]) | (a[3] ^ b[3])) == 0; /* 2,3 is array no  */
}

bool ip_fw_tuple_compare(struct nf_conntrack_tuple *a, struct nf_conntrack_tuple *b)
{
    if (compare_ipall_addr(a->src.u3.all, b->src.u3.all) && compare_ipall_addr(a->dst.u3.all, b->dst.u3.all) &&
        (a->src.u.all == b->src.u.all) && (a->src.l3num == b->src.l3num) && (a->dst.u.all == b->dst.u.all) &&
        (a->dst.protonum == b->dst.protonum)) {
        return true;
    }
    return false;
}

struct pfa_ip_fw_entry *ip_fw_entry_exist(struct pfa *pfa, struct nf_conntrack_tuple *tuple, uint32_t hkey)
{
    struct pfa_ip_fw_entry *entry = pfa->ipfw.hbucket;
    struct list_head *head = &entry[hkey].priv->list;
    struct pfa_ip_fw_entry *first = &entry[hkey];
    struct pfa_ip_fw_entry *pos = NULL;
    struct pfa_ip_fw_entry_priv *priv_pos = NULL;

    /* check if first header exist */
    if (first->valid && ip_fw_tuple_compare(&first->tuple, tuple)) {
        return first;
    }

    /* check if ip_fw exist */
    list_for_each_entry(priv_pos, head, list)
    {
        pos = priv_pos->entry;
        if (pos->valid && ip_fw_tuple_compare(&pos->tuple, tuple)) {
            return pos;
        }
    }

    return NULL;
}

unsigned int pfa_ipfw_get_hash(struct nf_conntrack_tuple *tuple, u16 zone)
{
    unsigned int n;
    u32 hash;

    n = (sizeof(tuple->src) + sizeof(tuple->dst.u3)) / sizeof(u32);
    hash = jhash2((u32 *)tuple, n, zone ^ 0 ^ (((__force __u16)tuple->dst.u.all << 16) | tuple->dst.protonum));

    return hash;
}
EXPORT_SYMBOL(pfa_ipfw_get_hash);

void ip_fw_list_add(struct pfa *pfa, struct pfa_ip_fw_entry *entry, uint32_t hkey)
{
    void *head_addr = pfa->ipfw.hbucket + hkey * sizeof(struct pfa_ip_fw_entry);
    struct pfa_ip_fw_entry *head = (struct pfa_ip_fw_entry *)head_addr;
    struct pfa_ip_fw_entry_priv *next_entry_priv = NULL;
    unsigned long flags;
    dma_addr_t next_entry_dma;
    spin_lock_irqsave(&pfa->ipfw.lock, flags);

    list_add(&entry->priv->list, &head->priv->list); /* add entry after first node */
    if (list_is_last(&entry->priv->list, &head->priv->list)) {
        entry->next_higher = 0; /* real add to list */
        entry->next_lower = 0;  /* real add to list */
    } else {
        next_entry_priv = list_entry(entry->priv->list.next, struct pfa_ip_fw_entry_priv, list);
        next_entry_dma = next_entry_priv->dma;
        entry->next_higher = upper_32_bits(next_entry_dma); /* real add to list */
        entry->next_lower = lower_32_bits(next_entry_dma);  /* real add to list */
    }
    entry->valid = 1;

    head->next_higher = upper_32_bits(entry->priv->dma); /* real add to list */
    head->next_lower = lower_32_bits(entry->priv->dma);  /* real add to list */
    head->priv->hkey = entry->priv->hkey;
    head->priv->add_cnt++;
    pfa->stat.ipfw_add++;
    entry->priv->entry = entry;
    if (pfa->msg_level & PFA_MSG_TUPLE) {
        PFA_ERR("(ADD)ip_fw_list_add start\n");
        PFA_ERR("entry->priv 0x%lx\n", (uintptr_t)entry->priv);
        PFA_ERR("head priv list: prev %lx next %lx entry %lx hash 0x%x hkey %u \n", (uintptr_t)head->priv->list.prev,
                (uintptr_t)head->priv->list.next, (uintptr_t)head->priv->entry, head->priv->hash, head->priv->hkey);
        if (head->priv->dma) {
            PFA_ERR("head->priv->dma = %llx\n", head->priv->dma);
        }
        iptable_dump_entry(0, entry);
        PFA_ERR("(ADD)ip_fw_list_add end\n\n\n");
    }
    spin_unlock_irqrestore(&pfa->ipfw.lock, flags);

    return;
}
EXPORT_SYMBOL(ip_fw_list_add);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
