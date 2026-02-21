/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : wal_linux_vendor.c 的头文件
 * 作    者 : wifi3
 * 创建日期 : 2020年7月14日
 */

#ifndef WAL_LINUX_VENDOR_H
#define WAL_LINUX_VENDOR_H

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "wlan_oneimage_define.h"
#include "wlan_types.h"
#include "mac_vap.h"

/* wifi 能力开关状态枚举值，通知到上层使用 */
typedef enum {
    WAL_WIFI_FEATURE_SUPPORT_11K = 0,
    WAL_WIFI_FEATURE_SUPPORT_11V = 1,
    WAL_WIFI_FEATURE_SUPPORT_11R = 2,
    WAL_WIFI_FEATURE_SUPPORT_VOWIFI_NAT_KEEP_ALIVE = 3,
    WAL_WIFI_FEATURE_SUPPORT_NARROWBAND = 4,
    WAL_WIFI_FEATURE_SUPPORT_160M_STA = 5,
    WAL_WIFI_FEATURE_SUPPORT_160M_AP = 6,
    WAL_WIFI_CONNECT_MLO = 7,
    WAL_WIFI_FEATURE_SUPPORT_WUR = 8,
    WAL_WIFI_FEATURE_SUPPORT_POLAR = 9,
    WAL_WIFI_FEATURE_SUPPORT_EMLSR = 10,

    WAL_WIFI_FEATURE_SUPPORT_BUTT
} wal_wifi_feature_capbility_enum;

typedef struct {
    uint32_t disable_capab_2ght40;
    uint32_t wifi_11be_switch_mask;
    uint8_t optimized_feature_switch_bitmap;
} cust_info_rpt; /* 仅用于supplicant通过ioctl获取定制化 */

/*
  私有命令函数表. 私有命令格式:
         设备名 命令名 参数
  hipriv "Hisilicon0 create vap0"
*/
/* private command strings */
#define CMD_SET_AP_WPS_P2P_IE         "SET_AP_WPS_P2P_IE"
#define CMD_SET_MLME_IE               "SET_MLME_IE"
#define CMD_P2P_SET_NOA               "P2P_SET_NOA"
#define CMD_P2P_SET_PS                "P2P_SET_PS"
#define CMD_SET_POWER_ON              "SET_POWER_ON"
#define CMD_SET_POWER_MGMT_ON         "SET_POWER_MGMT_ON"
#define CMD_COUNTRY                   "COUNTRY"
#define CMD_GET_CAPA_DBDC             "GET_CAPAB_RSDB"
#define CMD_CAPA_DBDC_SUPP            "RSDB:1"
#define CMD_CAPA_DBDC_NOT_SUPP        "RSDB:0"
#define CMD_SET_DC_STATE              "SET_DC_STATE"
#ifdef _PRE_WLAN_FEATURE_NRCOEX
#define CMD_SET_NRCOEX_PRIOR          "SET_NRCOEX_PRIOR"
#define CMD_GET_NRCOEX_INFO           "GET_NRCOEX_INFO"
#endif
#define CMD_SET_CALI_FEM_MODE         "SET_CALI_FEM_MODE"
#define CMD_SET_QOS_MAP               "SET_QOS_MAP"
#define CMD_TX_POWER                  "TX_POWER"
#define CMD_WPAS_GET_CUST             "WPAS_GET_CUST"
#define CMD_SET_STA_PM_ON             "SET_STA_PM_ON"
#define CMD_SET_DYNAMIC_DBAC_MODE     "SET_DYNAMIC_DBAC_MODE"
#if (defined(_PRE_CONFIG_HILINK_AMAX) && defined(_PRE_WLAN_TCP_OPT))
#define CMD_MULTILINK                 "MULTILINK"
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/* TAS天线切换命令 */
#define CMD_SET_MEMO_CHANGE           "SET_ANT_CHANGE"
/* 测量天线 */
#define CMD_MEASURE_TAS_RSSI          "SET_TAS_MEASURE"
/* TAS天线切换命令 */
#define CMD_SET_TAS_ANT_SWITCH        "SET_TAS_ANT_SWITCH"
/* 抬功率 */
#define CMD_SET_TAS_TXPOWER           "SET_TAS_TXPOWER"
/* 获取天线 */
#define CMD_TAS_GET_ANT               "TAS_GET_ANT"
#endif

#define CMD_SET_CLOSE_GO_CAC            "SET_CLOSE_GO_CAC"
#define CMD_SET_CLOSE_GO_CAC_LEN        (OAL_STRLEN(CMD_SET_CLOSE_GO_CAC))
#define CMD_SET_P2P_SCENE               "CMD_SET_P2P_SCENES"
#define CMD_SET_P2P_SCENE_LEN           (OAL_STRLEN(CMD_SET_P2P_SCENE))
#define CMD_SET_GO_DETECT_RADAR         "CMD_SET_GO_DETECT_RADAR"
#define CMD_SET_GO_DETECT_RADAR_LEN     (OAL_STRLEN(CMD_SET_GO_DETECT_RADAR))
#define CMD_SET_CHANGE_GO_CHANNEL       "CMD_SET_CHANGE_GO_CHANNEL"
#define CMD_SET_CHANGE_GO_CHANNEL_LEN   (OAL_STRLEN(CMD_SET_CHANGE_GO_CHANNEL))

#ifdef _PRE_WLAN_FEATURE_M2S
#ifdef _PRE_WLAN_FEATURE_LP_MIRACAST
#define CMD_SET_LP_ANT_SWITCH         "SET_LP_ANT_SWITCH"
#define CMD_GET_LP_ANT_SWITCH         "GET_LP_ANT_SWITCH"
#endif
#define CMD_SET_M2S_SWITCH            "SET_M2S_SWITCH"
#define CMD_SET_M2S_BLACKLIST         "SET_M2S_BLACKLIST"
#define CMD_SET_M2S_MODEM             "SET_M2S_MODEM"
#endif

#define CMD_VOWIFI_SET_MODE           "VOWIFI_DETECT SET MODE"
#define CMD_VOWIFI_GET_MODE           "VOWIFI_DETECT GET MODE"
#define CMD_VOWIFI_SET_PERIOD         "VOWIFI_DETECT SET PERIOD"
#define CMD_VOWIFI_GET_PERIOD         "VOWIFI_DETECT GET PERIOD"
#define CMD_VOWIFI_SET_LOW_THRESHOLD  "VOWIFI_DETECT SET LOW_THRESHOLD"
#define CMD_VOWIFI_GET_LOW_THRESHOLD  "VOWIFI_DETECT GET LOW_THRESHOLD"
#define CMD_VOWIFI_SET_HIGH_THRESHOLD "VOWIFI_DETECT SET HIGH_THRESHOLD"
#define CMD_VOWIFI_GET_HIGH_THRESHOLD "VOWIFI_DETECT GET HIGH_THRESHOLD"
#define CMD_VOWIFI_SET_TRIGGER_COUNT  "VOWIFI_DETECT SET TRIGGER_COUNT"
#define CMD_VOWIFI_GET_TRIGGER_COUNT  "VOWIFI_DETECT GET TRIGGER_COUNT"

#define CMD_VOWIFI_SET_PARAM          "VOWIFI_DETECT SET"
#define CMD_VOWIFI_GET_PARAM          "VOWIFI_DETECT GET"

#define CMD_VOWIFI_IS_SUPPORT_REPLY   "true"

#define CMD_VOWIFI_IS_SUPPORT         "VOWIFI_DETECT VOWIFI_IS_SUPPORT"

#define CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY "GET_WIFI_PRIV_FEATURE_CAPABILITY"

#define CMD_SETSUSPENDOPT             "SETSUSPENDOPT"
#define CMD_SETSUSPENDMODE            "SETSUSPENDMODE"

#define CMD_SET_SOFTAP_MIMOMODE       "SET_AP_MODE"

#define CMD_GET_AP_BANDWIDTH          "GET_AP_BANDWIDTH"

#define CMD_GET_VHT160_SUPPORTED      "GET_VHT160_SUPPORTED"

#define CMD_SET_VHT160_FEM_LOWER      "SET_VHT160_FEM_LOWER"
#define CMD_PK_APP                    "CMD_PK_APP"
#define CMD_PCIE_ASPM_STATE           "CMD_PCIE_ASPM_STATE"
#define CMD_WLAN_FREQ                 "CMD_WLAN_FREQ"
#define CMD_NAPI_STATE                "CMD_NAPI_STATE"
#define CMD_PM_STATE                  "CMD_PM_STATE"
#define CMD_GSO_STATE                 "CMD_GSO_STATE"
#define CMD_LOW_LATENCY_ON            "CMD_LOW_LATENCY_ON"
#define CMD_LOW_LATENCY_OFF           "CMD_LOW_LATENCY_OFF"
#define CMD_HID2D_PARAMS              "CMD_HID2D_PARAMS"
#define CMD_GET_CURRENT_TSF           "CMD_GET_CURRENT_TSF"
#ifdef _PRE_WLAN_CHBA_MGMT
#define CMD_SET_BATTERY_LEVEL         "SET_BATTERY_LEVEL"
#define CMD_CHBA_MODULE_INIT          "HID2D_MODULE_INIT"
#define CMD_SET_CHAN_ADJUST           "SET_CHAN_ADJUST"
#define CMD_SET_SUPP_COEX_CHAN_LIST   "SET_SUPP_COEX_CHAN_LIST"
#define CMD_RX_MGMT_REMAIN_ON_CHANNEL "RX_MGMT_REMAIN_ON_CHANNEL"
#define CMD_CANCEL_SAME_CHAN_REMAIN_ON_CHAN "CANCEL_SAME_CHAN_REMAIN_ON_CHAN"
#define CMD_GET_CHBA_CAP             "GET_CHBA_CAP"
#define CMD_CHBA_CAP_SUPP            1
#define CMD_CHBA_CAP_NOT_SUPP        2
#endif
#define CMD_SET_RX_FILTER_ENABLE       "set_rx_filter_enable"
#define CMD_ADD_RX_FILTER_ITEMS        "add_rx_filter_items"
#define CMD_CLEAR_RX_FILTERS           "clear_rx_filters"
#define CMD_GET_RX_FILTER_PKT_STATE    "get_rx_filter_pkt_state"
#define CMD_FILTER_SWITCH              "FILTER"

#define CMD_GET_APF_PKTS_CNT           "GET_APF_PKTS_CNT"
#define CMD_GET_APF_PKTS_CNT_LEN       (OAL_STRLEN(CMD_GET_APF_PKTS_CNT))
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
#define CMD_GET_ABNORMAL_PKTS_CNT      "GET_ABNORMAL_PKTS_CNT"
#define CMD_GET_ABNORMAL_PKTS_CNT_LEN  (OAL_STRLEN(CMD_GET_ABNORMAL_PKTS_CNT))
#endif
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#define CMD_QUERY_SNIFFER      "CMD_QUERY_SNIFFER"
#define CMD_QUERY_SNIFFER_LEN  (OAL_STRLEN(CMD_QUERY_SNIFFER))
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
#define CMD_QUERY_CSI      "CMD_QUERY_CSI"
#define CMD_QUERY_CSI_LEN  (OAL_STRLEN(CMD_QUERY_CSI))
#endif
#define CMD_SET_FASTSLEEP_SWITCH       "SET_FASTSLEEP_SWITCH "
#define CMD_SET_FASTSLEEP_SWITCH_LEN   (OAL_STRLEN(CMD_SET_FASTSLEEP_SWITCH))

#define CMD_GET_FAST_SLEEP_CNT         "GET_FAST_SLEEP_CNT"
#define CMD_GET_FAST_SLEEP_CNT_LEN     (OAL_STRLEN(CMD_GET_FAST_SLEEP_CNT))

#define CMD_GET_BEACON_CNT             "GET_BEACON_CNT"
#define CMD_GET_BEACON_CNT_LEN         (OAL_STRLEN(CMD_GET_BEACON_CNT))

#define CMD_SET_TCP_ACK_CTL             "SET_TCP_ACK_SWITCH "
#define CMD_SET_TCP_ACK_CTL_LEN        (OAL_STRLEN(CMD_SET_TCP_ACK_CTL))

#define CMD_GET_WIFI_CATEGORY_SUPPORT      "CMD_WIFI_CATEGORY"
#define CMD_GET_WIFI_CATEGORY_SUPPORT_LEN  (OAL_STRLEN(CMD_GET_WIFI_CATEGORY_SUPPORT))

#define CMD_SET_AX_CLOSE_HTC            "SET_AX_CLOSE_HTC"
#define CMD_SET_AX_CLOSE_HTC_LEN        (OAL_STRLEN(CMD_SET_AX_CLOSE_HTC))
#define CMD_SET_AX_BLACKLIST            "SET_AX_BLACKLIST"
#define CMD_SET_AX_BLACKLIST_LEN        (OAL_STRLEN(CMD_SET_AX_BLACKLIST))
#define CMD_SET_BE_BLACKLIST            "SET_BE_BLACKLIST"
#define CMD_SET_BE_BLACKLIST_LEN        (OAL_STRLEN(CMD_SET_BE_BLACKLIST))
#define CMD_SET_MLSR_LINK_SWITCH         "SET_MLSR_LINK_SWITCH"
#define CMD_SET_MLSR_LINK_SWITCH_LEN    (OAL_STRLEN(CMD_SET_MLSR_LINK_SWITCH))
#define CMD_SET_EMLSR_SWITCH            "SET_EMLSR_SWITCH"
#define CMD_EMLSR_SWITCH_LEN            (OAL_STRLEN(CMD_SET_EMLSR_SWITCH))
#define CMD_SET_CLEAR_11N_BLACKLIST    "CMD_CLEAR_11N_BLACKLIST"
#define CMD_SET_CLEAR_11N_BLACKLIST_LEN (OAL_STRLEN(CMD_SET_CLEAR_11N_BLACKLIST))
#define CMD_GET_RADAR_RESULT           "GET_RADAR_RESULT"
#define CMD_GET_RADAR_RESULT_LEN       (OAL_STRLEN(CMD_GET_RADAR_RESULT))
#define CMD_GET_TB_FRAME_GAIN        "GET_TB_FRAME_GAIN"
#define CMD_GET_TB_FRAME_GAIN_LEN    (OAL_STRLEN(CMD_GET_TB_FRAME_GAIN))
#define CMD_RXFILTER_START             "RXFILTER-START"
#define CMD_RXFILTER_STOP              "RXFILTER-STOP"
#define CMD_MIRACAST_START             "MIRACAST 1"
#define CMD_MIRACAST_STOP              "MIRACAST 0"
#define CMD_SET_WUR_PARAM              "SET_WUR_PARAM"
#define CMD_SET_LP_LONG_DTIM           "SET_LP_LONG_DTIM"
#define CMD_SET_OMI_BW_SWITCH          "SET_OMI_BW_SWITCH"
#define CMD_GET_LOWPOWER_STATISTICS     "GET_LOWPOWER_STATISTICS"

#define PROTOCOL_DEFAULT               1
#define PROTOCOL_WIFI6_SUPPORT         2
#define PROTOCOL_WIFI7_SUPPORT         4

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
#define CMD_SET_RX_LISTEN_PS_SWITCH    "SET_RX_LISTEN_PS_SWITCH "
#define CMD_SET_RX_LISTEN_PS_SWITCH_LEN (OAL_STRLEN(CMD_SET_RX_LISTEN_PS_SWITCH))
#define CMD_GET_RX_LISTEN_STATE        "GET_RX_LISTEN_STATE"
#define CMD_GET_RX_LISTEN_STATE_LEN    (OAL_STRLEN(CMD_GET_RX_LISTEN_STATE))
#endif
#define CMD_SET_SAE_PWE "SET_SAE_PWE"
#define CMD_SET_SAE_PWE_LEN  (OAL_STRLEN(CMD_SET_SAE_PWE))

#define CMD_SET_COMPETE_MODE "SET_COMPETE_MODE"
#define CMD_SET_COMPETE_MODE_LEN (OAL_STRLEN(CMD_SET_COMPETE_MODE))

#define CMD_SET_PKT_FILTER "SET_PKT_FILTER"
#define CMD_SET_PKT_FILTER_LEN (OAL_STRLEN(CMD_SET_PKT_FILTER))

#define CMD_SET_DBAC_ENABLE_FOLLOW_CHANNEL  "dbac_enable_follow_channel"
#define CMD_SET_DBAC_ENABLE_FOLLOW_CHANNEL_LEN (OAL_STRLEN(CMD_SET_DBAC_ENABLE_FOLLOW_CHANNEL))

#ifdef _PRE_WLAN_FEATURE_PWL
#define CMD_PWL_ENABLE "PWL_ENABLE"
#define CMD_PWL_ENABLE_LEN (OAL_STRLEN(CMD_PWL_ENABLE))

#define CMD_PWL_SET_PRE_KEY "PWL_SET_PRE_KEY"
#define CMD_PWL_SET_PRE_KEY_LEN (OAL_STRLEN(CMD_PWL_SET_PRE_KEY))
#endif

#ifdef _PRE_WLAN_FEATURE_PF_SCH_USER
#define CMD_ATF_SET_USER_PARAM "ATF_SET_USER_PARAM"
#define CMD_ATF_SET_USER_PARAM_LEN (OAL_STRLEN(CMD_ATF_SET_USER_PARAM))
#endif

#define CMD_SET_EDCA_PARAM "SET_EDCA_PARAM"
#define CMD_SET_EDCA_PARAM_LEN (OAL_STRLEN(CMD_SET_EDCA_PARAM))

#ifdef _PRE_WLAN_FEATURE_VSP
#define CMD_SET_VSP_CFG_PARAM "SET_VSP_CFG_PARAM"
#define CMD_SET_VSP_CFG_PARAM_LEN (OAL_STRLEN(CMD_SET_VSP_CFG_PARAM))

#define CMD_VSP_SET_NAV_EDCA "SET_VSP_NAV_EDCA"
#define CMD_VSP_SET_NAV_EDCA_LEN (OAL_STRLEN(CMD_VSP_SET_NAV_EDCA))
#endif
#define CMD_SET_AI_CSMA_EDCA_PARAM "SET_AI_CSMA_EDCA_PARAM"
#define CMD_SET_AI_CSMA_EDCA_PARAM_LEN (OAL_STRLEN(CMD_SET_AI_CSMA_EDCA_PARAM))
/* 私有命令参数长度宏 */
#define   CMD_SET_AP_WPS_P2P_IE_LEN                              (OAL_STRLEN(CMD_SET_AP_WPS_P2P_IE))
#define   CMD_P2P_SET_NOA_LEN                                    (OAL_STRLEN(CMD_P2P_SET_NOA))
#define   CMD_P2P_SET_PS_LEN                                     (OAL_STRLEN(CMD_P2P_SET_PS))
#define   CMD_SET_STA_PM_ON_LEN                                  (OAL_STRLEN(CMD_SET_STA_PM_ON))
#define   CMD_SET_QOS_MAP_LEN                                    (OAL_STRLEN(CMD_SET_QOS_MAP))
#define   CMD_COUNTRY_LEN                                        (OAL_STRLEN(CMD_COUNTRY))
#define   CMD_LTECOEX_MODE_LEN                                   (OAL_STRLEN(CMD_LTECOEX_MODE))
#define   CMD_TX_POWER_LEN                                       (OAL_STRLEN(CMD_TX_POWER))
#define   CMD_WPAS_GET_CUST_LEN                                  (OAL_STRLEN(CMD_WPAS_GET_CUST))
#define   CMD_VOWIFI_SET_PARAM_LEN                               (OAL_STRLEN(CMD_VOWIFI_SET_PARAM))
#define   CMD_VOWIFI_GET_PARAM_LEN                               (OAL_STRLEN(CMD_VOWIFI_GET_PARAM))
#define   CMD_SETSUSPENDOPT_LEN                                  (OAL_STRLEN(CMD_SETSUSPENDOPT))
#define   CMD_SETSUSPENDMODE_LEN                                 (OAL_STRLEN(CMD_SETSUSPENDMODE))
#define   CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY_LEN               (OAL_STRLEN(CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY))
#define   CMD_VOWIFI_IS_SUPPORT_LEN                              (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT))
#define   CMD_VOWIFI_IS_SUPPORT_REPLY_LEN                        (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT_REPLY))
#define   CMD_FILTER_SWITCH_LEN                                  (OAL_STRLEN(CMD_FILTER_SWITCH))
#define   CMD_RXFILTER_START_LEN                                 (OAL_STRLEN(CMD_RXFILTER_START))
#define   CMD_RXFILTER_STOP_LEN                                  (OAL_STRLEN(CMD_RXFILTER_STOP))
#define   CMD_SET_MLME_IE_LEN                                    (OAL_STRLEN(CMD_SET_MLME_IE))
#define   CMD_MIRACAST_START_LEN                                 (OAL_STRLEN(CMD_MIRACAST_START))
#define   CMD_MIRACAST_STOP_LEN                                  (OAL_STRLEN(CMD_MIRACAST_STOP))
#define   CMD_SET_POWER_ON_LEN                                   (OAL_STRLEN(CMD_SET_POWER_ON))
#define   CMD_SET_POWER_MGMT_ON_LEN                              (OAL_STRLEN(CMD_SET_POWER_MGMT_ON))
#define   CMD_GET_CAPA_DBDC_LEN                                  (OAL_STRLEN(CMD_GET_CAPA_DBDC))
#define   CMD_CAPA_DBDC_SUPP_LEN                                 (OAL_STRLEN(CMD_CAPA_DBDC_SUPP))
#define   CMD_CAPA_DBDC_NOT_SUPP_LEN                             (OAL_STRLEN(CMD_CAPA_DBDC_NOT_SUPP))
#define   CMD_SET_TAS_TXPOWER_LEN                                (OAL_STRLEN(CMD_SET_TAS_TXPOWER))
#define   CMD_MEASURE_TAS_RSSI_LEN                               (OAL_STRLEN(CMD_MEASURE_TAS_RSSI))
#define   CMD_SET_TAS_ANT_SWITCH_LEN                             (OAL_STRLEN(CMD_SET_TAS_ANT_SWITCH))
#define   CMD_TAS_GET_ANT_LEN                                    (OAL_STRLEN(CMD_TAS_GET_ANT))
#define   CMD_SET_MEMO_CHANGE_LEN                                (OAL_STRLEN(CMD_SET_MEMO_CHANGE))
#define   CMD_SET_M2S_BLACKLIST_LEN                              (OAL_STRLEN(CMD_SET_M2S_BLACKLIST))
#define   CMD_GET_AP_BANDWIDTH_LEN                               (OAL_STRLEN(CMD_GET_AP_BANDWIDTH))
#define   CMD_GET_VHT160_SUPPORTED_LEN                           (OAL_STRLEN(CMD_GET_VHT160_SUPPORTED))
#define   CMD_SET_VHT160_FEM_LOWER_LEN                           (OAL_STRLEN(CMD_SET_VHT160_FEM_LOWER))
#define   CMD_SET_DC_STATE_LEN                                   (OAL_STRLEN(CMD_SET_DC_STATE))
#define   CMD_PCIE_ASPM_STATE_LEN                                (OAL_STRLEN(CMD_PCIE_ASPM_STATE))
#define   CMD_WLAN_FREQ_LEN                                      (OAL_STRLEN(CMD_WLAN_FREQ))
#define   CMD_NAPI_STATE_LEN                                     (OAL_STRLEN(CMD_NAPI_STATE))
#define   CMD_PK_APP_LEN                                         (OAL_STRLEN(CMD_PK_APP))
#define   CMD_PM_STATE_LEN                                       (OAL_STRLEN(CMD_PM_STATE))
#define   CMD_GSO_STATE_LEN                                      (OAL_STRLEN(CMD_GSO_STATE))
#define   CMD_LOW_LATENCY_ON_LEN                                 (OAL_STRLEN(CMD_LOW_LATENCY_ON))
#define   CMD_LOW_LATENCY_OFF_LEN                                (OAL_STRLEN(CMD_LOW_LATENCY_OFF))
#define   CMD_SET_DYNAMIC_DBAC_MODE_LEN                          (OAL_STRLEN(CMD_SET_DYNAMIC_DBAC_MODE))
#define   CMD_HID2D_PARAMS_LEN                                   (OAL_STRLEN(CMD_HID2D_PARAMS))
#define   CMD_GET_CURRENT_TSF_LEN                                (OAL_STRLEN(CMD_GET_CURRENT_TSF))
#ifdef _PRE_WLAN_CHBA_MGMT
#define CMD_SET_BATTERY_LEVEL_LEN                (OAL_STRLEN(CMD_SET_BATTERY_LEVEL))
#define CMD_CHBA_MODULE_INIT_LEN                 (OAL_STRLEN(CMD_CHBA_MODULE_INIT))
#define CMD_SET_CHAN_ADJUST_LEN                  (OAL_STRLEN(CMD_SET_CHAN_ADJUST))
#define CMD_SET_SUPP_COEX_CHAN_LIST_LEN          (OAL_STRLEN(CMD_SET_SUPP_COEX_CHAN_LIST))
#define CMD_RX_MGMT_REMAIN_ON_CHANNEL_LEN        (OAL_STRLEN(CMD_RX_MGMT_REMAIN_ON_CHANNEL))
#define CMD_CANCEL_SAME_CHAN_REMAIN_ON_CHAN_LEN  (OAL_STRLEN(CMD_CANCEL_SAME_CHAN_REMAIN_ON_CHAN))
#define CMD_GET_CHBA_CAP_LEN                     (OAL_STRLEN(CMD_GET_CHBA_CAP))
#endif
#if (defined(_PRE_CONFIG_HILINK_AMAX) && defined(_PRE_WLAN_TCP_OPT))
#define CMD_MULTILINK_LEN                                        (OAL_STRLEN(CMD_MULTILINK))
#endif
#define CMD_SET_WUR_PARAM_LEN                     (OAL_STRLEN(CMD_SET_WUR_PARAM))
#define CMD_SET_LP_LONG_DTIM_LEN                  (OAL_STRLEN(CMD_SET_LP_LONG_DTIM))
#define CMD_SET_OMI_BW_SWITCH_LEN                 (OAL_STRLEN(CMD_SET_OMI_BW_SWITCH))
#define CMD_GET_LOWPOWER_STATISTICS_LEN           (OAL_STRLEN(CMD_GET_LOWPOWER_STATISTICS))

#define MS_TO_S             (1000)

#define WAL_AI_CSMA_EDCA_INVALID_VAL 0xFF
#define WAL_AI_CSMA_EDCA_TIMOEOUT (1 * HZ)
#define WAL_AI_CSMA_EDCA_HOST_ERR (-1)
/* wpa_supplicant 下发命令 */

#if defined(CONFIG_COMPAT) && defined(_PRE_HISI_HERTZ)
typedef struct wal_wifi_priv_cmd_compat {
    compat_caddr_t puc_buf;
    uint32_t total_len;
    uint32_t used_len;
} wal_wifi_priv_cmd_stru_compat;
#endif
typedef struct wal_wifi_priv_cmd {
    uint8_t *puc_buf;
    uint32_t total_len;
    uint32_t used_len;
} wal_wifi_priv_cmd_stru;

int32_t wal_vendor_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, int32_t cmd);
int32_t wal_netdev_stop(oal_net_device_stru *pst_net_dev);
int32_t wal_ioctl_set_sta_pm_on(oal_net_device_stru *pst_net_dev, uint8_t *pc_command, uint8_t cmd_size);
int32_t wal_ioctl_reduce_sar(oal_net_device_stru *pst_net_dev, uint16_t tx_power);
uint8_t wal_get_vap_band_width(wlan_channel_bandwidth_enum_uint8 enum_band_width);
uint8_t wal_bw_en_to_number(wlan_bw_cap_enum_uint8 bandwidth);
int32_t wal_vendor_set_ai_csma_edca_param(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd,
    uint8_t *cmd, uint32_t cmd_name_len);
#ifdef _PRE_WLAN_FEATURE_GET_STATION_INFO_EXT
int32_t wal_vendor_set_station_info_ext_cdf_th(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd,
    uint8_t *command, uint32_t cmd_name_len);
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
uint32_t wal_hipriv_skip_cur_cac(mac_vap_stru *mac_vap, uint32_t *params);
#endif
#endif
