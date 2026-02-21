/*
 * virtual_network_tx_rx.h
 *
 * virtual network kernel module implementation
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _VIRTUAL_NETWORK_TX_RX_H_
#define _VIRTUAL_NETWORK_TX_RX_H_

#include "virtual_network_session.h"
#include "virtual_network_device.h"

#include <linux/types.h>
#include <linux/skbuff.h>

typedef enum vnet_tx_rx_mode {
    VNET_TX_RX_MODE_ERROR = -1,
    VNET_TX_RX_MODE_NAT = 0,    // 转NAT处理
    VNET_TX_RX_MODE_POLICY = 1, // 进行policy tx/rx处理
} vnet_tx_rx_mode_t;

typedef struct vnet_tx_rx_policy_cfg {
    int policy_id;
    vnet_tx_rx_mode_t (*tx_match_func)(struct sk_buff *skb);
    int (*tx_func)(struct sk_buff *skb, vnet_session_t *ss);
    vnet_tx_rx_mode_t (*rx_match_func)(struct sk_buff *skb);
    int (*rx_func)(struct sk_buff *skb, vnet_session_t *ss);
} vnet_tx_rx_policy_cfg_t;

int vnet_register_tx_rx_policy(const vnet_tx_rx_policy_cfg_t *policy_cfg);
void vnet_unregister_tx_rx_policy(const vnet_tx_rx_policy_cfg_t *policy_cfg);

int vnet_skb_xmit(struct sk_buff *skb, vnet_session_t *ss, vnet_device_role_id_t role_id);
int vnet_skb_deliver(struct sk_buff *skb, vnet_session_t *ss);

int vnet_tx_rx_init(void);
void vnet_tx_rx_exit(void);

#endif // _VIRTUAL_NETWORK_TX_RX_H_