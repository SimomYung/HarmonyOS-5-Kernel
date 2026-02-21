/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Value definitions for interrupt processing exported for module.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 08 09:54:19 2020
 */
#ifndef MAPI_HMKERNEL_INTERRUPT_H
#define MAPI_HMKERNEL_INTERRUPT_H

#include <hmkernel/types.h>

#define IRQ_HDLR_RET_EOI	0x1UL
#define IRQ_HDLR_RET_DEACTIVE	0x4UL
#define IRQ_HDLR_RET_DISABLE	0x8UL
#define IRQ_HDLR_RET_PENDING 	0x10UL
#define IRQ_HDLR_RET_IS_IPI	0x80UL

/* PNode-irq feature apis */
extern void irqctrl_pnode_irq_exit(unsigned int cpu);
extern void irqctrl_pnode_irq_enter(unsigned int cpu);
extern bool irqctrl_is_pnode_irq_cpu(unsigned int cpu);
#endif
