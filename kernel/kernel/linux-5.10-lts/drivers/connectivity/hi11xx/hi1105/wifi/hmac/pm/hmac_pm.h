/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_pm.c 的头文件
 * 作    者 :
 * 创建日期 : 2013年9月18日
 */

#ifndef HMAC_PM_H
#define HMAC_PM_H

#include "mac_user.h"
#include "mac_vap.h"
#include "hmac_ext_if.h"

#include "hmac_user.h"

void hmac_register_pm_callback(void);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
void hmac_register_pm_init(void);
#endif
void hmac_pm_wkup_src_parse_set(uint8_t en);
uint8_t hmac_pm_wkup_src_parse_get(void);
extern oal_bool_enum_uint8 g_force_sleep_switch;
extern uint8_t g_wlan_device_pm_switch;
extern uint8_t g_wlan_min_fast_ps_idle;
extern uint8_t g_wlan_max_fast_ps_idle;
extern uint8_t g_wlan_auto_ps_screen_on;
extern uint8_t g_wlan_auto_ps_screen_off;
extern uint8_t g_wlan_min_fast_ps_idle_p2p;
extern uint8_t g_wlan_max_fast_ps_idle_p2p;
extern uint8_t g_wlan_auto_ps_screen_on_p2p;
extern uint8_t g_wlan_auto_ps_screen_off_p2p;
extern uint8_t g_wlan_ps_mode;
extern uint8_t g_wlan_fast_ps_dyn_ctl;

typedef struct {
    uint8_t fastsleep3_start_listen_time;
    uint8_t fastsleep3_sleep_rtc_time;
    uint8_t fastsleep3_rtc_wakeup_work_time;
    uint8_t fastsleep3_total_rx_cnt_per_period;
    uint8_t fastsleep3_wakeup_rx_cnt_per_period;
} fastsleep3_param_stru;

typedef enum {
    FASTSLEEP_DISABLE = 0,
    FASTSLEEP_V2 = 1,
    FASTSLEEP_V3 = 2
} fastsleep_type_enum;

void hmac_fastsleep_high_pps_close_check(uint32_t throughput);
void hmac_fastsleep_stop(mac_vap_stru *mac_vap);
void hmac_fastsleep_clear_param(mac_vap_stru *mac_vap);
uint32_t hmac_set_sta_ps_mode(mac_vap_stru *mac_vap, uint32_t *params);
void hmac_mvap_stop_fastsleep(void);
#endif /* end of hmac_pm.h */
