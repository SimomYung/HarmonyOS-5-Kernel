/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : hmac mld user的资源管理、配置等相关接口
 * 创建日期   : 2023年10月18日
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_ML
#include "hmac_mld_user.h"
#include "hmac_user.h"
#include "hmac_resource.h"
#include "hmac_mld_ext.h"
#include "hmac_ml_ttlm.h"
#include "hmac_config.h"
#include "hmac_roam_alg.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MLD_USER_C
/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
hmac_mld_user_manage_stru g_hmac_mld_user_manage[WLAN_ASSOC_USER_MAX_NUM];
uint32_t g_mld_user_man_bitmap = 0;
/*****************************************************************************
  3 函数实现
*****************************************************************************/
uint8_t hmac_user_get_mld_mode(hmac_user_stru *hmac_user)
{
    return hmac_user->mld_user_link_info.mld_mode;
}

uint32_t hmac_mld_user_get_ml_user_lut_idx(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *ret_ml_lut_idx)
{
    uint16_t                        mld_user_id;
    hmac_user_stru                 *hmac_mld_user = NULL;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    if (mld_user_link_info->mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_mld_user_get_ml_user_lut_idx::mld_user_manage NULL}");
        return OAL_FAIL;
    }
    if (hmac_vap_is_ml_mode(hmac_vap) == OAL_TRUE) {
        *ret_ml_lut_idx = hmac_user->ml_user_lut_idx;
        return OAL_SUCC;
    }
    if (hmac_vap_is_link_mode(hmac_vap) == OAL_FALSE) {
        return OAL_FAIL;
    }
    mld_user_id = mld_user_link_info->mld_user_manage->ml_user_id;
    hmac_mld_user = mac_res_get_hmac_user(mld_user_id);
    if (hmac_mld_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_mld_user_get_ml_user_lut_idx::hmac_mld_user NULL}");
        return OAL_FAIL;
    }
    if (hmac_user_is_ml_mode(hmac_mld_user)) {
        *ret_ml_lut_idx = hmac_mld_user->ml_user_lut_idx;
        return OAL_SUCC;
    }
    return OAL_FAIL;
}

static void hmac_user_ml_add_link_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_mld_user_manage_stru  *mld_user_manage)
{
    hmac_mld_record_link_info_stru *record_link_info = NULL;
    if (mld_user_manage->mld_link_cnt >= WLAN_MAX_ML_LINK_NUM) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_user_ml_add_link_info::mld_link_cnt[%d]Already > = max[%d]}",
            mld_user_manage->mld_link_cnt, WLAN_MAX_ML_LINK_NUM);
        return;
    }
    record_link_info = &mld_user_manage->record_link_info[mld_user_manage->mld_link_cnt];
    if (hmac_user_is_ml_mode(hmac_user)) {
        record_link_info->is_main_link_user = OAL_TRUE;
    } else {
        record_link_info->is_main_link_user = OAL_FALSE;
    }
    record_link_info->link_vap_id  = hmac_vap->st_vap_base_info.uc_vap_id;
    record_link_info->link_user_id = hmac_user->st_user_base_info.us_assoc_id;
    record_link_info->link_is_valid = OAL_TRUE;
    mld_user_manage->mld_link_cnt++;
}

static void hmac_user_del_record_link_info(hmac_mld_record_link_info_stru  *record_link_info)
{
    record_link_info->link_vap_id  = 0xff;
    record_link_info->link_user_id = 0xffff;
    record_link_info->link_is_valid = OAL_FALSE;
}

static void hmac_user_ml_del_link_info(hmac_vap_stru *hmac_vap, hmac_mld_user_link_info_stru  *mld_user_link_info,
    hmac_mld_user_manage_stru  *mld_user_manage)
{
    uint8_t idx;
    hmac_mld_record_link_info_stru  *record_link_info;
    uint8_t cur_mld_link_cnt = mld_user_manage->mld_link_cnt;
    if (cur_mld_link_cnt <= 0) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_user_ml_del_link_info::mld_link_cnt[%d]Already zero}", cur_mld_link_cnt);
        return;
    }
    for (idx = 0; idx < mld_user_manage->mld_link_cnt; idx++) {
        record_link_info = &mld_user_manage->record_link_info[idx];
        if (record_link_info->link_is_valid == OAL_FALSE) {
            continue;
        }
        if (record_link_info->link_vap_id == hmac_vap->st_vap_base_info.uc_vap_id) {
            hmac_user_del_record_link_info(record_link_info);
            /* 如果不是最后一个vap，则把最后一个vap移动到这个位置，使得该数组是紧凑的 */
            if (idx < (cur_mld_link_cnt - 1)) {
                memcpy_s(record_link_info, sizeof(hmac_mld_record_link_info_stru),
                    &(mld_user_manage->record_link_info[cur_mld_link_cnt - 1]), sizeof(hmac_mld_record_link_info_stru));
                break;
            }
        }
    }
    mld_user_manage->mld_link_cnt--;
}

static void hmac_mld_user_common_info_init(hmac_vap_stru *hmac_vap,
    hmac_mld_user_link_info_stru   *mld_user_link_info, hmac_mld_user_manage_stru   *mld_user_manage)
{
    mld_user_link_info->link_id = hmac_vap->mld_vap_link_info.link_id;
    mld_user_link_info->ap_link_id = hmac_vap->mld_vap_link_info.link_ap_info.link_id; /* user下记录对应ap的link id */
    mld_user_link_info->mld_user_manage = mld_user_manage;
}

void hmac_add_link_user(hmac_vap_stru *hmac_link_vap)
{
    uint16_t user_index;
    hmac_add_user_input_stru slave_user_info;

    /* ap不在master link add user时就add slave link user，而在收到assoc req时add slave link user */
    if (is_legacy_sta(&hmac_link_vap->st_vap_base_info)) {
        hmac_sta_fill_user_info(hmac_link_vap, hmac_link_vap->st_vap_base_info.auc_bssid, &slave_user_info);
        /*
         * sta master link已经配置mld mac地址，slave link不需要再次配置，user mld管理结构体只有一个
         * 注意ap slave link添加user时需要传user mld mac地址，否则找不到对应的user mld管理结构体（因为有多个）
         */
        hmac_user_add(&hmac_link_vap->st_vap_base_info, &slave_user_info, &user_index);
    }
}

void hmac_ml_add_link_user(hmac_vap_stru *hmac_master_vap)
{
    uint8_t link_idx;
    hmac_vap_stru *hmac_link_vap = NULL;
    hmac_mld_manage_stru   *mld_vap_manage = hmac_master_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return;
    }
    if (hmac_vap_is_ml_mode(hmac_master_vap) == OAL_FALSE) {
        return;
    }

    for (link_idx = 0; link_idx < mld_vap_manage->mld_link_cnt; link_idx++) {
        hmac_link_vap = mld_vap_manage->link_vap[link_idx];
        if (hmac_link_vap == NULL) {
            continue;
        }
        if (hmac_vap_is_link_mode(hmac_link_vap) == OAL_TRUE) {
            hmac_add_link_user(hmac_link_vap);
        }
    }
}

static void hmac_mld_vap_add_ml_user_manage(hmac_vap_stru *hmac_vap, hmac_mld_user_manage_stru *mld_user_manage)
{
    hmac_mld_manage_stru     *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_mld_vap_add_ml_user_manage::mld_vap_manage is NULL}");
        return;
    }

    oal_spin_lock_bh(&mld_vap_manage->ml_user_lock);
    if (mac_vap_user_exist(&(mld_user_manage->dlist_entry), &(mld_vap_manage->ml_user_list_head)) == OAL_SUCC) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_mld_vap_add_ml_user_manage::ml user[%d] already exist.}",
                       mld_user_manage->mld_user_man_id);
        oal_spin_unlock_bh(&mld_vap_manage->ml_user_lock);
        return;
    }
    oal_dlist_add_tail(&(mld_user_manage->dlist_entry), &(mld_vap_manage->ml_user_list_head));
    oal_spin_unlock_bh(&mld_vap_manage->ml_user_lock);
}

static hmac_mld_user_manage_stru *hmac_alloc_ml_user_manage(hmac_vap_stru *hmac_vap)
{
    hmac_mld_user_manage_stru      *mld_user_manage;
    uint8_t mld_user_man_id = oal_bit_find_first_zero_four_byte(g_mld_user_man_bitmap);
    if (mld_user_man_id >= WLAN_ASSOC_USER_MAX_NUM) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_mld_user_alloc_user_manage::mgn id[%d]too large}", mld_user_man_id);
        return NULL;
    }
    mld_user_manage =  &g_hmac_mld_user_manage[mld_user_man_id];
    if (mld_user_manage->in_use == OAL_TRUE) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_mld_user_alloc_user_manage::ml reuse mld manage}");
        return NULL;
    }
    memset_s(mld_user_manage, sizeof(hmac_mld_user_manage_stru), 0, sizeof(hmac_mld_user_manage_stru));
    mld_user_manage->in_use = OAL_TRUE;
    mld_user_manage->mld_user_man_id = mld_user_man_id;
    oal_bit_set_bit_four_byte(&g_mld_user_man_bitmap, (oal_bitops)mld_user_man_id);
    hmac_mld_vap_add_ml_user_manage(hmac_vap, mld_user_manage);
    return mld_user_manage;
}

static void hmac_ml_user_manage_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_add_user_input_stru *user_info)
{
    hmac_mld_user_manage_stru  *mld_user_manage = hmac_alloc_ml_user_manage(hmac_vap);
    if (mld_user_manage == NULL) {
        return;
    }
    mld_user_manage->ml_vap_id = hmac_vap->st_vap_base_info.uc_vap_id;
    mld_user_manage->ml_user_id = hmac_user->st_user_base_info.us_assoc_id;

    if (mac_addr_is_zero(user_info->mld_mac_addr)) {
        oal_set_mac_addr(mld_user_manage->mld_addr, user_info->mac_addr); // 非mld sta连接
    } else {
        oal_set_mac_addr(mld_user_manage->mld_addr, user_info->mld_mac_addr); // mld sta连接
    }
}

static void hmac_mld_user_init_log(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_add_user_input_stru *user_info)
{
    hmac_mld_user_manage_stru  *mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;

    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
        "{hmac_mld_user_init::vap_lind_id:%d, vap_mld_mode:%d, user_link_id:%d, user_mld_mode:%d}",
        hmac_vap->mld_vap_link_info.link_id, hmac_vap->mld_vap_link_info.mld_mode,
        hmac_user->mld_user_link_info.link_id, hmac_user->mld_user_link_info.mld_mode);
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
        "{hmac_mld_user_init::user mld addr %02X:XX:XX:XX:%02X:%02X}",
        mld_user_manage->mld_addr[MAC_ADDR_0],
        mld_user_manage->mld_addr[MAC_ADDR_4],
        mld_user_manage->mld_addr[MAC_ADDR_5]);
}

static void hmac_link_user_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, hmac_add_user_input_stru *user_info)
{
    hmac_mld_user_manage_stru      *mld_user_manage;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;

    mld_user_manage = hmac_vap_find_ml_manage_by_ml_addr(hmac_vap, user_info->mld_mac_addr);
    if (mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_link_user_init::mld_user_manage NULL}");
        return;
    }
    hmac_mld_user_common_info_init(hmac_vap, mld_user_link_info, mld_user_manage);
    hmac_user_ml_add_link_info(hmac_vap, hmac_user, mld_user_manage);
    hmac_mld_user_init_log(hmac_vap, hmac_user, user_info);
}

void hmac_mld_user_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, hmac_add_user_input_stru *user_info)
{
    hmac_user->mld_user_link_info.mld_mode = WLAN_LEGACY_MLD_MODE;
    if (mac_addr_is_zero(user_info->mld_mac_addr) == OAL_FALSE) {
        hmac_user->mld_user_link_info.mld_mode = hmac_vap_get_mld_mode(hmac_vap);
    }
    if (hmac_user_is_legacy_mld_mode(hmac_user) == OAL_TRUE) {
        return;
    }
    /* ml vap 必定有ml user管理 */
    if (hmac_vap_is_ml_mode(hmac_vap) == OAL_TRUE) {
        hmac_ml_user_manage_init(hmac_vap, hmac_user, user_info);
    }
    hmac_link_user_init(hmac_vap, hmac_user, user_info);
    /* ml user manage ttlm info 初始化 */
    hmac_mld_user_ttlm_init(hmac_user);
}

static void hmac_free_mld_user_manage(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_manage_stru  *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    hmac_mld_user_manage_stru  *mld_user_manage  = mld_user_link_info->mld_user_manage;
    if (mld_vap_manage == NULL || mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_free_mld_user_manage::mld_vap_manage/mld_user_manage is NULL}");
        return;
    }
    if (mld_user_manage->mld_link_cnt != 0) {
        return;
    }
    oal_bit_clear_bit_four_byte(&g_mld_user_man_bitmap, (oal_bitops)mld_user_manage->mld_user_man_id);
    oal_spin_lock_bh(&mld_vap_manage->ml_user_lock);
    oal_dlist_delete_entry(&(mld_user_manage->dlist_entry));
    oal_spin_unlock_bh(&mld_vap_manage->ml_user_lock);
    memset_s(mld_user_manage, sizeof(hmac_mld_user_manage_stru), 0, sizeof(hmac_mld_user_manage_stru));
}

static void hmac_ml_user_deinit(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    uint8_t                         idx;
    uint8_t                         link_vap_id;
    uint8_t                         link_user_id;
    mac_vap_stru                   *mac_link_vap   = NULL;
    hmac_user_stru                 *hmac_link_user = NULL;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    hmac_mld_user_manage_stru      *mld_user_manage    = mld_user_link_info->mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_ml_user_deinit::mld_user_manage is NULL}");
        return;
    }
    /* TBD reconfig sta & ap ml user删除时不能删link user */
    for (idx = 0; idx < mld_user_manage->mld_link_cnt; idx++) {
        if (mld_user_manage->record_link_info[idx].link_is_valid == OAL_FALSE) {
            continue;
        }
        link_vap_id = mld_user_manage->record_link_info[idx].link_vap_id;
        if (hmac_vap->st_vap_base_info.uc_vap_id == link_vap_id) {
            continue;
        }
        link_user_id = mld_user_manage->record_link_info[idx].link_user_id;
        mac_link_vap = mac_res_get_mac_vap(link_vap_id);
        hmac_link_user = mac_res_get_hmac_user(link_user_id);
        hmac_user_del(mac_link_vap, hmac_link_user);
    }
}

static void hmac_link_user_deinit(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    hmac_mld_user_manage_stru      *mld_user_manage = mld_user_link_info->mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_link_user_deinit::mld_user_manage is NULL}");
        return;
    }
    hmac_user_ml_del_link_info(hmac_vap, mld_user_link_info, mld_user_manage);
    hmac_ttlm_destory_timers(mld_user_manage);
    hmac_free_mld_user_manage(hmac_vap, hmac_user);
    memset_s(mld_user_link_info, sizeof(hmac_mld_user_link_info_stru), 0, sizeof(hmac_mld_user_link_info_stru));
}

void hmac_mld_user_deinit(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_user_link_info_stru   *mld_user_link_info = NULL;

    if (hmac_user_is_legacy_mld_mode(hmac_user) == OAL_TRUE) {
        return;
    }
    mld_user_link_info = &hmac_user->mld_user_link_info;
    if (mld_user_link_info->mld_user_manage == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_mld_user_deinit::mld_user_manage/mld_vap_manage is NULL}");
        return;
    }
    /* TBD识别出ML属性的User不用master标签 */
    if (hmac_user_is_ml_mode(hmac_user)) {
        hmac_ml_user_deinit(hmac_vap, hmac_user);
    }
    hmac_link_user_deinit(hmac_vap, hmac_user);
}

hmac_mld_user_manage_stru *hmac_vap_find_ml_manage_by_ml_addr(hmac_vap_stru *hmac_vap, unsigned char *ml_mac_addr)
{
    hmac_mld_manage_stru        *mld_vap_manage;
    oal_dlist_head_stru         *mld_user_entry = NULL;
    oal_dlist_head_stru         *mld_user_entry_tmp = NULL;
    hmac_mld_user_manage_stru   *mld_user_manage  = NULL;
    hmac_mld_user_manage_stru   *mld_user_manage_tmp  = NULL;

    if (oal_unlikely(oal_any_null_ptr2(hmac_vap, ml_mac_addr))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_vap_find_ml_manage_by_ml_addr::param null.}");
        return NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
            "{hmac_vap_find_ml_manage_by_ml_addr::mld_vap_manage is NULL}");
        return NULL;
    }

    oal_spin_lock_bh(&mld_vap_manage->ml_user_lock);
    oal_dlist_search_for_each_safe(mld_user_entry, mld_user_entry_tmp, &(mld_vap_manage->ml_user_list_head)) {
        mld_user_manage_tmp = oal_dlist_get_entry(mld_user_entry, hmac_mld_user_manage_stru, dlist_entry);
        if (oal_compare_mac_addr(mld_user_manage_tmp->mld_addr, ml_mac_addr) == 0) {
            mld_user_manage = mld_user_manage_tmp;
            break;
        }
    }
    oal_spin_unlock_bh(&mld_vap_manage->ml_user_lock);
    return mld_user_manage;
}

uint32_t hmac_mld_user_save_link_assoc_status(hmac_user_stru *hmac_user,
    uint8_t link_id, uint16_t status)
{
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_UM, "{hmac_mld_user_save_link_assoc_status::user is null.}");
        return OAL_FAIL;
    }

    if (hmac_user->mld_user_link_info.mld_user_manage == NULL) {
        oam_error_log0(0, OAM_SF_UM, "{hmac_mld_user_save_link_assoc_status::mld_user_manage is null.}");
        return OAL_FAIL;
    }

    if (link_id >= WLAN_MAX_ML_LINK_NUM) {
        return OAL_SUCC;
    }

    hmac_user->mld_user_link_info.mld_user_manage->link_assoc_info[link_id].link_status_code = status;
    hmac_user->mld_user_link_info.mld_user_manage->link_assoc_info[link_id].link_assoc_is_valid = OAL_TRUE;

    return OAL_SUCC;
}

void hmac_config_h2d_user_change_to_mld_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    user_mld_mode_change_stru add_mld_user_param;

    if (hmac_user->mld_user_link_info.mld_user_manage == NULL) {
        return;
    }
    add_mld_user_param.mld_mode_change_event = MLD_CHANGE_EVENT_CHANGE_TO_MLD;
    add_mld_user_param.user_idx = hmac_user->st_user_base_info.us_assoc_id;
    add_mld_user_param.mld_mode = hmac_user->mld_user_link_info.mld_mode;
    add_mld_user_param.mld_man_id = hmac_user->mld_user_link_info.mld_user_manage->mld_user_man_id;
    add_mld_user_param.ml_user_id = hmac_user->mld_user_link_info.mld_user_manage->ml_user_id;
    hmac_config_send_event(&(hmac_vap->st_vap_base_info), WLAN_CFGID_CHANGE_USER_MLD_MODE,
        sizeof(user_mld_mode_change_stru), (uint8_t *)&add_mld_user_param);
    dpe_wlan_config_h2d_user_change_to_mld_mode((uint8_t *)&add_mld_user_param);
}

void hmac_config_h2d_user_change_to_legacy_mld_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    user_mld_mode_change_stru add_mld_user_param;

    add_mld_user_param.mld_mode_change_event = MLD_CHANGE_EVENT_CHANGE_TO_LEGACY;
    add_mld_user_param.user_idx = hmac_user->st_user_base_info.us_assoc_id;
    add_mld_user_param.mld_mode = 0xff;
    add_mld_user_param.mld_man_id = 0xff;
    hmac_config_send_event(&(hmac_vap->st_vap_base_info), WLAN_CFGID_CHANGE_USER_MLD_MODE,
        sizeof(user_mld_mode_change_stru), (uint8_t *)&add_mld_user_param);
}

void hmac_user_mld_to_legacy_mode_change(hmac_vap_stru *hmac_link_vap, hmac_user_stru *hmac_link_user,
    uint8_t mld_mode_change_event)
{
    hmac_link_user_deinit(hmac_link_vap, hmac_link_user);
    hmac_config_h2d_user_change_to_legacy_mld_mode(hmac_link_vap, hmac_link_user);
}

static void hmac_mld_user_mode_change_null(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t mld_mode_change_event)
{
    uint8_t cur_mld_mode = hmac_user_get_mld_mode(hmac_user);
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{hmac_mld_user_mode_change_null::%d mode event%d no func", cur_mld_mode, mld_mode_change_event);
}

static void hmac_user_legacy_change_to_ml_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t mld_mode_change_event)
{
    hmac_add_user_input_stru user_info;

    hmac_sta_fill_user_info(hmac_vap, hmac_vap->st_vap_base_info.auc_bssid, &user_info);
    hmac_mld_user_init(hmac_vap, hmac_user, &user_info);
    hmac_config_h2d_user_change_to_mld_mode(hmac_vap, hmac_user);
}

typedef void (*hmac_mld_user_mode_change)(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t mld_mode_change_event);
static hmac_mld_user_mode_change g_hmac_mld_user_mode_change_func[WLAN_MLD_MODE_BUTT][MLD_CHANGE_FSM_EVENT_BUTT] = {
    [WLAN_LEGACY_MLD_MODE] =
         /* CHANGE_TO_LEGACY                    CHANGE_TO_MLD                        CHANGE_TO_LINK */
        {hmac_mld_user_mode_change_null, hmac_user_legacy_change_to_ml_mode, hmac_user_legacy_change_to_ml_mode},
    [WLAN_ML_MODE] =
        {hmac_user_mld_to_legacy_mode_change, hmac_mld_user_mode_change_null, hmac_mld_user_mode_change_null},
    [WLAN_LINK_MODE] =
        {hmac_user_mld_to_legacy_mode_change, hmac_mld_user_mode_change_null, hmac_mld_user_mode_change_null},
    [WLAN_ML_LINK_MODE] =
        {hmac_user_mld_to_legacy_mode_change, hmac_mld_user_mode_change_null, hmac_mld_user_mode_change_null},
    };

void hmac_user_mld_mode_change(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t mld_mode_change_event)
{
    uint8_t new_mld_mode;
    uint8_t cur_mld_mode = hmac_user_get_mld_mode(hmac_user);
    g_hmac_mld_user_mode_change_func[cur_mld_mode][mld_mode_change_event](hmac_vap, hmac_user, mld_mode_change_event);
    new_mld_mode = hmac_user_get_mld_mode(hmac_user);
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{hmac_user_mld_mode_change::mode event[%d]change user mld[%d]->[%d]",
        mld_mode_change_event, cur_mld_mode, new_mld_mode);
}

uint32_t hmac_change_other_link_user_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t change_event)
{
    uint8_t link_id;
    hmac_user_stru *hmac_link_user;
    hmac_vap_stru *link_hmac_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL || link_hmac_vap == hmac_vap) {
            continue;
        }
        hmac_link_user = mac_res_get_hmac_user(link_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        if (hmac_link_user == NULL) {
            continue;
        }
        hmac_user_mld_mode_change(link_hmac_vap, hmac_link_user, change_event);
    }
    return OAL_SUCC;
}

void hmac_change_all_user_to_legacy_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t ml_link_num)
{
    hmac_user_mld_mode_change(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    if (ml_link_num > WLAN_ML_SINGLE_RADIO) {
        hmac_change_other_link_user_mode(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    }
}

void hmac_change_all_user_to_mld_mode(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t ml_link_num)
{
    hmac_user_mld_mode_change(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    if (ml_link_num > WLAN_ML_SINGLE_RADIO) {
        hmac_change_other_link_user_mode(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    }
}

void hmac_user_update_mld_params(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_user_manage_stru  *mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    hmac_mld_manage_stru       *mld_vap_manage  = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL || mld_user_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_update_mld_params::mld_vap_manage || mld_user_manage null");
        return;
    }
    memcpy_s(mld_user_manage->mld_addr, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);
}
#endif
