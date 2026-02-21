/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_vap.c 的头文件
 * 作    者 :
 * 创建日期 : 2024年11月15日
 */

#ifndef DPE_WLAN_VAP_H
#define DPE_WLAN_VAP_H

#include "wlan_protocol_types.h"
#include "wlan_types.h"
#include "dpe_wlan_stat_stru.h"
#include "wlan_types.h"
#include "hd_event_vap.h"
#include "wlan_spec_host.h"

#define DPE_VAP_USER_HASH_MAX_VALUE     (WLAN_ASSOC_USER_MAX_NUM * 2) /* 2为扩展因子 */

typedef struct {
    oal_net_device_stru *pst_net_device;            /* VAP对应的net_devices */
    dpe_wlan_vap_stat_stru vap_stats;               /* mp16 trx数据统计 */
    uint16_t us_rx_timeout[WLAN_WME_AC_BUTT];       /* 不同业务重排序超时时间 */
    uint16_t us_rx_timeout_min[WLAN_WME_AC_BUTT];   /* 不同业务重排序超时时间 */
    uint8_t uc_vap_id;                              /* VAP ID */
    uint8_t uc_device_id;                           /* 设备ID */
    uint8_t uc_chip_id;                             /* chip ID */
    uint8_t hal_dev_id;
    uint32_t core_id;
    uint16_t us_multi_user_idx;                     /* 组播用户ID */
    wlan_vap_mode_enum_uint8 en_vap_mode;           /* vap模式 */
    mac_vap_state_enum_uint8 en_vap_state;          /* VAP状态 */
    wlan_p2p_mode_enum_uint8 en_p2p_mode;           /* 0:非P2P设备; 1:P2P_GO; 2:P2P_Device; 3:P2P_CL */
    oal_spin_lock_stru st_cache_user_lock;                          /* cache_user lock */
    uint8_t auc_cache_user_mac_addr[WLAN_MAC_ADDR_LEN];             /* cache user对应的 */
    uint16_t us_cache_user_id;                                      /* cache user对应的userID */
    oal_dlist_head_stru ast_user_hash[DPE_VAP_USER_HASH_MAX_VALUE]; /* hash数组,使用HASH结构内的DLIST */
    oal_dlist_head_stru st_mac_user_list_head;                      /* 关联用户节点双向链表,使用USER结构内的DLIST */
    oal_atomic tx_suspend_user_cnt;
    uint16_t us_assoc_vap_id;       /* VAP为STA模式时保存user(ap)的资源池索引；VAP为AP模式时，不用此成员变量 */
    uint16_t us_user_nums;                          /* VAP下已挂接的用户个数 */
    oal_atomic rx_unicast_pkt_to_lan;               /* 接收到的单播帧个数 */
#ifdef _PRE_WLAN_FEATURE_ML
    uint8_t mld_addr[WLAN_MAC_ADDR_LEN];            /* 本端的MLD地址 */
    uint8_t mld_mode;                               /* 参见枚举 wlan_mld_mode_enum 类型定义 */
    uint8_t mld_assoc_mode;                         /* 标记入网模式：MLSR or STR */
#endif
#ifdef _PRE_WLAN_FEATURE_EMLSR
    uint8_t emlsr_state;  /* 记录emlsr当前工作状态 emlsr_state_enum */
#endif
} dpe_wlan_vap_stru;

static inline uint32_t mac_calculate_hash_value(const unsigned char *mac_addr)
{
    return ((mac_addr[MAC_ADDR_0] + mac_addr[MAC_ADDR_1] + mac_addr[MAC_ADDR_2] +
        mac_addr[MAC_ADDR_3] + mac_addr[MAC_ADDR_4] + mac_addr[MAC_ADDR_5]) &
        (DPE_VAP_USER_HASH_MAX_VALUE - 1));
}

dpe_wlan_vap_stru *dpe_wlan_vap_get(uint16_t us_idx);
void dpe_wlan_config_add_vap(uint8_t *param, uint8_t dev_idx, uint8_t chip_id, uint32_t core_id);
void dpe_wlan_config_start_vap(uint16_t us_idx, mac_cfg_start_vap_param_stru *start_vap_param);
void dpe_wlan_config_del_vap(uint8_t vap_idx, uint8_t *param, oal_net_device_stru *net_dev);
void dpe_wlan_config_del_cfg_vap(uint8_t vap_idx);
void dpe_wlan_config_down_vap(uint8_t vap_idx, mac_cfg_down_vap_param_stru *down_vap_param);
void dpe_vap_init_set_rx_reorder_timeout(dpe_wlan_vap_stru *dpe_vap);
void dpe_wlan_cfg_vap_init(uint8_t dev_idx, uint16_t vap_idx, uint8_t chip_id);

void dpe_wlan_vap_state_sync(uint16_t vap_idx, uint8_t *param);
void dpe_wlan_cfg_vap_net_dev_set(uint8_t cfg_vap_idx, oal_net_device_stru *net_dev);

#ifdef _PRE_WLAN_FEATURE_ML
void dpe_wlan_mlsr_change_netdev_vap_relation(uint8_t cur_vap_idx, uint8_t next_vap_idx,
    oal_net_device_stru *cur_net_dev, oal_net_device_stru *next_net_dev);
void dpe_wlan_vap_mld_assoc_mode_set(uint8_t vap_idx, uint8_t mld_assoc_mode);
void dpe_wlan_vap_mld_mode_set(uint8_t vap_idx, uint8_t mld_mode);
void dpe_wlan_vap_mld_addr_set(uint8_t vap_idx, uint8_t *mld_addr);
#endif  /* _PRE_WLAN_FEATURE_ML */
void dpe_wlan_cfg_vap_net_dev_set(uint8_t cfg_vap_idx, oal_net_device_stru *net_dev);
void dpe_vap_set_assoc_id(uint8_t vap_id, uint16_t assoc_vap_id);
void dpe_wlan_vap_unicast_pkt_reset(uint8_t vap_idx);
uint32_t dpe_wlan_vap_unicast_pkt_get(uint8_t vap_idx);

#ifdef _PRE_WLAN_FEATURE_EMLSR
void dpe_wlan_vap_emlsr_state_sync(uint8_t vap_idx, uint8_t emlsr_state);
#endif  /* _PRE_WLAN_FEATURE_EMLSR */
#ifdef _PRE_WLAN_CHBA_MGMT
void dpe_wlan_dbac_vap_set_rx_reorder_timeout(uint8_t vap_idx, uint8_t is_dbac_running);
#endif  /* _PRE_WLAN_CHBA_MGMT */
#endif  /* DPE_WLAN_VAP_H */