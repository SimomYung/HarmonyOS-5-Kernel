/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * Description: nbw connect mgnt
 * Author: wifi
 * Create: 2025-01-14
 */

#ifndef HMAC_NBW_CONNECT_H
#define HMAC_NBW_CONNECT_H

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_CONNECT_H

#include "hmac_user.h"
#include "hmac_vap.h"
#include "hmac_chba_interface.h"
#include "hmac_chba_function.h"
#include "hmac_nbw_mgmt.h"

#ifdef _PRE_WLAN_FEATURE_NBW

/* 关联请求参数 */
typedef struct {
    uint8_t nbw_role;
    uint8_t auc_resv[3];
} hmac_nbw_slot_apply_param_stru;

void hmac_nbw_connect_fail_handler(hmac_vap_stru *hmac_vap);
uint32_t hmac_nbw_slave_wait_slot_indicate_timeout(void *arg);
void hmac_nbw_save_curr_conn_info(hmac_chba_conn_param_stru *conn_param, uint8_t connect_role);
void hmac_nbw_clear_curr_conn_info(void);
void hmac_nbw_slaveap_wait_assoc_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_nbw_master_election_finish_proc(void *arg);
void hmac_nbw_slave_sync_set(hmac_vap_stru *hmac_vap);
uint32_t hmac_nbw_apply_slot(hmac_vap_stru *hmac_vap);
void hmac_nbw_slave_config_sync_slaveap(hmac_vap_stru *hmac_vap);
#endif
#endif