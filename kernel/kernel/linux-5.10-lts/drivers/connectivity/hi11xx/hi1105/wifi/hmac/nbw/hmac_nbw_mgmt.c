/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw island domain device mgmt
 * Author: wifi
 * Create: 2024-12-26
 */
#include "mac_nbw_common.h"
#include "hmac_config.h"
#include "hmac_chba_function.h"
#include "hmac_nbw_frame.h"
#include "hmac_nbw_mgmt.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_MGMT_C

#ifdef _PRE_WLAN_FEATURE_NBW
hmac_nbw_mgmt_info_stru g_nbw_mgmt_info = {
    .nbw_role = MAC_NBW_ROLE_BUTT,
};
hmac_nbw_vap_stru g_nbw_vap_info = {
    .timeslot_level = MAC_NBW_USLOT_NUM,
};
#ifdef _PRE_WLAN_FEATURE_NBW
void hmac_nbw_init_device_info(hmac_chba_device_para_stru *device_info)
{
    if ((g_expand_feature_switch_bitmap & CUSTOM_NBW_CAP) != 0) {
        device_info->config_para.nbw_uslot_cnt_per_slot = MAC_NBW_USLOT_NUM;
        device_info->config_para.nbw_uslot_duaration = MAC_NBW_USLOT_DURATION;
        device_info->config_para.schedule_period = MAC_NBW_SCHED_PERIOD;
        device_info->config_para.slot_duration = MAC_NBW_SLOT_DURATION;
    }
}
#endif

oal_bool_enum_uint8 hmac_nbw_vap_start_check(hmac_vap_stru *hmac_vap)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;

    if (hmac_vap == NULL) {
        return OAL_FALSE;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);
    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if ((mac_vap->chba_mode == CHBA_MODE) && (mac_vap->nbw_mode == NBW_MODE)) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}


/*
 * 功能描述  : 生成user 同步信息并同步
 *   日    期  : 2021年3月25日
 *   作    者  : wifi
 */
void hmac_nbw_sync_user_bitmap_info(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, uint32_t user_bitmap)
{
    uint32_t ret;
    nbw_user_timeslot_bitmap_cfg_stru user_ps_cfg = { 0 };

    /* 将user同步信息初始化后, 同步给dmac */
    user_ps_cfg.user_idx = hmac_user->st_user_base_info.us_assoc_id;
    user_ps_cfg.bitmap = user_bitmap;
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_NBW_SET_USER_BITMAP, sizeof(nbw_user_timeslot_bitmap_cfg_stru),
        (uint8_t *)&user_ps_cfg);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_NBW, "hmac_nbw_sync_user_bitmap_info::send_event fail[%d]", ret);
    }
}

void hmac_nbw_sync_user_bitmap(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint32_t user_bitmap)
{
    /* 暂时先继续沿用chba_vap，挂在下面，用nbw_mode区分 */
    hmac_chba_vap_stru *chba_vap_info = NULL;
    chba_vap_info = hmac_chba_get_chba_vap_info(hmac_vap);
    if (chba_vap_info == NULL || hmac_vap->st_vap_base_info.nbw_mode != NBW_MODE) {
        return;
    }

    hmac_nbw_sync_user_bitmap_info(&(hmac_vap->st_vap_base_info), hmac_user, user_bitmap);
}

void hmac_nbw_sync_vap_bitmap_info(mac_vap_stru *mac_vap)
{
    uint32_t ret, bitmap;
    bitmap = hmac_nbw_get_slot_bitmap();

    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_NBW_SET_VAP_BITMAP, sizeof(uint32_t), (uint8_t *)&bitmap);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_sync_vap_bitmap_info:: send event fail");
    }
}

/*
 * 功能描述  : 检查此次时隙调整是接受还是拒绝
 * 日    期  : 2025年01月17日
 */
static uint32_t hmac_nbw_check_timeslot_adjust(hmac_nbw_uslot_info_stru before_uslot_bitmap_info,
    hmac_nbw_uslot_info_stru after_uslot_bitmap_info)
{
    uint8_t cur_uslot_bitmap = hmac_nbw_get_available_uslot_bitmap();
    /* 如果调整后的时隙被调整前的时隙完全覆盖，则可以调整，不用进行后面的检查 */
    if (after_uslot_bitmap_info.start_idx >= before_uslot_bitmap_info.start_idx &&
        after_uslot_bitmap_info.end_idx <= before_uslot_bitmap_info.end_idx) {
        return OAL_SUCC;
    }
    /* 不然的话要看去掉了before bitmap后的可用bitmap能否容纳after bitmap */
    if ((cur_uslot_bitmap & after_uslot_bitmap_info.uslot_bitmap) == 0) {
        return OAL_SUCC;
    } else {
        return OAL_FAIL;
    }
}

/*
 * 功能描述  : 给设备分配device id
 * 日    期  : 2025年01月06日
 */
uint8_t hmac_nbw_alloc_device_id(void)
{
    uint32_t i;
    hmac_nbw_per_device_id_info_stru *device_id_info = hmac_nbw_get_device_id_info();

    for (i = 0; i < MAC_NBW_MAX_DEVICE_NUM; i++) {
        if (device_id_info[i].use_flag == OAL_FALSE) {
            return i;
        }
    }

    return UINT8_INVALID_VALUE;
}

uint8_t hmac_nbw_master_find_island_idx(uint8_t *peer_addr)
{
    uint8_t idx;
    hmac_nbw_per_island_info_stru *island_info = hmac_nbw_get_raw_island_info();
    for (idx = 0; idx < MAX_ISLAND_CNT; idx++) {
        if (!oal_memcmp(peer_addr, island_info[idx].slaveap_mac, WLAN_MAC_ADDR_LEN)) {
            return idx;
        }
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_master_unused_idx(void)
{
    uint8_t idx;
    hmac_nbw_per_island_info_stru *island_info = hmac_nbw_get_raw_island_info();
    for (idx = 0; idx < MAX_ISLAND_CNT; idx++) {
        if (island_info[idx].use_flag == OAL_TRUE) {
            continue;
        }
        return idx;
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_find_unused_link_idx(hmac_nbw_per_island_info_stru *island_info)
{
    uint8_t idx;
    slot_info_stru *per_slot_info = &island_info->island_slot_info[0];
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (per_slot_info[idx].use_flag == OAL_TRUE) {
            continue;
        }
        return idx;
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_find_island_unused_slot_idx(void)
{
    uint8_t idx;
    slot_info_stru *island_slot_info = hmac_nbw_get_slaveap_island_slot_info();
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (island_slot_info[idx].use_flag == OAL_TRUE) {
            continue;
        }
        return idx;
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_find_island_link_idx(uint8_t *slave_addr, uint8_t addr_len)
{
    uint8_t idx;
    slot_info_stru *island_slot_info = hmac_nbw_get_slaveap_island_slot_info();

    if (addr_len > WLAN_MAC_ADDR_LEN) {
        return NBW_INVALID_IDX;
    }
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (!oal_memcmp(slave_addr, island_slot_info[idx].slave_device_mac, WLAN_MAC_ADDR_LEN)) {
            return idx;
        }
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_find_slave_unused_slot_idx(void)
{
    uint8_t idx;
    slave_slot_stru *slave_slot_info = hmac_nbw_get_slave_slot_info();
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (slave_slot_info[idx].use_flag == OAL_TRUE) {
            continue;
        }
        return idx;
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_find_slave_link_slot_idx(uint8_t *slaveap_addr)
{
    uint8_t idx;
    slave_slot_stru *slave_slot_info = hmac_nbw_get_slave_slot_info();
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (!oal_memcmp(slaveap_addr, slave_slot_info[idx].slave_ap_mac, WLAN_MAC_ADDR_LEN)) {
            return idx;
        }
    }
    return NBW_INVALID_IDX;
}

uint8_t hmac_nbw_master_find_link_idx(hmac_nbw_per_island_info_stru *island_info, uint8_t *slave_addr)
{
    uint8_t idx;
    slot_info_stru *per_slot_info = &island_info->island_slot_info[0];
    for (idx = 0; idx < MAX_ISLAND_SLOT_INFO_NUM; idx++) {
        if (!oal_memcmp(slave_addr, per_slot_info[idx].slave_device_mac, WLAN_MAC_ADDR_LEN)) {
            return idx;
        }
    }
    return NBW_INVALID_IDX;
}

void hmac_update_master_island_info(hmac_nbw_uslot_info_stru uslot_bitmap_info,
    uint8_t *slave_addr, uint8_t *peer_addr)
{
    uint8_t island_idx, link_idx;
    hmac_nbw_per_island_info_stru *island_info = hmac_nbw_get_raw_island_info();
    if (oal_any_null_ptr2(peer_addr, slave_addr)) {
        return;
    }
    oam_warning_log3(0, OAM_SF_CHBA,
        "{hmac_update_master_island_info::slaveap mac[%02X:XX:XX:XX:%02X:%02X]}",
        peer_addr[0], peer_addr[BYTE_OFFSET_4],
        peer_addr[BYTE_OFFSET_5]);
    oam_warning_log3(0, OAM_SF_CHBA,
        "{hmac_update_master_island_info::slave mac[%02X:XX:XX:XX:%02X:%02X]}",
        slave_addr[0], slave_addr[BYTE_OFFSET_4],
        slave_addr[BYTE_OFFSET_5]);

    /* 先查询当前的岛信息中有没有该岛信息 */
    island_idx = hmac_nbw_master_find_island_idx(peer_addr);
    /* 有的话 新增该链路(因为是req，所以不会链路信息更新) 没有岛信息的话，生成新岛信息 */
    if (island_idx != NBW_INVALID_IDX) {
        link_idx = hmac_nbw_find_unused_link_idx(&island_info[island_idx]);
        oam_warning_log1(0, OAM_SF_CHBA, "{hmac_update_master_island_info::link idx:%d", link_idx);

        if (link_idx == NBW_INVALID_IDX) {
            return;
        }
        if (memcpy_s(island_info[island_idx].island_slot_info[link_idx].slave_device_mac, WLAN_MAC_ADDR_LEN,
            slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return;
        }
        island_info[island_idx].island_slot_info[link_idx].per_slot_info.uslot_info = uslot_bitmap_info;
        island_info[island_idx].island_slot_info[link_idx].per_slot_info.shared_flag = 0;
        island_info[island_idx].island_slot_info[link_idx].use_flag = OAL_TRUE;
        island_info[island_idx].slot_period_indicate_cnt++;
    } else {
        island_idx = hmac_nbw_master_unused_idx();
        oam_warning_log1(0, OAM_SF_CHBA,
            "{hmac_update_master_island_info::master unused link idx:%d", island_idx);
        if (island_idx == NBW_INVALID_IDX) {
            return;
        }
        if (memcpy_s(island_info[island_idx].slaveap_mac, WLAN_MAC_ADDR_LEN, peer_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return;
        }
        if (memcpy_s(island_info[island_idx].island_slot_info[0].slave_device_mac, WLAN_MAC_ADDR_LEN,
            slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return;
        }
        island_info[island_idx].island_slot_info[0].per_slot_info.uslot_info = uslot_bitmap_info;
        island_info[island_idx].island_slot_info[0].per_slot_info.shared_flag = 0;
        island_info[island_idx].island_slot_info[0].use_flag = OAL_TRUE;
        island_info[island_idx].use_flag = OAL_TRUE;
        island_info[island_idx].slot_period_indicate_cnt = 1;
    }
}


static void hmac_nbw_master_release_timeslot(uint8_t *slaveap_addr, uint8_t *slave_addr)
{
    /* 说明，如果在此处回收时隙失败，在后面点名环节也能回收时隙，正常不会失败，除非查找不到 */
    uint8_t island_idx, link_idx;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    hmac_nbw_per_island_info_stru *island_info = hmac_nbw_get_raw_island_info();

    island_idx = hmac_nbw_master_find_island_idx(slaveap_addr);
    oam_warning_log3(0, OAM_SF_CHBA,
        "{hmac_nbw_master_release_timeslot::slaveap mac[%02X:XX:XX:XX:%02X:%02X]}",
        slaveap_addr[0], slaveap_addr[BYTE_OFFSET_4],
        slaveap_addr[BYTE_OFFSET_5]);
    oam_warning_log3(0, OAM_SF_CHBA,
        "{hmac_nbw_master_release_timeslot::slave mac[%02X:XX:XX:XX:%02X:%02X]}",
        slave_addr[0], slave_addr[BYTE_OFFSET_4],
        slave_addr[BYTE_OFFSET_5]);

    if (island_idx == NBW_INVALID_IDX) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_master_release_timeslot: not find island.");
        return;
    }
    link_idx = hmac_nbw_master_find_link_idx(&island_info[island_idx], slave_addr);
    if (link_idx == NBW_INVALID_IDX) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_master_release_timeslot: not find link.");
        return;
    }
    uslot_bitmap_info = island_info[island_idx].island_slot_info[link_idx].per_slot_info.uslot_info;
    oam_warning_log1(0, OAM_SF_CHBA, "hmac_nbw_master_release_timeslot: %d", uslot_bitmap_info.uslot_bitmap);

    /* 检查这次回收时隙 是否是最后一个该岛的时隙，如果是，那么是将整个岛的资源回收，否则只回收链路的 */
    if (island_info[island_idx].slot_period_indicate_cnt == 1) {
        memset_s(&island_info[island_idx], sizeof(hmac_nbw_per_island_info_stru), 0,
            sizeof(hmac_nbw_per_island_info_stru));
    } else {
        memset_s(&island_info[island_idx].island_slot_info[link_idx], sizeof(slot_info_stru), 0,
            sizeof(slot_info_stru));
        island_info[island_idx].slot_period_indicate_cnt--;
    }

    hmac_nbw_slaveap_link_cnt_dec();

    /* 需要将释放的timeslot置0, 后续要考虑shared_flag，如果该时隙是shared_flag，则需要所有共享此时隙的均释放才回收 */
    // TDO: 共享时隙的释放处理
    hmac_nbw_resume_uslot_bitmap(uslot_bitmap_info.uslot_bitmap);
    oam_warning_log1(0, OAM_SF_CHBA, "hmac_nbw_master_release_timeslot: %d", hmac_nbw_get_available_uslot_bitmap());
    // TDO: master更新beacon帧中的时隙状态
}


static void hmac_nbw_slave_release_timeslot(uint8_t *slaveap_addr)
{
    uint8_t slot_idx;
    slave_slot_stru *slave_slot_info = NULL;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    slot_idx = hmac_nbw_find_slave_link_slot_idx(slaveap_addr);
    if (slot_idx == NBW_INVALID_IDX) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_slave_release_timeslot: not find link slot.");
        return;
    }

    slave_slot_info = hmac_nbw_get_slave_slot_info();
    /* 更新vap bitmap */
    hmac_nbw_update_uslot_bitmap_for_link_disconnect(slave_slot_info[slot_idx].slot_info.uslot_info.uslot_bitmap);

    /* 清除uslot */
    memset_s(&slave_slot_info[slot_idx], sizeof(slave_slot_stru), 0, sizeof(slave_slot_stru));

    hmac_nbw_slave_link_cnt_dec();
    /* 发送时隙调整帧给master */
    uslot_bitmap_info.start_idx = INVALID_TIMESLOT_USLOT_IDX; // 255为无效值
    uslot_bitmap_info.end_idx = INVALID_TIMESLOT_USLOT_IDX; // 255为无效值
    hmac_nbw_send_timeslot_frame(hmac_nbw_get_master_macaddr(), slaveap_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
        MAC_NBW_TIMESLOT_ADJUST_ACTION, MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_ACTION);
}


static void hmac_nbw_slaveap_release_timeslot(uint8_t *slave_addr)
{
    uint8_t slot_idx;
    slot_info_stru *island_slot_info = NULL;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };

    slot_idx = hmac_nbw_find_island_link_idx(slave_addr, WLAN_MAC_ADDR_LEN);
    if (slot_idx == NBW_INVALID_IDX) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_slaveap_release_timeslot: not find slot.");
        return;
    }

    island_slot_info = hmac_nbw_get_slaveap_island_slot_info();
    /* 更新vap bitmap */
    hmac_nbw_update_uslot_bitmap_for_link_disconnect(island_slot_info[slot_idx].per_slot_info.uslot_info.uslot_bitmap);

    /* 清除uslot */
    memset_s(&island_slot_info[slot_idx], sizeof(slave_slot_stru), 0, sizeof(slave_slot_stru));

    hmac_nbw_slave_link_cnt_dec();

    /* 发送时隙调整帧给master */
    uslot_bitmap_info.start_idx = INVALID_TIMESLOT_USLOT_IDX; // 255为无效值
    uslot_bitmap_info.end_idx = INVALID_TIMESLOT_USLOT_IDX; // 255为无效值
    hmac_nbw_send_timeslot_frame(hmac_nbw_get_master_macaddr(), slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
        MAC_NBW_TIMESLOT_ADJUST_ACTION, MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_ACTION);
}


static void hmac_nbw_mix_role_release_timeslot(void)
{
    return;
}


void hmac_nbw_release_timeslot(uint8_t *peer_addr, uint8_t peer_len, uint8_t *other_addr, uint8_t other_len)
{
    uint8_t device_role;
    if (oal_any_null_ptr2(peer_addr, other_addr) || peer_len > WLAN_MAC_ADDR_LEN || other_len > WLAN_MAC_ADDR_LEN) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_release_timeslot: addr is null.");
        return;
    }

    device_role = hmac_nbw_get_role();
    switch (device_role) {
        case MAC_NBW_SLAVEAP_SLAVE_MIX_ROLE:
            hmac_nbw_mix_role_release_timeslot();
            break;
        case MAC_NBW_MASTER_ROLE:
            /* peer_addr为slave地址，other addr为slaveap地址或者master地址（与master自有连接断开） */
            hmac_nbw_master_release_timeslot(other_addr, peer_addr);
            break;
        case MAC_NBW_SLAVEAP_ROLE:
            /* peer_addr为slave地址，other_addr不使用 */
            hmac_nbw_slaveap_release_timeslot(peer_addr);
            break;
        case MAC_NBW_SLAVE_ROLE:
            /* peer_addr为slaveap地址，other_addr不使用 */
            hmac_nbw_slave_release_timeslot(peer_addr);
            break;
        default:
            oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_release_timeslot: device_role is wrong.");
            break;
    }
}

uint32_t hmac_nbw_master_adjust_timeslot(uint8_t *peer_addr, uint8_t *slave_addr, uint8_t slave_len,
    hmac_nbw_uslot_info_stru uslot_bitmap_info)
{
    uint8_t island_idx, link_idx;
    hmac_nbw_uslot_info_stru before_uslot_bitmap_info = { 0 };
    hmac_nbw_per_island_info_stru *island_info = hmac_nbw_get_raw_island_info();
    if (oal_any_null_ptr2(peer_addr, slave_addr) || slave_len > WLAN_MAC_ADDR_LEN) {
        return OAL_FAIL;
    }
    island_idx = hmac_nbw_master_find_island_idx(peer_addr);
    if (island_idx == NBW_INVALID_IDX) {
        return OAL_FAIL;
    }
    link_idx = hmac_nbw_master_find_link_idx(&island_info[island_idx], slave_addr);
    if (link_idx == NBW_INVALID_IDX) {
        return OAL_FAIL;
    }
    before_uslot_bitmap_info = island_info[island_idx].island_slot_info[link_idx].per_slot_info.uslot_info;
    if (hmac_nbw_check_timeslot_adjust(before_uslot_bitmap_info, uslot_bitmap_info) == OAL_FAIL) {
        return OAL_FAIL;
    }
    /* 如果可以调整的话, 将新的时隙资源覆盖，然后先resume 再重设 */
    island_info[island_idx].island_slot_info[link_idx].per_slot_info.uslot_info = uslot_bitmap_info;
    hmac_nbw_resume_uslot_bitmap(before_uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_set_available_uslot_bitmap(uslot_bitmap_info.uslot_bitmap);
    return OAL_SUCC;
}
#endif

