/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2014年11月25日
 */

/* 1 头文件包含 */
#include "hmac_rx.h"
#include "mac_data.h"
#include "mac_frame_if.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_rx_defrag.h"
#include "hmac_11i.h"
#include "hmac_wapi.h"
#include "hmac_ht_self_cure.h"
#include "hmac_tx_amsdu.h"
#include "hmac_ota_report.h"
#include "hmac_mgmt_sta_up.h"
#include "hmac_wifi_delay.h"
#include "hmac_mintp_log.h"
#include "oam_event_wifi.h"
#endif
#include "hmac_rx_reorder.h"
#include "dpe_wlan_resource.h"
#include "dpe_wlan_stat.h"
#include "host_hal_ext_if.h"

#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
#include "hmac_mcast_ampdu.h"
#endif
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_ps.h"
#endif
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/ip.h>
#include <linux/ieee80211.h>
#include <linux/ipv6.h>
#endif
#ifdef CONFIG_HUAWEI_DUBAI
#include <chipset_common/dubai/dubai.h>
#include <huawei_platform/log/hwlog_kernel.h>
#endif
#ifdef _PRE_WLAN_FEATURE_ISOLATION
#include "hmac_isolation.h"
#endif
#ifdef _PRE_WLAN_FEATURE_PWL
#include "hmac_pwl.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_PN_CHECK_C

/* mld场景单播数据帧的pn号，各个link维护一份，因此提取公共全局变量 */
hmac_last_ucast_pn_stru g_last_ucast_pn[WLAN_USER_MAX_USER_LIMIT];
hmac_last_mcast_pn_stru g_last_mcast_pn[WLAN_USER_MAX_USER_LIMIT];

/* 功能描述: rx流程强制打印ota */
void hmac_rx_netbuf_force_ota_report(mac_rx_ctl_stru *rx_ctl, oal_netbuf_stru *netbuf)
{
    oam_report_netbuf_cb(BROADCAST_MACADDR,
                         (uint8_t *)oal_netbuf_cb(netbuf),
                         sizeof(mac_rx_ctl_stru),
                         OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_RTH) {
        oam_report_eth_frame(BROADCAST_MACADDR,
                             (uint8_t *)oal_netbuf_data(netbuf),
                             rx_ctl->us_frame_len,
                             OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    } else if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        oam_report_80211_frame(BROADCAST_MACADDR,
                               (uint8_t *)oal_netbuf_data(netbuf),
                               rx_ctl->uc_mac_header_len,
                               (uint8_t *)oal_netbuf_data(netbuf) + rx_ctl->uc_mac_header_len,
                               rx_ctl->us_frame_len,
                               OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    }
}

OAL_STATIC uint32_t hmac_rx_pn_param_check(oal_netbuf_stru *netbuf,
    mac_rx_ctl_stru *rx_ctl, mac_header_frame_control_stru *frame_control)
{
    if ((frame_control->bit_protected_frame == OAL_FALSE) || (frame_control->bit_retry == OAL_TRUE)) {
        return OAL_FAIL;
    }

    /* 1）如果是BA缓存区超时上报的报文，则不进行检查 */
    /* 2）如果当前帧比之前已经上报最后一帧的seq num小，则不进行检查 */
    if (mac_get_rx_cb_is_reo_timeout(rx_ctl) == OAL_TRUE || rx_ctl->is_before_last_release == OAL_TRUE) {
        oam_info_log3(0, OAM_SF_RX, "{hmac_rx_pn_param_check::SEQ NUM[%d] reo_timeout[%d] before_last_release[%d].}",
            rx_ctl->us_seq_num, mac_get_rx_cb_is_reo_timeout(rx_ctl), rx_ctl->is_before_last_release);
        return OAL_FAIL;
    }

    /* AMSDU聚合包的pn号一样，故只检查首包  */
    if (mac_rxcb_is_amsdu_sub_msdu(rx_ctl)) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

void dpe_hmac_user_clear_ptk_rx_pn(uint16_t user_idx)
{
    hmac_last_mcast_pn_stru *last_mcast_pn = NULL;
    hmac_last_ucast_pn_stru *last_ucast_pn = NULL;
    dpe_wlan_user_stru *dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_user == NULL) {
        return;
    }

    last_mcast_pn = hmac_get_last_mcast_pn(dpe_user);
    last_ucast_pn = hmac_get_last_ucast_pn(dpe_user);
    if (last_mcast_pn == NULL || last_ucast_pn == NULL) {
        return;
    }

    memset_s(&(last_ucast_pn->ucast_qos_last_lsb_pn), WLAN_TID_MAX_NUM * sizeof(uint32_t),
        0, WLAN_TID_MAX_NUM * sizeof(uint32_t));
    memset_s(&(last_ucast_pn->ucast_qos_last_msb_pn), WLAN_TID_MAX_NUM * sizeof(uint16_t),
        0, WLAN_TID_MAX_NUM * sizeof(uint16_t));
    memset_s(&(last_mcast_pn->mcast_qos_last_lsb_pn), WLAN_TID_MAX_NUM * sizeof(uint32_t),
        0, WLAN_TID_MAX_NUM * sizeof(uint32_t));
    memset_s(&(last_mcast_pn->mcast_qos_last_msb_pn), WLAN_TID_MAX_NUM * sizeof(uint16_t),
        0, WLAN_TID_MAX_NUM * sizeof(uint16_t));
    last_ucast_pn->ucast_nonqos_last_lsb_pn = 0;
    last_ucast_pn->ucast_nonqos_last_msb_pn = 0;
}
void dpe_hmac_user_clear_gtk_rx_pn(uint16_t user_idx)
{
    hmac_last_mcast_pn_stru *last_mcast_pn = NULL;
    hmac_last_ucast_pn_stru *last_ucast_pn = NULL;
    dpe_wlan_user_stru *dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_user == NULL) {
        return;
    }

    last_mcast_pn = hmac_get_last_mcast_pn(dpe_user);
    last_ucast_pn = hmac_get_last_ucast_pn(dpe_user);
    if (last_mcast_pn == NULL || last_ucast_pn == NULL) {
        return;
    }

    last_mcast_pn->mcast_nonqos_last_lsb_pn = 0;
    last_mcast_pn->mcast_nonqos_last_msb_pn = 0;
    last_ucast_pn->ucast_nonqos_last_lsb_pn = 0;
    last_ucast_pn->ucast_nonqos_last_msb_pn = 0;
}

/*
 * 函 数 名   : hmac_user_pn_get
 * 功能描述   : 获取用户的PN信息
 * 1.日    期   : 2020-05-25
 *   修改内容   : 生成函数
 */
OAL_STATIC uint64_t hmac_user_pn_get(dpe_wlan_user_stru *dpe_user, mac_rx_ctl_stru *rx_ctl, uint8_t qos)
{
    uint64_t last_pn_val;
    hmac_last_mcast_pn_stru *last_mcast_pn = hmac_get_last_mcast_pn(dpe_user);
    hmac_last_ucast_pn_stru *last_ucast_pn = hmac_get_last_ucast_pn(dpe_user);
    if (last_mcast_pn == NULL || last_ucast_pn == NULL) {
        return 0xFFFFFFFF;
    }

    /* 非qos时，组播和单播使用的pn号不一样 */
    /* qos时，组播和单播也分开检查pn，marvell AP发的组播和单播qos帧使用不同的pn号 */
    if (mac_rx_cb_is_multicast(rx_ctl) == OAL_TRUE) {
        if (qos == OAL_TRUE) {
            last_pn_val = ((uint64_t)last_mcast_pn->mcast_qos_last_msb_pn[rx_ctl->bit_rx_tid] << BIT_OFFSET_32) |
                (last_mcast_pn->mcast_qos_last_lsb_pn[rx_ctl->bit_rx_tid]);
        } else {
            last_pn_val = ((uint64_t)last_mcast_pn->mcast_nonqos_last_msb_pn << BIT_OFFSET_32) |
                (last_mcast_pn->mcast_nonqos_last_lsb_pn);
        }
    } else {
        if (qos == OAL_TRUE) {
            last_pn_val = ((uint64_t)last_ucast_pn->ucast_qos_last_msb_pn[rx_ctl->bit_rx_tid] << BIT_OFFSET_32) |
                (last_ucast_pn->ucast_qos_last_lsb_pn[rx_ctl->bit_rx_tid]);
        } else {
            last_pn_val = ((uint64_t)last_ucast_pn->ucast_nonqos_last_msb_pn << BIT_OFFSET_32) |
                (last_ucast_pn->ucast_nonqos_last_lsb_pn);
        }
    }
    return last_pn_val;
}

/* 检查不同加密场景PN是否合法 */
OAL_STATIC uint32_t hmac_rx_pn_is_invalid(uint64_t rx_pn_val, uint64_t last_pn_val, oal_bool_enum_uint8 mcast,
    wlan_ciper_protocol_type_enum_uint8 cipher_type)
{
    if (cipher_type == WLAN_80211_CIPHER_SUITE_WAPI) {
        if (wlan_chip_is_support_hw_wapi() == OAL_FALSE) {
            /* 软件WAPI加解密不在此处做PN检查 */
            return OAL_FALSE;
        } else {
            /* WAPI硬件加密PN检查.当前默认STA模式
             * 接收单播帧，PN需要严格单调递增且为奇数
             * 接收组播帧，PN需要严格单调递增
             */
            if ((last_pn_val >= rx_pn_val) && (last_pn_val != 0)) {
                return OAL_TRUE;
            } else {
                return (mcast == OAL_FALSE) ? ((rx_pn_val & 0x1) == 0x0) : OAL_FALSE;
            }
        }
    } else {
        return (last_pn_val >= rx_pn_val) && (last_pn_val != 0);
    }
}

/*
* PN窗口:
*  1)新报文的PN如果在窗口外，强制移"窗"，同时丢掉新报文;
*  2)新报文的PN如果在(last_PN-256 ~ last_PN)之间，认为PN检查失败，丢掉新报文.
*/
#define HMAC_PN_WINDOW_SIZE 256
OAL_STATIC oal_bool_enum_uint8 hmac_need_pn_windod_shift(uint64_t new, uint64_t cur)
{
    return (oal_bool_enum_uint8)((new + HMAC_PN_WINDOW_SIZE) < cur);
}

/*
 * 函 数 名   : hmac_user_pn_update
 * 功能描述   : 更新用户的PN信息
 * 1.日    期   : 2020-05-25
 *   修改内容   : 生成函数
 */
OAL_STATIC void hmac_user_pn_update(dpe_wlan_user_stru *dpe_user, mac_rx_ctl_stru *rx_ctl, uint8_t qos)
{
    hmac_last_mcast_pn_stru *last_mcast_pn = hmac_get_last_mcast_pn(dpe_user);
    hmac_last_ucast_pn_stru *last_ucast_pn = hmac_get_last_ucast_pn(dpe_user);
    if (last_mcast_pn == NULL || last_ucast_pn == NULL) {
        return;
    }

    if (mac_rx_cb_is_multicast(rx_ctl) == OAL_TRUE) {
        if (qos == OAL_TRUE) {
            last_mcast_pn->mcast_qos_last_msb_pn[rx_ctl->bit_rx_tid] = rx_ctl->us_rx_msb_pn;
            last_mcast_pn->mcast_qos_last_lsb_pn[rx_ctl->bit_rx_tid] = rx_ctl->rx_lsb_pn;
        } else {
            last_mcast_pn->mcast_nonqos_last_msb_pn = rx_ctl->us_rx_msb_pn;
            last_mcast_pn->mcast_nonqos_last_lsb_pn = rx_ctl->rx_lsb_pn;
        }
    } else {
        if (qos == OAL_TRUE) {
            last_ucast_pn->ucast_qos_last_msb_pn[rx_ctl->bit_rx_tid] = rx_ctl->us_rx_msb_pn;
            last_ucast_pn->ucast_qos_last_lsb_pn[rx_ctl->bit_rx_tid] = rx_ctl->rx_lsb_pn;
        } else {
            last_ucast_pn->ucast_nonqos_last_msb_pn = rx_ctl->us_rx_msb_pn;
            last_ucast_pn->ucast_nonqos_last_lsb_pn = rx_ctl->rx_lsb_pn;
        }
    }
}

/*
 * 功能描述   : 检查上报协议栈的报文pn号
 * 1.日    期   : 2020-05-25
 *   修改内容   : 生成函数
 */
uint32_t hmac_rx_pn_check(oal_netbuf_stru *netbuf)
{
    uint64_t                       last_pn_val, rx_pn_val;
    uint8_t                        qos_flg = OAL_FALSE;
    hmac_user_stru                *pst_user = NULL;
    dpe_wlan_user_stru            *dpe_user = NULL;
    mac_rx_ctl_stru               *rx_ctl = NULL;
    mac_header_frame_control_stru *frame_control = NULL;

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (rx_ctl == NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_pn_check::rx_ctl null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    frame_control = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;

    if (hmac_rx_pn_param_check(netbuf, rx_ctl, frame_control) != OAL_SUCC) {
        return OAL_SUCC;
    }

    if ((frame_control->bit_type == WLAN_DATA_BASICTYPE) && (frame_control->bit_sub_type >= WLAN_QOS_DATA) &&
        (frame_control->bit_sub_type <= WLAN_QOS_NULL_FRAME)) {
        qos_flg = OAL_TRUE;
    }
    /* TID 合法性检查 */
    if (oal_unlikely((qos_flg == OAL_TRUE) && (rx_ctl->bit_rx_tid >= WLAN_TID_MAX_NUM))) {
        oam_warning_log3(0, OAM_SF_RX, "{hmac_rx_pn_check::uc_qos_flg %d user id[%d] tid[%d].}",
                         qos_flg, rx_ctl->bit_rx_user_id, rx_ctl->bit_rx_tid);
        return OAL_SUCC;
    }

    pst_user = (hmac_user_stru *)mac_res_get_hmac_user(rx_ctl->bit_rx_user_id);
    dpe_user = dpe_wlan_user_get(rx_ctl->bit_rx_user_id);
    if (dpe_user == NULL || pst_user == NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_pn_check::user[%d] null.}", rx_ctl->bit_rx_user_id);
        return OAL_SUCC;
    }

    rx_pn_val = ((uint64_t)rx_ctl->us_rx_msb_pn << BIT_OFFSET_32) | (rx_ctl->rx_lsb_pn);
    last_pn_val = hmac_user_pn_get(dpe_user, rx_ctl, qos_flg);
    /* 首包pn为0，此处对首包不检查 */
    if (hmac_rx_pn_is_invalid(rx_pn_val, last_pn_val, rx_ctl->bit_mcast_bcast,
        pst_user->st_user_base_info.st_key_info.en_cipher_type) == OAL_TRUE) {
        oam_warning_log4(0, OAM_SF_RX, "{hmac_rx_pn_check::last pn[%ld] send pn[%ld] user id[%d] tid[%d].}",
                         last_pn_val, rx_pn_val, rx_ctl->bit_rx_user_id, rx_ctl->bit_rx_tid);
        oam_warning_log4(0, OAM_SF_RX, "{hmac_rx_pn_check:: en_qos[%d] seq_num[%d] is_mcast[%d], sub_type[%d].}",
                         qos_flg, rx_ctl->us_seq_num, mac_rx_cb_is_multicast(rx_ctl), frame_control->bit_sub_type);
        if (hmac_need_pn_windod_shift(rx_pn_val, last_pn_val)) {
            hmac_user_pn_update(dpe_user, rx_ctl, qos_flg);
        }
        /* 不做PN号检查 */
        if (g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_FEATURE_CERTIFY_MODE)) {
            return OAL_SUCC;
        }
        return OAL_FAIL;
    }

    hmac_user_pn_update(dpe_user, rx_ctl, qos_flg);
    return OAL_SUCC;
}
