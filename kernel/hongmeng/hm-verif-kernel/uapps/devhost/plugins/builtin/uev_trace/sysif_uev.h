/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Declare interfaces of sysif uev list in tppmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 22 10:13:05 2022
 */
#ifndef DEVHOST_SYSIF_UEV_H
#define DEVHOST_SYSIF_UEV_H
#ifdef CONFIG_SYSIF_TRACE
#include <libsysif/devhost/api.h>
/* "__HM_UEV__" should be defined before including uev_template.h */
#define __HM_UEV__
#include <libhmuev/uev_template.h>
#define UEV_POINT       DECLARE_UEV
#include <libhmuev/sysif_uev/devhost/uev_list.h>
#undef UEV_POINT
#undef __HM_UEV__
#endif /* DEVHOST_SYSIF_UEV_H */
#endif
