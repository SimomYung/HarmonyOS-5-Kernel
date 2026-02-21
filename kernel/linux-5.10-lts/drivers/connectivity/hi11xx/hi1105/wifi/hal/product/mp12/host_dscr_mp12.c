/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL DSCR FUNCTION
 * 作    者 :
 * 创建日期 : 2021年2月22日
 */

#include "oal_util.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "oam_event_wifi.h"
#include "host_hal_dscr.h"
#include "host_hal_ring.h"
#include "host_hal_device.h"
#include "host_hal_ext_if.h"
#include "host_mac_mp12.h"
#include "hmac_tx_complete.h"
#include "hmac_tid_sche.h"
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_source.h"
#include "hmac_vsp_if.h"
#endif
#include "hmac_stat.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_ring_alloc.h"
#if(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#include "host_tx_mp12.h"
#include "host_dscr_mp12.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_DSCR_MP12_C

static hal_ring_mac_regs_info g_ring_reg_tbl[HAL_RING_ID_BUTT] = {
    [HAL_RING_TYPE_L_F] = {
        .base_lsb = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG,
        .rx_ring_reset = MAC_COMMON_REG_RX_RING_RESET_CTRL_REG,
    },
    [HAL_RING_TYPE_S_F] = {
        .base_lsb = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG,
        .rx_ring_reset = MAC_COMMON_REG_RX_RING_RESET_CTRL_REG,
    },
    [HAL_RING_TYPE_COMP] = {
        .base_lsb = MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_LSB_REG,
        .base_msb = MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_MSB_REG,
        .len      = MAC_RPU_REG_RX_DATA_BUFF_LEN_REG,
        .size     = MAC_COMMON_REG_RX_DATA_CMP_RING_SIZE_REG,
        .wptr_reg = MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_REG,
        .rptr_reg = MAC_COMMON_REG_RX_DATA_CMP_RING_RPTR_REG,
        .cfg_wptr_reg = MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_CFG_REG,
        .rx_ring_reset = MAC_COMMON_REG_RX_RING_RESET_CTRL_REG,
    },
    /* TODO：暂时只用ml0 */
    [TX_BA_INFO_RING] = {
        .base_lsb = MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_LSB_0_REG,
        .base_msb = MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_MSB_0_REG,
        .len      = 0,
        .size     = MAC_LUT_REG_TX_BA_INFO_BUF_DEPTH_0_REG,
        .wptr_reg = MAC_LUT_REG_ML0_TX_INFO_BUF_WPTR_REG,
        .rptr_reg = MAC_LUT_REG_TX_BA_INFO_RPTR_0_REG,
        .cfg_wptr_reg = MAC_LUT_REG_TX_BA_INFO_WPTR_0_REG,
        .rx_ring_reset = MAC_COMMON_REG_RX_RING_RESET_CTRL_REG,
    },
#ifdef _PRE_WLAN_FEATURE_SNIFFER
    [HAL_RING_TYPE_P_F] = {
        .base_lsb = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_LSB_REG,
        .base_msb = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_MSB_REG,
        .size     = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_SIZE_REG,
        .wptr_reg = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_WPTR_REG,
        .rptr_reg = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_REG,
        .cfg_wptr_reg = MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_CFG_REG,
        .rx_ring_reset = MAC_COMMON_REG_RX_RING_RESET_CTRL_REG,
    }
#endif
};

uint32_t g_mac_reg_offset_mp12[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP12] = {0};
uint32_t mp12_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr)
{
    uint32_t mac_reg_offset = g_mac_reg_offset_mp12[device_id];

    if (mp12_check_is_mac_common_reg_addr(reg_addr)) {
        mac_reg_offset = 0;
    }
    return mac_reg_offset;
}
hal_ring_mac_regs_info* mp12_get_ring_mac_regs(void)
{
    return g_ring_reg_tbl;
}

oal_netbuf_stru *mp12_rx_get_next_sub_msdu(hal_host_device_stru *hal_device,
    oal_netbuf_stru *netbuf)
{
    mp12_rx_msdu_dscr_stru     *rx_hw_dscr = NULL;
    uint64_t                     next_dscr_addr;

    hal_device->rx_statics.comp_ring_get++;
    rx_hw_dscr = (mp12_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);
    if (oal_unlikely(rx_hw_dscr == NULL)) {
        return NULL;
    }
    if ((!rx_hw_dscr->next_sub_msdu_addr_lsb) && (!rx_hw_dscr->next_sub_msdu_addr_msb)) {
        return NULL;
    }

    next_dscr_addr = oal_make_word64(rx_hw_dscr->next_sub_msdu_addr_lsb, rx_hw_dscr->next_sub_msdu_addr_msb);
    return hal_rx_get_sub_msdu(hal_device, next_dscr_addr);
}

void mp12_host_rx_amsdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next_netbuf = NULL;

    while (netbuf != NULL) {
        next_netbuf = mp12_rx_get_next_sub_msdu(hal_device, netbuf);
        oal_netbuf_next(netbuf) = next_netbuf;
        netbuf = next_netbuf;
    }
}

uint8_t mp12_host_get_rx_msdu_status(oal_netbuf_stru *netbuf)
{
    mp12_rx_msdu_dscr_stru *rx_hw_dscr = NULL;

    rx_hw_dscr = (mp12_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);
    if (rx_hw_dscr == NULL) {
        return HAL_RX_NEW;
    }
    return rx_hw_dscr->rx_status;
}


uint8_t mp12_rx_is_frag_frm(mac_header_frame_control_stru *fc_ctl_hdr, mp12_rx_msdu_dscr_stru *rx_msdu_hw_dscr)
{
    return (uint8_t)((rx_msdu_hw_dscr->frame_format == MAC_FRAME_TYPE_80211) &&
        (fc_ctl_hdr->bit_more_frag || rx_msdu_hw_dscr->fragment_num));
}
OAL_STATIC void mp12_host_rx_cb_set_first_msdu(mac_rx_ctl_stru *rx_ctl, mp12_rx_msdu_dscr_stru *rx_hw_dscr)
{
    rx_ctl->bit_mcast_bcast     = rx_hw_dscr->mcast_bcast;
    rx_ctl->bit_amsdu_enable    = rx_hw_dscr->is_amsdu;
    rx_ctl->bit_buff_nums       = 1;
    rx_ctl->us_frame_control    = rx_hw_dscr->frame_control;
    rx_ctl->bit_start_seqnum    = rx_hw_dscr->start_seqnum;
    rx_ctl->bit_cipher_type     = rx_hw_dscr->cipher_type;
    rx_ctl->bit_release_start_sn = rx_hw_dscr->release_start_seqnum;
    rx_ctl->bit_release_end_sn  = rx_hw_dscr->release_end_seqnum;
    rx_ctl->bit_fragment_num    = rx_hw_dscr->fragment_num;
    rx_ctl->bit_process_flag    = rx_hw_dscr->process_flag;
    rx_ctl->bit_rx_user_id      = rx_hw_dscr->user_id;
    rx_ctl->us_seq_num          = rx_hw_dscr->sequence_num;
    rx_ctl->bit_is_first_buffer = rx_hw_dscr->first_sub_msdu;
    rx_ctl->bit_rx_tid          = rx_hw_dscr->tid;
    rx_ctl->rx_status           = rx_hw_dscr->rx_status;
    rx_ctl->bit_is_ampdu        = rx_hw_dscr->is_ampdu;
    rx_ctl->uc_msdu_in_buffer   = rx_hw_dscr->sub_msdu_num;
    rx_ctl->rx_lsb_pn           = rx_hw_dscr->pn_lsb;
    rx_ctl->us_rx_msb_pn        = rx_hw_dscr->pn_msb;
    rx_ctl->bit_vap_id          = rx_hw_dscr->vap_id;
    rx_ctl->bit_release_valid   = rx_hw_dscr->release_is_valid;
    rx_ctl->bit_band_id         = rx_hw_dscr->band_id;
    rx_ctl->bit_ta_user_idx     = rx_hw_dscr->user_id & 0x1f; /* 1f valid bits */
    rx_ctl->last_msdu_in_amsdu  = rx_hw_dscr->sub_msdu_num == 0;
}

static void mp12_host_rx_cb_set(hal_host_device_stru *hal_dev,
    mac_rx_ctl_stru *rx_ctl, mp12_rx_msdu_dscr_stru *rx_hw_dscr, uint8_t first_msdu)
{
    mac_header_frame_control_stru *fc_ctl_hdr = NULL;
    if (oal_any_null_ptr2(rx_ctl, rx_hw_dscr)) {
        return;
    }
    if (first_msdu) {
        mp12_host_rx_cb_set_first_msdu(rx_ctl, rx_hw_dscr);
        if (rx_hw_dscr->vap_id != WLAN_HAL_OHTER_BSS_ID_MP12) {
            rx_ctl->bit_vap_id = hal_hw_vap_id_to_hal_vap_id(rx_ctl->bit_vap_id);
        }
        if (rx_ctl->bit_vap_id < HAL_MAX_VAP_NUM_MP12) {
            rx_ctl->uc_mac_vap_id = hal_dev->hal_vap_sts_info[rx_ctl->bit_vap_id].mac_vap_id;
        }
        fc_ctl_hdr = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;
        if (mp12_rx_is_frag_frm(fc_ctl_hdr, rx_hw_dscr)) {
            rx_ctl->bit_is_fragmented = OAL_TRUE;
        }
    }

    /* msdu级别 */
    rx_ctl->bit_frame_format    = rx_hw_dscr->frame_format; /* 0=eht2, 1=802.3, 3=802.11 */
    rx_ctl->dst_hal_vap_id      = rx_hw_dscr->dst_vap_id;
    rx_ctl->us_frame_len        = rx_hw_dscr->packet_len;
    rx_ctl->bit_dst_band_id     = rx_hw_dscr->dst_band_id;
    rx_ctl->bit_data_type       = rx_hw_dscr->rx_data_type;

    rx_ctl->bit_ptlcs_valid     = rx_hw_dscr->ptlcs_valid;
    rx_ctl->bit_ptlcs_pass      = rx_hw_dscr->ptlcs_pass;
    rx_ctl->bit_ipv4cs_pass     = rx_hw_dscr->ipv4cs_pass;
    rx_ctl->bit_ipv4cs_valid    = rx_hw_dscr->ipv4cs_valid;
}

/*
 * 功能描述  : cb中长度计算，并调整netbuf数据部分头尾指针
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static uint32_t mp12_host_rx_adjust_netbuf_len(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, mp12_rx_msdu_dscr_stru *rx_hw_dscr, uint8_t first_msdu)
{
    pcie_cb_dma_res               *cb_res = NULL;

    cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
    if (cb_res == NULL) {
        return OAL_FAIL;
    }

    rx_ctl->uc_mac_header_len = hal_host_rx_get_mac_header_len(rx_ctl);

    /* 1.fcs错误，后续字段信息不需要查看，本身不可信  2.fcs错误的amsdu非首帧信息本身不回填，字段不可信 */
    if ((rx_hw_dscr->rx_status == HAL_RX_SUCCESS) &&
        (rx_hw_dscr->packet_len == 0 || (rx_hw_dscr->packet_len > (cb_res->len - MP12_RX_MSDU_DSCR_LEN)))) {
        oam_error_log3(0, OAM_SF_RX, "host_rx_adjust_netbuf_len:dscr addr:0x%x, pkt len:%d, pcie cb len:%d",
            (uintptr_t)rx_hw_dscr, rx_hw_dscr->packet_len, cb_res->len);
        oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)rx_hw_dscr, MP12_RX_MSDU_DSCR_LEN, OAM_OTA_TYPE_RX_MSDU_DSCR);
        return OAL_FAIL;
    }

    oal_netbuf_pull(netbuf, MP12_RX_MSDU_DSCR_LEN);
    skb_trim(netbuf, rx_hw_dscr->packet_len);
    return OAL_SUCC;
}

uint32_t mp12_host_rx_get_msdu_info_dscr(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, uint8_t first_msdu)
{
    mp12_rx_msdu_dscr_stru      *rx_msdu_hw_dscr = NULL;
    uint32_t                       ret;

    memset_s(rx_ctl, sizeof(mac_rx_ctl_stru), 0, sizeof(mac_rx_ctl_stru));
    rx_msdu_hw_dscr = (mp12_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);
    mp12_host_rx_cb_set(hal_dev, rx_ctl, rx_msdu_hw_dscr, first_msdu);
    ret = mp12_host_rx_adjust_netbuf_len(hal_dev, netbuf, rx_ctl, rx_msdu_hw_dscr, first_msdu);
    if (ret != OAL_SUCC) {
        return ret;
    }

    if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        rx_ctl->st_expand_cb.pul_mac_hdr_start_addr = (uint32_t *)oal_netbuf_data(netbuf);
    }
    return OAL_SUCC;
}

hal_host_device_stru *mp12_host_rx_get_mpdu_link(hal_host_device_stru *origin_dev, oal_netbuf_stru *mpdu)
{
    mp12_rx_msdu_dscr_stru *msdu_dscr = (mp12_rx_msdu_dscr_stru *)oal_netbuf_data(mpdu);
    uint8_t link_id;
    uint8_t dev_num = mac_chip_get_hal_dev_num();
    if (msdu_dscr == NULL) {
        return NULL;
    }

    link_id = msdu_dscr->link_id;
    return link_id < dev_num ? hal_get_host_device(link_id) : NULL;
}

#define MP12_TX_MSDU_DSCR_LEN 20
void mp12_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp12_tx_msdu_dscr_stru *tx_msdu_dscr = (mp12_tx_msdu_dscr_stru *)((uintptr_t)oal_netbuf_data(netbuf) -
                                            MP12_TX_MSDU_DSCR_LEN);

    tx_msdu_info->seq_num = tx_msdu_dscr->seq_num;
    tx_msdu_info->sn_vld = tx_msdu_dscr->sn_vld;
    tx_msdu_info->tx_count = tx_msdu_dscr->ml0_txed_count;
}

uint8_t mp12_host_get_tx_tid_ring_size(uint8_t ring_size_grade, uint8_t tid_no)
{
    /* 根据tid流量大小设置对应的tx_ring内存大小 */
    /* 申请失败方案:降低ring_size_grade继续申请 */
    /*
    |   tid_no  |   ring_depth   |                mp12_ring_size             |         备注          |
    |   0/4/5/7 |      4096      |   HAL_HOST_USER_TID_TX_RING_SIZE_LARGE  12   |  流量较大,常发走tid7  |
    |      6    |      2048      |   HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL 11   |    语音VO,流量中等    |
    |    1/2/3  |      1024      |   HAL_HOST_USER_TID_TX_RING_SIZE_SMALL  10   |       流量较小        |
    注:1.mp16最小ring_depth为128,mp16_ring_depth = 128 << mp16_ring_size;
       2.mp12最小ring_depth为1,mp12_ring_depth = 1 << mp12_ring_size;
    */
    const uint8_t ring_size[WLAN_TID_MAX_NUM][HAL_TX_RING_SIZE_GRADE_BUTT] = {
        { HAL_HOST_USER_TID_TX_RING_SIZE_LARGE, HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_SMALL, HAL_HOST_USER_TID_TX_RING_SIZE_SMALL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_SMALL, HAL_HOST_USER_TID_TX_RING_SIZE_SMALL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_SMALL, HAL_HOST_USER_TID_TX_RING_SIZE_SMALL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_LARGE, HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_LARGE, HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL, HAL_HOST_USER_TID_TX_RING_SIZE_SMALL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
        { HAL_HOST_USER_TID_TX_RING_SIZE_LARGE, HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL,
          HAL_HOST_USER_TID_TX_RING_SIZE_SMALL },
    };

    if (ring_size_grade >= HAL_TX_RING_SIZE_GRADE_BUTT || tid_no >= WLAN_TID_MAX_NUM) {
        oam_error_log2(0, 0, "host_get_tx_tid_ring_size::error param grade[%d] tid_no[%d]", ring_size_grade, tid_no);
        return HAL_HOST_USER_TID_TX_RING_SIZE_ERROR;
    }

    return ring_size[tid_no][ring_size_grade];
}
uint32_t mp12_host_get_tx_tid_ring_depth(uint8_t size)
{
    return ((1) << (size));
}
void mp12_host_rx_tcp_udp_checksum(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *cur_netbuf = netbuf;
    oal_netbuf_stru *next_netbuf = NULL;
    mac_rx_ctl_stru *rx_ctrl = NULL;
    while (cur_netbuf != NULL) {
        next_netbuf = cur_netbuf->next;
        rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(cur_netbuf);
        if (rx_ctrl->bit_ptlcs_valid && rx_ctrl->bit_ptlcs_pass) {
            cur_netbuf->ip_summed = CHECKSUM_UNNECESSARY;
        }
        hmac_rx_checksum_stat(rx_ctrl);
        cur_netbuf = next_netbuf;
    }
}
oal_bool_enum_uint8 mp12_host_rx_ip_checksum_is_pass(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (rx_ctrl == NULL) {
        return OAL_FALSE;
    }
    if  (rx_ctrl->bit_ipv4cs_valid && (rx_ctrl->bit_ipv4cs_pass == 0)) {
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}

#ifdef _PRE_WLAN_FEATURE_VSP
void mp12_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp12_tx_msdu_dscr_stru *tx_msdu_dscr = (mp12_tx_msdu_dscr_stru *)buffer;

    tx_msdu_info->seq_num = tx_msdu_dscr->seq_num;
    tx_msdu_info->sn_vld = tx_msdu_dscr->sn_vld;
    tx_msdu_info->tx_count = tx_msdu_dscr->ml0_txed_count;
}

#define MAX_BUFFER_IN_RECYCLE_LIST 20
uint32_t mp12_host_rx_buff_recycle(hal_host_device_stru *hal_device, oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_stru *netbuf = NULL;
    uint32_t netbuf_total_len, headroom;
    uint32_t recycle_cnt = 0;

    if (oal_netbuf_list_len(&hal_device->vsp_netbuf_recycle_list) > MAX_BUFFER_IN_RECYCLE_LIST) {
        oal_netbuf_list_purge(netbuf_head);
        return 0;
    }

    while ((netbuf = oal_netbuf_delist_nolock(netbuf_head)) != NULL) {
        headroom = oal_netbuf_headroom(netbuf);
        netbuf_total_len = headroom + oal_netbuf_get_len(netbuf) + oal_netbuf_tailroom(netbuf);
        if (netbuf_total_len < LARGE_NETBUF_SIZE + MP12_RX_MSDU_DSCR_LEN) {
            oam_warning_log1(0, 0, "{host_rx_buff_recycle::buffer room[%d] not sufficent}", netbuf_total_len);
            oal_netbuf_free(netbuf);
            continue;
        }
        oal_netbuf_push(netbuf, headroom);
        oal_netbuf_set_len(netbuf, LARGE_NETBUF_SIZE + MP12_RX_MSDU_DSCR_LEN);
        oal_netbuf_add_to_list_tail(netbuf, &hal_device->vsp_netbuf_recycle_list);
        recycle_cnt++;
    }

    return recycle_cnt;
}
#endif

uint8_t mp12_host_tx_get_msdu_dscr_len(void)
{
    return MP12_TX_MSDU_DSCR_LEN;
}

uint8_t mp12_host_get_rx_msdu_dscr_len(void)
{
    return MP12_RX_MSDU_DSCR_LEN;
}

void mp12_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device)
{
    hal_host_writel(MP12_D2H_RX_RING_RESET, hal_device->st_host_rx_normal_free_ring.rx_ring_reset);
}
