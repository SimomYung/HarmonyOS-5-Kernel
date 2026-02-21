/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of MPAM Event
 * Author: Huawei Hisilicon
 * Create: Mon Aug 19 9:23:00 2024
 */
#ifndef __MPAM_EVENT_H
#define __MPAM_EVENT_H

#include <stdint.h>
#include <hmkernel/errno.h>

#ifdef CONFIG_MPAM_BIGMODEL_TRIGGER
int mpam_event_ctrl(void __user *buf, unsigned long buflen);
void mpam_event_notify(bool enable);
#else
static inline int mpam_event_ctrl(void __user *buf, unsigned long buflen)
{
    return E_HM_INVAL;
}
static inline void mpam_event_notify(bool enable)
{
}
#endif

#endif
