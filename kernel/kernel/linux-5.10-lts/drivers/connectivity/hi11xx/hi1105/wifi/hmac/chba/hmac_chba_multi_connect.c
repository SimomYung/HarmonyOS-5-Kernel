/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 * 功能说明 : chba并行连接相关文件
 * 作    者 : y30038709
 * 创建日期 : 2024年5月6日
 */
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_function.h"
#include "hmac_chba_user.h"
#endif
#include "hmac_config.h"
#include "oam_event_wifi.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CHBA_MULTI_CONNECT_C

#define MAC_CHBA_MAX_ASSOC_PRIV_IE_CACHE_NUMS 10 /* 当前最大支持保存10份chba私有app ie */

#ifdef _PRE_WLAN_CHBA_MGMT
hmac_chba_connect_list_stru g_chba_assoc_priv_ie_list = { 0 }; /* chba私有数据链表 */
hmac_chba_connect_list_stru g_chba_whitelist = { 0 }; /* chba连接白名单数据链表 */

/* 功能描述: 初始化chba白名单 */
void hmac_chba_whitelist_init(void)
{
    hmac_chba_connect_list_stru *chba_whitelist = &g_chba_whitelist;

    /* 清空白名单 */
    hmac_chba_whitelist_clear();

    oal_dlist_init_head(&(chba_whitelist->datalist_head));
    chba_whitelist->list_len = 0;
}

/* 功能描述: 将peer_addr添加到白名单中 */
uint32_t hmac_chba_whitelist_add_user(uint8_t *peer_addr, uint8_t addr_len)
{
    hmac_chba_connect_list_stru *chba_whitelist = &g_chba_whitelist;
    whitelist_cache_stru *new_user = NULL;

    /* 白名单长度不得超过最大支持chba user个数 */
    if (chba_whitelist->list_len >= MAC_CHBA_MAX_LINK_NUM) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_chba_whitelist_add_user:len[%d]}",
            chba_whitelist->list_len);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    new_user = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, sizeof(whitelist_cache_stru), OAL_TRUE);
    if (new_user == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_chba_whitelist_add_user: mem_alloc fail}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    oal_set_mac_addr(new_user->peer_mac_addr, peer_addr);
    oal_dlist_add_head(&(new_user->st_entry), &(chba_whitelist->datalist_head));
    chba_whitelist->list_len++;

    oam_warning_log4(0, OAM_SF_ANY,
        "hmac_chba_whitelist_add_user::add peer[%02X:XX:XX:XX:%02X:%02X] into whitelist, list_len[%d]",
        peer_addr[MAC_ADDR_0], peer_addr[MAC_ADDR_4], peer_addr[MAC_ADDR_5], chba_whitelist->list_len);
    return OAL_SUCC;
}

/* 功能描述: 查找peer_addr是否在白名单中 */
uint32_t hmac_chba_whitelist_check(uint8_t *peer_addr, uint8_t addr_len)
{
    hmac_chba_connect_list_stru *chba_whitelist = &g_chba_whitelist;
    whitelist_cache_stru *whitelist_cache = NULL;
    oal_dlist_head_stru *whitelist_entry = NULL;
    oal_dlist_head_stru *whitelist_entry_tmp = NULL;

    if (oal_dlist_is_empty(&(chba_whitelist->datalist_head))) {
        oam_warning_log0(0, OAM_SF_ANY, "hmac_chba_whitelist_check:whitelist is empty");
        return OAL_FAIL;
    }

    oal_dlist_search_for_each_safe(whitelist_entry, whitelist_entry_tmp, &(chba_whitelist->datalist_head))
    {
        whitelist_cache = oal_dlist_get_entry(whitelist_entry, whitelist_cache_stru, st_entry);
        if (oal_memcmp(whitelist_cache->peer_mac_addr, peer_addr, OAL_MAC_ADDR_LEN) == 0) {
            oam_warning_log3(0, OAM_SF_ANY,
                "{hmac_chba_whitelist_check:whitelist[%02x:XX:XX:XX:%02x:%02x]}",
                peer_addr[MAC_ADDR_0], peer_addr[MAC_ADDR_4], peer_addr[MAC_ADDR_5]);
            return OAL_SUCC;
        }
    }

    oam_warning_log3(0, OAM_SF_ANY,
        "{hmac_chba_whitelist_check:fail to find [%02x:XX:XX:XX:%02x:%02x]}",
        peer_addr[MAC_ADDR_0], peer_addr[MAC_ADDR_4], peer_addr[MAC_ADDR_5]);
    return OAL_FAIL;
}

/* 功能描述  : 从白名单中删除peer_addr */
void hmac_chba_whitelist_del_user(uint8_t *peer_addr, uint8_t addr_len)
{
    hmac_chba_connect_list_stru *chba_whitelist = &g_chba_whitelist;
    whitelist_cache_stru *whitelist_cache = NULL;
    oal_dlist_head_stru *whitelist_entry = NULL;
    oal_dlist_head_stru *whitelist_entry_tmp = NULL;

    if (oal_dlist_is_empty(&(chba_whitelist->datalist_head))) {
        return;
    }

    oal_dlist_search_for_each_safe(whitelist_entry, whitelist_entry_tmp, &(chba_whitelist->datalist_head))
    {
        whitelist_cache = oal_dlist_get_entry(whitelist_entry, whitelist_cache_stru, st_entry);
        if (oal_memcmp(whitelist_cache->peer_mac_addr, peer_addr, OAL_MAC_ADDR_LEN) == 0) {
            oal_dlist_delete_entry(whitelist_entry);
            oal_mem_free_m(whitelist_cache, OAL_TRUE);
            if (chba_whitelist->list_len == 0) {
                oam_error_log0(0, OAM_SF_ANY, "{hmac_chba_whitelist_del_user:list_len is 0!}");
            } else {
                chba_whitelist->list_len--;
            }
            oam_warning_log4(0, 0, "hmac_chba_whitelist_del_user:del [%02X:XX:XX:XX:%02X:%02X] in whitelist.len[%d]",
                peer_addr[MAC_ADDR_0], peer_addr[MAC_ADDR_4], peer_addr[MAC_ADDR_5], chba_whitelist->list_len);
            return;
        }
    }
}

/* 功能描述: 清空chba白名单 */
void hmac_chba_whitelist_clear(void)
{
    hmac_chba_connect_list_stru *chba_whitelist = &g_chba_whitelist;
    whitelist_cache_stru *whitelist_cache = NULL;
    oal_dlist_head_stru *whitelist_entry = NULL;
    oal_dlist_head_stru *whitelist_entry_tmp = NULL;

    if (oal_dlist_is_empty(&(chba_whitelist->datalist_head))) {
        chba_whitelist->list_len = 0;
        return;
    }

    oal_dlist_search_for_each_safe(whitelist_entry, whitelist_entry_tmp, &(chba_whitelist->datalist_head))
    {
        whitelist_cache = oal_dlist_get_entry(whitelist_entry, whitelist_cache_stru, st_entry);
        oal_dlist_delete_entry(whitelist_entry);
        oal_mem_free_m(whitelist_cache, OAL_TRUE);
    }
    chba_whitelist->list_len = 0;
}

/* 功能描述: 初始化chba私有数据链表 */
void hmac_chba_assoc_priv_ie_list_init(void)
{
    hmac_chba_connect_list_stru *assoc_priv_ie_list = &g_chba_assoc_priv_ie_list;

    /* 初始化前白名单非空 */
    if ((assoc_priv_ie_list->list_len != 0) ||
        (oal_dlist_is_empty(&(assoc_priv_ie_list->datalist_head)) == OAL_FALSE)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_chba_assoc_priv_ie_list_init:privdata_list is not empty, len[%d]}",
            assoc_priv_ie_list->list_len);
        hmac_chba_assoc_priv_ie_list_clear();
    }
    oal_dlist_init_head(&(assoc_priv_ie_list->datalist_head));
    assoc_priv_ie_list->list_len = 0;
}

/* 功能描述: 保存上层下发私有chba连接app ie */
uint32_t hmac_chba_save_assoc_priv_ie(hmac_vap_stru *hmac_vap, hmac_chba_conn_param_stru *chba_conn_params)
{
    int32_t ret;
    hmac_chba_assoc_priv_ie_cache_stru *new_cache = NULL;
    hmac_chba_connect_list_stru *assoc_priv_ie_list = &g_chba_assoc_priv_ie_list;

    /* 非极速连接模式无需保存私有app ie */
    if (hmac_chba_check_fast_connect_flag(chba_conn_params->conn_flag) == OAL_FALSE) {
        return OAL_SUCC;
    }

    /* 保存的私有app ie已达上限 */
    if (assoc_priv_ie_list->list_len >= MAC_CHBA_MAX_ASSOC_PRIV_IE_CACHE_NUMS) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_chba_save_assoc_priv_ie:len[%d]}",
            assoc_priv_ie_list->list_len);
        chba_conn_params->status_code = MAC_CHBA_ASSOC_PRIV_IE_FULL;
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    new_cache = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, sizeof(hmac_chba_assoc_priv_ie_cache_stru), OAL_TRUE);
    if (new_cache == NULL) {
        chba_conn_params->status_code = MAC_CHBA_ASSOC_PRIV_IE_SAVE_FAIL;
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    oal_set_mac_addr(new_cache->peer_mac_addr, chba_conn_params->peer_addr);
    ret = memcpy_s(new_cache->assoc_priv_ie, HMAC_CHBA_USER_ASSOC_PRIV_IE_MAX_LEN, chba_conn_params->assoc_priv_ie,
        chba_conn_params->assoc_priv_ie_len);
    if (ret != EOK) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_save_assoc_priv_ie:memcpy assoc_priv_ie fail ,ret[%d]}", ret);
        oal_mem_free_m(new_cache, OAL_TRUE);
        chba_conn_params->status_code = MAC_CHBA_ASSOC_PRIV_IE_SAVE_FAIL;
        return OAL_FAIL;
    }
    new_cache->data_len = chba_conn_params->assoc_priv_ie_len;
    oal_dlist_add_head(&(new_cache->st_entry), &(assoc_priv_ie_list->datalist_head));
    assoc_priv_ie_list->list_len++;
    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "hmac_chba_save_assoc_priv_ie::add[%02X:XX:XX:XX:%02X:%02X] assoc_priv_ie into datalist, list_len[%d]",
        chba_conn_params->peer_addr[MAC_ADDR_0], chba_conn_params->peer_addr[MAC_ADDR_4],
        chba_conn_params->peer_addr[MAC_ADDR_5], assoc_priv_ie_list->list_len);
    return OAL_SUCC;
}

/* 功能描述: 根据mac地址查找chba连接私有app ie */
uint8_t *hmac_chba_find_assoc_priv_ie(const unsigned char *user_mac_addr, uint32_t *app_ie_len)
{
    hmac_chba_assoc_priv_ie_cache_stru *cache = NULL;
    oal_dlist_head_stru *privdata_entry = NULL;
    oal_dlist_head_stru *privdata_entry_tmp = NULL;
    hmac_chba_connect_list_stru *assoc_priv_ie_list = &g_chba_assoc_priv_ie_list;

    *app_ie_len = 0;
    if (oal_dlist_is_empty(&(assoc_priv_ie_list->datalist_head))) {
        return NULL;
    }

    /* 根据mac地质查找对应私有app ie */
    oal_dlist_search_for_each_safe(privdata_entry, privdata_entry_tmp, &(assoc_priv_ie_list->datalist_head))
    {
        cache = oal_dlist_get_entry(privdata_entry, hmac_chba_assoc_priv_ie_cache_stru, st_entry);
        if (oal_memcmp(cache->peer_mac_addr, user_mac_addr, OAL_MAC_ADDR_LEN) == 0) {
            *app_ie_len = cache->data_len;
            return cache->assoc_priv_ie;
        }
    }
    return NULL;
}

/* 功能描述: 删除上层下发私有chba连接app ie */
void hmac_chba_del_assoc_priv_ie(uint8_t *user_mac_addr, uint8_t addr_len)
{
    hmac_chba_assoc_priv_ie_cache_stru *cache = NULL;
    oal_dlist_head_stru *privdata_entry = NULL;
    oal_dlist_head_stru *privdata_entry_tmp = NULL;
    hmac_chba_connect_list_stru *assoc_priv_ie_list = &g_chba_assoc_priv_ie_list;

    if (oal_dlist_is_empty(&(assoc_priv_ie_list->datalist_head))) {
        return;
    }

    /* 根据mac地质查找对应私有app ie */
    oal_dlist_search_for_each_safe(privdata_entry, privdata_entry_tmp, &(assoc_priv_ie_list->datalist_head))
    {
        cache = oal_dlist_get_entry(privdata_entry, hmac_chba_assoc_priv_ie_cache_stru, st_entry);
        if (oal_memcmp(cache->peer_mac_addr, user_mac_addr, OAL_MAC_ADDR_LEN) == 0) {
            oal_dlist_delete_entry(privdata_entry);
            oal_mem_free_m(cache, OAL_TRUE);
            if (assoc_priv_ie_list->list_len == 0) {
                oam_error_log0(0, OAM_SF_ANY, "{hmac_chba_del_assoc_priv_ie:list_len is 0!}");
            } else {
                assoc_priv_ie_list->list_len--;
            }
            oam_warning_log4(0, OAM_SF_ANY, "hmac_chba_del_assoc_priv_ie:del peer[%02X:XX:XX:XX:%02X:%02X], len[%d]",
                user_mac_addr[MAC_ADDR_0], user_mac_addr[MAC_ADDR_4], user_mac_addr[MAC_ADDR_5],
                assoc_priv_ie_list->list_len);
        }
    }
}

/* 功能描述: 清空chba私有ie链表 */
void hmac_chba_assoc_priv_ie_list_clear(void)
{
    hmac_chba_assoc_priv_ie_cache_stru *cache = NULL;
    oal_dlist_head_stru *privdata_entry = NULL;
    oal_dlist_head_stru *privdata_entry_tmp = NULL;
    hmac_chba_connect_list_stru *assoc_priv_ie_list = &g_chba_assoc_priv_ie_list;

    if (oal_dlist_is_empty(&(assoc_priv_ie_list->datalist_head))) {
        assoc_priv_ie_list->list_len = 0;
        return;
    }

    /* 根据mac地质查找对应私有app ie */
    oal_dlist_search_for_each_safe(privdata_entry, privdata_entry_tmp, &(assoc_priv_ie_list->datalist_head))
    {
        cache = oal_dlist_get_entry(privdata_entry, hmac_chba_assoc_priv_ie_cache_stru, st_entry);
        oal_dlist_delete_entry(privdata_entry);
        oal_mem_free_m(cache, OAL_TRUE);
    }
    assoc_priv_ie_list->list_len = 0;
    oam_warning_log0(0, OAM_SF_ANY, "{hmac_chba_assoc_priv_ie_list_clear:free list}");
}

/* 功能描述: 结束chba user入网流程 */
OAL_STATIC uint32_t hmac_chba_cancel_response_connect_stop_connect(hmac_vap_stru *hmac_vap,
    hmac_chba_cancel_conn_notify_stru *chba_cancel_conn_info)
{
    uint32_t ret;
    uint16_t user_idx;
    hmac_user_stru *hmac_user = NULL;
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info);
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    hmac_chba_conn_param_stru conn_param = { 0 };

    /* chba2.0极速连接响应方正在等待入网, 直接结束此流程 */
    if ((hmac_chba_is_not_verified_fast_connect_response(chba_vap_info) == OAL_TRUE) &&
        (oal_memcmp(chba_cancel_conn_info->peer_mac_addr, BROADCAST_MACADDR, OAL_MAC_ADDR_LEN) == 0)) {
        hmac_chba_not_verified_fast_conn_rsp_finish_proc((void *)hmac_vap);
        return OAL_SUCC;
    }

    /* 如果该mac地址user已添加, 则结束其入网流程并删除该user  */
    ret = mac_vap_find_user_by_macaddr(mac_vap, chba_cancel_conn_info->peer_mac_addr, &user_idx);
    if (ret != OAL_SUCC) { /* user已被删除或还未被添加, 所有处理已完成，直接返回 */
        return OAL_SUCC;
    }
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ASSOC,
            "{hmac_chba_cancel_response_connect::hmac_user[%d] null.}", user_idx);
        return OAL_FAIL;
    }

    /* user已成功入网, 无法删除 */
    if ((hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) ||
        (hmac_user->chba_user.connect_info.connect_role != CHBA_CONN_RESPONDER)) {
        oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_ASSOC,
            "{hmac_chba_cancel_response_connect::hmac_user[%d] assoc_state[%d], connect_role[%d]}",
            user_idx, hmac_user->st_user_base_info.en_user_asoc_state, hmac_user->chba_user.connect_info.connect_role);
        return OAL_SUCC;
    }

    /* 结束hmac_user入网准备动作 */
    if (hmac_user->chba_user.connect_info.assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer));
    }
    conn_param.op_id = chba_cancel_conn_info->op_id;
    conn_param.status_code = MAC_CHBA_CANCEL_CONNECT_RESPONDER;
    oal_set_mac_addr(conn_param.peer_addr, hmac_user->st_user_base_info.auc_user_mac_addr);
    hmac_chba_fail_to_connect(hmac_vap, hmac_user, &conn_param, CHBA_CONN_RESPONDER);
    return OAL_SUCC;
}

/* 功能描述: CancelConnectNotify命令处理接口 */
uint32_t hmac_chba_cancel_response_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_chba_cancel_conn_notify_stru *chba_cancel_conn_info = NULL;

    if (oal_any_null_ptr2(mac_vap, params)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        return OAL_FAIL;
    }
    chba_cancel_conn_info = (hmac_chba_cancel_conn_notify_stru *)params;

    /* 从chba私有app ie链表中删除对应mac的数据 */
    hmac_chba_del_assoc_priv_ie(chba_cancel_conn_info->peer_mac_addr, OAL_MAC_ADDR_LEN);

    /* 如果对于mac的user正处于入网流程中, 则结束其入网流程 */
    return hmac_chba_cancel_response_connect_stop_connect(hmac_vap, chba_cancel_conn_info);
}

/* 功能描述: 通过user id查找chba极速连接响应端添加的未校验user信息 */
OAL_STATIC uint16_t *hmac_chba_find_not_verified_info_by_user_id(hmac_chba_vap_stru *chba_vap_info,
    uint16_t user_idx)
{
    uint8_t idx;
 
    for (idx = 0; idx < MAC_RES_MAX_USER_LIMIT; idx++) {
        if (chba_vap_info->fast_conn_rsp_info.not_verified_user_id[idx] == user_idx) {
            return &(chba_vap_info->fast_conn_rsp_info.not_verified_user_id[idx]);
        }
    }
    return NULL;
}

/* 功能描述: 判断是否是极速连接响应方添加的未校验user */
oal_bool_enum_uint8 hmac_chba_is_not_verified_user(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx)
{
    return (hmac_chba_find_not_verified_info_by_user_id(chba_vap_info, user_idx) != NULL);
}

/* 功能描述: 保存未校验user_idx */
void hmac_chba_save_not_verified_user_id(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx)
{
    /* 未被使用的not_verified_user_id被初始化为invalid_user_id, 查找一个未被使用的not_verified_user_id用于保存 */
    uint16_t *save_user_info =
        hmac_chba_find_not_verified_info_by_user_id(chba_vap_info, CHBA_NOT_VERIFIED_USER_ID_INIT);
    if (save_user_info == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "{hmac_chba_save_not_verified_user_id:fail to save user_idx}");
        return;
    }
    *save_user_info = user_idx;
}

/* 功能描述: 删除未校验user_idx */
void hmac_chba_del_not_verified_user_id(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx)
{
    uint16_t *save_user_info = hmac_chba_find_not_verified_info_by_user_id(chba_vap_info, user_idx);
    if (save_user_info == NULL) {
        return;
    }
    *save_user_info = CHBA_NOT_VERIFIED_USER_ID_INIT; /* 将其初始化视为被删除 */
}

/* 功能描述: 清理一个极速连接响应端信息 */
void hmac_chba_clear_one_fast_conn_rsp_info(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info,
    hmac_user_stru *hmac_user)
{
    uint8_t is_fast_conn_rsp = OAL_FALSE;
    oal_dlist_head_stru *dlist_entry = NULL;
    oal_dlist_head_stru *dlist_tmp = NULL;
    mac_user_stru *user_tmp = NULL;
    hmac_user_stru *hmac_user_tmp = NULL;

    /* 当前不处理极速连接响应状态, 无需处理 */
    if (hmac_chba_is_fast_connect_response(hmac_vap) == OAL_FALSE) {
        return;
    }

    /* 清理响应端对应user信息, mac地址为全F的响应端没有hmac_user只需要清理not_verified_rsp_conn_mode标记 */
    if (hmac_user != NULL) {
        hmac_chba_user_set_connect_role(hmac_user, CHBA_CONN_ROLE_BUTT, CHBA_INVALID_CONNECT_FLAG);
        hmac_chba_del_not_verified_user_id(chba_vap_info, hmac_user->st_user_base_info.us_assoc_id);
    } else {
        chba_vap_info->fast_conn_rsp_info.not_verified_rsp_conn_mode = CHBA_NOT_VERIFIED_CONN_RSP_NOT_RUNNING;
    }

    /* 检查当前是否还有user处于极速连接响应端状态, 如果已经没有了则下发事件至dmac退出极速连接等待状态 */
    if (hmac_chba_is_not_verified_fast_connect_response(chba_vap_info)) { /* 特殊极速连接响应还未结束 */
        is_fast_conn_rsp = OAL_TRUE;
    } else { /* 轮询全部chba user, 检查是否有极速响应端 */
        oal_dlist_search_for_each_safe(dlist_entry, dlist_tmp, &(hmac_vap->st_vap_base_info.st_mac_user_list_head))
        {
            user_tmp = oal_dlist_get_entry(dlist_entry, mac_user_stru, st_user_dlist);
            if (user_tmp == NULL) {
                continue;
            }
            hmac_user_tmp = mac_res_get_hmac_user(user_tmp->us_assoc_id);
            if (hmac_user_tmp == NULL) {
                oam_error_log1(0, OAM_SF_CFG, "{hmac_chba_clear_one_fast_conn_rsp_info:error user_id[%d]}",
                    user_tmp->us_assoc_id);
                continue;
            }
            if ((hmac_chba_check_fast_connect_flag(hmac_user_tmp->chba_user.connect_info.conn_flag) == OAL_TRUE) &&
                (hmac_user_tmp->chba_user.connect_info.connect_role == CHBA_CONN_RESPONDER)) {
                is_fast_conn_rsp = OAL_TRUE;
                break;
            }
        }
    }
    if (is_fast_conn_rsp == OAL_FALSE) { /* 全部极速连接响应已结束, 告知dmac退出极速连接响应状态 */
        memset_s(&chba_vap_info->fast_conn_rsp_info, sizeof(chba_vap_info->fast_conn_rsp_info), 0,
            sizeof(chba_vap_info->fast_conn_rsp_info));
        hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHBA_CONNECT_PREPARE,
            sizeof(uint8_t), &is_fast_conn_rsp);
    }
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "{hmac_chba_clear_one_fast_conn_rsp_info:clear user[%d], is_fast_conn_rsp[%d]",
        (hmac_user == NULL) ? CHBA_NOT_VERIFIED_USER_ID_INIT : hmac_user->st_user_base_info.us_assoc_id,
        is_fast_conn_rsp);
}
#endif
