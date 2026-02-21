/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Wifi7黑名单功能函数定义
 * 作    者 : wifi
 * 创建日期 : 2024年7月25日
 */

#ifndef HMAC_WIFI7_SELF_CURE_C
#define HMAC_WIFI7_SELF_CURE_C
#ifdef _PRE_WLAN_FEATURE_11BE

#include "hmac_ext_if.h"
#include "mac_data.h"
#include "hmac_resource.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "hmac_fsm.h"
#include "mac_user.h"
#include "hmac_roam_main.h"
#include "hmac_arp_probe.h"
#include "hmac_wifi7_self_cure.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WIFI7_SELF_CURE_C

OAL_STATIC hmac_wifi7_self_cure_info_stru g_wifi7_selfcure_info;

/*
 * 功能描述  : 黑名单设置
 * 1.日    期  : 2024年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_wifi7_self_cure_black_list_set(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    size_t cure_info_len;
    hmac_wifi7_self_cure_info_stru *wifi7_self_cure_info = NULL;
    uint8_t index;
    if (mac_vap == NULL || param == NULL) {
        oam_warning_log0(0, OAM_SF_SCAN, "hmac_wifi7_self_cure_black_list_set::\
            mac_vap or param is null");
        return OAL_FAIL;
    }
    cure_info_len = sizeof(hmac_wifi7_self_cure_info_stru);
    if (len != cure_info_len) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_wifi7_self_cure_black_list_set::\
            cure_info_len[%d], len[%d]", cure_info_len, len);
        return OAL_FAIL;
    }

    if (!is_legacy_sta(mac_vap)) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_wifi7_self_cure_black_list_set::\
            legacy_sta check error");
        return OAL_FAIL;
    }

    wifi7_self_cure_info = (hmac_wifi7_self_cure_info_stru *)param;
    if (memcpy_s(&g_wifi7_selfcure_info, cure_info_len, wifi7_self_cure_info, cure_info_len) != EOK) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_wifi7_self_cure_black_list_set::memcpy fail!");
        return OAL_FAIL;
    }
    for (index = 0; index < g_wifi7_selfcure_info.blaclist_num; index++) {
        oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_wifi7_self_cure_black_list_set::\
            idx[%d], type[%d], user_mac XX:XX:XX:XX:%02X:%02X", index,
            g_wifi7_selfcure_info.wifi7_blacklist[index].blacklist_type,
            g_wifi7_selfcure_info.wifi7_blacklist[index].user_mac_addr[MAC_ADDR_4],
            g_wifi7_selfcure_info.wifi7_blacklist[index].user_mac_addr[MAC_ADDR_5]);
    }
    return OAL_SUCC;
}

/*
 * 功能描述  : 查询当前mac是否在wifi7 blacklist里面
 * 1.日    期  : 2024年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
oal_bool_enum_uint8 hmac_wifi7_self_cure_mac_is_wifi7_blacklist_type(uint8_t *mac)
{
    uint8_t loop;
    hmac_wifi7_self_cure_blacklist_stru *wifi7_list = NULL;

    for (loop = 0; loop < g_wifi7_selfcure_info.blaclist_num; loop++) {
        wifi7_list = &(g_wifi7_selfcure_info.wifi7_blacklist[loop]);
        if (!OAL_MEMCMP(wifi7_list->user_mac_addr, mac, WLAN_MAC_ADDR_LEN) &&
            wifi7_list->blacklist_type == HMAC_WIFI7_SELF_CURE_BLACKLIST_TYPE_WIFI7) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

/*
 * 功能描述  : 查询当前mac是否在mld blacklist里面
 * 1.日    期  : 2024年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
oal_bool_enum_uint8 hmac_wifi7_self_cure_mac_is_mld_blacklist_type(uint8_t *mac)
{
    uint8_t loop;
    hmac_wifi7_self_cure_blacklist_stru *wifi7_list = NULL;

    for (loop = 0; loop < g_wifi7_selfcure_info.blaclist_num; loop++) {
        wifi7_list = &(g_wifi7_selfcure_info.wifi7_blacklist[loop]);
        if (!OAL_MEMCMP(wifi7_list->user_mac_addr, mac, WLAN_MAC_ADDR_LEN) &&
            wifi7_list->blacklist_type == HMAC_WIFI7_SELF_CURE_BLACKLIST_TYPE_MLD) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

#endif
#endif /* end of HMAC_WIFI7_SELF_CURE_C */

