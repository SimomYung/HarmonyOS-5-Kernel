/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: add Namtso PMU list for hm perf
 * Author: HISI_DRV
 * Create: Sat May 18 10:30:43 2024
 */

#if defined(CONFIG_KIRIN_CHIP_CHARLOTTE) || defined(CONFIG_KIRIN_CHIP_CHARLOTTE_PRO)
#include "platform_namtso/perf_platform_namtso_event_charlotte.h"
#endif

#if defined(CONFIG_KIRIN_CHIP_NASHVILLE)
#include "platform_namtso/perf_platform_namtso_event_nashville.h"
#endif