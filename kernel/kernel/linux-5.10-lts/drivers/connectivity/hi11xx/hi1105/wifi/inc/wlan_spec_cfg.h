 /*
  * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
  * 功能说明 : wlan产品规格宏定义，里面划分各个模块的spec的定义
  * 作    者 :
  * 创建日期 : 2012年9月26日
  */

#ifndef WLAN_SPEC_CFG_H
#define WLAN_SPEC_CFG_H

#include "oal_types.h"
#include "oal_util.h"

// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    uint8_t feature_m2s_modem_is_open : 1;
    uint8_t feature_priv_closed_is_open : 1;
    uint8_t feature_hiex_is_open : 1;
    uint8_t rx_listen_ps_is_open : 1;
    uint8_t feature_11ax_is_open : 1;
    uint8_t feature_twt_is_open : 1;
    uint8_t feature_11ax_er_su_dcm_is_open : 1;
    uint8_t feature_11ax_uora_is_supported : 1;

    uint8_t feature_ftm_is_open : 1;
    uint8_t feature_psm_dfx_ext_is_open : 1;
    uint8_t feature_wow_opt_is_open : 1; // 06使用
    uint8_t feature_dual_wlan_is_supported : 1;
    uint8_t feature_ht_self_cure_is_open : 1; /* 05解决 ht sig_len 问题 */
    uint8_t feature_slave_ax_is_support : 1;
    uint8_t longer_than_16_he_sigb_support : 1;
    uint8_t p2p_device_gc_use_one_vap : 1;

    uint8_t feature_hw_wapi : 1;    /* 是否支持芯片加解密WAPI数据 */
    uint8_t full_bandwidth_ul_mu_mimo : 1;
    uint8_t feature_11be_is_open : 1;
    uint8_t feature_ml_is_supported : 1;
    uint8_t wur_support_5g : 1; /* wur是否支持5G */
    uint8_t wide_bw_support : 1; /* 是否支持wide bw */
    uint8_t multi_tid_aggr_tx_support : 1; /* multi tid 聚合发包 */
    uint8_t emlsr_str_link : 1;
    uint8_t tx_ba_info : 1;

    uint32_t max_sta_num;           /* STA 最大数量 */
    uint32_t max_p2p_group_num;     /* P2P GO/GC最大数量 */
    uint32_t p2p_go_max_user_num;   /* P2P GO最大关联用户数 */
    uint32_t max_asoc_user;         /* 1个chip支持的最大关联用户数 */
    uint32_t max_active_user;       /* 1个chip支持的最大激活用户数 */
    uint32_t max_user_limit;        /* board资源最大用户数，包括单播和组播用户 */
    uint32_t invalid_user_id;       /* 与tx dscr和CB字段,申请user idx三者同时对应,无效user id取全board最大用户LIMIT */
    uint32_t max_tx_ba;             /* 支持的建立tx ba 的最大个数 */
    uint32_t max_rx_ba;             /* 支持的建立rx ba 的最大个数 */
    uint32_t other_bss_id;          /* hal other bss id */
    uint32_t max_tx_ampdu_num;      /* tx聚合数上限 */

    uint8_t max_rf_num;                 /* 芯片最大天线数 */
    uint8_t max_slave_rf_num;           /* 辅路校准天线数目 */
    uint8_t max_slave_scan_rf_num;      /* 并发扫描用到的辅路天线数目 */
    uint8_t max_band_num;
    uint8_t max_5g_bw;
    uint8_t slave_scan_rf_start_idx;    /* 并发扫描辅路起始天线idx */
    uint8_t feature_txq_ns_support : 1; /* 支持网络切片队列 */
    uint8_t feature_txbf_mib_study : 1; /* 是否学习对端txbf能力 */
    uint8_t trigger_su_beamforming_feedback : 1;
    uint8_t trigger_mu_partialbw_feedback : 1;
    uint8_t vht_max_mpdu_lenth : 2; /* 11ac协议06最大支持的mpdu长度 */
    uint8_t pcie_speed : 2;
    uint8_t bfer_sounding_dimensions;   /* 发送sounding维数 */
    uint8_t bfee_ntx_supports;      /* vht 支持接收流数 */
    uint8_t rx_stbc_160;                /* he 160M接收stbc处理  */
    uint8_t num_different_channels;     /* 芯片支持的最大不同信道数 */
    uint8_t max_work_vap_num;           /* 最大work vap个数, 特指STA/AP/GC/GO/CHBA */
    uint8_t rx_checksum_hw_enable;      /* rx checksum是否硬化 */
    uint8_t vap_support_max_num_limit;  /* WIFI最大支持vap个数，配置vap个数 + 业务vap个数 */

    uint8_t hal_max_vap_num : 4;            /* 最大hal_vap个数 */
    uint8_t need_check_netbuf_pre_num : 1;  /* 是否需要检查rx netbuf为alloc list首个netbuf */
    uint8_t feature_same_chan_remain_on_chan_support : 1; /* 是否支持同信道remain_on_chan特性 */
    uint8_t hal_dev_num : 2;            /* 最大hal_dev个数 */

    uint8_t power_off_ready_support : 1; /* 业务下电保护能力 */
    uint8_t rx_ring_reduce_load_is_open : 1;
    uint8_t support_bip_gmac_128 : 1;
    uint8_t rx_support_hw_reorder : 1;
    uint8_t high_pps_fastsleep_close : 1;   /* 根据流量判断是否进入fastsleep */
    uint8_t rsv : 3;
    uint8_t vsp_timer_id;
} oal_wlan_cfg_stru;

extern const oal_wlan_cfg_stru *g_wlan_spec_cfg;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_spec.h */
