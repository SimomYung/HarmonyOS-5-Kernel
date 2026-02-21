/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of uev list in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 29 20:36:33 2019
 */

#ifndef SYSYMGR_INSPECTOR_UEV_H
#define SYSYMGR_INSPECTOR_UEV_H

#include <intapi/thread/thread.h>
#include <intapi/process/process.h>
#include <libhmsync/uev/uev_types.h>

#ifdef CONFIG_HMUEV
/* "__HM_UEV__" should be defined before including uev_template.h */
#define __HM_UEV__
#endif
#include <libhmuev/uev_template.h>

#define UEV_POINT	DECLARE_UEV
#include <intapi/inspector/uev/uev_list.h>
#undef	UEV_POINT

#ifdef CONFIG_HMUEV
#undef __HM_UEV__
#endif

#endif
