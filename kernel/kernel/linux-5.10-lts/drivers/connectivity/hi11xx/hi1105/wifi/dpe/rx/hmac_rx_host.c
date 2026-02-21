/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_rx_host.c文件
 * 创建日期 : 2020年09月22日
 */

/* 1 头文件包含 */
#include "hmac_rx_host.h"
#include "hmac_rx.h"
#include "mac_data.h"
#include "mac_frame_if.h"
#include "hmac_rx_reorder.h"
#include "host_hal_ext_if.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_ota_report.h"
#include "oam_event_wifi.h"
#endif
#include "hmac_host_performance.h"
#include "dpe_wlan_stat.h"
#include "dpe_wlan_resource.h"
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#include "hmac_sniffer.h"
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_if.h"
#endif
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
#include "hmac_mcast_ampdu.h"
#endif
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_ps.h"
#endif
#include "pcie_linux.h"
#include "host_hal_dma.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_HOST_C

#ifdef _PRE_WLAN_DFT_STAT
#define HMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt) ((_member) += (_cnt))
#else
#define HMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt)
#endif

/* 功能描述 : 检查上报帧的vap是否有效 */
OAL_STATIC uint32_t hmac_is_hal_vap_valid(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    uint8_t   hal_vap_id;

    if (mac_rxcb_is_amsdu_sub_msdu(rx_ctl)) {
        return OAL_SUCC;
    }

    hal_vap_id = mac_get_rx_cb_hal_vap_id(rx_ctl);
    if (oal_unlikely(hal_vap_id >= dpe_mac_chip_get_hal_max_vap_num()) &&
        (hal_vap_id != g_wlan_spec_cfg->other_bss_id)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_is_hal_vap_valid::invalid hal vap id[%d].}", hal_vap_id);
        return OAL_FAIL;
    }

    if ((hal_vap_id < dpe_mac_chip_get_hal_max_vap_num()) &&
        (!hal_device->hal_vap_sts_info[hal_vap_id].hal_vap_valid)) {
        oam_warning_log2(0, OAM_SF_RX, "{hmac_is_hal_vap_valid::haldev[%d] invalid hal vap[%d]. }",
            hal_device->device_id, hal_vap_id);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
/* 功能描述 : 检查上报帧的tid是否有效 */
OAL_STATIC uint32_t hmac_is_tid_valid(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    uint8_t frame_type;

    if (mac_rxcb_is_amsdu_sub_msdu(rx_ctl)) {
        return OAL_SUCC;
    }

    /* 判断该帧是不是qos帧 */
    frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);
    if (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) {
        return OAL_SUCC;
    }

    if (!mac_tid_is_valid(mac_get_rx_cb_tid(rx_ctl))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_is_tid_valid::invalid tid[%d].}", mac_get_rx_cb_tid(rx_ctl));
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
OAL_STATIC void hmac_alrx_process(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    if (rx_ctl->rx_status != HAL_RX_SUCCESS) {
        hal_device->st_alrx.rx_ppdu_fail_num++;
    } else if (rx_ctl->bit_is_ampdu == OAL_TRUE) {
        hal_device->st_alrx.rx_ampdu_succ_num++;
    } else {
        hal_device->st_alrx.rx_normal_mdpu_succ_num++;
    }
    return;
}

void hmac_rx_netbuf_update_rxctl_data_type(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl)
{
    mac_data_type_enum_uint8 datatype = MAC_DATA_NUM;
    uint16_t frame_format = rx_ctl->bit_frame_format;
    uint32_t frame_len = rx_ctl->us_frame_len;
    uint8_t eth_type = MAC_NETBUFF_PAYLOAD_BUTT;

    if (frame_format == MAC_FRAME_TYPE_80211) {
        datatype = mac_get_data_type_from_80211(netbuf, mac_get_rx_cb_mac_header_len(rx_ctl),
            mac_get_rx_cb_frame_len(rx_ctl) - mac_get_rx_cb_mac_header_len(rx_ctl));
    } else if (frame_format == MAC_FRAME_TYPE_8023) {
        /* 跳过DA/SA/length 14字节 */
        datatype = mac_get_data_type_from_8023(netbuf, oal_netbuf_data(netbuf) + ETHER_HDR_LEN, MAC_FRAME_TYPE_8023,
            frame_len - ETHER_HDR_LEN, OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    } else if (frame_format == MAC_FRAME_TYPE_RTH) {
        eth_type = mac_get_eth_data_subtype(netbuf, frame_len);
        if (eth_type == MAC_FRAME_TYPE_8023) { // MAC_NETBUFF_PAYLOAD_SNAP is equal MAC_FRAME_TYPE_8023
            /* 跳过DA/SA/length 14字节 */
            datatype = mac_get_data_type_from_8023(netbuf, oal_netbuf_data(netbuf) + ETHER_HDR_LEN, MAC_FRAME_TYPE_8023,
                frame_len - ETHER_HDR_LEN, OAM_OTA_FRAME_DIRECTION_TYPE_RX);
        } else {
            datatype = mac_get_data_type_from_8023(netbuf, oal_netbuf_data(netbuf), MAC_FRAME_TYPE_RTH, frame_len,
                OAM_OTA_FRAME_DIRECTION_TYPE_RX);
        }
    } else {
        oam_warning_log1(0, OAM_SF_RX, "hmac_rx_netbuf_update_rxctl_data_type:frame format[%d]", frame_format);
    }

    rx_ctl->bit_data_type  = datatype;
}

/* Rxcb中amsdu非首帧的信息填充 */
OAL_STATIC void hmac_rx_update_submsdu_rxctl(mac_rx_ctl_stru *dst_rxcb,
    mac_rx_ctl_stru *src_rxcb, oal_netbuf_stru *netbuf)
{
    mac_get_rx_cb_tid(dst_rxcb) = mac_get_rx_cb_tid(src_rxcb);
    mac_get_rx_cb_hal_vap_id(dst_rxcb) = mac_get_rx_cb_hal_vap_id(src_rxcb);
    mac_rxcb_is_amsdu(dst_rxcb) = mac_rxcb_is_amsdu(src_rxcb);
    mac_get_rx_cb_is_first_sub_msdu(dst_rxcb) = OAL_FALSE;
    mac_get_rx_cb_ta_user_id(dst_rxcb) = mac_get_rx_cb_ta_user_id(src_rxcb);
    dst_rxcb->bit_rx_user_id = src_rxcb->bit_rx_user_id;

    mac_get_rx_cb_mac_vap_id(dst_rxcb) = mac_get_rx_cb_mac_vap_id(src_rxcb);
    mac_get_rx_cb_seq_num(dst_rxcb) = mac_get_rx_cb_seq_num(src_rxcb);
    dst_rxcb->us_frame_control = src_rxcb->us_frame_control;
    dst_rxcb->us_rx_msb_pn = src_rxcb->us_rx_msb_pn;
    dst_rxcb->rx_lsb_pn = src_rxcb->rx_lsb_pn;

    /* frame len刷新 当芯片上报802.11格式的AMSDU帧时，且非首个msdu不包含802.11头 */
    if (dst_rxcb->bit_frame_format == MAC_FRAME_TYPE_80211 && mac_rxcb_is_amsdu_sub_msdu(dst_rxcb)) {
        dst_rxcb->us_frame_len += dst_rxcb->uc_mac_header_len;
    }
    dst_rxcb->last_msdu_in_amsdu = (netbuf->next == NULL);
}
OAL_STATIC uint32_t hmac_rx_netbuf_check_status(mac_rx_ctl_stru *cb, oal_netbuf_stru *netbuf)
{
    uint8_t rx_msdu_dscr_len = hal_host_get_rx_msdu_dscr_len();
    uint8_t  rx_status = cb->rx_status;
    if (rx_status == HAL_RX_SUCCESS) {
        return OAL_SUCC;
    }

    if (rx_status == HAL_RX_NEW) {
        oam_error_log1(0, OAM_SF_RX, "hmac_rx_netbuf_check_status::rx_status:[%d]", rx_status);
        oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)oal_netbuf_data(netbuf), rx_msdu_dscr_len,
            OAM_OTA_TYPE_RX_MSDU_DSCR);
    }
    return OAL_FAIL;
}

OAL_STATIC OAL_INLINE void hmac_rx_exception_free_stat(hal_host_device_stru *hal_dev)
{
    hal_dev->rx_statics.rx_exception_free_cnt++;
}

/* 功能描述  : 检查cb部分字段取值范围是否有效，若无效，上报对应cb和msdu */
OAL_STATIC uint32_t hmac_rx_netbuf_check_cb_is_valid(mac_rx_ctl_stru *rx_ctl, oal_netbuf_stru *netbuf)
{
    if (mac_get_rx_cb_tid(rx_ctl) < WLAN_TID_MAX_NUM) {
        return OAL_SUCC;
    }
    oam_error_log1(0, OAM_SF_RX, "{hmac_rx_netbuf_check_cb_is_valid::invalid cb, tid[%d].}",
                   mac_get_rx_cb_tid(rx_ctl));
    hmac_rx_netbuf_force_ota_report(rx_ctl, netbuf);
    return OAL_FAIL;
}
/*
 * 功能描述  : MPDU级别(msdu或者amsdu)的netbuf成链并生成cb
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_host_rx_mpdu_build_cb(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf)
{
    dpe_wlan_device_stat_stru *device_stats = dpe_wlan_device_stat_get();
    mac_rx_ctl_stru *rxctl = NULL;
    uint8_t          first_msdu = OAL_TRUE;
    mac_rx_ctl_stru  first_rx_ctl;
    oal_netbuf_stru *next = NULL;

    memset_s(&first_rx_ctl, sizeof(mac_rx_ctl_stru), 0, sizeof(mac_rx_ctl_stru));
    while (netbuf != NULL) {
        next = oal_netbuf_next(netbuf);
        rxctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

        oal_atomic_add(&(device_stats->host_ring_rx_packets), 1);

        /* 更新软件cb信息 */
        if ((first_msdu == OAL_TRUE) && (rxctl->bit_vap_id < HAL_MAX_VAP_NUM)) {
            rxctl->uc_mac_vap_id = hal_dev->hal_vap_sts_info[rxctl->bit_vap_id].mac_vap_id;
        }
        /*  fcs错误帧提前过滤, 防止帧长错误时候取报文类型读越界 */
        if (first_msdu && (hmac_rx_netbuf_check_status(rxctl, netbuf) != OAL_SUCC)) {
            hmac_rx_exception_free_stat(hal_dev);
            return OAL_FAIL;
        }
        if (hmac_rx_netbuf_check_cb_is_valid(rxctl, netbuf) != OAL_SUCC) {
            return OAL_FAIL;
        }
        /* AMSDU专有处理流程 */
        if (mac_rxcb_is_first_msdu(rxctl) == OAL_TRUE) {
            memcpy_s(&first_rx_ctl, sizeof(mac_rx_ctl_stru), rxctl, sizeof(mac_rx_ctl_stru));
            first_msdu = OAL_FALSE;
        } else if (first_msdu == OAL_FALSE) {
            hmac_rx_update_submsdu_rxctl(rxctl, &first_rx_ctl, netbuf);
        }

        hmac_rx_netbuf_update_rxctl_data_type(netbuf, rxctl);
        netbuf = next;
    }
    return OAL_SUCC;
}

/*
 * 功能描述  : 用户加密，接收到的数据帧不加密，则过滤
 * 1.日    期  : 2020.12.28
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC hmac_rx_frame_ctrl_enum hmac_host_rx_filter_encrypt(uint8_t vap_idx, uint16_t user_idx,
    mac_rx_ctl_stru *rx_ctl)
{
    mac_header_frame_control_stru *frame_ctl = NULL;
    wlan_ciper_protocol_type_enum_uint8 user_cipher_type;
    hmac_user_stru *hmac_user = mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        return HMAC_RX_FRAME_CTRL_GOON;
    }

    frame_ctl = (mac_header_frame_control_stru *)(&rx_ctl->us_frame_control);
    user_cipher_type = hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    if (user_cipher_type == WLAN_80211_CIPHER_SUITE_WAPI && wlan_chip_is_support_hw_wapi() == OAL_FALSE) {
        /* 软件WAPI 加解密，rx_ctrl加密类型未填写，不过滤 */
        return HMAC_RX_FRAME_CTRL_GOON;
    }
    /* 加密场景接收到非加密的数据帧，则过滤；null帧、QOS null帧、eapol帧、WAI帧无需过滤 */
    if ((user_cipher_type != WLAN_80211_CIPHER_SUITE_NO_ENCRYP) &&
        (rx_ctl->bit_cipher_type == WLAN_80211_CIPHER_SUITE_NO_ENCRYP) &&
        (frame_ctl->bit_type == WLAN_DATA_BASICTYPE)) {
        uint8_t uc_data_type = rx_ctl->bit_data_type;
        if ((frame_ctl->bit_sub_type != WLAN_NULL_FRAME) &&
            (frame_ctl->bit_sub_type != WLAN_QOS_NULL_FRAME) &&
            (uc_data_type != MAC_DATA_EAPOL) &&
            (uc_data_type != MAC_DATA_WAPI)) {
            oam_warning_log1(vap_idx, OAM_SF_RX, "{hmac_host_rx_filter_encrypt::plaintext drop! data type[%d]}",
                uc_data_type);
            return HMAC_RX_FRAME_CTRL_DROP;
        }
    }
    return HMAC_RX_FRAME_CTRL_GOON;
}

/* 过滤A-MSDU首帧DA为AA-AA-03-00-00-00 的异常A-MSDU 帧 */
OAL_STATIC hmac_rx_frame_ctrl_enum hmac_host_rx_filter_abnormal_amsdu(oal_netbuf_stru *netbuf,
    mac_rx_ctl_stru *rx_cb_ctrl)
{
    uint8_t *dst_addr = NULL;
    uint8_t frame_format;
    uint8_t mac_addr_snap_header[WLAN_MAC_ADDR_LEN] = {0xAA, 0xAA, 0x03, 0x00, 0x00, 0x00};

    if (!rx_cb_ctrl->bit_amsdu_enable || !rx_cb_ctrl->bit_is_first_buffer) {
        return HMAC_RX_FRAME_CTRL_GOON;
    }
    frame_format = rx_cb_ctrl->bit_frame_format;
    if (frame_format == MAC_FRAME_TYPE_80211) {
        uint8_t *ieee80211_hdr = oal_netbuf_data(netbuf);
        /* 80211 格式A-MSDU DA在80211 帧头后 */
        dst_addr = (ieee80211_hdr + rx_cb_ctrl->uc_mac_header_len + MAC_SUBMSDU_DA_OFFSET);
    } else if (frame_format == MAC_FRAME_TYPE_RTH || frame_format == MAC_FRAME_TYPE_8023) {
        mac_ether_header_stru *eth_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
        dst_addr = eth_hdr->auc_ether_dhost;
    } else {
        return HMAC_RX_FRAME_CTRL_GOON;
    }

    if (oal_memcmp(dst_addr, mac_addr_snap_header, WLAN_MAC_ADDR_LEN) == 0) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    return HMAC_RX_FRAME_CTRL_GOON;
}

/*
 * 功能描述  : host rx预先过滤流程，主要涉及device rx部分已经设置的过滤
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC hmac_rx_frame_ctrl_enum hmac_host_rx_pre_abnormal_filter(hmac_host_rx_context_stru *rx_context)
{
    hal_host_device_stru    *hal_dev = rx_context->hal_dev;
    oal_netbuf_stru         *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru         *rx_ctl = rx_context->cb;
    dpe_wlan_vap_stru       *dpe_vap = rx_context->dpe_vap;
    dpe_wlan_user_stru      *dpe_user = rx_context->dpe_user;
    dpe_wlan_vap_stat_stru  *vap_stats = NULL;

    if (dpe_vap == NULL) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    vap_stats = dpe_wlan_vap_stat_get(dpe_vap);
    if (hal_dev->st_alrx.en_al_rx_flag != 0) {
        hmac_alrx_process(hal_dev, rx_ctl);
        HMAC_VAP_DFT_STATS_PKT_INCR(vap_stats->rx_dropped_misc, 1);
        return HMAC_RX_FRAME_CTRL_DROP;
    }
#ifdef _PRE_WLAN_FEATURE_SNIFFER
    if (hmac_host_rx_sniffer_is_on(hal_dev) == OAL_TRUE) {
        hmac_host_rx_sniffer_filter_check(hal_dev, netbuf, rx_ctl);
        return HMAC_RX_FRAME_CTRL_DROP;
    }
#endif
    if ((hmac_is_hal_vap_valid(hal_dev, rx_ctl) != OAL_SUCC) ||
        (hmac_is_tid_valid(hal_dev, rx_ctl)) != OAL_SUCC) {
        HMAC_VAP_DFT_STATS_PKT_INCR(vap_stats->rx_dropped_misc, 1);
        return HMAC_RX_FRAME_CTRL_DROP;
    }
    /* 加密收到非加密的数据帧需要过滤处理,只处理首帧 */
    if (dpe_user == NULL) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }
    if (hmac_host_rx_filter_encrypt(dpe_vap->uc_vap_id, dpe_user->us_assoc_id, rx_ctl) == HMAC_RX_FRAME_CTRL_DROP) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    /* 异常AMSDU 帧过滤 */
    if (hmac_host_rx_filter_abnormal_amsdu(netbuf, rx_ctl) == HMAC_RX_FRAME_CTRL_DROP) {
        oam_warning_log0(dpe_vap->uc_vap_id, OAM_SF_RX,
            "hmac_host_rx_pre_abnormal_filter::msdu da is snap llc header!");
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    return HMAC_RX_FRAME_CTRL_GOON;
}

OAL_STATIC dpe_wlan_user_stru *dpe_wlan_ap_rx_recheck_user_idx(dpe_wlan_vap_stru *dpe_vap,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl)
{
    mac_ieee80211_frame_stru *frmhdr = NULL;
    uint8_t *transmit_addr = NULL;
    dpe_wlan_user_stru *dpe_user = NULL;
    uint8_t usr_lut = rx_ctl->bit_rx_user_id;
    uint16_t user_idx;

    /* lut为无效值且为80211帧时，才进行查找,此为异常保护流程 */
    if ((usr_lut < g_wlan_spec_cfg->invalid_user_id) ||
        (rx_ctl->bit_frame_format != MAC_FRAME_TYPE_80211)) {
        return NULL;
    }
    frmhdr = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);
    if (frmhdr == NULL) {
        return NULL;
    }
    transmit_addr = frmhdr->auc_address2;
    if (dpe_vap_find_user_by_macaddr(dpe_vap->uc_vap_id, transmit_addr, &user_idx) == OAL_SUCC) {
        dpe_user = dpe_wlan_user_get(user_idx);
    }
    return dpe_user;
}

OAL_STATIC OAL_INLINE dpe_wlan_user_stru *dpe_wlan_host_rx_lut_get_user(dpe_wlan_vap_stru *dpe_vap,
    oal_netbuf_stru *netbuf, hal_host_device_stru *hal_dev, mac_rx_ctl_stru *cb)
{
    dpe_wlan_user_stru *dpe_user = NULL;
    uint8_t usr_lut = cb->bit_rx_user_id;
    hal_host_chip_stru *hal_chip = hal_get_host_chip();

    if ((usr_lut < HAL_MAX_LUT) && (hal_chip->user_sts_info[usr_lut].user_valid)) {
        dpe_user = dpe_wlan_user_get(hal_chip->user_sts_info[usr_lut].user_id);
    }
    if (oal_unlikely(dpe_user == NULL) &&
       !mac_rx_dscr_need_drop(cb->rx_status) && (hal_dev->st_alrx.en_al_rx_flag == HAL_ALWAYS_RX_DISABLE)) {
        oam_warning_log1(hal_dev->device_id, OAM_SF_ANY,
            "dpe_wlan_host_rx_lut_get_user:get user fail, lut idx[%d]", usr_lut);
        /* 漫游过程中接收到数据帧信息输出 */
        if (dpe_vap->en_vap_state == MAC_VAP_STATE_ROAMING) {
#ifdef _PRE_WLAN_DFT_DUMP_FRAME
            hmac_rx_report_eth_frame(dpe_vap->uc_vap_id, netbuf);
#endif
        }
    }
    return dpe_user;
}

/*
 * 功能描述  : 将cb中的user id统一更新为mac user id
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_host_rx_update_cb_user_idx(dpe_wlan_user_stru *dpe_user, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = NULL;
    uint16_t user_idx;

    if (dpe_user == NULL) {
        return;
    }

    user_idx = dpe_user->us_assoc_id;

    for (; netbuf; netbuf = oal_netbuf_next(netbuf)) {
        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        rx_ctl->bit_rx_user_id = user_idx;
    }
}

/*
 * 功能描述  : 构建host rx上下文
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_host_build_rx_context(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf,
    hmac_host_rx_context_stru *rx_context)
{
    mac_rx_ctl_stru *cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    dpe_wlan_vap_stru *dpe_vap = NULL;
    dpe_wlan_user_stru *dpe_user = NULL;

    if (cb == NULL) {
        return OAL_FAIL;
    }
    dpe_vap = dpe_wlan_vap_get(cb->uc_mac_vap_id);
    if (dpe_vap == NULL) {
        oam_error_log0(cb->uc_mac_vap_id, OAM_SF_ANY, "hmac_host_build_rx_context:vap null");
        return OAL_FAIL;
    }
    rx_context->hal_dev = hal_dev;
    dpe_user = dpe_wlan_host_rx_lut_get_user(dpe_vap, netbuf, hal_dev, cb);
    if ((dpe_user == NULL) && (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP)) {
        dpe_user = dpe_wlan_ap_rx_recheck_user_idx(dpe_vap, netbuf, cb);
    }
    hmac_host_rx_update_cb_user_idx(dpe_user, netbuf);
    rx_context->dpe_vap = dpe_vap;
    rx_context->dpe_user = dpe_user;
    rx_context->netbuf = netbuf;
    rx_context->cb = cb;
    return OAL_SUCC;
}

/*
 * 功能描述  : host rx流程预处理函数，主要包括AMSDU成链，cb生成，device部分过滤操作
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_host_rx_proc_dev_routine(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, hmac_host_rx_context_stru *rx_context)
{
    /* 维测 */
    hal_host_rx_proc_msdu_dscr(netbuf);

    if (hmac_host_rx_mpdu_build_cb(hal_dev, netbuf) != OAL_SUCC) {
        return OAL_FAIL;
    }
    if (hmac_host_build_rx_context(hal_dev, netbuf, rx_context) != OAL_SUCC) {
        return OAL_FAIL;
    }
    if (hmac_host_rx_pre_abnormal_filter(rx_context) != HMAC_RX_FRAME_CTRL_GOON) {
        hmac_rx_exception_free_stat(hal_dev);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_host_get_rx_msdu_info(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rxctl = NULL;
    oal_netbuf_stru *next = NULL;
    uint8_t          first_msdu = OAL_TRUE;

    while (netbuf != NULL) {
        next = oal_netbuf_next(netbuf);
        rxctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        /* 统一获取参数接口 */
        if (hal_host_rx_get_msdu_info_dscr(hal_dev, netbuf, rxctl, first_msdu) != OAL_SUCC) {
            return OAL_FAIL;
        }

        /* 首帧标记 */
        if (mac_rxcb_is_first_msdu(rxctl) == OAL_TRUE) {
            first_msdu = OAL_FALSE;
        }
        netbuf = next;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_host_rx_mpdu_proc(
    hal_host_device_stru *origin_dev, oal_netbuf_stru *mpdu, oal_netbuf_head_stru *rpt_list)
{
    hmac_host_rx_context_stru rx_context = { 0 };
    hal_host_device_stru *hal_dev = NULL;

    /* 1. 外层异常处理基于链表的形式释放netbuf, 此处需要优先建链 */
    hal_host_rx_amsdu_list_build(origin_dev, mpdu);

    /* 2. rx msdu info归一化处理接口 */
    if (hmac_host_get_rx_msdu_info(origin_dev, mpdu) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 3. link选择 */
    hal_dev = hal_host_rx_get_mpdu_link(origin_dev, mpdu);
    if (oal_unlikely(hal_dev == NULL)) {
        oam_error_log0(0, 0, "{hmac_host_rx_mpdu_proc::get mpdu link failed}");
        return OAL_FAIL;
    }

    /* 4. 构建接收流程上下文 */
    if (hmac_host_rx_proc_dev_routine(hal_dev, mpdu, &rx_context) != OAL_SUCC) {
        return OAL_FAIL;
    }
#ifdef _PRE_WLAN_FEATURE_VSP
    if (hmac_vsp_rx_proc(rx_context.dpe_user->us_assoc_id, mpdu) == VSP_RX_ACCEPT) {
        /* vsp帧不采用上报协议栈方式，单独处理 */
        return OAL_SUCC;
    }
#endif
    /* 5. 业务流程上下文 */
    hmac_rx_route_print(rx_context.hal_dev, HOST_RX_REPORT, rx_context.cb);
    hmac_rx_common_proc(&rx_context, rpt_list);

    return OAL_SUCC;
}

OAL_STATIC void hmac_host_rx_mpdu_list_proc(hal_host_device_stru *origin_dev, oal_netbuf_head_stru *mpdu_list)
{
    oal_netbuf_head_stru rpt_list;
    oal_netbuf_stru *mpdu = oal_netbuf_delist(mpdu_list);

    host_cnt_init_record_performance(RX_RING_PROC);
    host_start_record_performance(RX_RING_PROC);

    oal_netbuf_list_head_init(&rpt_list);
    for (; mpdu; mpdu = oal_netbuf_delist(mpdu_list)) {
        host_cnt_inc_record_performance(RX_RING_PROC);
        if (hmac_host_rx_mpdu_proc(origin_dev, mpdu, &rpt_list) != OAL_SUCC) {
            hmac_rx_netbuf_list_free(mpdu);
            continue;
        }
    }

    if (oal_netbuf_list_len(&rpt_list)) {
        hmac_rx_rpt_netbuf(&rpt_list);
    }

    host_end_record_performance(host_cnt_get_record_performance(RX_RING_PROC), RX_RING_PROC);
}

/*
 * 功能描述  : host rx ring数据处理接口
 * 1.日    期  : 2020年1月5日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_host_rx_ring_data_event(frw_event_mem_stru *event_mem)
{
    hal_host_rx_event *wlan_rx_event = NULL;
    oal_netbuf_head_stru mpdu_list;
    oal_netbuf_head_stru unmap_list;

    if (oal_unlikely(event_mem == NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    wlan_rx_event = (hal_host_rx_event *)frw_get_event_stru(event_mem)->auc_event_data;
    if (oal_unlikely(wlan_rx_event->hal_dev == NULL)) {
        return OAL_FAIL;
    }

    oal_netbuf_list_head_init(&mpdu_list);
    oal_netbuf_list_head_init(&unmap_list);
    hal_host_rx_mpdu_que_pop(wlan_rx_event->hal_dev, &mpdu_list);
    hal_host_rx_dma_unmap_proc(&mpdu_list, &unmap_list);
    hmac_host_rx_mpdu_list_proc(wlan_rx_event->hal_dev, &unmap_list);

    return OAL_SUCC;
}
