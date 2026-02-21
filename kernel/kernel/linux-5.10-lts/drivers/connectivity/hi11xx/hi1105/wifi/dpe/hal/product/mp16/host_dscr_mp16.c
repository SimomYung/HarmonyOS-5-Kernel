/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL DSCR FUNCTION
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */
#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/err.h>
#endif

#include "oal_util.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "oam_event_wifi.h"
#include "oal_net.h"
#include "host_hal_dscr.h"
#include "host_hal_ring.h"
#include "host_hal_device.h"
#include "host_dscr_mp16.h"
#include "host_mac_mp16.h"
#include "host_irq_mp16.h"
#include "hmac_tx_complete.h"
#include "hmac_tid_sche.h"
#include "hmac_tx_msdu_dscr.h"
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_source.h"
#include "hmac_vsp_if.h"
#endif
#include "hmac_tx_ring_alloc.h"
#include "pcie_host.h"
#include "mac_common.h"
#include "oal_net.h"
#include "host_hal_ops_mp16.h"
#include "host_hal_ext_if.h"
#if(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_DSCR_MP16_C

#define MP16_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_MASK              0x8
#define MP16_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK             0x10

static hal_ring_mac_regs_info g_ring_reg_tbl[HAL_RING_ID_BUTT] = {
    [HAL_RING_TYPE_L_F] = {
        .base_lsb = MP16_MAC_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = MP16_MAC_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = MP16_MAC_RX_NORM_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = MP16_MAC_RX_NORM_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = MP16_MAC_RX_NORM_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = MP16_MAC_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG,
        .rx_ring_reset = MP16_MAC_RX_RING_RESET_CTRL_REG,
    },
    [HAL_RING_TYPE_S_F] = {
        .base_lsb = MP16_MAC_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = MP16_MAC_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = MP16_MAC_RX_SMALL_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = MP16_MAC_RX_SMALL_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = MP16_MAC_RX_SMALL_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = MP16_MAC_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG,
        .rx_ring_reset = MP16_MAC_RX_RING_RESET_CTRL_REG,
    },
    [HAL_RING_TYPE_COMP] = {
        .base_lsb = MP16_MAC_RX_DATA_CMP_RING_ADDR_LSB_REG,
        .base_msb = MP16_MAC_RX_DATA_CMP_RING_ADDR_MSB_REG,
        .len      = MP16_MAC_RX_DATA_BUFF_LEN_REG,
        .size     = MP16_MAC_RX_DATA_CMP_RING_SIZE_REG,
        .wptr_reg = MP16_MAC_RX_DATA_CMP_RING_WPTR_REG,
        .rptr_reg = MP16_MAC_RX_DATA_CMP_RING_RPTR_REG,
        .cfg_wptr_reg = MP16_MAC_RX_DATA_CMP_RING_WPTR_CFG_REG,
        .rx_ring_reset = MP16_MAC_RX_RING_RESET_CTRL_REG,
    },
    [TX_BA_INFO_RING] = {
        .base_lsb = MP16_MAC_TX_BA_INFO_BUF_ADDR_LSB_REG,
        .base_msb = MP16_MAC_TX_BA_INFO_BUF_ADDR_MSB_REG,
        .len      = 0,
        .size     = MP16_MAC_TX_BA_INFO_BUF_DEPTH_REG,
        .wptr_reg = MP16_MAC_BA_INFO_BUF_WPTR_REG,
        .rptr_reg = MP16_MAC_TX_BA_INFO_RPTR_REG,
        .cfg_wptr_reg = MP16_MAC_TX_BA_INFO_WPTR_REG,
        .rx_ring_reset = MP16_MAC_RX_RING_RESET_CTRL_REG,
    }
};

uint32_t g_mac_reg_offset_mp16[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP16] = {0, MP16_MAC_BANK_REG_OFFSET};
uint32_t mp16_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr)
{
    return g_mac_reg_offset_mp16[device_id];
}

hal_ring_mac_regs_info* mp16_get_ring_mac_regs(void)
{
    return g_ring_reg_tbl;
}

oal_netbuf_stru *mp16_rx_get_next_sub_msdu(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    mp16_rx_mpdu_desc_stru    *rx_hw_dscr = NULL;
    mac_rx_ctl_stru             *rx_cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    uint64_t                     next_dscr_addr;

    /* 统一清空rx cb字段 */
    memset_s(rx_cb, sizeof(mac_rx_ctl_stru), 0, sizeof(mac_rx_ctl_stru));

    hal_device->rx_statics.comp_ring_get++;
    rx_hw_dscr = (mp16_rx_mpdu_desc_stru *)oal_netbuf_data(netbuf);
    if ((!rx_hw_dscr->next_sub_msdu_addr_lsb) && (!rx_hw_dscr->next_sub_msdu_addr_msb)) {
        return NULL;
    }

    next_dscr_addr = oal_make_word64(rx_hw_dscr->next_sub_msdu_addr_lsb, rx_hw_dscr->next_sub_msdu_addr_msb);
    return hal_rx_get_sub_msdu(hal_device, next_dscr_addr);
}

static void mp16_host_rx_cb_set(mac_rx_ctl_stru *rx_ctl, mp16_rx_mpdu_desc_stru *rx_hw_dscr, uint8_t first_msdu)
{
    mac_header_frame_control_stru *fc_ctl_hdr = NULL;
    /* mpdu级别 */
    if (first_msdu) {
        rx_ctl->bit_mcast_bcast     = rx_hw_dscr->bit_mcast_bcast;
        rx_ctl->bit_amsdu_enable    = rx_hw_dscr->bit_is_amsdu;
        rx_ctl->bit_buff_nums       = 1;
        rx_ctl->us_frame_control    = rx_hw_dscr->bit_frame_control;
        rx_ctl->bit_start_seqnum    = rx_hw_dscr->bit_start_seqnum;
        rx_ctl->bit_cipher_type     = rx_hw_dscr->bit_cipher_type;
        rx_ctl->bit_release_start_sn = rx_hw_dscr->bit_release_start_seqnum;
        rx_ctl->bit_release_end_sn  = rx_hw_dscr->bit_release_end_seqnum;
        rx_ctl->bit_fragment_num    = rx_hw_dscr->bit_fragment_num;
        rx_ctl->bit_process_flag    = rx_hw_dscr->bit_process_flag;
        rx_ctl->bit_rx_user_id      = rx_hw_dscr->bit_user_id;
        rx_ctl->us_seq_num          = rx_hw_dscr->bit_sequence_num;
        rx_ctl->bit_is_first_buffer = rx_hw_dscr->bit_first_sub_msdu;
        rx_ctl->bit_rx_tid          = rx_hw_dscr->bit_tid;
        rx_ctl->rx_status           = rx_hw_dscr->bit_rx_status;
        rx_ctl->bit_is_ampdu        = rx_hw_dscr->bit_is_ampdu;
        rx_ctl->uc_msdu_in_buffer   = rx_hw_dscr->bit_sub_msdu_num;
        rx_ctl->rx_lsb_pn           = rx_hw_dscr->pn_lsb;
        rx_ctl->us_rx_msb_pn        = rx_hw_dscr->bit_pn_msb;
        rx_ctl->bit_vap_id          = rx_hw_dscr->bit_vap_id;
        rx_ctl->bit_release_valid   = rx_hw_dscr->bit_release_is_valid;
        rx_ctl->bit_band_id         = rx_hw_dscr->bit_band_id;
        rx_ctl->bit_ta_user_idx     = rx_hw_dscr->bit_user_id & 0x1f; /* 1f valid bits */
        rx_ctl->last_msdu_in_amsdu  = rx_hw_dscr->bit_sub_msdu_num == 0;
        if ((rx_hw_dscr->bit_vap_id != MP16_OTHER_BSS_ID)) {
            rx_ctl->bit_vap_id = mp16_hw_vap_id_to_hal_vap_id(rx_ctl->bit_vap_id);
        }

        if (hal_rx_is_frag_frm(rx_ctl)) {
            rx_ctl->bit_is_fragmented = OAL_TRUE;
        }
        /* 私有传输 */
        fc_ctl_hdr = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;
        rx_ctl->rx_priv_trans = fc_ctl_hdr->bit_more_data;
    }

    /* msdu级别 */
    rx_ctl->bit_frame_format    = rx_hw_dscr->bit_frame_format;
    rx_ctl->dst_hal_vap_id      = rx_hw_dscr->bit_dst_vap_id;
    rx_ctl->us_frame_len        = rx_hw_dscr->bit_packet_len;
    rx_ctl->bit_dst_band_id     = rx_hw_dscr->bit_dst_band_id;
}

/*
 * 功能描述  : cb中长度计算，并调整netbuf数据部分头尾指针
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t mp16_host_rx_adjust_netbuf_len(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl,
    mp16_rx_mpdu_desc_stru *rx_hw_dscr)
{
    pcie_cb_dma_res               *cb_res = NULL;

    cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);

    rx_ctl->uc_mac_header_len = hal_host_rx_get_mac_header_len(rx_ctl);

    /* 1.fcs错误，后续字段信息不需要查看，本身不可信  2.fcs错误的amsdu非首帧信息本身不回填，字段不可信 */
    if ((rx_hw_dscr->bit_rx_status == HAL_RX_SUCCESS) &&
        (rx_hw_dscr->bit_packet_len == 0 || (rx_hw_dscr->bit_packet_len > (cb_res->len - MP16_RX_MSDU_DSCR_LEN)))) {
        oam_error_log3(0, OAM_SF_RX, "host_rx_adjust_netbuf_len:dscr addr:0x%x, pkt len:%d, pcie cb len:%d",
            (uintptr_t)rx_hw_dscr, rx_hw_dscr->bit_packet_len, cb_res->len);
        oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)rx_hw_dscr, MP16_RX_MSDU_DSCR_LEN, OAM_OTA_TYPE_RX_MSDU_DSCR);
        return OAL_FAIL;
    }

    oal_netbuf_pull(netbuf, MP16_RX_MSDU_DSCR_LEN);
    skb_trim(netbuf, rx_hw_dscr->bit_packet_len);
    return OAL_SUCC;
}

/*
 * 函 数 名   : hal_rx_msdu_info_wait_valid
 * 功能描述   : wait rx complete ring addr valid
 * 1.日    期   : 2021年3月19日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
uint32_t mp16_rx_msdu_info_wait_valid(mp16_rx_mpdu_desc_stru *rx_msdu_hw_dscr, uint8_t first_msdu)
{
#define RX_MUSD_INFO_WAIT_CNT 1000
    uint16_t   wait_cnt = 0;
    /* check rx msdu if info valid */
    while (rx_msdu_hw_dscr->bit_rx_status == HAL_RX_NEW && first_msdu == OAL_TRUE) {
        wait_cnt++;
        if (wait_cnt > RX_MUSD_INFO_WAIT_CNT) {
            oam_warning_log2(0, OAM_SF_RX, "rx_msdu_info_wait_valid:waitcnt[%d]>waitmax[%d].",
                wait_cnt, RX_MUSD_INFO_WAIT_CNT);
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

uint32_t mp16_host_rx_get_msdu_info_dscr(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, uint8_t first_msdu)
{
    mp16_rx_mpdu_desc_stru      *rx_msdu_hw_dscr = NULL;
    uint32_t                       ret;

    rx_msdu_hw_dscr = (mp16_rx_mpdu_desc_stru *)oal_netbuf_data(netbuf);

    /* 需要等rx msdu信息回填完成 */
    ret = mp16_rx_msdu_info_wait_valid(rx_msdu_hw_dscr, first_msdu);
    if (ret != OAL_SUCC) {
        return ret;
    }
    mp16_host_rx_cb_set(rx_ctl, rx_msdu_hw_dscr, first_msdu);
    ret = mp16_host_rx_adjust_netbuf_len(netbuf, rx_ctl, rx_msdu_hw_dscr);
    if (ret != OAL_SUCC) {
        return ret;
    }
    if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        rx_ctl->val_mac_hdr_start_addr = (uint64_t)(uintptr_t)oal_netbuf_data(netbuf);
    }
    return OAL_SUCC;
}

static void mp16_init_host_ba_ring_ctl(hal_host_device_stru *hal_device)
{
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    oal_uint *rbase_vaddr = NULL;
    uint64_t devva = 0;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    if (pcie_dev == NULL) {
        oam_error_log0(0, 0, "init_host_ba_ring_ctl::pcie_dev null!");
        return;
    }

    rx_ring_ctl = &hal_device->host_ba_info_ring;
    memset_s(rx_ring_ctl, sizeof(hal_host_ring_ctl_stru), 0, sizeof(hal_host_ring_ctl_stru));

    alloc_size = (HAL_BA_INFO_COUNT * BA_INFO_DATA_SIZE);
    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, 0, "init_host_ba_ring_ctl::rbase_vaddr null!");
        return;
    }

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)rbase_dma_addr, &devva) != OAL_SUCC) {
        oam_error_log0(0, 0, "init_host_ba_ring_ctl::hostca_to_devva fail!");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        return;
    }

    rx_ring_ctl->entries = HAL_BA_INFO_COUNT;
    rx_ring_ctl->p_entries = (uintptr_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = BA_INFO_DATA_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_COMPLETE_RING;
    rx_ring_ctl->ring_subtype = TX_BA_INFO_RING;
    rx_ring_ctl->devva = (uintptr_t)devva;

    if (hal_rx_ring_reg_init(hal_device, rx_ring_ctl, TX_BA_INFO_RING) == OAL_FALSE) {
        oam_error_log0(0, 0, "init_host_ba_ring_ctl ring init fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        rx_ring_ctl->p_entries = NULL;
        return;
    }

    oam_warning_log3(0, 0, "{init_host_ba_ring_ctl::dev id[%d] devva lsb[0x%x], msb[0x%x]}",
        hal_device->device_id, get_low_32bits(rx_ring_ctl->devva), get_high_32bits(rx_ring_ctl->devva));
}

static inline void mp16_host_reset_ba_info_ring_rwptr(hal_host_ring_ctl_stru *ba_ring_ctl)
{
    hal_host_writel(0, ba_ring_ctl->read_ptr_reg);
    hal_host_writel(0, ba_ring_ctl->cfg_write_ptr_reg);
    ba_ring_ctl->un_read_ptr.read_ptr = 0;
    ba_ring_ctl->un_write_ptr.write_ptr = 0;
}

void mp16_host_ba_ring_regs_init(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);
    if (hal_device == NULL) {
        oam_error_log1(0, 0, "{host_ba_ring_regs_init::hal device[%d] null!}", hal_dev_id);
        return;
    }

    if (hal_device->host_ba_ring_regs_inited) {
        return;
    }

    mp16_init_host_ba_ring_ctl(hal_device);
    hal_device->host_ba_ring_regs_inited = OAL_TRUE;
}

void mp16_host_ring_tx_init(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);

    if (hal_device == NULL) {
        oam_error_log1(0, 0, "{host_ring_tx_init::hal device[%d] null!}", hal_dev_id);
        return;
    }

    if (!hal_device->host_ba_ring_regs_inited) {
        oam_error_log1(0, 0, "{host_ring_tx_init::hal device[%d] host ba ring regs not inited!}", hal_dev_id);
        return;
    }

    if (oal_atomic_read(&hal_device->ba_ring_type) == HOST_BA_INFO_RING) {
        return;
    }
    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_HOST_TX_RING_INIT);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_HOST_TX_RING_INIT);
        return;
    }

    mp16_host_reset_ba_info_ring_rwptr(&hal_device->host_ba_info_ring);
    hal_rx_set_ring_regs(&hal_device->host_ba_info_ring);
    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_HOST_TX_RING_INIT);
    oal_atomic_set(&hal_device->ba_ring_type, HOST_BA_INFO_RING);
    oam_warning_log1(0, 0, "{host_ring_tx_init::hal device[%d] host ba ring inited}", hal_dev_id);
}

void mp16_tx_ba_info_dscr_get(uint8_t *ba_info_data, hal_tx_ba_info_stru *tx_ba_info)
{
    hal_tx_ba_info_dscr_stru *tx_ba_info_dscr = (hal_tx_ba_info_dscr_stru *)ba_info_data;

    tx_ba_info->user_id = tx_ba_info_dscr->bit_user_id;
    tx_ba_info->tid_no = tx_ba_info_dscr->bit_tid;
    tx_ba_info->ba_info_vld = tx_ba_info_dscr->bit_ba_info_vld;
    tx_ba_info->ba_ssn = tx_ba_info_dscr->bit_tx_ba_ssn;
}

#define MP16_TX_MSDU_DSCR_LEN 24
void mp16_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp16_tx_msdu_dscr_stru *tx_msdu_dscr = (mp16_tx_msdu_dscr_stru *)((uintptr_t)oal_netbuf_data(netbuf) -
                                           MP16_TX_MSDU_DSCR_LEN);

    tx_msdu_info->seq_num = tx_msdu_dscr->seq_num;
    tx_msdu_info->sn_vld = tx_msdu_dscr->sn_vld;
    tx_msdu_info->tx_count = tx_msdu_dscr->tx_count;
}

uint8_t mp16_host_get_tx_tid_ring_size(uint8_t ring_size_grade, uint8_t tid_no)
{
    /* 根据tid流量大小设置对应的tx_ring内存大小 */
    /* 申请失败方案:降低ring_size_grade继续申请 */
    /*
    |   tid_no  |   ring_depth   |               mp16_ring_size             |         备注          |
    |   0/4/5/7 |      4096      |   HAL_HOST_USER_TID_TX_RING_SIZE_LARGE  5   |  流量较大,常发走tid7  |
    |      6    |      2048      |   HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL 4   |    语音VO,流量中等    |
    |    1/2/3  |      1024      |   HAL_HOST_USER_TID_TX_RING_SIZE_SMALL  3   |       流量较小        |
    注:1.mp16最小ring_depth为128,mp16_ring_depth = 128 << mp16_ring_size;
       2.mp16c最小ring_depth为1,mp16c ring_depth = 1 << mp16c ring_size;
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

uint32_t mp16_host_get_tx_tid_ring_depth(uint8_t size)
{
    return ((128) << (size)); // mp16 ring的大小 = 128*(2^size)
}

void mp16_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    mp16_tx_msdu_dscr_stru *tx_msdu_dscr = (mp16_tx_msdu_dscr_stru *)buffer;

    tx_msdu_info->seq_num = tx_msdu_dscr->seq_num;
    tx_msdu_info->sn_vld = tx_msdu_dscr->sn_vld;
    tx_msdu_info->tx_count = tx_msdu_dscr->tx_count;
}

uint8_t mp16_host_tx_get_msdu_dscr_len(void)
{
    return MP16_TX_MSDU_DSCR_LEN;
}

uint8_t mp16_host_get_rx_msdu_dscr_len(void)
{
    return MP16_RX_MSDU_DSCR_LEN;
}

void mp16_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device)
{
    hal_host_writel(MP16_D2H_RX_RING_RESET, hal_device->st_host_rx_normal_free_ring.rx_ring_reset);
}
