/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : MLD对外提供函数
 * 创建日期   : 2023年10月18日
 */

#ifndef __HMAC_MLD_EXT_H__
#define __HMAC_MLD_EXT_H__
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_vap.h"
#include "hmac_user.h"

#ifdef _PRE_WLAN_FEATURE_ML
extern uint8_t g_mld_assoc_mode;

uint8_t hmac_mld_enable_single_link(void);
uint8_t hmac_mld_enable_eml_cap(void);
uint8_t hmac_mld_enable_eht_double_wifi_disbale_mlo_cap(void);
uint8_t hmac_get_mld_assoc_mode(hmac_vap_stru *hmac_vap);
hmac_mld_manage_stru *hmac_mld_get_mld_vap_manage(hmac_vap_stru *hmac_vap);
uint8_t hmac_mld_get_link_id_by_macaddr(hmac_vap_stru *hmac_vap, uint8_t *mac_addr);
uint8_t hmac_mlo_check_can_enter_state(mac_vap_stru *joining_mac_vap, mac_vap_stru *other_vap);
void hmac_vap_mld_init(hmac_vap_stru *hmac_vap, mac_cfg_add_vap_param_stru *param);
void hmac_mld_vap_deinit(hmac_vap_stru *hmac_vap);
void hmac_mld_user_deinit(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
void hmac_mld_user_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, hmac_add_user_input_stru *user_info);
hmac_vap_stru *hmac_mld_get_link_vap(hmac_vap_stru *hmac_vap);
hmac_vap_stru *hmac_mld_get_master_link_vap(hmac_vap_stru *hmac_vap);
hmac_vap_stru *hmac_mld_get_another_vap(hmac_vap_stru *hmac_vap);
uint32_t hmac_mld_user_get_ml_user_lut_idx(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *ret_ml_lut_idx);
void hmac_ml_add_link_user(hmac_vap_stru *hmac_master_vap);
hmac_mld_user_manage_stru *hmac_vap_find_ml_manage_by_ml_addr(hmac_vap_stru *hmac_vap, unsigned char *ml_mac_addr);
void hmac_mld_vap_mode_change_process(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint32_t hmac_config_mlsr_link_switch(hmac_vap_stru *hmac_vap, uint8_t *param);
uint32_t hmac_mlsr_link_switch(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason);
void hmac_mlsr_link_switch_wake_all_queues(hmac_vap_stru *hmac_vap);
void hmac_mlsr_link_switch_deinit(hmac_vap_stru *hmac_vap);
uint32_t hmac_mlsr_link_switch_timeout(void *arg);
void hmac_mld_destroy_mlsr_link_switch_timer(hmac_vap_stru *hmac_vap);
uint8_t hmac_mlsr_link_switch_check_vap_mld_state(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap);
uint32_t hmac_mlsr_link_switch_check_all_vap_state(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap);
void hmac_mlsr_link_switch_post_action(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    hmac_mld_manage_stru *mld_vap_manage);
uint32_t hmac_mlsr_link_switch_post_proc(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason);
void hmac_mlsr_link_switch_action(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    oal_net_device_stru *netdev, uint8_t reason, uint8_t sub_reason);
uint32_t hmac_mlsr_link_switch_post_event_process(frw_event_mem_stru *event_mem);
void hmac_change_all_vap_to_mld_mode(hmac_vap_stru *hmac_vap, uint8_t ml_link_num);
void hmac_vap_change_to_mld_process(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
void hmac_switch_mlsr_work_link(hmac_vap_stru *hmac_vap, uint8_t *param);
uint8_t hmac_vap_get_mld_mode(hmac_vap_stru *hmac_vap);
void hmac_vap_set_mld_mode(hmac_vap_stru *hmac_vap, uint8_t new_mld_mode);
hmac_vap_stru *hmac_mlsr_get_cur_work_link_vap(hmac_mld_manage_stru *mld_vap_manage);

uint32_t hmac_mld_user_save_link_assoc_status(hmac_user_stru *hmac_user,
    uint8_t link_id, uint16_t status);
uint32_t hmac_mld_get_mlsr_link_sta(hmac_vap_stru **hmac_link_vap_work, hmac_vap_stru **hmac_link_vap_down);
void hmac_mld_update_link_switch_flag(void);
uint32_t hmac_config_al_rx_set_link_vap_param(oal_net_device_stru *net_dev, uint16_t len, uint8_t *param);
void hmac_vap_mld_mode_change(hmac_vap_stru *hmac_vap, mld_change_fsm_event_type_enum mld_event, uint8_t mld_vap_id);
uint8_t *hmac_get_mld_bssid(hmac_vap_stru *hmac_vap);
void hmac_change_all_mld_vap_to_legacy_mode(hmac_vap_stru *hmac_vap, uint32_t ml_link_num);
void hmac_change_all_user_to_legacy_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t ml_link_num);
void hmac_user_update_mld_params(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
void hmac_user_mld_mode_change(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t mld_mode_change_event);
void hmac_add_link_user(hmac_vap_stru *hmac_link_vap);
void hmac_vap_mld_mode_change(hmac_vap_stru *hmac_vap, mld_change_fsm_event_type_enum mld_event, uint8_t mld_vap_id);
void hmac_config_sta_h2d_mld_params_update(mac_vap_stru *mac_vap, hmac_mld_manage_stru *mld_vap_manage);
void hmac_mld_sync_link_switch_cap(hmac_vap_stru *hmac_vap, oal_bool_enum_uint8 value, uint32_t duration);
uint32_t hmac_mld_delete_link(mac_vap_stru *mac_vap);
void hmac_mld_reset_mode_switch_state(hmac_vap_stru *hmac_vap);
uint32_t hmac_config_mld_mode_switch_cmd(mac_vap_stru *mac_vap, uint32_t *param);
uint32_t hmac_config_mld_mode_switch(hmac_vap_stru *hmac_vap, mac_mld_mode_switch_enum next_mld_mode);
uint8_t hmac_mld_get_link_num(hmac_vap_stru *hmac_vap);
uint32_t hmac_emlsr_tx_omn_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_emlsr_action_param_stru *emlsr_param);
void hmac_change_other_link_vap_mode(hmac_vap_stru *hmac_vap, uint32_t ml_link_num,
    mld_change_fsm_event_type_enum mld_change_event);
uint32_t hmac_change_other_link_user_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t change_event);
void hmac_change_vap_mld_mode(hmac_vap_stru *hmac_vap, mld_change_fsm_event_type_enum mld_change_event);
void hmac_change_all_vap_to_legacy_mode(hmac_vap_stru *hmac_vap, uint8_t ml_link_num);
void hmac_change_all_user_to_mld_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t ml_link_num);
void hmac_vap_update_mld_params(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *cur_bss_dscr);
void hmac_mld_update_vap_link_info(hmac_vap_stru *hmac_vap, ml_link_info_stru *ml_link_info);
uint32_t hmac_mld_link_switch_get_tsf_systime(hmac_vap_stru *hmac_vap);
uint8_t hmac_mld_rx_deauth_need_reassoc(hmac_vap_stru *hmac_vap);
uint8_t hmac_mlsr_is_link_switching(void);
#else
static inline void hmac_mld_update_link_switch_flag(void)
{
    return;
}
static inline uint32_t hmac_config_al_rx_set_link_vap_param(oal_net_device_stru *net_dev, uint16_t len, uint8_t *param)
{
    return OAL_SUCC;
}
static inline void hmac_mld_reset_mode_switch_state(hmac_vap_stru *hmac_vap)
{
    return;
}
#endif /* end of _PRE_WLAN_FEATURE_ML */
uint32_t hmac_mlsr_link_switch_event_process(frw_event_mem_stru *event_mem);
uint32_t hmac_mlsr_link_switch_post_event_process(frw_event_mem_stru *event_mem);

#endif /* end of hmac_mld_ext.h */
