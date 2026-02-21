/*
* Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
* 功能说明   : Hmac与Dmac公用的NBW变量
* 作者       : wifi
* 创建日期   : 2025年1月7日
*/
#ifndef MAC_NBW_COMMON_H
#define MAC_NBW_COMMON_H
/* 1 其他头文件包含 */
#include "oal_types.h"
#include "wlan_types.h"
#include "wlan_spec.h"
#include "mac_device_common.h"
#include "mac_user_common.h"
#include "mac_chba_common.h"

#ifdef _PRE_WLAN_FEATURE_NBW
#define MAC_NBW_ACTION_OUI_TYPE_LEN 4
#define MAC_NBW_ACTION_TYPE_LEN 5
#define MAC_NBW_ACTION_HDR_LEN 6
#define MAC_NBW_ATTR_ID_LEN 1
#define MAC_NBW_ATTR_HDR_LEN 2
#define MIN_TIMESLOT_BODY_LEN 4

#define TIMESLOT_REQ_BODY_LEN 14
#define TIMESLOT_REQ_SLAVEMAC_POS 2
#define TIMESLOT_REQ_TIMEINTERVAL_POS 11

#define TIMESLOT_RSP_MIN_BODY_LEN 9
#define TIMESLOT_RSP_BODY_LEN 14
#define TIMESLOT_RSP_STATUS_POS 2
#define TIMESLOT_RSP_SLAVEMAC_POS 3
#define TIMESLOT_RSP_START_IDX_POS 12
#define TIMESLOT_RSP_END_IDX_POS 13

#define TIMESLOT_INDICATE_BODY_LEN_MIN 3
#define TIMESLOT_INDICATE_BODY_LEN 13
#define TIMESLOT_INDICATE_START_IDX_POS 11
#define TIMESLOT_INDICATE_END_IDX_POS 12
#define TIMESLOT_INDICATE_STATUS_POS 2
#define TIMESLOT_INDICATE_MASTERMAC_POS 3

#define TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_BODY_LEN 13
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_SLAVEMAC_POS 2
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_START_IDX_POS 11
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_END_IDX_POS 12
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_FLAG_POS 8

#define TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_MIN_BODY_LEN 3
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_BODY_LEN 13
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_START_IDX_POS 11
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_END_IDX_POS 12
#define TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_STATUS_POS 2

#define TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_MIN_BODY_LEN 3
#define TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_BODY_LEN 7
#define TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_STATUS_POS 2
#define TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_START_IDX_POS 5
#define TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_END_IDX_POS 6

/* beacon帧相关 */
#define MAC_NBW_BEACON_ELEMENT_OFFSET 12 /* nbw Beacon固定字段长度 */
#define INVALID_TIMESLOT_USLOT_IDX 255
#define MAC_OUI_TYPE_NBW 0x88
#define MAC_NBW_VENDOR_EID 221
#define INVALID_USLOT_BITMAP 0
#define FULL_USLOT_BITMAP (uint8_t)(0xFF)
#define NBW_USLOT0_1_BITMAP (uint8_t)(0x03)
#define NBW_USLOT2_3_BITMAP (uint8_t)(0x0C)
#define NBW_USLOT4_5_BITMAP (uint8_t)(0x30)
#define NBW_USLOT6_7_BITMAP (uint8_t)(0xC0)
#define NBW_0_1_WORK_BITMAP (uint32_t)(0x03030303)
#define NBW_2_3_WORK_BITMAP (uint32_t)(0x0C0C0C0C)
#define NBW_4_5_WORK_BITMAP (uint32_t)(0x30303030)
#define NBW_6_7_WORK_BITMAP (uint32_t)(0xC0C0C0C0)

typedef enum {
    MAC_NBW_BEACON_TYPE_BASIC = 0,
    MAC_NBW_BEACON_TYPE_MASTER_INDICATE,
    MAC_NBW_BEACON_TYPE_SLOT_ALLOC,
    MAC_NBW_BEACON_TYPE_SLOT_SWITCH,
    MAC_NBW_BEACON_TYPE_MASTER_SWITCH,
} hmac_nbw_beacon_type_enum;

/* nbw Attribute属性类型枚举 */
typedef enum {
    MAC_NBW_ATTR_BASIC = 0x00,
    MAC_NBW_ATTR_MASTER_INDICATE = 0x01,
    MAC_NBW_ATTR_SLOT_ALLOC = 0x02,
    MAC_NBW_ATTR_TIMESLOT_INFO = 0x03,
    MAC_NBW_ATTR_SLOT_SWITCH = 0x05,
    MAC_NBW_ATTR_MASTER_SWITCH = 0x06,

    MAC_NBW_ATTR_BUTT,
} nbw_attribute_type_enum;


#define MAC_NBW_USLOT_NUM (MAC_NBW_SLOT_DURATION / MAC_NBW_USLOT_DURATION)
#define MAC_NBW_USLOT_DURATION 4
#define MAC_NBW_SCHED_PERIOD 8
#define MAC_NBW_SLOT_DURATION 16
#define MAC_NBW_DEFALUT_PERIODS_MS 128 /* NBW默认调度周期时长(ms) */

typedef enum {
    NBW_BITMAP_USLOT_0_1_LEVEL = 3,
    NBW_BITMAP_USLOT_2_3_LEVEL = 4,
    NBW_BITMAP_USLOT_4_5_LEVEL = 5,
    NBW_BITMAP_USLOT_6_7_LEVEL = 6,
} mac_nbw_bitmap_level_enum;
typedef struct {
    uint16_t user_idx; /* 指定user的user_idx */
    uint32_t bitmap;
} nbw_user_timeslot_bitmap_cfg_stru; /* nbw user级别bitmap配置结构体 */

static inline uint8_t mac_nbw_bitmap_to_level(uint32_t bitmap)
{
    return (((bitmap) == NBW_0_1_WORK_BITMAP) ? NBW_BITMAP_USLOT_0_1_LEVEL : \
        ((bitmap) == NBW_2_3_WORK_BITMAP) ? NBW_BITMAP_USLOT_2_3_LEVEL : \
        ((bitmap) == NBW_4_5_WORK_BITMAP) ? NBW_BITMAP_USLOT_4_5_LEVEL : \
        ((bitmap) == NBW_6_7_WORK_BITMAP) ? NBW_BITMAP_USLOT_6_7_LEVEL : NBW_BITMAP_USLOT_0_1_LEVEL);
}

static inline uint32_t mac_nbw_level_to_bitmap(uint8_t level)
{
    return (((level) == NBW_BITMAP_USLOT_0_1_LEVEL) ? NBW_0_1_WORK_BITMAP : \
        ((level) == NBW_BITMAP_USLOT_2_3_LEVEL) ? NBW_2_3_WORK_BITMAP : \
        ((level) == NBW_BITMAP_USLOT_4_5_LEVEL) ? NBW_4_5_WORK_BITMAP : \
        ((level) == NBW_BITMAP_USLOT_6_7_LEVEL) ? NBW_6_7_WORK_BITMAP : 0);
}

static inline uint8_t mac_nbw_uslot_bitmap_to_level(uint8_t uslot_bitmap)
{
    return (((uslot_bitmap) == NBW_USLOT0_1_BITMAP) ? NBW_BITMAP_USLOT_0_1_LEVEL : \
        ((uslot_bitmap) == NBW_USLOT2_3_BITMAP) ? NBW_BITMAP_USLOT_2_3_LEVEL : \
        ((uslot_bitmap) == NBW_USLOT4_5_BITMAP) ? NBW_BITMAP_USLOT_4_5_LEVEL : \
        ((uslot_bitmap) == NBW_USLOT6_7_BITMAP) ? NBW_BITMAP_USLOT_6_7_LEVEL : NBW_BITMAP_USLOT_0_1_LEVEL);
}

/* 先实现repeat的code */
static inline uint32_t mac_nbw_uslot_bitmap_to_bitmap(uint8_t uslot_bitmap)
{
    return uslot_bitmap | (uslot_bitmap << BIT_OFFSET_8) | (uslot_bitmap << BIT_OFFSET_16) |
        (uslot_bitmap << BIT_OFFSET_24);
}
#endif
#endif

