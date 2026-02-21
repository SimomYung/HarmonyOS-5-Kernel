/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Uapi for power module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 16:40:45 UTC 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_POWER_MODULE_H
#define MAPI_UAPI_HMKERNEL_POWER_MODULE_H

#define __PM_CLASS_NAME "pm_class"

#define __PM_CLUSTERS_MAX_CAPACITY	0x100U
#define __PM_CLUSTER_THERMAL_CAPACITY	0x200U
#define __PM_CLUSTER_SUSPEND_FREQ_READ	0x201U
#define __PM_TIME_IN_STATE_READ		0x300U
#define __PM_POWER_TRACK_CPU_TOTAL_INST		0x301U
#define __PM_SUSPEND_CNT_READ		0x310U
#define __PM_SLOW_CPUS_READ	0x400U
#define __PM_FAST_CPU_READ	0x500U
#define __PM_CPU_LOAD_STAT_READ	0x600U
#define __PM_CPU_WINDOW_UPDATE	0x700U

#endif /* MAPI_UAPI_HMKERNEL_POWER_MODULE_H */
