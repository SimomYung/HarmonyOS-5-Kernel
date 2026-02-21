/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 用于定义11be相关内容
 * 创建日期   : 2023年9月18日
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifndef MAC_FRAME_EHT_IE_H
#define MAC_FRAME_EHT_IE_H

/* 1 其他头文件包含 */
#include "wlan_types.h"
#include "oal_util.h"
#include "securec.h"
#include "securectype.h"
#include "mac_element.h"
#include "mac_frame_macro.h"
#include "mac_frame_80211.h"


/* 该文件定义802.11be协议下 EHT cap IE和EHT opern IE的结构体 */
/* 此文件中定义的结构体与协议相关，需要1字节对齐 */
/*****************************************************************************
  2 宏定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/* 用#pragma pack(1)/#pragma pack()方式达到一字节对齐 */
#pragma pack(1)
/* com_alg仓使用，保留bit_前缀 */
typedef struct {
    uint32_t bit_rx_max_nss_eht_mcs0_7   : 4,
             bit_tx_max_nss_eht_mcs0_7   : 4,
             bit_rx_max_nss_eht_mcs8_9   : 4,
             bit_tx_max_nss_eht_mcs8_9   : 4,
             bit_rx_max_nss_eht_mcs10_11 : 4,
             bit_tx_max_nss_eht_mcs10_11 : 4,
             bit_rx_max_nss_eht_mcs12_13 : 4,
             bit_tx_max_nss_eht_mcs12_13 : 4;
} mac_frame_eht_basic_mcs_map_20mhz_stru;

typedef struct {
    uint8_t eht_operation_information_present      : 1,
            disabled_subchannel_bitmap_present     : 1,
            eht_default_pe_duration                : 1,
            group_addressed_bu_indication_limit    : 1,
            group_addressed_bu_indication_exponent : 2,
            reserved                               : 2;
} eht_operation_parameters_stru;

typedef struct {
    uint8_t channel_width : 3,
            reserved      : 5;
    uint8_t ccfs0;
    uint8_t ccfs1;
    uint16_t disabled_subchannel_bitmap; /* 0或2字节 */
} eht_operation_information_stru;

typedef struct {
    mac_frame_ie_ext_hdr                    eht_oper_ie_hdr;
    eht_operation_parameters_stru           eht_oper_parameters;
    mac_frame_eht_basic_mcs_map_20mhz_stru  basic_eht_mcs_nss_set;
    eht_operation_information_stru          eht_oper_info;
} mac_frame_eht_operation_ie_stru;

typedef struct {
    uint16_t epcs_priority_access_sup       : 1,   /* 是否支持epcs Priority Access */
             eht_om_control_support     : 1,     /* 是否支持接收带有EHT OM Control子字段的帧 */
             txop_sharing_mode1_support : 1, /* 是否支持Triggered TXOP Sharing Mode 1     */
             txop_sharing_mode2_support : 1, /* 是否支持Triggered TXOP Sharing Mode 2     */
             restricted_twt_support     : 1,     /* 是否支持Restricted TWT     */
             scs_traffic_dscr_support   : 1,   /* 是否支持SCS Traffic Description */
             max_mpdu_length            : 2,            /* 表示AP所能接收的最大MPDU长度(0:3895, 1:7991, 2:11454) */
             /* com_alg仓使用，保留bit_前缀 */
             bit_max_ampdu_length_exponent  : 1,
             eht_trs_support            : 1,
             txop_return_support_in_txop_sharing_mode2 : 1,
             support_of_2bqrs              : 1,
             eht_link_adaption_support  : 2,
             reserved                   : 2;
} mac_frame_eht_mac_capabilities_stru;

typedef struct {
    uint32_t reserved : 1,
             /* com_alg仓使用，保留bit_前缀 */
             bit_320mhz_6ghz_support : 1, /* 是否支持工作在6GHz频段的320MHz带宽 */
             support_of_242_tone_ru_over_20mhz : 1, /* 是否支持大于20MHz带宽下的242-tone RU           */

             /* com_alg仓使用，保留bit_前缀 */
             bit_ndp_4x_eht_ltf_3p2us_gi : 1, /* 对于beamformee，是否支持接收4x EHT-LTF and 3.2 μs的EHT sounding NDP */

             partial_bandwidth_ul_mu_mimo : 1, /* 是否支持UL MU-MIMO和OFDMA组合模式 */

             su_beamformer : 1,          /* Indicates support for operation as an SU beamformer */
             su_beamformee : 1,          /* Indicates support for operation as an SU beamformee */
             beamformee_ss_below_80mhz : 3, /* 在小于等于80MHz的频段上，STA能接收的最大的空间流数 */
             beamformee_ss_equal_160mhz : 3, /* 在等于160MHz的频段上，STA能接收的最大的空间流数 */
             beamformee_ss_equal_320mhz : 3, /* 在等于320MHz的频段上，STA能接收的最大的空间流数 */
             sounding_dimensions_num_below_80mhz : 3, /* 小于等于80MHz Sounding 规模数 */
             sounding_dimensions_num_equal_160mhz : 3, /* 等于160MHz Sounding 规模数 */
             sounding_dimensions_num_equal_320mhz : 3, /* 等于320MHz Sounding 规模数 */
             /* 是否支持SU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
             ng16_su_feedback : 1,
             /* 是否支持MU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
             ng16_mu_feedback : 1,
             /* 是否支持SU反馈的EHT Compressed Beamforming Report field中的codebook size (Φ, ψ) = {4, 2} */
             codebook_42_su_feedback : 1,
             /* 是否支持SU反馈的EHT Compressed Beamforming Report field中的codebook size (Φ, ψ) = {7, 5} */
             codebook_75_mu_feedback : 1,
             /* 是否支持接收或发送部分和全部带宽的SU beamforming反馈 */
             triggered_su_beamforming_feedback : 1,
             /* 是否支持接收或发送部分的MU beamforming反馈 */
             triggered_mu_beamforming_partial_bw_feedback : 1,
             triggered_cqi_feedback : 1;  /* 是否支持部分和全部带宽的CQI反馈 */

    uint32_t partial_bandwidth_dl_mu_mimo : 1,     /* 是否支持DL MU-MIMO和OFDMA组合模式 */
             eht_psr_based_sr_support : 1,         /* 是否支持EHT PSR-based SR操作 */
             /* 是否支持接收RU的功率提升系数在[0.5, 2]范围内的EHT MU PPDU */
             power_boost_factor_support : 1,

             /* 是否支持接收使用4x EHT-LTF and 0.8us 发送的EHT MU PPDU */
             /* com_alg仓使用，保留bit_前缀 */
             bit_eht_mu_ppdu_4x_eht_ltf_08us_gi : 1,

             max_nc : 4,      /* 表示EHT compressed beamforming/CQI report支持的最大Nc值 */
             non_triggered_cqi_feedback : 1,  /* 是否支持接收/发送全带宽的non-triggered CQI反馈 */
             /* 是否支持使用1024-QAM 和4096-QAM并且小于242-tone RU发送EHT TB PPDU */
             tx_1024qam_4096qam_below_242_tone_ru_support : 1,
             /* 是否支持接收使用1024-QAM 和4096-QAM并且小于242-tone RU发送EHT TB PPDU */
             rx_1024qam_4096qam_below_242_tone_ru_support : 1,
             ppe_thresholds_present : 1,         /* PPE Thresholds是否存在 */
             /* common nominal packet padding被用于STA支持的所有星座和RU/MRU分配中 */
             common_nominal_pkt_padding : 2,
             supported_eht_ltfs_max_number : 5,   /* 支持EHT MU PPDU中最多有多少个extra EHT-LTFs */

             /* com_alg仓使用，保留bit_前缀 */
             bit_support_of_mcs15 : 4,    /* 是否支持mcs15 */
             /* com_alg仓使用，保留bit_前缀 */
             bit_eht_dup_in_6ghz : 1,     /* 是否支持6GHz下的EHT DUP(mcs 14) */

             support_of_20mhz_sta_recv_ndp_over_20mhz     : 1,   /* 工作在20MHz下，是否能接收带宽大于20MHz的NDP PPDU */
             /* 是否支持在带宽小于或等于80MHz时，使用non-OFDMA UL MU-MIMO接收或发送的EHT TB PPDU */
             non_ofdma_ul_mu_mimo_below_80mhz  : 1,
             /* 是否支持在带宽等于160MHz时，使用non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
             non_ofdma_ul_mu_mimo_equal_160mhz : 1,
             /* 是否支持在带宽等于320MHz时，使用non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
             non_ofdma_ul_mu_mimo_equal_320mhz : 1,

             mu_beamformer_below_80mhz  : 1,     /* 在带宽小于等于80MHz时，是否支持non-OFDMA DL MU-MIMO */
             mu_beamformer_equal_160mhz : 1,    /* 在带宽等于160MHz时，是否支持non-OFDMA DL MU-MIMO */
             mu_beamformer_equal_320mhz : 1,    /* 在带宽等于320MHz时，是否支持non-OFDMA DL MU-MIMO */
             tb_sounding_feadback_rate_limit : 1;    /* EHT TB PPDU是否有最大速率限制 */

    uint8_t rx_1024qam_in_wider_bandwidth_dl_ofdma_support : 1,
            rx_4096qam_in_wider_bandwidth_dl_ofdma_support : 1,
            /* 对比561x，协议新增3个能力位 */
            support_of_20mhz_only_limited_cap_support : 1,
            support_of_20mhz_triggered_mu_beamforming_full_bw_feedback_dl_mu_mimo : 1,
            support_of_20mhz_only_mru_support : 1,
            reserved2 : 3;
} mac_frame_eht_phy_capabilities_stru;

/* com_alg仓使用，保留bit_前缀 */
typedef struct {
    uint16_t bit_rx_max_nss_eht_mcs0_9   : 4,
             bit_tx_max_nss_eht_mcs0_9   : 4,
             bit_rx_max_nss_eht_mcs10_11 : 4,
             bit_tx_max_nss_eht_mcs10_11 : 4;

    uint8_t bit_rx_max_nss_eht_mcs12_13 : 4,
            bit_tx_max_nss_eht_mcs12_13 : 4;
} mac_frame_eht_basic_mcs_map_except_20mhz_stru;

/* com_alg仓使用，保留st_前缀 */
typedef struct {
    mac_frame_eht_basic_mcs_map_20mhz_stru        st_eht_mcs_map_20mhz;
    mac_frame_eht_basic_mcs_map_except_20mhz_stru st_eht_mcs_map_below_80mhz;
    mac_frame_eht_basic_mcs_map_except_20mhz_stru st_eht_mcs_map_160mhz;
    mac_frame_eht_basic_mcs_map_except_20mhz_stru st_eht_mcs_map_320mhz;
} mac_frame_eht_mcs_nss_set_stru;


/* EHT PHY Capabilities : PPE Thresholds */
typedef struct {
    uint64_t nss : 4,
             ru_index0_mask : 1, /* 242-tone RU */
             ru_index1_mask : 1, /* 484-tone RU */
             ru_index2_mask : 1, /* 484+242 tone, 996-tone RU */
             ru_index3_mask : 1, /* 996+484 tone, 996+484+242 tone, 2x996-tone RU */
             ru_index4_mask : 1, /* 2x996+484 tone, 3x996 tone, 3x996+484 tone, 4x996-tone RU */
             ppetx_value0 : 3,
             ppet8_value0 : 3,
             ppetx_value1 : 3,
             ppet8_value1 : 3,
             ppetx_value2 : 3,
             ppet8_value2 : 3,
             ppetx_value3 : 3,
             ppet8_value3 : 3,
             ppetx_value4 : 3,
             ppet8_value4 : 3,
             ppetx_value5 : 3,
             ppet8_value5 : 3,
             ppetx_value6 : 3,
             ppet8_value6 : 3,
             ppetx_value7 : 3,
             ppet8_value7 : 3,
             ppe_pad : 7;
} mac_frame_eht_ppe_thresholds_stru;

typedef struct {
    mac_frame_ie_ext_hdr                eht_cap_ie_hdr;
    mac_frame_eht_mac_capabilities_stru eht_mac_cap;
    mac_frame_eht_phy_capabilities_stru st_eht_phy_cap;    /* com_alg仓使用，保留st_前缀 */
    mac_frame_eht_mcs_nss_set_stru      st_eht_mcs_nss;    /* com_alg仓使用，保留st_前缀 */
    mac_frame_eht_ppe_thresholds_stru   eht_ppe_thresholds;
} mac_frame_eht_capabilities_ie_stru;

typedef struct {
    uint64_t trig_type : 4;        /* trigger 类型: 此处由alg填充 */
    uint64_t ppdu_len : 12;          /* EHT TB PPDU的L-SIG长度 */
    uint64_t more_tf : 1;
    uint64_t cs_required : 1;      /* 是否需要载波侦听 */
    uint64_t bw : 2;               /* 带宽大小 */
    uint64_t ltf_gi : 2;           /* GI 和 LTF 类型 */
    uint64_t reserved_b22 : 1;
    uint64_t num_of_ltfs : 3;      /* EHT-LTF符号数 */
    uint64_t reserved_b26 : 1;
    uint64_t ldpc_extra_symbol : 1;          /* LDPC额外符号分段 */
    uint64_t ap_tx_pwr : 6;                  /* AP每20MHz发送功率 */

    uint64_t pe_fec_pading : 2;
    uint64_t pe_disambiguity : 1;

    uint64_t spatial_reuse : 16;            /* 频谱复用 */
    uint64_t reserved_b53 : 1;
    uint64_t he_eht_p160 : 1;
    uint64_t spec_user_info_fld_flag : 1;      /* 指示是否存在Special User Info字段 */
    uint64_t eht_resv : 7;                   /* 固定每bit都填1 */
    uint64_t reserved_b63 : 1;               /* 保留 */
}mac_frame_eht_trig_comm_info_stru;

typedef struct {
    uint64_t aid : 12;                           /* 用户关联ID */
    uint64_t phy_version_id : 3;                 /* TB PPDU phy version, EHT填0, 其余值保留 */
    uint64_t ul_bw_extension : 2;                /* 与common info的UL BW共同指示TB PPDU带宽 */
    uint64_t eht_spatial_reuse_1 : 4;            /* EHT TB PPDU Spatial Reuse 1 */
    uint64_t eht_spatial_reuse_2 : 4;            /* EHT TB PPDU Spatial Reuse 2 */
    uint64_t disregard_u_sig_1 : 6;              /* EHT TB PPDU U-SIG Disregard and Validate */
    uint64_t validate_u_sig_2 : 1;
    uint64_t disregard_u_sig_2_lsb : 4;
    uint64_t disregard_u_sig_2_msb : 1;
    uint64_t reserved : 3;
} mac_frame_eht_trig_spec_user_info_stru;

typedef struct {
    uint64_t aid : 12;                           /* 用户关联ID */
    uint64_t ru_alloc : 8;                       /* 每个用户分配到的RU信息 */
    uint64_t coding_type : 1;             /* UL FEC 编码类型 */
    uint64_t mcs : 4;                            /* 调制编码阶数 */
    uint64_t reserved_b25 : 1;
    uint64_t ss_alloc_or_rnd_access_ru_info : 6;      /* 空间流分配 或 随机接入RU信息 */
    uint64_t target_rssi : 7;                       /* 上行的目标RSSI */
    uint64_t ps160 : 1;                           /* 指示是否使用320MHz */
} mac_frame_eht_trig_user_info_stru;
/* 用#pragma pack(1)/#pragma pack()方式达到一字节对齐 */
#pragma pack()

#endif
