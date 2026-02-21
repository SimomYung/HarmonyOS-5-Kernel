/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host中断功能实现
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */

#ifndef HOST_IRQ_MP12_H
#define HOST_IRQ_MP12_H
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"

typedef union {
    struct {
        uint32_t    rpt_host_intr_tx_complete              : 1; /* [0] */
        uint32_t    rpt_host_intr_sound_proc_done          : 1; /* [1] */
        uint32_t    reserved_0                             : 4; /* [2..5] */
        uint32_t    rpt_host_intr_edca_occupied_chan       : 1; /* [6] */
        uint32_t    reserved_1                             : 1; /* [7] */
        uint32_t    rpt_host_intr_rx_self_frame            : 1; /* [8] */
        uint32_t    rpt_host_intr_ba_win_exceed            : 1; /* [9] */
        uint32_t    reserved_2                             : 3; /* [10..12] */
        uint32_t    rpt_host_intr_common_timer             : 1; /* [13] */
        uint32_t    reserved_3                             : 2; /* [14..15] */
        uint32_t    rpt_host_intr_smac_intr_o              : 1; /* [16] */
        uint32_t    reserved_4                             : 15; /* [17..31] */
    } bits;
    uint32_t        u32;
} host_intr_status_union;

typedef union {
    struct {
        uint32_t    rpt_host_intr_location_complete              : 1; /* [0] */
        uint32_t    rpt_host_intr_location_intr_fifo_overrun     : 1; /* [1] */
        uint32_t    rpt_ml0_host_intr_ba_info                    : 1; /* [2] */
        uint32_t    rpt_ml1_host_intr_ba_info                    : 1; /* [3] */
        uint32_t    rpt_ml2_host_intr_ba_info                    : 1; /* [4] */
        uint32_t    rpt_ml0_host_intr_monitor_info               : 1; /* [5] */
        uint32_t    rpt_ml1_host_intr_monitor_info               : 1; /* [6] */
        uint32_t    rpt_ml2_host_intr_monitor_info               : 1; /* [7] */
        uint32_t    rpt_ml0_host_intr_ba_info_ring_overrun       : 1; /* [9] */
        uint32_t    rpt_ml1_host_intr_ba_info_ring_overrun       : 1; /* [9] */
        uint32_t    rpt_ml2_host_intr_ba_info_ring_overrun       : 1; /* [10] */
        uint32_t    rpt_ml0_host_intr_monitor_info_ring_overrun  : 1; /* [11] */
        uint32_t    rpt_ml1_host_intr_monitor_info_ring_overrun  : 1; /* [12] */
        uint32_t    rpt_ml2_host_intr_monitor_info_ring_overrun  : 1; /* [13] */
        uint32_t    rpt_host_intr_rx_complete                    : 1; /* [14] */
        uint32_t    rpt_host_intr_rx_ppdu_desc                   : 1; /* [15] */
        uint32_t    rpt_host_intr_norm_ring_empty                : 1; /* [16] */
        uint32_t    rpt_host_intr_small_ring_empty               : 1; /* [17] */
        uint32_t    rpt_host_intr_data_ring_overrun              : 1; /* [18] */
        uint32_t    rpt_host_intr_host_desc_ring_empty           : 1; /* [19] */
        uint32_t    reserved_0                                   : 12; /* [20..31] */
    } bits;
    uint32_t        u32;
} host_comm_intr_status_union;

typedef union {
    /* Define the struct bits */
    struct {
        /* radar中断； */
        uint32_t rpt_phy_radar_detected_int  : 1;       /* [0] */
        /* nss 大于nrx的中断 */
        uint32_t rpt_nss_gt_nrx_int          : 1;       /* [1] */
        /* psd中断； */
        uint32_t rpt_psd_int                 : 1;       /* [2] */
        /* txrdy timeout中断； */
        uint32_t rpt_txrdy_time_out_int      : 1;       /* [3] */
        /* lmi使用的mem已经没有空间中断 */
        uint32_t rpt_lmi_mem_have_no_space_int : 1;       /* [4] */
        /* rx_freq PG上电超时中断； */
        uint32_t rpt_rx_freq_pwr_ack_timeout_int : 1;       /* [5] */
        /* tx oversifs中断； */
        uint32_t rpt_p2m_tx_oversifs_int     : 1;       /* [6] */
        /* his_master 0ch异常中断； */
        uint32_t rpt_hsi_master_abnormal_0ch_int : 1;       /* [7] */
        /* his_master 1ch异常中断； */
        uint32_t rpt_hsi_master_abnormal_1ch_int : 1;       /* [8] */
        /* his_master 2ch异常中断； */
        uint32_t rpt_hsi_master_abnormal_2ch_int : 1;       /* [9] */
        /* his_master 3ch异常中断； */
        uint32_t rpt_hsi_master_abnormal_3ch_int : 1;       /* [10] */
        /* his_master rap异常中断； */
        uint32_t rpt_hsi_master_abnormal_rap_int : 1;       /* [11] */
        /* rx_dfe看门狗异常中断； */
        uint32_t rpt_wdt_rx_dfe_err_int      : 1;       /* [12] */
        /* fft看门狗异常中断； */
        uint32_t rpt_wdt_fft_err_int         : 1;       /* [13] */
        /* chn_est看门狗异常中断； */
        uint32_t rpt_wdt_chn_est_err_int     : 1;       /* [14] */
        /* mimo_det看门狗异常中断； */
        uint32_t rpt_wdt_mimo_det_err_int    : 1;       /* [15] */
        /* chn_dec看门狗异常中断； */
        uint32_t rpt_wdt_chn_dec_err_int     : 1;       /* [16] */
        /* dotb看门狗异常中断； */
        uint32_t rpt_wdt_dotb_err_int        : 1;       /* [17] */
        /* 雷达感知coherent正常处理结束中断 */
        uint32_t rpt_radar_sen_finish_int    : 1;       /* [18] */
        /* 雷达感知处理异常中断 */
        uint32_t rpt_radar_sen_abnormal_int  : 1;       /* [19] */
        /* DPD告警中断； */
        uint32_t rpt_dpd_int                 : 1;       /* [20] */
        /* IQ告警中断； */
        uint32_t rpt_iq_lmsc_int             : 1;       /* [21] */
        /* 计算出的DC值过大中断 */
        uint32_t rpt_big_dc_flag_int         : 1;       /* [22] */
        /* 采数完成中断，包含固定长度数采和循环数采 */
        uint32_t rpt_sample_done_int         : 1;       /* [23] */
        /* wifi glp 联合测距时，配置glp发送或接收pulse时，WIFI PHY正在发送中断 */
        uint32_t rpt_glp_pulse_at_tx_int     : 1;       /* [24] */
        /* LP通道dcoc异常中断 */
        uint32_t rpt_lp_dcoc_err_int         : 1;       /* [25] */
        /* 软件启动的naci统计结束中断 */
        uint32_t rpt_naci_for_det_finish_int : 1;       /* [26] */
        /* 软件启动的aci统计结束中断 */
        uint32_t rpt_aci_for_det_finish_int  : 1;       /* [27] */
        /* 软件启动的oci统计结束中断 */
        uint32_t rpt_oci_for_det_finish_int  : 1;       /* [28] */
        /* rx_time自动PG响应超时中断 */
        uint32_t rpt_rx_time_pwr_ack_timeout_int : 1;       /* [29] */
        /* tx_freq自动PG响应超时中断 */
        uint32_t rpt_tx_freq_pwr_ack_timeout_int : 1;       /* [30] */
        /* tb sync超时中断 */
        uint32_t rpt_rx_ctrl_td_tb_sync_ovt_int : 1;       /* [31] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} phy_intr_rpt_union;

typedef union {
    /* Define the struct bits */
    struct {
        /* tone中断上报； */
        uint32_t rpt_tone_est_int            : 1;       /* [0] */
        /* self_protect超时自恢复中断上报； */
        uint32_t rpt_self_prot_int           : 1;       /* [1] */
        /* 计算出LP的DC值过大中断 */
        uint32_t rpt_lp_big_dc_flag_int      : 1;       /* [2] */
        /* TX功率配置过大中断上报； */
        uint32_t rpt_txpwr_overheat_int      : 1;       /* [3] */
        /* 进入和退出EMLSR切换完成之后中断上报 */
        uint32_t rpt_emlsr_switch_finish_int : 1;       /* [4] */
        /* EMLSR切换到一半被异常打断时中断上报 */
        uint32_t rpt_emlsr_switch_abnormal_int : 1;       /* [5] */
        uint32_t reserved                    : 26;      /* [31:6] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} phy_intr_rpt2_union;

typedef struct {
    phy_intr_rpt_union intr_rpt1;
    phy_intr_rpt2_union intr_rpt2;
} hal_intr_rpt_stru;

typedef struct {
    uint32_t value1; /* 中断状态1 */
    uint32_t value2; /* 中断状态2 */
} hal_intr_state_stru;

void mp12_host_chip_irq_init(void);
void mp12_irq_host_mac_isr(hal_host_device_stru *hal_dev);
void mp12_host_mac_clear_rx_irq(hal_host_device_stru *hal_device);
void mp12_host_mac_mask_rx_irq(hal_host_device_stru *hal_device);

#endif
