/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  UEV tracepoint definations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 27 14:16:32 2021
 */

#ifndef __UX_UEV_H__
#define __UX_UEV_H__

#ifdef CONFIG_UEV_TRACE
#define __HM_UEV__
#endif
#include <libhmuev/uev_template.h>

#define UEV_POINT       DECLARE_UEV
#include "ux_uev_list.h"
#undef  UEV_POINT

int uev_trace_init(void);

#ifdef CONFIG_UEV_TRACE
#undef __HM_UEV__
#endif

#endif
