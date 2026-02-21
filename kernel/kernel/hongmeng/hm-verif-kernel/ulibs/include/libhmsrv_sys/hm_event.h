/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of hm event
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 16 19:19:55 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_EVENT_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_EVENT_H

#include <libhmucap/ucap.h>
#include <hmsysmgr/eventmgr/event_def.h>

int hm_event_register_event(uref_t uref, enum evtmgr_etypes type, unsigned int callno);
int hm_event_unregister_event(uref_t uref, enum evtmgr_etypes type);
int hm_event_rule_ctrl(uref_t uref, enum evtmgr_erule_cmd cmd, const void *data, size_t size);
#endif /* ULIBS_INCLUDE_LIBHMSRV_SYS_HM_EVENT_H */
