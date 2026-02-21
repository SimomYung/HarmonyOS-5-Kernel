/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw timeslot mgmt and schedule
 * Author: wifi
 * Create: 2024-12-26
 */
#ifndef HMAC_NBW_TIMESLOT_MGMT_H
#define HMAC_NBW_TIMESLOT_MGMT_H

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_TIMESLOT_MGMT_H

#ifdef _PRE_WLAN_FEATURE_NBW
#define TWELVE_REPEAT 12
#define FOUR_LEVEL 4
#define EIGHT_LEVEL 8
/* ACTION帧subtype类型 */
typedef enum {
    MAC_NBW_ACCEPT = 0,
    MAC_NBW_REJECT = 1,

    MAC_NBW_TIMESLOT_STATUS_BUTT,
} hmac_nbw_timeslot_status_enum;

/* 描述单次申请的uslot bitmap情况, 当前默认repeat cnt为12，后续可以增加该元素 */
typedef struct {
    uint8_t start_idx;
    uint8_t end_idx;
    uint8_t uslot_bitmap;
    uint8_t timeslot_interval;
} hmac_nbw_uslot_info_stru;

typedef struct {
    uint8_t timeslot_alloced; /* 标识是否已有工作时隙，只有分配了工作时隙，才会置位 */
    uint8_t used_uslot_bitmap; /* 当前设备工作的uslot_bitmap，如果是多条链，则是取或 */
    uint32_t used_bitmap; /* 配置给固件的vap bitmap，由工作uslot_bitmap转换而来 */
} hmac_nbw_timeslot_mgmt_stru;
extern hmac_nbw_timeslot_mgmt_stru g_nbw_timeslot_mgmt_info;
static inline uint8_t hmac_nbw_get_timeslot_alloced(void)
{
    return g_nbw_timeslot_mgmt_info.timeslot_alloced;
}

static inline void hmac_nbw_set_timeslot_alloced(uint8_t val)
{
    g_nbw_timeslot_mgmt_info.timeslot_alloced = val;
}

static inline uint8_t hmac_nbw_get_used_slot_bitmap(void)
{
    return g_nbw_timeslot_mgmt_info.used_uslot_bitmap;
}

static inline uint32_t hmac_nbw_get_slot_bitmap(void)
{
    return g_nbw_timeslot_mgmt_info.used_bitmap;
}

static inline void hmac_nbw_set_slot_bitmap(uint32_t val)
{
    g_nbw_timeslot_mgmt_info.used_bitmap = val;
}

uint8_t hmac_nbw_timeslot_interval_transfrom(uint8_t timeslot_interval);
uint8_t hmac_nbw_convert_idx_to_uslotbitmap_level1(uint8_t start_idx, uint8_t end_idx);
hmac_nbw_uslot_info_stru hmac_nbw_calculate_uslotbitmap(uint8_t timeslot_interval);
void hmac_nbw_update_uslot_bitmap_for_slot_alloced(uint8_t val);
void hmac_nbw_set_uslot_bitmap(uint8_t val);
void hmac_nbw_update_uslot_bitmap_for_link_disconnect(uint8_t val);
#endif
#endif

