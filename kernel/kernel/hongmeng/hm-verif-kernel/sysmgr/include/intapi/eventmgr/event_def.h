/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Event definitions exported to sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 12 23:50:35 2023
 */
#ifndef SYSMGR_INCLUDE_INTAPI_EVENTMGR_EVENT_DEF_H
#define SYSMGR_INCLUDE_INTAPI_EVENTMGR_EVENT_DEF_H

#include <stddef.h>
#include <sysmgr/module_ctx.h>
#include <aapi/hmsysmgr/eventmgr/event_def.h>

int eventmgr_init(struct sysmgr_module_ctx *ctx);
int eventmgr_broadcast_event(enum evtmgr_etypes type, void *arg1, void *arg2);
#endif /* SYSMGR_INCLUDE_INTAPI_EVENTMGR_EVENT_DEF_H */
