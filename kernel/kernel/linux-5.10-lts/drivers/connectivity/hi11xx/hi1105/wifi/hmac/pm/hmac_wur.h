/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_wur.c的头文件
 * 作    者 : q30033932
 * 创建日期 : 2024年2月19日
 */

#ifndef HMAC_WUR_H
#define HMAC_WUR_H

#include "hmac_resource.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WUR_H

#ifdef _PRE_WLAN_FEATURE_WUR
/* wur相关能力宏 */
#define MAC_WUR_5G_SUPPORTED OAL_FALSE
#define MAC_WUR_VL_FRAME_SUPPORTED OAL_TRUE
#define MAC_WUR_GROUP_IDS_SUPPORTED OAL_FALSE
#define MAC_WUR_SHORT_WAKE_UP_SUPPORTED OAL_TRUE
#define MAC_WUR_HDR_SUPPORTED OAL_TRUE
#define MAC_WUR_TRANSITION_DELAY 0
#define MAC_WUR_MIN_WAKEUP_DURATION 20 /* ap支持的最小的service_time, 单位：256us */
#define MAC_WUR_DUTY_CYCLE_PERIOD_UNITS 1 /* ap指示的duty cycle period的基本单位，单位：4us */
#define MAC_WUR_DEFAULT_CHANNEL 1 /* vap工作在5G时，默认选择1信道作为wur channel */
#define MAC_WUR_DEFAULT_OFFSET_OF_TWBTT 1 /* 表示最小TSF时间（单位TU）的目标WUR beacon传输时间与TSF 0的时间差 */
#define MAC_WUR_DEFAULT_ON_DURATION_RATIO 2 /* 默认wur on_duration为duty cycle period的1/2 */
#define MAC_WUR_TIME_4US_UNITS 4 /* wur部分参数默认单位4us */
#define MAC_WUR_DEFAULT_DUTY_CYCLE_PERIOD 1 /* 默认duty cycle period = 1*beacon_period */
#define MAC_WUR_DEFAULT_SERVICE_TIME_RATID 50 /* 默认wur service time 占比50% */
#define MAC_WUR_DEVICE_SLEEP_MIN_RESTART_CNT 2 /* wur 睡眠参数，device睡眠定时器最小restart cnt */
#define MAC_WUR_CAP_REJECT_REASON 0xff /* 因wur基本能力不满足拒绝上层原因码 */

#ifdef _PRE_WLAN_FEATURE_WUR_BUGFIX
/* 只能对wur id为偶数的short wake up和ml wake up产生唤醒中断，所以wur id分配默认是user id*2 */
#define MAC_WUR_ID_EXPAND_NUM 2
#endif

/* 发wur action帧参数 */
typedef struct {
    uint8_t category; /* ACTION的类别 */
    uint8_t action;   /* 不同ACTION类别下的分类, setup 或者teardown, mac_wur_action_type_enum */
    uint8_t action_type; /* 参见wur_setup_type_enum */
    uint8_t resp_status;
    uint8_t wur_setup_cmd;
} mac_wur_action_mgmt_args_stru; /* only host */

hmac_vap_stru *hmac_find_up_wur_vap(void);
hmac_vap_stru *hmac_find_up_wur_enabled_vap(void);
uint32_t hmac_wur_teardown_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_wur_bt_release_channel_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_proc_rx_process_wur_sync_event_tx_adapt(frw_event_mem_stru *event_mem);
uint32_t hmac_rx_wur_setup_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len);
uint32_t hmac_rx_wur_teardown_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len);
uint32_t hmac_wur_update_parameters_to_device(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, uint8_t action_field);
void hmac_mgmt_tx_wur_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_wur_action_mgmt_args_stru *wur_action_args);
uint32_t hmac_mgmt_tx_wur_teardown(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t reason);
uint32_t calc_crc32(uint8_t *data, uint16_t data_len_input);
uint8_t hmac_wur_is_wur_enable(hmac_vap_stru *hmac_vap);
void hmac_wur_set_wur_enable(hmac_vap_stru *hmac_vap, uint8_t val);
void hmac_wur_exit(hmac_vap_stru *hmac_vap);
void hmac_wur_deinit(hmac_vap_stru *hmac_vap);
uint32_t hmac_config_set_wur_param(mac_vap_stru *mac_vap, uint8_t *param);
uint32_t hmac_config_device_allow_tx_wur_setup(frw_event_mem_stru *event_mem);
void hmac_vap_state_change_notify_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype);
void hmac_wur_high_pps_check(uint32_t throughput);
#endif /* end of _PRE_WLAN_FEATURE_WUR */
#endif /* end of OAM_FILE_ID_HMAC_WUR_H */
