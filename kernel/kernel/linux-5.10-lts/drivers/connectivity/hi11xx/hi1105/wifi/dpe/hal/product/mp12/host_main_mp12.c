/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 芯片初始化相关
 * 创建日期 : 2020年6月15日
 */

#include "host_hal_ops.h"
#include "host_dscr_mp12.h"
#include "host_irq_mp12.h"
#include "host_mac_mp12.h"
#include "host_hal_dscr.h"
#include "host_ftm_mp12.h"
#include "host_csi_mp12.h"
#include "host_sniffer_mp12.h"
#include "host_tx_mp12.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_MAIN_MP12_C

const struct hal_common_ops_stru g_hal_common_ops_mp12 = {
    .host_chip_mac_ptr_rpt_init  = mp12_host_chip_mac_ptr_rpt_init,
    .host_set_mac_ptr_rpt_regs   = mp12_host_set_mac_ptr_rpt_regs,
    .host_chip_irq_init          = mp12_host_chip_irq_init,
    .host_rx_get_mpdu_link       = hal_rx_get_mpdu_link,
    .rx_get_next_sub_msdu        = mp12_rx_get_next_sub_msdu,
    .host_get_rx_msdu_list = NULL,
    .host_rx_get_msdu_info_dscr  = mp12_host_rx_get_msdu_info_dscr,
    .tx_ba_info_dscr_get         = NULL,
    .tx_msdu_dscr_info_get       = mp12_tx_msdu_dscr_info_get,
    .host_regs_addr_init         = mp12_host_regs_addr_init,
    .get_mac_reg_offset          = mp12_get_mac_reg_offset,
    .get_ring_mac_regs           = mp12_get_ring_mac_regs,
    .host_ba_ring_regs_init      = NULL,
    .host_ba_ring_depth_get      = NULL,
    .host_ring_tx_init           = NULL,
    .host_init_common_timer      = mp12_host_init_common_timer,
    .host_set_mac_common_timer   = mp12_host_set_mac_common_timer,
    .host_set_mac_common_timer_no_pm_wakeup   = mp12_host_set_mac_common_timer_no_pm_wakeup,
    .host_get_tx_tid_ring_size   = mp12_host_get_tx_tid_ring_size,
    .host_get_tx_tid_ring_depth  = mp12_host_get_tx_tid_ring_depth,
    .host_mac_clear_rx_irq       = mp12_host_mac_clear_rx_irq,
    .host_mac_mask_rx_irq        = mp12_host_mac_mask_rx_irq,
    .host_get_host_mac_int_mask = mp12_get_host_mac_int_mask,
    .host_set_host_mac_int_mask = mp12_set_host_mac_irq_mask,
#ifdef _PRE_WLAN_FEATURE_CSI
    .get_host_ftm_csi            = NULL,
    .host_get_csi_info           = NULL,
    .host_csi_config             = NULL,
    .host_ftm_csi_init           = NULL,
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
    .host_vap_get_by_hw_vap_id   = NULL,
    .host_ftm_reg_init           = NULL,
    .host_ftm_get_info           = NULL,
    .host_ftm_get_divider        = NULL,
    .host_ftm_set_sample         = NULL,
    .host_ftm_set_enable         = NULL,
    .host_ftm_set_m2s_phy        = NULL,
    .host_ftm_set_buf_base_addr  = NULL,
    .host_ftm_set_buf_size       = NULL,
    .host_ftm_set_white_list     = NULL,
    .host_ftm_transfer_device_stru_to_host_stru = NULL,
    .host_ftm_get_rssi_selection = NULL,
#endif
#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    .host_psd_rpt_to_file        = NULL,
#endif
#ifdef _PRE_WLAN_FEATURE_SNIFFER
    .host_sniffer_rx_ppdu_free_ring_init = NULL,
    .host_sniffer_rx_ppdu_free_ring_deinit = NULL,
    .host_sniffer_rx_info_fill = NULL,
    .host_sniffer_add_rx_ppdu_dscr = NULL,
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
    .host_vsp_msdu_dscr_info_get = mp12_vsp_msdu_dscr_info_get,
#endif
    .host_get_tsf_lo = mp12_host_get_tsf_lo,
    .host_get_device_tx_ring_num = mp12_host_get_device_tx_ring_num,
    .host_rx_tcp_udp_checksum = hal_rx_tcp_udp_checksum,
    .host_rx_ip_checksum_is_pass = hal_rx_ip_checksum_is_pass,
    .host_mac_phy_irq_mask = mp12_host_mac_phy_irq_mask,
    .host_alloc_tx_ring = mp12_host_alloc_tx_ring,
    .host_release_tx_ring = mp12_host_release_tx_ring,
    .host_tx_ring_alloced = mp12_host_tx_ring_alloced,
    .host_tx_set_msdu_ring = mp12_host_tx_set_msdu_ring,
    .host_tx_get_msdu_ring = mp12_host_tx_get_msdu_ring,
    .host_tx_reset_msdu_ring = mp12_host_tx_reset_msdu_ring,
    .host_tx_get_msdu_info_devva = mp12_host_tx_get_msdu_info_devva,
    .host_tx_get_msdu_dscr_len = mp12_host_tx_get_msdu_dscr_len,
    .host_tx_set_ring_table_base_addr = mp12_host_tx_set_ring_table_base_addr,
    .host_tx_init_tid_ring_table = mp12_host_tx_init_tid_ring_table,
    .host_tx_ring_update_ops = mp12_host_tx_ring_update_ops,
    .host_tx_ring_get_ops = mp12_host_tx_ring_get_ops,
    .host_get_tx_ring_lut_idx = mp12_host_get_tx_ring_lut_idx,
    .host_get_rx_msdu_dscr_len = mp12_host_get_rx_msdu_dscr_len,
    .host_h2d_rx_ring_reset = mp12_host_h2d_rx_ring_reset,
    .host_unmask_soc_host_intr = NULL,
};
