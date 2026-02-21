/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: This is the header of udk irq related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 05 09:06:40 2019
 */

#ifndef __DEVHOST_UDK_IRQ_H__
#define __DEVHOST_UDK_IRQ_H__

#define UDK_IRQ_TRIGGER_NONE		0U
#define UDK_IRQ_TRIGGER_RISING		1U
#define UDK_IRQ_TRIGGER_FALLING		2U
#define UDK_IRQ_TRIGGER_EDG_BOTH	(UDK_IRQ_TRIGGER_RISING | UDK_IRQ_TRIGGER_FALLING)
#define UDK_IRQ_TRIGGER_HIGH		4U
#define UDK_IRQ_TRIGGER_LOW		8U
#define UDK_IRQ_FLAGS_TRIGGER_MASK	0xfU
#define UDK_IRQ_TRIGGER_DEFAULT		UDK_IRQ_FLAGS_TRIGGER_MASK

typedef int (*irq_handler_t)(int, void *);

extern unsigned int udk_irq_create_mapping(unsigned int oirq_offset,
					   const unsigned int *args,
					   unsigned int args_count);
extern int udk_request_irq(unsigned int irq, irq_handler_t handler,
			   unsigned long irqflags, const char *devname,
			   void *ctx);
extern void udk_free_irq(unsigned int irq);
extern void udk_enable_irq(unsigned int irq);
extern void udk_disable_irq(unsigned int irq);
extern int udk_irq_setaffinity(unsigned int irq, unsigned long affinity);

void udk_local_irq_disable(void);
void udk_local_irq_enable(void);
void udk_local_irq_restore(unsigned long flags);
unsigned long udk_local_irq_save(void);
unsigned long udk_local_save_flags(void);


#endif /* __DEVHOST_UDK_IRQ_H__ */
