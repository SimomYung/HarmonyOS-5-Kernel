/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : mp12 host 描述符
 * 作    者 :
 * 创建日期 : 2021年2月22日
 */

#ifndef HOST_SNIFFER_MP12_H
#define HOST_SNIFFER_MP12_H

#ifdef _PRE_WLAN_FEATURE_SNIFFER

#include "oal_ext_if.h"
#include "hal_common.h"

#define HAL_RX_PPDU_FREE_RING_COUNT 32
#define MEM_RX_PPDU_DSCR_SIZE1 244  /* 注：同device侧size */
#define MEM_RX_PPDU_DSCR_SIZE (MEM_RX_PPDU_DSCR_SIZE1 + 24)

#pragma pack(push, 1)
/* Host RX PPDU 描述符相关定义 与 device 一致 */
typedef struct {
    /* word 0 */
    uint32_t  ampdu_en                 : 1;
    uint32_t  protocol_mode            : 4;
    uint32_t  freq_bw                  : 3;
    uint32_t  gi_type                  : 2;
    uint32_t  he_eht_ltf_type          : 2;
    uint32_t  preamble                 : 1;
    uint32_t  uplink_flag              : 1;
    uint32_t  sounding_mode            : 2;
    uint32_t  smoothing                : 1;
    uint32_t  stbc_mode                : 2;
    uint32_t  hesigb_ehtsig_err        : 1;
    uint32_t  extended_spatial_streams : 3;
    uint32_t  txop_ps_not_allowed      : 1;
    uint32_t  user_num                 : 7;
    uint32_t  widebw_mode                : 1;

    /* word 1 */
    uint32_t txop_duration             : 7;
    uint32_t response_flag             : 1;
    uint32_t group_id_bss_color        : 6;
    uint32_t center_ru_allocation      : 2;
    uint32_t spatial_reuse             : 4;
    uint32_t legacy_length             : 12;

    /* word 2 */
    uint32_t psdu_len                  : 23;
    uint32_t ps160                     : 1;
    uint32_t ru_allocation             : 8;

    /* word 3 */
    int8_t rpt_rssi_0ch_msb;
    int8_t rpt_rssi_1ch_msb;
    int8_t rpt_rssi_2ch_msb;
    int8_t rpt_rssi_3ch_msb;

    /* word 4 */
    int8_t rpt_snr_0ch;
    int8_t rpt_snr_1ch;
    int8_t rpt_snr_2ch;
    int8_t rpt_snr_3ch;

    /* word 5 */
    uint32_t partial_aid               : 9;
    uint32_t bf_flag                   : 1;
    uint32_t ofdma_flag                : 1;
    uint32_t mu_mimo_flag              : 1;
    uint32_t emlsr_link_mode           : 2;
    uint32_t reserved1                 : 10;
    uint32_t rpt_rssi_0ch_lsb          : 2;
    uint32_t rpt_rssi_1ch_lsb          : 2;
    uint32_t rpt_rssi_2ch_lsb          : 2;
    uint32_t rpt_rssi_3ch_lsb          : 2;

    /* word 6, 7 */
    uint32_t he_eht_sig_ru_allocation_2ch;
    uint32_t he_eht_sig_ru_allocation_1ch;

    /* word 8 */
    uint32_t punc_info                 : 16;
    uint32_t reserved2                 : 16;
} mp12_rx_ppdu_vector_info_stru;

typedef struct {
    /* word 21 */
    uint32_t rpt_evm_ss0_avg           : 8;
    uint32_t rpt_evm_ss1_avg           : 8;
    uint32_t rpt_evm_ss2_avg           : 8;
    uint32_t rpt_evm_ss3_avg           : 8;
    /* word 22 */
    uint32_t rpt_freq_offset           : 10;
    uint32_t time_of_arrival           : 6;
    uint32_t sum_mpdu_len              : 16;
    /* word 23 */
    uint32_t delimiter_pass_mpdu_num   : 12;
    uint32_t fcs_pass_mpdu_num         : 12;
    uint32_t nss_mcs_rate              : 6;
    uint32_t fec_coding                : 1;
    uint32_t dcm                       : 1;
    /* word 24 */
    uint32_t reserved3                 : 8;
    uint32_t tid_mask                  : 8;
    uint32_t user_id                   : 12;
    uint32_t htc_vld_flag              : 1;
    uint32_t reserved4                 : 1;
    uint32_t ps_flag                   : 1;
    uint32_t ampdu_en                  : 1;
    /* word 25 */
    uint32_t htc_fld;
    /* word 26 */
    uint32_t frame_control             : 16;
    uint32_t qos_control               : 16;
} mp12_rx_ppdu_user_info_stru;

#define MP12_SNIFFER_RX_PPDU_RSV_NUM 12
/* 接收ppdu描述信息 */
typedef struct {
    /* word 0-8 */
    mp12_rx_ppdu_vector_info_stru rx_ppdu_vector_info;
    /* word 9-20 */
    uint32_t reserve[MP12_SNIFFER_RX_PPDU_RSV_NUM];
    /* word 21-26 */
    mp12_rx_ppdu_user_info_stru   rx_ppdu_user_info;
} mp12_rx_ppdu_dscr_stru;

typedef struct {
    oal_dlist_head_stru entry;
    edma_paddr_t paddr;
    mp12_rx_ppdu_dscr_stru hw_ppdu_data;
} hal_host_rx_ppdu_dscr_stru;
#pragma pack(pop)

void mp12_rx_set_ring_regs(hal_host_ring_ctl_stru *rx_ring_ctl);
uint32_t mp12_sniffer_rx_ppdu_free_ring_init(hal_host_device_stru *hal_device);
void mp12_sniffer_rx_ppdu_free_ring_deinit(hal_host_device_stru *hal_device);
void mp12_sniffer_rx_info_fill(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf,
    hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *rx_ctl);
uint32_t mp12_add_rx_ppdu_dscr(hal_host_device_stru *hal_device);
uint32_t hmac_get_rate_kbps(hal_statistic_stru *rate_info, uint32_t *rate_kbps);

#endif
#endif
