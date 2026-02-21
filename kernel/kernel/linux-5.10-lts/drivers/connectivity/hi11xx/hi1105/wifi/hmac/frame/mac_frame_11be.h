/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_11be.c 的头文件
 * 创建日期   : 2023年9月18日
 */

#ifndef MAC_FRAME_11BE_H
#define MAC_FRAME_11BE_H

#include "wlan_types.h"
#include "oam_ext_if.h"
#include "securec.h"
#include "mac_user.h"
#include "mac_vap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define   EHT_OPERATION_PARAMETERS_LEN          1
#define   BASIC_EHT_MCS_AND_NSS_SET_LEN         4
#define   EHT_OPERATION_INFO_MIN_LEN            3
#define   DISABLED_SUBCHANNEL_BITMAP_LEN        2

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/

#ifdef _PRE_WLAN_FEATURE_11BE
uint8_t mac_forbid_encap_eht_ie(mac_vap_stru *mac_vap);
void mac_set_eht_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len);
void mac_set_eht_operation_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len);
uint16_t mac_set_eht_ie(mac_vap_stru *mac_vap, uint8_t *origin_buf,
    wlan_frame_mgmt_subtype_enum mgmt_subtype);
uint32_t mac_parse_eht_operation_ie(mac_vap_stru *mac_vap, uint8_t *oper_ie,
    mac_frame_eht_operation_ie_stru *eht_oper);
uint32_t mac_process_eht_capabilities_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *eht_cap_ie);
uint32_t mac_process_eht_operation_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *oper_ie);
uint32_t mac_user_set_avail_nss_eht(mac_user_stru *mac_user, mac_eht_hdl_stru *mac_eht_hdl,
    uint8_t *user_num_spatial_stream);
wlan_channel_bandwidth_enum_uint8 mac_get_eht_bandwith_from_center_freq_seg0_seg1(
    uint8_t chan_width, uint8_t channel, uint8_t ccfs0, uint8_t ccfs1);
uint32_t mac_set_addba_extension_ie(uint8_t *payload_addr, uint8_t *ie_len, uint8_t para);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of MAC_FRAME_11BE_H */
