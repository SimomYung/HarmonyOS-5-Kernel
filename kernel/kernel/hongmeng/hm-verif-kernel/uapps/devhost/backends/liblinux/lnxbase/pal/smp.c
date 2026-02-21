/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for smp ipi calls
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 11 10:45:12 2024
 */
#include <liblinux/pal.h>
#include <devhost/interrupt.h>

int liblinux_pal_smp_call_cpu(int irq, unsigned int cpu)
{
	return devhost_smp_call_cpu(irq, cpu);
}
