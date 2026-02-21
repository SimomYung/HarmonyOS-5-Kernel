/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: Header file of the external public interface of the oal
 * Author: @CompanyNameTag
 */

#ifndef OAL_EXT_IF_H
#define OAL_EXT_IF_H

/* 其他头文件包含 */
#include "oal_types.h"
#include "oal_util.h"
#include "oal_hardware.h"
#include "oal_schedule.h"
#include "oal_bus_if.h"
#include "oal_net.h"
#include "oal_list.h"
#include "oal_queue.h"
#include "oal_workqueue.h"
#include "arch/oal_ext_if.h"
#include "oal_thread.h"
#include "oal_fsm.h"

/* 宏定义 */
#define basic_value_is_hex(_auc_str, _len)  \
    (((_auc_str)[0] == '0') && (((_auc_str)[1] == 'x') || ((_auc_str)[1] == 'X')))
/* 枚举定义 */
typedef enum {
    OAL_TRACE_ENTER_FUNC,
    OAL_TRACE_EXIT_FUNC,

    OAL_TRACE_DIRECT_BUTT
} oal_trace_direction_enum;
typedef uint8_t oal_trace_direction_enum_uint8;

/* 黑名单模式 */
typedef enum {
    CS_BLACKLIST_MODE_NONE,  /* 关闭         */
    CS_BLACKLIST_MODE_BLACK, /* 黑名单       */
    CS_BLACKLIST_MODE_WHITE, /* 白名单       */

    CS_BLACKLIST_MODE_BUTT
} cs_blacklist_mode_enum;
typedef uint8_t cs_blacklist_mode_enum_uint8;

#ifdef _PRE_WLAN_REPORT_WIFI_ABNORMAL
// 上报驱动异常状态，并期望上层解决
typedef enum {
    OAL_ABNORMAL_FRW_TIMER_BROKEN = 0,  // frw定时器断链
    OAL_ABNORMAL_OTHER = 1,             // 其他异常

    OAL_ABNORMAL_BUTT
} oal_wifi_abnormal_reason_enum;

typedef enum {
    OAL_ACTION_RESTART_VAP = 0,  // 重启vap，上层暂未实现
    OAL_ACTION_REBOOT = 1,       // 重启板子

    OAL_ACTION_BUTT
} oal_product_action_enum;

#define oal_report_wifi_abnormal(_l_reason, _l_action, _p_arg, _l_size)
#endif

typedef enum {
    OAL_WIFI_STA_LEAVE = 0,  // STA 离开
    OAL_WIFI_STA_JOIN = 1,   // STA 加入

    OAL_WIFI_BUTT
} oal_wifi_sta_action_report_enum;
#define oal_wifi_report_sta_action(_ul_ifindex, _ul_eventID, _p_arg, _l_size)

#define OAL_WIFI_COMPILE_OPT_CNT 10

extern int32_t oal_main_init(void);
extern void oal_main_exit(void);

#endif /* end of oal_ext_if.h */
