/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_resource.c 的头文件
 * 作    者 : wifi
 * 创建日期 : 2024年11月18日
 */

#ifndef DPE_WLAN_RESOURCE_H
#define DPE_WLAN_RESOURCE_H

#include "wlan_spec_host.h"
#include "wlan_spec_cfg.h"
#include "dpe_wlan_device.h"
#include "dpe_wlan_vap.h"
#include "dpe_wlan_user.h"

typedef struct {
    dpe_wlan_device_stru dpe_wlan_device_res[WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC];
    dpe_wlan_vap_stru dpe_wlan_vap_res[WLAN_VAP_MAX_NUM_LIMIT];
    dpe_wlan_user_stru dpe_wlan_user_res[WLAN_USER_MAX_USER_LIMIT];
} dpe_wlan_res_stru;

extern dpe_wlan_res_stru g_dpe_ast_wlan_res;

dpe_wlan_res_stru *dpe_wlan_res_addr_get(void);

/* 对接ACPU的spec cfg差异化配置同步接口 */
void dpe_wlan_spec_cfg_init(const oal_wlan_cfg_stru *spec_cfg);
uint8_t dpe_mac_chip_get_hal_max_vap_num(void);
#endif
