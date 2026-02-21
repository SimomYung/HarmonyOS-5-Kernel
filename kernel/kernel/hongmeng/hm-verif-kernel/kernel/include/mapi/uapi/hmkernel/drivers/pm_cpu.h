/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: pm_cpu for uapi
 * Create: Sat Jul 31 11:50:05 2021
 */
#ifndef UAPI_HMKERNEL_DRIVERS_PM_CPU
#define UAPI_HMKERNEL_DRIVERS_PM_CPU

#define CPU_POWERUP_DEFAULT_MODE		(__U(1))
#define CPU_POWERUP_USER_BOOTADDR_MODE		(__U(1) << 1)

enum pm_cpu_mode {
	PM_CPU_SUSPEND_DOWN,
	PM_CPU_HOTPLUG_DOWN,
	PM_CPU_UP,
};

#endif
