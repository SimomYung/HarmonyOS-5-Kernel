/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_emlsr.c 的头文件
 * 作    者 :
 * 创建日期 : 2012年9月18日
 */

#ifndef HMAC_EMLSR_H
#define HMAC_EMLSR_H

/* 1 其他头文件包含 */
#include "oam_ext_if.h"
#include "hmac_ext_if.h"
#include "oal_mem.h"
#include "frw_ext_if.h"
#include "frw_event_main.h"

#ifdef __cplusplus // windows ut编译不过，后续下线清理
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_EMLSR

#define EMLSR_OMN_RETRY_TIME        200  /* 超过100ms没收到omn发送成功事件则重传 */
#define EMLSR_CLOSE_OMN_RETRY_LIMIT 3    /* 强制退出emlsr，omn帧最多重传3次 */
#define EMLSR_OPEN_TIMEOUT          200  /* emlsr进入，等待omn帧发送成功超时时间 */

void hmac_emlsr_ram_init(mac_vap_stru *mac_vap);
uint32_t hmac_emlsr_auto_enter_process(void);
void hmac_wlan_vap_emlsr_state_sync_event(hmac_mld_manage_stru *mld_vap_manage);
uint32_t hmac_emlsr_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_emlsr_force_close_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_emlsr_switch_event(frw_event_mem_stru *event_mem);
uint8_t hmac_check_emlsr_enter_is_allowed(void);
void hmac_mld_update_emlsr_flag(void);
uint32_t hmac_emlsr_close_for_mvap(mac_vap_stru *mac_vap, mac_channel_stru *new_channel_info, uint8_t event_type);
void hmac_vap_state_change_notify_emlsr(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t is_enter_event);
void hmac_vap_state_change_notify_emlsr_common_proc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype);
uint8_t hmac_emlsr_check_bitmap_allowed_enter(void);
void hmac_emlsr_disable_bitmap_set(uint8_t event_type);
void hmac_emlsr_disable_bitmap_clear(uint8_t event_type);
hmac_vap_stru *hmac_emlsr_exit_choose_strong_rssi_link(hmac_vap_stru *hmac_vap);
uint32_t hmac_emlsr_enter_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_emlsr_enter_check_device_state_allowed(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_emlsr_force_close_process(hmac_vap_stru *expect_work_vap, uint8_t reason);
uint32_t hmac_config_d2h_emlsr_info_syn(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
#endif /* end of _PRE_WLAN_FEATURE_EMLSR */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_emlsr */

