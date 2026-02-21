/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of thermal
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 04 20:51:44 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMPM_THERMAL_H
#define ULIBS_INCLUDE_LIBHMPM_THERMAL_H

#include <libbunch/bunch.h>

#define THERMAL_STAT_ALL	"$thermal_stat_all$"

int hm_thermal_stat(const char *type, struct bunch *bunch);

#endif /* ULIBS_INCLUDE_LIBHMPM_THERMAL_H */
