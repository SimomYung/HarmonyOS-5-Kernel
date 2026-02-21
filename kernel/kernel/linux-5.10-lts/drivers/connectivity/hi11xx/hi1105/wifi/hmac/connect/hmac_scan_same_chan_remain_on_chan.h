/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 * 功能说明   : hmac_scan_same_chan_remain_on_chan.c的头文件
 * 创建日期   : 2024年04月19日
 */
#ifndef HMAC_SCAN_SAME_CHAN_REMAIN_ON_CHAN_H
#define HMAC_SCAN_SAME_CHAN_REMAIN_ON_CHAN_H
#include "hmac_vap.h"

typedef enum {
    MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_DEL_USER = 0, /* del user打断同信道监听, 针对sta/gc vap而言 */
    MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_DOWN_VAP = 1, /* down vap打断同信道监听 */
    MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_DEL_VAP = 2, /* del vap打断同信道监听 */
    MAC_ABOTR_SAME_CHAN_REMAIN_ON_CHAN_CSA = 3, /* csa打断同信道监听 */
    MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_ROAM = 4, /* 漫游打断同信道监听 */
    MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_LINK_SWITCH = 5, /* link切换打断同信道监听 */
} mac_abort_same_chan_remain_on_chan_reason_enum; /* 同信道监听被打断原因码 */

typedef struct {
    uint8_t same_chan_remain_on_chan_bitmap; /* bit0:长期同信道监听，由上层控制; bit1:由驱动控制, 定期同信道监听 */
    uint8_t resv[3];

    /* 长期同信道监听相关配置 */
    uint8_t long_listen_vap_id; /* 长期监听vap id */
    uint8_t long_listen_chan_numer; /* 保存长期同信道监听信道 */

    /* 定期同信道监听相关配置 */
    uint8_t normal_listen_vap_id; /* 定期监听vap_id */
    uint8_t normal_listen_chan_number; /* 保存定期同信道监听信道 */
    frw_timeout_stru remain_on_chan_timer; /* 定期同信道监听超时定时器 */
} mac_same_chan_remain_on_chan_info_stru; /* 同信道监听相关结构体 */

typedef struct {
    uint32_t wait_time; /* 监听时长 */
    mac_vap_stru *mac_work_vap; /* 需进行监听的工作vap */
    oal_bool_enum_uint8 remian_on_chan_need_wait; /* tx流程使用, 是否需要额外启动remain_on_chan定时器的标记 */
    uint8_t resv[3];
} mac_same_chan_remain_on_chan_cfg; /* 同信道监听配置参数 */

/* 扫描信息上报内核 */
typedef struct {
    uint8_t report_type; /* 内核用来区分消息类型 */
    uint8_t listen_chan_number; /* 被打断的同信道监听信道号 */
    uint8_t reason_code; /* 同信道监听被打断的原因码 */
    uint8_t resv;
} mac_same_chan_remain_on_chan_abort_report_stru; /* 同信道监听被打断上报上层消息结构体 */

uint32_t hmac_listen_start_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap, mac_vap_stru *mac_work_vap,
    uint32_t wait_time);
uint32_t hmac_cancel_same_chan_remain_on_chan(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_tx_mgmt_start_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap, mac_vap_stru *mac_work_vap,
    oal_bool_enum_uint8 remian_on_chan_need_wait, uint32_t wait_time);
void hmac_reset_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap);
mac_vap_stru *mac_find_listen_chan_work_vap(mac_device_stru *mac_device, mac_vap_stru *mac_self_vap,
    oal_bool_enum_uint8 is_allow_listen_byself, uint8_t scan_channel_num);
mac_same_chan_remain_on_chan_info_stru *hmac_get_same_chan_remain_on_chan_info(void);
oal_bool_enum_uint8 mac_normal_same_remain_on_chan_is_running(
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info);
void hmac_abort_long_same_chan_remain_on_chan(mac_vap_stru *mac_vap, oal_bool_enum_uint8 is_allow_listen_byself,
    uint8_t reascon_code);
void hmac_del_vap_same_chan_remain_handler(mac_vap_stru *mac_vap);
#endif
