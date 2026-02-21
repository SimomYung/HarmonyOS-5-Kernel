/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include <linux/if_vlan.h>
#include <linux/random.h>
#include <securec.h>

#include <mdrv_pfa.h>
#include <bsp_dra.h>
#include <bsp_pfa.h>
#include "bsp_diag.h"
#include "bsp_slice.h"
#include <bsp_net_om.h>
#include "pfa_desc.h"
#include "pfa_port.h"
#include "pfa_dbg.h"
#include "pfa_direct_fw.h"
#include <asm/barrier.h>
#include <bsp_syscache.h>
#include "pfa_interrupt.h"
#include "pfa_tft.h"
#include "pfa_ip_entry.h"

#ifdef CONFIG_PFA_XFRM
#include "pfa_xfrm.h"
#endif

#include <linux/timer.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define PFA_RD_FLAG_MAGIC 0x53504500
#define PFA_RD_AVG_GET_TIMES 2
#define PFA_RD_FLAG_SPORT_MASK 0xf0
#define PFA_RD_FLAG_MASK 0xffffff00

#define PFA_PTS_IFACE_ID_MASK   0x0000000f
#define PFA_PTS_PKT_ID_MASK   0xfffffff0
#define PFA_PTS_PKT_ID_OFFSET   4
#define PFA_PKT_IFCAE_ID  1
#define PFA_PKT_ID_MAX   0xfffffff
unsigned int g_pfa_pkt_id = 1;
unsigned int g_pfa_latency_rpt_en = 0;

#define PFA_AD_SIZE_MULTIPLES_OF_32(x) ((x) / 32)
unsigned int pfa_get_adq_size_sel(unsigned int adq_size)
{
    return (unsigned int)__ilog2_u32(PFA_AD_SIZE_MULTIPLES_OF_32(adq_size));
}

void __attribute__((weak)) pfa_checksum(struct pfa *pfa, unsigned long long dra_l2_addr, unsigned int len) {;}

/* desc number occupied by hardware */
static inline int pfa_busy_td_num(struct pfa *pfa, int portno)
{
    struct pfa_port_ctrl *ctrl = &pfa->ports[portno].ctrl;
    int ret;

    ret = (ctrl->td_free >= ctrl->td_busy) ? (ctrl->td_free - ctrl->td_busy)
                                            : (ctrl->td_depth - ctrl->td_busy + ctrl->td_free);
    return ret;
}

/* desc number can be used by software */
static inline int pfa_free_td_num(struct pfa *pfa, int portno)
{
    struct pfa_port_ctrl *ctrl = &pfa->ports[portno].ctrl;
    int ret;

    /* 1 for distinguish full and empty */
    ret = ctrl->td_depth - 1 - pfa_busy_td_num(pfa, portno);
    return ret;
}

void pfa_free_dra_mem(struct pfa *pfa, unsigned int portno, unsigned long long dra_addr)
{
    struct sk_buff *skb = NULL;
    unsigned long long dra_org_addr = 0;

    skb = bsp_dra_to_skb(dra_addr, &dra_org_addr);
    if (unlikely((skb == NULL) || (dra_org_addr == 0))) {
        PFA_ERR("portno %u td free dra 0x%llx fail \n", portno, dra_addr);
        pfa_bug(pfa);
        return;
    }
    bsp_dra_free(dra_org_addr);

    return;
}
static inline void pfa_update_pfa_tft_rptr(struct pfa *pfa, unsigned int td_rptr)
{
    pfa_writel(pfa->pfa_tftport.pfa_tft_rd_rptr_addr, 0, td_rptr);
    return;
}

static inline bool pfa_check_sport_is_modem(struct pfa *pfa, struct pfa_rd_desc *desc)
{
    if (pfa->pfa_version == PFA_VERSION_V200) {
        return desc->rd_sport >= PFA_CCORE_PORT_BEGIN && desc->rd_sport < PFA_PORT_NUM;
    } else {
        return desc->rd_sport == pfa->pfa_tftport.portno;
    }
}

/*
 * pfav200:software check td fail && push rptr to pfa_tft
 *         if td fail, do not produce rd desc.need free mem at td desc.
 *         for pfa_tft port, this operation is completed before pushing the pointer.
 * pfav300, Because the hardware pushes the pointer to pfa_tft, the software only handles td failures except pfa_tft port
 *          if pfa_tft port td fail, produce rd desc.need free mem at cpu rd desc.
 *          if other port,td fail, do not produce rd desc.need free mem at other port desc.
 */
unsigned int __pfa_process_td_desc(struct pfa *pfa, unsigned int portno, unsigned int td_rptr)
{
    struct pfa_port_ctrl *ctrl = &pfa->ports[portno].ctrl;
    struct pfa_port_stat *stat = &pfa->ports[portno].stat;
    struct pfa_td_desc *td_base = (struct pfa_td_desc *)ctrl->td_addr;
    struct pfa_td_desc *td_cur = NULL;
    unsigned int idx;
    unsigned int td_num = 0;

    if (ctrl->td_busy < td_rptr) {
        td_num = td_rptr - ctrl->td_busy;
    } else {
        td_num = ctrl->td_depth + td_rptr - ctrl->td_busy;
    }

    /* recycle td from td_busy to tdq rptr */
    for (idx = ctrl->td_busy; idx != td_rptr;) {
        td_cur = &td_base[idx];
        if (pfa->td_fail_generate_rd) {
            stat->td_pkt_complete++;
        } else {
            pfa->hal->check_td_result(pfa, portno, td_cur, stat);
        }
        stat->td_desc_complete++;

        if (unlikely(ctrl->ops.pfa_finish_td)) {
            ctrl->ops.pfa_finish_td(ctrl->td_param[idx], ctrl->port_priv);
        }
        idx = ((++idx) < ctrl->td_depth) ? idx : 0;
    }
    ctrl->td_busy = idx;

    return td_num;
}
/*
 * Function is only called in interrupt(pfa->smp.process_desc), No concurrency scenarios, no need to lock
 */
unsigned int pfa_process_td_desc(unsigned int portno)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_port_ctrl *ctrl = &pfa->ports[portno].ctrl;
    struct pfa_port_stat *stat = &pfa->ports[portno].stat;
    unsigned int td_rptr;
    unsigned int td_num = 0;

    /* get tdq rptr */
    td_rptr = pfa_readl(pfa->regs, PFA_TDQX_RPTR(portno));
    if (unlikely(td_rptr >= ctrl->td_depth)) { // security check
        PFA_ERR("td_rptr reg is error\n");
        return 0;
    }
    if (ctrl->td_busy == td_rptr) {
        return 0;
    }
    stat->td_finsh_intr_complete++;
    td_num = __pfa_process_td_desc(pfa, portno, td_rptr);

    return td_num;
}

void pfa_enable_port_bypass(unsigned int enable, unsigned int sport, unsigned int dport, unsigned int mac_ursp_with)
{
    struct pfa *pfa = &g_pfa;
    pfa->ports[sport].ctrl.bypassport_en = enable;
    pfa->ports[sport].ctrl.bypassport = dport;
    pfa->ports[sport].ctrl.with_mac_pdu = mac_ursp_with;
}

void pfa_pts_rpt(unsigned int pkt_id, enum fw_pos pos)
{
    struct packet_time_stamp time_info;

    if (g_pfa_latency_rpt_en != 0) {
        time_info.pkt_id.iface_id = pkt_id & PFA_PTS_IFACE_ID_MASK;
        time_info.pkt_id.id = (pkt_id & PFA_PTS_PKT_ID_MASK) >> PFA_PTS_PKT_ID_OFFSET;
        time_info.pos = pos;
        time_info.time_stamp = bsp_get_slice_value();

        bsp_diag_trans_report(LATENCY_MSGID_PFA, DIAG_DRV_HDS_PID, (unsigned char *)&time_info, sizeof(time_info));
    }
}

void pfa_pdcp_cnt_rpt(unsigned int pkt_id, unsigned int pdcp_cnt)
{
    struct packet_pdcp_cnt pdcp_cnt_info;

    if (g_pfa_latency_rpt_en != 0) {
        pdcp_cnt_info.pkt_id.iface_id = pkt_id & PFA_PTS_IFACE_ID_MASK;
        pdcp_cnt_info.pkt_id.id = (pkt_id & PFA_PTS_PKT_ID_MASK) >> PFA_PTS_PKT_ID_OFFSET;
        pdcp_cnt_info.pdcp_cnt = pdcp_cnt;

        bsp_diag_trans_report(LATENCY_PDCP_CNT, DIAG_DRV_HDS_PID, (unsigned char *)&pdcp_cnt_info, sizeof(pdcp_cnt_info));
    }
}

void pfa_set_latency_rpt(unsigned int enable)
{
    g_pfa_latency_rpt_en = enable;
    PFA_ERR("[PFA]: packet time stamp report enable is %d!\n", g_pfa_latency_rpt_en);
}

void pfa_td_config_non_cpuport_desc(struct pfa *pfa, struct pfa_port_ctrl *port_ctrl, struct pfa_td_desc *desc,
    struct pfa_td_user_info *td_user_info)
{
    struct rx_cb_map_s *cb_info = NULL;

    cb_info = (struct rx_cb_map_s *)(((struct sk_buff *)td_user_info->param)->cb);
    /* word 0 */
    desc->td_fc_head = 0;
    desc->td_push_en = !!(td_user_info->flags & PFA_TD_PUSH_USB);
    desc->td_high_pri_flag = 1;  // for pfa_tft; high priority packet flag

    desc->td_stick_en = port_ctrl->stick_en;
    /* word4 */
    desc->td_bypass_en = !!(port_ctrl->bypassport_en);
    desc->td_bypass_addr = port_ctrl->bypassport;
    desc->td_with_mac_pdu_rst = port_ctrl->with_mac_pdu;

    if (pfa->usbport.portno == port_ctrl->portno) {
        pfa_pts_rpt(cb_info->userfield2, PFA_UP_RCV);
        desc->td_net_id = cb_info->packet_info.bits.net_id;
        desc->td_user_field2 = cb_info->userfield2;
        desc->td_user_field1 = PFA_SET_PORT_TO_USERFIELD(desc->td_user_field1, port_ctrl->portno);
        desc->td_user_field1 |= PFA_SPORT_FREE_ORI_ADDR_FLAG;
    } else if (port_ctrl->portno == PFA_ACORE_LAN_CTRL_ID) {
        desc->td_user_field1 = cb_info->userfield1;
        desc->td_user_field0 = virt_to_phys(((struct sk_buff *)td_user_info->param)->dev);
        desc->td_user_field2 = cb_info->userfield2;
    } else {
        desc->td_user_field1 = cb_info->userfield1;
        desc->td_user_field1 = PFA_SET_PORT_TO_USERFIELD(desc->td_user_field1, port_ctrl->portno);
        if (g_pfa_latency_rpt_en != 0) {
            desc->td_user_field2 = cb_info->userfield2;
        }
        pfa_pts_rpt(cb_info->userfield2, PFA_UP_RCV);
        if (port_ctrl->port_multiple_en == 1) {
            desc->td_user_field2 = td_user_info->net_id;
        }
        desc->td_net_id = td_user_info->net_id;
    }
    return;
}

inline void pfa_config_bypass_dpa_td(struct pfa_td_desc *desc, struct pfa_td_user_info *td_user_info, struct wan_info_s *wan_info)
{
    desc->td_bypass_en = 1;
    desc->td_bypass_addr = td_user_info->portno;
    desc->td_user_field0 = wan_info->userfield0;
    desc->td_user_field1 = wan_info->userfield1;
}

void pfa_td_config_cpuport_desc(struct pfa *pfa, struct pfa_td_desc *desc, struct pfa_td_user_info *td_user_info)
{
    struct wan_info_s *wan_info = NULL;

    wan_info = (struct wan_info_s *)(((struct sk_buff *)td_user_info->param)->cb);
    if (td_user_info->bypass_to_dpa) {
        return pfa_config_bypass_dpa_td(desc, td_user_info, wan_info);
    }

    /* word0 */
    desc->td_fc_head = wan_info->info.fc_head;
    desc->td_high_pri_flag = wan_info->info.higi_pri_flag;

    pfa->hal->config_td_tft_mode(wan_info, desc);

    /* word3 */
    desc->td_pdu_ssid = wan_info->info.pdu_session_id;
    desc->td_modem_id = wan_info->info.modem_id;
    desc->v2x_ind = wan_info->info.v2x_ind;
    desc->pkt_ind = wan_info->info.pkt_ind;
    desc->td_with_mac_pdu_rst = wan_info->info.with_mac_pdu;
    desc->bypass_pkt_type = wan_info->info.bypass_dst_pkt_ind;

    /* word4 */
    desc->td_bypass_en = 1;
    desc->td_bypass_addr = pfa->pfa_tftport.portno;
    desc->td_host_ana = wan_info->info.parse_en;

    /* word7-9 */
    desc->td_user_field0 = wan_info->userfield0;
    desc->td_user_field1 = wan_info->userfield1;

    return;
}
void pfa_td_config_one(struct pfa_port_ctrl *port_ctrl, struct pfa_td_user_info *td_user_info, int portno)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_td_desc *cur_desc = (struct pfa_td_desc *)port_ctrl->td_addr;
    struct pfa_td_desc *desc = port_ctrl->axi_td;
    int ret;

    cur_desc = cur_desc + port_ctrl->td_free;

    ret = memset_s(desc, sizeof(*desc), 0, sizeof(struct pfa_td_desc));
    if (unlikely(ret)) {
        PFA_ERR("memset_s fail.\n");
    }

    // word 0
    desc->td_irq_en = 1;
    desc->td_mode = 1;
    desc->tft_filter_ind = 1;
    desc->td_pkt_len = td_user_info->len;
    desc->td_int_en = 1;
    desc->td_push_en = td_user_info->flags & PFA_TD_KICK_PKT;

    if (port_ctrl->td_copy_en) {
        desc->td_copy_en = PFA_PORT_TD_COPY_ENABLE;
    }
    /* word1-2 packet input header, suppose to point at ip header */
    desc->td_inaddr_lower = lower_32_bits(td_user_info->dra_l2_addr);
    desc->td_inaddr_upper = upper_32_bits(td_user_info->dra_l2_addr);

    /* word4 */
    desc->td_net_id = 0;

    td_user_info->portno = portno;

    if (pfa->cpuport.portno != port_ctrl->portno) {
        pfa_td_config_non_cpuport_desc(pfa, port_ctrl, desc, td_user_info);
    } else {
        pfa_td_config_cpuport_desc(pfa, desc, td_user_info);
    }

    ret = memcpy_s(cur_desc, sizeof(*cur_desc), desc, sizeof(struct pfa_td_desc));
    if (unlikely(ret)) {
        PFA_ERR("memcpy_s fail.\n");
    }
}

static int pfa_td_param_check(struct pfa *pfa, int portno, unsigned long long dra_l2_addr, void *param)
{
    if (unlikely(portno >= PFA_PORT_NUM || !test_bit(portno, &pfa->portmap[PFA_PORTS_NOMARL]))) {
        PFA_ERR_ONCE("port %d not exist!\n", portno);
        pfa_bug(pfa);
        return -EFAULT;
    }

    if (unlikely(!dra_l2_addr)) {
        pfa->ports[portno].stat.td_dma_null++;
        PFA_ERR_ONCE("port %d dra_l2_addr zero null!\n", portno);
        pfa_bug(pfa);
        return -EFAULT;
    }

    return 0;
}

static inline void pfa_td_user_info_init(struct pfa_td_user_info *user_info, unsigned long long dra_l2_addr,
    unsigned int len, void *param, unsigned int td_flags, int bypass_to_dpa)
{
    user_info->dra_l2_addr = dra_l2_addr;
    user_info->len = len;
    user_info->param = param;
    user_info->flags = td_flags;
    user_info->bypass_to_dpa = bypass_to_dpa;
}
// last pkt must push! add check;
int pfa_config_td(int vir_portno, unsigned long long dra_l2_addr, unsigned int len, void *param, unsigned int td_flags, int bypass_to_dpa)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_port_ctrl *ctrl = NULL;
    struct pfa_port_stat *stat = NULL;
    unsigned long flags;
    int portno = bypass_to_dpa ? 0 : PFA_GET_PHY_PORT(vir_portno);
    struct pfa_td_user_info td_user_info = {0};

    if (g_pfa.init == 0) {
        PFA_ERR("bsp_pfa_config_td fail, pfa no init\n");
        return -1;
    }

    if (pfa_td_param_check(pfa, portno, dra_l2_addr, param) != 0) {
        return -EFAULT;
    }

    ctrl = &pfa->ports[portno].ctrl;
    stat = &pfa->ports[portno].stat;

    if (unlikely(!ctrl->enable)) {
        stat->td_port_disabled++;
        PFA_ERR_ONCE("td config fail port %d still disabled! \n", portno);
        return -EIO;
    }

    if (unlikely(pfa->flags & PFA_FLAG_SUSPEND)) {
        PFA_ERR_ONCE("td config port %d when pfa suspend! \n", portno);
        return -EIO;
    }

    spin_lock_irqsave(&ctrl->td_lock, flags);

    if (unlikely(pfa_free_td_num(pfa, portno) == 0)) {
        spin_unlock_irqrestore(&ctrl->td_lock, flags);
        stat->td_full++;
        return -EBUSY;
    }
#ifndef CONFIG_PFA_PHONE_SOC
    if (pfa->soft_push) { /* evide dra hard error */
        pfa_update_adq_wptr(pfa);
    }
#endif
    ctrl->td_param[ctrl->td_free] = param;
    td_user_info.net_id = PFA_NET_ID_BY_VIR_PORT(vir_portno);
    pfa_td_user_info_init(&td_user_info, dra_l2_addr, len, param, td_flags, bypass_to_dpa);
    pfa_td_config_one(ctrl, &td_user_info, PFA_GET_PHY_PORT(vir_portno));

    ctrl->td_free = (++ctrl->td_free < ctrl->td_depth) ? ctrl->td_free : 0;

    if (unlikely(td_flags & PFA_TD_KICK_PKT)) {
        stat->td_kick++;
        pfa_writel(pfa->regs, PFA_TDQX_WPTR(portno), ctrl->td_free);
    }

    stat->td_config++;
    stat->td_config_bytes += len;

    spin_unlock_irqrestore(&ctrl->td_lock, flags);

    return 0;
}

int bsp_pfa_config_td(int vir_portno, unsigned long long dra_l2_addr, unsigned int len, void *param, unsigned int td_flags)
{
    if (unlikely(param == NULL) ) {
        PFA_ERR_ONCE("port %d, param  is null. \n", vir_portno);
        return -EFAULT;
    }
    return pfa_config_td(vir_portno, dra_l2_addr, len, param, td_flags, 0);
}

int bsp_pfa_config_bd_bypass_to_dpa(int vir_portno, unsigned long long dra_l2_addr, unsigned int len, struct sk_buff *skb)
{
    return pfa_config_td(vir_portno, dra_l2_addr, len, skb, PFA_TD_KICK_PKT, 1);
}

static inline unsigned int pfa_get_net_id(struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc)
{
    if (unlikely(desc->rd_net_id > PFA_PORT_NUM)) {
        port_ctx->stat.net_id_err++;
        return 0;
    }
    return desc->rd_net_id;
}

static inline void pfa_rd_transmit_desc(struct pfa_port_ctx *port_ctx, struct sk_buff *skb, struct pfa_rd_desc *desc)
{
    struct pfa_ops *ops = &port_ctx->ctrl.ops;
    unsigned int net_id = 0;
    void *port_priv = port_ctx->ctrl.port_priv;

    if (port_ctx->ctrl.port_multiple_en) {
        net_id = pfa_get_net_id(port_ctx, desc);
        ops = &port_ctx->ctrl.net_map[net_id].ops;
        port_priv = port_ctx->ctrl.net_map[net_id].port_priv;
    }
    ops->pfa_finish_rd(skb, skb->len, port_priv, net_id);
    port_ctx->stat.rd_sended++;
    port_ctx->ctrl.net_map[net_id].pkt_cnt++;

    return;
}

static inline void pfa_set_cb_packet_info(struct rx_cb_map_s *rx_cb, unsigned int ip_proto,
    unsigned int l4_proto, unsigned int is_accable)
{
    rx_cb->packet_info.bits.ip_proto = ip_proto;
    rx_cb->packet_info.bits.l4_proto = l4_proto;
    rx_cb->packet_info.bits.is_accable = is_accable;
}

static inline void pfa_rd_set_skb(struct pfa *pfa, struct sk_buff *skb, struct pfa_rd_desc *desc, unsigned long long dra_l2_addr,
    unsigned long long org_dra_addr, unsigned int unmaped)
{
    struct rx_cb_map_s *rx_cb = NULL;

    rx_cb = (struct rx_cb_map_s *)(&skb->cb);
    rx_cb->packet_info.u32 = 0;

    switch (desc->rd_pkt_type) {
        case (RD_PKT_IPV4_TCP):
            pfa_set_cb_packet_info(rx_cb, AF_INET, IPPROTO_TCP, PFA_ACCELETABLE_PKT_FLAG);
            break;
        case (RD_PKT_IPV4_UDP):
            pfa_set_cb_packet_info(rx_cb, AF_INET, IPPROTO_UDP, PFA_ACCELETABLE_PKT_FLAG);
            break;
        case (RD_PKT_IPV6_TCP):
            pfa_set_cb_packet_info(rx_cb, AF_INET6, IPPROTO_TCP, PFA_ACCELETABLE_PKT_FLAG);
            break;
        case (RD_PKT_IPV6_UDP):
            pfa_set_cb_packet_info(rx_cb, AF_INET6, IPPROTO_UDP, PFA_ACCELETABLE_PKT_FLAG);
            break;
        default:
            // because of the condition to get into this branch, default can not be reached.
            break;
    }

    rx_cb->packet_info.bits.net_id = desc->rd_net_id;
    pfa->hal->set_tft_result(rx_cb, desc);

    rx_cb->userfield0 = desc->rd_user_field0;
    rx_cb->userfield1 = desc->rd_user_field1;
    rx_cb->userfield2 = desc->rd_user_field2;
    rx_cb->dra_org = org_dra_addr;
    rx_cb->dra_l2 = dra_l2_addr;
    rx_cb->packet_info.bits.l2_hdr_offeset = desc->rd_l2_hdr_offset;
    rx_cb->packet_info.bits.unmapped = unmaped;
}

#ifdef CONFIG_PFA_FW
unsigned int pfa_get_skb_update_only_value(struct sk_buff *skb)
{
    return skb->update_only;
}

void pfa_set_skb_update_only_value(struct sk_buff *skb, unsigned int value)
{
    skb->update_only = value;
}

#else
unsigned int pfa_get_skb_update_only_value(struct sk_buff *skb)
{
    return 0;
}

void pfa_set_skb_update_only_value(struct sk_buff *skb, unsigned int value)
{
    return;
}
#endif
struct sk_buff *pfa_resolve_skb(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr, bool to_tcp_ip)
{
    unsigned int dra_addr_type;
    unsigned long long org_dra_addr = 0;
    struct sk_buff *skb = NULL;
    int ret;

    skb = bsp_dra_to_skb(dra_l2_addr, &org_dra_addr);
    if (unlikely((skb == NULL) || (org_dra_addr == 0))) {
        port_ctx->stat.dra_to_skb_fail++;
        PFA_ERR_ONCE("%u time dra to skb fail, sport %u dport %u, dra 0x%llx skb %lx \n",
                     port_ctx->stat.dra_to_skb_fail, desc->rd_sport, port_ctx->ctrl.portno, dra_l2_addr, (uintptr_t)skb);
        pfa_bug(pfa);
        return NULL;
    }

    dra_addr_type = bsp_dra_get_addr_type(dra_l2_addr);
    if (to_tcp_ip) {
        skb = bsp_dra_skb_unmap(org_dra_addr);
        if (unlikely(skb == NULL)) {
            PFA_ERR_ONCE("sport %u unmap dra skb ERR! dra 0x%llx org 0x%llx \n", desc->rd_sport, dra_l2_addr,
                         org_dra_addr);
            pfa_bug(pfa);
            return NULL;
        }
    }

    ret = pfa->hal->construct_skb(port_ctx, desc, skb, org_dra_addr, dra_l2_addr, dra_addr_type);
    if (ret) {
        PFA_ERR("pfa construct skb fail. \n");
        return NULL;
    }

    if (to_tcp_ip) {
        bsp_sc_dma_unmap_single(pfa->dev, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
    }
    pfa_rd_set_skb(pfa, skb, desc, dra_l2_addr, org_dra_addr, to_tcp_ip);

    return skb;
}

static inline void pfa_cpu_rd_check_update_only(struct pfa *pfa, struct pfa_rd_desc *desc, struct sk_buff *skb)
{
    bool flag = (desc->rd_trans_result == 0); // 0: update only

    if (pfa->pfa_version == PFA_VERSION_V200) {
        flag = (desc->rd_drop_rsn == RD_RESULT_UPDATA_ONLY &&
            desc->rd_trans_result == 0); // 0: trans success
    }
#ifdef CONFIG_PFA_FW
    if (unlikely(flag)) {
        pfa->cpuport.cpu_updonly++;
        skb->update_only = 1;
    }
#endif
}

static unsigned int get_pkt_id(void)
{
    unsigned int pkt_id;
    if (g_pfa_latency_rpt_en != 0) {
        pkt_id = PFA_PKT_IFCAE_ID & PFA_PTS_IFACE_ID_MASK;
        g_pfa_pkt_id++;
        if (g_pfa_pkt_id == PFA_PKT_ID_MAX) {
            g_pfa_pkt_id = 1;
        }
        pkt_id |= (g_pfa_pkt_id << PFA_PTS_PKT_ID_OFFSET);

        return pkt_id;
    }
    return -1;
}

static inline bool pfa_check_direct_fw_drop(struct pfa *pfa, struct pfa_rd_desc *desc)
{
    if (pfa->direct_fw.dfw_flags.enable == 0) {
        return false;
    }

    if (pfa->pfa_version == PFA_VERSION_V200) {
        if ((desc->rd_trans_result == 0) && (desc->rd_drop_rsn == 1)) {
            return true;
        }
    } else if (desc->rd_trans_result == 0) {
        return true;
    }
    return false;
}

/* phone process skb */
struct sk_buff *pfa_resolve_pfa_tft_skb(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr)
{
    struct sk_buff *skb = NULL;
    unsigned long long org_dra_addr;
    unsigned int offset = desc->rd_l2_hdr_offset;

    if (pfa->pfa_version == PFA_VERSION_V200) {
        offset = desc->pkt_dra_offset;
    }

    skb = bsp_dra_unmap_reuse(dra_l2_addr, &org_dra_addr);
    if (unlikely((skb == NULL) || (org_dra_addr < dra_l2_addr))) {
        port_ctx->stat.dra_to_skb_fail++;
        PFA_ERR_ONCE("%u time dra to skb fail, sport %u dport %u, dra 0x%llx skb %lx \n",
                     port_ctx->stat.dra_to_skb_fail, desc->rd_sport, port_ctx->ctrl.portno, dra_l2_addr, (uintptr_t)skb);
        pfa_bug(pfa);
        return NULL;
    }
    bsp_dra_reset_skb(skb);

    if (pfa->pfa_version == PFA_VERSION_V0 || pfa->pfa_version == PFA_VERSION_V100) {
        skb_put(skb, desc->rd_pkt_len - offset);
    } else {
        skb->data = phys_to_virt(bsp_dra_to_phy(dra_l2_addr));
        skb->len = 0;
        skb_reset_tail_pointer(skb);
        skb_put(skb, desc->rd_pkt_len);
    }

    pfa_rd_set_skb(pfa, skb, desc, dra_l2_addr, org_dra_addr, 1);

    bsp_sc_dma_unmap_single(pfa->dev, bsp_dra_to_phy(org_dra_addr) - PFA_CHECKSUM_OFFSET,
        skb->len + virt_to_phys(skb->data) - bsp_dra_to_phy(org_dra_addr) + PFA_CHECKSUM_OFFSET, DMA_FROM_DEVICE);

    return skb;
}

void pfa_rd_process_cpu_wan(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr)
{
    struct pfa_cpuport_ctx *cpuport = &pfa->cpuport;
    struct sk_buff *skb = NULL;
    struct rx_cb_map_s *rx_cb = NULL;
    unsigned int pdcp_cnt;

#ifdef CONFIG_PFA_FW
    skb = pfa_resolve_skb(pfa, port_ctx, desc, dra_l2_addr, false);
#else
    skb = pfa_resolve_pfa_tft_skb(pfa, port_ctx, desc, dra_l2_addr);
    if (unlikely(pfa->wakeup_flag == TRUE)) {
        PFA_ERR("first dl rd pkt after wakeup ! \n");
        if (skb != NULL) {
            pfa_print_pkt_info(skb->data);
        }
        pfa->wakeup_flag = FALSE;
    }
#endif
    if (unlikely(skb == NULL)) {
        cpuport->cpu_rd_to_wan_fail++;
        pfa->ports[cpuport->portno].stat.rd_droped++;
        return;
    }

    if (unlikely(g_pfa_latency_rpt_en != 0)) {
        rx_cb = (struct rx_cb_map_s *)(&skb->cb);
        rx_cb->pkt_id = get_pkt_id();
        pdcp_cnt = pfa->hal->get_pdcp_cnt(desc);
        pfa_pdcp_cnt_rpt(rx_cb->pkt_id, pdcp_cnt);
    }

    pfa_cpu_rd_check_update_only(pfa, desc, skb);
    cpuport->cpu_rd_to_wan++;

    if (pfa_check_direct_fw_drop(pfa, desc)) {
        bsp_dra_free(dra_l2_addr);
        pfa->cpuport.cpu_rd_dfw_updonly_drop++;
        return;
    }

    pfa_rd_transmit_desc(&pfa->ports[cpuport->portno], skb, desc);

    return;
}

static void pfa_rd_cpu_lan_to_netif_rx(struct pfa *pfa, struct net_device *net, struct pfa_port_ctx *src_port,
    struct sk_buff *skb, struct pfa_rd_desc *desc)
{
    int ret;

    dev_hold(net);
    if (src_port->ctrl.portno == PFA_ACORE_LAN_CTRL_ID) {
        skb->protocol = eth_type_trans(skb, phys_to_virt(desc->rd_user_field0));
    } else {
        skb->protocol = eth_type_trans(skb, net);
    }
    dev_put(net);

    if (pfa->smp.process_desc == pfa_process_desc_napi || pfa->smp.process_desc == pfa_process_desc_intr) {
        ret = netif_receive_skb(skb);
    } else {
        if (in_irq()) {
            ret = netif_rx(skb);
        } else {
            ret = netif_rx_ni(skb);
        }
    }
    if (ret == 0) {
        pfa->ports[pfa->cpuport.portno].stat.rd_sended++;
        pfa->cpuport.cpu_rd_to_netif_rx_succ++;
    } else {
        pfa->ports[pfa->cpuport.portno].stat.rd_droped++;
        pfa->cpuport.cpu_rd_to_netif_rx_fail++;
    }

    return;
}

static inline unsigned int pfa_get_muti_net_id(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc)
{
    unsigned int net_id;

    if (pfa->pfa_version == PFA_VERSION_V100 || pfa->pfa_version == PFA_VERSION_V0) {
        net_id = desc->rd_user_field2;
    } else {
        net_id = pfa_get_net_id(port_ctx, desc);
    }

    return net_id;
}

void pfa_rd_process_cpu_lan(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr)
{
    struct pfa_port_ctx *src_port = &pfa->ports[desc->rd_sport];
    struct pfa_cpuport_ctx *cpuport = &pfa->cpuport;
    struct sk_buff *skb = NULL;
    bool to_cpu = true;
    struct pfa_ops *ops = NULL;
    unsigned int net_id = 0;
    struct net_device *net = NULL;
    struct rx_cb_map_s *rx_cb = NULL;
    void *port_priv = NULL;

    if (desc->rd_sport == PFA_5GLAN_PORT) {
        src_port = &pfa->ports[PFA_GET_PORT_FORM_USERFIELD(desc->rd_user_field1)];
    }
    ops = &src_port->ctrl.ops;
    net = src_port->ctrl.net;
    port_priv = src_port->ctrl.port_priv;

    cpuport->cpu_rd_num++;
    if (src_port->ctrl.ops.pfa_netif_rx && src_port->ctrl.portno == pfa->usbport.portno) {
        to_cpu = false;
    }

    skb = pfa_resolve_skb(pfa, port_ctx, desc, dra_l2_addr, to_cpu);
    if (unlikely(skb == NULL)) {
        cpuport->cpu_rd_to_nic_fail++;
        pfa->ports[pfa->cpuport.portno].stat.rd_droped++;
        return;
    }
    if (unlikely(g_pfa_latency_rpt_en != 0)) {
        rx_cb = (struct rx_cb_map_s *)(&skb->cb);
        rx_cb->pkt_id = desc->rd_user_field2;
    }

    pfa_cpu_rd_check_update_only(pfa, desc, skb);
    if (src_port->ctrl.port_multiple_en) {
        net_id = pfa_get_muti_net_id(pfa, port_ctx, desc);
        ops = &src_port->ctrl.net_map[net_id].ops;
        net = src_port->ctrl.net_map[net_id].net;
        port_priv = src_port->ctrl.net_map[net_id].port_priv;
    }
    src_port->ctrl.net_map[net_id].cpu_lan_cnt++;

    if (ops->pfa_netif_rx) {
        cpuport->cpu_rd_to_nic++;
        ops->pfa_netif_rx(skb, port_priv);
    } else {
        cpuport->cpu_rd_to_netif_rx++;
        pfa_rd_cpu_lan_to_netif_rx(pfa, net, src_port, skb, desc);
    }
    return;
}

static inline int pfa_cpurd_drop_check_v100(struct pfa *pfa, struct pfa_rd_desc *desc, unsigned int *port)
{
    if (pfa->td_fail_generate_rd) {
        if (desc->rd_drop_rsn != TD_DROP_RSN_TRANS_SUCC) {
            return 1;
        }
    } else if (desc->rd_pfa_tft_dl_err_flag) {
        *port = pfa->pfa_tftport.portno;
        return 1;
    }
    return 0;
}
static inline int pfa_cpurd_drop_check_v200(struct pfa_rd_desc *desc)
{
    if (desc->rd_trans_result != 0) {
        return 1;
    }
    return 0;
}

static inline int pfa_cpurd_drop_check(struct pfa *pfa, struct pfa_rd_desc *desc, unsigned int *port)
{
    *port = desc->rd_sport;

    if (desc->rd_drop_rsn == RD_DROP_RSN_TTL_ZERO) {
        return 0;
    }

    if (pfa->pfa_version == PFA_VERSION_V200) {
        return pfa_cpurd_drop_check_v200(desc);
    } else {
        return pfa_cpurd_drop_check_v100(pfa, desc, port);
    }
}

static inline bool pfa_check_free_drop_pkt(struct pfa *pfa, unsigned long long dra_l2_addr, struct pfa_rd_desc *desc)
{
    if (pfa->pfa_version == PFA_VERSION_V200) {
        return (desc->rd_user_field1 & PFA_SPORT_FREE_ORI_ADDR_FLAG) == 0;
    } else {
        return dra_l2_addr != 0;
    }
}

void pfa_rd_process_cpuport_desc(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr)
{
    unsigned int port;

    if (unlikely(pfa_cpurd_drop_check(pfa, desc, &port))) {  // td fail, free mem
        pfa->ports[port].stat.td_desc_fail_drop++;
        if (pfa_check_free_drop_pkt(pfa, dra_l2_addr, desc)) { // drop pkt from usb port, dra_l2_addr may be 0
            pfa_free_dra_mem(pfa, port, dra_l2_addr);
        }
        return;
    }

    if (pfa_check_sport_is_modem(pfa, desc)) {
        pfa_rd_process_cpu_wan(pfa, port_ctx, desc, dra_l2_addr);
    } else {
        pfa_rd_process_cpu_lan(pfa, port_ctx, desc, dra_l2_addr);
    }
    return;
}

static void pfa_rd_pts_rpt(struct pfa *pfa, struct pfa_rd_desc *desc, unsigned int *pkt_id)
{
    unsigned int pdcp_cnt;

    if (g_pfa_latency_rpt_en != 0) {
        if (pfa_check_sport_is_modem(pfa, desc)) {
            *pkt_id = get_pkt_id();
            desc->rd_user_field2 = *pkt_id;
            pfa_pts_rpt(*pkt_id, PFA_DOWN_RCV);
            pdcp_cnt = pfa->hal->get_pdcp_cnt(desc);
            pfa_pdcp_cnt_rpt(*pkt_id, pdcp_cnt);
        } else {
            *pkt_id = desc->rd_user_field2;
        }
    }
}

static bool pfa_rd_filter_proc(struct pfa *pfa, struct pfa_port_ctx *port_ctx,
    struct pfa_rd_desc *desc, unsigned long long dra_l2_addr)
{
    struct sk_buff *skb = NULL;
    unsigned long long org_dra_addr = 0;

    if (pfa->ops.pfa_rx_filter) {
        skb = bsp_dra_to_skb(dra_l2_addr, &org_dra_addr);
        if (unlikely((skb == NULL) || (org_dra_addr == 0))) {
            port_ctx->stat.dra_to_skb_fail++;
            PFA_ERR_ONCE("%u time dra to skb fail, sport %u, dra 0x%llx skb %lx \n",
                port_ctx->stat.dra_to_skb_fail, desc->rd_sport, dra_l2_addr, (uintptr_t)skb);
            pfa_bug(pfa);
            return false;
        }
        return pfa->ops.pfa_rx_filter(skb);
    }
    return false;
}

static inline unsigned long long pfa_get_dra_addr_form_desc(struct pfa_rd_desc *desc)
{
    unsigned long long rdout32low;
    unsigned long long rdout32upper;

    rdout32low = (unsigned long long)desc->rd_outaddr_lower;
    rdout32upper = (unsigned long long)desc->rd_outaddr_upper;
    return (rdout32low + (rdout32upper << 32));
}

static inline unsigned long long pfa_get_eth_addr(struct pfa *pfa, struct pfa_port_ctx *port_ctx,
    struct pfa_rd_desc *desc, unsigned long long addr)
{
    unsigned long long dra_l2_addr = addr;

    if (pfa->pfa_version == PFA_VERSION_V100 || pfa->pfa_version == PFA_VERSION_V0) {
        if ((port_ctx->ctrl.portno != pfa->usbport.portno) || (pfa->usbport.attr.enc_type == PFA_ENC_ECM)) {
            dra_l2_addr = addr - desc->rd_l2_hdr_offset;
        }
    }

    return dra_l2_addr;
}

static inline struct sk_buff *pfa_bulid_skb(struct pfa *pfa, struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc,
    unsigned long long dra_l2_addr)
{
#ifdef CONFIG_PFA_FW
    return pfa_resolve_skb(pfa, port_ctx, desc, dra_l2_addr, false);
#else
    return pfa_resolve_pfa_tft_skb(pfa, port_ctx, desc, dra_l2_addr);
#endif
}

void pfa_rd_process_one_desc(struct pfa *pfa, struct pfa_port_ctx *port_ctx, void *rd_desc)
{
    struct pfa_rd_desc *desc = (struct pfa_rd_desc *)rd_desc;
    struct sk_buff *skb = NULL;
    unsigned int pkt_id;
    unsigned long long dra_l2_addr;

    if (pfa->usbport.portno == port_ctx->ctrl.portno) {
        port_ctx->stat.rd_finsh_pkt_num += desc->rd_pktnum;
    } else {
        port_ctx->stat.rd_finsh_pkt_num += 1;
    }

    dra_l2_addr = pfa_get_dra_addr_form_desc(desc);
    dra_l2_addr = pfa_get_eth_addr(pfa, port_ctx, desc, dra_l2_addr);
    if (pfa->drop_stub && (pfa_check_sport_is_modem(pfa, desc)) && (port_ctx->ctrl.portno == pfa->cpuport.portno)) {
        pfa->cpuport.cpu_rd_udp_drop++;
        pfa->ports[pfa->cpuport.portno].stat.rd_droped++;
        pfa_checksum(pfa, dra_l2_addr, desc->rd_pkt_len);
        bsp_dra_free(dra_l2_addr);
        return;
    }

    if (unlikely(pfa->dbg_level & PFA_DBG_RD_RESULT)) {
        pfa->hal->rd_record_result(desc, &port_ctx->stat);
    }

    if (unlikely(port_ctx->ctrl.portno == pfa->cpuport.portno)) {
        pfa->cpuport.cpu_rd_num++;
        pfa_rd_process_cpuport_desc(pfa, port_ctx, desc, dra_l2_addr);
        return;
    } else {
        if (pfa_rd_filter_proc(pfa, port_ctx, desc, dra_l2_addr)) {
            bsp_dra_free(dra_l2_addr);
            port_ctx->stat.rd_filter++;
            return;
        }
        pfa_rd_pts_rpt(pfa, desc, &pkt_id);
        if (port_ctx->ctrl.ops.pfa_finish_rd_dra) {
            port_ctx->ctrl.ops.pfa_finish_rd_dra(dra_l2_addr, desc->rd_pkt_len, desc->rd_l2_hdr_offset,
                port_ctx->ctrl.port_priv, pkt_id);
            port_ctx->stat.rd_sended++;
        } else {
            skb = pfa_bulid_skb(pfa, port_ctx, desc, dra_l2_addr);
            if (unlikely(skb == NULL)) {
                pfa->ports[pfa->cpuport.portno].stat.rd_droped++;
                return;
            }
            pfa_rd_transmit_desc(port_ctx, skb, desc);
            return;
        }
    }
}

unsigned int __pfa_process_rd_desc(struct pfa *pfa, struct pfa_port_ctx *port_ctx, unsigned int rd_rptr, unsigned int rd_wptr)
{
    struct pfa_rd_desc *rd_base = (struct pfa_rd_desc *)port_ctx->ctrl.rd_addr;
    struct pfa_rd_desc *long_buf = NULL;
    unsigned int rd_num = 0;
    unsigned int i = 0;
    int ret = 0;

    while (rd_rptr != rd_wptr) {
        if (rd_wptr > rd_rptr) {
            rd_num = rd_wptr - rd_rptr;
        } else {
            rd_num = port_ctx->ctrl.rd_depth - rd_rptr;
        }
        long_buf = (struct pfa_rd_desc *)port_ctx->ctrl.rd_long_buf;
        ret = memcpy_s(long_buf, port_ctx->ctrl.rd_depth * sizeof(struct pfa_rd_desc), &rd_base[rd_rptr],
                       rd_num * sizeof(struct pfa_rd_desc));
        if (unlikely(ret)) {
            port_ctx->stat.rewind_th_cpy_fail++;
        }
        rd_rptr += rd_num;
        if (rd_rptr == port_ctx->ctrl.rd_depth) {
            rd_rptr = 0;
        }
        port_ctx->ctrl.rd_free = rd_rptr;
        pfa_writel(pfa->regs, PFA_RDQX_RPTR(port_ctx->ctrl.portno), rd_rptr);
        for (i = 0; i < rd_num; i++) {
            port_ctx->stat.rd_finished++;
            pfa_rd_process_one_desc(pfa, port_ctx, long_buf++);
        }
        port_ctx->ctrl.rd_busy = rd_wptr;
    }

    return rd_num;
}

/*
 * The function does not need to be locked when receiving packets with the NAPI interface
 * The function needs to be locked when receiving packets with interrupt or workqueue interface
 * Because there is concurrency in MBB scenarios, locking is needed; when MBB uses NAPI, locking is not required
 */
unsigned int pfa_process_rd_desc(unsigned int portno)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_port_ctx *port_ctx = &pfa->ports[portno];
    unsigned int rd_rptr;
    unsigned int rd_wptr;
    unsigned long long get_rd_cnt = 0;
    unsigned int rd_num = 0;

    port_ctx->stat.rd_finsh_intr_complete++;
    while (get_rd_cnt < pfa->rd_loop_cnt) {
        rd_rptr = port_ctx->ctrl.rd_free;
        rd_wptr = pfa_readl(pfa->regs, PFA_RDQX_WPTR(portno));  // must be pfa_readl, not pfa_readl_relaxed
        if (unlikely(rd_wptr >= port_ctx->ctrl.rd_depth)) { // security check
            PFA_ERR("rd_wptr reg is error, may write memory out of bounds\n");
            return 0;
        }
        if (rd_rptr == rd_wptr) {
            port_ctx->ctrl.port_flags.rd_not_empty = 0;
            port_ctx->stat.rd_finsh_intr_complete++;
            break;
        }
        rd_num = __pfa_process_rd_desc(pfa, port_ctx, rd_rptr, rd_wptr);
        get_rd_cnt++;
    }

    if ((get_rd_cnt != 0) && port_ctx->ctrl.ops.pfa_complete_rd) {
        port_ctx->ctrl.ops.pfa_complete_rd(port_ctx->ctrl.port_priv);
        port_ctx->stat.rd_finsh_intr_complete_called++;
    }

    return rd_num;
}

/* this function for dra hard error eviding */
void pfa_update_adq_wptr(struct pfa *pfa)
{
    struct pfa_adq_ctrl *cur_ctrl = NULL;
    unsigned int wptr;
    unsigned long flags = 0;

    spin_lock_irqsave(&pfa->pfa_ad_lock, flags);
    cur_ctrl = &pfa->adqs_ctx.ctrl;
    wptr = readl(cur_ctrl->dra_wptr_stub_addr);
    if (wptr < pfa->adqs_ctx.ctrl.adq_size) {
        pfa_writel(pfa->regs, PFA_ADQ0_WPTR, wptr);
    }
    spin_unlock_irqrestore(&pfa->pfa_ad_lock, flags);

    return;
}

#ifndef CONFIG_PFA_PHONE_SOC
void pfa_adq_ctrl_timer(struct timer_list *t)
{
    struct pfa *pfa = &g_pfa;
    unsigned long flags = 0;

    spin_lock_irqsave(&pfa->reset_lock, flags);
    if (pfa->modem_resetting != 1) {
        spin_unlock_irqrestore(&pfa->reset_lock, flags);
        pfa_update_adq_wptr(pfa);
    } else {
        spin_unlock_irqrestore(&pfa->reset_lock, flags);
    }

    mod_timer(&pfa->pfa_adq_timer, PFA_DFS_T(pfa->pfa_adq_time_interval));

    return;
}

void pfa_adq_timer_init(struct pfa *pfa)
{
    timer_setup(&pfa->pfa_adq_timer, pfa_adq_ctrl_timer, 0);
    pfa->pfa_adq_timer.expires = PFA_DFS_T(pfa->pfa_adq_time_interval);
    add_timer(&pfa->pfa_adq_timer);
}
#endif

static void pfa_ad_init_defcfg(struct pfa *pfa)
{
    pfa->adqs_ctx.ctrl.dra_ipip_type = DRA_IPIPE_FOR_PFA_2K;
    pfa->adqs_ctx.ctrl.adbuf_len = PFA_AD1_PKT_LEN;

    pfa->adqs_ctx.ctrl.adq_size_sel = pfa_get_adq_size_sel(pfa->adqs_ctx.ctrl.adq_size);

    pfa_writel_relaxed(pfa->regs, PFA_ETH_MAXLEN, PFA_AD1_PKT_LEN);

    pfa->hal->config_adq_threshold_and_len(pfa, PFA_MIN_PKT_SIZE);

    return;
}

void pfa_ad_dts_init(struct pfa *pfa)
{
    int ret;

    ret = bsp_dt_property_read_u32_array(pfa->np, "pfa_tft_soft_push", &pfa->soft_push, 1);
    if (ret) {
        pfa->soft_push = 0;
        bsp_err("PFA-DRA ADQ pushed by hardware.\n");
    }
    ret = bsp_dt_property_read_u32_array(pfa->np, "pfa_adq_size", &pfa->adqs_ctx.ctrl.adq_size, 1);
    if (ret) {
        pfa->adqs_ctx.ctrl.adq_size = PFA_ADQ_SIZE_DEFAULT;
        bsp_err("PFA-DRA ADQ use default.\n");
    } else if (pfa->adqs_ctx.ctrl.adq_size > PFA_ADQ_SIZE_MAX ||
        pfa->adqs_ctx.ctrl.adq_size < PFA_ADQ_SIZE_MIN) {
        pfa->adqs_ctx.ctrl.adq_size = PFA_ADQ_SIZE_MAX;
        bsp_err("pfa ap adq size is too large or small size:0x%x!!\n", pfa->adqs_ctx.ctrl.adq_size);
    }

    return;
}

int pfa_ad_init(struct pfa *pfa)
{
    struct pfa_adq_ctrl *cur_ctrl = NULL;
    dma_addr_t dra_rptr_reg_dma_addr;
    dma_addr_t ad_wptr_reg_addr;
    int ret = 0;

    pfa_ad_init_defcfg(pfa);

    cur_ctrl = &pfa->adqs_ctx.ctrl;

    cur_ctrl->ad_base_addr = bsp_sc_alloc_ringbuf(pfa->dev, cur_ctrl->adq_size * sizeof(struct pfa_ad_desc), &cur_ctrl->ad_dma_addr, GFP_KERNEL);
    if (cur_ctrl->ad_base_addr == NULL) {
        PFA_ERR("alloc adq desc pool failed.\n");
        ret = -ENOMEM;
    }

    pfa_writel_relaxed(pfa->regs, PFA_ADQ0_BADDR_L, lower_32_bits(cur_ctrl->ad_dma_addr));
    pfa_writel_relaxed(pfa->regs, PFA_ADQ0_BADDR_H, upper_32_bits(cur_ctrl->ad_dma_addr));
    if (pfa->pfa_version == PFA_VERSION_V200) {
        pfa_writel_relaxed(pfa->regs, PFA_ADQ0_SIZE, cur_ctrl->adq_size - 0x10);
    } else {
        pfa_writel_relaxed(pfa->regs, PFA_ADQ0_SIZE, cur_ctrl->adq_size_sel);
    }

    ad_wptr_reg_addr = pfa->res + PFA_ADQ0_WPTR;
    dra_rptr_reg_dma_addr = (dma_addr_t)bsp_dra_set_adqbase((unsigned long long)cur_ctrl->ad_dma_addr,
                                                                lower_32_bits(ad_wptr_reg_addr), pfa->adqs_ctx.ctrl.adq_size,
                                                                pfa->adqs_ctx.ctrl.dra_ipip_type);

    pfa_writel(pfa->regs, PFA_ADQ0_RPTR_UPDATE_ADDR_L, lower_32_bits(dra_rptr_reg_dma_addr));
    pfa_writel(pfa->regs, PFA_ADQ0_RPTR_UPDATE_ADDR_H, upper_32_bits(dra_rptr_reg_dma_addr));
    pfa_writel(pfa->regs, PFA_ADQ0_EN, 1);

    return ret;
}

void pfa_ad_exit(struct pfa *pfa)
{
    struct pfa_adq_ctrl *cur_ctrl = NULL;

    cur_ctrl = &pfa->adqs_ctx.ctrl;
    if (cur_ctrl->ad_base_addr != NULL) {
        bsp_sc_free_ringbuf(pfa->dev, cur_ctrl->adq_size * sizeof(struct pfa_ad_desc), cur_ctrl->ad_base_addr,
                          cur_ctrl->ad_dma_addr);
        cur_ctrl->ad_base_addr = NULL;
    }
    if (pfa->soft_push) {
        if (cur_ctrl->dra_wptr_stub_addr != NULL) {
            dma_free_coherent(pfa->dev, sizeof(unsigned long long), cur_ctrl->dra_wptr_stub_addr,
                              cur_ctrl->dra_wptr_stub_dma_addr);
            cur_ctrl->dra_wptr_stub_addr = NULL;
        }
        del_timer(&pfa->pfa_adq_timer);
    }

    return;
}

int mdrv_pfa_config_td(int vir_portno, unsigned long long dra_l2_addr, unsigned int len, void *param, unsigned int td_flags)
{
    if (g_pfa.init == 0) {
        PFA_ERR("config td fail, pfa no init\n");
        return -1;
    }
    return bsp_pfa_config_td(vir_portno, dra_l2_addr, len, param, td_flags);
}

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(bsp_pfa_config_td);
EXPORT_SYMBOL(bsp_pfa_config_bd_bypass_to_dpa);
EXPORT_SYMBOL(pfa_enable_port_bypass);
EXPORT_SYMBOL(mdrv_pfa_config_td);
#ifdef CONFIG_PFA_FW
EXPORT_SYMBOL(pfa_rd_process_cpu_wan);
EXPORT_SYMBOL(pfa_rd_process_cpu_lan);
EXPORT_SYMBOL(pfa_rd_process_cpuport_desc);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
