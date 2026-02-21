/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Interfaces of locktrace uev list in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 20 10:57:42 2022
 */

#ifndef LIBHMSYNC_UEV_UEV_H
#define LIBHMSYNC_UEV_UEV_H

#include <libhmsync/uev/uev_types.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>

uint64_t uev_trace_lock_event_get_time(void);
uint64_t uev_trace_lock_event_set_lockid(atomic_t *atomic_id);

#ifdef CONFIG_LOCK_TRACE_UEV_LOGGING
/* "__HM_UEV__" should be defined before including uev_template.h */
#define __HM_UEV__
#endif
#include <libhmuev/uev_template.h>

#define UEV_POINT       DECLARE_UEV
#include <libhmsync/uev/uev_list.h>
#undef  UEV_POINT

#ifdef CONFIG_LOCK_TRACE_UEV_LOGGING
#undef __HM_UEV__
#endif

#endif
