/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw island domain device mgmt
 * Author: wifi
 * Create: 2024-12-26
 */
#ifndef HMAC_NBW_MGMT_H
#define HMAC_NBW_MGMT_H

#include "hmac_chba_common_function.h"
#include "hmac_chba_mgmt.h"
#include "hmac_nbw_timeslot_mgmt.h"
#include "hihash.h"
#include "hilist.h"
#include "hmac_vap.h"
#include "frw_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_MGMT_H

#ifdef _PRE_WLAN_FEATURE_NBW
#define MAC_NBW_MAX_DEVICE_NUM 32
#define MAC_NBW_MAX_BITMAP_WORD_NUM 4
#define MAX_ISLAND_SLOT_INFO_NUM 8
#define MAX_CONNECTION_SLOT_INFO_NUM 4
#define MAX_ISLAND_CNT 8
#define SLOT_NUM 12
#define NBW_INVALID_IDX 0xff

/* 用户统计信息上报时刻位置枚举 */
typedef enum {
    MAC_NBW_SLAVEAP_SLAVE_MIX_ROLE = 0, /* 设备既是岛主又是设备的情况 */
    MAC_NBW_MASTER_ROLE = 1, /* 后续的值，与chba角色取值保持一致 */
    MAC_NBW_SLAVEAP_ROLE = 2,
    MAC_NBW_SLAVE_ROLE = 3,
    MAC_NBW_ROLE_BUTT,
} hmac_nbw_role_enum;

/* 建链阶段状态枚举 */
typedef enum {
    MAC_NBW_CONNECT_NEGOTIATE = 0, /* 建链协商 */
    MAC_NBW_CONNECT_ASSOC = 1 /* 建链关联过程中，包括时隙申请、同步、关联、加密 */
} hmac_nbw_curr_conn_status_enum;

typedef struct {
    hmac_nbw_uslot_info_stru uslot_info;
    uint8_t shared_flag; /* 是否允许共享，cpe业务申请的时隙该flag要置位 */
} per_slot_stru;

typedef struct {
    uint8_t use_flag;
    uint8_t slave_device_mac[OAL_MAC_ADDR_LEN];
    per_slot_stru per_slot_info;  // 目前先实现一个链路只能申请一次时隙，后续再扩展
} slot_info_stru;

/* nbw单个岛的信息 */
typedef struct {
    uint8_t use_flag;
    uint8_t slaveap_mac[OAL_MAC_ADDR_LEN];
    uint8_t slaveap_rp;
    uint8_t slot_period_indicate_cnt;
    slot_info_stru island_slot_info[MAX_ISLAND_SLOT_INFO_NUM];
    uint8_t sync_flag; /* 该岛主是否回复心跳 */
    uint8_t no_sync_cnt; /* 该岛主未回复心跳的次数 */
    uint64_t last_sync_tsf; /* 该岛主上次同步上的时间 */
} hmac_nbw_per_island_info_stru;
/* nbw岛信息节点结构体 */
typedef struct {
    struct hi_node node;
    hmac_nbw_per_island_info_stru data;
} hmac_nbw_island_list_node_stru;
/* nbw 活跃设备哈希表的结构体 */
typedef struct {
    struct hi_hash_table raw;
    size_t node_cnt;
} hmac_nbw_alive_device_table_stru;

/* nbw维护所有岛信息的结构体 */
typedef struct {
    uint8_t island_cnt;
    struct hi_list island_list;
} hmac_nbw_island_list_stru;

/* 域主维护心跳信息结构体 */
typedef struct {
    uint8_t cur_slave_ap_mac[OAL_MAC_ADDR_LEN];
    /* 超时定时器暂时不定义，另外是否可以直接放在hmac_nbw_master_info_stru里待定 */
} hmac_nbw_slaveap_heartbeat_stru;

typedef struct {
    uint8_t use_flag;
    uint8_t mac_addr[OAL_MAC_ADDR_LEN];
} hmac_nbw_per_device_id_info_stru;

typedef struct {
    hmac_nbw_alive_device_table_stru* alive_device_table; /* nbw活跃设备表，哈希表 */
    /* nbw连接拓扑图，每一行4个uint32对应128bit,正常是最大个数32，所以只需要用第一列 */
    uint32_t network_topo[MAC_NBW_MAX_DEVICE_NUM][MAC_NBW_MAX_BITMAP_WORD_NUM];
    /* 当前域中岛信息维护在此，包括自己所在的岛 */
    hmac_nbw_island_list_stru island_info;
    /* demo阶段先简化，用数组表述有限的岛信息，后续再补充成全部的岛信息 */
    uint8_t island_cnt;
    hmac_nbw_per_island_info_stru island_raw_info[MAX_ISLAND_CNT];
    hmac_nbw_slaveap_heartbeat_stru slaveap_heartbeat_info;
    hmac_nbw_per_device_id_info_stru device_id_info[MAC_NBW_MAX_DEVICE_NUM];
    uint8_t cur_slot_bitmap[SLOT_NUM];  /* 当前已分配的时隙资源情况，用来做时隙分配时用 */
    uint8_t alloced_bitmap; /* master当前已分配的工作时隙 */
} hmac_nbw_master_info_stru;

typedef struct {
    uint8_t slaveap_mac_addr[OAL_MAC_ADDR_LEN];
    per_slot_stru slaveap_slot_info[MAX_ISLAND_SLOT_INFO_NUM * MAX_CONNECTION_SLOT_INFO_NUM];
} other_island_info_stru;

typedef struct {
    uint8_t slot_period_indicate_cnt;
    slot_info_stru island_slot_info[MAX_ISLAND_SLOT_INFO_NUM];
    other_island_info_stru other_island_info[MAX_ISLAND_CNT];
} hmac_nbw_slaveap_island_info_stru;

typedef struct {
    uint8_t use_flag;
    uint8_t slave_ap_mac[OAL_MAC_ADDR_LEN]; /* 该链路的岛主地址 */
    per_slot_stru slot_info; // 目前先实现一个链路只能申请一次时隙，后续再扩展
} slave_slot_stru;

typedef struct {
    uint8_t link_cnt;
    slave_slot_stru device_slot_info[MAX_ISLAND_SLOT_INFO_NUM]; /* 该设备的时隙信息，可能有多个岛主的情况 */
} hmac_nbw_slave_info_stru;

typedef struct {
    uint8_t connect_status; /* 当前连接的阶段, 状态包括0: 建链协商中；1：建链关联中 */
    uint8_t connect_role; /* 设备在当前连接中的角色 */
    uint8_t peer_addr[OAL_MAC_ADDR_LEN]; /* 当前连接的对端设备地址 */
    uint8_t op_id; /* 命令ID */
    int8_t conn_flag;
    mac_status_code_enum_uint16 status_code; /* 反馈给上层的建链结果码 */
    uint32_t connect_timeout; /* 上层下发的连接超时时间 */
    frw_timeout_stru assoc_waiting_timer;
} hmac_nbw_current_connect_info_stru;

typedef struct {
    uint8_t nbw_module_state;
    uint8_t island_flag;
    uint8_t nbw_role;
    uint8_t master_addr[OAL_MAC_ADDR_LEN];
    uint8_t island_owner_valid;
    hmac_nbw_current_connect_info_stru curr_conn_info; /* 当前建链信息，建链完成或者失败后删除 */
    hmac_nbw_master_info_stru nbw_master_info;
    hmac_nbw_slaveap_island_info_stru slave_ap_info;
    hmac_nbw_slave_info_stru slave_info;
} hmac_nbw_mgmt_info_stru;

typedef struct {
    uint8_t timeslot_level;
    uint32_t vap_bitmap;
} hmac_nbw_vap_stru;
extern hmac_nbw_mgmt_info_stru g_nbw_mgmt_info;
extern hmac_nbw_vap_stru g_nbw_vap_info;

static inline hmac_nbw_mgmt_info_stru *hmac_nbw_get_mgmt_info(void)
{
    return &g_nbw_mgmt_info;
}

static inline hmac_nbw_per_device_id_info_stru *hmac_nbw_get_device_id_info(void)
{
    return &g_nbw_mgmt_info.nbw_master_info.device_id_info[0];
}

static inline uint8_t hmac_nbw_get_role(void)
{
    return g_nbw_mgmt_info.nbw_role;
}

static inline void hmac_nbw_set_role(uint8_t val)
{
    g_nbw_mgmt_info.nbw_role = val;
}

static inline uint8_t hmac_nbw_get_available_uslot_bitmap(void)
{
    return g_nbw_mgmt_info.nbw_master_info.alloced_bitmap;
}

static inline void hmac_nbw_set_available_uslot_bitmap(uint8_t val)
{
    g_nbw_mgmt_info.nbw_master_info.alloced_bitmap |= val;
}

static inline void hmac_nbw_resume_uslot_bitmap(uint8_t val)
{
    g_nbw_mgmt_info.nbw_master_info.alloced_bitmap ^= val;
}

static inline hmac_nbw_island_list_stru *hmac_nbw_get_island_info(void)
{
    return &g_nbw_mgmt_info.nbw_master_info.island_info;
}

static inline hmac_nbw_per_island_info_stru *hmac_nbw_get_raw_island_info(void)
{
    return &g_nbw_mgmt_info.nbw_master_info.island_raw_info[0];
}

static inline uint8_t *hmac_nbw_get_master_macaddr(void)
{
    return g_nbw_mgmt_info.master_addr;
}

static inline hmac_nbw_current_connect_info_stru *hmac_nbw_get_curr_conn_info(void)
{
    return &g_nbw_mgmt_info.curr_conn_info;
}

static inline slot_info_stru *hmac_nbw_get_slaveap_island_slot_info(void)
{
    return &g_nbw_mgmt_info.slave_ap_info.island_slot_info[0];
}

static inline slave_slot_stru *hmac_nbw_get_slave_slot_info(void)
{
    return &g_nbw_mgmt_info.slave_info.device_slot_info[0];
}

static inline void hmac_nbw_slaveap_link_cnt_add(void)
{
    g_nbw_mgmt_info.slave_ap_info.slot_period_indicate_cnt++;
}

static inline void hmac_nbw_slaveap_link_cnt_dec(void)
{
    g_nbw_mgmt_info.slave_ap_info.slot_period_indicate_cnt--;
}

static inline void hmac_nbw_slave_link_cnt_add(void)
{
    g_nbw_mgmt_info.slave_info.link_cnt++;
}

static inline void hmac_nbw_slave_link_cnt_dec(void)
{
    g_nbw_mgmt_info.slave_info.link_cnt--;
}

static inline hmac_nbw_vap_stru *hmac_nbw_get_nbw_vap(void)
{
    return &g_nbw_vap_info;
}
oal_bool_enum_uint8 hmac_nbw_vap_start_check(hmac_vap_stru *hmac_vap);
uint8_t hmac_nbw_master_find_island_idx(uint8_t *peer_addr);
uint8_t hmac_nbw_master_unused_idx(void);
uint8_t hmac_nbw_find_unused_link_idx(hmac_nbw_per_island_info_stru *island_info);
uint8_t hmac_nbw_find_island_link_idx(uint8_t *slave_addr);
uint8_t hmac_nbw_find_island_unused_slot_idx(void);
uint8_t hmac_nbw_find_slave_link_slot_idx(uint8_t *peer_addr);
uint8_t hmac_nbw_find_slave_unused_slot_idx(void);
uint8_t hmac_nbw_master_find_link_idx(hmac_nbw_per_island_info_stru *island_info, uint8_t *slave_addr);
uint32_t hmac_nbw_master_adjust_timeslot(uint8_t *peer_addr, uint8_t *slave_addr, uint8_t slave_len,
    hmac_nbw_uslot_info_stru uslot_bitmap_info);
void hmac_nbw_sync_user_bitmap_info(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, uint32_t user_bitmap);
void hmac_nbw_sync_user_bitmap(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, uint32_t user_bitmap);
void hmac_nbw_sync_vap_bitmap_info(mac_vap_stru *mac_vap);
void hmac_update_master_island_info(hmac_nbw_uslot_info_stru uslot_bitmap_info,
    uint8_t *slave_addr, uint8_t *peer_addr);
void hmac_nbw_init_device_info(hmac_chba_device_para_stru *device_info);
void hmac_nbw_release_timeslot(uint8_t *peer_addr, uint8_t peer_len, uint8_t *other_addr, uint8_t other_len);
#endif
#endif
