/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : d2h user信息同步模块
 * 作    者 : wifi
 * 创建日期 : 2024年2月26日
 */

#ifndef HMAC_TX_USER_INFO_REPORT_H
#define HMAC_TX_USER_INFO_REPORT_H

#include "wlan_types.h"
#include "frw_ext_if.h"

#ifdef _PRE_WLAN_FEATURE_USER_INFO_REPORT
uint32_t hmac_user_info_report_event(frw_event_mem_stru *event_mem);
#endif

#endif
