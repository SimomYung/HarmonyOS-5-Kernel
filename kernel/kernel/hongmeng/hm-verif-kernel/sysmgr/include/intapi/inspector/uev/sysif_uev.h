/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Declare interfaces of sysif uev list in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 22 10:13:05 2022
 */
#ifndef SYSYMGR_INSPECTOR_SYSIF_UEV_H
#define SYSYMGR_INSPECTOR_SYSIF_UEV_H
#ifdef CONFIG_SYSIF_TRACE
#include <libsysif/sysmgr/api.h>
/* "__HM_UEV__" should be defined before including uev_template.h */
#define __HM_UEV__
#include <libhmuev/uev_template.h>
#define UEV_POINT       DECLARE_UEV
#include <libhmuev/sysif_uev/sysmgr/uev_list.h>
#undef UEV_POINT
#undef __HM_UEV__
#endif /* CONFIG_SYSIF_TRACE */
#endif
