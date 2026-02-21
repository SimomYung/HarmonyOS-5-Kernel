/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
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
#ifndef __WAN_ETH_TRB_TABLE_H__
#define __WAN_ETH_TRB_TABLE_H__

#include "wan_eth_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* _cplusplus */

struct weth_trb_info {
    unsigned long long base_addr;
    unsigned int depth;
};

union weth_pkt_info {
    struct {
        unsigned int ip_proto : 16;
        unsigned int vlan_proto : 16;
    } ul_bits;

    struct {
        unsigned int rsv;
    } dl_bits;

    unsigned int bit32;
};

struct weth_trb_desc {
    unsigned long long addr;
    unsigned short data_len;
    unsigned short ctrl; // 网卡ID
    union weth_pkt_info pkt_info;
};

typedef void (*weth_trb_tab_rx_cb_t)(unsigned int dev_id, struct sk_buff *skb, unsigned int num);
typedef void (*weth_trb_tab_table_sync_finish_cb_t)(void);

int weth_trb_tab_init(struct device *dev_for_map, struct weth_trb_info *tx_trb, struct weth_trb_info *rc_trb);
void weth_trb_tab_reinit(void);
void weth_trb_tab_regist_table_sync_cb(weth_trb_tab_table_sync_finish_cb_t table_sync_finish_cb);
void weth_trb_tab_set_addr_to_bar(struct weth_trb_info *tx_info, struct weth_trb_info *rx_info);

/* 上行接口 */
int weth_trb_tab_save_tx_desc(struct sk_buff *skb, unsigned int tot_num, unsigned int *txq_wptr);
int weth_trb_tab_start_tx_data_dma(unsigned int txq_wptr);
void weth_trb_tab_rc_tx_complete(unsigned int rw_ptr);

/* 下行接口 */
void weth_trb_tab_get_rx_desc_by_idx(unsigned int rw_ptr);
void weth_trb_tab_regist_rx_cb(weth_trb_tab_rx_cb_t rx_cb);

struct weth_trb_tab_ctx *weth_get_trb_tab_ctx(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __WAN_ETH_TABLE_H__ */
