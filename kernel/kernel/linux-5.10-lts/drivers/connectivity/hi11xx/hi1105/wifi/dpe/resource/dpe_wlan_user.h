/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_user.c 的头文件
 * 作    者 :
 * 创建日期 : 2024年11月15日
 */

#ifndef DPE_WLAN_USER_H
#define DPE_WLAN_USER_H

#include "wlan_protocol_types.h"
#include "frw_ext_if.h"
#include "dpe_wlan_stat_stru.h"
#include "dpe_wlan_tid.h"

typedef struct {
    hmac_tid_stru *tid_res;
    oal_netbuf_head_stru device_rx_list[WLAN_TID_MAX_NUM];
    dpe_wlan_user_stat_stru user_stats;
    /* user对应资源池索引值; user为STA时，表示填在管理帧中的AID，值为用户的资源池索引值1~32(协议规定范围为1~2007) */
    uint16_t us_assoc_id;
    uint8_t uc_vap_id;
    oal_bool_enum_uint8 en_is_multi_user;
    oal_dlist_head_stru st_user_dlist;              /* 用于用户遍历 */
    oal_dlist_head_stru st_user_hash_dlist;         /* 用于hash查找 */
    uint16_t us_user_hash_idx;                      /* 索引值(关联) */
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN];   /* user对应的mac地址 */
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    uint8_t is_mcast_ampdu;
#endif
    uint8_t lut_index;
    uint8_t ml_user_lut_idx;
    oal_atomic netdev_tx_suspend;
    mac_user_asoc_state_enum_uint8 en_user_asoc_state; /* 用户关联状态 */
#ifdef _PRE_WLAN_FEATURE_ML
    uint16_t ml_user_id;
#endif
    uint8_t uc_is_user_alloced;                        /* 标志此user资源是否已经被申请 */
    oal_bool_enum_uint8 en_port_valid;                 /* 802.1X端口合法性标识 */
} dpe_wlan_user_stru;

void dpe_wlan_lut_index_tbl_init(void);
dpe_wlan_user_stru *dpe_wlan_get_user_by_lut_index(uint8_t lut_idx);
dpe_wlan_user_stru *dpe_wlan_user_get(uint16_t us_idx);
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
void dpe_wlan_user_set_mcast_ampdu(uint16_t us_idx, oal_bool_enum_uint8 mcast_ampdu_enable);
#endif

/* 管理面调用DPE侧相关变量，通过接口实现，DPA架构需要通过事件传递调用 */
void dpe_wlan_user_add(frw_event_mem_stru *event_mem);
void dpe_wlan_multi_user_add(uint16_t us_idx, uint8_t vap_id, uint8_t multi_user_lut_idx);
void dpe_wlan_mcast_ampdu_user_add(frw_event_mem_stru *event_mem);
void dpe_wlan_user_del(frw_event_mem_stru *event_mem);
void dpe_wlan_del_multi_user(uint16_t user_idx);
void dpe_wlan_user_assoc_state_sync(uint16_t user_idx, mac_user_asoc_state_enum_uint8 user_asoc_state);
void dpe_wlan_user_info_sync(uint8_t *param);
void dpe_wlan_user_addr_sync(uint16_t user_idx, uint8_t *addr);
void dpe_user_set_port_sync(uint16_t user_idx, oal_bool_enum_uint8 en_port_valid);
uint32_t dpe_vap_find_user_by_macaddr(uint8_t vap_id, const unsigned char *sta_mac_addr,
    uint16_t *user_idx);
#ifdef _PRE_WLAN_FEATURE_ML
void dpe_wlan_config_h2d_user_change_to_mld_mode(uint8_t *event_mem);
#endif  /* _PRE_WLAN_FEATURE_ML */
#endif  /* DPE_WLAN_USER_H */
