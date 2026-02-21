/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fusion irq headers
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 25 18:25:00 2023
 */

#ifndef __UDK_FUSION_IRQ_H__
#define __UDK_FUSION_IRQ_H__

#include <devhost/interrupt.h>
#include <libdevhost/irq.h>

/* find first one bit from LSB(least-significant bit) */
static inline unsigned int udk_cpu_mask_to_valid_cpuid(unsigned int cpu_mask)
{
	return ((cpu_mask == 0U) ? 0U : (__builtin_ffs(cpu_mask) - 1U));
}

int udk_fusion_irq_server_init(void);
int udk_fusion_irq_ap_table_init(void);
unsigned int udk_fusion_irq_map(unsigned int oirq_offset,
				const unsigned int *args,
				unsigned int args_count);
int udk_fusion_irq_setup(unsigned int logic_irq, devhost_irq_handler_t handler,
			 const char *devname, void *dev_id);
int udk_fusion_irq_close(unsigned int logic_irq);
int udk_fusion_irq_enable(unsigned int logic_irq);
int udk_fusion_irq_setaffinity(unsigned int logic_irq, unsigned long affinity);
void udk_fusion_migrate_all_irq_of_cpu(unsigned int cpu);

#endif
