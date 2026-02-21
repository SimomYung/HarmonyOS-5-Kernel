/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Internal header of udk irq
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 11 11:36:22 2021
 */

#ifndef __INTERNAL_UDK_IRQ_H__
#define __INTERNAL_UDK_IRQ_H__

#include <udk/of.h>

#define MAX_INTR_CELLS		8U

int udk_irq_of_uirq(struct udk_dt_node *node, unsigned int index);

int udk_irq_kirq_create(const void *owner, unsigned int irqctrl_id,
			const unsigned int *irq_args, unsigned int irq_cells);

#ifdef CONFIG_UDK_IRQ_EXT

#include <udk/irq.h>

int udk_irq_ext_uirq_request(unsigned int uirq, irq_handler_t handler,
			     unsigned int flags, const char *devname,
			     void *ctx);
void udk_irq_ext_uirq_unrequest(unsigned int uirq);

void udk_irq_ext_uirq_enable(unsigned int uirq);
void udk_irq_ext_uirq_disable(unsigned int uirq);

#endif /* CONFIG_UDK_IRQ_EXT */

#endif /* __INTERNAL_UDK_IRQ_H__ */
