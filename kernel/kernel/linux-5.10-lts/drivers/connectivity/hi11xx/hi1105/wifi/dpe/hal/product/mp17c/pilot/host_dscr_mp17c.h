/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : mp17c host 描述符
 * 作    者 :
 * 创建日期 : 2021年2月22日
 */

#ifndef HOST_DSCR_MP17C_H
#define HOST_DSCR_MP17C_H

#include "oal_ext_if.h"
#include "pcie_linux.h"
#include "hal_common.h"
#include "host_hal_device.h"

#define MP17C_TX_MSDU_DSCR_LEN    20
#define MP17C_RX_MSDU_DSCR_LEN    52
#define MP17C_D2H_RX_RING_RESET   (MP17C_RING_RESET_NORM_DATA_FREE_RING_MASK | \
                                  MP17C_RING_RESET_SMALL_DATA_FREE_RING_MASK)

#ifndef _PRE_LINUX_TEST
#define HAL_HOST_USER_TID_TX_RING_SIZE_LARGE   12 /* large等级的ring(缓存4096个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL  11 /* normal等级的ring(缓存2048个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_SMALL   10 /* small等级的ring(缓存1024个msdu) */
#else
#define HAL_HOST_USER_TID_TX_RING_SIZE_LARGE   1
#define HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL  1
#define HAL_HOST_USER_TID_TX_RING_SIZE_SMALL   1
#endif

typedef struct {
    uint32_t msdu_len      : 12;
    uint32_t data_type     : 4;
    uint32_t frag_num      : 4;
    uint32_t to_ds         : 1;
    uint32_t from_ds       : 1;
    uint32_t more_frag     : 1;
    uint32_t reserved      : 1;
    uint32_t aggr_msdu_num : 4;
    uint32_t first_msdu    : 1;
    uint32_t csum_type     : 3;
} mp17c_tx_msdu_info_stru;

typedef struct {
    uint32_t msdu_addr_lsb;
    uint32_t msdu_addr_msb;
} mp17c_tx_msdu_addr_stru;

typedef struct {
    uint32_t pn_lsb;
    uint32_t pn_msb                   : 16,
             seq_num                  : 12,
             pn_vld                   : 1,
             sn_vld                   : 1,
             rsv0                     : 2;
    uint32_t ml0_txed_count           : 8,
             ml0_msdu_timestamp_reach : 1,
             rsv1                     : 7,
             ml1_txed_count           : 8,
             ml1_msdu_timestamp_reach : 1,
             rsv2                     : 7;
    uint32_t ml2_txed_count           : 8,
             ml2_msdu_timestamp_reach : 1,
             rsv3                     : 7,
             ml3_txed_count           : 8,
             ml3_msdu_timestamp_reach : 1,
             rsv4                     : 7;
    uint32_t msdu_timestamp           : 16,
             rsv5                     : 16;
} mp17c_tx_msdu_dscr_stru;

/* 接收描述符定义 */
typedef struct {
    /* word 0-1 */
    /* 通过寄存器配置（每个complete ring一个）选择上报ppdu_host_buf_addr还是device_rx_ppdu_desc地址 */
    uint32_t  ppdu_host_buf_addr_lsb;
    uint32_t  ppdu_host_buf_addr_msb;
    /* word 2-3 */
    uint32_t  next_sub_msdu_addr_lsb;
    uint32_t  next_sub_msdu_addr_msb;
    /* word 4 */
    uint32_t  start_seqnum            : 12;
    uint32_t  mcast_bcast             : 1;
    uint32_t  eosp                    : 1;
    uint32_t  is_amsdu                : 1;
    uint32_t  first_sub_msdu          : 1;
    uint32_t  sub_msdu_num            : 8;
    uint32_t  rx_protocol_mode        : 4;
    uint32_t  rx_status               : 4;

    /* word 5 */
    uint32_t  process_flag            : 3;
    uint32_t  release_is_valid        : 1;
    uint32_t  release_start_seqnum    : 12;
    uint32_t  release_end_seqnum      : 12;
    /* RX PPDU描述符的序号，RX PPDU描述符中也有该字段，同一个PPDU的RX MSDU描述符和RX PPDU描述符里的rx_ppdu_intr_seq相等 */
    uint32_t  rx_ppdu_intr_seq        : 4;

    /* word 6 */
    uint32_t  pn_lsb;

    /* word 7 */
    uint32_t  pn_msb                  : 16;
    uint32_t  frame_control           : 16;

    /* word 8 */
    uint32_t  user_id                 : 12;
    uint32_t  band_id                 : 2;
    uint32_t  link_id                 : 2;
    uint32_t  vap_id                  : 5;
    uint32_t  is_ampdu                : 1;
    uint32_t  ba_session_vld          : 1;
    uint32_t  bar_flag                : 1;
    uint32_t  tid                     : 4;
    uint32_t  cipher_type             : 4;

    /* word 9 */
    uint32_t  fragment_num            : 4;
    uint32_t  sequence_num            : 12;
    uint32_t  packet_len              : 16;

    /* word 10 */
    uint32_t  dst_user_id             : 12;
    uint32_t  dst_band_id             : 2;
    uint32_t  ptlcs_valid             : 1;
    uint32_t  ptlcs_pass              : 1;
    uint32_t  dst_vap_id              : 5;
    uint32_t  rx_data_type            : 5;
    uint32_t  frame_format            : 2; /* 0：以太网格式的eth2,1：以太网格式的802.3,3：802.11格式 */
    uint32_t  ipv4cs_valid            : 1;
    uint32_t  ipv4cs_pass             : 1;
    uint32_t  rsv4                    : 1;
    uint32_t  dst_is_valid            : 1;

    /* word 11 */
    int8_t  rpt_rssi_0ch_msb;
    int8_t  rpt_rssi_1ch_msb;
    int8_t  rpt_rssi_2ch_msb;
    int8_t  rpt_rssi_3ch_msb;

    /* word 12 */
    int8_t  rpt_snr_0ch;
    int8_t  rpt_snr_1ch;
    int8_t  rpt_snr_2ch;
    int8_t  rpt_snr_3ch;
} mp17c_rx_msdu_dscr_stru;
#define MP17C_TX_RING_INFO_LEN 40
#define MP17C_TX_RING_INFO_WORD_NUM (MP17C_TX_RING_INFO_LEN / sizeof(uint32_t))
typedef union {
    uint32_t words[MP17C_TX_RING_INFO_WORD_NUM];
    struct {
        uint32_t tx_msdu_info_ring_base_lsb;
        uint32_t tx_msdu_info_ring_base_msb;
        uint32_t tx_msdu_addr_ring_base_lsb;
        uint32_t tx_msdu_addr_ring_base_msb;
        uint32_t tx_msdu_info_ring_wptr     : 16;
        uint32_t tx_msdu_info_ring_depth    : 4;
        uint32_t max_aggr_amsdu_num         : 4;
        uint32_t ml_mode                    : 1;
        uint32_t ml0_dup_en                 : 1;
        uint32_t ml1_dup_en                 : 1;
        uint32_t ml2_dup_en                 : 1;
        uint32_t multi_link_bitmap          : 4;
        uint32_t tx_msdu_map_lut_base_lsb;
        uint32_t tx_msdu_map_lut_base_msb;
        uint32_t ml0_tx_msdu_info_ring_rptr : 16;
        uint32_t ml0_rptr_seq_num           : 12;
        uint32_t reserved0                  : 4;
        uint32_t ml1_tx_msdu_info_ring_rptr : 16;
        uint32_t ml1_rptr_seq_num           : 12;
        uint32_t reserved1                  : 4;
        uint32_t ml2_tx_msdu_info_ring_rptr : 16;
        uint32_t ml2_rptr_seq_num           : 12;
        uint32_t reserved2                  : 4;
    } ring_info;
} mp17c_tx_msdu_ring_table_stru;

#pragma pack(push, 1)

struct  mp17c_ftm_word0 {
    uint32_t bit_freq_bw : 8;       /* [7..0]    */
    uint32_t bit_rpt_info_len : 16; /* [23..8]  */
    uint32_t bit_frame_type : 6;    /* [29..24]  */
    uint32_t bit_dbg_mode : 1;      /* [30]  */
    uint32_t bit_reserved : 1;      /* [31]  */
    int8_t   rssi_lltf_ch[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct  mp17c_ftm_word0 mp17c_ftm_word0_stru;

struct mp17c_ftm_word1 {
    uint8_t  snr_ant[HAL_HOST_MAX_RF_NUM];
    int8_t   rssi_hltf[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp17c_ftm_word1 mp17c_ftm_word1_stru;

typedef struct {
    uint16_t agc_code_ant : 8; /* [8..0]    */
    uint16_t reserved : 8;        /* [15..9]  */
} mp17c_ftm_agc_code_stru;

struct mp17c_ftm_word2 {
    mp17c_ftm_agc_code_stru st_agc_code_ant[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp17c_ftm_word2 mp17c_ftm_word2_stru;

struct mp17c_ftm_word3 {
    mp17c_ftm_agc_code_stru nd_agc_code_ant[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp17c_ftm_word3 mp17c_ftm_word3_stru;

struct mp17c_ftm_word4 {
    uint64_t bit_phase_incr : 20;   /* [19..0]    */
    uint64_t bit_protocol_mode : 4; /* [23..20]  */
    uint64_t bit_nss_mcs_rate : 6;  /* [29..24]  */
    uint64_t bit_bf_flag : 1;       /* [30]  */
    uint64_t bit_reserved : 1;      /* [31]  */
    uint64_t bit_frm_ctrl : 16;     /* [47..32]  */
    uint64_t bit_vap_index : 8;     /* [55..48]  */
    uint64_t bit_dialog_token : 8;  /* [63..56]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word4 mp17c_ftm_word4_stru;

struct mp17c_ftm_word5 {
    uint64_t ra : 48;           /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word5 mp17c_ftm_word5_stru;

struct mp17c_ftm_word6 {
    uint64_t ta : 48;           /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word6 mp17c_ftm_word6_stru;

struct mp17c_ftm_word7 {
    uint64_t timestamp : 48;     /* [47..0]    */
    uint64_t bit_reserved : 16;  /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word7 mp17c_ftm_word7_stru;

struct mp17c_ftm_word8 {
    uint64_t tx_time : 48;      /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word8 mp17c_ftm_word8_stru;

struct mp17c_ftm_word9 {
    uint64_t rx_time : 48;      /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word9 mp17c_ftm_word9_stru;

struct mp17c_ftm_word10 {
    uint64_t bit_intp_time : 5;     /* [4..0]    */
    uint64_t bit_reserved_0 : 3;    /* [7..5]  */
    uint64_t bit_ftm_done_flag : 1; /* [8]  */
    uint64_t bit_init_resp_flag : 1; /* [9]  */
    uint64_t bit_11az_flag : 1;     /* [10]  */
    uint64_t bit_reserved_1 : 53;   /* [63..11]  */
} DECLARE_PACKED;
typedef struct mp17c_ftm_word10 mp17c_ftm_word10_stru;

struct mp17c_ftm_dscr {
    mp17c_ftm_word0_stru word0;
    mp17c_ftm_word1_stru word1;
    mp17c_ftm_word2_stru word2;
    mp17c_ftm_word3_stru word3;
    mp17c_ftm_word4_stru word4;
    mp17c_ftm_word5_stru word5;
    mp17c_ftm_word6_stru word6;
    mp17c_ftm_word7_stru word7;
    mp17c_ftm_word8_stru word8;
    mp17c_ftm_word9_stru word9;
    mp17c_ftm_word10_stru word10;
} DECLARE_PACKED;
typedef struct mp17c_ftm_dscr mp17c_ftm_dscr_stru;

struct mp17c_csi_ppu {
    mp17c_ftm_word0_stru word0;
    mp17c_ftm_word1_stru word1;
    mp17c_ftm_word2_stru word2;
    mp17c_ftm_word3_stru word3;
    mp17c_ftm_word4_stru word4;
    mp17c_ftm_word5_stru word5;
    mp17c_ftm_word6_stru word6;
    mp17c_ftm_word7_stru word7;
} DECLARE_PACKED;
typedef struct mp17c_csi_ppu mp17c_csi_ppu_stru;

#pragma pack(pop)
hal_ring_mac_regs_info* mp17c_get_ring_mac_regs(void);
uint32_t mp17c_rx_host_start_dscr_queue(uint8_t hal_dev_id);
int32_t mp17c_rx_host_stop_dscr_queue(uint8_t hal_dev_id);
void mp17c_tx_ba_info_dscr_get(uint8_t *ba_info_data, hal_tx_ba_info_stru *tx_ba_info);
void mp17c_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info);
void mp17c_host_ba_ring_regs_init(uint8_t hal_dev_id);
void mp17c_host_ring_tx_init(uint8_t hal_dev_id);
int32_t mp17c_rx_host_init_dscr_queue(uint8_t hal_dev_id);
hal_host_device_stru *mp17c_host_rx_get_mpdu_link(hal_host_device_stru *origin_dev, oal_netbuf_stru *mpdu);
uint32_t mp17c_rx_mpdu_que_len(hal_host_device_stru *pst_device);
uint8_t mp17c_host_get_tx_tid_ring_size(uint8_t ring_size_grade, uint8_t tid_no);
uint32_t mp17c_host_get_tx_tid_ring_depth(uint8_t size);
uint32_t mp17c_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr);
oal_netbuf_stru *mp17c_rx_get_next_sub_msdu(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
uint32_t mp17c_host_rx_reset_smac_handler(hal_host_device_stru *hal_dev);
uint32_t mp17c_host_rx_proc_msdu_dscr(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf);
uint32_t mp17c_host_rx_get_msdu_info_dscr(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, uint8_t first_msdu);
void mp17c_rx_free_res(hal_host_device_stru *hal_device);
void mp17c_rx_alloc_list_free(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list);
uint8_t mp17c_rx_ring_reg_init(hal_host_device_stru *hal_dev, hal_host_ring_ctl_stru *rctl, uint8_t ring_id);
#ifdef _PRE_WLAN_FEATURE_VSP
void mp17c_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info);
#endif
uint8_t mp17c_host_tx_get_msdu_dscr_len(void);
uint8_t mp17c_host_get_rx_msdu_dscr_len(void);
void mp17c_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device);
#endif /* end of host_dscr_mp17c.h */

