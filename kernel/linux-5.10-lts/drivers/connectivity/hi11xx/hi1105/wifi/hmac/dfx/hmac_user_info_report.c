/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 配置相关实现hmac接口实现源文件
 */

/* 1 头文件包含 */
#include "hmac_user_info_report.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_USER_INFO_REPORT_C

/* 相应dmac侧定时上报的 user级别 rssi */
#ifdef _PRE_WLAN_FEATURE_USER_INFO_REPORT
/* event_mem以及event 按照惯例，由函数调用者保证其有效性 */
uint32_t hmac_user_info_report_event(frw_event_mem_stru *event_mem)
{
    uint8_t i;
    uint16_t len;
    frw_event_stru *event = NULL;
    hmac_user_stru *hmac_user = NULL;
    mac_all_user_report_info *report = NULL;

    event = frw_get_event_stru(event_mem);
    report = (mac_all_user_report_info *)event->auc_event_data;

    len = report->user_num * MAC_USER_REPORT_INFO_SIZE + MAC_USER_REPORT_INFO_HEAD_SIZE + sizeof(frw_event_hdr_stru);
    if ((report->user_num > MAC_RES_MAX_USER_LIMIT) || (len > event->st_event_hdr.us_length)) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_user_info_report_event::user_num[%u] len[%u].}",
            report->user_num, event->st_event_hdr.us_length);
        return OAL_FAIL;
    }

    for (i = 0; i < report->user_num; i++) {
        hmac_user = mac_res_get_hmac_user(report->report_info[i].assoc_id);
        if (hmac_user == NULL) {
            continue;
        }
        if (hmac_user->st_user_base_info.en_is_multi_user == OAL_TRUE) {
            continue;
        }
        hmac_user->c_rssi = report->report_info[i].rssi;
        hmac_user->tx_rate = report->report_info[i].tx_rate;
        hmac_user->rx_rate = report->report_info[i].rx_rate;
    }
    return OAL_SUCC;
}
#endif
