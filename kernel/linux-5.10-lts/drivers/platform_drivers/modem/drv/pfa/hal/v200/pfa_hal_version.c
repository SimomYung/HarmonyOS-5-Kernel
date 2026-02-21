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

#include "pfa.h"
#include "pfa_dbg.h"
#include "pfa_tft_reg.h"
#include "pfa_hal_reg.h"
#include "pfa_tft.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

struct pfa_om_reg_set g_pfa_om_reg[] = {
    {
        .start = PFA_TDQX_BADDR_L(0),
        .end = PFA_TDQX_EN(PFA_CCORE_PORT_BEGIN - 1)
    },
    {
        .start = PFA_UNPACK_CTRL,
        .end = PFA_BDC_END_REG
    },
    {
        .start = PFA_RDQ_FULL_LEVEL,
        .end = PFA_RDQX_END_REG(PFA_CCORE_PORT_BEGIN - 1)
    },
    {
        .start = PFA_ADQ_EMPTY_LEVEL,
        .end = PFA_ADQ_EMPTY_LEVEL + 0xc
    },
    {
        .start = PFA_ADQ0_BADDR_L,
        .end = PFA_ADQ0_END_REG
    },
    {
        .start = PFA_VER,
        .end = PFA_PORTX_PROPERTY_ENC_TYPE(PFA_PORT_NUM - 1)
    },
    {
        .start = PFA_TD_DONE_INTA_STATUS,
        .end = PFA_DMA_DONE_INTA_END
    },
    {
        .start = PFA_TD_DONE_DPA_STATUS,
        .end = PFA_DMA_DONE_INTDPA_END
    },
    {
        .start = PFA_INTA_INTERVAL,
        .end = PFA_INTDPA_INTERVAL_END
    },
    {
        .start = PFA_ENG_BASE,
        .end = PFA_PORTX_UDP_LMTNUM(PFA_CCORE_PORT_BEGIN)
    },
};

unsigned int g_pfa_reset_bak_reg[PFA_RESET_BAK_REG_NUM][PFA_STASH_REG_BUTTON] = {
    { PFA_HASH_CACHE_EN, 0 },
    { PFA_PACK_MAX_PKT_CNT, 0 },
    { PFA_PACK_MAX_LEN, 0 },
    { PFA_BUSY_MASK, 0 },
    { PFA_INTDPA_INTERVAL, 0 },
    { PFA_TD_DONE_DPA_MASK, 0 },
    { PFA_RD_DONE_DPA_MASK, 0 },
    { PFA_IP_MINLEN, 0 },
    { PFA_IP_MAXLEN, 0 },
    { PFA_NO_IP_ETH_MINLEN, 0 },
    { PFA_NO_IP_ETH_MAXLEN, 0 },
    { PFA_DM_CFG_MAC_FW_REG, 0 },
};

void config_tft_svlan_tag_sel(unsigned int flag)
{
}

void pfa_config_packet_len_limit(struct pfa *pfa)
{
    pfa_writel(pfa->regs, PFA_ETH_MINLEN, PFA_ETH_MIN_PKT_SIZE);
    pfa_writel(pfa->regs, PFA_NO_IP_ETH_MINLEN, PFA_NO_IP_ETH_MIN_PKT_SIZE);
}

void config_tft_bd_rd_ip_head(struct pfa *pfa)
{
    return;
}

void tft_config_rd_ip_head(unsigned int ext_desc_en)
{
    return;
}

void config_tft_bd_rd_ptr_update(struct pfa *pfa, struct iport_pfa_tft_addr *pfa_tft_reg)
{
    return;
}

void queue_portx_save(struct pfa *pfa_ctx, unsigned int portno, struct pfa_queue_stash *queue_regs)
{
    queue_regs->pfa_portx_udp_lmtnum[portno] = pfa_readl(pfa_ctx->regs, PFA_PORTX_UDP_LMTNUM(portno));
    queue_regs->pfa_portx_property[portno] = pfa_readl(pfa_ctx->regs, PFA_PORTX_PROPERTY(portno));
    queue_regs->pfa_portx_enc_type[portno] = pfa_readl(pfa_ctx->regs, PFA_PORTX_PROPERTY_ENC_TYPE(portno));
    queue_regs->pfa_portx_mac_addr_l[portno] = pfa_readl(pfa_ctx->regs, PFA_PORTX_MAC_ADDR_L(portno));
    queue_regs->pfa_protx_mac_addr_h[portno] = pfa_readl(pfa_ctx->regs, PFA_PORTX_MAC_ADDR_H(portno));
    queue_regs->pfa_one_bd_size[portno] = pfa_readl(pfa_ctx->regs, PFA_TDQX_ONE_BD_SIZE(portno));
    queue_regs->pfa_tdqx_ctrl[portno] = pfa_readl(pfa_ctx->regs, PFA_TDQX_CTRL(portno));

    queue_regs->pfa_tdqx_rptr_update_addr_l[portno] = pfa_readl(pfa_ctx->regs, PFA_TDQX_RPTR_UPDATE_ADDR_L(portno));
    queue_regs->pfa_tdqx_rptr_update_addr_h[portno] = pfa_readl(pfa_ctx->regs, PFA_TDQX_RPTR_UPDATE_ADDR_H(portno));

    queue_regs->pfa_rdqx_wptr_update_addr_l[portno] = pfa_readl(pfa_ctx->regs, PFA_RDQX_WPTR_UPDATE_ADDR_L(portno));
    queue_regs->pfa_rdqx_wptr_update_addr_h[portno] = pfa_readl(pfa_ctx->regs, PFA_RDQX_WPTR_UPDATE_ADDR_H(portno));

    queue_regs->pfa_one_rd_size[portno] = pfa_readl(pfa_ctx->regs, PFA_RDQX_ONE_RD_SIZE(portno));
    queue_regs->pfa_rdqx_ctrl[portno] = pfa_readl(pfa_ctx->regs, PFA_RDQX_CTRL(portno));
}

void queue_portx_restore(struct pfa *pfa_ctx, unsigned int portno, struct pfa_queue_stash *queue_regs)
{
    pfa_writel(pfa_ctx->regs, PFA_PORTX_UDP_LMTNUM(portno), queue_regs->pfa_portx_udp_lmtnum[portno]);
    pfa_writel(pfa_ctx->regs, PFA_PORTX_PROPERTY(portno), queue_regs->pfa_portx_property[portno]);
    pfa_writel(pfa_ctx->regs, PFA_PORTX_PROPERTY_ENC_TYPE(portno), queue_regs->pfa_portx_enc_type[portno]);
    pfa_writel(pfa_ctx->regs, PFA_PORTX_MAC_ADDR_L(portno), queue_regs->pfa_portx_mac_addr_l[portno]);
    pfa_writel(pfa_ctx->regs, PFA_PORTX_MAC_ADDR_H(portno), queue_regs->pfa_protx_mac_addr_h[portno]);
    pfa_writel(pfa_ctx->regs, PFA_TDQX_ONE_BD_SIZE(portno), queue_regs->pfa_one_bd_size[portno]);
    pfa_writel(pfa_ctx->regs, PFA_TDQX_CTRL(portno), queue_regs->pfa_tdqx_ctrl[portno]);

    pfa_writel(pfa_ctx->regs, PFA_TDQX_RPTR_UPDATE_ADDR_L(portno), queue_regs->pfa_tdqx_rptr_update_addr_l[portno]);
    pfa_writel(pfa_ctx->regs, PFA_TDQX_RPTR_UPDATE_ADDR_H(portno), queue_regs->pfa_tdqx_rptr_update_addr_h[portno]);
    pfa_writel(pfa_ctx->regs, PFA_RDQX_WPTR_UPDATE_ADDR_L(portno), queue_regs->pfa_rdqx_wptr_update_addr_l[portno]);
    pfa_writel(pfa_ctx->regs, PFA_RDQX_WPTR_UPDATE_ADDR_H(portno), queue_regs->pfa_rdqx_wptr_update_addr_h[portno]);

    pfa_writel(pfa_ctx->regs, PFA_RDQX_ONE_RD_SIZE(portno), queue_regs->pfa_one_rd_size[portno]);
    pfa_writel(pfa_ctx->regs, PFA_RDQX_CTRL(portno), queue_regs->pfa_rdqx_ctrl[portno]);
}

void config_hash_cache_en(struct pfa *pfa, unsigned int enable)
{
    return;
}

void config_adq_threshold_and_len(struct pfa *pfa, unsigned int size)
{
    return;
}

void config_qos_lmttime(struct pfa *pfa, unsigned int value)
{
    return;
}

void config_eth_vlan_tag_sel(struct pfa *pfa, unsigned int flag)
{
    eth_vlan_tag_sel value;

    value.u32 = pfa_readl(pfa->regs, PFA_TOP_TAG_SEL_REG);
    value.bits.one_vlan_tag_sel = flag;
    pfa_writel(pfa->regs, PFA_TOP_TAG_SEL_REG, value.u32);
    if (flag) {
        pfa_writel(pfa->regs, PFA_TOP_SVLAN_TPID0_REG, 0x88a8);
        pfa_writel(pfa->regs, PFA_TOP_SVLAN_TPID1_REG, 0x9100);
        pfa_writel(pfa->regs, PFA_TOP_SVLAN_TPID2_REG, 0x9200);
        pfa_writel(pfa->regs, PFA_TOP_SVLAN_TPID3_REG, 0x8300);
    } else {
        pfa_writel(pfa->regs, PFA_TOP_SVLAN_TPID0_REG, 0x8100);
    }
}

void config_dport_adq_core_type(struct pfa *pfa, unsigned int portno, unsigned int type)
{
    pfa_port_prop_t prop;

    prop.u32 = pfa_readl(pfa->regs, PFA_PORTX_PROPERTY(portno));
    prop.bits.pfa_dport_adq_core_type = type;
    pfa_writel(pfa->regs, PFA_PORTX_PROPERTY(portno), prop.u32);
}

void pfa_print_one_rd_word_five_to_end(struct pfa_rd_desc *cur_rd)
{
    PFA_ERR("\n RD word 6 \n");
    PFA_ERR("eth_type %u \n", cur_rd->eth_type);
    PFA_ERR("rd_vlan_flag %u \n", cur_rd->rd_vlan_flag);
    PFA_ERR("rd_vlan_pcp %u \n", cur_rd->rd_vlan_pcp);
    PFA_ERR("rd_vlan_vid %u \n", cur_rd->rd_vlan_vid);

    PFA_ERR("\n RD word 7 \n");
    PFA_ERR("tcp_udp_field0 %u \n", cur_rd->tcp_udp_field0);

    PFA_ERR("\n RD word 8 \n");
    PFA_ERR("stmid %x \n", cur_rd->stream_id);

    PFA_ERR("\n RD word 9 \n");
    PFA_ERR("tft_pdcp_cnt %x \n", cur_rd->rd_user_field2);

    PFA_ERR("\n RD word 10-11 \n");
    PFA_ERR("rd_user_field0 %x \n", cur_rd->rd_user_field0);
    PFA_ERR("rd_user_field1 %x \n", cur_rd->rd_user_field1);
}

void pfa_print_one_rd(struct pfa_rd_desc *cur_rd)
{
    PFA_ERR("rd_out_lower %x \n", cur_rd->rd_outaddr_lower);
    PFA_ERR("rd_out_upper %x \n", cur_rd->rd_outaddr_upper);

    PFA_ERR("\n RD word 2 \n");
    PFA_ERR("pkt_dra_offset %u \n", cur_rd->pkt_dra_offset);
    PFA_ERR("dra_offset_positive %u \n", cur_rd->dra_offset_positive);
    PFA_ERR("rd_pkt_len %u \n", cur_rd->rd_pkt_len);
    PFA_ERR("rd_packet_ind %u \n", cur_rd->rd_packet_ind);
    PFA_ERR("rd_data_from_unsec %u \n", cur_rd->rd_tocpu_res);
    PFA_ERR("rd_mode %u \n", cur_rd->rd_mode);
    PFA_ERR("rd_sport %x \n", cur_rd->rd_sport);

    PFA_ERR("\n RD word 3 \n");
    PFA_ERR("rd_trans_result %u \n", cur_rd->rd_trans_result);
    PFA_ERR("rd_drop_rsn or rd_tocpu_rsn %u \n", cur_rd->rd_drop_rsn);
    PFA_ERR("rd_trans_path %u \n", cur_rd->rd_trans_path);
    PFA_ERR("rd_pktnum %x \n", cur_rd->rd_pktnum);
    PFA_ERR("rd_finish_warp_res %u \n", cur_rd->rd_finish_warp_res);

    PFA_ERR("\n RD word 4 \n");
    PFA_ERR("rd_high_pri_flag %u \n", cur_rd->rd_high_pri_flag);
    PFA_ERR("rd_ack_flag %u \n", cur_rd->rd_ack_flag);
    PFA_ERR("rd_net_id %u \n", cur_rd->rd_net_id);
    PFA_ERR("rd_fc_head %u \n", cur_rd->rd_fc_head);
    PFA_ERR("rd_pdu_ssid %u \n", cur_rd->rd_pdu_ssid);
    PFA_ERR("rd_modem_id %u \n", cur_rd->rd_modem_id);
    PFA_ERR("drb_id %u \n", cur_rd->drb_id);

    PFA_ERR("\n RD word 5 \n");
    PFA_ERR("qfi %u \n", cur_rd->qfi);
    PFA_ERR("special_flag %u \n", cur_rd->special_flag);
    PFA_ERR("rd_ptp_syn_ind %u \n", cur_rd->rd_ptp_syn_ind);
    PFA_ERR("ul_flow_id %u \n", cur_rd->ul_flow_id);
    PFA_ERR("rd_pkt_type %u \n", cur_rd->rd_pkt_type);
    PFA_ERR("rd_pfa_tft_dl_err_flag %u \n", cur_rd->rd_pfa_tft_dl_err_flag);
    PFA_ERR("version %u \n", cur_rd->version);
    PFA_ERR("protocol %u \n", cur_rd->protocol);

    pfa_print_one_rd_word_five_to_end(cur_rd);
}

void pfa_print_one_td(struct pfa_td_desc *cur_td)
{
    PFA_ERR("TD word 0~1 \n");
    PFA_ERR("td_in_lower %x \n", cur_td->td_inaddr_lower);
    PFA_ERR("td_in_upper %x \n", cur_td->td_inaddr_upper);

    PFA_ERR("TD word 2 \n");
    PFA_ERR("pkt_dra_offset %u \n", cur_td->pkt_dra_offset);
    PFA_ERR("dra_offset_positive %u \n", cur_td->dra_offset_positive);
    PFA_ERR("td_pkt_len %u \n", cur_td->td_pkt_len);
    PFA_ERR("td_pkt_ind %u \n", cur_td->pkt_ind);
    PFA_ERR("pkt_sec_n %u \n", cur_td->pkt_sec_n);
    PFA_ERR("td_copy_en %u \n", cur_td->td_copy_en);
    PFA_ERR("td_bypass_en %u \n", cur_td->td_bypass_en);
    PFA_ERR("td_bypass_addr %u \n", cur_td->td_bypass_addr);

    PFA_ERR("TD word 3 \n");
    PFA_ERR("tft_filter_ind %u \n", cur_td->tft_filter_ind);
    PFA_ERR("td_host_ana %u \n", cur_td->td_host_ana);
    PFA_ERR("td_stick_en %u \n", cur_td->td_stick_en);
    PFA_ERR("td_with_mac_pdu_rst %u \n", cur_td->td_with_mac_pdu_rst);
    PFA_ERR("bypass_pkt_type %u \n", cur_td->bypass_pkt_type);
    PFA_ERR("td_high_pri_flag %u \n", cur_td->td_high_pri_flag);
    PFA_ERR("ack_flag %u \n", cur_td->ack_flag);
    PFA_ERR("td_net_id %u \n", cur_td->td_net_id);
    PFA_ERR("td_fc_head %u \n", cur_td->td_fc_head);
    PFA_ERR("td_pdu_ssid %u \n", cur_td->td_pdu_ssid);
    PFA_ERR("td_modem_id %u \n", cur_td->td_modem_id);

    PFA_ERR("TD word 4 \n");
    PFA_ERR("ul_flow_id %u \n", cur_td->ul_flow_id);
    PFA_ERR("td_irq_en %u \n", cur_td->td_irq_en);
    PFA_ERR("td_push_en %u \n", cur_td->td_push_en);

    PFA_ERR("TD word 5 \n");
    PFA_ERR("ul_packet_id %x \n", cur_td->td_user_field2);

    PFA_ERR("td_user_field0 %x \n", cur_td->td_user_field0);
    PFA_ERR("td_user_field1 %x \n", cur_td->td_user_field1);
}
void set_update_only_produce_mod(struct pfa *pfa)
{
}

void enable_macfw_to_ip(struct pfa *pfa, unsigned int portno, unsigned int enable)
{
    pfa_port_prop_t property;

    property.u32 = pfa_readl_relaxed(pfa->regs, PFA_PORTX_PROPERTY(portno));
    if (enable) {
        property.bits.mac_ip_fw_flag = 1;
    } else {
        property.bits.mac_ip_fw_flag = 0;
    }
    pfa_writel_relaxed(pfa->regs, PFA_PORTX_PROPERTY(portno), property.u32);
}

void pfa_clk_gate_switch(struct pfa *pfa, unsigned int enable)
{
    unsigned int clken_sel = 0;

    if (enable) {
        clken_sel = 0xFFFFFF;
    }
    pfa_writel(pfa->regs, CLKEN_SEL, clken_sel);
}

void pfa_check_td_result(struct pfa *pfa, unsigned int portno, struct pfa_td_desc *td_cur, struct pfa_port_stat *stat)
{
}

void pfa_rd_record_result(struct pfa_rd_desc *cur_desc, struct pfa_port_stat *stat)
{
    struct desc_result_s *result = &stat->result;

    if (cur_desc->rd_drop_rsn == RD_DROP_RSN_UNDISCARD) {
        result->rd_send_cpu_rsn[cur_desc->rd_drop_rsn]++;          // rd desc bit23-26
    } else {
        result->rd_pkt_drop_rsn[cur_desc->rd_drop_rsn]++;           // rd desc bit2-5
    }

    result->rd_result[cur_desc->rd_trans_result]++;             // rd desc bit0-1
    result->rd_pkt_fw_path[__fls(cur_desc->rd_trans_path)]++;   // rd desc bit6-15
    result->rd_finsh_wrap_rsn[cur_desc->rd_finish_warp_res]++;  // rd desc bit20-22
    stat->result.rd_sport_cnt[cur_desc->rd_sport]++;
    stat->rd_finished_bytes += cur_desc->rd_pkt_len;

    return;
}

void pfa_config_td_tft_mode(struct wan_info_s *wan_info, struct pfa_td_desc *desc)
{
    desc->tft_filter_ind = wan_info->info.tft_filter_ind;
    desc->td_copy_en = wan_info->info.copy_mode;
    desc->td_user_field2 = wan_info->userfield2;
}

void dbgen_en(void)
{
}

void dbgen_dis(void)
{
}

int pfa_check_idle(struct pfa *pfa)
{
    pfa_bdq_stat_t bdq_status;
    pfa_rdq_stat_t rdq_status;
    unsigned int portno;
    unsigned int bd_wptr, bd_rptr;
    unsigned int rd_wptr, rd_rptr;
    unsigned int rd_depth;

    for (portno = 0; portno < PFA_CCORE_PORT_BEGIN; portno++) {
        bdq_status.u32 = pfa_readl(pfa->regs, PFA_TDQX_STATE(portno));
        bd_wptr = pfa_readl(pfa->regs, PFA_TDQX_WPTR(portno));
        bd_rptr = pfa_readl(pfa->regs, PFA_TDQX_RPTR(portno));
        if ((bdq_status.bits.empty != 1) || (bd_wptr != bd_rptr)) {
            pfa->ports[portno].stat.tdq_no_idle++;
            return -1;
        }
        rd_depth = pfa_readl(pfa->regs, PFA_RDQX_LEN(portno));
        rdq_status.u32 = pfa_readl(pfa->regs, PFA_RDQX_STATE(portno));
        rd_wptr = pfa_readl(pfa->regs, PFA_RDQX_WPTR(portno));
        rd_rptr = pfa_readl(pfa->regs, PFA_RDQX_RPTR(portno));
        if ((rdq_status.bits.empty != 1 && rd_depth != 0) || (rd_wptr != rd_rptr)) {
            pfa->ports[portno].stat.rdq_no_idle++;
            return -1;
        }
    }

    return 0;
}

void pfa_ackpkt_identify(struct pfa *pfa, unsigned int portno)
{
}

void pfa_update_usb_max_pkt_cnt(struct pfa *pfa, unsigned int pack_max_pkt_cnt)
{
    pfa_writel_relaxed(pfa->regs, PFA_PACK_MAX_PKT_CNT, pack_max_pkt_cnt);
}

void pfa_update_pack_max_len(struct pfa *pfa, unsigned int pack_max_len)
{
    pfa_writel_relaxed(pfa->regs, PFA_PACK_MAX_LEN, pack_max_len);
}

void pfa_config_port_td_pri(struct pfa *pfa, unsigned int portno, unsigned int pri)
{
    pfa_bd_ctrl_t ctrl;

    ctrl.u32 = pfa_readl(pfa->regs, PFA_TDQX_CTRL(portno));
    ctrl.bits.bdq_pri = pri;
    pfa_writel_relaxed(pfa->regs, PFA_TDQX_CTRL(portno), ctrl.u32);
}

void pfa_config_port_en(struct pfa *pfa, unsigned int portno, unsigned int en)
{
    pfa_writel(pfa->regs, PFA_TDQX_EN(portno), en);
}

void pfa_td_usb_result_print(struct pfa_port_ctrl *ctrl, struct pfa_port_stat *stat)
{
}

void pfa_td_result_print(struct pfa_port_ctrl *ctrl, struct pfa_port_stat *stat)
{
}

void pfa_clean_ad_buf(struct pfa *pfa)
{
    // clear adq wptr = rptr
}

void pfa_show_mac_entry(struct pfa *pfa, unsigned int flag)
{
    unsigned int pfa_tab[PFA_MAC_TAB_WORD_NO];
    unsigned int pfa_tab_reg[PFA_MAC_TAB_REG_WORD_NO];
    unsigned int i, j;
    struct pfa_mac_entry_in_sram *mac_entry = NULL;
    struct pfa_mac_entry_in_reg *mac_entry_reg = NULL;

    pfa->hal->config_update_req(pfa, PFA_FW_STOP);
    if (pfa->hal->wait_update_ack(pfa)) {
        pfa->hal->config_update_req(pfa, PFA_FW_ENABLE);
        return;
    }

    for (i = 0; i < PFA_MAC_TAB_NUMBER; i++) {
        for (j = 0; j < PFA_MAC_TAB_WORD_NO; j++) {
            pfa_tab[j] = pfa_readl(pfa->regs, PFA_MAC_TAB(i * PFA_MAC_TAB_WORD_NO + j));
        }
        for (j = 0; j < PFA_MAC_TAB_REG_WORD_NO; j++) {
            pfa_tab_reg[j] = pfa_readl(pfa->regs, PFA_MAC_TAB_REG(i * PFA_MAC_TAB_REG_WORD_NO + j));
        }
        if (pfa_tab_reg[1] & flag) { // 1 is mac entry valid flag
            mac_entry_reg = (struct pfa_mac_entry_in_reg *)pfa_tab_reg;
            PFA_ERR("mac entry (%d) : %pM ", i, mac_entry_reg->mac);
            mac_entry = (struct pfa_mac_entry_in_sram *)pfa_tab;
            PFA_ERR("bid:%u  vid:%u  pcp:%u  vlan_valid:%u  port:%u  timestamp:0x%x  userfield:0x%x  static:%u\n",
                    ((unsigned)mac_entry->bid) & 0xf, ((unsigned)mac_entry->vid) & 0xfff,
                    ((unsigned)mac_entry->vlan_pcp) & 0xf, ((unsigned)mac_entry->vlan_valid) & 0x1,
                    ((unsigned)mac_entry->port) & 0x1f, ((unsigned)mac_entry->timestamp) & 0xffff,
                    (unsigned)mac_entry->userfield_l | (mac_entry->userfield_h << 16), // 16 is high
                    ((unsigned)mac_entry->is_static) & 0x1);
        }
    }
    pfa->hal->config_update_req(pfa, PFA_FW_ENABLE);
}

int pfa_construct_skb(struct pfa_port_ctx *port_ctx, struct pfa_rd_desc *desc, struct sk_buff *skb,
    unsigned long long org_dra_addr, unsigned long long dra_l2_addr, unsigned int dra_addr_type)
{
    skb->data = phys_to_virt(bsp_dra_to_phy(dra_l2_addr));
    skb->len = 0;
    skb_reset_tail_pointer(skb);
    skb_put(skb, desc->rd_pkt_len);

    return 0;
}

void pfa_set_tft_result(struct rx_cb_map_s *rx_cb, struct pfa_rd_desc *desc)
{
    union pfa_tft_err_info_u tft_info;

    rx_cb->pfa_tft_result.bits_v200.bid_qosid = desc->qfi;
    rx_cb->pfa_tft_result.bits_v200.pf_type = desc->rd_finish_warp_res & 0x3;
    rx_cb->pfa_tft_result.bits_v200.ff_type = (desc->rd_finish_warp_res >> 0x2) & 0x1;
    tft_info.u32 = desc->rd_pktnum;
    rx_cb->pfa_tft_result.bits_v200.version_err = tft_info.bits.version_err;
    rx_cb->pfa_tft_result.bits_v200.head_len_err = tft_info.bits.head_len_err;
    rx_cb->pfa_tft_result.bits_v200.bd_pkt_noeq = tft_info.bits.bd_pkt_noeq;
    rx_cb->pfa_tft_result.bits_v200.pkt_parse_err = tft_info.bits.pkt_parse_err;
    rx_cb->pfa_tft_result.bits_v200.pkt_len_err = tft_info.bits.pkt_len_err;
    rx_cb->pfa_tft_result.bits_v200.pkt_mac_err = desc->pkt_mac_err;
    rx_cb->pfa_tft_result.bits_v200.pdu_session_id = desc->rd_pdu_ssid;
    rx_cb->pfa_tft_result.bits_v200.modem_id = desc->rd_modem_id;
    rx_cb->pfa_tft_result.bits_v200.version = desc->version;

    rx_cb->pfa_result.bits.acpu_pkt_rsn = desc->rd_drop_rsn;
}

void pfa_enable(void)
{
    struct pfa *pfa = &g_pfa;

    pfa_writel(pfa->regs, PFA_ACPU2PFA_SOFT_INT_SET, BIT(PFA_ENABLE_BIT));
}

void pfa_disable(void)
{
    struct pfa *pfa = &g_pfa;

    pfa_writel(pfa->regs, PFA_ACPU2PFA_SOFT_INT_SET, BIT(PFA_DISABLE_BIT));
}

void pfa_config_port_td_rd_info(unsigned int portno, unsigned int td_depth, unsigned int rd_depth)
{
    struct pfa *pfa = &g_pfa;

    pfa_writel_relaxed(pfa->regs, PFA_TDQX_LEN(portno), td_depth - 1);
    pfa_writel_relaxed(pfa->regs, PFA_RDQX_LEN(portno), rd_depth - 1);

    pfa_writel_relaxed(pfa->regs, PFA_RDQX_ONE_RD_SIZE(portno), sizeof(struct pfa_rd_desc));
    pfa_writel_relaxed(pfa->regs, PFA_TDQX_ONE_BD_SIZE(portno), sizeof(struct pfa_td_desc) / 4);
}

void pfa_enc_type_init(struct pfa *pfa)
{
    pfa->enc_type_map[PFA_ENC_NONE].phy_type = PFA_ENC_TPYE_NONE;
    pfa->enc_type_map[PFA_ENC_NCM_NTB16].phy_type = PFA_ENC_TPYE_NCM_NTH16;
    pfa->enc_type_map[PFA_ENC_NCM_NTB32].phy_type = PFA_ENC_TPYE_NCM_NTH32;
    pfa->enc_type_map[PFA_ENC_RNDIS].phy_type = PFA_ENC_TPYE_RNDIS;
    pfa->enc_type_map[PFA_ENC_WIFI].phy_type = PFA_ENC_TPYE_NONE;
    pfa->enc_type_map[PFA_ENC_CPU].phy_type = PFA_ENC_TPYE_ACPU;
    pfa->enc_type_map[PFA_ENC_ACK].phy_type = PFA_ENC_TPYE_NONE;
    pfa->enc_type_map[PFA_ENC_DIRECT_FW_NCM_NTB16].phy_type = PFA_ENC_TPYE_NCM_NTH16;
    pfa->enc_type_map[PFA_ENC_DIRECT_FW_NCM_NTB32].phy_type = PFA_ENC_TPYE_NCM_NTH32;
    pfa->enc_type_map[PFA_ENC_DIRECT_FW_RNDIS].phy_type = PFA_ENC_TPYE_RNDIS;
    pfa->enc_type_map[PFA_ENC_DIRECT_FW_ECM].phy_type = PFA_ENC_TPYE_NONE;
    pfa->enc_type_map[PFA_ENC_ECM].phy_type = PFA_ENC_TPYE_NONE;
    pfa->enc_type_map[PFA_ENC_WIFI_MULTIPLE].phy_type = PFA_ENC_TPYE_NONE;
}

void pfa_config_port_enc_type(struct pfa *pfa, unsigned int portno, int enc_type)
{
    pfa_writel_relaxed(pfa->regs, PFA_PORTX_PROPERTY_ENC_TYPE(portno), pfa->enc_type_map[enc_type].phy_type);
}

void pfa_config_sc(struct pfa *pfa, unsigned int idx, unsigned int value)
{
    pfa_writel_relaxed(pfa->regs, PFA_SC_REG(idx), value);
}

int pfa_get_version(void)
{
    return PFA_VERSION_V200;
}

int pfa_get_pdcp_cnt(struct pfa_rd_desc *desc)
{
    return desc->rd_user_field2;
}

void pfa_set_clk_div_num(struct pfa *pfa, unsigned int div_num)
{
    pfa_div_ctrl_t pfa_div;

    pfa_div.u32 = PFA_CLK_DIV_MASK;
    pfa_div.bits.pfa_div = div_num;

    writel(pfa_div.u32, pfa->peri.crg_addr + PFA_CLK_DIV);
    return;
}

void pfa_config_busy_mask(struct pfa *pfa)
{
    pfa_writel(pfa->regs, PFA_BUSY_MASK, PFA_BUSY_MASK_VALUE);
}

void pfa_reg_update_req(struct pfa *pfa, enum pfa_enable_reg enable)
{
    pfa_writel(pfa->regs, PFA_TBL_UPDATE_REQ_REG, enable);
}

int pfa_reg_update_wait_ack(struct pfa *pfa)
{
    unsigned int cnt = 5000; // 5000 cnt

    do {
        if ((pfa_readl(pfa->regs, PFA_TBL_UPDATE_ACK_REG) & 0x1) == 1) {
            break;
        }
    } while (--cnt);
    if (cnt == 0) {
        pfa->stat.wait_ack_fail++;
        PFA_ERR("pfa ack time_out\n");
        return -1;
    }

    return 0;
}

void pfa_config_pd_req(struct pfa *pfa)
{
    pfa_acpu2pfa_soft_int_t val;

    val.u32 = pfa_readl(pfa->regs, PFA_ACPU2PFA_SOFT_INT_SET);
    val.bits.pfa_disable = 1;
    pfa_writel(pfa->regs, PFA_ACPU2PFA_SOFT_INT_SET, val.u32);
}

void pfa_check_pd_req(struct pfa *pfa)
{
    unsigned int i = 0;
    unsigned int cnt = 20; // 20us

    while (i < cnt) {
        if (pfa_readl(pfa->regs, PFA_LPMCU_SOFT_INT_RAW)) {
            pfa_writel(pfa->regs, PFA_LPMCU_SOFT_INT_CLR, 1);
            break;
        }
        i++;
        udelay(1);
        if (i == cnt) {
            pfa->modem_reset_stat.no_wait_pd_req++;
            return;
        }
    }

    return;
}

void pfa_config_adq_en(struct pfa *pfa, unsigned int en)
{
    pfa_writel(pfa->regs, PFA_ADQ0_EN, en);
}

void pfa_check_adq_busy(struct pfa *pfa)
{
    unsigned int i;

    for (i = 0; i < PFA_BUSY_CHECK_CNT; i++) {
        if (pfa_readl(pfa->regs, PFA_BUSY) & (1 << PFA_BUSY_ADC_BIT)) {
            pfa->modem_reset_stat.adc_busy++;
        }
    }

    return;
}

int pfa_check_busy_state_step2(struct pfa *pfa)
{
    unsigned int i;

    for (i = 0; i < PFA_BUSY_CHECK_CNT; i++) {
        if (pfa_readl(pfa->regs, PFA_BUSY) & PFA_BUSY_VALUE2) {
            pfa->modem_reset_stat.busy_step2++;
            return -1;
        }
    }

    return 0;
}

void pfa_reset_before(struct pfa *pfa)
{
    pfa_config_pd_req(pfa);
    pfa_check_pd_req(pfa);

    pfa_config_adq_en(pfa, 0);
    pfa_check_adq_busy(pfa);

    pfa_check_busy_state_step2(pfa);

    pfa_writel(pfa->regs, PFA_CORE_HALT, 1);
}

void pfa_get_dpa_td_rd_cnt(struct pfa *pfa)
{
    unsigned int cnt;

    clear_bit(PFA_EVENT_DVFS_DPA_STOPED_BIT, (void *)&pfa->event);
    if (test_bit(PFA_EVENT_RESETTING_BIT, (void *)&pfa->event)){
        set_bit(PFA_EVENT_DVFS_DPA_STOPED_BIT, (void *)&pfa->event);
        wake_up(&pfa->wqueue);
        return;
    }

    cnt = pfa_readl(pfa->regs, PFA_TD_PKT_CNT_REG);
    if (cnt) {
        pfa->dfs_dpa_td_count = cnt;
    }
    cnt = pfa_readl(pfa->regs, PFA_RD_PKT_CNT_REG);
    if (cnt) {
        pfa->dfs_dpa_rd_count = cnt;
    }
    set_bit(PFA_EVENT_DVFS_DPA_STOPED_BIT, (void *)&pfa->event);
    wake_up(&pfa->wqueue);
}

void pfa_add_dm_mac_fw(struct pfa *pfa, struct pfa_mac_fw_entry *ent)
{
    int i;

    for (i = 0; i < PFA_MAC_TAB_NUMBER; i++) {
        if (pfa->mac_dm_entry[i].valid == 0) {
            pfa->mac_dm_entry[i].mac_hi = ent->mac_hi;
            pfa->mac_dm_entry[i].mac_vlan.bits.mac_lo = ent->mac_lo;
            pfa->mac_dm_entry[i].mac_vlan.bits.vid = ent->vid;
            pfa->mac_dm_entry[i].mac_vlan.bits.vlan_pcp = ent->pcp;
            pfa->mac_dm_entry[i].mac_vlan.bits.vlan_valid = ent->vlan_valid;
            pfa->mac_dm_entry[i].userfiled = ent->userfiled;
            pfa->mac_dm_entry[i].valid = 1;
            break;
        }
    }
    i = i < PFA_MAC_TAB_NUMBER ? i : 0;
    pfa_writel(pfa->regs, PFA_DM_MAC_ENTRY_HI(i), pfa->mac_dm_entry[i].mac_hi);
    pfa_writel(pfa->regs, PFA_DM_MAC_ENTRY_MAC_VLAN(i), pfa->mac_dm_entry[i].mac_vlan.u32);
    pfa_writel(pfa->regs, PFA_DM_MAC_ENTRY_USERFIELD(i), pfa->mac_dm_entry[i].userfiled);
    pfa_writel(pfa->regs, PFA_DM_MAC_ENTRY_VALID(i), pfa->mac_dm_entry[i].valid);

    pfa->dm_entry_cnt = (i >= pfa->dm_entry_cnt) ? (i + 1) : pfa->dm_entry_cnt;
}

void pfa_del_dm_mac_fw(struct pfa *pfa, struct pfa_mac_fw_entry *ent)
{
    int i;

    for (i = 0; i < PFA_MAC_TAB_NUMBER; i++) {
        if (pfa->mac_dm_entry[i].valid == 1) {
            if (pfa->mac_dm_entry[i].mac_hi == ent->mac_hi &&
                pfa->mac_dm_entry[i].mac_vlan.bits.mac_lo == ent->mac_lo) {
                    pfa->mac_dm_entry[i].valid = 0;
                    pfa_writel(pfa->regs, PFA_DM_MAC_ENTRY_VALID(i), pfa->mac_dm_entry[i].valid);
                    break;
            }
        }
    }

    pfa->dm_entry_cnt = (i == pfa->dm_entry_cnt) ? (pfa->dm_entry_cnt - 1) : pfa->dm_entry_cnt;
}

EXPORT_SYMBOL(pfa_construct_skb);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */