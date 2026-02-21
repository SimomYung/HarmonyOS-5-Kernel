/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 芯片初始化相关
 * 创建日期 : 2020年6月15日
 */

#include "host_hal_ops.h"
#include "host_dscr_mp17c_mpw.h"
#include "host_irq_mp17c_mpw.h"
#include "host_mac_mp17c_mpw.h"

#include "host_ftm_mp17c_mpw.h"
#include "host_csi_mp17c_mpw.h"
#include "host_sniffer_mp17c_mpw.h"
#include "host_tx_mp17c_mpw.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_MAIN_MP17C_MPW_C

const struct hal_common_ops_stru g_hal_common_ops_mp17c_mpw = {
    .host_chip_irq_init          = mp17c_mpw_host_chip_irq_init,
    .host_rx_get_mpdu_link       = mp17c_mpw_host_rx_get_mpdu_link,
    .host_get_rx_msdu_status     = mp17c_mpw_host_get_rx_msdu_status,
    .host_rx_amsdu_list_build    = mp17c_mpw_host_rx_amsdu_list_build,
    .rx_get_next_sub_msdu        = mp17c_mpw_rx_get_next_sub_msdu,
    .host_rx_get_msdu_info_dscr  = mp17c_mpw_host_rx_get_msdu_info_dscr,
    .tx_ba_info_dscr_get         = NULL,
    .tx_msdu_dscr_info_get       = mp17c_mpw_tx_msdu_dscr_info_get,
    .host_regs_addr_init         = mp17c_mpw_host_regs_addr_init,
    .get_mac_reg_offset          = mp17c_mpw_get_mac_reg_offset,
    .get_ring_mac_regs           = mp17c_mpw_get_ring_mac_regs,
    .host_ba_ring_regs_init      = NULL,
    .host_ba_ring_depth_get      = NULL,
    .host_ring_tx_init           = NULL,
    .host_init_common_timer      = mp17c_mpw_host_init_common_timer,
    .host_set_mac_common_timer   = mp17c_mpw_host_set_mac_common_timer,
    .host_set_mac_common_timer_no_pm_wakeup   = mp17c_mpw_host_set_mac_common_timer_no_pm_wakeup,
    .host_get_tx_tid_ring_size   = mp17c_mpw_host_get_tx_tid_ring_size,
    .host_get_tx_tid_ring_depth  = mp17c_mpw_host_get_tx_tid_ring_depth,
    .host_mac_clear_rx_irq       = mp17c_mpw_host_mac_clear_rx_irq,
    .host_mac_mask_rx_irq        = mp17c_mpw_host_mac_mask_rx_irq,
    .host_get_host_mac_int_mask = mp17c_mpw_get_host_mac_int_mask,
    .host_set_host_mac_int_mask = mp17c_mpw_set_host_mac_irq_mask,
#ifdef _PRE_WLAN_FEATURE_CSI
    .get_host_ftm_csi = mp17c_mpw_get_host_ftm_csi_locationinfo,
    .host_get_csi_info           = mp17c_mpw_get_csi_info,
    .host_csi_config             = mp17c_mpw_csi_config,
    .host_ftm_csi_init           = mp17c_mpw_host_ftm_csi_init,
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
    .host_vap_get_by_hw_vap_id     = mp17c_mpw_host_vap_get_by_hw_vap_id,
    .host_ftm_reg_init           = mp17c_mpw_host_ftm_reg_init,
    .host_ftm_get_info           = mp17c_mpw_host_ftm_get_info,
    .host_ftm_get_divider        = mp17c_mpw_host_ftm_get_divider,
    .host_ftm_set_sample         = mp17c_mpw_host_ftm_set_sample,
    .host_ftm_set_enable         = mp17c_mpw_host_ftm_set_enable,
    .host_ftm_set_m2s_phy        = mp17c_mpw_host_ftm_set_m2s_phy,
    .host_ftm_set_buf_base_addr  = mp17c_mpw_host_ftm_set_buf_base_addr,
    .host_ftm_set_buf_size       = mp17c_mpw_host_ftm_set_buf_size,
    .host_ftm_set_white_list     = mp17c_mpw_host_ftm_set_white_list,
    .host_ftm_transfer_device_stru_to_host_stru = mp17c_mpw_host_ftm_transfer_device_stru_to_host_stru,
    .host_ftm_get_rssi_selection = mp17c_mpw_host_ftm_get_rssi_selection,
#endif
#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    .host_psd_rpt_to_file        = NULL,
#endif
#ifdef _PRE_WLAN_FEATURE_SNIFFER
    .host_sniffer_rx_ppdu_free_ring_init = mp17c_mpw_sniffer_rx_ppdu_free_ring_init,
    .host_sniffer_rx_ppdu_free_ring_deinit = mp17c_mpw_sniffer_rx_ppdu_free_ring_deinit,
    .host_sniffer_rx_info_fill = mp17c_mpw_sniffer_rx_info_fill,
    .host_sniffer_add_rx_ppdu_dscr = mp17c_mpw_add_rx_ppdu_dscr,
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
    .host_vsp_msdu_dscr_info_get = mp17c_mpw_vsp_msdu_dscr_info_get,
    .host_rx_buff_recycle = mp17c_mpw_host_rx_buff_recycle,
#endif
    .host_get_tsf_lo = mp17c_mpw_host_get_tsf_lo,
    .host_get_device_tx_ring_num = mp17c_mpw_host_get_device_tx_ring_num,
    .host_rx_tcp_udp_checksum = mp17c_mpw_host_rx_tcp_udp_checksum,
    .host_rx_ip_checksum_is_pass = mp17c_mpw_host_rx_ip_checksum_is_pass,
    .host_mac_phy_irq_mask = mp17c_mpw_host_mac_phy_irq_mask,
    .host_alloc_tx_ring = mp17c_mpw_host_alloc_tx_ring,
    .host_release_tx_ring = mp17c_mpw_host_release_tx_ring,
    .host_tx_ring_alloced = mp17c_mpw_host_tx_ring_alloced,
    .host_tx_set_msdu_ring = mp17c_mpw_host_tx_set_msdu_ring,
    .host_tx_get_msdu_ring = mp17c_mpw_host_tx_get_msdu_ring,
    .host_tx_reset_msdu_ring = mp17c_mpw_host_tx_reset_msdu_ring,
    .host_tx_get_msdu_info_devva = mp17c_mpw_host_tx_get_msdu_info_devva,
    .host_tx_get_msdu_dscr_len = mp17c_mpw_host_tx_get_msdu_dscr_len,
    .host_tx_set_ring_table_base_addr = mp17c_mpw_host_tx_set_ring_table_base_addr,
    .host_tx_init_tid_ring_table = mp17c_mpw_host_tx_init_tid_ring_table,
    .host_tx_ring_update_ops = mp17c_mpw_host_tx_ring_update_ops,
    .host_tx_ring_get_ops = mp17c_mpw_host_tx_ring_get_ops,
    .host_get_tx_ring_lut_idx = mp17c_mpw_host_get_tx_ring_lut_idx,
    .host_get_rx_msdu_dscr_len = mp17c_mpw_host_get_rx_msdu_dscr_len,
    .host_h2d_rx_ring_reset = mp17c_mpw_host_h2d_rx_ring_reset,
    .host_unmask_slave_interrupt = NULL,
};
