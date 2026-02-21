/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 发送方向分片功能
 * 作    者 : wifi
 * 创建日期 : 2014年2月15日
 */
#include "hmac_frag.h"
#include "hmac_11i.h"
#include "dpe_wlan_user.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_FRAG_C

#define HMAC_FRAG_HEADER_RESV_LEN 32

/*
 * 功能描述  : 从pst_netbuf_original拷贝分片帧体和帧头到netbuf
 * 1.日    期  : 2020年4月29日
 */
OAL_STATIC mac_ieee80211_frame_stru *hmac_frag_init_frag_netbuf(oal_netbuf_stru *pst_netbuf_original,
    mac_tx_ctl_stru *pst_tx_ctl, uint32_t offset, oal_netbuf_stru *pst_netbuf, uint32_t payload)
{
    mac_tx_ctl_stru *pst_tx_ctl_copy = NULL;
    mac_ieee80211_frame_stru *pst_mac_header;
    uint32_t mac_hdr_size;
    mac_ieee80211_frame_stru *pst_frag_header = NULL;

    mac_hdr_size = mac_get_cb_frame_header_length(pst_tx_ctl);
    pst_mac_header = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_cb_frame_header_addr(pst_tx_ctl);
    pst_mac_header->st_frame_control.bit_more_frag = OAL_TRUE;

    pst_tx_ctl_copy = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    /* 拷贝cb字段 */
    if (memcpy_s(pst_tx_ctl_copy, MAC_TX_CTL_SIZE, pst_tx_ctl, MAC_TX_CTL_SIZE) != EOK) {
        return NULL;
    }
    oal_netbuf_copy_queue_mapping(pst_netbuf, pst_netbuf_original);

    /* netbuf的headroom大于802.11 mac头长度 */
    pst_frag_header =
        (mac_ieee80211_frame_stru *)(get_netbuf_payload(pst_netbuf) - MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
    mac_get_cb_80211_mac_head_type(pst_tx_ctl_copy) = 1; /* 指示mac头部在skb中 */

    /* 拷贝帧头内容 */
    if (memcpy_s(pst_frag_header, mac_hdr_size, pst_mac_header, mac_hdr_size) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_frag_init_netbuf::memcpy fail!");
        return NULL;
    }

    oal_netbuf_copydata(pst_netbuf_original, offset, get_netbuf_payload(pst_netbuf), payload);
    oal_netbuf_set_len(pst_netbuf, payload);
    mac_get_cb_frame_header_addr(pst_tx_ctl_copy) = (uint64_t)(uintptr_t)pst_frag_header;
    mac_get_cb_mpdu_len(pst_tx_ctl_copy) = (uint16_t)payload;

    return pst_frag_header;
}
/*
 * 函 数 名  : hmac_frag_process
 * 功能描述  : 报文分片处理
 * 1.日    期  : 2014年2月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_frag_process(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *pst_netbuf_original, mac_tx_ctl_stru *pst_tx_ctl,
    uint32_t cip_hdrsize, uint32_t max_tx_unit)
{
    mac_ieee80211_frame_stru *pst_frag_header = NULL;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_stru *pst_netbuf_prev = NULL;
    uint32_t total_hdrsize, frag_num, frag_size, payload, offset, mac_hdr_size;
    int32_t l_remainder;

    mac_hdr_size = mac_get_cb_frame_header_length(pst_tx_ctl);
    total_hdrsize = mac_hdr_size + cip_hdrsize;
    frag_num = 1;
    /* 加密字节数包含在分片门限中，预留加密字节长度，由硬件填写加密头 */
    offset = max_tx_unit - cip_hdrsize - mac_hdr_size;
    l_remainder = (int32_t)(oal_netbuf_len(pst_netbuf_original) - offset);
    pst_netbuf_prev = pst_netbuf_original;

    do {
        frag_size = total_hdrsize + (uint32_t)l_remainder;

        /* 判断是否还有更多的分片 */
        if (frag_size > max_tx_unit) {
            frag_size = max_tx_unit;
        }

        /* 防止反复扩充头部空间预留32字节 */
        pst_netbuf = oal_netbuf_alloc(frag_size + MAC_80211_QOS_HTC_4ADDR_FRAME_LEN + HMAC_FRAG_HEADER_RESV_LEN,
            MAC_80211_QOS_HTC_4ADDR_FRAME_LEN + HMAC_FRAG_HEADER_RESV_LEN, 4); /* 4 字节对齐 */
        if (pst_netbuf == NULL) {
            /* 在外部释放之前申请的报文 */
            oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_frag_process::netbuf null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        /* 计算分片报文帧体长度 */
        payload = frag_size - total_hdrsize;

        /* 赋值分片号 */
        pst_frag_header = hmac_frag_init_frag_netbuf(pst_netbuf_original, pst_tx_ctl, offset,
                                                     pst_netbuf, payload);
        if (pst_frag_header == NULL) {
            oal_netbuf_free(pst_netbuf);
            return OAL_FAIL;
        }
        pst_frag_header->bit_frag_num = frag_num;
        frag_num++;
        oal_netbuf_next(pst_netbuf_prev) = pst_netbuf;
        pst_netbuf_prev = pst_netbuf;

        /* 计算下一个分片报文的长度和偏移 */
        l_remainder -= (int32_t)payload;
        offset += payload;
    } while (l_remainder > 0);

    pst_frag_header->st_frame_control.bit_more_frag = OAL_FALSE;
    oal_netbuf_next(pst_netbuf) = NULL;

    /* 原始报文作为分片报文的第一个 */
    oal_netbuf_trim(pst_netbuf_original, oal_netbuf_len(pst_netbuf_original) -
                    (max_tx_unit - cip_hdrsize - mac_hdr_size));
    mac_get_cb_mpdu_len(pst_tx_ctl) = (uint16_t)(oal_netbuf_len(pst_netbuf_original));

    return OAL_SUCC;
}
