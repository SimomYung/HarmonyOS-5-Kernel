/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : dscr function
 * 作    者 : wifi
 * 创建日期 : 2020年3月10日
 */

#ifndef HOST_DSCR_MP16_H
#define HOST_DSCR_MP16_H

#include "oal_ext_if.h"
#include "frw_ext_if.h"

#include "pcie_linux.h"

#include "host_hal_dscr.h"
#include "host_hal_device.h"

#include "mac_device.h"
#include "mac_user.h"

#define MP16_RX_MSDU_DSCR_LEN     48
#define MP16_D2H_RX_RING_RESET   (MP16_RING_RESET_NORM_DATA_FREE_RING_MASK | \
                                  MP16_RING_RESET_SMALL_DATA_FREE_RING_MASK)

#ifndef _PRE_LINUX_TEST
#ifndef _PRE_PRODUCT_ID_E5
#define HAL_HOST_USER_TID_TX_RING_SIZE_LARGE   5 /* large等级的ring(缓存4096个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL  4 /* normal等级的ring(缓存2048个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_SMALL   3 /* small等级的ring(缓存1024个msdu) */
#else /* E5多用户UDP上ring buffer不足，暂时下调规格 */
#define HAL_HOST_USER_TID_TX_RING_SIZE_LARGE   3 /* large等级的ring(缓存1024个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL  2 /* normal等级的ring(缓存512个msdu) */
#define HAL_HOST_USER_TID_TX_RING_SIZE_SMALL   1 /* small等级的ring(缓存256个msdu) */
#endif
#else
#define HAL_HOST_USER_TID_TX_RING_SIZE_LARGE   1
#define HAL_HOST_USER_TID_TX_RING_SIZE_NORMAL  1
#define HAL_HOST_USER_TID_TX_RING_SIZE_SMALL   1
#endif

#define HAL_BA_INFO_COUNT 768

typedef struct {
    uint32_t msdu_addr_lsb;
    uint32_t msdu_addr_msb;
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
} mp16_tx_msdu_info_stru;

typedef struct {
    uint32_t ppdu_desc_host_addr_lsb; /* ppdu desc addr仅硬件使用 */
    uint32_t ppdu_desc_host_addr_msb;
    uint32_t pn_lsb;
    uint32_t pn_msb   : 16,
             seq_num  : 12,
             pn_vld   : 1,
             sn_vld   : 1,
             resv1    : 2;
    uint32_t tx_count : 8,
             resv2    : 24;
    uint32_t resv3;
} mp16_tx_msdu_dscr_stru;

/* 接收描述符定义 */
typedef struct tag_mp16_rx_mpdu_desc_stru {
    /* word0~1 */
    uint32_t  ppdu_host_buf_addr_lsb;
    uint32_t  ppdu_host_buf_addr_msb;

    /* word2 */
    uint32_t  bit_start_seqnum            :   12;
    uint32_t  bit_mcast_bcast             :   1;
    uint32_t  bit_eosp                    :   1;
    uint32_t  bit_is_amsdu                :   1;
    uint32_t  bit_first_sub_msdu          :   1;
    uint32_t  bit_sub_msdu_num            :   8;
    uint32_t  bit_is_ampdu                :   1;
    uint32_t  bit_ba_session_vld          :   1;
    uint32_t  bit_bar_flag                :   1;
    uint32_t  bit_reserved                :   1;
    uint32_t  bit_rx_status               :   4;

    /* word3 */
    uint32_t  bit_process_flag            :   3;
    uint32_t  bit_reserved1               :   1;
    uint32_t  bit_release_start_seqnum    :   12;
    uint32_t  bit_release_end_seqnum      :   12;
    uint32_t  bit_reserved2               :   3;
    uint32_t  bit_release_is_valid        :   1;

    /* word4 */
    uint32_t  pn_lsb;

    /* word5 */
    uint32_t  bit_pn_msb                  :   16;
    uint32_t  bit_frame_control           :   16;

    /* word6 */
    uint32_t  bit_user_id                 :   12;
    uint32_t  bit_band_id                 :   2;
    uint32_t  bit_reserved3               :   2;
    uint32_t  bit_vap_id                  :   8;
    uint32_t  bit_tid                     :   4;
    uint32_t  bit_cipher_type             :   4;

    /* word7 */
    uint32_t  bit_fragment_num            :   4;
    uint32_t  bit_sequence_num            :   12;
    uint32_t  bit_packet_len              :   16;

    /* word8 */
    uint32_t  bit_dst_user_id             :   12;
    uint32_t  bit_dst_band_id             :   2;
    uint32_t  bit_ptlcs_valid             :   1;
    uint32_t  bit_ptlcs_pass              :   1;
    uint32_t  bit_dst_vap_id              :   8;
    uint32_t  bit_frame_format            :   4;
    uint32_t  bit_ipv4cs_valid            :   1;
    uint32_t  bit_ipv4cs_pass             :   1;
    uint32_t  bit_reserved6               :   1;
    uint32_t  bit_dst_is_valid            :   1;

    /* word9 */
    uint32_t  bit_ptlcs_val               :   16;
    uint32_t  bit_ipv4cs_val              :   16;

    /* word 10~11 */
    uint32_t  next_sub_msdu_addr_lsb;
    uint32_t  next_sub_msdu_addr_msb;
} mp16_rx_mpdu_desc_stru;

#define MP16_TX_RING_INFO_LEN 16
#define MP16_TX_RING_INFO_WORD_NUM (MP16_TX_RING_INFO_LEN / sizeof(uint32_t))
typedef union {
    uint32_t words[MP16_TX_RING_INFO_WORD_NUM];
    struct {
        uint32_t tx_msdu_info_ring_base_lsb;
        uint32_t tx_msdu_info_ring_base_msb;
        uint32_t tx_msdu_info_ring_wptr     : 16;
        uint32_t tx_msdu_info_ring_depth    : 4;
        uint32_t max_aggr_amsdu_num         : 4;
        uint32_t reserve0                   : 8;
        uint32_t tx_msdu_info_ring_rptr     : 16;
        uint32_t reserve1                   : 16;
    } ring_info;
} mp16_tx_msdu_ring_table_stru;

typedef struct {
    uint32_t base_lsb;
    uint32_t base_msb;
    uint32_t size;
    uint32_t csi_pro_reg;
    uint32_t chn_set;
    uint32_t rx_ctrl_reg;
    uint32_t white_addr_msb;
    uint32_t white_addr_lsb;
    uint32_t location_info;
    uint32_t csi_info_lsb;
    uint32_t csi_info_msb;
} hal_csi_regs_info;

#pragma pack(push, 1)

struct  mp16_ftm_word0 {
    uint32_t bit_freq_bw : 8;       /* [7..0]    */
    uint32_t bit_rpt_info_len : 16; /* [23..8]  */
    uint32_t bit_frame_type : 6;    /* [29..24]  */
    uint32_t bit_dbg_mode : 1;      /* [30]  */
    uint32_t bit_reserved : 1;      /* [31]  */
    int8_t   rssi_lltf_ch[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp16_ftm_word0 mp16_ftm_word0_stru;

struct mp16_ftm_word1 {
    uint8_t  snr_ant[HAL_HOST_MAX_RF_NUM];
    int8_t   rssi_hltf[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp16_ftm_word1 mp16_ftm_word1_stru;

typedef struct {
    uint16_t agc_code_ant : 8; /* [8..0]    */
    uint16_t reserved : 8;        /* [15..9]  */
} mp16_ftm_agc_code_stru;

struct mp16_ftm_word2 {
    mp16_ftm_agc_code_stru st_agc_code_ant[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp16_ftm_word2 mp16_ftm_word2_stru;

struct mp16_ftm_word3 {
    mp16_ftm_agc_code_stru nd_agc_code_ant[HAL_HOST_MAX_RF_NUM];
} DECLARE_PACKED;
typedef struct mp16_ftm_word3 mp16_ftm_word3_stru;

struct mp16_ftm_word4 {
    uint64_t bit_phase_incr : 20;   /* [19..0]    */
    uint64_t bit_protocol_mode : 4; /* [23..20]  */
    uint64_t bit_nss_mcs_rate : 6;  /* [29..24]  */
    uint64_t bit_bf_flag : 1;       /* [30]  */
    uint64_t bit_reserved : 1;      /* [31]  */
    uint64_t bit_frm_ctrl : 16;     /* [47..32]  */
    uint64_t bit_vap_index : 8;     /* [55..48]  */
    uint64_t bit_dialog_token : 8;  /* [63..56]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word4 mp16_ftm_word4_stru;

struct mp16_ftm_word5 {
    uint64_t ra : 48;           /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word5 mp16_ftm_word5_stru;

struct mp16_ftm_word6 {
    uint64_t ta : 48;           /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word6 mp16_ftm_word6_stru;

struct mp16_ftm_word7 {
    uint64_t timestamp : 48;     /* [47..0]    */
    uint64_t bit_reserved : 16;  /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word7 mp16_ftm_word7_stru;

struct mp16_ftm_word8 {
    uint64_t tx_time : 48;      /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word8 mp16_ftm_word8_stru;

struct mp16_ftm_word9 {
    uint64_t rx_time : 48;      /* [47..0]    */
    uint64_t bit_reserved : 16; /* [63..48]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word9 mp16_ftm_word9_stru;

struct mp16_ftm_word10 {
    uint64_t bit_intp_time : 5;     /* [4..0]    */
    uint64_t bit_reserved_0 : 3;    /* [7..5]  */
    uint64_t bit_ftm_done_flag : 1; /* [8]  */
    uint64_t bit_init_resp_flag : 1; /* [9]  */
    uint64_t bit_11az_flag : 1;     /* [10]  */
    uint64_t bit_reserved_1 : 53;   /* [63..11]  */
} DECLARE_PACKED;
typedef struct mp16_ftm_word10 mp16_ftm_word10_stru;

struct mp16_ftm_dscr {
    mp16_ftm_word0_stru word0;
    mp16_ftm_word1_stru word1;
    mp16_ftm_word2_stru word2;
    mp16_ftm_word3_stru word3;
    mp16_ftm_word4_stru word4;
    mp16_ftm_word5_stru word5;
    mp16_ftm_word6_stru word6;
    mp16_ftm_word7_stru word7;
    mp16_ftm_word8_stru word8;
    mp16_ftm_word9_stru word9;
    mp16_ftm_word10_stru word10;
} DECLARE_PACKED;
typedef struct mp16_ftm_dscr mp16_ftm_dscr_stru;

struct mp16_csi_ppu {
    mp16_ftm_word0_stru word0;
    mp16_ftm_word1_stru word1;
    mp16_ftm_word2_stru word2;
    mp16_ftm_word3_stru word3;
    mp16_ftm_word4_stru word4;
    mp16_ftm_word5_stru word5;
    mp16_ftm_word6_stru word6;
    mp16_ftm_word7_stru word7;
} DECLARE_PACKED;
typedef struct mp16_csi_ppu mp16_csi_ppu_stru;

#pragma pack(pop)

static inline uint32_t mp16_host_ba_ring_depth_get(void)
{
    return HAL_BA_INFO_COUNT;
}
hal_ring_mac_regs_info* mp16_get_ring_mac_regs(void);
uint32_t mp16_rx_host_start_dscr_queue(uint8_t hal_dev_id);
int32_t mp16_rx_host_stop_dscr_queue(uint8_t hal_dev_id);
void mp16_tx_ba_info_dscr_get(uint8_t *ba_info_data, hal_tx_ba_info_stru *tx_ba_info);
void mp16_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info);
void mp16_host_ba_ring_regs_init(uint8_t hal_dev_id);
void mp16_host_intr_regs_init(uint8_t hal_dev_id);
void mp16_host_ring_tx_init(uint8_t hal_dev_id);
int32_t mp16_rx_host_init_dscr_queue(uint8_t hal_dev_id);
hal_host_device_stru *mp16_host_rx_get_mpdu_link(hal_host_device_stru *origin_dev, oal_netbuf_stru *mpdu);
uint32_t mp16_rx_mpdu_que_len(hal_host_device_stru *pst_device);
uint32_t mp16_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr);
oal_netbuf_stru *mp16_rx_get_next_sub_msdu(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
uint32_t mp16_host_rx_reset_smac_handler(hal_host_device_stru *hal_dev);
void mp16_host_rx_msdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf);
uint8_t mp16_host_get_tx_tid_ring_size(uint8_t ring_size_grade, uint8_t tid_no);
uint32_t mp16_host_get_tx_tid_ring_depth(uint8_t size);
void mp16_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info);
uint32_t mp16_host_rx_buff_recycle(hal_host_device_stru *hal_device, oal_netbuf_head_stru *netbuf_head);
uint8_t mp16_host_tx_get_msdu_dscr_len(void);
uint8_t mp16_host_get_rx_msdu_dscr_len(void);
void mp16_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device);
#endif

