/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override irq interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 25 15:50:54 2023
 */

#include <hongmeng/errno.h>
#include <libstrict/strict.h>

#include "irq.h"

unsigned int devhost_create_irq_mapping(unsigned int oirq_offset,
					const unsigned int *args,
					unsigned int args_count)
{
	return udk_fusion_irq_map(oirq_offset, args, args_count);
}

int devhost_request_threaded_irq(unsigned int logic_irq,
				 devhost_irq_handler_t handler,
				 int is_kvic,
				 const char *devname,
				 void *dev_id)
{
	UNUSED(is_kvic);

	return udk_fusion_irq_setup(logic_irq, handler, devname, dev_id);
}

void devhost_free_irq(unsigned int logic_irq)
{
	(void)udk_fusion_irq_close(logic_irq);
}

void devhost_enable_irq(unsigned int logic_irq)
{
	(void)udk_fusion_irq_enable(logic_irq);
}

int devhost_irq_setaffinity(unsigned int logic_irq, unsigned long affinity)
{
	return udk_fusion_irq_setaffinity(logic_irq, affinity);
}

int devhost_kvic_default_ctrl(void)
{
	return 0;
}

void devhost_migrate_all_irq_of_cpu(unsigned int cpu)
{
	udk_fusion_migrate_all_irq_of_cpu(cpu);
}
