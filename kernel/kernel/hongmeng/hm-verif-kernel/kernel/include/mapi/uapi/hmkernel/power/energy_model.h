/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Uapi for energy model
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 29 11:54:32 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_ENERGY_MODEL_H
#define MAPI_UAPI_HMKERNEL_ENERGY_MODEL_H

#define __EM_FREQUENCY_SCALE_FACTOR 1024

#define __EM_SYSTEM_MAX_CAPACITY 1024

#define __EM_INVALID_FREQ 0xffffffffU

#ifdef CONFIG_CPUTOPO_NR_LEVELS
#define PM_CONFIG_CPUTOPO_NR_LEVELS CONFIG_CPUTOPO_NR_LEVELS
#else
#define PM_CONFIG_CPUTOPO_NR_LEVELS 3
#endif

#ifdef ARCH_CPUTOPO_CLUSTER_INDEX
#define PM_CPUTOPO_CLUSTER_INDEX ARCH_CPUTOPO_CLUSTER_INDEX
#else
#define PM_CPUTOPO_CLUSTER_INDEX 2
#endif

#endif
