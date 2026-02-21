/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: board_gpio.c header file
 * Author: @CompanyNameTag
 */

#ifndef BOARD_PM_H
#define BOARD_PM_H
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/platform_device.h>
#include <linux/list.h>
#endif

#include "oal_types.h"

#if (_PRE_OS_VERSION_LINUX != _PRE_OS_VERSION)
/* 非linux环境, 复制内核定义 */
#define PM_HIBERNATION_PREPARE	0x0001 /* Going to hibernate */
#define PM_POST_HIBERNATION	0x0002 /* Hibernation finished */
#define PM_SUSPEND_PREPARE	0x0003 /* Going to suspend the system */
#define PM_POST_SUSPEND		0x0004 /* Suspend finished */
#endif

#define PM_NOTIFY_STR   "1:hiber;2:restore;3:suspend;4:resume"

#define NOTIFY_NAME_LEN   32

#define PLAT_NOITFY_PRIO  0
#define WIFI_NOITFY_PRIO  (PLAT_NOITFY_PRIO + 1)

typedef struct {
    uint32_t force_sleep;
} pm_notify_param;

typedef int (*pm_notify_cb)(unsigned long event, pm_notify_param* param, void *data);
typedef struct {
    struct list_head node;
    pm_notify_cb cb_func;
    void* data;
    int priority;
    char name[NOTIFY_NAME_LEN];
} pm_notify_node;


#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
void board_pm_register_notify_cb(pm_notify_node* my_cb);
void board_pm_unregister_notify_cb(pm_notify_node* my_cb);
int board_pm_notifier_register(void);
int board_pm_notifier_unregister(void);
#else
OAL_STATIC OAL_INLINE void board_pm_register_notify_cb(pm_notify_node* my_cb)
{
    return;
}

OAL_STATIC OAL_INLINE void board_pm_unregister_notify_cb(pm_notify_node* my_cb)
{
    return;
}

OAL_STATIC OAL_INLINE int board_pm_notifier_register(void)
{
    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE int board_pm_notifier_unregister(void)
{
    return OAL_SUCC;
}
#endif

#endif
