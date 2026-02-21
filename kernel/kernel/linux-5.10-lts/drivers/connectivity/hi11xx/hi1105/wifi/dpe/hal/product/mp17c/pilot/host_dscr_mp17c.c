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
#include "host_mac_mp17c.h"
#include "hmac_tx_complete.h"
#include "hmac_tid_sche.h"
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_source.h"
#include "hmac_vsp_if.h"
#endif
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_ring_alloc.h"
#if(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#include "host_dscr_mp17c.h"
#include "host_tx_mp17c.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_DSCR_MP17C_C

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

uint32_t g_mac_reg_offset_mp17c[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP17C] = {0, MP17C_MAC_BANK_REG_OFFSET};
uint32_t mp17c_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr)
{
    uint32_t mac_reg_offset = g_mac_reg_offset_mp17c[device_id];

    if (mp17c_check_is_mac_common_reg_addr(reg_addr)) {
        mac_reg_offset = 0;
    }
    return mac_reg_offset;
}
hal_ring_mac_regs_info* mp17c_get_ring_mac_regs(void)
{
    return g_ring_reg_tbl;
}

oal_netbuf_stru *mp17c_rx_get_next_sub_msdu(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    mp17c_rx_msdu_dscr_stru     *rx_hw_dscr = (mp17c_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);
    mac_rx_ctl_stru             *rx_cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    uint64_t                     next_dscr_addr;

    /* 统一清空rx cb字段 */
    memset_s(rx_cb, sizeof(mac_rx_ctl_stru), 0, sizeof(mac_rx_ctl_stru));

    hal_device->rx_statics.comp_ring_get++;

    if (oal_unlikely(rx_hw_dscr == NULL || rx_cb == NULL)) {
        return NULL;
    }
    if ((!rx_hw_dscr->next_sub_msdu_addr_lsb) && (!rx_hw_dscr->next_sub_msdu_addr_msb)) {
        return NULL;
    }

    /* sub msdu赋值 */
    rx_cb->next_sub_msdu_addr_lsb = rx_hw_dscr->next_sub_msdu_addr_lsb;
    rx_cb->next_sub_msdu_addr_msb = rx_hw_dscr->next_sub_msdu_addr_msb;

    next_dscr_addr = oal_make_word64(rx_cb->next_sub_msdu_addr_lsb, rx_cb->next_sub_msdu_addr_msb);
    return hal_rx_get_sub_msdu(hal_device, next_dscr_addr);
}

OAL_STATIC void mp17c_host_rx_cb_set_firts_msdu(mac_rx_ctl_stru *rx_ctl, mp17c_rx_msdu_dscr_stru *rx_hw_dscr)
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

static void mp17c_host_rx_cb_set(mac_rx_ctl_stru *rx_ctl, oal_netbuf_stru *netbuf, uint8_t first_msdu)
{
    mp17c_rx_msdu_dscr_stru      *rx_hw_dscr = (mp17c_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);

    if (rx_hw_dscr == NULL) {
        return;
    }
    if (first_msdu) {
        /* link获取skb */
        rx_ctl->link_id = rx_hw_dscr->link_id;

        mp17c_host_rx_cb_set_firts_msdu(rx_ctl, rx_hw_dscr);
        if (rx_hw_dscr->vap_id != WLAN_HAL_OHTER_BSS_ID_MP17C) {
            rx_ctl->bit_vap_id = mp17c_hw_vap_id_to_hal_vap_id(rx_ctl->bit_vap_id);
        }

        if (hal_rx_is_frag_frm(rx_ctl)) {
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

    rx_ctl->rx_status     = rx_hw_dscr->rx_status;
    rx_ctl->uc_mac_header_len = hal_host_rx_get_mac_header_len(rx_ctl);
}

uint32_t mp17c_host_rx_get_msdu_info_dscr(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, uint8_t first_msdu)
{
    mp17c_host_rx_cb_set(rx_ctl, netbuf, first_msdu);

    /* SKB->payload区间 */
    if (hal_rx_adjust_netbuf_len(netbuf, rx_ctl) != OAL_SUCC) {
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

void mp17c_get_tx_msdu_dscr_info(hal_tx_msdu_dscr_info_stru *tx_msdu_info, mp17c_tx_msdu_dscr_stru *tx_msdu_dscr)
{
    tx_msdu_info->seq_num = tx_msdu_dscr->seq_num;
    tx_msdu_info->sn_vld = tx_msdu_dscr->sn_vld;
    tx_msdu_info->tx_count = tx_msdu_dscr->ml0_txed_count;
}
void mp17c_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp17c_tx_msdu_dscr_stru *tx_msdu_dscr = (mp17c_tx_msdu_dscr_stru *)((uintptr_t)oal_netbuf_data(netbuf) -
                                            MP17C_TX_MSDU_DSCR_LEN);

    mp17c_get_tx_msdu_dscr_info(tx_msdu_info, tx_msdu_dscr);
}
#ifdef _PRE_WLAN_FEATURE_VSP
void mp17c_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp17c_tx_msdu_dscr_stru *tx_msdu_dscr = (mp17c_tx_msdu_dscr_stru *)buffer;

    mp17c_get_tx_msdu_dscr_info(tx_msdu_info, tx_msdu_dscr);
}
#endif
uint8_t mp17c_host_get_tx_tid_ring_size(uint8_t ring_size_grade, uint8_t tid_no)
{
    /* 根据tid流量大小设置对应的tx_ring内存大小 */
    /* 申请失败方案:降低ring_size_grade继续申请 */
    /*
    |   tid_no  |   ring_depth   |                mp17c_ring_size             |         备注          |
    |   0/4/5/7 |      4096      |   HAL_HOST_USER_TID_TX_RING_SIZE_LARGE  12   |  流量较大,常发走tid7  |
    |      6    |      2048      |   HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL 11   |    语音VO,流量中等    |
    |    1/2/3  |      1024      |   HAL_HOST_USER_TID_TX_RING_SIZE_SMALL  10   |       流量较小        |
    注:1.mp16最小ring_depth为128,mp16_ring_depth = 128 << mp16_ring_size;
       2.mp17c最小ring_depth为1,mp17c_ring_depth = 1 << mp17c_ring_size;
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
uint32_t mp17c_host_get_tx_tid_ring_depth(uint8_t size)
{
    return ((1) << (size));
}

uint8_t mp17c_host_tx_get_msdu_dscr_len(void)
{
    return MP17C_TX_MSDU_DSCR_LEN;
}

uint8_t mp17c_host_get_rx_msdu_dscr_len(void)
{
    return MP17C_RX_MSDU_DSCR_LEN;
}

void mp17c_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device)
{
    hal_host_writel(MP17C_D2H_RX_RING_RESET, hal_device->st_host_rx_normal_free_ring.rx_ring_reset);
}
