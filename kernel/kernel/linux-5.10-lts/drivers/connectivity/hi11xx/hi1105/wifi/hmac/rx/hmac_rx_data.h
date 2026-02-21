/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_rx_data.c 的头文件
 * 作    者 :
 * 创建日期 : 2012年11月20日
 */

#ifndef HMAC_RX_DATA_H
#define HMAC_RX_DATA_H

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_frame.h"
#include "hmac_user.h"
#include "oal_net.h"
#include "hmac_tcp_opt.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_resource.h"
#include "host_hal_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DATA_H

extern uint32_t g_wifi_trx_log;
/* 2 宏定义 */
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */

typedef struct {
    /* 是否使能手动配置接收聚合个数 */
    oal_bool_enum_uint8 en_rx_ampdu_bitmap_cmd;
    /* 是否定制化使能配置接收聚合个数 */
    oal_bool_enum_uint8 en_rx_ampdu_bitmap_ini;
    /* addba rsp回复的聚合BAW SIZE */
    uint16_t us_rx_buffer_size;
} mac_rx_buffer_size_stru; // only host

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */

uint32_t hmac_rx_process_data_event(frw_event_mem_stru *event_mem);
uint32_t hmac_ba_filter_serv(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, oal_netbuf_head_stru *netbuf_header,
    oal_bool_enum_uint8 *pen_is_ba_buf);

mac_rx_buffer_size_stru *mac_vap_get_rx_buffer_size(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_rx_data.h */
