/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : hmac_host_ring_tx头文件
 */

#ifndef HMAC_HOST_RING_TX_H
#define HMAC_HOST_RING_TX_H

/* 1 其他头文件包含 */
#include "dpe_wlan_tid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

void hmac_tx_reg_write_ring_info(hmac_tid_info_stru *tid_info, tid_cmd_enum_uint8 cmd, uint8_t d2h_switch_proc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
