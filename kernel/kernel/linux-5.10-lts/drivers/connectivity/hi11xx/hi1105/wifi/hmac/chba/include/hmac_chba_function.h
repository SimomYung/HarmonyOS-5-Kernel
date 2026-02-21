/*
* Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
* 功能说明   : hmac_chba_mgmt.c的头文件
* 作者       : wifi
* 创建日期   : 2021年1月27日
*/

#ifndef HMAC_CHBA_FUNCTION_H
#define HMAC_CHBA_FUNCTION_H
/* 1 其他头文件包含 */
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "mac_chba_common.h"
#include "hmac_chba_interface.h"
#include "hmac_chba_multi_connect.h"
#ifdef _PRE_WLAN_FEATURE_NBW
#include "hmac_nbw_mgmt.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CHBA_FUNCTION_H

#ifdef _PRE_WLAN_CHBA_MGMT
/* 2 宏定义 */
/* 同步相关宏 */
#define MAC_CHBA_SYNC_REQUEST_CNT 6 /* 同步请求帧软件重发次数 */
#define MAC_CHBA_CHANNEL_SWITCH_OVERHEAD_MS 5 /* 模拟信道切换时间 */
#define MAC_CHBA_SYNC_WAITING_TIMEOUT 48
/* 连接相关宏 */
#define CHBA_WHITELIST_TIMEOUT_MS (2000 * 1000)
#define CHBA_ASSOC_WAITING_TIMEOUT 100 /* 100ms */
#define CHBA_MAX_ASSOC_CNT 3
#define CHBA_SWITCH_STA_PSM_PERIOD 60000  /* chba打开低功耗超时的时间60s */
#define WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_HIGH_CHBA 600
#define WLAN_CPU_FREQ_THROUGHPUT_THRESHOLD_LOW_CHBA 400
#define CHBA_CONN_INITIATOR_FLAG_OFFSET 30 /* cfg80211_connect_params->flags其30bit、31bit位为chba连接发起方flag */
#define CHBA_CONN_FAST_CONNECT_BIT 0 /* 快速连接标记位 */
#define CHBA_CONN_NO_ENCRY_BIT 1 /* 非加密标记位 */
#define CHBA_NORMAL_CONNECT_FLAG 0x0 /* chba普通连接flag */
#define CHBA_ENCRY_FAST_CONNECT_FLAG 0x1 /* 加密模式的快速连接flag */
#define CHBA_NOT_ENCRY_FAST_CONNECT_FLAG 0x3 /* 加密模式的快速连接flag */
#define CHBA_INVALID_CONNECT_FLAG 0x4 /* 无效连接flag */
#define CHBA_NOT_VERIFIED_USER_ID_INIT 0xff /* chba极速连接不进行白名单校验的user_id初始值 */

/* 3 枚举定义 */
/* chba连接角色枚举 */
typedef enum {
    CHBA_CONN_INITIATOR = 0, /* 建链发起者 */
    CHBA_CONN_RESPONDER = 1, /* 建链响应者 */

    CHBA_CONN_ROLE_BUTT,
} chba_connect_role_enum;

/* chba user连接状态枚举 */
/* 当该状态为UP状态时，设置mac_user_stru中的en_user_asoc_state设置为MAC_USER_STATE_ASSOC */
typedef enum {
    CHBA_USER_STATE_INIT = 0,
    CHBA_USER_STATE_JOIN_COMPLETE = 1,
    CHBA_USER_STATE_WAIT_AUTH_SEQ2 = 2,
    CHBA_USER_STATE_WAIT_AUTH_SEQ4 = 3,
    CHBA_USER_STATE_AUTH_COMPLETE = 4,
    CHBA_USER_STATE_WAIT_ASSOC = 5,
    CHBA_USER_STATE_LINK_UP = 6,

    CHBA_USER_STATE_BUTT,
} chba_user_state_enum;

typedef enum {
    CHBA_NOT_VERIFIED_CONN_RSP_NOT_RUNNING = 0,
    CHBA_NOT_VERIFIED_CONN_RSP_ENCRY_CONN_MODE = 1, /* 不进行白名单校验的chba极速连接响应端, 新user以加密模式连接 */
    CHBA_NOT_VERIFIED_CONN_RSP_NOT_ENCRY_CONN_MODE = 2 /* 不进行白名单校验的chba极速连接响应端, 新user以非加密模式连接 */
} chba_not_verified_conn_rsp_conn_mode_enum; /* 不进行白名单校验的响应端连接模式枚举 */

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 白名单: 经过NEARBY设备认证过的Peer */
typedef struct {
    oal_dlist_head_stru st_entry;
    uint8_t peer_mac_addr[WLAN_MAC_ADDR_LEN];
} whitelist_cache_stru; /* 白名单成员缓存结构体 */

typedef struct {
    oal_dlist_head_stru st_entry;
    uint8_t peer_mac_addr[WLAN_MAC_ADDR_LEN];
    uint16_t data_len;
    uint8_t assoc_priv_ie[HMAC_CHBA_USER_ASSOC_PRIV_IE_MAX_LEN];
} hmac_chba_assoc_priv_ie_cache_stru; /* chba私有数据链表成员缓存结构体 */

typedef struct {
    uint8_t list_len;
    uint8_t resv[3];
    oal_dlist_head_stru datalist_head;
} hmac_chba_connect_list_stru; /* chba连接信息链表头结构体 */

typedef struct {
    uint16_t schedule_period; /* 调度周期的时隙个数，默认为32 */
    uint16_t sync_slot_cnt; /* 同步时隙个数，默认为2 */
    uint16_t schedule_time_ms; /* 调度周期时间，16 * schedule_period */
    uint16_t vap_alive_timeout; /* vap处于无链路状态的最长时隙个数，默认为1024 */
    uint16_t link_alive_timeout; /* 链路保活的最长时隙个数，默认为1024 */
} hmac_chba_system_params;

typedef struct {
    oal_bool_enum_uint8 is_fast_conn_rsp; /* 是否是极速连接响应端的标记位 */
    uint8_t not_verified_rsp_conn_mode; /* 无白名单校验极速连接响应端工作模式 */
    uint8_t resv[2];
    uint16_t not_verified_user_id[MAC_RES_MAX_USER_LIMIT]; /* 未经过校验便添加的user_id */
    frw_timeout_stru assoc_waiting_timer;
} fast_conn_rsp_info_stru; /* chba快速连接响应方信息结构体 */

/* 新增chba相关的成员变量，挂在hmac_vap_stru中 */
struct hmac_chba_vap_tag {
    uint8_t mac_vap_id;
    uint8_t need_recovery_work_channel;
    uint8_t chba_log_level;
    uint8_t vap_bitmap_level; /* vap低功耗bitmap */
#ifdef _PRE_WLAN_FEATURE_NBW
    uint8_t nbw_mode;
    hmac_nbw_vap_stru *nbw_info;
#endif
    fast_conn_rsp_info_stru fast_conn_rsp_info; /* 极速连接响应方数据结构 */

    uint32_t channel_sequence_bitmap;
    mac_channel_stru social_channel;
    mac_channel_stru work_channel; /* 记录岛工作信道 */
    mac_channel_stru second_work_channel; /* 记录CHBA第二个工作信道 */

    uint8_t pnf_buf[MAC_CHBA_MGMT_MID_PAYLOAD_BYTES];
    uint8_t beacon_buf[MAC_CHBA_MGMT_MAX_PAYLOAD_BYTES];
    uint16_t pnf_buf_len;
    uint16_t beacon_buf_len;

    hmac_chba_connect_list_stru whitelist; /* 保存经过Nearby设备认证过的peer设备信息 */
    /* chba debug相关 */
    chba_bitmap_switch_enum auto_bitmap_switch; /* 动态调整bitmap开关 */
    /* 本设备共存信道信息 */
    uint8_t self_coex_chan_cnt;
    uint8_t is_support_multiple_island; /* 是否支持一域多岛 */
    uint8_t self_coex_channels_list[WLAN_5G_CHANNEL_NUM];
};

typedef struct {
    uint8_t intf_rpt_slot;
    uint8_t switch_chan_num;
    uint8_t switch_chan_bw;
} mac_set_chan_switch_test_params;
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /*  */
    uint8_t ps_bitmap; /* bitmap信息 */
} mac_chba_set_ps_bitmap_params;

extern uint32_t g_discovery_bitmap;
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
hmac_chba_system_params *hmac_get_chba_system_params(void);
hmac_chba_vap_stru *hmac_get_chba_vap(void);
hmac_chba_vap_stru *hmac_get_up_chba_vap_info(void);
oal_bool_enum_uint8 hmac_chba_vap_start_check(hmac_vap_stru *hmac_vap);
uint32_t hmac_config_chba_base_params(mac_chba_set_init_para *init_params);
uint32_t hmac_start_chba(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_stop_chba(hmac_vap_stru *hmac_vap);
void hmac_update_chba_info_aftet_vap_del_user(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_chba_request_sync(mac_vap_stru *mac_vap, hmac_chba_vap_stru *chba_vap_info,
    mac_chba_set_sync_request *sync_req_params);
uint32_t hmac_chba_d2h_sync_event(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
uint32_t hmac_chba_initiate_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params);
uint32_t hmac_chba_response_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params);
uint32_t hmac_chba_cancel_response_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params);
void hmac_generate_chba_domain_bssid(uint8_t *bssid, uint8_t bssid_len, uint8_t *mac_addr, uint8_t mac_addr_len);
void hmac_chba_vap_update_domain_bssid(hmac_vap_stru *hmac_vap, uint8_t *mac_addr);

uint32_t hmac_chba_rcv_domain_merge_params(hmac_vap_stru *hmac_vap, uint8_t *buf);
uint32_t hmac_user_set_asoc_req_ie(hmac_user_stru *hmac_user,
    uint8_t *payload, uint32_t payload_len_temp, uint8_t reass_flag);
uint32_t hmac_user_free_asoc_req_ie(uint16_t us_idx);

void hmac_chba_connect_succ_handler(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len);
void hmac_update_chba_vap_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_chba_topo_info_sync(void);
uint32_t hmac_chba_device_info_sync(uint8_t update_type, uint8_t device_id, const uint8_t *mac_addr);
void hmac_chba_cur_master_info_init(uint8_t *own_mac_addr, mac_channel_stru *work_channel);
void hmac_chba_sync_process_after_asoc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
hmac_chba_vap_stru *hmac_chba_get_chba_vap_info(hmac_vap_stru *hmac_vap);
hmac_vap_stru *hmac_chba_find_chba_vap(mac_device_stru *mac_device);
void hmac_chba_set_freq_lock_th(uint16_t *cpu_freq_high_limit, uint16_t *cpu_freq_low_limit);
uint32_t hmac_chba_is_valid_channel(uint8_t chan_number);
uint32_t hmac_chba_conn_response_rx_assoc_proc(hmac_vap_stru *hmac_vap,
    mac_rx_ctl_stru *rx_info, uint8_t *mac_hdr);
oal_bool_enum_uint8 hmac_chba_is_connecting_or_syncing(mac_vap_stru *mac_vap);
oal_bool_enum_uint8 hmac_chba_is_support_fast_connect(void);
uint32_t hmac_chba_tx_complete_assoc_rsp(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
uint32_t hmac_chba_not_verified_fast_conn_rsp_finish_proc(void *arg);
void hmac_chba_fail_to_connect(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_chba_conn_param_stru *conn_param, uint8_t connect_role);

OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_check_fast_connect_flag(uint8_t conn_flag)
{
    return ((conn_flag & BIT(CHBA_CONN_FAST_CONNECT_BIT)) != 0);
}

OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_check_no_encry_connect_flag(uint8_t conn_flag)
{
    return ((conn_flag & BIT(CHBA_CONN_NO_ENCRY_BIT)) != 0);
}

/* 功能描述: 判断是否是非加密模式极速连接 */
OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_is_no_encry_fast_connect_flag(uint8_t conn_flag)
{
    return ((hmac_chba_check_fast_connect_flag(conn_flag) == OAL_TRUE) &&
        (hmac_chba_check_no_encry_connect_flag(conn_flag) == OAL_TRUE));
}
/* 功能描述: 判断是否是极速连接发起方 */
OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_is_fast_connect_initiate(hmac_user_stru *hmac_user)
{
    chba_connect_status_stru *connect_info = &(hmac_user->chba_user.connect_info);
    return ((connect_info->connect_role == CHBA_CONN_INITIATOR) &&
        (hmac_chba_check_fast_connect_flag(connect_info->conn_flag) == OAL_TRUE));
}

/* 功能描述: 判断是否是极速连接响应方 */
OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_is_fast_connect_response(hmac_vap_stru *hmac_vap)
{
    hmac_chba_vap_stru *chba_vap_info = NULL;
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        return OAL_FALSE;
    }
    chba_vap_info = hmac_vap->hmac_chba_vap_info;

    return (chba_vap_info && (chba_vap_info->fast_conn_rsp_info.is_fast_conn_rsp == OAL_TRUE));
}

/* 功能描述: 判断是否是极速连接响应方, 且无需对入网user进行校验 */
OAL_INLINE OAL_STATIC oal_bool_enum_uint8 hmac_chba_is_not_verified_fast_connect_response(
    hmac_chba_vap_stru *chba_vap_info)
{
    return (chba_vap_info->fast_conn_rsp_info.not_verified_rsp_conn_mode != CHBA_NOT_VERIFIED_CONN_RSP_NOT_RUNNING);
}
#endif
#endif /* end of this file */
