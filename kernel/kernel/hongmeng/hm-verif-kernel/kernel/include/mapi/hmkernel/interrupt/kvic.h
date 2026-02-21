/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Define generate of kvic irq for modules
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 19 16:24:54 2020
 */
#ifndef MAPI_HMKERNEL_KVIC_H
#define MAPI_HMKERNEL_KVIC_H

#include <uapi/hmkernel/interrupt/kvic.h>
#include <hmkernel/trigger.h>

struct trigger_source_s *kvic_trigger_source_of(unsigned int source_id);
#endif
