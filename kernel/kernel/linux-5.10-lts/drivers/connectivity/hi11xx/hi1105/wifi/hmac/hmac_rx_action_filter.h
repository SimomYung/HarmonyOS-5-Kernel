/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_rx_action_filter.c 文件
 * 作    者 : w30072618
 * 创建日期 : 2025年2月14日
 */
#ifndef HMAC_RX_ACTION_FILTER_H
#define HMAC_RX_ACTION_FILTER_H
#include "hmac_vap.h"
#include "hd_rx_action_filter.h"

oal_netbuf_stru *hmac_fill_rx_action_netbuf(mac_rx_action_blacklist_stru *action_blacklist,
    size_t blacklist_items);
void hmac_update_action_filter(mac_vap_stru *mac_vap);
#endif