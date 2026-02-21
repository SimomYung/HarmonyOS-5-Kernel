/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : mac mld 结构的定义
 * 创建日期 : 2023年10月12日
 */
#ifndef MAC_MLD_H
#define MAC_MLD_H

#ifdef _PRE_WLAN_FEATURE_ML
#include "mac_frame_11be_ml_ie.h"
#include "mac_user_common.h"
typedef struct {
    uint8_t             link_id;
    uint8_t             bpcc;
    uint8_t             complete_profile;
    uint8_t             sta_mac_addr[WLAN_MAC_ADDR_LEN];
    mac_channel_stru    st_channel;
    uint8_t             auc_supp_rates[WLAN_USER_MAX_SUPP_RATES];
    uint8_t             uc_num_supp_rates;
    uint8_t             asoc_succ;     /* 标识该link是否关联成功: 1表示成功，0表示失败 */
    uint16_t            inheritance_ie_len;
    uint16_t            non_inheritance_ie_len;
    uint8_t             is_add_link_list; /* 标识当前link info是否已添加到列表 */
    uint8_t             resv[1];
    uint8_t            *inheritance_ies;
    uint8_t            *non_inheritance_ies;
} ml_link_info_stru;

typedef struct {
    uint8_t                           *ml_ie;
    uint8_t                           ml_mac_addr[WLAN_MAC_ADDR_LEN];
    oal_bool_enum_uint8               ml_cap;
    uint8_t                           ml_link_num;
    mac_mld_capabilities_stru         ml_capabilities;
    mac_eml_capabilities_stru         eml_cap;
    mac_medium_sync_info_stru         medium_sync_cap;
    uint8_t                           link_id;
    uint8_t                           mld_id;
    uint8_t                           bpcc;
    ml_link_info_stru                 ml_link_info[WLAN_AP_MAX_ML_LINK_INFO_NUM];
} ml_info_stru;

typedef struct {
    ttlm_control    ttlm_ctrl;
    uint16_t        mapping_switch_time;
    uint8_t         expected_duration[3];
    uint8_t         resv;
    uint8_t link_bitmap[WLAN_TIDNO_BUTT];
} mac_cfg_ttlm_ie;

typedef struct {
    uint8_t             user_addr[OAL_MAC_ADDR_LEN];
    uint8_t             dialog_token;
    uint8_t             resv;
    mac_cfg_ttlm_ie     cfg_ttlm_ie;
} mac_cfg_ttlm_frame_param;

#endif

#endif /* end of mac_mld.h */
