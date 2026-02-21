/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : Hertz平台硬转适配源文件
 */
#ifdef _PRE_WLAN_FEATURE_ISOLATION
#include "hmac_isolation.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ISOLATION_C

uint32_t hmac_config_set_isolation(mac_vap_stru *mac_vap, uint8_t *param)
{
    hmac_vap_stru *hmac_vap = NULL;

    if (oal_any_null_ptr2(mac_vap, param)) {
        return OAL_FAIL;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }
    if (*param > (BCAST_MASK | MCAST_MASK | UCAST_MASK)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "hmac_config_set_isolation:param_invalid[%d]", *param);
        return OAL_FAIL;
    }
    hmac_vap->ap_isolation = *param;
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "hmac_config_set_isolation:set_val[%d]", *param);
    return OAL_SUCC;
}

uint32_t hmac_hipriv_set_isolation(mac_vap_stru *mac_vap, uint32_t *param)
{
    return hmac_config_set_isolation(mac_vap, (uint8_t *)param);
}

oal_bool_enum_uint8 hmac_isolation_filter(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf)
{
    hmac_vap_stru *hmac_vap = NULL;
    mac_ether_header_stru *ether_hdr = NULL;
    uint8_t param;

    if (oal_any_null_ptr2(mac_vap, netbuf)) {
        return OAL_TRUE;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_TRUE;
    }
    ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (ether_hdr == NULL) {
        return OAL_TRUE;
    }
    param = hmac_vap->ap_isolation;
    if (((param & BCAST_MASK) != 0) && (ether_is_broadcast(ether_hdr->auc_ether_dhost) == OAL_TRUE)) {
        return OAL_TRUE;
    }
    if (((param & MCAST_MASK) != 0) && (ether_is_multicast(ether_hdr->auc_ether_dhost) == OAL_TRUE) &&
        (ether_is_broadcast(ether_hdr->auc_ether_dhost) == OAL_FALSE)) {
        return OAL_TRUE;
    }
    if (((param & UCAST_MASK) != 0) && (mac_vap_get_user_by_addr(mac_vap, ether_hdr->auc_ether_dhost) != NULL)) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

#endif