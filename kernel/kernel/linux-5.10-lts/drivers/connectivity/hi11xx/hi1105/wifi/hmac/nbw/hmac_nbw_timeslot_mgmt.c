/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw timeslot mgmt and schedule
 * Author: wifi
 * Create: 2024-12-26
 */
#include "oam_ext_if.h"
#include "mac_nbw_common.h"
#include "hmac_nbw_mgmt.h"
#include "hmac_nbw_timeslot_mgmt.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_TIMESLOT_MGMT_C
#ifdef _PRE_WLAN_FEATURE_NBW
hmac_nbw_timeslot_mgmt_stru g_nbw_timeslot_mgmt_info;

void hmac_nbw_set_uslot_bitmap(uint8_t val)
{
    g_nbw_timeslot_mgmt_info.used_uslot_bitmap = val;
    hmac_nbw_set_slot_bitmap(mac_nbw_uslot_bitmap_to_bitmap(g_nbw_timeslot_mgmt_info.used_uslot_bitmap));
}


void hmac_nbw_update_uslot_bitmap_for_link_disconnect(uint8_t val)
{
    uint8_t cur_timeslot_alloc = hmac_nbw_get_timeslot_alloced();
    if (cur_timeslot_alloc == OAL_FALSE) {
        return;
    }

    g_nbw_timeslot_mgmt_info.used_uslot_bitmap &= ~val;
    hmac_nbw_set_slot_bitmap(mac_nbw_uslot_bitmap_to_bitmap(g_nbw_timeslot_mgmt_info.used_uslot_bitmap));
}

/*
 * 功能描述  : 申请设置vap的bitmap，并且完成uint8->uint32的转化，方便配置固件
 * 日    期  : 2025年01月21日
 */
void hmac_nbw_update_uslot_bitmap_for_slot_alloced(uint8_t val)
{
    uint8_t cur_timeslot_alloc;
    cur_timeslot_alloc = hmac_nbw_get_timeslot_alloced();
    /* 如果时隙没有分配过，则直接将本次bitmap进行配置 */
    /* 如果已经分配过，则与本次bitmap取或 */
    if (cur_timeslot_alloc == OAL_FALSE) {
        g_nbw_timeslot_mgmt_info.used_uslot_bitmap = val;
        hmac_nbw_set_timeslot_alloced(OAL_TRUE);
    } else {
        g_nbw_timeslot_mgmt_info.used_uslot_bitmap |= val;
    }
    hmac_nbw_set_slot_bitmap(mac_nbw_uslot_bitmap_to_bitmap(g_nbw_timeslot_mgmt_info.used_uslot_bitmap));
}

uint8_t hmac_nbw_timeslot_interval_transfrom(uint8_t timeslot_interval)
{
    uint8_t idx = 0, ans = 0;
    if (timeslot_interval >= MAC_NBW_USLOT_NUM)
        return INVALID_USLOT_BITMAP;
    for (; idx < timeslot_interval; idx++) {
        ans |= (1 << idx);
    }
    return ans;
}

/*
 * 功能描述  : start_end_idx转化成uslot_bitmap, 8uslot一个repeat的转换公式
 * 日    期  : 2025年01月06日
 */
uint8_t hmac_nbw_convert_idx_to_uslotbitmap_level1(uint8_t start_idx, uint8_t end_idx)
{
    uint8_t uslotbitmap = 0;
    int idx;
    hmac_nbw_vap_stru *nbw_info = hmac_nbw_get_nbw_vap();
    if (end_idx < start_idx) {
        return INVALID_USLOT_BITMAP;
    }
    for (idx = start_idx; idx <= end_idx; idx++) {
        uslotbitmap |= (1 << idx);
    }
    if (nbw_info->timeslot_level == FOUR_LEVEL)
        uslotbitmap |= (uslotbitmap << BIT_OFFSET_4);
    return uslotbitmap;
}

/*
 * 功能描述  : master根据实际情况计算出满足申请条件的bitmap, 第一版简化处理，先不考虑时隙借用和二分分配，只要有连续空闲时隙满足要求，就分配出去，后续可以优化算法
 * 日    期  : 2025年01月06日
 */
hmac_nbw_uslot_info_stru hmac_nbw_calculate_uslotbitmap(uint8_t timeslot_interval)
{
    uint8_t idx;
    hmac_nbw_uslot_info_stru uslot_info = { 0 };
    uint8_t cur_uslot_bitmap = hmac_nbw_get_available_uslot_bitmap();
    uint8_t timeslotneed_bitmap = hmac_nbw_timeslot_interval_transfrom(timeslot_interval);
    oam_warning_log1(0, OAM_SF_NBW, "hmac_nbw_calculate_uslotbitmap:: cur_uslot_bitmap:%d",
        cur_uslot_bitmap);
    if (cur_uslot_bitmap == FULL_USLOT_BITMAP || timeslotneed_bitmap == INVALID_USLOT_BITMAP) {
        oam_error_log0(0, OAM_SF_NBW,
                "hmac_nbw_calculate_uslotbitmap:: bitmap unvalid");
        return uslot_info;
    }
    for (idx = 0; idx < MAC_NBW_USLOT_NUM - timeslot_interval + 1; idx++) {
        if ((cur_uslot_bitmap & (timeslotneed_bitmap << idx)) == 0) {
            uslot_info.start_idx = idx;
            uslot_info.end_idx = idx + timeslot_interval - 1;
            uslot_info.uslot_bitmap =
                hmac_nbw_convert_idx_to_uslotbitmap_level1(uslot_info.start_idx, uslot_info.end_idx);
            break;
        }
    }
    oam_error_log3(0, OAM_SF_NBW, "hmac_nbw_calculate_uslotbitmap:: start:%d, end:%d,bitmap:%d",
        uslot_info.start_idx, uslot_info.end_idx, uslot_info.uslot_bitmap);
    return uslot_info;
}
#endif