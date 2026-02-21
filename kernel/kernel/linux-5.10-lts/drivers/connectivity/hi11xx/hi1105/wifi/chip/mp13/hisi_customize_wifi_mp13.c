/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : wifi定制化函数实现
 * 作者       : wifi
 * 创建日期   : 2015年10月08日
 */

/* 头文件包含 */
#include "hisi_customize_wifi_mp13.h"
#include "hisi_customize_config_dts_mp13.h"
#include "hisi_customize_config_priv_mp13.h"
#include "hisi_customize_config_cmd_mp13.h"
#include "hisi_customize_nvram_config_mp13.h"
#include "hisi_customize_wifi.h"

#include "mac_hiex.h"

#include "wal_config.h"
#include "wal_linux_ioctl.h"

#include "hmac_auto_adjust_freq.h"
#include "hmac_tx_data.h"
#include "hmac_cali_mgmt.h"
#include "hmac_tcp_ack_buf.h"
#include "hmac_tx_amsdu.h"
#include "hmac_rx.h"
#include "hmac_rx_data.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_CUSTOMIZE_WIFI_HI1103_C

#ifdef _PRE_DUAL_CHIP

OAL_STATIC bool g_cust_first_init_flg = OAL_TRUE; /* 是否首次配置载入定制化标志 */

OAL_STATIC wlan_customize_pwr_fit_para_stru g_pro_line_params_dev1[WLAN_RF_CHANNEL_NUMS][DY_CALI_PARAMS_NUM] = {{{0}}};
OAL_STATIC uint8_t g_cust_nvram_info_dev1[WLAN_CFG_DTS_NVRAM_END][CUS_PARAMS_LEN_MAX] = {{0}}; /* NVRAM数组 */

OAL_STATIC oal_bool_enum_uint8 g_en_fact_cali_completed_dev1 = OAL_FALSE;
OAL_STATIC int16_t g_gs_extre_point_vals[WLAN_RF_CHANNEL_NUMS][DY_CALI_NUM_5G_BAND] = {{0}};

OAL_STATIC wlan_customize_private_stru g_priv_cust_params_dev1[WLAN_CFG_PRIV_BUTT] = {{ 0, 0 }}; /* 私有定制化参数数组 */

OAL_STATIC int32_t g_dts_params_dev1[WLAN_CFG_DTS_BUTT] = {0};        /* dts定制化参数数组 */

OAL_STATIC int32_t g_host_init_params_dev1[WLAN_CFG_INIT_BUTT] = {0}; /* dev2的ini定制化参数数组 */

OAL_STATIC const int32_t g_host_init_params_default_value[WLAN_CFG_INIT_BUTT] = {    /* ini定制化参数数组默认值 */
    /* ROAM */
    WLAN_ROAM_SWITCH_MODE,          //  WLAN_CFG_INIT_ROAM_SWITCH = 0,
    WLAN_SCAN_ORTHOGONAL_VAL,       //  WLAN_CFG_INIT_SCAN_ORTHOGONAL,
    WLAN_TRIGGER_B_VAL,             //  WLAN_CFG_INIT_TRIGGER_B,
    WLAN_TRIGGER_A_VAL,             //  WLAN_CFG_INIT_TRIGGER_A,
    WLAN_DELTA_B_VAL,               //  WLAN_CFG_INIT_DELTA_B,
    WLAN_DELTA_A_VAL,               //  WLAN_CFG_INIT_DELTA_A,
    0,                              //  WLAN_CFG_INIT_DENSE_ENV_TRIGGER_B,
    0,                              //  WLAN_CFG_INIT_DENSE_ENV_TRIGGER_A,
    0,                              //  WLAN_CFG_INIT_SCENARIO_ENABLE,
    0,                              //  WLAN_CFG_INIT_CANDIDATE_GOOD_RSSI,
    0,                              //  WLAN_CFG_INIT_CANDIDATE_GOOD_NUM,
    0,                              //  WLAN_CFG_INIT_CANDIDATE_WEAK_NUM,
    0,                              //  WLAN_CFG_INIT_INTERVAL_VARIABLE,
    /* 性能 */
    64,                             //  WLAN_CFG_INIT_AMPDU_TX_MAX_NUM,  // 7
    WLAN_MEM_FOR_START,             //  WLAN_CFG_INIT_USED_MEM_FOR_START,
    WLAN_MEM_FOR_STOP,              //  WLAN_CFG_INIT_USED_MEM_FOR_STOP,
    WLAN_RX_ACK_LIMIT_VAL,              //  WLAN_CFG_INIT_RX_ACK_LIMIT,
    HISDIO_DEV2HOST_SCATT_MAX,          //  WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT,
    WLAN_SDIO_H2D_ASSEMBLE_COUNT_VAL,   //  WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT,
    /* LINKLOSS */
    WLAN_LOSS_THRESHOLD_WLAN_BT,        //  WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_BT,  // 13,这里不能直接赋值
    WLAN_LOSS_THRESHOLD_WLAN_DBAC,      //  WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_DBAC,
    WLAN_LOSS_THRESHOLD_WLAN_NORMAL,    //  WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_NORMAL,
    /* 自动调频 */
    PPS_VALUE_0,                        //  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0,  // 16
    CPU_MIN_FREQ_VALUE_0,               //  WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_0,
    DDR_MIN_FREQ_VALUE_0,               //  WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_0,
    PPS_VALUE_1,                        //  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_1,
    CPU_MIN_FREQ_VALUE_1,               //  WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_1,
    DDR_MIN_FREQ_VALUE_1,               //  WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_1,
    PPS_VALUE_2,                        //  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_2,
    CPU_MIN_FREQ_VALUE_2,               //  WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_2,
    DDR_MIN_FREQ_VALUE_2,               //  WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_2,
    PPS_VALUE_3,                        //  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_3,
    CPU_MIN_FREQ_VALUE_3,               //  WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_3,
    DDR_MIN_FREQ_VALUE_3,               //  WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_3,
    FREQ_IDLE,                          //  WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0,
    FREQ_MIDIUM,                        //  WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_1,
    FREQ_HIGHER,                        //  WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_2,
    FREQ_HIGHEST,                       //  WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3,
    OAL_FALSE,                          //  WLAN_CFG_PRIV_DMA_LATENCY,
    0,                                  //  WLAN_CFG_PRIV_LOCK_CPU_TH_HIGH,
    0,                                  //  WLAN_CFG_PRIV_LOCK_CPU_TH_LOW,
    OAL_FALSE,                          //  WLAN_CFG_INIT_IRQ_AFFINITY,
    WLAN_IRQ_TH_LOW,                    //  WLAN_CFG_INIT_IRQ_TH_LOW,
    WLAN_IRQ_TH_HIGH,                   //  WLAN_CFG_INIT_IRQ_TH_HIGH,
    WLAN_IRQ_PPS_TH_LOW,                //  WLAN_CFG_INIT_IRQ_PPS_TH_LOW,
    WLAN_IRQ_PPS_TH_HIGH,               //  WLAN_CFG_INIT_IRQ_PPS_TH_HIGH,
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    OAL_FALSE,                          //  WLAN_CFG_INIT_HW_AMPDU,
    WLAN_HW_AMPDU_TH_LOW,               //  WLAN_CFG_INIT_HW_AMPDU_TH_LOW,
    WLAN_HW_AMPDU_TH_HIGH,              //  WLAN_CFG_INIT_HW_AMPDU_TH_HIGH,
#endif
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    OAL_FALSE,                          //  WLAN_CFG_INIT_AMPDU_AMSDU_SKB,
    WLAN_AMSDU_AMPDU_TH_LOW,            //  WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW,
    WLAN_AMSDU_AMPDU_TH_MIDDLE,         //  WLAN_CFG_INIT_AMSDU_AMPDU_TH_MIDDLE,
    WLAN_AMSDU_AMPDU_TH_HIGH,           //  WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH,
#endif
#ifdef _PRE_WLAN_TCP_OPT
    OAL_FALSE,                          //  WLAN_CFG_INIT_TCP_ACK_FILTER,
    WLAN_TCP_ACK_FILTER_TH_LOW,         //  WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW,
    WLAN_TCP_ACK_FILTER_TH_HIGH,        //  WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH,
#endif
    OAL_TRUE,                           //  WLAN_CFG_INIT_TX_SMALL_AMSDU,
    WLAN_SMALL_AMSDU_HIGH,              //  WLAN_CFG_INIT_SMALL_AMSDU_HIGH,
    WLAN_SMALL_AMSDU_LOW,               //  WLAN_CFG_INIT_SMALL_AMSDU_LOW,
    WLAN_SMALL_AMSDU_PPS_HIGH,          //  WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH,
    WLAN_SMALL_AMSDU_PPS_LOW,           //  WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW,

    OAL_TRUE,                           //  WLAN_CFG_INIT_TX_TCP_ACK_BUF,
    OAL_FALSE,                          //  WLAN_CFG_DEVICE_TX_TCP_BUF,
    WLAN_TCP_ACK_BUF_HIGH,              //  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH,
    WLAN_TCP_ACK_BUF_LOW,               //  WLAN_CFG_INIT_TCP_ACK_BUF_LOW,
    WLAN_TCP_ACK_BUF_HIGH_40M,          //  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M,
    WLAN_TCP_ACK_BUF_LOW_40M,           //  WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M,
    WLAN_TCP_ACK_BUF_HIGH_80M,          //  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M,
    WLAN_TCP_ACK_BUF_LOW_80M,           //  WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M,
    WLAN_TCP_ACK_BUF_HIGH_160M,         //  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_160M,
    WLAN_TCP_ACK_BUF_LOW_160M,          //  WLAN_CFG_INIT_TCP_ACK_BUF_LOW_160M,

    OAL_FALSE,                          //  WLAN_CFG_INIT_TX_TCP_ACK_BUF_USERCTL,
    WLAN_TCP_ACK_BUF_USERCTL_HIGH,      //  WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_HIGH,
    WLAN_TCP_ACK_BUF_USERCTL_LOW,       //  WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_LOW,

    OAL_FALSE,                          //  WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA,
    WLAN_RX_DYN_BYPASS_EXTLNA_HIGH,     //  WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH,
    WLAN_RX_DYN_BYPASS_EXTLNA_LOW,      //  WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW,

    /* 接收ampdu+amsdu */
    OAL_FALSE,                          //  WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB,
    OAL_FALSE,                          //  WLAN_CFG_INIT_RX_AMPDU_BITMAP,

    /* 低功耗 */
    OAL_TRUE,                           //  WLAN_CFG_INIT_POWERMGMT_SWITCH,  // 31
    WLAN_PS_MODE,                       //  WLAN_CFG_INIT_PS_MODE,
    WLAN_MIN_FAST_PS_IDLE,              //  WLAN_CFG_INIT_MIN_FAST_PS_IDLE,  // How many 20ms
    WLAN_MAX_FAST_PS_IDLE,              //  WLAN_CFG_INIT_MAX_FAST_PS_IDLE,
    WLAN_AUTO_FAST_PS_SCREENON,         //  WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENON,
    WLAN_AUTO_FAST_PS_SCREENOFF,        //  WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENOFF,
    WLAN_MIN_FAST_PS_IDLE,              //  WLAN_CFG_INIT_MIN_FAST_PS_IDLE_P2P,
    WLAN_MAX_FAST_PS_IDLE,              //  WLAN_CFG_INIT_MAX_FAST_PS_IDLE_P2P,
    WLAN_AUTO_FAST_PS_SCREENON,         //  WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENON_P2P,
    WLAN_AUTO_FAST_PS_SCREENOFF,        //  WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENOFF_P2P,

    /* 低功耗代理 */
    OAL_FALSE,                          //  WLAN_CFG_INIT_LOWPOWER_AGENT_SWITCH_MP13,

    /* 可维可测 */
    OAM_LOG_LEVEL_WARNING,              //  WLAN_CFG_INIT_LOGLEVEL,
    /* 2G RF前端 */
    0xF4F4,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND1 = WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_START,
    0xF4F4,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND2,
    0xF4F4,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND3 = WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_END

    /* 5G RF前端 */
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND1 = WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_START,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND2,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND3,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND4,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND5,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND6,
    0xF8F8,         //  WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND7 = WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_END

    /* 用于定制化计算PWR RF值的偏差 */
    0,              //  WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C0_MULT4,
    0,              //  WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C1_MULT4,
    0,              //  WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C0_MULT4,
    0,              //  WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C1_MULT4,

    /* 用于定制化计算RSSI的偏差 */
    0,              //  WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C0,
    0,              //  WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C1,
    0,              //  WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C0,
    0,              //  WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C1,

    /* FEM mp13考虑2g */
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G,
    0x00140014,     //  WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G,
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G,
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G,
    0x00010001,     //  WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_PA_ISEXIST_2G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G,
    0x02760276,     //  WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G,
    0x01400140,     //  WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G,
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G,
    0x00140014,     //  WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G,
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G,
    0xFFF4FFF4,     //  WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G,
    0x00010001,     //  WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_PA_ISEXIST_5G,
    0x00010001,     //  WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G,
    0x02760276,     //  WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G,
    0x01400140,     //  WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G,
    /* SCAN */
    1,              //  WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN,
    /* 11AC2G */
    1,              //  WLAN_CFG_INIT_11AC2G_ENABLE,
    /* 11ac2g开关 */                    // 56
    0,              //  WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40, /* 2ght40禁止开关 */
    0,              //  WLAN_CFG_INIT_DUAL_ANTENNA_ENABLE,  /* 双天线开关 */

    /* probe req应答模式功能配置 */
    /* BIT7~BIT4:动态功能开关 ;BIT3~BIT0:功能模式(mac_probe_resp_mode_enum_uint8) , 目前此配置仅针对p2p dev设备有效 */
    0x10,           //  WLAN_CFG_INIT_PROBE_RESP_MODE,
    0,              //  WLAN_CFG_INIT_MIRACAST_SINK_ENABLE, /* miracast_sink特性开关 */
    0,              //  WLAN_CFG_INIT_REDUCE_MIRACAST_LOG, /* miracast log 降级开关 */
    0,              //  WLAN_CFG_INIT_BIND_LOWEST_LOAD_CPU, /* 是否支持绑定负载最轻的CPU */
    1,              //  WLAN_CFG_INIT_CORE_BIND_CAP, /* 是否支持cpu绑定 */
    0,              //  WLAN_CFG_INIT_FAST_MODE, /* BUCK stay in fastmode */

    /* sta keepalive */
    WLAN_STA_KEEPALIVE_CNT_TH,  //  WLAN_CFG_INIT_STA_KEEPALIVE_CNT_TH, /* sta keepalive th */
    1,                          //  WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH,
    1,          //  WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH, /* 超远距11b 1m 2m dbb scale提升使能开关 */
    0xF,        //  WLAN_CFG_INIT_CHANN_RADIO_CAP, /* 通道0、1 2g 5g频道能力 */

    0,          //  WLAN_CFG_LTE_GPIO_CHECK_SWITCH, /* lte?????? */
    0,          //  WLAN_ATCMDSRV_ISM_PRIORITY,
    0,          //  WLAN_ATCMDSRV_LTE_RX,
    0,          //  WLAN_ATCMDSRV_LTE_TX,
    0,          //  WLAN_ATCMDSRV_LTE_INACT,
    0,          //  WLAN_ATCMDSRV_ISM_RX_ACT,
    0,          //  WLAN_ATCMDSRV_BANT_PRI,
    0,          //  WLAN_ATCMDSRV_BANT_STATUS,
    0,          //  WLAN_ATCMDSRV_WANT_PRI,
    0,          //  WLAN_ATCMDSRV_WANT_STATUS,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_1,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_2,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_3,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_4,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_5,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_6,
    0,          //  WLAN_TX5G_UPC_MIX_GAIN_CTRL_7,
    /* PA bias */
    0x12081208,         //  WLAN_TX2G_PA_GATE_VCTL_REG236,
    0x2424292D,         //  WLAN_TX2G_PA_GATE_VCTL_REG237,
    0x24242023,         //  WLAN_TX2G_PA_GATE_VCTL_REG238,
    0x24242020,         //  WLAN_TX2G_PA_GATE_VCTL_REG239,
    0x24242020,         //  WLAN_TX2G_PA_GATE_VCTL_REG240,
    0x24241B1B,         //  WLAN_TX2G_PA_GATE_VCTL_REG241,
    0x24241B1B,         //  WLAN_TX2G_PA_GATE_VCTL_REG242,
    0x24241B1B,         //  WLAN_TX2G_PA_GATE_VCTL_REG243,
    0x24241B1B,         //  WLAN_TX2G_PA_GATE_VCTL_REG244,

    0x14141414,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG253,
    0x13131313,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG254,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG255,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG256,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG257,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG258,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG259,
    0x12121212,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG260,
    0x0F0F0F0F,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG261,
    0x0D0D0D0D,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG262,
    0x0A0B0A0B,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG263,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG264,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG265,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG266,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG267,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG268,
    0x0F0F0F0F,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG269,
    0x0D0D0D0D,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG270,
    0x0A0B0A0B,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG271,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG272,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG273,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG274,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG275,
    0x0A0A0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG276,
    0x0D0D0D0D,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG277,
    0x0D0D0D0D,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG278,
    0x0D0D0A0B,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG279,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND1,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG281,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG282,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG283,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG284,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND2,
    0x0D0D0A0A,         //  WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND3,
    0,                  //  WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G,
    0,                  //  WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G,
    0,                  //  WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G,
    0,                  //  WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G,
    0,                  //  WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_80TH_5G,
    0,                  //  WLAN_CFG_INIT_VOE_SWITCH,
    0,                  //  WLAN_CFG_INIT_11AX_SWITCH,
    0,                  //  WLAN_CFG_INIT_HTC_SWITCH,
    0,                  //  WLAN_CFG_INIT_MULTI_BSSID_SWITCH,

    /* ldac m2s rssi */
    WLAN_BTCOEX_THRESHOLD_MCM_DOWN,         //  WLAN_CFG_INIT_LDAC_THRESHOLD_M2S,
    WLAN_BTCOEX_THRESHOLD_MCM_UP,           //  WLAN_CFG_INIT_LDAC_THRESHOLD_S2M,

    /* btcoex mcm rssi */
    WLAN_BTCOEX_THRESHOLD_MCM_DOWN,         //  WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_DOWN,
    WLAN_BTCOEX_THRESHOLD_MCM_UP,           //  WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_UP,

#ifdef _PRE_WLAN_FEATURE_NRCOEX
    /* 5g nr coex */
    0,                  //  WLAN_CFG_INIT_NRCOEX_ENABLE,
    0x01,               //  WLAN_CFG_INIT_NRCOEX_VERSION,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_FREQ,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE0_SMALLGAP0_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE0_GAP01_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE0_GAP12_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_FREQ,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP2,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP2,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE1_SMALLGAP0_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE1_GAP01_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE1_GAP12_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE1_RXSLOT_RSSI,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_FREQ,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP2,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP2,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE2_SMALLGAP0_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE2_GAP01_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE2_GAP12_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE2_RXSLOT_RSSI,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_FREQ,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP0,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP1,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP2,
    0x00000000,         //  WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP2,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE3_SMALLGAP0_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE3_GAP01_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE3_GAP12_ACT,
    0xFFFFFFFF,         //  WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI,
#endif

    0,                  //  WLAN_CFG_INIT_DYNAMIC_DBAC_SWITCH,

#ifdef _PRE_WLAN_FEATURE_MBO
    0,                  //  WLAN_CFG_INIT_MBO_SWITCH,
#endif

    WLAN_DDR_CAHNL_FREQ,        //  WLAN_CFG_INIT_DDR_FREQ,
    WLAN_HIEX_DEV_CAP,          //  WLAN_CFG_INIT_HIEX_CAP,
    OAL_FALSE,                  //  WLAN_CFG_INIT_FTM_CAP,
    /* 用于修正由于phy mode滤波器收窄导致的rssi偏差 */
    0,                          //  WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C0,
    0,                          //  WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C1,
    0,                          //  WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C0,
    0,                          //  WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C1,

#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    /* 组播聚合参数配置 */
    OAL_FALSE,                  //  WLAN_CFG_INIT_MCAST_AMPDU_ENABLE,
#endif
    OAL_FALSE,                  //  WLAN_CFG_INIT_PT_MCAST_ENABLE,
    0,                          //  WLAN_CFG_INIT_FEM_POLAR_POLICY_2G,
    0,                          //  WLAN_CFG_INIT_FEM_POLAR_POLICY_5G,
    0xDDDDD3D3,                 //  WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_HIGH_TH,
    0xDADAD0D0,                 //  WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_LOW_TH,
    0,                          //  WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_HIGH_TH,
    0,                          //  WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_LOW_TH,
    0,                          //  WLAN_CFG_INIT_REDUCE_PWR_2G_40MHZ_CHANNEL_BITMAP,

#ifdef _PRE_WLAN_EXPORT
    -1,                         //  WLAN_CFG_INIT_EFUSE_MAC_ADDR,
    0,                          //  WLAN_CFG_INIT_EFUSE_MAC_ADDR_2BYTE,
    0x06f032bc,                 //  WLAN_CFG_INIT_5G_POW_PARA_L0,
    0x06f032bc,                 //  WLAN_CFG_INIT_5G_POW_PARA_L1,
    0x064032bc,                 //  WLAN_CFG_INIT_5G_POW_PARA_L2,
    0x064022bc,                 //  WLAN_CFG_INIT_5G_POW_PARA_L3,
    0x050022bc,                 //  WLAN_CFG_INIT_5G_POW_PARA_L4,
    0x0500028a,                 //  WLAN_CFG_INIT_5G_POW_PARA_L5,
    0x384022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L0,
    0x384022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L1,
    0x258022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L2,
    0x1f4022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L3,
    0x190022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L4,
    0x12c022ee,                 //  WLAN_CFG_INIT_2G_POW_PARA_L5,
    0,                          //  0,
#endif
                                //  end WLAN_CFG_INIT_BUTT,
};
#endif

static int32_t hwifi_hcc_custom_get_data(uint16_t syn_id, oal_netbuf_stru *netbuf)
{
    uint32_t data_len = 0;
    uint8_t *netbuf_data = (uint8_t *)oal_netbuf_data(netbuf);

    switch (syn_id) {
        case CUSTOM_CFGID_INI_ID:
            /* INI hmac to dmac 配置项 */
            data_len = (uint32_t)hwifi_custom_adapt_device_ini_param(netbuf_data);
            break;
        case CUSTOM_CFGID_PRIV_INI_ID:
            /* 私有定制化配置项 */
            data_len = (uint32_t)hwifi_custom_adapt_device_priv_ini_param(netbuf_data);
            break;
        default:
            oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf::unknown us_syn_id[%d]", syn_id);
            break;
    }

    return data_len;
}

/*
 * 函 数 名  : hwifi_hcc_custom_ini_data_buf
 * 功能描述  : 下发定制化配置命令
 */
OAL_STATIC int32_t hwifi_hcc_custom_ini_data_buf(uint16_t us_syn_id)
{
    oal_netbuf_stru *pst_netbuf = NULL;
    uint32_t data_len;
    int32_t l_ret;
    uint32_t max_data_len = hcc_get_handler(HCC_EP_WIFI_DEV)->tx_max_buf_len;

    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = hcc_get_handler(HCC_EP_WIFI_DEV);
    if (hcc == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf hcc::is is null");
        return -OAL_EFAIL;
    }
    pst_netbuf = hwifi_hcc_custom_netbuf_alloc();
    if (pst_netbuf == NULL) {
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 组netbuf */
    data_len = (uint32_t)hwifi_hcc_custom_get_data(us_syn_id, pst_netbuf);
    if (data_len > max_data_len) {
        oam_error_log2(0, OAM_SF_CFG,
            "hwifi_hcc_custom_ini_data_buf::got wrong data_len[%d] max_len[%d]", data_len, max_data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    if (data_len == 0) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf::data is null us_syn_id[%d]", us_syn_id);
        oal_netbuf_free(pst_netbuf);
        return OAL_SUCC;
    }

    if ((pst_netbuf->data_len) || (pst_netbuf->data == NULL)) {
        oal_io_print("len:%d\r\n", pst_netbuf->data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    oal_netbuf_put(pst_netbuf, data_len);
    hcc_hdr_param_init(&st_hcc_transfer_param, HCC_ACTION_TYPE_CUSTOMIZE,
                       us_syn_id, 0, HCC_FC_WAIT, DATA_HI_QUEUE);

    l_ret = hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf fail ret[%d]", l_ret);
        oal_netbuf_free(pst_netbuf);
    }

    return l_ret;
}

/*
 * 函 数 名  : custom_host_read_cfg_init
 * 功能描述  : 首次读取定制化配置文件总入口
 */
uint32_t hwifi_custom_host_read_cfg_init_mp13(void)
{
    int32_t l_nv_read_ret;
    int32_t l_ini_read_ret;

    hwifi_get_country_code();
    /* 先获取私有定制化项 */
    if (hwifi_config_init(CUS_TAG_PRIV_INI) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init::hwifi_config_init fail");
    }

    /* 读取nvram参数是否修改 */
    l_nv_read_ret = hwifi_custom_host_read_dyn_cali_nvram();
    /* 检查定制化文件中的产线配置是否修改 */
    l_ini_read_ret = ini_file_check_conf_update(INI_SECTION_WIFI);
    if (l_ini_read_ret || l_nv_read_ret) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init config is updated");
        hwifi_config_init(CUS_TAG_PRO_LINE_INI);
    }

    if (l_ini_read_ret == INI_FILE_TIMESPEC_UNRECONFIG) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init file is not updated");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (hwifi_config_init(CUS_TAG_DTS) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init::hwifi_config_init fail");
    }
    l_ini_read_ret = hwifi_config_init(CUS_TAG_NV);
    if (oal_unlikely(l_ini_read_ret != OAL_SUCC)) {
        oal_io_print("hwifi_custom_host_read_cfg_init NV fail l_ret[%d].\r\n", l_ini_read_ret);
    }

    if (hwifi_config_init(CUS_TAG_INI) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init::hwifi_config_init fail");
    }

    /* 启动完成后，输出打印 */
    oal_io_print("hwifi_custom_host_read_cfg_init finish!\r\n");

    return OAL_SUCC;
}

/*
 * 函 数 名  : hwifi_hcc_customize_h2d_data_cfg
 * 功能描述  : 协议栈初始化前定制化配置入口
 */
uint32_t hwifi_hcc_customize_h2d_data_cfg_mp13(void)
{
    int32_t l_ret;

    /* wifi上电时重读定制化配置 */
    l_ret = (int32_t)hwifi_custom_host_read_cfg_init_mp13();
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg data ret[%d]", l_ret);
    }

    // 如果不成功，返回失败
    l_ret = hwifi_hcc_custom_ini_data_buf(CUSTOM_CFGID_PRIV_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg priv data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }

    l_ret = hwifi_hcc_custom_ini_data_buf(CUSTOM_CFGID_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg ini data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}

/*
 * 函 数 名  : hwifi_get_cfg_delt_ru_pow_params
 * 功能描述  : host查看ini定制化参数维测命令
 * 修改历史      :
 *   1.日    期   : 2019年8月21日
 *     作    者   : wifi
 *     修改内容   : 新生成函数
 */
static void hwifi_get_cfg_delt_ru_pow_params(void)
{
    int32_t l_cfg_idx_one = 0;
    int32_t l_cfg_idx_two = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_BUTT; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_BUTT; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d]:%d \n", "5g_cfg_tpc_ru_pow", l_cfg_idx_one, l_cfg_idx_two,
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_80M; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_996; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d]:%d \n", "2g_cfg_tpc_ru_pow", l_cfg_idx_one, l_cfg_idx_two,
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
}

/*
 * 函 数 名  : hwifi_config_init
 * 功能描述  : netdev open 调用的定制化总入口
 *             读取ini文件，更新 g_host_init_params 全局数组
 */
int32_t hwifi_config_init(int32_t cus_tag)
{
    int32_t l_cfg_id;
    int32_t l_ret = INI_FAILED;
    int32_t l_ori_val;
    wlan_cfg_cmd *pgast_wifi_config = NULL;
    int32_t *pgal_params = NULL;
    int32_t l_cfg_value = 0;
    int32_t l_wlan_cfg_butt;

    switch (cus_tag) {
        case CUS_TAG_NV:
            original_value_for_nvram_params();
            return hwifi_config_init_nvram();
        case CUS_TAG_INI:
            host_params_init_first();
            pgast_wifi_config = hwifi_get_g_wifi_config_cmds();
            pgal_params = hwifi_get_g_host_init_params();
            l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
            break;
        case CUS_TAG_DTS:
            original_value_for_dts_params();
            pgast_wifi_config = hwifi_get_g_wifi_config_dts();
            pgal_params = hwifi_get_g_dts_params();
            l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
            break;
        case CUS_TAG_PRIV_INI:
            return hwifi_config_init_private_custom();
        case CUS_TAG_PRO_LINE_INI:
            return hwifi_config_init_dy_cali_custom();
        default:
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init tag number[0x%x] not correct!", cus_tag);
            return INI_FAILED;
    }

    for (l_cfg_id = 0; l_cfg_id < l_wlan_cfg_butt; l_cfg_id++) {
        /* 获取ini的配置值 */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, pgast_wifi_config[l_cfg_id].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            oam_info_log2(0, OAM_SF_CUSTOM, "hwifi_config_init read ini file cfg_id[%d]tag[%d] not exist!",
                          l_cfg_id, cus_tag);
            continue;
        }
        l_ori_val = pgal_params[pgast_wifi_config[l_cfg_id].case_entry];
        pgal_params[pgast_wifi_config[l_cfg_id].case_entry] = l_cfg_value;
    }

    return INI_SUCC;
}

int32_t hwifi_get_init_value(int32_t cus_tag, int32_t cfg_id)
{
    int32_t *pgal_params = NULL;
    int32_t l_wlan_cfg_butt;

    if (cus_tag == CUS_TAG_INI) {
        pgal_params = hwifi_get_g_host_init_params();
        l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
    } else if (cus_tag == CUS_TAG_DTS) {
        pgal_params = hwifi_get_g_dts_params();
        l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
    } else {
        oam_error_log1(0, OAM_SF_ANY, "hwifi_get_init_value tag number[0x%2x] not correct!", cus_tag);
        return INI_FAILED;
    }

    if (cfg_id < 0 || l_wlan_cfg_butt <= cfg_id) {
        oam_error_log2(0, OAM_SF_ANY, "hwifi_get_init_value cfg id[%d] out of range, max cfg id is:%d",
                       cfg_id, (l_wlan_cfg_butt - 1));
        return INI_FAILED;
    }

    return pgal_params[cfg_id];
}

/*
 * 函 数 名  : hwifi_get_cfg_pow_ctrl_params
 * 功能描述  : host查看ini定制化参数维测命令
 */
OAL_STATIC void hwifi_get_cfg_pow_ctrl_params(uint8_t uc_chn_idx)
{
    int32_t l_cfg_idx_one = 0;
    int32_t l_cfg_idx_two = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();
    wlan_cust_cfg_custom_fcc_ce_txpwr_limit_stru *pst_fcc_ce_param;

    pst_fcc_ce_param = &pst_g_cust_nv_params->ast_fcc_ce_param[uc_chn_idx];
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_20M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:20M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20m[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_40M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:40M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40m[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_80M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:80M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80m[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_160M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:160M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_160m[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < MAC_2G_CHANNEL_NUM; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < CUS_NUM_FCC_CE_2G_PRO; l_cfg_idx_two++) {
            oal_io_print("%s[%d] [%d] \t [config:%d]\n", "fcc_txpwr_limit_2g: chan", l_cfg_idx_one, l_cfg_idx_two,
                         pst_fcc_ce_param->auc_2g_fcc_txpwr_limit_params[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
}

/*
 * 函 数 名  : hwifi_get_cfg_iq_lpf_lvl_params
 * 功能描述  : host查看ini定制化参数维测命令
 */
OAL_STATIC void hwifi_get_cfg_iq_lpf_lvl_params(void)
{
    int32_t l_cfg_idx_one = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_5G_LPF_LVL; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_iq_cali_lpf_lvl", l_cfg_idx_one,
                     pst_g_cust_nv_params->auc_5g_iq_cali_lpf_params[l_cfg_idx_one]);
    }
}

OAL_STATIC void hwifi_get_cfg_ini_params(void)
{
    int32_t cfg_idx_one;
    int32_t *host_init_params = hwifi_get_g_host_init_params();
    wlan_cfg_cmd *wifi_config_cmds = hwifi_get_g_wifi_config_cmds();

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_CFG_INIT_BUTT; ++cfg_idx_one) {
        oal_io_print("%s \t [config:0x%x]\n", wifi_config_cmds[cfg_idx_one].name,
            host_init_params[cfg_idx_one]);
    }
}

OAL_STATIC void hwifi_get_cfg_txpwr_params(void)
{
    int32_t cfg_idx_one;
    int32_t cfg_idx_two;
    wlan_cfg_cmd *nvram_config_ini = hwifi_get_nvram_config();
    wlan_cust_nvram_params *cust_nv_params = hwifi_get_nvram_params(); /* 最大发送功率定制化数组 */

    for (cfg_idx_one = 0; cfg_idx_one < NUM_OF_NV_MAX_TXPOWER; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_txpwr_params", cfg_idx_one,
            cust_nv_params->ac_delt_txpwr_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < NUM_OF_NV_DPD_MAX_TXPOWER; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_dpd_txpwr_params",
            cfg_idx_one, cust_nv_params->ac_dpd_delt_txpwr_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < NUM_OF_NV_11B_DELTA_TXPOWER; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_11b_txpwr_params", cfg_idx_one,
            cust_nv_params->ac_11b_delt_txpwr_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_RF_CHANNEL_NUMS; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_IQ_cali_pow", cfg_idx_one,
                     cust_nv_params->auc_fem_off_iq_cal_pow_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < NUM_OF_NV_5G_UPPER_UPC; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_upper_upc_params", cfg_idx_one,
            cust_nv_params->auc_5g_upper_upc_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < NUM_OF_NV_2G_LOW_POW_DELTA_VAL; ++cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "2g_low_pow_amend_val", cfg_idx_one,
            cust_nv_params->ac_2g_low_pow_amend_params[cfg_idx_one]);
    }

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_RF_CHANNEL_NUMS; cfg_idx_one++) {
        for (cfg_idx_two = 0; cfg_idx_two < CUS_BASE_PWR_NUM_2G; cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "2G base_pwr_params", cfg_idx_one,
                cfg_idx_two, cust_nv_params->auc_2g_txpwr_base_params[cfg_idx_one][cfg_idx_two]);
        }
        for (cfg_idx_two = 0; cfg_idx_two < CUS_BASE_PWR_NUM_5G; cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "5G base_pwr_params", cfg_idx_one,
                cfg_idx_two, cust_nv_params->auc_5g_txpwr_base_params[cfg_idx_one][cfg_idx_two]);
        }
    }
    oal_io_print("%s \t [config:%d]\n", nvram_config_ini[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR].name,
        cust_nv_params->uc_5g_max_pwr_fcc_ce_for_high_band);
}

OAL_STATIC void hwifi_get_cfg_fcc_ce_params(void)
{
    int32_t cfg_idx_one;

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_RF_CHANNEL_NUMS; cfg_idx_one++) {
        /* FCC/CE */
        oal_io_print("%s \t [RF:%d]\n", "hwifi_get_cfg_pow_ctrl_params", cfg_idx_one);
        hwifi_get_cfg_pow_ctrl_params(cfg_idx_one);
    }
}

OAL_STATIC void hwifi_get_cfg_sar_params(void)
{
    int32_t cfg_idx_one;
    int32_t cfg_idx_two;
    wlan_init_cust_nvram_params *init_g_cust_nv_params = hwifi_get_init_nvram_params();

    for (cfg_idx_one = 0; cfg_idx_one < CUS_NUM_OF_SAR_LVL; cfg_idx_one++) {
        for (cfg_idx_two = 0; cfg_idx_two < CUS_NUM_OF_SAR_PARAMS; cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:C0 %d C1 %d]\n", "sar_ctrl_params: lvl", cfg_idx_one, cfg_idx_two,
                init_g_cust_nv_params->st_sar_ctrl_params[cfg_idx_one][cfg_idx_two].auc_sar_ctrl_params_c0,
                init_g_cust_nv_params->st_sar_ctrl_params[cfg_idx_one][cfg_idx_two].auc_sar_ctrl_params_c1);
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
OAL_STATIC void hwifi_get_cfg_tas_params(void)
{
    int32_t cfg_idx_one;
    int32_t cfg_idx_two;
    wlan_cust_nvram_params *cust_nv_params = hwifi_get_nvram_params(); /* 最大发送功率定制化数组 */

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_RF_CHANNEL_NUMS; cfg_idx_one++) {
        for (cfg_idx_two = 0; cfg_idx_two < WLAN_BAND_BUTT; cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "tas_ctrl_params: lvl", cfg_idx_one, cfg_idx_two,
                cust_nv_params->auc_tas_ctrl_params[cfg_idx_one][cfg_idx_two]);
        }
    }
}
#endif

OAL_STATIC void hwifi_get_cfg_dts_params(void)
{
    int32_t cfg_idx_one;
    wlan_cfg_cmd *wifi_config_dts_func = hwifi_get_g_wifi_config_dts();
    int32_t *dts_params = hwifi_get_g_dts_params();

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_CFG_DTS_BUTT; ++cfg_idx_one) {
        oal_io_print("%s \t [config:0x%x]\n", wifi_config_dts_func[cfg_idx_one].name, dts_params[cfg_idx_one]);
    }
}

/*
 * 函 数 名  : hwifi_get_cfg_params
 * 功能描述  : host查看ini定制化参数维测命令
 */
void hwifi_get_cfg_params(void)
{
    oal_io_print("\nhwifi_get_cfg_params\n");

    /* CUS_TAG_INI */
    hwifi_get_cfg_ini_params();

    /* CUS_TAG_TXPWR */
    hwifi_get_cfg_txpwr_params();

    /* FCC CE */
    hwifi_get_cfg_fcc_ce_params();

    /* SAR */
    hwifi_get_cfg_sar_params();

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    hwifi_get_cfg_tas_params();
#endif

    /* CUS_TAG_DTS */
    hwifi_get_cfg_dts_params();

    /* iq lvl */
    hwifi_get_cfg_iq_lpf_lvl_params();

    /* pro line */
    hwifi_get_cfg_pro_line_params();

    /* RU DELT POW  */
    hwifi_get_cfg_delt_ru_pow_params();
}

/*
 * 函 数 名  : hwifi_config_init_nvram_main_mp13
 * 功能描述  : 解析从nv或dts中读取的字符数组，存入结构体数组中
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hwifi_config_init_nvram_main_mp13(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_offset = sizeof(wlan_cust_nvram_params); /* 包括4个基准功率 */

    wal_write_msg_hdr_init(&st_write_msg, WLAN_CFGID_SET_CUS_NVRAM_PARAM, us_offset);
    l_ret = memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), hwifi_get_nvram_params(), us_offset);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_nvram_main::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + us_offset,
        (uint8_t *)&st_write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_nvram_main::err [%d]!}", l_ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : hwifi_config_performance_ini_param
 * 功能描述  : 初始化host全局变量
 * 1.日    期  : 2019年11月22日
 *   修改内容  : 新生成函数
 */
void hwifi_config_cpu_freq_ini_param_mp13(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_DMA_LATENCY);
    g_freq_lock_control.uc_lock_dma_latency = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    g_freq_lock_control.dma_latency_value = (uint16_t)l_val;
    oal_io_print("DMA latency[%d]\r\n", g_freq_lock_control.uc_lock_dma_latency);
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_LOCK_CPU_TH_HIGH);
    g_freq_lock_control.us_lock_cpu_th_high = (uint16_t)l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_PRIV_LOCK_CPU_TH_LOW);
    g_freq_lock_control.us_lock_cpu_th_low = (uint16_t)l_val;
    oal_io_print("CPU freq high[%d] low[%d]\r\n", g_freq_lock_control.us_lock_cpu_th_high,
        g_freq_lock_control.us_lock_cpu_th_low);

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_AFFINITY);
    g_freq_lock_control.en_irq_affinity = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_TH_HIGH);
    g_freq_lock_control.us_throughput_irq_high = (l_val > 0) ? (uint16_t)l_val : WLAN_IRQ_TH_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_TH_LOW);
    g_freq_lock_control.us_throughput_irq_low = (l_val > 0) ? (uint16_t)l_val : WLAN_IRQ_TH_LOW;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_PPS_TH_HIGH);
    g_freq_lock_control.irq_pps_high = (l_val > 0) ? (uint32_t)l_val : WLAN_IRQ_PPS_TH_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_IRQ_PPS_TH_LOW);
    g_freq_lock_control.irq_pps_low = (l_val > 0) ? (uint32_t)l_val : WLAN_IRQ_PPS_TH_LOW;
    g_freq_lock_control.en_userctl_bindcpu = OAL_FALSE; /* 用户指定绑核命令默认不开启 */
    g_freq_lock_control.uc_userctl_irqbind = 0;
    g_freq_lock_control.uc_userctl_threadbind = 0;
    oal_io_print("irq affinity enable[%d]High_th[%u]Low_th[%u]\r\n", g_freq_lock_control.en_irq_affinity,
        g_freq_lock_control.us_throughput_irq_high, g_freq_lock_control.us_throughput_irq_low);
}

/*
 * 函 数 名  : hwifi_set_voe_custom_param
 * 功能描述  : 根据ini文件配置漫游kvr能力的相关参数
 * 1.日    期  : 2019年04月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_set_voe_custom_param(void)
{
    uint32_t val;

    val = (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_VOE_SWITCH);
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11k = (val & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11v = (val & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r = (val & BIT2) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_11r_ds = (val & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_adaptive11r = (val & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_voe_custom_cfg.en_nb_rpt_11k = (val & BIT5) ? OAL_TRUE : OAL_FALSE;

    return;
}


OAL_STATIC void hwifi_config_host_roam_global_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_ROAM_SWITCH);
    g_wlan_cust.uc_roam_switch = (l_val == 0 || l_val == 1) ?
        (uint8_t)l_val : g_wlan_cust.uc_roam_switch;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SCAN_ORTHOGONAL);
        g_wlan_cust.uc_roam_scan_orthogonal = (l_val >= 1) ?
    (uint8_t)l_val : g_wlan_cust.uc_roam_scan_orthogonal;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_B);
    g_wlan_cust.c_roam_trigger_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_A);
    g_wlan_cust.c_roam_trigger_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_B);
    g_wlan_cust.c_roam_delta_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_A);
    g_wlan_cust.c_roam_delta_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DENSE_ENV_TRIGGER_B);
    g_wlan_cust.c_dense_env_roam_trigger_b = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DENSE_ENV_TRIGGER_A);
    g_wlan_cust.c_dense_env_roam_trigger_a = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SCENARIO_ENABLE);
    g_wlan_cust.uc_scenario_enable = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_GOOD_RSSI);
    g_wlan_cust.c_candidate_good_rssi = (int8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_GOOD_NUM);
    g_wlan_cust.uc_candidate_good_num = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CANDIDATE_WEAK_NUM);
    g_wlan_cust.uc_candidate_weak_num = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_INTERVAL_VARIABLE);
    g_wlan_cust.us_roam_interval = (uint16_t)l_val;
}

OAL_STATIC void hwifi_config_tcp_ack_buf_ini_param(void)
{
    int32_t l_val;
    mac_tcp_ack_buf_switch_stru *tcp_ack_buf_switch = mac_vap_get_tcp_ack_buf_switch();

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_TCP_ACK_BUF);
    tcp_ack_buf_switch->uc_ini_tcp_ack_buf_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_DEVICE_TX_TCP_BUF);
    tcp_ack_buf_switch->uc_ini_tcp_buf_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    hmac_device_tcp_buf_init(tcp_ack_buf_switch->uc_ini_tcp_buf_en);
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH);
    tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW);
    tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low = (l_val > 0) ? (uint16_t)l_val : 30; /* 吞吐量30 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_high_40m = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_HIGH_40M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_low_40m = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_LOW_40M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_high_80m = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_HIGH_80M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_low_80m = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_LOW_80M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_160M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_high_160m = (l_val > 0) ?
                                                              (uint16_t)l_val : WLAN_TCP_ACK_BUF_HIGH_160M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_160M);
    tcp_ack_buf_switch->tcp_ack_buf_throughput_low_160m = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_LOW_160M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_TCP_ACK_BUF_USERCTL);
    tcp_ack_buf_switch->uc_ini_tcp_ack_buf_userctl_test_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    /* 上层默认设置TCP ack上门限30M */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_HIGH);
    tcp_ack_buf_switch->us_tcp_ack_buf_userctl_high = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_USERCTL_HIGH;
     /* 上层默认设置TCP ack上门限20M */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_LOW);
    tcp_ack_buf_switch->us_tcp_ack_buf_userctl_low = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_BUF_USERCTL_LOW;
    oal_io_print("TCP ACK BUF en[%d],high/low:20M[%d]/[%d],40M[%d]/[%d],80M[%d]/[%d],160M[%d]/[%d],\
        TCP ACK BUF USERCTL[%d], userctl[%d]/[%d]",
        tcp_ack_buf_switch->uc_ini_tcp_ack_buf_en,
        tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high,
        tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_high_40m,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_low_40m,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_high_80m,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_low_80m,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_high_160m,
        tcp_ack_buf_switch->tcp_ack_buf_throughput_low_160m,
        tcp_ack_buf_switch->uc_ini_tcp_ack_buf_userctl_test_en,
        tcp_ack_buf_switch->us_tcp_ack_buf_userctl_high,
        tcp_ack_buf_switch->us_tcp_ack_buf_userctl_low);
}

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU

/*
 * 函 数 名  : hwifi_config_host_amsdu_th_ini_param
 * 功能描述  : 根据ini文件配置amsdu聚合门限的相关参数
 * 1.日    期  : 2019年11月9日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_host_amsdu_th_ini_param(void)
{
    int32_t l_val;
    mac_small_amsdu_switch_stru *small_amsdu_switch = mac_vap_get_small_amsdu_switch();

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_AMSDU_SKB);
    g_st_tx_large_amsdu.uc_host_large_amsdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH);
    g_st_tx_large_amsdu.us_amsdu_throughput_high = (l_val > 0) ? (uint16_t)l_val : 500;   /* 500是高聚合量 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_MIDDLE);
    g_st_tx_large_amsdu.us_amsdu_throughput_middle = (l_val > 0) ? (uint16_t)l_val : 100; /* 100是中等聚合量 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW);
    g_st_tx_large_amsdu.us_amsdu_throughput_low = (l_val > 0) ? (uint16_t)l_val : 50;     /* 50是低聚合量 */
    oal_io_print("ampdu+amsdu lareg amsdu en[%d],high[%d],low[%d],middle[%d]\r\n",
        g_st_tx_large_amsdu.uc_host_large_amsdu_en, g_st_tx_large_amsdu.us_amsdu_throughput_high,
        g_st_tx_large_amsdu.us_amsdu_throughput_low, g_st_tx_large_amsdu.us_amsdu_throughput_middle);

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_SMALL_AMSDU);
    small_amsdu_switch->uc_ini_small_amsdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_HIGH);
    small_amsdu_switch->us_small_amsdu_throughput_high = (l_val > 0) ? (uint16_t)l_val : WLAN_SMALL_AMSDU_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_LOW);
    small_amsdu_switch->us_small_amsdu_throughput_low = (l_val > 0) ? (uint16_t)l_val : WLAN_SMALL_AMSDU_LOW;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH);
    small_amsdu_switch->us_small_amsdu_pps_high = (l_val > 0) ? (uint16_t)l_val : WLAN_SMALL_AMSDU_PPS_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW);
    small_amsdu_switch->us_small_amsdu_pps_low = (l_val > 0) ? (uint16_t)l_val : WLAN_SMALL_AMSDU_PPS_LOW;
    oal_io_print("SMALL AMSDU SWITCH en[%d],high[%d],low[%d]\r\n", small_amsdu_switch->uc_ini_small_amsdu_en,
        small_amsdu_switch->us_small_amsdu_throughput_high, small_amsdu_switch->us_small_amsdu_throughput_low);
}
#endif

/*
 * 函 数 名  : hwifi_config_performance_ini_param
 * 功能描述  : 初始化host全局变量
 * 1.日    期  : 2019年11月22日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_performance_ini_param(void)
{
    int32_t l_val;
    mac_rx_buffer_size_stru *rx_buffer_size = mac_vap_get_rx_buffer_size();

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_TX_MAX_NUM);
    g_wlan_cust.ampdu_tx_max_num = (g_wlan_spec_cfg->max_tx_ampdu_num >= l_val && l_val >= 1) ?
        (uint32_t)l_val : g_wlan_cust.ampdu_tx_max_num;
    oal_io_print("hwifi_config_host_global_ini_param::ampdu_tx_max_num:%d", g_wlan_cust.ampdu_tx_max_num);

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU);
    g_st_ampdu_hw.uc_ampdu_hw_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU_TH_HIGH);
    g_st_ampdu_hw.us_throughput_high = (l_val > 0) ? (uint16_t)l_val : WLAN_HW_AMPDU_TH_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HW_AMPDU_TH_LOW);
    g_st_ampdu_hw.us_throughput_low = (l_val > 0) ? (uint16_t)l_val : WLAN_HW_AMPDU_TH_LOW;
    oal_io_print("ampdu_hw enable[%d]H[%u]L[%u]\r\n", g_st_ampdu_hw.uc_ampdu_hw_en,
        g_st_ampdu_hw.us_throughput_high, g_st_ampdu_hw.us_throughput_low);
#endif

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    hwifi_config_host_amsdu_th_ini_param();
#endif
#ifdef _PRE_WLAN_TCP_OPT
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER);
    g_st_tcp_ack_filter.uc_tcp_ack_filter_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH);
    g_st_tcp_ack_filter.us_rx_filter_throughput_high = (l_val > 0) ? (uint16_t)l_val : 50; /* 吞吐量50 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW);
    g_st_tcp_ack_filter.us_rx_filter_throughput_low = (l_val > 0) ? (uint16_t)l_val : WLAN_TCP_ACK_FILTER_TH_LOW;
    oal_io_print("tcp ack filter en[%d],high[%d],low[%d]\r\n", g_st_tcp_ack_filter.uc_tcp_ack_filter_en,
        g_st_tcp_ack_filter.us_rx_filter_throughput_high, g_st_tcp_ack_filter.us_rx_filter_throughput_low);
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB);
    g_uc_host_rx_ampdu_amsdu = (l_val > 0) ? (uint8_t)l_val : OAL_FALSE;
    oal_io_print("Rx:ampdu+amsdu skb en[%d]\r\n", g_uc_host_rx_ampdu_amsdu);

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_AMPDU_BITMAP);
    rx_buffer_size->en_rx_ampdu_bitmap_ini = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    rx_buffer_size->us_rx_buffer_size = (uint16_t)l_val;
    oal_io_print("Rx:ampdu bitmap size[%d]\r\n", l_val);
}

static void hwifi_config_host_global_11ax_ini_param(void)
{
    int32_t l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_11AX_SWITCH);
    g_pst_mac_device_capability[0].en_11ax_switch = (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_switch =
        (((uint32_t)l_val & 0x0F) & BIT1) ? OAL_TRUE : OAL_FALSE;

    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_ignore_non_he_cap_from_beacon =
            (((uint32_t)l_val & 0x0F) & BIT2) ? OAL_TRUE : OAL_FALSE;

    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_11ax_aput_he_cap_switch =
        (((uint32_t)l_val & 0x0F) & BIT3) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_responder_support =
        (((uint32_t)l_val & 0xFF) & BIT4) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_requester_support =
        (((uint32_t)l_val & 0xFF) & BIT5) ? OAL_TRUE : OAL_FALSE;
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MULTI_BSSID_SWITCH);
    g_pst_mac_device_capability[0].bit_multi_bssid_switch = (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
#ifdef _PRE_WLAN_FEATURE_11AX
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HTC_SWITCH);
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_htc_include =
        (((uint32_t)l_val & 0x0F) & BIT0) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_om_in_data =
        (((uint32_t)l_val & 0x0F) & BIT1) ? OAL_TRUE : OAL_FALSE;
    g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_rom_cap_switch =
        (((uint32_t)l_val & 0x0F) & BIT2) ? OAL_TRUE : OAL_FALSE;
#endif
}

/*
 * 函 数 名  : hwifi_config_dmac_freq_ini_param
 * 功能描述  : 初始化host全局变量
 * 1.日    期  : 2019年11月22日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_dmac_freq_ini_param(void)
{
    uint32_t cfg_id;
    uint32_t val;
    int32_t l_cfg_value;
    int8_t *pc_tmp;
    host_speed_freq_level_stru ast_host_speed_freq_level_tmp[NUM_4_BITS];
    device_speed_freq_level_stru ast_device_speed_freq_level_tmp[NUM_4_BITS];
    uint8_t uc_flag = OAL_FALSE;
    uint8_t uc_index;
    int32_t l_ret = EOK;

    /******************************************** 自动调频 ********************************************/
    /* config g_host_speed_freq_level */
    pc_tmp = (int8_t *)&ast_host_speed_freq_level_tmp;

    for (cfg_id = WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0; cfg_id <= WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_3; ++cfg_id) {
        val = (uint32_t)hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        *(uint32_t *)pc_tmp = val;
        pc_tmp += BYTE_OFFSET_4;
    }

    /* config g_device_speed_freq_level */
    pc_tmp = (int8_t *)&ast_device_speed_freq_level_tmp;
    for (cfg_id = WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0; cfg_id <= WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3; ++cfg_id) {
        l_cfg_value = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if (oal_value_in_valid_range(l_cfg_value, FREQ_IDLE, FREQ_HIGHEST)) {
            *pc_tmp = l_cfg_value;
            pc_tmp += BIT_OFFSET_4;
        } else {
            uc_flag = OAL_TRUE;
            break;
        }
    }

    if (!uc_flag) {
        l_ret += memcpy_s(&g_host_speed_freq_level, sizeof(g_host_speed_freq_level),
                          &ast_host_speed_freq_level_tmp, sizeof(ast_host_speed_freq_level_tmp));
        l_ret += memcpy_s(&g_device_speed_freq_level, sizeof(g_device_speed_freq_level),
                          &ast_device_speed_freq_level_tmp, sizeof(ast_device_speed_freq_level_tmp));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_config_host_global_ini_param::memcpy fail!");
            return;
        }

        for (uc_index = 0; uc_index < HOST_SPEED_FREQ_LEVEL_BUTT; uc_index++) {
            oam_warning_log4(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::ul_speed_level = %d, \
                min_cpu_freq = %d, min_ddr_freq = %d, uc_device_type = %d}\r\n",
                g_host_speed_freq_level[uc_index].speed_level,
                g_host_speed_freq_level[uc_index].min_cpu_freq,
                g_host_speed_freq_level[uc_index].min_ddr_freq,
                g_device_speed_freq_level[uc_index].uc_device_type);
        }
    }
}

OAL_STATIC void hwifi_config_bypass_extlna_ini_param(void)
{
    int32_t l_val;
    mac_rx_dyn_bypass_extlna_stru *rx_dyn_bypass_extlna_switch = NULL;
    rx_dyn_bypass_extlna_switch = mac_vap_get_rx_dyn_bypass_extlna_switch();

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA);
    rx_dyn_bypass_extlna_switch->uc_ini_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    rx_dyn_bypass_extlna_switch->uc_cur_status = OAL_TRUE; /* 默认低功耗场景 */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH);
    rx_dyn_bypass_extlna_switch->us_throughput_high = (l_val > 0) ? (uint16_t)l_val : WLAN_RX_DYN_BYPASS_EXTLNA_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW);
    rx_dyn_bypass_extlna_switch->us_throughput_low = (l_val > 0) ? (uint16_t)l_val : WLAN_RX_DYN_BYPASS_EXTLNA_LOW;

    oal_io_print("DYN_BYPASS_EXTLNA SWITCH en[%d],high[%d],low[%d]\r\n", rx_dyn_bypass_extlna_switch->uc_ini_en,
        rx_dyn_bypass_extlna_switch->us_throughput_high, rx_dyn_bypass_extlna_switch->us_throughput_low);
}


static void hwifi_config_factory_lte_gpio_ini_param(void)
{
    int32_t val;

    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_LTE_GPIO_CHECK_SWITCH);
    g_wlan_cust.lte_gpio_check_switch = (uint32_t) !!val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_ISM_PRIORITY);
    g_wlan_cust.ism_priority = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_RX);
    g_wlan_cust.lte_rx = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_TX);
    g_wlan_cust.lte_tx = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_INACT);
    g_wlan_cust.lte_inact = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_ISM_RX_ACT);
    g_wlan_cust.ism_rx_act = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_BANT_PRI);
    g_wlan_cust.bant_pri = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_BANT_STATUS);
    g_wlan_cust.bant_status = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_WANT_PRI);
    g_wlan_cust.want_pri = (uint32_t)val;
    val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_WANT_STATUS);
    g_wlan_cust.want_status = (uint32_t)val;
}

/*
 * 函 数 名  : hwifi_config_host_global_hiex_ini_param
 * 功能描述  : 根据ini文件配置dev hiex cap
 * 1.日    期  : 2020年1月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_host_global_ini_param_extend(void)
{
    int32_t l_val;

#ifdef _PRE_WLAN_FEATURE_MBO
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MBO_SWITCH);
    g_uc_mbo_switch = !!l_val;
#endif

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYNAMIC_DBAC_SWITCH);
    g_uc_dbac_dynamic_switch = (uint8_t)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DDR_FREQ);
    g_ddr_freq = (uint32_t)l_val;

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_HIEX_CAP);
        if (memcpy_s(&g_st_default_hiex_cap, sizeof(mac_hiex_cap_stru), &l_val,
            sizeof(l_val)) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_config_host_global_ini_param::hiex cap memcpy fail!");
        }
    }
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FTM_CAP);
        g_mac_ftm_cap = (uint8_t)l_val;
    }
#endif
}

/*
 * 函 数 名  : hwifi_config_host_global_ini_param
 * 功能描述  : 初始化host全局变量
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hwifi_config_host_global_ini_param_mp13(void)
{
    int32_t l_val;

    /******************************************** 漫游 ********************************************/
    hwifi_config_host_roam_global_ini_param();

    /******************************************** 性能 ********************************************/
    wlan_chip_cpu_freq_ini_param_init();

    hwifi_config_tcp_ack_buf_ini_param();

    hwifi_config_dmac_freq_ini_param();

    hwifi_config_bypass_extlna_ini_param();

    hwifi_config_performance_ini_param();

#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    /******************************************** 组播聚合 ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_MCAST_AMPDU_ENABLE);
    g_mcast_ampdu_cfg.mcast_ampdu_enable = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
#endif
    /******************************************** 组播图传 ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_PT_MCAST_ENABLE);
    g_pt_mcast_enable = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    /******************************************** 随机扫描 ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN);
    g_wlan_cust.uc_random_mac_addr_scan = !!l_val;

    /******************************************** CAPABILITY ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40);
    g_wlan_cust.uc_disable_capab_2ght40 = (uint8_t) !!l_val;
    /********************************************factory_lte_gpio_check ********************************************/
    hwifi_config_factory_lte_gpio_ini_param();

    hwifi_config_host_global_ini_param_extend();

    hwifi_set_voe_custom_param();
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hwifi_config_host_global_11ax_ini_param();
    }
#endif
    return;
}

OAL_STATIC uint32_t hwifi_cfg_front_end_set_2g_rf(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_idx; /* 结构体数组下标 */
    int32_t l_mult4;
    int8_t c_mult4_rf[NUM_2_BYTES];
    /* 配置: 2g rf */
    for (uc_idx = 0; uc_idx < MAC_NUM_2G_BAND; ++uc_idx) {
        /* 获取各2p4g 各band 0.25db及0.1db精度的线损值 */
        l_mult4 = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_START + uc_idx);
        /* rf0 */
        c_mult4_rf[0] = (int8_t)cus_get_first_byte(l_mult4);
        /* rf1 */
        c_mult4_rf[1] = (int8_t)cus_get_second_byte(l_mult4);
        if (cus_val_valid(c_mult4_rf[0], RF_LINE_TXRX_GAIN_DB_MAX, RF_LINE_TXRX_GAIN_DB_2G_MIN) &&
            cus_val_valid(c_mult4_rf[1], RF_LINE_TXRX_GAIN_DB_MAX, RF_LINE_TXRX_GAIN_DB_2G_MIN)) {
            pst_customize_rf->ast_rf_gain_db_rf[0].ac_gain_db_2g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[0];
            pst_customize_rf->ast_rf_gain_db_rf[1].ac_gain_db_2g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[1];
        } else {
            /* 值超出有效范围 */
            oam_error_log2(0, OAM_SF_CFG,
                "{hwifi_cfg_front_end_set_2g_rf::ini_id[%d]value out of range, 2g mult4[0x%0x}!}",
                WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND_START + uc_idx, l_mult4);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hwifi_cfg_front_end_set_5g_rf(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_idx; /* 结构体数组下标 */
    int32_t l_mult4;
    int8_t c_mult4_rf[NUM_2_BYTES];
    /* 配置: 5g rf */
    /* 配置: fem口到天线口的负增益 */
    for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; ++uc_idx) {
        /* 获取各5g 各band 0.25db及0.1db精度的线损值 */
        l_mult4 = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_START + uc_idx);
        c_mult4_rf[0] = (int8_t)cus_get_first_byte(l_mult4);
        c_mult4_rf[1] = (int8_t)cus_get_second_byte(l_mult4);
        if (c_mult4_rf[0] <= RF_LINE_TXRX_GAIN_DB_MAX && c_mult4_rf[1] <= RF_LINE_TXRX_GAIN_DB_MAX) {
            pst_customize_rf->ast_rf_gain_db_rf[0].ac_gain_db_5g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[0];
            pst_customize_rf->ast_rf_gain_db_rf[1].ac_gain_db_5g[uc_idx].c_rf_gain_db_mult4 = c_mult4_rf[1];
        } else {
            /* 值超出有效范围 */
            oam_error_log2(0, OAM_SF_CFG,
                "{hwifi_cfg_front_end_set_5g_rf::ini_id[%d]value out of range, 5g mult4[0x%0x}}",
                WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND_START + uc_idx, l_mult4);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}


/*
 * 函 数 名  : hwifi_cfg_front_end_value_range_check
 * 功能描述  : hw 2g 5g 前端值范围检查
 * 1.日    期  : 2019年2月13日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC oal_bool_enum_uint8 hwifi_cfg_front_end_value_range_check(mac_cfg_customize_rf *customize_rf,
    int32_t band, int32_t l_rf_db_min)
{
    return ((customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num == 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num > MAC_EXT_PA_GAIN_MAX_LVL ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db < LNA_GAIN_DB_MIN ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db > LNA_GAIN_DB_MAX) ||
        (customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db < l_rf_db_min ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db > 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num == 0 ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num > MAC_EXT_PA_GAIN_MAX_LVL ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_lna_gain_db < LNA_GAIN_DB_MIN ||
        customize_rf->ast_ext_rf[band][WLAN_RF_CHANNEL_ONE].c_lna_gain_db > LNA_GAIN_DB_MAX)) ? OAL_TRUE : OAL_FALSE;
}

OAL_STATIC void hwifi_cfg_front_end_2g_rf0_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* 2g 外部fem */
    /* RF0 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_pa_isexist =
        (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].uc_ext_lna_isexist =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].us_lna_on2off_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].us_lna_off2on_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ZERO].fem_polar_policy =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FEM_POLAR_POLICY_2G));
}

OAL_STATIC void hwifi_cfg_front_end_2g_rf1_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* RF1 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_pa_isexist =
        (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].uc_ext_lna_isexist =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].us_lna_on2off_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].us_lna_off2on_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].fem_polar_policy =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FEM_POLAR_POLICY_2G));
}

OAL_STATIC uint32_t hwifi_cfg_front_end_2g_fem(mac_cfg_customize_rf *customize_rf)
{
    uint8_t uc_idx;

    /* RF0 */
    hwifi_cfg_front_end_2g_rf0_fem(customize_rf);
    /* RF1 */
    hwifi_cfg_front_end_2g_rf1_fem(customize_rf);

    if (hwifi_cfg_front_end_value_range_check(customize_rf, WLAN_BAND_2G, RF_LINE_TXRX_GAIN_DB_2G_MIN) == OAL_TRUE) {
        /* 值超出有效范围 */
        oam_error_log4(0, OAM_SF_CFG,
            "{hwifi_cfg_front_end_2g_fem:2g gain db out of range! rf0 lna_bypass[%d] pa_b0[%d] lna gain[%d] pa_b1[%d]}",
            customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db,
            customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db,
            customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db,
            customize_rf->ast_ext_rf[WLAN_BAND_2G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db);
        return OAL_FAIL;
    }
    /* 2g定制化RF部分PA偏置寄存器  */
    for (uc_idx = 0; uc_idx < CUS_RF_PA_BIAS_REG_NUM; uc_idx++) {
        customize_rf->aul_2g_pa_bias_rf_reg[uc_idx] =
        (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_TX2G_PA_GATE_VCTL_REG236 + uc_idx);
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : hwifi_cfg_front_end_adjustment_range_check
 * 功能描述  : hw 5g fem前端值范围检查
 * 1.日    期  : 2019年2月13日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC oal_bool_enum_uint8 hwifi_cfg_front_end_adjustment_range_check(int8_t c_delta_cca_ed_high_20th_2g,
    int8_t c_delta_cca_ed_high_40th_2g, int8_t c_delta_cca_ed_high_20th_5g,
    int8_t c_delta_cca_ed_high_40th_5g, int8_t c_delta_cca_ed_high_80th_5g)
{
    return (cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_20th_2g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_40th_2g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_20th_5g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_40th_5g) ||
            cus_delta_cca_ed_high_th_out_of_range(c_delta_cca_ed_high_80th_5g)) ? OAL_TRUE : OAL_FALSE;
}

OAL_STATIC void hwifi_cfg_front_end_5g_rf0_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* 5g 外部fem */
    /* RF0 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_lna_bypass_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_lna_gain_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b0_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_pa_gain_b1_db =
        (int8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_pa_gain_lvl_num =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_pa_isexist =
        (uint8_t) !!(cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) &
        EXT_PA_ISEXIST_5G_MASK);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].en_fem_lp_enable =
        (oal_fem_lp_state_enum_uint8)((cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI,
            WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_LP_STATUS_MASK) >> EXT_FEM_LP_STATUS_OFFSET);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].c_fem_spec_value =
        (int8_t)((cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI,
            WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_FEM_SPEC_MASK) >> EXT_FEM_FEM_SPEC_OFFSET);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].uc_ext_lna_isexist =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].us_lna_on2off_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].us_lna_off2on_time_ns =
        (uint16_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ZERO].fem_polar_policy =
        (uint8_t)cus_get_low_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FEM_POLAR_POLICY_5G));
}
OAL_STATIC void hwifi_cfg_front_end_5g_rf1_fem(mac_cfg_customize_rf *pst_customize_rf)
{
    /* 5g 外部fem */
    /* RF1 */
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db =
        (int8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_pa_gain_lvl_num =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_switch_isexist =
        (uint8_t) !!cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_pa_isexist =
        (uint8_t) !!(cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) &
        EXT_PA_ISEXIST_5G_MASK);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].en_fem_lp_enable =
        (oal_fem_lp_state_enum_uint8)((cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI,
            WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) & EXT_FEM_LP_STATUS_MASK) >> EXT_FEM_LP_STATUS_OFFSET);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_fem_spec_value =
        (int8_t)((cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G)) &
        EXT_FEM_FEM_SPEC_MASK) >> EXT_FEM_FEM_SPEC_OFFSET);
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].uc_ext_lna_isexist =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].us_lna_on2off_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].us_lna_off2on_time_ns =
        (uint16_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G));
    pst_customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].fem_polar_policy =
        (uint8_t)cus_get_high_16bits(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FEM_POLAR_POLICY_5G));
}

OAL_STATIC uint32_t hwifi_cfg_front_end_5g_fem(mac_cfg_customize_rf *customize_rf)
{
    uint8_t uc_idx;
    /* RF0 */
    hwifi_cfg_front_end_5g_rf0_fem(customize_rf);
    /* RF1 */
    hwifi_cfg_front_end_5g_rf1_fem(customize_rf);

    /* 5g upc mix_bf_gain_ctl for P10 */
    for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; uc_idx++) {
        customize_rf->aul_5g_upc_mix_gain_rf_reg[uc_idx] =
            (uint32_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_TX5G_UPC_MIX_GAIN_CTRL_1 + uc_idx);
    }

    if (hwifi_cfg_front_end_value_range_check(customize_rf, WLAN_BAND_5G, RF_LINE_TXRX_GAIN_DB_5G_MIN) == OAL_TRUE) {
        /* 值超出有效范围 */
        oam_error_log4(0, OAM_SF_CFG,
            "{hwifi_cfg_front_end_5g_fem:2g gain db out of range! rf0 lna_bypass[%d] pa_b0[%d] lna gain[%d] pa_b1[%d]}",
            customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_bypass_gain_db,
            customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b0_db,
            customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_lna_gain_db,
            customize_rf->ast_ext_rf[WLAN_BAND_5G][WLAN_RF_CHANNEL_ONE].c_pa_gain_b1_db);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/* 配置: cca能量门限调整值 */
OAL_STATIC void hwifi_set_cca_energy_thrsehold(mac_cfg_customize_rf *pst_customize_rf)
{
    int8_t c_delta_cca_ed_high_20th_2g =
        (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G);
    int8_t c_delta_cca_ed_high_40th_2g =
        (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G);
    int8_t c_delta_cca_ed_high_20th_5g =
        (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G);
    int8_t c_delta_cca_ed_high_40th_5g =
        (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G);
    int8_t c_delta_cca_ed_high_80th_5g =
        (int8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_80TH_5G);
    /* 检查每一项的调整幅度是否超出最大限制 */
    if (hwifi_cfg_front_end_adjustment_range_check(c_delta_cca_ed_high_20th_2g, c_delta_cca_ed_high_40th_2g,
                                                   c_delta_cca_ed_high_20th_5g, c_delta_cca_ed_high_40th_5g,
                                                   c_delta_cca_ed_high_80th_5g) == OAL_TRUE) {
        oam_error_log4(0, OAM_SF_ANY,
            "{hwifi_set_cca_energy_thrsehold::one or more delta cca ed high threshold out of range \
            [delta_20th_2g=%d, delta_40th_2g=%d, delta_20th_5g=%d, delta_40th_5g=%d], please check the value!}",
            c_delta_cca_ed_high_20th_2g, c_delta_cca_ed_high_40th_2g,
            c_delta_cca_ed_high_20th_5g, c_delta_cca_ed_high_40th_5g);
        /* set 0 */
        pst_customize_rf->c_delta_cca_ed_high_20th_2g = 0;
        pst_customize_rf->c_delta_cca_ed_high_40th_2g = 0;
        pst_customize_rf->c_delta_cca_ed_high_20th_5g = 0;
        pst_customize_rf->c_delta_cca_ed_high_40th_5g = 0;
        pst_customize_rf->c_delta_cca_ed_high_80th_5g = 0;
    } else {
        pst_customize_rf->c_delta_cca_ed_high_20th_2g = c_delta_cca_ed_high_20th_2g;
        pst_customize_rf->c_delta_cca_ed_high_40th_2g = c_delta_cca_ed_high_40th_2g;
        pst_customize_rf->c_delta_cca_ed_high_20th_5g = c_delta_cca_ed_high_20th_5g;
        pst_customize_rf->c_delta_cca_ed_high_40th_5g = c_delta_cca_ed_high_40th_5g;
        pst_customize_rf->c_delta_cca_ed_high_80th_5g = c_delta_cca_ed_high_80th_5g;
    }
}
OAL_STATIC void hwifi_set_dyn_extlna_bypass_rssi_thrsehold(mac_cfg_customize_rf *pst_customize_rf)
{
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_2G][WLAN_BANDWIDTH_20] =
        (int8_t)cus_get_first_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_2G][WLAN_BANDWIDTH_40] =
        (int8_t)cus_get_second_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_2G][WLAN_BANDWIDTH_20] =
        (int8_t)cus_get_first_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_LOW_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_2G][WLAN_BANDWIDTH_40] =
        (int8_t)cus_get_second_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_2G_EXTLNA_RSSI_LOW_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_5G][WLAN_BANDWIDTH_20] =
        (int8_t)cus_get_first_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_5G][WLAN_BANDWIDTH_40] =
        (int8_t)cus_get_second_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_5G][WLAN_BANDWIDTH_80] =
        (int8_t)cus_get_third_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_h[WLAN_BAND_5G][WLAN_BANDWIDTH_160] =
        (int8_t)cus_get_fourth_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_HIGH_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_5G][WLAN_BANDWIDTH_20] =
        (int8_t)cus_get_first_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_LOW_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_5G][WLAN_BANDWIDTH_40] =
        (int8_t)cus_get_second_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_LOW_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_5G][WLAN_BANDWIDTH_80] =
        (int8_t)cus_get_third_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_LOW_TH));
    pst_customize_rf->dyn_bypass_extlna_rssi_th_l[WLAN_BAND_5G][WLAN_BANDWIDTH_160] =
        (int8_t)cus_get_fourth_byte(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DYN_BYPASS_5G_EXTLNA_RSSI_LOW_TH));
}
/*
 * 函 数 名  : hwifi_check_pwr_ref_delta
 * 功能描述  : 检查定制化中的delta_rssi值是否超过阈值
 * 1.日    期  : 2018年6月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static int8_t hwifi_check_pwr_ref_delta(int8_t c_pwr_ref_delta)
{
    int8_t c_ret = 0;
    if (c_pwr_ref_delta > WAL_HIPRIV_PWR_REF_DELTA_HI) {
        c_ret = WAL_HIPRIV_PWR_REF_DELTA_HI;
    } else if (c_pwr_ref_delta < WAL_HIPRIV_PWR_REF_DELTA_LO) {
        c_ret = WAL_HIPRIV_PWR_REF_DELTA_LO;
    } else {
        c_ret = c_pwr_ref_delta;
    }

    return c_ret;
}

/*
 * 函 数 名  : hwifi_cfg_pwr_ref_delta
 * 功能描述  : hw 2g 5g 前端
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static void hwifi_cfg_pwr_ref_delta(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_rf_idx;
    wlan_cfg_init cfg_id;
    int32_t l_pwr_ref_delta;
    mac_cfg_custom_delta_pwr_ref_stru *pst_delta_pwr_ref = NULL;
    mac_cfg_custom_amend_rssi_stru *pst_rssi_amend_ref = NULL;
    int8_t *rssi = NULL;

    for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
        pst_delta_pwr_ref = &pst_customize_rf->ast_delta_pwr_ref_cfg[uc_rf_idx];
        /* 2G 20M/40M */
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ?
            WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C0_MULT4 : WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C1_MULT4;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        rssi = pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_2g;
        rssi[0] = hwifi_check_pwr_ref_delta((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        rssi[1] = hwifi_check_pwr_ref_delta((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        /* 5G 20M/40M/80M/160M */
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ?
            WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C0_MULT4 : WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C1_MULT4;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        rssi = pst_delta_pwr_ref->c_cfg_delta_pwr_ref_rssi_5g;
        rssi[0] = hwifi_check_pwr_ref_delta((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        rssi[1] = hwifi_check_pwr_ref_delta((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        rssi[BYTE_OFFSET_2] = hwifi_check_pwr_ref_delta((int8_t)cus_get_third_byte(l_pwr_ref_delta));
        rssi[BYTE_OFFSET_3] = hwifi_check_pwr_ref_delta((int8_t)cus_get_fourth_byte(l_pwr_ref_delta));

        /* RSSI amend */
        pst_rssi_amend_ref = &pst_customize_rf->ast_rssi_amend_cfg[uc_rf_idx];
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ?
            WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C0 : WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        rssi = pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_2g;
        rssi[0] = cus_val_valid((int8_t)cus_get_first_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_first_byte(l_pwr_ref_delta) : 0;
        rssi[1] = cus_val_valid((int8_t)cus_get_second_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_second_byte(l_pwr_ref_delta) : 0;
        rssi[BYTE_OFFSET_2] = cus_val_valid((int8_t)cus_get_third_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_third_byte(l_pwr_ref_delta) : 0;
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ?
            WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C0 : WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        rssi = pst_rssi_amend_ref->ac_cfg_delta_amend_rssi_5g;
        rssi[0] = cus_val_valid((int8_t)cus_get_first_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_first_byte(l_pwr_ref_delta) : 0;
        rssi[1] = cus_val_valid((int8_t)cus_get_second_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_second_byte(l_pwr_ref_delta) : 0;
        rssi[BYTE_OFFSET_2] = cus_val_valid((int8_t)cus_get_third_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_third_byte(l_pwr_ref_delta) : 0;
        rssi[BYTE_OFFSET_3] = cus_val_valid((int8_t)cus_get_fourth_byte(l_pwr_ref_delta), WLAN_RF_RSSI_AMEND_TH_HIGH,
            WLAN_RF_RSSI_AMEND_TH_LOW) ? (int8_t)cus_get_fourth_byte(l_pwr_ref_delta) : 0;
    }
}

/*
 * 功能描述  : 获得有效的定制化rssi修正值，若定制化中的rssi修正值超过阈值，则为0
 * 1.日    期  : 2020年6月20日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static int8_t hwifi_get_valid_amend_rssi_val(int8_t pwr_ref_delta)
{
    int8_t rssi_amend_val;
    rssi_amend_val = cus_val_valid(pwr_ref_delta, WLAN_RF_RSSI_AMEND_TH_HIGH, WLAN_RF_RSSI_AMEND_TH_LOW) ?
        pwr_ref_delta : 0;
    return rssi_amend_val;
}

/*
 * 功能描述  : 滤波器收窄rssi修正定制化
 * 1.日    期  : 2020年6月20日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static void hwifi_cfg_filter_narrow_ref_delta(mac_cfg_customize_rf *pst_customize_rf)
{
    uint8_t uc_rf_idx;
    wlan_cfg_init cfg_id;
    int32_t l_pwr_ref_delta;
    mac_cfg_custom_filter_narrow_amend_rssi_stru *filter_narrow_rssi_amend = NULL;

    for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
        filter_narrow_rssi_amend = &pst_customize_rf->filter_narrow_rssi_amend[uc_rf_idx];
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C0 :
            WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_2g[0] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_2g[1] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        cfg_id = (uc_rf_idx == WLAN_RF_CHANNEL_ZERO) ? WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C0 :
            WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C1;
        l_pwr_ref_delta = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[0] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_first_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[1] =
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_second_byte(l_pwr_ref_delta));
        filter_narrow_rssi_amend->filter_narrowing_amend_rssi_5g[2] = /* rssi修正5G phy mode160M 数组下标2 */
            hwifi_get_valid_amend_rssi_val((int8_t)cus_get_third_byte(l_pwr_ref_delta));
    }
}

OAL_STATIC uint32_t hwifi_cfg_front_end(uint8_t *puc_param)
{
    mac_cfg_customize_rf *pst_customize_rf;

    pst_customize_rf = (mac_cfg_customize_rf *)puc_param;
    memset_s(pst_customize_rf, sizeof(mac_cfg_customize_rf), 0, sizeof(mac_cfg_customize_rf));

    /* 配置: 2g rf */
    if (hwifi_cfg_front_end_set_2g_rf(pst_customize_rf) != OAL_SUCC) {
        return OAL_FAIL;
    }

    hwifi_cfg_pwr_ref_delta(pst_customize_rf);
    hwifi_cfg_filter_narrow_ref_delta(pst_customize_rf);

    /* 通道radio cap */
    pst_customize_rf->uc_chn_radio_cap = (uint8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_CHANN_RADIO_CAP);

    /* 2g 外部fem */
    if (hwifi_cfg_front_end_2g_fem(pst_customize_rf) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (mac_device_check_5g_enable_per_chip() == OAL_TRUE) {
        /* 配置: 5g rf */
        /* 配置: fem口到天线口的负增益 */
        if (hwifi_cfg_front_end_set_5g_rf(pst_customize_rf) != OAL_SUCC) {
            return OAL_FAIL;
        }
        /* 5g 外部fem */
        /* RF0 */
        if (hwifi_cfg_front_end_5g_fem(pst_customize_rf) != OAL_SUCC) {
            return OAL_FAIL;
        }
    }

    pst_customize_rf->uc_far_dist_pow_gain_switch =
        (uint8_t) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH);
    pst_customize_rf->uc_far_dist_dsss_scale_promote_switch =
        (uint8_t) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH);

    /* 配置: cca能量门限调整值 */
    hwifi_set_cca_energy_thrsehold(pst_customize_rf);
    hwifi_set_dyn_extlna_bypass_rssi_thrsehold(pst_customize_rf);

    return OAL_SUCC;
}

/*
 * 函 数 名  : hwifi_config_init_ini_rf
 * 功能描述  : hw 2g 5g 前端定制化
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_init_ini_rf(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ret;
    uint16_t us_event_len = sizeof(mac_cfg_customize_rf);

    wal_write_msg_hdr_init(&st_write_msg, WLAN_CFGID_SET_CUS_RF, us_event_len);

    /* 定制化下发不能超过事件内存长 */
    if (us_event_len > WAL_MSG_WRITE_MAX_LEN) {
        oam_error_log2(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::event size[%d] larger than msg size[%d]!}",
                       us_event_len, WAL_MSG_WRITE_MAX_LEN);
        return;
    }

    ret = hwifi_cfg_front_end(st_write_msg.auc_value);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::front end rf wrong value, not send cfg!}");
        return;
    }

    /* 如果所有参数都在有效范围内，则下发配置值 */
    ret = (uint32_t)wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + \
        us_event_len, (uint8_t *)&st_write_msg,  OAL_FALSE, NULL);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::EVENT[wal_send_cfg_event] fail[%d]!}", ret);
    }
}


/*
 * 函 数 名  : hwifi_config_init_ini_Hisilicon_log
 * 功能描述  : 日志相关丁志远
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_init_ini_log(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    int32_t l_loglevel;

    /* log_level */
    l_loglevel = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LOGLEVEL);
    if (l_loglevel < OAM_LOG_LEVEL_ERROR ||
        l_loglevel > OAM_LOG_LEVEL_INFO) {
        oam_error_log3(0, OAM_SF_ANY, "{hwifi_config_init_ini_clock::loglevel[%d] out of range[%d,%d], check ini file}",
                       l_loglevel, OAM_LOG_LEVEL_ERROR, OAM_LOG_LEVEL_INFO);
        return;
    }

    wal_write_msg_hdr_init(&st_write_msg, WLAN_CFGID_SET_ALL_LOG_LEVEL, sizeof(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_loglevel;
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(int32_t),
                               (uint8_t *)&st_write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hwifi_config_init_ini_log::return err code[%d]!}\r\n", l_ret);
    }
}

/*
 * 函 数 名  : hwifi_config_init_ini_Hisilicon
 * 功能描述  : 配置vap定制化
 *            不涉及wlan p2p网络设备的，均由配置vap配置host参数或下发至device
 * 1.日    期  : 2015年10月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hwifi_config_init_ini_main_mp13(oal_net_device_stru *pst_cfg_net_dev)
{
    /* 国家码 */
    hwifi_config_init_ini_country(pst_cfg_net_dev);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    /* 自学习国家码初始化 */
    hwifi_config_selfstudy_init_country(pst_cfg_net_dev);
#endif
    /* 可维可测 */
    hwifi_config_init_ini_log(pst_cfg_net_dev);
    /* RF */
    hwifi_config_init_ini_rf(pst_cfg_net_dev);
}

void wal_send_cali_data_mp13(oal_net_device_stru *cfg_net_dev)
{
    hmac_send_cali_data_mp13(oal_net_dev_priv(cfg_net_dev), WLAN_SCAN_ALL_CHN);
}

void wal_send_cali_data_mp15(oal_net_device_stru *cfg_net_dev)
{
    hmac_send_cali_data_mp15(oal_net_dev_priv(cfg_net_dev), WLAN_SCAN_ALL_CHN);
}

/*
 * 函 数 名  : wal_custom_cali
 * 功能描述  : 下发定制化信息，初始化射频，校准
 * 1.日    期  : 2015年9月18日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_custom_cali_mp13(void)
{
    oal_net_device_stru *pst_net_dev;
    uint32_t ret;

    pst_net_dev = wal_config_get_netdev("Hisilicon0", OAL_STRLEN("Hisilicon0"));  // 通过cfg vap0来下c0 c1校准
#ifdef _PRE_DUAL_CHIP
    if (hcc_get_main_dev() == HCC_EP_WIFI_DEV1) {
        pst_net_dev = wal_config_get_netdev("Hisilicon1", OAL_STRLEN("Hisilicon1"));
    }
#endif
    if (oal_warn_on(pst_net_dev == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_custom_cali::the net_device is not exist!}");
        return OAL_ERR_CODE_PTR_NULL;
    } else {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(pst_net_dev);
    }

    if (hwifi_config_init_nvram_main_mp13(pst_net_dev) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_custom_cali::init_nvram fail!}");
    }

    hwifi_config_init_ini_main_mp13(pst_net_dev);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_DUAL_CHIP
    if (((hcc_get_main_dev() == HCC_EP_WIFI_DEV) && (g_custom_cali_done == OAL_TRUE)) ||
        ((hcc_get_main_dev() == HCC_EP_WIFI_DEV1) && (g_custom_cali_done_mpxx_dev1 == OAL_TRUE))) {
        wlan_chip_send_cali_data(pst_net_dev);
    } else {
        if (hcc_get_main_dev() == HCC_EP_WIFI_DEV) {
            g_custom_cali_done = OAL_TRUE;
        } else {
            g_custom_cali_done_mpxx_dev1 = OAL_TRUE;
        }
    }
#else
    if (g_custom_cali_done == OAL_TRUE) {
        /* 校准数据下发 */
        wlan_chip_send_cali_data(pst_net_dev);
    } else {
        g_custom_cali_done = OAL_TRUE;
    }
#endif
    wal_send_cali_matrix_data(pst_net_dev);
#endif
    /* 下发参数 */
    ret = hwifi_config_init_dts_main(pst_net_dev);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_custom_cali:init_dts_main fail!}");
    }
    return ret;
}

/*
 * 函 数 名  : hwifi_get_sar_ctrl_params_mp13
 * 功能描述  : 获取定制中降sar的当前档位的功率值
 * 1.日    期  : 2015年1月20日
 *   修改内容  : 新生成函数
 */
uint32_t hwifi_get_sar_ctrl_params_mp13(uint8_t lvl_num_temp, uint8_t *data_addr,
    uint16_t *data_len, uint16_t dest_len)
{
    uint8_t lvl_num = lvl_num_temp;
    wlan_init_cust_nvram_params *cust_nv_params = (wlan_init_cust_nvram_params *)hwifi_get_init_nvram_params();
    *data_len = sizeof(wlan_cust_sar_ctrl_stru) * CUS_NUM_OF_SAR_PARAMS;
    if ((lvl_num <= CUS_NUM_OF_SAR_LVL) && (lvl_num > 0)) {
        lvl_num--;
        if (memcpy_s(data_addr, dest_len, cust_nv_params->st_sar_ctrl_params[lvl_num], *data_len) != EOK) {
            oam_error_log0(0, OAM_SF_CFG, "hwifi_get_sar_ctrl_param::memcpy fail!");
            return OAL_FAIL;
        }
    } else {
        memset_s(data_addr, dest_len, 0xFF, dest_len);
    }
    return OAL_SUCC;
}

#ifdef _PRE_DUAL_CHIP
/* dual chip function */
int32_t *hwifi_get_g_host_init_params_dev1(void)
{
    return (int32_t *)g_host_init_params_dev1;
}

int32_t *hwifi_get_g_dts_params_dev1(void)
{
    return (int32_t *)g_dts_params_dev1;
}

OAL_STATIC void original_value_for_dts_params_dev1(void)
{
    /* 校准 */
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN1] = WLAN_CALI_TXPWR_REF_2G_CH1_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN2] = WLAN_CALI_TXPWR_REF_2G_CH2_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN3] = WLAN_CALI_TXPWR_REF_2G_CH3_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN4] = WLAN_CALI_TXPWR_REF_2G_CH4_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN5] = WLAN_CALI_TXPWR_REF_2G_CH5_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN6] = WLAN_CALI_TXPWR_REF_2G_CH6_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN7] = WLAN_CALI_TXPWR_REF_2G_CH7_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN8] = WLAN_CALI_TXPWR_REF_2G_CH8_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN9] = WLAN_CALI_TXPWR_REF_2G_CH9_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN10] = WLAN_CALI_TXPWR_REF_2G_CH10_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN11] = WLAN_CALI_TXPWR_REF_2G_CH11_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN12] = WLAN_CALI_TXPWR_REF_2G_CH12_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN13] = WLAN_CALI_TXPWR_REF_2G_CH13_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND1] = WLAN_CALI_TXPWR_REF_5G_BAND1_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND2] = WLAN_CALI_TXPWR_REF_5G_BAND2_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND3] = WLAN_CALI_TXPWR_REF_5G_BAND3_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND4] = WLAN_CALI_TXPWR_REF_5G_BAND4_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND5] = WLAN_CALI_TXPWR_REF_5G_BAND5_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND6] = WLAN_CALI_TXPWR_REF_5G_BAND6_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND7] = WLAN_CALI_TXPWR_REF_5G_BAND7_VAL;
    g_dts_params_dev1[WLAN_CFG_DTS_CALI_TONE_AMP_GRADE] = WLAN_CALI_TONE_GRADE_AMP;
    /* DPD校准定制化 */
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_CH_CORE0] = 0x641DA71,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE0] = 0x11110000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE0] = 0x33222,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE0] = 0x22211000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE0] = 0x2,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE0] = 0x00000000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE0] = 0x00000000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_CH_CORE1] = 0x641DA71,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE1] = 0x11110000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE1] = 0x33222,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE1] = 0x22211000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE1] = 0x2,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE1] = 0x00000000,
    g_dts_params_dev1[WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE1] = 0x00000000,
    g_dts_params_dev1[WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL] = 0x0;
}

OAL_STATIC int32_t hwifi_config_init_private_custom_dev1(void)
{
    int32_t l_cfg_id;
    int32_t l_ret;

    for (l_cfg_id = 0; l_cfg_id < WLAN_CFG_PRIV_BUTT; l_cfg_id++) {
        /* 获取 private 的配置值 */
        l_ret = hwifi_get_wifi_config_priv_val(l_cfg_id, &(g_priv_cust_params_dev1[l_cfg_id].l_val));
        if (l_ret == INI_FAILED) {
            g_priv_cust_params_dev1[l_cfg_id].en_value_state = OAL_FALSE;
            continue;
        }
        g_priv_cust_params_dev1[l_cfg_id].en_value_state = OAL_TRUE;
    }

    oam_warning_log0(0, OAM_SF_CFG, "hwifi_config_init_private_custom_dev1 read from ini success!");

    return INI_SUCC;
}

int32_t hwifi_get_init_priv_value_dev1(int32_t l_cfg_id, int32_t *pl_priv_value)
{
    if (l_cfg_id < 0 || l_cfg_id >= WLAN_CFG_PRIV_BUTT) {
        oam_error_log2(0, OAM_SF_ANY, "hwifi_get_init_priv_value_dev1 cfg id[%d] out of range, max[%d]",
                       l_cfg_id, WLAN_CFG_PRIV_BUTT - 1);
        return OAL_FAIL;
    }

    if (g_priv_cust_params_dev1[l_cfg_id].en_value_state == OAL_FALSE) {
        return OAL_FAIL;
    }

    *pl_priv_value = g_priv_cust_params_dev1[l_cfg_id].l_val;

    return OAL_SUCC;
}

void hwifi_set_g_fact_cali_completed_dev1(oal_bool_enum_uint8 val)
{
    g_en_fact_cali_completed_dev1 = val;
}

oal_bool_enum_uint8 hwifi_get_g_fact_cali_completed_dev1(void)
{
    return g_en_fact_cali_completed_dev1;
}

OAL_STATIC uint8_t hwifi_config_set_cali_param_2g_and_5g_dev1(int32_t *pl_params, int16_t *s_5g_delt_power)
{
    uint8_t rf_idx;
    uint8_t cali_param_idx;
    uint8_t delt_pwr_idx = 0;
    uint8_t idx = 0;
    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        for (cali_param_idx = 0; cali_param_idx < DY_CALI_PARAMS_BASE_NUM; cali_param_idx++) {
            if (cali_param_idx == (DY_2G_CALI_PARAMS_NUM - 1)) {
                /* band1 & CW */
                cali_param_idx += PRO_LINE_2G_TO_5G_OFFSET;
            }
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par2 = pl_params[idx++];
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par1 = pl_params[idx++];
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par0 = pl_params[idx++];
        }
    }

    /* 5g band2&3 4&5 6 7 low power */
    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        delt_pwr_idx = 0;
        for (cali_param_idx = DY_CALI_PARAMS_BASE_NUM + 1;
             cali_param_idx < DY_CALI_PARAMS_NUM - 1; cali_param_idx++) {
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par2 = pl_params[idx++];
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par1 = pl_params[idx++];
            g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par0 = pl_params[idx++];

            cus_flush_nv_ratio_by_delt_pow(&g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par0,
                                           &g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par1,
                                           g_pro_line_params_dev1[rf_idx][cali_param_idx].l_pow_par2,
                                           *(s_5g_delt_power + rf_idx * CUS_NUM_5G_BW + delt_pwr_idx));
            delt_pwr_idx++;
        }
    }
    return idx;
}

OAL_STATIC void hwifi_config_set_cali_param_left_num_dev1(int32_t *pl_params, uint8_t idx)
{
    uint8_t rf_idx;
    /* band1 & CW */
    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par2 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par1 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par0 = pl_params[idx++];
    }
    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par2 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par1 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par0 = pl_params[idx++];
    }
    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        /* 5g band1 low power */
        /* band1产线不校准 */
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par2 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par1 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par0 = pl_params[idx++];
    }

    for (rf_idx = 0; rf_idx < WLAN_RF_CHANNEL_NUMS; rf_idx++) {
        /* 2g cw ppa */
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par2 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par1 = pl_params[idx++];
        g_pro_line_params_dev1[rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par0 = pl_params[idx++];
    }
}

OAL_STATIC int32_t hwifi_config_get_cust_string(uint32_t cfg_id, uint8_t *nv_pa_params)
{
    uint32_t cfg_id_tmp;
    if (hwifi_get_nvram_pro_line_config_ini_name(cfg_id, nv_pa_params) == INI_FAILED) {
        if (oal_value_eq_any4(cfg_id, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW,
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW,
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW,
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1_LOW)) {
            cfg_id_tmp = ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW) ?
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1 :
                              (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) ?
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0 :
                              (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1_LOW) ?
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1 :
                              (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) ?
                              WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1 : cfg_id);
            if (hwifi_get_nvram_pro_line_config_ini_name(cfg_id_tmp, nv_pa_params) == INI_FAILED) {
                oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_get_cust_string get pro line fail! id[%d]", cfg_id_tmp);
            }
        } else {
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_get_cust_string read, check id[%d] exists!", cfg_id);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC void hwifi_config_get_5g_curv_switch_point(uint8_t *puc_ini_pa_params, uint32_t cfg_id)
{
    int32_t l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    uint16_t us_ini_param_num = 0;
    uint8_t uc_secon_ratio_idx = 0;
    uint8_t uc_param_idx;
    uint8_t uc_chain_idx;
    int16_t *ps_extre_point_val = NULL;

    if ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) || (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1)) {
        uc_chain_idx = WLAN_RF_CHANNEL_ZERO;
    } else if ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1) || (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1)) {
        uc_chain_idx = WLAN_RF_CHANNEL_ONE;
    } else {
        return;
    }

    /* 获取拟合系数项 */
    if (hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params, &us_ini_param_num,
                                                 sizeof(l_ini_params) / sizeof(int32_t)) != OAL_SUCC ||
        (us_ini_param_num % DY_CALI_PARAMS_TIMES)) {
        oam_error_log2(0, OAM_SF_CUSTOM,
                       "hwifi_config_get_5g_curv_switch_point::ini is unsuitable,num of ini[%d] cfg_id[%d]!",
                       us_ini_param_num, cfg_id);
        return;
    }

    ps_extre_point_val = g_gs_extre_point_vals[uc_chain_idx];
    us_ini_param_num /= DY_CALI_PARAMS_TIMES;
    if (cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1) {
        if (us_ini_param_num != CUS_NUM_5G_BW) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]", cfg_id, us_ini_param_num);
            return;
        }
        ps_extre_point_val++;
    } else {
        if (us_ini_param_num != 1) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]", cfg_id, us_ini_param_num);
            return;
        }
    }

    /* 计算5g曲线switch point */
    for (uc_param_idx = 0; uc_param_idx < us_ini_param_num; uc_param_idx++) {
        *(ps_extre_point_val + uc_param_idx) = (int16_t)hwifi_dyn_cali_get_extre_point(l_ini_params +
                                               uc_secon_ratio_idx);
        oal_io_print("hwifi_config_get_5g_curv_switch_point::extre power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                     *(ps_extre_point_val + uc_param_idx), uc_param_idx, cfg_id);
        oal_io_print("hwifi_config_get_5g_curv_switch_point::param[%d %d] uc_secon_ratio_idx[%d]!\r\n",
            (l_ini_params + uc_secon_ratio_idx)[0], (l_ini_params + uc_secon_ratio_idx)[1], uc_secon_ratio_idx);
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }

    return;
}

OAL_STATIC uint32_t hwifi_config_nvram_second_coefficient_check(uint8_t *puc_g_cust_nvram_info,
                                                                uint8_t *puc_ini_pa_params,
                                                                uint32_t cfg_id,
                                                                int16_t *ps_5g_delt_power)
{
    int32_t l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    int32_t l_nv_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    uint16_t us_ini_param_num = 0;
    uint16_t us_nv_param_num = 0;
    uint8_t uc_secon_ratio_idx = 0;
    uint8_t uc_param_idx;

    /* 获取拟合系数项 */
    if (hwifi_config_sepa_coefficient_from_param(puc_g_cust_nvram_info, l_nv_params, &us_nv_param_num,
                                                 sizeof(l_nv_params) / (sizeof(int16_t))) != OAL_SUCC ||
        (us_nv_param_num % DY_CALI_PARAMS_TIMES) ||
        hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params, &us_ini_param_num,
                                                 sizeof(l_ini_params) / (sizeof(int16_t))) != OAL_SUCC ||
        (us_ini_param_num % DY_CALI_PARAMS_TIMES) || (us_nv_param_num != us_ini_param_num)) {
        oam_error_log2(0, OAM_SF_CUSTOM,
            "hwifi_config_nvram_second_coefficient_check::nvram or ini is unsuitable,num of nv and ini[%d %d]!",
            us_nv_param_num, us_ini_param_num);
        return OAL_FAIL;
    }

    us_nv_param_num /= DY_CALI_PARAMS_TIMES;
    /* 检查nv和ini中二次系数是否匹配 */
    for (uc_param_idx = 0; uc_param_idx < us_nv_param_num; uc_param_idx++) {
        if (uc_secon_ratio_idx >= (CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES)) {
            continue;
        }
        if (l_ini_params[uc_secon_ratio_idx] != l_nv_params[uc_secon_ratio_idx]) {
            oam_warning_log4(0, OAM_SF_CUSTOM, "hwifi_config_nvram_second_coefficient_check::nvram get mismatch value \
                idx[%d %d] val are [%d] and [%d]!", uc_param_idx, uc_secon_ratio_idx, l_ini_params[uc_secon_ratio_idx],
                l_nv_params[uc_secon_ratio_idx]);

            /* 量产后二次系数以nvram中为准，刷新NV中的二次拟合曲线切换点 */
            hwifi_config_get_5g_curv_switch_point(puc_g_cust_nvram_info, cfg_id);
            uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
            continue;
        }

        if ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) || (cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1)) {
            /* 计算产线上的delt power */
            *(ps_5g_delt_power + uc_param_idx) = hwifi_get_5g_pro_line_delt_pow_per_band(
                l_nv_params + (int32_t)uc_secon_ratio_idx, l_ini_params + (int32_t)uc_secon_ratio_idx);
            oal_io_print("hwifi_config_nvram_second_coefficient_check::delt power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                         *(ps_5g_delt_power + uc_param_idx), uc_param_idx, cfg_id);
        }
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hwifi_nvram_param_check_second_coefficient(uint8_t *nv_pa_params, uint8_t *cust_nvram_info,
                                                               int16_t *delt_power_5g, uint32_t cfg_id)
{
    /* 先取nv中的参数值,为空则从ini文件中读取 */
    if (OAL_STRLEN(cust_nvram_info)) {
        /* NVRAM二次系数异常保护 */
        if (hwifi_config_nvram_second_coefficient_check(cust_nvram_info, nv_pa_params, cfg_id, delt_power_5g +
            (cfg_id < WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA1 ? WLAN_RF_CHANNEL_ZERO : WLAN_RF_CHANNEL_ONE)) == OAL_SUCC) {
            /* 手机如果low part为空,则取ini中的系数,并根据产测结果修正;否则直接从nvram中取得 */
            if ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) &&
                (oal_memcmp(cust_nvram_info, nv_pa_params, OAL_STRLEN(cust_nvram_info)))) {
                memset_s(delt_power_5g + WLAN_RF_CHANNEL_ZERO, CUS_NUM_5G_BW * sizeof(int16_t),
                         0, CUS_NUM_5G_BW * sizeof(int16_t));
            }
            if ((cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) &&
                (oal_memcmp(cust_nvram_info, nv_pa_params, OAL_STRLEN(cust_nvram_info)))) {
                memset_s(delt_power_5g + WLAN_RF_CHANNEL_ONE, CUS_NUM_5G_BW * sizeof(int16_t),
                         0, CUS_NUM_5G_BW * sizeof(int16_t));
            }

            if (memcpy_s(nv_pa_params, CUS_PARAMS_LEN_MAX * sizeof(uint8_t),
                         cust_nvram_info, OAL_STRLEN(cust_nvram_info)) != EOK) {
                return OAL_FAIL;
            }
        } else {
            return OAL_FAIL;
        }
    } else {
        /* 提供产线第一次上电校准初始值 */
        if (memcpy_s(cust_nvram_info, CUS_PARAMS_LEN_MAX,
                     nv_pa_params, OAL_STRLEN(nv_pa_params)) != EOK) {
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hwifi_coefficient_check_and_set(uint8_t **cust_nvram_info, uint8_t *nv_pa_params,
                                                    int32_t *params, int16_t *delt_power_5g,
                                                    uint16_t *param_num)
{
    uint32_t cfg_id;
    uint16_t per_param_num = 0;
    uint16_t param_len = WLAN_RF_CHANNEL_NUMS * DY_CALI_PARAMS_TIMES * DY_CALI_PARAMS_NUM * sizeof(int32_t);
    for (cfg_id = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0; cfg_id < WLAN_CFG_DTS_NVRAM_PARAMS_BUTT; cfg_id++) {
        /* 二次拟合系数 */
        if ((cfg_id >= WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0) && (cfg_id < WLAN_CFG_DTS_NVRAM_END)) {
            /* DPN */
            continue;
        }

        if (hwifi_config_get_cust_string(cfg_id, nv_pa_params) != OAL_SUCC) {
            return OAL_FAIL;
        }

        /* 获取ini中的二次拟合曲线切换点 */
        hwifi_config_get_5g_curv_switch_point(nv_pa_params, cfg_id);

        if (cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) {
            *cust_nvram_info = hwifi_get_nvram_param(cfg_id);
            if (hwifi_nvram_param_check_second_coefficient(nv_pa_params, *cust_nvram_info,
                                                           delt_power_5g, cfg_id) != OAL_SUCC) {
                return OAL_FAIL;
            }
        }

        if (hwifi_config_sepa_coefficient_from_param(nv_pa_params, params + *param_num,
                                                     &per_param_num, param_len - *param_num) != OAL_SUCC ||
                                                     (per_param_num % DY_CALI_PARAMS_TIMES)) {
            oam_error_log3(0, OAM_SF_CUSTOM,
                "hwifi_coefficient_check_and_set read get wrong value,len[%d] check id[%d] exists per_param_num[%d]!",
                OAL_STRLEN(*cust_nvram_info), cfg_id, per_param_num);
            return OAL_FAIL;
        }
        *param_num += per_param_num;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hwifi_config_init_dy_cali_custom_dev1(void)
{
    uint32_t ret;
    uint8_t uc_idx = 0;
    uint16_t us_param_num = 0;
    int16_t s_5g_delt_power[WLAN_RF_CHANNEL_NUMS][CUS_NUM_5G_BW] = {{0}};
    uint8_t *puc_g_cust_nvram_info = NULL;
    uint8_t *puc_nv_pa_params = NULL;
    int32_t *pl_params = NULL;
    uint16_t us_param_len = WLAN_RF_CHANNEL_NUMS * DY_CALI_PARAMS_TIMES * DY_CALI_PARAMS_NUM * sizeof(int32_t);

    puc_nv_pa_params = (uint8_t *)os_kzalloc_gfp(CUS_PARAMS_LEN_MAX * sizeof(uint8_t));
    if (puc_nv_pa_params == NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom_dev1::puc_nv_pa_params mem alloc fail!");
        return OAL_FAIL;
    }

    pl_params = (int32_t *)os_kzalloc_gfp(us_param_len);
    if (pl_params == NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom_dev1::ps_params mem alloc fail!");
        os_mem_kfree(puc_nv_pa_params);
        return OAL_FAIL;
    }

    memset_s(puc_nv_pa_params, CUS_PARAMS_LEN_MAX * sizeof(uint8_t), 0, CUS_PARAMS_LEN_MAX * sizeof(uint8_t));
    memset_s(pl_params, us_param_len, 0, us_param_len);

    ret = hwifi_coefficient_check_and_set(&puc_g_cust_nvram_info, puc_nv_pa_params, pl_params,
                                          (int16_t *)s_5g_delt_power, &us_param_num);

    os_mem_kfree(puc_nv_pa_params);

    if (ret == OAL_FAIL) {
        /* 置零防止下发到device */
        memset_s(g_pro_line_params_dev1, sizeof(g_pro_line_params_dev1), 0, sizeof(g_pro_line_params_dev1));
    } else {
        if (us_param_num != us_param_len / sizeof(int32_t)) {
            oam_error_log1(0, OAM_SF_CUSTOM,
                           "hwifi_config_init_dy_cali_custom_dev1 read get wrong ini value num[%d]!", us_param_num);
            memset_s(g_pro_line_params_dev1, sizeof(g_pro_line_params_dev1), 0, sizeof(g_pro_line_params_dev1));
            os_mem_kfree(pl_params);
            return OAL_FAIL;
        }

        uc_idx = hwifi_config_set_cali_param_2g_and_5g_dev1(pl_params, (int16_t *)s_5g_delt_power);
        hwifi_config_set_cali_param_left_num_dev1(pl_params, uc_idx);
    }

    os_mem_kfree(pl_params);
    return ret;
}

OAL_STATIC uint32_t hwifi_cfg_init_cus_5g_160m_dpn_cali(mac_cus_dy_cali_param_stru *dyn_cali_param,
    int8_t *pc_ctx, uint8_t rf_idx)
{
    uint8_t  uc_dpn_5g_nv_id = WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C0;
    uint8_t  nv_pa_params[CUS_PARAMS_LEN_MAX] = { 0 };
    int8_t   dpn_5g_nv[OAL_5G_160M_CHANNEL_NUM];
    uint8_t *pc_end = ";";
    uint8_t *pc_sep = ",";
    int8_t  *pc_token = NULL;
    uint8_t *cust_nvram_info = NULL;
    uint8_t  num_idx;
    int32_t  val;

    cust_nvram_info = hwifi_get_nvram_param(uc_dpn_5g_nv_id);
    uc_dpn_5g_nv_id++;
    if (OAL_STRLEN(cust_nvram_info)) {
        memset_s(nv_pa_params, sizeof(nv_pa_params), 0, sizeof(nv_pa_params));
        if (memcpy_s(nv_pa_params, sizeof(nv_pa_params),
                     cust_nvram_info, OAL_STRLEN(cust_nvram_info)) != EOK) {
            oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_cfg_init_cus_5g_160m_dpn_cali::memcpy fail!");
        }
        pc_token = oal_strtok(nv_pa_params, pc_end, &pc_ctx);
        pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
        num_idx = 0;
        while ((pc_token != NULL)) {
            if (num_idx >= OAL_5G_160M_CHANNEL_NUM) {
                num_idx++;
                break;
            }
            val = simple_strtol(pc_token, NULL, 10) / 10; /* 10表示十进制 */
            pc_token = oal_strtok(NULL, pc_sep, &pc_ctx);
            if (oal_value_not_in_valid_range(val, CUS_DY_CALI_5G_VAL_DPN_MIN, CUS_DY_CALI_5G_VAL_DPN_MAX)) {
                oam_error_log3(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn val[%d]\
                    unexpect:idx[%d] num_idx[%d}}", val, MAC_NUM_5G_BAND, num_idx);
                val = 0;
            }
            dpn_5g_nv[num_idx] = (int8_t)val;
            num_idx++;
        }

        if (num_idx != OAL_5G_160M_CHANNEL_NUM) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn num unexpected id[%d] rf[%d}}", MAC_NUM_5G_BAND, rf_idx);
            return OAL_FAIL;
        }
        /* 5250  5570 */
        for (num_idx = 0; num_idx < OAL_5G_160M_CHANNEL_NUM; num_idx++) {
            dyn_cali_param->ac_dy_cali_5g_dpn_params[num_idx + 1][WLAN_BW_CAP_160M] += dpn_5g_nv[num_idx];
        }
    }
    return OAL_SUCC;
}

OAL_STATIC void hwifi_cfg_init_cus_dpn_cali(mac_cus_dy_cali_param_stru *dyn_cali_param, int8_t *pc_ctx, uint8_t rf_idx)
{
    uint8_t  idx, num_idx;
    int32_t  val;
    uint8_t *cust_nvram_info = NULL;
    uint8_t  dpn_2g_nv_id = WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0;
    uint8_t  nv_pa_params[CUS_PARAMS_LEN_MAX] = { 0 };
    int8_t   ac_dpn_nv[HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_CW][MAC_2G_CHANNEL_NUM];
    uint8_t *pc_end = ";";
    uint8_t *pc_sep = ",";
    int8_t  *pc_token = NULL;

    for (idx = HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_11B;
        idx <= HWIFI_CFG_DYN_PWR_CALI_2G_SNGL_MODE_OFDM40; idx++) {
        /* 获取产线计算DPN值修正 */
        cust_nvram_info = hwifi_get_nvram_param(dpn_2g_nv_id);
        dpn_2g_nv_id++;

        if (OAL_STRLEN(cust_nvram_info) == 0) {
            continue;
        }

        memset_s(nv_pa_params, sizeof(nv_pa_params), 0, sizeof(nv_pa_params));
        if (memcpy_s(nv_pa_params, sizeof(nv_pa_params), cust_nvram_info, OAL_STRLEN(cust_nvram_info)) != EOK) {
            oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_cfg_init_cus_dpn_cali::memcpy fail!");
        }
        pc_token = oal_strtok(nv_pa_params, pc_end, &pc_ctx);
        pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
        num_idx = 0;
        while ((pc_token != NULL)) {
            if (num_idx >= MAC_2G_CHANNEL_NUM) {
                num_idx++;
                break;
            }
            val = simple_strtol(pc_token, NULL, 10) / 10; /* 10表示十进制 */
            pc_token = oal_strtok(NULL, pc_sep, &pc_ctx);
            if (oal_value_not_in_valid_range(val, CUS_DY_CALI_2G_VAL_DPN_MIN, CUS_DY_CALI_2G_VAL_DPN_MAX)) {
                oam_error_log3(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn val[%d]\
                    unexpected idx[%d] num_idx[%d}!}", val, idx, num_idx);
                val = 0;
            }
            ac_dpn_nv[idx][num_idx] = (int8_t)val;
            num_idx++;
        }

        if (num_idx != MAC_2G_CHANNEL_NUM) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                "{hwifi_cfg_init_cus_dyn_cali::nvram 2g dpn num is unexpect uc_id[%d] rf[%d}}", idx, rf_idx);
            continue;
        }

        for (num_idx = 0; num_idx < MAC_2G_CHANNEL_NUM; num_idx++) {
            dyn_cali_param->ac_dy_cali_2g_dpn_params[num_idx][idx] += ac_dpn_nv[idx][num_idx];
        }
    }
}

uint32_t hwifi_cfg_init_cus_dyn_cali_dev1(mac_cus_dy_cali_param_stru *puc_dyn_cali_param, int num)
{
    int32_t  val, ret = 0;
    uint8_t  uc_idx = 0;
    uint8_t  uc_rf_idx, uc_dy_cal_param_idx;
    uint8_t  uc_cfg_id = WLAN_CFG_DTS_2G_CORE0_DPN_CH1;
    int8_t  *pc_ctx = NULL;

    for (uc_rf_idx = 0; uc_rf_idx < num; uc_rf_idx++) {
        puc_dyn_cali_param->uc_rf_id = uc_rf_idx;

        /* 动态校准二次项系数入参检查 */
        for (uc_dy_cal_param_idx = 0; uc_dy_cal_param_idx < DY_CALI_PARAMS_NUM; uc_dy_cal_param_idx++) {
            if (!g_pro_line_params_dev1[uc_rf_idx][uc_dy_cal_param_idx].l_pow_par2) {
                oam_error_log1(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali_dev1::unexpect val[%d] s_pow_par2[0]!}",
                    uc_dy_cal_param_idx);
                return OAL_FAIL;
            }
        }
        ret = memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_params,
                       sizeof(puc_dyn_cali_param->al_dy_cali_base_ratio_params),
                       g_pro_line_params_dev1[uc_rf_idx], sizeof(puc_dyn_cali_param->al_dy_cali_base_ratio_params));

        ret += memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params,
                        sizeof(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params),
                        &g_pro_line_params_dev1[uc_rf_idx][CUS_DY_CALI_PARAMS_NUM],
                        sizeof(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params));

        ret += memcpy_s(puc_dyn_cali_param->as_extre_point_val, sizeof(puc_dyn_cali_param->as_extre_point_val),
                        g_gs_extre_point_vals[uc_rf_idx], sizeof(puc_dyn_cali_param->as_extre_point_val));

        /* DPN */
        for (uc_idx = 0; uc_idx < MAC_2G_CHANNEL_NUM; uc_idx++) {
            val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
            ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_2g_dpn_params[uc_idx],
                CUS_DY_CALI_DPN_PARAMS_NUM * sizeof(int8_t), &val, CUS_DY_CALI_DPN_PARAMS_NUM * sizeof(int8_t));
        }
        uc_cfg_id += MAC_2G_CHANNEL_NUM;
        hwifi_cfg_init_cus_dpn_cali(puc_dyn_cali_param, pc_ctx, uc_rf_idx);

        for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; uc_idx++) {
            val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
            ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_5g_dpn_params[uc_idx],
                CUS_DY_CALI_DPN_PARAMS_NUM * sizeof(int8_t), &val, CUS_DY_CALI_DPN_PARAMS_NUM * sizeof(int8_t));
        }
        uc_cfg_id += MAC_NUM_5G_BAND;

        /* 5G 160M DPN */
        if (hwifi_cfg_init_cus_5g_160m_dpn_cali(puc_dyn_cali_param, pc_ctx, uc_rf_idx) != OAL_SUCC) {
            continue;
        }
        puc_dyn_cali_param++;
    }
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_cfg_init_cus_dyn_cali_dev1::memcpy fail!");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


void hwifi_print_cfg_pro_line_params_dev1(void)
{
    int32_t cfg_idx_one;
    int32_t cfg_idx_two;

    for (cfg_idx_one = 0; cfg_idx_one < WLAN_RF_CHANNEL_NUMS; cfg_idx_one++) {
        for (cfg_idx_two = 0; cfg_idx_two < DY_CALI_PARAMS_NUM; cfg_idx_two++) {
            oal_io_print("%s CORE[%d]para_idx[%d]::{%d, %d, %d}\n", "g_pro_line_params_dev1: ",
                cfg_idx_one, cfg_idx_two, g_pro_line_params_dev1[cfg_idx_one][cfg_idx_two].l_pow_par2,
                g_pro_line_params_dev1[cfg_idx_one][cfg_idx_two].l_pow_par1,
                g_pro_line_params_dev1[cfg_idx_one][cfg_idx_two].l_pow_par0);
        }
    }
}

uint8_t *hwifi_get_nvram_param_dev1(uint32_t nvram_param_idx)
{
    return g_cust_nvram_info_dev1[nvram_param_idx];
}

void hwifi_clear_cust_nvram_info_dev1(void)
{
    memset_s(g_cust_nvram_info_dev1, sizeof(g_cust_nvram_info_dev1), 0, sizeof(g_cust_nvram_info_dev1));
}

int32_t hwifi_custom_set_nvram_data_dev1(uint8_t *cust_nvram_info)
{
    if (oal_memcmp(cust_nvram_info, g_cust_nvram_info_dev1, sizeof(g_cust_nvram_info_dev1)) == 0) {
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    if (memcpy_s(g_cust_nvram_info_dev1, sizeof(g_cust_nvram_info_dev1),
                 cust_nvram_info, WLAN_CFG_DTS_NVRAM_END * CUS_PARAMS_LEN_MAX * sizeof(uint8_t)) != EOK) {
        return INI_FAILED;
    }
    return INI_NVRAM_RECONFIG;
}

/*
 * 函 数 名  : hwifi_hcc_custom_ini_data_buf
 * 功能描述  : 下发定制化配置命令
 */
OAL_STATIC int32_t hwifi_hcc_custom_ini_data_buf_by_idx(uint32_t dev_id, uint16_t us_syn_id)
{
    oal_netbuf_stru *pst_netbuf = NULL;
    uint32_t data_len;
    int32_t l_ret;
    uint32_t max_data_len = hcc_get_handler(dev_id)->tx_max_buf_len;

    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = hcc_get_handler(dev_id);

    oam_info_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf_by_idx hcc::is is null", dev_id);
    if (hcc == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf_by_idx hcc::is is null");
        return -OAL_EFAIL;
    }
    pst_netbuf = hwifi_hcc_custom_netbuf_alloc();
    if (pst_netbuf == NULL) {
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 组netbuf */
    data_len = (uint32_t)hwifi_hcc_custom_get_data(us_syn_id, pst_netbuf);
    if (data_len > max_data_len) {
        oam_error_log2(0, OAM_SF_CFG,
            "hwifi_hcc_custom_ini_data_buf_by_idx::got wrong data_len[%d] max_len[%d]", data_len, max_data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    if (data_len == 0) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf_by_idx::data is null us_syn_id[%d]", us_syn_id);
        oal_netbuf_free(pst_netbuf);
        return OAL_SUCC;
    }

    if ((pst_netbuf->data_len) || (pst_netbuf->data == NULL)) {
        oal_io_print("len:%d\r\n", pst_netbuf->data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    oal_netbuf_put(pst_netbuf, data_len);
    hcc_hdr_param_init(&st_hcc_transfer_param, HCC_ACTION_TYPE_CUSTOMIZE,
                       us_syn_id, 0, HCC_FC_WAIT, DATA_HI_QUEUE);

    l_ret = hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf_by_idx fail ret[%d]", l_ret);
        oal_netbuf_free(pst_netbuf);
    }

    return l_ret;
}

/*
 * 函 数 名  : host_params_init_first_dev1
 * 功能描述  : 给定制化参数全局数组 g_host_init_params_dev1 赋初值，ini文件读取失败时用初值
 */
OAL_STATIC void host_params_init_first_dev1(void)
{
    wlan_cfg_init cfg_id;
    for (cfg_id = WLAN_CFG_INIT_ROAM_SWITCH; cfg_id < WLAN_CFG_INIT_BUTT; cfg_id++) {
        g_host_init_params_dev1[cfg_id] = g_host_init_params_default_value[cfg_id];
    }
}

/*
 * 函 数 名  : hwifi_config_init_dev1
 * 功能描述  : netdev open 调用的定制化总入口
 *             读取ini文件，更新 g_host_init_params 全局数组
 */
int32_t hwifi_config_init_dev1(int32_t cus_tag)
{
    int32_t l_cfg_id;
    int32_t l_ret = INI_FAILED;
    int32_t l_ori_val;
    wlan_cfg_cmd *pgast_wifi_config = NULL;
    int32_t *pgal_params = NULL;
    int32_t l_cfg_value = 0;
    int32_t l_wlan_cfg_butt;

    switch (cus_tag) {
        case CUS_TAG_INI:
            host_params_init_first_dev1();
            pgast_wifi_config = hwifi_get_g_wifi_config_cmds();
            pgal_params = hwifi_get_g_host_init_params_dev1();
            l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
            break;
        case CUS_TAG_DTS:
            original_value_for_dts_params_dev1();
            pgast_wifi_config = hwifi_get_g_wifi_config_dts();
            pgal_params = hwifi_get_g_dts_params_dev1();
            l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
            break;
        case CUS_TAG_PRIV_INI:
            return hwifi_config_init_private_custom_dev1();
        case CUS_TAG_PRO_LINE_INI:
            return hwifi_config_init_dy_cali_custom_dev1();
        default:
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_dev1 tag number[0x%x] not correct!", cus_tag);
            return INI_FAILED;
    }

    for (l_cfg_id = 0; l_cfg_id < l_wlan_cfg_butt; l_cfg_id++) {
        /* 获取ini的配置值 */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, pgast_wifi_config[l_cfg_id].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            oam_info_log2(0, OAM_SF_CUSTOM, "hwifi_config_init_dev1 read ini file cfg_id[%d]tag[%d] not exist!",
                          l_cfg_id, cus_tag);
            continue;
        }
        l_ori_val = pgal_params[pgast_wifi_config[l_cfg_id].case_entry];
        pgal_params[pgast_wifi_config[l_cfg_id].case_entry] = l_cfg_value;
    }

    return INI_SUCC;
}

/*
 * 函 数 名  : hwifi_custom_host_read_cfg_init_dev1
 * 功能描述  : 读取双MP15 chip2的定制化配置文件入口
 */
OAL_STATIC uint32_t hwifi_custom_host_read_cfg_init_dev1(void)
{
    int32_t l_nv_read_ret;
    int32_t l_ini_read_ret;

    /* 先获取私有定制化项 */
    if (hwifi_config_init_dev1(CUS_TAG_PRIV_INI) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init_dual_mp15_dev1::hwifi_config_init_dev1 fail");
    }

    /* 读取nvram参数是否修改 */
    l_nv_read_ret = hwifi_custom_host_read_dyn_cali_nvram();
    /* 检查DEV1定制化文件中的产线配置是否修改，复用INI_SECTION_GT即INI_SECTION_WIFI + 1 */
    l_ini_read_ret = ini_file_check_conf_update(INI_SECTION_WIFI + 1);
    if (l_ini_read_ret || l_nv_read_ret) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init_dual_mp15_dev1 config is updated");
        hwifi_config_init_dev1(CUS_TAG_PRO_LINE_INI);
    }

    if (l_ini_read_ret == INI_FILE_TIMESPEC_UNRECONFIG) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init_dual_mp15_dev1 file is not updated");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (hwifi_config_init_dev1(CUS_TAG_DTS) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init_dual_mp15_dev1::hwifi_config_init_dev1 fail");
    }

    l_ini_read_ret = hwifi_config_init(CUS_TAG_NV);
    if (oal_unlikely(l_ini_read_ret != OAL_SUCC)) {
        oal_io_print("hwifi_custom_host_read_cfg_init_dual_mp15_dev1 NV fail l_ret[%d].\r\n", l_ini_read_ret);
    }

    if (hwifi_config_init_dev1(CUS_TAG_INI) != INI_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init_dual_mp15_dev1::hwifi_config_init_dev1 fail");
    }

    /* 启动完成后，输出打印 */
    oal_io_print("hwifi_custom_host_read_cfg_init_dual_mp15_dev1 finish!\r\n");

    return OAL_SUCC;
}

/*
 * 函 数 名  : hwifi_custom_host_read_cfg_init_dual_mp15
 * 功能描述  : 读取双MP15 的定制化配置文件入口
 */
uint32_t hwifi_custom_host_read_cfg_init_dual_mp15(void)
{
    /* 仅在首次初始化wifi时执行 */
    if ((oal_bus_get_chip_num() == WLAN_CHIP_MAX_NUM_PER_BOARD) && (g_cust_first_init_flg == OAL_TRUE)) {
        g_cust_first_init_flg = OAL_FALSE;
        hwifi_custom_host_read_cfg_init_mp13();
        hcc_set_main_dev(HCC_EP_WIFI_DEV1);
        hwifi_custom_host_read_cfg_init_dev1();
        hcc_set_main_dev(HCC_EP_WIFI_DEV);
        return OAL_SUCC;
    }

    /* 后续wifi上电时重新读取时执行 */
    if (hcc_get_main_dev() == HCC_EP_WIFI_DEV1) {
        return hwifi_custom_host_read_cfg_init_dev1();
    }
    return hwifi_custom_host_read_cfg_init_mp13();
}

/*
 * 函 数 名  : hwifi_hcc_customize_h2d_data_cfg_dual_mp15
 * 功能描述  : dual_mp15 协议栈初始化前定制化配置入口
 */
uint32_t hwifi_hcc_customize_h2d_data_cfg_dual_mp15(void)
{
    int32_t l_ret;
    uint32_t dev_id = hcc_get_main_dev();
    oam_info_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg_dual_mp15::dev_id[%d]", dev_id);

    /* wifi上电时重读定制化配置 */
    l_ret = (int32_t)hwifi_custom_host_read_cfg_init_dual_mp15();
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg_dual_mp15 data ret[%d]", l_ret);
    }

    // 如果不成功，返回失败
    l_ret = hwifi_hcc_custom_ini_data_buf_by_idx(dev_id, CUSTOM_CFGID_PRIV_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg_dual_mp15 priv data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }

    l_ret = hwifi_hcc_custom_ini_data_buf_by_idx(dev_id, CUSTOM_CFGID_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg_dual_mp15 ini data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}

#endif