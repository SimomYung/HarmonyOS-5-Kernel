/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * 功能说明 : host、dpe公共使用的mac_frame相关处理函数头文件
 */

#ifndef MAC_FRMAE_IF_H
#define MAC_FRMAE_IF_H

#include "mac_frame_common.h"
/*
 * 函 数 名  : mac_rx_get_da
 * 功能描述  : 获取收到的帧的目的地址
 *             参考协议 <802.11权威指南> 81页
 * 1.日    期  : 2012年12月10日
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void mac_rx_get_da(mac_ieee80211_frame_stru *pst_mac_header,
                                         uint8_t **puc_da)
{
    if (pst_mac_header == NULL) {
        return;
    }
    /* IBSS、from AP */
    if (pst_mac_header->st_frame_control.bit_to_ds == 0) {
        *puc_da = pst_mac_header->auc_address1;
    /* WDS、to AP */
    } else {
        *puc_da = pst_mac_header->auc_address3;
    }
}

/*
 * 函 数 名  : mac_rx_get_sa
 * 功能描述  : 获取收到的帧的源地址
 *             参考协议 <802.11权威指南> 81页
 * 1.日    期  : 2012年12月11日
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void mac_rx_get_sa(mac_ieee80211_frame_stru *pst_mac_header,
                                         uint8_t **puc_sa)
{
    if (pst_mac_header == NULL) {
        return;
    }
    /* IBSS、to AP */
    if (pst_mac_header->st_frame_control.bit_from_ds == 0) {
        *puc_sa = pst_mac_header->auc_address2;
    /* from AP */
    } else if ((pst_mac_header->st_frame_control.bit_from_ds == 1) &&
             (pst_mac_header->st_frame_control.bit_to_ds == 0)) {
        *puc_sa = pst_mac_header->auc_address3;
    /* WDS */
    } else {
        *puc_sa = ((mac_ieee80211_frame_addr4_stru *)pst_mac_header)->auc_address4;
    }
}

/*
 * 函 数 名  : mac_get_transmitter_addr
 * 功能描述  : 获取收到的帧的发送端地址
 *             参考协议 <802.11权威指南> 81页
 * 1.日    期  : 2014年1月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void mac_get_transmit_addr(mac_ieee80211_frame_stru *pst_mac_header,
                                                 uint8_t **puc_bssid)
{
    if (pst_mac_header != NULL) {
        /* 对于IBSS, STA, AP, WDS 场景下，获取发送端地址 */
        *puc_bssid = pst_mac_header->auc_address2;
    }
}

/*
 * 函 数 名  : mac_get_frame_sub_type
 * 功能描述  : 获取报文的类型和子类型
 * 1.日    期  : 2013年4月18日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE uint8_t mac_get_frame_type_and_subtype(uint8_t *puc_mac_header)
{
    return (puc_mac_header[0] & 0xFC);
}

/*
 * 函 数 名  : mac_is_4addr
 * 功能描述  : 是否为4地址
 * 1.日    期  : 2015年5月21日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_4addr(mac_ieee80211_frame_stru *mac_hdr)
{
    if (mac_hdr == NULL) {
        return OAL_FALSE;
    }

    return (mac_hdr->st_frame_control.bit_from_ds && mac_hdr->st_frame_control.bit_to_ds);
}

#endif