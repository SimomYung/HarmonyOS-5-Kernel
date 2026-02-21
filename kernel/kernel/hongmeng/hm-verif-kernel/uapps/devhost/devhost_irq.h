/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for interrupt support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 14 14:40:12 2019
 */
#ifndef __DEVHOST_IRQ_H__
#define __DEVHOST_IRQ_H__

#include <hmkernel/interrupt/kvic.h>

/* devhost kvic */
/* void kvic irq starts from 1, so we could let kvic irq 1 using kernelmin */
#define DEVHOST_KVIC_CALLNUM_NR		1

extern void devhost_kvic_getinput(const struct __kvic_uart_read_data *pdata);

/* exposed for devhost_api */
int __devhost_kvic_prepare_irq(unsigned int kvic_irq_type,
			       void *arg, unsigned int arg_len);

#endif /* __DEVHOST_IRQ_H__ */
